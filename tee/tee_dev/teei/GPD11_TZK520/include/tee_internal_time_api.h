#ifndef _TEE_INTERNAL_TIME_API_H_
#define _TEE_INTERNAL_TIME_API_H_

#include "tee_internal_common.h"

/*
 * Time Protection Level
 */
#define TEE_TIME_REE_CONTROLLED_TIMER		(100)
#define TEE_TIME_TEE_CONTROLLED_TIMER		(1000)

#define TEE_SYSTEM_TIME_PROTECTION_LEVEL	TEE_TIME_TEE_CONTROLLED_TIMER
#define TEE_TA_PERSISTENT_TIME_PROTECTION_LEVEL	TEE_TIME_TEE_CONTROLLED_TIMER

#define TEE_TIMEOUT_INFINITE			(0xffffffff)

/**
 * @brief
 */
typedef struct {
	uint32_t seconds;
	uint32_t millis;
} TEE_Time;


/** Get system time.
 *
 * @param time
 */
void TEE_GetSystemTime(TEE_Time* time);

/** Wait for some time.
 *
 * @param timeout		in milliseconds.
 *
 * @retval TEE_SUCCESS		On success
 * @retval TEE_ERROR_CANCEL	If the wait has been cancelled
 */
TEE_Result TEE_Wait(uint32_t timeout);

/**
 * @brief
 *
 * @param time
 *
 * @retval TEE_SUCCESS		In case of success
 * @retval TEE_ERROR_TIME_NOT_SET
 * @retval TEE_ERROR_TIME_NEEDS_RESET
 * @retval TEE_ERROR_OVERFLOW	The number of seconds in the TA Persistent Time
 *				overflows the range of a uint32_t. The field
 *				time->seconds is still set to the TA Persistent
 *				Time truncated to 32 bits (i.e., modulo 2^32).
 * @retval TEE_ERROR_OUT_OF_MEMORY	If not enough memory is available to
 *					complete the operation
 */
TEE_Result TEE_SetTAPersistentTime(TEE_Time* time);

/**
 * @brief
 *
 * @param time
 *
 * @return
 */
TEE_Result TEE_GetTAPersistentTime(TEE_Time* time);

/**
 * @brief
 *
 * @param time
 */
void TEE_GetREETime(TEE_Time* time);

void TEE_GetSystemTimeNS(uint64_t *ns);

#endif /* _TEE_INTERNAL_TIME_API_H_ */
