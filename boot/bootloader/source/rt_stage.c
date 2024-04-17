#include "rt_stage.h"
#include "com_type.h"
#include "bootloader.h"

extern unsigned int __rt_bl_stage_descs_start__;
extern unsigned int __rt_bl_stage_descs_end__;

#define RT_BL_DESCS_START (&__rt_bl_stage_descs_start__)
#define RT_BL_DESCS_END (&__rt_bl_stage_descs_end__)
#define RT_BL_DESCS_NUM (((unsigned int)(long)(RT_BL_DESCS_END) \
		- (unsigned int)(long)(RT_BL_DESCS_START)) \
		/ sizeof(rt_stage_desc_t))

static rt_stage_desc_t *rt_stage_descs;

unsigned int get_rt_bl_descs_nums(void)
{
	return RT_BL_DESCS_NUM;
}
rt_stage_desc_t * get_rt_stage_desc(int index)
{
	if (RT_BL_DESCS_NUM == 0)
		return NULL;
	if (index >= RT_BL_DESCS_NUM)
		return NULL;
	rt_stage_descs = (rt_stage_desc_t *)RT_BL_DESCS_START;
	return &rt_stage_descs[index];
}
