#ifndef _TEE_COMMON_H_
#define _TEE_COMMON_H_

#include "types.h"

/** Return Code.
 *
 * It's same as TEEC_Result.
 */
typedef uint32_t TEE_Result;

/** Universally Unique IDentifier (UUID) type as defined in [RFC4122].A
 *
 * UUID is the mechanism by which a service (Trusted Application) is
 * identified.
 * It's same as TEEC_UUID.
 */
typedef struct
{
	uint32_t		timeLow;
	uint16_t		timeMid;
	uint16_t		timeHiAndVersion;
	uint8_t			clockSeqAndNode[8];
} TEE_UUID;

#endif /* _TEE_COMMON_H_ */
