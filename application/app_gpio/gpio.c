/*******************************************************************************
 * Copyright (C) 2016, Marvell International Ltd. and its affiliates
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * alternative licensing terms.
 *
 *******************************************************************************
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell and you have entered into a commercial
 * license agreement (a "Commercial License") with Marvell, the File is licensed
 * to you under the terms of the applicable Commercial License.
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int GPIO_PortRead_IOCTL(int port, int *value);
extern int SM_GPIO_PortRead_IOCTL(int port, int *value);
extern int GPIO_PortWrite_IOCTL(int port, int value);
extern int SM_GPIO_PortWrite_IOCTL(int port, int value);

static const char *prog_name;

static void usage(int status)
{
    fprintf(status ? stdout: stderr, "Usage: %s sm/soc read/write port value\n", prog_name);
    exit(status);
}

int main(int argc, char *argv[])
{
    int port, value;

    prog_name = strrchr(argv[0], '/');
    if (!prog_name)
        prog_name = argv[0];
    else prog_name++;

    if (argc < 4)
        usage(1);

    port  = strtoul(argv[3], NULL, 10);

    if (strcmp("soc", argv[1]) == 0){
        if (strcmp("read", argv[2]) == 0){
            GPIO_PortRead_IOCTL(port, &value);
            printf("%s gpio port %d value is %d\n", argv[1], port, value);
        } else if(strcmp("write", argv[2]) == 0){
            if (argc < 5)
                usage(1);
            value  = strtoul(argv[4], NULL, 10);
            GPIO_PortWrite_IOCTL(port, value);
            printf("%s gpio port %d value is %d\n", argv[1], port, value);
        } else
            usage(1);
    } else if (strcmp("sm", argv[1]) == 0){
        if (strcmp("read", argv[2]) == 0){
            SM_GPIO_PortRead_IOCTL(port, &value);
            printf("%s gpio port %d value is %d\n", argv[1], port, value);
        } else if(strcmp("write", argv[2]) == 0){
            if (argc < 5)
                usage(1);
            value  = strtoul(argv[4], NULL, 10);
            SM_GPIO_PortWrite_IOCTL(port, value);
            printf("%s gpio port %d value is %d\n", argv[1], port, value);
        } else
            usage(1);
    }
    else {
        usage(1);
    }

    return 0;
}
