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
#include <unistd.h>
#ifdef __ANDROID__
#include <sys/endian.h>
#endif
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>

#include "tee_comm.h"
#include "rpmb.h"
#include "ree_ca_rpmb_op.h"

/*
 * the device name of RPMB is difference between Linux and Android
 * platform, so, need locate where it's
 */
#define LINUX_RPMB_DEV_PATH		"/dev/mmcblk0rpmb"
#define ANDROID_RPMB_DEV_PATH		"/dev/block/mmcblk0rpmb"
static char rpmb_dev_name[32] = {0};
static TEEC_Result locate_rpmb_dev_path(void)
{
	if (0 == strlen(rpmb_dev_name)) {
		if (0 == access(LINUX_RPMB_DEV_PATH, F_OK)) {
			strncpy(rpmb_dev_name, LINUX_RPMB_DEV_PATH,
					strlen(LINUX_RPMB_DEV_PATH));
		} else if (0 == access(ANDROID_RPMB_DEV_PATH, F_OK)) {
			strncpy(rpmb_dev_name, ANDROID_RPMB_DEV_PATH,
					strlen(ANDROID_RPMB_DEV_PATH));

		} else {
			perror("can't find the rpmb dev\n");
			return TEEC_ERROR_ITEM_NOT_FOUND;
		}
	}

	return TEEC_SUCCESS;
}

/* data size should be 256 * blocks_cnt */
TEEC_Result Do_RPMBReadBlock(void *rpmbParam)
{
	uint32_t ret = TEEC_ERROR_GENERIC;
	int fd;
	struct rpmb_data_frame frameIn;
	struct rpmb_data_frame *frameOut = rpmbParam;

	memcpy(&frameIn, rpmbParam, sizeof(struct rpmb_data_frame));
	if (!frameOut) {
		perror("param null point\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	ret = locate_rpmb_dev_path();
	if (ret) {
		perror("can't find the rpmb device\n");
		return TEEC_ERROR_NOT_SUPPORTED;
	}

	fd = open(rpmb_dev_name, O_RDWR, 0);
	if (fd < 0) {
		perror("device open fail");
		return TEEC_ERROR_ACCESS_DENIED;
	}

	memset(frameOut, 0, sizeof(struct rpmb_data_frame));
	/* Execute RPMB op */
	ret = rpmb_process_request(fd, CMD_RPMB_READ, &frameIn, frameOut, 512);
	if (ret != 0) {
		perror("RPMB ioctl failed");
		ret = TEEC_ERROR_ACCESS_CONFLICT;
		goto err;
	}

	/* Check RPMB response */
	if (frameOut[0].op_result != 0) {
		printf("RPMB operation failed, retcode 0x%04x\n",
		    be16toh(frameOut[0].op_result));
		ret = TEEC_ERROR_ACCESS_DENIED;
		goto err;
	}

err:
	close(fd);
	return ret;
}
/*
Now we only support write 256 data once
So, the block_cnt should be 1
and the data size(len) should be less than 256
*/
TEEC_Result Do_RPMBWriteBlock(void *rpmbParam)
{
	uint32_t ret = TEEC_ERROR_GENERIC;
	int fd;
	struct rpmb_data_frame frameOut;
	struct rpmb_data_frame *frameIn = rpmbParam;

	if (!frameIn) {
		printf("param null point\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}
	if (0 == strlen(rpmb_dev_name)) {
		printf("can't find the rpmb dev\n");
		return TEEC_ERROR_ITEM_NOT_FOUND;
	}

	fd = open(rpmb_dev_name, O_RDWR, 0);
	if (fd < 0) {
		perror("device open fail");
		return TEEC_ERROR_ACCESS_DENIED;
	}

	/* Execute RPMB op */
	ret = rpmb_process_request(fd, CMD_RPMB_WRITE, frameIn, &frameOut, 512);
	if (ret != 0) {
		perror("RPMB ioctl failed");
		ret = TEEC_ERROR_ACCESS_CONFLICT;
		goto err;
	}

	/* Check RPMB response */
	if (frameOut.op_result != 0) {
		printf("RPMB operation failed, retcode 0x%04x\n",
				be16toh(frameOut.op_result));
		ret = TEEC_ERROR_ACCESS_DENIED;
		goto err;
	}

	memcpy(frameIn, &frameOut, sizeof(frameOut));

err:
	close(fd);
	return ret;
}

TEEC_Result Do_RPMBWriteKey(void *rpmbParam)
{
	uint32_t ret = TEEC_ERROR_GENERIC;
	int fd;
	struct rpmb_data_frame frameOut;
	struct rpmb_data_frame *frameIn = rpmbParam;

	if (!frameIn) {
		printf("param null point\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	ret = locate_rpmb_dev_path();
	if (ret) {
		perror("can't find the rpmb device\n");
		return TEEC_ERROR_NOT_SUPPORTED;
	}

	fd = open(rpmb_dev_name, O_RDWR, 0);
	if (fd < 0) {
		perror("device open fail");
		return TEEC_ERROR_ACCESS_DENIED;
	}

	/* Execute RPMB op */
	ret = rpmb_process_request(fd, CMD_RPMB_WRITE_KEY, frameIn, &frameOut, 512);
	if (ret != 0) {
		perror("RPMB ioctl failed");
		ret = TEEC_ERROR_ACCESS_CONFLICT;
		goto err;
	}

	/* Check RPMB response */
	if (frameOut.op_result != 0) {
		printf("RPMB operation failed, retcode 0x%04x\n",
				be16toh(frameOut.op_result));
		ret = TEEC_ERROR_ACCESS_DENIED;
		goto err;
	}

err:
	close(fd);
	return ret;
}

TEEC_Result Do_RPMBReadSize(void *rpmbParam)
{
	int ret = TEEC_ERROR_GENERIC;
	int fd;

	ret = locate_rpmb_dev_path();
	if (ret)
		return ret;

	fd = open(rpmb_dev_name, O_RDWR, 0);
	if (fd < 0) {
		perror("device open fail");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	/* Execute RPMB op */
	ret = rpmb_process_request(fd, CMD_RPMB_READ_SIZE, NULL, rpmbParam, 1);
	if (ret != 0) {
		perror("RPMB ioctl failed");
		ret = TEEC_ERROR_ACCESS_CONFLICT;
	}

	close(fd);
	return ret;
}
