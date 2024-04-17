#ifndef _TEE_CA_TEST_H_
#define _TEE_CA_TEST_H_

#include "types.h"
#include "test_cmd.h"

/** initialize test
 *
 * @retval 0	success
 * @retval <0	fail
 */
int TestInitialize(void);
void TestFinalize(void);

int TestAdd(int a, int b);
int TestSub(int a, int b);
int TestMul(int a, int b);
int TestDiv(int a, int b);
int TestInc(int a);
int TestDec(int a);

void *TestAllocateSharedMemory(unsigned int size, void **buffer);

void TestReleaseSharedMemory(void *shm_handle);

void *TestGetSharedMemoryInfo(void *shm_handle, unsigned int *size);

/** test memory.
 *
 * @param pattern	test pattern. the test memory will be filled by pattern.
 * 			0 for data same as address.
 *
 * @retval 0		success
 * @retval !0		error code
 */
int TestMem(void *shm_handle, unsigned int pattern);

/** reverser the data bit-by-bit in a buffer.
 *
 * it's for test only.
 *
 * @param shm_handle	shared memory handle.
 *
 * @retval 0		success
 * @retval !0		error code
 */
int TestReverse(void *shm_handle);

int TestCallback(void);
int TestMemRegion(void);
int TestMemTransfer(void);
int TestMutex(void);

int TestRawCommand(uint32_t commandID);

#endif /* _TEE_CA_TEST_H_ */
