/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _RPMB_IOCTL_H
#define _RPMB_IOCTL_H

/*
 * ioctl() interface
 * Comes from: uapi/linux/major.h, linux/mmc/core.h
 */

#define MMC_BLOCK_MAJOR	179

/* mmc_ioc_cmd.opcode */
#define MMC_SEND_EXT_CSD		 8
#define MMC_READ_MULTIPLE_BLOCK		18
#define MMC_WRITE_MULTIPLE_BLOCK	25

/* mmc_ioc_cmd.flags */
#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136     (1 << 1)	/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)	/* Expect valid CRC */
#define MMC_RSP_OPCODE	(1 << 4)	/* Response contains opcode */
#define MMC_RSP_SPI_S1	(1 << 7)		/* one status byte */

#define MMC_RSP_R1      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_SPI_R1	(MMC_RSP_SPI_S1)

#define MMC_CMD_ADTC	(1 << 5)	/* Addressed data transfer command */

/* mmc_ioc_cmd.write_flag */
#define MMC_CMD23_ARG_REL_WR	(1 << 31) /* CMD23 reliable write */

/*
 * MMC_IOC_MULTI_CMD
 * Comes from: linux/mmc/ioctl.h
 */

#ifndef  MMC_IOC_MULTI_CMD
/**
 * struct mmc_ioc_multi_cmd - multi command information
 * @num_of_cmds: Number of commands to send. Must be equal to or less than
 *      MMC_IOC_MAX_CMDS.
 * @cmds: Array of commands with length equal to 'num_of_cmds'
 */
struct mmc_ioc_multi_cmd {
	__u64 num_of_cmds;
	struct mmc_ioc_cmd cmds[0];
};

/*
 * MMC_IOC_MULTI_CMD: Used to send an array of MMC commands described by
 *      the structure mmc_ioc_multi_cmd. The MMC driver will issue all
 *      commands in array in sequence to card.
 */
#define MMC_IOC_MULTI_CMD _IOWR(MMC_BLOCK_MAJOR, 1, struct mmc_ioc_multi_cmd)
#endif

#endif
