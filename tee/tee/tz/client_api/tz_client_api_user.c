#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "tz_client_api.h"
#include "tz_nw_ioctl.h"
#include "log.h"

#ifndef CONFIG_TZ_CLIENT_MAX_FILE_DESCRIPTORS
#define CONFIG_TZ_CLIENT_MAX_FILE_DESCRIPTORS	(32)
#endif /* CONFIG_TZ_CLIENT_MAX_FILE_DESCRIPTORS */

void *tzc_malloc(uint32_t size)
{
	if (!size) return NULL;
	return malloc(size);
}

void tzc_free(void *ptr)
{
	if (ptr) free(ptr);
}

unsigned long tzc_strtoul(const char *nptr, char **endptr, int base)
{
	if (!nptr)
		return TZ_ERROR_BAD_PARAMETERS;

	return strtoul(nptr, endptr, base);
}

char *tzc_strdup(const char *str)
{
	if (!str)
		return NULL;

	return strdup(str);
}

#ifdef __ANDROID__
extern void ion_dump_backtrace(char *tag);
#endif

static void dump_backtrace(void)
{
/* TODO: add glibc dump_backtrace*/
#ifdef __ANDROID__
//	ion_dump_backtrace("tz");
#endif
}

static int opened = 0;		/* opened file descriptors */

int tzc_open(const char *dev)
{
	if (NULL == dev)
		dev = TZ_CLIENT_DEVICE_FULL_PATH;

	if (opened++ > CONFIG_TZ_CLIENT_MAX_FILE_DESCRIPTORS) {
		warn("open too many /dev/tz device file descriptors\n");
		dump_backtrace();
	}

	return open(dev, O_RDWR);
}

void tzc_close(int tzc)
{
	opened--;
	close(tzc);
}

void *tzc_alloc_shm(int tzc, size_t size, void **pa)
{
	void *va;
	int ret;

	*pa = (void *)size;

	ret = ioctl(tzc, TZ_CLIENT_IOCTL_ALLOC_MEM, pa);

	if (ret != 0) {
		dump_backtrace();
		*pa = NULL;
		return NULL;
	}
	va = mmap(0, size,
			PROT_READ | PROT_WRITE, MAP_SHARED,
			tzc, (long)(*pa));

	if (va == MAP_FAILED) {
		ioctl(tzc, TZ_CLIENT_IOCTL_FREE_MEM, (long)(*pa));
		*pa = NULL;
		va = NULL;
	}
	return va;
}

void tzc_free_shm(int tzc, void *va, void *pa, size_t size)
{
	munmap(va, size);
	ioctl(tzc, TZ_CLIENT_IOCTL_FREE_MEM, pa);
}

int tzc_invoke_command(int tzc, int task_id, uint32_t cmd_id,
			uint32_t param, uint32_t *origin,
			tz_cmd_handler callback, void *userdata)
{
	struct tz_nw_comm cc;
	int ret;

	cc.call.task_id = task_id;
	cc.call.cmd_id = cmd_id;
	cc.call.param = param;

	while (1) {
		ret = ioctl(tzc, TZ_CLIENT_IOCTL_CMD, &cc);
		if (ret != (int)TZ_PENDING)
			break;

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

	if (origin)
		*origin = cc.call.origin;

	return cc.call.result;
}

void *tzc_get_mem_info(int tzc, void *va, uint32_t *attr)
{
	int ret;
	struct tz_mem_info info;

	info.va = va;

	ret = ioctl(tzc, TZ_CLIENT_IOCTL_GET_MEMINFO, &info);
	if (ret != 0) {
		*attr = 0;
		return 0;
	}
	*attr = info.attr;
	return info.pa;
}

void *tzc_mutex_create(void)
{
	pthread_mutex_t *lock = malloc(sizeof(pthread_mutex_t));
	if (NULL == lock) {
		error("fail to allocate memory\n");
		return NULL;
	}

	int res = pthread_mutex_init(lock, NULL);
	if (res != 0) {
		error("system error, fail to init mutex\n");
		free(lock);
		return NULL;
	}
	return lock;
}

int tzc_mutex_destroy(void *lock)
{
	int res = pthread_mutex_destroy(lock);
	if (res != 0) {
		error("system error, fail to destroy mutex\n");
		return TZ_ERROR_GENERIC;
	}
	free(lock);
	return TZ_SUCCESS;
}

int tzc_mutex_lock(void *lock)
{
	int res = pthread_mutex_lock(lock);
	if (res != 0) {
		error("system error, fail to lock\n");
		return TZ_ERROR_GENERIC;
	}
	return TZ_SUCCESS;
}

int tzc_mutex_trylock(void *lock)
{
	int res = pthread_mutex_trylock(lock);

	if (res == 0)
		return TZ_SUCCESS;
	else if (res == EBUSY)
		return TZ_ERROR_BAD_STATE;
	else {
		error("system error\n");
		return TZ_ERROR_GENERIC;
	}
}

int tzc_mutex_unlock(void *lock)
{
	int res = pthread_mutex_unlock(lock);
	if (res != 0) {
		error("system error, fail to unlock\n");
		return TZ_ERROR_GENERIC;
	}
	return TZ_SUCCESS;
}

#ifdef CONFIG_TEE

struct tee_comm *tzc_acquire_tee_comm_channel(int tzc)
{
	void *pa = NULL;
	struct tee_comm *tc = tzc_alloc_shm(tzc, sizeof(struct tee_comm), &pa);

	if (tc) {
		tc->pa = (uint32_t)((unsigned long)pa);
		tc->va = (uint32_t)((unsigned long)tc);
	}

	return tc;
}

void tzc_release_tee_comm_channel(int tzc, struct tee_comm *tc)
{
	tzc_free_shm(tzc, (void*)tc->va, (void*)tc->pa, sizeof(*tc));
}
#endif /* CONFIG_TEE */


int tzc_fast_memmove(int tzc, void *dst_phy_addr, void *src_phy_addr, size_t size)
{
	int ret;
	struct tz_memmove_param param;

	param.dst = dst_phy_addr;
	param.src = src_phy_addr;
	param.size = size;

	ret = ioctl(tzc, TZ_CLIENT_IOCTL_FASTCALL_MEMMOVE, &param);

	if (ret == 0)
		return TZ_SUCCESS;
	else if (ret == -EACCES)
		return TZ_ERROR_ACCESS_DENIED;
	else if (ret == -EINVAL)
		return TZ_ERROR_NOT_SUPPORTED;
	else
		return TZ_ERROR_GENERIC;
}

static int tzc_fast_secure_cache_op(int tzc, void *phy_addr, size_t size, int op)
{
	int ret;
	struct tz_cache_param param;

	param.start = phy_addr;
	param.size = size;

	ret = ioctl(tzc, op, &param);

	if (ret == 0)
		return TZ_SUCCESS;
	else if (ret == -EACCES)
		return TZ_ERROR_ACCESS_DENIED;
	else if (ret == -EINVAL)
		return TZ_ERROR_NOT_SUPPORTED;
	else
		return TZ_ERROR_GENERIC;
}

int tzc_fast_secure_cache_clean(int tzc, void *phy_addr, size_t size)
{
	return tzc_fast_secure_cache_op(tzc, phy_addr, size,
			TZ_CLIENT_IOCTL_FASTCALL_CACHE_CLEAN);
}

int tzc_fast_secure_cache_invalidate(int tzc, void *phy_addr, size_t size)
{
	return tzc_fast_secure_cache_op(tzc, phy_addr, size,
			TZ_CLIENT_IOCTL_FASTCALL_CACHE_INVALIDATE);
}

int tzc_fast_secure_cache_flush(int tzc, void *phy_addr, size_t size)
{
	return tzc_fast_secure_cache_op(tzc, phy_addr, size,
			TZ_CLIENT_IOCTL_FASTCALL_CACHE_FLUSH);
}

int tzc_create_instance(int tzc, unsigned long param, uint32_t *origin)
{
	int ret;
	struct tz_instance_param create_param;

	create_param.param = param;
	ret = ioctl(tzc, TZ_CLIENT_IOCTL_CREATE_INSTANCE, &create_param);
	if(ret != 0) {
		error("TZ_CLIENT_IOCTL_CREATE_INSTANCE error\n");
	}
	*origin = create_param.origin;
	return create_param.result;
}

int tzc_destroy_instance(int tzc, unsigned long param, uint32_t *origin)
{
	int ret;
	struct tz_instance_param destroy_param;

	destroy_param.param = param;
	ret = ioctl(tzc, TZ_CLIENT_IOCTL_DESTROY_INSTANCE, &destroy_param);
	if(ret != 0) {
		error("TZ_CLIENT_IOCTL_DESTROY_INSTANCE error\n");
	}
	*origin = destroy_param.origin;
	return destroy_param.result;
}

int tzc_open_session(int tzc, unsigned long param, uint32_t taskId,
			uint32_t *origin)
{
	int ret;
	struct tz_session_param open_param;

	open_param.param = param;
	open_param.task_id = taskId;
	ret = ioctl(tzc, TZ_CLIENT_IOCTL_OPEN_SESSION, &open_param);
	if(ret != 0) {
		error("TZ_CLIENT_IOCTL_OPEN_SESSION error\n");
	}
	*origin = open_param.origin;
	return open_param.result;
}

int tzc_close_session(int tzc, unsigned long param, uint32_t taskId,
			uint32_t *origin)
{
	int ret;
	struct tz_session_param close_param;

	close_param.param = param;
	close_param.task_id = taskId;
	ret = ioctl(tzc, TZ_CLIENT_IOCTL_CLOSE_SESSION, &close_param);
	if(ret != 0) {
		error("TZ_CLIENT_IOCTL_CLOSE_SESSION error\n");
	}
	*origin = close_param.origin;
	return close_param.result;
}
