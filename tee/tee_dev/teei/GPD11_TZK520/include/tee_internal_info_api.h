#ifndef _TEE_INTERNAL_INFO_API_H_
#define _TEE_INTERNAL_INFO_API_H_

#include "tee_internal_common.h"
#include "tee_internal_config.h"

#define TEE_VERSION(major, minor)	(((major) << 16) | ((minor) << 16))
#define TEE_VERSION_STR(major, minor)	#major "." #minor

#define TEE_DESCRIPTION(major, minor)	"TEE SDK v" TEE_VERSION_STR(major, minor)

const char *TEE_GetVersion(uint32_t *major, uint32_t *minor);

const char *TEE_GetDescription(void);

const TEE_UUID *TEE_GetDeviceID(void);

#endif /* _TEE_INTERNAL_INFO_API_H_ */
