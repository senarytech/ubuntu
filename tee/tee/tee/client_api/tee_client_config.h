#ifndef _TEE_CLIENT_CONFIG_H_
#define _TEE_CLIENT_CONFIG_H_

/** The maximum size of a single Shared Memory block, in bytes, of both API
 * allocated and API registered memory. This version of the standard requires
 * that this maximum size is greater than or equal to 512kB.
 * In systems where there is no limit imposed by the Implementation then
 * this definition should be defined to be the size of the address space.
*/
#define TEEC_CONFIG_SHAREDMEM_MAX_SIZE		0x80000

#endif /* _TEE_CLIENT_CONFIG_H_ */