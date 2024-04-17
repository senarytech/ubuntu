#ifndef _TEE_INTERNAL_CORE_CANCEL_API_H_
#define _TEE_INTERNAL_CORE_CANCEL_API_H_

#include "tee_internal_core_common.h"

bool TEE_GetCancellationFlag(void);
bool TEE_UnmaskCancellation(void);
bool TEE_MaskCancellation(void);

#endif /* _TEE_INTERNAL_CORE_CANCEL_API_H_ */
