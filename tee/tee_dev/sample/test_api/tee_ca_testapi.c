#include "config.h"
#include "tee_testapi.h"
#include "tz_client_api.h"
#include "string.h"
#include "log.h"

#include "tee_ca_testapi.h"

#ifndef __LINUX__
#define usleep		udelay
#endif /* __LINUX__ */


struct GROUP_Cfg test_group_cfg[MAX_GROUP_NUM]=
{
	{
		.type		=	TEST_GROUP_CLIENT,
		.name		=	"client api",
		.enable		=	1,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEEC_INIT_CONTEXT,
				.name	=	"TEEC_InitializeContext",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEEC_FINIT_CONTEXT,
				.name	=	"TEEC_FinalizeContext",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEEC_CLOSE_SESSION,
				.name	=	"TEEC_CloseSession",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_OPEN_SESSION,
				.name	=	"TEEC_OpenSession",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_ALLOC_SHM,
				.name	=	"TEEC_AllocSharedMemory",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_RELEASE_SHM,
				.name	=	"TEEC_ReleaseSharedMemory",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_INVOKECOMMAND,
				.name	=	"TEEC_InvokeCommand",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_REQUESTCANCELLATION,
				.name	=	"TEEC_RequestCancellation",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_GETERROR,
				.name	=	"TEEC_GetError",
				.enable	=	0,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	TEST_TEEC_REGISTERTA,
				.name	=	"TEEC_RegisterTa",
				.enable	=	0,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},
			{
				.type	=	-1,
			},

		},
	},

	{
		.type		=	TEST_GROUP_CORE_CACHE,
		.name		=	"cache api",
		.enable		=	0,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEE_CACHE_FLUSH,
				.name	=	"TEE_FlushCache",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},
			},

			{
				.type = -1,
			},
		},

	},
	{
		.type		=	TEST_GROUP_CORE_CLIENT,
		.name		=	"core client api",
		.enable		=	1,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEE_CORE_OPENSESSION,
				.name	=	"TEE_OpenTASession",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"fake uuid parameter",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_CLOSESESSION,
				.name	=	"TEE_CloseTASession",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_INVOKECMD,
				.name	=	"TEE_InvokeTACommand",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter type error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"cmd ID error",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type = -1,
			},
		},
	},
	{
		.type		=	TEST_GROUP_CORE_MEM,
		.name		=	"core mem api",
		.enable		=	1,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEE_CORE_MEM_ALLOC,
				.name	=	"TEE_Malloc",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_MEM_REALLOC,
				.name	=	"TEE_Realloc",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_MEM_FREE,
				.name	=	"TEE_Free",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_MEM_MOVE,
				.name	=	"TEE_MemMove",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"not word align",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"word align",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_MEM_FILL,
				.name	=	"TEE_MemFill",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_MEM_CMP,
				.name	=	"TEE_MemCompare",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_MEM_ACCESS_CHECK,
				.name	=	"TEE_CheckMemoryAccessRights",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"zsp region",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"tz region",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"sys_init region",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"bootloader region",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"system region",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"shm region",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type = -1,
			},
		},
	},
	{
		.type		=	TEST_GROUP_CORE_CANCEL,
		.name		=	"core cancel api",
		.enable		=	1,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEE_CORE_CANCEL_GETFLAG,
				.name	=	"TEE_GetCancellationFlag",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_CANCEL_MASK,
				.name	=	"TEE_MaskCancellation",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_CANCEL_UNMASK,
				.name	=	"TEE_UnmaskCancellation",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type = -1,
			},
		},
	},
	{
		.type		=	TEST_GROUP_INTERNAL_TIME,
		.name		=	"internal time api",
		.enable		=	1,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEE_INTERNAL_GET_SYSTIME,
				.name	=	"TEE_GetSystemTime",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_INTERNAL_GET_REETIME,
				.name	=	"TEE_GetREETime",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_INTERNAL_TIME_WAIT,
				.name	=	"TEE_Wait",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_INTERNAL_GET_PERSIST_TIME,
				.name	=	"TEE_GetTAPersistentTime",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"get before set",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_INTERNAL_SET_PERSIST_TIME,
				.name	=	"TEE_SetTAPersistentTime",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"time overflow case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type = -1,
			},
		},
	},
	{
		.type		=	TEST_GROUP_CORE_CRYPTO,
		.name		=	"crypto api",
		.enable 	=	1,
		.module_cfg =
		{
			{
				.type	=	TEST_TEE_CORE_CRYPTO_PKCS1_V1_5_MD5,
				.name	=	"TEE_core_crypto_pkcs1_v1_5_md5",
				.enable =	1,
				.times	=	1,
				.cases	=
				{
					{
						.result =	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_CRYPTO_PKCS1_V1_5_SHA1,
				.name	=	"TEE_core_crypto_pkcs1_v1_5_sha1",
				.enable =	1,
				.times	=	1,
				.cases	=
				{
					{
						.result =	-1,
					},
				},
			},
			{
				.type	=	TEST_TEE_CORE_CRYPTO_PKCS1_V1_5_SHA256,
				.name	=	"TEE_core_crypto_pkcs1_v1_5_sha256",
				.enable =	1,
				.times	=	1,
				.cases	=
				{
					{
						.result =	-1,
					},
				},
			},
			{
				.type = -1,
			},
		},

	},
	{
		.type		=	TEST_GROUP_CORE_PROPERTY,
		.name		=	"core property api",
		.enable		=	1,
		.module_cfg	=
		{
			{
				.type	=	TEST_TEE_CORE_ALLOC_PROPERTY,
				.name	=	"TEE_AllocatePropertyEnumerator",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},

			},
			{
				.type	=	TEST_TEE_CORE_RESET_PROPERTY,
				.name	=	"TEE_ResetPropertyEnumerator",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_START_PROPERTY,
				.name	=	"TEE_StartPropertyEnumerator",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_NAME,
				.name	=	"TEE_GetPropertyName",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter namelen is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_AS_BOOL,
				.name	=	"TEE_GetPropertyAsBool",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter value is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_AS_STRING,
				.name	=	"TEE_GetPropertyAsString",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter value is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter valuelen is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter valuelen is 0",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_AS_U32,
				.name	=	"TEE_GetPropertyAsU32",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter value is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_AS_UUID,
				.name	=	"TEE_GetPropertyAsUUID",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter value is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_AS_ID,
				.name	=	"TEE_GetPropertyAsIdentity",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter value is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_PROPERTY_AS_BINBLOCK,
				.name	=	"TEE_GetPropertyAsBinaryBlock",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter name is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter value is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter valuelen is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"parameter valuelen is 0",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_GET_NEXT_PROPERTY,
				.name	=	"TEE_GetNextProperty",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"parameter property is NULL",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.name	=	"normal case",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type	=	TEST_TEE_CORE_FREE_PROPERTY,
				.name	=	"TEE_FreePropertyEnumerator",
				.enable	=	1,
				.times	=	1,
				.cases 	=
				{
					{
						.name	=	"get after free",
						.result	=	TEST_RESULT_FAIL,
					},
					{
						.result	=	-1,
					},
				},


			},
			{
				.type = -1,
			},
		},
	},
	{
		.type           =       -1,
	}
};


void initTestConfig()
{

}

int main(int argc, const char *argv[])
{

	int 		sleep_time_us = 0;

	struct GROUP_Cfg 	*group_cfg;
	struct MODULE_Cfg	*module_cfg;
	char		*group_name;
	int 		group_type;
	int		group_on;
	int group_index = 0;

	initTestConfig();

	printf("enter test api\n\n\n\n");
	while(1){
		group_cfg = &test_group_cfg[group_index];
		group_name = group_cfg->name;
		group_type = group_cfg->type;
		group_on = group_cfg->enable;
		module_cfg = group_cfg->module_cfg;

		printf("test group %s, index is %d,type is %d,enable is %d\n",
				group_name, group_index, group_type, group_on);
		if(group_type == -1)
			break;

		if(!group_on) {
			group_index++;
			continue;
		}
		switch(group_type){

		case TEST_GROUP_CLIENT:
			TestClientAPI(module_cfg);
			break;
		case TEST_GROUP_CORE_CACHE:
			TestCacheAPI(module_cfg);
			break;
		case TEST_GROUP_CORE_MEM:
			TestCoreMemAPI(module_cfg);
			break;
		case TEST_GROUP_CORE_CLIENT:
			TestCoreClientAPI(module_cfg);
			break;
		case TEST_GROUP_CORE_CANCEL:
			TestCoreCancelAPI(module_cfg);
			break;
		case TEST_GROUP_INTERNAL_TIME:
			TestInternalTimeAPI(module_cfg);
			break;
		case TEST_GROUP_CORE_CRYPTO:
			TestCoreCryptoAPI(module_cfg);
			break;
		case TEST_GROUP_CORE_PROPERTY:
			TestCorePropertyAPI(module_cfg);
			break;
		default:
			break;
		}
		group_index++;
	}



	return 0;

}


