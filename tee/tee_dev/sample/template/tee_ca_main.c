#include "config.h"
#include "tee_ca_sample.h"
#include "string.h"
#include "log.h"

static void dumpData(char *data, int len, const char *info)
{
	int i;

	printf("---------------------------%s dump start-------------------------\n", info);
	for (i = 0; i < len; i++)
		printf("%02x ", data[i]);
	printf("\n---------------------------%s dump end-------------------------\n", info);
}


int main(int argc, const char *argv[])
{
	void *inShm = NULL;
	void *outShm = NULL;
	char *in = NULL;
	char *out = NULL;
	int inSize = 256;
	int outSize = 256;
	int inLen = inSize;
	int outLen = 0;
	int i;

	int a = 1, b = 2, c = 0xdeadbeef;

	/* initialize functional API environment */
	if (Initialize()) {
		printf("fail to open device\n");
		return -1;
	}

	/* test add */
	if (OperateValue(CMD_VAL_ADD, &c, a, b))
		error("add(%d+%d) fail\n", a, b);
	else
		printf("add(%d+%d)=%d\n", a, b, c);

	/* prepare to test memory operations */
	inShm = AllocateSharedMemory(inSize, &in);
	outShm = AllocateSharedMemory(outSize, &out);

	if (!inShm || !outShm)
		goto cleanup2;

	for (i = 0; i < inSize; i++)
		in[i] = i;

	/* test reverse */
	outLen = OperateMemory(CMD_MEM_REVERSE, outShm, inShm, inLen);
	if (outLen < 0) {
		error("fail to reverse memory\n");
	} else {
		dumpData(in, inLen, "reverse input");
		dumpData(out, outLen, "reverse output");
	}

	/* test encrypt */
	outLen = OperateMemory(CMD_AES_ENCRYPT, outShm, inShm, inLen);
	if (outLen < 0) {
		error("fail to reverse memory\n");
	} else {
		dumpData(in, inLen, "encrypt input");
		dumpData(out, outLen, "encrypt output");
	}

	/* test decrypt: we swap the out and in */
	inLen = OperateMemory(CMD_AES_DECRYPT, inShm, outShm, outLen);
	if (inLen < 0) {
		error("fail to reverse memory\n");
	} else {
		dumpData(out, outLen, "decrypt input");
		dumpData(in, inLen, "decrypt output");
	}

cleanup2:
	ReleaseSharedMemory(inShm);
	ReleaseSharedMemory(outShm);

cleanup1:
	Finalize();
	return 0;
}
