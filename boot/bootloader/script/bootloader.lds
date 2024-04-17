/*******************************************************************************
 * Copyright (C) Marvell International Ltd. and its affiliates
 *******************************************************************************
 * Marvell GPL License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File in accordance with the terms and conditions of the General
 * Public License Version 2, June 1991 (the "GPL License"), a copy of which is
 * available along with the File in the license.txt file or by writing to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
 * on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
 * DISCLAIMED.  The GPL License provides additional details about this warranty
 * disclaimer.
 *******************************************************************************/
/*
 * bootloader memory layout:
 *  0x04100000 - (0x04100000 + 0x4000) 	16kb buff of bl_boot_arg from miniloader
 *  (0x04100000 + 0x4000) - (0x04900000 - 0x4000) code, rodata, data, bss, heap. stack
 *  (0x04100000 + 0x180000) - (0x04100000 + 0X200000) svc_stack, irq_stack
 *  (0x04100000 + 0x200000) - 0x04900000 		ttb buffer (more than 4M)
 *
 *******************************************************************************/
OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(BootLoaderEntry)
SECTIONS
{
	. = 0x10800000;
	__bootloader_buf_start = .;

	. = 0x04100000;
	. = ALIGN(4);
	__bl_boot_args_buf = .;

	. = 0x04100000 + 0x4000;
	. = ALIGN(4);
	.text :
	{
		*(.text)
		. = ALIGN(8);
		__rt_bl_stage_descs_start__ = .;
		KEEP(*(rt_bl_stage_descs))
		__rt_bl_stage_descs_end__ = .;
	}

	.rodata :
	{
		*(.rodata)
	}

        . = ALIGN(4);
	.data :
	{
		*(.data)
	}
	__exidx_start = .;
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	__exidx_end = .;

	. = ALIGN(16);
	__bss_start = .;
        .bss :
	{
		*(.bss*)
	}
	. = ALIGN(16);
	__bss_stop = .;

	. = 0x04100000 + 0x160000;
        . = ALIGN(0x4000);
        __img_buff_start = .;

	. = 0x04100000 + 0x180000;
	. = ALIGN(4);
	__svc_stack_start = .;

	. = 0x04100000 + 0x190000;
	. = ALIGN(32);
	__heap_start = .;

	. = 0x04100000 + 0x2f0000;
	. = ALIGN(32);
	__heap_end = .;

	. = 0x04100000 + 0x300000;
	. = ALIGN(0x4000);
	__ttb_base_start = .;
}
