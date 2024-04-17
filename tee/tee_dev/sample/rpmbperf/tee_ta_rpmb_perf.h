#ifndef __TEE_TA_RPMB_PERF_H__
#define __TEE_TA_RPMB_PERF_H__

#define RPMB_PERF_TEST_TA_TRUST_STORAGE_ID "RPMB PERF TEST TA TRUST STROAGE ID"

TEE_Result RPMB_Read_Perf_Test(TEE_Param params[4]);

TEE_Result RPMB_Write_Perf_Test(TEE_Param params[4]);

#endif /* __TEE_TA_RPMB_PERF_H__ */
