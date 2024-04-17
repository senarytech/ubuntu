/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2020 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#include "allocator_ion.hpp"
#include "synap/logging.hpp"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <linux/dma-buf.h>  // From toolchain sysroot
#include "ion_5_4.h"        // From ${VSSDK_DIR}/ampsdk/osal/include/
#include "synap_device.h"


using namespace std;

namespace synaptics {
namespace synap {


bool AllocatorIon::suspend_cpu_access(int fd)
{
    dma_buf_sync syncData;
    syncData.flags = DMA_BUF_SYNC_END | DMA_BUF_SYNC_WRITE;

    LOGI << "Suspending cpu access on dmabuf: " << fd;
    if (ioctl(fd, DMA_BUF_IOCTL_SYNC, &syncData) != 0) {
        LOGE << "Cannot suspend access to dmabuf " << fd << " error: " << strerror(errno);
        return false;
    }
    return true;
}


bool AllocatorIon::resume_cpu_access(int fd)
{
    dma_buf_sync syncData;
    syncData.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_READ;

    LOGI << "Resuming cpu access on dmabuf: " << fd;
    if (ioctl(fd, DMA_BUF_IOCTL_SYNC, &syncData) != 0) {
        LOGE << "Cannot resume access to dmabuf " << fd << " error: " << strerror(errno);
        return false;
    }
    return true;
}


AllocatorIon::AllocatorIon(bool contiguous, bool secure) :
    _contiguous{contiguous},
    _secure{secure}
{
    LOGI << "AllocatorIon contiguous: " << contiguous << ", secure: " << secure;
    _available = init();
}


bool AllocatorIon::init()
{
    _ion_fd = open("/dev/ion", O_RDWR, 0);
    if (_ion_fd < 0) {
        LOGE << "failed to open /dev/ion";
        return false;
    }

    ion_heap_query queryCnt{};
    if (ioctl(_ion_fd, ION_IOC_HEAP_QUERY, &queryCnt) < 0) {
        LOGE << "Failed to query ion heap count";
        return false;
    }
    LOGV << "Found " << queryCnt.cnt << " ion heaps in the system";

    vector<ion_heap_data> heapData(queryCnt.cnt);
    ion_heap_query query{};
    query.cnt = queryCnt.cnt;
    query.heaps = (uintptr_t)heapData.data();
    if (ioctl(_ion_fd, ION_IOC_HEAP_QUERY, &query) < 0) {
        LOGE << "Failed to query ion heaps";
        return false;
    }

    uint32_t mask_paged{};
    uint32_t mask_cma{};
    uint32_t mask_cma_secure{};

    for (const auto& hi: heapData) {
        switch (hi.type) {
        case ION_HEAP_TYPE_SYSTEM_CUST:
            mask_paged |= 1 << hi.heap_id;
            break;
        case ION_HEAP_TYPE_DMA_CUST:
            mask_cma |= 1 << hi.heap_id;
            break;
        case ION_HEAP_TYPE_BERLIN_SECURE:
            mask_cma_secure |= 1 << hi.heap_id;
            break;
       }
    }

    _heap_mask_std = _secure? (_contiguous? mask_cma_secure : mask_paged) :
                              (_contiguous? mask_cma : mask_paged);

    LOGI << "AllocatorIon ion fd: " << _ion_fd << " heapmask: " << _heap_mask_std;
    return true;
}


AllocatorIon::~AllocatorIon()
{
    if (_ion_fd >= 0) {
        close(_ion_fd);
    }
}


Allocator::Memory AllocatorIon::do_alloc(size_t size, uint32_t heap_mask)
{
    ion_allocation_data allocData{};

    /* since the driver must map the buffer to the NPU MMU we need to allocate a buffer that has
     * a size multiple of the page size otherwise we risk to corrupt what is located just after
     * the buffer */
    allocData.len = align(size);

    allocData.heap_id_mask = heap_mask;
    allocData.flags = ION_FLAG_CACHED;

    LOGV << "Allocating ion memory, mask: 0x" << hex <<  allocData.heap_id_mask << ", size: "
         << dec << size << ", flags: 0x" << hex << allocData.flags << dec;

    if (ioctl(_ion_fd, ION_IOC_ALLOC, &allocData) != 0) {
        LOGE << "Failed to allocate ion memory, size: " << size;
        return {};
    }
    int fd = allocData.fd;

    void* ptr{};
    uint32_t bid{};
    uint32_t mem_id{};
    if (_secure) {
        if (!synap_create_secure_io_buffer(fd, 0, allocData.len, &bid, &mem_id)) {
            LOGE << "Unable create secure buffer from fd " << fd;
            close(fd);
            return {};
        }
    }
    else {
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED) {
            LOGE << "Unable to map dmabuf handle " << allocData.fd;
            close(fd);
            return {};
        }
        if (align(ptr) != ptr) {
            LOGE << "Error memory address not correctly aligned: " << ptr;
            munmap(ptr, size);
            close(fd);
            return {};
        }
        if (!resume_cpu_access(fd)) {
            LOGE << "Unable to resume cpu access to fd " << fd;
            munmap(ptr, size);
            close(fd);
            return {};
        }
        if (!synap_create_io_buffer(fd, 0, allocData.len, &bid, &mem_id)) {
            LOGE << "Unable create buffer from fd " << fd;
            munmap(ptr, size);
            close(fd);
            return {};
        }
    }

    LOGV << "allocated fd is: " << fd;
    return Memory{ptr, 1, fd, bid, mem_id, size};
}


Allocator::Memory AllocatorIon::alloc(size_t size)
{
    return do_alloc(size, heap_mask());
}


void AllocatorIon::dealloc(const Memory& mem)
{
    if (!mem.handle) {
        LOGV << "dealloc null, ignore";
        return;
    }
    if (mem.fd >= 0) {
        if (mem.address) {
            if(munmap(mem.address, mem.size) != 0) {
                LOGE << "munmap failed for fd: " << mem.fd;
            }
            else {
                LOGV << "munmap completed successfully for fd: " << mem.fd;
            }
        }
        if (close(mem.fd) != 0) {
            LOGE << "close fd=" << mem.fd << " failed, errno= " << strerror(errno);
        }
    }

    synap_destroy_io_buffer(mem.bid);
}


bool AllocatorIon::cache_flush(const Memory& mem, size_t size)
{
    if (!mem.handle) {
        LOGV << "cache_flush null, ignore";
        return true;
    }
    if (_secure) {
        LOGI << "Flushing cache not implemented for secure buffer";
        return true;
    }
    LOGV << "Flushing cache at address: " << mem.address;
    if (!suspend_cpu_access(mem.fd)) {
        return {};
    }
    return true;
}


bool AllocatorIon::cache_invalidate(const Memory& mem, size_t size)
{
    if (!mem.handle) {
        LOGV << "cache_invalidate null, ignore";
        return true;
    }
    if (_secure) {
        LOGI << "Invalidating cache not implemented for secure buffer";
        return true;
    }
    LOGV << "Invalidating cache at address: " << mem.address;
    if (!resume_cpu_access(mem.fd)) {
        return {};
    }
    return true;
}


Allocator* ion_allocator()
{
    static AllocatorIon allocator{false, false};
    return allocator.available()? &allocator : nullptr;
}


Allocator* ion_secure_allocator()
{
#ifdef USE_ALLOCATOR_SECURE_DEBUG
    extern Allocator* ion_secure_allocator_debug();
    return ion_secure_allocator_debug();
#else
    static AllocatorIon allocator{false, true};
    return allocator.available()? &allocator : nullptr;
#endif
}

Allocator* ion_cma_allocator()
{
    static AllocatorIon allocator{true, false};
    return allocator.available()? &allocator : nullptr;
}

Allocator* ion_cma_secure_allocator()
{
#ifdef USE_ALLOCATOR_SECURE_DEBUG
    extern Allocator* ion_cma_secure_allocator_debug();
    return ion_cma_secure_allocator_debug();
#else
    static AllocatorIon allocator{true, true};
    return allocator.available()? &allocator : nullptr;
#endif
}

Allocator* cma_allocator()
{
    return ion_cma_allocator();
}


}  // namespace synap
}  // namespace synaptics
