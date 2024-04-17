#include "tee_ca_rpmb_perf.h"
#include "string.h"
#include "log.h"

int main(int argc, const char *argv[])
{
	int op_mode = 0;
	if (argc != 2) {
		printf("usage: rpmbperf -r(read rpmb test)| -w(write rpmb test) \n");
		return -1;
	}

	if (argv[1][0] == '-' && argv[1][1] == 'w')
		op_mode = 1;
	else if (argv[1][0] == '-' && argv[1][1] == 'r')
		op_mode = 0;
	else {
		printf("usage: rpmbperf -r(read rpmb test)| -w(write rpmb test) \n");
		return -1;
	}

	if (rpmbPerfInitialize()) {
		error("rpmbPerfInitialize failed\n");
		return -1;
	}

	if(rpmbPerfTest(op_mode)) {
		error("rpmbPerfTest fialed\n");
	}

	rpmbPerfFinalize();

	return 0;
}
