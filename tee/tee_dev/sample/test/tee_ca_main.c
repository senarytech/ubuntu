#include "config.h"
#include "tee_ca_test.h"
#include "string.h"
#include "log.h"
#include <sys/time.h>

static void perf_test(void)
{
	struct timeval stv, etv;
	int times = 1000000;
	int i;

	printf("start test %d times\n", times);

	gettimeofday(&stv, NULL);

	for (i = 0; i < times; i++)
		TestAdd(2, 2);

	gettimeofday(&etv, NULL);

	long us = (etv.tv_sec - stv.tv_sec) * 1000000 +
		etv.tv_usec - stv.tv_usec;

	printf("done, total %d times cost %d us, each switch cost %d ns\n",
			times, us, us / (times / 1000));
}

#define TEST_SHM_SIZE		(256 << 10)

int main(int argc, const char *argv[])
{
	int result = 0;

	int loop_times = -1;
	int show_interval = 10000;
	int sleep_time_us = 0;

	sleep_time_us = 200000;

	if (argc > 1)
		loop_times = atoi(argv[1]);

	if (argc > 2)
		show_interval = atoi(argv[2]);

	if (TestInitialize()) {
		printf("fail to open device\n");
		return -1;
	}

	perf_test();

	void *buf = NULL;
	void *shm = TestAllocateSharedMemory(TEST_SHM_SIZE, &buf);

	if (!shm) {
		result = -2;
		goto cleanup1;
	}

#if 0
	/* temporally disable it for there is a lot of log messages in them.
	 * it would be a problem when we do stress test.
	 * can enable them again after handle the log message issue.
	 */
	TestMemRegion();
	TestMemTransfer();
	TestMutex();
#endif

	int a = 0;
	int b = 0;

	while (loop_times < 0 || (loop_times > 0)) {
		usleep(sleep_time_us);

		TestCallback();
		TestTime();
		TestMem(shm, 0);
		TestRawCommand(TEST_CMD_TACLIENT);
		TestRawCommand(TEST_CMD_TASHM);

		int sum = TestAdd(a, b);

		/* only dump message when fail */
		if (sum != (a + b) || ((a % show_interval) == 0))
			info("cmd add(%d, %d), get %d, expect %d. %s\n",
				a, b, sum, (a + b),
				sum == (a + b) ? "PASS" : "FAIL");

		a++;
		b = a * 2;

		if (loop_times > 0)
			loop_times--;
	}

cleanup2:
	TestReleaseSharedMemory(shm);
cleanup1:
	TestFinalize();

	return result;
}
