#include "config.h"
#include "tee_ca_test_common.h"

#ifdef __LINUX__
#define get_cpu_id()		(0)
#endif /* __LINUX__ */

typedef struct PercpuContext {
	bool initialized;
	TEEC_Session session;
} PercpuContext;

static const TEEC_UUID TATest_UUID = TATEST_UUID;

static bool initialized = false;
static TEEC_Context context;
static PercpuContext percpuContext[CONFIG_CPU_NUM];

TEEC_Session *TestGetSession(void)
{
	int cpuId = get_cpu_id();
	return &percpuContext[cpuId].session;
}

static TEEC_Result TestUserCallback(
	TEEC_Session*		session,
	uint32_t		commandID,
	TEEC_Operation*		operation,
	void*			userdata)
{
	trace("callback: commandID=0x%08x, paramTypes=0x%08x, "
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

	return TEEC_SUCCESS;
}

int TestPercpuInitialize(void)
{
	TEEC_Result result;

	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;

	percpuContext[cpuId].initialized = true;

	/* ========================================================================
	   [2] Open session with TEE application
	   ======================================================================== */
	/* Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,	/* No connection data needed for TEEC_LOGIN_USER. */
			NULL,	/* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup2;
	}

	TEEC_RegisterCallback(session, TestUserCallback, NULL);

	return TEEC_SUCCESS;

cleanup2:
	return result;
}

int TestInitialize(void)
{
	TEEC_Result result;

	if (!initialized) {
		initialized = true;

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
	}

	result = TestPercpuInitialize();

	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup2;
	}

	return result;

cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}

void TestPercpuFinalize(void)
{
	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;

	if (percpuContext[cpuId].initialized) {
		TEEC_UnregisterCallback(session);
		TEEC_CloseSession(session);
		memset(&percpuContext[cpuId], 0, sizeof(percpuContext[cpuId]));
		percpuContext[cpuId].initialized = false;
	}
}

void TestFinalize(void)
{
	if (!initialized)
		return;
	initialized = false;

	TestPercpuFinalize();
	TEEC_FinalizeContext(&context);
}

void *TestAllocateSharedMemory(unsigned int size, void **buffer)
{
	TEEC_Result result;
	TEEC_SharedMemory *shm = NULL;

	if (buffer)
		*buffer = NULL;

	shm = malloc(sizeof(*shm));
	if (NULL == shm)
		return shm;

	memset(shm, 0, sizeof(*shm));

	shm->size = size;
	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		free(shm);
		return NULL;
	}

	if (buffer)
		*buffer = shm->buffer;

	return shm;
}

void TestReleaseSharedMemory(void *shm_handle)
{
	TEEC_SharedMemory *shm = shm_handle;
	if (NULL == shm)
		return;

	TEEC_ReleaseSharedMemory(shm);
	free(shm);
}

void *TestGetSharedMemoryInfo(void *shm_handle, unsigned int *size)
{
	TEEC_SharedMemory *shm = shm_handle;
	if (NULL == shm)
		return NULL;

	if (size)
		*size = shm->size;

	return shm->buffer;
}

