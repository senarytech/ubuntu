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
#include <dirent.h>
#include <math.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"
#include "isl/amp_buf_desc.h"
#include "isl/amp_event_queue.h"
#include "isl/amp_logger.h"
#include "stat.h"
#include <time.h>
#include "ringbuf.h"

#define DISABLE_HDMIRX_AUDIO
#ifdef CONFIG_AMP_COMPONENT_AVIN_ENABLE
#include "ndi_avin.h"

#define TEST_AVIN_BD_LIST_MAX_SIZE      1
#define TEST_AVIN_AREN_OUTPUT_PORTS     3

#define LOG_COLOR_RED       "\033[31m"
#define LOG_COLOR_GREEN     "\033[32m"
#define LOG_COLOR_YELLOW    "\033[33m"
#define LOG_COLOR_AUTO      "\033[m"

#define AUD_DATA_FILE       "/data/audio.dat"
#define VBI_DATA_FILE       "/data/vbi.dat"
#define SUSPEND_SHELL_FILE  "/data/test_suspend.sh"

#define CLOSE_COMPS_TO_SUSPEND

#undef STRINGIFY
#define STRINGIFY(x)    case x: return #x

#define READMAX_REGS        1024*10
#define AVIF_BASE_ADDRESS   0xF7980000
#define AVIF_HDMIRX_BASE    0xF79E0000
#define AVIF_HDMIRX_NUMS    0x2000
#define AVIN_NAME_MAX_LEN   (128)

#define EDID_BLK_SIZE 128
#define EDID_DATA_SIZE 256

#define EDID_CEA_EXTN_DTD_OFS_ADDR  0x02
#define EDID_CEA_EXTN_DATA_BLK_ADDR 0x04
#define EDID_CEA_AUDIO_BLK_TAG 0x01
#define EDID_CEA_SAD_LENGTH 3

// This size should be the max size of I frame.
// And please set this value to be 1024 divisible.
#define VENC_ES_BUFFER_ALLOC_SIZE (2 * 1024 * 1024)

#define MAX_OUTPUT_STREAM_NUM 16
#ifdef ANDROID
#define DEFAULT_DUMP_PATH  "/data/"
#else // ANDROID
#define DEFAULT_DUMP_PATH  "/mnt/"
#endif
#define VENC_TYPE_UNSUPPORTED (UINT32)(-1)

#define MAX_STREAM_SIZE       8
#define ES_BUFFER_SIZE       (4 * 1024 * 1024)

#define AVIN_OUTPUT_COMP_TUNNEL (0)
#define AVIN_OUTPUT_COMP_NS_TUNNEL_2K (1)
#define AVIN_OUTPUT_COMP_TUNNEL_2K (2)

#define ONE_FRAME_SIZE 3840*2160*2
#define MAX_FRAME  8
struct ringbuf_t gRingbuf ;
struct ringbuf_t gWMbuf;
static int frame_size = 1000;
#define NON_RB_SIZE  1000

//#define AVIN_DEMO_SIMPLE
#define RX_FILE_PATH  "/mnt/yuv_dump.raw"
#define DUMP_RX_PATH "/mnt/dump_tx.raw"

typedef enum tagEDID_AUDIO_FORMAT
{
    EDID_AUD_FMT_RESERVED = 0,
    EDID_AUD_FMT_LPCM,
    EDID_AUD_FMT_AC3,
    EDID_AUD_FMT_MPEG1,
    EDID_AUD_FMT_MP3,
    EDID_AUD_FMT_MPEG2,
    EDID_AUD_FMT_AAC,
    EDID_AUD_FMT_DTS,
    EDID_AUD_FMT_ATRAC,
    EDID_AUD_FMT_ONE_BIT_AUDIO,
    EDID_AUD_FMT_DOLBY_DIGITAL,
    EDID_AUD_FMT_DTS_HD,
    EDID_AUD_FMT_MAT,
    EDID_AUD_FMT_DST,
    EDID_AUD_FMT_WMA_PRO,
    EDID_AUD_FMT_MAX
}EDID_AUDIO_FORMAT;

//currently stability is not good, temply use printf instead of logcat for further deubg
//#define AVIN_TEST_LOGD(...) AMP_Log(MODULE_AVIN, AMP_LOG_LOW, __VA_ARGS__)
#define AVIN_TEST_LOGD(...)  \
    do {                       \
        printf("<app>%s:%d ",__FUNCTION__, __LINE__); \
        printf(__VA_ARGS__); \
    } while (0)

#define TEST_AVIN_PRINT(...)   \
    do {                       \
        {printf(__VA_ARGS__);} \
    } while (0)

#define VERIFY_RESULT(x) { \
    if (x != SUCCESS) { \
        printf("%s:%d Failed (ret = %x)\n", __FUNCTION__, __LINE__, x); \
        assert(0); \
    } \
}

typedef enum {
    TEST_AVIN_MODE_DEBUG_DUMP = 0,
    TEST_AVIN_MODE_TUNNEL,
    TEST_AVIN_MODE_NON_TUNNEL,
    TEST_AVIN_MODE_MAX
} TEST_AVIN_MODE;

typedef enum {
    TEST_AVIN_REG_CYPRESS = 0,
    TEST_AVIN_REG_AVIF_ANALOG,
    TEST_AVIN_REG_AVIF_HDMIRX1,
    TEST_AVIN_REG_AVIF_HDMIRX2,
    TEST_AVIN_REG_AVIF_HDMIRX3,
    TEST_AVIN_REG_AVIF_HDMIRX4,
    TEST_AVIN_REG_MAX
} TEST_AVIN_REG_MODE;

typedef struct TEST_AVIN_HLD {
    AMP_FACTORY factory;
    AMP_COMPONENT avin;
    HANDLE hListener;

    UINT32 uiVideoPort;
    UINT32 uiAudioPort;
    UINT32 uiVbiPort;
    UINT32 uiVideoPort2;
    UINT32 uiAudioPort2;

    AMP_COMPONENT vout;
    AMP_DISP voutDisp;
    AMP_DISP_ZORDER voutZorder;

    // VENC Component
    AMP_COMPONENT    venc;
    AMP_COMPONENT_MODE mode;
    AMP_BDCHAIN     *pFrameBDQ;
    AMP_BDCHAIN     *pStreamBDQ;
    UINT32          frameSize;
    BOOL            isFinished;
    BOOL            bEOS;
    BOOL            bSendEOS;
    UINT32          input_format;//yuv422/yuv420sp
    UINT32          enc_codec; //h264/vp8
    UINT32          width;
    UINT32          height;
    UINT32          bitrate;
    UINT32          fr_num;
    UINT32          fr_den;
    UINT32          gopSize;
    char            output_vid_file_path[128];
    char            output_aud_file_path[128];
    char            perf_log_path[128];
    BOOL            dump_en;
    FILE*           fp_out_es;
    UINT32          enc_num;
    UINT32          feedFramesCnt;
    Stat_T*         pStat;
    BOOL            bHdrSend;
    UINT32          colorAspect;
    MV_OSAL_HANDLE_TASK_t insTask;
    MV_OSAL_HANDLE_TASK_t rbTask;
    MV_OSAL_HANDLE_TASK_t fileTask;
    BOOL            playBackStopped;

    // VDEC Component
    AMP_COMPONENT vdec;
    INT32 VdecPort2Vout;
    INT32 VoutPort2Vdec;

    // DEINT Component
    AMP_COMPONENT deint;

    // DUM Component
    AMP_COMPONENT audDum;

    AMP_COMPONENT clk;
    AMP_COMPONENT adec;
    AMP_COMPONENT aren;
    HANDLE sndHandle;
    HANDLE tunnel_handle[TEST_AVIN_AREN_OUTPUT_PORTS];
    HANDLE tunnelIdx[3];

    BOOL enableClk;
    BOOL clkenabled;
    BOOL enableMS11;
#if ( BERLIN_CHIP_VERSION == BERLIN_BG4_CDP )
    BOOL enableDDUDC;
#endif
#if (BERLIN_CHIP_VERSION == BERLIN_VS680) || \
    (BERLIN_CHIP_VERSION == BERLIN_VS680_A0)
#ifdef CONFIG_AMP_IP_AUDIO_MS12_SUPPORTED
    BOOL enableMS12;
#endif
#endif

    AMP_AVIN_SOURCE_TYPE curSrc;
    AMP_AVIN_SOURCE_STATE srcStatus;
    AMP_AVIN_INPUT_VIDEO_PARAMS videoParams;
    AMP_AVIN_INPUT_AUDIO_PARAMS audioParams;
    AMP_AVIN_SIF_STATUS sifStatus;
    BOOL bDetSifStd;

    TEST_AVIN_MODE testMode;
    AMP_AVIN_COMBFILTER_MODE combMode;
    UINT32 keyEvent;
    MV_OSAL_HANDLE_TASK_t event_task;
    BOOL event_task_exit;

    BOOL enableDumpVbiData, enableDumpAudioData;
    char pcommand[1024][128];
    BOOL enableLog;
    char chip_name[50];
    BOOL enable_passthr;
    BOOL enable_mixpassthr;
    BOOL enable_mixpassthr_ddp; //force DDP output for MAT input audio
    BOOL enable_mixpassthr_mat; //force MAT output for DDP input audio

    volatile BOOL          mainTaskExit;
    MV_OSAL_HANDLE_MUTEX_t mainTaskExitMutex;
    MV_OSAL_HANDLE_MUTEX_t audioPipelineMutex;

    //port define
    INT32             AdecPort2ArenPcm;
    INT32             AdecPort2ArenSpdif;
    INT32             AdecPort2ArenHdmi;

    INT32             ArenPort2AdecPcm;
    INT32             ArenPort2AdecSpdif;
    INT32             ArenPort2AdecHdmi;

    INT32             ArenPort2SndPcm;
    INT32             ArenPort2SndSpdif;
    INT32             ArenPort2SndHdmi;

    //for 3D 2D setting
    AMP_DISP_OUT_RES resId_origin;
    AMP_AVIN_EDID_INFO EdidInfo;
    UINT32 outputPlane;
    UINT32 outputComp;
    BOOL isPassthroughOn;

    //for frame capture
    UINT32 reqFR;
    UINT32 crntTm;
    UINT32 frCnt;
    UINT32 flCnt;
    UINT32 stTm;
    UINT32 delIntvl;
} TEST_AVIN_HLD;

TEST_AVIN_HLD *pgHld = NULL;

typedef enum {
   TEST_AVIN_OUPUT_COMP_VOUT = 0,
   TEST_AVIN_OUPUT_COMP_VOUT_PASSTHROUGH,
   TEST_AVIN_OUPUT_COMP_VENC_FILE,
   TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT,
   TEST_AVIN_OUPUT_COMP_CAPTURE,
   TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT,
   TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE,
   TEST_AVIN_OUPUT_COMP_VOUT_NT,
   TEST_AVIN_OUPUT_COMP_MAX
} TEST_AVIN_OUPUT_COMP;

static UINT32 str2type(const char* str)
{
    if (!strcmp(str, "yuv420sp"))               return AMP_VENC_YUV420_SP;
    else if (!strcmp(str, "yuv420p"))           return AMP_VENC_YUV420_PLANE;
    else if (!strcmp(str, "yuv422"))            return AMP_VENC_YUV422_UYVY;
    else if (!strcmp(str, "rgb"))               return AMP_VENC_RGBA;
    else if (!strcmp(str, "h264"))               return MEDIA_VES_AVC;
    else if (!strcmp(str, "vp8"))               return MEDIA_VES_VP8;
    else return VENC_TYPE_UNSUPPORTED;
}

static void display_usage(void)
{
    printf("\nUsage: ampclient_alpha 7\n"
            "  -f format, --output_format=format,       Specify output file format\n"
            "                                               supported format:\n"
            "                                                   h264[default]\n"
            "                                                   vp8\n"
            "  -v path, --output_file_videoPath=path,   Specify output file path\n"
            "                                               for video data\n"
            "  -a path, --output_file_audioPath=path,   Specify output file path\n"
            "                                               for audio data\n"
            "  -p plane, --output_plane=plane,          Specify the output plane\n"
            "  -m mode, --avin_mode=mode,               Specify the avin pipeline:\n"
            "                                             0:  AVIN->VOUT[default]\n"
            "                                             1:  AVIN->VOUT[PassThrough]\n"
            "                                             2:  AVIN->VENC->DUMP\n"
            "                                             3:  AVIN->VOUT:\n"
            "                                                 AVIN->VENC->DUMP\n"
            "                                             4:  AVIN[CAPTURE]\n"
            "                                             5:  AVIN->VOUT[CAPTURE]\n"
            "                                             6:  AVIN->VENC->VDEC->VOUT\n"
            "  -r frame_rate, --frame_rate=frame_rate,  Specify frame rate for capture\n");
}

static INT32 process_options(int argc, char **argv)
{
    int c, index;
    char *short_options = "f:v:a:p:m:r:";
    struct option long_options[] = {
        {"output_format",             required_argument, NULL, 'f'},
        {"output_file_vidPath",       required_argument, NULL, 'v'},
        {"output_file_audPath",       required_argument, NULL, 'a'},
        {"output_plane",              required_argument, NULL, 'p'},
        {"avin_mode",                 required_argument, NULL, 'm'},
        {"frame_rate",                required_argument, NULL, 'r'},
        {NULL,                        no_argument,       NULL,  0}
    };

    while (1) {
        c = getopt_long(argc, argv, short_options, long_options, NULL);

        if (c == -1) {
            break;
        }
        switch (c) {
            case 'f':
                pgHld->enc_codec = str2type(optarg);
                break;
            case 'v':
                if (access(dirname(optarg), F_OK) != 0) {
                    printf("output file %s can't access\n", optarg);
                    return -1;
                }
                AmpMemcpy(pgHld->output_vid_file_path, optarg, strlen(optarg));
                break;
            case 'a':
                if (access(dirname(optarg), F_OK) != 0) {
                    printf("output file %s can't access\n", optarg);
                    return -1;
                }
                AmpMemcpy(pgHld->output_aud_file_path, optarg, strlen(optarg));
                break;
            case 'p':
                if (strcmp(optarg, "pip") == 0) { // space after pip added from calling func
                    pgHld->outputPlane = AMP_DISP_PLANE_PIP;
                } else {
                    pgHld->outputPlane = AMP_DISP_PLANE_MAIN;
                }
                break;
            case 'm':
                switch (strtol(optarg, NULL, 0)) {
                    case TEST_AVIN_OUPUT_COMP_VOUT:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VOUT;
                    break;
                    case TEST_AVIN_OUPUT_COMP_VOUT_PASSTHROUGH:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VOUT;
                        pgHld->isPassthroughOn = TRUE;
                    break;
                    case TEST_AVIN_OUPUT_COMP_VENC_FILE:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VENC_FILE;
                    break;
                    case TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT;
                    break;
                    case TEST_AVIN_OUPUT_COMP_CAPTURE:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_CAPTURE;
                    break;
                    case TEST_AVIN_OUPUT_COMP_VOUT_NT:
                        printf("TEST_AVIN_OUPUT_COMP_VOUT_NT\n");
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VOUT_NT;
                    break;
                    case TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT;
                    break;
                    case TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE:
                        printf ("Currently not supported...\n");
                        return -1;
                        //pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE;
                    break;
                    default:
                        pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VOUT;
                    break;
                }
                break;
            case 'r':
                pgHld->reqFR = strtol(optarg, NULL, 0);
                break;
           default:
                display_usage();
                return -1;
        }
    }

    if (strlen(pgHld->output_vid_file_path) == 0) {
        sprintf(pgHld->output_vid_file_path, "%svideo_dump_%02d.es",
                DEFAULT_DUMP_PATH, index);
    }
    if (strlen(pgHld->output_aud_file_path) == 0) {
        sprintf(pgHld->output_aud_file_path, "%saudio_dump_%02d.es",
                DEFAULT_DUMP_PATH, index);
    }
    if (pgHld->width == 0 || pgHld->height == 0) {
        AVIN_TEST_LOGD ("instance[%p] with invalid width/height, "
                "width:%d, height:%d\n", pgHld, pgHld->width, pgHld->height);
        goto error;
    }

    if (pgHld->bitrate == 0) {
        pgHld->bitrate = pgHld->width * pgHld->height * 12;
    }

    if (pgHld->reqFR == 0) {
        pgHld->reqFR = 5;
    }
    return 0;

error:
    return -1;
}

static AMP_EVENT_CODE eventList[] = {
    AMP_EVENT_API_AVIN_SRC_CONNECTION_CHANGED,
    AMP_EVENT_API_AVIN_LOCK_STATUS_CHANGED,
    AMP_EVENT_API_AVIN_VIDEO_CHANGED,
    AMP_EVENT_API_AVIN_AUDIO_CHANGED,
    AMP_EVENT_API_AVIN_AVMUTE_CHANGED,
    AMP_EVENT_API_AVIN_HDMI_GMD_PKT,
    AMP_EVENT_API_AVIN_HDMI_GMD_PKT_STOP,
    AMP_EVENT_API_AVIN_VIP_SD_INT_ON,
    AMP_EVENT_API_AVIN_JOB_COMPLETED,
    AMP_EVENT_API_AVIN_SIF_STATUS_CHANGED,
    AMP_EVENT_API_DISP_CALLBACK_HDMI,
#if (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
    AMP_EVENT_API_AVIN_HDMI_HDR_INFO
#else
    AMP_EVENT_API_AVIN_HDMI_HDR_INFO,
    AMP_EVENT_API_AVIN_HDMI_DV_CHANGED
#endif
};

typedef struct {
    const UINT8   pCommand[128];
    HRESULT (*AVIN_Command)(TEST_AVIN_HLD *h, UINT8 *pArgv);
    const UINT8   pDesc[256];
    BOOL bExitDone;
}AVIN_COMMAND;

typedef struct{
    const UINT8  pName[20];
    UINT8 iNumber;
}Test_3D_MODE_Mapping;

static Test_3D_MODE_Mapping p3DMode[]={
    {"Auto",            0},
    {"SideBySide",      1},
    {"TopAndBottom",    2},
    {"Sensio",          3},
    {"FramePacking",    4}
};

typedef struct{
    const UINT8  pName[20];
    UINT8 iNumber;
}Test_3D_Vout_Format_Mapping;

static Test_3D_Vout_Format_Mapping p3DVoutFmt[]={
    {"1920x1080@30",    0},
    {"1920x1080@24",    1},
    {"1280x720@60",     2},
    {"1280x720@50",     3},
    {"4096x2160@120",   4}
};



static struct AMP_BD_ST *globalBDList[TEST_AVIN_BD_LIST_MAX_SIZE + 1] = {0};
static UINT32 globalSize = 0;
static UINT32 globalCnt = 0;
static MV_OSAL_HANDLE_MUTEX_t globalVideoMutex, globalAudioMutex;

static HRESULT test_avin_cmd_help(TEST_AVIN_HLD *h, UINT8 *pArgv);

static const char* test_avin_convert2HDMIPortID(AMP_IN TEST_AVIN_HLD *h,
    AMP_AVIN_SOURCE_TYPE source);
static HRESULT avin_adec_event_handler(AMP_EVENT *pEvent, VOID *pUserData);
static HRESULT test_adec_register_event(HANDLE hListener, AMP_COMPONENT hAdec,
    BOOL bRegister, AMP_IN TEST_AVIN_HLD *pHandle);

static HRESULT test_avin_getavinfo(TEST_AVIN_HLD *h,UINT8 *pArgv);

static const char* test_avin_eventString(AMP_EVENT_CODE event) {
  switch (event) {
    STRINGIFY(AMP_EVENT_API_AVIN_SRC_CONNECTION_CHANGED);
    STRINGIFY(AMP_EVENT_API_AVIN_LOCK_STATUS_CHANGED);
    STRINGIFY(AMP_EVENT_API_AVIN_VIDEO_CHANGED);
    STRINGIFY(AMP_EVENT_API_AVIN_AUDIO_CHANGED);
    STRINGIFY(AMP_EVENT_API_AVIN_AVMUTE_CHANGED);
    STRINGIFY(AMP_EVENT_API_AVIN_HDMI_GMD_PKT);
    STRINGIFY(AMP_EVENT_API_AVIN_HDMI_GMD_PKT_STOP);
    STRINGIFY(AMP_EVENT_API_AVIN_VIP_SD_INT_ON);
    STRINGIFY(AMP_EVENT_API_AVIN_JOB_COMPLETED);
    STRINGIFY(AMP_EVENT_API_AVIN_SIF_STATUS_CHANGED);
    STRINGIFY(AMP_EVENT_ADEC_CALLBACK_STRMINFO);
    STRINGIFY(AMP_EVENT_API_DISP_CALLBACK_HDMI);
    STRINGIFY(AMP_EVENT_API_AVIN_HDMI_HDR_INFO);
#if (BERLIN_CHIP_VERSION == BERLIN_VS680_A0)
    STRINGIFY(AMP_EVENT_API_AVIN_HDMI_DV_CHANGED);
#endif
    default: return "event unknown";
  }
}

static const char* test_avin_audioFormatString(
    AMP_ADEC_FORMAT audioFormat)
{
  switch (audioFormat) {
      STRINGIFY(AMP_DD_AC3);
      STRINGIFY(AMP_DD_PLUS);
      STRINGIFY(AMP_DD_TRUE_HD);
      STRINGIFY(AMP_DTS_HD);
      STRINGIFY(AMP_MPG_AUDIO);
      STRINGIFY(AMP_MP3);
      STRINGIFY(AMP_HE_AAC);
      STRINGIFY(AMP_WMA);
      STRINGIFY(AMP_RAW_PCM);
      STRINGIFY(AMP_LPCM_SD);
      STRINGIFY(AMP_LPCM_HD);
      STRINGIFY(AMP_LPCM_BD);
      STRINGIFY(AMP_LPCM_SESF);
      STRINGIFY(AMP_PL2);
      STRINGIFY(AMP_DTS);
      STRINGIFY(AMP_DTS_MA);
      STRINGIFY(AMP_DTS_HRES);
      STRINGIFY(AMP_DTS_LBR);
      STRINGIFY(AMP_DV_SD);
      STRINGIFY(AMP_VORBIS);
      STRINGIFY(AMP_FLAC);
      STRINGIFY(AMP_REAL_AUD8);
      STRINGIFY(AMP_REAL_AAC);
      STRINGIFY(AMP_IMA_ADPCM);
      STRINGIFY(AMP_G711A);
      STRINGIFY(AMP_G711U);
      STRINGIFY(AMP_AMRNB);
      STRINGIFY(AMP_AMRWB);
#ifdef CONFIG_AMP_IP_AUDIO_MS12_SUPPORTED
      STRINGIFY(AMP_MS12_MAT);
#endif
    default: return "audio format unknown";
  }
}

static const char* test_avin_audioSifStandardString(
    AMP_AVIN_SIF_STANDARD sifStandard)
{
  switch (sifStandard) {
      STRINGIFY(AMP_AVIN_SIF_STD_UNKNOWN);
      STRINGIFY(AMP_AVIN_SIF_STD_MONO45);
      STRINGIFY(AMP_AVIN_SIF_STD_MONO55);
      STRINGIFY(AMP_AVIN_SIF_STD_MONO60);
      STRINGIFY(AMP_AVIN_SIF_STD_MONO65);
      STRINGIFY(AMP_AVIN_SIF_STD_MONO65_AML);
      STRINGIFY(AMP_AVIN_SIF_STD_AUTO);
      STRINGIFY(AMP_AVIN_SIF_STD_A2DK);
      STRINGIFY(AMP_AVIN_SIF_STD_BTSC);
      STRINGIFY(AMP_AVIN_SIF_STD_BTSCSAP);
      STRINGIFY(AMP_AVIN_SIF_STD_KOREA);
      STRINGIFY(AMP_AVIN_SIF_STD_BGNICAM);
      STRINGIFY(AMP_AVIN_SIF_STD_FMMONO_NICAM_CHINA);
      STRINGIFY(AMP_AVIN_SIF_STD_PALI);
      default: return "Unknown";
  }
}

static const char* test_avin_audioSifModeString(
    AMP_AVIN_AUD_SIF_MODE sifMode)
{
  switch (sifMode) {
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_MONO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_STEREO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_SAP);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_SAP_MONO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_MONO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_STEREO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO2);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1_MONO2);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_DATA704);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_NICAM_MONO_DATA352);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_A2_MONO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_A2_STEREO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO1);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_FM_MONO);
      STRINGIFY(AMP_AVIN_AUD_SIF_MODE_AM_MONO);
      default: return "Unknown";
  }
}

static const char* test_avin_audioSifSupportedModesString(
        UINT32 uiSupportedModes)
{
    int len = 0;
    static char modes[1024];
    AMP_AVIN_AUD_SIF_MODE sifMode;

    AmpMemClear((VOID*) &modes, sizeof(modes));
    if (uiSupportedModes == AMP_AVIN_AUD_SIF_MODE_UNKNOWN) {
        strcpy(modes, test_avin_audioSifModeString(uiSupportedModes));
    } else {
        for (sifMode = AMP_AVIN_AUD_SIF_MODE_FM_MONO;
                sifMode <= AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2;
                sifMode <<= 1) {
            if (uiSupportedModes & sifMode) {
                strcat(modes, test_avin_audioSifModeString(sifMode));
                strcat(modes, "|");
            }
        }
        len = strlen(modes);
        if (len > 0) {
            modes[len - 1] = '\0';
        }
    }

    return modes;
}

static const char* test_avin_inputStateString(
        AMP_IN AMP_AVIN_SOURCE_STATE state)
{
    switch (state) {
        STRINGIFY(AMP_AVIN_SOURCE_STATE_DISCONNECTED);
        STRINGIFY(AMP_AVIN_SOURCE_STATE_NO_SIGNAL);
        STRINGIFY(AMP_AVIN_SOURCE_STATE_NOT_SUPPORTED);
        STRINGIFY(AMP_AVIN_SOURCE_STATE_STABLE);
    default: return "Input State unknown";
    }
}

static const char* test_avin_bitDepthString(
        AMP_IN AMP_AVIN_COLOR_DEPTH_TYPE depth)
{
    switch (depth) {
        STRINGIFY(AMP_AVIN_COLOR_DEPTH_TYPE_8BIT);
        STRINGIFY(AMP_AVIN_COLOR_DEPTH_TYPE_10BIT);
        STRINGIFY(AMP_AVIN_COLOR_DEPTH_TYPE_12BIT);
        STRINGIFY(AMP_AVIN_COLOR_DEPTH_TYPE_16BIT);
    default: return "Input State unknown";
    }
}

static const char* test_avin_bitDepthString_Ex(
        AMP_IN AMP_AVIN_COLOR_DEPTH_TYPE depth)
{
    switch (depth) {
        case AMP_AVIN_COLOR_DEPTH_TYPE_8BIT:
            return "8bit";
        case AMP_AVIN_COLOR_DEPTH_TYPE_10BIT:
            return "10bit";
        case AMP_AVIN_COLOR_DEPTH_TYPE_12BIT:
            return "12bit";
        case AMP_AVIN_COLOR_DEPTH_TYPE_16BIT:
            return "16bit";
        default:
            return "unknown";
    }
}

static const char* test_avin_colorSpaceString(
        AMP_IN AMP_AVIN_COLOR_FORMAT_TYPE colorspace)
{
    switch (colorspace) {
        STRINGIFY(AMP_AVIN_COLOR_FORMAT_TYPE_RGB);
        STRINGIFY(AMP_AVIN_COLOR_FORMAT_TYPE_YUV444);
        STRINGIFY(AMP_AVIN_COLOR_FORMAT_TYPE_YUV422);
        STRINGIFY(AMP_AVIN_COLOR_FORMAT_TYPE_YUV420);
    default: return "Color space unknown";
    }
}

static const char* test_avin_colorSpaceString_Ex(
        AMP_IN AMP_AVIN_COLOR_FORMAT_TYPE colorspace)
{
    switch (colorspace) {
        case AMP_AVIN_COLOR_FORMAT_TYPE_RGB:
            return "RGB";
        case AMP_AVIN_COLOR_FORMAT_TYPE_YUV444:
            return "YUV444";
        case AMP_AVIN_COLOR_FORMAT_TYPE_YUV422:
            return "YUV422";
        case AMP_AVIN_COLOR_FORMAT_TYPE_YUV420:
            return "YUV420";
        default:
            return "UNKNOWN";
    }
}

static const char* test_avin_inputSourceString(
        AMP_IN AMP_AVIN_SOURCE_TYPE source)
{
      if(source == AMP_AVIN_SOURCE_TYPE_INVALID) return "Invalid";
      else if(source == AMP_AVIN_SOURCE_TYPE_HDMI_01) return "HDMI1";
      else if(source == AMP_AVIN_SOURCE_TYPE_HDMI_02) return "HDMI2";
      else if(source == AMP_AVIN_SOURCE_TYPE_HDMI_03) return "HDMI3";
      else if(source == AMP_AVIN_SOURCE_TYPE_HDMI_04) return "HDMI4";
      else if(source == AMP_AVIN_SOURCE_TYPE_HDMI_05) return "HDMI5";
      else if(source == AMP_AVIN_SOURCE_TYPE_COMPONENT_01) return "COMPONENT1";
      else if(source == AMP_AVIN_SOURCE_TYPE_COMPONENT_02) return "COMPONENT2";
      else if(source == AMP_AVIN_SOURCE_TYPE_CVBS_01) return "CVBS1";
      else if(source == AMP_AVIN_SOURCE_TYPE_CVBS_02) return "CVBS2";
      else if(source == AMP_AVIN_SOURCE_TYPE_VGA_01) return "VGA1";
      else if(source == AMP_AVIN_SOURCE_TYPE_VGA_02) return "VGA2";
      else if(source == AMP_AVIN_SOURCE_TYPE_ATV_01) return "ATV1";
      else if(source == AMP_AVIN_SOURCE_TYPE_ATV_02) return "ATV2";
      else if(source == AMP_AVIN_SOURCE_TYPE_SVIDEO_01) return "SVIDEO1";
      else if(source == AMP_AVIN_SOURCE_TYPE_SVIDEO_02) return "SVIDEO2";
      else if(source == AMP_AVIN_SOURCE_TYPE_SCART_01) return "SCART1";
      else if(source == AMP_AVIN_SOURCE_TYPE_SCART_02) return "SCART2";
      else if(source == AMP_AVIN_SOURCE_TYPE_AUD_ONLY) return "AUDONLY";
      else return "unknown";
}

static const char* test_avin_videoStdString(
        AMP_IN AMP_AVIN_VIDEO_STANDARD videoStd)
{
      if(videoStd == AMP_AVIN_VIDEO_STD_eNTSC_GENERAL) return "NTSC-M";
      else if(videoStd == AMP_AVIN_VIDEO_STD_eNTSC443) return "NTSC443";
      else if(videoStd == AMP_AVIN_VIDEO_STD_ePAL_GENERAL) return "PAL";
      else if(videoStd == AMP_AVIN_VIDEO_STD_ePALM) return "PALM";
      else if(videoStd == AMP_AVIN_VIDEO_STD_ePALN) return "PALN";
      else if(videoStd == AMP_AVIN_VIDEO_STD_eSECAM_GENERAL) return "SECAM";
      else if(videoStd == AMP_AVIN_VIDEO_STD_ePAL60) return "PAL60";
      else if(videoStd == AMP_AVIN_VIDEO_STD_ePALNC) return "PALNC";
      else if(videoStd == AMP_AVIN_VIDEO_STD_eAUTO) return "AUTO";
      else return "unknown";
}

static const char* test_avin_triple3DString(
        AMP_IN AMP_AVIN_RESOLUTION_MODE reMode)
{
    if(reMode == AMP_AVIN_RESOLUTION_MODE_2D) return "2D";
    else if(reMode == AMP_AVIN_RESOLUTION_MODE_FRAME_PACKING) return "3D-FP";
    else if(reMode == AMP_AVIN_RESOLUTION_MODE_SIDE_BY_SIDE) return "3D-SBS";
    else if(reMode == AMP_AVIN_RESOLUTION_MODE_TOP_AND_BOTTOM) return "3D-T/P";
    else if(reMode == AMP_AVIN_RESOLUTION_MODE_VIDEO_4K_2K) return "4K*2K";
    else if(reMode == AMP_AVIN_RESOLUTION_MODE_SENSIO) return "3D-SENSIO";
    else return "3dMode unknown";
}

static BOOL test_avin_is656source(AMP_AVIN_SOURCE_TYPE source)
{
    return ( AMP_AVIN_SOURCE_TYPE_CVBS_01 == source ||
             AMP_AVIN_SOURCE_TYPE_CVBS_02 == source ||
             AMP_AVIN_SOURCE_TYPE_ATV_01 == source ||
             AMP_AVIN_SOURCE_TYPE_ATV_02 == source ||
             AMP_AVIN_SOURCE_TYPE_SCART_01 == source ||
             AMP_AVIN_SOURCE_TYPE_SCART_02 == source);
}

static BOOL test_avin_isCVBSsource(AMP_AVIN_SOURCE_TYPE source,
        AMP_AVIN_SCART_FB_TYPE fb)
{
    BOOL isCVBS = FALSE;

    switch (source) {
        case AMP_AVIN_SOURCE_TYPE_ATV_01:
        case AMP_AVIN_SOURCE_TYPE_ATV_02:
        case AMP_AVIN_SOURCE_TYPE_CVBS_01:
        case AMP_AVIN_SOURCE_TYPE_CVBS_02:
            isCVBS = TRUE;
            break;

        case AMP_AVIN_SOURCE_TYPE_SCART_01:
        case AMP_AVIN_SOURCE_TYPE_SCART_02:
            if (AMP_AVIN_SCART_FB_CVBS == fb) {
                isCVBS = TRUE;
            }
            break;

        default:
            break;
    }

    return isCVBS;
}

static BOOL test_avin_isHdmisource(AMP_AVIN_SOURCE_TYPE source)
{
    return ( AMP_AVIN_SOURCE_TYPE_HDMI_01 == source ||
             AMP_AVIN_SOURCE_TYPE_HDMI_02 == source ||
             AMP_AVIN_SOURCE_TYPE_HDMI_03 == source ||
             AMP_AVIN_SOURCE_TYPE_HDMI_04 == source ||
             AMP_AVIN_SOURCE_TYPE_HDMI_05 == source );
}

static VOID test_avin_dumpAudioInfo(TEST_AVIN_HLD *h,
    AMP_IN AMP_AVIN_INPUT_AUDIO_PARAMS *pAudio)
{
    TEST_AVIN_PRINT("%17s\n", "Audio data");
    TEST_AVIN_PRINT("%17s: %s\n", "audFormat",
        test_avin_audioFormatString(pAudio->audFormat));
    TEST_AVIN_PRINT("%17s: %d\n", "bitDepth", pAudio->bitDepth);
    if(pAudio->audFormat == AMP_MS12_MAT)
        TEST_AVIN_PRINT("%17s: %d\n", "sampleRate", pAudio->sampleRate * 4);    /* Show sample rate multiplied by 4 for HBR content */
    else
        TEST_AVIN_PRINT("%17s: %d\n", "sampleRate", pAudio->sampleRate);
    TEST_AVIN_PRINT("%17s: %d\n\n", "channels", pAudio->channels);
}

static VOID test_avin_dumpVideoInfo(TEST_AVIN_HLD *h,
    AMP_IN AMP_AVIN_INPUT_VIDEO_PARAMS *pVideo)
{
    if( test_avin_isHdmisource(h->curSrc)){
        TEST_AVIN_PRINT("%17s: %s(%s)\n", "Source",
            test_avin_inputSourceString(h->curSrc),
            test_avin_convert2HDMIPortID(h, h->curSrc));
        TEST_AVIN_PRINT("%17s: %d\n", "Picture AR", pVideo->aspRatio);
        TEST_AVIN_PRINT("%17s: %d\n", "Active Format", pVideo->vidActFmt);
        TEST_AVIN_PRINT("%17s: %s\n", "HDMI 3D", test_avin_triple3DString(pVideo->resMode));
        TEST_AVIN_PRINT("%17s: %d\n", "DVI", !pVideo->isHdmiMode);
    } else {
        TEST_AVIN_PRINT("%17s: %s\n", "Source",
            test_avin_inputSourceString(h->curSrc));
    }
    if( test_avin_is656source(h->curSrc) ){
        TEST_AVIN_PRINT("%17s: %s\n", "Video Standard",
            test_avin_videoStdString(pVideo->videoStandard));
        TEST_AVIN_PRINT("%17s: %d\n", "Scart FB", pVideo->scartFB);
        TEST_AVIN_PRINT("%17s: %d\n", "Scart AR", pVideo->aspRatio);
        TEST_AVIN_PRINT("%17s: %d\n", "VCR mode", pVideo->vcrProp.bDetected);
        TEST_AVIN_PRINT("%17s: %d\n", "VCR TrickMode", pVideo->vcrProp.bTrickMode);
    } else {
        TEST_AVIN_PRINT("%17s: %d*%d%c@%0.2fHz\n", "Video resolution",
                                    pVideo->videoWidth,
                                    pVideo->videoHeight,
                                    pVideo->videoProgressive?'p':'i',
                                    pVideo->videoRefrate);
        TEST_AVIN_PRINT("%17s: %s\n", "colorFormat",
                test_avin_colorSpaceString(pVideo->colorFormat));
        TEST_AVIN_PRINT("%17s: %s\n", "colorDepth",
            test_avin_bitDepthString(pVideo->colorDepth));
        TEST_AVIN_PRINT("%17s: %d\n", "resFormat", pVideo->resFormat);
        TEST_AVIN_PRINT("%17s: %d\n", "Full Range", pVideo->videoFullrange);
    }
    TEST_AVIN_PRINT("%17s: %s\n", "outputColorFormat",
        test_avin_colorSpaceString(pVideo->outputColorFormat));
    TEST_AVIN_PRINT("%17s: %s\n", "outputColorDepth",
        test_avin_bitDepthString(pVideo->outputColorDepth));
    TEST_AVIN_PRINT("\n");
}

static VOID test_avin_dumpFetgParams(AMP_IN AMP_AVIN_FETG_PARAMS *pParams)
{
    TEST_AVIN_PRINT("Fetg params:\n");
    TEST_AVIN_PRINT("start: (%d, %d)\n", pParams->start_x, pParams->start_y);
    TEST_AVIN_PRINT("end: (%d, %d)\n", pParams->end_x, pParams->end_y);
    TEST_AVIN_PRINT("field_flag = %d\n", pParams->field_flag);
    TEST_AVIN_PRINT("htotal/vtotal: %d/%d\n", pParams->htotal, pParams->vtotal);
    TEST_AVIN_PRINT("mode_3d = %d\n", pParams->mode_3d);
    TEST_AVIN_PRINT("pixel_freq = %d\n", pParams->pixel_freq);
    TEST_AVIN_PRINT("refresh_rate = %d\n", pParams->refresh_rate);
    TEST_AVIN_PRINT("sync_type = %d\n", pParams->sync_type);
    TEST_AVIN_PRINT("vsamp = %d\n", pParams->vsamp);
}

static BOOL test_avin_isValidAudioParams(
    AMP_IN AMP_AVIN_INPUT_AUDIO_PARAMS *pAudio)
{
    if (0 == pAudio->audFormat ||
        AMP_UNKNOW_AUD == pAudio->audFormat) {
        TEST_AVIN_PRINT("invalid audio format: %d\n", pAudio->audFormat);
        return FALSE;
    }
    if (16 != pAudio->bitDepth &&
        20 != pAudio->bitDepth &&
        24 != pAudio->bitDepth) {
        TEST_AVIN_PRINT("invalid audio bit depth: %d\n", pAudio->bitDepth);
        return FALSE;
    }
    if (0 == pAudio->sampleRate ||
        192000 < pAudio->sampleRate) {
        TEST_AVIN_PRINT("invalid audio sample rate: %d\n", pAudio->sampleRate);
        return FALSE;
    }
    if (2 != pAudio->channels &&
        6 != pAudio->channels &&
        8 != pAudio->channels) {
        TEST_AVIN_PRINT("invalid audio channels: %d\n", pAudio->channels);
        return FALSE;
    }
    return TRUE;
}

static BOOL test_avin_get_mode(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;
    BOOL isTunnelMode = TRUE;
    AMP_COMPONENT avin, vout;
    AMP_AVIN_SOURCE_LIST sourceList;

    return TRUE; //currently stability is not good, will open it later if stability become better

    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_AVIN, 1, &avin);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_VOUT, 1, &vout);
    assert(SUCCESS == r);

    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_AVIN;
    config._u.pAVIN.mode = AMP_TUNNEL;
    config._u.pAVIN.bOpenVideoPort = TRUE;
    config._u.pAVIN.bOpenVideoPort2 = FALSE;
    config._u.pAVIN.bOpenVbiPort = TRUE;
    config._u.pAVIN.bOpenAudioPort = TRUE;
    config._u.pAVIN.bOpenAudioPort2 = FALSE;
    AMP_RPC(r, AMP_AVIN_Open, avin, &config);
    if( SUCCESS !=r ){
        TEST_AVIN_PRINT("AVIN was already running...\n");
        isTunnelMode = FALSE;
        goto exit;
    }

    AMP_RPC(r, AMP_AVIN_GetSupportedSourceList, avin, &sourceList);
    if (sourceList.size == 0) {
        TEST_AVIN_PRINT("No AVIN source is supported.\n");
        isTunnelMode = FALSE;
        AMP_RPC(r, AMP_AVIN_Close, avin);
        goto exit;
    }
    AMP_RPC(r, AMP_AVIN_Close, avin);

    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_VOUT;
    config._u.pVOUT.uiInputPortNum = 2;
    config._u.pVOUT.uiOutputPortNum = 0;
    config._u.pVOUT.uiPlaneID = h->outputPlane;
    AMP_RPC(r, AMP_VOUT_Open, vout, &config);
    if( SUCCESS !=r ){
        TEST_AVIN_PRINT("VOUT was already running...\n");
        isTunnelMode = FALSE;
        goto exit;
    } else {
        AMP_RPC(r, AMP_VOUT_Close, vout);
    }

exit:
    AMP_RPC(r, AMP_AVIN_Destroy, avin);
    assert(SUCCESS == r);
    AMP_RPC(r, AMP_VOUT_Destroy, vout);
    assert(SUCCESS == r);

    AMP_FACTORY_Release(avin);
    AMP_FACTORY_Release(vout);

    return isTunnelMode;
}

static HRESULT test_avin_openAvinComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    AVIN_TEST_LOGD("open avin.\n");
    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_AVIN;
    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        config._u.pAVIN.mode = AMP_TUNNEL;
        if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
            config._u.pAVIN.bOpenVideoPort = TRUE;
            config._u.pAVIN.bOpenVideoPort2 = FALSE;
            config._u.pAVIN.bOpenVbiPort = TRUE;
            config._u.pAVIN.bOpenAudioPort = TRUE;
            config._u.pAVIN.bOpenAudioPort2 = FALSE;
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
            config._u.pAVIN.bOpenVideoPort = FALSE;
            config._u.pAVIN.bOpenVideoPort2 = TRUE;
            config._u.pAVIN.bOpenVbiPort = FALSE;
            config._u.pAVIN.bOpenAudioPort = FALSE;
            config._u.pAVIN.bOpenAudioPort2 = TRUE;
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
            config._u.pAVIN.bOpenVideoPort = TRUE;
            config._u.pAVIN.bOpenVideoPort2 = TRUE;
            config._u.pAVIN.bOpenVbiPort = FALSE;
            config._u.pAVIN.bOpenAudioPort = TRUE;
            config._u.pAVIN.bOpenAudioPort2 = TRUE;
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
            config._u.pAVIN.bOpenVideoPort = FALSE;
            config._u.pAVIN.bOpenVideoPort2 = TRUE;
            config._u.pAVIN.bOpenVbiPort = FALSE;
            config._u.pAVIN.bOpenAudioPort = FALSE;
            config._u.pAVIN.bOpenAudioPort2 = FALSE;
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
            config._u.pAVIN.bOpenVideoPort = FALSE;
            config._u.pAVIN.bOpenVideoPort2 = TRUE;
            config._u.pAVIN.bOpenVbiPort = FALSE;
            config._u.pAVIN.bOpenAudioPort = FALSE;
            config._u.pAVIN.bOpenAudioPort2 = FALSE;
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
            config._u.pAVIN.bOpenVideoPort = TRUE;
            config._u.pAVIN.bOpenVideoPort2 = TRUE;
            config._u.pAVIN.bOpenVbiPort = FALSE;
            config._u.pAVIN.bOpenAudioPort = TRUE;
            config._u.pAVIN.bOpenAudioPort2 = FALSE;
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
            config._u.pAVIN.bOpenVideoPort = FALSE;
            config._u.pAVIN.bOpenVideoPort2 = TRUE;
            config._u.pAVIN.bOpenVbiPort = FALSE;
            config._u.pAVIN.bOpenAudioPort = FALSE;
            config._u.pAVIN.bOpenAudioPort2 = TRUE;
        }
    } else {
        config._u.pAVIN.mode = AMP_TUNNEL;
        config._u.pAVIN.bOpenVideoPort = FALSE;
        config._u.pAVIN.bOpenVideoPort2 = FALSE;
        config._u.pAVIN.bOpenVbiPort = FALSE;
        config._u.pAVIN.bOpenAudioPort = FALSE;
        config._u.pAVIN.bOpenAudioPort2 = FALSE;
    }

    AMP_RPC(r, AMP_AVIN_Open, h->avin, &config);
    return r;
}

static HRESULT test_avin_setupAvinComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    BOOL tunnelMode = TRUE;

    AVIN_TEST_LOGD("create avin component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_AVIN, 1, &h->avin);
    assert(SUCCESS == r);

    tunnelMode = test_avin_get_mode(h);
    if (tunnelMode == FALSE) {
        TEST_AVIN_PRINT("AV pipline is already created\n");
        h->testMode = TEST_AVIN_MODE_NON_TUNNEL;
    } else {
        TEST_AVIN_PRINT("AVIN is not in use, will setup AV pipeline\n");
        h->testMode = TEST_AVIN_MODE_TUNNEL;
    }
    r = test_avin_openAvinComponent(h);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

#ifdef AVIN_DEMO_SIMPLE
static HRESULT test_avin_avinVoutCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;
    UINT32 uiIdx = 0, uiTagCnt = 0;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    VOID *pVirtAddr;
    struct timeval tv1,tv2;

    gettimeofday(&tv1,NULL);

    assert(NULL != hBufDesc);
    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Lock(globalVideoMutex);

    //AVIN_TEST_LOGD ("push BD (%p) to avin out video port. hBufDesc->uiBDId = 0x%x\n",
    //        hBufDesc, hBufDesc->uiBDId);
    //
    // Any processing of the frame can be done here before pushing

    r = AMP_BDTAG_GetNum(hBufDesc, &uiTagCnt);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_BDTAG_GetNum! r = 0x%x\n", r);
        return r;
    }

    for (uiIdx = 0; uiIdx < uiTagCnt; ++uiIdx) {
        r = AMP_BDTAG_GetWithIndex(hBufDesc, uiIdx, (VOID**)&pMemInfo);
        if (SUCCESS == r) {
            if (AMP_BDTAG_ASSOCIATE_MEM_INFO == pMemInfo->Header.eType) {
                VOID *pAddr = NULL;
                r = AMP_SHM_Ref(pMemInfo->uMemHandle, NULL);
                if (SUCCESS != r) {
                    AVIN_TEST_LOGD("leave ref shmHanle 0x%x failed r = 0x%x\n",
                    pMemInfo->uMemHandle, r);
                    return r;
                }

                VERIFY_RESULT(AMP_SHM_GetVirtualAddress(pMemInfo->uMemHandle,
                                                                0, &pVirtAddr));
                //AVIN_TEST_LOGD ("pMemInfo->uSize = %d\n", pMemInfo->uSize);
                //AVIN_TEST_LOGD ("pMemInfo->uFlag = %d\n", pMemInfo->uFlag);
                if (pMemInfo->uSize > 0) {
                    addwatermark(pVirtAddr,pVirtAddr,pMemInfo->uSize);
                }

                AMP_SHM_Unref(pMemInfo->uMemHandle);
                break;
            }
        }
    }


    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_VOUT_PushBD, pHld->vout, AMP_PORT_INPUT, 0, hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(globalVideoMutex);

    gettimeofday(&tv2,NULL);

    AVIN_TEST_LOGD("cost time %ld\n",(tv2.tv_sec * 1000 + tv2.tv_usec / 1000)- (tv1.tv_sec * 1000 + tv1.tv_usec / 1000));


    return r;
}
#else
#define DROP_FRAME_NUM 300

static HRESULT test_avin_avinVoutCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;
    UINT32 uiIdx = 0, uiTagCnt = 0;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    VOID *pVirtAddr;
    struct timeval tv1,tv2;
    unsigned int avail;
    int ret;
    int watermark_len;
    static int count =0;

    gettimeofday(&tv1,NULL);

    assert(NULL != hBufDesc);
    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Lock(globalVideoMutex);

    //AVIN_TEST_LOGD ("push BD (%p) to avin out video port. hBufDesc->uiBDId = 0x%x\n",
    //        hBufDesc, hBufDesc->uiBDId);
    //
    // Any processing of the frame can be done here before pushing

    count ++;
    r = AMP_BDTAG_GetNum(hBufDesc, &uiTagCnt);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_BDTAG_GetNum! r = 0x%x\n", r);
        return r;
    }

    for (uiIdx = 0; uiIdx < uiTagCnt; ++uiIdx) {
        r = AMP_BDTAG_GetWithIndex(hBufDesc, uiIdx, (VOID**)&pMemInfo);
        if (SUCCESS == r) {
            if (AMP_BDTAG_ASSOCIATE_MEM_INFO == pMemInfo->Header.eType) {
                VOID *pAddr = NULL;
                r = AMP_SHM_Ref(pMemInfo->uMemHandle, NULL);
                if (SUCCESS != r) {
                    AVIN_TEST_LOGD("leave ref shmHanle 0x%x failed r = 0x%x\n",
                    pMemInfo->uMemHandle, r);
                    return r;
                }

                VERIFY_RESULT(AMP_SHM_GetVirtualAddress(pMemInfo->uMemHandle,
                                                                0, &pVirtAddr));
                AVIN_TEST_LOGD ("pMemInfo->uSize = %d\n", pMemInfo->uSize);
                //AVIN_TEST_LOGD ("pMemInfo->uFlag = %d\n", pMemInfo->uFlag);
                if (pMemInfo->uSize > 0) {

                    frame_size = pMemInfo->uSize;

                    avail = ringbuf_avail(&gRingbuf) ;
                    if(avail >= pMemInfo->uSize && count > DROP_FRAME_NUM){
                        ret = ringbuf_in(&gRingbuf, pVirtAddr, pMemInfo->uSize);
                        if(ret < pMemInfo->uSize)
                        {
                            printf("warning:write ringbuf less then size %ld %ld\n",ret,pMemInfo->uSize);
                        }
                    }
                    else
                    {
                        printf("rb full\n");
                    }

                }
                watermark_len = ringbuf_len(&gWMbuf);
                printf("frame size %d\n",frame_size);
                if(watermark_len >= frame_size)
                {
                    ret = ringbuf_out(&gWMbuf, pVirtAddr, frame_size);
                    if(ret < frame_size)
                        printf("warning,read watermark ringbuf less then size\n");
                }
                else if(watermark_len == frame_size)
                {
                    ret = ringbuf_out_peek(&gWMbuf, pVirtAddr, frame_size);
                    printf("wm one frame empty\n");
                }
                else
                {
                    printf("wm rb real empty\n");
                }

                AMP_SHM_Unref(pMemInfo->uMemHandle);
                break;
            }
        }
    }


    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_VOUT_PushBD, pHld->vout, AMP_PORT_INPUT, 0, hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(globalVideoMutex);

    gettimeofday(&tv2,NULL);

    AVIN_TEST_LOGD("cost time %ld\n",(tv2.tv_sec * 1000 + tv2.tv_usec / 1000)- (tv1.tv_sec * 1000 + tv1.tv_usec / 1000));


    return r;
}
#endif


static HRESULT test_avin_voutAvinCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;
    UINT32 uiIdx = 0, uiTagCnt = 0;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;

    assert(NULL != hBufDesc);
    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Lock(globalVideoMutex);

    //AVIN_TEST_LOGD ("push BD (%p) to vout out video port. hBufDesc->uiBDId = 0x%x\n",
    //        hBufDesc, hBufDesc->uiBDId);

    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_AVIN_PushBD, pHld->avin, AMP_PORT_OUTPUT, pHld->uiVideoPort2, hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(globalVideoMutex);

    return r;
}

#if 0
static HRESULT test_avin_videoBufferCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;
    UINT32 uiIdx = 0, uiTagCnt = 0;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;

    assert(NULL != hBufDesc);
    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Lock(globalVideoMutex);

    //AVIN_TEST_LOGD ("push BD (%p) to avin out video port. hBufDesc->uiBDId = 0x%x\n",
    //        hBufDesc, hBufDesc->uiBDId);
    time_t seconds;
    struct tm *tmStruct;
    seconds = time(NULL);
    tmStruct = localtime(&seconds);

    if ((tmStruct->tm_sec)%60 == pHld->crntTm%60) {
        if (pHld->fp_out_es == NULL) {
            INT8 dumpPath[1024];
            sprintf(dumpPath, "/mnt/capture_%d.dat", pHld->flCnt);
            pHld->fp_out_es = open(dumpPath, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
            if (pHld->fp_out_es < 0) {
                AVIN_TEST_LOGD("Error: the es file does not exist.\n");
                exit(-1);
            }
        }
        if (pHld->frCnt < pHld->reqFR) {
            r = AMP_BDTAG_GetNum(hBufDesc, &uiTagCnt);
            if (SUCCESS != r) {
                AMPLOGE("fail to call AMP_BDTAG_GetNum! r = 0x%x\n", r);
                goto exit1;
            }

            for (uiIdx = 0; uiIdx < uiTagCnt; ++uiIdx) {
                r = AMP_BDTAG_GetWithIndex(hBufDesc, uiIdx, (VOID**)&pMemInfo);
                if (SUCCESS == r) {
                   if (AMP_BDTAG_ASSOCIATE_MEM_INFO == pMemInfo->Header.eType) {
                        VOID *pAddr = NULL;
                        r = AMP_SHM_Ref(pMemInfo->uMemHandle, NULL);
                        if (SUCCESS != r) {
                            AVIN_TEST_LOGD("leave ref shmHanle 0x%x failed r = 0x%x\n",
                                    pMemInfo->uMemHandle, r);
                            goto exit1;
                        }
                        VOID *pVirtAddr;
                        VERIFY_RESULT(AMP_SHM_GetVirtualAddress(pMemInfo->uMemHandle,
                                                                0, &pVirtAddr));
                        //AVIN_TEST_LOGD ("pMemInfo->uSize = %d\n", pMemInfo->uSize);
                        //AVIN_TEST_LOGD ("pMemInfo->uFlag = %d\n", pMemInfo->uFlag);
                        if (pMemInfo->uSize > 0) {
                            char *buf = malloc(pMemInfo->uSize);
                            memcpy(buf, pVirtAddr, pMemInfo->uSize);
                            write(pHld->fp_out_es, buf, pMemInfo->uSize);
                            free(buf);
                        }

                        AMP_SHM_Unref(pMemInfo->uMemHandle);
                        break;
                    }
                }
            }
            pHld->frCnt++;
        }
    } else {
        pHld->crntTm = tmStruct->tm_sec;
        pHld->frCnt = 0;
        pHld->flCnt++;
        if (pHld->fp_out_es) {
            close(pHld->fp_out_es);
            pHld->fp_out_es = NULL;
        }
    }

    if (pHld->crntTm%60 == 0)
        pgHld->stTm = 0;
    if (abs(pHld->crntTm%60 - pgHld->stTm%60) == pHld->delIntvl) {
        int i;
        for (i = 0; i < pHld->flCnt-1; i++) {
            INT8 delPath[1024];
            sprintf(delPath, "/data/capture_%d.dat", i);
            remove (delPath);
        }
        pgHld->stTm = pHld->crntTm;
    }

exit1:
    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_AVIN_PushBD, hCompObj, ePortIo, uiPortIdx, hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(globalVideoMutex);

    return r;
}

#else
static int drop_cnt=0;
static HRESULT test_avin_videoBufferCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;
    UINT32 uiIdx = 0, uiTagCnt = 0;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    UINT32 delcnt;
    struct timeval tv1,tv2;
    unsigned long long now,end;
    unsigned int ret;
    unsigned int avail=0;
    drop_cnt ++;

    //printf ("push BD (%p) to avin out video port. hBufDesc->uiBDId = 0x%x uiTagCnt:%d cnt:%d\n",
    //        hBufDesc, hBufDesc->uiBDId,uiTagCnt,pHld->frCnt);

    gettimeofday(&tv1,NULL);

    now = tv1.tv_sec * 1000 + tv1.tv_usec / 1000;

    assert(NULL != hBufDesc);
    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Lock(globalVideoMutex);



    r = AMP_BDTAG_GetNum(hBufDesc, &uiTagCnt);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_BDTAG_GetNum! r = 0x%x\n", r);
        goto exit1;
    }

    for (uiIdx = 0; uiIdx < uiTagCnt; ++uiIdx) {
        r = AMP_BDTAG_GetWithIndex(hBufDesc, uiIdx, (VOID**)&pMemInfo);
        if (SUCCESS == r) {
            if (AMP_BDTAG_ASSOCIATE_MEM_INFO == pMemInfo->Header.eType) {
                VOID *pAddr = NULL;
                r = AMP_SHM_Ref(pMemInfo->uMemHandle, NULL);
                if (SUCCESS != r) {
                    AVIN_TEST_LOGD("leave ref shmHanle 0x%x failed r = 0x%x\n",
                            pMemInfo->uMemHandle, r);
                    goto exit1;
                }
                VOID *pVirtAddr;
                VERIFY_RESULT(AMP_SHM_GetVirtualAddress(pMemInfo->uMemHandle,
                                                            0, &pVirtAddr));
                printf ("pMemInfo->uSize = %d\n", pMemInfo->uSize);
                //AVIN_TEST_LOGD ("pMemInfo->uFlag = %d\n", pMemInfo->uFlag);
                if (pMemInfo->uSize > 0) {

                    frame_size = pMemInfo->uSize;

                    avail = ringbuf_avail(&gRingbuf) ;
                    if( avail >= pMemInfo->uSize && drop_cnt > 300 ){
                        ret = ringbuf_in(&gRingbuf, pVirtAddr, pMemInfo->uSize);
                        if(ret < pMemInfo->uSize)
                        {
                            printf("warning:write ringbuf less then size %ld %ld\n",ret,pMemInfo->uSize);
                        }
                    }
                    else
                    {
                        printf("ringbuf full,drop frame\n");
                    }
                }

                AMP_SHM_Unref(pMemInfo->uMemHandle);
                break;
            }

        }
        else
            printf("AMP_BDTAG_GetWithIndex fail\n");
    }


exit1:
    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_AVIN_PushBD, hCompObj, ePortIo, uiPortIdx, hBufDesc);
    assert(SUCCESS == r);

   MV_OSAL_Mutex_Unlock(globalVideoMutex);

    gettimeofday(&tv2,NULL);

    end = tv2.tv_sec * 1000 + tv2.tv_usec / 1000;

    printf("videoBufferCB tick %ld ms delt %ld\n",now, end-now);

    return r;
}

#endif

static HRESULT test_avin_openDumComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    AVIN_TEST_LOGD("open audDum.\n");
    AmpMemClear(&config,sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_DUM;
    config._u.pDUM.mode = AMP_TUNNEL;
    config._u.pDUM.uiPlaybackFragmentSize = 0x10000;
    config._u.pDUM.uiPlaybackIntervalTime = 1000;
    config._u.pDUM.uiPlaybackBdNumber = 20;
    strcpy(config._u.pDUM.sRecordingFile, h->output_aud_file_path);

    AMP_RPC(r, AMP_DUM_Open, h->audDum, &config);
    assert(SUCCESS == r);
    return r;
}

static HRESULT test_avin_setupDumComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter.\n");

    AVIN_TEST_LOGD("create audDum component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_DUM, 1, &h->audDum);
    assert(SUCCESS == r);

    r = test_avin_openDumComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_openDeintComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    AVIN_TEST_LOGD("open deint.\n");
    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_DEINT;
    config._u.pDEINT.mode = AMP_TUNNEL;

    AMP_RPC(r, AMP_DEINT_Open, h->deint, &config);
    return r;
}

static HRESULT test_avin_setupDeintComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter.\n");

    AVIN_TEST_LOGD("create deint component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_DEINT, 1, &h->deint);
    assert(SUCCESS == r);

    r = test_avin_openDeintComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_openVdecComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    AVIN_TEST_LOGD("open vdec.\n");
    AmpMemClear(&config,sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_VDEC;
    config._u.pVDEC.mode = AMP_TUNNEL;
    config._u.pVDEC.uiType = MEDIA_VES_AVC;
    config._u.pVDEC.uiSHMSize = MAX_STREAM_SIZE * ES_BUFFER_SIZE;
    config._u.pVDEC.uiFlag |= 1 << 15;
    AMP_RPC(r, AMP_VDEC_Open, h->vdec, &config);
    assert(SUCCESS == r);
    return r;
}
static HRESULT test_avin_setupVdecComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter.\n");

    AVIN_TEST_LOGD("create vdec component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_VDEC, 1, &h->vdec);
    assert(SUCCESS == r);

    r = test_avin_openVdecComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_vencPushOutputBuf(TEST_AVIN_HLD *h) {
    UINT32 uNum;
    AMP_BD_HANDLE hBD;
    AMP_BDTAG_MEMINFO *pMemInfo;
    AMP_BDTAG_STREAM_INFO *pStreamInfo;
    HRESULT result = SUCCESS;

    if(AMP_BDCHAIN_GetItemNum(h->pStreamBDQ, &uNum) == SUCCESS){
        while ( uNum > 0) {
            VERIFY_RESULT(AMP_BDCHAIN_PopItem(h->pStreamBDQ, &hBD));
            uNum--;
            VERIFY_RESULT(AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo));
            pMemInfo->uSize = VENC_ES_BUFFER_ALLOC_SIZE;
            pMemInfo->uMemOffset = 0;
            pMemInfo->uFlag = 0;

            VERIFY_RESULT(AMP_BDTAG_GetWithType(hBD, AMP_BDTAG_STREAM_INFO_META, NULL,
                                                (void **)&pStreamInfo));
            pStreamInfo->uType = 0;
            pStreamInfo->uiPtsHigh = 0;
            pStreamInfo->uiPtsLow = 0;
            pStreamInfo->uUnitFlag = 0;
            pStreamInfo->isHeader = 0;

            //AVIN_TEST_LOGD ("Push Stream BD %p\n", hBD);
            AMP_RPC(result, AMP_VENC_PushBD, h->venc, AMP_PORT_OUTPUT, 0, hBD);
            VERIFY_RESULT(result);
        }
    }

    return result;
}

static void *test_avin_sendBufferTask(void *arg)
{
    HRESULT result = SUCCESS;
    TEST_AVIN_HLD *h = NULL;

    h = (TEST_AVIN_HLD *)arg;

    while (!h->playBackStopped) {
        test_avin_vencPushOutputBuf(h);
        //usleep(50000);
        usleep(10000);
    }
    return NULL;
}

static void *test_avin_process_ringbuf(void *arg)
{
    HRESULT result = SUCCESS;
    TEST_AVIN_HLD *h = NULL;
    unsigned int avail;
    int ret = 0;
    h = (TEST_AVIN_HLD *)arg;
    char *gBuf = malloc(ONE_FRAME_SIZE);
    int fd =-1;
    //int res=0;


    if(gBuf == NULL)
    {
        printf("fatal error,can not alloc mem\n");
        return ;
    }

#if 0
    fd = open(RX_FILE_PATH, O_RDWR | O_CREAT, 0664);
    if(fd < 0)
    {
        printf("open file %s fail,error %d\n",RX_FILE_PATH, errno);
        return -1;
    }
#endif

    while (!h->playBackStopped) {

        if(ringbuf_len(&gRingbuf) < NON_RB_SIZE){
            printf("rb empty,need fill rb11\n");
            usleep(5000);
            continue;
        }

        //gBuf = ringbuf_get_in_pointer(&gWMbuf); //can not handle ring
        ret = ringbuf_out(&gRingbuf, gBuf, frame_size);
        printf("rb out %d %d\n",ret,ringbuf_len(&gRingbuf));
        if(ret < frame_size)
            printf("rb out less than one frame %d\n",ret);

        //ret = write(fd, gBuf,ONE_FRAME_SIZE);
        //if(ret <0)
            //printf("write file fail errno %d\n",errno);
	    addwatermark(gBuf,gBuf,frame_size);
        //memcpy(p_buf,gBuf,ONE_FRAME_SIZE);
        //ringbuf_update_in_pointer(&gWMbuf,ONE_FRAME_SIZE);
LOOP1:
        avail = ringbuf_avail(&gWMbuf) ;

        if( avail >= frame_size){

            ret = ringbuf_in(&gWMbuf,gBuf,frame_size);
            if(ret < frame_size)
                printf("wm ringbuf in no space\n");
        }
        else
        {
            usleep(5000);
            printf("watermark no space left\n");
            goto LOOP1;
        }

        //usleep(10000);
    }

    //close(fd);

    return NULL;
}

static void *test_avin_process_filedump(void *arg)
{
    HRESULT result = SUCCESS;
    TEST_AVIN_HLD *h = NULL;
    unsigned int avail;
    int ret = 0;
    h = (TEST_AVIN_HLD *)arg;
    char *gBuf = malloc(ONE_FRAME_SIZE);
    int fd =-1;
    //int res=0;


    if(gBuf == NULL)
    {
        printf("fatal error,can not alloc mem\n");
        return ;
    }

    fd = open(DUMP_RX_PATH, O_RDWR | O_CREAT, 0664);
    if(fd < 0)
    {
        printf("open file %s fail,error %d\n",RX_FILE_PATH, errno);
        return -1;
    }
    while (!h->playBackStopped) {

        if(ringbuf_len(&gRingbuf) < frame_size){
            printf("rb empty,need fill rb22\n");
            usleep(8000);
            continue;
        }

        //gBuf = ringbuf_get_in_pointer(&gWMbuf); //can not handle ring
        ret = ringbuf_out(&gRingbuf, gBuf, frame_size);
        //printf("rb out %d %d\n",ret,ringbuf_len(&gRingbuf));
        if(ret < frame_size)
            printf("rb out less than one frame %d\n",ret);

        ret = write(fd, gBuf,frame_size);
        if(ret < 0)
            printf("write file fail errno %d\n",errno);

        //usleep(10000);
    }

    close(fd);

    return NULL;
}

static HRESULT test_avin_vencCB(CORBA_Object hCompObj, AMP_PORT_IO ePortIo,
                       UINT32 uiPortIdx, struct AMP_BD_ST *hBD,
                       AMP_IN void *pUserData) {
    TEST_AVIN_HLD *h = (TEST_AVIN_HLD *) pUserData;
    AMP_BDTAG_MEMINFO *pMemInfo;
    AMP_BDTAG_STREAM_INFO *pStreamInfo;
    UINT32 uNum = 0;
    MV_OSAL_Mutex_Lock(globalVideoMutex);

    VERIFY_RESULT (AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo));

    if (ePortIo == AMP_PORT_OUTPUT) {
        if (h->bEOS == false) {
            if (pMemInfo->uFlag & AMP_MEMINFO_FLAG_EOS_MASK) {
                //AVIN_TEST_LOGD("##VENC Got EOS\n");
                h->bEOS = true;
                pMemInfo->uFlag = 0;
            }
        }

        VOID *pVirtAddr;
        VERIFY_RESULT(AMP_SHM_GetVirtualAddress(pMemInfo->uMemHandle,
                                                0, &pVirtAddr));
        //AVIN_TEST_LOGD ("pMemInfo->uSize = %d\n", pMemInfo->uSize);
        //AVIN_TEST_LOGD ("pMemInfo->uFlag = %d\n", pMemInfo->uFlag);
        if (pMemInfo->uSize > 0) {
            char *buf = malloc(pMemInfo->uSize);
            memcpy(buf, pVirtAddr, pMemInfo->uSize);
            write(h->fp_out_es, buf, pMemInfo->uSize);
            free(buf);
        }
        // Get streamInfo.
        if (pMemInfo->uSize > 0) {
            VERIFY_RESULT(AMP_BDTAG_GetNum(hBD, &uNum));
            VERIFY_RESULT(AMP_BDTAG_GetWithIndex(hBD, uNum - 1, (void **)&pStreamInfo));
            //AVIN_TEST_LOGD ("streamInfo ptsH=0x%08x, ptsL=0x%08x, picType=%d, uUnitFlag=%d, isHeader=%d\n",
            //        pStreamInfo->uiPtsHigh,
            //        pStreamInfo->uiPtsLow,
            //        pStreamInfo->uType,
            //        pStreamInfo->uUnitFlag,
            //        pStreamInfo->isHeader);
        }

        VERIFY_RESULT(AMP_BDCHAIN_PushItem(h->pStreamBDQ, hBD));
    }
    MV_OSAL_Mutex_Unlock(globalVideoMutex);
    return SUCCESS;
}


static HRESULT test_avin_vencInitInstance(TEST_AVIN_HLD *h, UINT32 mode) {
    HRESULT result;
    AMP_BD_HANDLE hBD;
    AMP_SHM_HANDLE hSHM;
    AMP_BDTAG_MEMINFO memInfo;
    AMP_BDTAG_STREAM_INFO streamInfo;
    int i = 0;
    UINT32 BDId;

    // One stream BD with one memInfo and one streamInfo.
    AmpMemClear(&memInfo, sizeof(AMP_BDTAG_MEMINFO));
    memInfo.Header.eType = AMP_BDTAG_ASSOCIATE_MEM_INFO;
    memInfo.Header.uLength = sizeof(AMP_BDTAG_MEMINFO);
    AmpMemClear(&streamInfo, sizeof(AMP_BDTAG_STREAM_INFO));
    streamInfo.Header.eType = AMP_BDTAG_STREAM_INFO_META;
    streamInfo.Header.uLength = sizeof(AMP_BDTAG_STREAM_INFO);

    h->mode = mode;
    h->bEOS = false;

    if (mode == AMP_NON_TUNNEL) {
        VERIFY_RESULT(AMP_BDCHAIN_Create(TRUE, &h->pStreamBDQ));

        for (i = 0; i < MAX_OUTPUT_STREAM_NUM; i++) {
            VERIFY_RESULT(AMP_BD_Allocate(&hBD));
            VERIFY_RESULT(AMP_SHM_Allocate(AMP_SHM_DYNAMIC,
                                           VENC_ES_BUFFER_ALLOC_SIZE, 1024, &hSHM));
            memInfo.uMemHandle = hSHM;
            memInfo.uMemOffset = 0;
            memInfo.uSize = VENC_ES_BUFFER_ALLOC_SIZE;

            VERIFY_RESULT(AMP_BDTAG_Append(hBD, (UINT8 *)&memInfo, NULL, NULL));
            VERIFY_RESULT(AMP_BDTAG_Append(hBD, (UINT8 *)&streamInfo, NULL, NULL));
            VERIFY_RESULT(AMP_BDCHAIN_PushItem(h->pStreamBDQ, hBD));
        }
    }

    return SUCCESS;
}

static HRESULT test_avin_vencDeinitInstance(TEST_AVIN_HLD *h) {
    HRESULT result;
    AMP_BD_HANDLE hBD;
    AMP_SHM_HANDLE hSHM;
    UINT32 uNum;
    UINT32 i = 0;

    if (h->mode == AMP_NON_TUNNEL) {
        VERIFY_RESULT(AMP_BDCHAIN_GetItemNum(h->pStreamBDQ, &uNum));
        for (i = 0; i < uNum; i++) {
            AMP_BDTAG_MEMINFO *pMemInfo;
            VERIFY_RESULT(AMP_BDCHAIN_PopItem(h->pStreamBDQ, &hBD));
            VERIFY_RESULT(AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo));
            VERIFY_RESULT(AMP_SHM_Release(pMemInfo->uMemHandle));
            VERIFY_RESULT(AMP_BD_Free(hBD));
        }
        VERIFY_RESULT(AMP_BDCHAIN_Destroy(h->pStreamBDQ));
    }
    return SUCCESS;
}

static HRESULT test_avin_openVencComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    AVIN_TEST_LOGD("open venc.\n");
    AmpMemClear(&config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_VENC;
    if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE ||
            pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
        config._u.pVENC.mode = AMP_NON_TUNNEL;
    } else if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
        config._u.pVENC.mode = AMP_TUNNEL;
    }

    /* Input config */
    config._u.pVENC.uiWidth = h->width;
    config._u.pVENC.uiHeight = h->height;
    config._u.pVENC.uiColorFmt = h->input_format;

    /* Output config */
    config._u.pVENC.uiCodingFmt = h->enc_codec;
    config._u.pVENC.uiEncMode = AMP_VENC_MODE_EXPRESS;
    config._u.pVENC.uiEntropyMode = 1; // default enable CABAC
    config._u.pVENC.uiTransformFlag = 1; // default enable 8x8 transform
    config._u.pVENC.uiInitQp = 10; //[0 ~ 100]
    config._u.pVENC.uiGopSize = 30;
    config._u.pVENC.uiGopType = AMP_RCGOP_IP;
    config._u.pVENC.uiH264Profile = 0; // VENC_PROFILE_MAIN
    config._u.pVENC.uiH264Level = 41; // VENC_LEVEL_4_1
    config._u.pVENC.uiFrameRateNum =  h->fr_num;
    config._u.pVENC.uiFrameRateDen =  1;
    config._u.pVENC.uiRateCtrlSel = 9;
    config._u.pVENC.uiVBVSize = 0;
    config._u.pVENC.uiHRDBitRate = config._u.pVENC.uiWidth * config._u.pVENC.uiHeight * 12;
    config._u.pVENC.uiDownScaleFlag = 0;
    config._u.pVENC.uiQuarterMv = 1; //adaptive
    config._u.pVENC.uiAvcOn = 1;
    config._u.pVENC.uiAudOn = 0;
    config._u.pVENC.uiDownScaleFlag = 0;

    AVIN_TEST_LOGD ("Open venc mode=%d size=%dx%d colorfmt=%d codingfmt=%d downscaleflag=%d "
            "uiAvcOn=%d uiAudOn=%d\n",
            config._u.pVENC.mode,
            config._u.pVENC.uiWidth, config._u.pVENC.uiHeight,
            config._u.pVENC.uiColorFmt, config._u.pVENC.uiCodingFmt,
            config._u.pVENC.uiDownScaleFlag,
            config._u.pVENC.uiAvcOn, config._u.pVENC.uiAudOn);

    AMP_RPC(r, AMP_VENC_Open, h->venc, &config);
    assert(SUCCESS == r);

    return r;
}

static HRESULT test_avin_setupVencComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter.\n");

    AVIN_TEST_LOGD("create venc component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_VENC, 1, &h->venc);
    assert(SUCCESS == r);

    r = test_avin_openVencComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_openVoutComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    AVIN_TEST_LOGD("open vout.\n");
    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_VOUT;
    config._u.pVOUT.uiInputPortNum = 2;
    config._u.pVOUT.uiOutputPortNum = 0;
    config._u.pVOUT.uiPlaneID = h->outputPlane;
    //config._u.pVOUT.uiPlaneMute = 1;

    AMP_RPC(r, AMP_VOUT_Open, h->vout, &config);
    return r;
}

static HRESULT test_avin_setupVoutComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter.\n");

    AVIN_TEST_LOGD("create vout component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_VOUT, 1, &h->vout);
    assert(SUCCESS == r);

    r = test_avin_openVoutComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static AMP_STATE test_avin_getAdecState(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_STATE state = AMP_LOADED;
    AMP_RPC(r, AMP_ADEC_GetState, h->adec, &state);
    assert(SUCCESS == r);
    return state;
}

static HRESULT test_avin_openAdecComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;

    if (FALSE == test_avin_isValidAudioParams(&h->audioParams)) {
        return ERR_ERRPARAM;
    }
    if (AMP_LOADED != test_avin_getAdecState(h)) {
        return ERR_SWSTATEWRONG;
    }

    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_ADEC;
    config._u.pADEC.uiInPortNum  = 1;
    config._u.pADEC.uiOutPortNum = h->enable_passthr ? 3: 1;
    TEST_AVIN_PRINT("%s:%d h->enable_passthr %d, h->audioParams.audFormat %d\n",
         __FUNCTION__, __LINE__, h->enable_passthr, h->audioParams.audFormat);

    AMP_ADEC_PARAS adecParam;
    AmpMemClear((VOID*) &adecParam, sizeof(AMP_ADEC_PARAS));

    switch (h->audioParams.audFormat) {
    case AMP_DD_AC3:
    case AMP_DD_PLUS:
        {
            if (h->enableMS11) {
                UINT32 i;
                TEST_AVIN_PRINT("config adec with MS11_DDC (DD_AC3/DD_PLUS)\n");
                config._u.pADEC.eAdecFmt = AMP_MS11_DDC;
                adecParam._d = AMP_ADEC_PARAIDX_MS11_DDC;
                adecParam._u.MS11_DDC.nDualInput = FALSE;
                adecParam._u.MS11_DDC.nDualDecode = FALSE;
                adecParam._u.MS11_DDC.nOutputMain = TRUE;
                adecParam._u.MS11_DDC.nOutputSec = FALSE;
                adecParam._u.MS11_DDC.iSubStreamSelect = -1;
                adecParam._u.MS11_DDC.iUserBalanceAdj = 0;
                for (i = 0; i < 1; i++) {
                    adecParam._u.MS11_DDC.iDrcMode[i] = AMP_AUDIO_COMPMODE_ON;
                    adecParam._u.MS11_DDC.iDrcLow[i] = 0x40000000L;
                    adecParam._u.MS11_DDC.iDrcHigh[i] = 0x40000000L;
                }
                adecParam._u.MS11_DDC.iStereoMode = AMP_AUDIO_STEREO_STEREO;
                adecParam._u.MS11_DDC.iDualMode = AMP_AUDIO_DUALMODE_STEREO;
                adecParam._u.MS11_DDC.iKMode = AMP_AUDIO_KARAOKE_BOTH_VOCAL;
                adecParam._u.MS11_DDC.iChanMode = AMP_AUDIO_CHMAP_3_2_0;
                adecParam._u.MS11_DDC.iLfeMode = AMP_AUDIO_LFE_ON;
                adecParam._u.MS11_DDC.iDDMultiChEnable = TRUE;
#if ( BERLIN_CHIP_VERSION == BERLIN_BG4_CDP )
            }else if ( h->enableDDUDC ){
                int i;
                TEST_AVIN_PRINT("config adec with DD_UDC (DD_AC3/DD_PLUS)\n");
                config._u.pADEC.eAdecFmt = AMP_DD_UDC;
                adecParam._d = AMP_ADEC_PARAIDX_DDUDC;
                adecParam._u.DDUDC.iRunningMode = 2;
                adecParam._u.DDUDC.iInputMode = 0;
                adecParam._u.DDUDC.iAssocIdSelect = -1;
                adecParam._u.DDUDC.bDecErrConcealFlag = FALSE;
                adecParam._u.DDUDC.iDecErrConcealType = 0;
                adecParam._u.DDUDC.bCnvErrConcealFlag = FALSE;
                adecParam._u.DDUDC.bEvoHashFlag = FALSE;
                adecParam._u.DDUDC.bIsEvolutionQuickAccess = FALSE;
                adecParam._u.DDUDC.bJocForceDownmix = FALSE;
                adecParam._u.DDUDC.iMixerMode = FALSE;
                adecParam._u.DDUDC.iMixerUserPref = 0;
                for (i = 0; i < AMP_ADEC_INPORT_MAX; i++)
                {
                    adecParam._u.DDUDC.iCompMode[i] = AMP_AUDIO_COMPMODE_LINE;
                    adecParam._u.DDUDC.iDynScaleHigh[i] = 100;
                    adecParam._u.DDUDC.iDynScaleLow[i] = 100;
                    adecParam._u.DDUDC.iDualMode[i] = AMP_AUDIO_DUALMODE_STEREO;
                    adecParam._u.DDUDC.iChanMode[i] = AMP_AUDIO_CHMAP_3_2_2;
                    adecParam._u.DDUDC.iLfeMode[i] = AMP_AUDIO_LFE_ON;
                    adecParam._u.DDUDC.iPcmScale[i] = 100;
                    adecParam._u.DDUDC.iStereoMode[i] = AMP_AUDIO_STEREO_SRND;
                    adecParam._u.DDUDC.bMdctBandLimit[i] = FALSE;
                    adecParam._u.DDUDC.iDecOrrMode[i] = 0;
                    adecParam._u.DDUDC.iMisdSubstreamIdx[i] = -1;
                }
#endif
#if (BERLIN_CHIP_VERSION == BERLIN_VS680) || \
    (BERLIN_CHIP_VERSION == BERLIN_VS680_A0)
#ifdef CONFIG_AMP_IP_AUDIO_MS12_SUPPORTED
            } else if ( h->enableMS12 ){
                TEST_AVIN_PRINT("config adec with MS12 (DD_AC3/DD_PLUS)\n");
                config._u.pADEC.eAdecFmt = AMP_MS12_DDP;
                adecParam._d = AMP_ADEC_PARAIDX_MS12_DDP;
                adecParam._u.MS12_DDP.bDualInput = FALSE;
                adecParam._u.MS12_DDP.bDapNode = FALSE;
                adecParam._u.MS12_DDP.iDapMode = -1;
                adecParam._u.MS12_DDP.uiMlpOnly[0] = 0;
                adecParam._u.MS12_DDP.uiNumObject[0] = 16;
                adecParam._u.MS12_DDP.uiJocDecoding[0] = 1;
                adecParam._u.MS12_DDP.uiFilePlayback[0] = 0;
#endif
#endif
            }
        }
        break;

    case AMP_HE_AAC:
        TEST_AVIN_PRINT("config adec with HE_AAC\n");
        config._u.pADEC.eAdecFmt = AMP_HE_AAC;
        adecParam._d = AMP_ADEC_PARAIDX_AAC;
        break;

    case AMP_DTS:
        TEST_AVIN_PRINT("config adec with DTS\n");
#if ( BERLIN_CHIP_VERSION != BERLIN_BG4_CDP )
        config._u.pADEC.eAdecFmt = AMP_DTS;
        adecParam._d = AMP_ADEC_PARAIDX_DTS;
        break;
#else
        config._u.pADEC.eAdecFmt = AMP_DTS_DUMMY;
        adecParam._d = AMP_ADEC_PARAIDX_DTSDUMMY;
        break;

    case AMP_DTS_HD:
        TEST_AVIN_PRINT("config adec with DTS_HD\n");
        config._u.pADEC.eAdecFmt = AMP_DTS_DUMMY;
        adecParam._d = AMP_ADEC_PARAIDX_DTSDUMMY;
        break;
#endif
#ifdef CONFIG_AMP_IP_AUDIO_MS12_SUPPORTED
    case AMP_MS12_MAT:
        TEST_AVIN_PRINT("config adec with MS12_MAT\n");
        config._u.pADEC.eAdecFmt = AMP_MS12_MAT;
        adecParam._d = AMP_ADEC_PARAIDX_MS12_MAT;
        break;
#endif

    case AMP_RAW_PCM:
    default:
        TEST_AVIN_PRINT("config adec with RAW_PCM\n");
        config._u.pADEC.eAdecFmt = AMP_RAW_PCM;
        adecParam._d = AMP_ADEC_PARAIDX_RAWPCM;
        adecParam._u.PCM.uiSampleRate = h->audioParams.sampleRate;
        adecParam._u.PCM.uiPcmType = 16;
        adecParam._u.PCM.unChanMode = 8;
        adecParam._u.PCM.cLittleEndian = 0;
        adecParam._u.PCM.unChanNr = h->audioParams.channels;
        adecParam._u.PCM.uiInThresh = 0x800;
        adecParam._u.PCM.uiChanMask = 0;
        break;
    }
    AVIN_TEST_LOGD("config._u.pADEC.eAdecFmt = 0x%x\n",
            config._u.pADEC.eAdecFmt);

    AVIN_TEST_LOGD("set adec parameters.\n");
    AMP_RPC(r, AMP_ADEC_SetParameters, h->adec, adecParam._d, &adecParam);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("open adec.\n");
    AMP_RPC(r, AMP_ADEC_Open, h->adec, &config);
    assert(SUCCESS == r);

    r = test_adec_register_event(h->hListener, h->adec, TRUE, h);

    return r;
}

static HRESULT test_avin_createAdecComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("create adec component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_ADEC, 1, &h->adec);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_openArenComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;
    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_AREN;
    if (h->enableClk) {
        config._u.pAREN.uiInClkPortNum = 1;
    } else {
        config._u.pAREN.uiInClkPortNum = 0;
    }
    config._u.pAREN.uiInPcmPortNum = 1;
    config._u.pAREN.uiOutPcmPortNum = 1;
    config._u.pAREN.uiInSpdifPortNum = h->enable_passthr ? 1: 0;
    config._u.pAREN.uiOutSpdifPortNum = h->enable_passthr ? 1: 0;
    config._u.pAREN.uiInHdmiPortNum = h->enable_passthr ? 1: 0;
    config._u.pAREN.uiOutHdmiPortNum = h->enable_passthr ? 1: 0;
    config._u.pAREN.mode = AMP_TUNNEL;

    AVIN_TEST_LOGD("open aren.\n");
    AMP_RPC(r, AMP_AREN_Open, h->aren, &config);
    return r;
}

static HRESULT test_avin_setupArenComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter.\n");

    AVIN_TEST_LOGD("create aren component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_AREN, 1, &h->aren);
    assert(SUCCESS == r);

    r = test_avin_openArenComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_openClkComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_CONFIG config;
    AMP_CLK_PTS pts;
    AmpMemClear((VOID*) &config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_CLK;
    config._u.pCLK.mode = AMP_TUNNEL;
    config._u.pCLK.uiInputPortNum = 0;
    config._u.pCLK.uiOutputPortNum = 2;
    config._u.pCLK.uiNotifierNum = 0;
    config._u.pCLK.eClockSource = AMP_CLK_SRC_VPP;
    config._u.pCLK.eAVSyncPolicy = AMP_CLK_POLICY_LIVE_AVIN;

    AVIN_TEST_LOGD("open clk.\n");
    AMP_RPC(r, AMP_CLK_Open, h->clk, &config);
    assert(SUCCESS == r);

    AmpMemClear((VOID*) &pts, sizeof(AMP_CLK_PTS));
    // pts.m_uiLow = 0x10000000;
    // pts.m_uiHigh = 0x80000000;
    AVIN_TEST_LOGD("set clk stc.\n");
    AMP_RPC(r, AMP_CLK_SetSTC, h->clk, &pts);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("set clk rate.\n");
    AMP_RPC(r, AMP_CLK_SetClockRate, h->clk, 1000, 1000);
    assert(SUCCESS == r);
    return r;
}

static HRESULT test_avin_setupClkComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("create clk component.\n");
    AMP_RPC(r, AMP_FACTORY_CreateComponent, h->factory,
            AMP_COMPONENT_CLK, 1, &h->clk);
    assert(SUCCESS == r);

    r = test_avin_openClkComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_reconstructVencPipeline(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_INPUT_VIDEO_PARAMS *pVideoParams = NULL;
    UINT32 uNum;

    AVIN_TEST_LOGD("enter.\n");
    pHld = (TEST_AVIN_HLD *)pUserdata;
    pHld->playBackStopped = TRUE;

    AMP_RPC(r, AMP_AVIN_GetCurrentSource, pHld->avin, &(pHld->curSrc));
    assert(SUCCESS == r);

    pVideoParams = (AMP_AVIN_INPUT_VIDEO_PARAMS*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pVideoParams);

    AVIN_TEST_LOGD("set venc state to IDLE.\n");
    AMP_RPC(r, AMP_VENC_SetState, pHld->venc, AMP_IDLE);
    assert(SUCCESS == r);

    if (pHld->deint) {
        AVIN_TEST_LOGD("set deint state to IDLE.\n");
        AMP_RPC(r, AMP_DEINT_SetState, pHld->deint, AMP_IDLE);
        assert(SUCCESS == r);
        r = AMP_DisconnectComp(pHld->avin, pHld->uiVideoPort2, pHld->deint, 0);
        assert(SUCCESS == r);
        r = AMP_DisconnectComp(pHld->deint, 0, pHld->venc, 0);
        assert(SUCCESS == r);
    } else {
        r = AMP_DisconnectComp(pHld->avin, pHld->uiVideoPort2, pHld->venc, 0);
        assert(SUCCESS == r);
    }

    while ((AMP_BDCHAIN_GetItemNum(pHld->pStreamBDQ, &uNum) == SUCCESS) &&
            (uNum < MAX_OUTPUT_STREAM_NUM)) {
        AVIN_TEST_LOGD ("venc Stream Bufs back %d, max %d\n", uNum,
                MAX_OUTPUT_STREAM_NUM);
        usleep(50000);
    }
    r = test_avin_vencDeinitInstance(pHld);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("close venc.\n");
    AMP_RPC(r, AMP_VENC_Close, pHld->venc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("destroy venc.\n");
    AMP_RPC(r, AMP_VENC_Destroy, pHld->venc);
    assert(SUCCESS == r);

    AMP_FACTORY_Release(pHld->venc);
    assert(SUCCESS == r);

    if (pHld->deint && pVideoParams->videoProgressive) {
        AVIN_TEST_LOGD("close deint.\n");
        AMP_RPC(r, AMP_DEINT_Close, pHld->deint);
        assert(SUCCESS == r);

        AVIN_TEST_LOGD("destroy deint.\n");
        AMP_RPC(r, AMP_DEINT_Destroy, pHld->deint);
        assert(SUCCESS == r);

        AMP_FACTORY_Release(pHld->deint);
        assert(SUCCESS == r);

        pHld->deint = NULL;
    }

    AmpMemcpy(&pHld->videoParams, pVideoParams, sizeof(AMP_AVIN_INPUT_VIDEO_PARAMS));
    AVIN_TEST_LOGD ("colorFormat = 0x%x width = %d height = %d fr rate = %f prog = %c\n",
            pHld->videoParams.colorFormat, pHld->videoParams.videoWidth,
            pHld->videoParams.videoHeight, pHld->videoParams.videoRefrate, pHld->videoParams.videoProgressive? 'p':'i');

    // reconfigure
    pHld->playBackStopped = FALSE;
    if ((pHld->videoParams.videoWidth > 1920) ||
            (pHld->videoParams.videoHeight > 1080)) {
        pHld->width = 1920;
        pHld->height = 1080;
    } else {
        pHld->width = pHld->videoParams.videoWidth;
        pHld->height = pHld->videoParams.videoHeight;
    }

    if (pHld->videoParams.colorFormat == AMP_AVIN_COLOR_FORMAT_TYPE_YUV420)
        pHld->input_format = AMP_VENC_YUV420_SP;
    else if (pHld->videoParams.colorFormat == AMP_AVIN_COLOR_FORMAT_TYPE_YUV422)
        pHld->input_format = AMP_VENC_YUV422_UYVY;

    if (pHld->videoParams.videoProgressive) {
        pHld->fr_num = pHld->videoParams.videoRefrate;
    } else {
        pHld->fr_num = pHld->videoParams.videoRefrate/2;
    }

    r = test_avin_setupVencComponent(pHld);
    assert(SUCCESS == r);

    if (pHld->videoParams.videoProgressive) {
        r = AMP_ConnectComp(pHld->avin, pHld->uiVideoPort2, pHld->venc, 0);
        assert(SUCCESS == r);
    } else {
        if (!pHld->deint) {
            r = test_avin_setupDeintComponent(pHld);
            assert(SUCCESS == r);
        }
        r = AMP_ConnectComp(pHld->avin, pHld->uiVideoPort2, pHld->deint, 0);
        assert(SUCCESS == r);
        r = AMP_ConnectComp(pHld->deint, 0, pHld->venc, 0);
        assert(SUCCESS == r);
    }

    r = AMP_ConnectApp(pHld->venc, AMP_PORT_OUTPUT, 0, test_avin_vencCB, pHld);
    assert(SUCCESS == r);

    r = test_avin_vencInitInstance(pHld, AMP_NON_TUNNEL);
    assert(SUCCESS == r);

    if (pHld->deint) {
        AMP_RPC(r, AMP_DEINT_SetState, pHld->deint, AMP_EXECUTING);
        assert(SUCCESS == r);
    }

    AMP_RPC(r, AMP_VENC_SetState, pHld->venc, AMP_EXECUTING);
    assert(SUCCESS == r);

    MV_OSAL_Task_Create(&pgHld->insTask, test_avin_sendBufferTask,pgHld);
    MV_OSAL_Task_Sleep(20);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_reconstructAudioPipeline(AMP_IN TEST_AVIN_HLD *h)
{
    return 0;
    if (h->outputPlane == AMP_DISP_PLANE_PIP)
        return 0;
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    MV_OSAL_Mutex_Lock(h->mainTaskExitMutex);
    if (h->mainTaskExit) {
        AVIN_TEST_LOGD("Main task is exiting, no need to reconstruct audio pipeline\n");
        MV_OSAL_Mutex_Unlock(h->mainTaskExitMutex);
        return SUCCESS;
    }
    MV_OSAL_Mutex_Unlock(h->mainTaskExitMutex);
    MV_OSAL_Mutex_Lock(h->audioPipelineMutex);

    if (AMP_EXECUTING == test_avin_getAdecState(h)) {
        AVIN_TEST_LOGD("set adec state to IDLE.\n");
        AMP_RPC(r, AMP_ADEC_SetState, h->adec, AMP_IDLE);
        assert(SUCCESS == r);

        AVIN_TEST_LOGD("set aren state to IDLE.\n");
        AMP_RPC(r, AMP_AREN_SetState, h->aren, AMP_IDLE);
        assert(SUCCESS == r);

        r = AMP_DisconnectComp(h->avin, h->uiAudioPort, h->adec, 0);
        assert(SUCCESS == r);
        TEST_AVIN_PRINT("%s:%d h->clkenabled %d, h->enable_passthr %d\n",
            __FUNCTION__, __LINE__, h->clkenabled, h->enable_passthr);

        // disconnect all ports with adec
        if (TRUE == h->clkenabled) {
            r = AMP_DisconnectComp(h->adec, 0, h->aren, 1);
        } else {
            if (h->enable_passthr) {
                r = AMP_DisconnectComp(h->adec, h->AdecPort2ArenPcm, h->aren, h->ArenPort2AdecPcm);
                assert(r == SUCCESS);

                r = AMP_DisconnectComp(h->adec, h->AdecPort2ArenSpdif, h->aren,
                    h->ArenPort2AdecSpdif);
                assert(r == SUCCESS);

                r = AMP_DisconnectComp(h->adec, h->AdecPort2ArenHdmi, h->aren,
                    h->ArenPort2AdecHdmi);
                assert(r == SUCCESS);
            } else {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 0);
            }
        }
        assert(SUCCESS == r);
    }

    if (AMP_IDLE == test_avin_getAdecState(h)) {
        r = test_adec_register_event(h->hListener, h->adec, FALSE, h);
        AVIN_TEST_LOGD("close adec.\n");
        AMP_RPC(r, AMP_ADEC_Close, h->adec);
        assert(SUCCESS == r);
    }

    r = AMP_SND_StopTunnel(h->tunnelIdx[0]);
    assert(r == SUCCESS);
    if (h->enable_passthr) {
        r = AMP_SND_StopTunnel(h->tunnelIdx[1]);
        assert(r == SUCCESS);
        r = AMP_SND_StopTunnel(h->tunnelIdx[2]);
        assert(r == SUCCESS);
    }

    r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[0]); //PCM
    assert(SUCCESS == r);

    if (h->enable_passthr) {
        r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[1]); // spdif
        assert(r == SUCCESS);

        r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[2]); // hdmi
        assert(r == SUCCESS);
    }

    r = test_avin_openAdecComponent(h);
    assert(SUCCESS == r);

    if (TRUE == h->enableClk) {
        r = AMP_ConnectComp(h->adec, 0, h->aren, 1);
    } else {
        if (h->enable_passthr) {
            AMP_PORT_INFO PortInfo;
            int cnt;
            for (cnt = 0; cnt < 3; cnt++) {
                AMP_RPC(r, AMP_ADEC_QueryPort, h->adec, AMP_PORT_OUTPUT, cnt, &PortInfo);
                assert(r == SUCCESS);
                if (PortInfo.ePortType == AMP_PORT_ADEC_OUT_PCM){
                    h->AdecPort2ArenPcm = cnt;
                    continue;
                }
                if (PortInfo.ePortType == AMP_PORT_ADEC_OUT_SPDIF){
                    h->AdecPort2ArenSpdif = cnt;
                    continue;
                }
                if (PortInfo.ePortType == AMP_PORT_ADEC_OUT_HDMI){
                    h->AdecPort2ArenHdmi = cnt;
                    continue;
                }
            }
            for (cnt = 0; cnt < 3; cnt++) {
                AMP_RPC(r, AMP_AREN_QueryPort, h->aren, AMP_PORT_INPUT, cnt, &PortInfo);
                assert(r == SUCCESS);
                if (PortInfo.ePortType == AMP_PORT_AREN_IN_PCM){
                    h->ArenPort2AdecPcm = cnt;
                    continue;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_IN_SPDIF){
                    h->ArenPort2AdecSpdif = cnt;
                    continue;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_IN_HDMI){
                    h->ArenPort2AdecHdmi = cnt;
                    continue;
                }
            }

            r = AMP_ConnectComp(h->adec, h->AdecPort2ArenPcm, h->aren, h->ArenPort2AdecPcm);
            assert(r == SUCCESS);

            r = AMP_ConnectComp(h->adec, h->AdecPort2ArenSpdif, h->aren, h->ArenPort2AdecSpdif);
            assert(r == SUCCESS);

            r = AMP_ConnectComp(h->adec, h->AdecPort2ArenHdmi, h->aren, h->ArenPort2AdecHdmi);
            assert(r == SUCCESS);
        } else {
            r = AMP_ConnectComp(h->adec, 0, h->aren, 0);
        }
    }
    assert(SUCCESS == r);

    r = AMP_ConnectComp(h->avin, h->uiAudioPort, h->adec, 0);
    assert(SUCCESS == r);

    if (h->enable_passthr || h->enable_mixpassthr) {
        r = AMP_SND_SetSpdifFormat(AMP_SND_SPDIF_FORMAT_RAW_MAIN);
        assert(r == SUCCESS);

        r = AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT_RAW_MAIN);
        assert(r == SUCCESS);
    }
    else
    {
        r = AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT_PCM_MULTI);
        assert(r == SUCCESS);
    }


    if (h->enable_mixpassthr) {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
        if(h->enable_mixpassthr_mat) {
            r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                               &h->tunnelIdx[0]);
        }
        else if(h->enable_mixpassthr_ddp) {
            r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                           &h->tunnelIdx[0]);
        }
        else {
            if ((h->audioParams.audFormat == AMP_DD_AC3) ||
                    (h->audioParams.audFormat == AMP_DD_PLUS)) {
                r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                                   &h->tunnelIdx[0]);
            } else if (h->audioParams.audFormat == AMP_MS12_MAT) {
                r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                   &h->tunnelIdx[0]);
            } else {
                r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0,
                                         &h->tunnelIdx[0]);
                assert(r == SUCCESS);
                r = AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT_PCM_MULTI);
            }
        }
        assert(r == SUCCESS);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
    } else if (h->enable_passthr) {
        r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 0, &h->tunnelIdx[0]); //PCM
        assert(r == SUCCESS);

        r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 1, &h->tunnelIdx[1]); // spdif
        assert(r == SUCCESS);

        r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 2, &h->tunnelIdx[2]); // hdmi
        assert(r == SUCCESS);
    } else {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
        r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0,
                                     &h->tunnelIdx[0]);
#else
        r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 0,
                                     &h->tunnelIdx[0]); //PCM
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        assert(SUCCESS == r);
    }

    r = AMP_SND_StartTunnel(h->tunnelIdx[0]);
    assert(r == SUCCESS);

#if (( BERLIN_CHIP_VERSION == BERLIN_BG4_CDP ) || \
    (BERLIN_CHIP_VERSION == BERLIN_VS680) || \
    (BERLIN_CHIP_VERSION == BERLIN_VS680_A0))
    r = AMP_SND_SetSRCMode(AMP_APP_SRC_SAMPRATE_PRIPORT, 0);
    assert(r == SUCCESS);
#endif

    if (h->enable_passthr) {
        r = AMP_SND_StartTunnel(h->tunnelIdx[1]);
        assert(r == SUCCESS);
        r = AMP_SND_StartTunnel(h->tunnelIdx[2]);
        assert(r == SUCCESS);
    }


    AVIN_TEST_LOGD("set aren state to EXECUTING.\n");
    AMP_RPC(r, AMP_AREN_SetState, h->aren, AMP_EXECUTING);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("set adec state to EXECUTING.\n");
    AMP_RPC(r, AMP_ADEC_SetState, h->adec, AMP_EXECUTING);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(h->audioPipelineMutex);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_muteVideoAudio(TEST_AVIN_HLD *h, BOOL bMute)
{
    HRESULT r = SUCCESS;
    AMP_DISP_OUT_RES ResId = 0;
    BOOL bInput4K = FALSE, bOutput4K = FALSE;

    HRESULT result;
    if( bMute){
        TEST_AVIN_PRINT("Mute MAIN & 4K plane\n");
        AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, h->outputPlane, bMute);
#if ((BERLIN_CHIP_VERSION != BERLIN_BG4_CDP)&& \
     (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
     (BERLIN_CHIP_VERSION != BERLIN_VS680_A0))
        AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_4K_BYPASS, bMute);
#endif
    } else {
        AMP_RPC(r, AMP_DISP_OUT_GetResolution, h->voutDisp, AMP_DISP_TG_PRI, &ResId);
        AVIN_TEST_LOGD("AMP_DISP_OUT_GetResolution %d\n", ResId);
        bInput4K = (h->videoParams.videoWidth >= 3840)||(h->videoParams.videoHeight>= 2160);
        bOutput4K = (ResId >= AMP_DISP_OUT_RES_4Kx2K_MIN)&&(ResId < AMP_DISP_OUT_RES_4Kx2K_MAX);

#if ((BERLIN_CHIP_VERSION != BERLIN_BG4_CDP)&& \
     (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
     (BERLIN_CHIP_VERSION != BERLIN_VS680_A0))

        if((TRUE == bInput4K) && (TRUE == bOutput4K)) {
            TEST_AVIN_PRINT("Unmute 4K plane\n");
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_4K_BYPASS, bMute);

            /* FIXME!! below code can only be used in test application, because it will mute
               entire 2K video and graphics plane.
            */
            TEST_AVIN_PRINT("4K video, mute CPCB0\n");
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, 9, TRUE);
        } else
#endif
        {
            TEST_AVIN_PRINT("Unmute main plane\n");
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, h->outputPlane, bMute);

            /* FIXME!! below code can only be used in test application, because it will mute
               entire 2K video and graphics plane.
            */
#if ((BERLIN_CHIP_VERSION != BERLIN_BG4_CDP)&& \
     (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
     (BERLIN_CHIP_VERSION != BERLIN_VS680_A0))
            TEST_AVIN_PRINT("non-4K video, unmute CPCB0\n");

            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, 9, FALSE);
#endif
        }
    }
    return r;
}

static HRESULT test_avin_setDispScale(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_DISP_OUT_RES resId = AMP_DISP_OUT_RES_1080P60;
    AMP_DISP_WIN src = {0, 0, 0, 0}, desc = {0, 0, 0, 0};

#if ((BERLIN_CHIP_VERSION != BERLIN_BG4_CDP) && \
     (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
     (BERLIN_CHIP_VERSION != BERLIN_VS680_A0))
    if ((h->videoParams.videoWidth > 1920) ||
        (h->videoParams.videoHeight > 1080)) {
        return r;
    }
#endif
    src.iWidth = 0;  //Adaptive scale //h->videoParams.videoWidth;
    src.iHeight = 0; //Adaptive scale //h->videoParams.videoHeight;

    AMP_RPC(r, AMP_DISP_OUT_GetResolution, h->voutDisp, AMP_DISP_TG_PRI, &resId);
    switch (resId) {
        case AMP_DISP_OUT_RES_525P60:
            desc.iWidth = 720;
            desc.iHeight = 480;
            break;
#if ((BERLIN_CHIP_VERSION == BERLIN_BG4_CDP) || \
     (BERLIN_CHIP_VERSION == BERLIN_VS680) || \
     (BERLIN_CHIP_VERSION == BERLIN_VS680_A0))
        case AMP_DISP_OUT_RES_4Kx2KP50:
        case AMP_DISP_OUT_RES_4Kx2KP5994:
        case AMP_DISP_OUT_RES_4Kx2KP60:
        case AMP_DISP_OUT_RES_4Kx2KP30:
        case AMP_DISP_OUT_RES_4Kx2KP2997:
        case AMP_DISP_OUT_RES_4Kx2KP25:
        case AMP_DISP_OUT_RES_4Kx2KP24:
        case AMP_DISP_OUT_RES_4Kx2KP2398:
            desc.iWidth = 3840;
            desc.iHeight = 2160;
            break;
#endif
        case AMP_DISP_OUT_RES_480P60:
            desc.iWidth = 640;
            desc.iHeight = 480;
            break;
        case AMP_DISP_OUT_RES_VESA_800x600P60:
            desc.iWidth = 800;
            desc.iHeight = 600;
            break;
        case AMP_DISP_OUT_RES_VESA_1024x768P60:
            desc.iWidth = 1024;
            desc.iHeight = 768;
            break;
        case AMP_DISP_OUT_RES_CUSTOM_1280x800P60:
            desc.iWidth = 1280;
            desc.iHeight = 800;
            break;
        case AMP_DISP_OUT_RES_VESA_1280x1024P60:
            desc.iWidth = 1280;
            desc.iHeight = 1024;
            break;
        case AMP_DISP_OUT_RES_VESA_1360x768P60:
            desc.iWidth = 1360;
            desc.iHeight = 768;
            break;
        case AMP_DISP_OUT_RES_VESA_1440X900P60:
            desc.iWidth = 1440;
            desc.iHeight = 900;
            break;
        case AMP_DISP_OUT_RES_VESA_1600x1200P60:
            desc.iWidth = 1600;
            desc.iHeight = 1200;
            break;
        case AMP_DISP_OUT_RES_VESA_1680x1050P60:
            desc.iWidth = 1680;
            desc.iHeight = 1050;
            break;
        case AMP_DISP_OUT_RES_DSI_1920x1200P60:
            desc.iWidth = 1920;
            desc.iHeight = 1200;
            break;
        default:
            desc.iWidth = 1920;
            desc.iHeight = 1080;
            break;
    }

    if(h->outputPlane == AMP_DISP_PLANE_PIP) {
        desc.iX = desc.iWidth /2;
        desc.iY = desc.iHeight /2;
        desc.iWidth = desc.iX;
        desc.iHeight = desc.iY;
    }
    AMP_RPC(r, AMP_DISP_SetScale, h->voutDisp, h->outputPlane, &src, &desc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("according to output resolution (%d), "
            "set display scale as %dx%d\n", resId, desc.iWidth, desc.iHeight);
    return r;
}

static void cec_calculate_checksum(UINT8 *edid)
{
    int i;
    unsigned char chk_sum = 0;

    for(i = 0;i < EDID_BLK_SIZE - 1;i++)
        chk_sum += edid[i];

    chk_sum = 0x100 - chk_sum;
    edid[EDID_BLK_SIZE - 1] = chk_sum;
}

static HRESULT test_avin_set_cec_addr(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_DISP_HDMI_SINK_CAPS sinkCaps;
    int phyAddr;
    int i;
    AMP_AVIN_EDID_INFO EdidInfo;

    AmpMemClear(&EdidInfo, sizeof(AMP_AVIN_EDID_INFO));

    AMP_RPC(r, AMP_DISP_OUT_HDMI_GetSinkCaps, h->voutDisp, &sinkCaps);
    if(SUCCESS == r) {
        phyAddr = sinkCaps.uiCecPhyAddr;
        if ((phyAddr & 0xF000) == phyAddr)
            phyAddr |= 0x100;
        else if ((phyAddr & 0xFF00) == phyAddr)
            phyAddr |= 0x10;
        else if ((phyAddr & 0xFFF0) == phyAddr)
            phyAddr |= 0x1;
        else if ((phyAddr & 0xFFFF) == phyAddr)
            phyAddr = 0xFFFF;  //all positions filled
        AVIN_TEST_LOGD("Set CEC physical address 0x%X\n", phyAddr);

        EdidInfo.source = 1;
        memset(EdidInfo.data, 0 , EDID_DATA_SIZE);
        AMP_RPC(r, AMP_AVIN_ReadEdid, h->avin, &EdidInfo);
        EdidInfo.size = EDID_DATA_SIZE;

        for (i = 128; i < EDID_DATA_SIZE; ++i) {
            if (EdidInfo.data[i] == 0x03 && EdidInfo.data[i + 1] == 0x0C &&
                    EdidInfo.data[i + 2] == 0x00) {
                if((((EdidInfo.data[i+3] & 0xFF) << 8) | (EdidInfo.data[i+4] & 0xFF)) == phyAddr) {
                    AVIN_TEST_LOGD("CEC Physical address is same. not updating\n");
                    goto exit;
                }
                EdidInfo.data[i+3] = (phyAddr >> 8) & 0xFF;
                EdidInfo.data[i+4] = (phyAddr) & 0xFF;
                AVIN_TEST_LOGD("CEC physical address updated in EDID\n");
                break;
            }
        }
        if (i < EDID_DATA_SIZE) {       //Updated EDID calculare checksum
            cec_calculate_checksum(&EdidInfo.data[128]);
        }

        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 0);
        AMP_RPC(r, AMP_AVIN_UpdateEdid, h->avin, &EdidInfo);
        MV_OSAL_Task_Sleep(200);
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 1);

        if (SUCCESS == r) {
            TEST_AVIN_PRINT("Write %s EDID[%d] complete\n",
                test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
        } else {
            TEST_AVIN_PRINT("Write %s EDID[%d] error 0x%x\n",
                test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
        }
    } else {
        TEST_AVIN_PRINT("Sink not connected\n");
    }
exit:
    return r;
}

static HRESULT test_avin_procSrcConnectionChanged(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_CONNECT_STATUS_CHANGED_INFO *pSrcConnChangedInfo;
    AMP_AVIN_SOURCE_TYPE source;
    BOOL srcConnected;

    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;
    pSrcConnChangedInfo = (AMP_AVIN_CONNECT_STATUS_CHANGED_INFO*)
            AMP_EVENT_PAYLOAD_PTR(pEvent);

    srcConnected = pSrcConnChangedInfo->srcConnected;
    source = pSrcConnChangedInfo->source;
    if (FALSE == srcConnected) {
        pHld->srcStatus = AMP_AVIN_SOURCE_STATE_DISCONNECTED;
        AMP_RPC(r, AMP_DISP_SetPlaneMute, pHld->voutDisp, pHld->outputPlane, FALSE);
    }

    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        if( source == pHld->curSrc && FALSE == srcConnected){
            test_avin_muteVideoAudio(pHld, TRUE);
            AMP_RPC(r, AMP_DISP_Push_BuildInFrame, pHld->voutDisp, pHld->outputPlane,
                    AMP_DISP_BUILD_IN_FRAME_BLACK);
            assert(SUCCESS == r);
        }
    }
    TEST_AVIN_PRINT("source %s %s\n", test_avin_inputSourceString(source),
            srcConnected ? "CONNECTED" : "DISCONNECTED");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procSrcLockStatusChanged(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_LOCK_STATUS_CHANGED_INFO *pLockStatusChangedInfo;
    TEST_AVIN_HLD *pHld = NULL;

    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;
    pLockStatusChangedInfo = (AMP_AVIN_LOCK_STATUS_CHANGED_INFO*)
            AMP_EVENT_PAYLOAD_PTR(pEvent);

    pHld->srcStatus = pLockStatusChangedInfo->status;

    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        if( AMP_AVIN_SOURCE_STATE_STABLE != pHld->srcStatus){
            AMP_RPC(r, AMP_DISP_SetPlaneMute, pHld->voutDisp, pHld->outputPlane, TRUE);
            test_avin_muteVideoAudio(pHld, TRUE);
            AMP_RPC(r, AMP_DISP_Push_BuildInFrame, pHld->voutDisp, pHld->outputPlane,
                    AMP_DISP_BUILD_IN_FRAME_BLACK);
            assert(SUCCESS == r);

        } else {
            test_avin_muteVideoAudio(pHld, FALSE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, pHld->voutDisp, pHld->outputPlane, FALSE);
        }
    }

    TEST_AVIN_PRINT("lock status changed to %s\n",
            test_avin_inputStateString(pHld->srcStatus));
    if (AMP_AVIN_SOURCE_STATE_STABLE == pHld->srcStatus)
        test_avin_getavinfo(pHld, NULL);
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procAudioChanged(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_INPUT_AUDIO_PARAMS *pAudioParams = NULL;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    pAudioParams = (AMP_AVIN_INPUT_AUDIO_PARAMS*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pAudioParams);

    AmpMemcpy(&(pHld->audioParams), pAudioParams, sizeof(AMP_AVIN_INPUT_AUDIO_PARAMS));
    test_avin_dumpAudioInfo(pHld, pAudioParams);

    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        #ifndef DISABLE_HDMIRX_AUDIO
            r = test_avin_reconstructAudioPipeline(pHld);
        #endif
        assert(SUCCESS == r);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procVideoChanged(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_INPUT_VIDEO_PARAMS *pVideoParams = NULL;

    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    AMP_RPC(r, AMP_AVIN_GetCurrentSource, pHld->avin, &(pHld->curSrc));
    assert(SUCCESS == r);

    pVideoParams = (AMP_AVIN_INPUT_VIDEO_PARAMS*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pVideoParams);

    AmpMemcpy(&pHld->videoParams, pVideoParams, sizeof(AMP_AVIN_INPUT_VIDEO_PARAMS));
    test_avin_dumpVideoInfo(pHld, &(pHld->videoParams));

    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        r = test_avin_setDispScale(pHld);
        assert(SUCCESS == r);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procAVMuteChanged(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_AVMUTE_CHANGED_INFO *pAvMuteChangedInfo = NULL;

    pHld = (TEST_AVIN_HLD *)pUserdata;
    pAvMuteChangedInfo = (AMP_AVIN_AVMUTE_CHANGED_INFO*)
            AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pAvMuteChangedInfo);

    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        if (TRUE == pAvMuteChangedInfo->avMute) {
            test_avin_muteVideoAudio(pHld, pAvMuteChangedInfo->avMute);
            AMP_RPC(r, AMP_DISP_Push_BuildInFrame, pHld->voutDisp, pHld->outputPlane,
                    AMP_DISP_BUILD_IN_FRAME_BLACK);
            assert(SUCCESS == r);
        } else if (AMP_AVIN_SOURCE_STATE_STABLE == pHld->srcStatus) {
            test_avin_muteVideoAudio(pHld, pAvMuteChangedInfo->avMute);
        }
    }
    TEST_AVIN_PRINT("AVMUTE changed to %d, status is %s\n", pAvMuteChangedInfo->avMute,
        test_avin_inputStateString(pHld->srcStatus));

    return r;
}

static HRESULT test_avin_procHdmiGmdPkt(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_HDMI_PKT_GMD *pHDMIGMDPkt = NULL;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    pHDMIGMDPkt = (AMP_AVIN_HDMI_PKT_GMD*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pHDMIGMDPkt);
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procStopHdmiGmdPkt(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procJobCompleted(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_COMPLETED_JOB_INFO *pJobInfo = NULL;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    pJobInfo = (AMP_AVIN_COMPLETED_JOB_INFO*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    if (NULL == pJobInfo) {
        AVIN_TEST_LOGD("can not get the job information from event payload!\n");
        return ERR_SWMODEWRONG;
    }

    if (AMP_AVIN_VGA_AUTO_ADJUSTMENT_COMPLETED == pJobInfo->jobType) {
        AMP_AVIN_VGA_VIDEO_PROP vgaParams;
        AmpMemClear((VOID*) &vgaParams, sizeof(AMP_AVIN_VGA_VIDEO_PROP));
        AMP_RPC(r, AMP_AVIN_GetVgaAutoAdjustment, pHld->avin, &vgaParams);
        if (SUCCESS == r) {
            TEST_AVIN_PRINT("VGA auto adjust finished: phase = %d, x = %d, y = %d\n",
                    vgaParams.ulPhase, vgaParams.xshift, vgaParams.yshift);
        } else {
            AMPLOGE("failed to call AMP_AVIN_GetVgaAutoAdjustment, r = 0x%x\n", r);
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_procSifStsChanged(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_SIF_STATUS *pSifStatus = NULL;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    pSifStatus = (AMP_AVIN_SIF_STATUS*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    if (NULL == pSifStatus) {
        AVIN_TEST_LOGD("can not get the sif status from event payload!\n");
        return ERR_SWMODEWRONG;
    }
    AmpMemcpy(&pHld->sifStatus, pSifStatus, sizeof(AMP_AVIN_SIF_STATUS));
    TEST_AVIN_PRINT("%17s: %#x (%s)\n", "Standard", pSifStatus->standard,
            test_avin_audioSifStandardString(pSifStatus->standard));
    TEST_AVIN_PRINT("%17s: %#x (%s)\n", "Mode", pSifStatus->mode,
            test_avin_audioSifModeString(pSifStatus->mode));
    TEST_AVIN_PRINT("%17s: %#x (%s)\n", "Supported Modes", pSifStatus->uiSupportedModes,
            test_avin_audioSifSupportedModesString(pSifStatus->uiSupportedModes));

    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        if (TRUE == pHld->bDetSifStd &&
                pSifStatus->mode == AMP_AVIN_AUD_SIF_MODE_UNKNOWN &&
                pSifStatus->uiSupportedModes == 0) {
            TEST_AVIN_PRINT("Set SIF Standard as 0x%x (%s)\n",
                    pSifStatus->standard,
                    test_avin_audioSifStandardString(pSifStatus->standard));
            AMP_RPC(r, AMP_AVIN_SetSifStandard, pHld->avin, pSifStatus->standard);
            if (SUCCESS != r) {
                AMPLOGE("failed to call AMP_AVIN_SetSifStandard. r = 0x%x\n", r);
            } else {
                pHld->bDetSifStd = FALSE;
            }
        }

        if (FALSE == pHld->bDetSifStd &&
                AMP_AVIN_AUD_SIF_MODE_UNKNOWN != pSifStatus->uiSupportedModes) {
            UINT32 uiSupportedModes = pSifStatus->uiSupportedModes;
            AMP_AVIN_AUD_SIF_MODE sifMode = AMP_AVIN_AUD_SIF_MODE_UNKNOWN;

            if (AMP_AVIN_SIF_STD_MONO45 == pSifStatus->standard) {
                if (AMP_AVIN_AUD_SIF_MODE_MONO & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_MONO;
                } else if (AMP_AVIN_AUD_SIF_MODE_STEREO & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_STEREO;
                } else if (AMP_AVIN_AUD_SIF_MODE_SAP & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_SAP_MONO;
                } else if (AMP_AVIN_AUD_SIF_MODE_FM_MONO & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_FM_MONO;
                }
            } else {
                if (AMP_AVIN_SIF_STD_MONO55 == pSifStatus->standard ||
                    AMP_AVIN_SIF_STD_MONO60 == pSifStatus->standard) {
                    if (AMP_AVIN_AUD_SIF_MODE_FM_MONO & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_FM_MONO;
                    }
                } else if (AMP_AVIN_SIF_STD_MONO65 == pSifStatus->standard) {
                    if (AMP_AVIN_AUD_SIF_MODE_A2_MONO & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_A2_MONO;
                    } else if (AMP_AVIN_AUD_SIF_MODE_A2_STEREO & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_A2_STEREO;
                    } else if (AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO1 & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO1;
                    } else if (AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2 & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2;
                    } else if (AMP_AVIN_AUD_SIF_MODE_FM_MONO & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_FM_MONO;
                    }
                } else if (AMP_AVIN_SIF_STD_MONO65_AML == pSifStatus->standard) {
                    if (AMP_AVIN_AUD_SIF_MODE_AM_MONO & uiSupportedModes) {
                        sifMode = AMP_AVIN_AUD_SIF_MODE_AM_MONO;
                    }
                }

                if (AMP_AVIN_AUD_SIF_MODE_NICAM_MONO & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_NICAM_MONO;
                } else if (AMP_AVIN_AUD_SIF_MODE_NICAM_STEREO & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_NICAM_STEREO;
                } else if (AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1 & uiSupportedModes
                        || AMP_AVIN_AUD_SIF_MODE_NICAM_MONO_DATA352 & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1;
                } else if (AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO2 & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO2;
                } else if (AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1_MONO2 & uiSupportedModes) {
                    sifMode = AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1_MONO2;
                }
            }

            TEST_AVIN_PRINT("Set SIF Mode as 0x%x (%s)\n",
                    sifMode, test_avin_audioSifModeString(sifMode));
            AMP_RPC(r, AMP_AVIN_SetSifMode, pHld->avin, sifMode);
            if (SUCCESS != r) {
                AMPLOGE("failed to set SIF mode as: %s\n",
                        test_avin_audioSifModeString(sifMode));
            }
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return SUCCESS;
}

static HRESULT avin_adec_event_handler(AMP_EVENT *pEvent, VOID *pUserData)
{
    AMP_ADEC_STRMINFO_EVENT *pStreamInfo = NULL;
    UINT32 *payload = AMP_EVENT_PAYLOAD_PTR(pEvent);

    if (AMP_EVENT_ADEC_CALLBACK_STRMINFO == pEvent->stEventHead.eEventCode){
        pStreamInfo = (AMP_ADEC_STRMINFO_EVENT *)AMP_EVENT_PAYLOAD_PTR(pEvent);
        TEST_AVIN_PRINT("Fmt = %u, PriChanMode=%u, ChNum = %u, LfeMode = %u  bAtmos = %d\n",
            pStreamInfo->uiStreamFmt, pStreamInfo->uiPriChanMode,
            pStreamInfo->uiChannelNum, pStreamInfo->uiLfeMode, pStreamInfo->bAtmos);
    }

    return SUCCESS;

}

static HRESULT avin_hdmitx_event_handler(AMP_EVENT *pEvent, VOID *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    UINT32 *payload = AMP_EVENT_PAYLOAD_PTR(pEvent);
    pHld = (TEST_AVIN_HLD *)pUserData;

    if (AMP_DISP_EVENT_HDMI_SINK_CONNECTED == *payload) {
        test_avin_set_cec_addr(pHld);
    }

    return SUCCESS;
}

static HRESULT avin_hdmi_hdr_event_handler(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_HDMI_PKT_HDR_INFOFRM *pHdrInfo = NULL;
    int i;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    pHdrInfo = (AMP_AVIN_HDMI_PKT_HDR_INFOFRM*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pHdrInfo);

    AVIN_TEST_LOGD(" uiEOTF = %d, \n", pHdrInfo->uiEOTF);
    for (i = 0; i < 3; i++) {
                AVIN_TEST_LOGD(" x[%d]: %u\n", i, pHdrInfo->uidisplay_primaries_x[i]);
                AVIN_TEST_LOGD(" y[%d]: %u\n", i, pHdrInfo->uidisplay_primaries_y[i]);
        }
    AVIN_TEST_LOGD(" white point: x %u, y %u\n",
                        pHdrInfo->uiwhite_point_x, pHdrInfo->uiwhite_point_y);
    AVIN_TEST_LOGD(" max display mastering luminance: %u\n", pHdrInfo->uimax_display_mastering_lumi);
    AVIN_TEST_LOGD(" min display mastering luminance: %u\n", pHdrInfo->uimin_display_mastering_lumi);
    AVIN_TEST_LOGD(" maximum content light level: %u\n", pHdrInfo->uiMaximum_Content_Light_Lvl);
    AVIN_TEST_LOGD(" maximum frame-average light level: %u\n", pHdrInfo->uiMaximum_Frame_Average_Light_Lvl);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

#if (BERLIN_CHIP_VERSION == BERLIN_VS680_A0)
static HRESULT avin_hdmi_dv_event_handler(AMP_IN AMP_EVENT *pEvent,
        AMP_IN void *pUserdata)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = NULL;
    AMP_AVIN_HDMI_DV_INFO *pDVInfo = NULL;
    AVIN_TEST_LOGD("enter.\n");

    pHld = (TEST_AVIN_HLD *)pUserdata;

    pDVInfo = (AMP_AVIN_HDMI_DV_INFO*)AMP_EVENT_PAYLOAD_PTR(pEvent);
    assert(NULL != pDVInfo);

    AVIN_TEST_LOGD(" colorFormat = %d, uiDvEnable = %d\n",
                    pDVInfo->colorFormat, pDVInfo->uiDvEnable);
    return r;
}
#endif

static HRESULT test_avin_event_callback(
    AMP_IN HANDLE hListener,
    AMP_IN AMP_EVENT *pEvent, AMP_IN void *pUserdata)
{
    HRESULT rc = SUCCESS;

    AVIN_TEST_LOGD("enter.\n");
    if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VOUT ||
            pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT ||
            pgHld->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {

        TEST_AVIN_PRINT("\nreceived callback: %s\n",
            test_avin_eventString(AMP_EVENT_GETCODE(*pEvent)));

        if (AMP_EVENT_API_AVIN_SRC_CONNECTION_CHANGED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procSrcConnectionChanged(pEvent, pUserdata);
            assert(SUCCESS == rc);
        }else if (AMP_EVENT_API_AVIN_LOCK_STATUS_CHANGED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procSrcLockStatusChanged(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_AUDIO_CHANGED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procAudioChanged(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_VIDEO_CHANGED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procVideoChanged(pEvent, pUserdata);
            assert(SUCCESS == rc);
            if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
                rc = test_avin_reconstructVencPipeline(pEvent, pUserdata);
                assert(SUCCESS == rc);
            }
        } else if (AMP_EVENT_API_AVIN_AVMUTE_CHANGED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procAVMuteChanged(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_HDMI_GMD_PKT ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procHdmiGmdPkt(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_HDMI_GMD_PKT_STOP ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procStopHdmiGmdPkt(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_JOB_COMPLETED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procJobCompleted(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_SIF_STATUS_CHANGED ==
                AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_procSifStsChanged(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_ADEC_CALLBACK_STRMINFO == AMP_EVENT_GETCODE(*pEvent)) {
            rc = avin_adec_event_handler(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_DISP_CALLBACK_HDMI == AMP_EVENT_GETCODE(*pEvent)) {
            rc = avin_hdmitx_event_handler(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else if (AMP_EVENT_API_AVIN_HDMI_HDR_INFO == AMP_EVENT_GETCODE(*pEvent)) {
            rc = avin_hdmi_hdr_event_handler(pEvent, pUserdata);
            assert(SUCCESS == rc);
        }
#if (BERLIN_CHIP_VERSION == BERLIN_VS680_A0)
        else if (AMP_EVENT_API_AVIN_HDMI_DV_CHANGED == AMP_EVENT_GETCODE(*pEvent)) {
            rc = avin_hdmi_dv_event_handler(pEvent, pUserdata);
            assert(SUCCESS == rc);
        }
#endif
        else {
            TEST_AVIN_PRINT("received a wrong event: 0x%x\n",
                    AMP_EVENT_GETCODE(*pEvent));
        }
    } else if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
        if (AMP_EVENT_API_AVIN_VIDEO_CHANGED == AMP_EVENT_GETCODE(*pEvent)) {
            rc = test_avin_reconstructVencPipeline(pEvent, pUserdata);
            assert(SUCCESS == rc);
        } else {
            TEST_AVIN_PRINT("received a wrong event: 0x%x\n",
                    AMP_EVENT_GETCODE(*pEvent));
        }
    }

    AVIN_TEST_LOGD("leave, rc = 0x%x\n", rc);
    return rc;
}

static HRESULT test_avin_dumpVbiData(AMP_IN AMP_BD_HANDLE hBufDesc)
{
    HRESULT r = SUCCESS;
    AMP_BDTAG_MEMINFO *pVbiMemInfo = NULL;
    VOID *pAddress = NULL;
    FILE *fp = NULL;

    AVIN_TEST_LOGD("enter\n");

    assert(NULL != hBufDesc);
    AVIN_TEST_LOGD("hBufDesc = %p, hBufDesc->uiBDId = 0x%x\n", hBufDesc,
            hBufDesc->uiBDId);

    r = AMP_BDTAG_GetWithIndex(hBufDesc, 0, (VOID**) &pVbiMemInfo);
    assert(SUCCESS == r);
    assert(NULL != pVbiMemInfo);

    if (AMP_BDTAG_ASSOCIATE_MEM_INFO == ((AMP_BDTAG_H *)pVbiMemInfo)->eType) {
        AVIN_TEST_LOGD("pVbiMemInfo->uMemOffset = %d, pVbiMemInfo->uSize = %d!\n",
                pVbiMemInfo->uMemOffset, pVbiMemInfo->uSize);

        r = AMP_SHM_Ref(pVbiMemInfo->uMemHandle, NULL);
        if (SUCCESS != r) {
            AVIN_TEST_LOGD("leave ref shmHanle 0x%x failed r = 0x%x\n", pVbiMemInfo->uMemHandle, r);
            return r;
        }
        r = AMP_SHM_GetVirtualAddress(pVbiMemInfo->uMemHandle,
                pVbiMemInfo->uMemOffset, &pAddress);

        if (NULL != pAddress) {
            AVIN_TEST_LOGD("write data to file: %s, size = %d\n",
                    VBI_DATA_FILE, pVbiMemInfo->uSize);
            fp = fopen(VBI_DATA_FILE, "ab+");
            if(NULL != fp) {
                fwrite(pAddress, pVbiMemInfo->uSize, 1, fp);
                fclose(fp);
            }
        } else {
            AVIN_TEST_LOGD("pAddress is NULL!\n");
        }
        r = AMP_SHM_Unref(pVbiMemInfo->uMemHandle);
    } else {
        AVIN_TEST_LOGD("it's not the vbi data!\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static const char* test_avin_convert2HDMIPortID(AMP_IN TEST_AVIN_HLD *h,
    AMP_AVIN_SOURCE_TYPE source)
{
    int i = 0;
    int video = -1;
    HRESULT r = SUCCESS;
    static char videoInputName[50] = {0};
    AMP_AVIN_PLATFORM_CONFIG pltConfig;

    AMP_RPC(r, AMP_AVIN_GetPlatformConfig, h->avin, &pltConfig);

    for(i = AMP_AVIN_SOURCE_TYPE_INVALID; i < AMP_AVIN_SOURCE_TYPE_MAX_NUM; i++){
        if( pltConfig.inputMux[i].source == source){
            break;
        }
    }

    if( i == AMP_AVIN_SOURCE_TYPE_MAX_NUM ){
        strcpy(videoInputName, "NONE");
    }

    if( test_avin_isHdmisource(source)) {
        strcpy(videoInputName, pltConfig.inputMux[i].videoInputName);
    }

    return videoInputName;
}

static UINT8 * test_avin_get_videopreffilename(AMP_IN TEST_AVIN_HLD *h)
{
    static char name[255];
    char strsource[255];
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime( &rawtime );

    if( test_avin_isHdmisource( h->curSrc))
    {
        sprintf(strsource, "%s", test_avin_convert2HDMIPortID(h, h->curSrc));
    }
    else
    {
        sprintf(strsource, "%s", test_avin_inputSourceString(h->curSrc));
    }

    sprintf(name, "%dx%d%c_%s_%s_%s_%02d%02d%02d",
                        h->videoParams.videoWidth,
                        h->videoParams.videoHeight,
                        h->videoParams.videoProgressive? 'p':'i',
                        test_avin_bitDepthString_Ex(h->videoParams.outputColorDepth),
                        test_avin_colorSpaceString_Ex(h->videoParams.outputColorFormat),
                        strsource,
                        timeinfo->tm_hour,
                        timeinfo->tm_min,
                        timeinfo->tm_sec);
    return name;
}

static UINT8 * test_avin_get_audiopreffilename(AMP_IN TEST_AVIN_HLD *h)
{
    static char name[255];
    char strsource[255];
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime( &rawtime );

    if( test_avin_isHdmisource(h->curSrc))
    {
        sprintf(strsource, "%s_%s_",
            test_avin_inputSourceString(h->curSrc),
            test_avin_convert2HDMIPortID(h, h->curSrc));
    }
    else
    {
        sprintf(strsource, "%s", test_avin_inputSourceString(h->curSrc));
    }

    sprintf(name, "%s_%s_%dk_%d_%d_%02d%02d%02d",
                        strsource,
                        test_avin_audioFormatString(h->audioParams.audFormat),
                        h->audioParams.sampleRate/1000,
                        h->audioParams.channels,
                        h->audioParams.bitDepth,
                        timeinfo->tm_hour,
                        timeinfo->tm_min,
                        timeinfo->tm_sec);
    return name;
}

static UINT8* test_avin_getUSBDir(AMP_IN UINT8 *usbDir)
{
    UINT8 *devPrefix = "/dev/block";
    UINT8 *mntPrefix = "/mnt/media";
    BOOL bIsFound = FALSE;
    FILE *fp;
    UINT8 buf[1024];
    UINT8 *mntPath;
    UINT8 *p;

    AVIN_TEST_LOGD("enter\n");

    if (NULL == usbDir) {
        return NULL;
    }

    fp = fopen("/proc/mounts", "r");
    if (NULL == fp) {
        return NULL;
    }
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), fp);
        p = buf;
        /* Parse device name field */
        if (strncmp(p, devPrefix, strlen(devPrefix)) == 0) {
            while (*p != ' ')
                ++p;
            ++p;
        } else {
            continue;
        }
        /* Parse mount path field */
        if (strncmp(p, mntPrefix, strlen(mntPrefix)) == 0) {
            mntPath = p;
            while (*p != ' ')
                ++p;
            *p = '\0';
            ++p;
        } else {
            continue;
        }
        /* Skip fs type field */
        while (*p != ' ')
            ++p;
        ++p;
        /* Parse option field */
        if (strncmp(p, "rw", 2) == 0) {
            TEST_AVIN_PRINT("USB disk `%s' found.\n", mntPath);
            strcpy(usbDir, mntPath);
            bIsFound = TRUE;
            break;
        } else {
            TEST_AVIN_PRINT("USB disk `%s' is read only.\n", mntPath);
        }
    }
    fclose(fp);

    if( FALSE == bIsFound ){
        TEST_AVIN_PRINT("USB disk is not found, if you want to dump file to "
                "USB disk, please insert USB disk and try again.\n");
    }
    AVIN_TEST_LOGD("leave\n");
    return (TRUE == bIsFound ? usbDir : NULL);
}

static UINT8 CC_HEAD[]={0xF8, 0x03, 0xFE, 0x80, 0x3F, 0xE0, 0x0F, 0xF8,
                                     0x03, 0xFE,0x80, 0x3F, 0x00, 0x00, 0x00, 0xFC, 0xFF};
static UINT8 TT_HEAD[]={0x55, 0x55, 0x27};
static UINT8 WSS_HEAD[]={0x01, 0x1F, 0xC7, 0x71, 0x1C, 0x8F, 0x07, 0x1F};

static char CC_string[]="[CC01]";
static char TT_string[]="[TTX ]";
static char WSS_string[]="[WSS ]";
#define ONE_FIELD_BUF_SIZE      1024
#define ERP_LINE_SIZE    64
#define VBI_NTSC_CC_TYPE        1
#define VBI_TELETEXT_TYPE       2
#define VBI_PAL_WSS_TYPE        3
#define VBI_PAL_CC_TYPE         4
#define VBI_NTSC_CGMS_TYPE    5

static void vbi_CCEprFormat(UINT8* inbuf, UINT8* outbuf, UINT8 field)
{
    UINT16 lineNum = inbuf[0];
    UINT16 CCData;
    UINT16 index = 152;
    UINT8 i,j,oversample;

    if(field == 0)
    {
        lineNum += 263;
    }
    memset(outbuf, 0, ERP_LINE_SIZE);
    outbuf[0] = (UINT8)lineNum;
    outbuf[1] = (UINT8)(((UINT8)(lineNum>>8))<<1);
    memcpy(&outbuf[2], CC_HEAD, sizeof(CC_HEAD));

    CCData = inbuf[3]|((UINT16)inbuf[4]<<8);
    for(i=0; i<16; i++)
    {
        if((i == 2)||(i == 6)||(i == 11)||(i == 15))
        {
            oversample = 13;
        }
        else
        {
            oversample = 14;
        }
        if(CCData&(1<<i))
        {
            for(j=0; j<oversample; j++)
            {
                outbuf[index/8] |= 1<<(index%8);
                index++;
            }
        }
        else
        {
            index += oversample;
        }
    }
    memcpy(&outbuf[51], CC_string, sizeof(CC_string));
}

static void vbi_TTEprFormat(UINT8* inbuf, UINT8* outbuf, UINT8 field)
{
    UINT16 lineNum = inbuf[0];

    if(field == 0)
    {
        lineNum += 313;
    }
    memset(outbuf, 0, ERP_LINE_SIZE);
    outbuf[0] = (UINT8)lineNum;
    outbuf[1] = (UINT8)(((UINT8)(lineNum>>8))<<1);
    memcpy(&outbuf[2], TT_HEAD, sizeof(TT_HEAD));
    memcpy(&outbuf[5], &inbuf[3], inbuf[2]);
    memcpy(&outbuf[51], TT_string, sizeof(TT_string));
}

static UINT32 vbi_DataRotate(UINT32 value, UINT32 nbits)
{

    unsigned int rot_value = 0;
    unsigned int i;

    while(nbits) {

        rot_value |= (value & 0x01);
        nbits--;
        if(nbits) {
            rot_value <<= 1;
            value >>= 1;
        }
    }

    return rot_value;
}

static UINT32 vbi_WSSDecode(UINT8* inbuf)
{
    UINT32 WssData = 0;

    inbuf[0] = vbi_DataRotate(inbuf[0], 8);
    inbuf[1] = vbi_DataRotate(inbuf[1], 8);
    inbuf[2] = vbi_DataRotate(inbuf[2], 8);
    inbuf[0] ^= 0xFF;
    inbuf[1] ^= 0xFF;
    inbuf[2] ^= 0xFF;
    WssData = ((UINT32)(inbuf[0]&0x3) << 12) | ((UINT32)(inbuf[1]) << 4) | ( (inbuf[2]&0xF0) >> 4);
    WssData = vbi_DataRotate(WssData, 14);

    TEST_AVIN_PRINT("WSS data =  %x\n",  WssData);
    return WssData;
}
static void vbi_WSSEprFormat(UINT8* inbuf, UINT8* outbuf)
{
    UINT32 WssData = 0;
    UINT8 index = 69;
    UINT8 i, j;
    memset(outbuf, 0, ERP_LINE_SIZE);
    outbuf[0] =  inbuf[0];
    memcpy(&outbuf[1], WSS_HEAD, sizeof(WSS_HEAD));
    WssData = vbi_WSSDecode(&inbuf[3]);

    for(i=0; i<14; i++)
    {
        if((WssData&(1<<i)) == 0)
        {
            index += 3;
        }
        for(j=0; j<3; j++)
        {
            outbuf[index/8] |= 1<<(index%8);
            index++;
        }
        if(WssData&(1<<i))
        {
            index += 3;
        }
    }

    memcpy(&outbuf[51], WSS_string, sizeof(WSS_string));
}

static UINT32 vbi_EprFormat(UINT8* inbuf, UINT8* outbuf)
{
    UINT8* InputLineData = inbuf;
    UINT8* OutputLineData = outbuf;
    UINT16 InOffset, OutOffset, lineNum;
    static UINT32 fieldID = 0;

    InOffset = 0;
    OutOffset = 0;
    fieldID++;
    while(InOffset < ONE_FIELD_BUF_SIZE)
    {
        if(InputLineData[0] == 0)
        {
            return OutOffset;
        }
        if((InputLineData[0] == 21)&&(inbuf[ONE_FIELD_BUF_SIZE-3] == VBI_NTSC_CC_TYPE))//NTSC CC
        {
            vbi_CCEprFormat(InputLineData, OutputLineData, inbuf[ONE_FIELD_BUF_SIZE-1]);
        }
        else if((InputLineData[0] == 23)
            &&(inbuf[ONE_FIELD_BUF_SIZE-2] == VBI_PAL_WSS_TYPE)
            &&(inbuf[ONE_FIELD_BUF_SIZE-1] == 1))//WSS
        {
            vbi_WSSEprFormat(InputLineData, OutputLineData);
        }
        else//TT
        {
            vbi_TTEprFormat(InputLineData, OutputLineData, inbuf[ONE_FIELD_BUF_SIZE-1]);
        }
        //add field count
        OutputLineData[57] = (UINT8)(fieldID>>24);
        OutputLineData[58] = (UINT8)(fieldID>>16);
        OutputLineData[59] = (UINT8)(fieldID>>8);
        OutputLineData[60] = (UINT8)fieldID;

        InOffset += InputLineData[2]+3;
        InputLineData = inbuf+InOffset;
        OutOffset += ERP_LINE_SIZE;
        OutputLineData = outbuf+OutOffset;
    }

    return OutOffset;
}

static void vbi_Raw2Epr(UINT8 *path)
{
    UINT8 EprPath[1024];
    UINT8 EprInBuf[ONE_FIELD_BUF_SIZE];
    UINT8 EprOutBuf[ONE_FIELD_BUF_SIZE];
    UINT32 pathsize, datasize;
    FILE *fpraw;
    FILE *fpepr;

    pathsize= strlen((char*)path);
    memcpy(EprPath, path, pathsize);
    strcpy(EprPath+pathsize-3, "epr");

    if((fpraw=fopen(path,"r")) == NULL)
    {
        TEST_AVIN_PRINT("Raw file open failed\n");
        return;
    }

    if ((fpepr = fopen(EprPath, "wb+")) == NULL)
    {
         TEST_AVIN_PRINT("EPR file creat failed\n");
         fclose(fpraw);
         return;
    }

    while(fread(EprInBuf, 1, ONE_FIELD_BUF_SIZE, fpraw) == ONE_FIELD_BUF_SIZE)
    {
        datasize = vbi_EprFormat(EprInBuf, EprOutBuf);
        if(fwrite(EprOutBuf, 1, datasize, fpepr) != datasize)
        {
            system("sync");
            fclose(fpraw);
            fclose(fpepr);
            TEST_AVIN_PRINT("EPR file write failed\n");
            return;
        }
    }
    system("sync");
    fclose(fpraw);
    fclose(fpepr);
    TEST_AVIN_PRINT("Convert RAW VBI data to %s - done\n",  EprPath);

}

static HRESULT test_avin_dumpFrames(AMP_IN TEST_AVIN_HLD *h,
        AMP_IN UINT32 dumpId, UINT8 *path)
{
    HRESULT r = SUCCESS;
    UINT8 *defaultPath = "/data";
    UINT8 filePath[1024] = {0};
    AMP_AVIN_DUMP_INFO dumpInfo;
    FILE *fp;
    UINT8 ch = 0;
    UINT8 dumpType[36];

    AVIN_TEST_LOGD("enter\n");

    if( NULL == path){
        if (NULL == test_avin_getUSBDir(filePath)) {
            strcpy(filePath, defaultPath);
        }
    }

    if( AMP_AVIN_DEBUG_DUMP_DATA_AUDIO == dumpId ){
        strcpy(dumpType, "audio frames");
    } else if( AMP_AVIN_DEBUG_DUMP_DATA_VIDEO == dumpId ){
        strcpy(dumpType, "video frames");
    } else if( AMP_AVIN_DEBUG_DUMP_DATA_VBI == dumpId ){
        strcpy(dumpType, "one vbi frame");
    }

    AmpMemClear((VOID*) &dumpInfo, sizeof(AMP_AVIN_DUMP_INFO));

    if( AMP_AVIN_DEBUG_DUMP_DATA_VIDEO == dumpId) {
        sprintf(dumpInfo.dumpPath, "%s/vip_buffer_%s.uyvy", filePath,
            test_avin_get_videopreffilename(h));
    } else if(  AMP_AVIN_DEBUG_DUMP_DATA_AUDIO == dumpId){
        sprintf(dumpInfo.dumpPath, "%s/aip_buffer_%s.audio", filePath,
            test_avin_get_audiopreffilename(h));
    } else if(  AMP_AVIN_DEBUG_DUMP_DATA_VBI == dumpId){
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime( &rawtime );
        sprintf(dumpInfo.dumpPath, "%s/vbi_data_%02d%02d%02d.vbi",
                        filePath,
                        timeinfo->tm_hour,
                        timeinfo->tm_min,
                        timeinfo->tm_sec);
    }

    dumpInfo.dumpId = dumpId;
    AMP_RPC(r, AMP_AVIN_DumpFrame, h->avin, &dumpInfo);

    TEST_AVIN_PRINT("Start dump %s, any key to stop\n", dumpType);
    ch = getchar();

    AMP_RPC(r, AMP_AVIN_DumpFrame, h->avin, &dumpInfo);

    system("sync");
    MV_OSAL_Task_Sleep(300);
    if((fp=fopen(dumpInfo.dumpPath,"r"))==NULL){
        TEST_AVIN_PRINT("Capture %s - failed\n", dumpType);
    }else{
        fclose(fp);
        TEST_AVIN_PRINT("Capture %s to%s %s %s - done\n", dumpType,
            LOG_COLOR_GREEN, dumpInfo.dumpPath, LOG_COLOR_AUTO);
    }

     if(AMP_AVIN_DEBUG_DUMP_DATA_VBI == dumpId)
     {
         vbi_Raw2Epr(dumpInfo.dumpPath);
     }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_videoConnectCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("enter\n");
    AVIN_TEST_LOGD("ePortIo = 0x%x, uiPortIdx = 0x%x\n", ePortIo, uiPortIdx);
    assert(NULL != hBufDesc);

    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("hBufDesc = %p, hBufDesc->uiBDId = 0x%x\n", hBufDesc,
            hBufDesc->uiBDId);

    MV_OSAL_Mutex_Lock(globalVideoMutex);

    AVIN_TEST_LOGD("push BD (%p) to avin out video port. hBufDesc->uiBDId = 0x%x\n",
            hBufDesc, hBufDesc->uiBDId);

    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_AVIN_PushBD, hCompObj, ePortIo, uiPortIdx, hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(globalVideoMutex);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_audioConnectCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;
    UINT32 uiIdx = 0, uiTagCnt = 0;
    AMP_BDTAG_MEMINFO *pAudFrameBDTag = NULL;

    AVIN_TEST_LOGD("enter\n");
    AVIN_TEST_LOGD("ePortIo = 0x%x, uiPortIdx = 0x%x\n", ePortIo, uiPortIdx);
    assert(NULL != hBufDesc);

    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("hBufDesc = %p, hBufDesc->uiBDId = 0x%x\n",
            hBufDesc, hBufDesc->uiBDId);

    MV_OSAL_Mutex_Lock(globalAudioMutex);

    if (TRUE == pHld->enableDumpAudioData) {
        r = AMP_BDTAG_GetNum(hBufDesc, &uiTagCnt);
        if (SUCCESS != r) {
            AMPLOGE("fail to call AMP_BDTAG_GetNum! r = 0x%x\n", r);
            return r;
        }

        for (uiIdx = 0; uiIdx < uiTagCnt; ++uiIdx) {
            r = AMP_BDTAG_GetWithIndex(hBufDesc, uiIdx, (VOID**)&pAudFrameBDTag);
            if (SUCCESS == r) {
                AVIN_TEST_LOGD("uiIdx = %d, pTag = %p\n", uiIdx, pAudFrameBDTag);
                if (AMP_BDTAG_AUD_FRAME_CTRL == pAudFrameBDTag->Header.eType) {
                    VOID *pAddr = NULL;
                    AMP_SHM_Ref(pAudFrameBDTag->uMemHandle, NULL);
                    AMP_SHM_GetVirtualAddress(pAudFrameBDTag->uMemHandle,
                            pAudFrameBDTag->uMemOffset, &pAddr);
                    if (NULL != pAddr) {
                        FILE *fp = fopen(AUD_DATA_FILE, "ab+");
                        if (NULL != fp) {
                            fwrite(pAddr, 1, pAudFrameBDTag->uSize, fp);
                            fclose(fp);
                            TEST_AVIN_PRINT("write (%d) bytes into the file %s!\n",
                                    pAudFrameBDTag->uSize, AUD_DATA_FILE);
                        }
                    }
                    AMP_SHM_Unref(pAudFrameBDTag->uMemHandle);
                    break;
                }
            } else {
                AMPLOGE("fail to call AMP_BDTAG_GetWithIndex! r = 0x%x\n", r);
            }
        }
    }

    AVIN_TEST_LOGD("push BD (%p) to avin out audio port. hBD->uiBDId = 0x%x\n",
                    hBufDesc, hBufDesc->uiBDId);

    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_AVIN_PushBD, hCompObj, ePortIo, uiPortIdx, hBufDesc);
    assert(SUCCESS == r);

    MV_OSAL_Mutex_Unlock(globalAudioMutex);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_anciDataConnectCB(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN AMP_PORT_IO ePortIo, AMP_IN UINT32 uiPortIdx,
        AMP_IN AMP_BD_HANDLE hBufDesc, AMP_IN void *pUserData)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = (TEST_AVIN_HLD*) pUserData;

    AVIN_TEST_LOGD("enter\n");
    AVIN_TEST_LOGD("ePortIo = 0x%x, uiPortIdx = 0x%x\n", ePortIo, uiPortIdx);
    assert(NULL != hBufDesc);
    r = AMP_BD_Ref(hBufDesc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("hBufDesc = %p, hBufDesc->uiBDId = 0x%x\n", hBufDesc,
            hBufDesc->uiBDId);
    if (TRUE == pHld->enableDumpVbiData) {
        r = test_avin_dumpVbiData(hBufDesc);
        assert(SUCCESS == r);
    }

    r = AMP_BD_Unref(hBufDesc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("push BD (%p) to avin vbi out port.\n", hBufDesc);
    AMP_RPC(r, AMP_AVIN_PushBD, hCompObj, ePortIo, uiPortIdx, hBufDesc);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_cleanUpBuf(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    UINT32 uiIdx = 0;
    AMP_BD_HANDLE hBD;

    AVIN_TEST_LOGD("enter\n");
    MV_OSAL_Mutex_Lock(globalAudioMutex);
    if (globalSize > 0) {
        for (uiIdx = 0; uiIdx < globalSize; ++uiIdx) {
            hBD = globalBDList[uiIdx];

            AVIN_TEST_LOGD("push BD (%p) to avin out audio port. hBD->uiBDId = 0x%x\n",
                    hBD, hBD->uiBDId);

            r = AMP_BD_Unref(hBD);
            assert(SUCCESS == r);

            AMP_RPC(r, AMP_AVIN_PushBD, h->avin, AMP_PORT_OUTPUT,
                    h->uiAudioPort, hBD);
            assert(SUCCESS == r);
        }
        globalSize = 0;
        AVIN_TEST_LOGD("clear the list, globalSize = %d!\n", globalSize);
    }
    MV_OSAL_Mutex_Unlock(globalAudioMutex);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_queryPorts(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AMP_COMPONENT_INFO avinCompInfo;
    AMP_PORT_INFO avinPortInfo;
    UINT32 uiIdx = 0;
    AVIN_TEST_LOGD("enter\n");

    /* query AVIN component information */
    AMP_RPC(r, AMP_AVIN_QueryInfo, h->avin, &avinCompInfo);
    if (SUCCESS == r) {
        for (uiIdx = 0; uiIdx < avinCompInfo.uiOutputPortNum; ++uiIdx) {
            /* query AVIN output port information */
            AMP_RPC(r, AMP_AVIN_QueryPort, h->avin, AMP_PORT_OUTPUT, uiIdx,
                &avinPortInfo);
            if (SUCCESS != r) {
                AMPLOGE("fail to call AMP_AVIN_QueryPort, r = 0x%x\n", r);
                return r;
            }
            if (AMP_PORT_AVIN_OUT_FRAMES == avinPortInfo.ePortType) {
                h->uiVideoPort = uiIdx;
            } else if (AMP_PORT_AVIN_OUT_FRAMES_2 == avinPortInfo.ePortType) {
                h->uiVideoPort2 = uiIdx;
            } else if (AMP_PORT_AVIN_OUT_VBI == avinPortInfo.ePortType) {
                h->uiVbiPort = uiIdx;
            } else if (AMP_PORT_AVIN_OUT_AUDIO == avinPortInfo.ePortType) {
                h->uiAudioPort = uiIdx;
            } else if (AMP_PORT_AVIN_OUT_AUDIO_2 == avinPortInfo.ePortType) {
                h->uiAudioPort2 = uiIdx;
            }
        }
    } else {
        AMPLOGE("fail to call AMP_AVIN_QueryInfo, r = 0x%x\n", r);
        return r;
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_connectComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
        // Correct sequence of connecting pipeline:
        // Video: 1) connect VOUT and AVIN.
        // Audio: 1) connect SND Srv and AREN.

        r = AMP_ConnectComp(h->avin, h->uiVideoPort, h->vout, 0);
        assert(SUCCESS == r);

        if (h->enable_mixpassthr) {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
            if(h->enable_mixpassthr_mat) {
                r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                   &h->tunnelIdx[0]);
            }
            else if(h->enable_mixpassthr_ddp) {
                r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                               &h->tunnelIdx[0]);
            }
            else {
                if ((h->audioParams.audFormat == AMP_DD_AC3) ||
                        (h->audioParams.audFormat == AMP_DD_PLUS)) {
                    r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                                       &h->tunnelIdx[0]);
                } else if (h->audioParams.audFormat == AMP_MS12_MAT) {
                    r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                       &h->tunnelIdx[0]);
                } else {
                    r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0,
                                             &h->tunnelIdx[0]);
                    assert(r == SUCCESS);
                    r = AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT_PCM_MULTI);
                }
            }
            assert(r == SUCCESS);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        } else if (h->enable_passthr) {
            AMP_PORT_INFO PortInfo;
            int cnt;
            for (cnt = 0; cnt < 3; cnt++) {
                AMP_RPC(r, AMP_AREN_QueryPort, h->aren, AMP_PORT_OUTPUT, cnt, &PortInfo);
                assert(r == SUCCESS);
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_PCM){
                    h->ArenPort2SndPcm = cnt;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_SPDIF){
                    h->ArenPort2SndSpdif = cnt;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_HDMI){
                    h->ArenPort2SndHdmi = cnt;
                }
            }
            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndPcm, &h->tunnelIdx[0]);
            assert(SUCCESS == r);

            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndSpdif,
                &h->tunnelIdx[1]);
            assert(r == SUCCESS);

            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndHdmi,
                &h->tunnelIdx[2]);
            assert(r == SUCCESS);
        } else {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
            r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0, &h->tunnelIdx[0]); //PCM
            AVIN_TEST_LOGD("r = 0x%x\n", r);
            assert(SUCCESS == r);
#else
            #ifndef DISABLE_HDMIRX_AUDIO
            //    r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 0, &h->tunnelIdx[0]); //PCM
            #endif
            //assert(SUCCESS == r);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        }

        if (TRUE == h->enableClk) {
            r = AMP_ConnectComp(h->clk, 1, h->vout, 1);
            assert(SUCCESS == r);

            AMP_AREN_PARAST arenParaSt;
            AmpMemClear((VOID*) &arenParaSt, sizeof(AMP_AREN_PARAST));
            arenParaSt._u.PORTASSOCCLK.uiAssocIdx = 0; // CLK input port index
            arenParaSt._d = AMP_AREN_PARAIDX_PORTASSOCCLK;

            AMP_RPC(r, AMP_AREN_SetPortParameter, h->aren, AMP_PORT_INPUT, 1,
                    arenParaSt._d, &arenParaSt);
            assert(SUCCESS == r);
        #ifndef DISABLE_HDMIRX_AUDIO
            r = AMP_ConnectComp(h->clk, 0, h->aren, 0);
        #endif
            assert(SUCCESS == r);
        }

        r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVbiPort,
                test_avin_anciDataConnectCB, (void*)h);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
        // Video: 1) connect VENC and AVIN.

        r = AMP_ConnectComp(h->avin, h->uiVideoPort2, h->venc, 0);
        assert(SUCCESS == r);
        r = AMP_ConnectApp(h->venc, AMP_PORT_OUTPUT, 0, test_avin_vencCB, h);
        assert(SUCCESS == r);

        r = test_avin_vencInitInstance(h, AMP_NON_TUNNEL);
        assert(SUCCESS == r);
        h->fp_out_es = open(h->output_vid_file_path, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (h->fp_out_es < 0) {
            AVIN_TEST_LOGD("Error: the es file does not exist.\n");
            exit(-1);
        }
        // Audio: 1) connect DUM and AVIN.
        r = AMP_ConnectComp(h->avin, h->uiAudioPort2, h->audDum, 0);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
        // Video: 1) connect VOUT and AVIN.
        // Video: 2) connect VENC and AVIN.

        r = AMP_ConnectComp(h->avin, h->uiVideoPort, h->vout, 0);
        assert(SUCCESS == r);

        r = AMP_ConnectComp(h->avin, h->uiVideoPort2, h->venc, 0);
        assert(SUCCESS == r);
        r = AMP_ConnectApp(h->venc, AMP_PORT_OUTPUT, 0, test_avin_vencCB, h);
        assert(SUCCESS == r);

        r = test_avin_vencInitInstance(h, AMP_NON_TUNNEL);
        assert(SUCCESS == r);
        h->fp_out_es = open(h->output_vid_file_path, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (h->fp_out_es < 0) {
            AVIN_TEST_LOGD("Error: the es file does not exist.\n");
            exit(-1);
        }
        // Audio: 1) connect DUM and AVIN.
        if (h->enable_mixpassthr) {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
            if(h->enable_mixpassthr_mat) {
                r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                   &h->tunnelIdx[0]);
            }
            else if(h->enable_mixpassthr_ddp) {
                r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                               &h->tunnelIdx[0]);
            }
            else {
                if ((h->audioParams.audFormat == AMP_DD_AC3) ||
                        (h->audioParams.audFormat == AMP_DD_PLUS)) {
                    r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                                       &h->tunnelIdx[0]);
                } else if (h->audioParams.audFormat == AMP_MS12_MAT) {
                    r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                       &h->tunnelIdx[0]);
                } else {
                    r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0,
                                             &h->tunnelIdx[0]);
                    assert(r == SUCCESS);
                    r = AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT_PCM_MULTI);
                }
            }
            assert(r == SUCCESS);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        } else if (h->enable_passthr) {
            AMP_PORT_INFO PortInfo;
            int cnt;
            for (cnt = 0; cnt < 3; cnt++) {
                AMP_RPC(r, AMP_AREN_QueryPort, h->aren, AMP_PORT_OUTPUT, cnt, &PortInfo);
                assert(r == SUCCESS);
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_PCM){
                    h->ArenPort2SndPcm = cnt;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_SPDIF){
                    h->ArenPort2SndSpdif = cnt;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_HDMI){
                    h->ArenPort2SndHdmi = cnt;
                }
            }
            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndPcm, &h->tunnelIdx[0]);
            assert(SUCCESS == r);

            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndSpdif,
                &h->tunnelIdx[1]);
            assert(r == SUCCESS);

            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndHdmi,
                &h->tunnelIdx[2]);
            assert(r == SUCCESS);
        } else {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
            r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0, &h->tunnelIdx[0]); //PCM
            AVIN_TEST_LOGD("r = 0x%x\n", r);
            assert(SUCCESS == r);
#else
            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 0, &h->tunnelIdx[0]); //PCM
            assert(SUCCESS == r);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        }

        if (TRUE == h->enableClk) {
            r = AMP_ConnectComp(h->clk, 1, h->vout, 1);
            assert(SUCCESS == r);

            AMP_AREN_PARAST arenParaSt;
            AmpMemClear((VOID*) &arenParaSt, sizeof(AMP_AREN_PARAST));
            arenParaSt._u.PORTASSOCCLK.uiAssocIdx = 0; // CLK input port index
            arenParaSt._d = AMP_AREN_PARAIDX_PORTASSOCCLK;

            AMP_RPC(r, AMP_AREN_SetPortParameter, h->aren, AMP_PORT_INPUT, 1,
                    arenParaSt._d, &arenParaSt);
            assert(SUCCESS == r);

            r = AMP_ConnectComp(h->clk, 0, h->aren, 0);
            assert(SUCCESS == r);
        }
        r = AMP_ConnectComp(h->avin, h->uiAudioPort2, h->audDum, 0);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
        // Connecting callback function
        r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort2,
                test_avin_videoBufferCB, (void*)h);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {

        // Connecting callback function
        r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort2,
                test_avin_avinVoutCB, (void*)h);
        assert(SUCCESS == r);
        r = AMP_ConnectApp(h->vout, AMP_PORT_INPUT, 0,
                test_avin_voutAvinCB, (void*)h);
        assert(SUCCESS == r);


    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
        // Video: 1) connect VOUT and AVIN.
        // Audio: 1) connect SND Srv and AREN.
        r = AMP_ConnectComp(h->avin, h->uiVideoPort, h->vout, 0);
        assert(SUCCESS == r);
        // Connecting callback function
        r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort2,
                test_avin_videoBufferCB, (void*)h);
        assert(SUCCESS == r);

        if (h->enable_mixpassthr) {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
            if(h->enable_mixpassthr_mat) {
                r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                   &h->tunnelIdx[0]);
            }
            else if(h->enable_mixpassthr_ddp) {
                r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                               &h->tunnelIdx[0]);
            }
            else {
                if ((h->audioParams.audFormat == AMP_DD_AC3) ||
                        (h->audioParams.audFormat == AMP_DD_PLUS)) {
                    r = AMP_SND_SetupTunnel_MS12_4_DDP(h->sndHandle, h->aren, 0, 0,
                                                       &h->tunnelIdx[0]);
                } else if (h->audioParams.audFormat == AMP_MS12_MAT) {
                    r = AMP_SND_SetupTunnel_MS12_4_MAT(h->sndHandle, h->aren, 0, 0,
                                                       &h->tunnelIdx[0]);
                } else {
                    r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0,
                                             &h->tunnelIdx[0]);
                    assert(r == SUCCESS);
                    r = AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT_PCM_MULTI);
                }
            }
            assert(r == SUCCESS);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        } else if (h->enable_passthr) {
            AMP_PORT_INFO PortInfo;
            int cnt;
            for (cnt = 0; cnt < 3; cnt++) {
                AMP_RPC(r, AMP_AREN_QueryPort, h->aren, AMP_PORT_OUTPUT, cnt, &PortInfo);
                assert(r == SUCCESS);
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_PCM){
                    h->ArenPort2SndPcm = cnt;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_SPDIF){
                    h->ArenPort2SndSpdif = cnt;
                }
                if (PortInfo.ePortType == AMP_PORT_AREN_OUT_HDMI){
                    h->ArenPort2SndHdmi = cnt;
                }
            }
            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndPcm, &h->tunnelIdx[0]);
            assert(SUCCESS == r);

            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndSpdif,
                &h->tunnelIdx[1]);
            assert(r == SUCCESS);

            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, h->ArenPort2SndHdmi,
                &h->tunnelIdx[2]);
            assert(r == SUCCESS);
        } else {
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
            r = AMP_SND_SetupTunnel_MS12(h->sndHandle, h->aren, 0, 0, &h->tunnelIdx[0]); //PCM
            AVIN_TEST_LOGD("r = 0x%x\n", r);
            assert(SUCCESS == r);
#else
//            r = AMP_SND_SetupInputTunnel(h->sndHandle, h->aren, 0, &h->tunnelIdx[0]); //PCM
//            assert(SUCCESS == r);
#endif /* CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE */
        }

        if (TRUE == h->enableClk) {
            r = AMP_ConnectComp(h->clk, 1, h->vout, 1);
            assert(SUCCESS == r);

            AMP_AREN_PARAST arenParaSt;
            AmpMemClear((VOID*) &arenParaSt, sizeof(AMP_AREN_PARAST));
            arenParaSt._u.PORTASSOCCLK.uiAssocIdx = 0; // CLK input port index
            arenParaSt._d = AMP_AREN_PARAIDX_PORTASSOCCLK;

            AMP_RPC(r, AMP_AREN_SetPortParameter, h->aren, AMP_PORT_INPUT, 1,
                    arenParaSt._d, &arenParaSt);
            assert(SUCCESS == r);

            r = AMP_ConnectComp(h->clk, 0, h->aren, 0);
            assert(SUCCESS == r);
        }
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE){
        // Connect Vdec and Vout
        // r = AMP_ConnectComp(h->avin, h->uiVideoPort2, h->venc, 0);
        // assert(SUCCESS == r);
        // r = AMP_ConnectComp(h->venc, 0, h->vdec, 0);
        // assert(SUCCESS == r);
        // r = AMP_ConnectComp(h->vdec, 0, h->vout, 0);
        // assert(SUCCESS == r);
        //r = AMP_ConnectComp(h->avin, 3, h->vout, 0);
        //assert(SUCCESS == r);
    }

    assert(SUCCESS == r);
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_disconnectComponent(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
        // Correct sequence of disconnecting pipeline:
        // Audio: 1) disconnect SND Srv and AREN;
        //        2) disconnect AREN and ADEC;
        //        3) disconnect ADEC and AVIN.
        // Video: 1) disconnect VOUT and AVIN.

        r = test_avin_cleanUpBuf(h);
        assert(SUCCESS == r);

        r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[0]); //PCM or MS12
        assert(SUCCESS == r);

        if (h->enable_passthr) {
            r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[1]); // spdif
            assert(r == SUCCESS);

            r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[2]); // hdmi
            assert(r == SUCCESS);
        }

        if (TRUE == h->clkenabled) {
            r = AMP_DisconnectComp(h->clk, 1, h->vout, 1);
            assert(SUCCESS == r);

            r = AMP_DisconnectComp(h->clk, 0, h->aren, 0);
            assert(SUCCESS == r);

            if (AMP_IDLE == test_avin_getAdecState(h)) {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 1);
                assert(SUCCESS == r);
            }
        } else {
            if (AMP_IDLE == test_avin_getAdecState(h)) {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 0);
                assert(SUCCESS == r);
            }
        }
        if (AMP_IDLE == test_avin_getAdecState(h)) {
            r = AMP_DisconnectComp(h->avin, h->uiAudioPort, h->adec, 0);
            assert(SUCCESS == r);
        }
        r = AMP_DisconnectComp(h->avin, h->uiVideoPort, h->vout, 0);
        assert(SUCCESS == r);
        r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVbiPort,
                test_avin_anciDataConnectCB);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
        // Correct sequence of disconnecting pipeline:
        // Video: 1) disconnect VENC and AVIN.

        if (!h->deint) {
            r = AMP_DisconnectComp(h->avin, h->uiVideoPort2, h->venc, 0);
            assert(SUCCESS == r);
        } else {
            r = AMP_DisconnectComp(h->avin, h->uiVideoPort2, h->deint, 0);
            assert(SUCCESS == r);
            r = AMP_DisconnectComp(h->deint, 0, h->venc, 0);
            assert(SUCCESS == r);
        }
        // Audio: 1) disconnect DUM and AVIN.
        r = AMP_DisconnectComp(h->avin, h->uiAudioPort2, h->audDum, 0);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
        // Correct sequence of disconnecting pipeline:
        // Audio: 1) disconnect SND Srv and AREN;
        //        2) disconnect AREN and ADEC;
        //        3) disconnect ADEC and AVIN.
        // Audio: 2) disconnect DUM and AVIN.
        // Video: 1) disconnect VOUT and AVIN.
        // Video: 2) disconnect VENC and AVIN.

        r = test_avin_cleanUpBuf(h);
        assert(SUCCESS == r);

        r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[0]); //PCM or MS12
        assert(SUCCESS == r);

        if (h->enable_passthr) {
            r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[1]); // spdif
            assert(r == SUCCESS);

            r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[2]); // hdmi
            assert(r == SUCCESS);
        }

        if (TRUE == h->clkenabled) {
            r = AMP_DisconnectComp(h->clk, 1, h->vout, 1);
            assert(SUCCESS == r);

            r = AMP_DisconnectComp(h->clk, 0, h->aren, 0);
            assert(SUCCESS == r);

            if (AMP_IDLE == test_avin_getAdecState(h)) {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 1);
                assert(SUCCESS == r);
            }
        } else {
            if (AMP_IDLE == test_avin_getAdecState(h)) {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 0);
                assert(SUCCESS == r);
            }
        }
        if (AMP_IDLE == test_avin_getAdecState(h)) {
            r = AMP_DisconnectComp(h->avin, h->uiAudioPort, h->adec, 0);
            assert(SUCCESS == r);
        }
        r = AMP_DisconnectComp(h->avin, h->uiAudioPort2, h->audDum, 0);
        assert(SUCCESS == r);

        r = AMP_DisconnectComp(h->avin, h->uiVideoPort, h->vout, 0);
        assert(SUCCESS == r);
        if (!h->deint) {
            r = AMP_DisconnectComp(h->avin, h->uiVideoPort2, h->venc, 0);
            assert(SUCCESS == r);
        } else {
            r = AMP_DisconnectComp(h->avin, h->uiVideoPort2, h->deint, 0);
            assert(SUCCESS == r);
            r = AMP_DisconnectComp(h->deint, 0, h->venc, 0);
            assert(SUCCESS == r);
        }
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
        r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort2,
                test_avin_videoBufferCB);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
        r = AMP_DisconnectApp(h->vout, AMP_PORT_INPUT, 0,
                test_avin_voutAvinCB);
        r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort2,
                test_avin_avinVoutCB);
        assert(SUCCESS == r);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
        // Correct sequence of disconnecting pipeline:
        // Audio: 1) disconnect SND Srv and AREN;
        //        2) disconnect AREN and ADEC;
        //        3) disconnect ADEC and AVIN.
        // Video: 1) disconnect VOUT and AVIN.

        r = test_avin_cleanUpBuf(h);
        assert(SUCCESS == r);

        r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[0]); //PCM or MS12
        assert(SUCCESS == r);

        if (h->enable_passthr) {
            r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[1]); // spdif
            assert(r == SUCCESS);

            r = AMP_SND_RemoveInputTunnel(h->sndHandle, h->tunnelIdx[2]); // hdmi
            assert(r == SUCCESS);
        }

        if (TRUE == h->clkenabled) {
            r = AMP_DisconnectComp(h->clk, 1, h->vout, 1);
            assert(SUCCESS == r);

            r = AMP_DisconnectComp(h->clk, 0, h->aren, 0);
            assert(SUCCESS == r);

            if (AMP_IDLE == test_avin_getAdecState(h)) {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 1);
                assert(SUCCESS == r);
            }
        } else {
            if (AMP_IDLE == test_avin_getAdecState(h)) {
                r = AMP_DisconnectComp(h->adec, 0, h->aren, 0);
                assert(SUCCESS == r);
            }
        }
        if (AMP_IDLE == test_avin_getAdecState(h)) {
            r = AMP_DisconnectComp(h->avin, h->uiAudioPort, h->adec, 0);
            assert(SUCCESS == r);
        }

        r = AMP_DisconnectComp(h->avin, h->uiVideoPort, h->vout, 0);
        assert(SUCCESS == r);
        r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort2,
                test_avin_videoBufferCB);
    } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
        // r = AMP_DisconnectComp(h->vdec, 0, h->vout, 0);
        // assert(SUCCESS == r);
        // r = AMP_DisconnectComp(h->venc, 0, h->vdec, 0);
        // assert(SUCCESS == r);
        // r = AMP_DisconnectComp(h->avin, h->uiVideoPort2, h->venc, 0);
        // assert(SUCCESS == r);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_connectCallback(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort,
        test_avin_videoConnectCB, (void*)h);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_ConnectApp, r = 0x%x\n", r);
    } else {
        AVIN_TEST_LOGD("connected video port to APP.\n");
    }

    r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiAudioPort,
        test_avin_audioConnectCB, (void*)h);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_ConnectApp, r = 0x%x\n", r);
    } else {
        AVIN_TEST_LOGD("connected audio port to APP.\n");
    }

    r = AMP_ConnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVbiPort,
        test_avin_anciDataConnectCB, (void*)h);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_ConnectApp, r = 0x%x\n", r);
    } else {
        AVIN_TEST_LOGD("connected vbi port to APP.\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_disconnectCallback(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    r = test_avin_cleanUpBuf(h);
    assert(SUCCESS == r);

    r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVideoPort,
            test_avin_videoConnectCB);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_DisconnectApp, r = 0x%x\n", r);
    }

    r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiAudioPort,
            test_avin_audioConnectCB);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_DisconnectApp, r = 0x%x\n", r);
    }

    r = AMP_DisconnectApp(h->avin, AMP_PORT_OUTPUT, h->uiVbiPort,
            test_avin_anciDataConnectCB);
    if (SUCCESS != r) {
        AMPLOGE("fail to call AMP_DisconnectApp, r = 0x%x\n", r);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_registerEventCallbacks(
        AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN HANDLE hListener, AMP_IN TEST_AVIN_HLD *pHandle)
{
    HRESULT r = SUCCESS;
    UINT32 uiIdx = 0;
    UINT32 uiSize = 0;
    UINT32 uiServiceId;
    AVIN_TEST_LOGD("enter\n");

    uiSize = sizeof(eventList)/sizeof(AMP_EVENT_CODE);
    for (uiIdx = 0; uiIdx < uiSize; ++uiIdx) {
        AVIN_TEST_LOGD("register a event (0x%x) into the listener.\n",
                eventList[uiIdx]);
        r = AMP_Event_RegisterCallback (hListener,
                eventList[uiIdx], test_avin_event_callback, (VOID*)pHandle);
        assert(S_OK == r);
    }

    AVIN_TEST_LOGD("register a notify.\n");
    uiServiceId = AMP_Event_GetServiceID(hListener);
    AMP_RPC(r, AMP_AVIN_RegisterNotify, hCompObj, uiServiceId, 0);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_unregisterEventCallbacks(AMP_IN AMP_COMPONENT hCompObj,
        AMP_IN HANDLE hListener)
{
    HRESULT r = SUCCESS;
    UINT32 uiIdx = 0;
    UINT32 uiSize = 0;
    UINT32 uiServiceId;
    AVIN_TEST_LOGD("enter\n");

    AVIN_TEST_LOGD("unregister the notify.\n");
    uiServiceId = AMP_Event_GetServiceID(hListener);
    AMP_RPC(r, AMP_AVIN_UnregisterNotify, hCompObj, uiServiceId, 0);

    uiSize = sizeof(eventList)/sizeof(AMP_EVENT_CODE);
    for (uiIdx = 0; uiIdx < uiSize; ++uiIdx) {
        AVIN_TEST_LOGD("unregister the event (0x%x) callback.\n", eventList[uiIdx]);
        r = AMP_Event_UnregisterCallback (hListener,
                eventList[uiIdx], test_avin_event_callback);
        assert(S_OK == r);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_adec_register_event(HANDLE hListener, AMP_COMPONENT hAdec, BOOL bRegister, AMP_IN TEST_AVIN_HLD *pHandle)
{
    HRESULT result = SUCCESS;

    if (bRegister) {
        AMP_Event_RegisterCallback(hListener,
                AMP_EVENT_ADEC_CALLBACK_STRMINFO,
                test_avin_event_callback,
                pHandle);
        AMP_RPC(result, AMP_ADEC_RegisterNotify, hAdec,
                AMP_Event_GetServiceID(hListener),
                AMP_EVENT_ADEC_CALLBACK_STRMINFO);
    } else {
        AMP_RPC(result, AMP_ADEC_UnregisterNotify, hAdec,
                AMP_Event_GetServiceID(hListener),
                AMP_EVENT_ADEC_CALLBACK_STRMINFO);
        result = AMP_Event_UnregisterCallback(hListener,
                AMP_EVENT_ADEC_CALLBACK_STRMINFO,
                test_avin_event_callback);
    }

    return result;
}

static HRESULT test_avin_removeSndTunnels(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = h;
    int i =0;

    for (i = 0; i < TEST_AVIN_AREN_OUTPUT_PORTS; i++) {
        if (NULL != pHld->tunnel_handle[i]) {
            r = AMP_SND_RemoveTunnel(pHld->tunnel_handle[i]);
            if (SUCCESS != r ) {
                TEST_AVIN_PRINT("AMP_SND_RemoveTunnel %d failed = 0x%x\n", i, r);
            }
            pHld->tunnel_handle[i] = NULL;
        }
    }
    return S_OK;
}

static HRESULT test_avin_setupSndTunnels(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = h;
    int i =0;

    for (i = 0; i < TEST_AVIN_AREN_OUTPUT_PORTS; i++) {
        r = AMP_SND_SetupTunnel(pHld->aren, i, &pHld->tunnel_handle[i]);
        if (SUCCESS != r || NULL == pHld->tunnel_handle[i]) {
            TEST_AVIN_PRINT("init AMP_SND fail, ret:0x%x!, tunnel_handle:%x",
                  r,pHld->tunnel_handle[i]);
            return S_FALSE;
        }
    }
    return S_OK;
}

static void test_avin_init(AMP_IN AMP_FACTORY factory, TEST_AVIN_HLD *pHld)
{
    HRESULT r = SUCCESS;
    HRESULT ret = S_OK;
    UINT32 uiIdx = 0;
    AMP_COMPONENT_CONFIG config;
    HANDLE hListener = NULL;
    AMP_AVIN_PLATFORM_CONFIG pltConfig;
    AMP_DISP_WIN src = {0, 0, 0, 0}, desc = {0, 0, 0, 0};

    AVIN_TEST_LOGD("enter.\n");

    ringbuf_alloc(&gRingbuf,1,ONE_FRAME_SIZE*MAX_FRAME);
    ringbuf_alloc(&gWMbuf,1,ONE_FRAME_SIZE*MAX_FRAME);



    ret = MV_OSAL_Mutex_Create(&(globalVideoMutex));
    assert(S_OK == ret);

    ret = MV_OSAL_Mutex_Create(&(globalAudioMutex));
    assert(S_OK == ret);

    pHld->factory = factory;
    pHld->srcStatus = AMP_AVIN_SOURCE_STATE_DISCONNECTED;
    pHld->enableClk = FALSE;
    pHld->clkenabled = FALSE;
    pHld->EdidInfo.size = 0;
#if ( BERLIN_CHIP_VERSION  == BERLIN_BG4_CDP )
    pHld->enableMS11 = FALSE;
    pHld->enableDDUDC   = TRUE;
#elif ( BERLIN_CHIP_VERSION == BERLIN_VS680 ) || \
    ( BERLIN_CHIP_VERSION == BERLIN_VS680_A0 )
    pHld->enableMS11 = FALSE;
#ifdef CONFIG_AMP_IP_AUDIO_MS12_SUPPORTED
    pHld->enableMS12 = TRUE;
#endif
#else
    pHld->enableMS11 = TRUE;
#endif
    pHld->enableDumpAudioData = FALSE;
    pHld->resId_origin = -1;
    pHld->fp_out_es = NULL;
    pHld->playBackStopped = FALSE;
    r = test_avin_setupAvinComponent(pHld);

    if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_VOUT ||
            pHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT ||
            pHld->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT ||
            pHld->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
        r = AMP_SND_Initialize(pHld->factory, &pHld->sndHandle);
        assert((SUCCESS == r) && (NULL != pHld->sndHandle));

        AMP_RPC(r, AMP_FACTORY_CreateDisplayService, factory, &pHld->voutDisp);
        assert(SUCCESS == r);

        AMP_RPC(r, AMP_DISP_GetPlaneZOrder, pHld->voutDisp, 0,
                &pHld->voutZorder);
        assert(SUCCESS == r);

        /* Set CEC PHY address */
        test_avin_set_cec_addr(pHld);
    }
    AMP_RPC(r, AMP_AVIN_SetOutputPlane, pHld->avin, pHld->outputPlane);
    assert(SUCCESS == r);

    r = test_avin_queryPorts(pHld);
    if(SUCCESS != r){
        pHld->testMode = TEST_AVIN_MODE_NON_TUNNEL;
    }

    AVIN_TEST_LOGD("pHld->testMode = %d\n", pHld->testMode);
    if (TEST_AVIN_MODE_TUNNEL == pHld->testMode) {
        if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
            r = test_avin_setupVoutComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setDispScale(pHld);
            assert(SUCCESS == r);
        #ifndef DISABLE_HDMIRX_AUDIO
            r = test_avin_createAdecComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupArenComponent(pHld);
            assert(SUCCESS == r);
        #endif
            r = test_avin_setupClkComponent(pHld);
            assert(SUCCESS == r);
            if(pgHld->isPassthroughOn)
                AMP_RPC(r, AMP_AVIN_EnablePassthrough, pHld->avin, pHld->isPassthroughOn);
        } else if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
            AVIN_TEST_LOGD ("setting the output mode to %d\n", pHld->outputComp);
            AMP_RPC(r, AMP_AVIN_SetOutputComp, pHld->avin, AVIN_OUTPUT_COMP_NS_TUNNEL_2K);
            assert(SUCCESS == r);
            r = test_avin_setupVencComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupDumComponent(pHld);
            assert(SUCCESS == r);
        } else if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
            AVIN_TEST_LOGD ("setting the output mode to %d\n", pHld->outputComp);
            AMP_RPC(r, AMP_AVIN_SetOutputComp, pHld->avin, AVIN_OUTPUT_COMP_NS_TUNNEL_2K);
            assert(SUCCESS == r);
            r = test_avin_setupVoutComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setDispScale(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupVencComponent(pHld);
            assert(SUCCESS == r);
        #ifndef DISABLE_HDMIRX_AUDIO
            r = test_avin_createAdecComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupArenComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupClkComponent(pHld);
            assert(SUCCESS == r);
        #endif
            r = test_avin_setupDumComponent(pHld);
            assert(SUCCESS == r);
        } else if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
            AVIN_TEST_LOGD ("setting the output mode to %d\n", pHld->outputComp);
            AMP_RPC(r, AMP_AVIN_SetOutputComp, pHld->avin, AVIN_OUTPUT_COMP_NS_TUNNEL_2K);
            assert(SUCCESS == r);
            time_t seconds;
            struct tm *tmStruct;
            seconds = time(NULL);
            tmStruct = localtime(&seconds);
            pHld->stTm = tmStruct->tm_sec;
            pHld->crntTm = tmStruct->tm_sec;
            pHld->frCnt = 0;
            pHld->flCnt = 1;
            pHld->delIntvl = 5;
        } else if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
            AVIN_TEST_LOGD ("setting the output mode to %d\n", pHld->outputComp);
            AMP_RPC(r, AMP_AVIN_SetOutputComp, pHld->avin, AVIN_OUTPUT_COMP_NS_TUNNEL_2K);
            assert(SUCCESS == r);
            r = test_avin_setupVoutComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setDispScale(pHld);
            assert(SUCCESS == r);
            MV_OSAL_Task_Create(&pgHld->rbTask, test_avin_process_ringbuf, pgHld);
        } else if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
            AVIN_TEST_LOGD ("setting the output mode to %d\n", pHld->outputComp);
            MV_OSAL_Task_Create(&pgHld->fileTask, test_avin_process_filedump, pgHld);
            AMP_RPC(r, AMP_AVIN_SetOutputComp, pHld->avin, AVIN_OUTPUT_COMP_NS_TUNNEL_2K);
            assert(SUCCESS == r);
            r = test_avin_setupVoutComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setDispScale(pHld);
            assert(SUCCESS == r);
        #ifndef DISABLE_HDMIRX_AUDIO
            r = test_avin_createAdecComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupArenComponent(pHld);
            assert(SUCCESS == r);
        #endif
            r = test_avin_setupClkComponent(pHld);
            assert(SUCCESS == r);
            time_t seconds;
            struct tm *tmStruct;
            seconds = time(NULL);
            tmStruct = localtime(&seconds);
            pHld->stTm = tmStruct->tm_sec;
            pHld->crntTm = tmStruct->tm_sec;
            pHld->frCnt = 0;
            pHld->flCnt = 1;
            pHld->delIntvl = 5;
        } else if (pHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
            AMP_RPC(r, AMP_AVIN_SetOutputComp, pHld->avin, AVIN_OUTPUT_COMP_TUNNEL_2K);
            assert(SUCCESS == r);
            r = test_avin_setupVencComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupVdecComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setupVoutComponent(pHld);
            assert(SUCCESS == r);
            r = test_avin_setDispScale(pHld);
            assert(SUCCESS == r);
        }

        AVIN_TEST_LOGD("connect component.\n");
        r = test_avin_connectComponent(pHld);
        assert(SUCCESS == r);

        AMP_RPC(r, AMP_AVIN_GetCurrentSource, pHld->avin, &pHld->curSrc);
        assert(SUCCESS == r);
        if( AMP_AVIN_SOURCE_TYPE_INVALID == pHld->curSrc){
            AMP_AVIN_SOURCE_LIST sourceList;
            AMP_AVIN_SOURCE_TYPE source;

            AMP_RPC(r, AMP_AVIN_GetSupportedSourceList, pHld->avin, &sourceList);
            for (uiIdx = 0; uiIdx < sourceList.size; ++uiIdx) {
                AVIN_TEST_LOGD("Get supported source list: [%d] %s\n", uiIdx,
                test_avin_inputSourceString(sourceList.source[uiIdx]));
            }

            source = sourceList.source[0];
            AMP_RPC(r, AMP_AVIN_SetCurrentSource, pHld->avin, source);
            assert(SUCCESS == r);
            pHld->curSrc = sourceList.source[0];
            TEST_AVIN_PRINT("No source was selected, set to %s\n",
            test_avin_inputSourceString(pHld->curSrc));
        }
    } else if (TEST_AVIN_MODE_NON_TUNNEL == pHld->testMode) {
        AVIN_TEST_LOGD("connect component.\n");
        r = test_avin_connectCallback(pHld);
    }

    AVIN_TEST_LOGD("create a callback listener.\n");
    hListener = (HANDLE) AMP_Event_CreateListener(AMP_EVENT_TYPE_MAX, 0);
    assert(NULL != hListener);
    pHld->hListener = hListener;

    AVIN_TEST_LOGD("register event callbacks.\n");
    r = test_avin_registerEventCallbacks(pHld->avin, hListener, pHld);

    //AMP_RPC(r, AMP_AVIN_GetPlatformConfig, pHld->avin, &pltConfig);
    // strcpy(pHld->chip_name, pltConfig.chipName);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
}

static HRESULT test_avin_keyevent_task(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;

    while( !h->event_task_exit ){
        h->keyEvent = getchar();
    }
    return r;
}

static HRESULT test_avin_keyevent_start(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;

    h->keyEvent = 0;
    h->event_task_exit = FALSE;
    r = MV_OSAL_Task_Create_Detailed(&h->event_task,
                                    test_avin_keyevent_task,
                                    h,
                                    MV_OSAL_TASK_PRIORITY_15,
                                    "TEST_COMPONENT_AVIN_EVENT_TASK",
                                    NULL,
                                    100, 0);
    return r;
}

static HRESULT test_avin_keyevent_stop(TEST_AVIN_HLD *h)
{
    h->event_task_exit = TRUE;
    return MV_OSAL_Task_Join(&h->event_task);
}

static HRESULT setDispZorder(AMP_DISP disp, UINT32 uMuteGfx, UINT32 plane)
{
    HRESULT result;
    AMP_DISP_ZORDER Zorder;
    static AMP_DISP_ZORDER originOrder;

    if (uMuteGfx) {
        if (plane == AMP_DISP_PLANE_PIP) {
            Zorder.iMain = 1;
            Zorder.iPip  = 2;
        } else {
            Zorder.iMain = 2;
            Zorder.iPip  = 1;
        }
        Zorder.iGfx0 = 0;
        Zorder.iGfx1 = -1;
        Zorder.iGfx2 = -1;
        Zorder.iPg   = -1;
        Zorder.iBg   = -1;
        Zorder.iAux  = -1;
    } else {
        // restore to the default order
        Zorder.iMain = 1;
        Zorder.iPip  = 0;
        Zorder.iGfx0 = 2;
        Zorder.iGfx1 = -1;
        Zorder.iGfx2 = -1;
        Zorder.iPg   = -1;
        Zorder.iBg   = -1;
        Zorder.iAux  = -1;
    }

    AMP_RPC(result, AMP_DISP_SetPlaneZOrder, disp, AMP_DISP_TG_PRI, &Zorder);
//    AMP_RPC(result, AMP_DISP_SetPlaneMute, disp, AMP_DISP_PLANE_GFX0, uMuteGfx);//mute graphic plane
//    VERIFY_RESULT(result);

    return result;
}
static HRESULT test_avin_play(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    HRESULT ret = SUCCESS;
    UINT32 voutTemp = 0;
    AMP_AVIN_INPUT_PARAMS inputParams;

    AVIN_TEST_LOGD("enter.\n");

    // Correct sequence of starting components:
    // Video: 1) set VOUT to EXECUTING;
    //        2) set AVIN to EXECUTING.
    // Audio: 1) set AREN to EXECUTING;
    //        2) set ADEC to EXECUTING;
    //        3) set AVIN to EXECUTING.

    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
            AVIN_TEST_LOGD("set vout state to EXECUTING.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_EXECUTING);
            assert(SUCCESS == r);

            if (TRUE == h->enableClk) {
                AVIN_TEST_LOGD("set clk state to EXECUTING.\n");
                AMP_RPC(r, AMP_CLK_SetState, h->clk, AMP_EXECUTING);
                assert(SUCCESS == r);
            }
            setDispZorder(h->voutDisp, 1, h->outputPlane);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, TRUE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, TRUE);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, TRUE);
#endif
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
            AVIN_TEST_LOGD("set venc state to EXECUTING.\n");
            AMP_RPC(r, AMP_VENC_SetState, h->venc, AMP_EXECUTING);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set audDum state to EXECUTING.\n");
            AMP_RPC(r, AMP_DUM_SetState, h->audDum, AMP_EXECUTING);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
            AVIN_TEST_LOGD("set vout state to EXECUTING.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_EXECUTING);
            assert(SUCCESS == r);

            if (TRUE == h->enableClk) {
                AVIN_TEST_LOGD("set clk state to EXECUTING.\n");
                AMP_RPC(r, AMP_CLK_SetState, h->clk, AMP_EXECUTING);
                assert(SUCCESS == r);
            }
            setDispZorder(h->voutDisp, 1, h->outputPlane);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, TRUE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, TRUE);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, TRUE);
#endif
            AVIN_TEST_LOGD("set venc state to EXECUTING.\n");
            AMP_RPC(r, AMP_VENC_SetState, h->venc, AMP_EXECUTING);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set audDum state to EXECUTING.\n");
            AMP_RPC(r, AMP_DUM_SetState, h->audDum, AMP_EXECUTING);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
            AVIN_TEST_LOGD("set vout state to EXECUTING.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_EXECUTING);
            assert(SUCCESS == r);

            setDispZorder(h->voutDisp, 1, h->outputPlane);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, TRUE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, TRUE);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, TRUE);
#endif
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
            AVIN_TEST_LOGD("set vout state to EXECUTING.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_EXECUTING);
            assert(SUCCESS == r);

            if (TRUE == h->enableClk) {
                AVIN_TEST_LOGD("set clk state to EXECUTING.\n");
                AMP_RPC(r, AMP_CLK_SetState, h->clk, AMP_EXECUTING);
                assert(SUCCESS == r);
            }
            setDispZorder(h->voutDisp, 1, h->outputPlane);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, TRUE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, TRUE);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, TRUE);
#endif
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
            AVIN_TEST_LOGD("set vout state to EXECUTING.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_EXECUTING);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set vdec state to EXECUTING.\n");
            AMP_RPC(r, AMP_VDEC_SetState, h->vdec, AMP_EXECUTING);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set venc state to EXECUTING.\n");
            AMP_RPC(r, AMP_VENC_SetState, h->venc, AMP_EXECUTING);
            assert(SUCCESS == r);
        }
    }

    AMP_RPC(r, AMP_AVIN_SetState, h->avin, AMP_EXECUTING);
    assert(SUCCESS == r);

    AmpMemClear((VOID*) &inputParams, sizeof(AMP_AVIN_INPUT_PARAMS));
    AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);

    AmpMemcpy(&h->videoParams, &inputParams.videoParams,
            sizeof(AMP_AVIN_INPUT_VIDEO_PARAMS));

    AmpMemcpy(&h->audioParams, &inputParams.audioParams,
            sizeof(AMP_AVIN_INPUT_AUDIO_PARAMS));

    if (AMP_UNKNOW_AUD == h->audioParams.audFormat) {
        h->audioParams.audFormat = AMP_HE_AAC;
    }

    if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT ||
            h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT ||
            h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
        if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        #ifndef DISABLE_HDMIRX_AUDIO
            r = test_avin_reconstructAudioPipeline(h);
            assert(SUCCESS == r);
        #endif
            r = test_avin_setDispScale(h);
            assert(SUCCESS == r);
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_stop(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    UINT32 voutTemp;

    AVIN_TEST_LOGD("enter.\n");

    // Correct sequence of stopping components:
    // Video: 1) set AVIN to IDLE;
    //        2) set VOUT to IDLE.
    // Audio: 1) set AVIN to IDLE;
    //        2) set ADEC to IDLE;
    //        3) set AREN to IDLE.

    AMP_RPC(r, AMP_AVIN_SetState, h->avin, AMP_IDLE);

    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
            if (TRUE == h->clkenabled) {
                AVIN_TEST_LOGD("set clk state to IDLE.\n");
                AMP_RPC(r, AMP_CLK_SetState, h->clk, AMP_IDLE);
                assert(SUCCESS == r);
            }

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
            #ifndef DISABLE_HDMIRX_AUDIO
                if (AMP_EXECUTING == test_avin_getAdecState(h)) {
                    AVIN_TEST_LOGD("set adec state to IDLE.\n");
                    AMP_RPC(r, AMP_ADEC_SetState, h->adec, AMP_IDLE);
                    assert(SUCCESS == r);

                    AVIN_TEST_LOGD("set aren state to IDLE.\n");
                    AMP_RPC(r, AMP_AREN_SetState, h->aren, AMP_IDLE);
                    assert(SUCCESS == r);
                }

                r = AMP_SND_StopTunnel(h->tunnelIdx[0]);
                assert(r == SUCCESS);
            #endif
                if (h->enable_passthr) {
                    r = AMP_SND_StopTunnel(h->tunnelIdx[1]);
                    assert(r == SUCCESS);
                    r = AMP_SND_StopTunnel(h->tunnelIdx[2]);
                    assert(r == SUCCESS);
                }
            }
            AVIN_TEST_LOGD("set vout state to IDLE.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_IDLE);
            assert(SUCCESS == r);

            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, FALSE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, FALSE);
            setDispZorder(h->voutDisp, 0, h->outputPlane);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, FALSE);

            /*FIX ME...Unmute 2K path*/
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, 9, FALSE);
#endif
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
            if (h->deint) {
                AVIN_TEST_LOGD("set deint state to IDLE.\n");
                AMP_RPC(r, AMP_DEINT_SetState, h->deint, AMP_IDLE);
                assert(SUCCESS == r);
            }
            AVIN_TEST_LOGD("set venc state to IDLE.\n");
            AMP_RPC(r, AMP_VENC_SetState, h->venc, AMP_IDLE);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set audDum state to IDLE.\n");
            AMP_RPC(r, AMP_DUM_SetState, h->audDum, AMP_IDLE);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
            if (TRUE == h->clkenabled) {
                AVIN_TEST_LOGD("set clk state to IDLE.\n");
                AMP_RPC(r, AMP_CLK_SetState, h->clk, AMP_IDLE);
                assert(SUCCESS == r);
            }

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                if (AMP_EXECUTING == test_avin_getAdecState(h)) {
                    AVIN_TEST_LOGD("set adec state to IDLE.\n");
                    AMP_RPC(r, AMP_ADEC_SetState, h->adec, AMP_IDLE);
                    assert(SUCCESS == r);

                    AVIN_TEST_LOGD("set aren state to IDLE.\n");
                    AMP_RPC(r, AMP_AREN_SetState, h->aren, AMP_IDLE);
                    assert(SUCCESS == r);
                }

                r = AMP_SND_StopTunnel(h->tunnelIdx[0]);
                assert(r == SUCCESS);
                if (h->enable_passthr) {
                    r = AMP_SND_StopTunnel(h->tunnelIdx[1]);
                    assert(r == SUCCESS);
                    r = AMP_SND_StopTunnel(h->tunnelIdx[2]);
                    assert(r == SUCCESS);
                }
            }
            AVIN_TEST_LOGD("set vout state to IDLE.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_IDLE);
            assert(SUCCESS == r);

            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, FALSE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, FALSE);
            setDispZorder(h->voutDisp, 0, h->outputPlane);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, FALSE);

            /*FIX ME...Unmute 2K path*/
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, 9, FALSE);
#endif
            if (h->deint) {
                AVIN_TEST_LOGD("set deint state to IDLE.\n");
                AMP_RPC(r, AMP_DEINT_SetState, h->deint, AMP_IDLE);
                assert(SUCCESS == r);
            }
            AVIN_TEST_LOGD("set venc state to IDLE.\n");
            AMP_RPC(r, AMP_VENC_SetState, h->venc, AMP_IDLE);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set audDum state to IDLE.\n");
            AMP_RPC(r, AMP_DUM_SetState, h->audDum, AMP_IDLE);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
            AVIN_TEST_LOGD("set vout state to IDLE.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_IDLE);
            assert(SUCCESS == r);

            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, FALSE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, FALSE);
            setDispZorder(h->voutDisp, 0, h->outputPlane);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, FALSE);

            /*FIX ME...Unmute 2K path*/
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, 9, FALSE);
#endif
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
            if (TRUE == h->clkenabled) {
                AVIN_TEST_LOGD("set clk state to IDLE.\n");
                AMP_RPC(r, AMP_CLK_SetState, h->clk, AMP_IDLE);
                assert(SUCCESS == r);
            }

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                if (AMP_EXECUTING == test_avin_getAdecState(h)) {
                    AVIN_TEST_LOGD("set adec state to IDLE.\n");
                    AMP_RPC(r, AMP_ADEC_SetState, h->adec, AMP_IDLE);
                    assert(SUCCESS == r);

                    AVIN_TEST_LOGD("set aren state to IDLE.\n");
                    AMP_RPC(r, AMP_AREN_SetState, h->aren, AMP_IDLE);
                    assert(SUCCESS == r);
                }

                r = AMP_SND_StopTunnel(h->tunnelIdx[0]);
                assert(r == SUCCESS);
                if (h->enable_passthr) {
                    r = AMP_SND_StopTunnel(h->tunnelIdx[1]);
                    assert(r == SUCCESS);
                    r = AMP_SND_StopTunnel(h->tunnelIdx[2]);
                    assert(r == SUCCESS);
                }
            }
            AVIN_TEST_LOGD("set vout state to IDLE.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_IDLE);
            assert(SUCCESS == r);

            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX0, FALSE);
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX1, FALSE);
            setDispZorder(h->voutDisp, 0, h->outputPlane);
#if (BERLIN_CHIP_VERSION != BERLIN_VS680) && \
    (BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, AMP_DISP_PLANE_GFX2, FALSE);

            /*FIX ME...Unmute 2K path*/
            AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, 9, FALSE);
#endif
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
            AVIN_TEST_LOGD("set venc state to IDLE.\n");
            AMP_RPC(r, AMP_VENC_SetState, h->venc, AMP_IDLE);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set vdec state to IDLE.\n");
            AMP_RPC(r, AMP_VDEC_SetState, h->vdec, AMP_IDLE);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("set vout state to IDLE.\n");
            AMP_RPC(r, AMP_VOUT_SetState, h->vout, AMP_IDLE);
            assert(SUCCESS == r);
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static void test_avin_show_source(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    UINT32 i = 0, j = 0;
    char *s[]={" ","(*)"};
    AMP_AVIN_SOURCE_LIST SourceList, ActiveSourceList;

    memset(&SourceList, 0, sizeof(AMP_AVIN_SOURCE_LIST));
    AMP_RPC(r, AMP_AVIN_GetSupportedSourceList, h->avin, &SourceList);

    TEST_AVIN_PRINT("\nSupported source list:\n");
    for(i = 0; i < SourceList.size; i++){
        BOOL isActive = FALSE;
        memset(&ActiveSourceList, 0, sizeof(ActiveSourceList));
        AMP_RPC(r, AMP_AVIN_GetActiveSourceList, h->avin, &ActiveSourceList);
        for(j=0; j<ActiveSourceList.size; j++){
            if( SourceList.source[i] == ActiveSourceList.source[j]){
                isActive = TRUE;
                break;
            }
        }

        TEST_AVIN_PRINT("%x. %s%s\n", SourceList.source[i],
            test_avin_inputSourceString(SourceList.source[i]),s[isActive]);
    }
    TEST_AVIN_PRINT("ff: Exit(Don't change source)\n\n");
}

static int test_avin_get_input(AMP_IN TEST_AVIN_HLD *h)
{
    int source = 0;

    test_avin_show_source(h);
    TEST_AVIN_PRINT("Source:");

    scanf("%x",&source);
    if( source > AMP_AVIN_SOURCE_TYPE_INVALID&&
    source < AMP_AVIN_SOURCE_TYPE_MAX_NUM){
        return source;
    } else if ( source == 0xF) {
        TEST_AVIN_PRINT("No source is changed in ampclient_alpha,"
            "will keep current source\n");
        return AMP_AVIN_SOURCE_TYPE_INVALID;
    } else {
        TEST_AVIN_PRINT("Invalid source is selected\n");
        return AMP_AVIN_SOURCE_TYPE_INVALID;
    }
}

static HRESULT test_avin_deInit(AMP_IN TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    HRESULT ret = S_OK;
    AMP_STATE state;
    UINT32 uNum;

    AVIN_TEST_LOGD("enter.\n");

    AMP_RPC(r, AMP_AVIN_DebugDump, h->avin, 0 /* STOP DUMPING ANY DATA */);
    assert(SUCCESS == r);

    if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT ||
            h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT ||
            h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
        test_avin_muteVideoAudio(h, TRUE);
    }
    AVIN_TEST_LOGD("disconnect component.\n");
    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        // Change the source to invalid before exiting the app.
        AMP_RPC(r, AMP_AVIN_SetCurrentSource, h->avin, AMP_AVIN_SOURCE_TYPE_INVALID);
        if (SUCCESS != r) {
            AMPLOGE("failed to set the source as invalid.\n");
            return r;
        }

        r = test_avin_disconnectComponent(h);
        assert(SUCCESS == r);
    } else if (TEST_AVIN_MODE_NON_TUNNEL == h->testMode) {
        r = test_avin_disconnectCallback(h);
    }

    AVIN_TEST_LOGD("unregister the notify.\n");
    r = test_avin_unregisterEventCallbacks(h->avin, h->hListener);
    assert(S_OK == r);

    AVIN_TEST_LOGD("destroy the listener.\n");
    r = AMP_Event_DestroyListener(h->hListener);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("close avin.\n");
    AMP_RPC(r, AMP_AVIN_Close, h->avin);

    /* Check if AVIN component state is LOADED */
    AMP_RPC(r, AMP_AVIN_GetState, h->avin, &state);
    AVIN_TEST_LOGD("get avin state: 0x%x\n", state);
    assert(AMP_LOADED == state);

    AVIN_TEST_LOGD("destroy avin.\n");
    AMP_RPC(r, AMP_AVIN_Destroy, h->avin);
    assert(SUCCESS == r);

    AMP_FACTORY_Release(h->avin);

    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT) {
            AVIN_TEST_LOGD("close vout.\n");
            AMP_RPC(r, AMP_VOUT_Close, h->vout);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vout.\n");
            AMP_RPC(r, AMP_VOUT_Destroy, h->vout);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->vout);
            assert(SUCCESS == r);

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                if (AMP_IDLE == test_avin_getAdecState(h)) {
                    r = test_adec_register_event(h->hListener, h->adec, FALSE, h);
                    AVIN_TEST_LOGD("close adec.\n");
                    AMP_RPC(r, AMP_ADEC_Close, h->adec);
                    assert(SUCCESS == r);
                }

                if (AMP_LOADED == test_avin_getAdecState(h)) {
                    AVIN_TEST_LOGD("destroy adec.\n");
                    AMP_RPC(r, AMP_ADEC_Destroy, h->adec);
                    assert(SUCCESS == r);
                }
            }

            AMP_FACTORY_Release(h->adec);

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                AVIN_TEST_LOGD("close aren.\n");
                AMP_RPC(r, AMP_AREN_Close, h->aren);
                assert(SUCCESS == r);

                AVIN_TEST_LOGD("destroy aren.\n");
                AMP_RPC(r, AMP_AREN_Destroy, h->aren);
                assert(SUCCESS == r);
            }

            AMP_FACTORY_Release(h->aren);

            AVIN_TEST_LOGD("close clk.\n");
            AMP_RPC(r, AMP_CLK_Close, h->clk);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy clk.\n");
            AMP_RPC(r, AMP_CLK_Destroy, h->clk);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->clk);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE) {
            while ((AMP_BDCHAIN_GetItemNum(h->pStreamBDQ, &uNum) == SUCCESS) &&
                    (uNum < MAX_OUTPUT_STREAM_NUM)) {
                AVIN_TEST_LOGD ("venc Stream Bufs back %d, max %d\n", uNum,
                        MAX_OUTPUT_STREAM_NUM);
                usleep(50000);
            }
            r = test_avin_vencDeinitInstance(h);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("close venc.\n");
            AMP_RPC(r, AMP_VENC_Close, h->venc);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy venc.\n");
            AMP_RPC(r, AMP_VENC_Destroy, h->venc);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->venc);
            assert(SUCCESS == r);
            close(h->fp_out_es);

            if (h->deint) {
                AVIN_TEST_LOGD("close deint.\n");
                AMP_RPC(r, AMP_DEINT_Close, h->deint);
                assert(SUCCESS == r);

                AVIN_TEST_LOGD("destroy deint.\n");
                AMP_RPC(r, AMP_DEINT_Destroy, h->deint);
                assert(SUCCESS == r);

                AMP_FACTORY_Release(h->deint);
                assert(SUCCESS == r);
            }

            AVIN_TEST_LOGD("close audDum.\n");
            AMP_RPC(r, AMP_DUM_Close, h->audDum);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy audDum.\n");
            AMP_RPC(r, AMP_DUM_Destroy, h->audDum);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->audDum);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
            AVIN_TEST_LOGD("close vout.\n");
            AMP_RPC(r, AMP_VOUT_Close, h->vout);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vout.\n");
            AMP_RPC(r, AMP_VOUT_Destroy, h->vout);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->vout);
            assert(SUCCESS == r);
            while ((AMP_BDCHAIN_GetItemNum(h->pStreamBDQ, &uNum) == SUCCESS) &&
                    (uNum < MAX_OUTPUT_STREAM_NUM)) {
                AVIN_TEST_LOGD ("venc Stream Bufs back %d, max %d\n", uNum,
                        MAX_OUTPUT_STREAM_NUM);
                usleep(50000);
            }
            r = test_avin_vencDeinitInstance(h);
            assert(SUCCESS == r);
            AVIN_TEST_LOGD("close venc.\n");
            AMP_RPC(r, AMP_VENC_Close, h->venc);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy venc.\n");
            AMP_RPC(r, AMP_VENC_Destroy, h->venc);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->venc);
            assert(SUCCESS == r);
            close(h->fp_out_es);

            if (h->deint) {
                AVIN_TEST_LOGD("close deint.\n");
                AMP_RPC(r, AMP_DEINT_Close, h->deint);
                assert(SUCCESS == r);

                AVIN_TEST_LOGD("destroy deint.\n");
                AMP_RPC(r, AMP_DEINT_Destroy, h->deint);
                assert(SUCCESS == r);

                AMP_FACTORY_Release(h->deint);
                assert(SUCCESS == r);
            }

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                if (AMP_IDLE == test_avin_getAdecState(h)) {
                    r = test_adec_register_event(h->hListener, h->adec, FALSE, h);
                    AVIN_TEST_LOGD("close adec.\n");
                    AMP_RPC(r, AMP_ADEC_Close, h->adec);
                    assert(SUCCESS == r);
                }

                if (AMP_LOADED == test_avin_getAdecState(h)) {
                    AVIN_TEST_LOGD("destroy adec.\n");
                    AMP_RPC(r, AMP_ADEC_Destroy, h->adec);
                    assert(SUCCESS == r);
                }
            }

            AMP_FACTORY_Release(h->adec);

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                AVIN_TEST_LOGD("close aren.\n");
                AMP_RPC(r, AMP_AREN_Close, h->aren);
                assert(SUCCESS == r);

                AVIN_TEST_LOGD("destroy aren.\n");
                AMP_RPC(r, AMP_AREN_Destroy, h->aren);
                assert(SUCCESS == r);
            }

            AMP_FACTORY_Release(h->aren);

            AVIN_TEST_LOGD("close clk.\n");
            AMP_RPC(r, AMP_CLK_Close, h->clk);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy clk.\n");
            AMP_RPC(r, AMP_CLK_Destroy, h->clk);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->clk);

            AVIN_TEST_LOGD("close audDum.\n");
            AMP_RPC(r, AMP_DUM_Close, h->audDum);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy audDum.\n");
            AMP_RPC(r, AMP_DUM_Destroy, h->audDum);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->audDum);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE) {
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VOUT_NT) {
            AVIN_TEST_LOGD("close vout.\n");
            AMP_RPC(r, AMP_VOUT_Close, h->vout);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vout.\n");
            AMP_RPC(r, AMP_VOUT_Destroy, h->vout);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->vout);
            assert(SUCCESS == r);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_CAPTURE_VOUT) {
            AVIN_TEST_LOGD("close vout.\n");
            AMP_RPC(r, AMP_VOUT_Close, h->vout);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vout.\n");
            AMP_RPC(r, AMP_VOUT_Destroy, h->vout);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->vout);
            assert(SUCCESS == r);

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                if (AMP_IDLE == test_avin_getAdecState(h)) {
                    r = test_adec_register_event(h->hListener, h->adec, FALSE, h);
                    AVIN_TEST_LOGD("close adec.\n");
                    AMP_RPC(r, AMP_ADEC_Close, h->adec);
                    assert(SUCCESS == r);
                }

                if (AMP_LOADED == test_avin_getAdecState(h)) {
                    AVIN_TEST_LOGD("destroy adec.\n");
                    AMP_RPC(r, AMP_ADEC_Destroy, h->adec);
                    assert(SUCCESS == r);
                }
            }

            AMP_FACTORY_Release(h->adec);

            if (h->outputPlane != AMP_DISP_PLANE_PIP) {
                AVIN_TEST_LOGD("close aren.\n");
                AMP_RPC(r, AMP_AREN_Close, h->aren);
                assert(SUCCESS == r);

                AVIN_TEST_LOGD("destroy aren.\n");
                AMP_RPC(r, AMP_AREN_Destroy, h->aren);
                assert(SUCCESS == r);
            }

            AMP_FACTORY_Release(h->aren);

            AVIN_TEST_LOGD("close clk.\n");
            AMP_RPC(r, AMP_CLK_Close, h->clk);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy clk.\n");
            AMP_RPC(r, AMP_CLK_Destroy, h->clk);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->clk);
        } else if (h->outputComp == TEST_AVIN_OUPUT_COMP_VENC_TRANSCODE) {
            AVIN_TEST_LOGD("close venc.\n");
            AMP_RPC(r, AMP_VENC_Close, h->venc);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vout.\n");
            AMP_RPC(r, AMP_VENC_Destroy, h->venc);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->venc);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("close vout.\n");
            AMP_RPC(r, AMP_VOUT_Close, h->vout);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vout.\n");
            AMP_RPC(r, AMP_VOUT_Destroy, h->vout);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->vout);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("close vdec.\n");
            AMP_RPC(r, AMP_VDEC_Close, h->vdec);
            assert(SUCCESS == r);

            AVIN_TEST_LOGD("destroy vdec.\n");
            AMP_RPC(r, AMP_VDEC_Destroy, h->vdec);
            assert(SUCCESS == r);

            AMP_FACTORY_Release(h->vdec);
            assert(SUCCESS == r);
        }
    }

    ret = MV_OSAL_Mutex_Destroy(&(globalVideoMutex));
    assert(S_OK == ret);

    ret = MV_OSAL_Mutex_Destroy(&(globalAudioMutex));
    assert(S_OK == ret);

    AVIN_TEST_LOGD("test_avin_deInit finished\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static INT32 htoi(char s[])
{
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))  {
        i = 2;
    }
    else {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') ||
        (s[i] >= 'a' && s[i] <= 'f') ||
        (s[i] >='A' && s[i] <= 'F');++i) {
        if (tolower(s[i]) > '9') {
            n = 16 * n + (10 + tolower(s[i]) - 'a');
        }
        else {
            n = 16 * n + (tolower(s[i]) - '0');
        }
    }
    return n;
}

static UINT32 test_avin_parse_argv(char *pCmd, TEST_AVIN_HLD *h)
{
    char pStr[1024*128];
    char *p = pStr;
    int cmdcnt = 0;

    if( NULL == pCmd){
        return 0;
    }
    strcpy(pStr, pCmd);
    p = strtok(p, " ");
    while( NULL != p){
        strcpy(h->pcommand[cmdcnt++], p);
        p = strtok(NULL, " ");
    }
    return cmdcnt;
}

static HRESULT test_avin_change_source(TEST_AVIN_HLD *h,
    UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_SOURCE_TYPE source;
    TEST_AVIN_HLD *pHld = h;
    int cmdnums = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        source = htoi(h->pcommand[0]);
    } else {
        source = test_avin_get_input(h);
    }

    if( source == AMP_AVIN_SOURCE_TYPE_INVALID){
        TEST_AVIN_PRINT("leave invalid Source, r = 0x%x\n", r);
        return r;
    }

    h->curSrc = source;
    TEST_AVIN_PRINT("Set source to %s\n", test_avin_inputSourceString(source));
    AMP_RPC(r, AMP_AVIN_SetCurrentSource, h->avin, source);

    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        if (AMP_AVIN_SOURCE_TYPE_ATV_01 == source
                || AMP_AVIN_SOURCE_TYPE_ATV_02 == source) {
            AMP_AVIN_SIF_STANDARD sifStd = AMP_AVIN_SIF_STD_MONO45
                                         | AMP_AVIN_SIF_STD_MONO55
                                         | AMP_AVIN_SIF_STD_MONO60
                                         | AMP_AVIN_SIF_STD_MONO65;
            /* Or change to: (65 and 65_AML should not be detected together)
            AMP_AVIN_SIF_STANDARD sifStd = AMP_AVIN_SIF_STD_MONO45
                                         | AMP_AVIN_SIF_STD_MONO55
                                         | AMP_AVIN_SIF_STD_MONO60
                                         | AMP_AVIN_SIF_STD_MONO65_AML;
             */
            AMP_RPC(r, AMP_AVIN_DetectSifStandard, h->avin, sifStd);
            if (SUCCESS != r) {
                AMPLOGE("failed to call AMP_AVIN_DetectSifStandard. r = 0x%x\n", r);
            } else {
                h->bDetSifStd = TRUE;
            }
        }

        /**
         * If the current source is AUD_ONLY for CYPRESS and the audio is not
         * (PCM, 48K, 2 chs and 16 bits), it needs to configure the audio parameters
         * using AMP_AVIN_SetAudioParams after the source is changed.
         * Currently, (PCM 2 chs) and (AC3 2 chs / 5.1 chs) are supported.
         * Here is just an example. Actually, They should be configured according to
         * the actual input audio data.
         *
         * The user only needs to set a correct audio sample frequency. Default
         * bit depth and channels are used, and audio format is detected by the
         * low level driver.
         */
        if (AMP_AVIN_SOURCE_TYPE_AUD_ONLY == h->curSrc) {
            AMP_AVIN_INPUT_AUDIO_PARAMS audioParams;
            AmpMemClear(&audioParams, sizeof(AMP_AVIN_INPUT_AUDIO_PARAMS));
            audioParams.sampleRate = 48000;
            AMP_RPC(r, AMP_AVIN_SetAudioParams, h->avin, &audioParams);
            if (SUCCESS != r) {
                AMPLOGE("failed to call AMP_AVIN_SetAudioParams. r = 0x%x\n", r);
            }
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

 static HRESULT test_avin_cmd_dump_audios(TEST_AVIN_HLD *h,
    UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    char *path = NULL;

    AVIN_TEST_LOGD("enter");
    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        path = h->pcommand[0];
    }

    r = test_avin_dumpFrames(h, AMP_AVIN_DEBUG_DUMP_DATA_AUDIO, path);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_cmd_dump_video(TEST_AVIN_HLD *h,
    UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    char *path = NULL;
    UINT8 *defaultPath = "/data";
    UINT8 filePath[1024] = {0};
    UINT8 file[1024] = {0};
    UINT32 uiBufHandler = 0, uiSize = 4096*2160*(36+7)/8;
    AMP_AVIN_INPUT_PARAMS inputParams;
    AMP_AVIN_RECT rect = {0, 0, 4096, 2160};


    AVIN_TEST_LOGD("enter");
    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        path = h->pcommand[0];
    }

    if( NULL == path && NULL == test_avin_getUSBDir(filePath)) {
        strcpy(filePath, defaultPath);
    }

    AmpMemClear((VOID*) &inputParams, sizeof(AMP_AVIN_INPUT_PARAMS));
    AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);
    if (SUCCESS != r) {
        return ERR_ERRSYSCALL;
    }
    if (AMP_AVIN_SOURCE_STATE_STABLE != inputParams.state) {
        TEST_AVIN_PRINT("Signal not stable %d!\n", inputParams.state);
        return r;
    }

    AmpMemcpy(&h->videoParams, &inputParams.videoParams,
            sizeof(AMP_AVIN_INPUT_VIDEO_PARAMS));
    AmpMemcpy(&h->audioParams, &inputParams.audioParams,
            sizeof(AMP_AVIN_INPUT_AUDIO_PARAMS));

    rect.x = 0;
    rect.y = 0;
    rect.w = h->videoParams.videoWidth;
    rect.h = h->videoParams.videoHeight;
    if (AMP_AVIN_COLOR_FORMAT_TYPE_YUV420 == inputParams.videoParams.colorFormat) {
        rect.w = h->videoParams.videoWidth/2;
    }
    uiSize = rect.w*rect.h*(36+7)/8;

    sprintf(file, "%s/vip_buffer_%s.uyvy", filePath,
            test_avin_get_videopreffilename(h));

    AMP_SHM_Allocate(AMP_SHM_VIDEO_FB, uiSize, 1024, &uiBufHandler);

    TEST_AVIN_PRINT("video rect (x, y, w, h) = (%d, %d, %d, %d)\n",
            rect.x, rect.y, rect.w, rect.h);
    AMP_RPC(r, AMP_AVIN_GetVideoFrame, h->avin, &rect, uiBufHandler, &uiSize);
    if (SUCCESS == r) {
        VOID *pAddr = NULL;
        AMP_SHM_GetVirtualAddress(uiBufHandler, 0, & pAddr);
        if (NULL == pAddr) {
            AMP_SHM_Release(uiBufHandler);
            return ERR_ERRSYSCALL;
        }
        FILE *fp = fopen(file, "wb");
        if (NULL != fp) {
            fwrite(pAddr, 1, uiSize, fp);
            fflush(fp);
            fclose(fp);
            TEST_AVIN_PRINT("write (%d) bytes into the file %s!\n", uiSize, file);
        }
    }

    AMP_SHM_Release(uiBufHandler);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_cmd_dump_videos(TEST_AVIN_HLD *h,
    UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    char *path = NULL;

    AVIN_TEST_LOGD("enter");
    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        path = h->pcommand[0];
    }

    r = test_avin_dumpFrames(h, AMP_AVIN_DEBUG_DUMP_DATA_VIDEO, path);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_cmd_dump_vbis(TEST_AVIN_HLD *h,
    UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    char *path = NULL;

    AVIN_TEST_LOGD("enter");
    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        path = h->pcommand[0];
    }

    r = test_avin_dumpFrames(h, AMP_AVIN_DEBUG_DUMP_DATA_VBI, path);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_get_database(TEST_AVIN_REG_MODE mode,
    TEST_AVIN_REG **pOut, int *pSize)
{
    HRESULT r = SUCCESS;

    if( TEST_AVIN_REG_CYPRESS == mode){
        *pOut = global_cypress_freg;
        *pSize = sizeof(global_cypress_freg) / sizeof(TEST_AVIN_REG);
    } else if( TEST_AVIN_REG_AVIF_ANALOG == mode){
        *pOut = global_avif_reg;
        *pSize = sizeof(global_avif_reg) / sizeof(TEST_AVIN_REG);
    } else if( TEST_AVIN_REG_AVIF_HDMIRX1 == mode){
        *pOut = global_AVIF_HDMIRX1_regs;
        *pSize = sizeof(global_AVIF_HDMIRX1_regs) / sizeof(TEST_AVIN_REG);
    } else if( TEST_AVIN_REG_AVIF_HDMIRX2 == mode){
        *pOut = global_AVIF_HDMIRX2_regs;
        *pSize = sizeof(global_AVIF_HDMIRX2_regs) / sizeof(TEST_AVIN_REG);
    } else if( TEST_AVIN_REG_AVIF_HDMIRX3 == mode){
        *pOut = global_AVIF_HDMIRX3_regs;
        *pSize = sizeof(global_AVIF_HDMIRX3_regs) / sizeof(TEST_AVIN_REG);
    } else if( TEST_AVIN_REG_AVIF_HDMIRX4 == mode){
        *pOut = global_AVIF_HDMIRX4_regs;
        *pSize = sizeof(global_AVIF_HDMIRX4_regs) / sizeof(TEST_AVIN_REG);
    }
    return S_OK;
}

static HRESULT test_avin_get_filename(TEST_AVIN_HLD *h,
    TEST_AVIN_REG_MODE mode, const char *pPath, char *pFullName)
{
    HRESULT r = SUCCESS;

    if( TEST_AVIN_REG_CYPRESS == mode){
        sprintf(pFullName, "%s/cypressreg_%s.txt", pPath,
            test_avin_get_videopreffilename(h));
    } else if( TEST_AVIN_REG_AVIF_ANALOG == mode){
        sprintf(pFullName, "%s/avif_anal_%s.txt", pPath,
            test_avin_get_videopreffilename(h));
    } else if( TEST_AVIN_REG_AVIF_HDMIRX1 == mode){
        sprintf(pFullName, "%s/avif_hrx1_%s.txt", pPath,
            test_avin_get_videopreffilename(h));
    } else if( TEST_AVIN_REG_AVIF_HDMIRX2 == mode){
        sprintf(pFullName, "%s/avif_hrx2_%s.txt", pPath,
            test_avin_get_videopreffilename(h));
    } else if( TEST_AVIN_REG_AVIF_HDMIRX3 == mode){
        sprintf(pFullName, "%s/avif_hrx3_%s.txt", pPath,
            test_avin_get_videopreffilename(h));
    } else if( TEST_AVIN_REG_AVIF_HDMIRX4 == mode){
        sprintf(pFullName, "%s/avif_hrx4_%s.txt", pPath,
            test_avin_get_videopreffilename(h));
    }
    return S_OK;
}

/*Search register by address*/
static HRESULT test_avin_findreg(TEST_AVIN_HLD *h,
    UINT32 addr, TEST_AVIN_REG *pReg)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_REG *pTestAvinReg = NULL;
    int database = 0;
    UINT32 i =0, size = 0, tmpAddr;

    for(database = TEST_AVIN_REG_AVIF_ANALOG; database < TEST_AVIN_REG_MAX; database ++){

        tmpAddr = ( database >= TEST_AVIN_REG_AVIF_HDMIRX1 ) ?
            addr - AVIF_HDMIRX_BASE : addr;

        r = test_avin_get_database(database, &pTestAvinReg, &size);
        for(i =0; i<size; i++){
            if( tmpAddr == pTestAvinReg[i].addr ){
                pReg->addr = addr;
                pReg->depth = pTestAvinReg[i].depth;
                strcpy(pReg->reg_name, pTestAvinReg[i].reg_name);
                return S_OK;
            }
        }
    }
    pReg->depth = AMP_AVIN_REG_32BIT;
    strcpy(pReg->reg_name, "NA");
    return S_FALSE;
}

/*Search registers by name*/
static UINT32 test_avin_findregs(TEST_AVIN_HLD *h, char *pName, TEST_AVIN_REG *pReg)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_REG *pTestAvinReg = NULL;
    int size = 0, i =0;
    UINT32 counter = 0, database;

    for(database = TEST_AVIN_REG_AVIF_ANALOG; database < TEST_AVIN_REG_MAX; database ++){
        r = test_avin_get_database(database, &pTestAvinReg, &size);

        for(i =0; i<size; i++){
             if( NULL != strstr( pTestAvinReg->reg_name, pName)){
                memcpy(pReg, pTestAvinReg, sizeof(TEST_AVIN_REG));
                AVIN_TEST_LOGD("Found reg %s(%x)\n", pTestAvinReg->reg_name, pTestAvinReg->addr);
                if( database >= TEST_AVIN_REG_AVIF_HDMIRX1 ){
                    pReg->addr += AVIF_HDMIRX_BASE;
                }
                pReg ++;
                if(counter++ >= READMAX_REGS){
                    printf("Found %d matched registers, limit to %d\n",counter, READMAX_REGS);
                    break;
                }
             }
             pTestAvinReg ++;
        }
    }
    AVIN_TEST_LOGD("Found %d registers by name[%s]\n", counter, pName);
    return counter;
}

static HRESULT test_avin_dump_avinreg(TEST_AVIN_HLD *h,
    TEST_AVIN_REG_MODE mode, UINT8 *path)
{
    HRESULT r = SUCCESS;
    UINT8 *defaultPath = "/data";
    UINT8 fullPath[1024] = {0};
    UINT8 *pathName = NULL;
    FILE *fp = NULL;
    UINT32 uiIdx = 0, uiSize = 0;
    TEST_AVIN_REG *pReg = NULL;
    int cmdnums = 0;
    int percent = 0;
    char strsource[255];

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(path, h);
    if( cmdnums > 0){
        pathName = h->pcommand[0];
    }

    if( NULL == pathName){
        pathName = test_avin_getUSBDir(fullPath);
        if (NULL == pathName) {
            pathName = defaultPath;
        }
    }

    test_avin_get_database(mode, &pReg, &uiSize);
    test_avin_get_filename(h, mode, pathName, fullPath);

    if( test_avin_isHdmisource( h->curSrc))
    {
        sprintf(strsource, "%s", test_avin_convert2HDMIPortID(h, h->curSrc));
    }
    else
    {
        sprintf(strsource, "%s", test_avin_inputSourceString(h->curSrc));
    }


    fp = fopen(fullPath, "w");
    if (NULL == fp) {
        TEST_AVIN_PRINT("failed to open file: %s\n", fullPath);
        return ERR_IOFAIL;
    }

    fprintf(fp, "\tDump date: %s, %s\n", __DATE__, __TIME__);
    fprintf(fp, "\tSelected source: %s\n\n", strsource);
    fprintf(fp, "%-4s\t%-40s\t%-18s\t%s\t%s\n", "No.", "Name", "Address", "Default","Current");
    printf("\033[?25l");/*hide cursor*/

    for (uiIdx = 0; uiIdx < uiSize; uiIdx++) {
        AMP_AVIN_REGISTER reg = {0, 0, 0};
       if(  mode < TEST_AVIN_REG_MAX ){
            UINT32 finalAddr = 0;

            if( mode == TEST_AVIN_REG_AVIF_ANALOG){
                reg.addr = pReg->addr;
                reg.depth = pReg->depth;
                AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg);

                finalAddr = AVIF_BASE_ADDRESS + (pReg->addr << 2);
                fprintf(fp, "%-4d\t%-40s\t0x%04x(0x%x)\t0x%02x\t0x%02x %c\n",
                        uiIdx + 1,
                        pReg->reg_name,
                        pReg->addr,
                        finalAddr,
                        pReg->value,
                        reg.value,
                        (SUCCESS != r)? '*':' ');

            } else if(mode >= TEST_AVIN_REG_AVIF_HDMIRX1){
                finalAddr= AVIF_HDMIRX_BASE + pReg->addr;
                reg.addr = finalAddr;
                reg.depth = pReg->depth;

                AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg);
                fprintf(fp, "%-4d\t%-40s\t0x%04x(0x%x)\t0x%08x\t0x%08x %c\n",
                        uiIdx + 1,
                        pReg->reg_name,
                        pReg->addr,
                        finalAddr,
                        pReg->value,
                        reg.value,
                        (SUCCESS != r)? '*':' ');

            }

        } else {
            reg.addr = pReg->addr;
            AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg);
            fprintf(fp, "%-4d\t%-40s\t0x%04x\t0x%02x\t0x%02x %c\n",
                    uiIdx + 1,
                    pReg->reg_name,
                    pReg->addr,
                    pReg->value,
                    reg.value,
                    (SUCCESS != r)? '*':' ');
        }
        pReg ++;
        percent = 100*uiIdx/uiSize;

        TEST_AVIN_PRINT("\r%02d%%", percent);
    }
    TEST_AVIN_PRINT("\rCapture AVIN register to %s - done", fullPath);
    printf("\033[?25h");/*enable cursor*/

    fclose(fp);
    system("sync");

    TEST_AVIN_PRINT("\rCapture AVIN register to%s %s %s - done\n",
        LOG_COLOR_GREEN, fullPath, LOG_COLOR_AUTO);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_dump_avinregs(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    char *path = pArgv;
    HRESULT r = SUCCESS;

    if( 0 == strcmp(h->chip_name, "cypress")){
        r = test_avin_dump_avinreg(h, TEST_AVIN_REG_CYPRESS, path);
    }else if( 0 == strcmp(h->chip_name, "AVIF")){
        r = test_avin_dump_avinreg(h, TEST_AVIN_REG_AVIF_ANALOG, path);
        r = test_avin_dump_avinreg(h, TEST_AVIN_REG_AVIF_HDMIRX1, path);
        r = test_avin_dump_avinreg(h, TEST_AVIN_REG_AVIF_HDMIRX2, path);
        r = test_avin_dump_avinreg(h, TEST_AVIN_REG_AVIF_HDMIRX3, path);
        r = test_avin_dump_avinreg(h, TEST_AVIN_REG_AVIF_HDMIRX4, path);
    }
    return r;
}

static UINT32 test_avin_get_regs(
    TEST_AVIN_HLD *h, char *pIn, TEST_AVIN_REG *pOut)
{
    char *pTmp = NULL;
    char pRegName[255];
    int dumpdepth = 1;
    UINT32 i =0, regnum=0, addr = 0, steps = 4;

    pTmp = strstr(pIn, ":");
    if( pTmp != NULL){
        sscanf(pIn, "%[^:]:%d", pRegName, &dumpdepth);
    } else {
        sscanf(pIn, "%s", pRegName);
    }

    regnum = test_avin_findregs(h, pRegName, pOut);
    if(regnum == 0) {
        sscanf(pRegName, "%x", &addr);
        regnum = dumpdepth;
        if( regnum > READMAX_REGS ){
            printf("Register dumps exceed the limit %d, exit\n", READMAX_REGS);
        } else {
            printf("Dump register 0x%x @ %d\n", addr, regnum);
            for(i=0; i<regnum; i++ ){
                TEST_AVIN_REG reg;

                addr += i*steps;
                test_avin_findreg(h, addr, &reg);
                strcpy(pOut->reg_name, reg.reg_name);
                pOut->addr = addr;
                pOut->depth = reg.depth;
                pOut++;
            }
        }
    }
    return regnum;
}

static HRESULT test_avin_print_regs(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    char pCmd[READMAX_REGS*128], *p = NULL;
    UINT32 i = 0, tnum = 0;
    TEST_AVIN_REG reg[READMAX_REGS], *pReg = NULL;
    AMP_AVIN_REGISTER avinReg;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        strcpy(pCmd, pArgv);
    } else {
        memset(pCmd, 0, sizeof(pCmd));
        while(strlen(pCmd) == 0){
            printf("Registers:");
            gets(pCmd);
        }
    }
    p = strtok(pCmd, " ");

    while( NULL != p ){
        tnum += test_avin_get_regs(h, p, reg);
        pReg += tnum;
        p = strtok(NULL, " ");
    }

    for( i=0; i< tnum; i++){
        avinReg.addr = reg[i].addr;
        avinReg.depth = reg[i].depth;
        AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &avinReg);
        if(avinReg.depth == AMP_AVIN_REG_8BIT){
            printf("Read 0x%04x %30s -> 0x%02x\n", avinReg.addr, reg[i].reg_name, avinReg.value);
        } else if(avinReg.depth == AMP_AVIN_REG_16BIT){
            printf("Read 0x%04x %30s -> 0x%04x\n", avinReg.addr, reg[i].reg_name, avinReg.value);
        } else {
            printf("Read 0x%04x %30s -> 0x%08x\n", avinReg.addr, reg[i].reg_name, avinReg.value);
        }
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

/*usage example:
disaplay register name, every 100 ms, dump registers 0x56 to 0x56+10
    ampclient_alpha 7 rr mode:1 t:100 56:10

dump register address, every 200 ms, dump registers 0x56 to 0x56+10, 0x78+20
    ampclient_alpha 7 rr mode:0 t:200 56:10 78:20

dump both register address and value, dump registers 0x56 0x10 0x78 0x20
    ampclient_alpha 7 rr mode:2 56 10 78 20

*/
static HRESULT test_avin_reg_monitor(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    char pCmd[READMAX_REGS*128], *p = NULL, pCon[256];
    TEST_AVIN_REG reg[READMAX_REGS];
    TEST_AVIN_REG *pReg = NULL;
    UINT32 cmdnums = 0, delay = 1, steps = 1, i =0, tnum = 0, cnt = 0;
    BOOL dumpmode = 2, scrolling = FALSE;

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums == 0){
        printf("rm help \n");
        printf("Usage:           rr [dumpmode:MODE] [scroll] "
                "[t:DUR] [REGA]... [REGB:REGNUMS]...\n");
        printf("[mode:MODE]: register display mode. MODE=0: address MODE=1: ");
        printf("register name MODE=2:both, MODE=3:dump one register in one line"
               "default value is 0.\n");
        printf("[scroll]: if this opition is selected, then will dump"
                "registe in scrolling mode\n");
        printf("[bg2]: if this opition is selected, then will dump"
                "bg2 registers\n");
        printf("[t:DUR]:         register refresh duration. DUR unit is ms, "
                "default is 20ms\n");
        printf("[REGA]:          REGA is register address in hex, support "
                "multiple register address\n");
        printf("[REGB:REGNUMS]:  REGB is the start address, REGNUMS is the "
                "register numbers need to dump\n");

        printf("Example:         rr t:100 dumpmode:2 56:10 10 1700:20 78 20\n");
        printf("                 Refesh every 100 ms, dump register 0x56, 0x57, "
                "0x58...0x66,0x10 0x1700\n");
        printf("                 0x1701...0x1720, 0x78 0x20, will display both "
                "register name and address\n\n");

        memset(pCmd, 0, sizeof(pCmd));
        while(strlen(pCmd) == 0){
            printf("Registers:");
            gets(pCmd);
        }
    } else {
        strcpy(pCmd, pArgv);
    }
    p = strtok(pCmd, " ");
    pReg = reg;

    while( NULL != p ){
        char *pTmp = NULL;
        char pRegName[255];
        UINT32 temp = 0;

        pTmp = strstr(p, "t:");
        if( pTmp != NULL){
            sscanf(p, "t:%d", &delay);
            printf("Set dump interval to %d ms\n", delay);
            p = strtok(NULL, " ");
            continue;
        }

        pTmp = strstr(p, "mode:");
        if( pTmp != NULL){
            sscanf(p, "mode:%d", &dumpmode);
            printf("Set dump mode to %d\n", dumpmode);
            if( dumpmode == 3 ) delay = 0;
            p = strtok(NULL, " ");
            continue;
        }

        pTmp = strstr(p, "scroll");
        if( pTmp != NULL){
            printf("Enable scrolling while dump\n");
            scrolling = TRUE;
            p = strtok(NULL, " ");
            continue;
        }

        pReg += tnum;
        tnum += test_avin_get_regs(h, p, pReg);

        p = strtok(NULL, " ");

    }

    test_avin_keyevent_start(h);
    h->enableLog = FALSE;

    if( !scrolling ){
        printf("\033[2J??");/*clear screen*/
        printf("\033[?25l");/*hide cursor*/
    }

    while( !h->keyEvent ){
        AMP_AVIN_REGISTER reg_tmp;
        BOOL changed = FALSE;
        char *pName = NULL;
        char pRegname[16];

        if( !scrolling ){
            printf("\033[0;0H");/*set cursor to (0,0)*/
            printf("Register dump, 'ENTER' key to stop\n");
        }

        for( i=0; i< tnum; i++){

            reg_tmp.addr = reg[i].addr;
            AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg_tmp);
            pName = reg[i].reg_name;
            if( NULL == pName){
                sprintf(pRegname, "NA(%X)", reg[i].addr);
                pName = pRegname;
            }
            changed = ( reg_tmp.value != reg[i].value);
            reg[i].value = reg_tmp.value;
            if( changed && !scrolling ){
                 printf(LOG_COLOR_RED);
            }

            if(  !scrolling){
                if( dumpmode == 0){
                    if( i % 16 ==0 ) printf("\n");
                    printf("%04X=%08X ", reg[i].addr, reg[i].value);
                } else if( dumpmode == 1){
                    if( i % 5 ==0 ) printf("\n");
                    printf("%28s=%08X ", pName, reg[i].value);
                } else {
                    if( i % 4 ==0 ) printf("\n");
                    printf("%30s(%04X)=%08X ", pName, reg[i].addr, reg[i].value);
                }
            } else {
                if( dumpmode == 0){
                    printf("%04X=%08X ", reg[i].addr, reg[i].value);
                } else if( dumpmode == 1){
                    printf("%s=%08X ", pName, reg[i].value);
                } else if( dumpmode == 2){
                    printf("%s(%04X)=%08X ", pName, reg[i].addr, reg[i].value);
                }
            }
            printf(LOG_COLOR_AUTO);
            MV_OSAL_Task_Sleep(delay);
        }
        if( scrolling ) printf("\n\nDump%d-> ", cnt++);
    }
    printf("\n");
    printf("\033[?25h");/*enable cursor*/
    h->enableLog = TRUE;
    test_avin_keyevent_stop(h);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set_avinreg(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_REGISTER reg = {0, 0xff, 0}, regPrev = {0, 0xff, 0};
    TEST_AVIN_REG pRegs[1024];
    TEST_AVIN_REG tReg;
    UINT32 cmdnums = 0;
    char *pName = NULL;
    UINT8 tmpVal = 0xff;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 1){
        int addr = 0, regnum=0;
        regnum = test_avin_findregs(h, h->pcommand[0], pRegs);
        if(regnum > 0){
            if(regnum > 1){
                TEST_AVIN_PRINT("Warnning: More than 1 registers are found, "\
                    "will write first one.\n", h->pcommand[0]);
            }
            reg.addr = pRegs[0].addr;
        } else {
            reg.addr = htoi(h->pcommand[0]);
        }
        reg.value = htoi(h->pcommand[1]);
    } else {
        TEST_AVIN_PRINT("Register Address:");
        scanf("%x", &reg.addr);
        TEST_AVIN_PRINT("Register Value:");
        scanf("%x", &reg.value);
    }

    test_avin_findreg(h, reg.addr, &tReg);

    regPrev.addr = reg.addr;
    regPrev.depth = reg.depth = tReg.depth;

    TEST_AVIN_PRINT("regPrev = %x, depth=%d\n", regPrev.addr, regPrev.depth);
    TEST_AVIN_PRINT("reg = %x, depth=%d\n", reg.addr, reg.depth);

    AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &regPrev);
    AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, &reg);

    TEST_AVIN_PRINT("Write 0x%04x %s -> 0x%x[%x]\n",
          reg.addr, tReg.reg_name, reg.value, regPrev.value);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set_avinreg_loop(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_REGISTER reg = {0, 0xff, 0};
    UINT32 cmdnums = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 1){
        reg.addr = htoi(h->pcommand[0]);
        reg.value = htoi(h->pcommand[1]);
    } else {
        TEST_AVIN_PRINT("Register Address:");
        scanf("%x", &reg.addr);
        TEST_AVIN_PRINT("Register Value:");
        scanf("%x", &reg.value);
    }

    test_avin_keyevent_start(h);
    while(!h->keyEvent){
        MV_OSAL_Task_Sleep(100);
        AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, &reg);
        TEST_AVIN_PRINT("Write *[0x%x] = 0x%x\n", reg.addr, reg.value);
    }
    test_avin_keyevent_stop(h);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

/*
Usage: tw [auto:xx] [reg]
auto:xx: will try to set register every xx mseconds.
reg: register address
*/
static HRESULT test_avin_set_avinreg_try(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_REGISTER reg = {0, 0xff, 0};
    UINT32 cmdnums = 0;
    UINT8 orgValue = 0;
    int i = 0;
    BOOL automode = FALSE;
    char *p = NULL;
    char pCmd[1024];
    int delay = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        strcpy(pCmd, pArgv);
    } else {
        TEST_AVIN_PRINT("Register Address:");
        gets(pCmd);
    }

    p = strtok(pCmd, " ");
    while( NULL != p ){
        if( strstr(p, "auto") != NULL){
            automode = TRUE;
            sscanf(p, "auto:%d", &delay);
            p = strtok(NULL, " ");
        } else {
            reg.addr = htoi(p);
            p = strtok(NULL, " ");
        }
    }

    if( TRUE == automode){
        TEST_AVIN_PRINT("Will set register 0x%x from 0x00 to "
            "0xFF automatically, duration=%d\n", reg.addr, delay);
    } else {
        TEST_AVIN_PRINT("Set register 0x%x from 0x00 to 0xFF, "
            "'enter' key to try next\n", reg.addr);
    }

    AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg);
    orgValue = reg.value;
    TEST_AVIN_PRINT("Read register 0x%x = 0x%x\n", reg.addr, orgValue);

    if( TRUE == automode ){
        test_avin_keyevent_start(h);
    }

    for( i = 0; i<=0xFF; i++){
        reg.value = i;
        AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, &reg);
        TEST_AVIN_PRINT("Try *[0x%x] -> 0x%x", reg.addr, reg.value);
        if( FALSE == automode ){
            if( getchar() == 'x'){
                TEST_AVIN_PRINT("\nExit key pressed, exit\n");
                break;
            }
        } else {
            if(h->keyEvent){
                TEST_AVIN_PRINT("\nKey pressed, exit\n");
                break;
            } else {
                TEST_AVIN_PRINT("\n");
                MV_OSAL_Task_Sleep(delay);
            }
        }
    }
    TEST_AVIN_PRINT("\nWrite back 0x%x -> 0x%x\n", reg.addr, orgValue);
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_dump_avin(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    char foldPath[255];
    char filepath[255];
    char cmd[255];
    int index = 0;
    UINT32 ret = 0xFF;

    AVIN_TEST_LOGD("enter\n");

    if(NULL == test_avin_getUSBDir(foldPath)){
        strcpy(foldPath, "/data");
    }

    while(0 !=ret ){
        sprintf(filepath, "%s/AVIN_DUMP_%d", foldPath, index++);
        sprintf(cmd, "mkdir %s", filepath);
        ret = system(cmd);
    }

    TEST_AVIN_PRINT("\nStart AVIN dump, filepath=%s\n", filepath);

    test_avin_cmd_dump_video(h, filepath);
    test_avin_dump_avinregs(h, filepath);

    TEST_AVIN_PRINT("All dump action is done, files had been put to %s\n", filepath);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set3dcomb_2d(TEST_AVIN_HLD *h,
    UINT8 *pArgv)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("enter\n");

    if(test_avin_is656source(h->curSrc) ) {
        TEST_AVIN_PRINT("Set CVBS 3D comb to Force2D\n");
        AMP_RPC(r, AMP_AVIN_Set3DComb, h->avin, AMP_AVIN_CombFilter_eForce2D);
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set3dcomb_3d(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    if(test_avin_is656source(h->curSrc)) {
        TEST_AVIN_PRINT("Set CVBS 3D comb to Force3D\n");
        AMP_RPC(r, AMP_AVIN_Set3DComb, h->avin, AMP_AVIN_CombFilter_eForce3D);
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set3dcomb_3dAdap(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("enter\n");

    if(test_avin_is656source(h->curSrc)) {
        TEST_AVIN_PRINT("Set CVBS 3D comb to 3DAdaptive\n");
        AMP_RPC(r, AMP_AVIN_Set3DComb, h->avin, AMP_AVIN_CombFilter_e3DAdaptive);
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_enable_vbidump(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("enter\n");
    h->enableDumpVbiData = TRUE;

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_disable_vbidump(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("enter\n");
    h->enableDumpVbiData = FALSE;

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_getfetg(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_FETG_PARAMS fetgParams;

    AVIN_TEST_LOGD("enter\n");

    AmpMemClear((VOID*) &fetgParams, sizeof(AMP_AVIN_FETG_PARAMS));
    AMP_RPC(r, AMP_AVIN_GetFetgParams, h->avin, &fetgParams);

    TEST_AVIN_PRINT("FETG(%dx%d%c@%fHz)\n",
            fetgParams.end_x - fetgParams.start_x,
            fetgParams.end_y - fetgParams.start_y,
            fetgParams.field_flag? 'i':'p',
#if ( BERLIN_CHIP_VERSION == BERLIN_BG4_CDP )
            fetgParams.refresh_rate/1000.00);
#else
            fetgParams.refresh_rate);
#endif

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_getpixel(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 pixelValue = 0;
    UINT32 x = 0, y = 0;
    UINT32 cmdnums = 0;
    UINT32 pixelY, pixelCb, pixelCr;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 1){
        x = atoi(h->pcommand[0]);
        y = atoi(h->pcommand[1]);
    } else {
        TEST_AVIN_PRINT("input x y:");
        scanf("%d %d", &x, &y);
    }

    AMP_RPC(r, AMP_AVIN_ReadPixel, h->avin, x&0xFFFE, y, &pixelValue);
    pixelY  = pixelValue>>8;
    pixelCb = pixelValue&0xFF;
    AMP_RPC(r, AMP_AVIN_ReadPixel, h->avin, (x&0xFFFE)+1, y, &pixelValue);
    pixelY  = (pixelY+(pixelValue>>8))/2;
    pixelCr = pixelValue&0xFF;
    TEST_AVIN_PRINT("AMP_AVIN_ReadPixel[%d,%d] = (%d,%d,%d)\n",
            x, y, pixelY, pixelCb, pixelCr);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_getvipstatus(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    INT16 state = -1;

    AVIN_TEST_LOGD("enter\n");

    AMP_RPC(r, AMP_AVIN_GetVipState, h->avin, &state);
    TEST_AVIN_PRINT("VIP status = %d\n", state);
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set_audiomute(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int i =0;

    AVIN_TEST_LOGD("enter\n");
    for (i = 0; i < TEST_AVIN_AREN_OUTPUT_PORTS; i++) {
        r = AMP_SND_SetStreamMute(h->tunnel_handle[i], TRUE);
    }
    if( SUCCESS != r){
        TEST_AVIN_PRINT("Mute audio failed = 0x%x\n", r);
    } else {
        TEST_AVIN_PRINT("Mute audio ok\n");
    }
    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_set_audiounmute(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int i=0;

    AVIN_TEST_LOGD("enter\n");
    for (i = 0; i < TEST_AVIN_AREN_OUTPUT_PORTS; i++) {
        r = AMP_SND_SetStreamMute(h->tunnel_handle[i], FALSE);
    }
    if( SUCCESS != r){
        TEST_AVIN_PRINT("UnMute audio failed = 0x%x\n", r);
    } else {
        TEST_AVIN_PRINT("Unmute audio ok\n");
    }
    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_set_videomute(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_DISP_PLANE_ID uiPlaneId = h->outputPlane;
    int cmdnums = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        uiPlaneId = htoi(h->pcommand[0]);
    }
    AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, uiPlaneId, TRUE);
    if( SUCCESS != r){
        TEST_AVIN_PRINT("Mute video plane %d failed = 0x%x\n", uiPlaneId, r);
    } else {
        TEST_AVIN_PRINT("Mute video plane %d OK\n", uiPlaneId);
    }
    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_set_videounmute(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_DISP_PLANE_ID uiPlaneId = h->outputPlane;
    int cmdnums= 0 ;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        uiPlaneId = htoi(h->pcommand[0]);
    }
    AMP_RPC(r, AMP_DISP_SetPlaneMute, h->voutDisp, uiPlaneId, FALSE);
    if( SUCCESS != r){
        TEST_AVIN_PRINT("UnMute video plane %d failed = 0x%x\n", uiPlaneId, r);
    } else {
        TEST_AVIN_PRINT("UnMute video plane %d OK\n", uiPlaneId);
    }
    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_togglehpd(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    BOOL hpd = 0;
    UINT32 cmdnums = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        hpd = htoi(h->pcommand[0]);
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, hpd);
        TEST_AVIN_PRINT("HPD to %d done\n", hpd);
    } else {
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 0);
        MV_OSAL_Task_Sleep(200);
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 1);
        TEST_AVIN_PRINT("HPD toggling is done\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_vgaautoadjust(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;

    AVIN_TEST_LOGD("enter\n");

    if( h->curSrc == AMP_AVIN_SOURCE_TYPE_VGA_01 ||
        h->curSrc == AMP_AVIN_SOURCE_TYPE_VGA_02) {
        AMP_AVIN_VGA_VIDEO_PROP vgaParams;
        AMP_RPC(r, AMP_AVIN_SetVgaAutoAdjustment, h->avin, &vgaParams);
    } else {
        TEST_AVIN_PRINT("not support in this source\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_readedid(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_EDID_INFO EdidInfo;
    UINT i = 0, j = 0;
    UINT32 cmdnums = 0;

    AmpMemClear(&EdidInfo, sizeof(AMP_AVIN_EDID_INFO));
    cmdnums = test_avin_parse_argv(pArgv, h);

    if( cmdnums >= 1 ){
        EdidInfo.source = htoi(h->pcommand[0]);
    } else {
        EdidInfo.source = h->curSrc;
    }

    if( cmdnums >= 2 ){
        EdidInfo.size = htoi(h->pcommand[1]);
    } else {
        EdidInfo.size = 256;
    }

    AMPLOGD("AMP_AVIN_ReadEdid, src:0x%x size %d\n",
        EdidInfo.source, EdidInfo.size);
    memset(EdidInfo.data, 0 , 256);
    AMP_RPC(r, AMP_AVIN_ReadEdid, h->avin, &EdidInfo);
    EdidInfo.size = 256;

    TEST_AVIN_PRINT("Read %s EDID[%d]:\n",
        test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
    for (i = 0; i < EdidInfo.size/16; i++) {
        for (j = 0; j < 16 && j < (EdidInfo.size - i*16); j++) {
            TEST_AVIN_PRINT("0x%02x ", EdidInfo.data[i*16+j]);
        }
        TEST_AVIN_PRINT("\n");
        if ((i*16+j) == 128) {
            TEST_AVIN_PRINT("\n");
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_writeedid(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT8 filename[128]={0};
    AMP_AVIN_EDID_INFO EdidInfo;
    UINT8 edid_tab[] = {
        0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
        0x21,0x33,0x00,0x00,0x00,0x00,0x00,0x00,
        0x01,0x17,0x01,0x03,0x68,0x8F,0x50,0x78,
        0x0A,0xE7,0xBD,0xA3,0x54,0x4F,0x9E,0x26,
        0x0C,0x47,0x4A,0x2F,0xCE,0x00,0x81,0x80,
        0x95,0x00,0xB3,0x00,0xD1,0xC0,0x01,0x01,
        0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
        0x50,0xB0,0x51,0x00,0x1B,0x30,0x40,0x70,
        0x36,0x00,0xA0,0x5A,0x00,0x00,0x00,0x1E,
        0x0E,0x1F,0x00,0x80,0x51,0x00,0x1E,0x30,
        0x40,0x80,0x37,0x00,0xBA,0x88,0x21,0x00,
        0x00,0x1C,0x00,0x00,0x00,0xFD,0x00,0x32,
        0x4B,0x1E,0x46,0x0F,0x00,0x0A,0x20,0x20,
        0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
        0x00,0x48,0x69,0x73,0x65,0x6E,0x73,0x65,
        0x20,0x47,0x54,0x56,0x0A,0x20,0x00,0x03
    };
    UINT i = 0, j = 0;
    UINT32 cmdnums = 0;

    AmpMemClear(&EdidInfo, sizeof(AMP_AVIN_EDID_INFO));
    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        EdidInfo.source = htoi(h->pcommand[0]);
    } else {
        TEST_AVIN_PRINT("Source:\n");
        scanf("%x", &EdidInfo.source);
    }

    if( cmdnums > 1){
        AmpMemcpy(filename, h->pcommand[1], sizeof(filename));
    } else {
        TEST_AVIN_PRINT("edid file(\"d\" default):\n");
        scanf("%128s",filename);
    }

    FILE *fp = NULL;
    if( 0 != strcmp(filename, "d")){
        fp = fopen(filename, "r");
    }

    if (fp != NULL) {
        fread(&edid_tab, 1, sizeof(edid_tab), fp);
        fclose(fp);
    } else {
        TEST_AVIN_PRINT("Write default EDID\n");
    }

    AmpMemcpy(EdidInfo.data, edid_tab, sizeof(edid_tab));
    EdidInfo.size = sizeof(edid_tab);

    AMPLOGE("AMP_AVIN_UpdateEdid, src:0x%x size %d\n",
        EdidInfo.source, EdidInfo.size);
    AMP_RPC(r, AMP_AVIN_UpdateEdid, h->avin, &EdidInfo);

    if (SUCCESS == r) {
        TEST_AVIN_PRINT("Write %s EDID[%d] complete\n",
            test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
    } else {
        TEST_AVIN_PRINT("Write %s EDID[%d] error 0x%x\n",
            test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
    }

    TEST_AVIN_PRINT("Write %s EDID[%d]:\n",
        test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
    for (i = 0; i < EdidInfo.size/16; i++) {
        for (j = 0; j < 16 && j < (EdidInfo.size - i*16); j++) {
            TEST_AVIN_PRINT("0x%2x ", EdidInfo.data[i*16+j]);
        }
        TEST_AVIN_PRINT("\n");
        if ((i*16+j) == 128) {
            TEST_AVIN_PRINT("\n");
        }
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setadc(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_ADC_CALIB_DATA ADCParams;

    TEST_AVIN_PRINT("ADC Gain:\n");
    scanf("%f %f %f",
        &ADCParams.gain[0], &ADCParams.gain[1], &ADCParams.gain[2]);
    TEST_AVIN_PRINT("ADC Offset:\n");
    scanf("%f %f %f",
        &ADCParams.offset[0], &ADCParams.offset[1], &ADCParams.offset[2]);

    AMPLOGE("AMP_AVIN_SetAdcGainOffset, gain:%f|%f|%f offset:%f|%f|%f\n",
        ADCParams.gain[0], ADCParams.gain[1], ADCParams.gain[2],
        ADCParams.offset[0], ADCParams.offset[1], ADCParams.offset[2]);
    AMP_RPC(r, AMP_AVIN_SetAdcGainOffset, h->avin, &ADCParams);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_getadc(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_ADC_CALIB_DATA ADCParams;

    AMP_RPC(r, AMP_AVIN_GetAdcGainOffset, h->avin, &ADCParams);

    TEST_AVIN_PRINT("ADC Gain: %f %f %f\n", ADCParams.gain[0],
        ADCParams.gain[1], ADCParams.gain[2]);
    TEST_AVIN_PRINT("ADC Offset: %f %f %f\n", ADCParams.offset[0],
        ADCParams.offset[1], ADCParams.offset[2]);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_hdmiarc(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    BOOL arc;
    int cmdnums = 0;

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        arc = atoi(h->pcommand[0]);
    } else {
        TEST_AVIN_PRINT("Hdmi ARC 0|1:\n");
        scanf("%d", &arc);
    }

    AMPLOGE("A AMP_AVIN_SetHdmiArc, arc:%d\n", arc);
    AMP_RPC(r, AMP_AVIN_SetHdmiArc, h->avin, arc);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_hdcpkey(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_HDCP_KEYS HdcpKey;
    UINT32 i;
    int cmdnums = 0;

    AmpMemClear(&HdcpKey, sizeof(AMP_AVIN_HDCP_KEYS));

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        HdcpKey.source = atoi(h->pcommand[0]);
    } else {
        TEST_AVIN_PRINT("HDMI port id 1~4:\n");
        scanf("%d", &HdcpKey.source);
    }

    AMPLOGE("AMP_AVIN_GetHdcpKeys\n");
    AMP_RPC(r, AMP_AVIN_GetHdcpKeys, h->avin, &HdcpKey);

    TEST_AVIN_PRINT("HDMI[%d] HDCP Key (%d):\n", HdcpKey.source, HdcpKey.length);
    for (i = 0; i < HdcpKey.length; i++) {
        TEST_AVIN_PRINT("0x%x ", HdcpKey.data[i]);
    }
    TEST_AVIN_PRINT("\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_gettermination(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    BOOL enable = FALSE;;
    UINT32 i, source;
    int cmdnums = 0;

    cmdnums = test_avin_parse_argv(pArgv, h);

    AMPLOGE("AMP_AVIN_GetHdmiTermination\n");
    AMP_RPC(r, AMP_AVIN_GetHdmiTermination, h->avin, &enable);

    TEST_AVIN_PRINT("HdmiTermination:%d\n", enable);
    TEST_AVIN_PRINT("\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_settermination(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    BOOL enable = FALSE;;
    UINT32 i, source;
    int cmdnums = 0;

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        enable = atoi(h->pcommand[0]);
    } else {
        TEST_AVIN_PRINT("set termination 1~0:\n");
        scanf("%d", &enable);
    }

    AMPLOGE("AMP_AVIN_GetHdmiTermination\n");
    AMP_RPC(r, AMP_AVIN_SetHdmiTermination, h->avin, enable);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}


static HRESULT test_avin_getequlizergain(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT8 eqGain = 0;
    UINT32 i, source;
    int cmdnums = 0;

    cmdnums = test_avin_parse_argv(pArgv, h);

    AMPLOGE("AMP_AVIN_GetHdmiEqualizerGain\n");
    AMP_RPC(r, AMP_AVIN_GetHdmiEqualizerGain, h->avin, &eqGain);

    TEST_AVIN_PRINT("Get Equalizer Gain:%d\n", eqGain);
    TEST_AVIN_PRINT("\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}


static HRESULT test_avin_setequlizergain(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT8 eqGain = 0;
    UINT32 i, source;
    int cmdnums = 0;

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        eqGain = atoi(h->pcommand[0]);
    } else {
        TEST_AVIN_PRINT("set Equalizer Gain:\n");
        scanf("%d", &eqGain);
    }

    AMPLOGE("AMP_AVIN_SetHdmiEqualizerGain\n");
    AMP_RPC(r, AMP_AVIN_SetHdmiEqualizerGain, h->avin, eqGain);


    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}


static HRESULT test_avin_getgmdpkt(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_HDMI_PKT_GMD gmdPkt;
    UINT32 i;
    int cmdnums = 0;

    AmpMemClear(&gmdPkt, sizeof(AMP_AVIN_HDMI_PKT_GMD));

    AMPLOGE("AMP_AVIN_GetGMDPkt\n");
    AMP_RPC(r, AMP_AVIN_GetGMDPkt, h->avin, &gmdPkt);

    TEST_AVIN_PRINT("GMD PKT:\n");
    for (i = 0; i < 28; i++) {
        TEST_AVIN_PRINT("0x%x ", gmdPkt.gbdData[i]);
    }
    TEST_AVIN_PRINT("\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_powerdown(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;

    AMPLOGE("AMP_AVIN_PowerDown\n");
    AMP_RPC(r, AMP_AVIN_PowerDown, h->avin);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}


static HRESULT test_avin_sifdetection(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = h;
    AMP_AVIN_SIF_STANDARD sifStd = AMP_AVIN_SIF_STD_UNKNOWN;
    INT32 iInput = 0;

    TEST_AVIN_PRINT("input 1 to select STD 4.5M, 0 to skip:\n");
    scanf("%d", &iInput);
    if (1 == iInput) {
        sifStd |= AMP_AVIN_SIF_STD_MONO45;
    }
    TEST_AVIN_PRINT("input 1 to select STD 5.5M, 0 to skip:\n");
    scanf("%d", &iInput);
    if (1 == iInput) {
        sifStd |= AMP_AVIN_SIF_STD_MONO55;
    }
    TEST_AVIN_PRINT("input 1 to select STD 6.0M, 0 to skip:\n");
    scanf("%d", &iInput);
    if (1 == iInput) {
        sifStd |= AMP_AVIN_SIF_STD_MONO60;
    }
    TEST_AVIN_PRINT("input 1 to select STD 6.5M, 0 to skip:\n");
    scanf("%d", &iInput);
    if (1 == iInput) {
        sifStd |= AMP_AVIN_SIF_STD_MONO65;
    }
    TEST_AVIN_PRINT("input 1 to select STD 6.5M-AML, 0 to skip:\n");
    scanf("%d", &iInput);
    if (1 == iInput) {
        sifStd |= AMP_AVIN_SIF_STD_MONO65_AML;
    }

    TEST_AVIN_PRINT("%s sifStd = 0x%x\n", __FUNCTION__, sifStd);

    AMP_RPC(r, AMP_AVIN_DetectSifStandard, pHld->avin, sifStd);
    if (SUCCESS != r){
        TEST_AVIN_PRINT("failed to call AMP_AVIN_DetectSifStandard. r=0x%x\n", r);
    }
    return r;
}

static HRESULT test_avin_setsifstandard(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = h;
    AMP_AVIN_SIF_STANDARD sifStd = AMP_AVIN_SIF_STD_UNKNOWN;

    TEST_AVIN_PRINT("0: STD 4.5M\n"
                    "1: STD 5.5M\n"
                    "2: STD 6.0M\n"
                    "3: STD 6.5M\n"
                    "4: STD 6.5M-AML\n");
    TEST_AVIN_PRINT("Please select a SIF standard:");
    scanf("%d", &sifStd);
    if (4 < sifStd) {
        return ERR_ERRPARAM;
    } else {
        sifStd = 1 << sifStd;
    }
    AMP_RPC(r, AMP_AVIN_SetSifStandard, pHld->avin, sifStd);
    if (SUCCESS != r){
        TEST_AVIN_PRINT("failed to call AMP_AVIN_SetSifStandard. r=0x%x\n", r);
    }
    return r;
}

static HRESULT test_avin_sifconfigure(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    TEST_AVIN_HLD *pHld = h;
    AMP_AVIN_SIF_STATUS sifStatus;
    AMP_AVIN_AUD_SIF_MODE sifMode;
    UINT8 *pModeList = NULL;

    AmpMemClear(&(sifStatus), sizeof(AMP_AVIN_SIF_STATUS));
    AMP_RPC(r, AMP_AVIN_GetSifStatus, pHld->avin, &sifStatus);
    if (SUCCESS != r){
        TEST_AVIN_PRINT("failed to call AMP_AVIN_GetSifStandard. r=0x%x\n", r);
        return r;
    }

    if (AMP_AVIN_SIF_STD_MONO45 == sifStatus.standard) {
        pModeList = "0: FM MONO\n"
                    "2: BTSC_MONO\n"
                    "3: BTSC_STEREO\n"
                    "5: SAP_MONO\n"
                    "Please select a BTSC audio mode:";
     } else if (AMP_AVIN_SIF_STD_MONO60 == sifStatus.standard) {
        pModeList = " 0: FM MONO\n"
                    " 8: NICAM_MONO\n"
                    " 9: NICAM_STEREO\n"
                    "10: NICAM_DUAL_MONO1\n"
                    "11: NICAM_DUAL_MONO2\n"
                    "Please select a NICAM audio mode:";
     } else if (AMP_AVIN_SIF_STD_MONO55 == sifStatus.standard) {
        pModeList = " 0: FM MONO\n"
                    " 8: NICAM_MONO\n"
                    " 9: NICAM_STEREO\n"
                    "10: NICAM_DUAL_MONO1\n"
                    "11: NICAM_DUAL_MONO2\n"
                    "17: A2_MONO\n"
                    "18: A2_STEREO\n"
                    "19: A2_DUAL_MONO1\n"
                    "20: A2_DUAL_MONO2\n"
                    "Please select a NICAM/A2 audio mode:";
     } else if (AMP_AVIN_SIF_STD_MONO65 == sifStatus.standard) {
        pModeList = " 0: FM MONO\n"
                    " 8: NICAM_MONO\n"
                    " 9: NICAM_STEREO\n"
                    "10: NICAM_DUAL_MONO1\n"
                    "11: NICAM_DUAL_MONO2\n"
                    "17: A2_MONO\n"
                    "18: A2_STEREO\n"
                    "19: A2_DUAL_MONO1\n"
                    "20: A2_DUAL_MONO2\n"
                    "Please select a NICAM/A2 audio mode:";
    } else if (AMP_AVIN_SIF_STD_MONO65_AML == sifStatus.standard) {
        pModeList = " 1: AM_MONO\n"
                    " 8: NICAML_MONO\n"
                    " 9: NICAML_STEREO\n"
                    "10: NICAML_DUAL_MONO1\n"
                    "11: NICAML_DUAL_MONO2\n"
                    "Please select a NICAM audio mode:";
    } else {
        TEST_AVIN_PRINT("invalid SIF standard: %d\n", sifStatus.standard);
        return ERR_SWSTATEWRONG;
    }

    TEST_AVIN_PRINT("%s", pModeList);
    scanf("%d", &sifMode);
    if (20 < sifMode) {
        TEST_AVIN_PRINT("Error input: %d!\n", sifMode);
        return ERR_ERRPARAM;
    } else {
        sifMode = 1 << sifMode;
    }
    AMP_RPC(r, AMP_AVIN_SetSifMode, pHld->avin, sifMode);
    if (SUCCESS != r) {
        TEST_AVIN_PRINT("failed to call AMP_AVIN_SetSifMode. r=0x%x\n", r);
    } else {
        TEST_AVIN_PRINT("set mode to %s done\n",
                    test_avin_audioSifModeString(sifMode));
    }
    return r;
}

static HRESULT test_avin_getsifstatus(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    AMP_AVIN_SIF_STATUS sifStatus;
    TEST_AVIN_HLD *pHld = h;
    HRESULT r = SUCCESS;

    AMP_RPC(r, AMP_AVIN_GetCurrentSource, pHld->avin, &(pHld->curSrc));
    if (pHld->curSrc == AMP_AVIN_SOURCE_TYPE_ATV_01 ||
            pHld->curSrc == AMP_AVIN_SOURCE_TYPE_ATV_02) {
        AmpMemClear((VOID *)&sifStatus, sizeof(AMP_AVIN_SIF_STATUS));
        AMP_RPC(r, AMP_AVIN_GetSifStatus, pHld->avin, &sifStatus);

        TEST_AVIN_PRINT("%17s: %#x (%s)\n", "Standard", sifStatus.standard,
                test_avin_audioSifStandardString(sifStatus.standard));
        TEST_AVIN_PRINT("%17s: %#x (%s)\n", "Mode", sifStatus.mode,
                test_avin_audioSifModeString(sifStatus.mode));
        TEST_AVIN_PRINT("%17s: %#x (%s)\n", "Supported Modes", sifStatus.uiSupportedModes,
                test_avin_audioSifSupportedModesString(sifStatus.uiSupportedModes));
    } else {
        TEST_AVIN_PRINT("Please run this command under ATV source.\n");
    }

    return r;
}

static HRESULT test_avin_dumpvbiconfigure(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_VBI_CONFIG vbiCfg;
    AMP_AVIN_VBI_LINES_CONFIG vbiLinesCfg;
    char str[256] = {0};
    UINT8 i = 0;

    AmpMemClear(&vbiCfg, sizeof(AMP_AVIN_VBI_CONFIG));
    AmpMemClear(&vbiLinesCfg, sizeof(AMP_AVIN_VBI_LINES_CONFIG));
    AmpMemClear(str, sizeof(str));

    AMP_RPC(r, AMP_AVIN_GetVbiConfig, h->avin, &vbiCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiConfig. r = 0x%x\n", r);
        return r;
    }

    TEST_AVIN_PRINT("VBI TTX%d|WWS%d|NCC%d|PCC%d|CGMS%d\n", vbiCfg.bEnTTX,
        vbiCfg.bEnWSS, vbiCfg.bEnNtscCC, vbiCfg.bEnPalCC, vbiCfg.bEnCGMS);

    AMP_RPC(r, AMP_AVIN_GetVbiLinesConfig, h->avin, &vbiLinesCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiLinesConfig. r = 0x%x\n", r);
        return r;
    }

    AmpMemClear(str, sizeof(str));
    sprintf(str, "%9s", "Teletext:");
    for (i = 0; i < AMP_AVIN_VBI_CONFIG_MAX_LINES; i ++) {
        strcat(str, (vbiLinesCfg.bLinesTTX[i] ? "1" : "-"));
    }
    TEST_AVIN_PRINT("%s\n", str);

    AmpMemClear(str, sizeof(str));
    sprintf(str, "%9s", "WSS:");
    for (i = 0; i < AMP_AVIN_VBI_CONFIG_MAX_LINES; i ++) {
        strcat(str, (vbiLinesCfg.bLinesWSS[i] ? "1" : "-"));
    }
    TEST_AVIN_PRINT("%s\n", str);

    AmpMemClear(str, sizeof(str));
    sprintf(str, "%9s", "NtscCC:");
    for (i = 0; i < AMP_AVIN_VBI_CONFIG_MAX_LINES; i ++) {
        strcat(str, (vbiLinesCfg.bLinesNtscCC[i] ? "1" : "-"));
    }
    TEST_AVIN_PRINT("%s\n", str);

    AmpMemClear(str, sizeof(str));
    sprintf(str, "%9s", "PalCC:");
    for (i = 0; i < AMP_AVIN_VBI_CONFIG_MAX_LINES; i ++) {
        strcat(str, (vbiLinesCfg.bLinesPalCC[i] ? "1" : "-"));
    }
    TEST_AVIN_PRINT("%s\n", str);

    AmpMemClear(str, sizeof(str));
    sprintf(str, "%9s", "CGMS:");
    for (i = 0; i < AMP_AVIN_VBI_CONFIG_MAX_LINES; i ++) {
        strcat(str, (vbiLinesCfg.bLinesCGMS[i] ? "1" : "-"));
    }
    TEST_AVIN_PRINT("%s\n", str);
    return r;
}

static HRESULT test_avin_vbiconfigure(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    char cmdline[256] = {0};
    char *pCmd = NULL, *pTmp = NULL;
    AMP_AVIN_VBI_CONFIG vbiCfg;
    AMP_AVIN_VBI_LINES_CONFIG vbiLinesCfg;
    BOOL *pbLinesCfg = NULL;
    UINT8 startLine = 0, endLine = 0;
    UINT8 i = 0;

    AmpMemClear(&vbiCfg, sizeof(AMP_AVIN_VBI_CONFIG));
    AmpMemClear(&vbiLinesCfg, sizeof(AMP_AVIN_VBI_LINES_CONFIG));
    AmpMemClear(cmdline, sizeof(cmdline));

    if (NULL != pArgv) {
        strcpy(cmdline, pArgv);
    }

    pCmd = cmdline;
    //Paser VBI type
    pCmd = strtok(pCmd, " ");
    if (NULL == pCmd) {
        TEST_AVIN_PRINT("VBI types:\n");
        TEST_AVIN_PRINT("T. Teletext\n");
        TEST_AVIN_PRINT("W. WSS\n");
        TEST_AVIN_PRINT("N. NTSC CC\n");
        TEST_AVIN_PRINT("P. PAL CC\n");
        TEST_AVIN_PRINT("C. CGMS\n");
        TEST_AVIN_PRINT("D. Disable VBI\n");
        TEST_AVIN_PRINT("Q. Quit\n");
        TEST_AVIN_PRINT("Please select a VBI type and the lines"
                " you want to enable:\n"
                "(The range is [6, 25], press \"ENTER\" key to quit)\n");
        gets(cmdline);
        pCmd = cmdline;
        pCmd = strtok(pCmd, " ");
    }

    while (NULL == pCmd) {
        TEST_AVIN_PRINT("Please select a VBI type:");
        gets(cmdline);
        pCmd = cmdline;
        pCmd = strtok(pCmd, " ");
    }

    if ('Q' == toupper(*pCmd)) {
        return r;
    }

    for (i = 0; i < strlen(pCmd); i++) {
        if ('T' == toupper(*(pCmd+i))) {
            vbiCfg.bEnTTX = TRUE;
        } else if ('W' == toupper(*(pCmd+i))) {
            vbiCfg.bEnWSS = TRUE;
        } else if ('N' == toupper(*(pCmd+i))) {
            vbiCfg.bEnNtscCC = TRUE;
        } else if ('P' == toupper(*(pCmd+i))) {
            vbiCfg.bEnPalCC = TRUE;
        } else if ('C' == toupper(*(pCmd+i))) {
            vbiCfg.bEnCGMS = TRUE;
        } else if ('D' == toupper(*(pCmd+i))) {
            AmpMemClear(&vbiCfg, sizeof(AMP_AVIN_VBI_CONFIG));
        } else {
            AMPLOGE("not support vbi type. %c\n", *(pCmd+i));
            return r;
        }
    }

    AMP_RPC(r, AMP_AVIN_SetVbiConfig, h->avin, &vbiCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiConfig. r = 0x%x\n", r);
        return r;
    }

    if (strlen(pCmd) > 1) {
        test_avin_dumpvbiconfigure(h, pCmd);
        return r;
    }

    if ('T' == toupper(*pCmd)) {
        pbLinesCfg = vbiLinesCfg.bLinesTTX;
    } else if ('W' == toupper(*pCmd)) {
        pbLinesCfg = vbiLinesCfg.bLinesWSS;
    } else if ('N' == toupper(*pCmd)) {
        pbLinesCfg = vbiLinesCfg.bLinesNtscCC;
    } else if ('P' == toupper(*pCmd)) {
        pbLinesCfg = vbiLinesCfg.bLinesPalCC;
    } else if ('C' == toupper(*pCmd)) {
        pbLinesCfg = vbiLinesCfg.bLinesCGMS;
    } else if ('D' == toupper(*pCmd)) {
        test_avin_dumpvbiconfigure(h, pCmd);
        return r;
    } else {
        TEST_AVIN_PRINT("VBI types selected: %c\n", *pCmd);
        return r;
    }

    //Parse VBI Lines
    pCmd = strtok(NULL, " ");
    if (NULL == pCmd) {
        test_avin_dumpvbiconfigure(h, pCmd);
        return r;
    }

    AMP_RPC(r, AMP_AVIN_GetVbiLinesConfig, h->avin, &vbiLinesCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiLinesConfig. r = 0x%x\n", r);
        AmpMemClear(&vbiLinesCfg, sizeof(AMP_AVIN_VBI_LINES_CONFIG));
    }
    AmpMemClear(pbLinesCfg, sizeof(BOOL) * AMP_AVIN_VBI_CONFIG_MAX_LINES);

    while ((NULL != pCmd) && ((UINT32)(pCmd - cmdline) < sizeof(cmdline))) {
        pTmp = strstr(pCmd, ":");
        if (NULL != pTmp) {
            sscanf(pCmd, "%d:%d", &startLine, &endLine);
            for (i = startLine;
                    (i < AMP_AVIN_VBI_CONFIG_MAX_LINES) && (i <= endLine); i++){
                pbLinesCfg[i] = TRUE;
            }
        } else {
            i = atoi(pCmd);
            if ((i < 26) && (i >= 6)) {
                pbLinesCfg[i] = TRUE;

            }
        }

        pCmd = strtok(NULL, " ");
    }

    AMP_RPC(r, AMP_AVIN_SetVbiLinesConfig, h->avin, &vbiLinesCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiLinesConfig. r = 0x%x\n", r);
        return r;
    }

    test_avin_dumpvbiconfigure(h, pCmd);

    return r;
}

static HRESULT test_avin_getavinfo(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    AMP_AVIN_INPUT_PARAMS inputParams;
    TEST_AVIN_HLD *pHld = h;
    HRESULT r = SUCCESS;

    AmpMemClear((VOID*) &inputParams, sizeof(AMP_AVIN_INPUT_PARAMS));
    AMP_RPC(r, AMP_AVIN_GetInputParams, pHld->avin, &inputParams);
    AMP_RPC(r, AMP_AVIN_GetCurrentSource, pHld->avin, &(pHld->curSrc));

    test_avin_dumpVideoInfo(h, &inputParams.videoParams);
    test_avin_dumpAudioInfo(h, &inputParams.audioParams);

    return r;
}
static HRESULT test_avin_getaudca(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    UINT16 audCA = 0;
    TEST_AVIN_HLD *pHld = h;
    HRESULT r = SUCCESS;

    AMP_RPC(r, AMP_AVIN_GetHdmiAudCAInfo, pHld->avin, &audCA);

    TEST_AVIN_PRINT("hdmi audio info ca: %d\n", audCA);

    return r;
}

static HRESULT test_avin_gettmdsclk(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    UINT32 clkRate = 0;
    TEST_AVIN_HLD *pHld = h;
    HRESULT r = SUCCESS;
    AMP_RPC(r, AMP_AVIN_GetCurrentSource, pHld->avin, &(pHld->curSrc));

    AMP_RPC(r, AMP_AVIN_GetHdmiTmdsClockRate, pHld->avin, pHld->curSrc, &clkRate);

    TEST_AVIN_PRINT("hdmi tmds clock: %d\n", clkRate);

    return r;
}

static HRESULT test_avin_setdecmode(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    AMP_AVIN_VIDEO_STANDARD decmode = AMP_AVIN_VIDEO_STD_eAUTO;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        decmode = (AMP_AVIN_VIDEO_STANDARD)htoi(h->pcommand[0]);
    } else {
        decmode = AMP_AVIN_VIDEO_STD_eAUTO;
    }

    TEST_AVIN_PRINT("Set Dec mode to %d\n", decmode);
    AMP_RPC(r, AMP_AVIN_SetStandardManual, h->avin, decmode);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setcombcoeff(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    AMP_AVIN_VIDEO_STANDARD decmode = AMP_AVIN_VIDEO_STD_eAUTO;
    AMP_AVIN_COMBFILTER_TYPE combtype;
    AMP_AVIN_COMBFILTER_COEFF coeff;
    UINT32 i, j;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        decmode = (AMP_AVIN_VIDEO_STANDARD)htoi(h->pcommand[0]);
    } else {
        decmode = AMP_AVIN_VIDEO_STD_eNTSC_GENERAL;
    }

    if( cmdnums > 1 ){
        combtype = (AMP_AVIN_COMBFILTER_TYPE)htoi(h->pcommand[1]);
    } else {
        combtype = AMP_AVIN_COMBFILTER_BANDPASS;
    }

    AmpMemClear(coeff.uCoeff, sizeof(UINT16) * 32);
    coeff.uSize = 0;
    for (i = 2; i < cmdnums; i ++) {
        coeff.uCoeff[i-2] = (UINT16)htoi(h->pcommand[i]);
        coeff.uSize ++;
    }

    TEST_AVIN_PRINT("Set comb std %d type %d size %d\n", decmode, combtype, coeff.uSize);
    // 1) set to avin
    AMP_RPC(r, AMP_AVIN_Set3DCombFilterCoeff, h->avin, decmode, combtype, &coeff);

    // 2) get from avin for verify
    AmpMemClear(coeff.uCoeff, sizeof(UINT16) * 32);
    AMP_RPC(r, AMP_AVIN_Get3DCombFilterCoeff, h->avin, decmode, combtype, &coeff);
    TEST_AVIN_PRINT("Comb Coeff: \n");
    char strtmp[16], str[256];
    AmpMemClear(strtmp, sizeof(strtmp));
    AmpMemClear(str, sizeof(str));
    for (i = 0; i < coeff.uSize; i ++) {
        sprintf(strtmp, "%03x, ", coeff.uCoeff[i]);
        strcat(str, strtmp);
        if (0 == ((i+1) % 8)) {
            strcat(str, "\n");
        }
    }
    TEST_AVIN_PRINT("%s\n", str);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setvgahshift(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT16 hshift = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        hshift = (INT16)htoi(h->pcommand[0]);
    } else {
        hshift = 0;
    }

    TEST_AVIN_PRINT("Set vga hshift to %d\n", hshift);
    AMP_RPC(r, AMP_AVIN_SetVgaHShift, h->avin, hshift);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setvgavshift(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT16 vshift = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        vshift = (INT16)htoi(h->pcommand[0]);
    } else {
        vshift = 0;
    }

    TEST_AVIN_PRINT("Set vga vshift to %d\n", vshift);
    AMP_RPC(r, AMP_AVIN_SetVgaVShift, h->avin, vshift);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setvgaclock(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 clock = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        clock = (INT8)htoi(h->pcommand[0]);
    } else {
        clock = 0;
    }

    TEST_AVIN_PRINT("Set vga clock to %d\n", clock);
    AMP_RPC(r, AMP_AVIN_SetVgaClock, h->avin, clock);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setvgaphase(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        phase = (INT8)htoi(h->pcommand[0]);
    } else {
        phase = 0;
    }

    TEST_AVIN_PRINT("Set vga phase to %d\n", phase);
    AMP_RPC(r, AMP_AVIN_SetVgaPhase, h->avin, phase);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setaudioparams(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    /**
     * The user only needs to set a correct audio sample frequency. Default
     * bit depth and channels are used, and audio format is detected by the
     * low level driver.
     */
    AMP_AVIN_INPUT_AUDIO_PARAMS audioParams;
    AmpMemClear(&audioParams, sizeof(AMP_AVIN_INPUT_AUDIO_PARAMS));
    audioParams.sampleRate = 48000;
    TEST_AVIN_PRINT("Sample frequency list:\n"
            "32000\n"
            "44100\n"
            "48000\n"
            "88200\n"
            "96000\n"
            "176400\n"
            "192000\n"
            "input a sample frequency: ");
    scanf("%d", &(audioParams.sampleRate));
    AMP_RPC(r, AMP_AVIN_SetAudioParams, h->avin, &audioParams);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetAudioParams. r = 0x%x\n", r);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setaudiovolume(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_APP_PARAVOLUME stVolume;
    UINT option;
    AVIN_TEST_LOGD("enter\n");

    TEST_AVIN_PRINT("Select a volume value:\n"
            "0 - Mute\n"
            "1 - Low\n"
            "2 - Mid\n"
            "3 - High\n");
    scanf("%d", &option);
    if (option <= 3) {
        stVolume.uiVolume = 0x7FF * pow(option+1, 10); //0x4000000;
        r = AMP_SND_SetPathVolume(AMP_SND_PATH_71, &stVolume);
        TEST_AVIN_PRINT("Set speaker volume as 0x%x.\n", stVolume.uiVolume);
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_reinitaren(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;

    AMP_RPC(r, AMP_AREN_Close, h->aren);
    assert(SUCCESS == r);
    AMP_RPC(r, AMP_AREN_Destroy, h->aren);
    assert(SUCCESS == r);
    AMP_FACTORY_Release(h->aren);
    test_avin_setupArenComponent(h);
    return r;
}

static HRESULT test_avin_enableavsync(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");

    if((h->enable_passthr == TRUE) || (h->enable_mixpassthr == TRUE)){
        TEST_AVIN_PRINT("Please input 'disaudiopt/dismixaudiopt' to disable audio passthrough firstly,"
            " then enable avsync\n");
        return r;
    }

    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        h->enableClk = TRUE;
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        h->clkenabled = TRUE;
        TEST_AVIN_PRINT("AV Sync was enabled\n");
    } else {
        TEST_AVIN_PRINT("Non-Tunnel mode, can not enable AV Sync\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_disableavsync(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");

    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        h->enableClk = FALSE;
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        h->clkenabled = FALSE;
        TEST_AVIN_PRINT("AV Sync was disabled\n");
    } else {
        TEST_AVIN_PRINT("Non-Tunnel mode, can not disable AV Sync\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_enaudiopassthr(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");
    if(h->enableClk == TRUE || h->clkenabled == TRUE){
        TEST_AVIN_PRINT("Please input 'disavsync' to disable AV Sync firstly,"
            " then enable audio pass through\n");
        return r;
    }
    if (h->enable_mixpassthr == TRUE) {
        TEST_AVIN_PRINT("Mix passthrough is enabled\n");
        return r;
    }
    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        h->enable_passthr = TRUE;
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        TEST_AVIN_PRINT("Audio passthrough is enabled\n");
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_disaudiopassthr(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");
    if (TEST_AVIN_MODE_TUNNEL == h->testMode && h->enable_passthr == TRUE) {
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        h->enable_passthr = FALSE;
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        TEST_AVIN_PRINT("Audio passthrough is enabled\n");
    }
    h->enable_passthr = FALSE;
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_enmixaudiopassthr(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");
    if(h->enableClk == TRUE || h->clkenabled == TRUE){
        TEST_AVIN_PRINT("Please input 'disavsync' to disable AV Sync firstly,"
            " then enable audio pass through\n");
        return r;
    }
    if (h->enable_passthr == TRUE) {
        TEST_AVIN_PRINT("Direct passthrough is enabled\n");
        return r;
    }
    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        h->enable_mixpassthr = TRUE;
        h->enable_mixpassthr_ddp = FALSE;
        h->enable_mixpassthr_mat = FALSE;
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        TEST_AVIN_PRINT("Audio passthrough is enabled\n");
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
#else
    h->enable_mixpassthr = FALSE;
    AVIN_TEST_LOGD("mix passthrough not supported\n");
#endif
    return r;
}

static HRESULT test_avin_dismixaudiopassthr(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");
    if (TEST_AVIN_MODE_TUNNEL == h->testMode && h->enable_mixpassthr == TRUE) {
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        h->enable_mixpassthr = FALSE;
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        TEST_AVIN_PRINT("Audio passthrough is enabled\n");
    }
    h->enable_mixpassthr = FALSE;
    h->enable_mixpassthr_ddp = FALSE;
    h->enable_mixpassthr_mat = FALSE;
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
#else
    h->enable_mixpassthr = FALSE;
    AVIN_TEST_LOGD("mix passthrough not supported\n");
#endif
    return r;
}

static HRESULT test_avin_enmixaudiopassthr_ddp(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");
    if(h->enableClk == TRUE || h->clkenabled == TRUE){
        TEST_AVIN_PRINT("Please input 'disavsync' to disable AV Sync firstly,"
            " then enable audio pass through\n");
        return r;
    }
    if (h->enable_passthr == TRUE) {
        TEST_AVIN_PRINT("Direct passthrough is enabled\n");
        return r;
    }
    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        h->enable_mixpassthr = TRUE;
        h->enable_mixpassthr_ddp = TRUE;
        h->enable_mixpassthr_mat = FALSE;
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        TEST_AVIN_PRINT("Audio passthrough(DDP) is enabled\n");
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
#else
    h->enable_mixpassthr = FALSE;
    AVIN_TEST_LOGD("mix passthrough not supported\n");
#endif
    return r;
}

static HRESULT test_avin_enmixaudiopassthr_mat(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");
    if(h->enableClk == TRUE || h->clkenabled == TRUE){
        TEST_AVIN_PRINT("Please input 'disavsync' to disable AV Sync firstly,"
            " then enable audio pass through\n");
        return r;
    }
    if (h->enable_passthr == TRUE) {
        TEST_AVIN_PRINT("Direct passthrough is enabled\n");
        return r;
    }
    if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
        MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
        r = test_avin_stop(h);
        r = test_avin_disconnectComponent(h);
        h->enable_mixpassthr = TRUE;
        h->enable_mixpassthr_mat = TRUE;
        h->enable_mixpassthr_ddp = FALSE;
        r = test_avin_reinitaren(h);
        r = test_avin_connectComponent(h);
        MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);
        r = test_avin_play(h);
        TEST_AVIN_PRINT("Audio passthrough(MAT) is enabled\n");
    }
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
#else
    h->enable_mixpassthr = FALSE;
    AVIN_TEST_LOGD("mix passthrough not supported\n");
#endif
    return r;
}

static HRESULT test_avin_getplatformconfig(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_PLATFORM_CONFIG pltConfig;
    UINT32 i, j;
    char str[256] = {0};

    AVIN_TEST_LOGD("enter\n");

    TEST_AVIN_PRINT("Get Platform Config\n");
    AMP_RPC(r, AMP_AVIN_GetPlatformConfig, h->avin, &pltConfig);
    if (SUCCESS != r) {
        AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
        return r;
    }

    TEST_AVIN_PRINT("--------------%s Pinmux Config---------------\n", pltConfig.chipName);
    TEST_AVIN_PRINT("%5s %-12s %-24s %-18s\n", "SrcId", "Source", "VideoInput", "AudioInput");
    for (i = AMP_AVIN_SOURCE_TYPE_INVALID; i < AMP_AVIN_SOURCE_TYPE_MAX_NUM; i++){
        TEST_AVIN_PRINT("%4d: %-12s %-24s %-18s\n", i,
            test_avin_inputSourceString(pltConfig.inputMux[i].source),
            pltConfig.inputMux[i].videoInputName,
            pltConfig.inputMux[i].audioInputName);
    }

    TEST_AVIN_PRINT("--------------%s Platform Config-------------\n", pltConfig.chipName);
    TEST_AVIN_PRINT("uChipResetPinAddress: 0x%x\n", pltConfig.uChipResetPinAddress);
    TEST_AVIN_PRINT("uCvbsDetectPinAddress: 0x%x\n", pltConfig.uCvbsDetectPinAddress);
    TEST_AVIN_PRINT("uComponentDetectPinAddress: 0x%x\n", pltConfig.uComponentDetectPinAddress);
    TEST_AVIN_PRINT("uHdmirx1HPDAddress: 0x%x\n", pltConfig.uHdmirx1HPDAddress);
    TEST_AVIN_PRINT("uHdmirx2HPDAddress: 0x%x\n", pltConfig.uHdmirx2HPDAddress);
    TEST_AVIN_PRINT("uHdmirx3HPDAddress: 0x%x\n", pltConfig.uHdmirx3HPDAddress);
    TEST_AVIN_PRINT("uHdmirx4HPDAddress: 0x%x\n", pltConfig.uHdmirx4HPDAddress);
    TEST_AVIN_PRINT("uScartFBDetectPinAddress: 0x%x\n", pltConfig.uScartFBDetectPinAddress);
    TEST_AVIN_PRINT("audioOutputName: %s\n", pltConfig.audioOutputName);
    TEST_AVIN_PRINT("defaultCvbsStandard: %s\n",
        test_avin_videoStdString(pltConfig.defaultCvbsStandard));
    TEST_AVIN_PRINT("bEnable3DCombFilter: %d\n", pltConfig.bEnable3DCombFilter);
    TEST_AVIN_PRINT("bEnableComponentVBI: %d\n", pltConfig.bEnableComponentVBI);
    TEST_AVIN_PRINT("bEnableCvbsVBI: %d\n", pltConfig.bEnableCvbsVBI);
    TEST_AVIN_PRINT("bEnable50HzTiming: %d\n", pltConfig.bEnable50HzTiming);
    TEST_AVIN_PRINT("bEnableHdcpRepeaterMode: %d\n", pltConfig.bEnableHdcpRepeaterMode);
    TEST_AVIN_PRINT("bEnableInternalEdid: %d\n", pltConfig.bEnableInternalEdid);
    if (TRUE == pltConfig.bEnableInternalEdid) {
        TEST_AVIN_PRINT("---------------Internal EDID--------------\n");
        for (i = 0; i < 256/16; i++) {
            for (j = 0; j < 16 && j < (256 - i*16); j++) {
                TEST_AVIN_PRINT("0x%02x ", pltConfig.internalEdid[i*16+j]);
            }
            TEST_AVIN_PRINT("\n");
            if ((i*16+j) == 128) {
                TEST_AVIN_PRINT("\n");
            }
        }
    }

    TEST_AVIN_PRINT("--------------Platform Config-------------\n");
    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setpcmode(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    BOOL isPCMode = FALSE;

    AVIN_TEST_LOGD("enter\n");

    if (test_avin_isHdmisource(h->curSrc)) {
        cmdnums = test_avin_parse_argv(pArgv, h);
        if (cmdnums == 1) {
            isPCMode = (atoi(h->pcommand[0]) == 0) ? FALSE : TRUE;

            TEST_AVIN_PRINT("Set PC Mode %s\n", isPCMode ? "on" : "off");
            AMP_RPC(r, AMP_AVIN_SetPCMode, h->avin, isPCMode);
        } else {
            TEST_AVIN_PRINT("Usage: setpcmode [0|1]\n");
        }
    } else {
        TEST_AVIN_PRINT("This command is only supported under HDMI source\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setscalerenable(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    BOOL isScalerOn = FALSE;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if (cmdnums == 1) {
        isScalerOn = (atoi(h->pcommand[0]) == 0) ? FALSE : TRUE;

        TEST_AVIN_PRINT("Set scaler enable %s\n", isScalerOn ? "on" : "off");
        AMP_RPC(r, AMP_AVIN_EnableScaler, h->avin, isScalerOn);
    } else {
        TEST_AVIN_PRINT("Usage: setscalerenable [0|1]\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setpassthroughenable(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    BOOL isPassthroughOn = FALSE;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if (cmdnums == 1) {
        isPassthroughOn = (atoi(h->pcommand[0]) == 0) ? FALSE : TRUE;

        TEST_AVIN_PRINT("Set passthrough enable %s\n", isPassthroughOn ? "on" : "off");
        AMP_RPC(r, AMP_AVIN_EnablePassthrough, h->avin, isPassthroughOn);
        pgHld->isPassthroughOn = isPassthroughOn;
    } else {
        TEST_AVIN_PRINT("Usage: setpassthroughenable [0|1]\n");
    }

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_calibrateadcgainext(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_ADC_CALIB_DATA_EXT adcCalibData;
    UINT uiIdx = 0;
    AVIN_TEST_LOGD("enter\n");

    AmpMemClear(&adcCalibData, sizeof(AMP_AVIN_ADC_CALIB_DATA_EXT));
    TEST_AVIN_PRINT("input expected pixel value (Y U V): ");
    for (uiIdx = 0; uiIdx < 3; ++uiIdx)
        scanf("%d", &(adcCalibData.expected[uiIdx]));
    TEST_AVIN_PRINT("input actual pixel value (Y U V): ");
    for (uiIdx = 0; uiIdx < 3; ++uiIdx)
        scanf("%d", &(adcCalibData.actual[uiIdx]));
    AMP_RPC(r, AMP_AVIN_CalibrateAdc, h->avin, &adcCalibData);

    if (SUCCESS == r)
        TEST_AVIN_PRINT("calibration done!\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_getadcgainext(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_ADC_CALIB_DATA adcCalibData;
    UINT uiIdx = 0;
    AVIN_TEST_LOGD("enter\n");

    AmpMemClear(&adcCalibData, sizeof(AMP_AVIN_ADC_CALIB_DATA));

    AMP_RPC(r, AMP_AVIN_GetAdcGainExt, h->avin, &adcCalibData);
    if (SUCCESS == r)
        TEST_AVIN_PRINT("get ADC gains [%f, %f, %f]\n",
                adcCalibData.gain[0],
                adcCalibData.gain[1],
                adcCalibData.gain[2]);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_setadcgainext(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_ADC_CALIB_DATA adcCalibData;
    UINT uiIdx = 0;
    AVIN_TEST_LOGD("enter\n");

    AmpMemClear(&adcCalibData, sizeof(AMP_AVIN_ADC_CALIB_DATA));
    TEST_AVIN_PRINT("input ADC gains you want to set (gain[0], gain[1], gain[2]): ");
    for (uiIdx = 0; uiIdx < 3; ++uiIdx)
        scanf("%f", &(adcCalibData.gain[uiIdx]));
    AMP_RPC(r, AMP_AVIN_SetAdcGainExt, h->avin, &adcCalibData);

    if (SUCCESS == r)
        TEST_AVIN_PRINT("set the gains successfully!\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_enableMS11(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    INT8 phase = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0 ){
        h->enableMS11 = (INT8)htoi(h->pcommand[0]);
    } else {
        h->enableMS11 = TRUE;
    }

    TEST_AVIN_PRINT("%s MS11\n", h->enableMS11 ? "Enable" : "Disable");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_vbi_enwss_palcc(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_VBI_CONFIG vbiCfg;

    AMPLOGE("test_avin_vbi_en\n");
    AmpMemClear(&vbiCfg, sizeof(AMP_AVIN_VBI_CONFIG));

    TEST_AVIN_PRINT("Enable WSS and PalCC\n");

    vbiCfg.bEnWSS = TRUE;
    vbiCfg.bEnPalCC = TRUE;

    AMP_RPC(r, AMP_AVIN_SetVbiConfig, h->avin, &vbiCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiConfig. r = 0x%x\n", r);
    }

    return r;
}

static HRESULT test_avin_vbi_enttx_wss_lines(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_VBI_LINES_CONFIG vbiLinesCfg;
    UINT32 i;

    AMPLOGE("test_avin_vbilines\n");
    AmpMemClear(&vbiLinesCfg, sizeof(AMP_AVIN_VBI_LINES_CONFIG));

    TEST_AVIN_PRINT("Configure lines 7~18 as Teletext and line 23 as WSS\n");

    for(i = 7; i <= 18; i++){
        vbiLinesCfg.bLinesTTX[i] = TRUE;
    }
    vbiLinesCfg.bLinesWSS[23] = TRUE;

    AMP_RPC(r, AMP_AVIN_SetVbiLinesConfig, h->avin, &vbiLinesCfg);
    if (SUCCESS != r) {
        AMPLOGE("failed to call AMP_AVIN_SetVbiLinesConfig. r = 0x%x\n", r);
    }

    return r;
}

static HRESULT test_avin_autosource(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT32 cmdnums = 0;
    UINT32 i = 0, delay = 6000, count = 0;
    UINT8 pCmd[255];
    UINT8 *p;
    AMP_AVIN_SOURCE_LIST SourceList;
    BOOL exit = FALSE;
    UINT32 signalAvailable[19], SourceChanged[19], sourceChangeFailed[19];
    AMP_AVIN_INPUT_PARAMS inputParams;
    AMP_AVIN_SOURCE_TYPE source = AMP_AVIN_SOURCE_TYPE_INVALID;

    AVIN_TEST_LOGD("enter\n");

    memset(&SourceList, 0, sizeof(AMP_AVIN_SOURCE_LIST));
    memset(&signalAvailable, 0, sizeof(signalAvailable));
    memset(&sourceChangeFailed, 0, sizeof(sourceChangeFailed));
    memset(&SourceChanged, 0, sizeof(SourceChanged));
    memset(&inputParams, 0, sizeof(inputParams));

    cmdnums = test_avin_parse_argv(pArgv, h);

    /*Read source list from command line*/
    if( cmdnums > 0){
        strcpy(pCmd, pArgv);
        p = strtok(pCmd, " ");
        if( p != NULL){
            sscanf(p, "%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",
                &SourceList.source[0], &SourceList.source[1], &SourceList.source[2],
                &SourceList.source[3], &SourceList.source[4], &SourceList.source[5],
                &SourceList.source[6], &SourceList.source[7], &SourceList.source[8],
                &SourceList.source[9], &SourceList.source[10], &SourceList.source[11],
                &SourceList.source[12], &SourceList.source[13], &SourceList.source[14],
                &SourceList.source[15], &SourceList.source[16], &SourceList.source[17],
                &SourceList.source[18]);

            for(i = 0; i < 19; i++){
                SourceList.size = (SourceList.source[i] > 0) ? (SourceList.size+1): SourceList.size;
            }
        }

        p = strtok(NULL, " ");
        if( p != NULL){
            delay = atoi(p);
            delay = ( delay <= 100 )? 100: delay;
        }
    }else {
        AMP_AVIN_SOURCE_LIST supportList;

        /*Test active sources*/

        h->enableLog = FALSE;
        printf("Now test which source are available...\n");
        AMP_RPC(r, AMP_AVIN_GetSupportedSourceList, h->avin, &supportList);

        for(i = 0; i < supportList.size; i++){
            BOOL detected = FALSE;
            int source = 0, j = 0;

            source = supportList.source[i];
            printf("Try source %x.%10s...", source, test_avin_inputSourceString(source));
            AMP_RPC(r, AMP_AVIN_SetCurrentSource, h->avin, source);
            for(j = 0; j<40; j++){
               MV_OSAL_Task_Sleep(100);
               memset(&inputParams, 0, sizeof(AMP_AVIN_INPUT_PARAMS));
               AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);
               if( inputParams.state == AMP_AVIN_SOURCE_STATE_STABLE){
                   detected = TRUE;
                   break;
               }
            }
            if( detected ){
               SourceList.source[SourceList.size] = source;
               SourceList.size ++;
               printf("%sOK%s\n", LOG_COLOR_GREEN, LOG_COLOR_AUTO);
            } else {
                printf("NG\n");
            }
        }
        printf("\nTest finish, ");
    }

    /*Get final source list*/
    printf("Source List:\n");
    for(i = 0; i < SourceList.size; i++){
        printf("%x. %s\n", SourceList.source[i],
              test_avin_inputSourceString(SourceList.source[i]));
    }
    printf("Delay = %d ms\n", delay);
    printf("\nNow Start testing...\n");

    h->enableLog = TRUE;
    test_avin_keyevent_start(h);
    while( !exit ){
        int randsourceId = 0;

        count ++;
        srand(time(NULL));
        randsourceId = rand()% SourceList.size;
        randsourceId = SourceList.source[randsourceId];
        TEST_AVIN_PRINT("\n%s%d -> Set source to %s(%x)%s\n",
            LOG_COLOR_GREEN, count, test_avin_inputSourceString(randsourceId),
            randsourceId, LOG_COLOR_AUTO);
        AMP_RPC(r, AMP_AVIN_SetCurrentSource, h->avin, randsourceId);

        if (TEST_AVIN_MODE_TUNNEL == h->testMode) {
            if (AMP_AVIN_SOURCE_TYPE_ATV_01 == randsourceId ||
                AMP_AVIN_SOURCE_TYPE_ATV_02 == randsourceId) {

                AMP_AVIN_SIF_STANDARD sifStd = AMP_AVIN_SIF_STD_MONO45
                                                | AMP_AVIN_SIF_STD_MONO55
                                                | AMP_AVIN_SIF_STD_MONO60
                                                | AMP_AVIN_SIF_STD_MONO65;
                AMP_RPC(r, AMP_AVIN_DetectSifStandard, h->avin, sifStd);
                if (SUCCESS != r) {
                    TEST_AVIN_PRINT("failed to call AMP_AVIN_DetectSifStandard. r = 0x%x\n", r);
                } else {
                    h->bDetSifStd = TRUE;
                }
            }
        }
        for(i = 0; i<100; i++){
            MV_OSAL_Task_Sleep(delay/100);
            if(h->keyEvent){
                exit = TRUE;
                break;
            }
        }
        AMP_RPC(r, AMP_AVIN_GetCurrentSource, h->avin, &source);
        if( source!= (UINT32)randsourceId ){
            TEST_AVIN_PRINT("Source change to %s failed: \n",
                test_avin_inputSourceString(randsourceId));
            sourceChangeFailed[randsourceId] ++;
        }

        AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);
        if( inputParams.state == AMP_AVIN_SOURCE_STATE_STABLE){
            signalAvailable[randsourceId] ++;
        }
        SourceChanged[randsourceId]++;
    }

finish:
    test_avin_keyevent_stop(h);
    TEST_AVIN_PRINT("%sAuto source change test finished\n", LOG_COLOR_GREEN);
    TEST_AVIN_PRINT("%16s%9s%20s", "Checked source", "Failed", "Signal Available\n");
    for(i = 0; i < SourceList.size; i++){
        source = SourceList.source[i];
        TEST_AVIN_PRINT("%15s,%4d,%9d/%d\n", test_avin_inputSourceString(source),
            sourceChangeFailed[source], signalAvailable[source], SourceChanged[source]);
    }
    TEST_AVIN_PRINT("%s", LOG_COLOR_AUTO);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

#ifdef CLOSE_COMPS_TO_SUSPEND
static HRESULT test_avin_constructPipelineOpenComponents(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    r = test_avin_openAvinComponent(h);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_DISP_GetPlaneZOrder, h->voutDisp, 0, &h->voutZorder);
    assert(SUCCESS == r);

    r = test_avin_queryPorts(h);
    assert(SUCCESS == r);

    r = test_avin_openVoutComponent(h);
    assert(SUCCESS == r);

    r = test_avin_openArenComponent(h);
    assert(SUCCESS == r);

    r = test_avin_openClkComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("connect component.\n");
    r = test_avin_connectComponent(h);
    assert(SUCCESS == r);

    /**************************************************************************/
    /**
     * Since audio changed event might not be received again, open & connect
     * adec before registering callback!!
     */
    if (AMP_LOADED == test_avin_getAdecState(h)) {
        r = test_avin_openAdecComponent(h);
        assert(SUCCESS == r);
    }

    if (AMP_IDLE == test_avin_getAdecState(h)) {
        if (TRUE == h->enableClk) {
            r = AMP_ConnectComp(h->adec, 0, h->aren, 1);
        } else {
            r = AMP_ConnectComp(h->adec, 0, h->aren, 0);
        }
        assert(SUCCESS == r);

        r = AMP_ConnectComp(h->avin, h->uiAudioPort, h->adec, 0);
        assert(SUCCESS == r);

        AVIN_TEST_LOGD("set aren state to EXECUTING.\n");
        AMP_RPC(r, AMP_AREN_SetState, h->aren, AMP_EXECUTING);
        assert(SUCCESS == r);

        AVIN_TEST_LOGD("set adec state to EXECUTING.\n");
        AMP_RPC(r, AMP_ADEC_SetState, h->adec, AMP_EXECUTING);
        assert(SUCCESS == r);
    }
    /**************************************************************************/

    AVIN_TEST_LOGD("register event callbacks.\n");
    r = test_avin_registerEventCallbacks(h->avin, h->hListener, h);
    assert(SUCCESS == r);

    AMP_RPC(r, AMP_AVIN_SetCurrentSource, h->avin, h->curSrc);
    assert(SUCCESS == r);
    AVIN_TEST_LOGD("set source as %s\n",
            test_avin_inputSourceString(h->curSrc));

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_destroyPipelineCloseComponents(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    AVIN_TEST_LOGD("enter\n");

    AMP_RPC(r, AMP_AVIN_DebugDump, h->avin, 0 /* STOP DUMPING ANY DATA */);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("disconnect component.\n");
    r = test_avin_disconnectComponent(h);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("unregister the notify.\n");
    r = test_avin_unregisterEventCallbacks(h->avin, h->hListener);
    assert(S_OK == r);

    AVIN_TEST_LOGD("close avin.\n");
    AMP_RPC(r, AMP_AVIN_Close, h->avin);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("close vout.\n");
    AMP_RPC(r, AMP_VOUT_Close, h->vout);
    assert(SUCCESS == r);

    if (AMP_IDLE == test_avin_getAdecState(h)) {
        r = test_adec_register_event(h->hListener, h->adec, FALSE, h);
        AVIN_TEST_LOGD("close adec.\n");
        AMP_RPC(r, AMP_ADEC_Close, h->adec);
        assert(SUCCESS == r);
    }

    AVIN_TEST_LOGD("close aren.\n");
    AMP_RPC(r, AMP_AREN_Close, h->aren);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("close clk.\n");
    AMP_RPC(r, AMP_CLK_Close, h->clk);
    assert(SUCCESS == r);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}
#endif

static HRESULT test_avin_suspend(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT uiDelayMs = 0, uiLoops = 0;
    UINT8 *u8Space = NULL;
    const UINT DEFAULT_LOOPS = 1; // loop once by default
    const UINT DEFAULT_DELAY_MS = 5000; // delay 5 ms by default

    if (TEST_AVIN_MODE_TUNNEL != h->testMode) {
        TEST_AVIN_PRINT("Suspend isn't supported in debug test mode.\n");
        return r;
    }

    if ((NULL != pArgv) && (0 < (uiLoops = atoi(pArgv)))) {
        if ((NULL == (u8Space = strchr(pArgv, ' '))) ||
            (0 >= (uiDelayMs = atoi(u8Space)))) {
            uiDelayMs = DEFAULT_DELAY_MS;
        }
    } else {
        uiLoops = DEFAULT_LOOPS;
        uiDelayMs = DEFAULT_DELAY_MS;
    }
    printf("Suspend Test: %d round(s)\nDelay: %d ms\n", uiLoops, uiDelayMs);

    if (-1 == access(SUSPEND_SHELL_FILE, 0/*Existence Only*/)) {
        system("echo -e \"#!/system/bin/sh\nWAKEUP_IN_SEC=8\n"
                "RESUME_PERIOD=16\nsmbox -w \\$WAKEUP_IN_SEC\n"
                "input keyevent 26\nsleep \\$RESUME_PERIOD\">"
                SUSPEND_SHELL_FILE"&&chmod 755 "SUSPEND_SHELL_FILE);
    }

    while (uiLoops--) {
        printf("========= Suspend-Resume, remaining: %d round(s) =========\n", uiLoops);
        MV_OSAL_Task_Sleep(uiDelayMs);
        r = test_avin_stop(h);
        if (SUCCESS != r) {
            AMPLOGE("failed to call test_avin_stop, r = 0x%x\n", r);
            break;
        }
#ifdef CLOSE_COMPS_TO_SUSPEND
        test_avin_destroyPipelineCloseComponents(h);
#endif
        system("/system/bin/sh "SUSPEND_SHELL_FILE);
#ifdef CLOSE_COMPS_TO_SUSPEND
        test_avin_constructPipelineOpenComponents(h);
#endif
        r = test_avin_play(h);
        if (SUCCESS != r) {
            AMPLOGE("failed to call test_avin_play, r = 0x%x\n", r);
            break;
        }
    }
    return r;
}

static HRESULT test_avin_set2Dmode(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_DISP_OUT_RES resId;
    AMP_DISP_WIN src_rect, dst_rect;
    AMP_DISP_OUT_RES vout_format = AMP_DISP_OUT_RES_1080P60;
    AMP_DISP_OUT_PARAMS disp_params;
    src_rect.iX = 0;
    src_rect.iY = 0;
    src_rect.iWidth = 0;
    src_rect.iHeight = 0;
    dst_rect.iX = 0;
    dst_rect.iY = 0;
    dst_rect.iWidth = 1920;
    dst_rect.iHeight = 1080;

    AVIN_TEST_LOGD("enter\n");

    if(-1 == h->resId_origin){
        TEST_AVIN_PRINT("set2Dmode before any 3D mode setting, fix to 4K60 or 1080P60\n");
        AMP_RPC(r, AMP_DISP_OUT_GetResolution,h->voutDisp, AMP_DISP_TG_PRI, &resId);
        if((resId >= AMP_DISP_OUT_RES_4Kx2K_MIN && resId <= AMP_DISP_OUT_RES_4Kx2K_MAX)){
            vout_format = AMP_DISP_OUT_RES_4Kx2KP60;
        } else {
            vout_format = AMP_DISP_OUT_RES_1080P60;
        }
    }else{
        vout_format = h->resId_origin;
        h->resId_origin = -1;
    }
    AmpMemClear(&disp_params, sizeof(AMP_DISP_OUT_PARAMS));

    disp_params.uiDispId = AMP_DISP_OUT_HDMI;
    disp_params.uiResId = vout_format;
    disp_params.uiBitDepth = AMP_DISP_OUT_BIT_DPE_8;
    disp_params.uiColorFmt = AMP_DISP_OUT_CLR_FMT_YCBCR422;
    disp_params.iPixelRepeat = 1;

    AMP_RPC(r, AMP_DISP_OUT_SetFormat, h->voutDisp,
                        AMP_DISP_TG_PRI, &disp_params);

    AMP_RPC(r, AMP_DISP_SetScale, h->voutDisp, h->outputPlane, &src_rect, &dst_rect);
    AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 0);
    AMP_RPC(r, AMP_DISP_OUT_3D_SetSSType, h->voutDisp, 1<<h->outputPlane, 0);
    AMP_RPC(r, AMP_DISP_OUT_SetMute, h->voutDisp, AMP_DISP_OUT_HDMI, 0);
    TEST_AVIN_PRINT("Output Mode had been set to %s\n",
        vout_format == AMP_DISP_OUT_RES_4Kx2KP60? "4Kx2KP60": "1080p@60");

    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_set3Dmode(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    AMP_DISP_OUT_RES resId;
    AMP_DISP_WIN src_rect, dst_rect;
    AMP_DISP_OUT_RES vout_format = AMP_DISP_OUT_RES_4Kx2KP120;
    Test_3D_MODE mode = 0;
    UINT8 i = 0;
    Test_3D_MODE_Mapping *p3DModeList = p3DMode;
    Test_3D_Vout_Format_Mapping *p3DVoutFmtList = p3DVoutFmt;
    Test_3D_VOUT_FORMAT vout_fmt = 0;
    AMP_DISP_OUT_PARAMS disp_params;

    src_rect.iX = 0;
    src_rect.iY = 0;
    src_rect.iWidth = 0;
    src_rect.iHeight = 0;
    dst_rect.iX = 0;
    dst_rect.iY = 0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        mode = atoi(h->pcommand[0]);
        vout_fmt = atoi(h->pcommand[1]);
    } else {
        for(i=0; i< sizeof(p3DMode)/sizeof(Test_3D_MODE_Mapping); i++){
            TEST_AVIN_PRINT("%14s: %d\n", p3DModeList->pName,p3DModeList->iNumber);
            p3DModeList++;
         }
        TEST_AVIN_PRINT("**********************************************\n");
        for(i=0; i< sizeof(p3DVoutFmt)/sizeof(Test_3D_Vout_Format_Mapping); i++){
            TEST_AVIN_PRINT("%14s: %d\n", p3DVoutFmtList->pName,p3DVoutFmtList->iNumber);
            p3DVoutFmtList++;
         }
        TEST_AVIN_PRINT("**********************************************\n");
        TEST_AVIN_PRINT("input 3Dmode & VoutFormat:");
        scanf("%d %d", &mode,&vout_fmt);
    }
    AMP_RPC(r, AMP_DISP_OUT_SetMute, h->voutDisp, AMP_DISP_OUT_HDMI, 1);
    AMP_RPC(r, AMP_DISP_OUT_GetResolution, h->voutDisp, AMP_DISP_TG_PRI, &resId);
    h->resId_origin = resId;
    {
        dst_rect.iWidth = 1920;
        dst_rect.iHeight = 1080;
        switch(vout_fmt){
            case Test_1920x1080_30:
                vout_format = AMP_DISP_OUT_RES_3D_1080P30;
                break;
            case Test_1920x1080_24:
                vout_format = AMP_DISP_OUT_RES_3D_1080P24;
                break;
            case Test_1280x720_60:
                vout_format = AMP_DISP_OUT_RES_3D_720P60;
                break;
            case Test_1280x720_50:
                vout_format = AMP_DISP_OUT_RES_3D_720P50;
                break;
            case Test_4096x2160_120:
                dst_rect.iWidth = 1280;
                dst_rect.iHeight = 720;
                vout_format = AMP_DISP_OUT_RES_4Kx2KP120;
                break;
            default:
                vout_format = AMP_DISP_OUT_RES_3D_720P60;
                break;
        }
    }
    AmpMemClear(&disp_params, sizeof(AMP_DISP_OUT_PARAMS));

    disp_params.uiDispId = AMP_DISP_OUT_HDMI;
    disp_params.uiResId = vout_format;
    disp_params.uiBitDepth = AMP_DISP_OUT_BIT_DPE_8;
    disp_params.uiColorFmt = AMP_DISP_OUT_CLR_FMT_YCBCR422;
    disp_params.iPixelRepeat = 1;

    AMP_RPC(r, AMP_DISP_OUT_SetFormat, h->voutDisp,
                        AMP_DISP_TG_PRI, &disp_params);
    TEST_AVIN_PRINT("Output Mode had been set to %s\n",
    vout_format == AMP_DISP_OUT_RES_4Kx2KP120? "4Kx2K@120 3D": p3DVoutFmt[vout_fmt].pName);

    AMP_RPC(r,AMP_DISP_SetScale, h->voutDisp, h->outputPlane, &src_rect, &dst_rect);
    switch (mode){
        case Test_3D_MODE_FP:
            AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 0);
            AMP_RPC(r, AMP_DISP_OUT_3D_SetSSType, h->voutDisp, 1<<h->outputPlane, 1);
            TEST_AVIN_PRINT("Select input mode as FP mode\n");
            break;

        case Test_3D_MODE_SBS:
            AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 1);
            AMP_RPC(r, AMP_DISP_OUT_3D_SetSSType, h->voutDisp, 1<<h->outputPlane, 0);
            TEST_AVIN_PRINT("Select input mode as SBS mode\n");
            break;

        case Test_3D_MODE_TAB:
            AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 2);
            AMP_RPC(r, AMP_DISP_OUT_3D_SetSSType, h->voutDisp, 1<<h->outputPlane, 0);
            TEST_AVIN_PRINT("Select input mode as T/B mode\n");
           break;

        case Test_3D_MODE_SENSIO:
            AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 4);
            AMP_RPC(r, AMP_DISP_OUT_3D_SetSSType, h->voutDisp, 1<<h->outputPlane, 0);
            TEST_AVIN_PRINT("Select input mode as SENSIO mode\n");
           break;

        case Test_3D_MODE_AUTO:
            AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 3);
            TEST_AVIN_PRINT("Select input mode as AUTO mode\n");
            break;

        default:
            AMP_RPC(r, AMP_DISP_OUT_3D_SetConvertMode, h->voutDisp, 0);
            AMP_RPC(r, AMP_DISP_OUT_3D_SetSSType, h->voutDisp, 1<<h->outputPlane, 0);
            break;
    }
    AMP_RPC(r, AMP_DISP_OUT_3D_SetHDMIStructure, h->voutDisp,
            AMP_DISP_3D_Structure_FRAME_PACKING, 0);
    TEST_AVIN_PRINT("Set output 3D mode as FP\n");
    AMP_RPC(r, AMP_DISP_OUT_SetMute, h->voutDisp, AMP_DISP_OUT_HDMI, 0);

    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_3DModeLRSwap(TEST_AVIN_HLD *h,UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    int cmdnums = 0;
    INT32 iMode=0;

    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        iMode = atoi(h->pcommand[0]);
    } else {
        TEST_AVIN_PRINT("Off: 0\n On: 1\n");
        TEST_AVIN_PRINT("input LRSwap:");
        scanf("%d", &iMode);
    }
    AMP_RPC(r, AMP_DISP_OUT_3D_SetInvertMode, h->voutDisp, iMode);

    AVIN_TEST_LOGD("leave\n");
    return r;
}

static HRESULT test_avin_getpixeloncolorbar(TEST_AVIN_HLD *h,
    UINT8 uColorBarStep, UINT8 *pExpected, UINT8* pActual)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_INPUT_PARAMS inputParams;
    UINT16 x, y, pixelValue = 0, stepWidth = 0;
    UINT16 temp, i, ysum, cbsum, crsum;
    UINT8 yexp, cbexp, crexp, yact, cbact, cract;
    // Fluke 54200 ColorBar COL_100_0
    // White 100% Black 0% Color 75%
    UINT8 sdtv100cbtable[3][8] = {
        {235,  162,   131,   112,   84,    65,    35,    16},
        {128,  44,    156,   72,    184,   100,   212,   128},
        {128,  142,   44,    58,    198,   212,   114,   128}
    };
    UINT8 hdtv100cbtable[3][8] = {
        {235,  219,   188,   173,   78,    63,    32,    16},
        {128,  16,    154,   42,    214,   102,   240,   128},
        {128,  138,   16,    26,    230,   240,   118,   128},
    };

    AmpMemClear((VOID*) &inputParams, sizeof(AMP_AVIN_INPUT_PARAMS));
    AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);

    if (AMP_AVIN_SOURCE_STATE_STABLE != inputParams.state) {
        yexp = sdtv100cbtable[0][uColorBarStep];
        cbexp = sdtv100cbtable[1][uColorBarStep];
        crexp = sdtv100cbtable[2][uColorBarStep];
        yact = sdtv100cbtable[0][uColorBarStep];
        cbact = sdtv100cbtable[1][uColorBarStep];
        cract = sdtv100cbtable[2][uColorBarStep];
        AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
        return r;
    }

    if (720 <= inputParams.videoParams.videoHeight) {
        yexp = hdtv100cbtable[0][uColorBarStep];
        cbexp = hdtv100cbtable[1][uColorBarStep];
        crexp = hdtv100cbtable[2][uColorBarStep];
    } else {
        yexp = sdtv100cbtable[0][uColorBarStep];
        cbexp = sdtv100cbtable[1][uColorBarStep];
        crexp = sdtv100cbtable[2][uColorBarStep];
    }

    if (inputParams.videoParams.videoHeight == 480) {
        stepWidth = inputParams.videoParams.videoWidth/7;
    } else {
        stepWidth = inputParams.videoParams.videoWidth/8;
    }
    x = (stepWidth*uColorBarStep + stepWidth*(uColorBarStep+1))/2;
    y = 0;
    ysum = cbsum = crsum = 0;
    for (i = 0; i < 3; ++i) {
        y += inputParams.videoParams.videoHeight/4;
        AMP_RPC(r, AMP_AVIN_ReadPixel, h->avin, x&0xFFFE, y, &pixelValue);
        ysum  += pixelValue>>8;
        cbsum += pixelValue&0x00FF;
        AMP_RPC(r, AMP_AVIN_ReadPixel, h->avin, (x&0xFFFE)+1, y, &pixelValue);
        ysum  += pixelValue>>8;
        crsum += pixelValue&0x00FF;
    }
    yact  = ysum/6;
    cbact = cbsum/3;
    cract = crsum/3;

    pExpected[0] = yexp;
    pExpected[1] = cbexp;
    pExpected[2] = crexp;
    pActual[0] = yact;
    pActual[1] = cbact;
    pActual[2] = cract;

    AVIN_TEST_LOGD("Pixel[%d,%d] = %d|%d|%d --> %d|%d|%d\n", x, y,
        pActual[0], pActual[1], pActual[2], pExpected[0], pExpected[1], pExpected[2]);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_calculategainoffset(TEST_AVIN_HLD *h, float* gain, float* offset,
    UINT8 expected1, UINT8 actual1, UINT8 expected2, UINT8 actual2)
{
    HRESULT r = SUCCESS;
    double temp;

    if (actual1 == actual2) {
        AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
        return r;
    }

    temp = *gain;
    temp *= (double)(expected1 - expected2);
    temp /= (double)(actual1 - actual2);
    *gain = (float)temp;

    temp = *offset;
    temp -= actual1;
    temp *= (double)(expected1 - expected2);
    temp /= (double)(actual1 - actual2);
    temp += expected1;
    *offset = (float)temp;

    AVIN_TEST_LOGD("ADC gain = %f offset = %f\n", *gain, *offset);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static inline INT32 test_avin_abs(INT32 n)
{
    return ((n > 0) ? n : -n);
}

static HRESULT test_avin_colorbarread(TEST_AVIN_HLD *h)
{
    HRESULT r = SUCCESS;
    INT32 i, steps;
    AMP_AVIN_INPUT_PARAMS inputParams;
    AMP_AVIN_ADC_CALIB_DATA_EXT pixelData;
    const char *colorBarName[] = {
        "White", "Yellow", "Cyan", "Green", "Magenta", "Red", "Blue", "Black"
    };

    AmpMemClear((VOID*) &inputParams, sizeof(AMP_AVIN_INPUT_PARAMS));
    AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);

    AVIN_TEST_LOGD("enter\n");

    TEST_AVIN_PRINT("----------- Expected ----- Actual ---- Diff -\n");
    if (inputParams.videoParams.videoHeight == 480) {
        steps = 7;
    } else {
        steps = 8;
    }
    AmpMemClear(&pixelData, sizeof(pixelData));
    for (i = 0; i < steps; ++i) {
        test_avin_getpixeloncolorbar(h, i, pixelData.expected, pixelData.actual);
        TEST_AVIN_PRINT("%8s: (%3d,%3d,%3d) (%3d,%3d,%3d) (%d,%d,%d)\n",
                colorBarName[i], pixelData.expected[0],
                pixelData.expected[1], pixelData.expected[2],
                pixelData.actual[0], pixelData.actual[1], pixelData.actual[2],
                test_avin_abs(pixelData.expected[0] - pixelData.actual[0]),
                test_avin_abs(pixelData.expected[1] - pixelData.actual[1]),
                test_avin_abs(pixelData.expected[2] - pixelData.actual[2]));
    }
    TEST_AVIN_PRINT("----------- Expected ----- Actual ---- Diff -\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static inline INT32 test_avin_ycbcr(UINT8 colorBarStep)
{
    switch (colorBarStep) {
        case 0:
        case 7:
            return 0;
        case 5:
            return 2;
        case 6:
            return 1;
        default:
            return 0;
    }
}

static HRESULT test_avin_calibrate_decoder(TEST_AVIN_HLD *h,
        AMP_AVIN_REGISTER *reg1, UINT8 colorBarStep1,
        AMP_AVIN_REGISTER *reg2, UINT8 colorBarStep2,
        UINT8 colorbarTotalSteps)
{
    HRESULT r = SUCCESS;
    UINT32 regValue[2];
    AMP_AVIN_ADC_CALIB_DATA_EXT pixelData[2];
    INT32 i, j, k, diff, diff1, diff2, ycbcr1, ycbcr2, count;

    typedef struct CALIB_RESULT {
        UINT32 regValue1;
        UINT32 regValue2;
        INT32 diff;
    } CALIB_RESULT;
    CALIB_RESULT *calibResult;

    AVIN_TEST_LOGD("enter\n");

    regValue[0] = reg1->value;
    regValue[1] = reg2->value;

    calibResult = AmpMalloc(sizeof(CALIB_RESULT) * 256 * 256);
    assert(NULL != calibResult);
    AmpMemClear(calibResult, sizeof(CALIB_RESULT) * 256 * 256);

    count = 0;
    ycbcr1 = test_avin_ycbcr(colorBarStep1);
    ycbcr2 = test_avin_ycbcr(colorBarStep2);
    if (ycbcr1 == 0 || ycbcr2 == 0) {
        ycbcr1 = ycbcr2 = 0;
    }
    for (i = 0; i < 256; ++i) {
        reg1->value = i;
        AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, reg1);
        MV_OSAL_Task_Sleep(200);

        AmpMemClear(&pixelData[0], sizeof(AMP_AVIN_ADC_CALIB_DATA_EXT));
        test_avin_getpixeloncolorbar(h, colorBarStep1,
                pixelData[0].expected, pixelData[0].actual);
        diff1 = test_avin_abs(pixelData[0].expected[ycbcr1] - pixelData[0].actual[ycbcr1]);
        if (diff1 < 2) {
            for (j = 0; j < 256; ++j) {
                reg2->value = j;
                AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, reg2);
                MV_OSAL_Task_Sleep(200);

                AmpMemClear(&pixelData[1], sizeof(AMP_AVIN_ADC_CALIB_DATA_EXT));
                test_avin_getpixeloncolorbar(h, colorBarStep2,
                        pixelData[1].expected, pixelData[1].actual);
                diff2 = test_avin_abs(pixelData[1].expected[ycbcr2] - pixelData[1].actual[ycbcr2]);
                if (diff2 < 2) {
                    diff = 0;
                    for (k = 0; k < colorbarTotalSteps; ++k) {
                        AmpMemClear(&pixelData[1], sizeof(AMP_AVIN_ADC_CALIB_DATA_EXT));
                        test_avin_getpixeloncolorbar(h, k,
                                pixelData[1].expected, pixelData[1].actual);
                        if (ycbcr1 == 0 && ycbcr2 == 0) {
                            diff += test_avin_abs(
                                    pixelData[1].expected[0] - pixelData[1].actual[0]);
                        } else {
                            diff += test_avin_abs(
                                    pixelData[1].expected[1] - pixelData[1].actual[1]);
                            diff += test_avin_abs(
                                    pixelData[1].expected[2] - pixelData[1].actual[2]);
                        }
                    }
                    calibResult[count].regValue1 = reg1->value;
                    calibResult[count].regValue2 = reg2->value;
                    calibResult[count].diff      = diff;
                    ++count;
                    TEST_AVIN_PRINT("0x%x[0x%02x] 0x%x[0x%02x] diff %d\n",
                            reg1->addr, reg1->value, reg2->addr, reg2->value, diff);
                }
            }
            reg2->value = regValue[1];
            AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, reg2);
            MV_OSAL_Task_Sleep(200);
        }
    }

    k = 0;
    for (i = 0; i < 64; ++i) {
        for (j = 0; j < count; ++j) {
            if (calibResult[j].diff == i) {
                calibResult[k].regValue1 = calibResult[j].regValue1;
                calibResult[k].regValue2 = calibResult[j].regValue2;
                calibResult[k].diff      = calibResult[j].diff;
                ++k;
            }
        }
        if (k > 0) {
            break;
        }
    }
    if (k > 0) {
        reg1->value = calibResult[k/2].regValue1;
        reg2->value = calibResult[k/2].regValue2;
    } else {
        reg1->value = regValue[0];
        reg2->value = regValue[1];
    }
    AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, reg1);
    AMP_RPC(r, AMP_AVIN_SetRegister, h->avin, reg2);
    MV_OSAL_Task_Sleep(200);

    AmpFree(calibResult);
    calibResult = NULL;

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_calibrateadc(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    AMP_AVIN_ADC_CALIB_DATA_EXT pixelData[2];
    AMP_AVIN_ADC_CALIB_DATA ADCParams;
    AMP_AVIN_INPUT_PARAMS inputParams;
    AMP_AVIN_REGISTER reg[2] = {0};
    INT32 i, diff[3] = {0, 0, 0};
    INT32 regBaseAddr, colorbarTotalSteps;

    AVIN_TEST_LOGD("enter\n");

    if (strcmp(h->chip_name, "cypress") == 0) {
        regBaseAddr = 0x0D00;
    } else if (strcmp(h->chip_name, "AVIF") == 0) {
        regBaseAddr = 0x0700;
    } else {
        TEST_AVIN_PRINT("Chip %s is not supported!\n", h->chip_name);
        AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
        return r;
    }

    AmpMemClear((VOID*) &inputParams, sizeof(AMP_AVIN_INPUT_PARAMS));
    AmpMemClear(&pixelData, sizeof(AMP_AVIN_ADC_CALIB_DATA_EXT)*2);
    AmpMemClear(&ADCParams, sizeof(AMP_AVIN_ADC_CALIB_DATA));

    if (test_avin_isHdmisource(h->curSrc)) {
        TEST_AVIN_PRINT("%s No need calibrate ADC!\n",
            test_avin_inputSourceString(h->curSrc));
        AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
        return r;
    }

    //1) CSC ADC: Reset ADC as default
    for (i = 0; i < 3; ++i) {
        ADCParams.gain[i]   = 1;
        ADCParams.offset[i] = 0;
    }
    AMP_RPC(r, AMP_AVIN_SetAdcGainOffset, h->avin, &ADCParams);
    MV_OSAL_Task_Sleep(200);

    AMP_RPC(r, AMP_AVIN_GetInputParams, h->avin, &inputParams);
    colorbarTotalSteps = ((inputParams.videoParams.videoHeight == 480) ? 7 : 8);

    //2) GFX ADC: Calibrate ADC gain for VGA and SCART-RGB
    //2) DEC ADC: Calibrate YCbCr for ATV, CVBS and SCART-CVBS
    if((AMP_AVIN_SOURCE_TYPE_VGA_01 == h->curSrc) ||
            (AMP_AVIN_SOURCE_TYPE_VGA_02 == h->curSrc) ||
            ((AMP_AVIN_SOURCE_TYPE_SCART_01 == h->curSrc) &&
                (AMP_AVIN_SCART_FB_RGB == inputParams.videoParams.scartFB)) ||
            ((AMP_AVIN_SOURCE_TYPE_SCART_02 == h->curSrc) &&
                (AMP_AVIN_SCART_FB_RGB == inputParams.videoParams.scartFB))){

        AVIN_TEST_LOGD("Step1 %s Calibrate ADC Gain Ext!\n",
            test_avin_inputSourceString(h->curSrc));
        for (i = 0; i < 3; ++i) {
            ADCParams.gain[i]   = 512;
            ADCParams.offset[i] = 0;
        }
        AMP_RPC(r, AMP_AVIN_SetAdcGainExt, h->avin, &ADCParams);
        MV_OSAL_Task_Sleep(200);
        r = test_avin_getpixeloncolorbar(h, 0, pixelData[0].expected, pixelData[0].actual);
        AMP_RPC(r, AMP_AVIN_CalibrateAdc, h->avin, &pixelData[0]);
        AMP_RPC(r, AMP_AVIN_GetAdcGainExt, h->avin, &ADCParams);
        TEST_AVIN_PRINT("ADC Gain Ext: %f %f %f\n", ADCParams.gain[0],
            ADCParams.gain[1], ADCParams.gain[2]);
        MV_OSAL_Task_Sleep(200);
    } else if (test_avin_isCVBSsource(h->curSrc, inputParams.videoParams.scartFB)) {
        AVIN_TEST_LOGD("Step1 %s Calibrate Y!\n", test_avin_inputSourceString(h->curSrc));
        reg[0].addr = regBaseAddr + 0x44;
        reg[1].addr = regBaseAddr + 0x45;
        AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg[0]);
        AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg[1]);
        test_avin_calibrate_decoder(h, &reg[0], 0, &reg[1],
                colorbarTotalSteps - 1, colorbarTotalSteps);
        TEST_AVIN_PRINT("Calibrate Y finished: 0x%x[0x%02x] 0x%x[0x%02x]\n",
                reg[0].addr, reg[0].value, reg[1].addr, reg[1].value);

        AVIN_TEST_LOGD("Step1 %s Calibrate CbCr!\n", test_avin_inputSourceString(h->curSrc));
        reg[0].addr = regBaseAddr + 0x42;
        reg[1].addr = regBaseAddr + 0x43;
        AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg[0]);
        AMP_RPC(r, AMP_AVIN_GetRegister, h->avin, &reg[1]);
        test_avin_calibrate_decoder(h, &reg[0], 6, &reg[1], 5, colorbarTotalSteps);
        TEST_AVIN_PRINT("Calibrate CbCr finished: 0x%x[0x%02x] 0x%x[0x%02x]\n",
                reg[0].addr, reg[0].value, reg[1].addr, reg[1].value);
    }

    //3) CSC ADC: Calibrate the gain/offset of Y channel through White/Black bar
    //            Get the Y_White of white bar and Y_Black of black bar
    //            Calculate the gain/offset by 4 values (expected and actual Y_White/Y_Black)
    AVIN_TEST_LOGD("Step2 %s Calibrate ADC (Y) Gain Offset!\n",
            test_avin_inputSourceString(h->curSrc));
    r = test_avin_getpixeloncolorbar(h, 0, pixelData[0].expected, pixelData[0].actual);
    r = test_avin_getpixeloncolorbar(h, colorbarTotalSteps - 1,
            pixelData[1].expected, pixelData[1].actual);
    AMP_RPC(r, AMP_AVIN_GetAdcGainOffset, h->avin, &ADCParams);
    r = test_avin_calculategainoffset(h, &ADCParams.gain[0], &ADCParams.offset[0],
            pixelData[0].expected[0], pixelData[0].actual[0],
            pixelData[1].expected[0], pixelData[1].actual[0]);
    AMP_RPC(r, AMP_AVIN_SetAdcGainOffset, h->avin, &ADCParams);
    MV_OSAL_Task_Sleep(200);

    //4) CSC ADC: Calibrate the gain/offset of Pb channel through Yellow/Blue bar
    //            Get the Cb_Yellow of yellow bar and Cb_Blue of blue bar
    //            Calculate the gain/offset by 4 values (expected and actual Cb_Yellow/Cb_Blue)
    AVIN_TEST_LOGD("Step3 %s Calibrate ADC (Pb) Gain Offset!\n",
            test_avin_inputSourceString(h->curSrc));
    r = test_avin_getpixeloncolorbar(h, 1, pixelData[0].expected, pixelData[0].actual);
    r = test_avin_getpixeloncolorbar(h, 6, pixelData[1].expected, pixelData[1].actual);
    AMP_RPC(r, AMP_AVIN_GetAdcGainOffset, h->avin, &ADCParams);
    r = test_avin_calculategainoffset(h, &ADCParams.gain[1], &ADCParams.offset[1],
            pixelData[0].expected[1], pixelData[0].actual[1],
            pixelData[1].expected[1], pixelData[1].actual[1]);
    AMP_RPC(r, AMP_AVIN_SetAdcGainOffset, h->avin, &ADCParams);
    MV_OSAL_Task_Sleep(200);

    //5) CSC ADC: Calibrate the gain/offset of Pr channel through Cyan/Red bar
    //            Get the Cr_Cyan of cyan bar and Cr_Red of red bar
    //            Calculate the gain/offset by 4 values (expected and actual Cr_Cyan/Cr_Red)
    AVIN_TEST_LOGD("Step4 %s Calibrate ADC (Pr) Gain Offset!\n",
            test_avin_inputSourceString(h->curSrc));
    r = test_avin_getpixeloncolorbar(h, 2, pixelData[0].expected, pixelData[0].actual);
    r = test_avin_getpixeloncolorbar(h, 5, pixelData[1].expected, pixelData[1].actual);
    AMP_RPC(r, AMP_AVIN_GetAdcGainOffset, h->avin, &ADCParams);
    r = test_avin_calculategainoffset(h, &ADCParams.gain[2], &ADCParams.offset[2],
            pixelData[0].expected[2], pixelData[0].actual[2],
            pixelData[1].expected[2], pixelData[1].actual[2]);
    AMP_RPC(r, AMP_AVIN_SetAdcGainOffset, h->avin, &ADCParams);
    MV_OSAL_Task_Sleep(200);

    //6) Verify the gain/offset
    //   Get the actual YCbCr of each bar
    //   and calculate the average of diff between the expected and the actual
    AVIN_TEST_LOGD("Step5 %s Verify ADC Gain Offset!\n",
            test_avin_inputSourceString(h->curSrc));
    diff[0] = 0;
    diff[1] = 0;
    diff[2] = 0;
    for (i = 0; i < colorbarTotalSteps; i++) {
        r = test_avin_getpixeloncolorbar(h, i, pixelData[0].expected, pixelData[0].actual);
        diff[0] += ((INT32)pixelData[0].expected[0] - (INT32)pixelData[0].actual[0]);
        diff[1] += ((INT32)pixelData[0].expected[1] - (INT32)pixelData[0].actual[1]);
        diff[2] += ((INT32)pixelData[0].expected[2] - (INT32)pixelData[0].actual[2]);
    }
    diff[0] /= i;
    diff[1] /= i;
    diff[2] /= i;
    AVIN_TEST_LOGD("Ydiff = %d, Pbdiff = %d, Prdiff = %d", diff[0], diff[1], diff[2]);
    AMP_RPC(r, AMP_AVIN_GetAdcGainOffset, h->avin, &ADCParams);
    TEST_AVIN_PRINT("Calibrate ADC Gain Offset Complete!\n");
    TEST_AVIN_PRINT("ADC Gain: %f %f %f\n", ADCParams.gain[0],
            ADCParams.gain[1], ADCParams.gain[2]);
    TEST_AVIN_PRINT("ADC Offset: %f %f %f\n", ADCParams.offset[0],
            ADCParams.offset[1], ADCParams.offset[2]);

    test_avin_colorbarread(h);

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static HRESULT test_avin_set_audio_block(TEST_AVIN_HLD *h, UINT8 *pArgv)
{

    UINT8 DtdOfs, DataBlkOfs, DataBlkTag, DataBlkLen, EndOfDataBlk;
    UINT8 NewDataOfs, maxCh, srcSADOfs, dstSADOfs;
    UINT8 audFmt, audFmtSupported, audLength;
    UINT8 chk_sum = 0, audBlkDone = 0, skipSAD = 0;
    UINT16 i, uIdx;
    int cmdnums = 0, enable = 0;
    int phyAddr;

    UINT8 ceaBlk[EDID_BLK_SIZE];
    UINT8 temp[EDID_BLK_SIZE];
    HRESULT r = SUCCESS;

    AMP_DISP_HDMI_SINK_CAPS sinkCaps;
    AMP_AVIN_EDID_INFO EdidInfo;


    AVIN_TEST_LOGD("enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        enable = htoi(h->pcommand[0]);
    }

    AmpMemClear(&EdidInfo, sizeof(AMP_AVIN_EDID_INFO));
    AmpMemClear(&sinkCaps, sizeof(AMP_DISP_HDMI_SINK_CAPS));
    AMP_RPC(r, AMP_DISP_OUT_HDMI_GetSinkCaps, h->voutDisp, &sinkCaps);

    if(enable) {

        if(SUCCESS != r) {
            TEST_AVIN_PRINT("Sink not connected\n");
            return r;
        }

        if(h->EdidInfo.size == 0) {
            h->EdidInfo.source = 1;
            memset(h->EdidInfo.data, 0 , EDID_DATA_SIZE);
            AMP_RPC(r, AMP_AVIN_ReadEdid, h->avin, &h->EdidInfo);
            h->EdidInfo.size = EDID_DATA_SIZE;
        }

        GaloisMemSet(ceaBlk, 0, EDID_BLK_SIZE);
        GaloisMemcpy(EdidInfo.data, h->EdidInfo.data, EDID_DATA_SIZE);
        GaloisMemcpy(temp, &EdidInfo.data[128], EDID_BLK_SIZE);
        GaloisMemcpy(ceaBlk, &EdidInfo.data[128], EDID_CEA_EXTN_DATA_BLK_ADDR);
        EdidInfo.source = 1;
        EdidInfo.size = EDID_DATA_SIZE;

        DtdOfs = temp[EDID_CEA_EXTN_DTD_OFS_ADDR];
        DataBlkOfs = EDID_CEA_EXTN_DATA_BLK_ADDR;
        NewDataOfs =  DataBlkOfs;
        EndOfDataBlk = 0;

        while (!EndOfDataBlk)
        {
            // Parse data block header
            DataBlkTag = (temp[DataBlkOfs] & 0xE0) >> 5;
            DataBlkLen = (temp[DataBlkOfs] & 0x1F);
            if(DataBlkTag != EDID_CEA_AUDIO_BLK_TAG) {
                if(!audBlkDone) {
                    //Copy this data blk to new cea data block
                    GaloisMemcpy(&ceaBlk[NewDataOfs], &temp[DataBlkOfs], DataBlkLen+1);

                    // Update offset
                    DataBlkOfs += (DataBlkLen+1);
                    NewDataOfs  += (DataBlkLen+1);
                    if ((DtdOfs != 0) && (DataBlkOfs >= EDID_BLK_SIZE)) {
                        EndOfDataBlk = 1;
                    }
                } else {
                     // Copy remaining edid data except checksum
                     GaloisMemcpy(&ceaBlk[NewDataOfs], &temp[DataBlkOfs],
                                   (EDID_BLK_SIZE -DataBlkOfs-1));
                     break;
                }
            } else {

                audBlkDone = 1;
                NewDataOfs = DataBlkOfs+DataBlkLen+1;
                ceaBlk[DataBlkOfs] = temp[DataBlkOfs];
                audLength = DataBlkLen;

                /* Update Rx audio block with Tx audio block*/
                for(i=0;i<DataBlkLen/EDID_CEA_SAD_LENGTH;i++)
                {
                    audFmtSupported = 0;
                    srcSADOfs = DataBlkOfs + 1 + i*EDID_CEA_SAD_LENGTH;
                    dstSADOfs = DataBlkOfs + 1 + (i - skipSAD)*EDID_CEA_SAD_LENGTH;
                    audFmt = (temp[srcSADOfs] & 0x78)>>3;
                    maxCh = (temp[srcSADOfs] & 0x07);
                    for (uIdx = 0; uIdx < 15; ++uIdx) {
                        if(audFmt == sinkCaps.AudioInfo[uIdx].iAudioFmt) {
                            //Fomrat supported by Rx & sink - Copy it to Rx
                            if(maxCh > sinkCaps.AudioInfo[uIdx].uiMaxNumChnls-1)
                                maxCh = sinkCaps.AudioInfo[uIdx].uiMaxNumChnls-1;

                            //Copy the least of max ch supported
                            ceaBlk[dstSADOfs] = ((audFmt << 3) | maxCh);

                            //Copy the frequency supported by both Rx and Sink
                            ceaBlk[dstSADOfs+1] = (temp[srcSADOfs+1] & \
                            sinkCaps.AudioInfo[uIdx].FreqSprt.uiAudioFreqSprtInfo>>1);

                            if(audFmt == EDID_AUD_FMT_LPCM) {
                                ceaBlk[dstSADOfs+2] = temp[srcSADOfs+2] & \
                                (sinkCaps.AudioInfo[uIdx].WdLenSprt.uiAudioWdlenSprtInfo >>1);
                            } else {
                                if (audFmt >= EDID_AUD_FMT_AC3 &&
                                    audFmt <= EDID_AUD_FMT_ATRAC)
                                    ceaBlk[dstSADOfs+2] = sinkCaps.AudioInfo[uIdx].uiMaxBitRate/8;
                                else
                                    ceaBlk[dstSADOfs+2] = sinkCaps.AudioInfo[uIdx].uiMaxBitRate;
                            }

                            audFmtSupported = 1;
                            break;
                        }
                    }

                    if(audFmtSupported == 0)
                    {
                        //Sink doesn't support this audio format - remove it from Rx
                        audLength -= EDID_CEA_SAD_LENGTH;
                        NewDataOfs -= EDID_CEA_SAD_LENGTH;
                        skipSAD++;
                        ceaBlk[DataBlkOfs] = (EDID_CEA_AUDIO_BLK_TAG<<5) | (audLength&0x1F);
                    }
                }

                DataBlkOfs += (DataBlkLen+1);
            }
        }

        ceaBlk[EDID_CEA_EXTN_DTD_OFS_ADDR] -= (skipSAD*3);
        cec_calculate_checksum(ceaBlk);
        GaloisMemcpy(&EdidInfo.data[EDID_BLK_SIZE], ceaBlk, EDID_BLK_SIZE);

        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 0);

        AMP_RPC(r, AMP_AVIN_UpdateEdid, h->avin, &EdidInfo);

        MV_OSAL_Task_Sleep(200);
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 1);

        if (SUCCESS == r) {
            TEST_AVIN_PRINT("Write %s EDID[%d] complete\n",
                test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
        } else {
            TEST_AVIN_PRINT("Write %s EDID[%d] error 0x%x\n",
                test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
        }

    } else if(h->EdidInfo.size == EDID_DATA_SIZE) {

        if(SUCCESS == r) {
            phyAddr = sinkCaps.uiCecPhyAddr;
            if ((phyAddr & 0xF000) == phyAddr)
                phyAddr |= 0x100;
            else if ((phyAddr & 0xFF00) == phyAddr)
                phyAddr |= 0x10;
            else if ((phyAddr & 0xFFF0) == phyAddr)
                phyAddr |= 0x1;
            else if ((phyAddr & 0xFFFF) == phyAddr)
                phyAddr = 0xFFFF;  //all positions filled
            AVIN_TEST_LOGD("Set CEC physical address 0x%X\n", phyAddr);


            for (i = 128; i < EDID_DATA_SIZE; ++i) {
                if (h->EdidInfo.data[i] == 0x03 && h->EdidInfo.data[i + 1] == 0x0C &&
                        h->EdidInfo.data[i + 2] == 0x00) {
                    h->EdidInfo.data[i+3] = (phyAddr >> 8) & 0xFF;
                    h->EdidInfo.data[i+4] = (phyAddr) & 0xFF;
                    AVIN_TEST_LOGD("CEC physical address updated in EDID\n");
                    break;
                }
            }
            if (i < EDID_DATA_SIZE) {       //Updated EDID calculare checksum
                cec_calculate_checksum(&h->EdidInfo.data[128]);
            }
        }

        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 0);

        AMP_RPC(r, AMP_AVIN_UpdateEdid, h->avin, &h->EdidInfo);

        MV_OSAL_Task_Sleep(200);
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 1);

        if (SUCCESS == r) {
            TEST_AVIN_PRINT("Write %s Original EDID[%d] complete\n",
                test_avin_inputSourceString(h->EdidInfo.source), h->EdidInfo.size);
        } else {
            TEST_AVIN_PRINT("Write %s Original EDID[%d] error 0x%x\n",
                test_avin_inputSourceString(h->EdidInfo.source), h->EdidInfo.size);
        }

    }
    return r;
}

static HRESULT test_avin_edid_passthrough(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    UINT8 temp[EDID_BLK_SIZE];
    HRESULT r = SUCCESS;
    int cmdnums = 0, enable = 0;

    AMP_DISP_HDMI_RAW_EDID sinkEdid;
    AMP_AVIN_EDID_INFO EdidInfo;

    AVIN_TEST_LOGD("jerry enter\n");

    cmdnums = test_avin_parse_argv(pArgv, h);
    if( cmdnums > 0){
        enable = htoi(h->pcommand[0]);
    }

    if (enable) {
        AmpMemClear(&EdidInfo, sizeof(AMP_AVIN_EDID_INFO));
        AmpMemClear(&sinkEdid, sizeof(AMP_DISP_HDMI_RAW_EDID));
        AMP_RPC(r, AMP_DISP_OUT_HDMI_GetRawEdid, h->voutDisp, &sinkEdid);

        if(SUCCESS != r) {
            TEST_AVIN_PRINT("Sink not connected\n");
            return r;
        }

        if(h->EdidInfo.size == 0) {
            h->EdidInfo.source = 1;
            memset(h->EdidInfo.data, 0 , EDID_DATA_SIZE);
            AMP_RPC(r, AMP_AVIN_ReadEdid, h->avin, &h->EdidInfo);
            h->EdidInfo.size = EDID_DATA_SIZE;
        }
        GaloisMemcpy(EdidInfo.data, h->EdidInfo.data, EDID_DATA_SIZE);
        EdidInfo.source = 1;
        EdidInfo.size = EDID_DATA_SIZE;

       // test_avin_edid_pt_update(sinkEdid.uiDataBuf, EdidInfo.data);

        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 0);

        AMP_RPC(r, AMP_AVIN_UpdateEdid, h->avin, &EdidInfo);

        MV_OSAL_Task_Sleep(200);
        AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 1);
        return 0;

        if (SUCCESS == r) {
            TEST_AVIN_PRINT("Write %s EDID[%d] complete\n",
                test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
        } else {
            TEST_AVIN_PRINT("Write %s EDID[%d] error 0x%x\n",
                test_avin_inputSourceString(EdidInfo.source), EdidInfo.size);
        }
    } else {
        if(h->EdidInfo.size == 0) {
            TEST_AVIN_PRINT("EDID not updated\n");
        } else {
            AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 0);

            AMP_RPC(r, AMP_AVIN_UpdateEdid, h->avin, &h->EdidInfo);

            MV_OSAL_Task_Sleep(200);
            AMP_RPC(r, AMP_AVIN_ToggleHpd, h->avin, 1);

            if (SUCCESS == r) {
                TEST_AVIN_PRINT("Write %s Original EDID[%d] complete\n",
                    test_avin_inputSourceString(h->EdidInfo.source), h->EdidInfo.size);
            } else {
                TEST_AVIN_PRINT("Write %s Original EDID[%d] error 0x%x\n",
                    test_avin_inputSourceString(h->EdidInfo.source), h->EdidInfo.size);
            }
        }
    }

    TEST_AVIN_PRINT("done\n");

    return r;
}


static AVIN_COMMAND pCommand[] = {
    {"help",        test_avin_cmd_help,         "Show help", TRUE},
    {"input",       test_avin_change_source,    "Change port for cypress", FALSE},
    {"autosource",  test_avin_autosource,       "Source switch test", TRUE},
    {"r",           test_avin_print_regs,       "Read AVIF/BG2 registers", TRUE},
    {"rr",          test_avin_reg_monitor,      "Monitor AVIF/BG2 registers", TRUE},
    {"w",           test_avin_set_avinreg,      "Write AVIF/BG2 registers", TRUE},
    {"ww",          test_avin_set_avinreg_loop, "Repeatly write register", TRUE},
    {"tw",          test_avin_set_avinreg_try,  "Try write registers from 0x00 to 0xFF", TRUE},
    {"video",       test_avin_cmd_dump_video,   "Dump one video frame", TRUE},
    {"audios",      test_avin_cmd_dump_audios,  "Dump multiple audio frames", TRUE},
    {"videos",      test_avin_cmd_dump_videos,  "Dump multiple video frames", TRUE},
    {"vbis",        test_avin_cmd_dump_vbis,    "Dump multiple vbi data", TRUE},
    {"muteaudio",   test_avin_set_audiomute,    "Mute audio from sound service", TRUE},
    {"unmuteaudio", test_avin_set_audiounmute,  "Unmute audio from sound service", TRUE},
    {"mutevideo",   test_avin_set_videomute,    "Mute video plane", TRUE},
    {"unmutevideo", test_avin_set_videounmute,  "Unmute video plane", TRUE},
    {"dumpreg",     test_avin_dump_avinregs,    "Dump Cypress or HDMIRX registers", TRUE},
    {"dump",        test_avin_dump_avin,        "Dump register,audio frame,video frame", TRUE},
    {"com2d",       test_avin_set3dcomb_2d,     "Set CVBS 3D comb to 2D", TRUE},
    {"com3d",       test_avin_set3dcomb_3d,     "Set CVBS 3D comb to 3D", TRUE},
    {"com3da",      test_avin_set3dcomb_3dAdap, "Set CVBS 3D comb to 3D Adaptive", TRUE},
    {"evbi",        test_avin_enable_vbidump,   "Enable dump vbi", TRUE},
    {"dvbi",        test_avin_disable_vbidump,  "Disable dump vbi", TRUE},
    {"getfetg",     test_avin_getfetg,          "Get the VIP fetg parameters", TRUE},
    {"getpixel",    test_avin_getpixel,         "Get VIP pixel value", TRUE},
    {"getvipstate", test_avin_getvipstatus,     "Get the vip status", TRUE},
    {"hpd",         test_avin_togglehpd,        "Toggle HDMI HPD [0|1]", TRUE},
    {"autoadj",     test_avin_vgaautoadjust,    "VGA auto adjustment", TRUE},
    {"redid",       test_avin_readedid,         "Read EDID [source][size]", TRUE},
    {"wedid",       test_avin_writeedid,        "Write EDID [source][file]", TRUE},
    {"setadc",      test_avin_setadc,           "Set ADC gain and offset", TRUE},
    {"getadc",      test_avin_getadc,           "Get ADC gain and offset", TRUE},
    {"getadcgainext",test_avin_getadcgainext,   "Get ADC gain of Scart-RGB", TRUE},
    {"setadcgainext",test_avin_setadcgainext,   "Set ADC gain of Scart-RGB", TRUE},
    {"calibadcgainext",test_avin_calibrateadcgainext,"Calibrate ADC gain of Scart-RGB ", TRUE},
    {"calibadc",    test_avin_calibrateadc,     "Calibrate ADC gain and offset", TRUE},
    {"hdmiarc",     test_avin_hdmiarc,          "Enable/disable hdmi arc", TRUE},
    {"hdcpkey",     test_avin_hdcpkey,          "Get hdcp key", TRUE},
    {"getrxterm",   test_avin_gettermination,   "Get termination", TRUE},
    {"setrxterm",   test_avin_settermination,   "Set termination", TRUE},
    {"geteqgain",   test_avin_getequlizergain,  "Get Equalizer Gain", TRUE},
    {"seteqgain",   test_avin_setequlizergain,  "Set Equalizer Gain", TRUE},
    {"getgmdpkt",   test_avin_getgmdpkt,        "Get GMD pkt", TRUE},
    {"powerdown",   test_avin_powerdown,        "AVIF power down", TRUE},
    {"getavinfo",   test_avin_getavinfo,        "Get Video Format", TRUE},
    {"getaudca",    test_avin_getaudca,         "Get Hdmi Audio Info channel assignment", TRUE},
    {"gettmdsclk",  test_avin_gettmdsclk,       "Get Hdmi Tmds Clock", TRUE},
    {"sif",         test_avin_sifdetection,     "Start to do sif detection", TRUE},
    {"sifstd",      test_avin_setsifstandard,   "Set sif standard", TRUE},
    {"sifmode",     test_avin_sifconfigure,     "Get and set sif mode", TRUE},
    {"getsifstatus",test_avin_getsifstatus,     "Get sif status", TRUE},
    {"vbicfg",      test_avin_vbiconfigure,     "Configure vbi types and lines", TRUE},
    {"dumpvbicfg",  test_avin_dumpvbiconfigure, "Dump current vbi configuration", TRUE},
    {"setdecmode",  test_avin_setdecmode,       "Set Decoder mode", TRUE},
    {"setcombcof",  test_avin_setcombcoeff,     "Set Comb Coeff", TRUE},
    {"sethshift",   test_avin_setvgahshift,     "Set VGA H Shift", TRUE},
    {"setvshift",   test_avin_setvgavshift,     "Set VGA V Shift", TRUE},
    {"setclock",    test_avin_setvgaclock,      "Set VGA Clock", TRUE},
    {"setphase",    test_avin_setvgaphase,      "Set VGA Phase", TRUE},
    {"setaudio",    test_avin_setaudioparams,   "Set audio params", TRUE},
    {"setvol",      test_avin_setaudiovolume,   "Set audio volume", TRUE},
    {"enavsync",    test_avin_enableavsync,     "Enable AV Sync", FALSE},
    {"disavsync",   test_avin_disableavsync,    "Disable AV Sync", FALSE},
    {"eMS11",       test_avin_enableMS11,       "Enable/Disable MS11", TRUE},
    {"getpltcfg",   test_avin_getplatformconfig,"Get Platform Config", TRUE},
    {"setpcmode",   test_avin_setpcmode,        "Set PC Mode", TRUE},
    {"avifsclenable",test_avin_setscalerenable, "Set scaler enable ", TRUE},
    {"avifptenable",test_avin_setpassthroughenable, "Set passthrough enable ", TRUE},
    {"set2DMode",    test_avin_set2Dmode,       "Set display Mode from 3D to 2D", TRUE},
    {"set3DLRSwap",  test_avin_3DModeLRSwap,    "Swap display 3D left/right frame", TRUE},
    {"set3DMode",    test_avin_set3Dmode,       "Set display(LVDS/HDMITX) 3D output mode", TRUE},
    {"suspend",      test_avin_suspend,         "suspend [rounds] [delay in ms]", TRUE},
    {"enaudiopt",    test_avin_enaudiopassthr,  "Enable audio pass through", TRUE},
    {"disaudiopt",   test_avin_disaudiopassthr, "Disable audio pass through", TRUE},
#ifdef CONFIG_AMP_IP_AUDIO_MS12_APP_ENABLE
    {"enmixaudiopt",    test_avin_enmixaudiopassthr,  "Enable mixaudio pass through", TRUE},
    {"dismixaudiopt",   test_avin_dismixaudiopassthr, "Disable mix audio pass through", TRUE},
    {"enmixaudioptddp",    test_avin_enmixaudiopassthr_ddp,  "Enable force DD/DDP output on mixaudio pass through", TRUE},
    {"enmixaudioptmat",    test_avin_enmixaudiopassthr_mat,  "Enable force MAT output on mixaudio pass through", TRUE},
#endif
    {"setaudblk",    test_avin_set_audio_block,  "Update Rx audio block as per Sink", TRUE},
    {"edidpt",    test_avin_edid_passthrough,  "Update EDID as per Sink", TRUE},
    {"x",           NULL,                       "Exit ampclient_alpha", TRUE},
};

static HRESULT test_avin_cmd_help(TEST_AVIN_HLD *h, UINT8 *pArgv)
{
    HRESULT r = SUCCESS;
    UINT8 uCount = 0, i =0;
    AVIN_COMMAND *pCmd = pCommand;

    TEST_AVIN_PRINT("|------------------------------------------------------\n");
    TEST_AVIN_PRINT("|%s                AVIN Test Command Help%s\n",
        LOG_COLOR_GREEN, LOG_COLOR_AUTO);
    TEST_AVIN_PRINT("|\n");
    TEST_AVIN_PRINT("|ampclient_alpha 7 [cmd] [param1] [param2]...[paramn]\n");
    TEST_AVIN_PRINT("| cmd can be below or shell command:\n");

    uCount = sizeof(pCommand)/sizeof(AVIN_COMMAND);
    for(i=0; i< uCount; i++){
        TEST_AVIN_PRINT("|%14s: %s\n", pCmd->pCommand, pCmd->pDesc);
        pCmd++;
    }
    TEST_AVIN_PRINT("|------------------------------------------------------\n");

    AVIN_TEST_LOGD("leave, r = 0x%x\n", r);
    return r;
}

static AVIN_COMMAND* test_avin_find_cmd(char *cmd)
{
    UINT8 uCount = 0, i =0;
    AVIN_COMMAND *pCmd = pCommand;

    if( NULL == cmd){
        return NULL;
    }
    uCount = sizeof(pCommand)/sizeof(AVIN_COMMAND);
    for(i=0; i< uCount; i++){
        if( 0 == strcmp(pCmd->pCommand, cmd)){
            return pCmd;
        }
        pCmd ++;
    }
    return NULL;
}


static HRESULT test_avinCreateDir(const char *pPath, const char *pDir)
{
    DIR *dirp = NULL, *dirq = NULL;
    struct dirent *dp = NULL;
    BOOL bIsFound = FALSE;
    UINT8 u8SysCmd[AVIN_NAME_MAX_LEN] = { 0 };
    if (NULL == pPath || 0 == strlen(pPath) || NULL == pDir) {
        return ERR_ERRPARAM;
    }

    dirp = opendir(pPath);
    if (NULL == dirp) {
        AMPLOGE("opendir: \"%s\" failed!\n", pPath);
        return ERR_ERRPARAM;
    }

    while (NULL != (dp = readdir(dirp))) {
        snprintf(u8SysCmd, AVIN_NAME_MAX_LEN, "%s%s",
            pPath, dp->d_name);
#ifdef DT_DIR
        if (dp->d_type == DT_DIR ||
            ( dirq = opendir(u8SysCmd) ) != NULL )
#endif
        {
            if (0 == strcmp(dp->d_name, pDir)) {
                bIsFound = TRUE;
                break;
            }
        }
    }

    if (!bIsFound) {
        if ('/' == pPath[strlen(pPath) - 1]) {
            snprintf(u8SysCmd, AVIN_NAME_MAX_LEN, "mkdir -p %s%s",
                pPath, pDir);
        } else {
            snprintf(u8SysCmd, AVIN_NAME_MAX_LEN, "mkdir -p %s/%s",
                pPath, pDir);
        }
        system(u8SysCmd);
    }
    return SUCCESS;
}

void ndi_avin(int argc, char **argv, AMP_IN AMP_FACTORY factory)
{
    HRESULT r = SUCCESS;
    char *pHeader = NULL;
    char *pArgv = NULL;
    BOOL bStopped = TRUE;
    BOOL bIsRunning = TRUE;
    AVIN_COMMAND *pAvincmd = NULL;
    char pCmd[1024*128];
    int cmdcums = 0, i, argcs = 0;

    AVIN_TEST_LOGD("enter.\n");

    // Check if data directory is existed, if no, create it.
    test_avinCreateDir("/", "data");

    pgHld = AmpMalloc(sizeof(TEST_AVIN_HLD));
    assert(NULL != pgHld);
    AmpMemClear(pgHld, sizeof(TEST_AVIN_HLD));
    pgHld->enableLog = TRUE;

    pgHld->mainTaskExit = FALSE;
    r = MV_OSAL_Mutex_Create(&(pgHld->mainTaskExitMutex));
    assert(S_OK == r);
    r = MV_OSAL_Mutex_Create(&(pgHld->audioPipelineMutex));
    assert(S_OK == r);

    pgHld->outputComp = TEST_AVIN_OUPUT_COMP_VOUT;
    pgHld->width = 1920;
    pgHld->height = 1080;
    pgHld->fr_num = 60;
    pgHld->outputPlane = AMP_DISP_PLANE_MAIN;
    pgHld->input_format = AMP_VENC_YUV420_SP;
    pgHld->enc_codec = MEDIA_VES_AVC; //h264
    pgHld->deint = NULL;
    pgHld->isPassthroughOn = FALSE;

    memset(pCmd, 0, sizeof(pCmd));
    for(i=0; i<argc-2; i++){
        strcat(pCmd, argv[i+2]);
        strcat(pCmd, " ");
    }

    if (process_options(argc, argv) != 0) return;

    AVIN_TEST_LOGD("Display plane is %s\n", pgHld->outputPlane?"PIP":"MAIN");
    test_avin_init(factory, pgHld);
    r = test_avin_play(pgHld);

    printf("\033[?25h");/*enable cursor*/

    if( argc > 0){
        cmdcums = test_avin_parse_argv(pCmd, pgHld);
        if( cmdcums > 0){
            pHeader = pgHld->pcommand[0];
        }
        if( cmdcums > 1){
            pArgv = strstr(pCmd, " ");
            pArgv = strstr(pArgv, pgHld->pcommand[1]);
        }

        pAvincmd = test_avin_find_cmd(pHeader);
        if( NULL != pAvincmd){
            TEST_AVIN_PRINT("\n%s-------------Start to execute command"
                "-------------\n", LOG_COLOR_RED );
            TEST_AVIN_PRINT("%s %s%s\n", pHeader,pArgv, LOG_COLOR_AUTO);
            pAvincmd->AVIN_Command(pgHld, pArgv);
            if( pAvincmd->bExitDone){
                TEST_AVIN_PRINT("%s-----------------------finished--------"
                    "------------%s\n", LOG_COLOR_RED, LOG_COLOR_AUTO);
                bIsRunning = FALSE;
                goto exit;
            } else {
                goto next;
            }
        } else {
            TEST_AVIN_PRINT("error command [%s]\n", argv);
            test_avin_cmd_help(pgHld, NULL);
        }
    }

    test_avin_cmd_help(pgHld, NULL);
    test_avin_dumpVideoInfo(pgHld, &pgHld->videoParams);
    test_avin_dumpAudioInfo(pgHld, &pgHld->audioParams);

next:
    TEST_AVIN_PRINT("CMD>");
    if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE ||
            pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
        MV_OSAL_Task_Create(&pgHld->insTask, test_avin_sendBufferTask, pgHld);
        MV_OSAL_Task_Sleep(20);
    }

    while (TRUE == bIsRunning) {
        pArgv = NULL;
        AmpMemClear(pCmd,sizeof(pCmd));
        gets(pCmd);
        cmdcums = test_avin_parse_argv(pCmd, pgHld);
        if( cmdcums > 0){
            pHeader = pgHld->pcommand[0];
        }
        if( cmdcums > 1){
            pArgv = strstr(pCmd, " ");
            pArgv = strstr(pArgv, pgHld->pcommand[1]);
        }

        pAvincmd = test_avin_find_cmd(pHeader);
        if( 0 == strlen(pCmd)){

        } else if(0 == strcmp("x", pCmd)){
            bIsRunning = FALSE;
            pgHld->playBackStopped = TRUE;

        } else if( pAvincmd != NULL ){
            pAvincmd->AVIN_Command(pgHld, pArgv);
        } else {
            system(pCmd);
        }
        TEST_AVIN_PRINT("CMD>");
    }

exit:

    MV_OSAL_Mutex_Lock(pgHld->mainTaskExitMutex);
    pgHld->mainTaskExit = TRUE;
    MV_OSAL_Mutex_Unlock(pgHld->mainTaskExitMutex);

    MV_OSAL_Mutex_Lock(pgHld->audioPipelineMutex);
    r = test_avin_stop(pgHld);
    r = test_avin_deInit(pgHld);
    assert(SUCCESS == r);
    MV_OSAL_Mutex_Unlock(pgHld->audioPipelineMutex);

    r = MV_OSAL_Mutex_Destroy(&(pgHld->mainTaskExitMutex));
    assert(S_OK == r);
    r = MV_OSAL_Mutex_Destroy(&(pgHld->audioPipelineMutex));
    assert(S_OK == r);

    if (pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE ||
            pgHld->outputComp == TEST_AVIN_OUPUT_COMP_VENC_FILE_VOUT) {
        MV_OSAL_Task_Destroy(&pgHld->insTask);
    }

    AmpFree(pgHld);
    pgHld = NULL;
    AVIN_TEST_LOGD("leave.\n");
    return;
}

#endif // #ifdef CONFIG_AMP_COMPONENT_AVIN_ENABLE

