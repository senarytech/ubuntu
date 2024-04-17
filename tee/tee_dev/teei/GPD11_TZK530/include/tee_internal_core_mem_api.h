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
	uint32_t mem_id; // id returned by reserve
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
TEE_Result TEE_Map_Register(void *pa, size_t len, uint32_t prot,
		uint32_t flags, void **va);
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
TEE_Result TEE_Unmap_Register(void *va, size_t len);
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

/** get the number of scatter list associated with the reservation mem_id
 *
 * @param mem_id	memory id returned by TEE_ReserveSecurePages
 * @param num		number of scatter list
 *
 * @retval TEE_SUCCESS			get the number successfully
 */
TEE_Result TEE_Mem_GetSglistNum(uint32_t mem_id, uint32_t *num);

/** get the scatter list associated with the reservation mem_id
 *
 * @param mem_id	memory id returned by TEE_ReserveSecurePages
 * @param sgt		buffer to copy the scatter list
 * @param size		size of the buffer
 *
 * @retval TEE_SUCCESS			get the scatter list
 */
TEE_Result TEE_Mem_GetSglist(uint32_t mem_id, scatter_list_t *sgt, uint32_t size);

/** get the MPT rule associated with the reservation mem_id
 *
 * @param mem_id	memory id returned by TEE_ReserveSecurePages
 * @param rule		mpt rule of the memory
 *
 * @retval TEE_SUCCESS			the rule retrived
 */
TEE_Result TEE_Reservation_GetAttribute(uint32_t mem_id, uint32_t *rule);

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

/** get the memory id by the physical address of contigious memory.
 *
 * @param mem_id	point of memory id.
 * @param phy_addr	physical address of contigious memory.
 * @param size		size of the contigious memory
 *
 * @retval TEE_SUCCESS				get memory id successfully.
 * @retval TEE_ERROR_ITEM_NOT_FOUND	Contigious memory is not registered
 * @retval TEE_ERROR_BAD_PARAMETERS	wrong parameters.
 *							wrong contigious physical memory
 */
TEE_Result TEE_GetMemid_By_Phys(uint32_t *mem_id, uintptr_t phy_addr, uint32_t size);

/** register a memory and get a mem_id
 *
 * @param mem		physical start address
 * @param size		size of the memory
 * @param mem_id	id of the memory generated
 *
 * @retval TEE_SUCCESS			register successfully
 * @retval TZ_ERROR_SHORT_BUFFER tzk heap dried up
 * @retval TZ_ERROR_ACCESS_CONFLICT memory already being registered
 * @retval TZ_ERROR_BAD_PARAMETERS memory is not in any ion region
 */
TEE_Result  TEE_Register_Memory(void *mem, size_t size,  uint32_t *mem_id);

/** unregister the memory
 *
 * @param mem_id	id of the memory
 *
 * @retval TEE_SUCCESS		successfully unregistered
 * @retval TZ_ERROR_BUSY	still in using
 * @retval TEE_ERROR_BAD_PARAMETERS  no such memory
 */
TEE_Result TEE_Unregister_Memory(uint32_t mem_id);

/** map the memory into the virtual space of caller TA
 *
 * @param mem_id	id of the memory
 * @param va		the returned virutal address
 * @param size		the returned memory size
 *
 * @retval TEE_SUCCESS			successfully mapped
 * @retval TEE_ERROR_BAD_PARAMETERS  error when map or already mapped
 */
TEE_Result TEE_Map_Memory(uint32_t mem_id, void **va, size_t *size);

/** unmap the memory from the virutal space of caller TA if mapped
 *
 * @param mem_id	id of the memory
 *
 * @retval TEE_SUCCESS			successfully unmapped
 * @retval TEE_ERROR_BAD_PARAMETERS  not mapped
 */
TEE_Result TEE_Unmap_Memory(uint32_t mem_id);

/** map the memory into the virtual space of caller TA and the instances that have the same UUID

 *
 * @param mem_id	id of the memory
 * @param va		the returned virutal address
 * @param size		the returned memory size
 *
 * @retval TEE_SUCCESS			successfully mapped
 * @retval TEE_ERROR_BAD_PARAMETERS  error when map or already mapped
 */
TEE_Result TEE_Map_Memory_ToAllInstances(uint32_t mem_id, void **va, size_t *size);

/** unmap the memory from the virutal space of caller TA and the instances that have the same UUID
 *
 * @param mem_id	id of the memory
 *
 * @retval TEE_SUCCESS			successfully unmapped
 * @retval TEE_ERROR_BAD_PARAMETERS  not mapped
 */
TEE_Result TEE_Unmap_Memory_FromAllInstances(uint32_t mem_id);


enum {
	MEMATTR_SECURE = 0x1,
	MEMATTR_NONSECURE = 0x2,

	MEMATTR_CACHEABLE = 0x10,
	MEMATTR_NONCACHEABLE = 0x20,

	MEMATTR_RESTRICTED = 0x100,
	MEMATTR_SYSTEM = 0x200,
	MEMATTR_ION = 0x400,
};

/** return if the memory has the memory attributes.
 *
 * @param mem_id	memory id.
 * @param attr		memory attribute (can be multiple options)
 *
 * @retval true			memory attributes checked are true
 * @retval false		error
 */
bool TEE_Check_Memory_Attr(uint32_t mem_id, uint32_t attr);

/** return if memory access permission of the caller TA is greater than current TA
 *
 * @param mem_id	memory id.
 *
 * @retval true 	caller TA's permission >= current ta
 * @retval false	error
 */
bool TEE_Mem_Validate_Caller(uint32_t mem_id);

/** get the ref count through memory id
 *
 * @param mem_id	id of the memory
 * @param refc		value of ref count
 *
 * @retval TEE_SUCCESS		successfully get the ref count
 */
TEE_Result TEE_Mem_GetRefCnt(uint32_t mem_id, uint32_t *refc);

/** increase the ref count of a memory
 *
 * @param mem_id	id of the memory
 *
 * @retval TEE_SUCCESS		successfully increase the ref count
 */
TEE_Result TEE_MemRef(uint32_t mem_id);

/** decrease the ref count of a memory
 *
 * @param mem_id	id of the memory
 *
 * @retval TEE_SUCCESS		successfully decrease the refcount
 */
TEE_Result TEE_MemUnref(uint32_t mem_id);

/** bind the hardware page table address with memory id
 *
 * @param mem_id	id of the memory
 * @param hw_virt	hardware page table address
 *
 * @retval TEE_SUCCESS		successfully bind the hardware page table address with memid
 */
TEE_Result TEE_Mem_Bind_HW_VirtAddr(uint32_t mem_id, void *hw_virt);

/** get the hardware page table address with memory id
 *
 * @param mem_id	id of the memory
 * @param hw_virt	hardware page table address
 *
 * @retval TEE_SUCCESS		successfully bind the hardware page table address with memid
 */
TEE_Result TEE_Mem_Get_HW_VirtAddr(uint32_t mem_id, void **hw_virt);

#endif /* _TEE_INTERNAL_MEM_API_H_ */
