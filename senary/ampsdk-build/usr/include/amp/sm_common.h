/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#ifndef __SM_COMMON_H__
#define __SM_COMMON_H__

#define SM_MSG_SIZE		32
#define SM_MSG_BODY_SIZE	(SM_MSG_SIZE - sizeof(short)*2)
#define SM_MSGQ_TOTAL_SIZE	512
#define SM_MSGQ_HEADER_SIZE	SM_MSG_SIZE
#define SM_MSGQ_SIZE		(SM_MSGQ_TOTAL_SIZE - SM_MSGQ_HEADER_SIZE)
#define SM_MSGQ_MSG_COUNT	(SM_MSGQ_SIZE/SM_MSG_SIZE)

typedef struct {
    short m_iModuleID;
    short m_iMsgLen;
    char m_pucMsgBody[SM_MSG_BODY_SIZE];
} MV_SM_Message;

typedef struct {
    int m_iWrite;
    int m_iRead;
    int m_iWriteTotal;
    int m_iReadTotal;
    char m_Padding[SM_MSGQ_HEADER_SIZE - sizeof(int)*4];
    char m_Queue[SM_MSGQ_SIZE];
} MV_SM_MsgQ;

/*
 * BOX_PRODUCT_RELEASE related
 * Decide whether we should enter standby or not after bootup.
 */
#define MV_SM_RSTFLAG_COLD		0xFFFF

/* power module message */
#define MV_SoC_STATE_SYSPOWERON	0x0
#define MV_SoC_STATE_COLDBOOT		0x21
#define MV_SM_POWER_FASTBOOT_ENABLE	0x0F
#define MV_SM_POWER_FASTBOOT_DISABLE	0x0E
#define MV_SM_POWER_INSPECT_REQUEST	0xED

/*msg from linux side*/
#define MV_SM_POWER_WAKEUP_SOURCE_REQUEST 3
#define MV_SM_POWER_WARMDOWN_TIME       11
#define MV_SM_POWER_WARMUP_TIME         12
#define MV_SM_POWER_WARMUP_TIME_REQUEST 13

/* sys module message */
#define MV_SM_GPIO_SET			0x6
#define MV_SM_GPIO_GET			0x7

/* ir module message */
#define MV_SM_IR_Linuxready		30

/* INPUT message, include IR and Key module */
#define MV_SM_INPUT_DISABLE		0x1
#define MV_SM_INPUT_ENABLE		0x2
#define MV_SM_INPUT_SEND_KEY		0x4
#define MV_SM_INPUT_POWERKEY_ONLY	0x5

/* watchdog module message */
#define MV_SM_WD_APP_START		0x12
#define MV_SM_WD_APP_EXIT		0x36
#define MV_SM_WD_APP_CONTINUE		0x63
#define MV_SM_WD_Kickoff		0x24

/* temperature module message */
#define MV_SM_TEMP_SAMPLE		0xF0
#define MV_SM_TEMP_CPU_TYPE		0xCC
#define MV_SM_TEMP_CPU_TYPE_B0		0xB0

/* cec module message */
#define MV_SM_CEC_APPREADY		0x1
#define MV_SM_CEC_INFO			0x2
#define MV_SM_CEC_OSD_INFO      0x3
#define MV_SM_CEC_HDMICTRL_ONOFF	0x4
#define MV_SM_CEC_APP_REQUESTED		0x5
#define MV_SM_CEC_NO_APP_REQUESTED	0x6
#define MV_SM_CEC_ACTIVE_SOURCE_EVENT	0x7
#define MV_SM_CEC_STREAM_PATH_EVENT	0x8
#define MV_SM_CEC_UC_PRESSED_EVENT	0x9
#define MV_SM_CEC_WAKEUP_MSG		0xA
#define MV_SM_CEC_COLD_BOOT		0xB

typedef enum
{
    MV_SM_ID_SYS = 1,
    MV_SM_ID_COMM,
    MV_SM_ID_IR,
    MV_SM_ID_KEY,
    MV_SM_ID_POWER,
    MV_SM_ID_WDT,
    MV_SM_ID_TEMP,
    MV_SM_ID_VFD,
    MV_SM_ID_SPI,
    MV_SM_ID_I2C,
    MV_SM_ID_UART,
    MV_SM_ID_CEC,
    MV_SM_ID_WOL,
    MV_SM_ID_LED,
    MV_SM_ID_ETH,
    MV_SM_ID_DDR,
    MV_SM_ID_WIFIBT,
    /* add new module id here */
    MV_SM_ID_DEBUG,
    MV_SM_ID_CONSOLE,
    MV_SM_ID_PMIC,
    MV_SM_ID_AUDIO,
    MV_SM_ID_MAX,
} MV_SM_MODULE_ID;

#define MAX_MSG_TYPE (MV_SM_ID_MAX - 1)

/* SM driver ioctl cmd */
#define SM_READ			0x1
#define SM_WRITE		0x2
#define SM_RDWR			0x3
#define SM_Enable_WaitQueue	0x1234
#define SM_Disable_WaitQueue	0x3456

typedef enum _MV_SM_WAKEUP_SOURCE_TYPE
{
    MV_SM_WAKEUP_SOURCE_INVALID = -1,
    MV_SM_WAKEUP_SOURCE_IR = 0,
    MV_SM_WAKEUP_SOURCE_WIFI_BT,
    MV_SM_WAKEUP_SOURCE_WOL,
    MV_SM_WAKEUP_SOURCE_VGA,
    MV_SM_WAKEUP_SOURCE_CEC,
    MV_SM_WAKEUP_SOURCE_TIMER,
    MV_SM_WAKEUP_SOURCE_NUM,
} MV_SM_WAKEUP_SOURCE_TYPE;

#endif /* __SM_COMMON_H__ */
