#ifndef _TEE_CA_TESTAPI_UTIL_
#define _TEE_CA_TESTAPI_UTIL_

#include "tee_testapi.h"


void* thread_for_cancellation(void *arg);


void show_result(struct MODULE_Cfg *module_cfg, int rantimes);

#endif /* _TEE_CA_TESTAPI_UTIL_ */
