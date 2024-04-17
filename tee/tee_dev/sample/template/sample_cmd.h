#ifndef _SAMPLE_COMMON_H_
#define _SAMPLE_COMMON_H_

#define TA_UUID	{ 0x88da5306, 0x8461, 0x11e3, \
	{0xaf, 0xd9, 0x78, 0x2b, 0xcb, 0x5c, 0xf3, 0xe3} }

enum {
	CMD_VAL_ADD,
	CMD_VAL_SUB,
	CMD_MEM_REVERSE,
	CMD_AES_ENCRYPT,
	CMD_AES_DECRYPT,
	CMD_MAX
};

#endif /* _SAMPLE_COMMON_H_ */
