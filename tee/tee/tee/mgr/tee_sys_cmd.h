#ifndef _TEE_SYS_CMD_H_
#define _TEE_SYS_CMD_H_

#include "tee_internal_core_common.h"

/** TA System Command Protocol.
 *
 * All TA must support these System Commands.
 *
 * currently, we only support 2 system command in TA:
 * - OpenSession
 * - CloseSession
 */

enum TASysCmd {
	TASYS_CMD_OPEN_SESSION,
	TASYS_CMD_CLOSE_SESSION,	/* return instanceDead by param[0].value.a */
	TASYS_CMD_MAX
};

typedef struct {
	TEE_UUID client;	/* input: client TA uuid */
	uint32_t login;		/* input: login method */
	uint32_t group;		/* input: group to login for TEE_LOGIN_GROUP &
				 * TEE_LOGIN_APPLICATION_GROUP */
	uint32_t sessionId;	/* output: taskId (8bits) | index (24bits) */
} TASysCmdOpenSessionParamExt;

#endif /* _TEE_SYS_CMD_H_ */
