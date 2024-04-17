#include "tz_driver.h"
#include <asm/memory.h>

void *tz_nw_phys_to_virt(void *call_info, void *pa)
{
	return tzd_phys_to_virt(call_info, pa);
}
