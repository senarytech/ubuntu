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
#include "io.h"
#include "memmap.h"
#include "global.h"
#include "chip_id.h"
#include "debug.h"

unsigned int get_chip_type(void)
{
	volatile unsigned int *p32 = (volatile unsigned int *)(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId);

	INFO("Product Revision ID    : 0x%04x\n", (p32[0]>>28)&0xF);
	INFO("Product Part Number    : 0x%04x\n", (p32[0]>>12)&0xFFFF);
	INFO("Product Manufacture ID : 0x%04x\n", (p32[0]>>1)&0x7FF);
	return 0x0;
}

unsigned int get_chip_ver(unsigned int silicon_ver_otp)
{
	unsigned int chip_ver = VS680_CHIP_VERSION_A0;
	volatile unsigned int *p32 = (volatile unsigned int *)(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId_ext);

	INFO("Product Extension ID   : 0x%02x\n", p32[0]&0xFF);

	if (0xB0 == ((*p32) & 0xFF)) {
		switch (silicon_ver_otp){
			case 0 :
				chip_ver = VS680_CHIP_VERSION_A0;
				break;
			case 1 :
				chip_ver = VS680_CHIP_VERSION_A1;
				break;
			default :
				ERR("Unknown silicon version otp 0x%x\n", silicon_ver_otp);
				break;
		}
	}
	else
		ERR("Unknown Product Extension ID(0x%x) for VS680\n", p32[0]&0xFF);

	return chip_ver;
}

