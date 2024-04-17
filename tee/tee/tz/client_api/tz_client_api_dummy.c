#include <config.h>
#include "tz_client_api.h"
#include "tz_nw_api.h"
#include "malloc.h"
#include "log.h"
#include "tee_internal_api.h"

#define TZC_DEV_ID	(1)

/* initialize TZ client.
 *
 * for dummy test only.
 */
int tzc_init(void)
{
	static bool initialized = false;

	if (!initialized) {
		tz_nw_task_client_init_all();
		initialized = true;
	}
	return 0;
}

int tzc_open(const char *dev)
{
	return TZC_DEV_ID;
}

void tzc_close(int tzc)
{
}

void *tzc_alloc_shm(int tzc, size_t size, void **pa)
{
	void *va = malloc(size);

	if (pa)
		*pa = va;

	return va;
}

void tzc_free_shm(int tzc, void *va, void *pa, size_t size)
{
	free(va);
}

int tzc_invoke_command(int tzc, int task_id, uint32_t cmd_id,
			uint32_t param, uint32_t *origin,
			tz_cmd_handler callback, void *userdata)
{
	static int call_id = 1;
	struct tz_nw_comm cc;
	struct tz_nw_task_client *tc = tz_nw_task_client_get(task_id);

	trace("task_id=%d, cmd_id=0x%08x, param=0x%08x.\n",
			task_id, cmd_id, param);

	cc.call.task_id = task_id;
	cc.call.cmd_id = cmd_id;
	cc.call.param = param;

	int ret;
	while (1) {
		ret = tz_nw_comm_invoke_command(tc, &cc, call_id, NULL);
		if (ret != TZ_PENDING)
			break;

		trace("user callback cmd=%d, param=0x%08x\n",
			cc.callback.cmd_id, cc.callback.param);

		if (callback) {
			cc.callback.result = callback(userdata,
					cc.callback.cmd_id,
					cc.callback.param,
					&cc.callback.origin);
		} else {
			cc.callback.result = TZ_ERROR_NOT_SUPPORTED;
			cc.callback.origin = TZ_ORIGIN_UNTRUSTED_APP;
		}
	}

	if (ret != 0) {
		cc.call.origin = TZ_ORIGIN_API;
		cc.call.result = TZ_ERROR_COMMUNICATION;
	}

	call_id++;

	if (origin)
		*origin = cc.call.origin;

	return cc.call.result;
}

/*
 * dummy mutex functions
 */
void *tzc_mutex_create(void)
{
	return (void *)0xdeadbeef;
}

int tzc_mutex_destroy(void *lock)
{
	return TZ_SUCCESS;
}

int tzc_mutex_lock(void *lock)
{
	return TZ_SUCCESS;
}

int tzc_mutex_trylock(void *lock)
{
	return TZ_SUCCESS;
}

int tzc_mutex_unlock(void *lock)
{
	return TZ_SUCCESS;
}


#ifdef CONFIG_TEE

struct tee_comm *tzc_acquire_tee_comm_channel(int tzc)
{
	void *pa = NULL;

	struct tee_comm *tc = tzc_alloc_shm(tzc, sizeof(struct tee_comm), &pa);
	if (tc) {
		tc->pa = pa;
		tc->va = tc;
	}

	return tc;
}

void tzc_release_tee_comm_channel(int tzc, struct tee_comm *tc)
{
	tzc_free_shm(tzc, tc, tc, sizeof(*tc));
}

#endif /* CONFIG_TEE */
void *tzc_get_mem_info(int tzc, void *va, uint32_t *attr)
{
	return va;
}

int tzc_fast_memmove(int tzc, void *dst_phy_addr, void *src_phy_addr, size_t size)
{
	TEE_Result res;

	void *dst = (void *)TEE_PhysToVirt(dst_phy_addr);
	void *src = (void *)TEE_PhysToVirt(src_phy_addr);

	res = TEE_CheckMemoryTransferRights(dst, size, src, size, TEE_MT_COPY);
	if (res == TEE_SUCCESS)
		TEE_MemMove(dst, src, size);

	return res;
}

int tzc_fast_secure_cache_clean(int tzc, void *phy_addr, size_t size)
{
	void *start = (void *)TEE_PhysToVirt(phy_addr);

	return TEE_CleanCache(start, size);
}

int tzc_fast_secure_cache_invalidate(int tzc, void *phy_addr, size_t size)
{
	void *start = (void *)TEE_PhysToVirt(phy_addr);

	return TEE_InvalidateCache(start, size);
}

int tzc_fast_secure_cache_flush(int tzc, void *phy_addr, size_t size)
{
	void *start = (void *)TEE_PhysToVirt(phy_addr);

	return TEE_FlushCache(start, size);
}

int tzc_open_session(int tzc, unsigned long param, uint32_t taskId,
			uint32_t *origin)
{
	return 0;
}

int tzc_close_session(int tzc, unsigned long param, uint32_t taskId,
			uint32_t *origin)
{
	return 0;
}

int tzc_create_instance(int tzc, unsigned long param, uint32_t *origin)
{
	return 0;
}

int tzc_destroy_instance(int tzc, unsigned long param, uint32_t *origin)
{
	return 0;
}
