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
#include <linux/kernel.h>
#include <linux/slab.h>
#include "config.h"
#include "ree_sys_callback_cmd.h"
#include "ree_sys_callback.h"
#include "log.h"

static REE_SemaphoreList semList;

void REE_SemaphoreInit(void)
{
	semList.count = 0;
	INIT_LIST_HEAD(&semList.head);
	spin_lock_init(&semList.lock);
}

TEE_Result REESysCmd_SemaphoreCreate(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Semaphore *reeSem;
	REE_SemaphoreCreateParam *param = param_ext;

	if (param_ext_size != sizeof(REE_SemaphoreCreateParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&semList.lock);
	if (*param->name) {
		list_for_each_entry(reeSem, &semList.head, node) {
			if (!strncmp(reeSem->name, param->name,
					REE_SEMAPHORE_NAME_MAX_LEN)) {
				param->sem = reeSem;
				reeSem->refCount++;
				spin_unlock(&semList.lock);
				return TEEC_SUCCESS;
			}
		}
	}
	spin_unlock(&semList.lock);

	reeSem = kmalloc(sizeof(REE_Semaphore), GFP_KERNEL);
	if (!reeSem)
		return TEEC_ERROR_OUT_OF_MEMORY;

	sema_init(&reeSem->sem, param->value);
	reeSem->refCount = 1;
	strncpy(reeSem->name, param->name, REE_SEMAPHORE_NAME_MAX_LEN);
	reeSem->name[REE_SEMAPHORE_NAME_MAX_LEN-1] = 0;

	spin_lock(&semList.lock);
	semList.count++;
	list_add_tail(&reeSem->node, &semList.head);
	param->sem = reeSem;
	spin_unlock(&semList.lock);

	return TEEC_SUCCESS;
}

TEE_Result REESysCmd_SemaphoreDestroy(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Semaphore *reeSem, *next;
	void **semParam = param_ext;
	int refcount;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&semList.lock);
	list_for_each_entry_safe(reeSem, next, &semList.head, node) {
		if (reeSem == *semParam) {
			refcount = --reeSem->refCount;
			if (!refcount) {
				list_del(&reeSem->node);
				semList.count--;
			}
			spin_unlock(&semList.lock);
			if (!refcount)
				kfree(reeSem);
			return TEEC_SUCCESS;
		}
	}
	spin_unlock(&semList.lock);
	return TEEC_ERROR_ITEM_NOT_FOUND;
}

TEE_Result REESysCmd_SemaphoreWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Semaphore *reeSem;
	void **semParam = param_ext;
	int found = 0;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&semList.lock);
	list_for_each_entry(reeSem, &semList.head, node) {
		if (reeSem == *semParam) {
			found = 1;
			break;
		}
	}
	spin_unlock(&semList.lock);

	if (!found)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	/*
	 * If sleep is interrupted by a signal, down_interruptible will return -EINTR.
	 * If the semaphore is successfully acquired, down_interruptible returns 0.
	 * No other return value.
	 */
	while (-EINTR == down_interruptible(&reeSem->sem));
	return TEEC_SUCCESS;
}

TEE_Result REESysCmd_SemaphoreTimedWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Semaphore *reeSem;
	REE_SemaphoreTimedWaitParam *param = param_ext;
	int found = 0;

	if (param_ext_size != sizeof(REE_SemaphoreTimedWaitParam))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&semList.lock);
	list_for_each_entry(reeSem, &semList.head, node) {
		if (reeSem == param->sem) {
			found = 1;
			break;
		}
	}
	spin_unlock(&semList.lock);

	if (!found)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	/*
	 * FIXME: support other value later
	 */
	if (param->timeout != 0xffffffff)
		return TEEC_ERROR_NOT_SUPPORTED;

	/*
	 * If sleep is interrupted by a signal, down_interruptible will return -EINTR.
	 * If the semaphore is successfully acquired, down_interruptible returns 0.
	 * No other return value.
	 */
	while (-EINTR == down_interruptible(&reeSem->sem));
	return TEEC_SUCCESS;
}

TEE_Result REESysCmd_SemaphoreTryWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Semaphore *reeSem;
	void **semParam = param_ext;
	int found = 0;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&semList.lock);
	list_for_each_entry(reeSem, &semList.head, node) {
		if (reeSem == *semParam) {
			found = 1;
			break;
		}
	}
	spin_unlock(&semList.lock);

	if (!found)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	if (down_trylock(&reeSem->sem))
		return TEEC_ERROR_BAD_STATE;

	return TEEC_SUCCESS;
}

TEE_Result REESysCmd_SemaphorePost(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Semaphore *reeSem;
	void **semParam = param_ext;
	int found = 0;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&semList.lock);
	list_for_each_entry(reeSem, &semList.head, node) {
		if (reeSem == *semParam) {
			found = 1;
			break;
		}
	}
	spin_unlock(&semList.lock);

	if (!found)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	up(&reeSem->sem);

	return TEEC_SUCCESS;
}
