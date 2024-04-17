/*
 * Copyright (c) 2017-2022 Synaptics International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Synaptics
 * International Ltd. and/or its affiliates ("Synaptics") under the following
 * licensing terms.
 *
 * If you received this File from Synaptics and you have entered into a
 * commercial license agreement (a "Commercial License") with Synaptics, the
 * File is licensed to you under the terms of the applicable Commercial
 * License.
 */

#include "tee_internal_api.h"
#include "tee_sample_cmd.h"
#include "tee_log.h"

#define ENABLE_DEBUG 1

TEE_Result TA_EXPORT TA_CreateEntryPoint(void)
{
	TEE_LOGI("\n");
	return TEE_SUCCESS;
}

void TA_EXPORT TA_DestroyEntryPoint(void)
{
	TEE_LOGI("\n");
}

TEE_Result TA_EXPORT TA_OpenSessionEntryPoint(
	uint32_t		paramTypes,
	TEE_Param		params[4],
	void**			sessionContext)
{
	TEE_LOGI("\n");
	return TEE_SUCCESS;
}

void TA_EXPORT TA_CloseSessionEntryPoint(void*  sessionContext)
{
	TEE_LOGI("\n");
}

void print_hex_data(const char *tag, uint8_t *dump_data, uint32_t dump_data_len) {
    int i;
    printf("------------------------------\n");
    printf(":item %s\n", tag);
    printf(":size %d\n", dump_data_len);
    for(i=0; i < dump_data_len; i++) {
        if(0 == (i%16)) { printf("\n");}
        printf(" 0x%2X", dump_data[i]);
    }
    printf("\n");
    printf("------------------------------\n");
}

void print_char_data(const uint8_t *tag, uint8_t *dump_data, uint32_t dump_data_len) {
    int i;
    printf("------------------------------\n");
    printf(":item %s\n", tag);
    printf(":size %d\n", dump_data_len);
    for(i=0; i < dump_data_len; i++) {
        printf("%c", dump_data[i]);
    }
    printf("\n");
    printf("\n------------------------------\n");
}


static TEE_Result CustBlob_VerifySecureStore(TEE_Param params[4])
{
    TEE_Result result = TEE_SUCCESS;
    uint8_t *sec_store_buff = NULL;
    uint8_t *verification_data_buff = NULL;
    uint32_t sec_store_size = 0;
    uint32_t clear_data_size = 0;
    uint32_t shmClear = 0;
    uint32_t shmDest = 0;

    sec_store_buff = (uint8_t *)params[0].memref.buffer;
    sec_store_size = params[0].memref.size;


    if((NULL == sec_store_buff) || (0 == sec_store_size)) {
        printf("param error:\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    void *va = NULL;
    result = TEE_AllocateSharedMemory2(sec_store_size, &shmClear, &va);
    if(TEE_SUCCESS != result) {
        printf("error allocating shared memory.\n");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    memcpy(va, sec_store_buff, sec_store_size);

    void *va_dest = NULL;
    result = TEE_AllocateSharedMemory2(sec_store_size, &shmDest, &va_dest);
    if(TEE_SUCCESS != result) {
        printf("error allocating shared memory.\n");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    memset(va_dest, 0x00, sec_store_size);

#if ENABLE_DEBUG
    print_hex_data("cusBlob", va, sec_store_size);
#endif
    result = TEE_GetCustomSecureStoreData(va, sec_store_size, va_dest, sec_store_size, (void *)&clear_data_size);
    if(TEE_SUCCESS != result) {
        printf("unable to retreive data from secure store.\n");
        return result;
    }

    if(params[1].memref.size != 0) {
        printf("verification data availabe...checking\n");
        if(params[1].memref.size != clear_data_size) {
            printf("clear data size doesn't match with expected.. %d \n", clear_data_size);
            return result;
        }
        verification_data_buff = (uint8_t *)params[1].memref.buffer;
        if (NULL == verification_data_buff) {
            printf("no verification data provided..\n");
        } else {
            if(0 == memcmp(va_dest, verification_data_buff, clear_data_size)) {
                printf("success: decoded data matches with the expected .............\n");
            } else {
                printf("fail: decoded data doesn't match expected..............\n");
            }
        }
    } else {
        print_hex_data("clear CustBlob data", va_dest, clear_data_size);
        printf("\n");
    }

    return result;
}

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result result = TEE_SUCCESS;
	TEE_LOGI("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch(commandID) {
	case CUSTBLOB_VERIFY_SECURESTORE:
		result = CustBlob_VerifySecureStore(params);
		break;
	default:
		break;
	}
	return result;
}

TA_DEFINE_BEGIN
	.uuid			= TASAMPLE_UUID,
	.name			= "SampleTA",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
