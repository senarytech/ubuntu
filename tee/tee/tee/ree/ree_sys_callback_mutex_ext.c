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

struct mutex_wait_list {
	struct mutex mu;
	struct list_head lh;
};

struct mutex_wait {
	struct list_head link;
	struct completion comp;
	struct mutex mu;
	uint32_t wait_after;
	uint32_t key;
};

static struct mutex_wait_list tee_wait_list;

static struct mutex_wait *mutex_wait_get(uint32_t key,
					struct mutex_wait_list *wait_list)
{
	struct mutex_wait *w = NULL;

	mutex_lock(&wait_list->mu);

	list_for_each_entry(w, &wait_list->lh, link)
		if (w->key == key)
			goto out;

	w = kmalloc(sizeof(struct mutex_wait), GFP_KERNEL);
	if (!w) {
		trace(dev, "kmalloc <struct tee_mutex_wait> failed\n");
		goto out;
	}

	init_completion(&w->comp);
	mutex_init(&w->mu);
	w->wait_after = 0;
	w->key = key;
	list_add_tail(&w->link, &wait_list->lh);
out:
	mutex_unlock(&wait_list->mu);
	return w;
}

TEE_Result REESysCmd_MutexWait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	struct mutex_wait *w;
	int key = params[0].value.a;
	int task_id = params[1].value.a;

	w = mutex_wait_get(key, &tee_wait_list);
	trace("task %d lock the lock %d w is %llx\n",
			task_id, key, (void*)w);

	wait_for_completion(&w->comp);

	return TEE_SUCCESS;
}

TEE_Result REESysCmd_MutexWake(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	struct mutex_wait *w;
	int key = params[0].value.a;
	int task_id = params[1].value.a;

	w = mutex_wait_get(key, &tee_wait_list);
	trace("task %d unlock the lock %d w is %llx\n",
			task_id, key, (void*)w);

	complete(&w->comp);

	return TEE_SUCCESS;
}

TEE_Result REESysCmd_MutexDel(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	struct mutex_wait *w;
	int key = params[0].value.a;

	mutex_lock(&tee_wait_list.mu);

	list_for_each_entry(w, &tee_wait_list.lh, link) {
		if (w->key == key) {
			list_del(&w->link);
			mutex_destroy(&w->mu);
			kfree(w);
			break;
		}
	}

	mutex_unlock(&tee_wait_list.mu);

	return TEE_SUCCESS;
}

void REE_MutexInitExt(void)
{
	mutex_init(&tee_wait_list.mu);
	INIT_LIST_HEAD(&tee_wait_list.lh);
}
