#ifndef _TEE_CA_SAMPLE_H_
#define _TEE_CA_SAMPLE_H_

#include "sample_cmd.h"

/** initialize calc
 *
 * @retval 0	success
 * @retval <0	fail
 */
int Initialize(void);
void Finalize(void);

void *AllocateSharedMemory(unsigned int size, char **buffer);

void ReleaseSharedMemory(void *shm);

char *GetSharedMemoryInfo(void *shm, unsigned int *size);

int OperateValue(int cmdId, int *out, int a, int b);

int OperateMemory(int cmdId, void *outShm, void *inShm, int len);

#endif /* _TEE_CA_SAMPLE_H_ */
