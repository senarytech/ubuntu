#ifndef _TEE_MGR_CMD_H_
#define _TEE_MGR_CMD_H_

#define TAMGR_UUID	{0x9ef9656c, 0x8f49, 0x11e3,	\
			{0x90, 0x1c, 0x78, 0x2b, 0xcb, 0x5c, 0xf3, 0xe3}}

enum TAMgrCmd {
	TAMGR_CMD_SUSPEND,
	TAMGR_CMD_RESUME,
	TAMGR_CMD_REGISTER,
	/* Create Instance
	 * input: destination UUID
	 * output: created taskId
	 */
	TAMGR_CMD_CREATE_INSTANCE,
	/* Destroy Instance
	 * input: taskId to destroy in params[0].value.a
	 * note: should NOT need call it explictly
	 */
	TAMGR_CMD_DESTROY_INSTANCE,
	TAMGR_CMD_MAX
};

typedef struct {
	TEE_UUID destination;	/* input: desitination TA uuid */
	uint32_t taskId;	/* output: created taskId */
} TAMgrCmdCreateInstanceParamExt;

#endif /* _TEE_MGR_CMD_H_ */
