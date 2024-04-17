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
#ifndef __ASN1_PARSE_H
#define __ASN1_PARSE_H


/* This is the base type that holds just about everything :-) */

//copy from openssl. just put it here and we define the struct ourself
struct asn1_string_st
{
	int length;
	int type;
	unsigned char *data;
	/* The value of the following field depends on the type being
	 * held.  It is mostly being used for BIT_STRING so if the
	 * input data has a non-zero 'unused bits' value, it will be
	 * handled correctly */
	long flags;
};


typedef struct {
	//uint8_t tag_type;		//universal(00)¡¢application(0 1)¡¢context-specific(1 0)ºÍprivate(11)
	uint8_t data_type;		// 0 basic; 1 complex
	uint32_t tag_len;		// the length of the tag
	uint32_t len_len;		// the length of the length area
	uint64_t data_len;		// the lenght of the real data
	uint8_t * start;		// the point of content
	uint8_t * data;			// the point of real data
}asn1_info;

int simple_asn1_parse(uint8_t * buff, asn1_info * info, uint32_t index);

#endif
