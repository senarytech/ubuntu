#include "tee_client_api.h"
#include <string.h>
/* ============================================================================
   Store the TEE application UUID in non-volatile memory (code ROM).
   ============================================================================ */
static const TEEC_UUID cryptoTEEApp =
{
	0x3E93632E, 0xA710, 0x469E,
	{ 0xAC, 0xC8, 0x5E, 0xDF, 0x8C, 0x85, 0x90, 0xE1 }
};
/* ============================================================================
   Definitions of the Trusted Application Command IDs
   ============================================================================ */
#define CMD_ENCRYPT_INIT 1
#define CMD_ENCRYPT_UPDATE 2
#define CMD_ENCRYPT_FINAL  3
#define CMD_DIGEST_INIT 4
#define CMD_DIGEST_UPDATE 5
#define CMD_DIGEST_FINAL 6
/* ============================================================================
   Implement our library function where the buffers of memory are pre-allocated
   by the calling entity. This is a common paradigm when interfacing with other  25
   libraries provided by other providers.
   ============================================================================ */
TEEC_Result libraryFunction(
		uint8_t const * inputBuffer,
		uint32_t inputSize,
		uint8_t* outputBuffer,
		uint32_t outputSize,
		uint8_t* digestBuffer
		)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result;
	TEEC_SharedMemory commsSM;
	TEEC_SharedMemory inputSM;
	TEEC_SharedMemory outputSM;
	uint8_t* ivPtr;
	/* ========================================================================
	   [1] Connect to TEE
	   ======================================================================== */
	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup1;
	}
	/* ========================================================================
	   [2] Open session with TEE application
	   ======================================================================== */
	/* Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			&session,
			&cryptoTEEApp,
			TEEC_LOGIN_USER,
			NULL,    /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,  /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup2;
	}
	/* ========================================================================
	   [3] Initialize the Shared Memory buffers
	   ======================================================================== */
	/* [a] Communications buffer. */
	commsSM.size = 20;
	commsSM.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(
			&context,
			&commsSM);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup3;
	}
	/* [b] Bulk input buffer. */
	inputSM.size = inputSize;
	inputSM.flags = TEEC_MEM_INPUT;
	/* Use TEE Client API to register the underlying memory buffer. */
	inputSM.buffer = (uint8_t*)inputBuffer;
	result = TEEC_RegisterSharedMemory(
			&context,
			&inputSM);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup4;
	}
	/* [c] Bulk output buffer (also input for digest). */
	outputSM.size = outputSize;
	outputSM.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	outputSM.buffer = outputBuffer;
	/* Use TEE Client API to register the underlying memory buffer. */
	result = TEEC_RegisterSharedMemory(
			&context,
			&outputSM);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup5;
	}
	/* ========================================================================
	   [4] Perform cryptographic operation initialization commands
	   ======================================================================== */
	/* [a] Start the encrypt operation within the TEE application. */
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_MEMREF_PARTIAL_INPUT,
			TEEC_NONE,
			TEEC_NONE);
	/* Write key ID (example uses key ID = 1) in parameter #1 */
	operation.params[0].value.a = 1;
	operation.params[1].memref.parent = &commsSM;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 16;
	/* Write IV (example uses an IV of all zeros) in to Memory buffer. */
	ivPtr = (uint8_t*)commsSM.buffer;
	memset(ivPtr, 0, 16);
	/* Start the encrypt operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			CMD_ENCRYPT_INIT,
			&operation,
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup6;
	}
	/* [b] Start the digest operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			CMD_DIGEST_INIT,
			NULL,
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup6;
	}
	/* ========================================================================
	   [5] Perform the cryptographic update commands
	   ======================================================================== */
	/* [a] Start the encrypt operation within the TEE application. */
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_WHOLE,
			TEEC_MEMREF_PARTIAL_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].memref.parent = &inputSM;
	/* Note that the other fields of operation.params[0].memref need not be
	   initialized because the parameter type is TEEC_MEMREF_WHOLE */
	/* Note: Even though we share the entire block we do so with less flags, so
	 * fallback on the TEEC_MEMREF_PARTIAL method. */
	operation.params[1].memref.parent = &outputSM;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = outputSize;
	/* Start the encrypt operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			CMD_ENCRYPT_UPDATE,
			&operation,
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup6;
	}
	/* [b] Start the digest operation within the TEE application. */
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_PARTIAL_INPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);
	/* Note: we use the updated size in the MemRef output by the encryption. */
	operation.params[0].memref.parent = &outputSM;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = operation.params[1].memref.size;
	/* Start the digest operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			CMD_DIGEST_UPDATE,
			&operation,
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup6;
	}
	/* ========================================================================
	   [6] Perform the cryptographic finalize commands  225
	   ======================================================================== */
	/* [a] Finalize the encrypt operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			CMD_ENCRYPT_FINAL,
			NULL,
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup6;
	}
	/* [b] Finalize the digest operation within the TEE application. */
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_PARTIAL_OUTPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].memref.parent = &commsSM;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 20;
	result = TEEC_InvokeCommand(
			&session,
			CMD_DIGEST_FINAL,
			&operation,
			NULL);
	if (result != TEEC_SUCCESS)
	{
		goto cleanup6;
	}
	/* Transfer digest in to user buffer. */
	memcpy(digestBuffer, commsSM.buffer, 20);
	/* ========================================================================
	   [7] Tidyup resources
	   ======================================================================== */
cleanup6:
	TEEC_ReleaseSharedMemory(&outputSM);
cleanup5:
	TEEC_ReleaseSharedMemory(&inputSM);
cleanup4:
	TEEC_ReleaseSharedMemory(&commsSM);
cleanup3:
	TEEC_CloseSession(&session);
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}
