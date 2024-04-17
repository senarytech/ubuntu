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

#ifndef _DISP_TEST_PRV_H_
#define _DISP_TEST_PRV_H_
#include "OSAL_api.h"
#include "amp_client_support.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"
#include "isl/amp_event_queue.h"
#include "vpp_api.h"
#include "disp_test_framebuffer.h"

#ifdef DEBUG
#define CMDLINE_PRINT(fmt, ...)    \
    printf("[%s:%s():%d]\t" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define CMDLINE_PRINT               printf
#endif
#define MAX_MSG_SIZE 128
#define MMU_ENABLED                   (0x02)
// Helper Macros for MTR and MMU feature.
#define VPP_BITMASK_MTR  0x01
#define VPP_BITMASK_MMU  0x02
//This macro check if MTR bit of x is set. x can be'pinfo->m_is_compressed' or'plane->m_mtrR__en'.
#define IS_MTR_ENABLED(x) ((x) & VPP_BITMASK_MTR)
//This macro check if MMU bit of x is set. x can be'pinfo->m_is_compressed'or'plane->m_mtrR__en'.
#define IS_MMU_ENABLED(x) ((x) & VPP_BITMASK_MMU)
//This macro check if MMU bit of x is only set.x can be'pinfo->m_is_compressed'or'plane->m_mtrR__en'
#define IS_MMU_ONLY_ENABLED(x) (!IS_MTR_ENABLED(x) &&  IS_MMU_ENABLED(x))

#define MV_UNUSED(x) ((void)(x))

extern char m_hdmitx_colorfmt_table[MAX_NUM_OUTPUT_COLOR_FMTS][MAX_MSG_SIZE];
extern char m_hdmitx_bitdepth_table[MAX_NUM_OUTPUT_BIT_DEPTHS][MAX_MSG_SIZE];
extern char m_hdmitx_resolution_table[MAX_NUM_RESS + 1][MAX_MSG_SIZE];
extern char m_display_tgid_table[2][MAX_MSG_SIZE];
extern char m_display_outmode_table[AMP_DISP_OUT_TYPE_MAX][MAX_MSG_SIZE];
extern char m_disp_id[AMP_DISP_OUT_MAX][MAX_MSG_SIZE];
extern AMP_DISP_WIN m_PerFrameSrcWin[AMP_DISP_PLANE_MAX], m_PerFrameDstWin[AMP_DISP_PLANE_MAX];

typedef int (*CMDFUNC) (int argc, char *argv[]);

typedef struct tagCmdHandler {
    char *pCmd;
    CMDFUNC Handler;
    char *pHelp;
} DISP_CMD_HANDLER;

typedef struct tagCmdAtuoHandler {
    int caseID;
    CMDFUNC Handler;
    char *pHelp;
} DISP_AUTO_HANDLER;

/*bd Action*/
HRESULT create_bd_queue(VOID);
HRESULT destroy_bd_queue(AMP_DISP objDS);
AMP_BD_HANDLE request_free_bd(AMP_DISP objDS, UINT32 uiPlaneId);

/*build in frame*/
HRESULT disptest_push_buildinframe(AMP_DISP objDS,
    int PlaneID, DISP_TEST_FRAME frame_info);

/*FUNC disp_build_frame -  build a frame depend on input information
 *iVideo - video or not
 *pFrameInfo 	- the Frame info that needed
 *iSrcFmt		- the input Frame fmt
 *iOrder		- the input Frame pixel order
 *iX			- the X start position
 *iY			- the Y start position
 *uiWidth		- the width of the Frame
 *uiHeight 		- the Height of the Frame
 *iProgressive	- the Frame is progressive or interlaced frame
*/
HRESULT disp_build_frame(INT32 iVideo, AMP_BGTAG_FRAME_INFO *pFrameInfo,
                           INT32 iSrcFmt, INT32 iOrder, INT32 iX, INT32 iY,
                           UINT32 uiWidth,  UINT32 uiHeight, INT32 iProgressive,
                           UINT32 uiPixel1, UINT32 uiPixel2, UINT32 uiPar);

/*Frame capture test*/
HRESULT capture_event_handler(HANDLE hListener, AMP_EVENT * pEvent,
                                  VOID * pUserData);

HRESULT disp_make_frame_data(INT32 iSrcFmt,   volatile UINT32 *pStartAddr,
                               UINT32 uiPicH,   UINT32 uiLineV, UINT32 uiWidth,
                               UINT32 uiHeight, UINT32 uiPicA,  UINT32 uiPicB);

HRESULT disp_set_hdmi(unsigned int resID, unsigned int colorfmt,
                      unsigned int bitdepth, unsigned int aspect, unsigned int tgid);
HRESULT disp_setplane_mute(unsigned int uiPlaneID, unsigned int uiMute);

HRESULT disptest_push_frame_from_localfile(AMP_DISP objDS, int PlaneID,
                                                DISP_TEST_FRAME frame_info, const char *file_path);

HRESULT disptest_push_frame_previous(AMP_DISP objDS, int PlaneID);
HRESULT disptest_push_frame_previous1(AMP_DISP objDS, int PlaneID, int iter, int ms_delay1, int ms_delay2);
HRESULT disp_set_format(AMP_DISP_OUT_PARAMS *pdisp_params, unsigned int aspect, unsigned int tgid);
HRESULT disptest_ngwm_configure(AMP_DISP gDisp, UINT32 pipeSourceId, char *testVectorFilename);
#endif
