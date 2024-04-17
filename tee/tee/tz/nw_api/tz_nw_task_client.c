#include "config.h"
#include "tz_nw_task_client.h"
#include "tz_nw_sys_callback.h"
#include "log.h"

#ifdef __KERNEL__
#include <linux/string.h>
#else
#include "string.h"
#endif

static struct tz_nw_task_client tz_nw_task_client[CONFIG_TASK_NUM];

struct tz_nw_task_client *tz_nw_task_client_get(int task_id)
{
	assert(0 <= task_id && task_id < CONFIG_TASK_NUM);

	if (task_id >= CONFIG_TASK_NUM)
		return NULL;

	return &tz_nw_task_client[task_id];
}

struct tz_nw_task_client *tz_nw_task_client_init(int task_id)
{
	struct tz_nw_task_client *tc = tz_nw_task_client_get(task_id);

	memset(tc, 0, sizeof(*tc));

	tc->task_id = task_id;
	/* by default, we set default callback to tz_nw_sys_callback */
	tc->callback = tz_nw_sys_callback;
	tc->userdata = tc;

	spin_lock_init(&tc->lock);

#ifdef __KERNEL__
	init_waitqueue_head(&tc->waitq);
#endif

	return tc;
}

int tz_nw_task_client_init_all(void)
{
	int i;
	for (i = 0; i < CONFIG_TASK_NUM; i++) {
		tz_nw_task_client_init(i);
	}
	return TZ_SUCCESS;
}

/*
 * register
 */
int tz_nw_task_client_register(int task_id,
		tz_cmd_handler callback, void *userdata)
{
	struct tz_nw_task_client *tc = tz_nw_task_client_get(task_id);

	if (!tc || !callback) {
		error("can't find task %d or callback (0x%08x) is invalid\n",
				task_id, (uint32_t)callback);
		return TZ_ERROR_BAD_PARAMETERS;
	}

	if (tc->state != TZ_NW_TASK_STATE_IDLE) {
		error("task %d, bad state (%d)\n", task_id, tc->state);
		return TZ_ERROR_BAD_STATE;
	}

	tc->callback = callback;
	tc->userdata = userdata;

	return TZ_SUCCESS;
}

int tz_nw_task_client_unregister(int task_id)
{
	struct tz_nw_task_client *tc;

	trace("task_id=%d\n", task_id);

	tc = tz_nw_task_client_get(task_id);

	if (!tc)
	    return TZ_ERROR_BAD_STATE;

	if (tc->state != TZ_NW_TASK_STATE_IDLE) {
		error("task %d, bad state (%d)\n", task_id, tc->state);
		return TZ_ERROR_BAD_STATE;
	}

	memset(&tc->callback, 0, sizeof(tc->callback));

	return TZ_SUCCESS;
}
