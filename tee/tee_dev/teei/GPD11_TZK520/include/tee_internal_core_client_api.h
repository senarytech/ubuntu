#ifndef _TEE_INTERNAL_CORE_CLIENT_API_H_
#define _TEE_INTERNAL_CORE_CLIENT_API_H_

#include "tee_internal_core_common.h"

TEE_Result TEE_OpenTASession(
	const TEE_UUID*		destination,
	uint32_t		cancellationRequestTimeout,
	uint32_t		paramTypes,
	TEE_Param		params[4],
	TEE_TASessionHandle*	session,
	uint32_t*		returnOrigin);

void TEE_CloseTASession(
	TEE_TASessionHandle	session);

TEE_Result TEE_InvokeTACommand(
	TEE_TASessionHandle	session,
	uint32_t		cancellationRequestTimeout,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4],
	uint32_t*		returnOrigin);

#endif /* _TEE_INTERNAL_CORE_CLIENT_API_H_ */
