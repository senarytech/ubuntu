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
#include "drm.h"
#include "drm_reg.h"
#include "io.h"
#ifdef CONFIG_TRUSTZONE
#include "tz_nw_boot.h"
#endif
#ifdef CONFIG_DCACHE
#include "cache.h"
#endif
#include "debug.h"

#ifdef RSA2048_SHA256
#include "string.h"
#include "sha2.h"
#include "timer.h"
#endif

extern void flush_dcache_all(void);

/* Encrypted Image Structure
 *
 * +---------------------------+
 * |    128 bytes header       |
 * +---------------------------+
 * | 64 or 128 bytes signature |
 * +---------------------------+
 * |                           |
 * |      encrypted data       |
 * |                           |
 * +---------------------------+
 *
 */

static inline unsigned int drm_decrypted_size(const void *encrypted_data)
{
	/*
	 * 69: signature length, most is 128 bytes (1024 bits)
	 * 124-127: length of decrypted data (including signature)
	 */
	return ((const unsigned int *)encrypted_data)[31]
		- ((const unsigned char *)encrypted_data)[69];
}

static inline unsigned int drm_encrypted_size(const void *encrypted_data)
{
	unsigned int enc_size;

	enc_size = ((const unsigned int *)encrypted_data)[31];
	enc_size = (enc_size + 15) & 0xfffffff0;	/* 16 bytes aligned */
	enc_size += 128;	/* 128 bytes header */

	return enc_size;
}

int drm_verify(const void *src, void *dst, unsigned int size, int is_verify)
{
	unsigned int reg;
	unsigned int flag;
	unsigned int enc_size, dec_size;

	/* src & dst address must be 32bytes aligned */
	if (((unsigned)src & 31) || ((unsigned)dst & 31))
		return -1;

	/* get the decrypted size first to avoid can't get it after dst
	 * overlaps src
	 */
	enc_size = drm_encrypted_size(src);
	dec_size = drm_decrypted_size(src);

	/* make sure the input data is enough to decrypt the image */
	if (enc_size > size)
		return -2;

	if(is_verify) {
		/*
		 * check the input key index (byte 64) first, must NOT be 0x00 or 0xFF
		 */
		reg = *((const unsigned char *)src + 64);
		if (0x00 == reg || 0xff == reg)
			return -3;
	}

	/*
	 * waiting for FIGO ready
	 */
	do {
		reg = readl(DRM_SECURITY_STATUS_REG);
		flag = reg & MSK32SECSTATUS_CFG_flag;
	} while (flag == SECSTATUS_CFG_flag_DISABLED);

	if (flag == SECSTATUS_CFG_flag_FAILED)
		return -4;

	/*
	 * check command reg
	 */
	reg = readl(DRM_ROM_CMD_STAT_REG);
	if (reg & MSK32DRMROM_CMD_STAT_en)
		return -5;

#ifdef CONFIG_DCACHE
	//flush_dcache_range(src, src + size);
	flush_dcache_all();
#endif /* CONFIG_DCACHE */

	/*
	 * command type is ARM image
	 */
	writel(DRMROM_CMD_TYPE_LD_ARMIMG | MSK32DRMROM_CMD_CMD_CFG_nonce,
			DRM_ROM_CMD_CFG_REG);

	/*
	 * clear CRC value
	 */
	writel(0, DRM_ROM_CMD_CRC_REG);

	/*
	 * set image src addr, dst addr, and size
	 */
	writel(size, DRM_ROM_CMD_IMG_SIZE_REG);
	writel((unsigned)src, DRM_ROM_CMD_IMG_SRC_ADDR_REG);
	writel((unsigned)dst, DRM_ROM_CMD_IMG_DST_ADDR_REG);

	/*
	 * Kick off FIGO
	 */
	writel(DRM_ROM_CMD_STAT_EN, DRM_ROM_CMD_STAT_REG);

	/*
	 * Waiting for FIGO done
	 */
	do {
		reg = readl(DRM_ROM_CMD_STAT_REG);
	} while (reg & MSK32DRMROM_CMD_STAT_en);

	/*
	 * Check type
	 */
	reg = readl(DRM_ROM_CMD_RESPONSE_CFG_REG);
	if ((reg & MSK32DRMROM_CMD_RSP_CFG_tag) != DRMROM_CMD_TYPE_LD_ARMIMG)
		return -11;

	/*
	 * Check verification result
	 */
	reg = readl(DRM_ROM_CMD_RESPONSE_ERR_REG);
	if ((reg & MSK32DRMROM_CMD_RSP_DAT1_error) != DRM_ROM_CMD_RESPONSE_SUCCEED)
		return -12;

#ifdef CONFIG_DCACHE
	//invalidate_dcache_range(dst, dst + size);
	flush_dcache_all();
#endif /* CONFIG_DCACHE */

	return dec_size;
}

#ifdef RSA2048_SHA256
#define ENCRYPTED_HASH_SIZE 1024
#define IMAGE_OFFSET 2048

static void sha256_caculate(unsigned char * data, unsigned int size, unsigned char * digest)
{
	SHA256_CTX context;

	dbg_printf(PRN_RES, "sha256 data size = %d\n", size);

	SHA256_Init(&context);

	SHA256_Update(&context, data, size);

	SHA256_Final(digest, &context);
}

int sha256_verify(const void * src, void * dst)
{
	int ret = 0, i = 0;
	unsigned char * img_src = (unsigned char *)src + IMAGE_OFFSET;
	unsigned char hash_value[SHA256_DIGEST_LENGTH];
	unsigned char hash_value_local[SHA256_DIGEST_LENGTH];
	unsigned int enc_img_size;

	//verify head1 to get hash
	ret = drm_verify(src, dst, ENCRYPTED_HASH_SIZE, 1);
	if(ret <= 0)
		return ret;

	for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		hash_value[i] = ((unsigned char *)dst)[i];
	}

	dbg_printf(PRN_RES, "[%6d] start sha256\n", get_time_ms());
	//get the size of encrypted image
	enc_img_size = ((unsigned int *)img_src)[31];
	enc_img_size = (enc_img_size + 15) & 0xfffffff0; //16 bytes aligned
	enc_img_size += 128;

	//caculate the hash of encrypted image
	sha256_caculate((img_src + 128), enc_img_size - 128, hash_value_local);

	dbg_printf(PRN_RES, "[%6d] end sha256\n", get_time_ms());

	if(strncmp((char *)hash_value, (char *)hash_value_local, 32)) {
		dbg_printf(PRN_RES, "image verify fail!!!\n");
		dbg_printf(PRN_RES, "remote:");
		for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
			dbg_printf(PRN_RES, "%02x", hash_value[i]);
		}
		dbg_printf(PRN_RES, "\n");

		dbg_printf(PRN_RES, "local :");
		for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
			dbg_printf(PRN_RES, "%02x", hash_value_local[i]);
		}
		dbg_printf(PRN_RES, "\n");
		return -13;
	}

	//if hash is matched, decrypted image and copy
	return drm_verify(img_src, dst, enc_img_size, 0);
}
#endif

int VerifyImage(const void *src, unsigned int size, void *dst)
{
#ifdef CLEAR_BOOTFLOW
	if(src != dst) {
		//can only support src==dst case. we just do like this
		return -1;
	}
	return 0;
#else
	int ret = 0;
#ifdef CONFIG_TRUSTZONE
	flush_dcache_range(dst, (void *)(((char *)dst) + size));
	ret = tz_nw_verify_image(4, src, size, dst, size);
	invalidate_dcache_range(dst, (void *)(((char *)dst) + size));
#else
#ifdef RSA1024_SHA1
	ret = drm_verify(src, dst, size, 1);
#else //RSA2048_SHA256
	ret = sha256_verify(src, dst);
#endif
#endif
	if (ret > 0)
		ret = 0;
	return ret;
#endif
}
