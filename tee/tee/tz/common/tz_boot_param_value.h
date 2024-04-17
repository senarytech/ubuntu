#ifndef _TZ_BOOT_PARAM_VALUE_H_
#define _TZ_BOOT_PARAM_VALUE_H_

#include "mem_region.h"
#include "ta_info.h"
#include "tee_ta_perm_rule_def.h"

extern const struct mem_region mem_region_param[];
extern const uint32_t mem_region_num;

extern const struct reg_region reg_region_param[];
extern const uint32_t reg_region_param_num;

extern const struct mem_zone mem_zone_param[];
extern const uint32_t mem_zone_num;

extern const struct rf_window rf_window_param[];
extern const uint32_t rf_window_num;

extern const struct master_setting master_setting_param[];
extern const uint32_t master_setting_num;

extern const struct ta_info ta_info_param[];
extern const uint32_t ta_info_param_num;

extern const struct trust_storage_cfg trust_storage_cfg_param;

extern const tz_object_t g_tz_object_list[];
extern const uint32_t g_tz_object_num;
extern const tz_perm_rule_t g_tz_rule_list[];
extern const uint32_t g_tz_rule_num;

extern const uint32_t tz_boot_mode_param;

extern const struct tz_boot_param_list bp_list[];
extern const uint32_t bp_num;
#endif /* _TZ_BOOT_PARAM_VALUE_H_ */
