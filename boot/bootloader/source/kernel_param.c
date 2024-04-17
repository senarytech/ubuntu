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

#ifdef CONFIG_DTB
#include "libfdt.h"
#include "libfdt_syna.h"
#endif

#if defined(PV_COMP)
#include "chip_voltage_info.h"
#include "pv_comp.h"
#include "pmic_select.h"
#endif
#include "libxbc.h"

#include "global.h"
#include "system_manager.h"
#include "bl_boot_arg.h"
#include "gpt.h"
#include "load_gpt.h"

#if defined(SOC_RAM_TS_ENABLE) || defined (SOC_RAM_PARAM_ENABLE)
#include "mem_map_itcm.h"
#endif

#ifdef CONFIG_TRUSTZONE
#include "mem_region.h"
#include "mem_region_userdata.h"
#endif

#include "boot_mode.h"
#include "bootloader_header.h"
#include "chip_id.h"


// including seting up dtb
#if defined(CONFIG_VERIFYBOOT)
#define REBOOTFLAG_MAGIC            (0xF5F5F5F5)
#define SERIALNO_COMMAND_LINE       "androidboot.serialno="
#define BOOT_REASON_COMMAND_LINE    "androidboot.bootreason="
#define SYNA_SDK_RELEASE_REVISION   "androidboot.syna.sdk.revision="

typedef enum {
	UNKNOW = 0x0,
	REBOOT = 0x2,
	WATCHDOG = 0x4,
	KERNEL_PANIC = 0x8,
	NORMAL_FLOW = 0xFF,
}REBOOT_REASON_STATE;

static int rebootflag = NORMAL_FLOW;

#endif

#define MAC_ADDR_LEN 17

#if defined(LINUXSDK_BOOTLOADER)
#define ROOTFS_CONFIG_LINE "root=/dev/mmcblk0p"
#define ROOTFS_A "rootfs_a"
#define ROOTFS_B "rootfs_b"
#endif


#ifdef CONFIG_VERIFYBOOT
void parse_reboot_reason(void)
{
#ifdef CONFIG_SM
#if defined(SOC_RAM_PARAM_ENABLE)
	rebootflag_t * g_rebootflag = (rebootflag_t *)(uintptr_t)(SOC_REBOOTREASON_ADDR);
#else
	rebootflag_t * g_rebootflag = (rebootflag_t *)(uintptr_t)(SM_REBOOTREASON_ADDR_SOC);
#endif

	if(g_rebootflag->magic == REBOOTFLAG_MAGIC) {//flag is set
		if(g_rebootflag->flag == REBOOT) {
			dbg_printf(PRN_NOTICE, "BOOT_REASON: reboot\n");
			rebootflag = REBOOT;
			return;
		}
		else if(g_rebootflag->flag == WATCHDOG) {
			dbg_printf(PRN_NOTICE, "BOOT_REASON: watchdog\n");
			rebootflag = WATCHDOG;
			return;
		}
		else if(g_rebootflag->flag == KERNEL_PANIC) {
			dbg_printf(PRN_NOTICE, "BOOT_REASON: kernel_panic\n");
			rebootflag = KERNEL_PANIC;
			return;
		}
		else {
			dbg_printf(PRN_NOTICE, "BOOT_REASON: unknow\n");
			rebootflag = UNKNOW;
			return;
		}
	}

	dbg_printf(PRN_RES, "BOOT_REASON: normal by default, magic= 0x%x, flag= 0x%x\n", g_rebootflag->magic, g_rebootflag->flag);
	rebootflag = NORMAL_FLOW;

	g_rebootflag->magic = NORMAL_FLOW;  //Need clear the status after obtaining the flag.
	g_rebootflag->flag  = NORMAL_FLOW;
#else
	rebootflag = NORMAL_FLOW;
#endif
}
#endif

#ifdef CONFIG_DTB
int set_chiprev(void *fdt, int total_space, unsigned int chip_rev)
{
	int ret;
	int offset = 0;
	unsigned int rev = 0x0;

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if(ret < 0)
		return ret;

	/* find the offset in FDT*/
	offset = fdt_path_offset(fdt, "/soc/chipid");
	if(offset == -FDT_ERR_NOTFOUND) {
		ERR("didn't find point of /soc/chipid\n");
		return offset;
	}

	rev = cpu_to_fdt32(chip_rev);
	fdt_setprop(fdt, offset, "chip-revision", (const void *)(uintptr_t)(&rev), sizeof(rev));

	return fdt_pack(fdt);
}

int set_tz_enable(void *fdt, int total_space)
{
	int ret;
	int offset_soc = 0;
	int offset_fw = 0;

	const char cmdline[] = "berlin,secure-firmware";

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ret;

	/* find the offset in FDT */
	offset_fw = fdt_path_offset(fdt, "/soc/firmware");
	if (offset_fw == -FDT_ERR_NOTFOUND) {
		offset_soc = fdt_path_offset(fdt, "/soc");
		if (offset_soc == -FDT_ERR_NOTFOUND)
			return offset_soc;
		offset_fw = fdt_add_subnode(fdt, offset_soc, "/firmware");
		if (offset_fw < 0)
			return offset_fw;
	}

	/* modify the bootargs in FDT */
	ret = fdt_setprop_string(fdt, offset_fw, "compatible", cmdline);
	if(ret)
		return ret;

	return fdt_pack(fdt);
}

int set_system_memory_region(void *fdt, int total_space, struct mem_region *system_mem, int pool_num)
{
	unsigned int mem_region_property[8];
	unsigned int value[8];
	int memcount = 0;
	int offset = 0;
	int len = 0, i = 0;
	int ret;
	const struct fdt_property * fdt_prop = NULL;

	// linux only support two system region maximumly
	if((pool_num > 2) || (pool_num <=0))
		return -1;

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ret;

	offset = fdt_path_offset(fdt, "/memory");

	fdt_prop = fdt_get_property(fdt, offset, "reg", &len);

	dbg_printf(PRN_RES, "original system memory region:\n");
	for(i = 0; i < len/4; i += 4) {
		value[i] = fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i*4)));
		value[i + 1] = fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i*4 + 4)));
		value[i + 2] = fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i*4 + 8)));
		value[i + 3] = fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i*4 + 12)));
		dbg_printf(PRN_RES, "(0x%08x, 0x%08x, 0x%08x, 0x%08x)\n", value[i], value[i+1], value[i+2], value[i+3]);
	}

	dbg_printf(PRN_RES, "update system memory region to:\n");
	for(i = 0; i < pool_num; i++) {
		mem_region_property[memcount++] = cpu_to_fdt32(0x0);
		mem_region_property[memcount++] = cpu_to_fdt32(system_mem[i].base);

		mem_region_property[memcount++] = cpu_to_fdt32(0x0);
		mem_region_property[memcount++] = cpu_to_fdt32(system_mem[i].size);
		dbg_printf(PRN_RES, "(0x%08x, 0x%08x, 0x%08x, 0x%08x)\n", 0x0, system_mem[i].base, 0x0, system_mem[i].size);
	}

	fdt_setprop(fdt, offset, "reg", mem_region_property, 4*memcount);

	return fdt_pack(fdt);
}

int set_gpu_memory_region(void *fdt, int total_space, struct mem_region *mem)
{
	unsigned int mem_region_property[2* NR_BANKS];

	unsigned int nonsecure_mem_baseh = 0;
	unsigned int nonsecure_mem_basel = 0;
	unsigned int nonsecure_mem_sizeh = 0;
	unsigned int nonsecure_mem_sizel = 0;

	int offset, offset_soc;
	int len = 0;
	int ret;
	const struct fdt_property * fdt_prop = NULL;

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ret;

	offset = fdt_path_offset(fdt, "/soc/gpu3d");
	if (offset == -FDT_ERR_NOTFOUND) {
		offset = fdt_path_offset(fdt, "/soc/gpu");
		if (offset == -FDT_ERR_NOTFOUND) {
			offset_soc = fdt_path_offset(fdt, "/soc");
			if (offset_soc == -FDT_ERR_NOTFOUND)
				return offset_soc;
			offset = fdt_add_subnode(fdt, offset_soc, "/gpu3d");
			if (offset < 0)
				return offset;
		}
	}

	fdt_prop = fdt_get_property(fdt, offset, "marvell,nonsecure-mem-base", &len);

	const char *ptprop1 = fdt_prop->data;
	nonsecure_mem_baseh = fdt32_to_cpu(*((unsigned int *)ptprop1));
	nonsecure_mem_basel = fdt32_to_cpu(*((unsigned int *)(ptprop1 + 4)));
	dbg_printf(PRN_DBG, "old: nonsecure-mem-base start: (0x%08x, 0x%08x) \n",
		nonsecure_mem_baseh, nonsecure_mem_basel);

	mem_region_property[0] = cpu_to_fdt32(0x0);
	mem_region_property[1] = cpu_to_fdt32(mem->base);

	fdt_setprop(fdt, offset, "marvell,nonsecure-mem-base", mem_region_property, 4 * 2);

	dbg_printf(PRN_DBG, "new: nonsecure-mem-base start: (0x%08x, 0x%08x) \n", 0x0, mem->base);

	fdt_prop = fdt_get_property(fdt, offset, "marvell,nonsecure-mem-size", &len);

	const char *ptprop2 = fdt_prop->data;
	nonsecure_mem_sizeh = fdt32_to_cpu(*((unsigned int *)ptprop2));
	nonsecure_mem_sizel = fdt32_to_cpu(*((unsigned int *)(ptprop2 + 4)));

	dbg_printf(PRN_DBG, "old:  nonsecure_mem_size: (0x%08x, 0x%08x)\n",
		nonsecure_mem_sizeh, nonsecure_mem_sizel);

	mem_region_property[0] = cpu_to_fdt32(0x0);
	mem_region_property[1] = cpu_to_fdt32(mem->size);

	fdt_setprop(fdt, offset, "marvell,nonsecure-mem-size", mem_region_property, 4 * 2);

	dbg_printf(PRN_DBG, "new:  nonsecure_mem_size: (0x%08x, 0x%08x)\n", 0x0, mem->size);

	return fdt_pack(fdt);
}

#define ION_COMPATIBLE_NAME "syna,ion-berlin-heaps"
#define ION_POOL_NUM "pool-num"
#define ION_ATTR_NUM_PER_POOL "attributes-num-per-pool"
#define ION_POOL_ATTRIBUTES "pool-attributes"

void dump_ion_memory_region(void *fdt, int total_space)
{
	const struct fdt_property * fdt_prop = NULL;
	struct mem_region mem[MAX_REGION_COUNT];
	unsigned int pool_num, attr_num, attr_val;
	int i, j, len, ret, offset;

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ;

	offset = fdt_node_offset_by_compatible(fdt, -1, ION_COMPATIBLE_NAME);
	if (offset == -FDT_ERR_NOTFOUND) {
		dbg_printf(PRN_ERR, "can't find ion node in dtb\n");
		return;
	}

	fdt_prop = fdt_get_property(fdt, offset, ION_POOL_NUM, &len);

	const char *ptprop1 = fdt_prop->data;
	pool_num = fdt32_to_cpu(*((unsigned int *)ptprop1));

	fdt_prop = fdt_get_property(fdt, offset, "reg", &len);
	for (i = 0; i < pool_num; i++) {
		mem[i].base = fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i * 16 + 4) ));
		mem[i].size= fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i * 16 + 12)));
	}

	fdt_prop = fdt_get_property(fdt, offset, "reg-names", &len);
	strcpy(mem[0].name, (char *)(fdt_prop->data));
	for (i = 0, j = 1; i < len; i++) {
		if (*((unsigned char *)(fdt_prop->data + i)) == '\0') {
			if ( (i + 1) == len)
				break;
			strcpy(mem[j++].name, (char *)(fdt_prop->data + i + 1));
		}
	}

	fdt_prop = fdt_get_property(fdt, offset, ION_ATTR_NUM_PER_POOL, &len);
	const char *ptprop2 = fdt_prop->data;
	attr_num = fdt32_to_cpu(*((unsigned int *)ptprop2));

	dbg_printf(PRN_RES, "ion_pool_num : 0x%08x, ion-attributes-num-per-pool : 0x%08x\n", pool_num, attr_num);

	fdt_prop = fdt_get_property(fdt, offset, ION_POOL_ATTRIBUTES, &len);

	for (i = 0; i < pool_num; i++) {
		dbg_printf(PRN_RES, "pool[%d]: name: %s, base =0x%08x , size = 0x%08x ", i, mem[i].name, mem[i].base, mem[i].size);

		for (j = 0; j < attr_num; j++) {
			attr_val = fdt32_to_cpu(*((unsigned int *)(fdt_prop->data + i * attr_num*4 + j * 4) ));
			dbg_printf(PRN_RES, "ion_attr[%d]: 0x%08x ",  j, attr_val);
		}
		dbg_printf(PRN_RES,"\n");
	}

}
int set_ion_memory_region(void *fdt, int total_space, struct mem_region *ion_mem, int pool_num)
{
	unsigned int mem_region_property[4 * MAX_REGION_COUNT];
	int offset;
	int i, ret, len, memcount=0;
	char reg_names[MAX_REGION_COUNT * 16];

	if (pool_num <= 0 || ion_mem == NULL)
		return -1;

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ret;

	offset = fdt_node_offset_by_compatible(fdt, -1, ION_COMPATIBLE_NAME);
	if (offset == -FDT_ERR_NOTFOUND) {
		dbg_printf(PRN_ERR, "can't find ion node in dtb\n");
		return offset;
	}

	fdt_get_property(fdt, offset, ION_POOL_NUM, &len);

	mem_region_property[0] = cpu_to_fdt32(pool_num);
	fdt_setprop(fdt, offset, ION_POOL_NUM, mem_region_property, 4);

	for (memcount=0, i = 0; i < pool_num; i++) {
		mem_region_property[memcount++] = cpu_to_fdt32(0x0);
		mem_region_property[memcount++] = cpu_to_fdt32(ion_mem[i].base);
		mem_region_property[memcount++] = cpu_to_fdt32(0x0);
		mem_region_property[memcount++] = cpu_to_fdt32(ion_mem[i].size);
	}

	fdt_setprop(fdt, offset, "reg", mem_region_property, 4 * memcount);

	for (len=0,i = 0; i < pool_num; i++) {
		strcpy(reg_names + len, ion_mem[i].name);
		len += strlen(ion_mem[i].name) + 1;
	}
	ret = fdt_setprop(fdt, offset, "reg-names", reg_names, len);

	mem_region_property[0] = cpu_to_fdt32(2);
	fdt_setprop(fdt, offset, ION_ATTR_NUM_PER_POOL, mem_region_property, 4);

	for (memcount=0, i = 0; i < pool_num; i++) {
		mem_region_property[memcount++] = cpu_to_fdt32(TEE_MR_ION_ALG(ion_mem + i));
		mem_region_property[memcount++] = cpu_to_fdt32(TEE_MR_ION_ATTRIB(ion_mem + i));
	}
	fdt_setprop(fdt, offset, ION_POOL_ATTRIBUTES, mem_region_property, 4 * memcount);

	return fdt_pack(fdt);
}

int set_tz_memory_region(void *fdt, int total_space)
{
	struct mem_region ion_mem[MAX_REGION_COUNT];
	struct mem_region system_mem[2];
	int i, ion_num = 0, system_num = 0;
	unsigned int sys_start = 0, sys_end = 0;
	struct mem_region * mem = NULL;

	unsigned int reserved_num = 0x0;
	struct reserve_entry reserved_mem[MAX_REGION_COUNT];

	mem = (struct mem_region *)get_tz_memregion();
	dbg_printf(PRN_DBG, "max region count :0x%08x, region_count = 0x%08x.\n", MAX_REGION_COUNT, get_tz_memregion_num());

	for (i = 0; i < get_tz_memregion_num(); i++) {
		dbg_printf(PRN_DBG, "memory region from TZ [%d]: name: %s, base =0x%08x , size = 0x%08x, userdata[0] = 0x%08x, userdata[1] = 0x%08x\n",
			i, mem[i].name, mem[i].base, mem[i].size, mem[i].userdata[0], mem[i].userdata[1]);

		if (TEE_MR_USER_IS_SYSTEM(mem+i)) {
			memcpy(system_mem + system_num++, mem + i, sizeof(struct mem_region));
			if((sys_start == 0) && (sys_end == 0)) {
				//only care the first system memory region
				sys_start = mem[i].base;
				sys_end = sys_start + mem[i].size;
			}
		}

#ifdef LINUX_4_14
		if (TEE_MR_USER_IS_ION(mem+i) && !TEE_MR_ION_IS_CMA(mem+i)) {
#else
			if (TEE_MR_USER_IS_ION(mem+i)) {
#endif
			memcpy(ion_mem + ion_num++, mem + i, sizeof(struct mem_region));
		}
	}

	//set the region of memreserve, it's the overlap between system and ion
	memset(reserved_mem, 0x0, sizeof(reserved_mem));
	for(i = 0; i < get_tz_memregion_num(); i++) {
		if(TEE_MR_ION_IS_CMA(mem+i) && TEE_MR_USER_IS_ION(mem+i))
			continue;

		/* Continue below reserve memory check for Non-CMA ION */
		/* Notice:
		 *     1. Only the first system is the case we handle.
		 *     2. ION regions should not have overlay with each other.
		 */
		if(TEE_MR_USER_IS_ION(mem+i)) {
			//ion is overlap with system, then set the reserved memory
			if((mem[i].base >= sys_start) && (mem[i].base < sys_end)) {
				dbg_printf(PRN_RES, "memory layout find overlap, 0x%x, 0x%x, (0x%x:0x%x)\n", mem[i].base, mem[i].size, sys_start, sys_end);
				if(mem[i].size <= (sys_end - mem[i].base)) {
					reserved_mem[reserved_num].reserved_mem_start = mem[i].base;
					reserved_mem[reserved_num].reserved_mem_size = mem[i].size;
					reserved_num++;
				} else {
					//fdt_set_reserved_mem(fdt, mem[i].base, (sys_end - mem[i].base));
					dbg_printf(PRN_ERR, "memory layout configuration error!!!\n");
				}
			}
		}
	}
	if(reserved_num <= MAX_REGION_COUNT) {
		fdt_set_reserved_mem(fdt, (void *)&reserved_mem, reserved_num);
		for(i = 0; i < reserved_num; i++)
			dbg_printf(PRN_RES, "reserved memory %d: Addr 0x%x, Size 0x%x\n", i, reserved_mem[i].reserved_mem_start,
					reserved_mem[i].reserved_mem_size);
	}

	if(system_num != 0)
		set_system_memory_region(fdt, total_space, system_mem, system_num);

	if (ion_num != 0) {
		set_ion_memory_region(fdt, total_space, ion_mem, ion_num);
		dump_ion_memory_region(fdt, total_space);
	}
#ifdef DUMP_DTB
	extern void dump_blob(void *blob);
	dump_blob(fdt);
#endif
	return 0;
}

#endif

#ifdef CONFIG_ENABLE_DTBO
#include "ufdt_impl.h"
#include "libfdt_env.h"
#include "dtbo.h"
#include "mmgr.h"

extern unsigned char * get_dtbo_image(void);

static int apply_dtbo(void)
{
	int ret = -1;
	void * dtbo_buff = (void *)get_dtbo_image();
	unsigned int overlay_size = 0;
	void * main_buff = (void *)(uintptr_t)get_dtb_addr();
	unsigned int main_size = fdt_totalsize((void *)(uintptr_t)get_dtb_addr());
	void * merged = NULL;

	struct dt_table_header * header = (struct dt_table_header *)dtbo_buff;
	struct dt_table_entry * entry = NULL;
	void * pdtbo = NULL;
	unsigned int num = 0, i = 0;

	num = cpu_to_fdt32(header->dt_entry_count);

	//the first entry
	entry = (struct dt_table_entry *)(dtbo_buff + cpu_to_fdt32(header->header_size));
	// apply all dtbo to main
	// there is only one dtbo currently
	for(i = 0; i < num; i++) {
		pdtbo = (void *)(dtbo_buff + cpu_to_fdt32(entry->dt_offset));
		overlay_size = cpu_to_fdt32(entry->dt_size);
		//merge
		merged = apply_overlay(main_buff, main_size, pdtbo, overlay_size);
		if(merged) {
			//copy
			memcpy(main_buff, merged, fdt_totalsize(merged));
			// the buff is allocated in dtbo lib
			// to avoid there are many leak memory, free it here
			free(merged);
		} else {
			ret = -1;
			goto done;
		}
		//next entry
		entry++;
	}

	ret = 0;

	//FIXME: if failed to load/merge, how can we do?
done:
	return ret;
}
#endif

static int verify_macaddr(char const * const mac_addr)
{
	int i;

	if (mac_addr == NULL)
		return 1;

	for (i = 0; i < 17; i++) {
		const char digit = mac_addr[i];
		if ((i % 3) == 2) {
			if (digit != ':') // Ensure you have a colon
				return 1;
			continue;
		}
		// Ensure this is hex value
		if (((digit < '0') || (digit > '9'))
		&& ((digit < 'A') || (digit > 'F'))
		&& ((digit < 'a') || (digit > 'f')))
			return 1;
	} // Test end of string
	if ( mac_addr[17] != '\0')
		return 1;

	return 0;
}

static int make_macaddr(unsigned char *mac_addr_buf)
{
	char mac_addr[64], *string;
	unsigned char val;

	if (mac_addr_buf == NULL)
		return -1;

	get_macaddr_from_flash(mac_addr);
	mac_addr[MAC_ADDR_LEN] = '\0';
	dbg_printf(PRN_RES,"get macaddr from flash: %s\n", mac_addr);
	if(0 != verify_macaddr(mac_addr)) {
		NOTICE("Doesn't get valid MAC_ADDR from flash!!!!\n");
		return -1;
	}

	dbg_printf(PRN_RES,"Final macaddr: %s\n", mac_addr);

	string = mac_addr;
	do {
		val = (unsigned char)strtoul(string, &string, 16);
		*mac_addr_buf = val;
		mac_addr_buf++;
		string++;
	}while (*string != 0);

	return 0;
}

static int set_mac_addr(void *fdt, int total_space)
{
	int ret = -1;
	int offset = 0;
	int len = 0;
	unsigned char mac_addr[6];
	const unsigned char *p;

	/* Read MAC address from FTS*/
	if(make_macaddr(mac_addr))
	{
		NOTICE("Make mac_addr fail, do not change fdt for mac_addr!!!\n");
		return ret;
	}
	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ret;

	offset = fdt_path_offset(fdt, "/soc/ethernet");
	p = fdt_getprop(fdt, offset, "mac-address", &len);

	if (p) {
		// mac-address exists in node, change
		ret = fdt_setprop(fdt, offset, "mac-address", (const void*)mac_addr, 6);
	}
	else {
		// mac-address does not exist in node, append
		ret = fdt_appendprop(fdt, offset, "mac-address", (const void*)mac_addr, 6);
	}

	if (ret != 0)
		dbg_printf(PRN_RES,"%s: set mac address to DTB fail\n", __func__);

	return ret;
}

static void setup_chipid(char *kernel_param)
{
	char tmp_buf[128];

	memset(tmp_buf, 0x0, sizeof(tmp_buf));
	snprintf(tmp_buf, (sizeof(tmp_buf) - 1), " chipid=%016llx", get_chip_id());
	strcat(kernel_param, tmp_buf);
}


static void setup_cma_param(char *kernel_param)
{
	char tmp_buf[128];

	/* Notice: strict check for the cma_pool_addr should be non-zero */
	if(get_ion_cma_pool_size() && get_ion_cma_pool_addr()) {
		memset(tmp_buf, 0x0, sizeof(tmp_buf));
		snprintf(tmp_buf, (sizeof(tmp_buf) - 1), " cma=%d@%d", get_ion_cma_pool_size(), get_ion_cma_pool_addr());
		strcat(kernel_param, tmp_buf);
		dbg_printf(PRN_INFO, "Enable CMA Pool for Kernel: %s\n", tmp_buf);
	}
}

static void setup_fb_driver(char *kernel_param)
{
#if defined(LINUXSDK_BOOTLOADER)
	if(get_bootmode() == BOOTMODE_NORMAL) {
        //Allow FB driver loading/registration for linux-sdk normal mode only
		strcat(kernel_param, " tz_enable");
		strcat(kernel_param, " vppta");
	}
#endif

	if(get_bootmode() == BOOTMODE_RECOVERY) {
		strcat(kernel_param, " recovery");
	}
}

static void fdt_update_opp(void)
{
#if defined(PV_COMP)
	unsigned int opp[2];

	INFO("add vcpu opp to dtb\n");
	get_vcpu_vh_vl(opp);
	fdt_set_vcpu_opp((void *)(uintptr_t)get_dtb_addr(), opp[1], opp[0]);

	//Fix Me: if kernel support VCORE DVFS feature for VS680/VS640, we need enable below.
#if 0
	INFO("add vcore opp to dtb\n");
	opp[0] = opp[1] = 0u;
	get_vcore_vh_vl(opp);
	fdt_set_vcore_opp((void *)(uintptr_t)get_dtb_addr(), opp[1], opp[0]);
#endif
#endif
}

#if defined(LINUXSDK_BOOTLOADER)
static void setup_linux_rootfs_index(char *kernel_param)
{
#if defined(CONFIG_AB_BOOTFLOW)
	int index = -1;
	char index_str[8];

	/*upate rootfs partition*/
	strcat (kernel_param, ROOTFS_CONFIG_LINE);
	if (get_abmode() == BOOTSEL_B)
		index = find_partition(ROOTFS_B) + 1;
	else
		index = find_partition(ROOTFS_A) + 1;

	if (index != PARTITION_NOT_EXIST) {
		sprintf(index_str, "%d", index);
		strcat(kernel_param, index_str);
		strcat(kernel_param, " ro");
	}
#endif
}

void setup_linuxsdk_kernel_params(char *kernel_param)
{
	setup_linux_rootfs_index(kernel_param);
	setup_fb_driver(kernel_param);
	setup_chipid(kernel_param);
}
#endif

#if defined(CONFIG_VERIFYBOOT)
static void setup_android_bootreason(char *kernel_param)
{
#ifdef CONFIG_SM
	char reboot_reason[16] = "\0";

	switch(rebootflag){
		case NORMAL_FLOW:
			strcpy(reboot_reason, "normal");
			break;
		case REBOOT:
			strcpy(reboot_reason, "reboot");
			break;
		case KERNEL_PANIC:
			strcpy(reboot_reason, "kernel_panic");
			break;
		case WATCHDOG:
			strcpy(reboot_reason, "watchdog");
			break;
		default:
			strcpy(reboot_reason, "unknown");
			break;
	}

	strcat(kernel_param, " ");
	strcat(kernel_param, BOOT_REASON_COMMAND_LINE);
	strcat(kernel_param, reboot_reason);
#endif
}

static void setup_android_quiescent(char *kernel_param)
{
	if (get_bootoptions() & BOOTOPTION_QUIESCENT) {
		strcat(kernel_param, " ");
		strcat(kernel_param, "androidboot.quiescent=1");
		NOTICE("Boot Mode Option is quiescent!!!!\n");
	}
}

static void setup_android_serialno(char *kernel_param)
{
	char tmp_buf[128];

	memset(tmp_buf, 0x0, sizeof(tmp_buf));
	get_serialno_from_flash(tmp_buf);
	strcat(kernel_param, " ");
	strcat(kernel_param, SERIALNO_COMMAND_LINE);

	if(strlen(tmp_buf) > 0) {
		strcat(kernel_param, tmp_buf);//the max len is 64
	} else {
		//Chip_id as serial no if can't get it from flash.
		snprintf(tmp_buf, (sizeof(tmp_buf) - 1), "%016llx", get_chip_id());
		strcat(kernel_param, tmp_buf);
	}
}

static void setup_android_avb_param(char *kernel_param)
{
#ifdef CONFIG_VB20
	extern char * get_avb_kernel_commandline(void);
	strcat(kernel_param, get_avb_kernel_commandline());

	extern void vb_setup_linux_parameter(char * kernel_param);
	vb_setup_linux_parameter(kernel_param);
#endif

#ifdef CONFIG_AB_BOOTFLOW
	strcat(kernel_param, " ");
	strcat(kernel_param, "androidboot.slot_suffix=_");
	strcat(kernel_param, slot_name(get_abmode()));
#endif //CONFIG_AB_BOOTFLOW

	if(get_bootmode() == BOOTMODE_NORMAL)
		strcat(kernel_param, " androidboot.force_normal_boot=1");

	strcat(kernel_param, " ");
	// we only have one dtbo, set the fixed command line here
	strcat(kernel_param, "androidboot.dtbo_idx=0");

	strcat(kernel_param, " ");
	strcat(kernel_param, "androidboot.bootloader=2019.10-18917-g28fe186-dirty");
}

static void setup_sdk_release_revision(char *kernel_param)
{
	extern char *sdk_revision;
	if(sdk_revision)
	{
		strcat(kernel_param, " ");
		strcat(kernel_param, SYNA_SDK_RELEASE_REVISION);
		strcat(kernel_param, sdk_revision);
	}
}

static void cmdline_to_bootconfig(char *cmdline)
{
	for (int i = 0; i < strlen(cmdline); i++)
		if (cmdline[i] == ' ')
			cmdline[i] = '\n';
}

void setup_android_kernel_params(char *kernel_param)
{
	char params[2048];

	memset(params, 0, 2048);

	setup_android_avb_param(params);
	setup_android_bootreason(params);
	setup_android_quiescent(params);
	setup_android_serialno(params);
	setup_sdk_release_revision(params);

	if (strstr(kernel_param, "bootconfig"))
	{
		UINT32 bootconfig_addr = get_bootconfig_start_addr();
		UINT32 bootconfig_size = get_bootconfig_size();
		UINT32 additional_bootconfig_size;
		UINT32 ramdisk_size;

		cmdline_to_bootconfig(params);
		additional_bootconfig_size = addBootConfigParameters(params, strlen(params), bootconfig_addr, bootconfig_size);
		ramdisk_size = get_ramdisk_size() + additional_bootconfig_size;
		set_ramdisk_size(ramdisk_size);
	}
	else
	{
		strcat(kernel_param, " ");
		strcat(kernel_param, params);
	}
	setup_chipid(kernel_param);
	setup_fb_driver(kernel_param);
}
#endif

static void fdt_update(char *kernel_param)
{
#ifndef CONFIG_SSTZ
	set_fdt_addr(get_dtb_addr(), DTB_SPACE);

	if (set_fdt((void *)((uintptr_t)get_dtb_addr()), DTB_SPACE,
				get_ramdisk_buff(), get_ramdisk_size(), kernel_param))
		ERR("set fdt command line/initrd error!\n");

	if (set_mac_addr((void *)((uintptr_t)get_dtb_addr()), DTB_SPACE))
		ERR("set_mac_addr fail \n");

	if (set_chiprev((void *)((uintptr_t)get_dtb_addr()), DTB_SPACE,
				get_chip_revision()))
		ERR("fdt_set_chiprev fail \n");

	if (set_tz_enable((void *)((uintptr_t)get_dtb_addr()), DTB_SPACE))
		ERR("set_tz_enable fail \n");

	if (set_tz_memory_region((void *)((uintptr_t)get_dtb_addr()), DTB_SPACE))
		ERR("set_tz_memory_region fail \n");

	fdt_update_opp();
#endif

#ifdef CONFIG_ENABLE_DTBO
	apply_dtbo();
#endif
}


void setup_kernel_params(void)
{
#ifndef CONFIG_SSTZ
	char kernel_param[2048];

	strcpy(kernel_param, (char*)get_cmdline());

#if defined(CONFIG_VERIFYBOOT)
	setup_android_kernel_params(kernel_param);
#elif defined(LINUXSDK_BOOTLOADER)
	setup_linuxsdk_kernel_params(kernel_param);
#endif

	setup_cma_param(kernel_param);

	strcat(kernel_param, get_chip_kernel_param());

	dbg_printf(PRN_RES,"Generated bootargs: %s\n", kernel_param);
#endif

	fdt_update(kernel_param);
}


