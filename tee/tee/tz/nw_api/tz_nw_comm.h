#ifndef _TZ_NW_COMM_H_
#define _TZ_NW_COMM_H_

#include "tz_nw_task_client.h"
#include "tz_nw_comm_protocol.h"

uint32_t tz_nw_comm_invoke_command(struct tz_nw_task_client *tc,
		struct tz_nw_comm *cc, uint32_t call_id, void *call_info);

#endif /* _TZ_NW_COMM_H_ */
