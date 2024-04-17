#ifndef _TZ_BOOT_ARG_H_
#define _TZ_BOOT_ARG_H_

#define TZ_BOOT_FLAG_RESUME		(0x00000001)
#define TZ_BOOT_FLAG_RESUME_CPU_ON	(0x00000002)
#define TZ_BOOT_FLAG_RESUME_SUSPEND_ON	(0x00000004)
#define TZ_BOOT_FLAG_RECOVERY		(0x00000080)

#ifndef __ASSEMBLY__

struct tz_boot_arg {
	unsigned long flags;		/* r0. boot flags */
	void *boot_param;		/* r1. NULL for default */
	unsigned long machine_id;	/* r2 */
	void *nw_entry;			/* r3 */
	unsigned long nw_param[4];	/* r4-r7 for nw r0-r3 */
};

#endif /* __ASSEMBLY__ */

#endif /* _TZ_BOOT_ARG_H_ */
