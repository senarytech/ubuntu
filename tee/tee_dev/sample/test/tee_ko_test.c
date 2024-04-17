#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include "test_cmd.h"
#include "tee_client_api.h"

#define error(fmt, args...)	printk(KERN_ERR fmt, ##args)
#define info(fmt, args...)	printk(KERN_ALERT fmt, ##args)

static const TEEC_UUID TATest_UUID = TATEST_UUID;
static TEEC_Session context;
static TEEC_Session session;

static TEEC_Result TestUserCallback(
	TEEC_Session*		session,
	uint32_t		commandID,
	TEEC_Operation*		operation,
	void*			userdata)
{
	info("callback: commandID=0x%08x, paramTypes=0x%08x, "
			"{0x%08x,0x%08x}, {0x%08x,0x%08x}, "
			"{0x%08x,0x%08x}, {0x%08x,0x%08x}\n",
			commandID,
			operation->paramTypes,
			operation->params[0].value.a,
			operation->params[0].value.b,
			operation->params[1].value.a,
			operation->params[1].value.b,
			operation->params[2].value.a,
			operation->params[2].value.b,
			operation->params[3].value.a,
			operation->params[3].value.b);

	operation->params[0].value.a = ~operation->params[0].value.a;
	operation->params[0].value.b = ~operation->params[0].value.b;
	operation->params[1].value.a = ~operation->params[1].value.a;
	operation->params[1].value.b = ~operation->params[1].value.b;
	operation->params[2].value.a = ~operation->params[2].value.a;
	operation->params[2].value.b = ~operation->params[2].value.b;
	operation->params[3].value.a = ~operation->params[3].value.a;
	operation->params[3].value.b = ~operation->params[3].value.b;

	info("Callback Done\n");
	return TEEC_SUCCESS;
}

TEEC_Result TestInitialize(void)
{
	TEEC_Result result;

	/* ========================================================================
	   [1] Connect to TEE
	   ======================================================================== */
	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup1;
	}

	/* ========================================================================
	   [2] Open session with TEE application
	   ======================================================================== */
	/* Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,	/* No connection data needed for TEEC_LOGIN_USER. */
			NULL,	/* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup2;
	}

	TEEC_RegisterCallback(&session, TestUserCallback, NULL);

	return result;

cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}

static void TestFinalize(void)
{
	TEEC_UnregisterCallback(&session);
	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&context);
}

static int TestOperate(int cmdId, int a, int b)
{
	TEEC_Result result;
	TEEC_Operation operation = {0};
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].value.a = a;
	operation.params[0].value.b = b;

	/* clear result */
	operation.params[1].value.a = 0xdeadbeef;

	/* Start the arithmetic operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);

	/*
	 * get the result
	 * FIXME: need handle the error (result != TEEC_SUCCESS)
	 */
	return operation.params[1].value.a;
}

int TestAdd(int a, int b)
{
	return TestOperate(TEST_CMD_ADD, a, b);
}
int TestSub(int a, int b)
{
	return TestOperate(TEST_CMD_SUB, a, b);
}

int TestCallback(void)
{
	return TEEC_InvokeCommand(
			&session,
			TEST_CMD_CALLBACK,
			NULL,
			NULL);
}

int TestMutex(void)
{
	return TEEC_InvokeCommand(
			&session,
			TEST_CMD_MUTEX,
			NULL,
			NULL);
}

static int __init ta_test_init(void)
{
	int a = 1, b = 2, c = 0;
	info("Hello, world\n");

	if (TEEC_SUCCESS != TestInitialize())
		return 0;

	c = TestAdd(1, 2);

	info("%d + %d = %d\n", a, b, c);

	TestCallback();

	TestMutex();

	return 0;
}

static void __exit ta_test_exit(void)
{
	TestFinalize();
	info("Goodbye\n");
}


MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Marvell");
MODULE_DESCRIPTION("Marvell TEE Kernel Test Code");
MODULE_VERSION("1.00");

module_init(ta_test_init);
module_exit(ta_test_exit);
