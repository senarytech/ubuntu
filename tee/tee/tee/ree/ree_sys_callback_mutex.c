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

static REE_MutexList mutexList;

TEEC_Result REESysCmd_MutexCreate(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Mutex *reeMutex;
	REE_MutexCreateParam *param = param_ext;

	if (param_ext_size != sizeof(REE_MutexCreateParam)) {
		trace("param_ext_size: %u, sizeof(REE_MutexCreateParam): %lu\n",
			param_ext_size, sizeof(REE_MutexCreateParam));
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	spin_lock(&mutexList.lock);
	if (*param->name) {
		list_for_each_entry(reeMutex, &mutexList.head, node) {
			if (!strncmp(reeMutex->name, param->name,
					REE_MUTEX_NAME_MAX_LEN)) {
				param->lock = reeMutex;
				reeMutex->refCount++;
				spin_unlock(&mutexList.lock);
				return TEEC_SUCCESS;
			}
		}
	}
	spin_unlock(&mutexList.lock);

	reeMutex = kmalloc(sizeof(REE_Mutex), GFP_KERNEL);
	if (!reeMutex)
		return TEEC_ERROR_OUT_OF_MEMORY;

	trace("reeMutex->mutex: %p\n", &reeMutex->mutex);
	mutex_init(&reeMutex->mutex);
	reeMutex->refCount = 1;
	strncpy(reeMutex->name, param->name, REE_MUTEX_NAME_MAX_LEN);
	reeMutex->name[REE_MUTEX_NAME_MAX_LEN-1] = 0;

	spin_lock(&mutexList.lock);
	list_add_tail(&reeMutex->node, &mutexList.head);
	mutexList.count++;
	param->lock = reeMutex;
	spin_unlock(&mutexList.lock);

	return TEEC_SUCCESS;
}

TEEC_Result REESysCmd_MutexDestroy(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Mutex *reeMutex, *next;
	void **lockParam = param_ext;
	int refcount;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&mutexList.lock);
	list_for_each_entry_safe(reeMutex, next, &mutexList.head, node) {
		if (reeMutex == *lockParam) {
			refcount = --reeMutex->refCount;
			if (!refcount) {
				list_del(&reeMutex->node);
				mutexList.count--;
			}
			spin_unlock(&mutexList.lock);
			if (!refcount)
				kfree(reeMutex);
			return TEEC_SUCCESS;
		}
	}
	spin_unlock(&mutexList.lock);

	return TEEC_ERROR_ITEM_NOT_FOUND;
}

TEEC_Result REESysCmd_MutexLock(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Mutex *reeMutex;
	void **lockParam = param_ext;
	int found = 0;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&mutexList.lock);
	list_for_each_entry(reeMutex, &mutexList.head, node) {
		if (reeMutex == *lockParam) {
			found = 1;
			break;
		}
	}
	spin_unlock(&mutexList.lock);

	if (!found)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	trace("reeMutex->mutex: %p\n", &reeMutex->mutex);

	/*
	 * If sleep is interrupted by a signal, mutex_lock_interruptible will return -EINTR.
	 * If the mutex is successfully acquired, mutex_lock_interruptible returns 0.
	 * No other return value.
	 */
	while (-EINTR == mutex_lock_interruptible(&reeMutex->mutex));
	return TEEC_SUCCESS;
}

TEEC_Result REESysCmd_MutexTryLock(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Mutex *reeMutex;
	void **lockParam = param_ext;
	int found = 0;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&mutexList.lock);
	list_for_each_entry(reeMutex, &mutexList.head, node) {
		if (reeMutex == *lockParam) {
			found = 1;
			break;
		}
	}
	spin_unlock(&mutexList.lock);

	if (!found)
		return TEEC_ERROR_ITEM_NOT_FOUND;

	if (mutex_trylock(&reeMutex->mutex))
		return TEEC_SUCCESS;

	return TEEC_ERROR_BAD_STATE;
}

TEEC_Result REESysCmd_MutexUnlock(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	REE_Mutex *reeMutex;
	void **lockParam = param_ext;

	if (param_ext_size != sizeof(void*))
		return TEEC_ERROR_BAD_PARAMETERS;

	spin_lock(&mutexList.lock);
	list_for_each_entry(reeMutex, &mutexList.head, node) {
		if (reeMutex == *lockParam) {
			spin_unlock(&mutexList.lock);
			/* unlock the mutex */
			mutex_unlock(&reeMutex->mutex);
			return TEEC_SUCCESS;
		}
	}
	spin_unlock(&mutexList.lock);

	return TEEC_ERROR_ITEM_NOT_FOUND;
}

void REE_MutexInit(void)
{
	mutexList.count = 0;
	INIT_LIST_HEAD(&mutexList.head);
	spin_lock_init(&mutexList.lock);
}
