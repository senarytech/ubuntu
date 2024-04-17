/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2013-2020 Synaptics Incorporated. All rights reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __ANDROID__
#include <sys/endian.h>
#endif
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>

#include <tee_internal_crypto_rpmb.h>
#include <tee_comm.h>
#include <ree_sys_callback_cmd.h>
#include "ree_ca_file_op.h"

#ifdef DEBUG
#define dbg_printf(fmt, args...)      	printf(fmt, ##args)
#else
#define dbg_printf(fmt, ...)          	do { } while (0)
#endif

TEEC_Result Do_FileSeek(void *param)
{
	int fd;
	int origin;
	REE_FileSeekParam *flSkParam = param;
	off_t offset;

	fd = flSkParam->filp & 0xffffffff;
	switch (flSkParam->whence) {
		case TEE_DATA_SEEK_SET:
			origin = SEEK_SET;
			break;
		case TEE_DATA_SEEK_CUR:
			origin = SEEK_CUR;
			break;
		case TEE_DATA_SEEK_END:
			origin = SEEK_END;
			break;
		default:
			return TEEC_ERROR_BAD_PARAMETERS;
	}
	offset = lseek(fd, flSkParam->offset, origin);
	if (offset < 0) {
		return TEEC_ERROR_ACCESS_CONFLICT;
	}
	flSkParam->pos = offset;

	return TEEC_SUCCESS;
}

TEEC_Result Do_FolderMake(void *param, char *root_dir)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	IPC_FolderMakeParam *dirMkParam = param;
	char pathname[REE_FILE_PATH_MAX_LEN + 1];
	char* midpath;

	/* replace the tee data dir(/data/tee) with root_dir */
	midpath = strstr(dirMkParam->folderPath, "/data/tee/");
	if (midpath == NULL)
		midpath = dirMkParam->folderPath;
	else
		midpath += strlen("/data/tee/");
	snprintf(pathname, REE_FILE_PATH_MAX_LEN,
			"%s/%s", root_dir,
			midpath);

	res = mkdir(pathname, dirMkParam->flags);
	if (res && (errno == EEXIST)) {
		res = TEEC_SUCCESS;
	}
	dbg_printf("fokd mk %s return 0x%x\n", pathname, res);
	return res;
}

TEEC_Result Do_FolderOpen(void *param, char *root_dir)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	IPC_FolderMakeParam *dirOpParam = param;
	char pathname[REE_FILE_PATH_MAX_LEN + 1];
	char* midpath;
	DIR* dirp;

	/* replace the tee data dir(/data/tee) with root_dir */
	midpath = strstr(dirOpParam->folderPath, "/data/tee");
	if (midpath == NULL)
		midpath = dirOpParam->folderPath;
	else
		midpath += strlen("/data/tee");

	if (midpath[0] == '/')
		midpath++;

	snprintf(pathname, REE_FILE_PATH_MAX_LEN,
			"%s/%s", root_dir,
			midpath);

	dirp = opendir(pathname);
	if (!dirp){
		res = TEEC_ERROR_ITEM_NOT_FOUND;
		perror("Fail to opendir!\n");
		goto done;
	}
	dirOpParam->dir = (uintptr_t)dirp;

	res = TEEC_SUCCESS;

done:
	dbg_printf("folder op %s return 0x%x\n", pathname, res);
	return res;
}
TEEC_Result Do_FolderRead(void *param, char *root_dir)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	IPC_FolderReadParam* dirRdParam = param;
	DIR* dirp = (DIR*)dirRdParam->dir;
	char* midpath;

	struct dirent* dirent = readdir(dirp);
	if (!dirent) {
		perror("Fail to readdir\n");
		return TEEC_ERROR_ITEM_NOT_FOUND;
	}

	midpath = strstr(dirRdParam->name, "/data/tee");
	if (midpath == NULL)
		midpath = dirRdParam->name;
	else
		midpath += strlen("/data/tee");

	if (midpath[0] == '/')
		midpath++;

	size_t len = strlen(root_dir);

	len += strlen(midpath) + 2;
	if (len > 0) {
		struct stat st;
		len += strlen(dirent->d_name) + 2;
		char* path = malloc(len);
		if (path) {
			snprintf(path, len, "%s/%s/%s",root_dir, midpath, dirent->d_name);
			if (lstat(path, &st) == 0)
				dirRdParam->isdir = S_ISDIR(st.st_mode);
			free(path);
		}
	}

	len = strlen(dirent->d_name) + 1;
	if (len > sizeof(dirRdParam->name))
		 return TEEC_ERROR_SHORT_BUFFER;

	memcpy(dirRdParam->name, dirent->d_name, len);

	return TEEC_SUCCESS;
}

TEEC_Result Do_FolderClose(void *param)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	DIR* dirp = (DIR*)*(uintptr_t*)param;

	res = closedir(dirp);
	if (res) {
		perror("close folder fail\n");
		res = TEEC_ERROR_ACCESS_CONFLICT;
	} else {
		res = TEEC_SUCCESS;
	}

	return res;

}

TEEC_Result Do_FileWrite(void *param, uint32_t *outlen)
{
	int fd;
	REE_FileReadWriteParam *flWrParam = param;

	fd = flWrParam->filp & 0xffffffff;
	dbg_printf("fw fd %d, size %d\n", fd, flWrParam->size);
	flWrParam->ret = write(fd, flWrParam->buff, flWrParam->size);
	if (flWrParam->ret < 0)
		return TEEC_ERROR_NO_DATA;
	flWrParam->offset = lseek(fd, 0, SEEK_CUR);

	fsync(fd);
	*outlen = sizeof(REE_FileReadWriteParam);
	dbg_printf("fw fd %d, write %d bytes, outlen %d\n", fd, flWrParam->ret, *outlen);
	return TEEC_SUCCESS;
}

TEEC_Result Do_FileRead(void *param)
{
	int fd;
	REE_FileReadWriteParam *flRdParam = param;

	fd = flRdParam->filp & 0xffffffff;
	flRdParam->ret = read(fd, flRdParam->buff, flRdParam->size);
	if (flRdParam->ret < 0)
		return TEEC_ERROR_NO_DATA;
	flRdParam->offset = lseek(fd, 0, SEEK_CUR);

	return TEEC_SUCCESS;
}

TEEC_Result Do_FileClose(void *param)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	uint32_t fd = *(uint32_t*)param;

	res = close(fd);
	if (res) {
		perror("close file fail\n");
		res = TEEC_ERROR_ACCESS_CONFLICT;
	} else {
		res = TEEC_SUCCESS;
	}

	return res;
}

TEEC_Result Do_FileOpen(void *param, char *root_dir)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	IPC_fileOpenParam *flOpParam = param;
	uint32_t teeFlags;
	uint32_t flags = 0;
	loff_t offset = 0;
	char pathname[REE_FILE_PATH_MAX_LEN + 1];
	char* midpath;

	if (param == NULL)
		return TEEC_ERROR_BAD_PARAMETERS;

	teeFlags = flOpParam->flags;
	switch (teeFlags & (TEE_DATA_FLAG_ACCESS_READ |
				TEE_DATA_FLAG_ACCESS_WRITE)) {
		case TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE:
			flags |= O_RDWR;
			break;
		case TEE_DATA_FLAG_ACCESS_READ:
			flags |= O_RDONLY;
			break;
		case TEE_DATA_FLAG_ACCESS_WRITE:
			flags |= O_WRONLY;
			break;
	}

	if (teeFlags & TEE_DATA_FLAG_CREATE)
		flags |= O_CREAT;

	if (teeFlags & TEE_DATA_FLAG_OVERWRITE)
		flags |= O_TRUNC;

	/* replace the tee data dir(/data/tee) with root_dir */
	midpath = strstr(flOpParam->fileName, "/data/tee/");
	if (midpath == NULL)
		midpath = flOpParam->fileName;
	else
		midpath += strlen("/data/tee/");
	snprintf(pathname, REE_FILE_PATH_MAX_LEN,
			"%s/%s", root_dir,
			midpath);

	flOpParam->fd = open(pathname, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (flOpParam->fd < 0) {
		if (flags & O_CREAT)
			res = TEEC_ERROR_ACCESS_CONFLICT;
		else
			res = TEEC_ERROR_ITEM_NOT_FOUND;
		perror("open file fail\n");
		goto done;
	}

	offset = lseek(flOpParam->fd, 0, SEEK_END);
	if (offset < 0) {
		res = TEEC_ERROR_ACCESS_CONFLICT;
		perror("Fail to seek the file\n");
		close(flOpParam->fd);
		goto done;
	}

	flOpParam->size = (uint32_t)offset;
	lseek(flOpParam->fd, 0, SEEK_SET);


	res = TEEC_SUCCESS;
done:
	dbg_printf("open file return 0x%x\n", res);
	return res;
}

TEEC_Result Do_FileDelete(void *param, char *root_dir)
{
	TEEC_Result res = TEEC_ERROR_GENERIC;
	const char *filePath = param;
	char pathname[REE_FILE_PATH_MAX_LEN + 1];
	char* midpath;

	if (NULL == param) {
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	/* replace the tee data dir(/data/tee) with root_dir */
	midpath = strstr(filePath, "/data/tee/");
	if (midpath == NULL)
		midpath = (char*)filePath;
	else
		midpath += strlen("/data/tee/");
	snprintf(pathname, REE_FILE_PATH_MAX_LEN,
			"%s/%s", root_dir,
			midpath);
	res = unlink(pathname);
	sync();

	return res;
}
