#ifndef _TEE_INTERNAL_MEM_API_H_
#define _TEE_INTERNAL_MEM_API_H_

#include "tee_internal_core_common.h"

enum TEE_AccessFlags {
	TEE_ACCESS_READ			= 0x00000001,
	TEE_ACCESS_WRITE		= 0x00000002,
	TEE_ACCESS_ANY_OWNER		= 0x00000004,
};

enum TEE_MallocHint {
	TEE_MALLOC_ZEROS		= 0x00000000,
	TEE_MALLOC_FLAG_DEFAULT		= TEE_MALLOC_ZEROS,
	TEE_MALLOC_FLAG_SYS_MAX		= 0x7fffffff,
	TEE_MALLOC_FLAG_USER_START	= 0x80000000,
	TEE_MALLOC_FLAG_USER_MAX	= 0xffffffff
};

typedef struct {
	unsigned int addr; // physical address
	unsigned int npage; // number of scatter
} scatter_item_t;

typedef struct {
	unsigned int num; // number of element in the table
	scatter_item_t s_item[]; // point to the first element of scatter_item
} scatter_list_t;

#define SIZE_OF_SCATTERLIST(num) (sizeof(unsigned int) + (num * sizeof(scatter_item_t)))

typedef struct {
	scatter_list_t *s_list; // raw pages
	unsigned int a;  // single security attribute
} TEE_Reservation;

#define TEE_MallocHintIsUser(hint)	((hint) & 0x80000000)
#define TEE_MallocHintIsSys(hint)	(!TEE_MallocHintIsUser(hint))

TEE_Result TEE_CheckMemoryAccessRights(
	uint32_t		accessFlags,
	void			*buffer,
	size_t			size);

void TEE_SetInstanceData(
	void *instanceData);

void *TEE_GetInstanceData(void);

void *TEE_Malloc(
	size_t			size,
	uint32_t		hint);

/**
 * @brief
 *
 * @param buffer
 * @param newSize
 *
 * @return
 */
void *TEE_Realloc(
	void			*buffer,
	uint32_t		newSize);

/**
 * @brief
 *
 * @param buffer
 */
void TEE_Free(void *buffer);

/** Copies size bytes from the object pointed to by src into the object pointed
 * to by dest
 *
 * @param dest		destination buffer
 * @param src		source buffer
 * @param size		number of bytes to be copied
 *
 * @sa memcpy() in C
 */
void TEE_MemMove(
	void			*dest,
	void			*src,
	uint32_t		size);

/** Compares the first size bytes of the object pointed to by buffer1 to the
 * first size bytes of the object pointed to by buffer2.
 *
 * @param buffer1	A pointer to the first buffer
 * @param buffer2	A pointer to the second buffer
 * @param size		The number of bytes to be compared
 *
 * @retval >0		If the first byte that differs is higher in buffer1
 * @retval =0		If the first size bytes of the two buffers are identical
 * @retval <0		If the first byte that differs is higher in buffer2
 *
 * @note buffer1 and buffer2 can reside in any kinds of memory, including
 * shared memory.
 * @sa memcmp() in C
 */
int32_t TEE_MemCompare(
	void			*buffer1,
	void			*buffer2,
	uint32_t		size);

/** Fill memory with byte x.
 *
 * @param buffer	A pointer to the destination buffer
 * @param x		The value to be set. Will convert to uint8_t.
 * @param size		The number of bytes to be set
 *
 * @sa memset() in C
 */
void TEE_MemFill(
	void			*buffer,
	uint32_t		x,
	uint32_t		size);

/** map physical address.
 *
 * currently, it can only be used to map registers.
 *
 * @param pa		Physical Address
 * @param len		bytes to map
 * @param prot		protection type. reserved for future use
 * @param flags		reserved for future use
 * @param va		buffer to return Virtual Address
 *
 * @retval TEE_SUCCESS			success to map.
 * @retval TEE_ERROR_BAD_PARAMETERS	bad parameters.
 * @retval TEE_ERROR_ACCESS_DENIED	not allow to map
 */
TEE_Result TEE_MemMap(void *pa, size_t len, uint32_t prot,
		uint32_t flags, void **va);

/** unmap memory.
 *
 * @param va		Virtual Address to unmap.
 * @param len		bytes to unmap.
 *
 * @retval TEE_SUCCESS			success to unmap.
 * @retval TEE_ERROR_BAD_PARAMETERS	wrong parameters. !va or len == 0.
 */
TEE_Result TEE_MemUnmap(void *va, size_t len);

/** create MPT reservation
 *
 * the reservation includes a scatter list and rule index
 *
 * @param sctl_t		contains MPT table and numbers
 * @param rule			MPT rule
 *
 * @retval TEE_SUCCESS				MPT rule is set successfully
 * @retval TEE_ERROR_BAD_PARAMETERS	fail to set MPT rule
 */
TEE_Reservation *TEE_CreateReservation(void *sctl_t, unsigned int rule);

/** destory MPT reservation after it is released
 *
 * @param r		structure of MPT reservation
 *
 * @return
 */
void TEE_DestroyReservation(TEE_Reservation *r);

/** reserve the memory in TEE_Reservation
 *
 * if the rule is secure rule, the MPT rule of memorys in the scatter list
 * will be set to the secure rule
 * if the rule is non-secure rule, the return result shows whether it
 * matchs with the non-secure rule (this is required by PTM TA)
 *
 * @param r		structure of MPT reservation
 *
 * @retval TEE_SUCCESS				secure memory is reserved
 * @retval TEE_ERROR_BAD_PARAMETERS	fail to reserve
 */
TEE_Result TEE_ReserveSecurePages(TEE_Reservation *r);

/** release MPT reservation
 *
 * it means the MPT rule of the memorys in the scatter list will be set
 * to non-secure rule (by default, non-secure rule is 0)
 *
 * @param r		structure of MPT reservation
 *
 * @retval TEE_SUCCESS				secure memory is released
 * @retval TEE_ERROR_BAD_PARAMETERS	fail to release
 */
TEE_Result TEE_ReleaseSecurePages(TEE_Reservation *r);

/** get the MPT rule associated with the reservation
 *
 * @param r	contains MPT table and numbers
 *
 * @retval TEE_SUCCESS			r->a is the rule retrived
 * @retval TEE_ERROR_BAD_PARAMETERS  rule is not consistent
 */
TEE_Result TEE_Reservation_GetAttribute(TEE_Reservation *r);

/** map the scatter pages into the virtual space of caller TA
 *
 * @param r		contains MPT table and numbers
 *
 * @retval TEE_SUCCESS			successfully mapped
 * @retval TEE_ERROR_BAD_PARAMETERS  error when map or already mapped
 */
TEE_Result TEE_Reservation_MAP(TEE_Reservation *r);

/** unmap the scatter pages from the virutal space of caller TA if mapped
 *
 * @param r		contains MPT table and numbers
 *
 * @retval TEE_SUCCESS			successfully unmapped
 * @retval TEE_ERROR_BAD_PARAMETERS  not mapped
 */
TEE_Result TEE_Reservation_UNMAP(TEE_Reservation *r);

/** get the physical address of contigious memory by memory id.
 *
 * @param mem_id	memory id.
 * @param phy_addr	point of physical address.
 *
 * @retval TEE_SUCCESS			get physical address successfully.
 * @retval TEE_ERROR_BAD_PARAMETERS	wrong parameters.
 *							wrong mem_id or non-contigious memory
 */
TEE_Result TEE_GetPhys_By_Memid(uint32_t mem_id, void **phy_addr);

#endif /* _TEE_INTERNAL_MEM_API_H_ */
