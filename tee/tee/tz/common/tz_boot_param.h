#ifndef _TZ_BOOT_PARAM_H_
#define _TZ_BOOT_PARAM_H_

#include "types.h"

/*
 * setup & retrieve back boot param
 *
 * note: bellow functions are not thread safe.
 */

/*
 * boot parameter definition is platform specified.
 * different platform may have different boot parameters.
 */
enum tz_boot_param_id {
	TZ_BOOT_PARAM_USER_START	= 0x00000000,
	TZ_BOOT_PARAM_SYS_START		= 0x80000000,
	TZ_BOOT_PARAM_CMDLINE,
	TZ_BOOT_PARAM_MEMREGION,
	TZ_BOOT_PARAM_MEMZONE,
	TZ_BOOT_PARAM_REGFIREWALL,
	TZ_BOOT_PARAM_MASTER,
	TZ_BOOT_PARAM_ANTIROLLBACK,
	TZ_BOOT_PARAM_TRUST_STORAGE,
	TZ_BOOT_PARAM_REG_REGION,
	TZ_BOOT_PARAM_TA_INFO,
	TZ_BOOT_PARAM_PERM_OBJECT,
	TZ_BOOT_PARAM_PERM_RULE,
	TZ_BOOT_PARAM_BOOT_MODE,
	TZ_BOOT_PARAM_MAX
};

#define TZ_BOOT_PARAM_MAGIC_NUM		(0x545a2a42)	/* 'TZ*B' */
#define TZ_BOOT_PARAM_VERSION		(0)

/*
 * boot parameter format
 *
 * +----------------------------+
 * | tz_boot_param_header	|
 * +----------------------------+
 * | tz_boot_param 0		|
 * +----------------------------+
 * | tz_boot_param 1		|
 * +----------------------------+
 * | ...			|
 * +----------------------------+
 */
struct tz_boot_param_header {
	uint32_t	magic_num;	/* 'TZ*B' */
	uint32_t	size;		/* size of whole boot parameters */
	uint32_t	version;
	uint32_t	reserved;
};

struct tz_boot_param {
	uint32_t	size;		/* current boot param size, including header */
	uint32_t	id;
	uint32_t	reserved[2];	/* must be 0 */
	uint32_t	data[0];
};

struct tz_boot_param_list {
	uint32_t	id;		/* tz_boot_param_id */
	const void	*data;	/* point to boot parameter array */
	uint32_t	size;	/* size of data */
};

#define TRUSTED_STORAGE_CHECK_DEFALUT	(0x00000000)
#define TRUSTED_STORAGE_CHECK_LEGACY	(0x00000001)
#define TRUSTED_STORAGE_EMMC_RPMB	(0x00000002)
/* struct to surpport diff trust storage cfg */
struct trust_storage_cfg {
	uint32_t	flags;			/* whether need check legacy folder and
						 * whether use emmc rpmb for trust storage
						 */
	char		base_folder[32];	/* base folder for trust storage */
};

/** initialize the boot parameter structure.
 *
 * @param addr		address of boot parameter
 * @param reset		whether to reset the boot parameters.
 * 			when create the boot parameter, then must be true.
 * 			when parse the boot parameter, must be false.
 */
void tz_boot_param_init(void *addr, bool reset);

/** add a new boot parameter.
 *
 * @param id		ID of the parameter, see tz_boot_param_id.
 * @param data		data of the parameter.
 * @param size		size of the parameter data.
 *
 * @return pointer the to new boot parameter structure.
 */
struct tz_boot_param *tz_boot_param_add(uint32_t id,
		void *data, uint32_t size);

/** get boot parameter data by ID.
 *
 * @param id		id of the parameter, see tz_boot_param_id.
 *
 * @return pointer the to new boot parameter structure.
 */
const struct tz_boot_param *tz_boot_param_get(uint32_t id);

#endif /* _TZ_BOOT_PARAM_H_ */
