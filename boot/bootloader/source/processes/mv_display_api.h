/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2013-2018 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
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
#ifndef _MV_DISP_INTERFACE_H_
#define _MV_DISP_INTERFACE_H_

#ifdef CONFIG_PANEL_FASTLOGO
#include "pwm.h"
#include "io.h"
#endif

#define MV_DISP_SUCESS         (0)
#define MV_DISP_E_RES_CFG     (1)
#define MV_DISP_E_CREATE      (2)
#define MV_DISP_E_CFG         (3)
#define MV_DISP_E_RST         (4)
#define MV_DISP_E_WIN         (5)
#define MV_DISP_E_DISP        (6)
#define MV_DISP_E_NOMEM       (7)
#define MV_DISP_E_FAIL        (8)
#define MV_DISP_E_TX_CTRL     (9)
#define MV_DISP_E_RES         (10)
#define MV_DISP_E_OPEN        (11)

/* VPP_DISP_PARAM_PRIMARY_DISPID / VPP_DISP_PARAM_SECONDARY_DISPID
 *                              - Selects the primary/secondary Display output resolution
 *                              - Decided based on the Configuration selected in defconfig.
 *                              - DSI/HDMI o/p's is also selected in configuration
 *
 * VPP_PRIMARY_DISPLAYINFO_RESID - Selects the Resolution info params based on Display
 *                               - Mipi always Loads info of ResID configured from profile
 *                                 for BL in DSI_CUSTOM resid.
 *                               - HDMI Handles based on ResID configured from profile.
 *
 * VPP_PRIMARY_DISP_MODE         - Select the o/p display mode (Primary/Secondary/Dual)
 */
#ifdef CONFIG_PANEL_FASTLOGO
#define MV_DISP_isValidRes(ResId) ((ResId >= RES_720P30) && \
		(ResId <= RES_MAX_DSI))

#if defined (BL_DISPLAY_DUAL_PIP)
#define VPP_DISP_PARAM_PRIMARY_DISPID       VOUT_HDMI
#define VPP_PRIMARY_DISP_MODE               VOUT_DISP_DUAL_MODE_PIP
#define VPP_PRIMARY_DISPLAYINFO_RESID       default_primary_system
#define VPP_SECONDARY_DISP_MODE             VOUT_DISP_DUAL_MODE_PIP
#define PANEL_RESID                         SECONDARY_DISPLAY_RES
#else /* BL_DISPLAY_DUAL_PIP */
#define VPP_DISP_PARAM_PRIMARY_DISPID       VOUT_DSI
#define VPP_PRIMARY_DISP_MODE               VOUT_DISP_SINGLE_MODE_SEC
#define VPP_PRIMARY_DISPLAYINFO_RESID       RES_DSI_CUSTOM
#define PANEL_RESID                         PRIMARY_DISPLAY_RES
#endif /* BL_DISPLAY_DUAL_PIP */

#define VPP_DISP_PARAM_SECONDARY_DISPID     VOUT_DSI
#else //CONFIG_PANEL_FASTLOGO
#define MV_DISP_isValidRes(ResId) ((ResId >= RES_720P30) && \
		(ResId <= RES_MIN_DSI))

#define VPP_PRIMARY_DISP_MODE               VOUT_DISP_SINGLE_MODE_PRI
#define VPP_DISP_PARAM_PRIMARY_DISPID       VOUT_HDMI
#define VPP_PRIMARY_DISPLAYINFO_RESID       default_primary_system
#endif //CONFIG_PANEL_FASTLOGO

typedef struct DISP_LOGO_CONFIG_T
{
    unsigned int    width;
    unsigned int    height;
    unsigned int    stride;
    unsigned char*  buffer;
    unsigned int    start;
    unsigned int    size;
    unsigned int    valid;
}DISP_LOGO_CONFIG;

/********************************************************************************
 * FUNCTION : Get width, height and display resolution of the system
 * OUTPUT   : resid - resolution ID, refer vpp_api.h
 *            width - width correponding to resolution
 *            height - height correponding to resolution
 *
 * RETURN   : None
 ********************************************************************************/
void MV_DISP_GetDisplayInfo(int *resid, int *width, int *height);

/********************************************************************************
 * FUNCTION : Config the system  with fixed resolution
 * INPUT    : resid - resolution ID, refer vpp_api.h
 *
 *
 * RETURN: MV_DISP_E_SUC - SUCCEED
 *         MV_DISP_E_RES_CFG - Failed to configure resolution
 *         MV_DISP_E_CREATE - Failed to instantiate VPP
 *         MV_DISP_E_RST - Failed to reset VPP
 *         MV_DISP_E_CFG - Failed to configure VPP
 *         MV_DISP_E_WIN - Failed to configure VPP Window
 *         MV_DISP_E_DISP - Failed to configure VPP display
 ********************************************************************************/

int MV_DISP_ConfigDisplay(int res_width, int res_height, int gfb_width, int gfb_height);
//int MV_DISP_ConfigDisplay(int resid);
/********************************************************************************
 * FUNCTION : Get width, height and display resolution of the system
 * INPUT    : buf - pointer to frame buffer
 *            width - width of the frame
 *            height - height of the frame
 *
 * RETURN  : None
 ********************************************************************************/

void MV_DISP_Display_Frame(void *buf,int width,int height);
/********************************************************************************
 * FUNCTION : Mute the graphic display
 * INPUT    : mute - 1:mute 0:unmute
 *
 * RETURN   : None
 ********************************************************************************/

void MV_DISP_MuteDisplay(int mute);
/********************************************************************************
 * FUNCTION  : Stop the graphic display
 * INPUT     : None
 *
 * RETURN   : None
 ********************************************************************************/

void MV_DISP_StopDisplay(void);
/********************************************************************************
 * FUNCTION : Destroy the graphic display
 * INPUT    : None
 *
 *
 * RETURN   : None
 ********************************************************************************/
void MV_DISP_DestroyDisplay(void);
int MV_DISP_InitDisplay(int resid, int hdmi_bitdepth);
int MV_DISP_LogoStart(int planeID, DISP_LOGO_CONFIG* logocfg, int win_width, int win_height);
void MV_DISP_EnableIRQ(void);
#ifdef CONFIG_PANEL_FASTLOGO
int i2c_master_init(int id, int speed, int b_10bit_addr);
int i2c_master_write_and_read(int id, int target_addr, unsigned char* send_buff, int send_len, unsigned  char* recv_buff, int recv_len);
void MV_DISP_Backlight_Enable(int resid);
int Mipi_LoadInfoTable(void);
#endif
#endif
