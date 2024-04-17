#ifndef _TZ_NW_SYS_CALLBACK_H_
#define _TZ_NW_SYS_CALLBACK_H_

#include "tz_comm.h"

tz_errcode_t tz_nw_register_sys_callback(uint32_t cmd_id,
			tz_cmd_handler handler, void *userdata);

tz_errcode_t tz_nw_unregister_sys_callback(uint32_t cmd_id);

uint32_t tz_nw_sys_callback(void *userdata, uint32_t cmd_id,
				uint32_t param, uint32_t *p_origin);

#endif /* _TZ_NW_SYS_CALLBACK_H_ */