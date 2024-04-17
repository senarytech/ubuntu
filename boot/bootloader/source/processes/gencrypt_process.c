/*******************************************************************************
 *                 2017 Synaptics Incorporated. All Rights Reserved            *
 * THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF Synaptics.                   *
 * NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT *
 * OF Synaptics OR ANY THIRD PARTY. Synaptics RESERVES THE RIGHT AT ITS SOLE   *
 * DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO Synaptics.  *
 * THIS CODE IS PROVIDED "AS IS". Synaptics MAKES NO WARRANTIES, EXPRESSED,    *
 * IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.  *
 *                                                                             *
 *******************************************************************************/
#include "tee_client_api.h"
#include "debug.h"
#include "image_chunk.h"
#include "gencrypto_cmd.h"


unsigned int gOs_version = 0, gOs_lvl = 0;
uint64_t gBoot_lvl = 0, gSystem_lvl = 0, gVendor_lvl = 0;
bool allow_verification_error = false;

void GenCrypto_Initialize(unsigned , unsigned);
void GenCrypto_Initialize_V4(unsigned os_ver, uint64_t boot_lvl,
				uint64_t vendor_lvl, uint64_t system_lvl);

extern int is_ta_registered(unsigned int chunk_id);

static const TEEC_UUID TA_GC_UUID = TA_GENERICCRYPTO_UUID;
static TEEC_Session teeSession;

void GenCrypto_Initialize(unsigned os_ver, unsigned os_lvl)
{
    TEEC_Result result;
    TEEC_Operation operation;
    result = TEEC_OpenSession( &teeSession, &TA_GC_UUID, TEEC_LOGIN_USER,
            NULL,    /* No connection data needed for TEEC_LOGIN_USER. */
            NULL,    /* No payload, and do not want cancellation. */
            NULL);
    if (result != TEEC_SUCCESS) {
        dbg_printf(PRN_ERR, "%s: TEEC_OpenSession failed with 0x%08x\n", __FUNCTION__, result);
        return;
    }
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);
    operation.params[0].value.a = os_ver;
    operation.params[0].value.b = os_lvl;

    result = TEEC_InvokeCommand(&teeSession,TZ_ALGO_SET_VERSIONS,&operation);
    if (result != TEEC_SUCCESS) {
        dbg_printf(PRN_ERR, "%s: TEEC_InvokeCommand failed with 0x%08x\n", __FUNCTION__, result);
    }
}

#ifdef CONFIG_VB20
#include "string.h"
#include "bootloader_header.h"
#include "vb_state.h"
extern char * get_avb_kernel_commandline(void);

static char * pubkey = NULL;
static int pubkey_size = 0;
static char * boothash = NULL;

void set_public_key(const char * buf, int size)
{
	if(size > 0) {
		pubkey = malloc_ion_cacheable(size);
		memcpy(pubkey, buf, size);
		pubkey_size = size;
	}
}

static char * find_value_through_key(char* str, char * key)
{
	int i = 0;
	char * p = str;
	int str_size = strlen(str);
	int key_size = strlen(key);

	for(i = 0; i < (str_size - key_size); i++, p++) {
		if(strncmp(p, key, key_size) == 0) {
			return (p + key_size + 1);
		}
	}

	return NULL;
}

static int parse_algorithm_type(char * str)
{
	char * p = find_value_through_key(str, "androidboot.vbmeta.hash_alg");
	if(p != NULL) {
		if(strncmp(p, "sha256", 6) == 0)
			return 0;
		if(strncmp(p, "sha512", 6) == 0)
			return 1;
	}

	return -1;
}

#define DIG_CONVERT(x) (x >= 'a'? (x - 'a' + 10) : (x - '0'))

static void hex_2_bin(char * str, char * bin, int size)
{
	int i = 0;
	for(i = 0; i < size; i++) {
		bin[i] = (DIG_CONVERT(str[i * 2]) << 4) + DIG_CONVERT(str[i * 2 + 1]);
	}
}

static char * parse_boothash(char * str, int size)
{
	char * p = find_value_through_key(str, "androidboot.vbmeta.digest");
	if((p != NULL) && (size > 0)) {
		boothash = malloc_ion_cacheable(size);
		hex_2_bin(p, boothash, size);
	}

	return boothash;
}
#endif

void GenCrypto_Initialize_V4(unsigned os_ver, uint64_t boot_lvl,
				uint64_t vendor_lvl, uint64_t system_lvl)
{
	TEEC_Result result;
	TEEC_Operation operation;
	result = TEEC_OpenSession( &teeSession, &TA_GC_UUID, TEEC_LOGIN_USER,
			NULL,    /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,    /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		dbg_printf(PRN_ERR, "%s: TEEC_OpenSession failed with 0x%08x\n", __FUNCTION__, result);
		return;
	}
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
							TEEC_VALUE_INPUT,
							TEEC_VALUE_INPUT,
							TEEC_VALUE_INPUT);
	if (allow_verification_error && vendor_lvl == 0 && system_lvl == 0) {
		// This is for the case when device is unlocked
		os_ver = 0;
		boot_lvl = 0;
		dbg_printf(PRN_INFO, "DEBUG: allow verification error with vendor and system lvl 0\n");
	}
	if (!allow_verification_error && vendor_lvl == 0) {
		// On Android Q launching device, we remvoed to chained partition to vendor. This syncs with mainline
		// due to fastboot host side tool doesn't support flash vbmeta_vendor.
		// Our solution set the boot anti rollback index to vendor SPL, so we could use it here.
		vendor_lvl = boot_lvl;
	}
	operation.params[0].value.a = os_ver;
	operation.params[1].value.a = (unsigned int)((boot_lvl >> 32) & 0xFFFFFFFF);
	operation.params[1].value.b = (unsigned int)(boot_lvl & 0xFFFFFFFF);
	operation.params[2].value.a = (unsigned int)((vendor_lvl >> 32) & 0xFFFFFFFF);
	operation.params[2].value.b = (unsigned int)(vendor_lvl & 0xFFFFFFFF);
	operation.params[3].value.a = (unsigned int)((system_lvl >> 32) & 0xFFFFFFFF);
	operation.params[3].value.b = (unsigned int)(system_lvl & 0xFFFFFFFF);

	result = TEEC_InvokeCommand(&teeSession, TZ_ALGO_SET_VERSIONS_V4, &operation);
	if (result != TEEC_SUCCESS) {
		dbg_printf(PRN_ERR, "%s: TEEC_InvokeCommand failed with 0x%08x\n", __FUNCTION__, result);
	}

#ifdef CONFIG_VB20
	TEEC_SharedMemory tmp1;
	TEEC_SharedMemory tmp2;
	if (is_dev_lock()) {
		operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
								TEEC_MEMREF_PARTIAL_INPUT,
								TEEC_MEMREF_PARTIAL_INPUT,
								TEEC_VALUE_INPUT);
	} else {
		operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
								TEEC_NONE,
								TEEC_MEMREF_PARTIAL_INPUT,
								TEEC_VALUE_INPUT);
	}

	// 0: unlock; 1: lock
	operation.params[0].value.a = is_dev_lock();
	// 0: yellow; 1: orange; 2: red; 3: green;
	operation.params[0].value.b = get_verify_state();

	if (is_dev_lock()) {
		tmp1.buffer = pubkey;
		tmp1.phyAddr = pubkey;
		tmp1.size = pubkey_size;
		tmp1.flags = TEEC_MEM_INPUT;
		tmp1.allocated = true;
		tmp1.operationCount = 0;
		operation.params[1].memref.parent = &tmp1;
		operation.params[1].memref.size = pubkey_size;
		operation.params[1].memref.offset = 0;
	}

	// 0: sha256; 1: sha512; -1: invalid
	operation.params[3].value.a = parse_algorithm_type(get_avb_kernel_commandline());

	if(operation.params[3].value.a >= 0) {
		// 32 (sha256); 64 (sha512); 0 (invalid)
		operation.params[2].memref.size = 32 * (operation.params[3].value.a + 1);
	}
	tmp2.buffer = parse_boothash(get_avb_kernel_commandline(), operation.params[2].tmpref.size);
	tmp2.phyAddr = tmp2.buffer;
	tmp2.size = operation.params[2].memref.size;
	tmp2.flags = TEEC_MEM_INPUT;
	tmp2.allocated = true;
	tmp2.operationCount = 0;
	operation.params[2].memref.parent = &tmp2;
	operation.params[2].memref.offset = 0;

	result = TEEC_InvokeCommand(&teeSession, TZ_ALGO_SET_ROOTOFTRUST_V4, &operation);
	if (result != TEEC_SUCCESS) {
		dbg_printf(PRN_ERR, "%s(%d): TEEC_InvokeCommand failed with 0x%08x\n", __FUNCTION__, __LINE__, result);
	}
#endif
}

void Setup_KeyMaster(void)
{
	if(is_ta_registered(IMAGE_CHUNK_ID_KEYMASTER)) {
#ifndef TA_KEYMASTER_V4
		GenCrypto_Initialize(gOs_version,gOs_lvl);
#else
		GenCrypto_Initialize_V4(gOs_version, gBoot_lvl,
				gVendor_lvl, gSystem_lvl);
#endif
	}
}
