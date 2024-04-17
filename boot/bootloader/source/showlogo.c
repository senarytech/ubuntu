/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
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
#include "io.h"
#include "thinvpp_api.h"
#include "thinvpp_isr.h"
#include "api_dhub.h"
#include "api_avio_dhub.h"
#include "memmap.h"
#include "irq.h"
#include "pic.h"

#include "debug.h"

#ifndef bTST
#define bTST(x, b) (((x) >> (b)) & 1)
#endif

volatile unsigned logo_isr_count;
extern void flush_dcache_range(unsigned int start,unsigned int end);
extern int MV_THINVPP_SetMainDisplayFrame(VBUF_INFO *pinfo);
extern int MV_THINVPP_OpenDispWindow(int planeID, const VPP_WIN *win, const VPP_WIN_ATTR *attr);
extern int MV_THINVPP_Stop(void);
void showlogo_isr(void);

static void showlogo_init_irq(void)
{
    if(register_isr(showlogo_isr, IRQ_dHubIntrAvio0))
        dbg_printf(PRN_ERR, "mmc4 isr can't be registered\n");

    set_irq_enable(IRQ_dHubIntrAvio0);
}

static VBUF_INFO vbuf;

int showlogo_start(void *buf,int width,int height,int stride, int win_width, int win_height)
{
    VPP_WIN_ATTR showlogo_attr;
    VPP_WIN showlogo_win;
    void *start;
    unsigned size;

    dbg_printf(PRN_RES,"[showlogo] start: buf=0x0%8x, width=%d, heigth=%d, stride=%d\n", buf, width, height, stride);

    logo_isr_count = 0;
    showlogo_init_irq();

    vbuf.m_pbuf_start    = buf; // base address of the frame buffer;
    vbuf.m_disp_offset   = 0;
    vbuf.m_active_left   = (win_width- width)/2; // at center
    vbuf.m_active_top    = (win_height- height)/2; // at center

    // adjust the following three fields to reflect the actual logo dimensions
    vbuf.m_buf_stride    = stride;
    vbuf.m_active_width  = width;
    vbuf.m_active_height = height;

    vbuf.alpha   = 255;
    // Format of bgcolor: YUYV
    // This matches the background color of our boot logo.
    // vbuf.bgcolor = 0x2982297E;
    vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);

    start = vbuf.m_pbuf_start;
    size = vbuf.m_buf_stride * vbuf.m_active_height;

    //	UtilMemCpy(start, yuv_logo, size);
    flush_dcache_range((unsigned int)start, (unsigned int)(((int)(start))+(size)));

    MV_THINVPP_SetMainDisplayFrame(&vbuf);

    showlogo_win.x = 0;
    showlogo_win.y = 0;
    showlogo_win.width  = win_width;
    showlogo_win.height = win_height;

    showlogo_attr.bgcolor = vbuf.bgcolor;
    showlogo_attr.alpha   = vbuf.alpha;
#if LOGO_ENABLE_MAIN
    MV_THINVPP_OpenDispWindow(PLANE_MAIN, &showlogo_win, &showlogo_attr);
#else
    MV_THINVPP_OpenDispWindow(PLANE_PIP, &showlogo_win, &showlogo_attr);
#endif

    MV_THINVPP_OpenDispWindow(PLANE_AUX, &showlogo_win, NULL);

    return 0;
}

int showlogo_stop()
{
    dbg_printf(PRN_DBG,"[showlogo] stop: isr_count=%u\n", logo_isr_count) ;
#if BOOTLOADER_ANIMLOGO
    {
        // force to show logo
        int count = ((logo_isr_count/100)+1)*100;
        while (logo_isr_count < 500)
        {
            if (count < logo_isr_count)
            {
                count += 100;
                dbg_printf(PRN_DBG,"%s, isr_count=%u\n", __FUNCTION__, logo_isr_count) ;
            }
        }
    }
#endif
    //MV_THINVPP_CloseDispWindow();
    MV_THINVPP_Stop();
    MV_THINVPP_Destroy();
    dbg_printf(PRN_RES,"[showlogo] stopped\n");

    return 0;
}

void showlogo_isr(void)
{
    int instat;
    HDL_semaphore *pSemHandle;

    ++logo_isr_count;

    /* VPP interrupt handling  */
    pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    instat = semaphore_chk_full(pSemHandle, -1);

    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr))
    {
#if BOOTLOADER_ANIMLOGO
        static int anim, anim_count;

        if (logo_isr_count == 0)
            anim = anim_count = 0;

        switch (anim) {
        default:
                anim = 0;
        case 0:
            // fade out animation
            if (vbuf.alpha > 1)
                vbuf.alpha -= 2;
            else
            {
                vbuf.alpha = 0;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }

            // bgcolor animation
            vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);
            break;
        case 1:
            // fade in animation
            if (vbuf.alpha<255)
                vbuf.alpha += 2;
            else
            {
                vbuf.alpha = 255;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }

            // bgcolor animation
            vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);
            break;
        case 2:
            // active window locatoin animation
            // move to upper left
            if (vbuf.m_active_left > 48)
            {
                vbuf.m_active_left -= 2;
                vbuf.m_active_top = vbuf.m_active_left*2/3;
            }
            else
            {
                vbuf.m_active_left = 48;
                vbuf.m_active_top = 32;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }
            break;

        case 3:
            // active window locatoin animation
            // move to center
            if (vbuf.m_active_left < 216)
            {
                vbuf.m_active_left += 2;
                vbuf.m_active_top = vbuf.m_active_left*2/3;
            }
            else
            {
                vbuf.m_active_left = 216;
                vbuf.m_active_top = 144;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }
            break;
        }
#endif

        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);
        //dbg_printf(PRN_RES,"%s.%d call cpcb isr service\n",__FUNCTION__,__LINE__);
        THINVPP_CPCB_ISR_service(thinvpp_obj, CPCB_1);
    }

    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB2_intr))
    {
#if BOOTLOADER_ANIMLOGO
        static int anim, anim_count;

        if (logo_isr_count == 0)
            anim = anim_count = 0;

        switch (anim) {
        default:
                anim = 0;
        case 0:
            // fade out animation
            if (vbuf.alpha > 1)
                vbuf.alpha -= 2;
            else
            {
                vbuf.alpha = 0;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }

            // bgcolor animation
            vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);
            break;
        case 1:
            // fade in animation
            if (vbuf.alpha<255)
                vbuf.alpha += 2;
            else
            {
                vbuf.alpha = 255;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }

            // bgcolor animation
            vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);
            break;
        case 2:
            // active window locatoin animation
            // move to upper left
            if (vbuf.m_active_left > 48)
            {
                vbuf.m_active_left -= 2;
                vbuf.m_active_top = vbuf.m_active_left*2/3;
            }
            else
            {
                vbuf.m_active_left = 48;
                vbuf.m_active_top = 32;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }
            break;

        case 3:
            // active window locatoin animation
            // move to center
            if (vbuf.m_active_left < 216)
            {
                vbuf.m_active_left += 2;
                vbuf.m_active_top = vbuf.m_active_left*2/3;
            }
            else
            {
                vbuf.m_active_left = 216;
                vbuf.m_active_top = 144;
                ++anim;
                dbg_printf(PRN_RES,"%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }
            break;
        }
#endif

        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB2_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB2_intr);
        //dbg_printf(PRN_RES,"%s.%d call cpcb isr service\n",__FUNCTION__,__LINE__);
        THINVPP_CPCB_ISR_service(thinvpp_obj, CPCB_3);
    }
}
