#ifndef _TEE_CA_MGR_CMD_H_
#define _TEE_CA_MGR_CMD_H_

#include "tee_client_api.h"

TEEC_Result TAMgr_Register(int tzc, struct tee_comm *comm,
	TEEC_Operation *operation);

TEE_Result TAMgr_CreateInstance(
	int			tzc,
	struct tee_comm*	comm,
	const TEE_UUID*		destination,
	uint32_t*		returnOrigin,
	uint32_t*		taskId);

TEE_Result TAMgr_DestroyInstance(
	int			tzc,
	struct tee_comm*	comm,
	uint32_t		taskId,
	uint32_t*		returnOrigin);

#endif /* _TEE_CA_MGR_CMD_H_ */
