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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"
#include "isl/amp_event_queue.h"
#include "disp_test_prv.h"
#include "disp_test_framebuffer.h"
#include <sys/time.h>
#include "ndi_buf.h"



//vdec_com.h
#define VINFO_FLAG_MASK_SCALING_FRAME   0x200
//vdec/.../internal_api_priv.h
#define FLAG_MASK_SCALING_FRAME         0x200


#define APP_QUEUE_SIZE    (8)

#define AMP_FRAME_TYPE_LEGACY         0
#define AMP_FRAME_TYPE_DOLBY_BL_MD    5
#define AMP_FRAME_TYPE_DOLBY_BL_EL_MD 7
static AMP_BDCHAIN *pAppQue;

extern AMP_SHM_HANDLE hCapBuf;
extern AMP_DISP gDisp;

extern AMP_BD_HANDLE prev_hBD;

/*BD action*/
HRESULT create_bd_queue(VOID)
{
    UINT32 i;
    HRESULT Ret = SUCCESS;
    AMP_BD_HANDLE hBD;

    Ret = AMP_BDCHAIN_Create(0, &pAppQue);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("%s:%d, AMP_BDCHAIN_Create return 0x%x!\n",\
            __FUNCTION__, __LINE__, Ret);
        goto create_bd_queue_exit;
    }

    for (i = 0; i < APP_QUEUE_SIZE; i++) {

        Ret = AMP_BD_Allocate(&hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, AMP_BDCHAIN_Create return 0x%x!\n",\
                __FUNCTION__, __LINE__, Ret);
            goto create_bd_queue_exit;
        }

        Ret = AMP_BDCHAIN_PushItem(pAppQue, hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, AMP_BDCHAIN_PushItem return 0x%x!\n",\
                __FUNCTION__, __LINE__, Ret);
            goto create_bd_queue_exit;
        }
    }
    return SUCCESS;

create_bd_queue_exit:
    while (1) {
        Ret = AMP_BDCHAIN_PopItem(pAppQue, &hBD);
        if (Ret != SUCCESS) {
            break;
        }
        Ret = AMP_BD_Free(hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, AMP_BDCHAIN_Create return 0x%x!\n",\
                __FUNCTION__, __LINE__, Ret);
        }
    }

    if (pAppQue) {
        Ret = AMP_BDCHAIN_Destroy(pAppQue);
    }
    return Ret;
}

HRESULT destroy_bd_queue(AMP_DISP objDS)
{
    UINT32 i;
    HRESULT Ret = SUCCESS;
    AMP_BD_ST tempBD;
    AMP_BD_HANDLE hBD = NULL;
    UINT32 ItemNum = 0;
    AMP_BDCHAIN_GetItemNum(pAppQue, &ItemNum);

    while(ItemNum != APP_QUEUE_SIZE) {

        CORBA_Environment _ev;
        Ret = AMP_DISP_VID_PopFrame(objDS, AMP_DISP_PLANE_MAIN, &tempBD, &_ev);
        if (Ret == SUCCESS) {
            hBD = (AMP_BD_HANDLE)(tempBD.uiAllocVA);
            Ret = AMP_BDCHAIN_PushItem(pAppQue, hBD);
            if (Ret != SUCCESS) {
                CMDLINE_PRINT("%s:%d, can not push bd!", __FUNCTION__, __LINE__);
            }
        }
        else {
            MV_OSAL_Task_Sleep(100);
        }
        AMP_BDCHAIN_GetItemNum(pAppQue, &ItemNum);
    }

    for (i = 0; i < APP_QUEUE_SIZE; i++) {

        Ret = AMP_BDCHAIN_PopItem(pAppQue, &hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, AMP_BDCHAIN_PushItem return 0x%x!\n",\
                __FUNCTION__, __LINE__, Ret);
        }
        Ret = AMP_BD_Free(hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, AMP_BDCHAIN_Create return 0x%x!\n",\
                __FUNCTION__, __LINE__, Ret);
        }
    }
    if (pAppQue) {
        Ret = AMP_BDCHAIN_Destroy(pAppQue);
    }

    CMDLINE_PRINT("App BD Chain Destroy Done\n");
    return Ret;
}

AMP_BD_HANDLE request_free_bd(AMP_DISP objDS, UINT32 uiPlaneId)
{
    MV_UNUSED(uiPlaneId);
    HRESULT Ret = SUCCESS;
    AMP_BD_HANDLE hBD = NULL;
    AMP_BD_ST *tempBD;

    tempBD = malloc(sizeof(AMP_BD_ST));

    while(1) {
        CORBA_Environment _ev;
        Ret = AMP_DISP_VID_PopFrame(objDS, AMP_DISP_PLANE_MAIN, tempBD, &_ev);
        if (Ret != SUCCESS) {
            break;
        }

        hBD = (AMP_BD_HANDLE)(tempBD->uiAllocVA);
        Ret = AMP_BDCHAIN_PushItem(pAppQue, hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not push bd!", __FUNCTION__, __LINE__);
            goto request_free_bd_exit;
        }
    }

    free(tempBD);

    Ret = AMP_BDCHAIN_PopItem(pAppQue, &hBD);
    if ((Ret == SUCCESS) && (hBD)) {
        return hBD;
    }

request_free_bd_exit:
    return NULL;
}

#if 0
/*Build in frame*/
// PerWord is buggy, too.
HRESULT disp_make_frame_data(INT32 iSrcFmt,   volatile UINT32 *pStartAddr,
                               UINT32 uiPicH,   UINT32 uiLineV, UINT32 uiWidth,
                               UINT32 uiHeight, UINT32 uiPicA,  UINT32 uiPicB)
{
    UINT32 *Ptr;
    UINT32 VLines, VRep,  HS, HRep, PixVal, VBlock, HBlock, PixelPerWord;

    HRESULT Ret = SUCCESS;

    if (!pStartAddr) {
        Ret = ERR_ERRPARAM;
        goto make_frame_data_exit;
    }

    /**
    * make the whole frame data
    */
    Ptr = pStartAddr;

    if (iSrcFmt == 0) {     //YUV422
        CMDLINE_PRINT("**************YUV422**********!\n");
        PixelPerWord = 2;
    }
    else if (iSrcFmt == 11) {       //ARGB32
        CMDLINE_PRINT("**************ARGB32**********!\n");
        PixelPerWord = 1;
    } else {
        CMDLINE_PRINT("**************UNKOWN**********!\n");
        PixelPerWord = 1;
    }

    VBlock = uiLineV;

    for(VLines = 0; VLines < uiHeight; VLines += VBlock) {
        if ((uiHeight - VLines) < VBlock) {
            VBlock = uiHeight - VLines;
        }

        for (VRep=0; VRep < VBlock; VRep++) {
            HBlock = uiPicH * PixelPerWord;

            for (HS = 0; HS < uiWidth; HS += HBlock) {
                if ((VLines / uiLineV + HS/(uiPicH * PixelPerWord)) & 0x1) {
                    PixVal = uiPicB;
                }
                else {
                    PixVal = uiPicA;
                }

                if ((uiWidth - HS) < HBlock) {
                    HBlock = uiWidth - HS;
                }
                for (HRep = 0; HRep < HBlock/PixelPerWord; HRep++) {
                    *Ptr++ = PixVal;
                }
            }
        }
    }

make_frame_data_exit:
  return Ret;

}

HRESULT disp_build_frame(INT32 iVideo, AMP_BGTAG_FRAME_INFO *pFrameInfo,
                           INT32 iSrcFmt, INT32 iOrder, INT32 iX, INT32 iY,
                           UINT32 uiWidth,  UINT32 uiHeight, INT32 iProgressive,
                           UINT32 uiPixel1, UINT32 uiPixel2, UINT32 uiPar)
{
    VOID *Base;
    AMP_SHM_HANDLE hShm = 0;
    HRESULT Ret = SUCCESS;
    INT32 Size,ShmOffset;

    AmpMemClear(pFrameInfo, sizeof(AMP_BGTAG_FRAME_INFO));

    pFrameInfo->Header.eType   = AMP_BGTAG_FRAME_INFO_META;
    pFrameInfo->Header.uLength = sizeof(AMP_BGTAG_FRAME_INFO);

    /**
    * set other fields of logo frame descriptor
    */
    // uiBpp is buggy
    if (iSrcFmt == 0) {     /*UYUV fmt*/
        /**
        * number of bytes per pixel
        */
        pFrameInfo->uiBpp = 2;
    } else if (iSrcFmt == 11){
        pFrameInfo->uiBpp = 4;
    } else if (iSrcFmt == DISP_SRCFMT_YUV420) {
        pFrameInfo->uiBpp = 3;
    } else {
        pFrameInfo->uiBpp = 4;
        CMDLINE_PRINT("unkown source format %d, default bpp to %d\n", iSrcFmt, pFrameInfo->uiBpp);
    }
    Size = uiWidth * uiHeight * pFrameInfo->uiBpp;

    Ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, Size, 1024, &hShm);

    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Allocate Fails\n");
        Ret = ERR_NOMEM;
        goto build_frame_exit;
    }

    /**
    * get virtual address
    */
    Ret = AMP_SHM_GetVirtualAddress(hShm, 0, &Base);

    if ((Ret != SUCCESS) || !Base) {
        CMDLINE_PRINT("AMP_SHM_GetVirtualAddress Fails,Ret:%d, Base:%d\n", Ret, Base);
        Ret = ERR_NOMEM;
        goto build_frame_exit;
    }

    Ret = disp_make_frame_data(iSrcFmt, (volatile UINT32 *)Base, 32, 36,
                          uiWidth, uiHeight, uiPixel1, uiPixel2);

    if (Ret != SUCCESS) {
        CMDLINE_PRINT("make_frame_data fails");
        goto build_frame_exit;
    }

    /**
    * flush data in cache
    */
    Ret = AMP_SHM_CleanCache(hShm, 0, Size);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_CleanCache Fails,Ret:%d\n", Ret);
        Ret = ERR_NOMEM;
        goto build_frame_exit;
    }

    /**
    * get cache physical address for DMA
    */
    Ret = AMP_SHM_GetPhysicalAddress(hShm, 0, &Base);

    if ((Ret != SUCCESS) || !Base) {
        CMDLINE_PRINT("AMP_SHM_GetPhysicalAddress Fails,Ret:%d, Base:%d\n", Ret, Base);
        Ret = ERR_NOMEM;
        goto build_frame_exit;
    }

    pFrameInfo->uStatus = AMP_FRAME_ST_DECODED;
    pFrameInfo->pBufStart = Base;
    pFrameInfo->uiSrcFmt = iSrcFmt;
    pFrameInfo->uiOrder = iOrder;
    pFrameInfo->uiContentW = uiWidth;
    pFrameInfo->uiContentH = uiHeight;
    pFrameInfo->iActiveOff = 0;

    pFrameInfo->uiBufStride = pFrameInfo->uiContentW
        * pFrameInfo->uiBpp;

    pFrameInfo->iActiveOff += iY * pFrameInfo->uiBufStride
        + iX * pFrameInfo->uiBpp;

    pFrameInfo->uiIsFs = iProgressive;
    pFrameInfo->uiFrNum = 60000;
    pFrameInfo->uiFrDen = 1001;
    pFrameInfo->uiActiveW = uiWidth;
    pFrameInfo->uiActiveH = uiHeight;
    pFrameInfo->iActiveX = iX;
    pFrameInfo->iActiveY = iY;
    pFrameInfo->uiPar = uiPar;

    pFrameInfo->uiBitsPerPixel = 8;
    pFrameInfo->uiPbufStartUV = 0; //Start Address of UV data in 420SP format
    pFrameInfo->uiBufStrideUV = 0; //tride of UV data in 420SP format

    pFrameInfo->uiClutKey = -1;
    pFrameInfo->uiClutStart = 0;
    pFrameInfo->uiClutLength = 255;

    if(pFrameInfo->uiSrcFmt == 11){
        pFrameInfo->uiBitsPerPixel = 8;
        pFrameInfo->uiBufStrideUV = uiWidth * 4;
        pFrameInfo->uiOrder = 2; //RGBA
    }

build_frame_exit:
    return Ret;

}
#endif

AMP_BD_HANDLE disptest_getAmpBD(AMP_DISP objDS, int PlaneID)
{
    AMP_BD_HANDLE hBD;
    while (1) {
        hBD = request_free_bd(objDS, PlaneID);
        if (hBD) {
            break;
        }
        else {
            CMDLINE_PRINT("can not get free bd, wait a moment!");
            MV_OSAL_Task_Sleep(10);
        }
    }
    return hBD;
}
void AddWaterMark(int k, char *pBuf, int w, int h)
{
	// UYVY 422
	int i,j;
	char tmp_buf[1920*2];
	//int strid_w = w * 2;
	
	memcpy(tmp_buf,pBuf,w*2);
	
	for(i = 0; i < h - 1; i ++)
	{
		memcpy(pBuf+w*2*i,pBuf+w*2*(i+1),w*2);
	}
	memcpy(pBuf+w*2*(h-1) , tmp_buf,w*2);
}
HRESULT disptest_push_buildinframe(AMP_DISP objDS, int PlaneID, DISP_TEST_FRAME frame_info)
{
    AMP_BD_HANDLE hBD, hElBD = NULL, hMdBD = NULL;
    AMP_BD_HANDLE hThdrMdBD;
    HRESULT Ret = SUCCESS;
    AMP_BGTAG_FRAME_INFO FrameInfo;
    AMP_BGTAG_FRAME_INFO *pFrameInfo;
    AMP_BGTAG_FRAME_INFO FrameInfoEL;
    AMP_BDTAG_MEMINFO MemInfo;
    UINT32 uiIndex;
    UINT32 uiBytes;
    AMP_DISP_ZORDER NewZorder;
    AMP_DISP_ZORDER DefaultZorder;
    AMP_DISP_WIN SrcWin, DstWin;
    AMP_DISP_TG_ID TG_ID = 0; // default to 0
    AMP_HDR_INFO *pstHdrInfo = NULL;

    /* Display service test */
    AmpMemClear(&SrcWin, sizeof(AMP_DISP_WIN));
    AmpMemClear(&DstWin, sizeof(AMP_DISP_WIN));

    if(frame_info.isSec == 1) {
        create_buildin_frame_secure(&FrameInfo,frame_info.color_fmt,frame_info.w,
            frame_info.h, frame_info.intelace_mode,frame_info.bit_depth, frame_info.src_order,
            frame_info.pattern, frame_info.raw_frame_path, &frame_info.rampAttr,frame_info.frame_type);
    } else if (PlaneID != AMP_DISP_PLANE_4K_BYPASS) {
        TG_ID = 0;
        create_buildin_frame(&FrameInfo,frame_info.color_fmt,frame_info.w,
            frame_info.h, frame_info.intelace_mode,frame_info.bit_depth, frame_info.src_order,
            frame_info.pattern, frame_info.raw_frame_path, &frame_info.rampAttr,frame_info.frame_type);

        // AMP_RPC(Ret, AMP_DISP_SetScale, objDS, PlaneID, &SrcWin, &DstWin);
    } else if (PlaneID == AMP_DISP_PLANE_4K_BYPASS) {
        TG_ID = 1;
        CMDLINE_PRINT("call create_buildin_frame as 4K_BYPASS plane\n");
        create_buildin_frame(&FrameInfo,frame_info.color_fmt,frame_info.w,
            frame_info.h, frame_info.intelace_mode,frame_info.bit_depth, frame_info.src_order,
            frame_info.pattern, frame_info.raw_frame_path, &frame_info.rampAttr, frame_info.frame_type);

        // AMP_RPC(Ret, AMP_DISP_SetScale, objDS, PlaneID, &SrcWin, &DstWin);
        // Ret = disp_build_frame(1, &FrameInfo, frame_info.color_fmt, 0, 0, 0,
        //                       frame_info.w, frame_info.h, 1, 0x40604060, 0xb0a0b0a0, 0x10001);
    }
    if(DOLBY_FRAME_TYPE_BL_EL_MD == frame_info.frame_type){
        create_buildin_frame(&FrameInfoEL,frame_info.color_fmt,frame_info.sub_w,
                frame_info.sub_h, frame_info.intelace_mode,frame_info.bit_depth, frame_info.src_order,
                frame_info.pattern, frame_info.raw_frame_subpath, &frame_info.rampAttr,0);
    }
    if(frame_info.frame_type >= DOLBY_FRAME_TYPE_16bBL_MD && frame_info.frame_type <= DOLBY_FRAME_TYPE_BL_P5EL_MD)
    {
        create_buildin_metadata_frame(&MemInfo, frame_info.comp_md_path, frame_info.dm_md_path);
    }

    if (IS_MMU_ONLY_ENABLED(frame_info.isMtr)) {
        FrameInfo.uiIsMtr = MMU_ENABLED;
        update_mtrr_info(&FrameInfo, frame_info );
    }
    pFrameInfo = &FrameInfo;
    pFrameInfo->iSrcWinX     = m_PerFrameSrcWin[PlaneID].iX;
    pFrameInfo->iSrcWinY     = m_PerFrameSrcWin[PlaneID].iY;
    pFrameInfo->uiSrcWinW    = m_PerFrameSrcWin[PlaneID].iWidth;
    pFrameInfo->uiSrcWinH    = m_PerFrameSrcWin[PlaneID].iHeight;

    pFrameInfo->iDestWinX    = m_PerFrameDstWin[PlaneID].iX;
    pFrameInfo->iDestWinY    = m_PerFrameDstWin[PlaneID].iY;
    pFrameInfo->uiDestWinW   = m_PerFrameDstWin[PlaneID].iWidth;
    pFrameInfo->uiDestWinH   = m_PerFrameDstWin[PlaneID].iHeight;

    if((pFrameInfo->iSrcWinX || pFrameInfo->iSrcWinY || pFrameInfo->uiSrcWinW || pFrameInfo->uiSrcWinH) ||
       (pFrameInfo->iDestWinX || pFrameInfo->iDestWinY || pFrameInfo->uiDestWinW || pFrameInfo->uiDestWinH)) {
        //pFrameInfo->uiGFXFrameFlag == TRUE;       //This is only for GFX frame
        pFrameInfo->uiBufFlag   = VINFO_FLAG_MASK_SCALING_FRAME;
        //pFrameInfo->uiBufFlag   = FLAG_MASK_SCALING_FRAME;

        printf("Per Frame Scaling-copied/enabled: %d,%d,%d,%d  -> %d,%d,%d,%d , flag:%x\n",
            pFrameInfo->iSrcWinX, pFrameInfo->iSrcWinY, pFrameInfo->uiSrcWinW, pFrameInfo->uiSrcWinH,
            pFrameInfo->iDestWinX, pFrameInfo->iDestWinY, pFrameInfo->uiDestWinW, pFrameInfo->uiDestWinH, pFrameInfo->uiBufFlag);
    } else {
        printf("Per Frame Scaling not copied/enabled: flag:%x\n", pFrameInfo->uiBufFlag);
    }

    create_bd_queue();
    AMP_RPC(Ret, AMP_DISP_GetPlaneZOrder, objDS, TG_ID, &DefaultZorder);

    if (PlaneID == AMP_DISP_PLANE_MAIN) {
        /* Change Z-order */
#if (BERLIN_CHIP_VERSION >= BERLIN_BG4_CD && BERLIN_CHIP_VERSION != BERLIN_BG4_DTV)
        NewZorder.iMain = 0;
        NewZorder.iPip  = 1;
        NewZorder.iGfx0 = 2;
#else
        NewZorder.iMain = 6;
        NewZorder.iPip  = 5;
        NewZorder.iGfx0 = 4;
        NewZorder.iGfx1 = 3;
        NewZorder.iGfx2 = 2;
        NewZorder.iPg   = 1;
        NewZorder.iBg   = 0;
#endif
    } else if (PlaneID == AMP_DISP_PLANE_PIP) {
        /* Change Z-order */
#if (BERLIN_CHIP_VERSION >= BERLIN_BG4_CD && BERLIN_CHIP_VERSION != BERLIN_BG4_DTV)
        NewZorder.iMain = 0;
        NewZorder.iPip  = 1;
        NewZorder.iGfx0 = 2;
#else
        NewZorder.iMain = 5;
        NewZorder.iPip  = 6;
        NewZorder.iGfx0 = 4;
        NewZorder.iGfx1 = 3;
        NewZorder.iGfx2 = 2;
        NewZorder.iPg   = 1;
        NewZorder.iBg   = 0;
#endif
    } else if (PlaneID == AMP_DISP_PLANE_GFX0) {
        /* Change Z-order */
#if (BERLIN_CHIP_VERSION >= BERLIN_BG4_CD && BERLIN_CHIP_VERSION != BERLIN_BG4_DTV)
        NewZorder.iMain = 0;
        NewZorder.iPip  = 1;
        NewZorder.iGfx0 = 2;
#else
        NewZorder.iMain = 4;
        NewZorder.iPip  = 5;
        NewZorder.iGfx0 = 6;
        NewZorder.iGfx1 = 3;
        NewZorder.iGfx2 = 2;
        NewZorder.iPg   = 1;
        NewZorder.iBg   = 0;
#endif
    } else {
        /* Change Z-order */
#if (BERLIN_CHIP_VERSION >= BERLIN_BG4_CD && BERLIN_CHIP_VERSION != BERLIN_BG4_DTV)
        NewZorder.iMain = 2;
        NewZorder.iPip  = 1;
        NewZorder.iGfx0 = 0;
#else
        NewZorder.iMain = 6;
        NewZorder.iPip  = 5;
        NewZorder.iGfx0 = 4;
        NewZorder.iGfx1 = 3;
        NewZorder.iGfx2 = 2;
        NewZorder.iPg   = 1;
        NewZorder.iBg   = 0;
#endif
    }

    AMP_RPC(Ret, AMP_DISP_SetPlaneZOrder, objDS, AMP_DISP_TG_PRI, &NewZorder);

    FrameInfo.uStatus = AMP_FRAME_ST_DECODED;
    FrameInfo.uiDvPresentMode = AMP_FRAME_TYPE_LEGACY; //BL
    if((frame_info.frame_type >= DOLBY_FRAME_TYPE_BL) && (frame_info.frame_type <= DOLBY_FRAME_TYPE_BL_P5EL_MD)){
        FrameInfo.uiDvPresentMode = AMP_FRAME_TYPE_DOLBY_BL_MD; //BL+MD
        if(DOLBY_FRAME_TYPE_BL_EL_MD == frame_info.frame_type){
            FrameInfo.uiDvPresentMode = AMP_FRAME_TYPE_DOLBY_BL_EL_MD; //BL+EL+MD
        }
    }
	
    while (1) {
		
		printf("disptest_push_buildinframe ... \n");
		
        hBD = disptest_getAmpBD(objDS,PlaneID);

        Ret = AMP_BDTAG_Clear(hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not clear bd, error code is %x!",
                 __FUNCTION__,
                 __LINE__,
                 Ret);
            goto disptest_entry_exit;
        }

        FrameInfo.uiThdrPresentMode = frame_info.uiThdrPresentMode;
        FrameInfo.m_iDisplayOETF = frame_info.m_iDisplayOETF;
        FrameInfo.m_iPeakLuminance = frame_info.m_iPeakLuminance;
        FrameInfo.m_iProcessMode = frame_info.m_iProcessMode;

        if(0x3 == frame_info.uiThdrPresentMode)
        {
                //Mark this frame as HDR frame
            printf(" File names are %s %s at %s \n", frame_info.comp_md_path, frame_info.dm_md_path, __FUNCTION__);

            create_buildin_tHDR_metadata_frame(&MemInfo,frame_info.comp_md_path, frame_info.dm_md_path);

            hThdrMdBD = disptest_getAmpBD(objDS, PlaneID);

            Ret = AMP_BDTAG_Clear(hThdrMdBD);
            if (Ret != SUCCESS)
            {
                CMDLINE_PRINT("%s:%d, can not clear bd, error code is %x!",
                        __FUNCTION__,
                        __LINE__,
                        Ret);
                goto disptest_entry_exit;
            }

            AmpMemcpy(&FrameInfo.thdrMetaBD, hThdrMdBD, sizeof(AMP_BD_ST));

            uiIndex = 0;
            Ret = AMP_BDTAG_Append(hThdrMdBD, (UINT8 *)&MemInfo, &uiIndex, &uiBytes);

            if (Ret != SUCCESS) {
                CMDLINE_PRINT("%s:%d, can not set bd tag, error code is %x!",
                        __FUNCTION__,
                        __LINE__,
                        Ret);
                goto disptest_entry_exit;
            }

        }

        if((FrameInfo.uiDvPresentMode == AMP_FRAME_TYPE_DOLBY_BL_MD) || (FrameInfo.uiDvPresentMode == AMP_FRAME_TYPE_DOLBY_BL_EL_MD))
        {
            if(FrameInfo.uiDvPresentMode == AMP_FRAME_TYPE_DOLBY_BL_EL_MD)
            {
                hElBD = disptest_getAmpBD(objDS,PlaneID);

                Ret = AMP_BDTAG_Clear(hElBD);
                if (Ret != SUCCESS) {
                    CMDLINE_PRINT("%s:%d, can not clear bd, error code is %x!",
                            __FUNCTION__,
                            __LINE__,
                            Ret);
                    goto disptest_entry_exit;
                }
                AmpMemcpy(&FrameInfo.DvElBD,hElBD,sizeof(AMP_BD_ST));
            }
            hMdBD = disptest_getAmpBD(objDS,PlaneID);

            Ret = AMP_BDTAG_Clear(hMdBD);
            if (Ret != SUCCESS) {
                CMDLINE_PRINT("%s:%d, can not clear bd, error code is %x!",
                        __FUNCTION__,
                        __LINE__,
                        Ret);
                goto disptest_entry_exit;
            }
            AmpMemcpy(&FrameInfo.DvRpuBD,hMdBD,sizeof(AMP_BD_ST));
        }

        if(frame_info.hdrEn)
        {
            pstHdrInfo = (AMP_HDR_INFO *) &(FrameInfo.sHdrInfo);

            pstHdrInfo->type = AMP_HDR_TYPE_HDR10;
            pstHdrInfo->PrimaryChromaticityR[0] = 0.708;
            pstHdrInfo->PrimaryChromaticityR[1] = 0.292;
            pstHdrInfo->PrimaryChromaticityG[0] = 0.170;
            pstHdrInfo->PrimaryChromaticityG[1] = 0.797;
            pstHdrInfo->PrimaryChromaticityB[0] = 0.131;
            pstHdrInfo->PrimaryChromaticityB[1] = 0.046;
            pstHdrInfo->WhitePointChromaticity[0] = 0.31270;
            pstHdrInfo->WhitePointChromaticity[1] = 0.32900;
            pstHdrInfo->LuminanceMax = 4000;
            pstHdrInfo->LuminanceMin = 0;
            pstHdrInfo->TransferCharacteristics = 16;    //VPP_TRANSFER_CHAR_SMPTE_ST_2048
            pstHdrInfo->MatrixCoefficients = 0;
            pstHdrInfo->MaxCLL = 4008;
            pstHdrInfo->MaxFALL = 64;
            FrameInfo.sHdrInfo.ColorPrimaries = 9;     //VPP_COLOR_PRIM_BT2020
        }
        else if(frame_info.hlgEn)
        {
            pstHdrInfo = (AMP_HDR_INFO *) &(FrameInfo.sHdrInfo);

            pstHdrInfo->type = AMP_HDR_TYPE_HLG;
            pstHdrInfo->PrimaryChromaticityR[0] = 0;
            pstHdrInfo->PrimaryChromaticityR[1] = 0;
            pstHdrInfo->PrimaryChromaticityG[0] = 0;
            pstHdrInfo->PrimaryChromaticityG[1] = 0;
            pstHdrInfo->PrimaryChromaticityB[0] = 0;
            pstHdrInfo->PrimaryChromaticityB[1] = 0;
            pstHdrInfo->WhitePointChromaticity[0] = 0;
            pstHdrInfo->WhitePointChromaticity[1] = 0;
            pstHdrInfo->LuminanceMax = 0;
            pstHdrInfo->LuminanceMin = 0;
            pstHdrInfo->TransferCharacteristics = 18;    //VPP_TRANSFER_CHAR_ARIB_B67
            pstHdrInfo->MatrixCoefficients = 0;
            pstHdrInfo->MaxCLL = 0;
            pstHdrInfo->MaxFALL = 0;
            FrameInfo.sHdrInfo.ColorPrimaries = 9;     //VPP_COLOR_PRIM_BT2020
        }

        Ret = AMP_BDTAG_Append(hBD,
                               (UINT8 *)&FrameInfo,
                               &uiIndex,
                               &uiBytes);

        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not set bd tag, error code is %x!",
                 __FUNCTION__,
                 __LINE__,
                 Ret);
            goto disptest_entry_exit;
        }
        //EL
        if(DOLBY_FRAME_TYPE_BL_EL_MD == frame_info.frame_type && hElBD != NULL){
            uiIndex = 0;
            FrameInfoEL.uStatus = AMP_FRAME_ST_DECODED;
            Ret = AMP_BDTAG_Append(hElBD,
                    (UINT8 *)&FrameInfoEL,
                    &uiIndex,
                    &uiBytes);

            if (Ret != SUCCESS) {
                CMDLINE_PRINT("%s:%d, can not set bd tag, error code is %x!",
                        __FUNCTION__,
                        __LINE__,
                        Ret);
                goto disptest_entry_exit;
            }

        }
        //Metadata
        if(frame_info.frame_type >= DOLBY_FRAME_TYPE_16bBL_MD && frame_info.frame_type <= DOLBY_FRAME_TYPE_BL_P5EL_MD
           && hMdBD != NULL)
        {
            uiIndex = 0;
            Ret = AMP_BDTAG_Append(hMdBD, (UINT8 *)&MemInfo, &uiIndex, &uiBytes);

            if (Ret != SUCCESS) {
                CMDLINE_PRINT("%s:%d, can not set bd tag, error code is %x!",
                        __FUNCTION__,
                        __LINE__,
                        Ret);
                goto disptest_entry_exit;
            }
        }

        AMP_BDTAG_MEMINFO MemInfoTag;
        AmpMemSet(&MemInfoTag, 0, sizeof(MemInfoTag));
        MemInfoTag.Header.eType = AMP_BDTAG_ASSOCIATE_MEM_INFO;
        MemInfoTag.Header.uLength = sizeof(AMP_BDTAG_MEMINFO);
        MemInfoTag.uMemHandle = (UINT32) pFrameInfo->hShm;
        MemInfoTag.uMemOffset = 0;
        MemInfoTag.uSize = get_frame_size(frame_info.color_fmt, frame_info.bit_depth,\
                                                   frame_info.w, frame_info.h);
        set_frameinfo_data(pFrameInfo, frame_info.color_fmt, frame_info.w, frame_info.h,
                        frame_info.intelace_mode, frame_info.bit_depth, frame_info.src_order);

        AMP_BDTAG_Append(hBD, (UINT8 *)&MemInfoTag, NULL, NULL);

        if(frame_info.frame_type == DOLBY_FRAME_TYPE_LEGACY_GFX) {
            CMDLINE_PRINT("Pushing a GFX/RGB frame using <AMP_DISP_GFX_PushBD>\r\n");
            AMP_RPC(Ret, AMP_DISP_GFX_PushBD, objDS, PlaneID, hBD);
        }
        else
        {
			struct timeval start;
			struct timeval end;
			static int num =0; 
			double time_diff;
			//int frame_num = 1000;
			int k = 0,j;
			int ret_buf = 0;
			VOID *Base1 =NULL;
            void *base2 =NULL;
            char *buf2 =NULL;
            char *p_data=NULL;
			Ret = AMP_SHM_GetVirtualAddress(FrameInfo.hShm, 0, &Base1);
			p_data = (char *)Base1;
			gettimeofday(&start,NULL);

            //Ret = AMP_SHM_GetVirtualAddress(FrameInfo.hShm, 1920*1080*2, &base2);
           // printf("get frame buf2 ret %d\n",Ret);
            //if(Ret == SUCCESS)
            //    buf2 = (char*)base2;
            
            //printf("the p_data %p buf2 %p\n",p_data,buf2);

			printf("start pull data to hdmi ... \n");
			while(1)
			{
				    ret_buf = ndi_buf_pull_hdmi(p_data);

                    AMP_SHM_CleanCache(FrameInfo.hShm, 0,1920*1080*2);

				    if(ret_buf >=0)
				    {
                        k ++;
                        AMP_RPC(Ret, AMP_DISP_VID_PushFrame, objDS, PlaneID, hBD);
                        if(k % 100 == 0)
                        {
                            gettimeofday(&end,NULL);
                            time_diff = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
                            printf("pull hdmi fps =  %f\n",1000*(double)100 / time_diff);
                            gettimeofday(&start,NULL);
                            if(k % 1000 == 0)
                            {
                                FILE *fp_save = NULL;
                                char save_name[128];
                                sprintf(save_name,"/root/save_%d.bin",start.tv_sec);
                                printf("save file =  %s\n",save_name);
                                fp_save = fopen(save_name,"wb");
                                if(fp_save)
                                {
                                    fwrite(p_data,1,1920*1080*2,fp_save);
                                    fclose(fp_save);
                                }
                                
                            }
					    }
					//AMP_RPC(Ret, AMP_DISP_Push_BuildInFrame, objDS, PlaneID, 0);

                }

				MV_OSAL_Task_Sleep(10);
				
			}
			/*for(k = 0; k < frame_num; k ++)
			{
				AddWaterMark(k,p_data,1920,1080);
				//printf("push frame %d\n",k);
				AMP_RPC(Ret, AMP_DISP_VID_PushFrame, objDS, PlaneID, hBD);
				MV_OSAL_Task_Sleep(5);
				//AMP_RPC(Ret, AMP_DISP_Push_BuildInFrame, objDS, PlaneID, 0);
				//MV_OSAL_Task_Sleep(500);
			}
			gettimeofday(&end,NULL);
			time_diff = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
			printf("fps =  %f\n",1000*(double)frame_num / time_diff);
			*/
			
        }
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not push frame, error code is %x!",
                 __FUNCTION__, __LINE__, Ret);
            goto disptest_entry_exit;
        }

        break;
    }

    prev_hBD = hBD;

     if(0x1 == frame_info.uiThdrPresentMode)
     {
         int aiParam[10] = {0};
         aiParam[0] = 1;
         aiParam[1] = frame_info.vitm_params.iYuvInRang;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 2;
         aiParam[1] = frame_info.vitm_params.iYuvOutRange;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 3;
         aiParam[1] = frame_info.vitm_params.iGaussianFilter;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 4;
         aiParam[1] = frame_info.vitm_params.iYLutFilter;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 5;
         aiParam[1] = frame_info.vitm_params.iPqEn;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 6;
         aiParam[1] = frame_info.vitm_params.iPqLmax;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 7;
         aiParam[1] = frame_info.vitm_params.iInCscMode;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 8;
         aiParam[1] = frame_info.vitm_params.iOutCscMode;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 9;
         aiParam[1] = frame_info.vitm_params.iDebanding;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);
         aiParam[0] = 10;
         aiParam[1] = frame_info.vitm_params.iDenoising;
         AMP_RPC(Ret, AMP_DISP_OUT_SetThdrVitmParams, objDS, aiParam[0], aiParam[1]);

         if (Ret != SUCCESS) {
             CMDLINE_PRINT("%s:%d, can not set VITM params, error code is %x!",
                     __FUNCTION__,
                     __LINE__,
                     Ret);
         }
     }
    CMDLINE_PRINT("%s:%d, FrameInfo.uiDvPresentMode %d frame_type %u\r\n",
            __FUNCTION__, __LINE__, FrameInfo.uiDvPresentMode, frame_info.frame_type);
    return Ret;

disptest_entry_exit:
    return Ret;
}

/*Event test*/
static HRESULT test_event_handler(HANDLE hListener, AMP_EVENT *pEvent,
                                  VOID *pUserData)
{
    MV_UNUSED(hListener);
    MV_UNUSED(pUserData);
    INT32 *pPayLoad = AMP_EVENT_PAYLOAD_PTR(pEvent);

    if (AMP_EVENT_GETCODE (*pEvent) == AMP_EVENT_API_DISP_CALLBACK_VSYNC) {
        //AMPLOGD("received vsync event from display service\n");
    }
    else if (AMP_EVENT_GETCODE (*pEvent) == AMP_EVENT_API_DISP_CALLBACK_HDMI) {
        if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_SINK_CONNECTED) {
            CMDLINE_PRINT("received hdmi connected event from display service\n");
        }
        else if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_SINK_DISCONNECTED){
            CMDLINE_PRINT("received hdmi disconnected event from display service\n");
        }
        if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_START) {
            CMDLINE_PRINT("received hdr10 playback started event from display service\n");
        }
        else if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_STOP){
            CMDLINE_PRINT("received hdr10 playback stopped event from display service\n");
        }
        if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_DV_PLAYBACK_START) {
            CMDLINE_PRINT("received DV playback started event from display service\n");
        }
        else if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_DV_PLAYBACK_STOP){
            CMDLINE_PRINT("received DV playback stopped event from display service\n");
        }
        if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_DV_ON) {
            CMDLINE_PRINT("received switched to dolby event from display service\n");
        }
        else if (pPayLoad[0] == AMP_DISP_EVENT_HDMI_DV_OFF){
            CMDLINE_PRINT("received switched to legacy event from display service\n");
        }
    }
    else if (AMP_EVENT_GETCODE (*pEvent) == AMP_EVENT_API_DISP_CALLBACK_CEC) {
        CMDLINE_PRINT("received cec event from display service (event id: %d)\n", pPayLoad[0]);
    }
    return SUCCESS;
}

VOID test_disp_event(VOID)
{
    HANDLE hListener;

    hListener = AMP_Event_CreateListener(16, 0);

    AMP_Event_RegisterCallback(hListener,
                               AMP_EVENT_API_DISP_CALLBACK_VSYNC,
                               test_event_handler, hListener);

    AMP_Event_RegisterCallback(hListener,
                               AMP_EVENT_API_DISP_CALLBACK_HDMI,
                               test_event_handler, hListener);

    AMP_Event_RegisterCallback(hListener,
                               AMP_EVENT_API_DISP_CALLBACK_CEC,
                               test_event_handler, hListener);
}

/*Capture frame test*/
HRESULT capture_event_handler(HANDLE hListener, AMP_EVENT * pEvent,
                              VOID * pUserData)
{
    MV_UNUSED(hListener);
    MV_UNUSED(pUserData);
    HRESULT result = SUCCESS;
    FILE *fpCapFile = NULL;
    char cFileName[128];
    UINT8 *pBuf = NULL;
    static UINT32 cap_frame_cnt = 0;

    if(pEvent->stEventHead.eEventCode == AMP_EVENT_API_DISP_CALLBACK_AUXCAPTURE){
        AMP_DISP_VPP_AUX_FRAME_INFO *pCapFrame =
            (AMP_DISP_VPP_AUX_FRAME_INFO *)AMP_EVENT_PAYLOAD_PTR(pEvent);

        cap_frame_cnt++;
        if(!pCapFrame->uSuccess){
            CMDLINE_PRINT("DISP test:%d success to capture screen from aux channel, "
                     "viewID: %d, capture size: %d\n",__LINE__,
                    pCapFrame->viewID,pCapFrame->uSize);

            sprintf(cFileName,"/data/cap_auxframe_%d.yuv", cap_frame_cnt);
            fpCapFile = fopen(cFileName, "wb");
            if (fpCapFile) {
                result = AMP_SHM_GetVirtualAddress(hCapBuf,
                                                   0,
                                                   (void **) &pBuf);
                fwrite (pBuf, 1, pCapFrame->uSize, fpCapFile);
                fflush (fpCapFile);
                fclose (fpCapFile);
                CMDLINE_PRINT("DISP test:%d save a captured frame!\n",__LINE__);
            }else{
                CMDLINE_PRINT("DISP test:%d the file is null!\n",__LINE__);
            }
        }else{
            CMDLINE_PRINT("DISP test: %d fail to capture screen from aux channel, "
                 "viewID: %d, capture size: %d, success: %d\n",__LINE__,
                    pCapFrame->viewID,pCapFrame->uSize, pCapFrame->uSuccess);
        }
    }
    return result;
}

HRESULT disp_set_hdmi(unsigned int resID, unsigned int colorfmt,
                      unsigned int bitdepth, unsigned int aspect, unsigned int tgid)
{
    HRESULT result = SUCCESS;
    UINT32 uiResId = resID & 0x7FFFFFFF;
    AMP_DISP_OUT_PARAMS disp_params;
    AmpMemClear(&disp_params, sizeof(AMP_DISP_OUT_PARAMS));

    if((uiResId < MAX_NUM_RESS) && (colorfmt < MAX_NUM_OUTPUT_COLOR_FMTS) &&
            (bitdepth <MAX_NUM_OUTPUT_BIT_DEPTHS))
        CMDLINE_PRINT("Set HDMI: Resolution: %s, Color Fmt: %s, Bitdepth: %s\n",
                                    m_hdmitx_resolution_table[uiResId],
                                    m_hdmitx_colorfmt_table[colorfmt],
                                    m_hdmitx_bitdepth_table[bitdepth]);

    disp_params.uiDispId = AMP_DISP_OUT_HDMI;
    disp_params.uiResId = uiResId;
    disp_params.uiBitDepth = bitdepth;
    disp_params.iPixelRepeat = 1;
    if (colorfmt >= MAX_NUM_OUTPUT_COLOR_FMTS) {
        CMDLINE_PRINT("Set output fmt with default value: OUTPUT_COLOR_FMT_YCBCR444!\n");
        disp_params.uiColorFmt = 1;
    } else {
        CMDLINE_PRINT("Set output fmt %s!\n", m_hdmitx_colorfmt_table[colorfmt]);
        disp_params.uiColorFmt = colorfmt;
    }

    AMP_RPC(result, AMP_DISP_OUT_SetFormat, gDisp, 0, &disp_params);
    if (0 != result) {
        CMDLINE_PRINT("AMP_DISP_OUT_SetFormat failed with error code:0x%x.\n", result);
    } else {
        CMDLINE_PRINT("AMP_DISP_OUT_SetFormat success.\n");
    }

    if(resID < RES_720P30) {
        /*
         * for SD resolution, if aspect ration is specified by cmd line,
         * configure it to vpp drv and keep the scaninfo unchanged.
        */
        if (aspect == 1 ||
            aspect == 2) {
            CMDLINE_PRINT("configure aspect ratio to %s\n",
                          (aspect == 1)? "4:3":"16:9");
            AMP_RPC(result, AMP_DISP_OUT_HDMI_SetVidInfo, gDisp, aspect, -1);
        }
    }
    return result;
}

HRESULT disp_setplane_mute(unsigned int uiPlaneID, unsigned int uiMute)
{
    HRESULT result = SUCCESS;
    AMP_RPC(result, AMP_DISP_SetPlaneMute, gDisp, uiPlaneID, uiMute);
    if (0 != result) {
        CMDLINE_PRINT("AMP_DISP_SetPlaneMute failed with error code:0x%x.\n", result);
    } else {
        CMDLINE_PRINT("AMP_DISP_SetPlaneMute success.\n");
    }
    return result;
}

#define MAX_FRAME_BUFFER    (8) //Max there will be 8 frame buffered during DDR
typedef struct test_frame_queue_t{
    AMP_BGTAG_FRAME_INFO frame_info;
    AMP_SHM_HANDLE            frame_handle;
    void                                 *vir_addr;
    void                                 *phy_addr;
    bool                                 is_free;
}test_frame_queue;
static test_frame_queue frame_queue[MAX_FRAME_BUFFER];

#if 0
/*This task will read frame from local file and put them during a frame q*/
static void read_frame_task(DISP_TEST_FRAME *frame_info)
{
    int cnt, ret;
    FILE *yuv_file = NULL;
    unsigned int frame_size;

    CMDLINE_PRINT("Enter read_frame_task\n");

    yuv_file = fopen(frame_info->raw_frame_path, "rw");
    if (!yuv_file) {
        CMDLINE_PRINT("File open faill!\n");
        return;
    }

    frame_size = get_frame_size(frame_info->color_fmt, frame_info->bit_depth,\
                                                   frame_info->w, frame_info->h);
    /*Allocate frame buffer firstly*/
    for (cnt = 0; cnt < MAX_FRAME_BUFFER; cnt++) {
        ret = AMP_SHM_Allocate(AMP_SHM_VIDEO_FB, frame_size, 1024, &frame_queue[cnt].frame_handle);
        if (ret != SUCCESS) {
            CMDLINE_PRINT("Frame buffer allocate fail\n");
            return;
        }
        AMP_SHM_GetVirtualAddress(frame_queue[cnt].frame_handle, 0, &frame_queue[cnt].vir_addr);
        AMP_SHM_GetPhysicalAddress(frame_queue[cnt].frame_handle, 0, &frame_queue[cnt].phy_addr);
        frame_queue[cnt].is_free = 1;
    }

    /*This task reamain to the end of the file*/
    while(!feof(yuv_file)) {
        /*Check if frame queue has free frame and fill it with a valid frame*/
        while (0/*there are free frame available*/) {
                /*read a frame*/
                break;
        }

        /*check valid frame and signal pushframe task*/
    }

    return;
}
#endif

/*This task will get the frame from frame queue and push it to display service*/
HRESULT disptest_push_frame_from_localfile(AMP_DISP objDS, int PlaneID,
                                                DISP_TEST_FRAME frame_info, const char *file_path)
{
    AMP_BD_HANDLE hBD;
    HRESULT Ret = SUCCESS;
    AMP_BGTAG_FRAME_INFO FrameInfo;
    UINT32 uiIndex;
    UINT32 uiBytes;
    AMP_DISP_ZORDER DefaultZorder;
    AMP_DISP_TG_ID TG_ID = 0;
    // static MV_OSAL_HANDLE_TASK_t readframe_task_handle;
    int cnt = 0;
    unsigned int frame_size;

    CMDLINE_PRINT("Push frame from file : %s to TG=%d\n", file_path, TG_ID);
    CMDLINE_PRINT("Frame setting info:\n"
                                "\twidth:       %u\n"
                                "\theight:      %u\n"
                                "\tIs_progressive: %d\n"
                                "\tColor_fmt:   %d\n"
                                "\tBitdepth:      %d\n",
                                frame_info.w, frame_info.h, frame_info.intelace_mode,
                                frame_info.color_fmt, frame_info.bit_depth);

    /*Creat bd queue*/
    create_bd_queue();

    /*Set plane Zorder*/
    AMP_RPC(Ret, AMP_DISP_GetPlaneZOrder, objDS, TG_ID, &DefaultZorder);

    /*for frame read task */
    frame_info.raw_frame_path = file_path;
    FILE *yuv_file = NULL;

    yuv_file = fopen(file_path, "rw");
    //MV_OSAL_Task_Create(&readframe_task_handle, read_frame_task, &frame_info);

    frame_size = get_frame_size(frame_info.color_fmt, frame_info.bit_depth,\
                                                   frame_info.w, frame_info.h);

    /*Allocate frame buffer firstly*/
    for (cnt = 0; cnt < MAX_FRAME_BUFFER; cnt++) {
        Ret = AMP_SHM_Allocate(AMP_SHM_VIDEO_FB, frame_size, 1024, &frame_queue[cnt].frame_handle);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("Frame buffer allocate failed with error code:0x%x.\n", Ret);
            return Ret;
        }
        AMP_SHM_GetVirtualAddress(frame_queue[cnt].frame_handle, 0, &frame_queue[cnt].vir_addr);
        AMP_SHM_GetPhysicalAddress(frame_queue[cnt].frame_handle, 0, &frame_queue[cnt].phy_addr);
        frame_queue[cnt].is_free = 1;
    }

    if (fread(frame_queue[0].vir_addr, frame_size, 1, yuv_file) != 1) {
        CMDLINE_PRINT("fail to load from yuv_file\n");
        return ERR_ERRPARAM;
    }

    AmpMemClear(&FrameInfo, sizeof(AMP_BGTAG_FRAME_INFO));
    FrameInfo.Header.eType   = AMP_BGTAG_FRAME_INFO_META;
    FrameInfo.Header.uLength = sizeof(AMP_BGTAG_FRAME_INFO);
    FrameInfo.uStatus = AMP_FRAME_ST_DECODED;
    FrameInfo.pBufStart =  frame_queue[0].phy_addr;
    set_frameinfo_data(&FrameInfo, frame_info.color_fmt, frame_info.w, frame_info.h,
                        frame_info.intelace_mode, frame_info.bit_depth, frame_info.src_order);

    while (1) {
        /*request free bd from display service*/
        while (1) {
            hBD = request_free_bd(objDS, PlaneID);
            if (hBD) {
                break;
            }
            else {
                CMDLINE_PRINT("can not get free bd, wait a moment!");
                MV_OSAL_Task_Sleep(10);
            }
        }
        /*Update frame and frame info*/
        //disp_readframe_from_file();
        //disp_set_frame_info();

        /*Clear BD*/
        Ret = AMP_BDTAG_Clear(hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not clear bd, error code is %x!",
                 __FUNCTION__,
                 __LINE__,
                 Ret);
            goto disptest_push_frame_from_localfile_exit;
        }

        FrameInfo.uStatus = AMP_FRAME_ST_DECODED;
        Ret = AMP_BDTAG_Append(hBD,
                               (UINT8 *)&FrameInfo,
                               &uiIndex,
                               &uiBytes);
        CMDLINE_PRINT("Push frame to display!\n");

        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not set bd tag, error code is %x!",
                 __FUNCTION__,
                 __LINE__,
                 Ret);
            goto disptest_push_frame_from_localfile_exit;
        }

        AMP_RPC(Ret, AMP_DISP_VID_PushFrame, objDS, PlaneID, hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not push frame, error code is %x!",
                 __FUNCTION__, __LINE__, Ret);
            goto disptest_push_frame_from_localfile_exit;
        }
    }
    prev_hBD = hBD;
disptest_push_frame_from_localfile_exit:
    return Ret;
}

HRESULT disptest_push_frame_previous(AMP_DISP objDS, int PlaneID) {
    HRESULT Ret = SUCCESS;
    //struct AMP_BD_ST null_hBD={0,0};

    //if((null_hBD.uiBDId == prev_hBD->uiBDId) && (null_hBD.uiAllocVA == prev_hBD->uiAllocVA))
    if(!prev_hBD)
    {
        CMDLINE_PRINT("%s:%d, previous hBD is null (Pelase do pushframe[X]) - No action\n",
             __FUNCTION__, __LINE__);
        return Ret;
    }

    AMP_RPC(Ret, AMP_DISP_VID_PushFrame, objDS, PlaneID, prev_hBD);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("%s:%d, can not push frame, error code is %x!",
             __FUNCTION__, __LINE__, Ret);
    }
    return Ret;
}

HRESULT disptest_push_frame_previous1(AMP_DISP objDS, int PlaneID, int iter, int ms_delay1, int ms_delay2) {
    HRESULT Ret = SUCCESS;
    int i;
    //struct AMP_BD_ST null_hBD={0,0};

    //if((null_hBD.uiBDId == prev_hBD->uiBDId) && (null_hBD.uiAllocVA == prev_hBD->uiAllocVA))
    if(!prev_hBD)
    {
        CMDLINE_PRINT("%s:%d, previous hBD is null (Pelase do pushframe[X]) - No action\n",
             __FUNCTION__, __LINE__);
        return Ret;
    }

    CMDLINE_PRINT("%s:%d, iter:%d, ms_delay1:%d, ms_delay2:%d\n",__FUNCTION__, __LINE__, iter, ms_delay1, ms_delay2);

    for(i=0;i<iter;i++) {
        AMP_RPC(Ret, AMP_DISP_VID_PushFrame, objDS, PlaneID, prev_hBD);
        if (Ret != SUCCESS) {
            CMDLINE_PRINT("%s:%d, can not push frame, error code is %x!",
                 __FUNCTION__, __LINE__, Ret);
        }
        MV_OSAL_Task_Sleep(ms_delay1);

        AMP_RPC(Ret, AMP_DISP_Push_BuildInFrame, objDS, PlaneID, 0);
        MV_OSAL_Task_Sleep(ms_delay2);
    }
    return Ret;
}

HRESULT disp_set_format(AMP_DISP_OUT_PARAMS *pdisp_params, unsigned int aspect, unsigned int tgid)
{
    HRESULT result = SUCCESS;
    UINT32 uiResId;

    if(!pdisp_params) {
        CMDLINE_PRINT("%s:%d, invalid argument",__FUNCTION__, __LINE__);
        return ERR_ERRPARAM;
    }
    uiResId = pdisp_params->uiResId & 0x7FFFFFFF;

    if((uiResId < MAX_NUM_RESS) && (pdisp_params->uiColorFmt < MAX_NUM_OUTPUT_COLOR_FMTS) &&
            (pdisp_params->uiBitDepth <MAX_NUM_OUTPUT_BIT_DEPTHS) &&
            (pdisp_params->uiDisplayMode < AMP_DISP_OUT_TYPE_MAX))
        CMDLINE_PRINT("Set Output DispID %s, format: %s, Color Fmt: %s, Bitdepth: %s, TG: %s Mode: %s\n",
                                    m_disp_id[pdisp_params->uiDispId],
                                    m_hdmitx_resolution_table[uiResId],
                                    m_hdmitx_colorfmt_table[pdisp_params->uiColorFmt],
                                    m_hdmitx_bitdepth_table[pdisp_params->uiBitDepth],
                                    m_display_tgid_table[tgid],
                                    m_display_outmode_table[pdisp_params->uiDisplayMode]);

    pdisp_params->iPixelRepeat = 1;
    if (pdisp_params->uiColorFmt >= MAX_NUM_OUTPUT_COLOR_FMTS) {
        CMDLINE_PRINT("Set output fmt with default value: OUTPUT_COLOR_FMT_YCBCR444!\n");
        pdisp_params->uiColorFmt = 1;
    }
    AMP_RPC(result, AMP_DISP_OUT_SetFormat, gDisp, tgid, pdisp_params);
    if (0 != result) {
        CMDLINE_PRINT("AMP_DISP_OUT_SetFormat failed with error code:0x%x.\n", result);
    } else {
        CMDLINE_PRINT("AMP_DISP_OUT_SetFormat success.\n");
    }

    if(pdisp_params->uiResId < RES_720P30) {
        /*
         * for SD resolution, if aspect ration is specified by cmd line,
         * configure it to vpp drv and keep the scaninfo unchanged.
        */
        if (aspect == 1 ||
            aspect == 2) {
            CMDLINE_PRINT("configure aspect ratio to %s\n",
                          (aspect == 1)? "4:3":"16:9");
            AMP_RPC(result, AMP_DISP_OUT_HDMI_SetVidInfo, gDisp, aspect, -1);
        }
    }
    return result;
}
