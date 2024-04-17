#include "config.h"
#include "tee_comm.h"
#include "tz_nw_comm.h"
#include "tz_nw_sys_callback.h"
#include "tz_nw_osal.h"
#ifdef CONFIG_REE_SYS_MUTEX
#include "ree_sys_callback_mutex.h"
#endif
#ifdef CONFIG_TEE_SYS_MUTEX
#include "ree_sys_callback_mutex_ext.h"
#endif
#include "ree_sys_callback_semaphore.h"
#include "log.h"

extern TEEC_Result REE_InvokeSysCommandEntryPoint(
		void*			task_client,
		uint32_t		commandID,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size);

static uint32_t REE_ExecCommand(void *userdata,
		uint32_t cmd_id, uint32_t param, uint32_t *p_origin)
{
	struct tz_nw_task_client *tc = userdata;

	uint32_t result = TEEC_ERROR_NOT_SUPPORTED;
	uint32_t origin = TEE_ORIGIN_COMMS;
	struct tee_comm_param *cp;

	trace("ENTER. cmd_id=%d, param=0x%08x\n", cmd_id, param);

	/* tee task can only support tee commands */
	if (cmd_id != TZ_CMD_TEE_SYS) {
		result = TEEC_ERROR_NOT_SUPPORTED;
		goto out;
	}

	/* need convert to kernel space address */
	cp = tz_nw_phys_to_virt(tc->call_info,
			(void *)((unsigned long)param));
	if (!cp) {
		result = TEEC_ERROR_BAD_STATE;
		goto out;
	}

	trace("TEE cmd_id=%d, flags=0x%08x, sessionId=0x%08x, "
		"param_types=0x%08x, params[0].value.a=0x%08x\n",
		cp->cmd_id, cp->flags, cp->session_id,
		cp->param_types, cp->params[0].value.a);

	/* step 1: check whether the command has been cancelled
	 * if yes, then just return with error.
	 */
	if (cp->flags & TZ_COMM_REQ_CANCELLED) {
		origin = TEE_ORIGIN_COMMS;
		result = TEEC_ERROR_CANCEL;
		goto out;
	}

	/* step 2: execute the command
	 */
	origin = TEE_ORIGIN_TRUSTED_APP;
	result = REE_InvokeSysCommandEntryPoint(
		userdata,
		cp->cmd_id,
		cp->param_types,
		cp->params,
		cp->param_ext,
		cp->param_ext_size);
out:
	trace("EXIT. origin=%d, result=0x%08x\n", origin, result);
	if (p_origin)
		*p_origin = origin;
	return result;
}

void REE_SysCallbackInit(void)
{
	tz_nw_register_sys_callback(TZ_CMD_TEE_SYS, REE_ExecCommand, NULL);
}

int REE_RuntimeInit(void)
{
	/* init sys callback */
	REE_SysCallbackInit();
#ifdef CONFIG_REE_SYS_MUTEX
	REE_MutexInit();
#endif
#ifdef CONFIG_TEE_SYS_MUTEX
	REE_MutexInitExt();
#endif
#ifdef CONFIG_REE_SYS_SEMAPHORE
	REE_SemaphoreInit();
#endif
	return 0;
}
