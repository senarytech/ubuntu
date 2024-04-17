#ifndef _TEE_INTERNAL_CORE_PANIC_API_H_
#define _TEE_INTERNAL_CORE_PANIC_API_H_

#include "tee_internal_core_common.h"

/*
 * @brief This is a panic function, which indicates that an un-recoverable
 * 		  error has occurred. This function may not return control to the
 * 		  caller function.
 *
 * @param panicCode A variable that may be used to indicate error status.
  */
void TEE_Panic(
	TEE_Result		panicCode);

#endif /* _TEE_INTERNAL_CORE_PANIC_API_H_ */
