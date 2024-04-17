/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
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
#include "com_type.h"
#include "io.h"
#include "string.h"
#include "debug.h"

#include "bootloader.h"
#include "bootloader_header.h"

#ifdef CONFIG_TRUSTZONE
#include "mem_region.h"
#include "tz_nw_boot.h"
#include "mem_region_userdata.h"

#ifndef CONFIG_SSTZ
static struct mem_region mem[MAX_REGION_COUNT] __attribute__((aligned(4096)));
static int region_count = 0;
#else

static struct mem_region mem[MAX_REGION_COUNT] = {
	{"sys_init",	0x00000000, 0x00100000, 0x0003000f, 0x0, 0x03000000, 0x00000000, 0x00000000, 0x00000000},
	{"tz", 			0x00100000, 0x03f00000, 0x8003000f, 0x0, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{"zsp", 		0x04000000, 0x00100000, 0x80020003, 0x0, 0x01000000, 0x00000000, 0x00000000, 0x00000000},
	{"bootloader", 	0x04100000, 0x00900000, 0x80020100, 0x0, 0x04000000, 0x00000000, 0x00000000, 0x00000000},
	{"system", 		0x04a00000, 0x15000000, 0x00030100, 0x0, 0x05000000, 0x00000000, 0x00000000, 0x00000000},
	{"NonSecure", 	0x14a00000, 0x05000000, 0x80030100, 0x0, 0x06000002, 0x00000f36, 0x00000000, 0x00000000},
	{"NonSecure-NC",0x19a00000, 0x02600000, 0x80020100, 0x0, 0x06000005, 0x00000f3a, 0x00000000, 0x00000000},
	{"Secure", 		0x1c000000, 0x04000000, 0x80030003, 0x0, 0x06000005, 0x00000f3d, 0x00000000, 0x00000000},
	{"system-ion", 	0x00000000, 0x00000000, 0x00030100, 0x0, 0x06000000, 0x00000000, 0x00000000, 0x00000000},
	{"register", 	0xf6000000, 0x03000000, 0x80018003, 0x0, 0x07000000, 0x00000000, 0x00000000, 0x00000000},
};

static int region_count = 10;
#endif
#endif
#include "mmgr.h"

#ifdef ARMV8_64BIT
#define LINUX_OFFSET 0x80000
#else
#define LINUX_OFFSET 0x8000
#endif
#define DTB_OFFSET 0x11000000    //256M after ramdisk

#define RAMDISK_OFFSET 0x08000000 //128M after kernel

static unsigned int kernel_addr = CPU0_START_ADDR;
static unsigned int ramdisk_addr = CPU0_START_ADDR - LINUX_OFFSET + RAMDISK_OFFSET;
static unsigned int dtb_addr = CPU0_START_ADDR - LINUX_OFFSET + DTB_OFFSET;

static unsigned int cma_pool_start = 0x0;
static unsigned int cma_pool_size = 0x0;

#define CACHEABLE_MEM_POOL_SIZE (128*1024*1024)
void init_ion_cacheable_mempool(void)
{
#ifdef CONFIG_TRUSTZONE
	int i = 0;
	unsigned long mstart = 0;
	unsigned int size = 0;
	for(i = 0; i < region_count; i++) {
		if(TEE_MR_ION_FOR_NONSECURE(&mem[i]) && TEE_MR_ION_FOR_CACHEABLE(&mem[i])) {
			mstart = (unsigned long)(mem[i].base);
			size = mem[i].size;

			/* ION_CMA must be one of Non-secure and Cacheable pool, Usually it is the same pool */
			if(TEE_MR_ION_IS_CMA(&mem[i]) && TEE_MR_USER_IS_ION(&mem[i]) && size) {
				cma_pool_start = mem[i].base;
				cma_pool_size = mem[i].size;
			}

			dbg_printf(PRN_RES, "nonsecure cacheable memory region base = 0x%08x, size 0x%08x\n", mstart, cma_pool_size);
			if(size < CACHEABLE_MEM_POOL_SIZE) {
				dbg_printf(PRN_RES, "Warning: nonsecure cacheable memory region size (0x%x) < 0x%08x\n", size, CACHEABLE_MEM_POOL_SIZE);
				dbg_printf(PRN_ERR, "Error: potential memory related issue caused by small ION_CACHEABLE pool !\n");
			}

			init_mmgr_by_type(MEM_ION_CACHEABLE, mstart, size);
			return;
		}
	}
	ERR("Error: cann't find non-secure ION cacheable memory!\n");
#else
	#error "don't support!"
#endif
}
void init_ion_noncacheable_mempool(void)
{
#ifdef CONFIG_TRUSTZONE
	int i = 0;
	unsigned long mstart = 0;
	unsigned int size = 0;
	for(i = 0; i < region_count; i++) {
		if(TEE_MR_ION_FOR_NONSECURE(&mem[i]) && TEE_MR_ION_FOR_NONCACHEABLE(&mem[i])) {
			mstart = (unsigned long)(mem[i].base);
			size = mem[i].size;
			//nonsecure noncacheable memory is small
			//if(size > NONCACHEABLE_MEM_POOL_SIZE) {
				dbg_printf(PRN_RES, "nonsecure noncacheable memory region base = 0x%08x, size 0x%08x\n", mstart, size);
				init_mmgr_by_type(MEM_ION_NONCACHEABLE, mstart, size);
				return;
			//}
		}
	}
	ERR("cann't find non-secure ION noncacheable memory!\n");
#else
	#error "don't support!"
#endif
}

void get_mem_region_list_from_tz(void)
{
#ifdef CONFIG_TRUSTZONE
	int i;
#ifndef CONFIG_SSTZ
	region_count = tz_nw_get_mem_region_list(mem, MAX_REGION_COUNT, 0, 0);
	if (region_count == 0)
		ERR("Memory Region count from TZ is 0 !!!\n");
#endif

	for(i=0;i<region_count;i++)
		dbg_printf(PRN_RES, "pool[%d]: name: %s, base =0x%08x, size = 0x%08x, attr = 0x%08x,"
		"userdata0 = 0x%08x,  userdata1 = 0x%08x, userdata2 = 0x%08x, userdata3 = 0x%08x\n",
				i, mem[i].name, mem[i].base, mem[i].size, mem[i].attr, mem[i].userdata[0],
				mem[i].userdata[1], mem[i].userdata[2], mem[i].userdata[3]);

	init_ion_cacheable_mempool();
	init_ion_noncacheable_mempool();
#endif
}

void * get_tz_memregion(void)
{
	return (void *)mem;
}

int get_tz_memregion_num(void)
{
	return region_count;
}

unsigned int get_ion_cma_pool_addr(void)
{
	return cma_pool_start;
}

unsigned int get_ion_cma_pool_size(void)
{
	return cma_pool_size;
}

unsigned int get_systempt_addr(void)
{
#ifdef CONFIG_TRUSTZONE
	int i = 0;
	unsigned int mstart = 0, system_start = 0;
	unsigned int msize = 0, system_size = 0;
	unsigned int pool_num = 0;

	for(i = 0; i < region_count; i++) {
		if(pool_num >= 0x2)
			break;

		if(TEE_MR_USER_IS_SYSTEM(&mem[i])) {
			system_start = mem[i].base;
			system_size = mem[i].size;
			pool_num++;
			continue;
		}
		if(TEE_MR_ION_FOR_NONSECURE(&mem[i]) && TEE_MR_ION_FOR_CACHEABLE(&mem[i])) {
			mstart = mem[i].base;
			msize = mem[i].size;
			pool_num++;
		}
	}

	if(pool_num >= 0x2) {
		if(system_start == mstart)
			return (mstart + msize);
		else if(mstart > system_start && (mstart + msize) == (system_start + system_size))
			return system_start;
		else {
			dbg_printf(PRN_RES, "WARN: system memory is physical discontinuity !\n");
			return system_start;
		}
	}

	return 0;
#else
	return 0;
#endif
}

void init_linux_addr(void)
{
	unsigned int sys_addr = get_systempt_addr();
	if(sys_addr != 0) {
		kernel_addr = sys_addr + LINUX_OFFSET;
		dtb_addr = sys_addr + DTB_OFFSET;
		ramdisk_addr = sys_addr + RAMDISK_OFFSET;
	}
}

unsigned int get_kernel_addr(void)
{
	return kernel_addr;
}

unsigned int get_dtb_addr(void)
{
	return dtb_addr;
}

unsigned int get_ramdisk_addr(void)
{
	return ramdisk_addr;
}

void * malloc_ion_cacheable(int size)
{
	return mmgr_alloc_by_type(MEM_ION_CACHEABLE, size);
}

void free_ion_cacheable(void * m)
{
	mmgr_free_by_type(MEM_ION_CACHEABLE, m);
}

void * malloc_ion_noncacheable(int size)
{
	return mmgr_alloc_by_type(MEM_ION_NONCACHEABLE, size);
}

void free_ion_noncacheable(void * m)
{
	mmgr_free_by_type(MEM_ION_NONCACHEABLE, m);
}
