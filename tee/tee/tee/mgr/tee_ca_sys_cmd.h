#ifndef _TEE_CA_SYS_H_
#define _TEE_CA_SYS_H_

#include "tee_client_api.h"

TEEC_Result TASysCmd_OpenSession(
	int			tzc,
	struct tee_comm*	comm,
	uint32_t		taskId,
	uint32_t		connectionMethod,
	const void*		connectionData,
	TEEC_Operation*		operation,
	uint32_t*		returnOrigin,
	uint32_t*		sessionId);

TEEC_Result TASysCmd_CloseSession(
	int			tzc,
	struct tee_comm*	comm,
	uint32_t		taskId,
	uint32_t		sessionId,
	uint32_t*		returnOrigin,
	bool*			instanceDead);

#endif /* _TEE_CA_SYS_H_ */