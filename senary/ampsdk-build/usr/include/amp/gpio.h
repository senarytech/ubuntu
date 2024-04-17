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


#ifndef _GPIO_H_
#define _GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_DDR_IN     0
#define PORT_DDR_OUT    1

#define GPIO_MODE_DATA_IN           1
#define GPIO_MODE_DATA_OUT          2
#define GPIO_MODE_IRQ_LOWLEVEL      3
#define GPIO_MODE_IRQ_HIGHLEVEL     4
#define GPIO_MODE_IRQ_RISEEDGE      5
#define GPIO_MODE_IRQ_FALLEDGE      6
#define GPIO_MODE_IRQ_BOTH          7
#define GPIO_MODE_DATA_OUT_HIGH     8

int GPIO_PortWrite_IOCTL(int port, int value);
int GPIO_PortRead_IOCTL(int port, int *value);
int GPIO_PortSetInOut_IOCTL(int port, int in);
int GPIO_PortGetInOut_IOCTL(int port, int* inout);
int GPIO_PortGetData_IOCTL(int port, int *data);
int GPIO_PortEnableIRQ_IOCTL(int port, int edge);
int GPIO_PortDisableIRQ_IOCTL(int port);
int GPIO_PortHasInterrupt_IOCTL(int port);
int GPIO_PortClearInterrupt_IOCTL(int port);


int SM_GPIO_PortWrite_IOCTL(int port, int value);
int SM_GPIO_PortRead_IOCTL(int port, int *value);
int SM_GPIO_PortSetInOut_IOCTL(int port, int in);
int SM_GPIO_PortGetInOut_IOCTL(int port, int* inout);
int SM_GPIO_PortGetData_IOCTL(int port, int *data);
int SM_GPIO_PortEnableIRQ_IOCTL(int port, int edge);
int SM_GPIO_PortDisableIRQ_IOCTL(int port);
int SM_GPIO_PortHasInterrupt_IOCTL(int port);
int SM_GPIO_PortClearInterrupt_IOCTL(int port);
#if((BERLIN_CHIP_VERSION == BERLIN_BG4_CDP) || \
    (BERLIN_CHIP_VERSION == BERLIN_BG4_CT) || \
    (BERLIN_CHIP_VERSION == BERLIN_BG4_CT_A0) || \
    (BERLIN_CHIP_VERSION == BERLIN_BG5_CT) || \
    (BERLIN_CHIP_VERSION == BERLIN_BG5_CT_A0) || \
    (BERLIN_CHIP_VERSION == BERLIN_BG6_CT))
int AVIO_GPIO_PortWrite_IOCTL(int port, int value);
int AVIO_GPIO_PortRead_IOCTL(int port, int *value);
int AVIO_PortSetInOut_IOCTL(int port, int in);
#endif

//For backword compatility
#define GPIO_DOMAIN_SM 0x00010000
#define GPIO_PortWriteAll(port,value) (port&GPIO_DOMAIN_SM?\
                    SM_GPIO_PortWrite_IOCTL(port&0xffff,value):\
                    GPIO_PortWrite_IOCTL(port&0xffff,value))

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif

