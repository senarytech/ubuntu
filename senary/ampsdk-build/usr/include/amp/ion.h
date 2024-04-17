/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
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

#ifndef _LINUX_ION_H
#define _LINUX_ION_H

#include "shm_uapi.h"
#if defined(MV_SHM_ION_4_14)
#include "ion_4_14.h"
#elif defined(MV_SHM_ION_5_4)
#include "ion_5_4.h"
#else
#error No uapi ion header file, please check!
#endif

#include "berlin_cc.h"
#include "bm.h"

struct ion_handle {
    int fd;
    int gid;
};


#define ION_A_CH            0x1000  /* For hardware (class) memory */

#define ion_open(...)       osal_ion_open(__VA_ARGS__)
#define ion_close(...)      osal_ion_close(__VA_ARGS__)
#define ion_alloc(...)      osal_ion_alloc(__VA_ARGS__)
#define ion_free(...)       osal_ion_free(__VA_ARGS__)
#define ion_share(...)      osal_ion_share(__VA_ARGS__)
#define ion_import(...)     osal_ion_import(__VA_ARGS__)

/**  open ion device
 *
 * This function will open /dev/ion device and get a file descriptor
 *
 * @retval fd < 0		open device fail
 * @retval fd >= 0		open device success
 *
 */
int osal_ion_open(void);


/**  close ion device
 *
 * This function will close /dev/ion device
 *
 * @param fd  a file descriptor by open ion device to got.
 *
 * @retval -1		close device fail
 * @retval 0		close device success
 *
 */
int osal_ion_close(int fd);


/**  ion allocate memory
 *
 * This function will allocate memory
 *
 * @param fd		a file descriptor by open ion device to got.
 *
 * @param len		size of the allocation
 *
 * @param align		requested allocation alignment, lots of hardware blocks
 *			have alignment requirements of some kind.
 *
 * @param heap_mask	use it to choose a memory pool
 *
 * @param flags		ION_CACHED: cacheable memory.
 *			ION_NONCACHED: non-cacheable memory.
 *
 * @param handle		return a pointers to pointers of manage a ion buffer, this
 *			handle will be used by some ion apis, eg: ion_free,
 *			ion_share and so on.
 *
 * @retval -1  		allocate memory fail
 * @retval  0  		allocate memory success
 *
 */
int osal_ion_alloc(int fd, size_t len, size_t align, unsigned int heap_mask,
	               unsigned int flags, struct ion_handle **handle);


/**  ion free memory
 *
 * This function will free memory
 *
 * @param fd		a file descriptor by open ion device to got.
 *
 * @param handle		a pointers to pointers of manage a ion buffer, this handle by
 *			ion_alloc function got.
 *
 * @retval -1		free memory fail
 * @retval  0		free memory success
 *
 */
int osal_ion_free(int fd, struct ion_handle *handle);


/**  ion creates a file descriptor to use to share an allocation memory
 *
 * This function will get a share file descriptor for mmap() function to get virtual address and
 * use it to get a global id for another process to use it.
 *
 * @param fd		a file descriptor by open ion device to got.
 *
 * @param handle		a pointers to pointers of manage a ion buffer, this handle by
 *			ion_alloc function got.
 *
 * @param share_fd	return a dma-buf share file descriptor
 *
 * @retval -EINVAL 	get dma-buf share file descritor fail
 * @retval  0		get dma-buf share file descritor success
 *
 */
int osal_ion_share(int fd, struct ion_handle *handle, int *share_fd);


/**  ion imports a shared file descriptor
 *
 * This function will import a dma-buf share file descriptor.
 *
 * @param fd		a file descriptor by open ion device to got.
 *
 * @param handle		return a pointers to pointers of manage a ion buffer.
 *
 * @param share_fd	a dma-buf share file descriptor.
 *
 * @retval -1 		import dma-buf share file descritor fail
 * @retval  0		import dma-buf share file descritor success
 *
 */
int osal_ion_import(int fd, int share_fd, struct ion_handle **handle);


/**  ion get physical information of a ion buffer
 *
 * This function will get a ion buffer's physical address and size
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param share_fd      a dma-buf share file descriptor.
 *
 * @param addr          return the ion buffer's physical address
 *
 * @param len           return the ion buffer's memory size.
 *
 * @retval -1           get physical information fail
 * @retval  0           get physical information success
 *
 */
int ion_getphys(int fd, int share_fd, size_t *addr, size_t *len);


/**  ion clean cache operate
 *
 * This function will clean cache
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param share_fd      a dma-buf share file descriptor.
 *
 * @param virtaddr      need clean cache memory's virtual address
 *
 * @param offset        need clean cache memory's offset
 *
 * @param size          need clean cache memory's size.
 *
 * @retval -1           clean cache operate fail
 * @retval  0           clean cache operate success
 *
 */
int ion_cleancache(int fd, int share_fd, void *virtaddr, size_t offset, size_t size);


/**  ion clean and invalidatecache operate
 *
 * This function will clean cache
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param share_fd      a dma-buf share file descriptor.
 *
 * @param virtaddr      need clean and invalidatecache cache memory's virtual address
 *
 * @param offset        need clean and invalidatecache cache memory's offset
 *
 * @param size          need clean and invalidatecache cache memory's size.
 *
 * @retval -1           clean and invalidatecache cache operate fail
 * @retval  0           clean and invalidatecache cache operate success
 *
 */
int ion_flushcache(int fd, int share_fd, void *virtaddr, size_t offset, size_t size);


/**  ion invalidate cache operate
 *
 * This function will invalidate cache
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param share_fd      a dma-buf share file descriptor.
 *
 * @param virtaddr      need invalidate cache memory's virtual address
 *
 * @param offset        need invalidate cache memory's offset
 *
 * @param size          need invalidate cache memory's size.
 *
 * @retval -1           invalidate cache operate fail
 * @retval  0           invalidate cache operate success
 *
 */
int ion_invalidatecache(int fd, int share_fd, void *virtaddr, size_t offset, size_t size);


/**  ion get gid
 *
 * This function will get a global id in the linux kernel
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param share_fd      a dma-buf share file descriptor.
 *
 * @param *gid      save the return a global id.
 *
 * @retval -1           get a global id fail
 * @retval  0           get a global id success
 *
 */
int ion_getgid(int fd, int share_fd, int *gid);


/**  ion put gid
 *
 * This function will put a global id in the linux kernel, then, the global
 * id will be free.
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param gid	   went free's global id.
 *
 * @retval -1           put a global id fail
 * @retval  0            put a global id success
 *
 */
int ion_putgid(int fd, int gid);


/**  ion getsfd
 *
 * This function will get a current process's share fd by the global id
 *
 * @param fd            a file descriptor by open ion device to got.
 *
 * @param share_fd      a dma-buf share file descriptor.
 *
 * @param virtaddr      need invalidate cache memory's virtual address
 *
 * @param size          need invalidate cache memory's size.
 *
 * @retval -1           get a current process's share fd fail
 * @retval  0            get a current process's share fd success
 *
 */
int ion_getsfd(int fd, int gid, int *share_fd);

/*
 * decrease ref count on the dmabuf associated with the gid
 */
int ion_put(int fd, int gid);

int ion_init(void);


void ion_release(void);


/**  ion get osal fd
 *
 * This function will get a fd by osal init to open /dev/ion.
 *
 * @retval <0           get a /dev/ion fd fail
 * @retval  other       get a /dev/ion fd success
 *
 */
int ion_get_osal_fd(void);
int ion_get_osal_heap_fd(void);

int ion_get_cc_fd(void);

/**  ion query heap data
 *
 * This function will get all heap data
 *
 * @param fd	a file descriptor by open ion device to got.
 *
 * @param query	got all heap data, and the head data's memory is
 *		sizeof(struct ion_heap_data) * heap_num.
 *
 * @retval <0	fail
 * @retval  0	success
 *
 */
int ion_query_heap(int fd, struct ion_heap_query *query);

/**  ion get heap number
 *
 * This function will get heap number
 *
 * @param fd	a file descriptor by open ion device to got.
 *
 * @param heap_num	got heap number.
 *
 * @retval <0	fail
 * @retval  0	success
 *
 */
int ion_get_heap_num(int fd, int *heap_num);


/**  ion get all heap info
 *
 * This function will get all heap info
 *
 * @param fd	a file descriptor by open ion device to got.
 *
 * @param info	got all heap info, and the info's memory is
 *		sizeof(struct ion_heap_info) * heap_num.
 *
 * @retval <0	fail
 * @retval  0	success
 *
 */
int ion_get_heap_info(int fd, struct ion_heap_info *info);


/**  ion get cc heap mask
 *
 * This function will get cc heap mask
 *
 * @param fd	a file descriptor by open ion device to got.
 *
 * @retval  0	fail
 * @retval  >0	success
 *
 */
unsigned int ion_get_cc_heap(void);
void ion_dump_backtrace(char *tag);


int ion_cc_reg(int fd, struct berlin_cc_info *info);
int ion_cc_free(int fd, struct berlin_cc_info *info);
int ion_cc_inquiry(int fd, struct berlin_cc_info *info);
int ion_cc_update(int fd, struct berlin_cc_info *info);

int ion_get_bm_fd(void);
int ion_allocpt(int fd, struct bm_fb_data *fb);
int ion_getpt(int fd, struct bm_pt_data *pt);
int ion_putpt(int fd, int share_fd);

#endif /* _LINUX_ION_H */
