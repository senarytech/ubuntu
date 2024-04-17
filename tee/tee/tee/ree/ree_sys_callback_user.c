/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2020 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#ifdef __ANDROID__
#include <sys/endian.h>
#endif
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>

#include <tee_client_const.h>
#include <tee_client_type.h>

#include "ree_sys_callback_cmd.h"
#include "ree_sys_callback_ipc_client.h"
#include "tee_comm.h"
#include "log.h"

#include <tee_internal_crypto_rpmb.h>
#include "config.h"

#ifdef DEBUG
#define dbg_printf(fmt, args...)      	printf(fmt, ##args)
#else
#define dbg_printf(fmt, ...)          	do { } while (0)
#endif

typedef struct REE_FileOpenParam
{
	char		fileName[REE_FILE_PATH_MAX_LEN + 1];
} REE_FileOpenParam;

typedef struct REE_FolderMakeParam
{
	char		folderPath[REE_FOLDER_PATH_MAX_LEN + 1];
} REE_FolderMakeParam;

typedef struct REE_FolderReadParam
{
	bool		isdir;
	char		name[REE_FOLDER_PATH_MAX_LEN + 1];
} REE_FolderReadParam;

/*
 * Command functions
 */

static TEEC_Result REESysCmd_Wait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	usleep(params[0].value.a * 1000);

	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_MutexCreate(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_MutexCreateParam *param = param_ext;

	pthread_mutex_t *lock = malloc(sizeof(pthread_mutex_t));
	if (NULL == lock) {
		error("fail to allocate memory\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	int res = pthread_mutex_init(lock, NULL);
	if (res != 0) {
		error("system error, fail to init mutex\n");
		free(lock);
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	param->lock = lock;

	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_MutexDestroy(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	pthread_mutex_t *lock = *((void **)param_ext);

	int res = pthread_mutex_destroy(lock);
	if (res != 0) {
		error("system error, fail to destroy mutex\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	free(lock);
	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_MutexLock(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	pthread_mutex_t *lock = *((void **)param_ext);
	int res = pthread_mutex_lock(lock);
	if (res != 0) {
		error("system error, fail to lock\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_MutexTryLock(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	pthread_mutex_t *lock = *((void **)param_ext);

	int res = pthread_mutex_trylock(lock);
	if (res == 0)
		return TEEC_SUCCESS;
	else if (res == EBUSY)
		return TEEC_ERROR_BAD_STATE;
	else {
		error("system error\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
}

static TEEC_Result REESysCmd_MutexUnlock(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	pthread_mutex_t *lock = *((void **)param_ext);

	int res = pthread_mutex_unlock(lock);
	if (res != 0) {
		error("system error, fail to unlock\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_SemaphoreCreate(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_SemaphoreCreateParam *param = param_ext;

	if (param_ext_size != sizeof(REE_SemaphoreCreateParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	sem_t *sem = malloc(sizeof(sem_t));
	if (NULL == sem) {
		error("fail to allocate memory\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	if (sem_init(sem, 0, param->value) != 0) {
		error("system error, fail to init semaphore\n");
		free(sem);
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	param->sem = sem;

	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_SemaphoreDestroy(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	sem_t *sem = *((void **)param_ext);

	if (sem_destroy(sem) != 0) {
		switch (errno) {
		case EINVAL:
			return TEEC_ERROR_BAD_PARAMETERS;
		case EBUSY:
			return TEEC_ERROR_BAD_STATE;
		default:
			return TEEC_ERROR_GENERIC;
		}
	}

	free(sem);
	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_SemaphoreWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	sem_t *sem = *((void **)param_ext);

	while (sem_wait(sem) != 0) {
		switch (errno) {
		case EINTR:
			continue;
		case EINVAL:
			return TEEC_ERROR_BAD_PARAMETERS;
		case EDEADLK:
			return TEEC_ERROR_BAD_STATE;
		default:
			return TEEC_ERROR_GENERIC;
		}
	}
	return TEEC_SUCCESS;
}

static void clock_addtime(struct timespec *ts, unsigned long msec)
{
	ts->tv_sec += msec / 1000;
	ts->tv_nsec += (msec % 1000) * 1000000;
	if (ts->tv_nsec >= 1000000000L) {
		ts->tv_nsec -= 1000000000L;
		ts->tv_sec++;
	}
}

static TEEC_Result REESysCmd_SemaphoreTimedWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_SemaphoreTimedWaitParam *param = param_ext;
	sem_t *sem = param->sem;

	struct timespec ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	clock_addtime(&ts, param->timeout);

	while (sem_timedwait(sem, &ts) != 0) {
		switch (errno) {
		case EINTR:
			continue;
		case ETIMEDOUT:
			return TEEC_ERROR_BUSY;
		case EINVAL:
			return TEEC_ERROR_BAD_PARAMETERS;
		case EDEADLK:
			return TEEC_ERROR_ACCESS_DENIED;
		default:
			return TEEC_ERROR_GENERIC;
		}
	}

	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_SemaphoreTryWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	sem_t *sem = *((void **)param_ext);
	while (sem_trywait(sem) != 0) {
		switch (errno) {
		case EINTR:
			continue;
		case EINVAL:
			return TEEC_ERROR_BAD_PARAMETERS;
		case EAGAIN:
			return TEEC_ERROR_BAD_STATE;
		case EDEADLK:
			return TEEC_ERROR_ACCESS_DENIED;
		default:
			return TEEC_ERROR_GENERIC;
		}
	}

	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_SemaphorePost(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	sem_t *sem = *((void **)param_ext);

	if (sem_post(sem) != 0) {
		switch (errno) {
		case EINVAL:
			return TEEC_ERROR_BAD_PARAMETERS;
		default:
			return TEEC_ERROR_GENERIC;
		}
	}
	return TEEC_SUCCESS;

}

int teeRequestCallback(int proc_result,
		void *out_response, int response_len,
		void *out_param, int out_len)
{
	if (response_len > 0) {
		if (response_len > out_len)
			return TEEC_ERROR_SHORT_BUFFER;
		if (out_response && out_param)
			memcpy(out_param, out_response, response_len);
	}

	return proc_result;
}

static TEEC_Result REESysCmd_RPMBReadSize(
		void		*userdata,
		uint32_t	paramTypes,
		TEE_Param	params[4],
		void		*param_ext,
		uint32_t	param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	unsigned int cnt = 0;

	REE_RPMBReadCounterParam *param =
			    (REE_RPMBReadCounterParam *)(param_ext);

	//res = Do_RPMBReadSize(&cnt);
	res = REE_ClientProcessIpcRequest(REE_CMD_RPMB_READ_SIZE, &cnt,
			sizeof(cnt), &cnt, sizeof(cnt), teeRequestCallback);
	if (res != 0)
		res = -EFAULT;

	param->cnt = cnt;

	return res;
}

static TEEC_Result REESysCmd_RPMBReadBlock(
		void		*userdata,
		uint32_t	paramTypes,
		TEE_Param	params[4],
		void		*param_ext,
		uint32_t	param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;

	struct rpmb_data_frame frameIn;
	REE_RPMBReadBlockParam *param = (REE_RPMBReadBlockParam *)(param_ext);
	memcpy(&frameIn, param->frameInOut, sizeof(struct rpmb_data_frame));

	res = REE_ClientProcessIpcRequest(REE_CMD_RPMB_READ_BLOCK, &frameIn,
			sizeof(struct rpmb_data_frame), param->frameInOut,
			sizeof(struct rpmb_data_frame), teeRequestCallback);
	return res;
}

static TEEC_Result REESysCmd_RPMBWriteBlock(
		void		*userdata,
		uint32_t	paramTypes,
		TEE_Param	params[4],
		void		*param_ext,
		uint32_t	param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;

	struct rpmb_data_frame frameIn;
	REE_RPMBWriteBlockParam *param = (REE_RPMBWriteBlockParam *)(param_ext);
	memcpy(&frameIn, param->frameInOut, sizeof(struct rpmb_data_frame));

	res = REE_ClientProcessIpcRequest(REE_CMD_RPMB_WRITE_BLOCK,
			&frameIn, sizeof(struct rpmb_data_frame),
			param->frameInOut, sizeof(struct rpmb_data_frame),
			teeRequestCallback);
	return res;
}

static TEEC_Result REESysCmd_FileClose(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint64_t *filp = param_ext;
	uint32_t fd;

	fd = (*filp) & 0xffffffff;
	res = REE_ClientProcessIpcRequest(REE_CMD_FILE_CLOSE,
			&fd,
			sizeof(fd),
			NULL,
			0,
			teeRequestCallback);
	dbg_printf("file %d close\n", fd);
	return res;
}

static TEEC_Result REESysCmd_FileSeek(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FileSeekParam *param = param_ext;

	if (param_ext_size != sizeof(REE_FileSeekParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	res = REE_ClientProcessIpcRequest(REE_CMD_FILE_SEEK,
			param,
			sizeof(REE_FileSeekParam),
			param,
			sizeof(REE_FileSeekParam),
			teeRequestCallback);
	dbg_printf("file seek to %d, res 0x%x\n", param->pos, res);
	return res;
}
static TEEC_Result REESysCmd_FolderMake(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FolderMakeParam *param = param_ext;
	IPC_FolderMakeParam ipcParam;

	if (param_ext_size != sizeof(REE_FolderMakeParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	ipcParam.flags = params[0].value.a;
	memcpy(ipcParam.folderPath, param->folderPath, sizeof(ipcParam.folderPath));
	res = REE_ClientProcessIpcRequest(REE_CMD_FOLDER_MAKE,
			&ipcParam,
			sizeof(IPC_FolderMakeParam),
			NULL,
			0,
			teeRequestCallback);

	dbg_printf("folder make %s flags 0x%x, return 0x%x\n", ipcParam.folderPath, ipcParam.flags, res);
	return res;
}

static TEEC_Result REESysCmd_FileWrite(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FileReadWriteParam *param = param_ext;

	if (param_ext_size != sizeof(REE_FileReadWriteParam) + param->size) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	dbg_printf("file write fd is 0x%llx, size is %d\n", param->filp, param->size);
	res = REE_ClientProcessIpcRequest(REE_CMD_FILE_WRITE,
			param,
			sizeof(REE_FileReadWriteParam) + param->size,
			param,
			sizeof(REE_FileReadWriteParam),
			teeRequestCallback);
	dbg_printf("file fd %llx write %d bytes, buf len %d, res 0x%x, stuct len %d\n",
					param->filp,
					param->ret,
					param->size,
					res,
					param->offset);

	return res;
}

static TEEC_Result REESysCmd_FileRead(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FileReadWriteParam *param = param_ext;

	if (param_ext_size != sizeof(REE_FileReadWriteParam) + param->size) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	dbg_printf("file read fd is 0x%llx, size is %d\n", param->filp, param->size);
	res = REE_ClientProcessIpcRequest(REE_CMD_FILE_READ,
			param,
			sizeof(REE_FileReadWriteParam) + param->size,
			param,
			sizeof(REE_FileReadWriteParam) + param->size,
			teeRequestCallback);

	dbg_printf("file fd 0x%llx read %d bytes, buf len %d, res 0x%x\n", param->filp,
					param->ret,
					param->size,
					res);
	return res;
}

static TEEC_Result REESysCmd_FileOpen(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FileOpenParam *param = param_ext;
	uint32_t tee_flags = params[0].value.a;
	IPC_fileOpenParam ipcParam;

	if (param_ext_size != sizeof(REE_FileOpenParam)) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	memcpy(ipcParam.fileName, param->fileName, sizeof(ipcParam.fileName));
	ipcParam.flags = tee_flags;
	res = REE_ClientProcessIpcRequest(REE_CMD_FILE_OPEN,
			&ipcParam,
			sizeof(IPC_fileOpenParam),
			&ipcParam,
			sizeof(IPC_fileOpenParam),
			teeRequestCallback);

	params[1].value.a = 0;
	params[1].value.b = ipcParam.fd;
	params[0].value.b = ipcParam.size;

	dbg_printf("open file %s, fd is %d, size is %d, res 0x%x\n", ipcParam.fileName,
							ipcParam.fd,
							ipcParam.size,
							res);
	return res;
}
static TEEC_Result REESysCmd_FileDelete(
		void*                   userdata,
		uint32_t                paramTypes,
		TEE_Param               params[4],
		void*                   param_ext,
		uint32_t                param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FolderMakeParam *param = param_ext;

	if (param_ext_size != sizeof(REE_FolderMakeParam)) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	res = REE_ClientProcessIpcRequest(REE_CMD_FILE_DELETE, param->folderPath,
			sizeof(param->folderPath), NULL,
			0, teeRequestCallback);
	dbg_printf("delete file %s, res 0x%x\n", param->folderPath,res);
	return res;
}

static TEEC_Result REESysCmd_DirOpen(
		void		*userdata,
		uint32_t	paramTypes,
		TEE_Param	params[4],
		void		*param_ext,
		uint32_t	param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FolderMakeParam *param = (REE_FolderMakeParam* )param_ext;
	IPC_FolderMakeParam ipcParam={0};
	uintptr_t dir = 0;

	if (param_ext_size != sizeof(REE_FolderMakeParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	memcpy(ipcParam.folderPath, param->folderPath, sizeof(ipcParam.folderPath));
	res = REE_ClientProcessIpcRequest(REE_CMD_FOLDER_OPEN,
			&ipcParam,
			sizeof(IPC_FolderMakeParam),
			&ipcParam,
			sizeof(IPC_FolderMakeParam),
			teeRequestCallback);
	dbg_printf("folder open %s, return 0x%x\n", ipcParam.folderPath, res);

	if (!ipcParam.dir)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	dir = ipcParam.dir;
	dbg_printf("folder open dir = 0x%x ipcParam.dir=0x%x\n", dir, ipcParam.dir);

	params[0].value.b = (uint64_t)dir >> 32;
	params[0].value.a = dir;
	dbg_printf("folder open: b = 0x%x, a = 0x%x\n", params[0].value.b, params[0].value.a);

	return res;
}

static TEEC_Result REESysCmd_DirRead(
		void		*userdata,
		uint32_t	paramTypes,
		TEE_Param	params[4],
		void		*param_ext,
		uint32_t	param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	REE_FolderReadParam* param = (REE_FolderReadParam*)param_ext;
	IPC_FolderReadParam ipcParam = {0};
	uintptr_t dir = (uint64_t)params[0].value.b << 32 | params[0].value.a;

	dbg_printf("folder read: dir = 0x%x\n", dir);

	if (param_ext_size != sizeof(REE_FolderReadParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	memcpy(ipcParam.name, param->name, sizeof(ipcParam.name));

	ipcParam.dir = dir;
	res = REE_ClientProcessIpcRequest(REE_CMD_FOLDER_READ,
			&ipcParam,
			sizeof(IPC_FolderReadParam),
			&ipcParam,
			sizeof(IPC_FolderReadParam),
			teeRequestCallback);
	param->isdir = ipcParam.isdir;
	memcpy(param->name, ipcParam.name, sizeof(ipcParam.name));

	dbg_printf("folder read param name %s,isdir = %d, return 0x%x\n", param->name, param->isdir, res);
	return res;

}

static TEEC_Result REESysCmd_DirClose(
		void		*userdata,
		uint32_t	paramTypes,
		TEE_Param	params[4],
		void		*param_ext,
		uint32_t	param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uintptr_t dir = (uint64_t)params[0].value.b << 32 | params[0].value.a;

	dbg_printf("dir 0x%x close\n", dir);

	res = REE_ClientProcessIpcRequest(REE_CMD_FOLDER_CLOSE,
			&dir,
			sizeof(dir),
			NULL,
			0,
			teeRequestCallback);

	return res;
}

TEEC_Result REE_InvokeSysCommandEntryPoint(
		void*			sessionContext,
		uint32_t		commandID,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_NOT_SUPPORTED;

	trace("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
		case REE_CMD_TIME_WAIT:
			res = REESysCmd_Wait(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_RPMB_READ_COUNTER:
		case REE_CMD_RPMB_READ_BLOCK:
			res = REESysCmd_RPMBReadBlock(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_RPMB_WRITE_KEY:
		case REE_CMD_RPMB_WRITE_BLOCK:
			res = REESysCmd_RPMBWriteBlock(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_RPMB_READ_SIZE:
			res = REESysCmd_RPMBReadSize(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FOLDER_OPEN:
			res = REESysCmd_DirOpen(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FOLDER_READ:
			res = REESysCmd_DirRead(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FOLDER_CLOSE:
			res = REESysCmd_DirClose(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FILE_OPEN:
			res = REESysCmd_FileOpen(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FILE_CLOSE:
			res = REESysCmd_FileClose(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FILE_READ:
			res = REESysCmd_FileRead(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FILE_WRITE:
			res = REESysCmd_FileWrite(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FILE_SEEK:
			res = REESysCmd_FileSeek(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FILE_DELETE:
			res = REESysCmd_FileDelete(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
		case REE_CMD_FOLDER_MAKE:
			res = REESysCmd_FolderMake(sessionContext, paramTypes,
					params, param_ext, param_ext_size);
			break;
	}

	trace("exit: param_ext=%08x, param_ext_size=%d\n", param_ext, param_ext_size);
	return res;
}
