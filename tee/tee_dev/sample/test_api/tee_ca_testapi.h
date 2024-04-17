#ifndef _TEE_CA_TESTAPI_H_
#define _TEE_CA_TESTAPI_H_

void TestClientAPI(struct MODULE_Cfg *test_cfg);
void TestCacheAPI(struct MODULE_Cfg *test_cfg);
void TestCoreCancelAPI(struct MODULE_Cfg *test_cfg);
void TestCoreMemAPI(struct MODULE_Cfg *test_cfg);
void TestCorePropertyAPI(struct MODULE_Cfg *test_cfg);
void TestInternalTimeAPI(struct MODULE_Cfg *test_cfg);
void TestCoreClientAPI(struct MODULE_Cfg *test_cfg);
#endif
