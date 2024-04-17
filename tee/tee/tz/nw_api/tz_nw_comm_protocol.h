#ifndef _TZ_NW_COMM_PROTOCOL_H_
#define _TZ_NW_COMM_PROTOCOL_H_

#include "types.h"

struct tz_nw_cmd {
	/* task_id: for call, it's callee; for callback, it's caller */
	uint32_t	task_id;
	uint32_t	cmd_id;
	uint32_t	param;
	uint32_t	result;
	uint32_t	origin;
};

struct tz_nw_comm {
	struct tz_nw_cmd	call;
	struct tz_nw_cmd	callback;
};

#endif /* _TZ_NW_COMM_PROTOCOL_H_ */
