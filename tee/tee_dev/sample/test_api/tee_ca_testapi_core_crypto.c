/*
 * Copyright (c) 2013-2014 Marvell International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * licensing terms.
 *
 * If you received this File from Marvell and you have entered into a
 * commercial license agreement (a "Commercial License") with Marvell, the
 * File is licensed to you under the terms of the applicable Commercial
 * License.
 */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "tee_testapi.h"
#include "tee_client_api.h"
#include "testapi_cmd.h"
#include "tee_ca_testapi_util.h"
#include "string.h"
#include "memory.h"
#include "log.h"
#include "config.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>

static const TEEC_UUID TATest_UUID = TATEST_UUID;
#define SIGNATURE_LEN					(256)
#define DIGEST_MAX_LEN					32


void GenerateMD5Digest(char *dataorigin, char *digest)
{
	uint32_t			len;

	RAND_pseudo_bytes(dataorigin, 100);
	dataorigin[99] = '\0';

	len = strlen((const char*)dataorigin);

	MD5(dataorigin,len,digest);

	return;

}

void GenerateSHA1Digest(char *dataorigin, char *digest)
{
	uint32_t			len;

	RAND_pseudo_bytes(dataorigin, 100);
	dataorigin[99] = '\0';

	len = strlen((const char*)dataorigin);

	SHA1(dataorigin,len,digest);

	return;

}

void GenerateSHA256Digest(char *dataorigin, char *digest)
{
	uint32_t			len;

	RAND_pseudo_bytes(dataorigin, 100);
	dataorigin[99] = '\0';

	len = strlen((const char*)dataorigin);

	SHA256(dataorigin,len,digest);

	return;

}

TEST_Result TestCoreCrypto(struct MODULE_Cfg* test_cfg)
{

	TEEC_Session 		session;
	TEEC_Context 		context;
	TEEC_SharedMemory 	shm_common;
	TEEC_SharedMemory	shm_digest;
	TEEC_SharedMemory	shm_signature;
	TEEC_Operation 		operation;
	TEEC_Result 		result;
	int 				case_index=0;
	int					cmdId = TEST_CMD_CORE_CRYPTO;

	char				dataorigin[100];
	char				digest[DIGEST_MAX_LEN];
	char  				signature[SIGNATURE_LEN];
	int 				i;
	printf("*******enter internal TestCoreCrypto*********\n");

	/* first, initial it*/
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS) {
		error("TestCoreCrypto init context fail, can't continue....ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("TestCoreCrypto open session fail, can't continue....ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup1;
	}

	/*initialize the shm_common of test_cfg*/
	shm_common.size = sizeof(*test_cfg);

	shm_common.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	result = TEEC_AllocateSharedMemory(
			&context,
			&shm_common);
	if (result != TEEC_SUCCESS) {
		error("TestCoreCrypto TEEC alloc shm fail, can't continue....ret=0x%08x\n",
				result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup2;
	}

	memcpy(shm_common.buffer, test_cfg, sizeof(*test_cfg));


	/*initialize the shm_digest of digest*/
	switch(test_cfg->type)
		{
			case TEST_TEE_CORE_CRYPTO_PKCS1_V1_5_MD5:

				GenerateMD5Digest(dataorigin, digest);

				shm_digest.size   = MD5_DIGEST_LENGTH;
				shm_digest.flags  = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

				result = TEEC_AllocateSharedMemory(
						&context,
						&shm_digest);

				if (result != TEEC_SUCCESS){
					goto cleanup3;
				}
				memcpy(shm_digest.buffer, digest, MD5_DIGEST_LENGTH);

				break;

			case TEST_TEE_CORE_CRYPTO_PKCS1_V1_5_SHA1:

				GenerateSHA1Digest(dataorigin, digest);

				shm_digest.size   = SHA_DIGEST_LENGTH;
				shm_digest.flags  = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

				result = TEEC_AllocateSharedMemory(
						&context,
						&shm_digest);

				if (result != TEEC_SUCCESS){
					goto cleanup3;
				}
				memcpy(shm_digest.buffer, digest, SHA_DIGEST_LENGTH);
				break;

			case TEST_TEE_CORE_CRYPTO_PKCS1_V1_5_SHA256:

				GenerateSHA256Digest(dataorigin, digest);

				shm_digest.size   = SHA256_DIGEST_LENGTH;
				shm_digest.flags  = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

				result = TEEC_AllocateSharedMemory(
						&context,
						&shm_digest);

				if (result != TEEC_SUCCESS){
					goto cleanup3;
				}
				memcpy(shm_digest.buffer, digest, SHA256_DIGEST_LENGTH);
				break;

			default:
				break;

	}

	/*initialize the shm of signature*/

	shm_signature.size   = SIGNATURE_LEN;
	shm_signature.flags  = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	result = TEEC_AllocateSharedMemory(
			&context,
			&shm_signature);

	if (result != TEEC_SUCCESS){
		goto cleanup4;
	}

	memset(shm_signature.buffer, 0, SHA256_DIGEST_LENGTH);


	/*initialize the operation*/
	memset(&operation, 0, sizeof(operation));

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_WHOLE,
			TEEC_MEMREF_WHOLE,
			TEEC_MEMREF_WHOLE,
			TEEC_NONE);

	operation.params[0].memref.parent = &shm_common;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;

	operation.params[1].memref.parent = &shm_digest;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;

	operation.params[2].memref.parent = &shm_signature;
	operation.params[2].memref.offset = 0;
	operation.params[2].memref.size = 0;

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);

	if (result != TEEC_SUCCESS) {
		error("TestCoreCrypto TEEC invoke cmd fail. ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup5;
	}
	memcpy(test_cfg, shm_common.buffer, sizeof(*test_cfg));

	memcpy(signature, shm_signature.buffer, SIGNATURE_LEN);

	printf("\n\n\nthe signature len is %d\n\n\n", operation.params[2].memref.size);
	for(i=0;i<SIGNATURE_LEN;i++){
		printf("%x",signature[i]);
	}
	/* end test*/

cleanup5:
	TEEC_ReleaseSharedMemory(&shm_signature);
cleanup4:
	TEEC_ReleaseSharedMemory(&shm_digest);
cleanup3:
	TEEC_ReleaseSharedMemory(&shm_common);
cleanup2:
	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
cleanup:
	return result;
}

void TestCoreCryptoAPI(struct MODULE_Cfg *test_cfg)
{
	TEST_Result	result;
	int			test_type,test_times;
	bool		test_on;
	char 		*test_name;
	int 		i;
	int 		test_index = 0;

	printf("*******enter TestCoreCryptoAPI*********\n");

	while(1) {

		test_name = test_cfg[test_index].name;
		test_type = test_cfg[test_index].type;
		test_times = test_cfg[test_index].times;
		test_on = test_cfg[test_index].enable;

		printf("TestCoreCryptoAPI name %s, type is %d, times is %d, enable is %d\n",
				test_name, test_type, test_times, test_on);

		/* is tail? */
		if(test_type == -1)
			break;

		if(!test_on){
			test_index++;
			continue;
		}

		for(i = 0; i < test_times;i++) {
			result = TestCoreCrypto(&test_cfg[test_index]);
		}

		show_result(&test_cfg[test_index], i);
		test_index++;

	}
	return;
}
