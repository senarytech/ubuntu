/*
 * INTERNAL USE ONLY
 *
 * Unpublished Work Copyright © 2013-2017 Synaptics Incorporated.
 * All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated (“Synaptics”). The holder of this file shall treat all
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
 */

///////////////////////////////////////////////////////////////////////////////
//! \file    gencrypto_cmd.h
//! \brief   Generic Crypto TZ Entry commands
//! \author  Girish Shetty
//! \date    Dec 2015
///////////////////////////////////////////////////////////////////////////////

#ifndef __GENERICCRYPTO_CMD_H__
#define __GENERICCRYPTO_CMD_H__

#define TA_GENERICCRYPTO_UUID {0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x03, 0xf8}}

enum {
	TZ_ALGO_INVALID,
	//RSA Keystore API
	TZ_ALGO_RSA_LOADKEY,
	TZ_ALGO_RSA_UNLOADKEY,
	TZ_ALGO_RSA_SIGN,

	//Generic Crypto (KeyMaster) API
	TZ_ALGO_GENERATE_SECURE_KEY,
	TZ_ALGO_GENERATE_SECURE_RSAKEY,
	TZ_ALGO_GENERATE_SECURE_ECKEY,
	TZ_ALGO_IMPORT_SECURE_KEY,
	TZ_ALGO_EXPORT_SECURE_KEY,
	TZ_ALGO_GENERATE_RANDOM,
	TZ_ALGO_BLOCKCIPHER,
	TZ_ALGO_AESGCM_CIPHER_INIT,
	TZ_ALGO_AESGCM_CIPHER_UPDATE,
	TZ_ALGO_AESGCM_CIPHER_FINAL,
	TZ_ALGO_AESGCM_CIPHER_CLEANUP,
	TZ_ALGO_HASH_INIT,
	TZ_ALGO_HASH_UPDATE,
	TZ_ALGO_HASH_FINAL,
	TZ_ALGO_RSA_OPERATION,
	TZ_ALGO_EC_OPERATION,
	TZ_ALGO_ATTEST_KEY,
	TZ_ALGO_CONFIGURE,
	TZ_ALGO_SET_VERSIONS,
	TZ_ALGO_ENCRYPT_KEYDATA,
	TZ_ALGO_DECRYPT_KEYDATA,
	TZ_ALGO_GET_HMAC_SHARING_PARAMS,
	TZ_ALGO_COMPUTE_SHARED_HMAC,
	TZ_ALGO_VERIFY_AUTHORIZATION,
	TZ_ALGO_SET_VERSIONS_V4,
	TZ_ALGO_SET_ROOTOFTRUST_V4,
	TZ_ALGO_CMD_MAX,
};

#endif
