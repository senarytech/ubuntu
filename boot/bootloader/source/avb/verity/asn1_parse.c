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
#include "asn1_parse.h"
#include "debug.h"


//#define ASN1_DEBUG

#define HIGH_TAG_FLAG 0x1F
#define HIGH_TAG_END 0x80
#define LONG_LENGHT_FLAG 0x80

/*
//bit[7:6]: universal(00)¡¢application(01)¡¢context-specific(10)ºÍprivate(11)
#define UNIVERSAL 0b00
#define APPLICATION 0b01
#define CONTEXT_SPECIFIC 0b10
#define PRIVATE 0b11
static void tag_type(uint8_t tag)
{
	switch((tag >> 6) && 0x3) {
		case UNIVERSAL:
			dbg_printf(PRN_RES,"universal\n");
			break;
		case APPLICATION:
			dbg_printf(PRN_RES,"application\n");
			break;
		case CONTEXT_SPECIFIC:
			dbg_printf(PRN_RES,"context-specific\n");
			break;
		case PRIVATE:
			dbg_printf(PRN_RES,"private\n");
			break;
		default:
			dbg_printf(PRN_RES,"unknow tag type\n");
			break;
	}
}
*/

/*
ASN1_SEQUENCE(BootSignature) = {
    ASN1_SIMPLE(BootSignature, formatVersion, ASN1_INTEGER),
    ASN1_SIMPLE(BootSignature, certificate, X509),
    ASN1_SIMPLE(BootSignature, algorithmIdentifier, X509_ALGOR),
    ASN1_SIMPLE(BootSignature, authenticatedAttributes, AuthAttrs),
    ASN1_SIMPLE(BootSignature, signature, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(BootSignature)
*/

static uint32_t caculate_tag_len(uint8_t * buff)
{
	uint32_t i = 0, tag_len = 0;
	uint8_t * p = buff;
	if((p[i] & HIGH_TAG_FLAG) == HIGH_TAG_FLAG) {
#ifdef ASN1_DEBUG
		dbg_printf(PRN_ERR,"high %x\n", p[i]);
#endif
		tag_len = 1;
		for(i = 1; ;i++) {
			tag_len++;
			if((p[i] >> 7) & 0x1)
				break;
		}
	} else {// low tag
#ifdef ASN1_DEBUG
		dbg_printf(PRN_ERR,"low %x\n", p[i]);
#endif
		tag_len = 1;
	}
#ifdef ASN1_DEBUG
	dbg_printf(PRN_ERR,"tag_len = %d\n", tag_len);
#endif
	return tag_len;
}

static uint32_t caculate_len_data_len(uint8_t * buff, uint64_t * data_len)
{
	uint32_t i = 0, len_len = 0, len = 0;
	uint64_t d_len = 0;
	uint8_t * p = buff;

	if((p[i] & LONG_LENGHT_FLAG) == LONG_LENGHT_FLAG) {
#ifdef ASN1_DEBUG
		dbg_printf(PRN_ERR,"long %x\n", p[i]);
#endif
		//long
		len = p[i] - LONG_LENGHT_FLAG;
		len_len = len + 1;
		for(i = 1; i <= len; i++) {
			d_len = d_len * 0x100 + p[i];
		}
		*data_len = d_len;
	} else {
		//short
#ifdef ASN1_DEBUG
		dbg_printf(PRN_ERR,"short %x\n", p[i]);
#endif
		len_len = 1;
		*data_len = p[i];
	}
#ifdef ASN1_DEBUG
	dbg_printf(PRN_ERR,"len_len = %d, data_len = %d\n", len_len, (uint32_t)(*data_len));
#endif
	return len_len;
}


// index is start from 0
int simple_asn1_parse(uint8_t * buff, asn1_info * info, uint32_t index)
{
	int ret = 1;
	uint8_t * p = buff;
	uint32_t i = 0, j = 0, k = 0;
	asn1_info tmp;
	uint8_t * p1 = NULL;

	tmp.start = buff;
	//if basic data type, return directly
	tmp.data_type = (p[i] >> 5) & 0x1;
	if(tmp.data_type == 0) {
		// simple data type
		dbg_printf(PRN_ERR,"it's not a complex object!\n");
		return ret;
	}

	//1. tag
	//high tag
	tmp.tag_len = caculate_tag_len(&buff[i]);
	i += tmp.tag_len;

	//2. data length
	tmp.len_len = caculate_len_data_len(&buff[i], &tmp.data_len);
	i += tmp.len_len;

	p1 = tmp.data = &buff[i];

	for(j = 0; j < tmp.data_len;) {
		info->start = &p1[j];
		info->tag_len = caculate_tag_len(&p1[j]);
		j += info->tag_len;
		info->len_len = caculate_len_data_len(&p1[j], &info->data_len);
		j += info->len_len;
		info->data = &p1[j];
		if(k == index)
			return 0;
		k++;
		j += info->data_len;
	}

	if(k != index) {
		dbg_printf(PRN_ERR,"can't find the %d object!\n", index);
		return ret;
	}

	return 0;

}
