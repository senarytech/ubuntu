#include "config.h"
#include "tee_client_api.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	TEEC_Context context;
	TEEC_Result result = 0;

	if (argc < 4) {
		printf("usage: %s <src> <dst> <size>\n", argv[0]);
		return -1;
	}

	void *src = (void *)strtoul(argv[1], NULL, 0);
	void *dst = (void *)strtoul(argv[2], NULL, 0);
	size_t len = strtoul(argv[3], NULL, 0);

	printf("copy from %p to %p, len=%d\n", dst, src, len);

	usleep(10000);

	result = TEEC_InitializeContext(
                        NULL,
                        &context);
	if (result != TEEC_SUCCESS) {
		printf("fail to initialize context, error=0x%08x\n", result);
		goto cleanup1;
	}

	result = TEEC_FastMemMove(&context, dst, src, len);

	if (result == TEEC_SUCCESS)
		printf("Copy Done. suceeded!\n");
	else
		printf("TEEC_FastMemMove() Fail. error=0x%08x\n", result);

cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return 0;

}
