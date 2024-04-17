#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "tee_testapi.h"
#include "tee_client_api.h"

#define RESULT_FILE_NAME	"/data/tz_test.txt"


static const char* ResultString[] =
{
	"Pass",
	"Fail",
	"Memory out",
	"Hang",
	"Unsupport",
	"can't continue"
	"Other"
};

static FILE *result_file = NULL;

static int fileopend = 0;
void show_result(struct MODULE_Cfg *module_cfg, int rantimes)
{
	int case_index = 0;

	int result = 0;

	if(!fileopend){
		result_file = fopen(RESULT_FILE_NAME, "w+");

		fileopend = 1;
	}


	while(1) {

		result = module_cfg->cases[case_index].result;
		if((result == -1) || (result >= TEST_RESULT_MAX))
			break;

		fprintf(result_file, "%s	%s		%s \
				%d	%d\n", module_cfg->name,
				module_cfg->cases[case_index].name,
				ResultString[result], module_cfg->times,
				rantimes);

		case_index++;



	}

}

void *thread_for_cancellation(void *arg)
{

	struct timeval 		now;
	struct timespec		outtime;
	pthread_mutex_t		mutex;
	pthread_cond_t 		cond;
	TEEC_Operation 		*operation = (TEEC_Operation*)arg;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	/*delay 3 s*/

	printf("delay 3s before request cancel.\n");
	pthread_mutex_lock(&mutex);
	gettimeofday(&now, NULL);
	outtime.tv_sec = now.tv_sec + 3;
	outtime.tv_nsec = now.tv_usec * 1000;
	pthread_cond_timedwait(&cond, &mutex, &outtime);
	pthread_mutex_unlock(&mutex);

	/* go set the cacellation*/


	TEEC_RequestCancellation(operation);

	return NULL;



}
