TEEC_CFLAGS := -D__LINUX__ -DCONFIG_TEE

TEEC_INCLUDES := include/
TEEC_INCLUDES += tee/common/
TEEC_INCLUDES += tee/client_api/
TEEC_INCLUDES += tee/app/tee_daemon/
TEEC_INCLUDES += tee/internal_api/
TEEC_INCLUDES += tee/ree/

TEEC_CFILES += tee/app/tee_daemon/ree_sys_callback_ipc_server.c
TEEC_CFILES += tee/app/tee_daemon/rpmb.c
TEEC_CFILES += tee/app/tee_daemon/ree_ca_rpmb_op.c
TEEC_CFILES += tee/app/tee_daemon/ree_ca_file_op.c

TEEC_ABS_INCLUDES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_INCLUDES))
TEEC_REL_INCLUDES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_INCLUDES))

TEEC_ABS_CFILES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_CFILES))
TEEC_REL_CFILES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_CFILES))

TEEC_ABS_SHARED_LIBRARIES := $(addprefix $(TEE_TOP_ABS_PATH)/, $(TEEC_SHARED_LIBRARIES))
TEEC_REL_SHARED_LIBRARIES := $(addprefix $(TEE_TOP_REL_PATH)/, $(TEEC_SHARED_LIBRARIES))
