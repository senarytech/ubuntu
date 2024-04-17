TEEC_CFLAGS := -D__LINUX__ -DCONFIG_TEE

TEEC_INCLUDES := include/
TEEC_INCLUDES += tz/nw_api/
TEEC_INCLUDES += tz/common/
TEEC_INCLUDES += tz/client_api/
TEEC_INCLUDES += tee/common/
TEEC_INCLUDES += tee/client_api/
TEEC_INCLUDES += tee/app/tee_daemon/
TEEC_INCLUDES += tee/mgr/
TEEC_INCLUDES += tee/internal_api/
TEEC_INCLUDES += tee/ree/

TEEC_CFILES := tz/client_api/tz_client_api_user.c
TEEC_CFILES += tee/client_api/tee_client_api.c
TEEC_CFILES += tee/client_api/tee_client_util.c
TEEC_CFILES += tee/mgr/tee_ca_mgr_cmd.c
TEEC_CFILES += tee/mgr/tee_ca_sys_cmd.c
TEEC_CFILES += tee/ree/ree_sys_callback_user.c
TEEC_CFILES += tee/ree/ree_sys_callback_ipc_client.c

TEEC_STATIC_LIBRARIES := tee/client_api/lib/libteec.a
TEEC_SHARED_LIBRARIES := tee/client_api/lib/libteec.so

TEEC_ABS_INCLUDES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_INCLUDES))
TEEC_REL_INCLUDES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_INCLUDES))

TEEC_ABS_CFILES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_CFILES))
TEEC_REL_CFILES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_CFILES))

TEEC_ABS_STATIC_LIBRARIES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_STATIC_LIBRARIES))
TEEC_REL_STATIC_LIBRARIES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_STATIC_LIBRARIES))

TEEC_ABS_SHARED_LIBRARIES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_SHARED_LIBRARIES))
TEEC_REL_SHARED_LIBRARIES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_SHARED_LIBRARIES))
