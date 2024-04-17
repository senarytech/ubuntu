/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2021 Synaptics Incorporated. All rights reserved.
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

#include "synap/allocator.hpp"
#include "synap/logging.hpp"
#include "allocator_osal_secure.hpp"

namespace synaptics {
namespace synap {


AllocatorOsalSecure::AllocatorOsalSecure() {
    LOGI << "AllocatorSecure initialized";
}

Allocator::Memory AllocatorOsalSecure::alloc(size_t size)
{
    LOGI << "Allocating secure memory. Size: " << size;
    return do_alloc(size, true);
}

bool AllocatorOsalSecure::cache_flush(const Memory& mem, size_t size)
{
    if (!mem.handle) {
        LOGV << "cache_flush null, ignore";
        return true;
    }
    LOGI << "Flushing secure cache NOT IMPLEMENTED";
    return true;
}

bool AllocatorOsalSecure::cache_invalidate(const Memory& mem, size_t size)
{
    if (!mem.handle) {
        LOGV << "cache_invalidate null, ignore";
        return true;
    }
    LOGI << "Invalidating secure cache NOT IMPLEMENTED";
    return true;
}


Allocator* osal_secure_allocator()
{
#ifdef USE_ALLOCATOR_SECURE_DEBUG
    extern Allocator* osal_secure_allocator_debug();
    return osal_secure_allocator_debug();
#else
    static AllocatorOsalSecure allocator;
    return &allocator;
#endif
}

}  // namespace synap
}  // namespace synaptics
