#include "config.h"
#include "tee_ca_calc.h"
#include "string.h"
#include "log.h"

#ifndef __LINUX__
#include "delay.h"
#define usleep		udelay
#endif /* __LINUX__ */

static void perf_test(void)
{
	int times = 10000000;
	int i;

	printf("start test %d times\n", times);

	for (i = 0; i < times; i++)
		CalcAdd(2, 2);

	printf("done\n");
}

int main(int argc, const char *argv[])
{
	int loop_times = -1;
	int show_interval = 10000;
	int sleep_time_us = 0;

#ifdef __LINUX__
	sleep_time_us = 200000;

	if (argc > 1)
		loop_times = atoi(argv[1]);

	if (argc > 2)
		show_interval = atoi(argv[2]);
#endif /* __LINUX__ */

	if (CalcInitialize()) {
		printf("fail to open device\n");
		return -1;
	}

	int a = 0;
	int b = 0;

	while (loop_times < 0 || (loop_times > 0)) {
		usleep(sleep_time_us);

#if 0
		char str[32];
		sprintf(str, "run %d times\n", a);
		CalcEcho(str);
#else
#define N	64
		int in[N], out[N];
		int i;
		for (i = 0; i < N; i++)
			in[i] = 0xa5a5a5a5;

		CalcReverse(out, in, sizeof(in));
		for (i = 0; i < N; i++) {
			if (out[i] != ~in[i])
				error(" CalcReverse() Failed! expect 0x%08x, get 0x%08x\n",
						~in[i], out[i]);
		}
#endif
		int sum = CalcAdd(a, b);
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
	CalcFinalize();
	return 0;
}
