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
#ifndef	_IMAGE2_HEADER_H_
#define	_IMAGE2_HEADER_H_

#include "version_table.h"

/*
 * bootloader block layout:
 * 0       : Im2-part1
 * 52K     : VERSION TABLE
 * 56K     : cust figo
 * 60K     : cust key
 * 64K     : Im2-part2
 */
#define IMG2_PART1_SIZE		52<<10
#define VT_OFFSET		(IMG2_PART1_SIZE)
#define VT_SIZE			4096
#define FIGO_OFFSET		((VT_OFFSET) + (VT_SIZE))
#define FIGO_SIZE		4096
#define KEY_OFFSET		((FIGO_OFFSET) + (FIGO_SIZE))
#define KEY_SIZE		4096
#define IMG2_PART2_OFFSET	((KEY_OFFSET) + (KEY_SIZE))

typedef struct _gen_header_t_ {
	unsigned int magic;
	unsigned int offset;
	unsigned int length;
	unsigned int exe_addr;
	unsigned int crc;
	unsigned int flag;
} gen_header_t;

typedef struct _image2_header_t {
	char part1[IMG2_PART1_SIZE];
	union{
		version_table_t vt;
		char vt_pad[VT_SIZE];
	};
	union{
		gen_header_t figo;
		char figo_pad[FIGO_SIZE];
	};
	union{
		gen_header_t cust_key;
		char cust_key_pad[KEY_SIZE];
	};
	char part2[];
} image2_header_t;

#endif
