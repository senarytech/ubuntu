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
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "ErrorCode.h"
#include "sm_agent.h"
#include "sm_common.h"

#define FTS_KEY_MACADDR  "macaddr"
#define MAC_STR_LEN      17
#define MAC_HEX_LEN      6

static bool is_hex(char hex) {
    if (hex >= '0' && hex <= '9') {
        return true;
    } else if (hex >= 'a' && hex <= 'f') {
        return true;
    } else if (hex >= 'A' && hex <= 'F') {
        return true;
    }

    return false;
}

static bool is_valid_macaddr(const char *ethaddr) {
    int index = 0;

    while (index < MAC_STR_LEN) {
        if (index % 3 == 2) {
            if (ethaddr[index] != ':') {
                return false;
            }
        } else if (!is_hex(ethaddr[index])) {
            return false;
        }
        index++;
    }

    return true;
}

static unsigned int str_to_hex(unsigned char ch) {
    if (ch >= '0' && ch <= '9')
        return (ch - '0');

    if (ch >= 'a' && ch <= 'f')
        return (ch - 'a' + 10);

    if (ch >= 'A' && ch <= 'F')
        return (ch - 'A' + 10);

    return 0;
}

static unsigned char hex_to_str(unsigned int h) {
    h &= 0xf;
    if (h <= 9)
        return (h + '0');

    if (h >= 0xa && h <= 0xf)
        return (h + 'a' - 10);

    return 0;
}


static void macaddr_str_to_val(const char *str, char *val) {
    int index = 0;

    while (index < MAC_HEX_LEN) {
        val[index] = (str_to_hex(str[index * 3]) << 4)
                        + str_to_hex(str[index * 3 + 1]);
        index ++;
    }
}

static void macaddr_val_to_str(const char *val, char *str) {
    int index = 0;

    while (index < MAC_STR_LEN) {
        if (index % 3 == 0) {
            str[index] = hex_to_str(val[index / 3] >> 4);
        } else if (index % 3 == 1) {
            str[index] = hex_to_str(val[index / 3] & 0xf);
        } else {
            str[index] = ':';
        }
        index++;
    }

    str[MAC_STR_LEN] = '\0';
}

static int get_macaddr_from_dev(char *ethaddr) {
    struct ifreq ifr;
    int skfd;

    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;

    skfd = socket(PF_INET, SOCK_STREAM, 0);
    if (skfd < 0) {
        printf("%s, socket error", __FUNCTION__);
        return -1;
    }

    if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
        printf("%s, ioctl error", __FUNCTION__);
        close(skfd);
        return -1;
    }

    memcpy(ethaddr, ifr.ifr_hwaddr.sa_data, MAC_HEX_LEN);
    close(skfd);

    return 0;
}

static int get_macaddr_from_file(const char *filename, char *macaddr) {
    if (filename == NULL)
        return -1;

    FILE *pfd = fopen(filename, "r");
    if(pfd == NULL) {
        return -1;
    }

    if (fread(macaddr, MAC_STR_LEN, 1, pfd) != 1){
        fclose(pfd);
        return -1;
    }
    fclose(pfd);

    if (!is_valid_macaddr(macaddr)) {
        return -1;
    }

    macaddr[MAC_STR_LEN] = '\0';

    return 0;
}

static int set_macaddr_to_wol(char *ethaddr) {
    MV_SM_Message msg;
    int ret;

    ret = MV_SM_Agent_Init();
    if (ret != 0) {
        printf("Failed to init sm agent.\n");
        return -1;
    }

    memset(&msg, 0, sizeof(MV_SM_Message));
    msg.m_iModuleID = MV_SM_ID_WOL;
    msg.m_iMsgLen = MAC_HEX_LEN;
    memcpy(msg.m_pucMsgBody, ethaddr, MAC_HEX_LEN);

    ret = MV_SM_Agent_Write_Msg(msg.m_iModuleID, msg.m_pucMsgBody, msg.m_iMsgLen);

    MV_SM_Agent_Close();
    return ret;
}

int sm_set_macaddr_to_wol(int argc, char *argv[])
{
    char macaddr[MAC_STR_LEN + 1];
    char ethaddr[MAC_HEX_LEN];

    // Get ethaddr from dev.
    if (get_macaddr_from_dev(ethaddr) == 0) {
        macaddr_val_to_str(ethaddr, macaddr);
        if (!is_valid_macaddr(macaddr)) {
            printf("%s, invalid mac addr format\n", __FUNCTION__);
            return -1;
        }
        printf("get ethaddr from device: %s\n", macaddr);
    }

    // Get mac addr from file.
    if (get_macaddr_from_file(argv[1], macaddr) == 0) {
        printf("get macaddr(%s) from file: %s\n", macaddr, argv[1]);
        macaddr_str_to_val(macaddr, ethaddr);
    }

    if (set_macaddr_to_wol(ethaddr) != 0)
        printf("%s, set mac address to wol failed", __FUNCTION__);

    return 0;
}

int sm_input_disable(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send;
    INT32 *p_msg = NULL;

    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_INPUT_DISABLE;

    sm_msg_send.m_iModuleID = MV_SM_ID_IR;
    sm_msg_send.m_iMsgLen = sizeof(INT32);

    printf("SM agent disable ir\n");

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                          sm_msg_send.m_pucMsgBody,
                          sm_msg_send.m_iMsgLen);
    MV_SM_Agent_Close();

    return 0;
}

int sm_input_enable(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send;
    INT32 *p_msg = NULL;

    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    sm_msg_send.m_iModuleID = MV_SM_ID_IR;
    sm_msg_send.m_iMsgLen = sizeof(INT32);
    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_INPUT_ENABLE;

    printf("SM agent enable ir\n");

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                          sm_msg_send.m_pucMsgBody,
                          sm_msg_send.m_iMsgLen);
    MV_SM_Agent_Close();

    return 0;
}

int sm_reset_soc(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send;
    INT32 *p_msg = NULL;
#define MV_SM_POWER_SYS_RESET 0xFF
    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    sm_msg_send.m_iModuleID = MV_SM_ID_POWER;
    sm_msg_send.m_iMsgLen = sizeof(INT32);
    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_POWER_SYS_RESET;

    printf("SM Reset SOC\n");

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                          sm_msg_send.m_pucMsgBody,
                          sm_msg_send.m_iMsgLen);
    MV_SM_Agent_Close();

    return 0;
}

#ifndef MV_SM_GPIO_SET
#define MV_SM_GPIO_SET      0x6
#endif

int sm_gpio(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send;
    INT32 *p_msg = NULL;

    if (argc < 3)
        return -1;

    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_GPIO_SET;
    p_msg[1] = strtoul(argv[1], NULL, 10);
    p_msg[2] = strtoul(argv[2], NULL, 10);

    sm_msg_send.m_iModuleID = MV_SM_ID_SYS;
    sm_msg_send.m_iMsgLen = sizeof(INT32)*3;

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                          sm_msg_send.m_pucMsgBody,
                          sm_msg_send.m_iMsgLen);

    MV_SM_Agent_Close();

    return 0;
}

int sm_input_sendkey(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send;
    INT32 *p_msg = NULL;
    int i;
    char *p;

    if (argc < 2)
        return 0;

    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    sm_msg_send.m_iModuleID = MV_SM_ID_IR;
    sm_msg_send.m_iMsgLen = sizeof(INT32)*2;

    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_INPUT_SEND_KEY;

    for (i = 1; i < argc; i++)
    {
        p_msg[1] = strtoul(argv[i], &p, 10);
        if (p && *p != 0)
            p_msg[1] = strtoul(argv[i], NULL, 16);

        MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                sm_msg_send.m_pucMsgBody,
                sm_msg_send.m_iMsgLen);
    }

    MV_SM_Agent_Close();

    return 0;
}

int sm_temp_get(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send, sm_msg_recv;
    INT32 *p_msg = NULL, readlen = 0;
    int i;
    char *p;

    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    sm_msg_send.m_iModuleID = MV_SM_ID_TEMP;
    sm_msg_send.m_iMsgLen = sizeof(INT32);

    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_TEMP_SAMPLE;

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                sm_msg_send.m_pucMsgBody,
                sm_msg_send.m_iMsgLen);

    memset(&sm_msg_recv, 0, sizeof(MV_SM_Message));
    sm_msg_recv.m_iModuleID = MV_SM_ID_TEMP;

    if (MV_SM_Agent_Read_Msg_Ext(sm_msg_recv.m_iModuleID,
                         &sm_msg_recv, &readlen, 1) == S_OK)
    {
        p_msg = (INT32 *)(sm_msg_recv.m_pucMsgBody);
        printf("Chip Temperture: %d degC\n", p_msg[2]);
    }
    else
        printf("Read message from SM failed!\n");

    MV_SM_Agent_Close();

    return 0;
}

static void  print_wakeupsource(INT32 wakeup_source)
{
    switch (wakeup_source)
    {
    case MV_SM_WAKEUP_SOURCE_INVALID:
            printf("%s: INVALID\n", __func__);
            break;
    case MV_SM_WAKEUP_SOURCE_IR:
            printf("%s: IR\n", __func__);
            break;
    case MV_SM_WAKEUP_SOURCE_WIFI_BT:
            printf("%s: WIFI/BT\n", __func__);
            break;
    case MV_SM_WAKEUP_SOURCE_WOL:
            printf("%s: WOL\n", __func__);
            break;
    case MV_SM_WAKEUP_SOURCE_VGA:
            printf("%s: VGA\n", __func__);
            break;
    case MV_SM_WAKEUP_SOURCE_CEC:
            printf("%s: CEC\n", __func__);
            break;
    case MV_SM_WAKEUP_SOURCE_TIMER:
            printf("%s: TIMER\n", __func__);
            break;
    default:
            printf("Unknow wake up source (%d)!\n", wakeup_source);
            break;
    }
}

int sm_get_wakeup_reason(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send, sm_msg_recv;
    INT32 *p_msg = NULL, readlen = 0;
    int i;
    char *p;

    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));

    sm_msg_send.m_iModuleID = MV_SM_ID_POWER;
    sm_msg_send.m_iMsgLen = sizeof(INT32);

    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = MV_SM_POWER_WAKEUP_SOURCE_REQUEST;

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
                sm_msg_send.m_pucMsgBody,
                sm_msg_send.m_iMsgLen);

    memset(&sm_msg_recv, 0, sizeof(MV_SM_Message));
    sm_msg_recv.m_iModuleID = MV_SM_ID_POWER;

    if (MV_SM_Agent_Read_Msg_Ext(sm_msg_recv.m_iModuleID,
                         &sm_msg_recv, &readlen, 1) == S_OK)
    {
        INT32* ws = (INT32*)sm_msg_recv.m_pucMsgBody;
        print_wakeupsource(*ws);
    }
    else
        printf("Read message from SM failed!\n");

    MV_SM_Agent_Close();

    return 0;
}

void sm_set_wakeup_timer(int argc, char *argv[])
{
    MV_SM_Message sm_msg_send;
    INT32 type, time;
    INT32 *p_msg = NULL;
    char *p;

    if (argc == 1) {
        printf("please add timeout parameter in seconds");
    } else {
        time = strtoul(argv[1], &p, 10);
        if (p && *p != 0)
            time = strtoul(argv[1], NULL, 16);
    }
    type = MV_SM_POWER_WARMUP_TIME;
    MV_SM_Agent_Init();

    memset(&sm_msg_send, 0, sizeof(MV_SM_Message));
    sm_msg_send.m_iModuleID = MV_SM_ID_POWER;
    sm_msg_send.m_iMsgLen = 2*sizeof(INT32);

    p_msg = (INT32 *)(sm_msg_send.m_pucMsgBody);
    p_msg[0] = type;
    p_msg[1] = time;

    printf("set wakeup in %d seconds ", time);

    MV_SM_Agent_Write_Msg(sm_msg_send.m_iModuleID,
        sm_msg_send.m_pucMsgBody,
        sm_msg_send.m_iMsgLen);

    MV_SM_Agent_Close();
}


typedef struct command_s {
    const char *name;
    const char *alias;
    const char *help;
    int (*cmd)(int, char **);
} command_t;

static command_t commands[] = {
    { "enable", "-e", "-e|enable", sm_input_enable },
    { "disable", "-d", "-d|disable", sm_input_disable },
    { "gpio", "-g", "-g|gpio num val", sm_gpio},
    { "sendkey", "-s", "-s|sendkey key", sm_input_sendkey},
    { "gettemp", "-t", "-t|gettemp", sm_temp_get},
    { "setwaketimer", "-w", "-w|set num seconds", sm_set_wakeup_timer},
    { "getwakereason", "-r", "-r|get wakeup reason", sm_get_wakeup_reason},
    { "setmac2wol", "-m", "-m|set macaddr to wol", sm_set_macaddr_to_wol},
    { "resetsoc", "-p", "-p|reset soc", sm_reset_soc},
    { 0, 0, 0, 0 },
};

static const char *prog_name;

void usage(int error)
{
    int i;
    fprintf(stdout, "Usage:\n");
    for (i = 0; commands[i].name != NULL; i++) {
        if (commands[i].help != NULL)
            fprintf(stdout, "  %s %s\n", prog_name, commands[i].help);
    }
    exit(error);
}

int do_cmd(const char *cmd, int argc, char *argv[])
{
    int i;
    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(cmd, commands[i].name) == 0 ||
            ((commands[i].alias != NULL) &&
             (strcmp(cmd, commands[i].alias) == 0)))
            return commands[i].cmd(argc, argv);
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int ret;

    prog_name = strrchr(argv[0], '/');
    if (prog_name) prog_name++;
    else prog_name = argv[0];

    if (argc > 1) {
        if (strncmp(argv[1], "help", 4) == 0)
            usage(0);
        if (do_cmd(argv[1], argc-1, argv+1))
            usage(1);
    }

    return 0;
}
