#include <config.h>
#include <string.h>
#include "tz_boot_param.h"
#include "assert.h"
#include "log.h"

#ifndef CONFIG_BOOT_PARAM_MAX_SIZE
#define CONFIG_BOOT_PARAM_MAX_SIZE	(100 << 10)
#endif /* CONFIG_BOOT_PARAM_MAX_SIZE */

static struct tz_boot_param_header *root = NULL;

void tz_boot_param_init(void *addr, bool reset)
{
	if (!addr)
		return;

	root = addr;
	if (reset) {
		root->magic_num = TZ_BOOT_PARAM_MAGIC_NUM;
		root->size = sizeof(*root);
		root->version = TZ_BOOT_PARAM_VERSION;
		root->reserved = 0;
	}
}

struct tz_boot_param *tz_boot_param_add(uint32_t id,
		void *data, uint32_t size)
{
	if (!root)
		return NULL;

	struct tz_boot_param *curr = (void *)root + root->size;
	/* make size 4B aligned */
	size = (size + 3) & (~3);

	/* fill current parameter */
	curr->size = size + sizeof(struct tz_boot_param);
	curr->id = id;
	memcpy(curr->data, data, size);

	/* increase the whole boot param size */
	root->size += curr->size;

	return curr;
}

const struct tz_boot_param *tz_boot_param_get(uint32_t id)
{
	if (!root)
		return NULL;

	uint32_t size = sizeof(struct tz_boot_param_header);

	while (size < root->size) {
		struct tz_boot_param *curr = (void *)root + size;
		if (curr->id == id)
			return curr;
		size += curr->size;
	}

	return NULL;
}
