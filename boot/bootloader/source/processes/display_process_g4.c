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
#ifdef CONFIG_FASTLOGO

#include "system_manager.h"

#include "debug.h"
#include "load_gpt.h"
#include "bootloader.h"
#include "vpp_api.h"
#include "mv_display_api.h"
#include "image_chunk.h"
#include "bootloader.h"
#include "rt_stage.h"
#include "string.h"
#include "misc.h"
#include "flash_ts.h"
#include "ptinfo.h"
#include "image_info.h"
#include "bcm_verify.h"
#include "vpp_cfg.h"

#include "minui.h"
#include "boot_mode.h"
//#define STATIC_LOGO
#ifdef STATIC_LOGO
#include "logom.h"
static unsigned yuv_logo_width = 288;
static unsigned yuv_logo_height = 192;
static unsigned yuv_logo_stride = 576;
//static unsigned yuv_logo_bytepp = 2;
#endif

#ifndef bTST
#define bTST(x, b) (((x) >> (b)) & 1)
#endif

#define  DISPLAY_PRIMARY_RES   "DISP_RES"

#define PRN_DEBUG_LEVEL PRN_DBG
typedef struct{
    unsigned int offset;
    unsigned int width;
    unsigned int height;
    unsigned int stride;
}fastlogo_info_t;

typedef struct {
    unsigned int versionNum;
    unsigned int logoNum;
    fastlogo_info_t info[];
}fastlogo_header_t;

#define avioDhubSemMap_vpp_vppCPCB0_intr avioDhubSemMap_vpp128b_vpp_inr0
#define avioDhubSemMap_vpp_vppOUT4_intr avioDhubSemMap_vpp128b_vpp_inr6

#define IS_RES_LOWER_THAN_HD(WIDTH, HEIGHT)      ((WIDTH < 1280) || (HEIGHT < 720))

DISP_LOGO_CONFIG   primaryLogoCfg = {0};
int logo_rotate = ROTATION_0D;

#ifdef BL_DISPLAY_DUAL_PIP
#define IS_LOGO_VALID()     primaryLogoCfg.valid && secondaryLogoCfg.valid
#else
#define IS_LOGO_VALID()     primaryLogoCfg.valid
#endif

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
extern void * malloc_ion_cacheable(int size);

#include "tz_comm.h"
#include "tee_comm.h"
#include "tee_client_api.h"
#include "preload_ta.h"

#ifndef STATIC_LOGO
extern void * GaloisMalloc(unsigned int size);
#endif

#if defined(CONFIG_MANUAL_TA_LOADING)
#define ADDR32_TO_PTR(x) ((void *)(uintptr_t)(x))
#define TEEC_BUFF_SIZE (1 << 20)

static int tz_mem_init(void)
{
    extern unsigned int __fastlogo_ta_start;
    extern unsigned int __fastlogo_ta_end;
    unsigned char * pta = malloc_ion_cacheable(TEEC_BUFF_SIZE);
    TEEC_Result result = TEEC_SUCCESS;

    dbg_printf(PRN_ERR, "begin\n");
    dbg_printf(PRN_ERR, "memcpy %x, %x\n", __fastlogo_ta_start, __fastlogo_ta_end);
    memcpy(pta, ADDR32_TO_PTR(__fastlogo_ta_start), (__fastlogo_ta_end - __fastlogo_ta_start));

    dbg_printf(PRN_ERR, "register\n");
    result = register_ta(pta, (__fastlogo_ta_end - __fastlogo_ta_start));
    if (result != TEEC_SUCCESS) {
        dbg_printf(PRN_ERR, "register TA failed, ret = 0x%08x\n", result);
    }

	free_ion_cacheable(pta);
    return ((result == TEEC_SUCCESS) ? 0 : -1);
}
#else //CONFIG_MANUAL_TA_LOADING
static int tz_mem_init(void)
{
	TEEC_Result result = TEEC_SUCCESS;

	return ((result == TEEC_SUCCESS) ? 0 : -1);
}
#endif //CONFIG_MANUAL_TA_LOADING
#endif //CONFIG_VPP_USE_FASTLOGO_TA

static int DISP_CheckandUpdateLogoCfg(fastlogo_info_t* info, int width, int height, DISP_LOGO_CONFIG* logocfg) {
    int logo_width = info->width,
        logo_height = info->height;

    if((logo_width > 0) && (logo_width <= width) &&
            (logo_height > 0) && (logo_height <= height) ) {
        dbg_printf(PRN_INFO,"[fastlogo] got fit size logo: %dx%d\n",
                    logo_width, logo_height);

        logocfg->start = info->offset;
        logocfg->size  = info->stride * info->height;

        if (logocfg->size > MAX_LOGO_FRM_SIZE) {
            dbg_printf(PRN_ERR,"[fastlogo] the logo size is abnormal: %d\n", logocfg->size);
            return -1;
        }

        logocfg->width  = info->width;
        logocfg->height = info->height;
        logocfg->stride = info->stride;
        dbg_printf(PRN_DBG,"[fastlogo] logocfg->start :%d ,logocfg->size :%d ,logocfg->width :%d , logocfg->height :%d \
        logocfg->stride :%d ! \n",logocfg->start ,logocfg->size, logocfg->width, logocfg->height,logocfg->stride );
        logocfg->valid  = 1;
    }

    return 0;
}
#ifndef CONFIG_GENX_ENABLE
static int DISP_LoadLogoImage(struct gpt_ent *vt_logo, DISP_LOGO_CONFIG* logocfg) {
    int ret = -1;

    if (!logocfg->valid)
        return ret;

    logocfg->buffer = (unsigned char *)(uintptr_t) GaloisMalloc(logocfg->size);

    if (logocfg->buffer == NULL)
    {
        dbg_printf(PRN_ERR, "GaloisMalloc failed: Memory not available for logo\n");
        return ret;
    }

    // logo_start address is kept 1k alignment
    ret = read_image_from_offset(vt_logo, logocfg->start, logocfg->size,\
                                        logocfg->buffer);
    if(ret < 0) {
        dbg_printf(PRN_ERR,"[fastlogo] Read bootlogo error!\n");
        logocfg->valid = 0;
    }

    return ret;
}
#endif
static void fastlogo_func(void)
{
    struct gpt_ent vt_logo;
    int logo_found = 0;
#ifndef STATIC_LOGO
    int ret = 0, num = 0;
#endif
    int display_primary_HDMI_BitDepth = 2;
    int default_primary_system = PRIMARY_DISPLAY_RES;
    if (!MV_DISP_isValidRes (default_primary_system))
        default_primary_system = RES_720P60;
    int display_primary_width = m_resinfo_table[VPP_PRIMARY_DISPLAYINFO_RESID].active_width;
    int display_primary_height = m_resinfo_table[VPP_PRIMARY_DISPLAYINFO_RESID].active_height;
    int display_primary_system = default_primary_system;
#if defined (BL_DISPLAY_DUAL_PIP)
    int display_secondary_width = m_resinfo_table[RES_DSI_CUSTOM].active_width;
    int display_secondary_height = m_resinfo_table[RES_DSI_CUSTOM].active_height;
    DISP_LOGO_CONFIG   secondaryLogoCfg = {0};
#endif
    int gfx_FB_width, gfx_FB_height;

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
    extern void GaloisInit(void);
    GaloisInit();
#endif //CONFIG_VPP_USE_FASTLOGO_TA
#ifndef STATIC_LOGO
    unsigned char * temp_buff = (unsigned char *)(uintptr_t) GaloisMalloc(1024);
    primaryLogoCfg.buffer = temp_buff;
#ifdef CONFIG_GENX_ENABLE
    fastlogo_header_t * header = NULL;
#else
    fastlogo_header_t * header = (fastlogo_header_t *)temp_buff;
#endif
#else
    primaryLogoCfg.buffer = (unsigned char * const)yuv_logo;
#endif

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
	if(tz_mem_init() != 0)
		return;
#endif //CONFIG_VPP_USE_FASTLOGO_TA

#ifndef STATIC_LOGO
    int logo_index = 0;
#endif

    int display_width;
    int display_height;

    if(primaryLogoCfg.buffer == NULL)
    {
        dbg_printf(PRN_ERR, "GaloisMalloc failed: Memory not available\n");
        return;
    }

    memset(&vt_logo, 0, sizeof(struct gpt_ent));
#ifndef STATIC_LOGO
    num = get_pt_index(PT_FASTLOGO);

    if(num >= 0)
#endif
        logo_found = 1;

    if( (1 == logo_found) &&
            (is_poweron() ||
             is_reset()  ||
             is_coldbootup() ||
             is_standby_2_active()  ||
             is_lowpowerstandby_2_active() ||
             (!is_resume())) ) {

        if ( (display_primary_HDMI_BitDepth <= OUTPUT_BIT_DEPTH_INVALID) ||
                (display_primary_HDMI_BitDepth >= MAX_NUM_OUTPUT_BIT_DEPTHS)) {
            dbg_printf(PRN_ERR,"[fastlogo] invalid value of key \
              display_primary_HDMI_BitDepth=%d", display_primary_HDMI_BitDepth);
            return;
        }

        if(display_primary_width > display_primary_height) {
            /*lanscape display*/
            display_width = display_primary_width;
            display_height = display_primary_height;
        } else {
            /*portrait display*/
            display_width = display_primary_height;
            display_height = display_primary_width;
        }

        if ( (display_width < 720) || (display_width > 3840) ) {
            dbg_printf(PRN_ERR,"[fastlogo] invalid value of key \
                display_primary_width=%d", display_width);
            return;
        }

        if ( (display_height < 480) || (display_height > 2160) )
         {
            dbg_printf(PRN_ERR,"[fastlogo] invalid value of key \
                display_primary_height=%d", display_height);
            return;
        }

        ret = MV_DISP_InitDisplay(display_primary_system, display_primary_HDMI_BitDepth);
        if(ret != MV_VPP_OK) {
            dbg_printf(PRN_NOTICE,"[fastlogo] MV_DISP_InitDisplay error - %d!\n", ret);
            return;
        }

#ifndef STATIC_LOGO
        ret = fetch_partition_info(num, &vt_logo);
        if(ret == PARTITION_NOT_EXIST){
            dbg_printf(PRN_NOTICE,"[fastlogo] fetch partition info error!\n");
            return;
        }
        /* the format of bootlogo partition
           In linear structure, every <logoN_parameter> ( logo_version, logo_num, logonN_offset,
                                                                logoN_width, logoN_height,
           logoN_stride) takes 4 byte,  [logoN_data] takes bytes of (logoN_height * logoN_stride).
           the address of [logoN_data] is a 1k aligned address
           the order of logo should be from big size to small size

        // the first 1k bytes
        < logo_version >< logo_num >< logo1_offset >< logo1_width >< logo1_height >< logo1_stride >
        < logo2_offset >< logo2_width >< logo2_height >< logo2_stride > ... ...
                                                                    < logoN_offset >< logoN_width >
        < logoN_height >< logoN_stride >
        // 1k aligned address
        [logo1_data]
        [logo2_data]
        ... ...
        [logoN_data]
         */
#ifndef CONFIG_GENX_ENABLE
        //read the first 1k bytes (header)
        ret = read_image(&vt_logo, 1024, (unsigned char *)primaryLogoCfg.buffer);
        if(ret < 0) {
            dbg_printf(PRN_ERR,"[fastlogo] Read information error!\n");
            return;
        }
#else
        struct image_info *img_info = (struct image_info *)temp_buff;
        unsigned int img_size = 0;
        unsigned int img_offset = 0;
        unsigned char * logo_buffer = NULL;

        ret = read_image(&vt_logo, 1024, temp_buff);
        if(ret < 0) {
            dbg_printf(PRN_ERR,"[fastlogo] Read image information error!\n");
            return;
        }

        /* Get GenX format fastlogo image size and offset */
        img_offset = img_info->image_offset;
        img_size = img_info->image_size; //actual image size
        logo_buffer  = (unsigned char *)(uintptr_t) malloc_ion_cacheable(img_size+img_offset);
        if(logo_buffer) {
            memset(logo_buffer, 0x0, img_size+img_offset);
            /* Read GenX format fastlogo image */
            ret = read_image(&vt_logo, img_size+img_offset, logo_buffer);
            if(ret < 0) {
                dbg_printf(PRN_ERR,"[fastlogo] Read image information error!\n");
            }

            /* Verify Image, skip prepended image header */
            ret = VerifyImage((void *)(logo_buffer+img_offset), img_size, logo_buffer+img_offset, IMAGE_TYPE_FASTLOGO);
            if(ret < 0) {
                dbg_printf(PRN_ERR,"[ERROR]: fastlogo image verify fail, ret(%d)!\n", ret);
                return;
            }
        }
        else
        {
            dbg_printf(PRN_ERR,"[ERROR]: alloc logo image buffer fail!\n");
            return;
        }
        header = (fastlogo_header_t *)(temp_buff + img_offset + GENX_IMAGE_HEADER_FASTLOGO_SIZE);
#endif

        if((header->logoNum < 1) || (header->logoNum > 5)) {
            dbg_printf(PRN_ERR,"[fastlogo] invalid val ue of frame number=%d\n", header->logoNum);
            return;
        }
        dbg_printf(PRN_DBG,"[fastlogo] value of frame number=%d \n", header->logoNum);
        /* 
        dbg_printf(PRN_DBG,"[fastlogo] logocfg->start :%d ,logocfg->size :%d ,logocfg->width :%d , logocfg->height :%d \
           logocfg->stride :%d ! \n",logocfg->start ,logocfg->size, logocfg->width, logocfg->height,logocfg->stride ); 
        */
        for(logo_index = 0; logo_index < header->logoNum; logo_index++) {
            dbg_printf(PRN_DBG,"logo_index=%d ,primaryLogoCfg.valid =%d \n", logo_index, primaryLogoCfg.valid);
            if (!primaryLogoCfg.valid) {
                if ( 0 > DISP_CheckandUpdateLogoCfg (&header->info[logo_index],\
                                display_primary_width, display_primary_height, &primaryLogoCfg)) {
                    return;
                }
            }
        #if defined (BL_DISPLAY_DUAL_PIP)
                    if (!secondaryLogoCfg.valid) {
                        dbg_printf(PRN_INFO,"[fastlogo] DISP_CheckandUpdateLogoCfg secondaryLogoCfg \n");
                        if ( 0 > DISP_CheckandUpdateLogoCfg (&header->info[logo_index],\
                                    display_secondary_width, display_secondary_height, &secondaryLogoCfg)) {
                                
                            return;
                        }
                    }
        #endif
            if (IS_LOGO_VALID())
               break;
        }
#else
        primaryLogoCfg.width= yuv_logo_width;
        primaryLogoCfg.height = yuv_logo_height;
        primaryLogoCfg.stride = yuv_logo_stride;
        primaryLogoCfg.valid=1;
        //static unsigned yuv_logo_bytepp = 2;
#endif

        if(IS_LOGO_VALID()) {
#ifndef STATIC_LOGO
#ifdef CONFIG_GENX_ENABLE
            primaryLogoCfg.buffer = (unsigned char *)(logo_buffer + img_offset + primaryLogoCfg.start + GENX_IMAGE_HEADER_FASTLOGO_SIZE);
#else
    #ifdef BL_DISPLAY_DUAL_PIP
            dbg_printf(PRN_INFO,"[fastlogo] no DISP_LoadLogoImage primaryLogoCfg \n");
    #else
            if ( DISP_LoadLogoImage(&vt_logo, &primaryLogoCfg) < 0) {
                return;
            }
    #endif
#endif
#endif

#if defined (BL_DISPLAY_DUAL_PIP)
#ifdef CONFIG_GENX_ENABLE
            secondaryLogoCfg.buffer = (unsigned char *)(logo_buffer + img_offset + secondaryLogoCfg.start + GENX_IMAGE_HEADER_FASTLOGO_SIZE);
#else
            dbg_printf(PRN_INFO,"[fastlogo] DISP_LoadLogoImage secondaryLogoCfg \n");
            if ( DISP_LoadLogoImage(&vt_logo, &secondaryLogoCfg) < 0) {
                return;
            }
#endif
#endif

          dbg_printf(PRN_INFO,"[fastlogo] show primary start=%d, size=%d, width=%d, height=%d, stride=%d\n",\
                       primaryLogoCfg.start, primaryLogoCfg.size, primaryLogoCfg.width,\
                       primaryLogoCfg.height, primaryLogoCfg.stride);

#if defined (BL_DISPLAY_DUAL_PIP)
            dbg_printf(PRN_INFO,"[fastlogo] show secondary start:=%d, size=%d, width=%d, height=%d, stride=%d\n",
                        secondaryLogoCfg.start, secondaryLogoCfg.size, secondaryLogoCfg.width,\
                        secondaryLogoCfg.height, secondaryLogoCfg.stride);
#endif

#if !defined(CONFIG_VPP_USE_FASTLOGO_TA)
            MV_DISP_EnableIRQ();
#endif //CONFIG_VPP_USE_FASTLOGO_TA

            /* Warning message is hardcoded for 650x400. So, if the Logo size is less than this
             * GFX calculation for x,y co-ordinates start and end will be not proper. So, o/p
             * will be seen like, GFX misses some of the text message, covers most of the fastlogo etc.
             * So, fixed the FB size to 720P, if the size is less than 720P to avoid the issue.
             */
            if (IS_RES_LOWER_THAN_HD(primaryLogoCfg.width, primaryLogoCfg.height)) {
                gfx_FB_width = 1280;
                gfx_FB_height = 720;
            } else {
                gfx_FB_width = primaryLogoCfg.width;
                gfx_FB_height = primaryLogoCfg.height;
            }

            MV_DISP_ConfigDisplay(display_primary_width, display_primary_height, gfx_FB_width, gfx_FB_height);
            MV_DISP_MuteDisplay(1);
#ifndef LINUXSDK_BOOTLOADER
            gr_init(gfx_FB_width, gfx_FB_height, logo_rotate);
#else
            //Push a blank frame to GFX1
            MV_DISP_Display_Frame(NULL, 0, 0);
#endif

            MV_DISP_LogoStart(PLANE_MAIN, &primaryLogoCfg,\
                         display_primary_width, display_primary_height);

#if defined (BL_DISPLAY_DUAL_PIP)
            MV_DISP_LogoStart(PLANE_PIP, &secondaryLogoCfg,
                         display_secondary_width, display_secondary_height);
#endif

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
            /*Note: VPP driver enable interrupt as part of PUSHFRAME
             *For TZ: enable AVIO interrupt only after enbling interrupt in VPP driver.
             *Otherwise TZK/bootloader executed from single process env on single core will return error for PUSHFRAME callback.*/
            MV_DISP_EnableIRQ();
#endif //CONFIG_VPP_USE_FASTLOGO_TA
#ifdef CONFIG_PANEL_FASTLOGO
            MV_DISP_Backlight_Enable(display_primary_system);
#endif
        } else {
            dbg_printf(PRN_ERR,"[fastlogo] no valid logo\n");
            return;
        }

    }
}

/******************************************************************
* FASTLOGO process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) fastlogo_bootloader_initfs_poststage(void)
{
#if defined(CONFIG_PRELOADTA) && defined(CONFIG_VPP_USE_FASTLOGO_TA)
	if(!is_ta_registered(IMAGE_CHUNK_ID_FASTLOGO)) {
		return BOOTLOADER_NULL_STAGE;
	}
#endif

#ifdef CONFIG_PANEL_FASTLOGO
    if (Mipi_LoadInfoTable())
    {
        dbg_printf(PRN_ERR, "Fail to Load MIPI info table\n");
        return BOOTLOADER_NULL_STAGE;
    }
#endif

	if (check_fastlogo_start())
	fastlogo_func();
    return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) fastlogo_bootloader_loadkernel_poststage(void)
{
#if defined(CONFIG_PRELOADTA) && defined(CONFIG_VPP_USE_FASTLOGO_TA)
	if(!is_ta_registered(IMAGE_CHUNK_ID_FASTLOGO)) {
		return BOOTLOADER_NULL_STAGE;
	}
#endif

	if (check_fastlogo_start())
	if(primaryLogoCfg.valid)
		MV_DISP_DestroyDisplay();
    return BOOTLOADER_NULL_STAGE;
}

DECLARE_RT_STAGE(BOOTLOADER_INITFTS_POSTSTAGE, fastlogoinitfs_poststage, 0, fastlogo_bootloader_initfs_poststage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_POSTSTAGE, fastlogoloadkernel_poststage, 0, fastlogo_bootloader_loadkernel_poststage);

#endif
