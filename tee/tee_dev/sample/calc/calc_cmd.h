#ifndef _CALC_CMD_H_
#define _CALC_CMD_H_

#ifndef CONFIG_SO
#define TACALC_UUID	{ 0xdbf3091a, 0x2fc8, 0x4dc5, { 0xb2, 0x9, 0x40, 0x5, 0xca, 0xa4, 0xfd, 0x1a } }
#else
#define TACALC_UUID	{ 0xa2619c16, 0x26b3, 0x4431, { 0x97, 0x1, 0xc1, 0x37, 0xd, 0xee, 0xc1, 0x1f } }
#endif

enum {
	CALC_CMD_INC,
	CALC_CMD_DEC,
	CALC_CMD_ADD,
	CALC_CMD_SUB,
	CALC_CMD_MUL,
	CALC_CMD_DIV,
	CALC_CMD_ECHO,
	CALC_CMD_CONVERT,
	CALC_CMD_REVERSE,
	CALC_CMD_MAX
};

#endif /* _CALC_CMD_H_ */
