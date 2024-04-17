/*
 *      * DO NOT EDIT!  THIS FILE IS MACHINE-GENERATED! *
 *
 *      amp_client_rpc.h
 *      from file amp_client.prc
 *      with Flick version 2.1 (compiled on Aug 25 2022 10:48:54)
 *      send bug reports to <flick-bugs@cs.utah.edu>
 */

#ifndef _amp_client_rpc_h_
#define _amp_client_rpc_h_
#ifdef __cplusplus
extern "C" {
#endif

#include <flick/link/mvcc.h>
#include <flick/encode/cdr.h>
#include <flick/pres/corba.h>

#ifndef _typedef___UINT8
#define _typedef___UINT8
typedef CORBA_octet UINT8;
#endif /* _typedef___UINT8 */

#ifndef _typedef___UINT16
#define _typedef___UINT16
typedef CORBA_unsigned_short UINT16;
#endif /* _typedef___UINT16 */

#ifndef _typedef___UINT32
#define _typedef___UINT32
typedef CORBA_unsigned_long UINT32;
#endif /* _typedef___UINT32 */

#ifndef _typedef___UINT64
#define _typedef___UINT64
typedef CORBA_unsigned_long_long UINT64;
#endif /* _typedef___UINT64 */

#ifndef _typedef___BOOL
#define _typedef___BOOL
typedef CORBA_boolean BOOL;
#endif /* _typedef___BOOL */

#ifndef _typedef___INT8
#define _typedef___INT8
typedef CORBA_char INT8;
#endif /* _typedef___INT8 */

#ifndef _typedef___INT16
#define _typedef___INT16
typedef CORBA_short INT16;
#endif /* _typedef___INT16 */

#ifndef _typedef___INT32
#define _typedef___INT32
typedef CORBA_long INT32;
#endif /* _typedef___INT32 */

#ifndef _typedef___INT64
#define _typedef___INT64
typedef CORBA_long_long INT64;
#endif /* _typedef___INT64 */

#ifndef _typedef___HRESULT
#define _typedef___HRESULT
typedef CORBA_long HRESULT;
#endif /* _typedef___HRESULT */

#ifndef _typedef___AMP_STATE
#define _typedef___AMP_STATE
typedef UINT8 AMP_STATE;
#endif /* _typedef___AMP_STATE */

#ifndef _define___AMP_LOADED
#define _define___AMP_LOADED
#define AMP_LOADED (0)
#endif

#ifndef _define___AMP_IDLE
#define _define___AMP_IDLE
#define AMP_IDLE (1)
#endif

#ifndef _define___AMP_EXECUTING
#define _define___AMP_EXECUTING
#define AMP_EXECUTING (2)
#endif

#ifndef _define___AMP_PAUSED
#define _define___AMP_PAUSED
#define AMP_PAUSED (3)
#endif

#ifndef _typedef___AMP_PTS
#define _typedef___AMP_PTS
typedef UINT64 AMP_PTS;
#endif /* _typedef___AMP_PTS */

#ifndef _typedef___AMP_MEDIA_TYPE
#define _typedef___AMP_MEDIA_TYPE
typedef UINT32 AMP_MEDIA_TYPE;
#endif /* _typedef___AMP_MEDIA_TYPE */

#ifndef _define___AMP_MEDIA_MAX
#define _define___AMP_MEDIA_MAX
#define AMP_MEDIA_MAX (128)
#endif

#ifndef _typedef___AMP_MEDIA_CAP
#define _typedef___AMP_MEDIA_CAP
typedef UINT32 AMP_MEDIA_CAP[4];
#endif /* _typedef___AMP_MEDIA_CAP */

#ifndef _typedef___AMP_MEDIA_CAP_slice
#define _typedef___AMP_MEDIA_CAP_slice
typedef UINT32 AMP_MEDIA_CAP_slice;
#endif /* _typedef___AMP_MEDIA_CAP_slice */

#ifndef _define___MEDIA_INVALIDATE
#define _define___MEDIA_INVALIDATE
#define MEDIA_INVALIDATE (0)
#endif

#ifndef _define___MEDIA_MPEG2_TS
#define _define___MEDIA_MPEG2_TS
#define MEDIA_MPEG2_TS (1)
#endif

#ifndef _define___MEDIA_MPEG2_PS
#define _define___MEDIA_MPEG2_PS
#define MEDIA_MPEG2_PS (2)
#endif

#ifndef _define___MEDIA_MPEG2_PES
#define _define___MEDIA_MPEG2_PES
#define MEDIA_MPEG2_PES (3)
#endif

#ifndef _define___MEDIA_MPEG2_ES
#define _define___MEDIA_MPEG2_ES
#define MEDIA_MPEG2_ES (4)
#endif

#ifndef _define___MEDIA_MPEG2_TABLE
#define _define___MEDIA_MPEG2_TABLE
#define MEDIA_MPEG2_TABLE (5)
#endif

#ifndef _define___MEDIA_VES_MPEG1
#define _define___MEDIA_VES_MPEG1
#define MEDIA_VES_MPEG1 (16)
#endif

#ifndef _define___MEDIA_VES_MPEG2
#define _define___MEDIA_VES_MPEG2
#define MEDIA_VES_MPEG2 (17)
#endif

#ifndef _define___MEDIA_VES_H263
#define _define___MEDIA_VES_H263
#define MEDIA_VES_H263 (18)
#endif

#ifndef _define___MEDIA_VES_MPEG4
#define _define___MEDIA_VES_MPEG4
#define MEDIA_VES_MPEG4 (19)
#endif

#ifndef _define___MEDIA_VES_WMV
#define _define___MEDIA_VES_WMV
#define MEDIA_VES_WMV (20)
#endif

#ifndef _define___MEDIA_VES_RV
#define _define___MEDIA_VES_RV
#define MEDIA_VES_RV (21)
#endif

#ifndef _define___MEDIA_VES_AVC
#define _define___MEDIA_VES_AVC
#define MEDIA_VES_AVC (22)
#endif

#ifndef _define___MEDIA_VES_MJPEG
#define _define___MEDIA_VES_MJPEG
#define MEDIA_VES_MJPEG (23)
#endif

#ifndef _define___MEDIA_VES_VC1
#define _define___MEDIA_VES_VC1
#define MEDIA_VES_VC1 (24)
#endif

#ifndef _define___MEDIA_VES_VP8
#define _define___MEDIA_VES_VP8
#define MEDIA_VES_VP8 (25)
#endif

#ifndef _define___MEDIA_VES_DIVX
#define _define___MEDIA_VES_DIVX
#define MEDIA_VES_DIVX (26)
#endif

#ifndef _define___MEDIA_VES_XVID
#define _define___MEDIA_VES_XVID
#define MEDIA_VES_XVID (27)
#endif

#ifndef _define___MEDIA_VES_AVS
#define _define___MEDIA_VES_AVS
#define MEDIA_VES_AVS (28)
#endif

#ifndef _define___MEDIA_VES_SORENSON
#define _define___MEDIA_VES_SORENSON
#define MEDIA_VES_SORENSON (29)
#endif

#ifndef _define___MEDIA_VES_DIV50
#define _define___MEDIA_VES_DIV50
#define MEDIA_VES_DIV50 (30)
#endif

#ifndef _define___MEDIA_VES_VP6
#define _define___MEDIA_VES_VP6
#define MEDIA_VES_VP6 (31)
#endif

#ifndef _define___MEDIA_VES_RV30
#define _define___MEDIA_VES_RV30
#define MEDIA_VES_RV30 (32)
#endif

#ifndef _define___MEDIA_VES_RV40
#define _define___MEDIA_VES_RV40
#define MEDIA_VES_RV40 (33)
#endif

#ifndef _define___MEDIA_VES_DIV3
#define _define___MEDIA_VES_DIV3
#define MEDIA_VES_DIV3 (34)
#endif

#ifndef _define___MEDIA_VES_DIV4
#define _define___MEDIA_VES_DIV4
#define MEDIA_VES_DIV4 (35)
#endif

#ifndef _define___MEDIA_VES_HEVC
#define _define___MEDIA_VES_HEVC
#define MEDIA_VES_HEVC (36)
#endif

#ifndef _define___MEDIA_VES_VP9
#define _define___MEDIA_VES_VP9
#define MEDIA_VES_VP9 (37)
#endif

#ifndef _define___MEDIA_VES_AV1
#define _define___MEDIA_VES_AV1
#define MEDIA_VES_AV1 (38)
#endif

#ifndef _define___MEDIA_PIC_JPEG
#define _define___MEDIA_PIC_JPEG
#define MEDIA_PIC_JPEG (48)
#endif

#ifndef _define___MEDIA_PIC_PNG
#define _define___MEDIA_PIC_PNG
#define MEDIA_PIC_PNG (49)
#endif

#ifndef _define___MEDIA_PIC_GIF
#define _define___MEDIA_PIC_GIF
#define MEDIA_PIC_GIF (50)
#endif

#ifndef _define___MEDIA_PIC_TIFF
#define _define___MEDIA_PIC_TIFF
#define MEDIA_PIC_TIFF (51)
#endif

#ifndef _define___MEDIA_PIC_BMP
#define _define___MEDIA_PIC_BMP
#define MEDIA_PIC_BMP (52)
#endif

#ifndef _define___MEDIA_PIC_RAW
#define _define___MEDIA_PIC_RAW
#define MEDIA_PIC_RAW (53)
#endif

#ifndef _define___MEDIA_VFB_YUV420
#define _define___MEDIA_VFB_YUV420
#define MEDIA_VFB_YUV420 (64)
#endif

#ifndef _define___MEDIA_VFB_YUV422
#define _define___MEDIA_VFB_YUV422
#define MEDIA_VFB_YUV422 (65)
#endif

#ifndef _define___MEDIA_VFB_YUV444
#define _define___MEDIA_VFB_YUV444
#define MEDIA_VFB_YUV444 (66)
#endif

#ifndef _define___MEDIA_VFB_ARGB8888
#define _define___MEDIA_VFB_ARGB8888
#define MEDIA_VFB_ARGB8888 (67)
#endif

#ifndef _define___MEDIA_AES_AC3
#define _define___MEDIA_AES_AC3
#define MEDIA_AES_AC3 (80)
#endif

#ifndef _define___MEDIA_AES_DDP
#define _define___MEDIA_AES_DDP
#define MEDIA_AES_DDP (81)
#endif

#ifndef _define___MEDIA_AES_TRUEHD
#define _define___MEDIA_AES_TRUEHD
#define MEDIA_AES_TRUEHD (82)
#endif

#ifndef _define___MEDIA_AES_DTSHD
#define _define___MEDIA_AES_DTSHD
#define MEDIA_AES_DTSHD (83)
#endif

#ifndef _define___MEDIA_AES_MPG
#define _define___MEDIA_AES_MPG
#define MEDIA_AES_MPG (84)
#endif

#ifndef _define___MEDIA_AES_MP3
#define _define___MEDIA_AES_MP3
#define MEDIA_AES_MP3 (85)
#endif

#ifndef _define___MEDIA_AES_HEAAC
#define _define___MEDIA_AES_HEAAC
#define MEDIA_AES_HEAAC (86)
#endif

#ifndef _define___MEDIA_AES_WMA
#define _define___MEDIA_AES_WMA
#define MEDIA_AES_WMA (87)
#endif

#ifndef _define___MEDIA_AES_PCM
#define _define___MEDIA_AES_PCM
#define MEDIA_AES_PCM (88)
#endif

#ifndef _define___MEDIA_AES_LPCMSD
#define _define___MEDIA_AES_LPCMSD
#define MEDIA_AES_LPCMSD (89)
#endif

#ifndef _define___MEDIA_AES_LPCMHD
#define _define___MEDIA_AES_LPCMHD
#define MEDIA_AES_LPCMHD (90)
#endif

#ifndef _define___MEDIA_AES_LPCMBD
#define _define___MEDIA_AES_LPCMBD
#define MEDIA_AES_LPCMBD (91)
#endif

#ifndef _define___MEDIA_AES_LPCMSESF
#define _define___MEDIA_AES_LPCMSESF
#define MEDIA_AES_LPCMSESF (92)
#endif

#ifndef _define___MEDIA_AES_PL2
#define _define___MEDIA_AES_PL2
#define MEDIA_AES_PL2 (93)
#endif

#ifndef _define___MEDIA_AES_DTS
#define _define___MEDIA_AES_DTS
#define MEDIA_AES_DTS (95)
#endif

#ifndef _define___MEDIA_AES_DTSMA
#define _define___MEDIA_AES_DTSMA
#define MEDIA_AES_DTSMA (96)
#endif

#ifndef _define___MEDIA_AES_DTSLBR
#define _define___MEDIA_AES_DTSLBR
#define MEDIA_AES_DTSLBR (98)
#endif

#ifndef _define___MEDIA_AES_DVSD
#define _define___MEDIA_AES_DVSD
#define MEDIA_AES_DVSD (99)
#endif

#ifndef _define___MEDIA_AES_VORBIS
#define _define___MEDIA_AES_VORBIS
#define MEDIA_AES_VORBIS (100)
#endif

#ifndef _define___MEDIA_AES_FLAC
#define _define___MEDIA_AES_FLAC
#define MEDIA_AES_FLAC (101)
#endif

#ifndef _define___MEDIA_AES_IMAADPCM
#define _define___MEDIA_AES_IMAADPCM
#define MEDIA_AES_IMAADPCM (104)
#endif

#ifndef _define___MEDIA_AES_G711A
#define _define___MEDIA_AES_G711A
#define MEDIA_AES_G711A (105)
#endif

#ifndef _define___MEDIA_AES_G711U
#define _define___MEDIA_AES_G711U
#define MEDIA_AES_G711U (106)
#endif

#ifndef _define___MEDIA_AES_AMRNB
#define _define___MEDIA_AES_AMRNB
#define MEDIA_AES_AMRNB (107)
#endif

#ifndef _define___MEDIA_AES_AMRWB
#define _define___MEDIA_AES_AMRWB
#define MEDIA_AES_AMRWB (108)
#endif

#ifndef _define___MEDIA_AES_M4ALATM
#define _define___MEDIA_AES_M4ALATM
#define MEDIA_AES_M4ALATM (110)
#endif

#ifndef _define___MEDIA_AES_DOLBY_PULSE
#define _define___MEDIA_AES_DOLBY_PULSE
#define MEDIA_AES_DOLBY_PULSE (114)
#endif

#ifndef _define___MEDIA_AES_IEC61937
#define _define___MEDIA_AES_IEC61937
#define MEDIA_AES_IEC61937 (115)
#endif

#ifndef _define___MEDIA_AES_AC4
#define _define___MEDIA_AES_AC4
#define MEDIA_AES_AC4 (116)
#endif

#ifndef _define___MEDIA_AES_MAT
#define _define___MEDIA_AES_MAT
#define MEDIA_AES_MAT (117)
#endif

#ifndef _define___MEDIA_AES_UNKNOWN
#define _define___MEDIA_AES_UNKNOWN
#define MEDIA_AES_UNKNOWN (118)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV422
#define _define___AMP_DISP_SRCFMT_YUV422
#define AMP_DISP_SRCFMT_YUV422 (0)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARGB32
#define _define___AMP_DISP_SRCFMT_ARGB32
#define AMP_DISP_SRCFMT_ARGB32 (1)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARGB24
#define _define___AMP_DISP_SRCFMT_ARGB24
#define AMP_DISP_SRCFMT_ARGB24 (2)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARGB4444
#define _define___AMP_DISP_SRCFMT_ARGB4444
#define AMP_DISP_SRCFMT_ARGB4444 (3)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RGB565
#define _define___AMP_DISP_SRCFMT_RGB565
#define AMP_DISP_SRCFMT_RGB565 (4)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARGB1555
#define _define___AMP_DISP_SRCFMT_ARGB1555
#define AMP_DISP_SRCFMT_ARGB1555 (5)
#endif

#ifndef _define___AMP_DISP_SRCFMT_LUT8
#define _define___AMP_DISP_SRCFMT_LUT8
#define AMP_DISP_SRCFMT_LUT8 (6)
#endif

#ifndef _define___AMP_DISP_SRCFMT_AYUV32
#define _define___AMP_DISP_SRCFMT_AYUV32
#define AMP_DISP_SRCFMT_AYUV32 (7)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420
#define _define___AMP_DISP_SRCFMT_YUV420
#define AMP_DISP_SRCFMT_YUV420 (8)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP
#define _define___AMP_DISP_SRCFMT_YUV420SP
#define AMP_DISP_SRCFMT_YUV420SP (9)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV444
#define _define___AMP_DISP_SRCFMT_YUV444
#define AMP_DISP_SRCFMT_YUV444 (10)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RGB444
#define _define___AMP_DISP_SRCFMT_RGB444
#define AMP_DISP_SRCFMT_RGB444 (11)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARGB32_4K
#define _define___AMP_DISP_SRCFMT_ARGB32_4K
#define AMP_DISP_SRCFMT_ARGB32_4K (12)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_MODE0
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_MODE0
#define AMP_DISP_SRCFMT_YUV420SP_TILED_MODE0 (13)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_MODE1
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_MODE1
#define AMP_DISP_SRCFMT_YUV420SP_TILED_MODE1 (14)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARGB32_PM
#define _define___AMP_DISP_SRCFMT_ARGB32_PM
#define AMP_DISP_SRCFMT_ARGB32_PM (15)
#endif

#ifndef _define___AMP_DISP_SRCFMT_XRGB32
#define _define___AMP_DISP_SRCFMT_XRGB32
#define AMP_DISP_SRCFMT_XRGB32 (16)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V4H8
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V4H8
#define AMP_DISP_SRCFMT_YUV420SP_TILED_V4H8 (17)
#endif

#ifndef _define___AMP_DISP_SRCFMT_IPT420
#define _define___AMP_DISP_SRCFMT_IPT420
#define AMP_DISP_SRCFMT_IPT420 (18)
#endif

#ifndef _define___AMP_DISP_SRCFMT_IPT422
#define _define___AMP_DISP_SRCFMT_IPT422
#define AMP_DISP_SRCFMT_IPT422 (19)
#endif

#ifndef _define___AMP_DISP_SRCFMT_IPT444
#define _define___AMP_DISP_SRCFMT_IPT444
#define AMP_DISP_SRCFMT_IPT444 (20)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_AUTO_MODE
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_AUTO_MODE
#define AMP_DISP_SRCFMT_YUV420SP_TILED_AUTO_MODE (21)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV444_DWA
#define _define___AMP_DISP_SRCFMT_YUV444_DWA
#define AMP_DISP_SRCFMT_YUV444_DWA (22)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV422SP_DWA
#define _define___AMP_DISP_SRCFMT_YUV422SP_DWA
#define AMP_DISP_SRCFMT_YUV422SP_DWA (23)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_DWA
#define _define___AMP_DISP_SRCFMT_YUV420SP_DWA
#define AMP_DISP_SRCFMT_YUV420SP_DWA (24)
#endif

#ifndef _define___AMP_DISP_SRCFMT_ARBG2101010
#define _define___AMP_DISP_SRCFMT_ARBG2101010
#define AMP_DISP_SRCFMT_ARBG2101010 (25)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV422SP
#define _define___AMP_DISP_SRCFMT_YUV422SP
#define AMP_DISP_SRCFMT_YUV422SP (26)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V8H8
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V8H8
#define AMP_DISP_SRCFMT_YUV420SP_TILED_V8H8 (27)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V8H6
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V8H6
#define AMP_DISP_SRCFMT_YUV420SP_TILED_V8H6 (28)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V6H8
#define _define___AMP_DISP_SRCFMT_YUV420SP_TILED_V6H8
#define AMP_DISP_SRCFMT_YUV420SP_TILED_V6H8 (29)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV422SP_TILED_V8H8
#define _define___AMP_DISP_SRCFMT_YUV422SP_TILED_V8H8
#define AMP_DISP_SRCFMT_YUV422SP_TILED_V8H8 (30)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV422SP_TILED_V8H6
#define _define___AMP_DISP_SRCFMT_YUV422SP_TILED_V8H6
#define AMP_DISP_SRCFMT_YUV422SP_TILED_V8H6 (31)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV422SP_TILED_V6H8
#define _define___AMP_DISP_SRCFMT_YUV422SP_TILED_V6H8
#define AMP_DISP_SRCFMT_YUV422SP_TILED_V6H8 (32)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV444_NHWC
#define _define___AMP_DISP_SRCFMT_YUV444_NHWC
#define AMP_DISP_SRCFMT_YUV444_NHWC (33)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RGB444_NHWC
#define _define___AMP_DISP_SRCFMT_RGB444_NHWC
#define AMP_DISP_SRCFMT_RGB444_NHWC (34)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RGB888_NHWC
#define _define___AMP_DISP_SRCFMT_RGB888_NHWC
#define AMP_DISP_SRCFMT_RGB888_NHWC (35)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RAW
#define _define___AMP_DISP_SRCFMT_RAW
#define AMP_DISP_SRCFMT_RAW (36)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RAW_DWA
#define _define___AMP_DISP_SRCFMT_RAW_DWA
#define AMP_DISP_SRCFMT_RAW_DWA (37)
#endif

#ifndef _define___AMP_DISP_SRCFMT_RAW_QWA
#define _define___AMP_DISP_SRCFMT_RAW_QWA
#define AMP_DISP_SRCFMT_RAW_QWA (38)
#endif

#ifndef _define___AMP_DISP_SRCFMT_YUV420P
#define _define___AMP_DISP_SRCFMT_YUV420P
#define AMP_DISP_SRCFMT_YUV420P (39)
#endif

#ifndef _define___AMP_DISP_SRCFMT_MAX
#define _define___AMP_DISP_SRCFMT_MAX
#define AMP_DISP_SRCFMT_MAX (40)
#endif

#ifndef _typedef___AMP_DISP_SRCFMT
#define _typedef___AMP_DISP_SRCFMT
typedef CORBA_enum AMP_DISP_SRCFMT;
#endif /* _typedef___AMP_DISP_SRCFMT */

#ifndef _typedef___AMP_DISP_ORDER
#define _typedef___AMP_DISP_ORDER
typedef UINT32 AMP_DISP_ORDER;
#endif /* _typedef___AMP_DISP_ORDER */

#ifndef _define___AMP_DISP_ORDER_ARGB
#define _define___AMP_DISP_ORDER_ARGB
#define AMP_DISP_ORDER_ARGB (0)
#endif

#ifndef _define___AMP_DISP_ORDER_ABGR
#define _define___AMP_DISP_ORDER_ABGR
#define AMP_DISP_ORDER_ABGR (1)
#endif

#ifndef _define___AMP_DISP_ORDER_RGBA
#define _define___AMP_DISP_ORDER_RGBA
#define AMP_DISP_ORDER_RGBA (2)
#endif

#ifndef _define___AMP_DISP_ORDER_BGRA
#define _define___AMP_DISP_ORDER_BGRA
#define AMP_DISP_ORDER_BGRA (3)
#endif

#ifndef _define___AMP_DISP_ORDER_ARGB32_MAX
#define _define___AMP_DISP_ORDER_ARGB32_MAX
#define AMP_DISP_ORDER_ARGB32_MAX (4)
#endif

#ifndef _define___AMP_DISP_ORDER_AVYU
#define _define___AMP_DISP_ORDER_AVYU
#define AMP_DISP_ORDER_AVYU (0)
#endif

#ifndef _define___AMP_DISP_ORDER_AUYV
#define _define___AMP_DISP_ORDER_AUYV
#define AMP_DISP_ORDER_AUYV (1)
#endif

#ifndef _define___AMP_DISP_ORDER_VYUA
#define _define___AMP_DISP_ORDER_VYUA
#define AMP_DISP_ORDER_VYUA (2)
#endif

#ifndef _define___AMP_DISP_ORDER_UYVA
#define _define___AMP_DISP_ORDER_UYVA
#define AMP_DISP_ORDER_UYVA (3)
#endif

#ifndef _define___AMP_DISP_ORDER_UYVY
#define _define___AMP_DISP_ORDER_UYVY
#define AMP_DISP_ORDER_UYVY (0)
#endif

#ifndef _define___AMP_DISP_ORDER_VYUY
#define _define___AMP_DISP_ORDER_VYUY
#define AMP_DISP_ORDER_VYUY (1)
#endif

#ifndef _define___AMP_DISP_ORDER_YUYV
#define _define___AMP_DISP_ORDER_YUYV
#define AMP_DISP_ORDER_YUYV (2)
#endif

#ifndef _define___AMP_DISP_ORDER_YVYU
#define _define___AMP_DISP_ORDER_YVYU
#define AMP_DISP_ORDER_YVYU (3)
#endif

#ifndef _define___AMP_DISP_ORDER_RGGB
#define _define___AMP_DISP_ORDER_RGGB
#define AMP_DISP_ORDER_RGGB (0)
#endif

#ifndef _define___AMP_DISP_ORDER_GRBG
#define _define___AMP_DISP_ORDER_GRBG
#define AMP_DISP_ORDER_GRBG (1)
#endif

#ifndef _define___AMP_DISP_ORDER_GBRG
#define _define___AMP_DISP_ORDER_GBRG
#define AMP_DISP_ORDER_GBRG (2)
#endif

#ifndef _define___AMP_DISP_ORDER_BGGR
#define _define___AMP_DISP_ORDER_BGGR
#define AMP_DISP_ORDER_BGGR (3)
#endif

#ifndef _define___AMP_DRMSCHEME_CUSTOMER_BASE
#define _define___AMP_DRMSCHEME_CUSTOMER_BASE
#define AMP_DRMSCHEME_CUSTOMER_BASE (100)
#endif

#ifndef _define___AMP_DRMSCHEME_INVALID
#define _define___AMP_DRMSCHEME_INVALID
#define AMP_DRMSCHEME_INVALID (0)
#endif

#ifndef _define___AMP_DRMSCHEME_HDCP2X
#define _define___AMP_DRMSCHEME_HDCP2X
#define AMP_DRMSCHEME_HDCP2X (1)
#endif

#ifndef _define___AMP_DRMSCHEME_DTCPIP
#define _define___AMP_DRMSCHEME_DTCPIP
#define AMP_DRMSCHEME_DTCPIP (2)
#endif

#ifndef _define___AMP_DRMSCHEME_CIPLUS
#define _define___AMP_DRMSCHEME_CIPLUS
#define AMP_DRMSCHEME_CIPLUS (3)
#endif

#ifndef _define___AMP_DRMSCHEME_WIDEVINE
#define _define___AMP_DRMSCHEME_WIDEVINE
#define AMP_DRMSCHEME_WIDEVINE (4)
#endif

#ifndef _define___AMP_DRMSCHEME_PLAYREADY
#define _define___AMP_DRMSCHEME_PLAYREADY
#define AMP_DRMSCHEME_PLAYREADY (5)
#endif

#ifndef _define___AMP_DRMSCHEME_VUDU
#define _define___AMP_DRMSCHEME_VUDU
#define AMP_DRMSCHEME_VUDU (6)
#endif

#ifndef _define___AMP_DRMSCHEME_VRMTX
#define _define___AMP_DRMSCHEME_VRMTX
#define AMP_DRMSCHEME_VRMTX (7)
#endif

#ifndef _define___AMP_DRMSCHEME_NDS
#define _define___AMP_DRMSCHEME_NDS
#define AMP_DRMSCHEME_NDS (8)
#endif

#ifndef _define___AMP_DRMSCHEME_MLA
#define _define___AMP_DRMSCHEME_MLA
#define AMP_DRMSCHEME_MLA (9)
#endif

#ifndef _define___AMP_DRMSCHEME_MOBITV
#define _define___AMP_DRMSCHEME_MOBITV
#define AMP_DRMSCHEME_MOBITV (10)
#endif

#ifndef _define___AMP_DRMSCHEME_PVR_C0
#define _define___AMP_DRMSCHEME_PVR_C0
#define AMP_DRMSCHEME_PVR_C0 (11)
#endif

#ifndef _define___AMP_DRMSCHEME_DUMMY
#define _define___AMP_DRMSCHEME_DUMMY
#define AMP_DRMSCHEME_DUMMY (12)
#endif

#ifndef _define___AMP_DRMSCHEME_STUB
#define _define___AMP_DRMSCHEME_STUB
#define AMP_DRMSCHEME_STUB (13)
#endif

#ifndef _define___AMP_DRMSCHEME_PVR_GENERAL
#define _define___AMP_DRMSCHEME_PVR_GENERAL
#define AMP_DRMSCHEME_PVR_GENERAL (14)
#endif

#ifndef _define___AMP_DRMSCHEME_MARLIN
#define _define___AMP_DRMSCHEME_MARLIN
#define AMP_DRMSCHEME_MARLIN (15)
#endif

#ifndef _define___AMP_DRMSCHEME_RAW
#define _define___AMP_DRMSCHEME_RAW
#define AMP_DRMSCHEME_RAW (16)
#endif

#ifndef _define___AMP_DRMSCHEME_NAGRA
#define _define___AMP_DRMSCHEME_NAGRA
#define AMP_DRMSCHEME_NAGRA (17)
#endif

#ifndef _define___AMP_DRMSCHEME_CENC3
#define _define___AMP_DRMSCHEME_CENC3
#define AMP_DRMSCHEME_CENC3 (18)
#endif

#ifndef _define___AMP_DRMSCHEME_ALTICAST
#define _define___AMP_DRMSCHEME_ALTICAST
#define AMP_DRMSCHEME_ALTICAST (19)
#endif

#ifndef _define___AMP_DRMSCHEME_IRDETO
#define _define___AMP_DRMSCHEME_IRDETO
#define AMP_DRMSCHEME_IRDETO (20)
#endif

#ifndef _define___AMP_DRMSCHEME_GKL
#define _define___AMP_DRMSCHEME_GKL
#define AMP_DRMSCHEME_GKL (21)
#endif

#ifndef _typedef___AMP_DRMSCHEME_TYPE
#define _typedef___AMP_DRMSCHEME_TYPE
typedef CORBA_enum AMP_DRMSCHEME_TYPE;
#endif /* _typedef___AMP_DRMSCHEME_TYPE */

#ifndef _typedef___AMP_DRM
#define _typedef___AMP_DRM
typedef CORBA_Object AMP_DRM;
#endif /* _typedef___AMP_DRM */

#ifndef _define___AMP_DRM_KEY_ID_MAX_LEN
#define _define___AMP_DRM_KEY_ID_MAX_LEN
#define AMP_DRM_KEY_ID_MAX_LEN (48)
#endif

#ifndef _typedef___AMP_DRM_CRYPTO_ST
#define _typedef___AMP_DRM_CRYPTO_ST
typedef struct AMP_DRM_CRYPTO_ST {
  AMP_DRMSCHEME_TYPE uiSchemeType;
  UINT32 uiSessionID;
  UINT8 uiKeyID[48];
  UINT32 uiKeyIdLen;
} AMP_DRM_CRYPTO_ST;
#endif /* _typedef___AMP_DRM_CRYPTO_ST */

#ifndef _define___AMP_DRM_DRM_RSA_CMD_PUB_ENC
#define _define___AMP_DRM_DRM_RSA_CMD_PUB_ENC
#define AMP_DRM_DRM_RSA_CMD_PUB_ENC (0)
#endif

#ifndef _define___AMP_DRM_DRM_RSA_CMD_PRI_DEC
#define _define___AMP_DRM_DRM_RSA_CMD_PRI_DEC
#define AMP_DRM_DRM_RSA_CMD_PRI_DEC (1)
#endif

#ifndef _define___AMP_DRM_DRM_RSA_CMD_PRI_ENC
#define _define___AMP_DRM_DRM_RSA_CMD_PRI_ENC
#define AMP_DRM_DRM_RSA_CMD_PRI_ENC (2)
#endif

#ifndef _define___AMP_DRM_DRM_RSA_CMD_PUB_DEC
#define _define___AMP_DRM_DRM_RSA_CMD_PUB_DEC
#define AMP_DRM_DRM_RSA_CMD_PUB_DEC (3)
#endif

#ifndef _define___AMP_DRM_DRM_RSA_CMD_MAX
#define _define___AMP_DRM_DRM_RSA_CMD_MAX
#define AMP_DRM_DRM_RSA_CMD_MAX (4)
#endif

#ifndef _typedef___AMP_DRM_DRM_RSA_CMD
#define _typedef___AMP_DRM_DRM_RSA_CMD
typedef CORBA_enum AMP_DRM_DRM_RSA_CMD;
#endif /* _typedef___AMP_DRM_DRM_RSA_CMD */

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_AES128
#define _define___AMP_DRM_OPENCRYPTO_ALG_AES128
#define AMP_DRM_OPENCRYPTO_ALG_AES128 (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_AES192
#define _define___AMP_DRM_OPENCRYPTO_ALG_AES192
#define AMP_DRM_OPENCRYPTO_ALG_AES192 (1)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_AES256
#define _define___AMP_DRM_OPENCRYPTO_ALG_AES256
#define AMP_DRM_OPENCRYPTO_ALG_AES256 (2)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_DES
#define _define___AMP_DRM_OPENCRYPTO_ALG_DES
#define AMP_DRM_OPENCRYPTO_ALG_DES (3)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_TDES
#define _define___AMP_DRM_OPENCRYPTO_ALG_TDES
#define AMP_DRM_OPENCRYPTO_ALG_TDES (4)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_SHA1
#define _define___AMP_DRM_OPENCRYPTO_ALG_SHA1
#define AMP_DRM_OPENCRYPTO_ALG_SHA1 (5)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_SHA256
#define _define___AMP_DRM_OPENCRYPTO_ALG_SHA256
#define AMP_DRM_OPENCRYPTO_ALG_SHA256 (6)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_RSA_PKCS1
#define _define___AMP_DRM_OPENCRYPTO_ALG_RSA_PKCS1
#define AMP_DRM_OPENCRYPTO_ALG_RSA_PKCS1 (7)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_RSA_OAEP
#define _define___AMP_DRM_OPENCRYPTO_ALG_RSA_OAEP
#define AMP_DRM_OPENCRYPTO_ALG_RSA_OAEP (8)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_RC4
#define _define___AMP_DRM_OPENCRYPTO_ALG_RC4
#define AMP_DRM_OPENCRYPTO_ALG_RC4 (9)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_C2
#define _define___AMP_DRM_OPENCRYPTO_ALG_C2
#define AMP_DRM_OPENCRYPTO_ALG_C2 (10)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_ECDSA
#define _define___AMP_DRM_OPENCRYPTO_ALG_ECDSA
#define AMP_DRM_OPENCRYPTO_ALG_ECDSA (11)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_MD5
#define _define___AMP_DRM_OPENCRYPTO_ALG_MD5
#define AMP_DRM_OPENCRYPTO_ALG_MD5 (12)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_HMAC
#define _define___AMP_DRM_OPENCRYPTO_ALG_HMAC
#define AMP_DRM_OPENCRYPTO_ALG_HMAC (13)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_HMAC_SHA256
#define _define___AMP_DRM_OPENCRYPTO_ALG_HMAC_SHA256
#define AMP_DRM_OPENCRYPTO_ALG_HMAC_SHA256 (14)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_SHA384
#define _define___AMP_DRM_OPENCRYPTO_ALG_SHA384
#define AMP_DRM_OPENCRYPTO_ALG_SHA384 (15)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_DH
#define _define___AMP_DRM_OPENCRYPTO_ALG_DH
#define AMP_DRM_OPENCRYPTO_ALG_DH (16)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_ECDH
#define _define___AMP_DRM_OPENCRYPTO_ALG_ECDH
#define AMP_DRM_OPENCRYPTO_ALG_ECDH (17)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ALG_INVALID
#define _define___AMP_DRM_OPENCRYPTO_ALG_INVALID
#define AMP_DRM_OPENCRYPTO_ALG_INVALID (18)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_ALG_ID
#define _typedef___AMP_DRM_OPENCRYPTO_ALG_ID
typedef CORBA_enum AMP_DRM_OPENCRYPTO_ALG_ID;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_ALG_ID */

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_PARM_INVALID
#define _define___AMP_DRM_OPENCRYPTO_KEY_PARM_INVALID
#define AMP_DRM_OPENCRYPTO_KEY_PARM_INVALID (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_PARM
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_PARM
#define AMP_DRM_OPENCRYPTO_KEYDAT_PARM (1)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_EC_GROUP_PARM
#define _define___AMP_DRM_OPENCRYPTO_EC_GROUP_PARM
#define AMP_DRM_OPENCRYPTO_EC_GROUP_PARM (2)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_EC_PUBKEY_PARM
#define _define___AMP_DRM_OPENCRYPTO_EC_PUBKEY_PARM
#define AMP_DRM_OPENCRYPTO_EC_PUBKEY_PARM (3)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_EC_PRVKEY_PARM
#define _define___AMP_DRM_OPENCRYPTO_EC_PRVKEY_PARM
#define AMP_DRM_OPENCRYPTO_EC_PRVKEY_PARM (4)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_EC_AACS_CERT_PARM
#define _define___AMP_DRM_OPENCRYPTO_EC_AACS_CERT_PARM
#define AMP_DRM_OPENCRYPTO_EC_AACS_CERT_PARM (5)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_RSA_PUBKEY_PARM
#define _define___AMP_DRM_OPENCRYPTO_RSA_PUBKEY_PARM
#define AMP_DRM_OPENCRYPTO_RSA_PUBKEY_PARM (6)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_RSA_PRVKEY_PARM
#define _define___AMP_DRM_OPENCRYPTO_RSA_PRVKEY_PARM
#define AMP_DRM_OPENCRYPTO_RSA_PRVKEY_PARM (7)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_KEY_PARM
#define _typedef___AMP_DRM_OPENCRYPTO_KEY_PARM
typedef CORBA_enum AMP_DRM_OPENCRYPTO_KEY_PARM;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_KEY_PARM */

#ifndef _define___AMP_DRM_OPENCRYPTO_HASH_PARM_INVALID
#define _define___AMP_DRM_OPENCRYPTO_HASH_PARM_INVALID
#define AMP_DRM_OPENCRYPTO_HASH_PARM_INVALID (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_HASHKEYDAT_PARM
#define _define___AMP_DRM_OPENCRYPTO_HASHKEYDAT_PARM
#define AMP_DRM_OPENCRYPTO_HASHKEYDAT_PARM (1)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_HASH_PARM
#define _typedef___AMP_DRM_OPENCRYPTO_HASH_PARM
typedef CORBA_enum AMP_DRM_OPENCRYPTO_HASH_PARM;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_HASH_PARM */

#ifndef _define___AMP_DRM_OPENCRYPTO_ENC_ALG_ASE128
#define _define___AMP_DRM_OPENCRYPTO_ENC_ALG_ASE128
#define AMP_DRM_OPENCRYPTO_ENC_ALG_ASE128 (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ENC_ALG_RSA_PKCS1
#define _define___AMP_DRM_OPENCRYPTO_ENC_ALG_RSA_PKCS1
#define AMP_DRM_OPENCRYPTO_ENC_ALG_RSA_PKCS1 (1)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ENC_ALG_RSA_OAEP
#define _define___AMP_DRM_OPENCRYPTO_ENC_ALG_RSA_OAEP
#define AMP_DRM_OPENCRYPTO_ENC_ALG_RSA_OAEP (2)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_ENC_ALG
#define _typedef___AMP_DRM_OPENCRYPTO_ENC_ALG
typedef CORBA_enum AMP_DRM_OPENCRYPTO_ENC_ALG;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_ENC_ALG */

#ifndef _define___AMP_DRM_OPENCRYPTO_ENGINE_INVALID
#define _define___AMP_DRM_OPENCRYPTO_ENGINE_INVALID
#define AMP_DRM_OPENCRYPTO_ENGINE_INVALID (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ENGINE_SW
#define _define___AMP_DRM_OPENCRYPTO_ENGINE_SW
#define AMP_DRM_OPENCRYPTO_ENGINE_SW (1)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_ENGINE_HW
#define _define___AMP_DRM_OPENCRYPTO_ENGINE_HW
#define AMP_DRM_OPENCRYPTO_ENGINE_HW (2)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_ENGINE_TYPE
#define _typedef___AMP_DRM_OPENCRYPTO_ENGINE_TYPE
typedef CORBA_enum AMP_DRM_OPENCRYPTO_ENGINE_TYPE;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_ENGINE_TYPE */

#ifndef _define___AMP_DRM_OPENCRYPTO_PADDING_TYPE_INVALID
#define _define___AMP_DRM_OPENCRYPTO_PADDING_TYPE_INVALID
#define AMP_DRM_OPENCRYPTO_PADDING_TYPE_INVALID (0)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_PADDING_TYPE
#define _typedef___AMP_DRM_OPENCRYPTO_PADDING_TYPE
typedef CORBA_enum AMP_DRM_OPENCRYPTO_PADDING_TYPE;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_PADDING_TYPE */

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_INVALID_TYPE
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_INVALID_TYPE
#define AMP_DRM_OPENCRYPTO_KEYDAT_INVALID_TYPE (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_PLAINTXT
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_PLAINTXT
#define AMP_DRM_OPENCRYPTO_KEYDAT_PLAINTXT (1)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_SECURESTORE
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_SECURESTORE
#define AMP_DRM_OPENCRYPTO_KEYDAT_SECURESTORE (2)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_KEYID
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_KEYID
#define AMP_DRM_OPENCRYPTO_KEYDAT_KEYID (3)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_SW_SCRAMBLE
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_SW_SCRAMBLE
#define AMP_DRM_OPENCRYPTO_KEYDAT_SW_SCRAMBLE (4)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_RKEK_PROT
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_RKEK_PROT
#define AMP_DRM_OPENCRYPTO_KEYDAT_RKEK_PROT (5)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_KEYID_PROTECT
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_KEYID_PROTECT
#define AMP_DRM_OPENCRYPTO_KEYDAT_KEYID_PROTECT (6)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_RANDOMDATA
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_RANDOMDATA
#define AMP_DRM_OPENCRYPTO_KEYDAT_RANDOMDATA (7)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_CIPHERKEY_BYID
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_CIPHERKEY_BYID
#define AMP_DRM_OPENCRYPTO_KEYDAT_CIPHERKEY_BYID (8)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEYDAT_MAX
#define _define___AMP_DRM_OPENCRYPTO_KEYDAT_MAX
#define AMP_DRM_OPENCRYPTO_KEYDAT_MAX (9)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_KEYDAT_TYPE
#define _typedef___AMP_DRM_OPENCRYPTO_KEYDAT_TYPE
typedef CORBA_enum AMP_DRM_OPENCRYPTO_KEYDAT_TYPE;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_KEYDAT_TYPE */

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_INVALID
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_INVALID
#define AMP_DRM_OPENCRYPTO_KEY_MODE_INVALID (0)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_ECB
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_ECB
#define AMP_DRM_OPENCRYPTO_KEY_MODE_ECB (1)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CBC
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CBC
#define AMP_DRM_OPENCRYPTO_KEY_MODE_CBC (2)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CTR
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CTR
#define AMP_DRM_OPENCRYPTO_KEY_MODE_CTR (3)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CTS
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CTS
#define AMP_DRM_OPENCRYPTO_KEY_MODE_CTS (4)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CFB
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_CFB
#define AMP_DRM_OPENCRYPTO_KEY_MODE_CFB (5)
#endif

#ifndef _define___AMP_DRM_OPENCRYPTO_KEY_MODE_MAX
#define _define___AMP_DRM_OPENCRYPTO_KEY_MODE_MAX
#define AMP_DRM_OPENCRYPTO_KEY_MODE_MAX (6)
#endif

#ifndef _typedef___AMP_DRM_OPENCRYPTO_KEY_MODE
#define _typedef___AMP_DRM_OPENCRYPTO_KEY_MODE
typedef CORBA_enum AMP_DRM_OPENCRYPTO_KEY_MODE;
#endif /* _typedef___AMP_DRM_OPENCRYPTO_KEY_MODE */

#ifndef _define___AMP_DRM_CHIPINFO_INVALID
#define _define___AMP_DRM_CHIPINFO_INVALID
#define AMP_DRM_CHIPINFO_INVALID (0)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_NUID
#define _define___AMP_DRM_CHIPINFO_NUID
#define AMP_DRM_CHIPINFO_NUID (1)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_NUID64
#define _define___AMP_DRM_CHIPINFO_NUID64
#define AMP_DRM_CHIPINFO_NUID64 (2)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_STBCASN
#define _define___AMP_DRM_CHIPINFO_STBCASN
#define AMP_DRM_CHIPINFO_STBCASN (3)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_MKTSEGID
#define _define___AMP_DRM_CHIPINFO_MKTSEGID
#define AMP_DRM_CHIPINFO_MKTSEGID (4)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_CHECKNUM
#define _define___AMP_DRM_CHIPINFO_CHECKNUM
#define AMP_DRM_CHIPINFO_CHECKNUM (5)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_CHIPINFO
#define _define___AMP_DRM_CHIPINFO_CHIPINFO
#define AMP_DRM_CHIPINFO_CHIPINFO (6)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_CHIPREV
#define _define___AMP_DRM_CHIPINFO_CHIPREV
#define AMP_DRM_CHIPINFO_CHIPREV (7)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_CHIPEXT
#define _define___AMP_DRM_CHIPINFO_CHIPEXT
#define AMP_DRM_CHIPINFO_CHIPEXT (8)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_DISABLE_KEYLADDER
#define _define___AMP_DRM_CHIPINFO_DISABLE_KEYLADDER
#define AMP_DRM_CHIPINFO_DISABLE_KEYLADDER (9)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_KEYLADDER_STATUS
#define _define___AMP_DRM_CHIPINFO_KEYLADDER_STATUS
#define AMP_DRM_CHIPINFO_KEYLADDER_STATUS (10)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_BOOTMODE
#define _define___AMP_DRM_CHIPINFO_BOOTMODE
#define AMP_DRM_CHIPINFO_BOOTMODE (11)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_CS_ENABLE
#define _define___AMP_DRM_CHIPINFO_CS_ENABLE
#define AMP_DRM_CHIPINFO_CS_ENABLE (12)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_ACTIVATION_FLAG
#define _define___AMP_DRM_CHIPINFO_ACTIVATION_FLAG
#define AMP_DRM_CHIPINFO_ACTIVATION_FLAG (13)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_PERSISTENT_VAL
#define _define___AMP_DRM_CHIPINFO_PERSISTENT_VAL
#define AMP_DRM_CHIPINFO_PERSISTENT_VAL (14)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_RAMUNSCRAMBLEDSTORAGE
#define _define___AMP_DRM_CHIPINFO_RAMUNSCRAMBLEDSTORAGE
#define AMP_DRM_CHIPINFO_RAMUNSCRAMBLEDSTORAGE (15)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_DEBUG_IPL
#define _define___AMP_DRM_CHIPINFO_DEBUG_IPL
#define AMP_DRM_CHIPINFO_DEBUG_IPL (16)
#endif

#ifndef _define___AMP_DRM_CHIPINFO_MAX
#define _define___AMP_DRM_CHIPINFO_MAX
#define AMP_DRM_CHIPINFO_MAX (17)
#endif

#ifndef _typedef___AMP_DRM_CHIPINFO_TYPE
#define _typedef___AMP_DRM_CHIPINFO_TYPE
typedef CORBA_enum AMP_DRM_CHIPINFO_TYPE;
#endif /* _typedef___AMP_DRM_CHIPINFO_TYPE */

HRESULT AMP_DRM_Suspend(AMP_DRM _obj, CORBA_Environment *_ev);

HRESULT AMP_DRM_Resume(AMP_DRM _obj, CORBA_Environment *_ev);

HRESULT AMP_DRM_Destroy(AMP_DRM _obj, CORBA_Environment *_ev);

HRESULT AMP_DRM_TransferTest(AMP_DRM _obj, UINT32 uiCode, UINT32 *pOut, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CreateCtx(AMP_DRM _obj, UINT32 hShareCtxShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_ReleaseCtx(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_SetEntitlementKey(AMP_DRM _obj, UINT32 hTitleKeyShm, UINT32 hShareCtxShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_AES128_DeriveCW(AMP_DRM _obj, UINT32 hCWShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_AES128_DECWithCTS(AMP_DRM _obj, UINT32 hCipherShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetKeyData(AMP_DRM _obj, UINT32 uKeyBoxStoreSize, UINT32 uShmSize, UINT32 hKeyBoxStoreShm, UINT32 hKeyDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_EncryptKeyBox(AMP_DRM _obj, UINT32 uKeyBoxSize, UINT32 uKeyBoxStoreShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetRandom(AMP_DRM _obj, UINT32 hRandomShm, UINT32 uRandomSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Initialize(AMP_DRM _obj, UINT32 hShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_OpenSession(AMP_DRM _obj, UINT32 uKeyboxSize, UINT32 uShmSize, UINT32 hKeyboxShm, UINT32 hSessionShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CloseSession(AMP_DRM _obj, UINT32 uSessionNum, UINT32 hSessionShm, UINT32 uSessionSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Terminate(AMP_DRM _obj, UINT32 uSessionNum, UINT32 hSessionShm, UINT32 uSessionSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GenerateDerivedKeys(AMP_DRM _obj, UINT32 uSession, UINT32 hDeriveShm, UINT32 uDeriveSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GenerateSignature(AMP_DRM _obj, UINT32 uSession, UINT32 hSignattureShm, UINT32 uSignattureSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadKeys(AMP_DRM _obj, UINT32 uSession, UINT32 uNumKeys, UINT32 hDataShm, UINT32 uDataSize, UINT32 uPstSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadLicense(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadRenewal(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadEntitledContentKeys(AMP_DRM _obj, UINT32 uSession, UINT32 uNumKeys, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_RefreshKeys(AMP_DRM _obj, UINT32 uSession, UINT32 uNumKeys, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GenerateNonce(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_RewrapDeviceRSAKey(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadProvisioning(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadDeviceRSAKey(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_PrepAndSignRenewalRequest(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, UINT32 queryLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_PrepAndSignLicenseRequest(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, UINT32 queryLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_PrepAndSignProvisioningRequest(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, UINT32 queryLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GenerateRSASignature(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, UINT32 uPaddingScheme, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_DeriveKeysFromSessionKey(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_SelectKey(AMP_DRM _obj, struct AMP_DRM_CRYPTO_ST *pCryptInfo, UINT32 *keyOffset, UINT32 *cipherMode, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_ExtractPESHeaderInfo(AMP_DRM _obj, UINT32 uSession, UINT16 programID, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetDescCryptoInfo(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CalcHash(AMP_DRM _obj, UINT32 uSession, UINT32 addr, UINT32 length, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_DecCTR(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetHDCPCapability(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetMaxNumberOfSessions(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetNumberOfOpenSessions(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_UpdateUsageTable(AMP_DRM _obj, UINT32 uDummyArg, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_DeactivateUsageEntry(AMP_DRM _obj, UINT32 hDataShm, UINT32 uPstSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_ReportUsage(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uReportType, UINT32 uPstSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_DeleteUsageEntry(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uMsgSize, UINT32 uPstSize, UINT32 uSigSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_ForceDeleteUsageEntry(AMP_DRM _obj, UINT32 hDataShm, UINT32 uPstSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_DeleteUsageTable(AMP_DRM _obj, UINT32 uDummyArg, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_DeleteOldUsageTable(AMP_DRM _obj, UINT32 uDummyArg, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Generic_Encrypt(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Generic_Decrypt(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Generic_Sign(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Generic_Verify(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_QueryKeyControl(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_Security_Patch_Level(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_M2M_TZ_Open(AMP_DRM _obj, UINT32 uType, UINT32 ID_SHM, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_M2M_TZ_SetScheme(AMP_DRM _obj, UINT32 uSessionId, UINT32 uCryptoType, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_M2M_TZ_Close(AMP_DRM _obj, UINT32 uSessionId, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_M2M_TZ_KeyConfig(AMP_DRM _obj, UINT32 SessionHandle, UINT32 keyoption, UINT32 TSP_key_Idx, UINT32 IV_Phy, UINT32 IVlen, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_M2M_TZ_SyncUpdate(AMP_DRM _obj, UINT32 SessionHandle, UINT32 InPhyAddr, UINT32 InOffset, UINT32 InSize, UINT32 OutPhyAddr, UINT32 OutOffset, UINT32 OutSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadUsageEntry(AMP_DRM _obj, UINT32 uSession, UINT32 uIndex, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_MoveEntry(AMP_DRM _obj, UINT32 uSession, UINT32 uNewIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CreateNewUsageEntry(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_ShrinkUsageTableHeader(AMP_DRM _obj, UINT32 uEntryCount, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_UpdateUsageEntry(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CreateUsageTableHeader(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadUsageTableHeader(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_GetCurSRMVersion(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadSRM(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CopyOldUsageEntry(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CreateOldUsageEntry(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_RewrapDeviceRSAKey30(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDateSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_LoadCertRSA(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDateSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_CreateEntitledKeySession(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDateSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_WideVine_ReassociateEntitledKeySession(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDateSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_PRIKEYLADDER_PVR_Key_Prepare(AMP_DRM _obj, UINT32 hShareCtxShm, UINT32 uSeedLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_PRIKEYLADDER_PVR_GenerateKey(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_PRIKEYLADDER_PVR_ReleaseKey(AMP_DRM _obj, UINT32 uSession, UINT32 hDataShm, UINT32 uDataLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_HDCP_KEY_Load_Status(AMP_DRM _obj, UINT32 hShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_MISC_Utils_Calc_SStore_SHA1(AMP_DRM _obj, UINT32 hShmSStore, UINT32 uSStoreSize, UINT32 hShmSHA1, UINT32 uSHA1Size, CORBA_Environment *_ev);

HRESULT AMP_DRM_MISC_Utils_SHA256_Initialize(AMP_DRM _obj, CORBA_Environment *_ev);

HRESULT AMP_DRM_MISC_Utils_SHA256_Finalize(AMP_DRM _obj, CORBA_Environment *_ev);

HRESULT AMP_DRM_OpenCrypto_R2R_KeyLadder(AMP_DRM _obj, UINT32 hDatShm, UINT32 uShmSize, UINT32 uDataInlength, UINT32 uCryptoCmd, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_FIGO_Burner(AMP_DRM _obj, UINT32 hOTPFigoBurnerShmKey, UINT32 uEntryIdx, UINT32 uEntryCnt, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_FIGO_BurnVersion(AMP_DRM _obj, UINT32 hOTPFigoBurnVersionShmKey, UINT32 uVersionNum, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_FIGO_BurnVersion_Ext(AMP_DRM _obj, UINT32 hOTPFigoBurnVersionShmKey, UINT32 uComVersionNum, UINT32 uCustVersionNum, CORBA_Environment *_ev);

HRESULT AMP_DRM_FIRSTBOOT_OTP_RKEK_RandomInit(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_SECURE_STORE_OpenGenerator(AMP_DRM _obj, UINT32 hSecureStoreShmKey, UINT32 uDataDecSize, UINT32 uIsUseDefaultKey, UINT32 uKeyType, CORBA_Environment *_ev);

HRESULT AMP_DRM_SECURE_STORE_Generator(AMP_DRM _obj, UINT32 hSecureStoreShmKey, UINT32 uDataDecSize, UINT32 uIsUseDefaultKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_FIRSTBOOT_CheckPrvk_FIGOIMG(AMP_DRM _obj, UINT32 hCheckPrvkFigoImgShmKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_FIGO_Burner_CipherLayout(AMP_DRM _obj, UINT32 hOTPFigoBurnerShmKey, UINT32 uEntryIdx, UINT32 uEntryCnt, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_BCM_Burner_CipherLayout(AMP_DRM _obj, UINT32 hOTPBCMurnerShmKey, UINT32 uSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OtpBindInfo_Init(AMP_DRM _obj, UINT32 hOTPInfoShmKey, UINT32 uDataSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_FIGO_ReadAntiRollBackVersion(AMP_DRM _obj, UINT32 hOTPFigoReadVersionShmKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OTP_FIGO_ReadAntiRollBackVersion_Ext(AMP_DRM _obj, UINT32 hOTPFigoReadVersionShmKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_HDCP2_CRYPTO_Test(AMP_DRM _obj, UINT32 hDataShm, UINT32 uDataLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_HDCP2X_MSG_Dispatcher(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DUMMY_CreateCtx(AMP_DRM _obj, UINT32 hShareCtxShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DUMMY_ReleaseCtx(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DUMMY_SetKey(AMP_DRM _obj, UINT32 hKeyBoxStoreShm, UINT32 uKeyBoxDataLen, UINT32 uKeyBoxIDLen, UINT32 uKeyBoxType, UINT32 uKeyBoxAlgType, UINT32 uKeyBoxAlgMode, CORBA_Environment *_ev);

HRESULT AMP_DRM_DUMMY_GetKeyIndexByKeyID(AMP_DRM _obj, UINT32 hKeyBoxStoreShm, UINT32 uKeyBoxIDLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_DUMMY_ReleaseKeyTableByKeyID(AMP_DRM _obj, UINT32 hKeyBoxStoreShm, UINT32 uKeyBoxIDLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_STUB_CreateCtx(AMP_DRM _obj, UINT32 hShareCtxShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_STUB_ReleaseCtx(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_STUB_SetKey(AMP_DRM _obj, UINT32 hKeyBoxStoreShm, UINT32 uKeyBoxLen, UINT32 uKeyBoxType, UINT32 uKeyBoxIVWrapType, UINT32 uKeyBoxAlgType, UINT32 uKeyBoxAlgMode, CORBA_Environment *_ev);

HRESULT AMP_DRM_PVR_GenerateKey_ex(AMP_DRM _obj, UINT32 hDataShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_PVR_LoadKey_ex(AMP_DRM _obj, UINT32 hDataShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_PVR_LoadKey_Ext_ex(AMP_DRM _obj, UINT32 hDataShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_PVR_UnloadKey_ex(AMP_DRM _obj, UINT32 hDataShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_RequestDRMScheme(AMP_DRM _obj, UINT32 *puScheme, UINT32 uSessionID, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_ReleaseDRMScheme(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_AllocateContentKeyUnit(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 *puKeyID, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_FreeContentKeyUnit(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_AllocateKeySlot(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID, UINT32 uKeyLen, UINT32 *puKeyIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_AllocateKeySlotPair(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uOddKeyID, UINT32 uEvenKeyID, UINT32 uKeyLen, UINT32 *puOddKeyIndex, UINT32 *puEvenKeyIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_AllocateIVSlot(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID, UINT32 uIVLen, UINT32 *puIVIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_AllocateIVSlotPair(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uOddKeyID, UINT32 uEvenKeyID, UINT32 uIVLen, UINT32 *puOddIVIndex, UINT32 *puEvenIVIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_ConfigAlgorithm(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID, UINT32 uAlgType, UINT32 uAlgMode, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_LoadClearKey(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID, UINT32 puKeyData, UINT32 uKeyDataLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_KEY_LoadClearIV(AMP_DRM _obj, UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID, UINT32 puIVData, UINT32 uIVDataLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_RSA_Gen_Key(AMP_DRM _obj, UINT32 hshm, UINT32 udnlen, UINT32 uelen, UINT32 ubits, CORBA_Environment *_ev);

HRESULT AMP_DRM_RSA_Crypto_CMD(AMP_DRM _obj, AMP_DRM_DRM_RSA_CMD ucmd, UINT32 hshm, UINT32 uflen, UINT32 *putlen, UINT32 udnlen, UINT32 uelen, UINT32 upadding, CORBA_Environment *_ev);

HRESULT AMP_DRM_RSA_Import_Key(AMP_DRM _obj, UINT32 hshm, UINT32 udnlen, CORBA_Environment *_ev);

HRESULT AMP_DRM_RSA_PKCS_PSS_Sign_Verify_Msg_Key(AMP_DRM _obj, UINT32 inputData, UINT32 len, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_GENERATE_XV(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_GENERATE_XV(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_GENERATE_AUTHKEY(AMP_DRM _obj, UINT32 uSinkIdx, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_GENERATE_MAC12(AMP_DRM _obj, UINT32 uSinkIdx, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_GENERATE_MAC12(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_GENERATE_MAC34(AMP_DRM _obj, UINT32 uSinkIdx, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_GENERATE_MAC34(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_GENERATE_AUTHKEY(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_GENERATE_SRCDEV_SIGNATURE(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_GENERATE_SNKDEV_SIGNATURE(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_VERIFY_SNKDEV_SIGNATURE(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_VERIFY_SRCDEV_SIGNATURE(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_VERIFY_DLNA_SIGNATURE(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_VERIFY_DLNA_SIGNATURE(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_CREATECTX(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_RELEASECTX(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_CREATECTX(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_RELEASECTX(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_INIT_CTX(AMP_DRM _obj, UINT32 hDataShm_Key, UINT32 hDataShm_Cert, UINT32 hDataShm_RNG, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_INIT_CTX(AMP_DRM _obj, UINT32 hDataShm_Key, UINT32 hDataShm_Cert, UINT32 hDataShm_RNG, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_PCPENC_INIT(AMP_DRM _obj, UINT32 uSinkIdx, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SOURCE_PCPENCPROC(AMP_DRM _obj, UINT32 hDataShm_Data, UINT32 uDataSize, UINT32 hDataShm_Buf, UINT32 hDataShm_BufSize, UINT32 uSinkIdx, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_PCPENC_INIT(AMP_DRM _obj, UINT32 hDataShm, UINT32 uPCPLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_PCPENCPROC(AMP_DRM _obj, UINT32 hDataShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_DTCPIP_SINK_SESSION_PREPAREKEY(AMP_DRM _obj, UINT32 hCIHandle, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_SERVICE_ALLOCATE(AMP_DRM _obj, UINT32 hInShmBuffer, UINT32 hInShmSrvInfo, UINT32 hInShmCryptoBuffer, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_SERVICE_RELEASE(AMP_DRM _obj, UINT32 uInServiceIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_SERVICE_PROCESS_KLAC(AMP_DRM _obj, UINT32 uInServiceIndex, UINT32 hInShmEvenBuffer, UINT32 uInEvenLength, UINT32 hInShmOddBuffer, UINT32 uInOddLength, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_SERVICE_ADD_KL(AMP_DRM _obj, UINT32 uInServiceIndex, UINT32 hShmEvenKeyBuffer, UINT32 hShmOddKeyBuffer, UINT32 hShmKey0Buffer, UINT32 hShmKey1Buffer, UINT32 uInLength, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_SERVICE_BIND(AMP_DRM _obj, UINT32 uInServiceIndex1, UINT32 uInServiceIndex2, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_SERVICE_UNBIND(AMP_DRM _obj, UINT32 uInServiceIndex1, UINT32 uInServiceIndex2, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_GET_CHIP_ID(AMP_DRM _obj, UINT32 hInChipId, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_GET_CHIP_CONFIG(AMP_DRM _obj, UINT32 hInCccMac, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_CALCULATE_HMAC(AMP_DRM _obj, UINT32 uInhmacData, CORBA_Environment *_ev);

HRESULT AMP_DRM_IRDETO_DECRYPT_OTT_SAMPLES(AMP_DRM _obj, UINT32 hInOttData, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_CreateKey(AMP_DRM _obj, UINT32 hKeyHandleShm, UINT32 uShmSize, UINT32 uAlgID, UINT32 uType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_ReleaseKey(AMP_DRM _obj, UINT32 hKeyHandle, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_SetKeyParm(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hKeyDatShm, UINT32 uKeyDatShmSize, UINT32 uKeyDatType, UINT32 uKeyParm, UINT32 uMode, UINT32 uPadType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_SetIV(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hIVShm, UINT32 uIVShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_SetMode(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 uMode, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_Decrypt(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hCipherDatShm, UINT32 uShmSize, UINT32 uCipherShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_Encrypt(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hPlainDatShm, UINT32 uShmSize, UINT32 uPlainShmSize, UINT32 uCipherShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_ImportKey(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 uEncAlg, UINT32 hEncKeyDatShm, UINT32 uEncKeyDatShmSize, UINT32 uProcType, UINT32 uOutAlgID, UINT32 uKeyParm, UINT32 uOutEngineType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_ExportKeyDat(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hOutShm, UINT32 uOutShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_EncKeyDat(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 uEncAlg, UINT32 hEncKeyDatShm, UINT32 uEncKeyDatShmSize, UINT32 uKeyDatType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_GenKey(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hParmShm, UINT32 uParmShmSize, UINT32 hKeyDatShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_ComputeKey(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hParmShm, UINT32 uParmShmSize, UINT32 hKeyDatShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_Sign(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hDatShm, UINT32 uShmSize, UINT32 uDgstSize, UINT32 uSignatureSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_Verify(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hDatShm, UINT32 uShmSize, UINT32 uDgstSize, UINT32 uSignatureSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_ComputeCRT(AMP_DRM _obj, UINT32 uKeySize, UINT32 hShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_MoveKey(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hDatShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_MoveKey_Ext(AMP_DRM _obj, UINT32 hKeyHandle, UINT32 hDatShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_CreateHash(AMP_DRM _obj, UINT32 hHashHandleShm, UINT32 uShmSize, UINT32 uAlgID, UINT32 uType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_SetHashParm(AMP_DRM _obj, UINT32 hHashHandle, UINT32 hHashParmShm, UINT32 uHashParmShmSize, UINT32 uHashParmType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_ReleaseHash(AMP_DRM _obj, UINT32 hHashHandle, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_MakeDigest(AMP_DRM _obj, UINT32 hHashHandle, UINT32 hDatShm, UINT32 uShmSize, UINT32 uDatSize, UINT32 uDgstSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_HashInit(AMP_DRM _obj, UINT32 hHashHandle, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_HashUpdate(AMP_DRM _obj, UINT32 hHashHandle, UINT32 hDatShm, UINT32 uShmSize, UINT32 uDatSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_HashFinalize(AMP_DRM _obj, UINT32 hHashHandle, UINT32 hDatShm, UINT32 uShmSize, UINT32 uDgstSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_GetSecureClock(AMP_DRM _obj, UINT32 hDatShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OpenCrypto_Gen_Random(AMP_DRM _obj, UINT32 hDatShm, UINT32 uShmSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_Get_ChipInfo(AMP_DRM _obj, UINT32 uInfoType, UINT32 hDatShm, UINT32 uDatShmMax, UINT32 *puDatShmUsed, UINT32 *puReserve0, UINT32 *puReserve1, CORBA_Environment *_ev);

HRESULT AMP_DRM_OPENCRYPTO_Set_ChipInfo(AMP_DRM _obj, UINT32 uInfoType, UINT32 hDatShm, UINT32 uDatShmUsed, UINT32 uReserve0, UINT32 uReserve1, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_Initialize(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_Deinitialize(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_AllocateRegister(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_FreeRegister(AMP_DRM _obj, INT32 keyType, INT32 keyRegisterIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_RegisterCount(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_GetPreloadedIndex(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, UINT32 cbKeyId, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_SignWithPrivateKey(AMP_DRM _obj, UINT32 hOemHalShm, UINT32 cbMessage, INT32 hashType, INT32 signatureScheme, INT32 keyType, INT32 indexIntegrityKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_VerifyMessageSignature(AMP_DRM _obj, UINT32 hOemHalShm, UINT32 cbMessage, UINT32 cbSignature, INT32 hashType, INT32 signatureScheme, INT32 indexIntegrityKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_GenerateKey(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, INT32 indexKey, UINT32 dwRequestedSecurityLevel, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_CreateOMAC1Signature(AMP_DRM _obj, UINT32 hOemHalShm, UINT32 cbData, INT32 keyType, INT32 indexKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_VerifyOMAC1Signature(AMP_DRM _obj, UINT32 hOemHalShm, UINT32 cbData, UINT32 cbSignature, INT32 keyType, INT32 indexKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_WrapKey(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, INT32 indexKey, INT32 wrappingKeyType, INT32 indexWrappingKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_UnwrapKey(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, INT32 indexKey, INT32 wrappingKeyType, INT32 indexWrappingKey, UINT32 cbWrappedKeyData, UINT32 cbParameterData, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_DecryptContent(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, INT32 indexKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_OEM_Hal_GetKeyInfo(AMP_DRM _obj, UINT32 hOemHalShm, INT32 keyType, INT32 indexContentKey, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_Load_Content_Key(AMP_DRM _obj, UINT32 hPrShm, UINT32 uKeySize, UINT32 uKeyIDSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_Remove_Content_Key(AMP_DRM _obj, UINT32 hPrShm, UINT32 uKeySize, UINT32 uKeyIDSize, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_Load_SecKeyStore(AMP_DRM _obj, UINT32 hPrShm, UINT32 uDrmScheme, UINT32 cbSecStore, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_Initialize(AMP_DRM _obj, UINT32 hTeeShm, UINT32 uInLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_Update(AMP_DRM _obj, UINT32 hTeeShm, UINT32 uInLen, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_Reinitialize(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_Uninitialize(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_TurnSecureStop(AMP_DRM _obj, UINT32 isOnOff, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_IsSecureStopOn(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_StartSession(AMP_DRM _obj, UINT32 hTeeShm, UINT32 isLdl, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_CancelSession(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_GetLdlSessionsLimit(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_FlushLdlChallenges(AMP_DRM _obj, UINT32 dummy, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_BindLicense(AMP_DRM _obj, UINT32 hTeeShm, UINT32 uLDLSeconds, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_HasSessionByKID(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_BindSession(AMP_DRM _obj, UINT32 hTeeShm, UINT32 uKeyIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_StopSession(AMP_DRM _obj, UINT32 inKeyIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_DecryptUpdate(AMP_DRM _obj, UINT32 hTeeShm, UINT32 uKeyIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_GetSecureStopIds(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_GetSecureStop(AMP_DRM _obj, UINT32 hTeeShm, UINT32 uKeyIndex, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_CommitSecureStop(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_ResetSecureStops(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

HRESULT AMP_DRM_PR_TEE_GetState(AMP_DRM _obj, UINT32 hTeeShm, CORBA_Environment *_ev);

#ifndef _define___AMP_DEBUG_CMD_DUMP_CLIENTS
#define _define___AMP_DEBUG_CMD_DUMP_CLIENTS
#define AMP_DEBUG_CMD_DUMP_CLIENTS (0)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_SHM_STATE
#define _define___AMP_DEBUG_CMD_DUMP_SHM_STATE
#define AMP_DEBUG_CMD_DUMP_SHM_STATE (1)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_LIS_STATE
#define _define___AMP_DEBUG_CMD_DUMP_LIS_STATE
#define AMP_DEBUG_CMD_DUMP_LIS_STATE (2)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_REGISTER
#define _define___AMP_DEBUG_CMD_DUMP_REGISTER
#define AMP_DEBUG_CMD_DUMP_REGISTER (3)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_SHM
#define _define___AMP_DEBUG_CMD_DUMP_SHM
#define AMP_DEBUG_CMD_DUMP_SHM (4)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_COMP_STATE
#define _define___AMP_DEBUG_CMD_DUMP_COMP_STATE
#define AMP_DEBUG_CMD_DUMP_COMP_STATE (5)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_INST_STATE
#define _define___AMP_DEBUG_CMD_DUMP_INST_STATE
#define AMP_DEBUG_CMD_DUMP_INST_STATE (6)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_I_DATA_BEGIN
#define _define___AMP_DEBUG_CMD_DUMP_I_DATA_BEGIN
#define AMP_DEBUG_CMD_DUMP_I_DATA_BEGIN (7)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_I_DATA_END
#define _define___AMP_DEBUG_CMD_DUMP_I_DATA_END
#define AMP_DEBUG_CMD_DUMP_I_DATA_END (8)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_O_DATA_BEGIN
#define _define___AMP_DEBUG_CMD_DUMP_O_DATA_BEGIN
#define AMP_DEBUG_CMD_DUMP_O_DATA_BEGIN (9)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_O_DATA_END
#define _define___AMP_DEBUG_CMD_DUMP_O_DATA_END
#define AMP_DEBUG_CMD_DUMP_O_DATA_END (10)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_PRIVATE
#define _define___AMP_DEBUG_CMD_DUMP_PRIVATE
#define AMP_DEBUG_CMD_DUMP_PRIVATE (11)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_DISPLAY
#define _define___AMP_DEBUG_CMD_DUMP_DISPLAY
#define AMP_DEBUG_CMD_DUMP_DISPLAY (12)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_SOUND
#define _define___AMP_DEBUG_CMD_DUMP_SOUND
#define AMP_DEBUG_CMD_DUMP_SOUND (13)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_DRM
#define _define___AMP_DEBUG_CMD_DUMP_DRM
#define AMP_DEBUG_CMD_DUMP_DRM (14)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_PM
#define _define___AMP_DEBUG_CMD_DUMP_PM
#define AMP_DEBUG_CMD_DUMP_PM (15)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_MEM_STATE
#define _define___AMP_DEBUG_CMD_DUMP_MEM_STATE
#define AMP_DEBUG_CMD_DUMP_MEM_STATE (16)
#endif

#ifndef _define___AMP_DEBUG_CMD_DUMP_SHM_USAGE
#define _define___AMP_DEBUG_CMD_DUMP_SHM_USAGE
#define AMP_DEBUG_CMD_DUMP_SHM_USAGE (17)
#endif

#ifndef _define___AMP_DEBUG_CMD_ION_CTRL
#define _define___AMP_DEBUG_CMD_ION_CTRL
#define AMP_DEBUG_CMD_ION_CTRL (18)
#endif

#ifndef _typedef___AMP_DEBUG_CMD
#define _typedef___AMP_DEBUG_CMD
typedef CORBA_enum AMP_DEBUG_CMD;
#endif /* _typedef___AMP_DEBUG_CMD */

#ifndef _typedef___AMP_POWER_FLAG
#define _typedef___AMP_POWER_FLAG
typedef UINT32 AMP_POWER_FLAG;
#endif /* _typedef___AMP_POWER_FLAG */

#ifndef _typedef___AMP_SHM_TYPE
#define _typedef___AMP_SHM_TYPE
typedef UINT32 AMP_SHM_TYPE;
#endif /* _typedef___AMP_SHM_TYPE */

#ifndef _typedef___AMP_SHM_HANDLE
#define _typedef___AMP_SHM_HANDLE
typedef UINT32 AMP_SHM_HANDLE;
#endif /* _typedef___AMP_SHM_HANDLE */

#ifndef _define___AMP_POWER_FLAG_INVALID
#define _define___AMP_POWER_FLAG_INVALID
#define AMP_POWER_FLAG_INVALID (0)
#endif

#ifndef _define___AMP_POWER_FLAG_SEL_KERNEL
#define _define___AMP_POWER_FLAG_SEL_KERNEL
#define AMP_POWER_FLAG_SEL_KERNEL (1)
#endif

#ifndef _define___AMP_POWER_FLAG_SEL_SND
#define _define___AMP_POWER_FLAG_SEL_SND
#define AMP_POWER_FLAG_SEL_SND (2)
#endif

#ifndef _define___AMP_POWER_FLAG_SEL_DISP
#define _define___AMP_POWER_FLAG_SEL_DISP
#define AMP_POWER_FLAG_SEL_DISP (4)
#endif

#ifndef _define___AMP_POWER_FLAG_SEL_DRM
#define _define___AMP_POWER_FLAG_SEL_DRM
#define AMP_POWER_FLAG_SEL_DRM (8)
#endif

#ifndef _define___AMP_POWER_FLAG_SEL_MAIN
#define _define___AMP_POWER_FLAG_SEL_MAIN
#define AMP_POWER_FLAG_SEL_MAIN (16)
#endif

#ifndef _define___AMP_POWER_FLAG_SEL_ALL
#define _define___AMP_POWER_FLAG_SEL_ALL
#define AMP_POWER_FLAG_SEL_ALL (255)
#endif

#ifndef _define___AMP_DEBUG_CTL_POWER_DOWN
#define _define___AMP_DEBUG_CTL_POWER_DOWN
#define AMP_DEBUG_CTL_POWER_DOWN (0)
#endif

#ifndef _define___AMP_DEBUG_CTL_COMMAND_MAX
#define _define___AMP_DEBUG_CTL_COMMAND_MAX
#define AMP_DEBUG_CTL_COMMAND_MAX (1)
#endif

#ifndef _typedef___AMP_DEBUG_CTL
#define _typedef___AMP_DEBUG_CTL
typedef CORBA_enum AMP_DEBUG_CTL;
#endif /* _typedef___AMP_DEBUG_CTL */

#ifndef _typedef___AMP_ACTIVE_STANDBY_FLAG
#define _typedef___AMP_ACTIVE_STANDBY_FLAG
typedef UINT32 AMP_ACTIVE_STANDBY_FLAG;
#endif /* _typedef___AMP_ACTIVE_STANDBY_FLAG */

#ifndef _define___AMP_ACTIVE_STANDBY_FLAG_SYS_SUSPEND
#define _define___AMP_ACTIVE_STANDBY_FLAG_SYS_SUSPEND
#define AMP_ACTIVE_STANDBY_FLAG_SYS_SUSPEND (1)
#endif

#ifndef _define___AMP_ACTIVE_STANDBY_FLAG_LOW_POWER
#define _define___AMP_ACTIVE_STANDBY_FLAG_LOW_POWER
#define AMP_ACTIVE_STANDBY_FLAG_LOW_POWER (2)
#endif

#ifndef _define___AMP_ACTIVE_STANDBY_FLAG_RECOVER
#define _define___AMP_ACTIVE_STANDBY_FLAG_RECOVER
#define AMP_ACTIVE_STANDBY_FLAG_RECOVER (3)
#endif

#ifndef _define___AMP_ACTIVE_STANDBY_FLAG_MAX
#define _define___AMP_ACTIVE_STANDBY_FLAG_MAX
#define AMP_ACTIVE_STANDBY_FLAG_MAX (4)
#endif

#ifndef _typedef___AMP_MEDIA_SESSION_ID
#define _typedef___AMP_MEDIA_SESSION_ID
typedef UINT32 AMP_MEDIA_SESSION_ID;
#endif /* _typedef___AMP_MEDIA_SESSION_ID */

#ifndef _typedef___AMP_SHM_INTERNAL_CLIENT
#define _typedef___AMP_SHM_INTERNAL_CLIENT
typedef UINT32 AMP_SHM_INTERNAL_CLIENT;
#endif /* _typedef___AMP_SHM_INTERNAL_CLIENT */

#ifndef _define___AMP_TA_UUID_MAX_NUM
#define _define___AMP_TA_UUID_MAX_NUM
#define AMP_TA_UUID_MAX_NUM (8)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_NONE
#define _define___AMP_SHM_INTERNAL_CLIENT_NONE
#define AMP_SHM_INTERNAL_CLIENT_NONE (0)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DMX
#define _define___AMP_SHM_INTERNAL_CLIENT_DMX
#define AMP_SHM_INTERNAL_CLIENT_DMX (1)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_M2M
#define _define___AMP_SHM_INTERNAL_CLIENT_M2M
#define AMP_SHM_INTERNAL_CLIENT_M2M (2)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_VDEC
#define _define___AMP_SHM_INTERNAL_CLIENT_VDEC
#define AMP_SHM_INTERNAL_CLIENT_VDEC (4)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_VENC
#define _define___AMP_SHM_INTERNAL_CLIENT_VENC
#define AMP_SHM_INTERNAL_CLIENT_VENC (8)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DEINT
#define _define___AMP_SHM_INTERNAL_CLIENT_DEINT
#define AMP_SHM_INTERNAL_CLIENT_DEINT (16)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DVS
#define _define___AMP_SHM_INTERNAL_CLIENT_DVS
#define AMP_SHM_INTERNAL_CLIENT_DVS (32)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_SCALER
#define _define___AMP_SHM_INTERNAL_CLIENT_SCALER
#define AMP_SHM_INTERNAL_CLIENT_SCALER (64)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DISPLAY
#define _define___AMP_SHM_INTERNAL_CLIENT_DISPLAY
#define AMP_SHM_INTERNAL_CLIENT_DISPLAY (128)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_ADEC
#define _define___AMP_SHM_INTERNAL_CLIENT_ADEC
#define AMP_SHM_INTERNAL_CLIENT_ADEC (256)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_AENC
#define _define___AMP_SHM_INTERNAL_CLIENT_AENC
#define AMP_SHM_INTERNAL_CLIENT_AENC (512)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_AREN
#define _define___AMP_SHM_INTERNAL_CLIENT_AREN
#define AMP_SHM_INTERNAL_CLIENT_AREN (1024)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_APP
#define _define___AMP_SHM_INTERNAL_CLIENT_APP
#define AMP_SHM_INTERNAL_CLIENT_APP (2048)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_AVIN
#define _define___AMP_SHM_INTERNAL_CLIENT_AVIN
#define AMP_SHM_INTERNAL_CLIENT_AVIN (4096)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_AOUT
#define _define___AMP_SHM_INTERNAL_CLIENT_AOUT
#define AMP_SHM_INTERNAL_CLIENT_AOUT (8192)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_SOUNDSINK
#define _define___AMP_SHM_INTERNAL_CLIENT_SOUNDSINK
#define AMP_SHM_INTERNAL_CLIENT_SOUNDSINK (16384)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DATA_EXPORTER
#define _define___AMP_SHM_INTERNAL_CLIENT_DATA_EXPORTER
#define AMP_SHM_INTERNAL_CLIENT_DATA_EXPORTER (32768)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_GPU
#define _define___AMP_SHM_INTERNAL_CLIENT_GPU
#define AMP_SHM_INTERNAL_CLIENT_GPU (65536)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_NPU
#define _define___AMP_SHM_INTERNAL_CLIENT_NPU
#define AMP_SHM_INTERNAL_CLIENT_NPU (131072)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_ISP
#define _define___AMP_SHM_INTERNAL_CLIENT_ISP
#define AMP_SHM_INTERNAL_CLIENT_ISP (262144)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DEWARP
#define _define___AMP_SHM_INTERNAL_CLIENT_DEWARP
#define AMP_SHM_INTERNAL_CLIENT_DEWARP (524288)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_TA
#define _define___AMP_SHM_INTERNAL_CLIENT_TA
#define AMP_SHM_INTERNAL_CLIENT_TA (1048576)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_BCM
#define _define___AMP_SHM_INTERNAL_CLIENT_BCM
#define AMP_SHM_INTERNAL_CLIENT_BCM (2097152)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_DRM
#define _define___AMP_SHM_INTERNAL_CLIENT_DRM
#define AMP_SHM_INTERNAL_CLIENT_DRM (4194304)
#endif

#ifndef _define___AMP_SHM_INTERNAL_CLIENT_APPLICATION
#define _define___AMP_SHM_INTERNAL_CLIENT_APPLICATION
#define AMP_SHM_INTERNAL_CLIENT_APPLICATION (8388608)
#endif

#ifndef _typedef___AMP_TA_UUID
#define _typedef___AMP_TA_UUID
typedef struct AMP_TA_UUID {
  UINT32 timeLow;
  UINT16 timeMid;
  UINT16 timeHiAndVersion;
  UINT8 clockSeqAndNode[8];
} AMP_TA_UUID;
#endif /* _typedef___AMP_TA_UUID */

#ifndef _typedef___CORBA_sequence_AMP_TA_UUID
#define _typedef___CORBA_sequence_AMP_TA_UUID
typedef struct CORBA_sequence_AMP_TA_UUID {
  CORBA_unsigned_long _maximum;
  CORBA_unsigned_long _length;
  struct AMP_TA_UUID *_buffer;
  CORBA_boolean _release;
} CORBA_sequence_AMP_TA_UUID;
#endif /* _typedef___CORBA_sequence_AMP_TA_UUID */

#ifndef _typedef___AMP_SHM_CLIENTS
#define _typedef___AMP_SHM_CLIENTS
typedef struct AMP_SHM_CLIENTS {
  AMP_SHM_INTERNAL_CLIENT uiAmpClients;
  struct CORBA_sequence_AMP_TA_UUID trustedApps;
} AMP_SHM_CLIENTS;
#endif /* _typedef___AMP_SHM_CLIENTS */

#ifndef _define___AMP_CONTENT_TYPE_NONE
#define _define___AMP_CONTENT_TYPE_NONE
#define AMP_CONTENT_TYPE_NONE (0)
#endif

#ifndef _define___AMP_CONTENT_TYPE_VIDEO_FRAME
#define _define___AMP_CONTENT_TYPE_VIDEO_FRAME
#define AMP_CONTENT_TYPE_VIDEO_FRAME (1)
#endif

#ifndef _define___AMP_CONTENT_TYPE_AUDIO_FRAME
#define _define___AMP_CONTENT_TYPE_AUDIO_FRAME
#define AMP_CONTENT_TYPE_AUDIO_FRAME (2)
#endif

#ifndef _define___AMP_CONTENT_TYPE_ENCRYPTED_VIDEO_ES
#define _define___AMP_CONTENT_TYPE_ENCRYPTED_VIDEO_ES
#define AMP_CONTENT_TYPE_ENCRYPTED_VIDEO_ES (3)
#endif

#ifndef _define___AMP_CONTENT_TYPE_ENCRYPTED_AUDIO_ES
#define _define___AMP_CONTENT_TYPE_ENCRYPTED_AUDIO_ES
#define AMP_CONTENT_TYPE_ENCRYPTED_AUDIO_ES (4)
#endif

#ifndef _define___AMP_CONTENT_TYPE_CLEAR_VIDEO_ES
#define _define___AMP_CONTENT_TYPE_CLEAR_VIDEO_ES
#define AMP_CONTENT_TYPE_CLEAR_VIDEO_ES (5)
#endif

#ifndef _define___AMP_CONTENT_TYPE_CLEAR_AUDIO_ES
#define _define___AMP_CONTENT_TYPE_CLEAR_AUDIO_ES
#define AMP_CONTENT_TYPE_CLEAR_AUDIO_ES (6)
#endif

#ifndef _define___AMP_CONTENT_TYPE_TRANSPORT_STREAM
#define _define___AMP_CONTENT_TYPE_TRANSPORT_STREAM
#define AMP_CONTENT_TYPE_TRANSPORT_STREAM (7)
#endif

#ifndef _define___AMP_CONTENT_TYPE_DATA
#define _define___AMP_CONTENT_TYPE_DATA
#define AMP_CONTENT_TYPE_DATA (8)
#endif

#ifndef _define___AMP_CONTENT_TYPE_MAX
#define _define___AMP_CONTENT_TYPE_MAX
#define AMP_CONTENT_TYPE_MAX (9)
#endif

#ifndef _typedef___AMP_CONTENT_TYPE
#define _typedef___AMP_CONTENT_TYPE
typedef CORBA_enum AMP_CONTENT_TYPE;
#endif /* _typedef___AMP_CONTENT_TYPE */

#ifndef _typedef___AMP_BD
#define _typedef___AMP_BD
typedef CORBA_Object AMP_BD;
#endif /* _typedef___AMP_BD */

#ifndef _typedef___AMP_BD_ST
#define _typedef___AMP_BD_ST
typedef struct AMP_BD_ST {
  UINT32 uiBDId;
  UINT64 uiAllocVA;
} AMP_BD_ST;
#endif /* _typedef___AMP_BD_ST */

#ifndef _typedef___AMP_M2M_DATA_HANDLE
#define _typedef___AMP_M2M_DATA_HANDLE
typedef CORBA_Object AMP_M2M_DATA_HANDLE;
#endif /* _typedef___AMP_M2M_DATA_HANDLE */

#ifndef _typedef___AMP_M2M_DATA_HANDLE_ST
#define _typedef___AMP_M2M_DATA_HANDLE_ST
typedef struct AMP_M2M_DATA_HANDLE_ST {
  UINT32 hSHM;
  UINT32 uOffset;
  UINT32 uSize;
} AMP_M2M_DATA_HANDLE_ST;
#endif /* _typedef___AMP_M2M_DATA_HANDLE_ST */

#ifndef _typedef___AMP_LIB
#define _typedef___AMP_LIB
typedef CORBA_Object AMP_LIB;
#endif /* _typedef___AMP_LIB */

#ifndef _typedef___AMP_LIB_M2M_DATA_ST
#define _typedef___AMP_LIB_M2M_DATA_ST
typedef struct AMP_LIB_M2M_DATA_ST {
  UINT32 hSHM;
  UINT32 uOffset;
  UINT32 uSize;
} AMP_LIB_M2M_DATA_ST;
#endif /* _typedef___AMP_LIB_M2M_DATA_ST */

#ifndef _typedef___AMP_LIB_M2M_UIN8_32_ARRAY_ST
#define _typedef___AMP_LIB_M2M_UIN8_32_ARRAY_ST
typedef struct AMP_LIB_M2M_UIN8_32_ARRAY_ST {
  UINT8 Data[32];
} AMP_LIB_M2M_UIN8_32_ARRAY_ST;
#endif /* _typedef___AMP_LIB_M2M_UIN8_32_ARRAY_ST */

#ifndef _typedef___AMP_LIB_M2M_PATTERN_MODE_CFG_ST
#define _typedef___AMP_LIB_M2M_PATTERN_MODE_CFG_ST
typedef struct AMP_LIB_M2M_PATTERN_MODE_CFG_ST {
  UINT8 pattern_enc;
  UINT8 pattern_clr;
} AMP_LIB_M2M_PATTERN_MODE_CFG_ST;
#endif /* _typedef___AMP_LIB_M2M_PATTERN_MODE_CFG_ST */

#ifndef _define___AMP_LIB_RESIDUE_TYPE_DEFAULT
#define _define___AMP_LIB_RESIDUE_TYPE_DEFAULT
#define AMP_LIB_RESIDUE_TYPE_DEFAULT (0)
#endif

#ifndef _define___AMP_LIB_RESIDUE_TYPE_PATTERN
#define _define___AMP_LIB_RESIDUE_TYPE_PATTERN
#define AMP_LIB_RESIDUE_TYPE_PATTERN (1)
#endif

#ifndef _define___AMP_LIB_RESIDUE_TYPE_EXOR
#define _define___AMP_LIB_RESIDUE_TYPE_EXOR
#define AMP_LIB_RESIDUE_TYPE_EXOR (2)
#endif

#ifndef _define___AMP_LIB_RESIDUE_TYPE_RP
#define _define___AMP_LIB_RESIDUE_TYPE_RP
#define AMP_LIB_RESIDUE_TYPE_RP (3)
#endif

#ifndef _define___AMP_LIB_RESIDUE_TYPE_CTS
#define _define___AMP_LIB_RESIDUE_TYPE_CTS
#define AMP_LIB_RESIDUE_TYPE_CTS (4)
#endif

#ifndef _define___AMP_LIB_RESIDUE_TYPE_MAX
#define _define___AMP_LIB_RESIDUE_TYPE_MAX
#define AMP_LIB_RESIDUE_TYPE_MAX (5)
#endif

#ifndef _typedef___AMP_LIB_M2M_RESIDUE_TYPE
#define _typedef___AMP_LIB_M2M_RESIDUE_TYPE
typedef CORBA_enum AMP_LIB_M2M_RESIDUE_TYPE;
#endif /* _typedef___AMP_LIB_M2M_RESIDUE_TYPE */

#ifndef _typedef___AMP_LIB_DV_FRAME_DUMP_CONFIG
#define _typedef___AMP_LIB_DV_FRAME_DUMP_CONFIG
typedef struct AMP_LIB_DV_FRAME_DUMP_CONFIG {
  UINT32 uiPhyAddr;
  UINT32 uiFrameSize;
  UINT32 uiNFrames;
  UINT32 uiSkipFrames;
  UINT32 uiOverwriteFlag;
  UINT32 uiForceCapture;
} AMP_LIB_DV_FRAME_DUMP_CONFIG;
#endif /* _typedef___AMP_LIB_DV_FRAME_DUMP_CONFIG */

#ifndef _define___AMP_LIB_MAX_DV_FRAME_DUMP_FRAMES
#define _define___AMP_LIB_MAX_DV_FRAME_DUMP_FRAMES
#define AMP_LIB_MAX_DV_FRAME_DUMP_FRAMES (1000)
#endif

#ifndef _define___AMP_LIB_MAX_DV_FRAME_DUMP_FRAMES_WORDS
#define _define___AMP_LIB_MAX_DV_FRAME_DUMP_FRAMES_WORDS
#define AMP_LIB_MAX_DV_FRAME_DUMP_FRAMES_WORDS (32)
#endif

#ifndef _typedef___AMP_LIB_DV_FRAME_DUMP_STATUS
#define _typedef___AMP_LIB_DV_FRAME_DUMP_STATUS
typedef struct AMP_LIB_DV_FRAME_DUMP_STATUS {
  UINT32 uiCurrentNdx;
  UINT32 uiNextNdx;
  UINT32 uiUsedFrames[32];
  UINT32 ErrorStatus;
} AMP_LIB_DV_FRAME_DUMP_STATUS;
#endif /* _typedef___AMP_LIB_DV_FRAME_DUMP_STATUS */

#ifndef _define___AMP_LIB_SHM_SCATTER_GENERIC
#define _define___AMP_LIB_SHM_SCATTER_GENERIC
#define AMP_LIB_SHM_SCATTER_GENERIC (0)
#endif

#ifndef _define___AMP_LIB_SHM_SCATTER_UV_ALIGN
#define _define___AMP_LIB_SHM_SCATTER_UV_ALIGN
#define AMP_LIB_SHM_SCATTER_UV_ALIGN (1)
#endif

#ifndef _typedef___AMP_LIB_SHM_BUF_TYPE
#define _typedef___AMP_LIB_SHM_BUF_TYPE
typedef CORBA_enum AMP_LIB_SHM_BUF_TYPE;
#endif /* _typedef___AMP_LIB_SHM_BUF_TYPE */

#ifndef _typedef___AMP_LIB_SHM_CONFIG
#define _typedef___AMP_LIB_SHM_CONFIG
typedef struct AMP_LIB_SHM_CONFIG {
  AMP_LIB_SHM_BUF_TYPE eType;
  UINT32 uiSizeY;
} AMP_LIB_SHM_CONFIG;
#endif /* _typedef___AMP_LIB_SHM_CONFIG */

HRESULT AMP_LIB_Destroy(AMP_LIB _obj, CORBA_Environment *_ev);

HRESULT AMP_LIB_Suspend(AMP_LIB _obj, UINT32 uiFlags, CORBA_Environment *_ev);

HRESULT AMP_LIB_ListenerCreate(AMP_LIB _obj, UINT32 uiSrvIdN, UINT32 uiSrvIdC, UINT32 *hListener, CORBA_Environment *_ev);

HRESULT AMP_LIB_ListenerDestroy(AMP_LIB _obj, UINT32 hListener, CORBA_Environment *_ev);

HRESULT AMP_LIB_ListenerAddEvent(AMP_LIB _obj, UINT32 hListener, UINT32 uiEventCode, UINT32 uiParam1, UINT32 uiParam2, CORBA_Environment *_ev);

HRESULT AMP_LIB_ListenerRemoveEvent(AMP_LIB _obj, UINT32 hListener, UINT32 uiEventCode, CORBA_Environment *_ev);

HRESULT AMP_LIB_LogControl(AMP_LIB _obj, UINT32 uiCmd, UINT32 uiParam1, UINT32 uiParam2, UINT32 *puiOutParam, CORBA_char **sName, CORBA_Environment *_ev);

HRESULT AMP_LIB_DebugDump(AMP_LIB _obj, AMP_DEBUG_CMD eCmd, UINT32 uiParam1, UINT32 uiParam2, UINT32 uiParam3, CORBA_char *sPath, CORBA_Environment *_ev);

HRESULT AMP_LIB_DebugControl(AMP_LIB _obj, UINT32 uiCmd, UINT32 uiParam1, UINT32 uiParam2, UINT32 *puiOutParam, CORBA_char **sData, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mOpen(AMP_LIB _obj, UINT32 uType, UINT32 *pSessionID, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mClose(AMP_LIB _obj, UINT32 uSessionID, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mRegisterNotify(AMP_LIB _obj, UINT32 uSessionID, UINT32 uServiceId, UINT32 uEventType, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mUnregisterNotify(AMP_LIB _obj, UINT32 uSessionID, UINT32 uServiceId, UINT32 uEventType, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mSetScheme(AMP_LIB _obj, UINT32 uSessionID, UINT32 eCryptoType, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mSetKey(AMP_LIB _obj, UINT32 uSessionID, UINT32 KeyOption, UINT32 KeyTblIndex, struct AMP_LIB_M2M_UIN8_32_ARRAY_ST *KeyData, UINT32 KeyLength, UINT32 IVOption, UINT32 IVTblIndex, struct AMP_LIB_M2M_UIN8_32_ARRAY_ST *IVData, UINT32 IVLength, struct AMP_DRM_CRYPTO_ST *pCryptInfo, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mSyncUpdate(AMP_LIB _obj, UINT32 uSessionID, struct AMP_LIB_M2M_DATA_ST *InDat, struct AMP_LIB_M2M_DATA_ST *OutDat, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mAsyncUpdate(AMP_LIB _obj, UINT32 uSessionID, struct AMP_LIB_M2M_DATA_ST *InDat, struct AMP_LIB_M2M_DATA_ST *OutDat, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mSetPatternMode(AMP_LIB _obj, UINT32 uSessionID, struct AMP_LIB_M2M_PATTERN_MODE_CFG_ST *patternModeCfg, CORBA_Environment *_ev);

HRESULT AMP_LIB_M2mSetResidueMode(AMP_LIB _obj, UINT32 uSessionID, AMP_LIB_M2M_RESIDUE_TYPE residueType, CORBA_Environment *_ev);

HRESULT AMP_LIB_ActiveStandby(AMP_LIB _obj, UINT32 uiFlags, CORBA_Environment *_ev);

HRESULT AMP_LIB_OUT_DV_InitFrameDump(AMP_LIB _obj, UINT32 Path, UINT32 Enable, struct AMP_LIB_DV_FRAME_DUMP_CONFIG *pFrameDumpConfig, CORBA_Environment *_ev);

HRESULT AMP_LIB_OUT_DV_GetFrameDumpStatus(AMP_LIB _obj, struct AMP_LIB_DV_FRAME_DUMP_STATUS *pFrameDumpStatus, CORBA_Environment *_ev);

HRESULT AMP_LIB_OUT_DV_EnableDolbyDump(AMP_LIB _obj, UINT32 Enable, CORBA_Environment *_ev);

HRESULT AMP_LIB_SHM_Allocate(AMP_LIB _obj, AMP_SHM_TYPE eMemType, UINT32 uiSize, UINT32 uiAlign, AMP_SHM_HANDLE *phShm, CORBA_Environment *_ev);

HRESULT AMP_LIB_SHM_V1_0_Allocator(AMP_LIB _obj, AMP_MEDIA_SESSION_ID uiSessionId, AMP_CONTENT_TYPE eContentType, AMP_SHM_TYPE eMemType, struct AMP_SHM_CLIENTS *stWriters, struct AMP_SHM_CLIENTS *stReaders, struct AMP_LIB_SHM_CONFIG *stShmConfig, UINT32 uiSize, AMP_SHM_HANDLE *phShm, CORBA_Environment *_ev);

HRESULT AMP_LIB_SHM_Release(AMP_LIB _obj, AMP_SHM_HANDLE hShm, CORBA_Environment *_ev);

HRESULT AMP_LIB_SHMServerMapMemory(AMP_LIB _obj, AMP_SHM_HANDLE hShm, CORBA_Environment *_ev);

HRESULT AMP_LIB_SHMServerUnmapMemory(AMP_LIB _obj, AMP_SHM_HANDLE hShm, CORBA_Environment *_ev);

#ifndef _typedef___AMP_COMPONENT
#define _typedef___AMP_COMPONENT
typedef CORBA_Object AMP_COMPONENT;
#endif /* _typedef___AMP_COMPONENT */

#ifndef _define___AMP_TUNNEL
#define _define___AMP_TUNNEL
#define AMP_TUNNEL (0)
#endif

#ifndef _define___AMP_NON_TUNNEL
#define _define___AMP_NON_TUNNEL
#define AMP_NON_TUNNEL (1)
#endif

#ifndef _define___AMP_SECURE_TUNNEL
#define _define___AMP_SECURE_TUNNEL
#define AMP_SECURE_TUNNEL (2)
#endif

#ifndef _typedef___AMP_COMPONENT_MODE
#define _typedef___AMP_COMPONENT_MODE
typedef CORBA_enum AMP_COMPONENT_MODE;
#endif /* _typedef___AMP_COMPONENT_MODE */

#ifndef _define___AMP_USE_NONE
#define _define___AMP_USE_NONE
#define AMP_USE_NONE (0)
#endif

#ifndef _define___AMP_USE_ARM
#define _define___AMP_USE_ARM
#define AMP_USE_ARM (1)
#endif

#ifndef _define___AMP_USE_ZSP
#define _define___AMP_USE_ZSP
#define AMP_USE_ZSP (2)
#endif

#ifndef _typedef___AMP_USE_MODULE_ENUM
#define _typedef___AMP_USE_MODULE_ENUM
typedef CORBA_enum AMP_USE_MODULE_ENUM;
#endif /* _typedef___AMP_USE_MODULE_ENUM */

#ifndef _define___AMP_PT_TYPE_DOLBY
#define _define___AMP_PT_TYPE_DOLBY
#define AMP_PT_TYPE_DOLBY (0)
#endif

#ifndef _define___AMP_PT_TYPE_DTS
#define _define___AMP_PT_TYPE_DTS
#define AMP_PT_TYPE_DTS (1)
#endif

#ifndef _define___AMP_PT_TYPE_MAX
#define _define___AMP_PT_TYPE_MAX
#define AMP_PT_TYPE_MAX (2)
#endif

#ifndef _typedef___AMP_PT_TYPE_ENUM
#define _typedef___AMP_PT_TYPE_ENUM
typedef CORBA_enum AMP_PT_TYPE_ENUM;
#endif /* _typedef___AMP_PT_TYPE_ENUM */

#ifndef _define___AMP_PT_DOLBY_NONE
#define _define___AMP_PT_DOLBY_NONE
#define AMP_PT_DOLBY_NONE (0)
#endif

#ifndef _define___AMP_PT_DOLBY_DD
#define _define___AMP_PT_DOLBY_DD
#define AMP_PT_DOLBY_DD (1)
#endif

#ifndef _define___AMP_PT_DOLBY_DDP
#define _define___AMP_PT_DOLBY_DDP
#define AMP_PT_DOLBY_DDP (2)
#endif

#ifndef _define___AMP_PT_DOLBY_DCV
#define _define___AMP_PT_DOLBY_DCV
#define AMP_PT_DOLBY_DCV (3)
#endif

#ifndef _define___AMP_PT_DOLBY_MAX
#define _define___AMP_PT_DOLBY_MAX
#define AMP_PT_DOLBY_MAX (4)
#endif

#ifndef _typedef___AMP_PT_DOLBY_ENUM
#define _typedef___AMP_PT_DOLBY_ENUM
typedef CORBA_enum AMP_PT_DOLBY_ENUM;
#endif /* _typedef___AMP_PT_DOLBY_ENUM */

#ifndef _define___AMP_PT_DTS_NONE
#define _define___AMP_PT_DTS_NONE
#define AMP_PT_DTS_NONE (0)
#endif

#ifndef _define___AMP_PT_DTS_DTS
#define _define___AMP_PT_DTS_DTS
#define AMP_PT_DTS_DTS (1)
#endif

#ifndef _define___AMP_PT_DTS_DTSHD
#define _define___AMP_PT_DTS_DTSHD
#define AMP_PT_DTS_DTSHD (2)
#endif

#ifndef _define___AMP_PT_DTS_DTSMA
#define _define___AMP_PT_DTS_DTSMA
#define AMP_PT_DTS_DTSMA (3)
#endif

#ifndef _define___AMP_PT_DTS_MAX
#define _define___AMP_PT_DTS_MAX
#define AMP_PT_DTS_MAX (4)
#endif

#ifndef _typedef___AMP_PT_DTS_ENUM
#define _typedef___AMP_PT_DTS_ENUM
typedef CORBA_enum AMP_PT_DTS_ENUM;
#endif /* _typedef___AMP_PT_DTS_ENUM */

#ifndef _define___AMP_DUM_FILE_NAME_MAX
#define _define___AMP_DUM_FILE_NAME_MAX
#define AMP_DUM_FILE_NAME_MAX (64)
#endif

#ifndef _typedef___AMP_DUM_CONFIG
#define _typedef___AMP_DUM_CONFIG
typedef struct AMP_DUM_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiInputPortNum;
  UINT32 uiOutputPortNum;
  UINT32 uiPlaybackFragmentSize;
  UINT32 uiPlaybackIntervalTime;
  UINT32 uiPlaybackBdNumber;
  CORBA_char sPlaybackFile[64];
  CORBA_char sRecordingFile[64];
} AMP_DUM_CONFIG;
#endif /* _typedef___AMP_DUM_CONFIG */

#ifndef _define___AMP_CLK_PORT_IRRELEVANT
#define _define___AMP_CLK_PORT_IRRELEVANT
#define AMP_CLK_PORT_IRRELEVANT (-1)
#endif

#ifndef _define___AMP_CLK_PORT_VIDEO_IRRELEVANT
#define _define___AMP_CLK_PORT_VIDEO_IRRELEVANT
#define AMP_CLK_PORT_VIDEO_IRRELEVANT (-2)
#endif

#ifndef _define___AMP_CLK_PORT_AUDIO_IRRELEVANT
#define _define___AMP_CLK_PORT_AUDIO_IRRELEVANT
#define AMP_CLK_PORT_AUDIO_IRRELEVANT (-3)
#endif

#ifndef _define___AMP_CLK_SRC_AUTO
#define _define___AMP_CLK_SRC_AUTO
#define AMP_CLK_SRC_AUTO (0)
#endif

#ifndef _define___AMP_CLK_SRC_AVIF
#define _define___AMP_CLK_SRC_AVIF
#define AMP_CLK_SRC_AVIF (1)
#endif

#ifndef _define___AMP_CLK_SRC_HW
#define _define___AMP_CLK_SRC_HW
#define AMP_CLK_SRC_HW (2)
#endif

#ifndef _define___AMP_CLK_SRC_SW
#define _define___AMP_CLK_SRC_SW
#define AMP_CLK_SRC_SW (3)
#endif

#ifndef _define___AMP_CLK_SRC_SYSTEM
#define _define___AMP_CLK_SRC_SYSTEM
#define AMP_CLK_SRC_SYSTEM (4)
#endif

#ifndef _define___AMP_CLK_SRC_VPP
#define _define___AMP_CLK_SRC_VPP
#define AMP_CLK_SRC_VPP (5)
#endif

#ifndef _define___AMP_CLK_SRC_VIP
#define _define___AMP_CLK_SRC_VIP
#define AMP_CLK_SRC_VIP (6)
#endif

#ifndef _define___AMP_CLK_SRC_DMX
#define _define___AMP_CLK_SRC_DMX
#define AMP_CLK_SRC_DMX (7)
#endif

#ifndef _define___AMP_CLK_SRC_DEFAULT
#define _define___AMP_CLK_SRC_DEFAULT
#define AMP_CLK_SRC_DEFAULT (8)
#endif

#ifndef _define___AMP_CLK_SRC_MAX
#define _define___AMP_CLK_SRC_MAX
#define AMP_CLK_SRC_MAX (9)
#endif

#ifndef _typedef___AMP_CLK_SRC
#define _typedef___AMP_CLK_SRC
typedef CORBA_enum AMP_CLK_SRC;
#endif /* _typedef___AMP_CLK_SRC */

#ifndef _define___AMP_CLK_OPT_SYNC_POLICY
#define _define___AMP_CLK_OPT_SYNC_POLICY
#define AMP_CLK_OPT_SYNC_POLICY (0)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_OVERWRITE
#define _define___AMP_CLK_OPT_SYNC_OVERWRITE
#define AMP_CLK_OPT_SYNC_OVERWRITE (1)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_DISABLE_PCR
#define _define___AMP_CLK_OPT_SYNC_DISABLE_PCR
#define AMP_CLK_OPT_SYNC_DISABLE_PCR (2)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_SHOW_FIRST
#define _define___AMP_CLK_OPT_SYNC_SHOW_FIRST
#define AMP_CLK_OPT_SYNC_SHOW_FIRST (3)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_DYNAMIC_SYNC
#define _define___AMP_CLK_OPT_SYNC_DYNAMIC_SYNC
#define AMP_CLK_OPT_SYNC_DYNAMIC_SYNC (4)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_DYNAMIC_SYNC_RATE
#define _define___AMP_CLK_OPT_SYNC_DYNAMIC_SYNC_RATE
#define AMP_CLK_OPT_SYNC_DYNAMIC_SYNC_RATE (5)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_DISP_LIMIT
#define _define___AMP_CLK_OPT_SYNC_DISP_LIMIT
#define AMP_CLK_OPT_SYNC_DISP_LIMIT (6)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_AUTO_PULL_DOWN
#define _define___AMP_CLK_OPT_SYNC_AUTO_PULL_DOWN
#define AMP_CLK_OPT_SYNC_AUTO_PULL_DOWN (7)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_FREE_RUN
#define _define___AMP_CLK_OPT_SYNC_FREE_RUN
#define AMP_CLK_OPT_SYNC_FREE_RUN (8)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_SET_MASTER
#define _define___AMP_CLK_OPT_SYNC_SET_MASTER
#define AMP_CLK_OPT_SYNC_SET_MASTER (9)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_SIM_OUTPUT
#define _define___AMP_CLK_OPT_SYNC_SIM_OUTPUT
#define AMP_CLK_OPT_SYNC_SIM_OUTPUT (10)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_STREAM_DELAY
#define _define___AMP_CLK_OPT_SYNC_STREAM_DELAY
#define AMP_CLK_OPT_SYNC_STREAM_DELAY (11)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_BUFFER_THRESH
#define _define___AMP_CLK_OPT_SYNC_BUFFER_THRESH
#define AMP_CLK_OPT_SYNC_BUFFER_THRESH (12)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_FAST_CHANNEL_CHANGE
#define _define___AMP_CLK_OPT_SYNC_FAST_CHANNEL_CHANGE
#define AMP_CLK_OPT_SYNC_FAST_CHANNEL_CHANGE (13)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_RESET_START_STC
#define _define___AMP_CLK_OPT_SYNC_RESET_START_STC
#define AMP_CLK_OPT_SYNC_RESET_START_STC (14)
#endif

#ifndef _define___AMP_CLK_OPT_SYNC_DIRECTV_DSS
#define _define___AMP_CLK_OPT_SYNC_DIRECTV_DSS
#define AMP_CLK_OPT_SYNC_DIRECTV_DSS (15)
#endif

#ifndef _define___AMP_CLK_OPT_MAX
#define _define___AMP_CLK_OPT_MAX
#define AMP_CLK_OPT_MAX (16)
#endif

#ifndef _typedef___AMP_CLK_OPT
#define _typedef___AMP_CLK_OPT
typedef CORBA_enum AMP_CLK_OPT;
#endif /* _typedef___AMP_CLK_OPT */

#ifndef _define___AMP_CLK_POLICY_LOCAL_FILE
#define _define___AMP_CLK_POLICY_LOCAL_FILE
#define AMP_CLK_POLICY_LOCAL_FILE (0)
#endif

#ifndef _define___AMP_CLK_POLICY_OMX
#define _define___AMP_CLK_POLICY_OMX
#define AMP_CLK_POLICY_OMX (1)
#endif

#ifndef _define___AMP_CLK_POLICY_LOCAL_NET
#define _define___AMP_CLK_POLICY_LOCAL_NET
#define AMP_CLK_POLICY_LOCAL_NET (2)
#endif

#ifndef _define___AMP_CLK_POLICY_LIVE_DTV
#define _define___AMP_CLK_POLICY_LIVE_DTV
#define AMP_CLK_POLICY_LIVE_DTV (3)
#endif

#ifndef _define___AMP_CLK_POLICY_LIVE_NET
#define _define___AMP_CLK_POLICY_LIVE_NET
#define AMP_CLK_POLICY_LIVE_NET (4)
#endif

#ifndef _define___AMP_CLK_POLICY_LIVE_AVIN
#define _define___AMP_CLK_POLICY_LIVE_AVIN
#define AMP_CLK_POLICY_LIVE_AVIN (5)
#endif

#ifndef _define___AMP_CLK_POLICY_ANDROID_SIDEBAND
#define _define___AMP_CLK_POLICY_ANDROID_SIDEBAND
#define AMP_CLK_POLICY_ANDROID_SIDEBAND (6)
#endif

#ifndef _define___AMP_CLK_POLICY_MAX
#define _define___AMP_CLK_POLICY_MAX
#define AMP_CLK_POLICY_MAX (7)
#endif

#ifndef _typedef___AMP_CLK_POLICY
#define _typedef___AMP_CLK_POLICY
typedef CORBA_enum AMP_CLK_POLICY;
#endif /* _typedef___AMP_CLK_POLICY */

#ifndef _define___AMP_RND_TYPE_VIDEO
#define _define___AMP_RND_TYPE_VIDEO
#define AMP_RND_TYPE_VIDEO (0)
#endif

#ifndef _define___AMP_RND_TYPE_AUDIO
#define _define___AMP_RND_TYPE_AUDIO
#define AMP_RND_TYPE_AUDIO (1)
#endif

#ifndef _define___AMP_RND_TYPE_MAX
#define _define___AMP_RND_TYPE_MAX
#define AMP_RND_TYPE_MAX (2)
#endif

#ifndef _typedef___AMP_RND_TYPE
#define _typedef___AMP_RND_TYPE
typedef CORBA_enum AMP_RND_TYPE;
#endif /* _typedef___AMP_RND_TYPE */

#ifndef _typedef___AMP_RND_STAT
#define _typedef___AMP_RND_STAT
typedef struct AMP_RND_STAT {
  UINT32 uiRndType;
  UINT32 uiRndPtsLow;
  UINT32 uiRndPtsHigh;
  UINT32 uiNumVidInputFrames;
  UINT32 uiNumVidReadyFrames;
  UINT32 uiNumVidDisplayedFrames;
  UINT32 uiNumVidDroppedFrames;
  UINT32 uiNumVidDelayedFrames;
  UINT32 uiNumVidRepeatedFrames;
  UINT32 uiNumVidPtsErrors;
  UINT32 uiNumResync;
} AMP_RND_STAT;
#endif /* _typedef___AMP_RND_STAT */

#ifndef _typedef___AMP_CLK_CONFIG
#define _typedef___AMP_CLK_CONFIG
typedef struct AMP_CLK_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiInputPortNum;
  UINT32 uiOutputPortNum;
  UINT32 uiNotifierNum;
  AMP_CLK_SRC eClockSource;
  AMP_CLK_POLICY eAVSyncPolicy;
} AMP_CLK_CONFIG;
#endif /* _typedef___AMP_CLK_CONFIG */

#ifndef _define___AMP_DMX_MOD_SW
#define _define___AMP_DMX_MOD_SW
#define AMP_DMX_MOD_SW (0)
#endif

#ifndef _define___AMP_DMX_MOD_HW
#define _define___AMP_DMX_MOD_HW
#define AMP_DMX_MOD_HW (1)
#endif

#ifndef _typedef___AMP_DMX_MOD
#define _typedef___AMP_DMX_MOD
typedef CORBA_enum AMP_DMX_MOD;
#endif /* _typedef___AMP_DMX_MOD */

#ifndef _typedef___AMP_DMX_CONFIG
#define _typedef___AMP_DMX_CONFIG
typedef struct AMP_DMX_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 CryptoStub;
  UINT32 uiInputPortNum;
  UINT32 uiOutputPortNum;
  AMP_DMX_MOD eDMXMode;
} AMP_DMX_CONFIG;
#endif /* _typedef___AMP_DMX_CONFIG */

#ifndef _typedef___AMP_VDEC_CONFIG
#define _typedef___AMP_VDEC_CONFIG
typedef struct AMP_VDEC_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiType;
  UINT32 uiWidth;
  UINT32 uiHeight;
  UINT16 uiFrameRate;
  UINT32 uiFlag;
  UINT32 uiSecureType;
  UINT32 uiSHMHandle;
  UINT32 uiSHMSize;
  UINT32 uiPadSize;
  UINT32 uiSessionID;
  UINT8 uiKeyID[16];
  UINT32 uiKeyIdLen;
  UINT32 uiOverAllocationMode;
  UINT32 uiOverAllocationWidth;
  UINT32 uiOverAllocationHeight;
  UINT32 uiVPxStrmMode;
  UINT32 uiHscl;
  UINT32 uiVscl;
  UINT32 uiDpbSize;
} AMP_VDEC_CONFIG;
#endif /* _typedef___AMP_VDEC_CONFIG */

#ifndef _typedef___AMP_VCAP_CONFIG
#define _typedef___AMP_VCAP_CONFIG
typedef struct AMP_VCAP_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT16 uiFrameRate;
} AMP_VCAP_CONFIG;
#endif /* _typedef___AMP_VCAP_CONFIG */

#ifndef _define___AMP_SR_MODEL_FAST
#define _define___AMP_SR_MODEL_FAST
#define AMP_SR_MODEL_FAST (0)
#endif

#ifndef _define___AMP_SR_MODEL_QDEO
#define _define___AMP_SR_MODEL_QDEO
#define AMP_SR_MODEL_QDEO (1)
#endif

#ifndef _define___AMP_SR_MODEL_FULL
#define _define___AMP_SR_MODEL_FULL
#define AMP_SR_MODEL_FULL (2)
#endif

#ifndef _define___AMP_SR_MODEL_NUM
#define _define___AMP_SR_MODEL_NUM
#define AMP_SR_MODEL_NUM (3)
#endif

#ifndef _typedef___AMP_SR_MODEL
#define _typedef___AMP_SR_MODEL
typedef CORBA_enum AMP_SR_MODEL;
#endif /* _typedef___AMP_SR_MODEL */

#ifndef _typedef___AMP_SR_CONFIG
#define _typedef___AMP_SR_CONFIG
typedef struct AMP_SR_CONFIG {
  AMP_COMPONENT_MODE mode;
  AMP_SR_MODEL SRModel;
} AMP_SR_CONFIG;
#endif /* _typedef___AMP_SR_CONFIG */

#ifndef _typedef___AMP_VA_CONFIG
#define _typedef___AMP_VA_CONFIG
typedef struct AMP_VA_CONFIG {
  UINT32 modelData;
  UINT32 modelMetadata;
} AMP_VA_CONFIG;
#endif /* _typedef___AMP_VA_CONFIG */

#ifndef _define___AMP_VENC_YUV420_TILE
#define _define___AMP_VENC_YUV420_TILE
#define AMP_VENC_YUV420_TILE (0)
#endif

#ifndef _define___AMP_VENC_YUV422_UYVY
#define _define___AMP_VENC_YUV422_UYVY
#define AMP_VENC_YUV422_UYVY (1)
#endif

#ifndef _define___AMP_VENC_YUV422_YUYV
#define _define___AMP_VENC_YUV422_YUYV
#define AMP_VENC_YUV422_YUYV (2)
#endif

#ifndef _define___AMP_VENC_YUV420_PLANE
#define _define___AMP_VENC_YUV420_PLANE
#define AMP_VENC_YUV420_PLANE (3)
#endif

#ifndef _define___AMP_VENC_YUV422_PLANE
#define _define___AMP_VENC_YUV422_PLANE
#define AMP_VENC_YUV422_PLANE (4)
#endif

#ifndef _define___AMP_VENC_YUV420_SP
#define _define___AMP_VENC_YUV420_SP
#define AMP_VENC_YUV420_SP (5)
#endif

#ifndef _define___AMP_VENC_YUV420_SPA
#define _define___AMP_VENC_YUV420_SPA
#define AMP_VENC_YUV420_SPA (6)
#endif

#ifndef _define___AMP_VENC_YUV420_YV12
#define _define___AMP_VENC_YUV420_YV12
#define AMP_VENC_YUV420_YV12 (7)
#endif

#ifndef _define___AMP_VENC_RGBA
#define _define___AMP_VENC_RGBA
#define AMP_VENC_RGBA (8)
#endif

#ifndef _define___AMP_VENC_INPUT_UNSUPPORTED
#define _define___AMP_VENC_INPUT_UNSUPPORTED
#define AMP_VENC_INPUT_UNSUPPORTED (9)
#endif

#ifndef _typedef___AMP_VENC_COLOR_FORMAT
#define _typedef___AMP_VENC_COLOR_FORMAT
typedef CORBA_enum AMP_VENC_COLOR_FORMAT;
#endif /* _typedef___AMP_VENC_COLOR_FORMAT */

#ifndef _define___AMP_RCGOP_I
#define _define___AMP_RCGOP_I
#define AMP_RCGOP_I (0)
#endif

#ifndef _define___AMP_RCGOP_IP
#define _define___AMP_RCGOP_IP
#define AMP_RCGOP_IP (1)
#endif

#ifndef _define___AMP_RCGOP_IBP
#define _define___AMP_RCGOP_IBP
#define AMP_RCGOP_IBP (2)
#endif

#ifndef _define___AMP_RCGOP_IBBP
#define _define___AMP_RCGOP_IBBP
#define AMP_RCGOP_IBBP (3)
#endif

#ifndef _define___AMP_RCGOP_IBBBP
#define _define___AMP_RCGOP_IBBBP
#define AMP_RCGOP_IBBBP (4)
#endif

#ifndef _typedef___AMP_VENC_GOP_TYPE
#define _typedef___AMP_VENC_GOP_TYPE
typedef CORBA_enum AMP_VENC_GOP_TYPE;
#endif /* _typedef___AMP_VENC_GOP_TYPE */

#ifndef _define___AMP_VENC_MODE_EXPRESS
#define _define___AMP_VENC_MODE_EXPRESS
#define AMP_VENC_MODE_EXPRESS (0)
#endif

#ifndef _define___AMP_VENC_MODE_TURBO
#define _define___AMP_VENC_MODE_TURBO
#define AMP_VENC_MODE_TURBO (1)
#endif

#ifndef _define___AMP_VENC_MODE_EXPERT
#define _define___AMP_VENC_MODE_EXPERT
#define AMP_VENC_MODE_EXPERT (2)
#endif

#ifndef _define___AMP_VENC_MODE_UNSUPPORTED
#define _define___AMP_VENC_MODE_UNSUPPORTED
#define AMP_VENC_MODE_UNSUPPORTED (3)
#endif

#ifndef _typedef___AMP_VENC_MODE
#define _typedef___AMP_VENC_MODE
typedef CORBA_enum AMP_VENC_MODE;
#endif /* _typedef___AMP_VENC_MODE */

#ifndef _define___AMP_VENC_ASPECT_RATIO_FORBIDDEN
#define _define___AMP_VENC_ASPECT_RATIO_FORBIDDEN
#define AMP_VENC_ASPECT_RATIO_FORBIDDEN (0)
#endif

#ifndef _define___AMP_VENC_ASPECT_RATIO_1_1
#define _define___AMP_VENC_ASPECT_RATIO_1_1
#define AMP_VENC_ASPECT_RATIO_1_1 (1)
#endif

#ifndef _define___AMP_VENC_ASPECT_RATIO_12_11
#define _define___AMP_VENC_ASPECT_RATIO_12_11
#define AMP_VENC_ASPECT_RATIO_12_11 (2)
#endif

#ifndef _define___AMP_VENC_ASPECT_RATIO_10_11
#define _define___AMP_VENC_ASPECT_RATIO_10_11
#define AMP_VENC_ASPECT_RATIO_10_11 (3)
#endif

#ifndef _define___AMP_VENC_ASPECT_RATIO_16_11
#define _define___AMP_VENC_ASPECT_RATIO_16_11
#define AMP_VENC_ASPECT_RATIO_16_11 (4)
#endif

#ifndef _define___AMP_VENC_ASPECT_RATIO_40_33
#define _define___AMP_VENC_ASPECT_RATIO_40_33
#define AMP_VENC_ASPECT_RATIO_40_33 (5)
#endif

#ifndef _define___AMP_VENC_ASPECT_RATIO_UNSUPPORTED
#define _define___AMP_VENC_ASPECT_RATIO_UNSUPPORTED
#define AMP_VENC_ASPECT_RATIO_UNSUPPORTED (6)
#endif

#ifndef _typedef___AMP_VENC_ASPECT_RATIO
#define _typedef___AMP_VENC_ASPECT_RATIO
typedef CORBA_enum AMP_VENC_ASPECT_RATIO;
#endif /* _typedef___AMP_VENC_ASPECT_RATIO */

#ifndef _define___AMP_ISP_INPUT_MEMORY
#define _define___AMP_ISP_INPUT_MEMORY
#define AMP_ISP_INPUT_MEMORY (0)
#endif

#ifndef _define___AMP_ISP_INPUT_SENSOR1
#define _define___AMP_ISP_INPUT_SENSOR1
#define AMP_ISP_INPUT_SENSOR1 (1)
#endif

#ifndef _define___AMP_ISP_INPUT_SENSOR2
#define _define___AMP_ISP_INPUT_SENSOR2
#define AMP_ISP_INPUT_SENSOR2 (2)
#endif

#ifndef _define___AMP_ISP_INPUT_SENSOR_ALL
#define _define___AMP_ISP_INPUT_SENSOR_ALL
#define AMP_ISP_INPUT_SENSOR_ALL (3)
#endif

#ifndef _define___AMP_ISP_INPUT_TPG
#define _define___AMP_ISP_INPUT_TPG
#define AMP_ISP_INPUT_TPG (4)
#endif

#ifndef _typedef___AMP_ISP_INPUT_SOURCE
#define _typedef___AMP_ISP_INPUT_SOURCE
typedef CORBA_enum AMP_ISP_INPUT_SOURCE;
#endif /* _typedef___AMP_ISP_INPUT_SOURCE */

#ifndef _define___AMP_VENC_PROGRESSIVE
#define _define___AMP_VENC_PROGRESSIVE
#define AMP_VENC_PROGRESSIVE (0)
#endif

#ifndef _define___AMP_VENC_INTERLACED_FRAME
#define _define___AMP_VENC_INTERLACED_FRAME
#define AMP_VENC_INTERLACED_FRAME (1)
#endif

#ifndef _define___AMP_VENC_FILED_FRAME
#define _define___AMP_VENC_FILED_FRAME
#define AMP_VENC_FILED_FRAME (2)
#endif

#ifndef _define___AMP_VENC_FIELD_INTERLACED_UNSUPPORTED
#define _define___AMP_VENC_FIELD_INTERLACED_UNSUPPORTED
#define AMP_VENC_FIELD_INTERLACED_UNSUPPORTED (3)
#endif

#ifndef _typedef___AMP_VENC_FIELD_INTERLACED
#define _typedef___AMP_VENC_FIELD_INTERLACED
typedef CORBA_enum AMP_VENC_FIELD_INTERLACED;
#endif /* _typedef___AMP_VENC_FIELD_INTERLACED */

#ifndef _define___AMP_VENC_CAVLC
#define _define___AMP_VENC_CAVLC
#define AMP_VENC_CAVLC (0)
#endif

#ifndef _define___AMP_VENC_CABAC
#define _define___AMP_VENC_CABAC
#define AMP_VENC_CABAC (1)
#endif

#ifndef _typedef___AMP_VENC_ENTROPY_MODE
#define _typedef___AMP_VENC_ENTROPY_MODE
typedef CORBA_enum AMP_VENC_ENTROPY_MODE;
#endif /* _typedef___AMP_VENC_ENTROPY_MODE */

#ifndef _define___AMP_VENC_TRANSFORM_4X4
#define _define___AMP_VENC_TRANSFORM_4X4
#define AMP_VENC_TRANSFORM_4X4 (0)
#endif

#ifndef _define___AMP_VENC_TRANSFORM_ADAPTIVE
#define _define___AMP_VENC_TRANSFORM_ADAPTIVE
#define AMP_VENC_TRANSFORM_ADAPTIVE (1)
#endif

#ifndef _define___AMP_VENC_TRANSFORM_8X8
#define _define___AMP_VENC_TRANSFORM_8X8
#define AMP_VENC_TRANSFORM_8X8 (2)
#endif

#ifndef _typedef___AMP_VENC_TRANSFORM_FLAG
#define _typedef___AMP_VENC_TRANSFORM_FLAG
typedef CORBA_enum AMP_VENC_TRANSFORM_FLAG;
#endif /* _typedef___AMP_VENC_TRANSFORM_FLAG */

#ifndef _define___AMP_VENC_QPEL_OFF
#define _define___AMP_VENC_QPEL_OFF
#define AMP_VENC_QPEL_OFF (0)
#endif

#ifndef _define___AMP_VENC_QPEL_ADAPTIVE
#define _define___AMP_VENC_QPEL_ADAPTIVE
#define AMP_VENC_QPEL_ADAPTIVE (1)
#endif

#ifndef _define___AMP_VENC_QPEL_ON
#define _define___AMP_VENC_QPEL_ON
#define AMP_VENC_QPEL_ON (2)
#endif

#ifndef _typedef___AMP_VENC_QPEL_MODE
#define _typedef___AMP_VENC_QPEL_MODE
typedef CORBA_enum AMP_VENC_QPEL_MODE;
#endif /* _typedef___AMP_VENC_QPEL_MODE */

#ifndef _typedef___AMP_VENC_H264_PROFILE
#define _typedef___AMP_VENC_H264_PROFILE
typedef UINT32 AMP_VENC_H264_PROFILE;
#endif /* _typedef___AMP_VENC_H264_PROFILE */

#ifndef _define___AMP_VENC_PROFILE_BASE
#define _define___AMP_VENC_PROFILE_BASE
#define AMP_VENC_PROFILE_BASE (66)
#endif

#ifndef _define___AMP_VENC_PROFILE_MAIN
#define _define___AMP_VENC_PROFILE_MAIN
#define AMP_VENC_PROFILE_MAIN (77)
#endif

#ifndef _define___AMP_VENC_PROFILE_EXTENDED
#define _define___AMP_VENC_PROFILE_EXTENDED
#define AMP_VENC_PROFILE_EXTENDED (88)
#endif

#ifndef _define___AMP_VENC_PROFILE_HIGH
#define _define___AMP_VENC_PROFILE_HIGH
#define AMP_VENC_PROFILE_HIGH (100)
#endif

#ifndef _typedef___AMP_VENC_H264_LEVEL
#define _typedef___AMP_VENC_H264_LEVEL
typedef UINT32 AMP_VENC_H264_LEVEL;
#endif /* _typedef___AMP_VENC_H264_LEVEL */

#ifndef _define___AMP_VENC_LEVEL_1_0
#define _define___AMP_VENC_LEVEL_1_0
#define AMP_VENC_LEVEL_1_0 (10)
#endif

#ifndef _define___AMP_VENC_LEVEL_1_1
#define _define___AMP_VENC_LEVEL_1_1
#define AMP_VENC_LEVEL_1_1 (11)
#endif

#ifndef _define___AMP_VENC_LEVEL_1_2
#define _define___AMP_VENC_LEVEL_1_2
#define AMP_VENC_LEVEL_1_2 (12)
#endif

#ifndef _define___AMP_VENC_LEVEL_1_3
#define _define___AMP_VENC_LEVEL_1_3
#define AMP_VENC_LEVEL_1_3 (13)
#endif

#ifndef _define___AMP_VENC_LEVEL_2_0
#define _define___AMP_VENC_LEVEL_2_0
#define AMP_VENC_LEVEL_2_0 (20)
#endif

#ifndef _define___AMP_VENC_LEVEL_2_1
#define _define___AMP_VENC_LEVEL_2_1
#define AMP_VENC_LEVEL_2_1 (21)
#endif

#ifndef _define___AMP_VENC_LEVEL_2_2
#define _define___AMP_VENC_LEVEL_2_2
#define AMP_VENC_LEVEL_2_2 (22)
#endif

#ifndef _define___AMP_VENC_LEVEL_3_0
#define _define___AMP_VENC_LEVEL_3_0
#define AMP_VENC_LEVEL_3_0 (30)
#endif

#ifndef _define___AMP_VENC_LEVEL_3_1
#define _define___AMP_VENC_LEVEL_3_1
#define AMP_VENC_LEVEL_3_1 (31)
#endif

#ifndef _define___AMP_VENC_LEVEL_3_2
#define _define___AMP_VENC_LEVEL_3_2
#define AMP_VENC_LEVEL_3_2 (32)
#endif

#ifndef _define___AMP_VENC_LEVEL_4_0
#define _define___AMP_VENC_LEVEL_4_0
#define AMP_VENC_LEVEL_4_0 (40)
#endif

#ifndef _define___AMP_VENC_LEVEL_4_1
#define _define___AMP_VENC_LEVEL_4_1
#define AMP_VENC_LEVEL_4_1 (41)
#endif

#ifndef _define___AMP_VENC_LEVEL_4_2
#define _define___AMP_VENC_LEVEL_4_2
#define AMP_VENC_LEVEL_4_2 (42)
#endif

#ifndef _define___AMP_VENC_LEVEL_5_0
#define _define___AMP_VENC_LEVEL_5_0
#define AMP_VENC_LEVEL_5_0 (50)
#endif

#ifndef _define___AMP_VENC_LEVEL_5_1
#define _define___AMP_VENC_LEVEL_5_1
#define AMP_VENC_LEVEL_5_1 (51)
#endif

#ifndef _typedef___AMP_VENC_CONFIG
#define _typedef___AMP_VENC_CONFIG
typedef struct AMP_VENC_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiWidth;
  UINT32 uiHeight;
  AMP_VENC_COLOR_FORMAT uiColorFmt;
  UINT32 uiCodingFmt;
  UINT32 uiInitQp;
  AMP_VENC_MODE uiEncMode;
  UINT32 uiGopSize;
  AMP_VENC_GOP_TYPE uiGopType;
  AMP_VENC_H264_PROFILE uiH264Profile;
  AMP_VENC_H264_LEVEL uiH264Level;
  UINT32 uiFrameRateNum;
  UINT32 uiFrameRateDen;
  INT32 uiRateCtrlSel;
  UINT32 uiVBVSize;
  UINT32 uiHRDBitRate;
  UINT32 uiMinRate;
  UINT32 uiMaxRate;
  UINT32 uiDownScaleFlag;
  UINT32 uiOutputFrameWidthLimit;
  UINT32 uiOutputFrameHeightLimit;
  UINT32 uiAvcOn;
  UINT32 uiAudOn;
  UINT32 uiSarWidth;
  UINT32 uiSarHeight;
  AMP_VENC_FIELD_INTERLACED uiFieldInterlaced;
  AMP_VENC_ASPECT_RATIO uiPAR;
  AMP_VENC_ENTROPY_MODE uiEntropyMode;
  AMP_VENC_TRANSFORM_FLAG uiTransformFlag;
  AMP_VENC_QPEL_MODE uiQuarterMv;
  UINT32 uiDisableDeblocking;
  UINT32 uiSVCLevel;
} AMP_VENC_CONFIG;
#endif /* _typedef___AMP_VENC_CONFIG */

#ifndef _typedef___AMP_IMGDEC_CONFIG
#define _typedef___AMP_IMGDEC_CONFIG
typedef struct AMP_IMGDEC_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiType;
} AMP_IMGDEC_CONFIG;
#endif /* _typedef___AMP_IMGDEC_CONFIG */

#ifndef _typedef___AMP_VOUT_PLANE_ID
#define _typedef___AMP_VOUT_PLANE_ID
typedef INT32 AMP_VOUT_PLANE_ID;
#endif /* _typedef___AMP_VOUT_PLANE_ID */

#ifndef _define___AMP_VOUT_MAIN_PLANE
#define _define___AMP_VOUT_MAIN_PLANE
#define AMP_VOUT_MAIN_PLANE (0)
#endif

#ifndef _define___AMP_VOUT_PIP_PLANE
#define _define___AMP_VOUT_PIP_PLANE
#define AMP_VOUT_PIP_PLANE (1)
#endif

#ifndef _define___AMP_VOUT_UHD_PLANE
#define _define___AMP_VOUT_UHD_PLANE
#define AMP_VOUT_UHD_PLANE (2)
#endif

#ifndef _define___AMP_VOUT_UHD_AUTO
#define _define___AMP_VOUT_UHD_AUTO
#define AMP_VOUT_UHD_AUTO (3)
#endif

#ifndef _typedef___AMP_VOUT_CONFIG
#define _typedef___AMP_VOUT_CONFIG
typedef struct AMP_VOUT_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiInputPortNum;
  UINT32 uiOutputPortNum;
  AMP_VOUT_PLANE_ID uiPlaneID;
  UINT32 uiNotifierNum;
  UINT32 uiPlaneMuteBypass;
  UINT32 uiPlaneMute;
  UINT32 uiPlaneMode;
} AMP_VOUT_CONFIG;
#endif /* _typedef___AMP_VOUT_CONFIG */

#ifndef _typedef___AMP_DEINT_CONFIG
#define _typedef___AMP_DEINT_CONFIG
typedef struct AMP_DEINT_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiOutBufAllocAlign;
} AMP_DEINT_CONFIG;
#endif /* _typedef___AMP_DEINT_CONFIG */

#ifndef _typedef___AMP_OVPP_CONFIG
#define _typedef___AMP_OVPP_CONFIG
typedef struct AMP_DEINT_CONFIG AMP_OVPP_CONFIG;
#endif /* _typedef___AMP_OVPP_CONFIG */

#ifndef _typedef___AMP_SCALER_CONFIG
#define _typedef___AMP_SCALER_CONFIG
typedef struct AMP_SCALER_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiOutWidth;
  UINT32 uiOutHeight;
  UINT32 uiOutFmt;
  UINT32 uiOutDepth;
} AMP_SCALER_CONFIG;
#endif /* _typedef___AMP_SCALER_CONFIG */

#ifndef _typedef___AMP_ADEC_FORMAT
#define _typedef___AMP_ADEC_FORMAT
typedef UINT32 AMP_ADEC_FORMAT;
#endif /* _typedef___AMP_ADEC_FORMAT */

#ifndef _define___AMP_DD_AC3
#define _define___AMP_DD_AC3
#define AMP_DD_AC3 (80)
#endif

#ifndef _define___AMP_DD_PLUS
#define _define___AMP_DD_PLUS
#define AMP_DD_PLUS (81)
#endif

#ifndef _define___AMP_DD_TRUE_HD
#define _define___AMP_DD_TRUE_HD
#define AMP_DD_TRUE_HD (82)
#endif

#ifndef _define___AMP_DTS_HD
#define _define___AMP_DTS_HD
#define AMP_DTS_HD (83)
#endif

#ifndef _define___AMP_MPG_AUDIO
#define _define___AMP_MPG_AUDIO
#define AMP_MPG_AUDIO (84)
#endif

#ifndef _define___AMP_MP3
#define _define___AMP_MP3
#define AMP_MP3 (85)
#endif

#ifndef _define___AMP_HE_AAC
#define _define___AMP_HE_AAC
#define AMP_HE_AAC (86)
#endif

#ifndef _define___AMP_WMA
#define _define___AMP_WMA
#define AMP_WMA (87)
#endif

#ifndef _define___AMP_RAW_PCM
#define _define___AMP_RAW_PCM
#define AMP_RAW_PCM (88)
#endif

#ifndef _define___AMP_LPCM_SD
#define _define___AMP_LPCM_SD
#define AMP_LPCM_SD (89)
#endif

#ifndef _define___AMP_LPCM_HD
#define _define___AMP_LPCM_HD
#define AMP_LPCM_HD (90)
#endif

#ifndef _define___AMP_LPCM_BD
#define _define___AMP_LPCM_BD
#define AMP_LPCM_BD (91)
#endif

#ifndef _define___AMP_LPCM_SESF
#define _define___AMP_LPCM_SESF
#define AMP_LPCM_SESF (92)
#endif

#ifndef _define___AMP_PL2
#define _define___AMP_PL2
#define AMP_PL2 (93)
#endif

#ifndef _define___AMP_DD_DCV
#define _define___AMP_DD_DCV
#define AMP_DD_DCV (94)
#endif

#ifndef _define___AMP_DTS
#define _define___AMP_DTS
#define AMP_DTS (95)
#endif

#ifndef _define___AMP_DTS_MA
#define _define___AMP_DTS_MA
#define AMP_DTS_MA (96)
#endif

#ifndef _define___AMP_DTS_HRES
#define _define___AMP_DTS_HRES
#define AMP_DTS_HRES (97)
#endif

#ifndef _define___AMP_DTS_LBR
#define _define___AMP_DTS_LBR
#define AMP_DTS_LBR (98)
#endif

#ifndef _define___AMP_DV_SD
#define _define___AMP_DV_SD
#define AMP_DV_SD (99)
#endif

#ifndef _define___AMP_VORBIS
#define _define___AMP_VORBIS
#define AMP_VORBIS (100)
#endif

#ifndef _define___AMP_FLAC
#define _define___AMP_FLAC
#define AMP_FLAC (101)
#endif

#ifndef _define___AMP_REAL_AUD8
#define _define___AMP_REAL_AUD8
#define AMP_REAL_AUD8 (102)
#endif

#ifndef _define___AMP_REAL_AAC
#define _define___AMP_REAL_AAC
#define AMP_REAL_AAC (103)
#endif

#ifndef _define___AMP_IMA_ADPCM
#define _define___AMP_IMA_ADPCM
#define AMP_IMA_ADPCM (104)
#endif

#ifndef _define___AMP_G711A
#define _define___AMP_G711A
#define AMP_G711A (105)
#endif

#ifndef _define___AMP_G711U
#define _define___AMP_G711U
#define AMP_G711U (106)
#endif

#ifndef _define___AMP_DTS_DUMMY
#define _define___AMP_DTS_DUMMY
#define AMP_DTS_DUMMY (107)
#endif

#ifndef _define___AMP_AMRNB
#define _define___AMP_AMRNB
#define AMP_AMRNB (108)
#endif

#ifndef _define___AMP_AMRWB
#define _define___AMP_AMRWB
#define AMP_AMRWB (109)
#endif

#ifndef _define___AMP_M4A_LATM
#define _define___AMP_M4A_LATM
#define AMP_M4A_LATM (110)
#endif

#ifndef _define___AMP_MS11_DDC
#define _define___AMP_MS11_DDC
#define AMP_MS11_DDC (111)
#endif

#ifndef _define___AMP_MS11_DDT
#define _define___AMP_MS11_DDT
#define AMP_MS11_DDT (112)
#endif

#ifndef _define___AMP_MS11_DDRE
#define _define___AMP_MS11_DDRE
#define AMP_MS11_DDRE (113)
#endif

#ifndef _define___AMP_DD_UDC
#define _define___AMP_DD_UDC
#define AMP_DD_UDC (114)
#endif

#ifndef _define___AMP_MS12_AC4
#define _define___AMP_MS12_AC4
#define AMP_MS12_AC4 (115)
#endif

#ifndef _define___AMP_MS12_DDP
#define _define___AMP_MS12_DDP
#define AMP_MS12_DDP (116)
#endif

#ifndef _define___AMP_MS12_MAT
#define _define___AMP_MS12_MAT
#define AMP_MS12_MAT (117)
#endif

#ifndef _define___AMP_MS12_AAC
#define _define___AMP_MS12_AAC
#define AMP_MS12_AAC (118)
#endif

#ifndef _define___AMP_OPUS
#define _define___AMP_OPUS
#define AMP_OPUS (119)
#endif

#ifndef _define___AMP_UNKNOW_AUD
#define _define___AMP_UNKNOW_AUD
#define AMP_UNKNOW_AUD (120)
#endif

#ifndef _define___AMP_MAX_AUDIO
#define _define___AMP_MAX_AUDIO
#define AMP_MAX_AUDIO (121)
#endif

#ifndef _define___AMP_MIXER
#define _define___AMP_MIXER
#define AMP_MIXER (0)
#endif

#ifndef _define___AMP_SRC
#define _define___AMP_SRC
#define AMP_SRC (1)
#endif

#ifndef _define___AMP_SRS
#define _define___AMP_SRS
#define AMP_SRS (2)
#endif

#ifndef _define___AMP_TS
#define _define___AMP_TS
#define AMP_TS (3)
#endif

#ifndef _define___AMP_NEURAL
#define _define___AMP_NEURAL
#define AMP_NEURAL (4)
#endif

#ifndef _define___AMP_MS12
#define _define___AMP_MS12
#define AMP_MS12 (5)
#endif

#ifndef _define___AMP_ROUTER
#define _define___AMP_ROUTER
#define AMP_ROUTER (6)
#endif

#ifndef _typedef___AMP_APP_FORMAT
#define _typedef___AMP_APP_FORMAT
typedef CORBA_enum AMP_APP_FORMAT;
#endif /* _typedef___AMP_APP_FORMAT */

#ifndef _define___AMP_DDCE
#define _define___AMP_DDCE
#define AMP_DDCE (0)
#endif

#ifndef _define___AMP_DTSE
#define _define___AMP_DTSE
#define AMP_DTSE (1)
#endif

#ifndef _define___AMP_AACE
#define _define___AMP_AACE
#define AMP_AACE (2)
#endif

#ifndef _define___AMP_OPUSE
#define _define___AMP_OPUSE
#define AMP_OPUSE (3)
#endif

#ifndef _typedef___AMP_AENC_FORMAT
#define _typedef___AMP_AENC_FORMAT
typedef CORBA_enum AMP_AENC_FORMAT;
#endif /* _typedef___AMP_AENC_FORMAT */

#ifndef _typedef___AMP_ADEC_CONFIG
#define _typedef___AMP_ADEC_CONFIG
typedef struct AMP_ADEC_CONFIG {
  AMP_COMPONENT_MODE mode;
  AMP_ADEC_FORMAT eAdecFmt;
  UINT32 uiInPortNum;
  UINT32 uiOutPortNum;
  UINT32 uiSampRate;
  UINT32 uiBitRate;
  UINT8 ucBitDepth;
  UINT8 ucChannelNum;
  UINT8 ucFrameIn;
  UINT32 uiFlag;
} AMP_ADEC_CONFIG;
#endif /* _typedef___AMP_ADEC_CONFIG */

#ifndef _typedef___AMP_AENC_CONFIG
#define _typedef___AMP_AENC_CONFIG
typedef struct AMP_AENC_CONFIG {
  AMP_COMPONENT_MODE mode;
  AMP_AENC_FORMAT eAencFmt;
  UINT32 uiInPortNum;
  UINT32 uiOutPortNum;
} AMP_AENC_CONFIG;
#endif /* _typedef___AMP_AENC_CONFIG */

#ifndef _typedef___AMP_AREN_CONFIG
#define _typedef___AMP_AREN_CONFIG
typedef struct AMP_AREN_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiInClkPortNum;
  UINT32 uiInPcmPortNum;
  UINT32 uiInSpdifPortNum;
  UINT32 uiInHdmiPortNum;
  UINT32 uiOutPcmPortNum;
  UINT32 uiOutSpdifPortNum;
  UINT32 uiOutHdmiPortNum;
} AMP_AREN_CONFIG;
#endif /* _typedef___AMP_AREN_CONFIG */

#ifndef _typedef___AMP_APP_CONFIG
#define _typedef___AMP_APP_CONFIG
typedef struct AMP_APP_CONFIG {
  AMP_COMPONENT_MODE mode;
  AMP_APP_FORMAT eAppFmt;
  UINT32 uiInPortNum;
  UINT32 uiOutPortNum;
  CORBA_char cAppCompName[128];
  UINT32 eType;
} AMP_APP_CONFIG;
#endif /* _typedef___AMP_APP_CONFIG */

#ifndef _typedef___AMP_AOUT_CONFIG
#define _typedef___AMP_AOUT_CONFIG
typedef struct AMP_AOUT_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_AOUT_CONFIG;
#endif /* _typedef___AMP_AOUT_CONFIG */

#ifndef _typedef___AMP_SNDSINK_CONFIG
#define _typedef___AMP_SNDSINK_CONFIG
typedef struct AMP_SNDSINK_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiInPortNum;
  UINT32 uiOutPortNum;
} AMP_SNDSINK_CONFIG;
#endif /* _typedef___AMP_SNDSINK_CONFIG */

#ifndef _define___AMP_SNDEXP_ROLE_TYPE_SERVER
#define _define___AMP_SNDEXP_ROLE_TYPE_SERVER
#define AMP_SNDEXP_ROLE_TYPE_SERVER (0)
#endif

#ifndef _define___AMP_SNDEXP_ROLE_TYPE_CLIENT
#define _define___AMP_SNDEXP_ROLE_TYPE_CLIENT
#define AMP_SNDEXP_ROLE_TYPE_CLIENT (1)
#endif

#ifndef _define___AMP_SNDEXP_ROLE_TYPE_MAX
#define _define___AMP_SNDEXP_ROLE_TYPE_MAX
#define AMP_SNDEXP_ROLE_TYPE_MAX (2)
#endif

#ifndef _typedef___AMP_SNDEXP_SOCKET_ROLE
#define _typedef___AMP_SNDEXP_SOCKET_ROLE
typedef CORBA_enum AMP_SNDEXP_SOCKET_ROLE;
#endif /* _typedef___AMP_SNDEXP_SOCKET_ROLE */

#ifndef _typedef___AMP_SNDEXP_CONFIG
#define _typedef___AMP_SNDEXP_CONFIG
typedef struct AMP_SNDEXP_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiInPortNum;
  UINT32 uiOutPortNum;
  AMP_SNDEXP_SOCKET_ROLE Role;
} AMP_SNDEXP_CONFIG;
#endif /* _typedef___AMP_SNDEXP_CONFIG */

#ifndef _typedef___AMP_AUDIO_DATA_FMT_TYPE
#define _typedef___AMP_AUDIO_DATA_FMT_TYPE
typedef UINT32 AMP_AUDIO_DATA_FMT_TYPE;
#endif /* _typedef___AMP_AUDIO_DATA_FMT_TYPE */

#ifndef _define___AMP_AUDIO_DATA_INVLIAD_FORMAT
#define _define___AMP_AUDIO_DATA_INVLIAD_FORMAT
#define AMP_AUDIO_DATA_INVLIAD_FORMAT (0)
#endif

#ifndef _define___AMP_AUDIO_PCM_NON_INTERLEAVE
#define _define___AMP_AUDIO_PCM_NON_INTERLEAVE
#define AMP_AUDIO_PCM_NON_INTERLEAVE (1)
#endif

#ifndef _define___AMP_AUDIO_PCM_INTERLEAVE
#define _define___AMP_AUDIO_PCM_INTERLEAVE
#define AMP_AUDIO_PCM_INTERLEAVE (2)
#endif

#ifndef _define___AMP_AUDIO_PCM_PAIR_INTERLEAVE
#define _define___AMP_AUDIO_PCM_PAIR_INTERLEAVE
#define AMP_AUDIO_PCM_PAIR_INTERLEAVE (4)
#endif

#ifndef _define___AMP_AUDIO_SPDIF_FORMAT
#define _define___AMP_AUDIO_SPDIF_FORMAT
#define AMP_AUDIO_SPDIF_FORMAT (8)
#endif

#ifndef _define___AMP_AUDIO_HDMI_FORMAT
#define _define___AMP_AUDIO_HDMI_FORMAT
#define AMP_AUDIO_HDMI_FORMAT (16)
#endif

#ifndef _define___AMP_AUDIO_ES_FORMAT
#define _define___AMP_AUDIO_ES_FORMAT
#define AMP_AUDIO_ES_FORMAT (32)
#endif

#ifndef _define___AMP_AUDIO_HDMI_CFG_DATA
#define _define___AMP_AUDIO_HDMI_CFG_DATA
#define AMP_AUDIO_HDMI_CFG_DATA (64)
#endif

#ifndef _define___AMP_AUDIO_SPDIF_CFG_DATA
#define _define___AMP_AUDIO_SPDIF_CFG_DATA
#define AMP_AUDIO_SPDIF_CFG_DATA (128)
#endif

#ifndef _define___AMP_AUDIO_ES_AAC
#define _define___AMP_AUDIO_ES_AAC
#define AMP_AUDIO_ES_AAC (256)
#endif

#ifndef _define___AMP_AUDIO_ES_DOLBY
#define _define___AMP_AUDIO_ES_DOLBY
#define AMP_AUDIO_ES_DOLBY (512)
#endif

#ifndef _define___AMP_AUDIO_ES_DOLBY_PLUS
#define _define___AMP_AUDIO_ES_DOLBY_PLUS
#define AMP_AUDIO_ES_DOLBY_PLUS (1024)
#endif

#ifndef _define___AMP_AUDIO_ES_DTS
#define _define___AMP_AUDIO_ES_DTS
#define AMP_AUDIO_ES_DTS (2048)
#endif

#ifndef _define___AMP_AUDIO_SPDIF_PCM
#define _define___AMP_AUDIO_SPDIF_PCM
#define AMP_AUDIO_SPDIF_PCM (4096)
#endif

#ifndef _define___AMP_AUDIO_ES_DTSHD
#define _define___AMP_AUDIO_ES_DTSHD
#define AMP_AUDIO_ES_DTSHD (8192)
#endif

#ifndef _define___AMP_AUDIO_IEC61937
#define _define___AMP_AUDIO_IEC61937
#define AMP_AUDIO_IEC61937 (16384)
#endif

#ifndef _define___AMP_AUDIO_ES_MAT
#define _define___AMP_AUDIO_ES_MAT
#define AMP_AUDIO_ES_MAT (32768)
#endif

#ifndef _typedef___AMP_AUDIO_CHANMASK_TYPE
#define _typedef___AMP_AUDIO_CHANMASK_TYPE
typedef UINT32 AMP_AUDIO_CHANMASK_TYPE;
#endif /* _typedef___AMP_AUDIO_CHANMASK_TYPE */

#ifndef _define___AMP_AUDIO_CHMASK_UNDEFINED
#define _define___AMP_AUDIO_CHMASK_UNDEFINED
#define AMP_AUDIO_CHMASK_UNDEFINED (0)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LEFT
#define _define___AMP_AUDIO_CHMASK_LEFT
#define AMP_AUDIO_CHMASK_LEFT (1)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_RGHT
#define _define___AMP_AUDIO_CHMASK_RGHT
#define AMP_AUDIO_CHMASK_RGHT (2)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_CNTR
#define _define___AMP_AUDIO_CHMASK_CNTR
#define AMP_AUDIO_CHMASK_CNTR (4)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_SRRD_LEFT
#define _define___AMP_AUDIO_CHMASK_SRRD_LEFT
#define AMP_AUDIO_CHMASK_SRRD_LEFT (8)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_SRRD_RGHT
#define _define___AMP_AUDIO_CHMASK_SRRD_RGHT
#define AMP_AUDIO_CHMASK_SRRD_RGHT (16)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LFE
#define _define___AMP_AUDIO_CHMASK_LFE
#define AMP_AUDIO_CHMASK_LFE (32)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_SRRD_CNTR
#define _define___AMP_AUDIO_CHMASK_SRRD_CNTR
#define AMP_AUDIO_CHMASK_SRRD_CNTR (64)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LEFT_REAR
#define _define___AMP_AUDIO_CHMASK_LEFT_REAR
#define AMP_AUDIO_CHMASK_LEFT_REAR (128)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_RGHT_REAR
#define _define___AMP_AUDIO_CHMASK_RGHT_REAR
#define AMP_AUDIO_CHMASK_RGHT_REAR (256)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LEFT_SIDE
#define _define___AMP_AUDIO_CHMASK_LEFT_SIDE
#define AMP_AUDIO_CHMASK_LEFT_SIDE (512)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_RGHT_SIDE
#define _define___AMP_AUDIO_CHMASK_RGHT_SIDE
#define AMP_AUDIO_CHMASK_RGHT_SIDE (1024)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LEFT_CNTR
#define _define___AMP_AUDIO_CHMASK_LEFT_CNTR
#define AMP_AUDIO_CHMASK_LEFT_CNTR (2048)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_RGHT_CNTR
#define _define___AMP_AUDIO_CHMASK_RGHT_CNTR
#define AMP_AUDIO_CHMASK_RGHT_CNTR (4096)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_LEFT
#define _define___AMP_AUDIO_CHMASK_HIGH_LEFT
#define AMP_AUDIO_CHMASK_HIGH_LEFT (8192)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_CNTR
#define _define___AMP_AUDIO_CHMASK_HIGH_CNTR
#define AMP_AUDIO_CHMASK_HIGH_CNTR (16384)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_RGHT
#define _define___AMP_AUDIO_CHMASK_HIGH_RGHT
#define AMP_AUDIO_CHMASK_HIGH_RGHT (32768)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LEFT_WIDE
#define _define___AMP_AUDIO_CHMASK_LEFT_WIDE
#define AMP_AUDIO_CHMASK_LEFT_WIDE (65536)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_RGHT_WIDE
#define _define___AMP_AUDIO_CHMASK_RGHT_WIDE
#define AMP_AUDIO_CHMASK_RGHT_WIDE (131072)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_CNTR
#define _define___AMP_AUDIO_CHMASK_TOP_CNTR
#define AMP_AUDIO_CHMASK_TOP_CNTR (262144)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_SIDE_LEFT
#define _define___AMP_AUDIO_CHMASK_HIGH_SIDE_LEFT
#define AMP_AUDIO_CHMASK_HIGH_SIDE_LEFT (524288)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_SIDE_RGHT
#define _define___AMP_AUDIO_CHMASK_HIGH_SIDE_RGHT
#define AMP_AUDIO_CHMASK_HIGH_SIDE_RGHT (1048576)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_REAR_CNTR
#define _define___AMP_AUDIO_CHMASK_HIGH_REAR_CNTR
#define AMP_AUDIO_CHMASK_HIGH_REAR_CNTR (2097152)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_REAR_LEFT
#define _define___AMP_AUDIO_CHMASK_HIGH_REAR_LEFT
#define AMP_AUDIO_CHMASK_HIGH_REAR_LEFT (4194304)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_HIGH_REAR_RGHT
#define _define___AMP_AUDIO_CHMASK_HIGH_REAR_RGHT
#define AMP_AUDIO_CHMASK_HIGH_REAR_RGHT (8388608)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LOW_FRONT_CNTR
#define _define___AMP_AUDIO_CHMASK_LOW_FRONT_CNTR
#define AMP_AUDIO_CHMASK_LOW_FRONT_CNTR (16777216)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LOW_FRONT_LEFT
#define _define___AMP_AUDIO_CHMASK_LOW_FRONT_LEFT
#define AMP_AUDIO_CHMASK_LOW_FRONT_LEFT (33554432)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LOW_FRONT_RGHT
#define _define___AMP_AUDIO_CHMASK_LOW_FRONT_RGHT
#define AMP_AUDIO_CHMASK_LOW_FRONT_RGHT (67108864)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_LFE2
#define _define___AMP_AUDIO_CHMASK_LFE2
#define AMP_AUDIO_CHMASK_LFE2 (134217728)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_MIDDLE_LEFT
#define _define___AMP_AUDIO_CHMASK_TOP_MIDDLE_LEFT
#define AMP_AUDIO_CHMASK_TOP_MIDDLE_LEFT (1073741824)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_MIDDLE_RGHT
#define _define___AMP_AUDIO_CHMASK_TOP_MIDDLE_RGHT
#define AMP_AUDIO_CHMASK_TOP_MIDDLE_RGHT (-2147483648)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_FRONT_LEFT
#define _define___AMP_AUDIO_CHMASK_TOP_FRONT_LEFT
#define AMP_AUDIO_CHMASK_TOP_FRONT_LEFT (524288)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_FRONT_RGHT
#define _define___AMP_AUDIO_CHMASK_TOP_FRONT_RGHT
#define AMP_AUDIO_CHMASK_TOP_FRONT_RGHT (1048576)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_REAR_LEFT
#define _define___AMP_AUDIO_CHMASK_TOP_REAR_LEFT
#define AMP_AUDIO_CHMASK_TOP_REAR_LEFT (4194304)
#endif

#ifndef _define___AMP_AUDIO_CHMASK_TOP_REAR_RGHT
#define _define___AMP_AUDIO_CHMASK_TOP_REAR_RGHT
#define AMP_AUDIO_CHMASK_TOP_REAR_RGHT (8388608)
#endif

#ifndef _typedef___AMP_AUDIO_FS_TYPE
#define _typedef___AMP_AUDIO_FS_TYPE
typedef UINT32 AMP_AUDIO_FS_TYPE;
#endif /* _typedef___AMP_AUDIO_FS_TYPE */

#ifndef _define___AMP_AUDIO_FS_8K
#define _define___AMP_AUDIO_FS_8K
#define AMP_AUDIO_FS_8K (8000)
#endif

#ifndef _define___AMP_AUDIO_FS_11K
#define _define___AMP_AUDIO_FS_11K
#define AMP_AUDIO_FS_11K (11025)
#endif

#ifndef _define___AMP_AUDIO_FS_12K
#define _define___AMP_AUDIO_FS_12K
#define AMP_AUDIO_FS_12K (12000)
#endif

#ifndef _define___AMP_AUDIO_FS_16K
#define _define___AMP_AUDIO_FS_16K
#define AMP_AUDIO_FS_16K (16000)
#endif

#ifndef _define___AMP_AUDIO_FS_22K
#define _define___AMP_AUDIO_FS_22K
#define AMP_AUDIO_FS_22K (22050)
#endif

#ifndef _define___AMP_AUDIO_FS_24K
#define _define___AMP_AUDIO_FS_24K
#define AMP_AUDIO_FS_24K (24000)
#endif

#ifndef _define___AMP_AUDIO_FS_32K
#define _define___AMP_AUDIO_FS_32K
#define AMP_AUDIO_FS_32K (32000)
#endif

#ifndef _define___AMP_AUDIO_FS_44K
#define _define___AMP_AUDIO_FS_44K
#define AMP_AUDIO_FS_44K (44100)
#endif

#ifndef _define___AMP_AUDIO_FS_48K
#define _define___AMP_AUDIO_FS_48K
#define AMP_AUDIO_FS_48K (48000)
#endif

#ifndef _define___AMP_AUDIO_FS_64K
#define _define___AMP_AUDIO_FS_64K
#define AMP_AUDIO_FS_64K (64000)
#endif

#ifndef _define___AMP_AUDIO_FS_88K
#define _define___AMP_AUDIO_FS_88K
#define AMP_AUDIO_FS_88K (88200)
#endif

#ifndef _define___AMP_AUDIO_FS_96K
#define _define___AMP_AUDIO_FS_96K
#define AMP_AUDIO_FS_96K (96000)
#endif

#ifndef _define___AMP_AUDIO_FS_176K
#define _define___AMP_AUDIO_FS_176K
#define AMP_AUDIO_FS_176K (176400)
#endif

#ifndef _define___AMP_AUDIO_FS_192K
#define _define___AMP_AUDIO_FS_192K
#define AMP_AUDIO_FS_192K (192000)
#endif

#ifndef _define___AMP_AUDIO_FS_384K
#define _define___AMP_AUDIO_FS_384K
#define AMP_AUDIO_FS_384K (384000)
#endif

#ifndef _define___AMP_AUDIO_FS_768K
#define _define___AMP_AUDIO_FS_768K
#define AMP_AUDIO_FS_768K (768000)
#endif

#ifndef _typedef___AMP_AUDIO_BITDEPTH_TYPE
#define _typedef___AMP_AUDIO_BITDEPTH_TYPE
typedef UINT32 AMP_AUDIO_BITDEPTH_TYPE;
#endif /* _typedef___AMP_AUDIO_BITDEPTH_TYPE */

#ifndef _define___AMP_AUDIO_PCM_8BIT
#define _define___AMP_AUDIO_PCM_8BIT
#define AMP_AUDIO_PCM_8BIT (8)
#endif

#ifndef _define___AMP_AUDIO_PCM_16BIT
#define _define___AMP_AUDIO_PCM_16BIT
#define AMP_AUDIO_PCM_16BIT (16)
#endif

#ifndef _define___AMP_AUDIO_PCM_20BIT
#define _define___AMP_AUDIO_PCM_20BIT
#define AMP_AUDIO_PCM_20BIT (20)
#endif

#ifndef _define___AMP_AUDIO_PCM_24BIT
#define _define___AMP_AUDIO_PCM_24BIT
#define AMP_AUDIO_PCM_24BIT (24)
#endif

#ifndef _define___AMP_AUDIO_PCM_32BIT
#define _define___AMP_AUDIO_PCM_32BIT
#define AMP_AUDIO_PCM_32BIT (32)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS8_SINGED
#define _define___AMP_AUDIO_PCMBITS8_SINGED
#define AMP_AUDIO_PCMBITS8_SINGED (65544)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS16_SINGED
#define _define___AMP_AUDIO_PCMBITS16_SINGED
#define AMP_AUDIO_PCMBITS16_SINGED (65552)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS24_SINGED
#define _define___AMP_AUDIO_PCMBITS24_SINGED
#define AMP_AUDIO_PCMBITS24_SINGED (65560)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS32_SINGED
#define _define___AMP_AUDIO_PCMBITS32_SINGED
#define AMP_AUDIO_PCMBITS32_SINGED (65568)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS8_UNSIGNED
#define _define___AMP_AUDIO_PCMBITS8_UNSIGNED
#define AMP_AUDIO_PCMBITS8_UNSIGNED (131080)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS16_UNSIGNED
#define _define___AMP_AUDIO_PCMBITS16_UNSIGNED
#define AMP_AUDIO_PCMBITS16_UNSIGNED (131088)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS24_UNSIGNED
#define _define___AMP_AUDIO_PCMBITS24_UNSIGNED
#define AMP_AUDIO_PCMBITS24_UNSIGNED (131096)
#endif

#ifndef _define___AMP_AUDIO_PCMBITS32_UNSIGNED
#define _define___AMP_AUDIO_PCMBITS32_UNSIGNED
#define AMP_AUDIO_PCMBITS32_UNSIGNED (131104)
#endif

#ifndef _define___AMP_AUDIO_PCMBITFLT_SIGNED
#define _define___AMP_AUDIO_PCMBITFLT_SIGNED
#define AMP_AUDIO_PCMBITFLT_SIGNED (196640)
#endif

#ifndef _typedef___AMP_AUDIO_ENDIAN_TYPE
#define _typedef___AMP_AUDIO_ENDIAN_TYPE
typedef UINT32 AMP_AUDIO_ENDIAN_TYPE;
#endif /* _typedef___AMP_AUDIO_ENDIAN_TYPE */

#ifndef _define___AMP_AUDIO_PCM_LITTLE_ENDIAN
#define _define___AMP_AUDIO_PCM_LITTLE_ENDIAN
#define AMP_AUDIO_PCM_LITTLE_ENDIAN (0)
#endif

#ifndef _define___AMP_AUDIO_PCM_BIG_ENDIAN
#define _define___AMP_AUDIO_PCM_BIG_ENDIAN
#define AMP_AUDIO_PCM_BIG_ENDIAN (1)
#endif

#ifndef _typedef___AMP_AVIN_CONFIG
#define _typedef___AMP_AVIN_CONFIG
typedef struct AMP_AVIN_CONFIG {
  AMP_COMPONENT_MODE mode;
  UINT32 uiRegisterNotifyNum;
  BOOL bOpenVideoPort;
  BOOL bOpenVideoPort2;
  BOOL bOpenVbiPort;
  BOOL bOpenAudioPort;
  BOOL bOpenAudioPort2;
} AMP_AVIN_CONFIG;
#endif /* _typedef___AMP_AVIN_CONFIG */

#ifndef _typedef___AMP_DVS_CONFIG
#define _typedef___AMP_DVS_CONFIG
typedef struct AMP_DVS_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_DVS_CONFIG;
#endif /* _typedef___AMP_DVS_CONFIG */

#ifndef _typedef___AMP_DATA_EXPORTER_CONFIG
#define _typedef___AMP_DATA_EXPORTER_CONFIG
typedef struct AMP_DATA_EXPORTER_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_DATA_EXPORTER_CONFIG;
#endif /* _typedef___AMP_DATA_EXPORTER_CONFIG */

#ifndef _typedef___AMP_ISP_CONFIG
#define _typedef___AMP_ISP_CONFIG
typedef struct AMP_ISP_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_ISP_CONFIG;
#endif /* _typedef___AMP_ISP_CONFIG */

#ifndef _typedef___AMP_DEWARP_CONFIG
#define _typedef___AMP_DEWARP_CONFIG
typedef struct AMP_DEWARP_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_DEWARP_CONFIG;
#endif /* _typedef___AMP_DEWARP_CONFIG */

#ifndef _typedef___AMP_ROTATE_CONFIG
#define _typedef___AMP_ROTATE_CONFIG
typedef struct AMP_ROTATE_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_ROTATE_CONFIG;
#endif /* _typedef___AMP_ROTATE_CONFIG */

#ifndef _typedef___AMP_DNS_CONFIG
#define _typedef___AMP_DNS_CONFIG
typedef struct AMP_DNS_CONFIG {
  AMP_COMPONENT_MODE mode;
} AMP_DNS_CONFIG;
#endif /* _typedef___AMP_DNS_CONFIG */

#ifndef _typedef___AMP_PORT
#define _typedef___AMP_PORT
typedef CORBA_Object AMP_PORT;
#endif /* _typedef___AMP_PORT */

#ifndef _typedef___AMP_PORT_IO
#define _typedef___AMP_PORT_IO
typedef UINT8 AMP_PORT_IO;
#endif /* _typedef___AMP_PORT_IO */

#ifndef _define___AMP_PORT_INPUT
#define _define___AMP_PORT_INPUT
#define AMP_PORT_INPUT (0)
#endif

#ifndef _define___AMP_PORT_OUTPUT
#define _define___AMP_PORT_OUTPUT
#define AMP_PORT_OUTPUT (1)
#endif

#ifndef _define___AMP_PORT_INVALIDE_IO
#define _define___AMP_PORT_INVALIDE_IO
#define AMP_PORT_INVALIDE_IO (2)
#endif

#ifndef _define___AMP_PORT_INVALIDE_IDX
#define _define___AMP_PORT_INVALIDE_IDX
#define AMP_PORT_INVALIDE_IDX (-1)
#endif

#ifndef _define___AMP_PORT_NONE
#define _define___AMP_PORT_NONE
#define AMP_PORT_NONE (0)
#endif

#ifndef _define___AMP_PORT_APPLICATION
#define _define___AMP_PORT_APPLICATION
#define AMP_PORT_APPLICATION (1)
#endif

#ifndef _define___AMP_PORT_DUM_IN
#define _define___AMP_PORT_DUM_IN
#define AMP_PORT_DUM_IN (2)
#endif

#ifndef _define___AMP_PORT_DUM_OUT
#define _define___AMP_PORT_DUM_OUT
#define AMP_PORT_DUM_OUT (3)
#endif

#ifndef _define___AMP_PORT_CLK_IN_CLOCK
#define _define___AMP_PORT_CLK_IN_CLOCK
#define AMP_PORT_CLK_IN_CLOCK (4)
#endif

#ifndef _define___AMP_PORT_CLK_OUT_CLOCK
#define _define___AMP_PORT_CLK_OUT_CLOCK
#define AMP_PORT_CLK_OUT_CLOCK (5)
#endif

#ifndef _define___AMP_PORT_DMX_IN_TS_MEM
#define _define___AMP_PORT_DMX_IN_TS_MEM
#define AMP_PORT_DMX_IN_TS_MEM (6)
#endif

#ifndef _define___AMP_PORT_DMX_IN_TS_FE
#define _define___AMP_PORT_DMX_IN_TS_FE
#define AMP_PORT_DMX_IN_TS_FE (7)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_CLOCK
#define _define___AMP_PORT_DMX_OUT_CLOCK
#define AMP_PORT_DMX_OUT_CLOCK (8)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_ANY
#define _define___AMP_PORT_DMX_OUT_ANY
#define AMP_PORT_DMX_OUT_ANY (9)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_VIDEO_ES
#define _define___AMP_PORT_DMX_OUT_VIDEO_ES
#define AMP_PORT_DMX_OUT_VIDEO_ES (10)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_AUDIO_ES
#define _define___AMP_PORT_DMX_OUT_AUDIO_ES
#define AMP_PORT_DMX_OUT_AUDIO_ES (11)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_VIDEO_PES
#define _define___AMP_PORT_DMX_OUT_VIDEO_PES
#define AMP_PORT_DMX_OUT_VIDEO_PES (12)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_AUDIO_PES
#define _define___AMP_PORT_DMX_OUT_AUDIO_PES
#define AMP_PORT_DMX_OUT_AUDIO_PES (13)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_TS
#define _define___AMP_PORT_DMX_OUT_TS
#define AMP_PORT_DMX_OUT_TS (14)
#endif

#ifndef _define___AMP_PORT_DMX_OUT_TABLE
#define _define___AMP_PORT_DMX_OUT_TABLE
#define AMP_PORT_DMX_OUT_TABLE (15)
#endif

#ifndef _define___AMP_PORT_VDEC_IN_ES_MEM
#define _define___AMP_PORT_VDEC_IN_ES_MEM
#define AMP_PORT_VDEC_IN_ES_MEM (16)
#endif

#ifndef _define___AMP_PORT_VDEC_OUT_FRAMES
#define _define___AMP_PORT_VDEC_OUT_FRAMES
#define AMP_PORT_VDEC_OUT_FRAMES (17)
#endif

#ifndef _define___AMP_PORT_VDEC_OUT_USERDATA
#define _define___AMP_PORT_VDEC_OUT_USERDATA
#define AMP_PORT_VDEC_OUT_USERDATA (18)
#endif

#ifndef _define___AMP_PORT_IMGDEC_IN_ES
#define _define___AMP_PORT_IMGDEC_IN_ES
#define AMP_PORT_IMGDEC_IN_ES (19)
#endif

#ifndef _define___AMP_PORT_IMGDEC_OUT_FRAMES
#define _define___AMP_PORT_IMGDEC_OUT_FRAMES
#define AMP_PORT_IMGDEC_OUT_FRAMES (20)
#endif

#ifndef _define___AMP_PORT_VENC_IN_FRAMES
#define _define___AMP_PORT_VENC_IN_FRAMES
#define AMP_PORT_VENC_IN_FRAMES (21)
#endif

#ifndef _define___AMP_PORT_VENC_IN_USERDATA
#define _define___AMP_PORT_VENC_IN_USERDATA
#define AMP_PORT_VENC_IN_USERDATA (22)
#endif

#ifndef _define___AMP_PORT_VENC_OUT_ES
#define _define___AMP_PORT_VENC_OUT_ES
#define AMP_PORT_VENC_OUT_ES (23)
#endif

#ifndef _define___AMP_PORT_VOUT_IN_CLOCK
#define _define___AMP_PORT_VOUT_IN_CLOCK
#define AMP_PORT_VOUT_IN_CLOCK (24)
#endif

#ifndef _define___AMP_PORT_VOUT_IN_TO_MAIN
#define _define___AMP_PORT_VOUT_IN_TO_MAIN
#define AMP_PORT_VOUT_IN_TO_MAIN (25)
#endif

#ifndef _define___AMP_PORT_VOUT_IN_TO_SUB
#define _define___AMP_PORT_VOUT_IN_TO_SUB
#define AMP_PORT_VOUT_IN_TO_SUB (26)
#endif

#ifndef _define___AMP_PORT_VOUT_IN_DV_EL
#define _define___AMP_PORT_VOUT_IN_DV_EL
#define AMP_PORT_VOUT_IN_DV_EL (27)
#endif

#ifndef _define___AMP_PORT_VOUT_IN_DV_META
#define _define___AMP_PORT_VOUT_IN_DV_META
#define AMP_PORT_VOUT_IN_DV_META (28)
#endif

#ifndef _define___AMP_PORT_ADEC_IN_ES
#define _define___AMP_PORT_ADEC_IN_ES
#define AMP_PORT_ADEC_IN_ES (29)
#endif

#ifndef _define___AMP_PORT_ADEC_IN_ASSOC_ES
#define _define___AMP_PORT_ADEC_IN_ASSOC_ES
#define AMP_PORT_ADEC_IN_ASSOC_ES (30)
#endif

#ifndef _define___AMP_PORT_ADEC_IN_EXT_PCM
#define _define___AMP_PORT_ADEC_IN_EXT_PCM
#define AMP_PORT_ADEC_IN_EXT_PCM (31)
#endif

#ifndef _define___AMP_PORT_ADEC_OUT_PCM
#define _define___AMP_PORT_ADEC_OUT_PCM
#define AMP_PORT_ADEC_OUT_PCM (32)
#endif

#ifndef _define___AMP_PORT_ADEC_OUT_SPDIF
#define _define___AMP_PORT_ADEC_OUT_SPDIF
#define AMP_PORT_ADEC_OUT_SPDIF (33)
#endif

#ifndef _define___AMP_PORT_ADEC_OUT_HDMI
#define _define___AMP_PORT_ADEC_OUT_HDMI
#define AMP_PORT_ADEC_OUT_HDMI (34)
#endif

#ifndef _define___AMP_PORT_ADEC_OUT_DUPLI_PCM
#define _define___AMP_PORT_ADEC_OUT_DUPLI_PCM
#define AMP_PORT_ADEC_OUT_DUPLI_PCM (35)
#endif

#ifndef _define___AMP_PORT_ADEC_OUT_SEC_PCM
#define _define___AMP_PORT_ADEC_OUT_SEC_PCM
#define AMP_PORT_ADEC_OUT_SEC_PCM (36)
#endif

#ifndef _define___AMP_PORT_ADEC_OUT_ES
#define _define___AMP_PORT_ADEC_OUT_ES
#define AMP_PORT_ADEC_OUT_ES (37)
#endif

#ifndef _define___AMP_PORT_AENC_IN_SAMPLES
#define _define___AMP_PORT_AENC_IN_SAMPLES
#define AMP_PORT_AENC_IN_SAMPLES (38)
#endif

#ifndef _define___AMP_PORT_AENC_OUT_ES
#define _define___AMP_PORT_AENC_OUT_ES
#define AMP_PORT_AENC_OUT_ES (39)
#endif

#ifndef _define___AMP_PORT_AENC_OUT_SPDIF
#define _define___AMP_PORT_AENC_OUT_SPDIF
#define AMP_PORT_AENC_OUT_SPDIF (40)
#endif

#ifndef _define___AMP_PORT_AENC_OUT_HDMI
#define _define___AMP_PORT_AENC_OUT_HDMI
#define AMP_PORT_AENC_OUT_HDMI (41)
#endif

#ifndef _define___AMP_PORT_AREN_IN_CLOCK
#define _define___AMP_PORT_AREN_IN_CLOCK
#define AMP_PORT_AREN_IN_CLOCK (42)
#endif

#ifndef _define___AMP_PORT_AREN_IN_PCM
#define _define___AMP_PORT_AREN_IN_PCM
#define AMP_PORT_AREN_IN_PCM (43)
#endif

#ifndef _define___AMP_PORT_AREN_IN_SPDIF
#define _define___AMP_PORT_AREN_IN_SPDIF
#define AMP_PORT_AREN_IN_SPDIF (44)
#endif

#ifndef _define___AMP_PORT_AREN_IN_HDMI
#define _define___AMP_PORT_AREN_IN_HDMI
#define AMP_PORT_AREN_IN_HDMI (45)
#endif

#ifndef _define___AMP_PORT_AREN_OUT_PCM
#define _define___AMP_PORT_AREN_OUT_PCM
#define AMP_PORT_AREN_OUT_PCM (46)
#endif

#ifndef _define___AMP_PORT_AREN_OUT_SPDIF
#define _define___AMP_PORT_AREN_OUT_SPDIF
#define AMP_PORT_AREN_OUT_SPDIF (47)
#endif

#ifndef _define___AMP_PORT_AREN_OUT_HDMI
#define _define___AMP_PORT_AREN_OUT_HDMI
#define AMP_PORT_AREN_OUT_HDMI (48)
#endif

#ifndef _define___AMP_PORT_APP_IN_SAMPLES
#define _define___AMP_PORT_APP_IN_SAMPLES
#define AMP_PORT_APP_IN_SAMPLES (49)
#endif

#ifndef _define___AMP_PORT_APP_OUT_SAMPLES
#define _define___AMP_PORT_APP_OUT_SAMPLES
#define AMP_PORT_APP_OUT_SAMPLES (50)
#endif

#ifndef _define___AMP_PORT_AOUT_IN_TO_SPDIF
#define _define___AMP_PORT_AOUT_IN_TO_SPDIF
#define AMP_PORT_AOUT_IN_TO_SPDIF (51)
#endif

#ifndef _define___AMP_PORT_AOUT_IN_TO_I2S
#define _define___AMP_PORT_AOUT_IN_TO_I2S
#define AMP_PORT_AOUT_IN_TO_I2S (52)
#endif

#ifndef _define___AMP_PORT_AOUT_IN_TO_HDMI
#define _define___AMP_PORT_AOUT_IN_TO_HDMI
#define AMP_PORT_AOUT_IN_TO_HDMI (53)
#endif

#ifndef _define___AMP_PORT_AVIN_OUT_FRAMES
#define _define___AMP_PORT_AVIN_OUT_FRAMES
#define AMP_PORT_AVIN_OUT_FRAMES (54)
#endif

#ifndef _define___AMP_PORT_AVIN_OUT_FRAMES_2
#define _define___AMP_PORT_AVIN_OUT_FRAMES_2
#define AMP_PORT_AVIN_OUT_FRAMES_2 (55)
#endif

#ifndef _define___AMP_PORT_AVIN_OUT_VBI
#define _define___AMP_PORT_AVIN_OUT_VBI
#define AMP_PORT_AVIN_OUT_VBI (56)
#endif

#ifndef _define___AMP_PORT_AVIN_OUT_AUDIO
#define _define___AMP_PORT_AVIN_OUT_AUDIO
#define AMP_PORT_AVIN_OUT_AUDIO (57)
#endif

#ifndef _define___AMP_PORT_AVIN_OUT_AUDIO_2
#define _define___AMP_PORT_AVIN_OUT_AUDIO_2
#define AMP_PORT_AVIN_OUT_AUDIO_2 (58)
#endif

#ifndef _define___AMP_PORT_FE_OUT_TS
#define _define___AMP_PORT_FE_OUT_TS
#define AMP_PORT_FE_OUT_TS (59)
#endif

#ifndef _define___AMP_PORT_CI_IN_TS
#define _define___AMP_PORT_CI_IN_TS
#define AMP_PORT_CI_IN_TS (60)
#endif

#ifndef _define___AMP_PORT_CI_OUT_TS
#define _define___AMP_PORT_CI_OUT_TS
#define AMP_PORT_CI_OUT_TS (61)
#endif

#ifndef _define___AMP_PORT_SNDEXP_IN
#define _define___AMP_PORT_SNDEXP_IN
#define AMP_PORT_SNDEXP_IN (62)
#endif

#ifndef _define___AMP_PORT_DEINT_IN
#define _define___AMP_PORT_DEINT_IN
#define AMP_PORT_DEINT_IN (63)
#endif

#ifndef _define___AMP_PORT_DEINT_OUT
#define _define___AMP_PORT_DEINT_OUT
#define AMP_PORT_DEINT_OUT (64)
#endif

#ifndef _define___AMP_PORT_SCALER_IN
#define _define___AMP_PORT_SCALER_IN
#define AMP_PORT_SCALER_IN (65)
#endif

#ifndef _define___AMP_PORT_SCALER_OUT
#define _define___AMP_PORT_SCALER_OUT
#define AMP_PORT_SCALER_OUT (66)
#endif

#ifndef _define___AMP_PORT_DVS_IN_0
#define _define___AMP_PORT_DVS_IN_0
#define AMP_PORT_DVS_IN_0 (67)
#endif

#ifndef _define___AMP_PORT_DVS_IN_1
#define _define___AMP_PORT_DVS_IN_1
#define AMP_PORT_DVS_IN_1 (68)
#endif

#ifndef _define___AMP_PORT_DVS_OUT_BL
#define _define___AMP_PORT_DVS_OUT_BL
#define AMP_PORT_DVS_OUT_BL (69)
#endif

#ifndef _define___AMP_PORT_DVS_OUT_EL
#define _define___AMP_PORT_DVS_OUT_EL
#define AMP_PORT_DVS_OUT_EL (70)
#endif

#ifndef _define___AMP_PORT_DVS_OUT_RPU
#define _define___AMP_PORT_DVS_OUT_RPU
#define AMP_PORT_DVS_OUT_RPU (71)
#endif

#ifndef _define___AMP_PORT_VCAP_IN
#define _define___AMP_PORT_VCAP_IN
#define AMP_PORT_VCAP_IN (72)
#endif

#ifndef _define___AMP_PORT_VCAP_OUT
#define _define___AMP_PORT_VCAP_OUT
#define AMP_PORT_VCAP_OUT (73)
#endif

#ifndef _define___AMP_PORT_SR_IN_Y
#define _define___AMP_PORT_SR_IN_Y
#define AMP_PORT_SR_IN_Y (74)
#endif

#ifndef _define___AMP_PORT_SR_IN_UV
#define _define___AMP_PORT_SR_IN_UV
#define AMP_PORT_SR_IN_UV (75)
#endif

#ifndef _define___AMP_PORT_SR_OUT
#define _define___AMP_PORT_SR_OUT
#define AMP_PORT_SR_OUT (76)
#endif

#ifndef _define___AMP_PORT_VA_IN
#define _define___AMP_PORT_VA_IN
#define AMP_PORT_VA_IN (77)
#endif

#ifndef _define___AMP_PORT_VA_OUT
#define _define___AMP_PORT_VA_OUT
#define AMP_PORT_VA_OUT (78)
#endif

#ifndef _define___AMP_PORT_DATA_EXPORTER_IN
#define _define___AMP_PORT_DATA_EXPORTER_IN
#define AMP_PORT_DATA_EXPORTER_IN (79)
#endif

#ifndef _define___AMP_PORT_DATA_EXPORTER_OUT
#define _define___AMP_PORT_DATA_EXPORTER_OUT
#define AMP_PORT_DATA_EXPORTER_OUT (80)
#endif

#ifndef _define___AMP_PORT_ISP_IN
#define _define___AMP_PORT_ISP_IN
#define AMP_PORT_ISP_IN (81)
#endif

#ifndef _define___AMP_PORT_ISP_OUT_MP
#define _define___AMP_PORT_ISP_OUT_MP
#define AMP_PORT_ISP_OUT_MP (82)
#endif

#ifndef _define___AMP_PORT_ISP_OUT_SP1
#define _define___AMP_PORT_ISP_OUT_SP1
#define AMP_PORT_ISP_OUT_SP1 (83)
#endif

#ifndef _define___AMP_PORT_ISP_OUT_SP2
#define _define___AMP_PORT_ISP_OUT_SP2
#define AMP_PORT_ISP_OUT_SP2 (84)
#endif

#ifndef _define___AMP_PORT_DEWARP_IN
#define _define___AMP_PORT_DEWARP_IN
#define AMP_PORT_DEWARP_IN (85)
#endif

#ifndef _define___AMP_PORT_DEWARP_OUT
#define _define___AMP_PORT_DEWARP_OUT
#define AMP_PORT_DEWARP_OUT (86)
#endif

#ifndef _define___AMP_PORT_ROTATE_IN
#define _define___AMP_PORT_ROTATE_IN
#define AMP_PORT_ROTATE_IN (87)
#endif

#ifndef _define___AMP_PORT_ROTATE_OUT
#define _define___AMP_PORT_ROTATE_OUT
#define AMP_PORT_ROTATE_OUT (88)
#endif

#ifndef _define___AMP_PORT_DNS_IN
#define _define___AMP_PORT_DNS_IN
#define AMP_PORT_DNS_IN (89)
#endif

#ifndef _define___AMP_PORT_DNS_OUT
#define _define___AMP_PORT_DNS_OUT
#define AMP_PORT_DNS_OUT (90)
#endif

#ifndef _typedef___AMP_PORT_TYPE
#define _typedef___AMP_PORT_TYPE
typedef CORBA_enum AMP_PORT_TYPE;
#endif /* _typedef___AMP_PORT_TYPE */

#ifndef _typedef___AMP_PORT_INFO
#define _typedef___AMP_PORT_INFO
typedef struct AMP_PORT_INFO {
  AMP_PORT_IO ePortIo;
  AMP_PORT_TYPE ePortType;
  AMP_MEDIA_CAP pMediaCap;
  UINT32 uiPortAttrib;
  UINT32 uiBufAlign;
  UINT32 uiPaddingSize;
  UINT32 pBuffStart;
  UINT32 uiBufSize;
  UINT32 uiBufPaddingSize;
} AMP_PORT_INFO;
#endif /* _typedef___AMP_PORT_INFO */

#ifndef _typedef___AMP_PORT_CONN
#define _typedef___AMP_PORT_CONN
typedef struct AMP_PORT_CONN {
  AMP_PORT_TYPE ePeerType;
  AMP_COMPONENT hPeerComponent;
  UINT32 uiPeerPortIdx;
  UINT32 hInterHandle;
  UINT32 hInterLockM;
  UINT32 hInterLockC;
  UINT32 uiFlags;
} AMP_PORT_CONN;
#endif /* _typedef___AMP_PORT_CONN */

#ifndef _typedef___AMP_COMPONENT
#define _typedef___AMP_COMPONENT
typedef CORBA_Object AMP_COMPONENT;
#endif /* _typedef___AMP_COMPONENT */

#ifndef _define___AMP_COMPONENT_NAME_LENGTH_MAX
#define _define___AMP_COMPONENT_NAME_LENGTH_MAX
#define AMP_COMPONENT_NAME_LENGTH_MAX (16)
#endif

#ifndef _typedef___AMP_COMPONENT_TYPE
#define _typedef___AMP_COMPONENT_TYPE
typedef UINT8 AMP_COMPONENT_TYPE;
#endif /* _typedef___AMP_COMPONENT_TYPE */

#ifndef _define___AMP_COMPONENT_DUM
#define _define___AMP_COMPONENT_DUM
#define AMP_COMPONENT_DUM (1)
#endif

#ifndef _define___AMP_COMPONENT_CLK
#define _define___AMP_COMPONENT_CLK
#define AMP_COMPONENT_CLK (2)
#endif

#ifndef _define___AMP_COMPONENT_IMGDEC
#define _define___AMP_COMPONENT_IMGDEC
#define AMP_COMPONENT_IMGDEC (3)
#endif

#ifndef _define___AMP_COMPONENT_DMX
#define _define___AMP_COMPONENT_DMX
#define AMP_COMPONENT_DMX (4)
#endif

#ifndef _define___AMP_COMPONENT_VDEC
#define _define___AMP_COMPONENT_VDEC
#define AMP_COMPONENT_VDEC (5)
#endif

#ifndef _define___AMP_COMPONENT_VENC
#define _define___AMP_COMPONENT_VENC
#define AMP_COMPONENT_VENC (6)
#endif

#ifndef _define___AMP_COMPONENT_VOUT
#define _define___AMP_COMPONENT_VOUT
#define AMP_COMPONENT_VOUT (7)
#endif

#ifndef _define___AMP_COMPONENT_ADEC
#define _define___AMP_COMPONENT_ADEC
#define AMP_COMPONENT_ADEC (8)
#endif

#ifndef _define___AMP_COMPONENT_AENC
#define _define___AMP_COMPONENT_AENC
#define AMP_COMPONENT_AENC (9)
#endif

#ifndef _define___AMP_COMPONENT_AREN
#define _define___AMP_COMPONENT_AREN
#define AMP_COMPONENT_AREN (10)
#endif

#ifndef _define___AMP_COMPONENT_APP
#define _define___AMP_COMPONENT_APP
#define AMP_COMPONENT_APP (11)
#endif

#ifndef _define___AMP_COMPONENT_AVIN
#define _define___AMP_COMPONENT_AVIN
#define AMP_COMPONENT_AVIN (12)
#endif

#ifndef _define___AMP_COMPONENT_AOUT
#define _define___AMP_COMPONENT_AOUT
#define AMP_COMPONENT_AOUT (13)
#endif

#ifndef _define___AMP_COMPONENT_FE
#define _define___AMP_COMPONENT_FE
#define AMP_COMPONENT_FE (14)
#endif

#ifndef _define___AMP_COMPONENT_IVPP
#define _define___AMP_COMPONENT_IVPP
#define AMP_COMPONENT_IVPP (15)
#endif

#ifndef _define___AMP_COMPONENT_DEINT
#define _define___AMP_COMPONENT_DEINT
#define AMP_COMPONENT_DEINT (16)
#endif

#ifndef _define___AMP_COMPONENT_OVPP
#define _define___AMP_COMPONENT_OVPP
#define AMP_COMPONENT_OVPP (255)
#endif

#ifndef _define___AMP_COMPONENT_SNDSINK
#define _define___AMP_COMPONENT_SNDSINK
#define AMP_COMPONENT_SNDSINK (17)
#endif

#ifndef _define___AMP_COMPONENT_CI
#define _define___AMP_COMPONENT_CI
#define AMP_COMPONENT_CI (18)
#endif

#ifndef _define___AMP_COMPONENT_SNDEXP
#define _define___AMP_COMPONENT_SNDEXP
#define AMP_COMPONENT_SNDEXP (19)
#endif

#ifndef _define___AMP_COMPONENT_DVS
#define _define___AMP_COMPONENT_DVS
#define AMP_COMPONENT_DVS (20)
#endif

#ifndef _define___AMP_COMPONENT_DATA_EXPORTER
#define _define___AMP_COMPONENT_DATA_EXPORTER
#define AMP_COMPONENT_DATA_EXPORTER (21)
#endif

#ifndef _define___AMP_COMPONENT_SCALER
#define _define___AMP_COMPONENT_SCALER
#define AMP_COMPONENT_SCALER (22)
#endif

#ifndef _define___AMP_COMPONENT_VCAP
#define _define___AMP_COMPONENT_VCAP
#define AMP_COMPONENT_VCAP (23)
#endif

#ifndef _define___AMP_COMPONENT_ISP
#define _define___AMP_COMPONENT_ISP
#define AMP_COMPONENT_ISP (24)
#endif

#ifndef _define___AMP_COMPONENT_DEWARP
#define _define___AMP_COMPONENT_DEWARP
#define AMP_COMPONENT_DEWARP (25)
#endif

#ifndef _define___AMP_COMPONENT_ROTATE
#define _define___AMP_COMPONENT_ROTATE
#define AMP_COMPONENT_ROTATE (26)
#endif

#ifndef _define___AMP_COMPONENT_DNS
#define _define___AMP_COMPONENT_DNS
#define AMP_COMPONENT_DNS (27)
#endif

#ifndef _define___AMP_COMPONENT_SR
#define _define___AMP_COMPONENT_SR
#define AMP_COMPONENT_SR (28)
#endif

#ifndef _define___AMP_COMPONENT_VA
#define _define___AMP_COMPONENT_VA
#define AMP_COMPONENT_VA (29)
#endif

#ifndef _typedef___AMP_COMPONENT_INFO
#define _typedef___AMP_COMPONENT_INFO
typedef struct AMP_COMPONENT_INFO {
  AMP_COMPONENT_TYPE eType;
  AMP_COMPONENT_MODE mode;
  CORBA_char sName[16];
  AMP_STATE eState;
  AMP_MEDIA_CAP pMediaCap;
  UINT32 uiInputPortNum;
  UINT32 uiOutputPortNum;
  UINT32 hInterHandle;
} AMP_COMPONENT_INFO;
#endif /* _typedef___AMP_COMPONENT_INFO */

#ifndef _typedef___AMP_COMPONENT_CONFIG
#define _typedef___AMP_COMPONENT_CONFIG
typedef struct AMP_COMPONENT_CONFIG {
  AMP_COMPONENT_TYPE _d;
  union {
    struct AMP_DUM_CONFIG pDUM;
    struct AMP_CLK_CONFIG pCLK;
    struct AMP_DMX_CONFIG pDMX;
    struct AMP_VDEC_CONFIG pVDEC;
    struct AMP_VENC_CONFIG pVENC;
    struct AMP_IMGDEC_CONFIG pIMGDEC;
    struct AMP_VOUT_CONFIG pVOUT;
    AMP_OVPP_CONFIG pOVPP;
    struct AMP_DEINT_CONFIG pDEINT;
    struct AMP_SCALER_CONFIG pSCALER;
    struct AMP_VCAP_CONFIG pVCAP;
    struct AMP_SR_CONFIG pSR;
    struct AMP_VA_CONFIG pVA;
    struct AMP_ADEC_CONFIG pADEC;
    struct AMP_AENC_CONFIG pAENC;
    struct AMP_AREN_CONFIG pAREN;
    struct AMP_APP_CONFIG pAPP;
    struct AMP_AOUT_CONFIG pAOUT;
    struct AMP_AVIN_CONFIG pAVIN;
    struct AMP_SNDSINK_CONFIG pSNDSINK;
    struct AMP_SNDEXP_CONFIG pSNDEXP;
    struct AMP_DVS_CONFIG pDVS;
    struct AMP_DATA_EXPORTER_CONFIG pDATA_EXPORTER;
    struct AMP_ISP_CONFIG pISP;
    struct AMP_DEWARP_CONFIG pDEWARP;
    struct AMP_ROTATE_CONFIG pROTATE;
    struct AMP_DNS_CONFIG pDNS;
  } _u;
} AMP_COMPONENT_CONFIG;
#endif /* _typedef___AMP_COMPONENT_CONFIG */

HRESULT AMP_COMPONENT_Open(AMP_COMPONENT _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_Close(AMP_COMPONENT _obj, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_Destroy(AMP_COMPONENT _obj, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_SetState(AMP_COMPONENT _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_GetState(AMP_COMPONENT _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_QueryInfo(AMP_COMPONENT _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_SetupPort(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_DynamicSetupPort(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_DisconnectPorts(AMP_COMPONENT _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_ClearPort(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_QueryPort(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_ClearPortBuf(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_DebugDump(AMP_COMPONENT _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_RegisterNotify(AMP_COMPONENT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_UnregisterNotify(AMP_COMPONENT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_RegisterNotifyDetail(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_UnregisterNotifyDetail(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_RegisterBD(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_UnregisterBD(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_COMPONENT_PushBD(AMP_COMPONENT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DUM
#define _typedef___AMP_DUM
typedef CORBA_Object AMP_DUM;
#endif /* _typedef___AMP_DUM */

#ifndef _typedef___AMP_DUM_PLAYBACK_ATTRIBUTE
#define _typedef___AMP_DUM_PLAYBACK_ATTRIBUTE
typedef struct AMP_DUM_PLAYBACK_ATTRIBUTE {
  UINT32 uiPlaybackIntervalTime;
} AMP_DUM_PLAYBACK_ATTRIBUTE;
#endif /* _typedef___AMP_DUM_PLAYBACK_ATTRIBUTE */

HRESULT AMP_DUM_ConfigPlayback(AMP_DUM _obj, struct AMP_DUM_PLAYBACK_ATTRIBUTE *pAttribute, CORBA_Environment *_ev);

HRESULT AMP_DUM_Open(AMP_DUM _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DUM_Close(AMP_DUM _obj, CORBA_Environment *_ev);

HRESULT AMP_DUM_Destroy(AMP_DUM _obj, CORBA_Environment *_ev);

HRESULT AMP_DUM_SetState(AMP_DUM _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DUM_GetState(AMP_DUM _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DUM_QueryInfo(AMP_DUM _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DUM_SetupPort(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DUM_DynamicSetupPort(AMP_DUM _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DUM_DisconnectPorts(AMP_DUM _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DUM_ClearPort(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DUM_QueryPort(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DUM_ClearPortBuf(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DUM_DebugDump(AMP_DUM _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DUM_RegisterNotify(AMP_DUM _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DUM_UnregisterNotify(AMP_DUM _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DUM_RegisterNotifyDetail(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DUM_UnregisterNotifyDetail(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DUM_RegisterBD(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DUM_UnregisterBD(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DUM_PushBD(AMP_DUM _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_CLK_PTS
#define _typedef___AMP_CLK_PTS
typedef struct AMP_CLK_PTS {
  UINT32 m_uiHigh;
  UINT32 m_uiLow;
  UINT32 m_uiRsvd1;
  UINT32 m_uiRsvd2;
} AMP_CLK_PTS;
#endif /* _typedef___AMP_CLK_PTS */

#ifndef _typedef___AMP_CLK
#define _typedef___AMP_CLK
typedef CORBA_Object AMP_CLK;
#endif /* _typedef___AMP_CLK */

#ifndef _define___AMP_CLK_DISP
#define _define___AMP_CLK_DISP
#define AMP_CLK_DISP (0)
#endif

#ifndef _define___AMP_CLK_HOLD
#define _define___AMP_CLK_HOLD
#define AMP_CLK_HOLD (1)
#endif

#ifndef _define___AMP_CLK_DROP
#define _define___AMP_CLK_DROP
#define AMP_CLK_DROP (2)
#endif

#ifndef _define___AMP_CLK_ACT_MAX
#define _define___AMP_CLK_ACT_MAX
#define AMP_CLK_ACT_MAX (3)
#endif

#ifndef _typedef___AMP_CLK_ACT
#define _typedef___AMP_CLK_ACT
typedef CORBA_enum AMP_CLK_ACT;
#endif /* _typedef___AMP_CLK_ACT */

#ifndef _typedef___AMP_CLK_DROP_INFO
#define _typedef___AMP_CLK_DROP_INFO
typedef struct AMP_CLK_DROP_INFO {
  UINT32 m_uiTotalBD;
  UINT32 m_uiBD2Drop;
  UINT32 m_uiSamples2Drop;
} AMP_CLK_DROP_INFO;
#endif /* _typedef___AMP_CLK_DROP_INFO */

#ifndef _typedef___AMP_CLK_STR_SETTINGS
#define _typedef___AMP_CLK_STR_SETTINGS
typedef struct AMP_CLK_STR_SETTINGS {
  UINT32 m_uiFlag;
  BOOL m_fDisabled;
  INT32 m_iDelay;
} AMP_CLK_STR_SETTINGS;
#endif /* _typedef___AMP_CLK_STR_SETTINGS */

#ifndef _define___AMP_CLK_FLAG_STR_SETTING_DISABLE
#define _define___AMP_CLK_FLAG_STR_SETTING_DISABLE
#define AMP_CLK_FLAG_STR_SETTING_DISABLE (1)
#endif

#ifndef _define___AMP_CLK_FLAG_STR_SETTING_DELAY
#define _define___AMP_CLK_FLAG_STR_SETTING_DELAY
#define AMP_CLK_FLAG_STR_SETTING_DELAY (2)
#endif

HRESULT AMP_CLK_GetSTC(AMP_CLK _obj, struct AMP_CLK_PTS *pSTC, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetSTC(AMP_CLK _obj, struct AMP_CLK_PTS *pSTC, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetSTC64(AMP_CLK _obj, AMP_PTS *pSTC, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetSTC64(AMP_CLK _obj, AMP_PTS uiSTC, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetClockRate(AMP_CLK _obj, INT32 *pNum, UINT32 *pDen, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetClockRate(AMP_CLK _obj, INT32 iNum, UINT32 uiDen, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetIsMaster(AMP_CLK _obj, BOOL bIsMaster, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetIsMaster(AMP_CLK _obj, BOOL *pbIsMaster, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetSlewRate(AMP_CLK _obj, INT32 *pSlewRate, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetSlewRate(AMP_CLK _obj, INT32 iSlewRate, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetMaxSlewRate(AMP_CLK _obj, INT32 *pMaxSlewRate, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetAVSyncOption(AMP_CLK _obj, UINT32 uiPortIdx, AMP_CLK_OPT eOpt, UINT32 uiParam1, UINT32 uiParam2, UINT32 uiParam3, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetAVSyncOption(AMP_CLK _obj, UINT32 uiPortIdx, AMP_CLK_OPT *peOpt, UINT32 *puiParam1, UINT32 *puiParam2, UINT32 *puiParam3, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetAVSyncOptionParam(AMP_CLK _obj, UINT32 uiPortIdx, AMP_CLK_OPT eOpt, UINT32 *puiParam1, UINT32 *puiParam2, UINT32 *puiParam3, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetRndStatistics(AMP_CLK _obj, UINT32 uiPortIdx, struct AMP_RND_STAT *pstRndStat, CORBA_Environment *_ev);

HRESULT AMP_CLK_Step(AMP_CLK _obj, UINT32 uiOpt, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetStartPTS(AMP_CLK _obj, UINT32 uiPortIdx, AMP_PTS uiStartPTS, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetStopPTS(AMP_CLK _obj, UINT32 uiPortIdx, AMP_PTS uiStopPTS, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetPausePTS(AMP_CLK _obj, UINT32 uiPortIdx, AMP_PTS uiPausePTS, CORBA_Environment *_ev);

HRESULT AMP_CLK_ForceResync(AMP_CLK _obj, UINT32 uiOpt, CORBA_Environment *_ev);

HRESULT AMP_CLK_Open(AMP_CLK _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_CLK_Close(AMP_CLK _obj, CORBA_Environment *_ev);

HRESULT AMP_CLK_Destroy(AMP_CLK _obj, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetState(AMP_CLK _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_CLK_GetState(AMP_CLK _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_CLK_QueryInfo(AMP_CLK _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_CLK_SetupPort(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_CLK_DynamicSetupPort(AMP_CLK _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_CLK_DisconnectPorts(AMP_CLK _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_CLK_ClearPort(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_CLK_QueryPort(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_CLK_ClearPortBuf(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_CLK_DebugDump(AMP_CLK _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_CLK_RegisterNotify(AMP_CLK _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_CLK_UnregisterNotify(AMP_CLK _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_CLK_RegisterNotifyDetail(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_CLK_UnregisterNotifyDetail(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_CLK_RegisterBD(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_CLK_UnregisterBD(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_CLK_PushBD(AMP_CLK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DMX
#define _typedef___AMP_DMX
typedef CORBA_Object AMP_DMX;
#endif /* _typedef___AMP_DMX */

#ifndef _define___AMP_DMX_ANY_PORT_IDX
#define _define___AMP_DMX_ANY_PORT_IDX
#define AMP_DMX_ANY_PORT_IDX (268435456)
#endif

#ifndef _typedef___AMP_DMX_TAG
#define _typedef___AMP_DMX_TAG
typedef UINT32 AMP_DMX_TAG;
#endif /* _typedef___AMP_DMX_TAG */

#ifndef _typedef___AMP_DMX_BUFFER_HANDLE
#define _typedef___AMP_DMX_BUFFER_HANDLE
typedef UINT32 AMP_DMX_BUFFER_HANDLE;
#endif /* _typedef___AMP_DMX_BUFFER_HANDLE */

#ifndef _typedef___AMP_DMX_HANDLE
#define _typedef___AMP_DMX_HANDLE
typedef UINT32 AMP_DMX_HANDLE;
#endif /* _typedef___AMP_DMX_HANDLE */

#ifndef _typedef___AMP_DMX_INPUT_ST
#define _typedef___AMP_DMX_INPUT_ST
typedef struct AMP_DMX_INPUT_ST {
  AMP_DMX_HANDLE hInput;
  UINT32 uiPortIdx;
} AMP_DMX_INPUT_ST;
#endif /* _typedef___AMP_DMX_INPUT_ST */

#ifndef _typedef___AMP_DMX_CHNL_ST
#define _typedef___AMP_DMX_CHNL_ST
typedef struct AMP_DMX_CHNL_ST {
  AMP_DMX_HANDLE hChnl;
  UINT32 uiPortIdx;
} AMP_DMX_CHNL_ST;
#endif /* _typedef___AMP_DMX_CHNL_ST */

#ifndef _typedef___AMP_DMX_FLT_ST
#define _typedef___AMP_DMX_FLT_ST
typedef struct AMP_DMX_FLT_ST {
  AMP_DMX_HANDLE hFilter;
  UINT32 uiPortIdx;
} AMP_DMX_FLT_ST;
#endif /* _typedef___AMP_DMX_FLT_ST */

#ifndef _typedef___AMP_DMX_DSS_FLT_ST
#define _typedef___AMP_DMX_DSS_FLT_ST
typedef struct AMP_DMX_DSS_FLT_ST {
  AMP_DMX_HANDLE hFilter;
  UINT32 uiPortIdx;
} AMP_DMX_DSS_FLT_ST;
#endif /* _typedef___AMP_DMX_DSS_FLT_ST */

#ifndef _typedef___AMP_DMX_CA_PID_ST
#define _typedef___AMP_DMX_CA_PID_ST
typedef struct AMP_DMX_CA_PID_ST {
  AMP_DMX_HANDLE hDmx;
  AMP_DMX_HANDLE hCaPid;
} AMP_DMX_CA_PID_ST;
#endif /* _typedef___AMP_DMX_CA_PID_ST */

#ifndef _typedef___AMP_DMX_DBGCFG
#define _typedef___AMP_DMX_DBGCFG
typedef struct AMP_DMX_DBGCFG {
  UINT32 uiLogLevel;
  UINT32 fpDump;
} AMP_DMX_DBGCFG;
#endif /* _typedef___AMP_DMX_DBGCFG */

#ifndef _define___AMP_DMX_INPUT_TS_MEM
#define _define___AMP_DMX_INPUT_TS_MEM
#define AMP_DMX_INPUT_TS_MEM (0)
#endif

#ifndef _define___AMP_DMX_INPUT_TS_FE
#define _define___AMP_DMX_INPUT_TS_FE
#define AMP_DMX_INPUT_TS_FE (1)
#endif

#ifndef _define___AMP_DMX_INPUT_ES
#define _define___AMP_DMX_INPUT_ES
#define AMP_DMX_INPUT_ES (2)
#endif

#ifndef _define___AMP_DMX_INPUT_RAW
#define _define___AMP_DMX_INPUT_RAW
#define AMP_DMX_INPUT_RAW (3)
#endif

#ifndef _define___AMP_DMX_INPUT_DSS_MEM
#define _define___AMP_DMX_INPUT_DSS_MEM
#define AMP_DMX_INPUT_DSS_MEM (4)
#endif

#ifndef _define___AMP_DMX_INPUT_DSS_FE
#define _define___AMP_DMX_INPUT_DSS_FE
#define AMP_DMX_INPUT_DSS_FE (5)
#endif

#ifndef _define___AMP_DMX_INPUT_MAX
#define _define___AMP_DMX_INPUT_MAX
#define AMP_DMX_INPUT_MAX (6)
#endif

#ifndef _typedef___AMP_DMX_INPUT_TYPE
#define _typedef___AMP_DMX_INPUT_TYPE
typedef CORBA_enum AMP_DMX_INPUT_TYPE;
#endif /* _typedef___AMP_DMX_INPUT_TYPE */

#ifndef _define___AMP_DMX_CRYPTO_SCHEME_TYPE_NONE
#define _define___AMP_DMX_CRYPTO_SCHEME_TYPE_NONE
#define AMP_DMX_CRYPTO_SCHEME_TYPE_NONE (0)
#endif

#ifndef _define___AMP_DMX_AES_128_CBC_CLR_CLR
#define _define___AMP_DMX_AES_128_CBC_CLR_CLR
#define AMP_DMX_AES_128_CBC_CLR_CLR (1)
#endif

#ifndef _define___AMP_DMX_AES_128_CTR_NO_NO
#define _define___AMP_DMX_AES_128_CTR_NO_NO
#define AMP_DMX_AES_128_CTR_NO_NO (2)
#endif

#ifndef _define___AMP_DMX_AES_128_CBC_CTS_CLR
#define _define___AMP_DMX_AES_128_CBC_CTS_CLR
#define AMP_DMX_AES_128_CBC_CTS_CLR (3)
#endif

#ifndef _define___AMP_DMX_AES_128_CBC_RP_CLR
#define _define___AMP_DMX_AES_128_CBC_RP_CLR
#define AMP_DMX_AES_128_CBC_RP_CLR (4)
#endif

#ifndef _define___AMP_DMX_AES_128_CBC_EXOR_EXOR
#define _define___AMP_DMX_AES_128_CBC_EXOR_EXOR
#define AMP_DMX_AES_128_CBC_EXOR_EXOR (5)
#endif

#ifndef _define___AMP_DMX_AES_128_CBC_NO_NO
#define _define___AMP_DMX_AES_128_CBC_NO_NO
#define AMP_DMX_AES_128_CBC_NO_NO (6)
#endif

#ifndef _define___AMP_DMX_AES_128_CBC_NO_CLR
#define _define___AMP_DMX_AES_128_CBC_NO_CLR
#define AMP_DMX_AES_128_CBC_NO_CLR (7)
#endif

#ifndef _define___AMP_DMX_DVB_CSA_NO_NO
#define _define___AMP_DMX_DVB_CSA_NO_NO
#define AMP_DMX_DVB_CSA_NO_NO (8)
#endif

#ifndef _define___AMP_DMX_AES_128_ECB_CLR_CLR
#define _define___AMP_DMX_AES_128_ECB_CLR_CLR
#define AMP_DMX_AES_128_ECB_CLR_CLR (9)
#endif

#ifndef _define___AMP_DMX_DES_56_ECB_CLR_CLR
#define _define___AMP_DMX_DES_56_ECB_CLR_CLR
#define AMP_DMX_DES_56_ECB_CLR_CLR (10)
#endif

#ifndef _define___AMP_DMX_TDES_128_CBC_EXOR_EXOR
#define _define___AMP_DMX_TDES_128_CBC_EXOR_EXOR
#define AMP_DMX_TDES_128_CBC_EXOR_EXOR (11)
#endif

#ifndef _define___AMP_DMX_TDES_128_ECB_CLR_CLR
#define _define___AMP_DMX_TDES_128_ECB_CLR_CLR
#define AMP_DMX_TDES_128_ECB_CLR_CLR (12)
#endif

#ifndef _define___AMP_DMX_DVB_CSA3_NO_NO
#define _define___AMP_DMX_DVB_CSA3_NO_NO
#define AMP_DMX_DVB_CSA3_NO_NO (13)
#endif

#ifndef _define___AMP_DMX_ASA_NO_NO
#define _define___AMP_DMX_ASA_NO_NO
#define AMP_DMX_ASA_NO_NO (14)
#endif

#ifndef _typedef___AMP_DMX_CRYPTO_SCHEME_TYPE
#define _typedef___AMP_DMX_CRYPTO_SCHEME_TYPE
typedef CORBA_enum AMP_DMX_CRYPTO_SCHEME_TYPE;
#endif /* _typedef___AMP_DMX_CRYPTO_SCHEME_TYPE */

#ifndef _define___AMP_DMX_AES_128_CBC_RBT_CLR
#define _define___AMP_DMX_AES_128_CBC_RBT_CLR
#define AMP_DMX_AES_128_CBC_RBT_CLR (5)
#endif

#ifndef _define___AMP_DMX_CRYPTO_IV_FIXED
#define _define___AMP_DMX_CRYPTO_IV_FIXED
#define AMP_DMX_CRYPTO_IV_FIXED (0)
#endif

#ifndef _define___AMP_DMX_CRYPTO_IV_UNSET
#define _define___AMP_DMX_CRYPTO_IV_UNSET
#define AMP_DMX_CRYPTO_IV_UNSET (1)
#endif

#ifndef _typedef___AMP_DMX_CRYPTO_IV_MODE
#define _typedef___AMP_DMX_CRYPTO_IV_MODE
typedef CORBA_enum AMP_DMX_CRYPTO_IV_MODE;
#endif /* _typedef___AMP_DMX_CRYPTO_IV_MODE */

#ifndef _define___AMP_DMX_CRYPTO_WHOLE_PACKET
#define _define___AMP_DMX_CRYPTO_WHOLE_PACKET
#define AMP_DMX_CRYPTO_WHOLE_PACKET (0)
#endif

#ifndef _define___AMP_DMX_CRYPTO_TS_PAYLOAD
#define _define___AMP_DMX_CRYPTO_TS_PAYLOAD
#define AMP_DMX_CRYPTO_TS_PAYLOAD (1)
#endif

#ifndef _typedef___AMP_DMX_CRYPTO_LEVEL
#define _typedef___AMP_DMX_CRYPTO_LEVEL
typedef CORBA_enum AMP_DMX_CRYPTO_LEVEL;
#endif /* _typedef___AMP_DMX_CRYPTO_LEVEL */

#ifndef _typedef___AMP_DMX_CRYPTO_CFG
#define _typedef___AMP_DMX_CRYPTO_CFG
typedef struct AMP_DMX_CRYPTO_CFG {
  AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType;
  AMP_DMX_CRYPTO_LEVEL Level;
} AMP_DMX_CRYPTO_CFG;
#endif /* _typedef___AMP_DMX_CRYPTO_CFG */

#ifndef _define___AMP_DMX_TS_PACKET_SIZE_188
#define _define___AMP_DMX_TS_PACKET_SIZE_188
#define AMP_DMX_TS_PACKET_SIZE_188 (0)
#endif

#ifndef _define___AMP_DMX_TS_PACKET_SIZE_192
#define _define___AMP_DMX_TS_PACKET_SIZE_192
#define AMP_DMX_TS_PACKET_SIZE_192 (1)
#endif

#ifndef _define___AMP_DMX_DSS_PACKET_SIZE_130
#define _define___AMP_DMX_DSS_PACKET_SIZE_130
#define AMP_DMX_DSS_PACKET_SIZE_130 (2)
#endif

#ifndef _typedef___AMP_DMX_TS_PKTSZ
#define _typedef___AMP_DMX_TS_PKTSZ
typedef CORBA_enum AMP_DMX_TS_PKTSZ;
#endif /* _typedef___AMP_DMX_TS_PKTSZ */

#ifndef _typedef___AMP_DMX_INPUT_CFG
#define _typedef___AMP_DMX_INPUT_CFG
typedef struct AMP_DMX_INPUT_CFG {
  AMP_DMX_INPUT_TYPE InputType;
  struct AMP_DMX_CRYPTO_CFG CyptCfg;
  AMP_DMX_BUFFER_HANDLE hInBuf;
  UINT32 uiBufSize;
  UINT32 uiBufPaddingSize;
  UINT32 EventMask;
  AMP_DMX_TS_PKTSZ TsPktSz;
  struct AMP_DRM_CRYPTO_ST CryptInfo;
  UINT32 uiBufHighWaterMask;
  UINT32 uiBufLowWaterMask;
} AMP_DMX_INPUT_CFG;
#endif /* _typedef___AMP_DMX_INPUT_CFG */

#ifndef _typedef___AMP_DMX_INPUT_PROP_MASK
#define _typedef___AMP_DMX_INPUT_PROP_MASK
typedef UINT32 AMP_DMX_INPUT_PROP_MASK;
#endif /* _typedef___AMP_DMX_INPUT_PROP_MASK */

#ifndef _define___AMP_DMX_INPUT_PROP_BUFF_MASK
#define _define___AMP_DMX_INPUT_PROP_BUFF_MASK
#define AMP_DMX_INPUT_PROP_BUFF_MASK (1)
#endif

#ifndef _define___AMP_DMX_INPUT_PROP_EVENTMASK_MASK
#define _define___AMP_DMX_INPUT_PROP_EVENTMASK_MASK
#define AMP_DMX_INPUT_PROP_EVENTMASK_MASK (2)
#endif

#ifndef _define___AMP_DMX_INPUT_PROP_CRYPTOCFG_MASK
#define _define___AMP_DMX_INPUT_PROP_CRYPTOCFG_MASK
#define AMP_DMX_INPUT_PROP_CRYPTOCFG_MASK (4)
#endif

#ifndef _define___AMP_DMX_INPUT_PROP_CRYPTOINFO_MASK
#define _define___AMP_DMX_INPUT_PROP_CRYPTOINFO_MASK
#define AMP_DMX_INPUT_PROP_CRYPTOINFO_MASK (8)
#endif

#ifndef _define___AMP_DMX_INPUT_PROP_HIGHWATER_MASK
#define _define___AMP_DMX_INPUT_PROP_HIGHWATER_MASK
#define AMP_DMX_INPUT_PROP_HIGHWATER_MASK (16)
#endif

#ifndef _define___AMP_DMX_INPUT_PROP_LOWWATER_MASK
#define _define___AMP_DMX_INPUT_PROP_LOWWATER_MASK
#define AMP_DMX_INPUT_PROP_LOWWATER_MASK (32)
#endif

#ifndef _typedef___AMP_DMX_INPUT_PROP
#define _typedef___AMP_DMX_INPUT_PROP
typedef struct AMP_DMX_INPUT_PROP {
  AMP_DMX_INPUT_PROP_MASK uPropMask;
  struct AMP_DMX_INPUT_CFG Property;
} AMP_DMX_INPUT_PROP;
#endif /* _typedef___AMP_DMX_INPUT_PROP */

#ifndef _define___AMP_DMX_OUTPUT_INV
#define _define___AMP_DMX_OUTPUT_INV
#define AMP_DMX_OUTPUT_INV (0)
#endif

#ifndef _define___AMP_DMX_OUTPUT_VES
#define _define___AMP_DMX_OUTPUT_VES
#define AMP_DMX_OUTPUT_VES (1)
#endif

#ifndef _define___AMP_DMX_OUTPUT_AES
#define _define___AMP_DMX_OUTPUT_AES
#define AMP_DMX_OUTPUT_AES (2)
#endif

#ifndef _define___AMP_DMX_OUTPUT_PES
#define _define___AMP_DMX_OUTPUT_PES
#define AMP_DMX_OUTPUT_PES (3)
#endif

#ifndef _define___AMP_DMX_OUTPUT_PCR
#define _define___AMP_DMX_OUTPUT_PCR
#define AMP_DMX_OUTPUT_PCR (4)
#endif

#ifndef _define___AMP_DMX_OUTPUT_TS
#define _define___AMP_DMX_OUTPUT_TS
#define AMP_DMX_OUTPUT_TS (5)
#endif

#ifndef _define___AMP_DMX_OUTPUT_SEC
#define _define___AMP_DMX_OUTPUT_SEC
#define AMP_DMX_OUTPUT_SEC (6)
#endif

#ifndef _define___AMP_DMX_OUTPUT_SCRAMDETECT
#define _define___AMP_DMX_OUTPUT_SCRAMDETECT
#define AMP_DMX_OUTPUT_SCRAMDETECT (7)
#endif

#ifndef _define___AMP_DMX_OUTPUT_RAW
#define _define___AMP_DMX_OUTPUT_RAW
#define AMP_DMX_OUTPUT_RAW (8)
#endif

#ifndef _define___AMP_DMX_OUTPUT_DSS
#define _define___AMP_DMX_OUTPUT_DSS
#define AMP_DMX_OUTPUT_DSS (9)
#endif

#ifndef _define___AMP_DMX_OUTPUT_MAX
#define _define___AMP_DMX_OUTPUT_MAX
#define AMP_DMX_OUTPUT_MAX (10)
#endif

#ifndef _typedef___AMP_DMX_OUT_TYPE
#define _typedef___AMP_DMX_OUT_TYPE
typedef CORBA_enum AMP_DMX_OUT_TYPE;
#endif /* _typedef___AMP_DMX_OUT_TYPE */

#ifndef _define___AMP_DMX_KEY_INVALID
#define _define___AMP_DMX_KEY_INVALID
#define AMP_DMX_KEY_INVALID (0)
#endif

#ifndef _define___AMP_DMX_KEY_EVEN
#define _define___AMP_DMX_KEY_EVEN
#define AMP_DMX_KEY_EVEN (1)
#endif

#ifndef _define___AMP_DMX_KEY_ODD
#define _define___AMP_DMX_KEY_ODD
#define AMP_DMX_KEY_ODD (2)
#endif

#ifndef _define___AMP_DMX_KEY_MAX
#define _define___AMP_DMX_KEY_MAX
#define AMP_DMX_KEY_MAX (3)
#endif

#ifndef _typedef___AMP_DMX_KEY_TYPE
#define _typedef___AMP_DMX_KEY_TYPE
typedef CORBA_enum AMP_DMX_KEY_TYPE;
#endif /* _typedef___AMP_DMX_KEY_TYPE */

#ifndef _define___AMP_DMX_KEY_OPTION_INVALID
#define _define___AMP_DMX_KEY_OPTION_INVALID
#define AMP_DMX_KEY_OPTION_INVALID (0)
#endif

#ifndef _define___AMP_DMX_KEY_CONTENT
#define _define___AMP_DMX_KEY_CONTENT
#define AMP_DMX_KEY_CONTENT (1)
#endif

#ifndef _define___AMP_DMX_KEY_TBLIDX
#define _define___AMP_DMX_KEY_TBLIDX
#define AMP_DMX_KEY_TBLIDX (2)
#endif

#ifndef _define___AMP_DMX_KEY_OPTION_MAX
#define _define___AMP_DMX_KEY_OPTION_MAX
#define AMP_DMX_KEY_OPTION_MAX (3)
#endif

#ifndef _typedef___AMP_DMX_KEY_OPTION
#define _typedef___AMP_DMX_KEY_OPTION
typedef CORBA_enum AMP_DMX_KEY_OPTION;
#endif /* _typedef___AMP_DMX_KEY_OPTION */

#ifndef _define___AMP_DMX_NOT_UPDATE_IV
#define _define___AMP_DMX_NOT_UPDATE_IV
#define AMP_DMX_NOT_UPDATE_IV (0)
#endif

#ifndef _define___AMP_DMX_UPDATE_IV
#define _define___AMP_DMX_UPDATE_IV
#define AMP_DMX_UPDATE_IV (1)
#endif

#ifndef _typedef___AMP_DMX_IV_UPDATE_FLAG
#define _typedef___AMP_DMX_IV_UPDATE_FLAG
typedef CORBA_enum AMP_DMX_IV_UPDATE_FLAG;
#endif /* _typedef___AMP_DMX_IV_UPDATE_FLAG */

#ifndef _typedef___AMP_DMX_SCRAM_CTX
#define _typedef___AMP_DMX_SCRAM_CTX
typedef struct AMP_DMX_SCRAM_CTX {
  AMP_DMX_KEY_TYPE KeyType;
  AMP_DMX_KEY_OPTION KeyOption;
  AMP_DMX_KEY_OPTION IVOption;
  AMP_DMX_IV_UPDATE_FLAG UpdateIVFlag;
  UINT32 KeyLength;
  UINT8 KeyData[32];
  UINT8 IVData[32];
  UINT16 KeyTblIndex;
  UINT16 IVTblIndex;
} AMP_DMX_SCRAM_CTX;
#endif /* _typedef___AMP_DMX_SCRAM_CTX */

#ifndef _define___AMP_DMX_VID_ES_ALIGN_NON
#define _define___AMP_DMX_VID_ES_ALIGN_NON
#define AMP_DMX_VID_ES_ALIGN_NON (0)
#endif

#ifndef _define___AMP_DMX_VID_ES_ALIGN_2K
#define _define___AMP_DMX_VID_ES_ALIGN_2K
#define AMP_DMX_VID_ES_ALIGN_2K (1)
#endif

#ifndef _define___AMP_DMX_VID_ES_ALIGN_4K
#define _define___AMP_DMX_VID_ES_ALIGN_4K
#define AMP_DMX_VID_ES_ALIGN_4K (2)
#endif

#ifndef _define___AMP_DMX_VID_ES_ALIGN_8K
#define _define___AMP_DMX_VID_ES_ALIGN_8K
#define AMP_DMX_VID_ES_ALIGN_8K (3)
#endif

#ifndef _define___AMP_DMX_VID_ES_ALIGN_16K
#define _define___AMP_DMX_VID_ES_ALIGN_16K
#define AMP_DMX_VID_ES_ALIGN_16K (4)
#endif

#ifndef _define___AMP_DMX_VID_ES_ALIGN_32K
#define _define___AMP_DMX_VID_ES_ALIGN_32K
#define AMP_DMX_VID_ES_ALIGN_32K (5)
#endif

#ifndef _typedef___AMP_DMX_VID_ES_ALIGN
#define _typedef___AMP_DMX_VID_ES_ALIGN
typedef CORBA_enum AMP_DMX_VID_ES_ALIGN;
#endif /* _typedef___AMP_DMX_VID_ES_ALIGN */

#ifndef _define___AMP_DMX_INDEX_MEDIA_TYPE_INVALID
#define _define___AMP_DMX_INDEX_MEDIA_TYPE_INVALID
#define AMP_DMX_INDEX_MEDIA_TYPE_INVALID (0)
#endif

#ifndef _define___AMP_DMX_INDEX_MEDIA_TYPE_VID_MPEG2
#define _define___AMP_DMX_INDEX_MEDIA_TYPE_VID_MPEG2
#define AMP_DMX_INDEX_MEDIA_TYPE_VID_MPEG2 (1)
#endif

#ifndef _define___AMP_DMX_INDEX_MEDIA_TYPE_VID_H264
#define _define___AMP_DMX_INDEX_MEDIA_TYPE_VID_H264
#define AMP_DMX_INDEX_MEDIA_TYPE_VID_H264 (2)
#endif

#ifndef _define___AMP_DMX_INDEX_MEDIA_TYPE_VID_H265
#define _define___AMP_DMX_INDEX_MEDIA_TYPE_VID_H265
#define AMP_DMX_INDEX_MEDIA_TYPE_VID_H265 (3)
#endif

#ifndef _typedef___AMP_DMX_INDEX_MEDIA_TYPE
#define _typedef___AMP_DMX_INDEX_MEDIA_TYPE
typedef CORBA_enum AMP_DMX_INDEX_MEDIA_TYPE;
#endif /* _typedef___AMP_DMX_INDEX_MEDIA_TYPE */

#ifndef _define___AMP_DMX_RECORDING_OUTPUT_MODE_INVALID
#define _define___AMP_DMX_RECORDING_OUTPUT_MODE_INVALID
#define AMP_DMX_RECORDING_OUTPUT_MODE_INVALID (0)
#endif

#ifndef _define___AMP_DMX_RECORDING_OUTPUT_MODE_DYNAMIC_BD
#define _define___AMP_DMX_RECORDING_OUTPUT_MODE_DYNAMIC_BD
#define AMP_DMX_RECORDING_OUTPUT_MODE_DYNAMIC_BD (1)
#endif

#ifndef _define___AMP_DMX_RECORDING_OUTPUT_MODE_FIXED_BD
#define _define___AMP_DMX_RECORDING_OUTPUT_MODE_FIXED_BD
#define AMP_DMX_RECORDING_OUTPUT_MODE_FIXED_BD (2)
#endif

#ifndef _typedef___AMP_DMX_RECORDING_OUTPUT_MODE
#define _typedef___AMP_DMX_RECORDING_OUTPUT_MODE
typedef CORBA_enum AMP_DMX_RECORDING_OUTPUT_MODE;
#endif /* _typedef___AMP_DMX_RECORDING_OUTPUT_MODE */

#ifndef _typedef___AMP_DMX_RECORDING_CFG
#define _typedef___AMP_DMX_RECORDING_CFG
typedef struct AMP_DMX_RECORDING_CFG {
  BOOL isIndex;
  AMP_DMX_INDEX_MEDIA_TYPE MediaType;
  AMP_DMX_RECORDING_OUTPUT_MODE OutMode;
  UINT32 RecBufSize;
  UINT32 BDNum;
  UINT32 DataSizePerBD;
} AMP_DMX_RECORDING_CFG;
#endif /* _typedef___AMP_DMX_RECORDING_CFG */

#ifndef _define___AMP_DMX_ES_OUTPUT_MODE_DEFAULT
#define _define___AMP_DMX_ES_OUTPUT_MODE_DEFAULT
#define AMP_DMX_ES_OUTPUT_MODE_DEFAULT (0)
#endif

#ifndef _define___AMP_DMX_ES_OUTPUT_MODE_UNITSTART_ALIGN
#define _define___AMP_DMX_ES_OUTPUT_MODE_UNITSTART_ALIGN
#define AMP_DMX_ES_OUTPUT_MODE_UNITSTART_ALIGN (1)
#endif

#ifndef _define___AMP_DMX_ES_OUTPUT_MODE_ES_INPUT_ALIGN
#define _define___AMP_DMX_ES_OUTPUT_MODE_ES_INPUT_ALIGN
#define AMP_DMX_ES_OUTPUT_MODE_ES_INPUT_ALIGN (2)
#endif

#ifndef _typedef___AMP_DMX_ES_OUTPUT_MODE
#define _typedef___AMP_DMX_ES_OUTPUT_MODE
typedef CORBA_enum AMP_DMX_ES_OUTPUT_MODE;
#endif /* _typedef___AMP_DMX_ES_OUTPUT_MODE */

#ifndef _typedef___AMP_DMX_CH_CFG
#define _typedef___AMP_DMX_CH_CFG
typedef struct AMP_DMX_CH_CFG {
  AMP_DMX_OUT_TYPE eType;
  AMP_DMX_TAG Tag;
  UINT32 uPid;
  AMP_DMX_VID_ES_ALIGN uAlign;
  UINT32 EventMask;
  AMP_DMX_BUFFER_HANDLE hStrmBuf;
  UINT32 uBufSize;
  UINT32 uBufPaddingSize;
  struct AMP_DMX_SCRAM_CTX DescramCtx;
  AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType;
  struct AMP_DRM_CRYPTO_ST CryptInfo;
  struct AMP_DMX_SCRAM_CTX ScramCtx;
  AMP_DMX_CRYPTO_SCHEME_TYPE ScramSchemeType;
  struct AMP_DRM_CRYPTO_ST ScramCryptInfo;
  struct AMP_DMX_RECORDING_CFG RecordCfg;
  UINT32 ScramNodifyInterval;
  UINT32 uiBufHighWaterMask;
  UINT32 uiBufLowWaterMask;
  UINT32 PesNumInOneBD;
  UINT32 DiscontNodifyInterval;
  UINT32 isCSA1;
  AMP_DMX_ES_OUTPUT_MODE eESOutputMode;
  UINT32 uFullness;
  UINT32 uSpace;
  UINT32 uSize;
} AMP_DMX_CH_CFG;
#endif /* _typedef___AMP_DMX_CH_CFG */

#ifndef _typedef___AMP_DMX_CH_PROP_MASK
#define _typedef___AMP_DMX_CH_PROP_MASK
typedef UINT32 AMP_DMX_CH_PROP_MASK;
#endif /* _typedef___AMP_DMX_CH_PROP_MASK */

#ifndef _define___AMP_DMX_CH_PROP_PID_MASK
#define _define___AMP_DMX_CH_PROP_PID_MASK
#define AMP_DMX_CH_PROP_PID_MASK (1)
#endif

#ifndef _define___AMP_DMX_CH_PROP_ALIGN_MASK
#define _define___AMP_DMX_CH_PROP_ALIGN_MASK
#define AMP_DMX_CH_PROP_ALIGN_MASK (2)
#endif

#ifndef _define___AMP_DMX_CH_PROP_EVENTMASK_MASK
#define _define___AMP_DMX_CH_PROP_EVENTMASK_MASK
#define AMP_DMX_CH_PROP_EVENTMASK_MASK (4)
#endif

#ifndef _define___AMP_DMX_CH_PROP_DESCRAM_MASK
#define _define___AMP_DMX_CH_PROP_DESCRAM_MASK
#define AMP_DMX_CH_PROP_DESCRAM_MASK (8)
#endif

#ifndef _define___AMP_DMX_CH_PROP_SCRAM_MASK
#define _define___AMP_DMX_CH_PROP_SCRAM_MASK
#define AMP_DMX_CH_PROP_SCRAM_MASK (16)
#endif

#ifndef _define___AMP_DMX_CH_PROP_GETVESBUFINFO_MASK
#define _define___AMP_DMX_CH_PROP_GETVESBUFINFO_MASK
#define AMP_DMX_CH_PROP_GETVESBUFINFO_MASK (32)
#endif

#ifndef _define___AMP_DMX_CH_PROP_CRYPTOSCHEME_MASK
#define _define___AMP_DMX_CH_PROP_CRYPTOSCHEME_MASK
#define AMP_DMX_CH_PROP_CRYPTOSCHEME_MASK (64)
#endif

#ifndef _define___AMP_DMX_CH_PROP_SCRAMNODIFYTIMEOUT_MASK
#define _define___AMP_DMX_CH_PROP_SCRAMNODIFYTIMEOUT_MASK
#define AMP_DMX_CH_PROP_SCRAMNODIFYTIMEOUT_MASK (128)
#endif

#ifndef _define___AMP_DMX_CH_PROP_HIGHWATER_MASK
#define _define___AMP_DMX_CH_PROP_HIGHWATER_MASK
#define AMP_DMX_CH_PROP_HIGHWATER_MASK (256)
#endif

#ifndef _define___AMP_DMX_CH_PROP_LOWWATER_MASK
#define _define___AMP_DMX_CH_PROP_LOWWATER_MASK
#define AMP_DMX_CH_PROP_LOWWATER_MASK (512)
#endif

#ifndef _define___AMP_DMX_CH_PROP_PESNUMINONEBD_MASK
#define _define___AMP_DMX_CH_PROP_PESNUMINONEBD_MASK
#define AMP_DMX_CH_PROP_PESNUMINONEBD_MASK (1024)
#endif

#ifndef _define___AMP_DMX_CH_PROP_DISCOUNTNODIFYTIMEOUT_MASK
#define _define___AMP_DMX_CH_PROP_DISCOUNTNODIFYTIMEOUT_MASK
#define AMP_DMX_CH_PROP_DISCOUNTNODIFYTIMEOUT_MASK (2048)
#endif

#ifndef _define___AMP_DMX_CH_PROP_DMXPTSOUT_MASK
#define _define___AMP_DMX_CH_PROP_DMXPTSOUT_MASK
#define AMP_DMX_CH_PROP_DMXPTSOUT_MASK (4096)
#endif

#ifndef _define___AMP_DMX_CH_PROP_CONFORMCSA1_MASK
#define _define___AMP_DMX_CH_PROP_CONFORMCSA1_MASK
#define AMP_DMX_CH_PROP_CONFORMCSA1_MASK (8192)
#endif

#ifndef _define___AMP_DMX_CH_PROP_UPDATEBUFF_MASK
#define _define___AMP_DMX_CH_PROP_UPDATEBUFF_MASK
#define AMP_DMX_CH_PROP_UPDATEBUFF_MASK (16384)
#endif

#ifndef _define___AMP_DMX_CH_PROP_GETBUFFULLNESS_MASK
#define _define___AMP_DMX_CH_PROP_GETBUFFULLNESS_MASK
#define AMP_DMX_CH_PROP_GETBUFFULLNESS_MASK (32768)
#endif

#ifndef _typedef___AMP_DMX_CH_PROP
#define _typedef___AMP_DMX_CH_PROP
typedef struct AMP_DMX_CH_PROP {
  AMP_DMX_CH_PROP_MASK uPropMask;
  struct AMP_DMX_CH_CFG Property;
} AMP_DMX_CH_PROP;
#endif /* _typedef___AMP_DMX_CH_PROP */

#ifndef _define___AMP_DMX_CH_CTRL_OFF
#define _define___AMP_DMX_CH_CTRL_OFF
#define AMP_DMX_CH_CTRL_OFF (0)
#endif

#ifndef _define___AMP_DMX_CH_CTRL_ON
#define _define___AMP_DMX_CH_CTRL_ON
#define AMP_DMX_CH_CTRL_ON (1)
#endif

#ifndef _define___AMP_DMX_CH_CTRL_MAX
#define _define___AMP_DMX_CH_CTRL_MAX
#define AMP_DMX_CH_CTRL_MAX (2)
#endif

#ifndef _typedef___AMP_DMX_CH_CTRL
#define _typedef___AMP_DMX_CH_CTRL
typedef CORBA_enum AMP_DMX_CH_CTRL;
#endif /* _typedef___AMP_DMX_CH_CTRL */

#ifndef _define___AMP_DMX_MERGED_CONTINUITY_MODE
#define _define___AMP_DMX_MERGED_CONTINUITY_MODE
#define AMP_DMX_MERGED_CONTINUITY_MODE (0)
#endif

#ifndef _define___AMP_DMX_SEPARATE_CONTINUITY_MODE
#define _define___AMP_DMX_SEPARATE_CONTINUITY_MODE
#define AMP_DMX_SEPARATE_CONTINUITY_MODE (1)
#endif

#ifndef _typedef___AMP_DMX_MERGE_MODE
#define _typedef___AMP_DMX_MERGE_MODE
typedef CORBA_enum AMP_DMX_MERGE_MODE;
#endif /* _typedef___AMP_DMX_MERGE_MODE */

#ifndef _define___AMP_DMX_CONTINUOUS
#define _define___AMP_DMX_CONTINUOUS
#define AMP_DMX_CONTINUOUS (0)
#endif

#ifndef _define___AMP_DMX_ONESHOT
#define _define___AMP_DMX_ONESHOT
#define AMP_DMX_ONESHOT (1)
#endif

#ifndef _typedef___AMP_DMX_FLT_MODE
#define _typedef___AMP_DMX_FLT_MODE
typedef CORBA_enum AMP_DMX_FLT_MODE;
#endif /* _typedef___AMP_DMX_FLT_MODE */

#ifndef _define___AMP_DMX_IGNORE
#define _define___AMP_DMX_IGNORE
#define AMP_DMX_IGNORE (0)
#endif

#ifndef _define___AMP_DMX_CHECK_BUT_ACCEPT
#define _define___AMP_DMX_CHECK_BUT_ACCEPT
#define AMP_DMX_CHECK_BUT_ACCEPT (1)
#endif

#ifndef _typedef___AMP_DMX_FLT_CRC_MODE
#define _typedef___AMP_DMX_FLT_CRC_MODE
typedef CORBA_enum AMP_DMX_FLT_CRC_MODE;
#endif /* _typedef___AMP_DMX_FLT_CRC_MODE */

#ifndef _define___AMP_DMX_SECFILT_DEPTH
#define _define___AMP_DMX_SECFILT_DEPTH
#define AMP_DMX_SECFILT_DEPTH (16)
#endif

#ifndef _define___AMP_DMX_TABLE_ID_FLT_NBR
#define _define___AMP_DMX_TABLE_ID_FLT_NBR
#define AMP_DMX_TABLE_ID_FLT_NBR (16)
#endif

#ifndef _define___AMP_DMX_EMM_ADDR_NBR
#define _define___AMP_DMX_EMM_ADDR_NBR
#define AMP_DMX_EMM_ADDR_NBR (4)
#endif

#ifndef _define___AMP_DMX_EMM_ADDR_FLT_NBR
#define _define___AMP_DMX_EMM_ADDR_FLT_NBR
#define AMP_DMX_EMM_ADDR_FLT_NBR (8)
#endif

#ifndef _typedef___AMP_DMX_SEC_PATTERN
#define _typedef___AMP_DMX_SEC_PATTERN
typedef struct AMP_DMX_SEC_PATTERN {
  UINT8 Range[16];
  UINT8 Coef[16];
  UINT8 Neg[16];
  UINT8 Mask[16];
} AMP_DMX_SEC_PATTERN;
#endif /* _typedef___AMP_DMX_SEC_PATTERN */

#ifndef _typedef___AMP_DMX_EMM_ADDR_FLT_PATTERN
#define _typedef___AMP_DMX_EMM_ADDR_FLT_PATTERN
typedef struct AMP_DMX_EMM_ADDR_FLT_PATTERN {
  UINT8 Type;
  UINT8 Address[4];
  UINT8 Mask[4];
} AMP_DMX_EMM_ADDR_FLT_PATTERN;
#endif /* _typedef___AMP_DMX_EMM_ADDR_FLT_PATTERN */

#ifndef _typedef___AMP_DMX_EMM_FLT_PATTERN
#define _typedef___AMP_DMX_EMM_FLT_PATTERN
typedef struct AMP_DMX_EMM_FLT_PATTERN {
  UINT8 TableId[16];
  UINT8 TableIdFlag[16];
  UINT32 uAddrFltNbr;
  struct AMP_DMX_EMM_ADDR_FLT_PATTERN Pattern[8];
} AMP_DMX_EMM_FLT_PATTERN;
#endif /* _typedef___AMP_DMX_EMM_FLT_PATTERN */

#ifndef _typedef___AMP_DMX_FLT_CFG
#define _typedef___AMP_DMX_FLT_CFG
typedef struct AMP_DMX_FLT_CFG {
  AMP_DMX_TAG Tag;
  UINT32 uPid;
  UINT32 uSecBuffSize;
  UINT32 uPaddingSize;
  UINT32 EventMask;
  struct AMP_DMX_SEC_PATTERN Pattern;
  BOOL isEmmFilterEnabled;
  struct AMP_DMX_EMM_FLT_PATTERN EmmFltPattern;
  AMP_DMX_FLT_MODE FilterMode;
  AMP_DMX_FLT_CRC_MODE CrcMode;
  struct AMP_DMX_SCRAM_CTX DescramCtx;
  AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType;
  struct AMP_DRM_CRYPTO_ST CryptInfo;
  UINT32 uPushSecTimeout;
  UINT32 uiBufHighWaterMask;
  UINT32 uiBufLowWaterMask;
  UINT32 SecNumInOneBD;
} AMP_DMX_FLT_CFG;
#endif /* _typedef___AMP_DMX_FLT_CFG */

#ifndef _typedef___AMP_DMX_FLT_PROP_MASK
#define _typedef___AMP_DMX_FLT_PROP_MASK
typedef UINT32 AMP_DMX_FLT_PROP_MASK;
#endif /* _typedef___AMP_DMX_FLT_PROP_MASK */

#ifndef _define___AMP_DMX_FLT_PROP_PID
#define _define___AMP_DMX_FLT_PROP_PID
#define AMP_DMX_FLT_PROP_PID (1)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_EVENTMASK
#define _define___AMP_DMX_FLT_PROP_EVENTMASK
#define AMP_DMX_FLT_PROP_EVENTMASK (2)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_DESCRAM
#define _define___AMP_DMX_FLT_PROP_DESCRAM
#define AMP_DMX_FLT_PROP_DESCRAM (4)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_PATTERN
#define _define___AMP_DMX_FLT_PROP_PATTERN
#define AMP_DMX_FLT_PROP_PATTERN (8)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_FLTMODE
#define _define___AMP_DMX_FLT_PROP_FLTMODE
#define AMP_DMX_FLT_PROP_FLTMODE (16)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_CRCMODE
#define _define___AMP_DMX_FLT_PROP_CRCMODE
#define AMP_DMX_FLT_PROP_CRCMODE (32)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_HIGHWATER
#define _define___AMP_DMX_FLT_PROP_HIGHWATER
#define AMP_DMX_FLT_PROP_HIGHWATER (64)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_LOWWATER
#define _define___AMP_DMX_FLT_PROP_LOWWATER
#define AMP_DMX_FLT_PROP_LOWWATER (128)
#endif

#ifndef _define___AMP_DMX_FLT_PROP_SECNUMINONEBD
#define _define___AMP_DMX_FLT_PROP_SECNUMINONEBD
#define AMP_DMX_FLT_PROP_SECNUMINONEBD (256)
#endif

#ifndef _typedef___AMP_DMX_FLT_PROP
#define _typedef___AMP_DMX_FLT_PROP
typedef struct AMP_DMX_FLT_PROP {
  AMP_DMX_FLT_PROP_MASK uPropMask;
  struct AMP_DMX_FLT_CFG Property;
} AMP_DMX_FLT_PROP;
#endif /* _typedef___AMP_DMX_FLT_PROP */

#ifndef _define___AMP_DMX_FLT_CTRL_OFF
#define _define___AMP_DMX_FLT_CTRL_OFF
#define AMP_DMX_FLT_CTRL_OFF (0)
#endif

#ifndef _define___AMP_DMX_FLT_CTRL_ON
#define _define___AMP_DMX_FLT_CTRL_ON
#define AMP_DMX_FLT_CTRL_ON (1)
#endif

#ifndef _define___AMP_DMX_FLT_CTRL_MAX
#define _define___AMP_DMX_FLT_CTRL_MAX
#define AMP_DMX_FLT_CTRL_MAX (2)
#endif

#ifndef _typedef___AMP_DMX_FLT_CTRL
#define _typedef___AMP_DMX_FLT_CTRL
typedef CORBA_enum AMP_DMX_FLT_CTRL;
#endif /* _typedef___AMP_DMX_FLT_CTRL */

#ifndef _define___AMP_DMX_DSS_PKT_FLT_NONE
#define _define___AMP_DMX_DSS_PKT_FLT_NONE
#define AMP_DMX_DSS_PKT_FLT_NONE (0)
#endif

#ifndef _define___AMP_DMX_DSS_PKT_FLT_CWP
#define _define___AMP_DMX_DSS_PKT_FLT_CWP
#define AMP_DMX_DSS_PKT_FLT_CWP (1)
#endif

#ifndef _define___AMP_DMX_DSS_PKT_FLT_CAP
#define _define___AMP_DMX_DSS_PKT_FLT_CAP
#define AMP_DMX_DSS_PKT_FLT_CAP (2)
#endif

#ifndef _define___AMP_DMX_DSS_PKT_FLT_APG
#define _define___AMP_DMX_DSS_PKT_FLT_APG
#define AMP_DMX_DSS_PKT_FLT_APG (3)
#endif

#ifndef _define___AMP_DMX_DSS_PKT_FLT_GENERIC
#define _define___AMP_DMX_DSS_PKT_FLT_GENERIC
#define AMP_DMX_DSS_PKT_FLT_GENERIC (4)
#endif

#ifndef _define___AMP_DMX_DSS_PKT_FLT_MAX
#define _define___AMP_DMX_DSS_PKT_FLT_MAX
#define AMP_DMX_DSS_PKT_FLT_MAX (5)
#endif

#ifndef _typedef___AMP_DMX_DSS_PKT_FLT_TYPE
#define _typedef___AMP_DMX_DSS_PKT_FLT_TYPE
typedef CORBA_enum AMP_DMX_DSS_PKT_FLT_TYPE;
#endif /* _typedef___AMP_DMX_DSS_PKT_FLT_TYPE */

#ifndef _define___AMP_DMX_DSS_PKT_FLT_DEPTH
#define _define___AMP_DMX_DSS_PKT_FLT_DEPTH
#define AMP_DMX_DSS_PKT_FLT_DEPTH (16)
#endif

#ifndef _typedef___AMP_DMX_DSS_PKT_FLT_PATTERN
#define _typedef___AMP_DMX_DSS_PKT_FLT_PATTERN
typedef struct AMP_DMX_DSS_PKT_FLT_PATTERN {
  UINT8 Coef[16];
  UINT8 Neg[16];
  UINT8 Mask[16];
} AMP_DMX_DSS_PKT_FLT_PATTERN;
#endif /* _typedef___AMP_DMX_DSS_PKT_FLT_PATTERN */

#ifndef _typedef___AMP_DMX_DSS_PKT_FLT_CFG
#define _typedef___AMP_DMX_DSS_PKT_FLT_CFG
typedef struct AMP_DMX_DSS_PKT_FLT_CFG {
  AMP_DMX_TAG Tag;
  UINT32 uSCID;
  UINT32 uPktBuffSize;
  UINT32 uPaddingSize;
  AMP_DMX_DSS_PKT_FLT_TYPE eFltType;
  struct AMP_DMX_DSS_PKT_FLT_PATTERN FltPattern;
  UINT8 CAPFltAddr[4];
  AMP_DMX_FLT_MODE FilterMode;
} AMP_DMX_DSS_PKT_FLT_CFG;
#endif /* _typedef___AMP_DMX_DSS_PKT_FLT_CFG */

#ifndef _define___AMP_DMX_NON_SCRAMBLED
#define _define___AMP_DMX_NON_SCRAMBLED
#define AMP_DMX_NON_SCRAMBLED (0)
#endif

#ifndef _define___AMP_DMX_TS_SCRAMBLED
#define _define___AMP_DMX_TS_SCRAMBLED
#define AMP_DMX_TS_SCRAMBLED (1)
#endif

#ifndef _define___AMP_DMX_PES_SCRAMBLED
#define _define___AMP_DMX_PES_SCRAMBLED
#define AMP_DMX_PES_SCRAMBLED (2)
#endif

#ifndef _typedef___AMP_DMX_SCRAMBLE_TYPE
#define _typedef___AMP_DMX_SCRAMBLE_TYPE
typedef CORBA_enum AMP_DMX_SCRAMBLE_TYPE;
#endif /* _typedef___AMP_DMX_SCRAMBLE_TYPE */

#ifndef _typedef___AMP_DMX_EVENT_SCRAMINFO
#define _typedef___AMP_DMX_EVENT_SCRAMINFO
typedef struct AMP_DMX_EVENT_SCRAMINFO {
  AMP_DMX_SCRAMBLE_TYPE ScrambleType;
  UINT32 CurrScrambleMode;
  UINT32 LastScrambleMode;
  UINT32 uPid;
} AMP_DMX_EVENT_SCRAMINFO;
#endif /* _typedef___AMP_DMX_EVENT_SCRAMINFO */

#ifndef _typedef___AMP_DMX_EVENT_DISCONTINUITY_COUNT
#define _typedef___AMP_DMX_EVENT_DISCONTINUITY_COUNT
typedef struct AMP_DMX_EVENT_DISCONTINUITY_COUNT {
  UINT32 uPid;
  UINT32 count;
} AMP_DMX_EVENT_DISCONTINUITY_COUNT;
#endif /* _typedef___AMP_DMX_EVENT_DISCONTINUITY_COUNT */

#ifndef _typedef___AMP_DMX_EVENT_PESHEAD
#define _typedef___AMP_DMX_EVENT_PESHEAD
typedef struct AMP_DMX_EVENT_PESHEAD {
  UINT32 uPrefix;
  UINT32 uPid;
  UINT32 uStreamId;
} AMP_DMX_EVENT_PESHEAD;
#endif /* _typedef___AMP_DMX_EVENT_PESHEAD */

#ifndef _typedef___AMP_DMX_IOBUFF_STATE
#define _typedef___AMP_DMX_IOBUFF_STATE
typedef struct AMP_DMX_IOBUFF_STATE {
  AMP_DMX_HANDLE hHandle;
  UINT32 uFullness;
  UINT32 uSize;
} AMP_DMX_IOBUFF_STATE;
#endif /* _typedef___AMP_DMX_IOBUFF_STATE */

#ifndef _typedef___AMP_DMX_PTS_OUT_INFO
#define _typedef___AMP_DMX_PTS_OUT_INFO
typedef struct AMP_DMX_PTS_OUT_INFO {
  AMP_DMX_OUT_TYPE uChlType;
  UINT32 uPid;
  UINT32 uPtsHigh;
  UINT32 uPtsLow;
  UINT32 uDtsHigh;
  UINT32 uDtsLow;
  UINT32 uStrmPos;
} AMP_DMX_PTS_OUT_INFO;
#endif /* _typedef___AMP_DMX_PTS_OUT_INFO */

#ifndef _typedef___AMP_DMX_PCR_OUT_INFO
#define _typedef___AMP_DMX_PCR_OUT_INFO
typedef struct AMP_DMX_PCR_OUT_INFO {
  UINT32 uPid;
  UINT32 uPcrHigh;
  UINT32 uPcrLow;
} AMP_DMX_PCR_OUT_INFO;
#endif /* _typedef___AMP_DMX_PCR_OUT_INFO */

#ifndef _define___AMP_DMX_PUSH_STREAM_IN
#define _define___AMP_DMX_PUSH_STREAM_IN
#define AMP_DMX_PUSH_STREAM_IN (0)
#endif

#ifndef _define___AMP_DMX_PUSH_FRAME_IN
#define _define___AMP_DMX_PUSH_FRAME_IN
#define AMP_DMX_PUSH_FRAME_IN (1)
#endif

#ifndef _define___AMP_DMX_PUSH_IN_MAX
#define _define___AMP_DMX_PUSH_IN_MAX
#define AMP_DMX_PUSH_IN_MAX (2)
#endif

#ifndef _typedef___AMP_DMX_PUSH_MODE
#define _typedef___AMP_DMX_PUSH_MODE
typedef CORBA_enum AMP_DMX_PUSH_MODE;
#endif /* _typedef___AMP_DMX_PUSH_MODE */

#ifndef _define___AMP_DMX_DBG_OPT_DUMP
#define _define___AMP_DMX_DBG_OPT_DUMP
#define AMP_DMX_DBG_OPT_DUMP (0)
#endif

#ifndef _define___AMP_DMX_DBG_OPT_IODUMP_START
#define _define___AMP_DMX_DBG_OPT_IODUMP_START
#define AMP_DMX_DBG_OPT_IODUMP_START (1)
#endif

#ifndef _define___AMP_DMX_DBG_OPT_IODUMP_STOP
#define _define___AMP_DMX_DBG_OPT_IODUMP_STOP
#define AMP_DMX_DBG_OPT_IODUMP_STOP (2)
#endif

#ifndef _define___AMP_DMX_DBG_OPT_MAX
#define _define___AMP_DMX_DBG_OPT_MAX
#define AMP_DMX_DBG_OPT_MAX (3)
#endif

#ifndef _typedef___AMP_DMX_DBG_OPT
#define _typedef___AMP_DMX_DBG_OPT
typedef CORBA_enum AMP_DMX_DBG_OPT;
#endif /* _typedef___AMP_DMX_DBG_OPT */

#ifndef _typedef___AMP_DMX_DEBUG_MASK
#define _typedef___AMP_DMX_DEBUG_MASK
typedef UINT32 AMP_DMX_DEBUG_MASK;
#endif /* _typedef___AMP_DMX_DEBUG_MASK */

#ifndef _define___AMP_DMX_DEBUG_MASK_COMP
#define _define___AMP_DMX_DEBUG_MASK_COMP
#define AMP_DMX_DEBUG_MASK_COMP (1)
#endif

#ifndef _define___AMP_DMX_DEBUG_MASK_PORT
#define _define___AMP_DMX_DEBUG_MASK_PORT
#define AMP_DMX_DEBUG_MASK_PORT (2)
#endif

#ifndef _define___AMP_DMX_DEBUG_MASK_SOURCE
#define _define___AMP_DMX_DEBUG_MASK_SOURCE
#define AMP_DMX_DEBUG_MASK_SOURCE (4)
#endif

#ifndef _define___AMP_DMX_DEBUG_MASK_STREAM
#define _define___AMP_DMX_DEBUG_MASK_STREAM
#define AMP_DMX_DEBUG_MASK_STREAM (8)
#endif

#ifndef _define___AMP_DMX_DEBUG_MASK_RCCHK
#define _define___AMP_DMX_DEBUG_MASK_RCCHK
#define AMP_DMX_DEBUG_MASK_RCCHK (16)
#endif

#ifndef _define___AMP_DMX_DBG_DEST_FILE
#define _define___AMP_DMX_DBG_DEST_FILE
#define AMP_DMX_DBG_DEST_FILE (0)
#endif

#ifndef _define___AMP_DMX_DBG_DEST_CONSOLE
#define _define___AMP_DMX_DBG_DEST_CONSOLE
#define AMP_DMX_DBG_DEST_CONSOLE (1)
#endif

#ifndef _define___AMP_DMX_DBG_DEST_MAX
#define _define___AMP_DMX_DBG_DEST_MAX
#define AMP_DMX_DBG_DEST_MAX (2)
#endif

#ifndef _typedef___AMP_DMX_DBG_DEST
#define _typedef___AMP_DMX_DBG_DEST
typedef CORBA_enum AMP_DMX_DBG_DEST;
#endif /* _typedef___AMP_DMX_DBG_DEST */

#ifndef _typedef___AMP_DMX_DBG_CFG
#define _typedef___AMP_DMX_DBG_CFG
typedef struct AMP_DMX_DBG_CFG {
  UINT32 DebugMask;
  AMP_DMX_DBG_DEST Dest;
  INT8 DumpFile[64];
} AMP_DMX_DBG_CFG;
#endif /* _typedef___AMP_DMX_DBG_CFG */

#ifndef _typedef___AMP_DMX_DBG_IOBUF_DUMP_CTRL_MASK
#define _typedef___AMP_DMX_DBG_IOBUF_DUMP_CTRL_MASK
typedef UINT32 AMP_DMX_DBG_IOBUF_DUMP_CTRL_MASK;
#endif /* _typedef___AMP_DMX_DBG_IOBUF_DUMP_CTRL_MASK */

#ifndef _define___AMP_DMX_DBG_INBUF_MASK_DATA
#define _define___AMP_DMX_DBG_INBUF_MASK_DATA
#define AMP_DMX_DBG_INBUF_MASK_DATA (1)
#endif

#ifndef _define___AMP_DMX_DBG_INBUF_MASK_FLT
#define _define___AMP_DMX_DBG_INBUF_MASK_FLT
#define AMP_DMX_DBG_INBUF_MASK_FLT (2)
#endif

#ifndef _define___AMP_DMX_DBG_INBUF_MASK_TSI
#define _define___AMP_DMX_DBG_INBUF_MASK_TSI
#define AMP_DMX_DBG_INBUF_MASK_TSI (4)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_ES_DATA
#define _define___AMP_DMX_DBG_OUTBUF_MASK_ES_DATA
#define AMP_DMX_DBG_OUTBUF_MASK_ES_DATA (256)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_PES_DATA
#define _define___AMP_DMX_DBG_OUTBUF_MASK_PES_DATA
#define AMP_DMX_DBG_OUTBUF_MASK_PES_DATA (512)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_TS_DATA
#define _define___AMP_DMX_DBG_OUTBUF_MASK_TS_DATA
#define AMP_DMX_DBG_OUTBUF_MASK_TS_DATA (1024)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_SEC_DATA
#define _define___AMP_DMX_DBG_OUTBUF_MASK_SEC_DATA
#define AMP_DMX_DBG_OUTBUF_MASK_SEC_DATA (2048)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_RAW_DATA
#define _define___AMP_DMX_DBG_OUTBUF_MASK_RAW_DATA
#define AMP_DMX_DBG_OUTBUF_MASK_RAW_DATA (4096)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_TS_PORT_DATA
#define _define___AMP_DMX_DBG_OUTBUF_MASK_TS_PORT_DATA
#define AMP_DMX_DBG_OUTBUF_MASK_TS_PORT_DATA (8192)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_ES_FLT
#define _define___AMP_DMX_DBG_OUTBUF_MASK_ES_FLT
#define AMP_DMX_DBG_OUTBUF_MASK_ES_FLT (1048576)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_PES_FLT
#define _define___AMP_DMX_DBG_OUTBUF_MASK_PES_FLT
#define AMP_DMX_DBG_OUTBUF_MASK_PES_FLT (2097152)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_TS_FLT
#define _define___AMP_DMX_DBG_OUTBUF_MASK_TS_FLT
#define AMP_DMX_DBG_OUTBUF_MASK_TS_FLT (4194304)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_SEC_FLT
#define _define___AMP_DMX_DBG_OUTBUF_MASK_SEC_FLT
#define AMP_DMX_DBG_OUTBUF_MASK_SEC_FLT (8388608)
#endif

#ifndef _define___AMP_DMX_DBG_OUTBUF_MASK_RAW_FLT
#define _define___AMP_DMX_DBG_OUTBUF_MASK_RAW_FLT
#define AMP_DMX_DBG_OUTBUF_MASK_RAW_FLT (16777216)
#endif

HRESULT AMP_DMX_AddInput(AMP_DMX _obj, struct AMP_DMX_INPUT_CFG *pInputCfg, struct AMP_DMX_INPUT_ST *pInputObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_RmInput(AMP_DMX _obj, struct AMP_DMX_INPUT_ST *pInputObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_ResetInput(AMP_DMX _obj, struct AMP_DMX_INPUT_ST *pInputObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_InputPropSet(AMP_DMX _obj, struct AMP_DMX_INPUT_ST *pInputObj, struct AMP_DMX_INPUT_PROP *pProp, CORBA_Environment *_ev);

HRESULT AMP_DMX_InputPropGet(AMP_DMX _obj, struct AMP_DMX_INPUT_ST *pInputObj, struct AMP_DMX_INPUT_PROP *pProp, CORBA_Environment *_ev);

HRESULT AMP_DMX_AddChannel(AMP_DMX _obj, struct AMP_DMX_CH_CFG *pChCfg, struct AMP_DMX_CHNL_ST *pChObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_RmChannel(AMP_DMX _obj, struct AMP_DMX_CHNL_ST *pChObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_ChannelPropSet(AMP_DMX _obj, struct AMP_DMX_CHNL_ST *pChObj, struct AMP_DMX_CH_PROP *pProp, CORBA_Environment *_ev);

HRESULT AMP_DMX_ChannelPropGet(AMP_DMX _obj, struct AMP_DMX_CHNL_ST *pChObj, struct AMP_DMX_CH_PROP *pProp, CORBA_Environment *_ev);

HRESULT AMP_DMX_AddKeyToken(AMP_DMX _obj, UINT32 uPid, UINT32 uKeyToken, CORBA_Environment *_ev);

HRESULT AMP_DMX_RemoveKeyToken(AMP_DMX _obj, UINT32 uPid, UINT32 uKeyToken, CORBA_Environment *_ev);

HRESULT AMP_DMX_ChannelControl(AMP_DMX _obj, struct AMP_DMX_CHNL_ST *pChObj, AMP_DMX_CH_CTRL eCtrlType, UINT32 uParam, CORBA_Environment *_ev);

HRESULT AMP_DMX_GetHandle(AMP_DMX _obj, AMP_DMX_HANDLE *pHandle, CORBA_Environment *_ev);

HRESULT AMP_DMX_AddPrimaryPID(AMP_DMX _obj, AMP_DMX_HANDLE Handle, UINT32 uPrimaryPID, UINT32 uCmChannelIdx, struct AMP_DMX_CA_PID_ST *pCaPidObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_RmPrimaryPID(AMP_DMX _obj, struct AMP_DMX_CA_PID_ST *pCaPidObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_SetSecondaryPID(AMP_DMX _obj, struct AMP_DMX_CA_PID_ST *pCaPidObj, UINT32 uSecondaryPID, AMP_DMX_MERGE_MODE eMergeMode, UINT32 uCmChannelIdx, CORBA_Environment *_ev);

HRESULT AMP_DMX_RmSecondaryPID(AMP_DMX _obj, struct AMP_DMX_CA_PID_ST *pCaPidObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_AddSectionFilter(AMP_DMX _obj, struct AMP_DMX_FLT_CFG *pFltCfg, struct AMP_DMX_FLT_ST *pFilterObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_RmSectionFilter(AMP_DMX _obj, struct AMP_DMX_FLT_ST *pFilterObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_SectionFilterPropSet(AMP_DMX _obj, struct AMP_DMX_FLT_ST *pFilterObj, struct AMP_DMX_FLT_PROP *pProp, CORBA_Environment *_ev);

HRESULT AMP_DMX_SectionFilterPropGet(AMP_DMX _obj, struct AMP_DMX_FLT_ST *pFilterObj, struct AMP_DMX_FLT_PROP *pProp, CORBA_Environment *_ev);

HRESULT AMP_DMX_SectionFilterControl(AMP_DMX _obj, struct AMP_DMX_FLT_ST *pFilterObj, AMP_DMX_FLT_CTRL eCtrlType, UINT32 uParam, CORBA_Environment *_ev);

HRESULT AMP_DMX_AddDSSPktFilter(AMP_DMX _obj, struct AMP_DMX_DSS_PKT_FLT_CFG *pDSSFltCfg, struct AMP_DMX_DSS_FLT_ST *pDSSFilterObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_RmDSSPktFilter(AMP_DMX _obj, struct AMP_DMX_DSS_FLT_ST *pDSSFilterObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_StartDSSPktFilter(AMP_DMX _obj, struct AMP_DMX_DSS_FLT_ST *pDSSFilterObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_StopDSSPktFilter(AMP_DMX _obj, struct AMP_DMX_DSS_FLT_ST *pDSSFilterObj, CORBA_Environment *_ev);

HRESULT AMP_DMX_ConfigPushMode(AMP_DMX _obj, AMP_DMX_PUSH_MODE ePushMode, CORBA_Environment *_ev);

HRESULT AMP_DMX_DebugControl(AMP_DMX _obj, AMP_DMX_DBG_OPT Opt, struct AMP_DMX_DBG_CFG *pDbgCfg, CORBA_Environment *_ev);

HRESULT AMP_DMX_Open(AMP_DMX _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DMX_Close(AMP_DMX _obj, CORBA_Environment *_ev);

HRESULT AMP_DMX_Destroy(AMP_DMX _obj, CORBA_Environment *_ev);

HRESULT AMP_DMX_SetState(AMP_DMX _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DMX_GetState(AMP_DMX _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DMX_QueryInfo(AMP_DMX _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DMX_SetupPort(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DMX_DynamicSetupPort(AMP_DMX _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DMX_DisconnectPorts(AMP_DMX _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DMX_ClearPort(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DMX_QueryPort(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DMX_ClearPortBuf(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DMX_DebugDump(AMP_DMX _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DMX_RegisterNotify(AMP_DMX _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DMX_UnregisterNotify(AMP_DMX _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DMX_RegisterNotifyDetail(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DMX_UnregisterNotifyDetail(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DMX_RegisterBD(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DMX_UnregisterBD(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DMX_PushBD(AMP_DMX _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_VDEC
#define _typedef___AMP_VDEC
typedef CORBA_Object AMP_VDEC;
#endif /* _typedef___AMP_VDEC */

#ifndef _define___AMP_VDEC_IOPARAM_USER_DATA_ENABLE
#define _define___AMP_VDEC_IOPARAM_USER_DATA_ENABLE
#define AMP_VDEC_IOPARAM_USER_DATA_ENABLE (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_USER_DATA_DISABLE
#define _define___AMP_VDEC_IOPARAM_USER_DATA_DISABLE
#define AMP_VDEC_IOPARAM_USER_DATA_DISABLE (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_LOW_LATENCY_ENABLE
#define _define___AMP_VDEC_IOPARAM_LOW_LATENCY_ENABLE
#define AMP_VDEC_IOPARAM_LOW_LATENCY_ENABLE (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_LOW_LATENCY_DISABLE
#define _define___AMP_VDEC_IOPARAM_LOW_LATENCY_DISABLE
#define AMP_VDEC_IOPARAM_LOW_LATENCY_DISABLE (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DEBLOCK_IP
#define _define___AMP_VDEC_IOPARAM_DEBLOCK_IP
#define AMP_VDEC_IOPARAM_DEBLOCK_IP (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DEBLOCK_IPB
#define _define___AMP_VDEC_IOPARAM_DEBLOCK_IPB
#define AMP_VDEC_IOPARAM_DEBLOCK_IPB (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DEBLOCK_I
#define _define___AMP_VDEC_IOPARAM_DEBLOCK_I
#define AMP_VDEC_IOPARAM_DEBLOCK_I (2)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DEBLOCK_NONE
#define _define___AMP_VDEC_IOPARAM_DEBLOCK_NONE
#define AMP_VDEC_IOPARAM_DEBLOCK_NONE (3)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_REORDER_ENABLE
#define _define___AMP_VDEC_IOPARAM_REORDER_ENABLE
#define AMP_VDEC_IOPARAM_REORDER_ENABLE (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_REORDER_DISABLE
#define _define___AMP_VDEC_IOPARAM_REORDER_DISABLE
#define AMP_VDEC_IOPARAM_REORDER_DISABLE (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_SCHED_EQUAL
#define _define___AMP_VDEC_IOPARAM_SCHED_EQUAL
#define AMP_VDEC_IOPARAM_SCHED_EQUAL (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_SCHED_FR
#define _define___AMP_VDEC_IOPARAM_SCHED_FR
#define AMP_VDEC_IOPARAM_SCHED_FR (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_SCHED_PRIO
#define _define___AMP_VDEC_IOPARAM_SCHED_PRIO
#define AMP_VDEC_IOPARAM_SCHED_PRIO (2)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_SCHED_PTS
#define _define___AMP_VDEC_IOPARAM_SCHED_PTS
#define AMP_VDEC_IOPARAM_SCHED_PTS (3)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_SCHED_BUF
#define _define___AMP_VDEC_IOPARAM_SCHED_BUF
#define AMP_VDEC_IOPARAM_SCHED_BUF (4)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DECODE_ALL
#define _define___AMP_VDEC_IOPARAM_DECODE_ALL
#define AMP_VDEC_IOPARAM_DECODE_ALL (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DECODE_IP
#define _define___AMP_VDEC_IOPARAM_DECODE_IP
#define AMP_VDEC_IOPARAM_DECODE_IP (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DECODE_I
#define _define___AMP_VDEC_IOPARAM_DECODE_I
#define AMP_VDEC_IOPARAM_DECODE_I (2)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DECODE_REVERSE
#define _define___AMP_VDEC_IOPARAM_DECODE_REVERSE
#define AMP_VDEC_IOPARAM_DECODE_REVERSE (3)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_SPECIAL_FRAME
#define _define___AMP_VDEC_IOPARAM_SPECIAL_FRAME
#define AMP_VDEC_IOPARAM_SPECIAL_FRAME (4)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_DECODE_1_AUTO_STOP
#define _define___AMP_VDEC_IOPARAM_DECODE_1_AUTO_STOP
#define AMP_VDEC_IOPARAM_DECODE_1_AUTO_STOP (5)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_ERROR_DROP
#define _define___AMP_VDEC_IOPARAM_ERROR_DROP
#define AMP_VDEC_IOPARAM_ERROR_DROP (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_ERROR_NODROP
#define _define___AMP_VDEC_IOPARAM_ERROR_NODROP
#define AMP_VDEC_IOPARAM_ERROR_NODROP (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_ERROR_DROP_HIGH
#define _define___AMP_VDEC_IOPARAM_ERROR_DROP_HIGH
#define AMP_VDEC_IOPARAM_ERROR_DROP_HIGH (2)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_3D_FORMAT_SBS
#define _define___AMP_VDEC_IOPARAM_3D_FORMAT_SBS
#define AMP_VDEC_IOPARAM_3D_FORMAT_SBS (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_3D_FORMAT_TAB
#define _define___AMP_VDEC_IOPARAM_3D_FORMAT_TAB
#define AMP_VDEC_IOPARAM_3D_FORMAT_TAB (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_3D_FORMAT_FS
#define _define___AMP_VDEC_IOPARAM_3D_FORMAT_FS
#define AMP_VDEC_IOPARAM_3D_FORMAT_FS (2)
#endif

#ifndef _define___AMP_VDEC_ASPECT_RATIO_1_1
#define _define___AMP_VDEC_ASPECT_RATIO_1_1
#define AMP_VDEC_ASPECT_RATIO_1_1 (65537)
#endif

#ifndef _define___AMP_VDEC_ASPECT_RATIO_4_3
#define _define___AMP_VDEC_ASPECT_RATIO_4_3
#define AMP_VDEC_ASPECT_RATIO_4_3 (262147)
#endif

#ifndef _define___AMP_VDEC_ASPECT_RATIO_16_9
#define _define___AMP_VDEC_ASPECT_RATIO_16_9
#define AMP_VDEC_ASPECT_RATIO_16_9 (1048585)
#endif

#ifndef _define___AMP_VDEC_ASPECT_RATIO_221_100
#define _define___AMP_VDEC_ASPECT_RATIO_221_100
#define AMP_VDEC_ASPECT_RATIO_221_100 (14483556)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VSCAN_PROGRESSIVE
#define _define___AMP_VDEC_IOPARAM_VSCAN_PROGRESSIVE
#define AMP_VDEC_IOPARAM_VSCAN_PROGRESSIVE (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VSCAN_INTERLACE
#define _define___AMP_VDEC_IOPARAM_VSCAN_INTERLACE
#define AMP_VDEC_IOPARAM_VSCAN_INTERLACE (2)
#endif

#ifndef _typedef___AMP_VDEC_PARASTATISTICS
#define _typedef___AMP_VDEC_PARASTATISTICS
typedef struct AMP_VDEC_PARASTATISTICS {
  UINT32 decoder_pts_high;
  UINT32 decoder_pts_low;
  UINT32 num_decoded_samples;
  UINT32 num_decoder_errors;
  UINT32 num_decoded_Iframes;
  UINT32 num_Iframe_errors;
  UINT32 samples_duration;
  UINT32 decoded_framerate;
} AMP_VDEC_PARASTATISTICS;
#endif /* _typedef___AMP_VDEC_PARASTATISTICS */

#ifndef _define___AMP_VDEC_IOPARAM_VIDROTATE_ORG
#define _define___AMP_VDEC_IOPARAM_VIDROTATE_ORG
#define AMP_VDEC_IOPARAM_VIDROTATE_ORG (0)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VIDROTATE_90
#define _define___AMP_VDEC_IOPARAM_VIDROTATE_90
#define AMP_VDEC_IOPARAM_VIDROTATE_90 (1)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VIDROTATE_180
#define _define___AMP_VDEC_IOPARAM_VIDROTATE_180
#define AMP_VDEC_IOPARAM_VIDROTATE_180 (2)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VIDROTATE_270
#define _define___AMP_VDEC_IOPARAM_VIDROTATE_270
#define AMP_VDEC_IOPARAM_VIDROTATE_270 (3)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VIDROTATE_FLIP_HOR
#define _define___AMP_VDEC_IOPARAM_VIDROTATE_FLIP_HOR
#define AMP_VDEC_IOPARAM_VIDROTATE_FLIP_HOR (4)
#endif

#ifndef _define___AMP_VDEC_IOPARAM_VIDROTATE_FLIP_VERT
#define _define___AMP_VDEC_IOPARAM_VIDROTATE_FLIP_VERT
#define AMP_VDEC_IOPARAM_VIDROTATE_FLIP_VERT (5)
#endif

#ifndef _define___AMP_VDEC_CMD_START_P1
#define _define___AMP_VDEC_CMD_START_P1
#define AMP_VDEC_CMD_START_P1 (0)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_LOW_LATENCY
#define _define___AMP_VDEC_CMD_SET_LOW_LATENCY
#define AMP_VDEC_CMD_SET_LOW_LATENCY (1)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_LOW_LATENCY
#define _define___AMP_VDEC_CMD_GET_LOW_LATENCY
#define AMP_VDEC_CMD_GET_LOW_LATENCY (2)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_DEBLOCK
#define _define___AMP_VDEC_CMD_SET_DEBLOCK
#define AMP_VDEC_CMD_SET_DEBLOCK (3)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_DEBLOCK
#define _define___AMP_VDEC_CMD_GET_DEBLOCK
#define AMP_VDEC_CMD_GET_DEBLOCK (4)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_REORDER
#define _define___AMP_VDEC_CMD_SET_REORDER
#define AMP_VDEC_CMD_SET_REORDER (5)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_REORDER
#define _define___AMP_VDEC_CMD_GET_REORDER
#define AMP_VDEC_CMD_GET_REORDER (6)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_SCHED
#define _define___AMP_VDEC_CMD_SET_SCHED
#define AMP_VDEC_CMD_SET_SCHED (7)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_SCHED
#define _define___AMP_VDEC_CMD_GET_SCHED
#define AMP_VDEC_CMD_GET_SCHED (8)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_DECODE
#define _define___AMP_VDEC_CMD_SET_DECODE
#define AMP_VDEC_CMD_SET_DECODE (9)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_DECODE
#define _define___AMP_VDEC_CMD_GET_DECODE
#define AMP_VDEC_CMD_GET_DECODE (10)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_ERROR_HANDLE
#define _define___AMP_VDEC_CMD_SET_ERROR_HANDLE
#define AMP_VDEC_CMD_SET_ERROR_HANDLE (11)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_ERROR_HANDLE
#define _define___AMP_VDEC_CMD_GET_ERROR_HANDLE
#define AMP_VDEC_CMD_GET_ERROR_HANDLE (12)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_3D_FORMAT
#define _define___AMP_VDEC_CMD_SET_3D_FORMAT
#define AMP_VDEC_CMD_SET_3D_FORMAT (13)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_3D_FORMAT
#define _define___AMP_VDEC_CMD_GET_3D_FORMAT
#define AMP_VDEC_CMD_GET_3D_FORMAT (14)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_VSCAN
#define _define___AMP_VDEC_CMD_GET_VSCAN
#define AMP_VDEC_CMD_GET_VSCAN (15)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_LOW_WATERMARK
#define _define___AMP_VDEC_CMD_SET_LOW_WATERMARK
#define AMP_VDEC_CMD_SET_LOW_WATERMARK (16)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_HIGH_WATERMARK
#define _define___AMP_VDEC_CMD_SET_HIGH_WATERMARK
#define AMP_VDEC_CMD_SET_HIGH_WATERMARK (17)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_EVENT_MASK
#define _define___AMP_VDEC_CMD_SET_EVENT_MASK
#define AMP_VDEC_CMD_SET_EVENT_MASK (18)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_EVENT_MASK
#define _define___AMP_VDEC_CMD_GET_EVENT_MASK
#define AMP_VDEC_CMD_GET_EVENT_MASK (19)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_DECODE_FRAME_NUMBER
#define _define___AMP_VDEC_CMD_GET_DECODE_FRAME_NUMBER
#define AMP_VDEC_CMD_GET_DECODE_FRAME_NUMBER (20)
#endif

#ifndef _define___AMP_VDEC_CMD_START_P2
#define _define___AMP_VDEC_CMD_START_P2
#define AMP_VDEC_CMD_START_P2 (21)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_RESOLUTION
#define _define___AMP_VDEC_CMD_SET_RESOLUTION
#define AMP_VDEC_CMD_SET_RESOLUTION (22)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_RESOLUTION
#define _define___AMP_VDEC_CMD_GET_RESOLUTION
#define AMP_VDEC_CMD_GET_RESOLUTION (23)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_FRAME_RATE
#define _define___AMP_VDEC_CMD_SET_FRAME_RATE
#define AMP_VDEC_CMD_SET_FRAME_RATE (24)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_FRAME_RATE
#define _define___AMP_VDEC_CMD_GET_FRAME_RATE
#define AMP_VDEC_CMD_GET_FRAME_RATE (25)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_ASPECT_RATIO
#define _define___AMP_VDEC_CMD_SET_ASPECT_RATIO
#define AMP_VDEC_CMD_SET_ASPECT_RATIO (26)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_ASPECT_RATIO
#define _define___AMP_VDEC_CMD_GET_ASPECT_RATIO
#define AMP_VDEC_CMD_GET_ASPECT_RATIO (27)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_USER_DATA
#define _define___AMP_VDEC_CMD_SET_USER_DATA
#define AMP_VDEC_CMD_SET_USER_DATA (28)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_USER_DATA
#define _define___AMP_VDEC_CMD_GET_USER_DATA
#define AMP_VDEC_CMD_GET_USER_DATA (29)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_HEAD_BITRATE
#define _define___AMP_VDEC_CMD_GET_HEAD_BITRATE
#define AMP_VDEC_CMD_GET_HEAD_BITRATE (30)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_GOP_BITRATE
#define _define___AMP_VDEC_CMD_GET_GOP_BITRATE
#define AMP_VDEC_CMD_GET_GOP_BITRATE (31)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_GOP_QP
#define _define___AMP_VDEC_CMD_GET_GOP_QP
#define AMP_VDEC_CMD_GET_GOP_QP (32)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_MEDIA_TYPE
#define _define___AMP_VDEC_CMD_GET_MEDIA_TYPE
#define AMP_VDEC_CMD_GET_MEDIA_TYPE (33)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_PROFILE
#define _define___AMP_VDEC_CMD_GET_PROFILE
#define AMP_VDEC_CMD_GET_PROFILE (34)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_VIDEO_ROTATION_MODE
#define _define___AMP_VDEC_CMD_SET_VIDEO_ROTATION_MODE
#define AMP_VDEC_CMD_SET_VIDEO_ROTATION_MODE (35)
#endif

#ifndef _define___AMP_VDEC_CMD_GET_VIDEO_ROTATION_MODE
#define _define___AMP_VDEC_CMD_GET_VIDEO_ROTATION_MODE
#define AMP_VDEC_CMD_GET_VIDEO_ROTATION_MODE (36)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_START_PTS
#define _define___AMP_VDEC_CMD_SET_START_PTS
#define AMP_VDEC_CMD_SET_START_PTS (37)
#endif

#ifndef _define___AMP_VDEC_CMD_SET_USERDATA_OUTPUT_PORT
#define _define___AMP_VDEC_CMD_SET_USERDATA_OUTPUT_PORT
#define AMP_VDEC_CMD_SET_USERDATA_OUTPUT_PORT (38)
#endif

#ifndef _define___AMP_VDEC_CMD_END
#define _define___AMP_VDEC_CMD_END
#define AMP_VDEC_CMD_END (39)
#endif

#ifndef _typedef___AMP_VDEC_CMD
#define _typedef___AMP_VDEC_CMD
typedef CORBA_enum AMP_VDEC_CMD;
#endif /* _typedef___AMP_VDEC_CMD */

#ifndef _define___AMP_VDEC_EVENT_NONE_MASK
#define _define___AMP_VDEC_EVENT_NONE_MASK
#define AMP_VDEC_EVENT_NONE_MASK (0)
#endif

#ifndef _define___AMP_VDEC_EVENT_RES_CHANGE
#define _define___AMP_VDEC_EVENT_RES_CHANGE
#define AMP_VDEC_EVENT_RES_CHANGE (1)
#endif

#ifndef _typedef___AMP_VDEC_RES_CHANGE_DESC
#define _typedef___AMP_VDEC_RES_CHANGE_DESC
typedef struct AMP_VDEC_RES_CHANGE_DESC {
  UINT32 uiBufferWidth;
  UINT32 uiBufferHeight;
  UINT32 uiSarWidth;
  UINT32 uiSarHeight;
  UINT32 uiBufferSize;
  UINT32 uiBitDepth;
  UINT32 uiBufferNum;
  UINT32 uiOutputMode;
  UINT32 uiCropTop;
  UINT32 uiCropLeft;
  UINT32 uiCropWidth;
  UINT32 uiCropHeight;
  UINT32 uiFrameWidth;
  UINT32 uiFrameHeight;
  UINT32 bYseparateWithUV;
  UINT32 bReqContBuf;
} AMP_VDEC_RES_CHANGE_DESC;
#endif /* _typedef___AMP_VDEC_RES_CHANGE_DESC */

#ifndef _define___AMP_VDEC_EVENT_FR_CHANGE
#define _define___AMP_VDEC_EVENT_FR_CHANGE
#define AMP_VDEC_EVENT_FR_CHANGE (2)
#endif

#ifndef _typedef___AMP_VDEC_FR_CHANGE_DESC
#define _typedef___AMP_VDEC_FR_CHANGE_DESC
typedef struct AMP_VDEC_FR_CHANGE_DESC {
  UINT32 uiFrameRateNum;
  UINT32 uiFrameRateDen;
} AMP_VDEC_FR_CHANGE_DESC;
#endif /* _typedef___AMP_VDEC_FR_CHANGE_DESC */

#ifndef _define___AMP_VDEC_EVENT_AR_CHANGE
#define _define___AMP_VDEC_EVENT_AR_CHANGE
#define AMP_VDEC_EVENT_AR_CHANGE (4)
#endif

#ifndef _typedef___AMP_VDEC_AR_CHANGE_DESC
#define _typedef___AMP_VDEC_AR_CHANGE_DESC
typedef struct AMP_VDEC_AR_CHANGE_DESC {
  UINT32 uiARWidth;
  UINT32 uiARHeight;
} AMP_VDEC_AR_CHANGE_DESC;
#endif /* _typedef___AMP_VDEC_AR_CHANGE_DESC */

#ifndef _define___AMP_VDEC_EVENT_STREAM_DONE
#define _define___AMP_VDEC_EVENT_STREAM_DONE
#define AMP_VDEC_EVENT_STREAM_DONE (8)
#endif

#ifndef _define___AMP_VDEC_EVENT_STREAM_LOW
#define _define___AMP_VDEC_EVENT_STREAM_LOW
#define AMP_VDEC_EVENT_STREAM_LOW (16)
#endif

#ifndef _define___AMP_VDEC_EVENT_STREAM_HIGH
#define _define___AMP_VDEC_EVENT_STREAM_HIGH
#define AMP_VDEC_EVENT_STREAM_HIGH (32)
#endif

#ifndef _define___AMP_VDEC_EVENT_SEQ_CHANGE
#define _define___AMP_VDEC_EVENT_SEQ_CHANGE
#define AMP_VDEC_EVENT_SEQ_CHANGE (64)
#endif

#ifndef _define___AMP_VDEC_EVENT_1ST_I_DECODED
#define _define___AMP_VDEC_EVENT_1ST_I_DECODED
#define AMP_VDEC_EVENT_1ST_I_DECODED (128)
#endif

#ifndef _typedef___AMP_VDEC_FST_FRAME_DESC
#define _typedef___AMP_VDEC_FST_FRAME_DESC
typedef struct AMP_VDEC_FST_FRAME_DESC {
  UINT32 uiFrameWidth;
  UINT32 uiFrameHeight;
  UINT32 bIsIntlSeq;
  UINT32 uiFrameRateNum;
  UINT32 uiFrameRateDen;
  UINT32 uiARWidth;
  UINT32 uiARHeight;
  UINT32 uiPtsHigh;
  UINT32 uiPtsLow;
} AMP_VDEC_FST_FRAME_DESC;
#endif /* _typedef___AMP_VDEC_FST_FRAME_DESC */

#ifndef _define___AMP_VDEC_EVENT_ERROR_HANDLE
#define _define___AMP_VDEC_EVENT_ERROR_HANDLE
#define AMP_VDEC_EVENT_ERROR_HANDLE (256)
#endif

#ifndef _define___AMP_VDEC_EVENT_USER_DATA
#define _define___AMP_VDEC_EVENT_USER_DATA
#define AMP_VDEC_EVENT_USER_DATA (1024)
#endif

#ifndef _define___AMP_VDEC_EVENT_ONE_FRAME_DECODED
#define _define___AMP_VDEC_EVENT_ONE_FRAME_DECODED
#define AMP_VDEC_EVENT_ONE_FRAME_DECODED (2048)
#endif

#ifndef _typedef___AMP_VDEC_ONE_FRAME_DESC
#define _typedef___AMP_VDEC_ONE_FRAME_DESC
typedef struct AMP_VDEC_ONE_FRAME_DESC {
  UINT32 uiFrameType;
} AMP_VDEC_ONE_FRAME_DESC;
#endif /* _typedef___AMP_VDEC_ONE_FRAME_DESC */

#ifndef _define___AMP_VDEC_FRAME_TYPE_NULL
#define _define___AMP_VDEC_FRAME_TYPE_NULL
#define AMP_VDEC_FRAME_TYPE_NULL (0)
#endif

#ifndef _define___AMP_VDEC_FRAME_TYPE_HEAD
#define _define___AMP_VDEC_FRAME_TYPE_HEAD
#define AMP_VDEC_FRAME_TYPE_HEAD (1)
#endif

#ifndef _define___AMP_VDEC_FRAME_TYPE_I
#define _define___AMP_VDEC_FRAME_TYPE_I
#define AMP_VDEC_FRAME_TYPE_I (2)
#endif

#ifndef _define___AMP_VDEC_FRAME_TYPE_P
#define _define___AMP_VDEC_FRAME_TYPE_P
#define AMP_VDEC_FRAME_TYPE_P (3)
#endif

#ifndef _define___AMP_VDEC_FRAME_TYPE_B
#define _define___AMP_VDEC_FRAME_TYPE_B
#define AMP_VDEC_FRAME_TYPE_B (4)
#endif

#ifndef _define___AMP_VDEC_FRAME_TYPE_END
#define _define___AMP_VDEC_FRAME_TYPE_END
#define AMP_VDEC_FRAME_TYPE_END (5)
#endif

#ifndef _typedef___AMP_VDEC_FRAME_TYPE
#define _typedef___AMP_VDEC_FRAME_TYPE
typedef CORBA_enum AMP_VDEC_FRAME_TYPE;
#endif /* _typedef___AMP_VDEC_FRAME_TYPE */

#ifndef _define___AMP_VDEC_EVENT_DISP_CROP_CHANGE
#define _define___AMP_VDEC_EVENT_DISP_CROP_CHANGE
#define AMP_VDEC_EVENT_DISP_CROP_CHANGE (4096)
#endif

#ifndef _define___AMP_VDEC_EVENT_SCAN_TYPE_CHANGE
#define _define___AMP_VDEC_EVENT_SCAN_TYPE_CHANGE
#define AMP_VDEC_EVENT_SCAN_TYPE_CHANGE (8192)
#endif

#ifndef _typedef___AMP_VDEC_SCAN_TYPE_DESC
#define _typedef___AMP_VDEC_SCAN_TYPE_DESC
typedef struct AMP_VDEC_SCAN_TYPE_DESC {
  UINT32 bIsField;
} AMP_VDEC_SCAN_TYPE_DESC;
#endif /* _typedef___AMP_VDEC_SCAN_TYPE_DESC */

#ifndef _define___AMP_VDEC_EVENT_NOTIFY_LOW_WATERMARK
#define _define___AMP_VDEC_EVENT_NOTIFY_LOW_WATERMARK
#define AMP_VDEC_EVENT_NOTIFY_LOW_WATERMARK (16384)
#endif

#ifndef _define___AMP_VDEC_EVENT_NOTIFY_HIGH_WATERMARK
#define _define___AMP_VDEC_EVENT_NOTIFY_HIGH_WATERMARK
#define AMP_VDEC_EVENT_NOTIFY_HIGH_WATERMARK (32768)
#endif

#ifndef _define___AMP_VDEC_EVENT_NOTIFY_NORMAL_WATERMARK
#define _define___AMP_VDEC_EVENT_NOTIFY_NORMAL_WATERMARK
#define AMP_VDEC_EVENT_NOTIFY_NORMAL_WATERMARK (65536)
#endif

#ifndef _define___AMP_VDEC_EVENT_API_UNDERFLOW
#define _define___AMP_VDEC_EVENT_API_UNDERFLOW
#define AMP_VDEC_EVENT_API_UNDERFLOW (131072)
#endif

#ifndef _define___AMP_VDEC_EVENT_DECODER_FATAL
#define _define___AMP_VDEC_EVENT_DECODER_FATAL
#define AMP_VDEC_EVENT_DECODER_FATAL (262144)
#endif

#ifndef _define___AMP_VDEC_EVENT_HDR_CHANGE
#define _define___AMP_VDEC_EVENT_HDR_CHANGE
#define AMP_VDEC_EVENT_HDR_CHANGE (524288)
#endif

#ifndef _define___AMP_VDEC_EVENT_ALL_MASK
#define _define___AMP_VDEC_EVENT_ALL_MASK
#define AMP_VDEC_EVENT_ALL_MASK (1048575)
#endif

#ifndef _define___AMP_VDEC_MODE_H264_PROGRESSIVE_ONLY
#define _define___AMP_VDEC_MODE_H264_PROGRESSIVE_ONLY
#define AMP_VDEC_MODE_H264_PROGRESSIVE_ONLY (1)
#endif

#ifndef _define___AMP_VDEC_MODE_DPB_FORCE_FLUSH
#define _define___AMP_VDEC_MODE_DPB_FORCE_FLUSH
#define AMP_VDEC_MODE_DPB_FORCE_FLUSH (2)
#endif

#ifndef _define___AMP_VDEC_MODE_MPEG4_SKIP_NONDECODED
#define _define___AMP_VDEC_MODE_MPEG4_SKIP_NONDECODED
#define AMP_VDEC_MODE_MPEG4_SKIP_NONDECODED (4)
#endif

#ifndef _define___AMP_VDEC_MODE_PP_FILTER_MASK
#define _define___AMP_VDEC_MODE_PP_FILTER_MASK
#define AMP_VDEC_MODE_PP_FILTER_MASK (24)
#endif

#ifndef _define___AMP_VDEC_MODE_PP_HSCALE
#define _define___AMP_VDEC_MODE_PP_HSCALE
#define AMP_VDEC_MODE_PP_HSCALE (32)
#endif

#ifndef _define___AMP_VDEC_MODE_LOW_LATENCY
#define _define___AMP_VDEC_MODE_LOW_LATENCY
#define AMP_VDEC_MODE_LOW_LATENCY (64)
#endif

#ifndef _define___AMP_VDEC_MODE_SEI_3D
#define _define___AMP_VDEC_MODE_SEI_3D
#define AMP_VDEC_MODE_SEI_3D (128)
#endif

#ifndef _define___AMP_VDEC_MODE_LOW_LATENCY_TURBO
#define _define___AMP_VDEC_MODE_LOW_LATENCY_TURBO
#define AMP_VDEC_MODE_LOW_LATENCY_TURBO (256)
#endif

#ifndef _define___AMP_VDEC_MODE_FRAME_IN
#define _define___AMP_VDEC_MODE_FRAME_IN
#define AMP_VDEC_MODE_FRAME_IN (512)
#endif

#ifndef _define___AMP_VDEC_MODE_OUTPUT_MASK
#define _define___AMP_VDEC_MODE_OUTPUT_MASK
#define AMP_VDEC_MODE_OUTPUT_MASK (3072)
#endif

#ifndef _define___AMP_VDEC_MODE_DISPLAY_COPY_FRAME
#define _define___AMP_VDEC_MODE_DISPLAY_COPY_FRAME
#define AMP_VDEC_MODE_DISPLAY_COPY_FRAME (4096)
#endif

#ifndef _define___AMP_VDEC_MODE_IDR_MODE
#define _define___AMP_VDEC_MODE_IDR_MODE
#define AMP_VDEC_MODE_IDR_MODE (8192)
#endif

#ifndef _define___AMP_VDEC_MODE_DE_BLOCKING
#define _define___AMP_VDEC_MODE_DE_BLOCKING
#define AMP_VDEC_MODE_DE_BLOCKING (16384)
#endif

#ifndef _define___AMP_VDEC_MODE_DE_INTERLACE
#define _define___AMP_VDEC_MODE_DE_INTERLACE
#define AMP_VDEC_MODE_DE_INTERLACE (32768)
#endif

#ifndef _define___AMP_VDEC_MODE_HIGH_GFX_CLK
#define _define___AMP_VDEC_MODE_HIGH_GFX_CLK
#define AMP_VDEC_MODE_HIGH_GFX_CLK (65536)
#endif

#ifndef _define___AMP_VDEC_MODE_OUTPUT_TILE_MASK
#define _define___AMP_VDEC_MODE_OUTPUT_TILE_MASK
#define AMP_VDEC_MODE_OUTPUT_TILE_MASK (917504)
#endif

#ifndef _define___AMP_VDEC_MODE_OUTPUT_TILE
#define _define___AMP_VDEC_MODE_OUTPUT_TILE
#define AMP_VDEC_MODE_OUTPUT_TILE (131072)
#endif

#ifndef _define___AMP_VDEC_MODE_OUTPUT_TILE_420SP_AUTO
#define _define___AMP_VDEC_MODE_OUTPUT_TILE_420SP_AUTO
#define AMP_VDEC_MODE_OUTPUT_TILE_420SP_AUTO (262144)
#endif

#ifndef _define___AMP_VDEC_MODE_OUTPUT_TILE_MULTI
#define _define___AMP_VDEC_MODE_OUTPUT_TILE_MULTI
#define AMP_VDEC_MODE_OUTPUT_TILE_MULTI (393216)
#endif

#ifndef _define___AMP_VDEC_MODE_FORCE_DITHERING
#define _define___AMP_VDEC_MODE_FORCE_DITHERING
#define AMP_VDEC_MODE_FORCE_DITHERING (1048576)
#endif

#ifndef _define___AMP_VDEC_MODE_KEEP_ORIGIN_PTS
#define _define___AMP_VDEC_MODE_KEEP_ORIGIN_PTS
#define AMP_VDEC_MODE_KEEP_ORIGIN_PTS (2097152)
#endif

#ifndef _define___AMP_VDEC_MODE_MTR_DISABLE
#define _define___AMP_VDEC_MODE_MTR_DISABLE
#define AMP_VDEC_MODE_MTR_DISABLE (4194304)
#endif

#ifndef _define___AMP_VDEC_MODE_SUB_OUTPUT_DS_HALF_QUAT
#define _define___AMP_VDEC_MODE_SUB_OUTPUT_DS_HALF_QUAT
#define AMP_VDEC_MODE_SUB_OUTPUT_DS_HALF_QUAT (8388608)
#endif

#ifndef _define___AMP_VDEC_MODE_SUB_OUTPUT_TILE_SPUV
#define _define___AMP_VDEC_MODE_SUB_OUTPUT_TILE_SPUV
#define AMP_VDEC_MODE_SUB_OUTPUT_TILE_SPUV (16777216)
#endif

#ifndef _define___AMP_VDEC_MODE_4K_EXCLUSIVE
#define _define___AMP_VDEC_MODE_4K_EXCLUSIVE
#define AMP_VDEC_MODE_4K_EXCLUSIVE (33554432)
#endif

#ifndef _define___AMP_VDEC_MODE_MMU_DISABLE
#define _define___AMP_VDEC_MODE_MMU_DISABLE
#define AMP_VDEC_MODE_MMU_DISABLE (67108864)
#endif

#ifndef _define___AMP_VDEC_MODE_MPEG2_DSS_USER_DATA
#define _define___AMP_VDEC_MODE_MPEG2_DSS_USER_DATA
#define AMP_VDEC_MODE_MPEG2_DSS_USER_DATA (134217728)
#endif

#ifndef _define___AMP_VDEC_MODE_FILM_GRAIN_OFF
#define _define___AMP_VDEC_MODE_FILM_GRAIN_OFF
#define AMP_VDEC_MODE_FILM_GRAIN_OFF (268435456)
#endif

#ifndef _typedef___AMP_VDEC_BUF_INFO
#define _typedef___AMP_VDEC_BUF_INFO
typedef struct AMP_VDEC_BUF_INFO {
  UINT32 uiDataCount;
} AMP_VDEC_BUF_INFO;
#endif /* _typedef___AMP_VDEC_BUF_INFO */

HRESULT AMP_VDEC_SetCmdP1(AMP_VDEC _obj, UINT32 uiCmd, UINT32 uiParam, CORBA_Environment *_ev);

HRESULT AMP_VDEC_GetCmdP1(AMP_VDEC _obj, UINT32 uiCmd, UINT32 *uiResult, CORBA_Environment *_ev);

HRESULT AMP_VDEC_SetCmdP2(AMP_VDEC _obj, UINT32 uiCmd, UINT32 uiParam1, UINT32 uiParam2, CORBA_Environment *_ev);

HRESULT AMP_VDEC_GetCmdP2(AMP_VDEC _obj, UINT32 uiCmd, UINT32 *uiResult1, UINT32 *uiResult2, CORBA_Environment *_ev);

HRESULT AMP_VDEC_QueryPortBuf(AMP_VDEC _obj, UINT32 uiPortIdx, struct AMP_VDEC_BUF_INFO *stBufInfo, CORBA_Environment *_ev);

HRESULT AMP_VDEC_GetStatistics(AMP_VDEC _obj, struct AMP_VDEC_PARASTATISTICS *stStatistic, CORBA_Environment *_ev);

HRESULT AMP_VDEC_Open(AMP_VDEC _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_VDEC_Close(AMP_VDEC _obj, CORBA_Environment *_ev);

HRESULT AMP_VDEC_Destroy(AMP_VDEC _obj, CORBA_Environment *_ev);

HRESULT AMP_VDEC_SetState(AMP_VDEC _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_VDEC_GetState(AMP_VDEC _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_VDEC_QueryInfo(AMP_VDEC _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_VDEC_SetupPort(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_VDEC_DynamicSetupPort(AMP_VDEC _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VDEC_DisconnectPorts(AMP_VDEC _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VDEC_ClearPort(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VDEC_QueryPort(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_VDEC_ClearPortBuf(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VDEC_DebugDump(AMP_VDEC _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_VDEC_RegisterNotify(AMP_VDEC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VDEC_UnregisterNotify(AMP_VDEC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VDEC_RegisterNotifyDetail(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VDEC_UnregisterNotifyDetail(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VDEC_RegisterBD(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VDEC_UnregisterBD(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VDEC_PushBD(AMP_VDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _define___AMP_VENC_PIC_TYPE_I
#define _define___AMP_VENC_PIC_TYPE_I
#define AMP_VENC_PIC_TYPE_I (0)
#endif

#ifndef _define___AMP_VENC_PIC_TYPE_P
#define _define___AMP_VENC_PIC_TYPE_P
#define AMP_VENC_PIC_TYPE_P (1)
#endif

#ifndef _define___AMP_VENC_PIC_TYPE_PSKIP
#define _define___AMP_VENC_PIC_TYPE_PSKIP
#define AMP_VENC_PIC_TYPE_PSKIP (2)
#endif

#ifndef _define___AMP_VENC_PIC_TYPE_B
#define _define___AMP_VENC_PIC_TYPE_B
#define AMP_VENC_PIC_TYPE_B (3)
#endif

#ifndef _define___AMP_VENC_PIC_TYPE_BREF
#define _define___AMP_VENC_PIC_TYPE_BREF
#define AMP_VENC_PIC_TYPE_BREF (4)
#endif

#ifndef _define___AMP_VENC_PIC_TYPE_FORCE_IDR
#define _define___AMP_VENC_PIC_TYPE_FORCE_IDR
#define AMP_VENC_PIC_TYPE_FORCE_IDR (5)
#endif

#ifndef _define___AMP_VENC_PIC_TYPE_NA
#define _define___AMP_VENC_PIC_TYPE_NA
#define AMP_VENC_PIC_TYPE_NA (6)
#endif

#ifndef _typedef___AMP_VENC_FRAME_TYPE
#define _typedef___AMP_VENC_FRAME_TYPE
typedef CORBA_enum AMP_VENC_FRAME_TYPE;
#endif /* _typedef___AMP_VENC_FRAME_TYPE */

#ifndef _define___AMP_VENC_VP8_RAW
#define _define___AMP_VENC_VP8_RAW
#define AMP_VENC_VP8_RAW (0)
#endif

#ifndef _define___AMP_VENC_VP8_IVF
#define _define___AMP_VENC_VP8_IVF
#define AMP_VENC_VP8_IVF (1)
#endif

#ifndef _typedef___AMP_VENC_VP8_TYPE
#define _typedef___AMP_VENC_VP8_TYPE
typedef CORBA_enum AMP_VENC_VP8_TYPE;
#endif /* _typedef___AMP_VENC_VP8_TYPE */

#ifndef _define___AMP_VENC_SETBITRATE
#define _define___AMP_VENC_SETBITRATE
#define AMP_VENC_SETBITRATE (0)
#endif

#ifndef _define___AMP_VENC_SETFRAMERATE
#define _define___AMP_VENC_SETFRAMERATE
#define AMP_VENC_SETFRAMERATE (1)
#endif

#ifndef _define___AMP_VENC_FORCEIDR
#define _define___AMP_VENC_FORCEIDR
#define AMP_VENC_FORCEIDR (2)
#endif

#ifndef _typedef___AMP_VENC_CMD
#define _typedef___AMP_VENC_CMD
typedef CORBA_enum AMP_VENC_CMD;
#endif /* _typedef___AMP_VENC_CMD */

#ifndef _typedef___AMP_VENC
#define _typedef___AMP_VENC
typedef CORBA_Object AMP_VENC;
#endif /* _typedef___AMP_VENC */

HRESULT AMP_VENC_ReduceFrameRate(AMP_VENC _obj, UINT32 divisor, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetFrameRate(AMP_VENC _obj, UINT32 uiFrameRateNum, UINT32 uiFrameRateDen, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetBitRate(AMP_VENC _obj, UINT32 uiBitRate, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetForceIDR(AMP_VENC _obj, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetResolution(AMP_VENC _obj, UINT32 uiWidth, UINT32 uiHeight, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetCmdP(AMP_VENC _obj, UINT32 uiCmd, UINT32 uiParam1, UINT32 uiParam2, CORBA_Environment *_ev);

HRESULT AMP_VENC_Open(AMP_VENC _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_VENC_Close(AMP_VENC _obj, CORBA_Environment *_ev);

HRESULT AMP_VENC_Destroy(AMP_VENC _obj, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetState(AMP_VENC _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_VENC_GetState(AMP_VENC _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_VENC_QueryInfo(AMP_VENC _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_VENC_SetupPort(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_VENC_DynamicSetupPort(AMP_VENC _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VENC_DisconnectPorts(AMP_VENC _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VENC_ClearPort(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VENC_QueryPort(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_VENC_ClearPortBuf(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VENC_DebugDump(AMP_VENC _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_VENC_RegisterNotify(AMP_VENC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VENC_UnregisterNotify(AMP_VENC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VENC_RegisterNotifyDetail(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VENC_UnregisterNotifyDetail(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VENC_RegisterBD(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VENC_UnregisterBD(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VENC_PushBD(AMP_VENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_IMGDEC
#define _typedef___AMP_IMGDEC
typedef CORBA_Object AMP_IMGDEC;
#endif /* _typedef___AMP_IMGDEC */

#ifndef _typedef___AMP_IMGDEC_IMGCONF
#define _typedef___AMP_IMGDEC_IMGCONF
typedef struct AMP_IMGDEC_IMGCONF {
  UINT32 uiOutputMode;
  UINT32 uiHScaling;
  UINT32 uiVScaling;
  UINT32 uiROIX;
  UINT32 uiROIY;
  UINT32 uiROIW;
  UINT32 uiROIH;
} AMP_IMGDEC_IMGCONF;
#endif /* _typedef___AMP_IMGDEC_IMGCONF */

#ifndef _typedef___AMP_IMGDEC_IMGINFO
#define _typedef___AMP_IMGDEC_IMGINFO
typedef struct AMP_IMGDEC_IMGINFO {
  UINT32 uiWidth;
  UINT32 uiHeight;
  UINT32 uiOutputW;
  UINT32 uiOutputH;
  UINT32 uiOffset;
  UINT32 uiStride;
  UINT32 uiDisBufSize;
  UINT32 uiDisBufNum;
  UINT32 uiIsProgressive;
  UINT32 uiImgNum;
  UINT32 uiImgSize[4];
  UINT32 uiImgOffset[4];
} AMP_IMGDEC_IMGINFO;
#endif /* _typedef___AMP_IMGDEC_IMGINFO */

HRESULT AMP_IMGDEC_SetConfig(AMP_IMGDEC _obj, struct AMP_IMGDEC_IMGCONF *pImgConf, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_ReadHeader(AMP_IMGDEC _obj, struct AMP_IMGDEC_IMGINFO *pImgInfo, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_Open(AMP_IMGDEC _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_Close(AMP_IMGDEC _obj, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_Destroy(AMP_IMGDEC _obj, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_SetState(AMP_IMGDEC _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_GetState(AMP_IMGDEC _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_QueryInfo(AMP_IMGDEC _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_SetupPort(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_DynamicSetupPort(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_DisconnectPorts(AMP_IMGDEC _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_ClearPort(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_QueryPort(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_ClearPortBuf(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_DebugDump(AMP_IMGDEC _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_RegisterNotify(AMP_IMGDEC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_UnregisterNotify(AMP_IMGDEC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_RegisterNotifyDetail(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_UnregisterNotifyDetail(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_RegisterBD(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_UnregisterBD(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_IMGDEC_PushBD(AMP_IMGDEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _define___AMP_MAX_CHANNELS
#define _define___AMP_MAX_CHANNELS
#define AMP_MAX_CHANNELS (10)
#endif

#ifndef _define___AMP_ASC_BUF_SIZE
#define _define___AMP_ASC_BUF_SIZE
#define AMP_ASC_BUF_SIZE (256)
#endif

#ifndef _define___AMP_AAC_AOT_LC
#define _define___AMP_AAC_AOT_LC
#define AMP_AAC_AOT_LC (2)
#endif

#ifndef _define___AMP_AAC_AOT_LTP
#define _define___AMP_AAC_AOT_LTP
#define AMP_AAC_AOT_LTP (4)
#endif

#ifndef _define___AMP_AAC_AOT_HE
#define _define___AMP_AAC_AOT_HE
#define AMP_AAC_AOT_HE (5)
#endif

#ifndef _define___AMP_AAC_AOT_HE_PS
#define _define___AMP_AAC_AOT_HE_PS
#define AMP_AAC_AOT_HE_PS (29)
#endif

#ifndef _define___AMP_AAC_AOT_USAC
#define _define___AMP_AAC_AOT_USAC
#define AMP_AAC_AOT_USAC (42)
#endif

#ifndef _define___AMP_MPEG_CHMODE_MONO
#define _define___AMP_MPEG_CHMODE_MONO
#define AMP_MPEG_CHMODE_MONO (0)
#endif

#ifndef _define___AMP_MPEG_CHMODE_DUAL_MONO
#define _define___AMP_MPEG_CHMODE_DUAL_MONO
#define AMP_MPEG_CHMODE_DUAL_MONO (1)
#endif

#ifndef _define___AMP_MPEG_CHMODE_STEREO
#define _define___AMP_MPEG_CHMODE_STEREO
#define AMP_MPEG_CHMODE_STEREO (2)
#endif

#ifndef _define___AMP_MPEG_CHMODE_JOINT_STEREO
#define _define___AMP_MPEG_CHMODE_JOINT_STEREO
#define AMP_MPEG_CHMODE_JOINT_STEREO (3)
#endif

#ifndef _define___AMP_AAC_MPEG4
#define _define___AMP_AAC_MPEG4
#define AMP_AAC_MPEG4 (0)
#endif

#ifndef _define___AMP_AAC_MPEG2
#define _define___AMP_AAC_MPEG2
#define AMP_AAC_MPEG2 (1)
#endif

#ifndef _define___AMP_AAC_SF_MP2ADTS
#define _define___AMP_AAC_SF_MP2ADTS
#define AMP_AAC_SF_MP2ADTS (0)
#endif

#ifndef _define___AMP_AAC_SF_MP4ADTS
#define _define___AMP_AAC_SF_MP4ADTS
#define AMP_AAC_SF_MP4ADTS (1)
#endif

#ifndef _define___AMP_AAC_SF_ADIF
#define _define___AMP_AAC_SF_ADIF
#define AMP_AAC_SF_ADIF (4)
#endif

#ifndef _define___AMP_AAC_SF_RAW
#define _define___AMP_AAC_SF_RAW
#define AMP_AAC_SF_RAW (6)
#endif

#ifndef _define___AMP_AAC_SF_LOAS
#define _define___AMP_AAC_SF_LOAS
#define AMP_AAC_SF_LOAS (8)
#endif

#ifndef _define___AMP_AAC_SF_AUTO
#define _define___AMP_AAC_SF_AUTO
#define AMP_AAC_SF_AUTO (10)
#endif

#ifndef _define___AMP_MAX_LATM_CFG_SIZE
#define _define___AMP_MAX_LATM_CFG_SIZE
#define AMP_MAX_LATM_CFG_SIZE (16)
#endif

#ifndef _define___AMP_M4ALATM_LAYER_LATM
#define _define___AMP_M4ALATM_LAYER_LATM
#define AMP_M4ALATM_LAYER_LATM (0)
#endif

#ifndef _define___AMP_M4ALATM_LAYER_LOAS
#define _define___AMP_M4ALATM_LAYER_LOAS
#define AMP_M4ALATM_LAYER_LOAS (1)
#endif

#ifndef _define___AMP_M4ALATM_LATMFMT_AME
#define _define___AMP_M4ALATM_LATMFMT_AME
#define AMP_M4ALATM_LATMFMT_AME (0)
#endif

#ifndef _define___AMP_M4ALATM_LATMFMT_EPME
#define _define___AMP_M4ALATM_LATMFMT_EPME
#define AMP_M4ALATM_LATMFMT_EPME (1)
#endif

#ifndef _define___AMP_M4ALATM_LOASFMT_ASS
#define _define___AMP_M4ALATM_LOASFMT_ASS
#define AMP_M4ALATM_LOASFMT_ASS (2)
#endif

#ifndef _define___AMP_M4ALATM_LOASFMT_EPASS
#define _define___AMP_M4ALATM_LOASFMT_EPASS
#define AMP_M4ALATM_LOASFMT_EPASS (3)
#endif

#ifndef _define___AMP_M4ALATM_LOASFMT_APS
#define _define___AMP_M4ALATM_LOASFMT_APS
#define AMP_M4ALATM_LOASFMT_APS (4)
#endif

#ifndef _define___AMP_MS11_OM_PARAIDX
#define _define___AMP_MS11_OM_PARAIDX
#define AMP_MS11_OM_PARAIDX (0)
#endif

#ifndef _define___AMP_MS11_OMS_PARAIDX
#define _define___AMP_MS11_OMS_PARAIDX
#define AMP_MS11_OMS_PARAIDX (1)
#endif

#ifndef _define___AMP_MS11_OUT_PARAMAX
#define _define___AMP_MS11_OUT_PARAMAX
#define AMP_MS11_OUT_PARAMAX (2)
#endif

#ifndef _define___AMP_MS11_EXTPCM_PRL_MIN
#define _define___AMP_MS11_EXTPCM_PRL_MIN
#define AMP_MS11_EXTPCM_PRL_MIN (0)
#endif

#ifndef _define___AMP_MS11_EXTPCM_PRL_MAX
#define _define___AMP_MS11_EXTPCM_PRL_MAX
#define AMP_MS11_EXTPCM_PRL_MAX (31)
#endif

#ifndef _typedef___AMP_ADEC
#define _typedef___AMP_ADEC
typedef CORBA_Object AMP_ADEC;
#endif /* _typedef___AMP_ADEC */

#ifndef _typedef___AMP_ADEC_FLAG
#define _typedef___AMP_ADEC_FLAG
typedef UINT32 AMP_ADEC_FLAG;
#endif /* _typedef___AMP_ADEC_FLAG */

#ifndef _define___AMP_ADEC_FLAG_PRO
#define _define___AMP_ADEC_FLAG_PRO
#define AMP_ADEC_FLAG_PRO (1)
#endif

#ifndef _define___AMP_ADEC_FLAG_DEMO
#define _define___AMP_ADEC_FLAG_DEMO
#define AMP_ADEC_FLAG_DEMO (2)
#endif

#ifndef _define___AMP_ADEC_FLAG_METADATA_DISABLE
#define _define___AMP_ADEC_FLAG_METADATA_DISABLE
#define AMP_ADEC_FLAG_METADATA_DISABLE (4)
#endif

#ifndef _define___AMP_ADEC_MPEG_STEREO
#define _define___AMP_ADEC_MPEG_STEREO
#define AMP_ADEC_MPEG_STEREO (0)
#endif

#ifndef _define___AMP_ADEC_MPEG_DUAL_MONO_A
#define _define___AMP_ADEC_MPEG_DUAL_MONO_A
#define AMP_ADEC_MPEG_DUAL_MONO_A (1)
#endif

#ifndef _define___AMP_ADEC_MPEG_DUAL_MONO_B
#define _define___AMP_ADEC_MPEG_DUAL_MONO_B
#define AMP_ADEC_MPEG_DUAL_MONO_B (2)
#endif

#ifndef _define___AMP_ADEC_INPORT_MAX
#define _define___AMP_ADEC_INPORT_MAX
#define AMP_ADEC_INPORT_MAX (2)
#endif

#ifndef _define___AMP_ADEC_NONE_MASK
#define _define___AMP_ADEC_NONE_MASK
#define AMP_ADEC_NONE_MASK (0)
#endif

#ifndef _define___AMP_ADEC_KEEP_ORIGINAL_PTS
#define _define___AMP_ADEC_KEEP_ORIGINAL_PTS
#define AMP_ADEC_KEEP_ORIGINAL_PTS (1)
#endif

#ifndef _define___AMP_ADEC_ALL_MASK
#define _define___AMP_ADEC_ALL_MASK
#define AMP_ADEC_ALL_MASK (1048575)
#endif

#ifndef _typedef___AMP_ADEC_PARADDPTYPE
#define _typedef___AMP_ADEC_PARADDPTYPE
typedef struct AMP_ADEC_PARADDPTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT16 unCompMode;
  UINT16 unKMode;
  UINT16 unStereoMode;
  UINT16 unDualMode;
  UINT32 uiPcmScale;
  UINT32 uiDrcHigh;
  UINT32 uiDrcLow;
  UINT32 uiBsType;
  UINT32 uiChanMask;
  UINT32 uiOrigChanMask;
  UINT32 uiStreamId;
  UINT32 uiDecFlags;
} AMP_ADEC_PARADDPTYPE;
#endif /* _typedef___AMP_ADEC_PARADDPTYPE */

#ifndef _typedef___AMP_ADEC_PARATHDTYPE
#define _typedef___AMP_ADEC_PARATHDTYPE
typedef struct AMP_ADEC_PARATHDTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT16 unDrcEnable;
  UINT32 uiDrcBoost;
  UINT32 uiDrcCut;
  UINT32 uiDialRef;
  UINT16 unFbaChannelOrder;
  UINT16 unDialRefSupplied;
  UINT32 uiConfig;
  UINT32 uiContentType;
  UINT32 uiChanMask;
  UINT32 uiOrigChanMask;
} AMP_ADEC_PARATHDTYPE;
#endif /* _typedef___AMP_ADEC_PARATHDTYPE */

#ifndef _typedef___AMP_ADEC_PARADTSTYPE
#define _typedef___AMP_ADEC_PARADTSTYPE
typedef struct AMP_ADEC_PARADTSTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  INT8 cEnableDialNorm;
  INT8 cEnableDrc;
  UINT16 unDrcRatio;
  INT8 cLfeMix2Frnt;
  UINT32 uiDecOutFreq;
  INT8 cCoreInterpx2Flag;
  INT8 cEnblDecDefauitSpkrRemap;
  UINT8 ucExtSSAudioPresIdx;
  UINT8 ucAudioPresIdx;
  UINT8 ucSndFieldSpkrConfigIdx;
  UINT8 ucSpecialDecMode;
  INT8 cEmb2ChDwnMix;
  UINT8 ucReplaceChSet[3];
  INT8 cSkipDownMix;
  INT8 cSkipPostProcess;
  UINT32 uiStreamFmt;
  UINT32 uiPlaybackMode;
  UINT32 uiChanMask;
  UINT32 uiOrigChanMask;
  INT8 cDtsWav;
  UINT32 uiSpkrMode;
} AMP_ADEC_PARADTSTYPE;
#endif /* _typedef___AMP_ADEC_PARADTSTYPE */

#ifndef _typedef___AMP_ADEC_PARAMPGTYPE
#define _typedef___AMP_ADEC_PARAMPGTYPE
typedef struct AMP_ADEC_PARAMPGTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT8 ucLayer;
  UINT8 ucVersion;
  UINT8 ucEmphasis;
  UINT32 uiChanMask;
  UINT32 uiDualMode;
} AMP_ADEC_PARAMPGTYPE;
#endif /* _typedef___AMP_ADEC_PARAMPGTYPE */

#ifndef _typedef___AMP_ADEC_PARAMP3TYPE
#define _typedef___AMP_ADEC_PARAMP3TYPE
typedef struct AMP_ADEC_PARAMP3TYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT8 ucLayer;
  UINT8 ucVersion;
  UINT8 ucEmphasis;
  UINT32 uiChanMask;
} AMP_ADEC_PARAMP3TYPE;
#endif /* _typedef___AMP_ADEC_PARAMP3TYPE */

#ifndef _typedef___AMP_ADEC_PARAOPUSTYPE
#define _typedef___AMP_ADEC_PARAOPUSTYPE
typedef struct AMP_ADEC_PARAOPUSTYPE {
  UINT32 uiInThresh;
  UINT32 fs;
  UINT32 chanNum;
  UINT32 inbandfec;
  UINT32 bsType;
  UINT32 maxPageSize;
  UINT32 rangeDecState;
  UINT32 version;
  UINT32 preskip;
  UINT32 gain;
  UINT32 nb_streams;
  UINT32 nb_coupled;
  UINT32 channel_mapping;
  UINT8 map[255];
} AMP_ADEC_PARAOPUSTYPE;
#endif /* _typedef___AMP_ADEC_PARAOPUSTYPE */

#ifndef _typedef___AMP_ADEC_PARAAACTYPE
#define _typedef___AMP_ADEC_PARAAACTYPE
typedef struct AMP_ADEC_PARAAACTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT8 ucProfileType;
  UINT8 ucStreamFmt;
  UINT8 ucLayer;
  UINT8 ucVersion;
  UINT32 uiChanMask;
  UINT32 uiChanMapValid;
  UINT32 uiChanMap[10];
} AMP_ADEC_PARAAACTYPE;
#endif /* _typedef___AMP_ADEC_PARAAACTYPE */

#ifndef _typedef___AMP_ADEC_PARAWMATYPE
#define _typedef___AMP_ADEC_PARAWMATYPE
typedef struct AMP_ADEC_PARAWMATYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT16 unFormatTag;
  INT16 nBlockAlign;
  INT32 iAvgBytesPerSec;
  INT16 nCbSize;
  UINT8 ucExtraData[32];
  INT16 nRmsAmplitudeRef;
  INT16 nRmsAmplitudeTarget;
  INT16 nPeakAmplitudeRef;
  INT16 nPeakAmplitudeTarget;
  INT16 nPad;
  INT32 iDecoderFlags;
  INT32 iPacketSize;
  UINT32 uiChanMask;
} AMP_ADEC_PARAWMATYPE;
#endif /* _typedef___AMP_ADEC_PARAWMATYPE */

#ifndef _typedef___AMP_ADEC_PARAAMRNBTYPE
#define _typedef___AMP_ADEC_PARAAMRNBTYPE
typedef struct AMP_ADEC_PARAAMRNBTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT16 unFormatId;
  UINT16 unFrameType;
  UINT32 uiChanMask;
} AMP_ADEC_PARAAMRNBTYPE;
#endif /* _typedef___AMP_ADEC_PARAAMRNBTYPE */

#ifndef _typedef___AMP_ADEC_PARAAMRWBTYPE
#define _typedef___AMP_ADEC_PARAAMRWBTYPE
typedef struct AMP_ADEC_PARAAMRWBTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT16 unFormatId;
  UINT32 uiChanMask;
} AMP_ADEC_PARAAMRWBTYPE;
#endif /* _typedef___AMP_ADEC_PARAAMRWBTYPE */

#ifndef _typedef___AMP_ADEC_PARAADPCMTYPE
#define _typedef___AMP_ADEC_PARAADPCMTYPE
typedef struct AMP_ADEC_PARAADPCMTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT16 unBlockAlign;
  UINT16 unBitsPerSample;
  UINT32 uiChanMask;
} AMP_ADEC_PARAADPCMTYPE;
#endif /* _typedef___AMP_ADEC_PARAADPCMTYPE */

#ifndef _typedef___AMP_ADEC_PARARA8TYPE
#define _typedef___AMP_ADEC_PARARA8TYPE
typedef struct AMP_ADEC_PARARA8TYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT16 unLfeMode;
  UINT32 uiCodec4CC;
  UINT32 uiActualRate;
  UINT16 unAudioQuality;
  UINT16 unFlavorIndex;
  UINT32 uiBitsPerFrame;
  UINT32 uiGranularity;
  UINT32 uiOpaqueDataSize;
  UINT8 ucOpaqueData[32];
  UINT32 uiChanMask;
} AMP_ADEC_PARARA8TYPE;
#endif /* _typedef___AMP_ADEC_PARARA8TYPE */

#ifndef _typedef___AMP_ADEC_PARAPCMTYPE
#define _typedef___AMP_ADEC_PARAPCMTYPE
typedef struct AMP_ADEC_PARAPCMTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT16 unChanMode;
  UINT32 uiChanMap[10];
  UINT32 uiPcmType;
  INT8 cLittleEndian;
  UINT16 unCDDADeeType;
  UINT32 uiChanMask;
} AMP_ADEC_PARAPCMTYPE;
#endif /* _typedef___AMP_ADEC_PARAPCMTYPE */

#ifndef _typedef___AMP_ADEC_PARALPCMSDTYPE
#define _typedef___AMP_ADEC_PARALPCMSDTYPE
typedef struct AMP_ADEC_PARALPCMSDTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiChanAssign;
  UINT32 uiPcmType;
  INT8 cLittleEndian;
  INT8 cDrcApplied;
  INT32 iDrcExp;
  INT32 iDrcMant;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
} AMP_ADEC_PARALPCMSDTYPE;
#endif /* _typedef___AMP_ADEC_PARALPCMSDTYPE */

#ifndef _typedef___AMP_ADEC_PARALPCMHDTYPE
#define _typedef___AMP_ADEC_PARALPCMHDTYPE
typedef struct AMP_ADEC_PARALPCMHDTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiChanAssign;
  UINT32 uiPcmType;
  INT8 cLittleEndian;
  INT8 cDrcApplied;
  INT32 iDrcExp;
  INT32 iDrcMant;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
} AMP_ADEC_PARALPCMHDTYPE;
#endif /* _typedef___AMP_ADEC_PARALPCMHDTYPE */

#ifndef _typedef___AMP_ADEC_PARALPCMBDTYPE
#define _typedef___AMP_ADEC_PARALPCMBDTYPE
typedef struct AMP_ADEC_PARALPCMBDTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiChanAssign;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
} AMP_ADEC_PARALPCMBDTYPE;
#endif /* _typedef___AMP_ADEC_PARALPCMBDTYPE */

#ifndef _typedef___AMP_ADEC_PARAFLACTYPE
#define _typedef___AMP_ADEC_PARAFLACTYPE
typedef struct AMP_ADEC_PARAFLACTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiChanMask;
} AMP_ADEC_PARAFLACTYPE;
#endif /* _typedef___AMP_ADEC_PARAFLACTYPE */

#ifndef _typedef___AMP_ADEC_PARALPCMSESFTYPE
#define _typedef___AMP_ADEC_PARALPCMSESFTYPE
typedef struct AMP_ADEC_PARALPCMSESFTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiChanAssign;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
} AMP_ADEC_PARALPCMSESFTYPE;
#endif /* _typedef___AMP_ADEC_PARALPCMSESFTYPE */

#ifndef _typedef___AMP_ADEC_PARADVSDTYPE
#define _typedef___AMP_ADEC_PARADVSDTYPE
typedef struct AMP_ADEC_PARADVSDTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiSysType;
  UINT32 uiSctNr;
  UINT32 uiQuant;
  UINT32 uiChanMask;
} AMP_ADEC_PARADVSDTYPE;
#endif /* _typedef___AMP_ADEC_PARADVSDTYPE */

#ifndef _typedef___AMP_ADEC_PARAG711TYPE
#define _typedef___AMP_ADEC_PARAG711TYPE
typedef struct AMP_ADEC_PARAG711TYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
} AMP_ADEC_PARAG711TYPE;
#endif /* _typedef___AMP_ADEC_PARAG711TYPE */

#ifndef _typedef___AMP_ADEC_PARADTSDUMMYTYPE
#define _typedef___AMP_ADEC_PARADTSDUMMYTYPE
typedef struct AMP_ADEC_PARADTSDUMMYTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
  INT8 cDtsWav;
} AMP_ADEC_PARADTSDUMMYTYPE;
#endif /* _typedef___AMP_ADEC_PARADTSDUMMYTYPE */

#ifndef _typedef___AMP_ADEC_PARAM4ALATMTYPE
#define _typedef___AMP_ADEC_PARAM4ALATMTYPE
typedef struct AMP_ADEC_PARAM4ALATMTYPE {
  UINT32 uiInThresh;
  UINT32 uiSampleRate;
  UINT32 uiBitRate;
  UINT16 unBitDepth;
  UINT16 unChanNr;
  UINT32 uiBlkSize;
  UINT32 uiChanMask;
  UINT32 m_Layer;
  UINT32 m_Format;
  UINT32 m_MuxConfigPresent;
  UINT8 m_ConfigData[16];
  UINT8 m_configDataSize;
} AMP_ADEC_PARAM4ALATMTYPE;
#endif /* _typedef___AMP_ADEC_PARAM4ALATMTYPE */

#ifndef _typedef___AMP_ADEC_PARAMS11DDCTYPE
#define _typedef___AMP_ADEC_PARAMS11DDCTYPE
typedef struct AMP_ADEC_PARAMS11DDCTYPE {
  INT16 nDualInput;
  INT16 nDualDecode;
  INT16 nOutputMain;
  INT16 nOutputSec;
  INT32 iSubStreamSelect;
  INT32 iUserBalanceAdj;
  INT32 iDrcMode[2];
  INT32 iDrcLow[2];
  INT32 iDrcHigh[2];
  INT32 iStereoMode;
  INT32 iDualMode;
  INT32 iKMode;
  INT32 iChanMode;
  INT32 iLfeMode;
  INT32 iDDMultiChEnable;
  INT32 nMixerEnabled;
  UINT32 uiRTParaMask;
  UINT32 uiDecFlags;
  UINT32 uiSampleRate[2];
  UINT32 uiBitRate[2];
  UINT32 uiChanNr[2];
  UINT32 uiBsType[2];
  UINT32 uiChanMask[2];
  UINT32 uiOrigChanMask[2];
} AMP_ADEC_PARAMS11DDCTYPE;
#endif /* _typedef___AMP_ADEC_PARAMS11DDCTYPE */

#ifndef _typedef___AMP_ADEC_PARAMS11DDRETYPE
#define _typedef___AMP_ADEC_PARAMS11DDRETYPE
typedef struct AMP_ADEC_PARAMS11DDRETYPE {
  INT32 iDataFmt;
  INT32 iBitDepth;
  INT32 iChanMode;
  INT32 iLfeMode;
  INT32 iDDMultiChEnable;
  UINT32 uiSampleRate;
  UINT32 uiBitRateIdx;
  UINT32 uiChanNr;
  UINT32 uiChanMask[6];
  INT32 iTestMode;
  INT32 iDrcMode[2];
  INT32 iDrcLow[2];
  INT32 iDrcHigh[2];
  INT32 iExtPcmProgRefLevel;
} AMP_ADEC_PARAMS11DDRETYPE;
#endif /* _typedef___AMP_ADEC_PARAMS11DDRETYPE */

#ifndef _typedef___AMP_ADEC_PARAMS11DDTTYPE
#define _typedef___AMP_ADEC_PARAMS11DDTTYPE
typedef struct AMP_ADEC_PARAMS11DDTTYPE {
  INT16 nDualInput;
  INT16 nOutputMain;
  INT16 nOutputSec;
  INT16 nTestConformanceMode;
  INT16 nMixerEnabled;
  INT16 nTransportFormat;
  INT32 iUserBalanceAdj;
  INT32 iDrcMode[2];
  INT32 iDrcLow[2];
  INT32 iDrcHigh[2];
  INT32 iDialNorm;
  INT32 iStereoMode;
  INT32 iDualMode;
  INT32 iChanMode;
  INT32 iLfeMode;
  UINT32 uiDecFlags;
  INT32 iChannelConfig;
  INT32 iSampleRateIdx;
  UINT32 uiRTParaMask;
  UINT32 uiSampleRate[2];
  UINT32 uiBitRate[2];
  UINT32 uiChanNr[2];
  UINT32 uiBsType[2];
  UINT32 uiChanMask[2];
  UINT32 uiOrigChanMask[2];
} AMP_ADEC_PARAMS11DDTTYPE;
#endif /* _typedef___AMP_ADEC_PARAMS11DDTTYPE */

#ifndef _define___AMP_ADEC_MS11PARA_RT_SUBSTREAMSELECT
#define _define___AMP_ADEC_MS11PARA_RT_SUBSTREAMSELECT
#define AMP_ADEC_MS11PARA_RT_SUBSTREAMSELECT (1)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_USERBALANCEADJ
#define _define___AMP_ADEC_MS11PARA_RT_USERBALANCEADJ
#define AMP_ADEC_MS11PARA_RT_USERBALANCEADJ (2)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCMODE_MAIN
#define _define___AMP_ADEC_MS11PARA_RT_DRCMODE_MAIN
#define AMP_ADEC_MS11PARA_RT_DRCMODE_MAIN (4)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCMODE_ASSOC
#define _define___AMP_ADEC_MS11PARA_RT_DRCMODE_ASSOC
#define AMP_ADEC_MS11PARA_RT_DRCMODE_ASSOC (8)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCLOW_MAIN
#define _define___AMP_ADEC_MS11PARA_RT_DRCLOW_MAIN
#define AMP_ADEC_MS11PARA_RT_DRCLOW_MAIN (16)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCLOW_ASSOC
#define _define___AMP_ADEC_MS11PARA_RT_DRCLOW_ASSOC
#define AMP_ADEC_MS11PARA_RT_DRCLOW_ASSOC (32)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCHIGH_MAIN
#define _define___AMP_ADEC_MS11PARA_RT_DRCHIGH_MAIN
#define AMP_ADEC_MS11PARA_RT_DRCHIGH_MAIN (64)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCHIGH_ASSOC
#define _define___AMP_ADEC_MS11PARA_RT_DRCHIGH_ASSOC
#define AMP_ADEC_MS11PARA_RT_DRCHIGH_ASSOC (128)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_STEREOMODE
#define _define___AMP_ADEC_MS11PARA_RT_STEREOMODE
#define AMP_ADEC_MS11PARA_RT_STEREOMODE (256)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DUALMODE
#define _define___AMP_ADEC_MS11PARA_RT_DUALMODE
#define AMP_ADEC_MS11PARA_RT_DUALMODE (512)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_MULTICHENABLE
#define _define___AMP_ADEC_MS11PARA_RT_MULTICHENABLE
#define AMP_ADEC_MS11PARA_RT_MULTICHENABLE (1024)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_MIXERENABLE
#define _define___AMP_ADEC_MS11PARA_RT_MIXERENABLE
#define AMP_ADEC_MS11PARA_RT_MIXERENABLE (2048)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DUALDECODE
#define _define___AMP_ADEC_MS11PARA_RT_DUALDECODE
#define AMP_ADEC_MS11PARA_RT_DUALDECODE (1073741824)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCMODE
#define _define___AMP_ADEC_MS11PARA_RT_DRCMODE
#define AMP_ADEC_MS11PARA_RT_DRCMODE (12)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCRATIO_MAIN
#define _define___AMP_ADEC_MS11PARA_RT_DRCRATIO_MAIN
#define AMP_ADEC_MS11PARA_RT_DRCRATIO_MAIN (80)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCRATIO_ASSOC
#define _define___AMP_ADEC_MS11PARA_RT_DRCRATIO_ASSOC
#define AMP_ADEC_MS11PARA_RT_DRCRATIO_ASSOC (160)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_DRCRATIO
#define _define___AMP_ADEC_MS11PARA_RT_DRCRATIO
#define AMP_ADEC_MS11PARA_RT_DRCRATIO (240)
#endif

#ifndef _define___AMP_ADEC_MS11PARA_RT_ALL
#define _define___AMP_ADEC_MS11PARA_RT_ALL
#define AMP_ADEC_MS11PARA_RT_ALL (4095)
#endif

#ifndef _typedef___AMP_ADEC_PARAMS12AC4TYPE
#define _typedef___AMP_ADEC_PARAMS12AC4TYPE
typedef struct AMP_ADEC_PARAMS12AC4TYPE {
  INT8 iDualType;
  INT32 iAC4Mode;
  INT32 bDapNode;
  INT32 iDapMode;
  INT32 iOutChanMode;
  INT32 iOutChanConfig[2];
  INT32 iPresSelectMode;
  BOOL bAssocMixing;
  INT8 cFirstPrefLang[4];
  INT8 cSeccondPrefLang[4];
  INT32 iPrefAssocType;
  BOOL bPrefAssocTypeOverLang;
  BOOL bSelectByPresIndex;
  INT32 iPresIndex;
  BOOL bLockToProgId;
  INT32 bProgramIdIsUuid;
  UINT8 uiProgramId[16];
  UINT32 uiOutType;
} AMP_ADEC_PARAMS12AC4TYPE;
#endif /* _typedef___AMP_ADEC_PARAMS12AC4TYPE */

#ifndef _typedef___AMP_ADEC_PARAMS12DDPTYPE
#define _typedef___AMP_ADEC_PARAMS12DDPTYPE
typedef struct AMP_ADEC_PARAMS12DDPTYPE {
  BOOL bDualInput;
  INT8 iDualType;
  INT32 bDapNode;
  INT32 iDapMode;
  BOOL bLegacyDdplusOut;
  INT8 uiMaxOutputChannel;
  INT8 uiJocDecoding[2];
  INT8 uiMlpOnly[2];
  INT8 uiFilePlayback[2];
  UINT32 uiNumObject[2];
  INT32 uiDrcCutMc[2];
  INT32 uiDrcboostMc[2];
  INT32 uiDrcCut2c[2];
  INT32 uiDrcboost2c[2];
  INT32 uiDrcMode[2];
  INT32 uiChanNum[2];
  INT32 uiChanMode[2];
  INT32 uiLfeMode[2];
} AMP_ADEC_PARAMS12DDPTYPE;
#endif /* _typedef___AMP_ADEC_PARAMS12DDPTYPE */

#ifndef _typedef___AMP_ADEC_PARAMS12MATTYPE
#define _typedef___AMP_ADEC_PARAMS12MATTYPE
typedef struct AMP_ADEC_PARAMS12MATTYPE {
  BOOL iDualType;
  INT32 bDapNode;
  INT32 iDapMode;
  INT8 uiMlpOnly[2];
  UINT32 uiNumObject[2];
  INT32 uiDrcCutMc[2];
  INT32 uiDrcboostMc[2];
  INT32 uiDrcCut2c[2];
  INT32 uiDrcboost2c[2];
  INT32 uiDrcMode[2];
  INT32 uiChanNum[2];
  INT32 uiChanMode[2];
  INT32 uiLfeMode[2];
} AMP_ADEC_PARAMS12MATTYPE;
#endif /* _typedef___AMP_ADEC_PARAMS12MATTYPE */

#ifndef _typedef___AMP_ADEC_PARAMS12AACTYPE
#define _typedef___AMP_ADEC_PARAMS12AACTYPE
typedef struct AMP_ADEC_PARAMS12AACTYPE {
  BOOL bDualInput;
  INT8 iDualType;
  INT32 bDapNode;
  INT32 iDapMode;
  UINT32 uiSampleRate;
  INT32 uiAACDialnorm[2];
  INT8 uiAACAribmode[2];
  INT8 uiFilePlayback[2];
  UINT32 uiHeaacDefaultDialnorm;
  INT32 uiDrcCutMc[2];
  INT32 uiDrcboostMc[2];
  INT32 uiDrcCut2c[2];
  INT32 uiDrcboost2c[2];
  INT32 uiDrcMode[2];
  INT32 uiChanNum[2];
  INT32 uiChanMode[2];
  INT32 uiLfeMode[2];
} AMP_ADEC_PARAMS12AACTYPE;
#endif /* _typedef___AMP_ADEC_PARAMS12AACTYPE */

#ifndef _typedef___AMP_ADEC_PARADDUDCTYPE
#define _typedef___AMP_ADEC_PARADDUDCTYPE
typedef struct AMP_ADEC_PARADDUDCTYPE {
  UINT32 uiInThresh;
  INT32 iRunningMode;
  INT32 iInputMode;
  INT32 iAssocIdSelect;
  BOOL bDecErrConcealFlag;
  INT32 iDecErrConcealType;
  BOOL bCnvErrConcealFlag;
  BOOL bEvoHashFlag;
  BOOL bIsEvolutionQuickAccess;
  INT32 iEvoQuickAccessSubstreamId;
  INT32 iEvoQuickAccessStrmType;
  BOOL bJocForceDownmix;
  INT32 iMixerMode;
  INT32 iMixerUserPref;
  INT32 iCompMode[2];
  INT32 iDynScaleHigh[2];
  INT32 iDynScaleLow[2];
  INT32 iDualMode[2];
  INT32 iChanMode[2];
  INT32 iLfeMode[2];
  INT32 iPcmScale[2];
  INT32 iStereoMode[2];
  BOOL bMdctBandLimit[2];
  INT32 iDecOrrMode[2];
  INT32 iMisdSubstreamIdx[2];
  INT32 iChanNum[2];
} AMP_ADEC_PARADDUDCTYPE;
#endif /* _typedef___AMP_ADEC_PARADDUDCTYPE */

#ifndef _typedef___AMP_ADEC_PARAOUTFMT
#define _typedef___AMP_ADEC_PARAOUTFMT
typedef struct AMP_ADEC_PARAOUTFMT {
  BOOL bIsFmtValid;
  UINT32 uiOutPortIdx;
  AMP_AUDIO_DATA_FMT_TYPE eDataFmt;
  UINT32 uiBitDepth;
} AMP_ADEC_PARAOUTFMT;
#endif /* _typedef___AMP_ADEC_PARAOUTFMT */

#ifndef _typedef___AMP_ADEC_PARAPASSTHRCFG
#define _typedef___AMP_ADEC_PARAPASSTHRCFG
typedef struct AMP_ADEC_PARAPASSTHRCFG {
  UINT32 uiEventType;
  UINT32 uiEventData;
} AMP_ADEC_PARAPASSTHRCFG;
#endif /* _typedef___AMP_ADEC_PARAPASSTHRCFG */

#ifndef _typedef___AMP_ADEC_PARAINTHRESHCFG
#define _typedef___AMP_ADEC_PARAINTHRESHCFG
typedef struct AMP_ADEC_PARAINTHRESHCFG {
  UINT32 uiInPortIdx;
  UINT32 uiInThresh;
} AMP_ADEC_PARAINTHRESHCFG;
#endif /* _typedef___AMP_ADEC_PARAINTHRESHCFG */

#ifndef _typedef___AMP_ADEC_PARAADCFG
#define _typedef___AMP_ADEC_PARAADCFG
typedef struct AMP_ADEC_PARAADCFG {
  BOOL bADEnable;
} AMP_ADEC_PARAADCFG;
#endif /* _typedef___AMP_ADEC_PARAADCFG */

#ifndef _typedef___AMP_ADEC_PARASTATISTICS
#define _typedef___AMP_ADEC_PARASTATISTICS
typedef struct AMP_ADEC_PARASTATISTICS {
  UINT32 decoder_pts_high;
  UINT32 decoder_pts_low;
  UINT32 num_decoded_samples;
  UINT32 num_decoder_errors;
} AMP_ADEC_PARASTATISTICS;
#endif /* _typedef___AMP_ADEC_PARASTATISTICS */

#ifndef _define___AMP_ADEC_PARAIDX_DDP
#define _define___AMP_ADEC_PARAIDX_DDP
#define AMP_ADEC_PARAIDX_DDP (0)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_THD
#define _define___AMP_ADEC_PARAIDX_THD
#define AMP_ADEC_PARAIDX_THD (1)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_DTS
#define _define___AMP_ADEC_PARAIDX_DTS
#define AMP_ADEC_PARAIDX_DTS (2)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MPG
#define _define___AMP_ADEC_PARAIDX_MPG
#define AMP_ADEC_PARAIDX_MPG (3)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MP3
#define _define___AMP_ADEC_PARAIDX_MP3
#define AMP_ADEC_PARAIDX_MP3 (4)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_AAC
#define _define___AMP_ADEC_PARAIDX_AAC
#define AMP_ADEC_PARAIDX_AAC (5)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_WMA
#define _define___AMP_ADEC_PARAIDX_WMA
#define AMP_ADEC_PARAIDX_WMA (6)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_RAWPCM
#define _define___AMP_ADEC_PARAIDX_RAWPCM
#define AMP_ADEC_PARAIDX_RAWPCM (7)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_LPCMSD
#define _define___AMP_ADEC_PARAIDX_LPCMSD
#define AMP_ADEC_PARAIDX_LPCMSD (8)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_LPCMHD
#define _define___AMP_ADEC_PARAIDX_LPCMHD
#define AMP_ADEC_PARAIDX_LPCMHD (9)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_LPCMBD
#define _define___AMP_ADEC_PARAIDX_LPCMBD
#define AMP_ADEC_PARAIDX_LPCMBD (10)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_LPCMSESF
#define _define___AMP_ADEC_PARAIDX_LPCMSESF
#define AMP_ADEC_PARAIDX_LPCMSESF (11)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_VORBIS
#define _define___AMP_ADEC_PARAIDX_VORBIS
#define AMP_ADEC_PARAIDX_VORBIS (12)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_FLAC
#define _define___AMP_ADEC_PARAIDX_FLAC
#define AMP_ADEC_PARAIDX_FLAC (13)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_DVSD
#define _define___AMP_ADEC_PARAIDX_DVSD
#define AMP_ADEC_PARAIDX_DVSD (14)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_RA8
#define _define___AMP_ADEC_PARAIDX_RA8
#define AMP_ADEC_PARAIDX_RA8 (15)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_ADPCM
#define _define___AMP_ADEC_PARAIDX_ADPCM
#define AMP_ADEC_PARAIDX_ADPCM (16)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_G711
#define _define___AMP_ADEC_PARAIDX_G711
#define AMP_ADEC_PARAIDX_G711 (17)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_DTSDUMMY
#define _define___AMP_ADEC_PARAIDX_DTSDUMMY
#define AMP_ADEC_PARAIDX_DTSDUMMY (18)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_AMRNB
#define _define___AMP_ADEC_PARAIDX_AMRNB
#define AMP_ADEC_PARAIDX_AMRNB (19)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_AMRWB
#define _define___AMP_ADEC_PARAIDX_AMRWB
#define AMP_ADEC_PARAIDX_AMRWB (20)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_M4ALATM
#define _define___AMP_ADEC_PARAIDX_M4ALATM
#define AMP_ADEC_PARAIDX_M4ALATM (21)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS11_DDC
#define _define___AMP_ADEC_PARAIDX_MS11_DDC
#define AMP_ADEC_PARAIDX_MS11_DDC (22)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS11_DDRE
#define _define___AMP_ADEC_PARAIDX_MS11_DDRE
#define AMP_ADEC_PARAIDX_MS11_DDRE (23)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS11_DDT
#define _define___AMP_ADEC_PARAIDX_MS11_DDT
#define AMP_ADEC_PARAIDX_MS11_DDT (24)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_DDPBSI
#define _define___AMP_ADEC_PARAIDX_DDPBSI
#define AMP_ADEC_PARAIDX_DDPBSI (25)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_DDUDC
#define _define___AMP_ADEC_PARAIDX_DDUDC
#define AMP_ADEC_PARAIDX_DDUDC (26)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_OUTFMT
#define _define___AMP_ADEC_PARAIDX_OUTFMT
#define AMP_ADEC_PARAIDX_OUTFMT (27)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_LOWWATERMARK
#define _define___AMP_ADEC_PARAIDX_LOWWATERMARK
#define AMP_ADEC_PARAIDX_LOWWATERMARK (28)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_HIGHWATERMARK
#define _define___AMP_ADEC_PARAIDX_HIGHWATERMARK
#define AMP_ADEC_PARAIDX_HIGHWATERMARK (29)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_PASSTHRCFG
#define _define___AMP_ADEC_PARAIDX_PASSTHRCFG
#define AMP_ADEC_PARAIDX_PASSTHRCFG (30)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_INTHRESHCFG
#define _define___AMP_ADEC_PARAIDX_INTHRESHCFG
#define AMP_ADEC_PARAIDX_INTHRESHCFG (31)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_ADCFG
#define _define___AMP_ADEC_PARAIDX_ADCFG
#define AMP_ADEC_PARAIDX_ADCFG (32)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_STATISTICS
#define _define___AMP_ADEC_PARAIDX_STATISTICS
#define AMP_ADEC_PARAIDX_STATISTICS (33)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_CAPABILITY
#define _define___AMP_ADEC_PARAIDX_CAPABILITY
#define AMP_ADEC_PARAIDX_CAPABILITY (34)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS12SET
#define _define___AMP_ADEC_PARAIDX_MS12SET
#define AMP_ADEC_PARAIDX_MS12SET (35)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS12_DDP
#define _define___AMP_ADEC_PARAIDX_MS12_DDP
#define AMP_ADEC_PARAIDX_MS12_DDP (36)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS12_MAT
#define _define___AMP_ADEC_PARAIDX_MS12_MAT
#define AMP_ADEC_PARAIDX_MS12_MAT (37)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS12_AC4
#define _define___AMP_ADEC_PARAIDX_MS12_AC4
#define AMP_ADEC_PARAIDX_MS12_AC4 (38)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_MS12_AAC
#define _define___AMP_ADEC_PARAIDX_MS12_AAC
#define AMP_ADEC_PARAIDX_MS12_AAC (39)
#endif

#ifndef _define___AMP_ADEC_PARAIDX_OPUS
#define _define___AMP_ADEC_PARAIDX_OPUS
#define AMP_ADEC_PARAIDX_OPUS (40)
#endif

#ifndef _typedef___AMP_ADEC_PARAIDX
#define _typedef___AMP_ADEC_PARAIDX
typedef CORBA_enum AMP_ADEC_PARAIDX;
#endif /* _typedef___AMP_ADEC_PARAIDX */

#ifndef _define___AMP_ADEC_CAP_NORMAL
#define _define___AMP_ADEC_CAP_NORMAL
#define AMP_ADEC_CAP_NORMAL (0)
#endif

#ifndef _define___AMP_ADEC_CAP_AC3_ONLY
#define _define___AMP_ADEC_CAP_AC3_ONLY
#define AMP_ADEC_CAP_AC3_ONLY (1)
#endif

#ifndef _define___AMP_ADEC_CAP_DTS_CORE
#define _define___AMP_ADEC_CAP_DTS_CORE
#define AMP_ADEC_CAP_DTS_CORE (2)
#endif

#ifndef _typedef___AMP_ADEC_CAPABILITY
#define _typedef___AMP_ADEC_CAPABILITY
typedef CORBA_enum AMP_ADEC_CAPABILITY;
#endif /* _typedef___AMP_ADEC_CAPABILITY */

#ifndef _define___AMP_ADEC_FRAME
#define _define___AMP_ADEC_FRAME
#define AMP_ADEC_FRAME (0)
#endif

#ifndef _define___AMP_ADEC_STEAM
#define _define___AMP_ADEC_STEAM
#define AMP_ADEC_STEAM (1)
#endif

#ifndef _typedef___AMP_ADEC_BD_TYPE
#define _typedef___AMP_ADEC_BD_TYPE
typedef CORBA_enum AMP_ADEC_BD_TYPE;
#endif /* _typedef___AMP_ADEC_BD_TYPE */

#ifndef _define___AMP_ADEC_DUMP_SUBID_IOSTATUS
#define _define___AMP_ADEC_DUMP_SUBID_IOSTATUS
#define AMP_ADEC_DUMP_SUBID_IOSTATUS (0)
#endif

#ifndef _define___AMP_ADEC_DUMP_SUBID_DECPROC
#define _define___AMP_ADEC_DUMP_SUBID_DECPROC
#define AMP_ADEC_DUMP_SUBID_DECPROC (1)
#endif

#ifndef _define___AMP_ADEC_DUMP_SUBID_DESCPROC
#define _define___AMP_ADEC_DUMP_SUBID_DESCPROC
#define AMP_ADEC_DUMP_SUBID_DESCPROC (2)
#endif

#ifndef _typedef___AMP_ADEC_DUMP_SUBID
#define _typedef___AMP_ADEC_DUMP_SUBID
typedef CORBA_enum AMP_ADEC_DUMP_SUBID;
#endif /* _typedef___AMP_ADEC_DUMP_SUBID */

#ifndef _typedef___AMP_ADEC_mix_fade_s
#define _typedef___AMP_ADEC_mix_fade_s
typedef struct AMP_ADEC_mix_fade_s {
  INT32 target_attenuation;
  INT32 duration_in_ms;
  INT32 type;
} AMP_ADEC_mix_fade_s;
#endif /* _typedef___AMP_ADEC_mix_fade_s */

#ifndef _typedef___AMP_ADEC_PARAMS12SET
#define _typedef___AMP_ADEC_PARAMS12SET
typedef struct AMP_ADEC_PARAMS12SET {
  INT16 b_pcm_dmx_params_changed;
  INT16 drc_cut_fac_mc;
  INT16 drc_boost_fac_mc;
  INT16 drc_cut_fac_2ch;
  INT16 drc_boost_fac_2ch;
  INT16 downmix_type;
  INT16 drc_mode;
  INT16 dual_mono_mode;
  INT16 cmix_lev;
  INT16 smix_lev;
  INT16 multichannel_enable;
  INT16 associated_audio_mixing;
  INT16 user_balance_adjustment;
  INT16 b_sys_app_sound_mixing;
  INT8 b_dialogue_enhancer_gain;
  INT32 ac4_de_gain;
  INT8 b_pres_group_idx_selected;
  INT32 ac4_presentation_selection_idx;
  INT8 b_ad_mixing_user_pref;
  INT32 ad_mixing_user_pref;
  INT8 b_heaac_asc;
  UINT8 heaac_asc[256];
  INT32 heaac_asc_len;
  INT16 b_mixing_gain_input_changed;
  struct AMP_ADEC_mix_fade_s input_mix_input1;
  struct AMP_ADEC_mix_fade_s input_mix_input2;
  struct AMP_ADEC_mix_fade_s input_mix_input3;
  struct AMP_ADEC_mix_fade_s syssound_mix_input1;
  struct AMP_ADEC_mix_fade_s syssound_mix_input2;
  struct AMP_ADEC_mix_fade_s syssound_mix_input3;
} AMP_ADEC_PARAMS12SET;
#endif /* _typedef___AMP_ADEC_PARAMS12SET */

#ifndef _typedef___AMP_ADEC_PARAS
#define _typedef___AMP_ADEC_PARAS
typedef struct AMP_ADEC_PARAS {
  UINT8 _d;
  union {
    struct AMP_ADEC_PARADDPTYPE DDP;
    struct AMP_ADEC_PARAMS12DDPTYPE MS12_DDP;
    struct AMP_ADEC_PARAMS12MATTYPE MS12_MAT;
    struct AMP_ADEC_PARAMS12AC4TYPE MS12_AC4;
    struct AMP_ADEC_PARAMS12AACTYPE MS12_AAC;
    struct AMP_ADEC_PARATHDTYPE THD;
    struct AMP_ADEC_PARADTSTYPE DTS;
    struct AMP_ADEC_PARAMPGTYPE MPG;
    struct AMP_ADEC_PARAMP3TYPE MP3;
    struct AMP_ADEC_PARAAACTYPE AAC;
    struct AMP_ADEC_PARAWMATYPE WMA;
    struct AMP_ADEC_PARAPCMTYPE PCM;
    struct AMP_ADEC_PARALPCMSDTYPE LPCMSD;
    struct AMP_ADEC_PARALPCMHDTYPE LPCMHD;
    struct AMP_ADEC_PARALPCMBDTYPE LPCMBD;
    struct AMP_ADEC_PARALPCMSESFTYPE LPCMSESF;
    struct AMP_ADEC_PARAFLACTYPE FLAC;
    struct AMP_ADEC_PARADVSDTYPE ADVSD;
    struct AMP_ADEC_PARARA8TYPE RA8;
    struct AMP_ADEC_PARAADPCMTYPE ADPCM;
    struct AMP_ADEC_PARAG711TYPE G711;
    struct AMP_ADEC_PARADTSDUMMYTYPE DTSDUMMY;
    struct AMP_ADEC_PARAAMRNBTYPE AMRNB;
    struct AMP_ADEC_PARAAMRWBTYPE AMRWB;
    struct AMP_ADEC_PARAM4ALATMTYPE M4ALATM;
    struct AMP_ADEC_PARAMS11DDCTYPE MS11_DDC;
    struct AMP_ADEC_PARAMS11DDRETYPE MS11_DDRE;
    struct AMP_ADEC_PARAMS11DDTTYPE MS11_DDT;
    struct AMP_ADEC_PARADDUDCTYPE DDUDC;
    struct AMP_ADEC_PARAOUTFMT OUTFORMAT;
    UINT32 LOWWATERMARK;
    UINT32 HIGHWATERMARK;
    struct AMP_ADEC_PARAPASSTHRCFG PassThrCfg;
    struct AMP_ADEC_PARAINTHRESHCFG InThreshCfg;
    struct AMP_ADEC_PARAADCFG ADCfg;
    struct AMP_ADEC_PARASTATISTICS AdecStat;
    UINT32 DecoderCap;
    struct AMP_ADEC_PARAMS12SET MS12PARAM;
    struct AMP_ADEC_PARAOPUSTYPE OPUS;
  } _u;
} AMP_ADEC_PARAS;
#endif /* _typedef___AMP_ADEC_PARAS */

#ifndef _typedef___AMP_ADEC_STRMINFO_EVENT
#define _typedef___AMP_ADEC_STRMINFO_EVENT
typedef struct AMP_ADEC_STRMINFO_EVENT {
  UINT32 uiAdecFmt;
  UINT32 uiStreamFmt;
  UINT32 uiSampleRate;
  UINT32 uiChannelNum;
  UINT32 uiPriChanMode;
  UINT32 uiLfeMode;
  UINT32 uiBitDepth;
  INT8 bAtmos;
  INT32 iPortIdx;
} AMP_ADEC_STRMINFO_EVENT;
#endif /* _typedef___AMP_ADEC_STRMINFO_EVENT */

#ifndef _typedef___AMP_ADEC_WRGCODEC_EVENT
#define _typedef___AMP_ADEC_WRGCODEC_EVENT
typedef struct AMP_ADEC_WRGCODEC_EVENT {
  UINT32 uiStreamFmt;
} AMP_ADEC_WRGCODEC_EVENT;
#endif /* _typedef___AMP_ADEC_WRGCODEC_EVENT */

#ifndef _typedef___AMP_ADEC_FMTNOTIFY_EVENT
#define _typedef___AMP_ADEC_FMTNOTIFY_EVENT
typedef struct AMP_ADEC_FMTNOTIFY_EVENT {
  UINT32 uiAudioFmt;
} AMP_ADEC_FMTNOTIFY_EVENT;
#endif /* _typedef___AMP_ADEC_FMTNOTIFY_EVENT */

#ifndef _define___AMP_ADEC_EVENT_NOMASK
#define _define___AMP_ADEC_EVENT_NOMASK
#define AMP_ADEC_EVENT_NOMASK (0)
#endif

#ifndef _define___AMP_ADEC_EVENT_ALLMASK
#define _define___AMP_ADEC_EVENT_ALLMASK
#define AMP_ADEC_EVENT_ALLMASK (4095)
#endif

#ifndef _typedef___AMP_ADEC_BUF_INFO
#define _typedef___AMP_ADEC_BUF_INFO
typedef struct AMP_ADEC_BUF_INFO {
  UINT32 uiDataCount;
} AMP_ADEC_BUF_INFO;
#endif /* _typedef___AMP_ADEC_BUF_INFO */

#ifndef _typedef___AMP_ADEC_MPG_INFO
#define _typedef___AMP_ADEC_MPG_INFO
typedef struct AMP_ADEC_MPG_INFO {
  UINT32 uiLayer;
  UINT32 uiID;
} AMP_ADEC_MPG_INFO;
#endif /* _typedef___AMP_ADEC_MPG_INFO */

#ifndef _typedef___AMP_ADEC_AAC_INFO
#define _typedef___AMP_ADEC_AAC_INFO
typedef struct AMP_ADEC_AAC_INFO {
  UINT32 uiLayer;
  UINT32 uiProfile;
} AMP_ADEC_AAC_INFO;
#endif /* _typedef___AMP_ADEC_AAC_INFO */

#ifndef _typedef___AMP_ADEC_DOLBY_INFO
#define _typedef___AMP_ADEC_DOLBY_INFO
typedef struct AMP_ADEC_DOLBY_INFO {
  UINT32 uiBsType;
} AMP_ADEC_DOLBY_INFO;
#endif /* _typedef___AMP_ADEC_DOLBY_INFO */

#ifndef _define___AMP_ADEC_DTS_UNKNOWN
#define _define___AMP_ADEC_DTS_UNKNOWN
#define AMP_ADEC_DTS_UNKNOWN (0)
#endif

#ifndef _define___AMP_ADEC_DTS
#define _define___AMP_ADEC_DTS
#define AMP_ADEC_DTS (1)
#endif

#ifndef _define___AMP_ADEC_DTS_ES_61_MATRIX
#define _define___AMP_ADEC_DTS_ES_61_MATRIX
#define AMP_ADEC_DTS_ES_61_MATRIX (2)
#endif

#ifndef _define___AMP_ADEC_DTS_ES_61_DISCRETE
#define _define___AMP_ADEC_DTS_ES_61_DISCRETE
#define AMP_ADEC_DTS_ES_61_DISCRETE (3)
#endif

#ifndef _define___AMP_ADEC_DTS_96_24
#define _define___AMP_ADEC_DTS_96_24
#define AMP_ADEC_DTS_96_24 (4)
#endif

#ifndef _define___AMP_ADEC_DTS_ES_8CH_DISCRETE
#define _define___AMP_ADEC_DTS_ES_8CH_DISCRETE
#define AMP_ADEC_DTS_ES_8CH_DISCRETE (5)
#endif

#ifndef _define___AMP_ADEC_DTS_HD_HIGH_RESOLUTION
#define _define___AMP_ADEC_DTS_HD_HIGH_RESOLUTION
#define AMP_ADEC_DTS_HD_HIGH_RESOLUTION (6)
#endif

#ifndef _define___AMP_ADEC_DTS_HD_MASTER_AUDIO
#define _define___AMP_ADEC_DTS_HD_MASTER_AUDIO
#define AMP_ADEC_DTS_HD_MASTER_AUDIO (7)
#endif

#ifndef _define___AMP_ADEC_DTS_HD_LOW_BIT_RATE
#define _define___AMP_ADEC_DTS_HD_LOW_BIT_RATE
#define AMP_ADEC_DTS_HD_LOW_BIT_RATE (8)
#endif

#ifndef _typedef___AMP_ADEC_DTS_INFO
#define _typedef___AMP_ADEC_DTS_INFO
typedef struct AMP_ADEC_DTS_INFO {
  UINT32 uiStreamFormat;
  UINT32 uiPlaybackMode;
  UINT32 uiChannelMask;
  UINT32 uiRealChannelMask;
} AMP_ADEC_DTS_INFO;
#endif /* _typedef___AMP_ADEC_DTS_INFO */

#ifndef _typedef___AMP_ADEC_AUD_INFO
#define _typedef___AMP_ADEC_AUD_INFO
typedef struct AMP_ADEC_AUD_INFO {
  UINT8 _d;
  union {
    struct AMP_ADEC_MPG_INFO stMpgInfo;
    struct AMP_ADEC_AAC_INFO stAacInfo;
    struct AMP_ADEC_DOLBY_INFO stDolbyInfo;
    struct AMP_ADEC_DOLBY_INFO stMs11DdcInfo;
    struct AMP_ADEC_DOLBY_INFO stMs11DdtInfo;
    struct AMP_ADEC_DTS_INFO stDtsInfo;
  } _u;
} AMP_ADEC_AUD_INFO;
#endif /* _typedef___AMP_ADEC_AUD_INFO */

HRESULT AMP_ADEC_SetParameters(AMP_ADEC _obj, AMP_ADEC_PARAIDX eParaIdx, struct AMP_ADEC_PARAS *pParas, CORBA_Environment *_ev);

HRESULT AMP_ADEC_GetParameters(AMP_ADEC _obj, AMP_ADEC_PARAIDX eParaIdx, struct AMP_ADEC_PARAS *pParas, CORBA_Environment *_ev);

HRESULT AMP_ADEC_QueryPortBuf(AMP_ADEC _obj, UINT32 uiPortIdx, struct AMP_ADEC_BUF_INFO *stBufInfo, CORBA_Environment *_ev);

HRESULT AMP_ADEC_GetEsDataCnt(AMP_ADEC _obj, UINT32 uiPortIdx, struct AMP_ADEC_BUF_INFO *stBufInfo, CORBA_Environment *_ev);

HRESULT AMP_ADEC_SetFormatDetection(AMP_ADEC _obj, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_ADEC_GetInfoWithFmt(AMP_ADEC _obj, UINT32 uiFormat, struct AMP_ADEC_AUD_INFO *stAudInfo, CORBA_Environment *_ev);

HRESULT AMP_ADEC_QuerySupport(AMP_ADEC _obj, UINT32 uiFormat, BOOL *bSupport, CORBA_Environment *_ev);

HRESULT AMP_ADEC_Open(AMP_ADEC _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_ADEC_Close(AMP_ADEC _obj, CORBA_Environment *_ev);

HRESULT AMP_ADEC_Destroy(AMP_ADEC _obj, CORBA_Environment *_ev);

HRESULT AMP_ADEC_SetState(AMP_ADEC _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_ADEC_GetState(AMP_ADEC _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_ADEC_QueryInfo(AMP_ADEC _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_ADEC_SetupPort(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_ADEC_DynamicSetupPort(AMP_ADEC _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ADEC_DisconnectPorts(AMP_ADEC _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ADEC_ClearPort(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ADEC_QueryPort(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_ADEC_ClearPortBuf(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ADEC_DebugDump(AMP_ADEC _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_ADEC_RegisterNotify(AMP_ADEC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ADEC_UnregisterNotify(AMP_ADEC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ADEC_RegisterNotifyDetail(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ADEC_UnregisterNotifyDetail(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ADEC_RegisterBD(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_ADEC_UnregisterBD(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_ADEC_PushBD(AMP_ADEC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_AENC
#define _typedef___AMP_AENC
typedef CORBA_Object AMP_AENC;
#endif /* _typedef___AMP_AENC */

#ifndef _define___AMP_AENC_EVENT_NOMASK
#define _define___AMP_AENC_EVENT_NOMASK
#define AMP_AENC_EVENT_NOMASK (0)
#endif

#ifndef _define___AMP_AENC_EVENT_ALLMASK
#define _define___AMP_AENC_EVENT_ALLMASK
#define AMP_AENC_EVENT_ALLMASK (4095)
#endif

#ifndef _typedef___AMP_AENC_PARADDCE
#define _typedef___AMP_AENC_PARADDCE
typedef struct AMP_AENC_PARADDCE {
  INT32 uiAcmod;
  INT32 uiBitRate;
} AMP_AENC_PARADDCE;
#endif /* _typedef___AMP_AENC_PARADDCE */

#ifndef _typedef___AMP_AENC_PARADTSE
#define _typedef___AMP_AENC_PARADTSE
typedef struct AMP_AENC_PARADTSE {
  INT32 uiAcmod;
} AMP_AENC_PARADTSE;
#endif /* _typedef___AMP_AENC_PARADTSE */

#ifndef _typedef___AMP_AENC_PARAAACE
#define _typedef___AMP_AENC_PARAAACE
typedef struct AMP_AENC_PARAAACE {
  INT32 uiAcmod;
  INT32 uiBitRate;
} AMP_AENC_PARAAACE;
#endif /* _typedef___AMP_AENC_PARAAACE */

#ifndef _define___AMP_AENC_PARAIDX_DDCE
#define _define___AMP_AENC_PARAIDX_DDCE
#define AMP_AENC_PARAIDX_DDCE (0)
#endif

#ifndef _define___AMP_AENC_PARAIDX_DTSE
#define _define___AMP_AENC_PARAIDX_DTSE
#define AMP_AENC_PARAIDX_DTSE (1)
#endif

#ifndef _define___AMP_AENC_PARAIDX_AACE
#define _define___AMP_AENC_PARAIDX_AACE
#define AMP_AENC_PARAIDX_AACE (2)
#endif

#ifndef _typedef___AMP_AENC_PARAIDX
#define _typedef___AMP_AENC_PARAIDX
typedef CORBA_enum AMP_AENC_PARAIDX;
#endif /* _typedef___AMP_AENC_PARAIDX */

#ifndef _typedef___AMP_AENC_PARAST
#define _typedef___AMP_AENC_PARAST
typedef struct AMP_AENC_PARAST {
  UINT8 _d;
  union {
    struct AMP_AENC_PARADDCE stDdce;
    struct AMP_AENC_PARADTSE stDtse;
    struct AMP_AENC_PARAAACE stAace;
  } _u;
} AMP_AENC_PARAST;
#endif /* _typedef___AMP_AENC_PARAST */

HRESULT AMP_AENC_SetPortParameter(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_AENC_PARAIDX eParamIdx, struct AMP_AENC_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_AENC_GetPortParameter(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_AENC_PARAIDX eParamIdx, struct AMP_AENC_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_AENC_SetSampleRate(AMP_AENC _obj, UINT32 uiSampleRate, CORBA_Environment *_ev);

HRESULT AMP_AENC_SetBitRate(AMP_AENC _obj, UINT32 uiBitRate, CORBA_Environment *_ev);

HRESULT AMP_AENC_Open(AMP_AENC _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_AENC_Close(AMP_AENC _obj, CORBA_Environment *_ev);

HRESULT AMP_AENC_Destroy(AMP_AENC _obj, CORBA_Environment *_ev);

HRESULT AMP_AENC_SetState(AMP_AENC _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_AENC_GetState(AMP_AENC _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_AENC_QueryInfo(AMP_AENC _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_AENC_SetupPort(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_AENC_DynamicSetupPort(AMP_AENC _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AENC_DisconnectPorts(AMP_AENC _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AENC_ClearPort(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AENC_QueryPort(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_AENC_ClearPortBuf(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AENC_DebugDump(AMP_AENC _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_AENC_RegisterNotify(AMP_AENC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AENC_UnregisterNotify(AMP_AENC _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AENC_RegisterNotifyDetail(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AENC_UnregisterNotifyDetail(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AENC_RegisterBD(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AENC_UnregisterBD(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AENC_PushBD(AMP_AENC _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_AREN
#define _typedef___AMP_AREN
typedef CORBA_Object AMP_AREN;
#endif /* _typedef___AMP_AREN */

#ifndef _typedef___AMP_AREN_PARAPORTTYPE
#define _typedef___AMP_AREN_PARAPORTTYPE
typedef struct AMP_AREN_PARAPORTTYPE {
  UINT32 uiPortType;
} AMP_AREN_PARAPORTTYPE;
#endif /* _typedef___AMP_AREN_PARAPORTTYPE */

#ifndef _typedef___AMP_AREN_PARAPORTASSOCCLK
#define _typedef___AMP_AREN_PARAPORTASSOCCLK
typedef struct AMP_AREN_PARAPORTASSOCCLK {
  UINT32 uiAssocIdx;
} AMP_AREN_PARAPORTASSOCCLK;
#endif /* _typedef___AMP_AREN_PARAPORTASSOCCLK */

#ifndef _define___AMP_AREN_PARAIDX_PORTTYPE
#define _define___AMP_AREN_PARAIDX_PORTTYPE
#define AMP_AREN_PARAIDX_PORTTYPE (0)
#endif

#ifndef _define___AMP_AREN_PARAIDX_PORTASSOCCLK
#define _define___AMP_AREN_PARAIDX_PORTASSOCCLK
#define AMP_AREN_PARAIDX_PORTASSOCCLK (1)
#endif

#ifndef _typedef___AMP_AREN_PARAIDX
#define _typedef___AMP_AREN_PARAIDX
typedef CORBA_enum AMP_AREN_PARAIDX;
#endif /* _typedef___AMP_AREN_PARAIDX */

#ifndef _define___AMP_AREN_DUMP_SUBID_IOSTATUS
#define _define___AMP_AREN_DUMP_SUBID_IOSTATUS
#define AMP_AREN_DUMP_SUBID_IOSTATUS (0)
#endif

#ifndef _typedef___AMP_AREN_DUMP_SUBID
#define _typedef___AMP_AREN_DUMP_SUBID
typedef CORBA_enum AMP_AREN_DUMP_SUBID;
#endif /* _typedef___AMP_AREN_DUMP_SUBID */

#ifndef _typedef___AMP_AREN_PARAST
#define _typedef___AMP_AREN_PARAST
typedef struct AMP_AREN_PARAST {
  UINT8 _d;
  union {
    struct AMP_AREN_PARAPORTTYPE PORTTYPE;
    struct AMP_AREN_PARAPORTASSOCCLK PORTASSOCCLK;
  } _u;
} AMP_AREN_PARAST;
#endif /* _typedef___AMP_AREN_PARAST */

#ifndef _typedef___AMP_AREN_BUF_INFO
#define _typedef___AMP_AREN_BUF_INFO
typedef struct AMP_AREN_BUF_INFO {
  UINT32 uiSampleCount;
} AMP_AREN_BUF_INFO;
#endif /* _typedef___AMP_AREN_BUF_INFO */

#ifndef _define___AMP_AREN_EVENT_NOMASK
#define _define___AMP_AREN_EVENT_NOMASK
#define AMP_AREN_EVENT_NOMASK (0)
#endif

#ifndef _define___AMP_AREN_EVENT_ALLMASK
#define _define___AMP_AREN_EVENT_ALLMASK
#define AMP_AREN_EVENT_ALLMASK (4095)
#endif

HRESULT AMP_AREN_SetPortParameter(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_AREN_PARAIDX eParamIdx, struct AMP_AREN_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_AREN_GetPortParameter(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_AREN_PARAIDX eParamIdx, struct AMP_AREN_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_AREN_QueryPortBuf(AMP_AREN _obj, UINT32 uiPortIdx, struct AMP_AREN_BUF_INFO *pBufInfo, CORBA_Environment *_ev);

HRESULT AMP_AREN_Open(AMP_AREN _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_AREN_Close(AMP_AREN _obj, CORBA_Environment *_ev);

HRESULT AMP_AREN_Destroy(AMP_AREN _obj, CORBA_Environment *_ev);

HRESULT AMP_AREN_SetState(AMP_AREN _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_AREN_GetState(AMP_AREN _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_AREN_QueryInfo(AMP_AREN _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_AREN_SetupPort(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_AREN_DynamicSetupPort(AMP_AREN _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AREN_DisconnectPorts(AMP_AREN _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AREN_ClearPort(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AREN_QueryPort(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_AREN_ClearPortBuf(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AREN_DebugDump(AMP_AREN _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_AREN_RegisterNotify(AMP_AREN _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AREN_UnregisterNotify(AMP_AREN _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AREN_RegisterNotifyDetail(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AREN_UnregisterNotifyDetail(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AREN_RegisterBD(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AREN_UnregisterBD(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AREN_PushBD(AMP_AREN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _define___AMP_AUDIO_STEREO_AUTO
#define _define___AMP_AUDIO_STEREO_AUTO
#define AMP_AUDIO_STEREO_AUTO (0)
#endif

#ifndef _define___AMP_AUDIO_STEREO_SRND
#define _define___AMP_AUDIO_STEREO_SRND
#define AMP_AUDIO_STEREO_SRND (1)
#endif

#ifndef _define___AMP_AUDIO_STEREO_STEREO
#define _define___AMP_AUDIO_STEREO_STEREO
#define AMP_AUDIO_STEREO_STEREO (2)
#endif

#ifndef _define___AMP_AUDIO_STEREO_PLII
#define _define___AMP_AUDIO_STEREO_PLII
#define AMP_AUDIO_STEREO_PLII (3)
#endif

#ifndef _define___AMP_AUDIO_STEREOMODE_NUM
#define _define___AMP_AUDIO_STEREOMODE_NUM
#define AMP_AUDIO_STEREOMODE_NUM (4)
#endif

#ifndef _typedef___AMP_AUDIO_STEREO_MODE
#define _typedef___AMP_AUDIO_STEREO_MODE
typedef CORBA_enum AMP_AUDIO_STEREO_MODE;
#endif /* _typedef___AMP_AUDIO_STEREO_MODE */

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_UNDEF
#define _define___AMP_APP_HDMI_AUDIO_FMT_UNDEF
#define AMP_APP_HDMI_AUDIO_FMT_UNDEF (0)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_PCM
#define _define___AMP_APP_HDMI_AUDIO_FMT_PCM
#define AMP_APP_HDMI_AUDIO_FMT_PCM (1)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_AC3
#define _define___AMP_APP_HDMI_AUDIO_FMT_AC3
#define AMP_APP_HDMI_AUDIO_FMT_AC3 (2)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_MPEG1
#define _define___AMP_APP_HDMI_AUDIO_FMT_MPEG1
#define AMP_APP_HDMI_AUDIO_FMT_MPEG1 (3)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_MP3
#define _define___AMP_APP_HDMI_AUDIO_FMT_MP3
#define AMP_APP_HDMI_AUDIO_FMT_MP3 (4)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_MPEG2
#define _define___AMP_APP_HDMI_AUDIO_FMT_MPEG2
#define AMP_APP_HDMI_AUDIO_FMT_MPEG2 (5)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_AAC
#define _define___AMP_APP_HDMI_AUDIO_FMT_AAC
#define AMP_APP_HDMI_AUDIO_FMT_AAC (6)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_DTS
#define _define___AMP_APP_HDMI_AUDIO_FMT_DTS
#define AMP_APP_HDMI_AUDIO_FMT_DTS (7)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_ATRAC
#define _define___AMP_APP_HDMI_AUDIO_FMT_ATRAC
#define AMP_APP_HDMI_AUDIO_FMT_ATRAC (8)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_ONE_BIT_AUDIO
#define _define___AMP_APP_HDMI_AUDIO_FMT_ONE_BIT_AUDIO
#define AMP_APP_HDMI_AUDIO_FMT_ONE_BIT_AUDIO (9)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_DOLBY_DIGITAL_PLUS
#define _define___AMP_APP_HDMI_AUDIO_FMT_DOLBY_DIGITAL_PLUS
#define AMP_APP_HDMI_AUDIO_FMT_DOLBY_DIGITAL_PLUS (10)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_DTS_HD
#define _define___AMP_APP_HDMI_AUDIO_FMT_DTS_HD
#define AMP_APP_HDMI_AUDIO_FMT_DTS_HD (11)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_MAT
#define _define___AMP_APP_HDMI_AUDIO_FMT_MAT
#define AMP_APP_HDMI_AUDIO_FMT_MAT (12)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_DST
#define _define___AMP_APP_HDMI_AUDIO_FMT_DST
#define AMP_APP_HDMI_AUDIO_FMT_DST (13)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_WMA_PRO
#define _define___AMP_APP_HDMI_AUDIO_FMT_WMA_PRO
#define AMP_APP_HDMI_AUDIO_FMT_WMA_PRO (14)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_MPEG_H
#define _define___AMP_APP_HDMI_AUDIO_FMT_MPEG_H
#define AMP_APP_HDMI_AUDIO_FMT_MPEG_H (15)
#endif

#ifndef _define___AMP_APP_HDMI_AUDIO_FMT_MAX
#define _define___AMP_APP_HDMI_AUDIO_FMT_MAX
#define AMP_APP_HDMI_AUDIO_FMT_MAX (16)
#endif

#ifndef _typedef___AMP_HDMI_AUDIO_FMT_T
#define _typedef___AMP_HDMI_AUDIO_FMT_T
typedef CORBA_enum AMP_HDMI_AUDIO_FMT_T;
#endif /* _typedef___AMP_HDMI_AUDIO_FMT_T */

#ifndef _define___AMP_AUDIO_DUALMODE_STEREO
#define _define___AMP_AUDIO_DUALMODE_STEREO
#define AMP_AUDIO_DUALMODE_STEREO (0)
#endif

#ifndef _define___AMP_AUDIO_DUALMODE_LEFTMONO
#define _define___AMP_AUDIO_DUALMODE_LEFTMONO
#define AMP_AUDIO_DUALMODE_LEFTMONO (1)
#endif

#ifndef _define___AMP_AUDIO_DUALMODE_RGHTMONO
#define _define___AMP_AUDIO_DUALMODE_RGHTMONO
#define AMP_AUDIO_DUALMODE_RGHTMONO (2)
#endif

#ifndef _define___AMP_AUDIO_DUALMODE_MIXMONO
#define _define___AMP_AUDIO_DUALMODE_MIXMONO
#define AMP_AUDIO_DUALMODE_MIXMONO (3)
#endif

#ifndef _define___AMP_AUDIO_DUALMODE_NUM
#define _define___AMP_AUDIO_DUALMODE_NUM
#define AMP_AUDIO_DUALMODE_NUM (4)
#endif

#ifndef _typedef___AMP_AUDIO_DUAL_MODE
#define _typedef___AMP_AUDIO_DUAL_MODE
typedef CORBA_enum AMP_AUDIO_DUAL_MODE;
#endif /* _typedef___AMP_AUDIO_DUAL_MODE */

#ifndef _define___AMP_AUDIO_CHMAP_1_PLUS_1
#define _define___AMP_AUDIO_CHMAP_1_PLUS_1
#define AMP_AUDIO_CHMAP_1_PLUS_1 (0)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_1_0_0
#define _define___AMP_AUDIO_CHMAP_1_0_0
#define AMP_AUDIO_CHMAP_1_0_0 (1)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_2_0_0
#define _define___AMP_AUDIO_CHMAP_2_0_0
#define AMP_AUDIO_CHMAP_2_0_0 (2)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_0_0
#define _define___AMP_AUDIO_CHMAP_3_0_0
#define AMP_AUDIO_CHMAP_3_0_0 (3)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_2_1_0
#define _define___AMP_AUDIO_CHMAP_2_1_0
#define AMP_AUDIO_CHMAP_2_1_0 (4)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_1_0
#define _define___AMP_AUDIO_CHMAP_3_1_0
#define AMP_AUDIO_CHMAP_3_1_0 (5)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_2_2_0
#define _define___AMP_AUDIO_CHMAP_2_2_0
#define AMP_AUDIO_CHMAP_2_2_0 (6)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_2_0
#define _define___AMP_AUDIO_CHMAP_3_2_0
#define AMP_AUDIO_CHMAP_3_2_0 (7)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_2_2
#define _define___AMP_AUDIO_CHMAP_3_2_2
#define AMP_AUDIO_CHMAP_3_2_2 (8)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_LTRT
#define _define___AMP_AUDIO_CHMAP_LTRT
#define AMP_AUDIO_CHMAP_LTRT (9)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_2_0_2
#define _define___AMP_AUDIO_CHMAP_2_0_2
#define AMP_AUDIO_CHMAP_2_0_2 (10)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_0_1
#define _define___AMP_AUDIO_CHMAP_3_0_1
#define AMP_AUDIO_CHMAP_3_0_1 (11)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_2_2_1
#define _define___AMP_AUDIO_CHMAP_2_2_1
#define AMP_AUDIO_CHMAP_2_2_1 (12)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_2_1
#define _define___AMP_AUDIO_CHMAP_3_2_1
#define AMP_AUDIO_CHMAP_3_2_1 (13)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_3_0_2
#define _define___AMP_AUDIO_CHMAP_3_0_2
#define AMP_AUDIO_CHMAP_3_0_2 (14)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_2_2_2
#define _define___AMP_AUDIO_CHMAP_2_2_2
#define AMP_AUDIO_CHMAP_2_2_2 (15)
#endif

#ifndef _define___AMP_AUDIO_CHMAP_NUM
#define _define___AMP_AUDIO_CHMAP_NUM
#define AMP_AUDIO_CHMAP_NUM (16)
#endif

#ifndef _typedef___AMP_AUDIO_PRICHMAP_MODE
#define _typedef___AMP_AUDIO_PRICHMAP_MODE
typedef CORBA_enum AMP_AUDIO_PRICHMAP_MODE;
#endif /* _typedef___AMP_AUDIO_PRICHMAP_MODE */

#ifndef _define___AMP_AUDIO_LFE_OFF
#define _define___AMP_AUDIO_LFE_OFF
#define AMP_AUDIO_LFE_OFF (0)
#endif

#ifndef _define___AMP_AUDIO_LFE_ON
#define _define___AMP_AUDIO_LFE_ON
#define AMP_AUDIO_LFE_ON (1)
#endif

#ifndef _define___AMP_AUDIO_LFE_DUAL
#define _define___AMP_AUDIO_LFE_DUAL
#define AMP_AUDIO_LFE_DUAL (2)
#endif

#ifndef _define___AMP_AUDIO_LFE_NUM
#define _define___AMP_AUDIO_LFE_NUM
#define AMP_AUDIO_LFE_NUM (3)
#endif

#ifndef _typedef___AMP_AUDIO_LFE_MODE
#define _typedef___AMP_AUDIO_LFE_MODE
typedef CORBA_enum AMP_AUDIO_LFE_MODE;
#endif /* _typedef___AMP_AUDIO_LFE_MODE */

#ifndef _define___AMP_AUDIO_TOP_OFF
#define _define___AMP_AUDIO_TOP_OFF
#define AMP_AUDIO_TOP_OFF (0)
#endif

#ifndef _define___AMP_AUDIO_TOP_DUAL
#define _define___AMP_AUDIO_TOP_DUAL
#define AMP_AUDIO_TOP_DUAL (1)
#endif

#ifndef _define___AMP_AUDIO_TOP_FOUR
#define _define___AMP_AUDIO_TOP_FOUR
#define AMP_AUDIO_TOP_FOUR (2)
#endif

#ifndef _define___AMP_AUDIO_TOP_NUM
#define _define___AMP_AUDIO_TOP_NUM
#define AMP_AUDIO_TOP_NUM (3)
#endif

#ifndef _typedef___AMP_AUDIO_TOP_MODE
#define _typedef___AMP_AUDIO_TOP_MODE
typedef CORBA_enum AMP_AUDIO_TOP_MODE;
#endif /* _typedef___AMP_AUDIO_TOP_MODE */

#ifndef _define___AMP_AUDIO_COMPMODE_CUSTOM_0
#define _define___AMP_AUDIO_COMPMODE_CUSTOM_0
#define AMP_AUDIO_COMPMODE_CUSTOM_0 (0)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_CUSTOM_1
#define _define___AMP_AUDIO_COMPMODE_CUSTOM_1
#define AMP_AUDIO_COMPMODE_CUSTOM_1 (1)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_LINE
#define _define___AMP_AUDIO_COMPMODE_LINE
#define AMP_AUDIO_COMPMODE_LINE (2)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_RF
#define _define___AMP_AUDIO_COMPMODE_RF
#define AMP_AUDIO_COMPMODE_RF (3)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_AUTO
#define _define___AMP_AUDIO_COMPMODE_AUTO
#define AMP_AUDIO_COMPMODE_AUTO (4)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_ON
#define _define___AMP_AUDIO_COMPMODE_ON
#define AMP_AUDIO_COMPMODE_ON (5)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_OFF
#define _define___AMP_AUDIO_COMPMODE_OFF
#define AMP_AUDIO_COMPMODE_OFF (6)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_TV
#define _define___AMP_AUDIO_COMPMODE_TV
#define AMP_AUDIO_COMPMODE_TV (7)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_PORTABLE_L8
#define _define___AMP_AUDIO_COMPMODE_PORTABLE_L8
#define AMP_AUDIO_COMPMODE_PORTABLE_L8 (8)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_PORTABLE_L11
#define _define___AMP_AUDIO_COMPMODE_PORTABLE_L11
#define AMP_AUDIO_COMPMODE_PORTABLE_L11 (9)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_PORTABLE_L14
#define _define___AMP_AUDIO_COMPMODE_PORTABLE_L14
#define AMP_AUDIO_COMPMODE_PORTABLE_L14 (10)
#endif

#ifndef _define___AMP_AUDIO_COMPMODE_DRCSUPPRESSION
#define _define___AMP_AUDIO_COMPMODE_DRCSUPPRESSION
#define AMP_AUDIO_COMPMODE_DRCSUPPRESSION (11)
#endif

#ifndef _typedef___AMP_AUDIO_COMPMODE
#define _typedef___AMP_AUDIO_COMPMODE
typedef CORBA_enum AMP_AUDIO_COMPMODE;
#endif /* _typedef___AMP_AUDIO_COMPMODE */

#ifndef _define___AMP_AUDIO_KARAOKE_NONE_VOCAL
#define _define___AMP_AUDIO_KARAOKE_NONE_VOCAL
#define AMP_AUDIO_KARAOKE_NONE_VOCAL (0)
#endif

#ifndef _define___AMP_AUDIO_KARAOKE_LEFT_VOCAL
#define _define___AMP_AUDIO_KARAOKE_LEFT_VOCAL
#define AMP_AUDIO_KARAOKE_LEFT_VOCAL (1)
#endif

#ifndef _define___AMP_AUDIO_KARAOKE_RGHT_VOCAL
#define _define___AMP_AUDIO_KARAOKE_RGHT_VOCAL
#define AMP_AUDIO_KARAOKE_RGHT_VOCAL (2)
#endif

#ifndef _define___AMP_AUDIO_KARAOKE_BOTH_VOCAL
#define _define___AMP_AUDIO_KARAOKE_BOTH_VOCAL
#define AMP_AUDIO_KARAOKE_BOTH_VOCAL (3)
#endif

#ifndef _define___AMP_AUDIO_KARAOKE_NUM
#define _define___AMP_AUDIO_KARAOKE_NUM
#define AMP_AUDIO_KARAOKE_NUM (4)
#endif

#ifndef _typedef___AMP_AUDIO_KARAOKE
#define _typedef___AMP_AUDIO_KARAOKE
typedef CORBA_enum AMP_AUDIO_KARAOKE;
#endif /* _typedef___AMP_AUDIO_KARAOKE */

#ifndef _define___AMP_AUDIO_DYNRNG_MAX
#define _define___AMP_AUDIO_DYNRNG_MAX
#define AMP_AUDIO_DYNRNG_MAX (1073741824)
#endif

#ifndef _define___AMP_AUDIO_DYNRNG_MIN
#define _define___AMP_AUDIO_DYNRNG_MIN
#define AMP_AUDIO_DYNRNG_MIN (0)
#endif

#ifndef _define___AMP_AUDIO_METADATA_INVALID
#define _define___AMP_AUDIO_METADATA_INVALID
#define AMP_AUDIO_METADATA_INVALID (0)
#endif

#ifndef _define___AMP_AUDIO_METADATA_DMX
#define _define___AMP_AUDIO_METADATA_DMX
#define AMP_AUDIO_METADATA_DMX (1)
#endif

#ifndef _define___AMP_AUDIO_METADATA_MIX
#define _define___AMP_AUDIO_METADATA_MIX
#define AMP_AUDIO_METADATA_MIX (2)
#endif

#ifndef _typedef___AMP_AUDIO_METADATA
#define _typedef___AMP_AUDIO_METADATA
typedef CORBA_enum AMP_AUDIO_METADATA;
#endif /* _typedef___AMP_AUDIO_METADATA */

#ifndef _define___AMP_AUDIO_NEURAL_DMX_NONE
#define _define___AMP_AUDIO_NEURAL_DMX_NONE
#define AMP_AUDIO_NEURAL_DMX_NONE (0)
#endif

#ifndef _define___AMP_AUDIO_NEURAL_DMX_522
#define _define___AMP_AUDIO_NEURAL_DMX_522
#define AMP_AUDIO_NEURAL_DMX_522 (1)
#endif

#ifndef _define___AMP_AUDIO_NEURAL_DMX_722
#define _define___AMP_AUDIO_NEURAL_DMX_722
#define AMP_AUDIO_NEURAL_DMX_722 (2)
#endif

#ifndef _define___AMP_AUDIO_NEURAL_DMX_725
#define _define___AMP_AUDIO_NEURAL_DMX_725
#define AMP_AUDIO_NEURAL_DMX_725 (3)
#endif

#ifndef _typedef___AMP_AUDIO_NEURAL_DMX
#define _typedef___AMP_AUDIO_NEURAL_DMX
typedef CORBA_enum AMP_AUDIO_NEURAL_DMX;
#endif /* _typedef___AMP_AUDIO_NEURAL_DMX */

#ifndef _define___AMP_AUDIO_USER_PCM
#define _define___AMP_AUDIO_USER_PCM
#define AMP_AUDIO_USER_PCM (0)
#endif

#ifndef _define___AMP_AUDIO_USER_AUTO
#define _define___AMP_AUDIO_USER_AUTO
#define AMP_AUDIO_USER_AUTO (1)
#endif

#ifndef _typedef___AMP_AUDIO_USER_MODE
#define _typedef___AMP_AUDIO_USER_MODE
typedef CORBA_enum AMP_AUDIO_USER_MODE;
#endif /* _typedef___AMP_AUDIO_USER_MODE */

#ifndef _define___AMP_AUDIO_CONTINUES_OFF
#define _define___AMP_AUDIO_CONTINUES_OFF
#define AMP_AUDIO_CONTINUES_OFF (0)
#endif

#ifndef _define___AMP_AUDIO_CONTINUES_ON
#define _define___AMP_AUDIO_CONTINUES_ON
#define AMP_AUDIO_CONTINUES_ON (1)
#endif

#ifndef _typedef___AMP_AUDIO_CONTINUES
#define _typedef___AMP_AUDIO_CONTINUES
typedef CORBA_enum AMP_AUDIO_CONTINUES;
#endif /* _typedef___AMP_AUDIO_CONTINUES */

#ifndef _typedef___AMP_APP
#define _typedef___AMP_APP
typedef CORBA_Object AMP_APP;
#endif /* _typedef___AMP_APP */

#ifndef _define___AMP_APP_EVENT_NOMASK
#define _define___AMP_APP_EVENT_NOMASK
#define AMP_APP_EVENT_NOMASK (0)
#endif

#ifndef _define___AMP_APP_EVENT_ALLMASK
#define _define___AMP_APP_EVENT_ALLMASK
#define AMP_APP_EVENT_ALLMASK (4095)
#endif

#ifndef _define___AMP_APP_MAX_EQ_BAND
#define _define___AMP_APP_MAX_EQ_BAND
#define AMP_APP_MAX_EQ_BAND (8)
#endif

#ifndef _define___AMP_APP_MIXER_GEQ_BAND
#define _define___AMP_APP_MIXER_GEQ_BAND
#define AMP_APP_MIXER_GEQ_BAND (5)
#endif

#ifndef _define___AMP_APP_SRS_GEQ_BAND
#define _define___AMP_APP_SRS_GEQ_BAND
#define AMP_APP_SRS_GEQ_BAND (5)
#endif

#ifndef _define___AMP_APP_SRS_TRUEQ_FLT_LEN
#define _define___AMP_APP_SRS_TRUEQ_FLT_LEN
#define AMP_APP_SRS_TRUEQ_FLT_LEN (56)
#endif

#ifndef _define___AMP_APP_SRS_HPF_FLT_LEN
#define _define___AMP_APP_SRS_HPF_FLT_LEN
#define AMP_APP_SRS_HPF_FLT_LEN (19)
#endif

#ifndef _define___AMP_APP_MAX_INPORT_NR
#define _define___AMP_APP_MAX_INPORT_NR
#define AMP_APP_MAX_INPORT_NR (5)
#endif

#ifndef _define___AMP_APP_MAX_OUTPORT_NR
#define _define___AMP_APP_MAX_OUTPORT_NR
#define AMP_APP_MAX_OUTPORT_NR (6)
#endif

#ifndef _define___AMP_APP_MAX_NS227_IN_CH
#define _define___AMP_APP_MAX_NS227_IN_CH
#define AMP_APP_MAX_NS227_IN_CH (2)
#endif

#ifndef _define___AMP_APP_MAX_NS527_IN_CH
#define _define___AMP_APP_MAX_NS527_IN_CH
#define AMP_APP_MAX_NS527_IN_CH (6)
#endif

#ifndef _define___AMP_APP_MAX_NEURAL_OUT_CH
#define _define___AMP_APP_MAX_NEURAL_OUT_CH
#define AMP_APP_MAX_NEURAL_OUT_CH (8)
#endif

#ifndef _define___AMP_APP_MAX_AUDIO_BUF_NR
#define _define___AMP_APP_MAX_AUDIO_BUF_NR
#define AMP_APP_MAX_AUDIO_BUF_NR (10)
#endif

#ifndef _define___AMP_APP_GEQ_MAX_BANDS
#define _define___AMP_APP_GEQ_MAX_BANDS
#define AMP_APP_GEQ_MAX_BANDS (20)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_12
#define _define___AMP_APP_GEQ_GAIN_NG_12
#define AMP_APP_GEQ_GAIN_NG_12 (0)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_11
#define _define___AMP_APP_GEQ_GAIN_NG_11
#define AMP_APP_GEQ_GAIN_NG_11 (1)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_10
#define _define___AMP_APP_GEQ_GAIN_NG_10
#define AMP_APP_GEQ_GAIN_NG_10 (2)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_9
#define _define___AMP_APP_GEQ_GAIN_NG_9
#define AMP_APP_GEQ_GAIN_NG_9 (3)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_8
#define _define___AMP_APP_GEQ_GAIN_NG_8
#define AMP_APP_GEQ_GAIN_NG_8 (4)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_7
#define _define___AMP_APP_GEQ_GAIN_NG_7
#define AMP_APP_GEQ_GAIN_NG_7 (5)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_6
#define _define___AMP_APP_GEQ_GAIN_NG_6
#define AMP_APP_GEQ_GAIN_NG_6 (6)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_5
#define _define___AMP_APP_GEQ_GAIN_NG_5
#define AMP_APP_GEQ_GAIN_NG_5 (7)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_4
#define _define___AMP_APP_GEQ_GAIN_NG_4
#define AMP_APP_GEQ_GAIN_NG_4 (8)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_3
#define _define___AMP_APP_GEQ_GAIN_NG_3
#define AMP_APP_GEQ_GAIN_NG_3 (9)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_2
#define _define___AMP_APP_GEQ_GAIN_NG_2
#define AMP_APP_GEQ_GAIN_NG_2 (10)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_NG_1
#define _define___AMP_APP_GEQ_GAIN_NG_1
#define AMP_APP_GEQ_GAIN_NG_1 (11)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_0
#define _define___AMP_APP_GEQ_GAIN_0
#define AMP_APP_GEQ_GAIN_0 (12)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_1
#define _define___AMP_APP_GEQ_GAIN_1
#define AMP_APP_GEQ_GAIN_1 (13)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_2
#define _define___AMP_APP_GEQ_GAIN_2
#define AMP_APP_GEQ_GAIN_2 (14)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_3
#define _define___AMP_APP_GEQ_GAIN_3
#define AMP_APP_GEQ_GAIN_3 (15)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_4
#define _define___AMP_APP_GEQ_GAIN_4
#define AMP_APP_GEQ_GAIN_4 (16)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_5
#define _define___AMP_APP_GEQ_GAIN_5
#define AMP_APP_GEQ_GAIN_5 (17)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_6
#define _define___AMP_APP_GEQ_GAIN_6
#define AMP_APP_GEQ_GAIN_6 (18)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_7
#define _define___AMP_APP_GEQ_GAIN_7
#define AMP_APP_GEQ_GAIN_7 (19)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_8
#define _define___AMP_APP_GEQ_GAIN_8
#define AMP_APP_GEQ_GAIN_8 (20)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_9
#define _define___AMP_APP_GEQ_GAIN_9
#define AMP_APP_GEQ_GAIN_9 (21)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_10
#define _define___AMP_APP_GEQ_GAIN_10
#define AMP_APP_GEQ_GAIN_10 (22)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_11
#define _define___AMP_APP_GEQ_GAIN_11
#define AMP_APP_GEQ_GAIN_11 (23)
#endif

#ifndef _define___AMP_APP_GEQ_GAIN_12
#define _define___AMP_APP_GEQ_GAIN_12
#define AMP_APP_GEQ_GAIN_12 (24)
#endif

#ifndef _define___AMP_APP_NUM_GEQ_GAIN_LEVEL
#define _define___AMP_APP_NUM_GEQ_GAIN_LEVEL
#define AMP_APP_NUM_GEQ_GAIN_LEVEL (25)
#endif

#ifndef _typedef___AMP_APP_GEQ_GAIN_LEVEL
#define _typedef___AMP_APP_GEQ_GAIN_LEVEL
typedef CORBA_enum AMP_APP_GEQ_GAIN_LEVEL;
#endif /* _typedef___AMP_APP_GEQ_GAIN_LEVEL */

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_12
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_12
#define AMP_APP_TREBLE_BASS_GAIN_NG_12 (0)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_11
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_11
#define AMP_APP_TREBLE_BASS_GAIN_NG_11 (1)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_10
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_10
#define AMP_APP_TREBLE_BASS_GAIN_NG_10 (2)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_9
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_9
#define AMP_APP_TREBLE_BASS_GAIN_NG_9 (3)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_8
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_8
#define AMP_APP_TREBLE_BASS_GAIN_NG_8 (4)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_7
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_7
#define AMP_APP_TREBLE_BASS_GAIN_NG_7 (5)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_6
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_6
#define AMP_APP_TREBLE_BASS_GAIN_NG_6 (6)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_5
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_5
#define AMP_APP_TREBLE_BASS_GAIN_NG_5 (7)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_4
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_4
#define AMP_APP_TREBLE_BASS_GAIN_NG_4 (8)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_3
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_3
#define AMP_APP_TREBLE_BASS_GAIN_NG_3 (9)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_2
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_2
#define AMP_APP_TREBLE_BASS_GAIN_NG_2 (10)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_NG_1
#define _define___AMP_APP_TREBLE_BASS_GAIN_NG_1
#define AMP_APP_TREBLE_BASS_GAIN_NG_1 (11)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_0
#define _define___AMP_APP_TREBLE_BASS_GAIN_0
#define AMP_APP_TREBLE_BASS_GAIN_0 (12)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_1
#define _define___AMP_APP_TREBLE_BASS_GAIN_1
#define AMP_APP_TREBLE_BASS_GAIN_1 (13)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_2
#define _define___AMP_APP_TREBLE_BASS_GAIN_2
#define AMP_APP_TREBLE_BASS_GAIN_2 (14)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_3
#define _define___AMP_APP_TREBLE_BASS_GAIN_3
#define AMP_APP_TREBLE_BASS_GAIN_3 (15)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_4
#define _define___AMP_APP_TREBLE_BASS_GAIN_4
#define AMP_APP_TREBLE_BASS_GAIN_4 (16)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_5
#define _define___AMP_APP_TREBLE_BASS_GAIN_5
#define AMP_APP_TREBLE_BASS_GAIN_5 (17)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_6
#define _define___AMP_APP_TREBLE_BASS_GAIN_6
#define AMP_APP_TREBLE_BASS_GAIN_6 (18)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_7
#define _define___AMP_APP_TREBLE_BASS_GAIN_7
#define AMP_APP_TREBLE_BASS_GAIN_7 (19)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_8
#define _define___AMP_APP_TREBLE_BASS_GAIN_8
#define AMP_APP_TREBLE_BASS_GAIN_8 (20)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_9
#define _define___AMP_APP_TREBLE_BASS_GAIN_9
#define AMP_APP_TREBLE_BASS_GAIN_9 (21)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_10
#define _define___AMP_APP_TREBLE_BASS_GAIN_10
#define AMP_APP_TREBLE_BASS_GAIN_10 (22)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_11
#define _define___AMP_APP_TREBLE_BASS_GAIN_11
#define AMP_APP_TREBLE_BASS_GAIN_11 (23)
#endif

#ifndef _define___AMP_APP_TREBLE_BASS_GAIN_12
#define _define___AMP_APP_TREBLE_BASS_GAIN_12
#define AMP_APP_TREBLE_BASS_GAIN_12 (24)
#endif

#ifndef _define___AMP_APP_NUM_TREBLE_BASS_GAIN_LEVEL
#define _define___AMP_APP_NUM_TREBLE_BASS_GAIN_LEVEL
#define AMP_APP_NUM_TREBLE_BASS_GAIN_LEVEL (25)
#endif

#ifndef _typedef___AMP_APP_TREBLE_BASS_GAIN_LEVEL
#define _typedef___AMP_APP_TREBLE_BASS_GAIN_LEVEL
typedef CORBA_enum AMP_APP_TREBLE_BASS_GAIN_LEVEL;
#endif /* _typedef___AMP_APP_TREBLE_BASS_GAIN_LEVEL */

#ifndef _define___AMP_APP_HPF_COFREQ_50HZ
#define _define___AMP_APP_HPF_COFREQ_50HZ
#define AMP_APP_HPF_COFREQ_50HZ (0)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_60HZ
#define _define___AMP_APP_HPF_COFREQ_60HZ
#define AMP_APP_HPF_COFREQ_60HZ (1)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_70HZ
#define _define___AMP_APP_HPF_COFREQ_70HZ
#define AMP_APP_HPF_COFREQ_70HZ (2)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_80HZ
#define _define___AMP_APP_HPF_COFREQ_80HZ
#define AMP_APP_HPF_COFREQ_80HZ (3)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_90HZ
#define _define___AMP_APP_HPF_COFREQ_90HZ
#define AMP_APP_HPF_COFREQ_90HZ (4)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_100HZ
#define _define___AMP_APP_HPF_COFREQ_100HZ
#define AMP_APP_HPF_COFREQ_100HZ (5)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_120HZ
#define _define___AMP_APP_HPF_COFREQ_120HZ
#define AMP_APP_HPF_COFREQ_120HZ (6)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_140HZ
#define _define___AMP_APP_HPF_COFREQ_140HZ
#define AMP_APP_HPF_COFREQ_140HZ (7)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_160HZ
#define _define___AMP_APP_HPF_COFREQ_160HZ
#define AMP_APP_HPF_COFREQ_160HZ (8)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_180HZ
#define _define___AMP_APP_HPF_COFREQ_180HZ
#define AMP_APP_HPF_COFREQ_180HZ (9)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_200HZ
#define _define___AMP_APP_HPF_COFREQ_200HZ
#define AMP_APP_HPF_COFREQ_200HZ (10)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_250HZ
#define _define___AMP_APP_HPF_COFREQ_250HZ
#define AMP_APP_HPF_COFREQ_250HZ (11)
#endif

#ifndef _define___AMP_APP_HPF_COFREQ_300HZ
#define _define___AMP_APP_HPF_COFREQ_300HZ
#define AMP_APP_HPF_COFREQ_300HZ (12)
#endif

#ifndef _typedef___AMP_APP_HPF_COFREQ
#define _typedef___AMP_APP_HPF_COFREQ
typedef CORBA_enum AMP_APP_HPF_COFREQ;
#endif /* _typedef___AMP_APP_HPF_COFREQ */

#ifndef _define___AMP_APP_SRC_SAMPRATE_DEFAULT
#define _define___AMP_APP_SRC_SAMPRATE_DEFAULT
#define AMP_APP_SRC_SAMPRATE_DEFAULT (0)
#endif

#ifndef _define___AMP_APP_SRC_SAMPRATE_PRIPORT
#define _define___AMP_APP_SRC_SAMPRATE_PRIPORT
#define AMP_APP_SRC_SAMPRATE_PRIPORT (1)
#endif

#ifndef _define___AMP_APP_SRC_SAMPRATE_USERSET
#define _define___AMP_APP_SRC_SAMPRATE_USERSET
#define AMP_APP_SRC_SAMPRATE_USERSET (2)
#endif

#ifndef _define___AMP_APP_SRC_SAMPRATE_SYSTEM
#define _define___AMP_APP_SRC_SAMPRATE_SYSTEM
#define AMP_APP_SRC_SAMPRATE_SYSTEM (3)
#endif

#ifndef _typedef___AMP_APP_SRC_SAMPRATE_MODE
#define _typedef___AMP_APP_SRC_SAMPRATE_MODE
typedef CORBA_enum AMP_APP_SRC_SAMPRATE_MODE;
#endif /* _typedef___AMP_APP_SRC_SAMPRATE_MODE */

#ifndef _define___AMP_APP_PORT_STOP
#define _define___AMP_APP_PORT_STOP
#define AMP_APP_PORT_STOP (0)
#endif

#ifndef _define___AMP_APP_PORT_PLAY
#define _define___AMP_APP_PORT_PLAY
#define AMP_APP_PORT_PLAY (1)
#endif

#ifndef _define___AMP_APP_PORT_PAUSE
#define _define___AMP_APP_PORT_PAUSE
#define AMP_APP_PORT_PAUSE (2)
#endif

#ifndef _typedef___AMP_APP_PORT_STATUS
#define _typedef___AMP_APP_PORT_STATUS
typedef CORBA_enum AMP_APP_PORT_STATUS;
#endif /* _typedef___AMP_APP_PORT_STATUS */

#ifndef _define___AMP_APP_DUMP_SUBID_IOSTATUS
#define _define___AMP_APP_DUMP_SUBID_IOSTATUS
#define AMP_APP_DUMP_SUBID_IOSTATUS (0)
#endif

#ifndef _typedef___AMP_APP_DUMP_SUBID
#define _typedef___AMP_APP_DUMP_SUBID
typedef CORBA_enum AMP_APP_DUMP_SUBID;
#endif /* _typedef___AMP_APP_DUMP_SUBID */

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_40HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_40HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_40HZ (0)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_60HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_60HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_60HZ (1)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_100HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_100HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_100HZ (2)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_150HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_150HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_150HZ (3)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_200HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_200HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_200HZ (4)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_250HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_250HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_250HZ (5)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_300HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_300HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_300HZ (6)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_400HZ
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_400HZ
#define AMP_APP_SRS_TRUBASS_SPKSIZE_400HZ (7)
#endif

#ifndef _define___AMP_APP_SRS_TRUBASS_SPKSIZE_NUM
#define _define___AMP_APP_SRS_TRUBASS_SPKSIZE_NUM
#define AMP_APP_SRS_TRUBASS_SPKSIZE_NUM (8)
#endif

#ifndef _typedef___AMP_APP_SRS_TRUBASS_SPKSIZE
#define _typedef___AMP_APP_SRS_TRUBASS_SPKSIZE
typedef CORBA_enum AMP_APP_SRS_TRUBASS_SPKSIZE;
#endif /* _typedef___AMP_APP_SRS_TRUBASS_SPKSIZE */

#ifndef _define___AMP_APP_MODE_NORMAL
#define _define___AMP_APP_MODE_NORMAL
#define AMP_APP_MODE_NORMAL (0)
#endif

#ifndef _typedef___AMP_APP_SPECIALMODE
#define _typedef___AMP_APP_SPECIALMODE
typedef CORBA_enum AMP_APP_SPECIALMODE;
#endif /* _typedef___AMP_APP_SPECIALMODE */

#ifndef _typedef___AMP_APP_PARAVOLUME
#define _typedef___AMP_APP_PARAVOLUME
typedef struct AMP_APP_PARAVOLUME {
  UINT32 uiVolume;
} AMP_APP_PARAVOLUME;
#endif /* _typedef___AMP_APP_PARAVOLUME */

#ifndef _typedef___AMP_APP_PARABALANCE
#define _typedef___AMP_APP_PARABALANCE
typedef struct AMP_APP_PARABALANCE {
  UINT32 uiLeftVolume;
  UINT32 uiRghtVolume;
} AMP_APP_PARABALANCE;
#endif /* _typedef___AMP_APP_PARABALANCE */

#ifndef _typedef___AMP_APP_PARAGEQ
#define _typedef___AMP_APP_PARAGEQ
typedef struct AMP_APP_PARAGEQ {
  UINT8 cEnable;
  AMP_APP_GEQ_GAIN_LEVEL eGEQGain[8];
} AMP_APP_PARAGEQ;
#endif /* _typedef___AMP_APP_PARAGEQ */

#ifndef _typedef___AMP_APP_PARATREBLEBASS
#define _typedef___AMP_APP_PARATREBLEBASS
typedef struct AMP_APP_PARATREBLEBASS {
  UINT8 cEnable;
  AMP_APP_TREBLE_BASS_GAIN_LEVEL eGain;
} AMP_APP_PARATREBLEBASS;
#endif /* _typedef___AMP_APP_PARATREBLEBASS */

#ifndef _typedef___AMP_APP_PARAHPF
#define _typedef___AMP_APP_PARAHPF
typedef struct AMP_APP_PARAHPF {
  UINT8 cEnable;
  AMP_APP_HPF_COFREQ eCoFreq;
} AMP_APP_PARAHPF;
#endif /* _typedef___AMP_APP_PARAHPF */

#ifndef _typedef___AMP_APP_PARADELAY
#define _typedef___AMP_APP_PARADELAY
typedef struct AMP_APP_PARADELAY {
  UINT8 cEnable;
  INT32 uiDelayInMs;
} AMP_APP_PARADELAY;
#endif /* _typedef___AMP_APP_PARADELAY */

#ifndef _typedef___AMP_APP_PARASTEREO
#define _typedef___AMP_APP_PARASTEREO
typedef struct AMP_APP_PARASTEREO {
  AMP_AUDIO_STEREO_MODE eStereoMode;
} AMP_APP_PARASTEREO;
#endif /* _typedef___AMP_APP_PARASTEREO */

#ifndef _typedef___AMP_APP_PARADUALMODE
#define _typedef___AMP_APP_PARADUALMODE
typedef struct AMP_APP_PARADUALMODE {
  AMP_AUDIO_DUAL_MODE eDualMode;
} AMP_APP_PARADUALMODE;
#endif /* _typedef___AMP_APP_PARADUALMODE */

#ifndef _typedef___AMP_APP_PARALFEDMX
#define _typedef___AMP_APP_PARALFEDMX
typedef struct AMP_APP_PARALFEDMX {
  UINT8 cEnable;
} AMP_APP_PARALFEDMX;
#endif /* _typedef___AMP_APP_PARALFEDMX */

#ifndef _typedef___AMP_APP_PARASPKMODE
#define _typedef___AMP_APP_PARASPKMODE
typedef struct AMP_APP_PARASPKMODE {
  AMP_AUDIO_PRICHMAP_MODE eSpkMode;
  AMP_AUDIO_LFE_MODE eLfeMode;
  AMP_AUDIO_TOP_MODE eTopMode;
} AMP_APP_PARASPKMODE;
#endif /* _typedef___AMP_APP_PARASPKMODE */

#ifndef _typedef___AMP_APP_PARAMIXGAIN
#define _typedef___AMP_APP_PARAMIXGAIN
typedef struct AMP_APP_PARAMIXGAIN {
  UINT32 uiLeftGain;
  UINT32 uiRghtGain;
  UINT32 uiCntrGain;
  UINT32 uiLeftSndGain;
  UINT32 uiRghtSndGain;
  UINT32 uiLfeGain;
  UINT32 uiLeftRearGain;
  UINT32 uiRhgtRearGain;
} AMP_APP_PARAMIXGAIN;
#endif /* _typedef___AMP_APP_PARAMIXGAIN */

#ifndef _typedef___AMP_APP_PARASRSGEN
#define _typedef___AMP_APP_PARASRSGEN
typedef struct AMP_APP_PARASRSGEN {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiInputGain;
  UINT32 uiOutputGain;
  UINT32 uiBypassGain;
  UINT32 uiHeadRoomGain;
  UINT32 uiInputMode;
} AMP_APP_PARASRSGEN;
#endif /* _typedef___AMP_APP_PARASRSGEN */

#ifndef _typedef___AMP_APP_PARASRSTRUVOL
#define _typedef___AMP_APP_PARASRSTRUVOL
typedef struct AMP_APP_PARASRSTRUVOL {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiVolumeInputGain;
  UINT32 uiVolumeOutputGain;
  UINT32 uiVolumeBypassGain;
  UINT32 uiVolumeMode;
} AMP_APP_PARASRSTRUVOL;
#endif /* _typedef___AMP_APP_PARASRSTRUVOL */

#ifndef _typedef___AMP_APP_PARASRSTRUSUR
#define _typedef___AMP_APP_PARASRSTRUSUR
typedef struct AMP_APP_PARASRSTRUSUR {
  UINT8 cEnable;
  UINT8 cWOWHDSRS3DEnable;
  UINT8 cWOWHDFOCUSEnable;
  UINT8 cWOWHDTrubassEnable;
  UINT8 cWOWHDDefinitionEnable;
  UINT8 cDialogClarityEnable;
  UINT8 cDefaultParameters;
  UINT32 uiWOWHDSRS3DHighBitRate;
  UINT32 uiWOWHDSRS3DSpaceControl;
  UINT32 uiWOWHDSRS3DCenterControl;
  UINT32 uiWOWHDSRS3DMode;
  UINT32 uiWOWHDFOCUSFOCUSControl;
  UINT32 uiTruSurroundHDSurroundLevel;
  UINT32 uiTruSurroundHDInputGain;
  UINT32 uiTruSurroundHDOutputGain;
  UINT32 uiTruSurroundHDBypassGain;
  UINT32 uiWOWHDDefinitionControl;
  UINT32 uiDialogClarityFOCUSControl;
  UINT32 uiWOWHDTrubassTruBassControl;
  UINT32 uiWOWHDTrubassFrontSpeakerSize;
  UINT32 uiWOWHDTrubassProcessMode;
  UINT32 uiTruSurroundHDOutputMode;
  UINT32 uiTruSurroundHDInputMode;
} AMP_APP_PARASRSTRUSUR;
#endif /* _typedef___AMP_APP_PARASRSTRUSUR */

#ifndef _typedef___AMP_APP_PARASRSCS
#define _typedef___AMP_APP_PARASRSCS
typedef struct AMP_APP_PARASRSCS {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiCSDecoderInputGain;
  UINT32 uiCSDecoderMode;
  UINT32 uiCSDecOutputGainLR;
  UINT32 uiCSDecOutputGainLsRs;
  UINT32 uiCSDecOutputGainC;
  UINT32 uiCSDecOutputGainSub;
} AMP_APP_PARASRSCS;
#endif /* _typedef___AMP_APP_PARASRSCS */

#ifndef _typedef___AMP_APP_PARASRSTRUDIAL
#define _typedef___AMP_APP_PARASRSTRUDIAL
typedef struct AMP_APP_PARASRSTRUDIAL {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiTruDialogInputGain;
  UINT32 uiTruDialogOutputGain;
  UINT32 uiTruDialogBypassGain;
  UINT32 uiTruDialogProcessGain;
  UINT32 uiTruDialogDialogClarityGain;
} AMP_APP_PARASRSTRUDIAL;
#endif /* _typedef___AMP_APP_PARASRSTRUDIAL */

#ifndef _typedef___AMP_APP_PARASRSHDLIM
#define _typedef___AMP_APP_PARASRSHDLIM
typedef struct AMP_APP_PARASRSHDLIM {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiInputGain;
  UINT32 uiOutputGain;
  UINT32 uiBypassGain;
  UINT32 uiLimiterBoost;
  UINT32 uiHardLimit;
  UINT32 uiDelayLength;
} AMP_APP_PARASRSHDLIM;
#endif /* _typedef___AMP_APP_PARASRSHDLIM */

#ifndef _typedef___AMP_APP_PARASRSTRUEQ
#define _typedef___AMP_APP_PARASRSTRUEQ
typedef struct AMP_APP_PARASRSTRUEQ {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiInputGain;
  UINT32 uiOutputGain;
  UINT32 uiBypassGain;
  UINT8 cBandEnable[8];
  UINT32 uiBandNum;
  UINT32 uiChannelIdx;
} AMP_APP_PARASRSTRUEQ;
#endif /* _typedef___AMP_APP_PARASRSTRUEQ */

#ifndef _typedef___AMP_APP_PARASRSGEQ
#define _typedef___AMP_APP_PARASRSGEQ
typedef struct AMP_APP_PARASRSGEQ {
  UINT8 cEnable;
  UINT8 cDefaultParameters;
  UINT32 uiInputGain;
  UINT32 uiOutputGain;
  UINT32 uiBypassGain;
  UINT32 uiBandGain[5];
  UINT32 uiChannelIdx;
} AMP_APP_PARASRSGEQ;
#endif /* _typedef___AMP_APP_PARASRSGEQ */

#ifndef _typedef___AMP_APP_PARASRSTRUEQFLT
#define _typedef___AMP_APP_PARASRSTRUEQFLT
typedef struct AMP_APP_PARASRSTRUEQFLT {
  UINT32 uiSampleRate;
  UINT32 uiFilterCoef[56];
} AMP_APP_PARASRSTRUEQFLT;
#endif /* _typedef___AMP_APP_PARASRSTRUEQFLT */

#ifndef _typedef___AMP_APP_PARASRSHPFCONFIG
#define _typedef___AMP_APP_PARASRSHPFCONFIG
typedef struct AMP_APP_PARASRSHPFCONFIG {
  UINT8 cEnable;
  UINT8 cStartFilter;
} AMP_APP_PARASRSHPFCONFIG;
#endif /* _typedef___AMP_APP_PARASRSHPFCONFIG */

#ifndef _typedef___AMP_APP_PARASRSHPFFLT
#define _typedef___AMP_APP_PARASRSHPFFLT
typedef struct AMP_APP_PARASRSHPFFLT {
  UINT32 uiSampleRate;
  UINT32 uiFilterCoef[19];
  UINT8 cStartFilter;
} AMP_APP_PARASRSHPFFLT;
#endif /* _typedef___AMP_APP_PARASRSHPFFLT */

#ifndef _typedef___AMP_APP_PARAPORTSTATUS
#define _typedef___AMP_APP_PARAPORTSTATUS
typedef struct AMP_APP_PARAPORTSTATUS {
  AMP_APP_PORT_STATUS eStatus;
} AMP_APP_PARAPORTSTATUS;
#endif /* _typedef___AMP_APP_PARAPORTSTATUS */

#ifndef _typedef___AMP_APP_PARAPORTDATAFMT
#define _typedef___AMP_APP_PARAPORTDATAFMT
typedef struct AMP_APP_PARAPORTDATAFMT {
  UINT32 uiDataFmt;
} AMP_APP_PARAPORTDATAFMT;
#endif /* _typedef___AMP_APP_PARAPORTDATAFMT */

#ifndef _typedef___AMP_APP_PARASRCCFG
#define _typedef___AMP_APP_PARASRCCFG
typedef struct AMP_APP_PARASRCCFG {
  UINT32 uiMode;
  UINT32 uiPortIdx;
  UINT32 uiOutSampRate;
  UINT32 uiQuality;
} AMP_APP_PARASRCCFG;
#endif /* _typedef___AMP_APP_PARASRCCFG */

#ifndef _typedef___AMP_APP_PARASONICCFG
#define _typedef___AMP_APP_PARASONICCFG
typedef struct AMP_APP_PARASONICCFG {
  UINT32 uiEnable;
  UINT32 uiSpeed;
} AMP_APP_PARASONICCFG;
#endif /* _typedef___AMP_APP_PARASONICCFG */

#ifndef _typedef___AMP_APP_PARARESETPARA
#define _typedef___AMP_APP_PARARESETPARA
typedef struct AMP_APP_PARARESETPARA {
  UINT32 uiResetMode;
} AMP_APP_PARARESETPARA;
#endif /* _typedef___AMP_APP_PARARESETPARA */

#ifndef _typedef___AMP_APP_PARABIQUAFILTER
#define _typedef___AMP_APP_PARABIQUAFILTER
typedef struct AMP_APP_PARABIQUAFILTER {
  UINT8 cEnable;
  UINT32 uiChannelIdx;
  UINT32 uiFilterIdx;
  UINT32 uiGain0;
  UINT32 uiGain1;
  UINT32 uiGain2;
  UINT32 uiCoefA0;
  UINT32 uiCoefA1;
  UINT32 uiCoefA2;
  UINT32 uiCoefB1;
  UINT32 uiCoefB2;
  UINT32 uiSampRate;
} AMP_APP_PARABIQUAFILTER;
#endif /* _typedef___AMP_APP_PARABIQUAFILTER */

#ifndef _typedef___AMP_APP_PARABIQUAFILTERGAIN
#define _typedef___AMP_APP_PARABIQUAFILTERGAIN
typedef struct AMP_APP_PARABIQUAFILTERGAIN {
  UINT8 cEnable;
  UINT32 uiChannelIdx;
  UINT32 uiFilterIdx;
  UINT32 uiGain0;
  UINT32 uiGain1;
  UINT32 uiGain2;
} AMP_APP_PARABIQUAFILTERGAIN;
#endif /* _typedef___AMP_APP_PARABIQUAFILTERGAIN */

#ifndef _typedef___AMP_APP_PARABIQUAFILTERCOEF
#define _typedef___AMP_APP_PARABIQUAFILTERCOEF
typedef struct AMP_APP_PARABIQUAFILTERCOEF {
  UINT32 uiChannelIdx;
  UINT32 uiFilterIdx;
  UINT32 uiSampRate;
  UINT32 uiCoefA0;
  UINT32 uiCoefA1;
  UINT32 uiCoefA2;
  UINT32 uiCoefB1;
  UINT32 uiCoefB2;
} AMP_APP_PARABIQUAFILTERCOEF;
#endif /* _typedef___AMP_APP_PARABIQUAFILTERCOEF */

#ifndef _typedef___AMP_APP_PARANOISEDETECTCFG
#define _typedef___AMP_APP_PARANOISEDETECTCFG
typedef struct AMP_APP_PARANOISEDETECTCFG {
  UINT32 uiParaIdx;
  UINT32 uiValue;
} AMP_APP_PARANOISEDETECTCFG;
#endif /* _typedef___AMP_APP_PARANOISEDETECTCFG */

#ifndef _typedef___AMP_APP_PARAMIXCFG
#define _typedef___AMP_APP_PARAMIXCFG
typedef struct AMP_APP_PARAMIXCFG {
  BOOL bMainAud;
} AMP_APP_PARAMIXCFG;
#endif /* _typedef___AMP_APP_PARAMIXCFG */

#ifndef _typedef___AMP_APP_PARAASSOCIATE
#define _typedef___AMP_APP_PARAASSOCIATE
typedef struct AMP_APP_PARAASSOCIATE {
  BOOL bEnable;
  UINT32 uiMainPortIdx;
  UINT32 uiADPortIdx;
} AMP_APP_PARAASSOCIATE;
#endif /* _typedef___AMP_APP_PARAASSOCIATE */

#ifndef _typedef___AMP_APP_PARAMULTI2BT
#define _typedef___AMP_APP_PARAMULTI2BT
typedef struct AMP_APP_PARAMULTI2BT {
  BOOL bMulti2BT;
} AMP_APP_PARAMULTI2BT;
#endif /* _typedef___AMP_APP_PARAMULTI2BT */

#ifndef _typedef___AMP_APP_PARACGMSA
#define _typedef___AMP_APP_PARACGMSA
typedef struct AMP_APP_PARACGMSA {
  UINT32 uiCGMSA;
} AMP_APP_PARACGMSA;
#endif /* _typedef___AMP_APP_PARACGMSA */

#ifndef _typedef___AMP_APP_PARACHANMASK
#define _typedef___AMP_APP_PARACHANMASK
typedef struct AMP_APP_PARACHANMASK {
  UINT32 uiChanMask[10];
} AMP_APP_PARACHANMASK;
#endif /* _typedef___AMP_APP_PARACHANMASK */

#ifndef _typedef___AMP_APP_PARASRSENABLE
#define _typedef___AMP_APP_PARASRSENABLE
typedef struct AMP_APP_PARASRSENABLE {
  BOOL bSRSEnable;
} AMP_APP_PARASRSENABLE;
#endif /* _typedef___AMP_APP_PARASRSENABLE */

#ifndef _typedef___AMP_APP_PARAHEADROOM
#define _typedef___AMP_APP_PARAHEADROOM
typedef struct AMP_APP_PARAHEADROOM {
  UINT32 uiVolume;
} AMP_APP_PARAHEADROOM;
#endif /* _typedef___AMP_APP_PARAHEADROOM */

#ifndef _typedef___AMP_APP_PARASRCOUTSAMPLERATE
#define _typedef___AMP_APP_PARASRCOUTSAMPLERATE
typedef struct AMP_APP_PARASRCOUTSAMPLERATE {
  UINT32 uiOutSampleRate;
} AMP_APP_PARASRCOUTSAMPLERATE;
#endif /* _typedef___AMP_APP_PARASRCOUTSAMPLERATE */

#ifndef _typedef___AMP_APP_PARAPATHMUTE
#define _typedef___AMP_APP_PARAPATHMUTE
typedef struct AMP_APP_PARAPATHMUTE {
  BOOL bPathMute;
} AMP_APP_PARAPATHMUTE;
#endif /* _typedef___AMP_APP_PARAPATHMUTE */

#ifndef _typedef___AMP_APP_PARANEURALENABLE
#define _typedef___AMP_APP_PARANEURALENABLE
typedef struct AMP_APP_PARANEURALENABLE {
  BOOL bNeuralEnable;
  BOOL bConfigValid;
  UINT32 uiDownMixType;
  UINT32 uiChConfigID;
} AMP_APP_PARANEURALENABLE;
#endif /* _typedef___AMP_APP_PARANEURALENABLE */

#ifndef _typedef___AMP_APP_PARANS227CFG
#define _typedef___AMP_APP_PARANS227CFG
typedef struct AMP_APP_PARANS227CFG {
  UINT8 cDefaultParameters;
  INT32 iDecoderMode;
  INT32 iChanConfig;
  INT32 iLatencyMode;
  INT32 iLimiter;
  INT32 iInputGain[2];
  INT32 iOutputGain[8];
  INT32 iDICEProcLevel;
  INT32 iDepth;
  INT32 iFrontWidth;
  INT32 iBassManXOver;
  INT32 iSurroundDelay;
  INT32 iLimiterCeiling;
} AMP_APP_PARANS227CFG;
#endif /* _typedef___AMP_APP_PARANS227CFG */

#ifndef _typedef___AMP_APP_PARANS527CFG
#define _typedef___AMP_APP_PARANS527CFG
typedef struct AMP_APP_PARANS527CFG {
  UINT8 cDefaultParameters;
  INT32 iChanConfig;
  INT32 iLatencyMode;
  INT32 iLimiter;
  INT32 iInputGain[6];
  INT32 iOutputGain[8];
  INT32 iSurroundDepth;
  INT32 iLimiterCeiling;
  INT32 iLFECutOff;
} AMP_APP_PARANS527CFG;
#endif /* _typedef___AMP_APP_PARANS527CFG */

#ifndef _typedef___AMP_APP_PARAAUDIOPATCHCFG
#define _typedef___AMP_APP_PARAAUDIOPATCHCFG
typedef struct AMP_APP_PARAAUDIOPATCHCFG {
  BOOL bCreate;
  BOOL bDevice;
  UINT32 uiDeviceType;
  UINT8 iHwAddress;
  UINT32 uiSndPortIdx;
  UINT32 uiSinkPath[6];
  UINT32 uiSinkDataFormat[6];
  UINT32 uiSinkNum;
  BOOL bWaitLinkPatchStream;
} AMP_APP_PARAAUDIOPATCHCFG;
#endif /* _typedef___AMP_APP_PARAAUDIOPATCHCFG */

#ifndef _typedef___AMP_APP_PARAAUDIOPATCHSTATUS
#define _typedef___AMP_APP_PARAAUDIOPATCHSTATUS
typedef struct AMP_APP_PARAAUDIOPATCHSTATUS {
  BOOL bAudioPatchEnable;
} AMP_APP_PARAAUDIOPATCHSTATUS;
#endif /* _typedef___AMP_APP_PARAAUDIOPATCHSTATUS */

#ifndef _typedef___AMP_APP_PARAHDMIMULTISTATUS
#define _typedef___AMP_APP_PARAHDMIMULTISTATUS
typedef struct AMP_APP_PARAHDMIMULTISTATUS {
  BOOL bHDMIMulti;
} AMP_APP_PARAHDMIMULTISTATUS;
#endif /* _typedef___AMP_APP_PARAHDMIMULTISTATUS */

#ifndef _typedef___AMP_APP_PARADEVICEATTACHCFG
#define _typedef___AMP_APP_PARADEVICEATTACHCFG
typedef struct AMP_APP_PARADEVICEATTACHCFG {
  UINT32 uiDeviceType;
  UINT8 iHwAddress;
  UINT32 uiSndPortIdx;
  UINT32 hSinkHandle;
} AMP_APP_PARADEVICEATTACHCFG;
#endif /* _typedef___AMP_APP_PARADEVICEATTACHCFG */

#ifndef _typedef___AMP_APP_PARAIDXAPPCAP
#define _typedef___AMP_APP_PARAIDXAPPCAP
typedef struct AMP_APP_PARAIDXAPPCAP {
  AMP_APP_FORMAT uCompFormat;
  BOOL bSupport;
  AMP_APP_SPECIALMODE uSpecialMode;
} AMP_APP_PARAIDXAPPCAP;
#endif /* _typedef___AMP_APP_PARAIDXAPPCAP */

#ifndef _typedef___AMP_APP_PARAMIXPORTUSED
#define _typedef___AMP_APP_PARAMIXPORTUSED
typedef struct AMP_APP_PARAMIXPORTUSED {
  BOOL bPortUsed;
} AMP_APP_PARAMIXPORTUSED;
#endif /* _typedef___AMP_APP_PARAMIXPORTUSED */

#ifndef _typedef___AMP_APP_PARATRICMODECF
#define _typedef___AMP_APP_PARATRICMODECF
typedef struct AMP_APP_PARATRICMODECF {
  UINT32 uiTrickMode;
} AMP_APP_PARATRICMODECF;
#endif /* _typedef___AMP_APP_PARATRICMODECF */

#ifndef _typedef___AMP_APP_PARASINKCAP
#define _typedef___AMP_APP_PARASINKCAP
typedef struct AMP_APP_PARASINKCAP {
  UINT32 uiMaxChan;
  UINT32 uiChanMask;
  UINT32 uiSampleRate;
} AMP_APP_PARASINKCAP;
#endif /* _typedef___AMP_APP_PARASINKCAP */

#ifndef _typedef___AMP_APP_PARAMS12SURRDEC
#define _typedef___AMP_APP_PARAMS12SURRDEC
typedef struct AMP_APP_PARAMS12SURRDEC {
  UINT32 uiSurrDecEn;
} AMP_APP_PARAMS12SURRDEC;
#endif /* _typedef___AMP_APP_PARAMS12SURRDEC */

#ifndef _typedef___AMP_APP_PARAMS12SURRVIR
#define _typedef___AMP_APP_PARAMS12SURRVIR
typedef struct AMP_APP_PARAMS12SURRVIR {
  UINT32 virtualizer_mode;
  UINT32 boost_level;
} AMP_APP_PARAMS12SURRVIR;
#endif /* _typedef___AMP_APP_PARAMS12SURRVIR */

#ifndef _typedef___AMP_APP_PARAMS12GEQ
#define _typedef___AMP_APP_PARAMS12GEQ
typedef struct AMP_APP_PARAMS12GEQ {
  UINT32 eq_enable;
  UINT32 eq_nb_bands;
  INT32 a_geq_band_center[20];
  INT32 a_geq_band_target[20];
} AMP_APP_PARAMS12GEQ;
#endif /* _typedef___AMP_APP_PARAMS12GEQ */

#ifndef _typedef___AMP_APP_PARAMS12GAIN
#define _typedef___AMP_APP_PARAMS12GAIN
typedef struct AMP_APP_PARAMS12GAIN {
  INT32 target_attenuation;
  INT32 duration_in_ms;
  INT32 type;
} AMP_APP_PARAMS12GAIN;
#endif /* _typedef___AMP_APP_PARAMS12GAIN */

#ifndef _typedef___AMP_APP_PARAMS12ATMOSLOCK
#define _typedef___AMP_APP_PARAMS12ATMOSLOCK
typedef struct AMP_APP_PARAMS12ATMOSLOCK {
  UINT32 uiAtmosLock;
} AMP_APP_PARAMS12ATMOSLOCK;
#endif /* _typedef___AMP_APP_PARAMS12ATMOSLOCK */

#ifndef _typedef___AMP_APP_PARAMS12DOWNMIXOUT
#define _typedef___AMP_APP_PARAMS12DOWNMIXOUT
typedef struct AMP_APP_PARAMS12DOWNMIXOUT {
  UINT32 uiDownmixType;
  UINT32 uiDrcMode;
  UINT32 uiDrcBoost;
  UINT32 uiDrcCut;
  BOOL bDownmixOut;
  BOOL bDualMono;
  UINT32 uiDualMode;
} AMP_APP_PARAMS12DOWNMIXOUT;
#endif /* _typedef___AMP_APP_PARAMS12DOWNMIXOUT */

#ifndef _typedef___AMP_APP_PARAAUDMODE
#define _typedef___AMP_APP_PARAAUDMODE
typedef struct AMP_APP_PARAAUDMODE {
  AMP_AUDIO_USER_MODE eAudioMode;
} AMP_APP_PARAAUDMODE;
#endif /* _typedef___AMP_APP_PARAAUDMODE */

#ifndef _typedef___AMP_APP_PARATUNFILE
#define _typedef___AMP_APP_PARATUNFILE
typedef struct AMP_APP_PARATUNFILE {
  CORBA_char tunfile[50];
} AMP_APP_PARATUNFILE;
#endif /* _typedef___AMP_APP_PARATUNFILE */

#ifndef _typedef___AMP_APP_PARAAUDCONTINUES
#define _typedef___AMP_APP_PARAAUDCONTINUES
typedef struct AMP_APP_PARAAUDCONTINUES {
  AMP_AUDIO_CONTINUES eContinuesAudio;
} AMP_APP_PARAAUDCONTINUES;
#endif /* _typedef___AMP_APP_PARAAUDCONTINUES */

#ifndef _typedef___AMP_APP_PARAMS12MEDIAINT
#define _typedef___AMP_APP_PARAMS12MEDIAINT
typedef struct AMP_APP_PARAMS12MEDIAINT {
  UINT8 cMiEnable;
} AMP_APP_PARAMS12MEDIAINT;
#endif /* _typedef___AMP_APP_PARAMS12MEDIAINT */

#ifndef _typedef___AMP_APP_PARAMS12VOLUMELEVELER
#define _typedef___AMP_APP_PARAMS12VOLUMELEVELER
typedef struct AMP_APP_PARAMS12VOLUMELEVELER {
  UINT32 uiSetting;
  UINT32 uiAmount;
} AMP_APP_PARAMS12VOLUMELEVELER;
#endif /* _typedef___AMP_APP_PARAMS12VOLUMELEVELER */

#ifndef _typedef___AMP_APP_PARAMS12ENFORCESINGLEOA
#define _typedef___AMP_APP_PARAMS12ENFORCESINGLEOA
typedef struct AMP_APP_PARAMS12ENFORCESINGLEOA {
  UINT32 uiEnable;
} AMP_APP_PARAMS12ENFORCESINGLEOA;
#endif /* _typedef___AMP_APP_PARAMS12ENFORCESINGLEOA */

#ifndef _typedef___AMP_APP_PARAMS12DIALOGENH
#define _typedef___AMP_APP_PARAMS12DIALOGENH
typedef struct AMP_APP_PARAMS12DIALOGENH {
  UINT32 uiEnable;
  INT32 iAmount;
} AMP_APP_PARAMS12DIALOGENH;
#endif /* _typedef___AMP_APP_PARAMS12DIALOGENH */

#ifndef _typedef___AMP_APP_PARAAUDCODECTYPE
#define _typedef___AMP_APP_PARAAUDCODECTYPE
typedef struct AMP_APP_PARAAUDCODECTYPE {
  UINT32 uiCodecType;
} AMP_APP_PARAAUDCODECTYPE;
#endif /* _typedef___AMP_APP_PARAAUDCODECTYPE */

#ifndef _typedef___AMP_APP_PARAMAUDCHMODLOCK
#define _typedef___AMP_APP_PARAMAUDCHMODLOCK
typedef struct AMP_APP_PARAMAUDCHMODLOCK {
  UINT32 uiMode;
} AMP_APP_PARAMAUDCHMODLOCK;
#endif /* _typedef___AMP_APP_PARAMAUDCHMODLOCK */

#ifndef _typedef___AMP_APP_FREQSPRT_CAP
#define _typedef___AMP_APP_FREQSPRT_CAP
typedef struct AMP_APP_FREQSPRT_CAP {
  CORBA_char Fs32KHz;
  CORBA_char Fs44_1KHz;
  CORBA_char Fs48KHz;
  CORBA_char Fs88_2KHz;
  CORBA_char Fs96KHz;
  CORBA_char Fs176_4KHz;
  CORBA_char Fs192KHz;
} AMP_APP_FREQSPRT_CAP;
#endif /* _typedef___AMP_APP_FREQSPRT_CAP */

#ifndef _typedef___AMP_APP_WDLENSPRT_CAP
#define _typedef___AMP_APP_WDLENSPRT_CAP
typedef struct AMP_APP_WDLENSPRT_CAP {
  CORBA_char WdLen16;
  CORBA_char WdLen20;
  CORBA_char WdLen24;
} AMP_APP_WDLENSPRT_CAP;
#endif /* _typedef___AMP_APP_WDLENSPRT_CAP */

#ifndef _typedef___AMP_APP_AUDIOBLOCKCAP
#define _typedef___AMP_APP_AUDIOBLOCKCAP
typedef struct AMP_APP_AUDIOBLOCKCAP {
  BOOL bSupport;
  UINT32 iMaxChannelNr;
  UINT32 iChannelMask;
  struct AMP_APP_WDLENSPRT_CAP stBitDepth;
  struct AMP_APP_FREQSPRT_CAP stSampleRate;
} AMP_APP_AUDIOBLOCKCAP;
#endif /* _typedef___AMP_APP_AUDIOBLOCKCAP */

#ifndef _typedef___AMP_APP_PARASINKPORTSCAP
#define _typedef___AMP_APP_PARASINKPORTSCAP
typedef struct AMP_APP_PARASINKPORTSCAP {
  struct AMP_APP_AUDIOBLOCKCAP stAudioBlock[16];
} AMP_APP_PARASINKPORTSCAP;
#endif /* _typedef___AMP_APP_PARASINKPORTSCAP */

#ifndef _typedef___AMP_APP_PARAMS12RST
#define _typedef___AMP_APP_PARAMS12RST
typedef struct AMP_APP_PARAMS12RST {
  BOOL bMs12Rst;
} AMP_APP_PARAMS12RST;
#endif /* _typedef___AMP_APP_PARAMS12RST */

#ifndef _define___AMP_APP_PARAIDX_VOLUME
#define _define___AMP_APP_PARAIDX_VOLUME
#define AMP_APP_PARAIDX_VOLUME (0)
#endif

#ifndef _define___AMP_APP_PARAIDX_BALANCE
#define _define___AMP_APP_PARAIDX_BALANCE
#define AMP_APP_PARAIDX_BALANCE (1)
#endif

#ifndef _define___AMP_APP_PARAIDX_GEQ
#define _define___AMP_APP_PARAIDX_GEQ
#define AMP_APP_PARAIDX_GEQ (2)
#endif

#ifndef _define___AMP_APP_PARAIDX_TREBLE
#define _define___AMP_APP_PARAIDX_TREBLE
#define AMP_APP_PARAIDX_TREBLE (3)
#endif

#ifndef _define___AMP_APP_PARAIDX_BASS
#define _define___AMP_APP_PARAIDX_BASS
#define AMP_APP_PARAIDX_BASS (4)
#endif

#ifndef _define___AMP_APP_PARAIDX_HPF
#define _define___AMP_APP_PARAIDX_HPF
#define AMP_APP_PARAIDX_HPF (5)
#endif

#ifndef _define___AMP_APP_PARAIDX_DELAY
#define _define___AMP_APP_PARAIDX_DELAY
#define AMP_APP_PARAIDX_DELAY (6)
#endif

#ifndef _define___AMP_APP_PARAIDX_STEREO
#define _define___AMP_APP_PARAIDX_STEREO
#define AMP_APP_PARAIDX_STEREO (7)
#endif

#ifndef _define___AMP_APP_PARAIDX_DUALMODE
#define _define___AMP_APP_PARAIDX_DUALMODE
#define AMP_APP_PARAIDX_DUALMODE (8)
#endif

#ifndef _define___AMP_APP_PARAIDX_LFEDMX
#define _define___AMP_APP_PARAIDX_LFEDMX
#define AMP_APP_PARAIDX_LFEDMX (9)
#endif

#ifndef _define___AMP_APP_PARAIDX_SPKMODE
#define _define___AMP_APP_PARAIDX_SPKMODE
#define AMP_APP_PARAIDX_SPKMODE (10)
#endif

#ifndef _define___AMP_APP_PARAIDX_MIXGAIN
#define _define___AMP_APP_PARAIDX_MIXGAIN
#define AMP_APP_PARAIDX_MIXGAIN (11)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSGEN
#define _define___AMP_APP_PARAIDX_SRSGEN
#define AMP_APP_PARAIDX_SRSGEN (12)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSTRUVOL
#define _define___AMP_APP_PARAIDX_SRSTRUVOL
#define AMP_APP_PARAIDX_SRSTRUVOL (13)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSTRUSUR
#define _define___AMP_APP_PARAIDX_SRSTRUSUR
#define AMP_APP_PARAIDX_SRSTRUSUR (14)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSCS
#define _define___AMP_APP_PARAIDX_SRSCS
#define AMP_APP_PARAIDX_SRSCS (15)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSTRUDIAL
#define _define___AMP_APP_PARAIDX_SRSTRUDIAL
#define AMP_APP_PARAIDX_SRSTRUDIAL (16)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSHDLIM
#define _define___AMP_APP_PARAIDX_SRSHDLIM
#define AMP_APP_PARAIDX_SRSHDLIM (17)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSTRUEQ
#define _define___AMP_APP_PARAIDX_SRSTRUEQ
#define AMP_APP_PARAIDX_SRSTRUEQ (18)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSGEQ
#define _define___AMP_APP_PARAIDX_SRSGEQ
#define AMP_APP_PARAIDX_SRSGEQ (19)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSTRUEQFLT
#define _define___AMP_APP_PARAIDX_SRSTRUEQFLT
#define AMP_APP_PARAIDX_SRSTRUEQFLT (20)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSHPFCFG
#define _define___AMP_APP_PARAIDX_SRSHPFCFG
#define AMP_APP_PARAIDX_SRSHPFCFG (21)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSHPFFLT
#define _define___AMP_APP_PARAIDX_SRSHPFFLT
#define AMP_APP_PARAIDX_SRSHPFFLT (22)
#endif

#ifndef _define___AMP_APP_PARAIDX_PORTSTATUS
#define _define___AMP_APP_PARAIDX_PORTSTATUS
#define AMP_APP_PARAIDX_PORTSTATUS (23)
#endif

#ifndef _define___AMP_APP_PARAIDX_PORTDATAFMT
#define _define___AMP_APP_PARAIDX_PORTDATAFMT
#define AMP_APP_PARAIDX_PORTDATAFMT (24)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRCCFG
#define _define___AMP_APP_PARAIDX_SRCCFG
#define AMP_APP_PARAIDX_SRCCFG (25)
#endif

#ifndef _define___AMP_APP_PARAIDX_SONICCFG
#define _define___AMP_APP_PARAIDX_SONICCFG
#define AMP_APP_PARAIDX_SONICCFG (26)
#endif

#ifndef _define___AMP_APP_PARAIDX_RESETPARA
#define _define___AMP_APP_PARAIDX_RESETPARA
#define AMP_APP_PARAIDX_RESETPARA (27)
#endif

#ifndef _define___AMP_APP_PARAIDX_MIXCFG
#define _define___AMP_APP_PARAIDX_MIXCFG
#define AMP_APP_PARAIDX_MIXCFG (28)
#endif

#ifndef _define___AMP_APP_PARAIDX_ASSOCIATE
#define _define___AMP_APP_PARAIDX_ASSOCIATE
#define AMP_APP_PARAIDX_ASSOCIATE (29)
#endif

#ifndef _define___AMP_APP_PARAIDX_BIQUADFILTER
#define _define___AMP_APP_PARAIDX_BIQUADFILTER
#define AMP_APP_PARAIDX_BIQUADFILTER (30)
#endif

#ifndef _define___AMP_APP_PARAIDX_BIQUADFILTERGAIN
#define _define___AMP_APP_PARAIDX_BIQUADFILTERGAIN
#define AMP_APP_PARAIDX_BIQUADFILTERGAIN (31)
#endif

#ifndef _define___AMP_APP_PARAIDX_BIQUADFILTERCOEF
#define _define___AMP_APP_PARAIDX_BIQUADFILTERCOEF
#define AMP_APP_PARAIDX_BIQUADFILTERCOEF (32)
#endif

#ifndef _define___AMP_APP_PARAIDX_NOISE_DETECT
#define _define___AMP_APP_PARAIDX_NOISE_DETECT
#define AMP_APP_PARAIDX_NOISE_DETECT (33)
#endif

#ifndef _define___AMP_APP_PARAIDX_MULTI2BT
#define _define___AMP_APP_PARAIDX_MULTI2BT
#define AMP_APP_PARAIDX_MULTI2BT (34)
#endif

#ifndef _define___AMP_APP_PARAIDX_DISASSOCIATE
#define _define___AMP_APP_PARAIDX_DISASSOCIATE
#define AMP_APP_PARAIDX_DISASSOCIATE (35)
#endif

#ifndef _define___AMP_APP_PARAIDX_VOLUME_MULTIMIXER
#define _define___AMP_APP_PARAIDX_VOLUME_MULTIMIXER
#define AMP_APP_PARAIDX_VOLUME_MULTIMIXER (36)
#endif

#ifndef _define___AMP_APP_PARAIDX_CGMSA
#define _define___AMP_APP_PARAIDX_CGMSA
#define AMP_APP_PARAIDX_CGMSA (37)
#endif

#ifndef _define___AMP_APP_PARAIDX_CHANMASK
#define _define___AMP_APP_PARAIDX_CHANMASK
#define AMP_APP_PARAIDX_CHANMASK (38)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRSENABLE
#define _define___AMP_APP_PARAIDX_SRSENABLE
#define AMP_APP_PARAIDX_SRSENABLE (39)
#endif

#ifndef _define___AMP_APP_PARAIDX_HEADROOM
#define _define___AMP_APP_PARAIDX_HEADROOM
#define AMP_APP_PARAIDX_HEADROOM (40)
#endif

#ifndef _define___AMP_APP_PARAIDX_SRCOUTSAMPLERATE
#define _define___AMP_APP_PARAIDX_SRCOUTSAMPLERATE
#define AMP_APP_PARAIDX_SRCOUTSAMPLERATE (41)
#endif

#ifndef _define___AMP_APP_PARAIDX_PATHMUTE
#define _define___AMP_APP_PARAIDX_PATHMUTE
#define AMP_APP_PARAIDX_PATHMUTE (42)
#endif

#ifndef _define___AMP_APP_PARAIDX_NEURALENABLE
#define _define___AMP_APP_PARAIDX_NEURALENABLE
#define AMP_APP_PARAIDX_NEURALENABLE (43)
#endif

#ifndef _define___AMP_APP_PARAIDX_NS227CFG
#define _define___AMP_APP_PARAIDX_NS227CFG
#define AMP_APP_PARAIDX_NS227CFG (44)
#endif

#ifndef _define___AMP_APP_PARAIDX_NS527CFG
#define _define___AMP_APP_PARAIDX_NS527CFG
#define AMP_APP_PARAIDX_NS527CFG (45)
#endif

#ifndef _define___AMP_APP_PARAIDX_AUDIOPATCHCFG
#define _define___AMP_APP_PARAIDX_AUDIOPATCHCFG
#define AMP_APP_PARAIDX_AUDIOPATCHCFG (46)
#endif

#ifndef _define___AMP_APP_PARAIDX_AUDIOPATCHSTATUS
#define _define___AMP_APP_PARAIDX_AUDIOPATCHSTATUS
#define AMP_APP_PARAIDX_AUDIOPATCHSTATUS (47)
#endif

#ifndef _define___AMP_APP_PARAIDX_HDMIMULTISTATUS
#define _define___AMP_APP_PARAIDX_HDMIMULTISTATUS
#define AMP_APP_PARAIDX_HDMIMULTISTATUS (48)
#endif

#ifndef _define___AMP_APP_PARAIDX_DEVICEATTACH
#define _define___AMP_APP_PARAIDX_DEVICEATTACH
#define AMP_APP_PARAIDX_DEVICEATTACH (49)
#endif

#ifndef _define___AMP_APP_PARAIDX_APP_CAP
#define _define___AMP_APP_PARAIDX_APP_CAP
#define AMP_APP_PARAIDX_APP_CAP (50)
#endif

#ifndef _define___AMP_APP_PARAIDX_MIX_PORTUSED
#define _define___AMP_APP_PARAIDX_MIX_PORTUSED
#define AMP_APP_PARAIDX_MIX_PORTUSED (51)
#endif

#ifndef _define___AMP_APP_PARAIDX_TRICKMODE
#define _define___AMP_APP_PARAIDX_TRICKMODE
#define AMP_APP_PARAIDX_TRICKMODE (52)
#endif

#ifndef _define___AMP_APP_PRARIDX_SINK_CAP
#define _define___AMP_APP_PRARIDX_SINK_CAP
#define AMP_APP_PRARIDX_SINK_CAP (53)
#endif

#ifndef _define___AMP_APP_PARAIDX_SURR_DEC
#define _define___AMP_APP_PARAIDX_SURR_DEC
#define AMP_APP_PARAIDX_SURR_DEC (54)
#endif

#ifndef _define___AMP_APP_PARAIDX_SURR_VIR
#define _define___AMP_APP_PARAIDX_SURR_VIR
#define AMP_APP_PARAIDX_SURR_VIR (55)
#endif

#ifndef _define___AMP_APP_PARAIDX_DAP_GEQ
#define _define___AMP_APP_PARAIDX_DAP_GEQ
#define AMP_APP_PARAIDX_DAP_GEQ (56)
#endif

#ifndef _define___AMP_APP_PARAIDX_GAIN
#define _define___AMP_APP_PARAIDX_GAIN
#define AMP_APP_PARAIDX_GAIN (57)
#endif

#ifndef _define___AMP_APP_PARAIDX_ATMOS_LOCK
#define _define___AMP_APP_PARAIDX_ATMOS_LOCK
#define AMP_APP_PARAIDX_ATMOS_LOCK (58)
#endif

#ifndef _define___AMP_APP_PARAIDX_DOWNMIX_OUT
#define _define___AMP_APP_PARAIDX_DOWNMIX_OUT
#define AMP_APP_PARAIDX_DOWNMIX_OUT (59)
#endif

#ifndef _define___AMP_APP_PARAIDX_AUDMODE
#define _define___AMP_APP_PARAIDX_AUDMODE
#define AMP_APP_PARAIDX_AUDMODE (60)
#endif

#ifndef _define___AMP_APP_PARAIDX_RL_TUNFILE
#define _define___AMP_APP_PARAIDX_RL_TUNFILE
#define AMP_APP_PARAIDX_RL_TUNFILE (61)
#endif

#ifndef _define___AMP_APP_PARAIDX_CONTINUES_AUDIO
#define _define___AMP_APP_PARAIDX_CONTINUES_AUDIO
#define AMP_APP_PARAIDX_CONTINUES_AUDIO (62)
#endif

#ifndef _define___AMP_APP_PARAIDX_DIALOG_ENH
#define _define___AMP_APP_PARAIDX_DIALOG_ENH
#define AMP_APP_PARAIDX_DIALOG_ENH (63)
#endif

#ifndef _define___AMP_APP_PARAIDX_AUDCODEC_TYPE
#define _define___AMP_APP_PARAIDX_AUDCODEC_TYPE
#define AMP_APP_PARAIDX_AUDCODEC_TYPE (64)
#endif

#ifndef _define___AMP_APP_PARAIDX_CHMOD_LOCKING_MODE
#define _define___AMP_APP_PARAIDX_CHMOD_LOCKING_MODE
#define AMP_APP_PARAIDX_CHMOD_LOCKING_MODE (65)
#endif

#ifndef _define___AMP_APP_PARAIDX_MEDIA_INT
#define _define___AMP_APP_PARAIDX_MEDIA_INT
#define AMP_APP_PARAIDX_MEDIA_INT (66)
#endif

#ifndef _define___AMP_APP_PARAIDX_VOLUME_LEVELER
#define _define___AMP_APP_PARAIDX_VOLUME_LEVELER
#define AMP_APP_PARAIDX_VOLUME_LEVELER (67)
#endif

#ifndef _define___AMP_APP_PARAIDX_ENFORCE_SINGLE_OA
#define _define___AMP_APP_PARAIDX_ENFORCE_SINGLE_OA
#define AMP_APP_PARAIDX_ENFORCE_SINGLE_OA (68)
#endif

#ifndef _define___AMP_APP_PARAIDX_SINKCAPS
#define _define___AMP_APP_PARAIDX_SINKCAPS
#define AMP_APP_PARAIDX_SINKCAPS (69)
#endif

#ifndef _define___AMP_APP_PARAIDX_MS12RST
#define _define___AMP_APP_PARAIDX_MS12RST
#define AMP_APP_PARAIDX_MS12RST (70)
#endif

#ifndef _typedef___AMP_APP_PARAIDX
#define _typedef___AMP_APP_PARAIDX
typedef CORBA_enum AMP_APP_PARAIDX;
#endif /* _typedef___AMP_APP_PARAIDX */

#ifndef _typedef___AMP_APP_PARAST
#define _typedef___AMP_APP_PARAST
typedef struct AMP_APP_PARAST {
  UINT8 _d;
  union {
    struct AMP_APP_PARAVOLUME stVolume;
    struct AMP_APP_PARABALANCE stBalance;
    struct AMP_APP_PARAGEQ stGEQ;
    struct AMP_APP_PARATREBLEBASS stTreble;
    struct AMP_APP_PARATREBLEBASS stBass;
    struct AMP_APP_PARAHPF stHpf;
    struct AMP_APP_PARADELAY stDelay;
    struct AMP_APP_PARASTEREO stStereo;
    struct AMP_APP_PARADUALMODE stDualMode;
    struct AMP_APP_PARALFEDMX stLfeDmx;
    struct AMP_APP_PARASPKMODE stSpkMode;
    struct AMP_APP_PARAMIXGAIN stMixGain;
    struct AMP_APP_PARASRSGEN stSrsGen;
    struct AMP_APP_PARASRSTRUVOL stSrsTruVol;
    struct AMP_APP_PARASRSTRUSUR stSrsTruSur;
    struct AMP_APP_PARASRSCS stSrsCs;
    struct AMP_APP_PARASRSTRUDIAL stSrsTruDial;
    struct AMP_APP_PARASRSHDLIM stSrsHdLim;
    struct AMP_APP_PARASRSTRUEQ stSrsTruEQ;
    struct AMP_APP_PARASRSGEQ stSrsGEQ;
    struct AMP_APP_PARASRSTRUEQFLT stSrsTruEQFlt;
    struct AMP_APP_PARASRSHPFFLT stSrsHPFFlt;
    struct AMP_APP_PARASRSHPFCONFIG stSrsHPFCfg;
    struct AMP_APP_PARAPORTSTATUS stPortStatus;
    struct AMP_APP_PARAPORTDATAFMT stPortDataFmt;
    struct AMP_APP_PARASRCCFG stSrcCfg;
    struct AMP_APP_PARASONICCFG stSonicCfg;
    struct AMP_APP_PARARESETPARA stResetPara;
    struct AMP_APP_PARAMIXCFG stMixCfg;
    struct AMP_APP_PARAASSOCIATE stAssoc;
    struct AMP_APP_PARABIQUAFILTER stBiquaFilter;
    struct AMP_APP_PARABIQUAFILTERGAIN stBiquaFilterGain;
    struct AMP_APP_PARABIQUAFILTERCOEF stBiquaFilterCoef;
    struct AMP_APP_PARANOISEDETECTCFG stNoiseDetectCfg;
    struct AMP_APP_PARAMULTI2BT stMulti2BT;
    struct AMP_APP_PARAVOLUME stMultiMixerVolume;
    struct AMP_APP_PARACGMSA stCGMSA;
    struct AMP_APP_PARACHANMASK stChanMask;
    struct AMP_APP_PARASRSENABLE stSRSEnable;
    struct AMP_APP_PARAHEADROOM stHeadRoom;
    struct AMP_APP_PARASRCOUTSAMPLERATE stOutSampleRate;
    struct AMP_APP_PARAASSOCIATE stDisAssoc;
    struct AMP_APP_PARAPATHMUTE stPathMute;
    struct AMP_APP_PARANEURALENABLE stNeuralEnable;
    struct AMP_APP_PARANS227CFG stNS227Cfg;
    struct AMP_APP_PARANS527CFG stNS527Cfg;
    struct AMP_APP_PARAAUDIOPATCHCFG stAudioPatchCfg;
    struct AMP_APP_PARAAUDIOPATCHSTATUS stAudioPatchStatus;
    struct AMP_APP_PARAHDMIMULTISTATUS stHDMIMultiStatus;
    struct AMP_APP_PARADEVICEATTACHCFG stDeviceAttachCfg;
    struct AMP_APP_PARAIDXAPPCAP stAppCap;
    struct AMP_APP_PARAMIXPORTUSED stMixPortUsed;
    struct AMP_APP_PARATRICMODECF stTricModeCfg;
    struct AMP_APP_PARASINKCAP stSinkCapInfo;
    struct AMP_APP_PARAMS12SURRDEC stMS12SurrDec;
    struct AMP_APP_PARAMS12SURRVIR stMS12SurrVir;
    struct AMP_APP_PARAMS12GEQ stMS12GEQ;
    struct AMP_APP_PARAMS12GAIN stMS12Gain;
    struct AMP_APP_PARAMS12ATMOSLOCK stMS12AtmosLock;
    struct AMP_APP_PARAAUDMODE stAudMode;
    struct AMP_APP_PARATUNFILE stTunFile;
    struct AMP_APP_PARAAUDCONTINUES stAudContinues;
    struct AMP_APP_PARAMS12DIALOGENH stMS12DialogEnh;
    struct AMP_APP_PARAMS12DOWNMIXOUT stMS12DownmixOut;
    struct AMP_APP_PARAAUDCODECTYPE stAudCodecType;
    struct AMP_APP_PARAMAUDCHMODLOCK stAudChmodLockMode;
    struct AMP_APP_PARAMS12MEDIAINT stMS12MediaInt;
    struct AMP_APP_PARAMS12VOLUMELEVELER stMS12VolumeLeveler;
    struct AMP_APP_PARAMS12ENFORCESINGLEOA stMS12EnforeSingleOA;
    struct AMP_APP_PARASINKPORTSCAP stSinkCap;
    struct AMP_APP_PARAMS12RST stMs12Rst;
  } _u;
} AMP_APP_PARAST;
#endif /* _typedef___AMP_APP_PARAST */

HRESULT AMP_APP_SetPortParameter(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_APP_PARAIDX eParamIdx, struct AMP_APP_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_APP_GetPortParameter(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_APP_PARAIDX eParamIdx, struct AMP_APP_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_APP_Open(AMP_APP _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_APP_Close(AMP_APP _obj, CORBA_Environment *_ev);

HRESULT AMP_APP_Destroy(AMP_APP _obj, CORBA_Environment *_ev);

HRESULT AMP_APP_SetState(AMP_APP _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_APP_GetState(AMP_APP _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_APP_QueryInfo(AMP_APP _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_APP_SetupPort(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_APP_DynamicSetupPort(AMP_APP _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_APP_DisconnectPorts(AMP_APP _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_APP_ClearPort(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_APP_QueryPort(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_APP_ClearPortBuf(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_APP_DebugDump(AMP_APP _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_APP_RegisterNotify(AMP_APP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_APP_UnregisterNotify(AMP_APP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_APP_RegisterNotifyDetail(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_APP_UnregisterNotifyDetail(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_APP_RegisterBD(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_APP_UnregisterBD(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_APP_PushBD(AMP_APP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_AOUT
#define _typedef___AMP_AOUT
typedef CORBA_Object AMP_AOUT;
#endif /* _typedef___AMP_AOUT */

#ifndef _define___AMP_AOUT_MODE_NORMAL
#define _define___AMP_AOUT_MODE_NORMAL
#define AMP_AOUT_MODE_NORMAL (0)
#endif

#ifndef _typedef___AMP_AOUT_SPECIALMODE
#define _typedef___AMP_AOUT_SPECIALMODE
typedef CORBA_enum AMP_AOUT_SPECIALMODE;
#endif /* _typedef___AMP_AOUT_SPECIALMODE */

#ifndef _typedef___AMP_AOUT_PARAIDXAOUTPTCAP
#define _typedef___AMP_AOUT_PARAIDXAOUTPTCAP
typedef struct AMP_AOUT_PARAIDXAOUTPTCAP {
  UINT32 uiPtCap[2];
} AMP_AOUT_PARAIDXAOUTPTCAP;
#endif /* _typedef___AMP_AOUT_PARAIDXAOUTPTCAP */

#ifndef _define___AMP_AOUT_PARAIDX_PT_CAP
#define _define___AMP_AOUT_PARAIDX_PT_CAP
#define AMP_AOUT_PARAIDX_PT_CAP (0)
#endif

#ifndef _typedef___AMP_AOUT_PARAIDX
#define _typedef___AMP_AOUT_PARAIDX
typedef CORBA_enum AMP_AOUT_PARAIDX;
#endif /* _typedef___AMP_AOUT_PARAIDX */

#ifndef _typedef___AMP_AOUT_PARAST
#define _typedef___AMP_AOUT_PARAST
typedef struct AMP_AOUT_PARAST {
  UINT8 _d;
  union {
    struct AMP_AOUT_PARAIDXAOUTPTCAP stAoutPtCap;
  } _u;
} AMP_AOUT_PARAST;
#endif /* _typedef___AMP_AOUT_PARAST */

HRESULT AMP_AOUT_Open(AMP_AOUT _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_AOUT_Close(AMP_AOUT _obj, CORBA_Environment *_ev);

HRESULT AMP_AOUT_Destroy(AMP_AOUT _obj, CORBA_Environment *_ev);

HRESULT AMP_AOUT_SetState(AMP_AOUT _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_AOUT_GetState(AMP_AOUT _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_AOUT_QueryInfo(AMP_AOUT _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_AOUT_SetupPort(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_AOUT_DynamicSetupPort(AMP_AOUT _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AOUT_DisconnectPorts(AMP_AOUT _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AOUT_ClearPort(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AOUT_QueryPort(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_AOUT_ClearPortBuf(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AOUT_DebugDump(AMP_AOUT _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_AOUT_RegisterNotify(AMP_AOUT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AOUT_UnregisterNotify(AMP_AOUT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AOUT_RegisterNotifyDetail(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AOUT_UnregisterNotifyDetail(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AOUT_RegisterBD(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AOUT_UnregisterBD(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AOUT_PushBD(AMP_AOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_AVIN
#define _typedef___AMP_AVIN
typedef CORBA_Object AMP_AVIN;
#endif /* _typedef___AMP_AVIN */

#ifndef _typedef___AMP_AVIN_PORT_ATTRIB
#define _typedef___AMP_AVIN_PORT_ATTRIB
typedef struct AMP_AVIN_PORT_ATTRIB {
  AMP_PORT_TYPE ePortType;
  UINT32 uiBufferSize;
} AMP_AVIN_PORT_ATTRIB;
#endif /* _typedef___AMP_AVIN_PORT_ATTRIB */

#ifndef _define___AMP_AVIN_SOURCE_TYPE_INVALID
#define _define___AMP_AVIN_SOURCE_TYPE_INVALID
#define AMP_AVIN_SOURCE_TYPE_INVALID (0)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_HDMI_01
#define _define___AMP_AVIN_SOURCE_TYPE_HDMI_01
#define AMP_AVIN_SOURCE_TYPE_HDMI_01 (1)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_HDMI_02
#define _define___AMP_AVIN_SOURCE_TYPE_HDMI_02
#define AMP_AVIN_SOURCE_TYPE_HDMI_02 (2)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_HDMI_03
#define _define___AMP_AVIN_SOURCE_TYPE_HDMI_03
#define AMP_AVIN_SOURCE_TYPE_HDMI_03 (3)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_HDMI_04
#define _define___AMP_AVIN_SOURCE_TYPE_HDMI_04
#define AMP_AVIN_SOURCE_TYPE_HDMI_04 (4)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_HDMI_05
#define _define___AMP_AVIN_SOURCE_TYPE_HDMI_05
#define AMP_AVIN_SOURCE_TYPE_HDMI_05 (5)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_CVBS_01
#define _define___AMP_AVIN_SOURCE_TYPE_CVBS_01
#define AMP_AVIN_SOURCE_TYPE_CVBS_01 (6)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_CVBS_02
#define _define___AMP_AVIN_SOURCE_TYPE_CVBS_02
#define AMP_AVIN_SOURCE_TYPE_CVBS_02 (7)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_VGA_01
#define _define___AMP_AVIN_SOURCE_TYPE_VGA_01
#define AMP_AVIN_SOURCE_TYPE_VGA_01 (8)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_VGA_02
#define _define___AMP_AVIN_SOURCE_TYPE_VGA_02
#define AMP_AVIN_SOURCE_TYPE_VGA_02 (9)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_COMPONENT_01
#define _define___AMP_AVIN_SOURCE_TYPE_COMPONENT_01
#define AMP_AVIN_SOURCE_TYPE_COMPONENT_01 (10)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_COMPONENT_02
#define _define___AMP_AVIN_SOURCE_TYPE_COMPONENT_02
#define AMP_AVIN_SOURCE_TYPE_COMPONENT_02 (11)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_SVIDEO_01
#define _define___AMP_AVIN_SOURCE_TYPE_SVIDEO_01
#define AMP_AVIN_SOURCE_TYPE_SVIDEO_01 (12)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_SVIDEO_02
#define _define___AMP_AVIN_SOURCE_TYPE_SVIDEO_02
#define AMP_AVIN_SOURCE_TYPE_SVIDEO_02 (13)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_SCART_01
#define _define___AMP_AVIN_SOURCE_TYPE_SCART_01
#define AMP_AVIN_SOURCE_TYPE_SCART_01 (14)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_SCART_02
#define _define___AMP_AVIN_SOURCE_TYPE_SCART_02
#define AMP_AVIN_SOURCE_TYPE_SCART_02 (15)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_ATV_01
#define _define___AMP_AVIN_SOURCE_TYPE_ATV_01
#define AMP_AVIN_SOURCE_TYPE_ATV_01 (16)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_ATV_02
#define _define___AMP_AVIN_SOURCE_TYPE_ATV_02
#define AMP_AVIN_SOURCE_TYPE_ATV_02 (17)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_AUD_ONLY
#define _define___AMP_AVIN_SOURCE_TYPE_AUD_ONLY
#define AMP_AVIN_SOURCE_TYPE_AUD_ONLY (18)
#endif

#ifndef _define___AMP_AVIN_SOURCE_TYPE_MAX_NUM
#define _define___AMP_AVIN_SOURCE_TYPE_MAX_NUM
#define AMP_AVIN_SOURCE_TYPE_MAX_NUM (19)
#endif

#ifndef _typedef___AMP_AVIN_SOURCE_TYPE
#define _typedef___AMP_AVIN_SOURCE_TYPE
typedef CORBA_enum AMP_AVIN_SOURCE_TYPE;
#endif /* _typedef___AMP_AVIN_SOURCE_TYPE */

#ifndef _define___AMP_AVIN_SOURCE_STATE_DISCONNECTED
#define _define___AMP_AVIN_SOURCE_STATE_DISCONNECTED
#define AMP_AVIN_SOURCE_STATE_DISCONNECTED (0)
#endif

#ifndef _define___AMP_AVIN_SOURCE_STATE_NO_SIGNAL
#define _define___AMP_AVIN_SOURCE_STATE_NO_SIGNAL
#define AMP_AVIN_SOURCE_STATE_NO_SIGNAL (1)
#endif

#ifndef _define___AMP_AVIN_SOURCE_STATE_NOT_SUPPORTED
#define _define___AMP_AVIN_SOURCE_STATE_NOT_SUPPORTED
#define AMP_AVIN_SOURCE_STATE_NOT_SUPPORTED (2)
#endif

#ifndef _define___AMP_AVIN_SOURCE_STATE_STABLE
#define _define___AMP_AVIN_SOURCE_STATE_STABLE
#define AMP_AVIN_SOURCE_STATE_STABLE (3)
#endif

#ifndef _typedef___AMP_AVIN_SOURCE_STATE
#define _typedef___AMP_AVIN_SOURCE_STATE
typedef CORBA_enum AMP_AVIN_SOURCE_STATE;
#endif /* _typedef___AMP_AVIN_SOURCE_STATE */

#ifndef _typedef___AMP_AVIN_RESOLUTION_FORMAT
#define _typedef___AMP_AVIN_RESOLUTION_FORMAT
typedef UINT32 AMP_AVIN_RESOLUTION_FORMAT;
#endif /* _typedef___AMP_AVIN_RESOLUTION_FORMAT */

#ifndef _define___AMP_AVIN_RESOLUTION_MODE_2D
#define _define___AMP_AVIN_RESOLUTION_MODE_2D
#define AMP_AVIN_RESOLUTION_MODE_2D (0)
#endif

#ifndef _define___AMP_AVIN_RESOLUTION_MODE_FRAME_PACKING
#define _define___AMP_AVIN_RESOLUTION_MODE_FRAME_PACKING
#define AMP_AVIN_RESOLUTION_MODE_FRAME_PACKING (1)
#endif

#ifndef _define___AMP_AVIN_RESOLUTION_MODE_SIDE_BY_SIDE
#define _define___AMP_AVIN_RESOLUTION_MODE_SIDE_BY_SIDE
#define AMP_AVIN_RESOLUTION_MODE_SIDE_BY_SIDE (2)
#endif

#ifndef _define___AMP_AVIN_RESOLUTION_MODE_TOP_AND_BOTTOM
#define _define___AMP_AVIN_RESOLUTION_MODE_TOP_AND_BOTTOM
#define AMP_AVIN_RESOLUTION_MODE_TOP_AND_BOTTOM (3)
#endif

#ifndef _define___AMP_AVIN_RESOLUTION_MODE_VIDEO_4K_2K
#define _define___AMP_AVIN_RESOLUTION_MODE_VIDEO_4K_2K
#define AMP_AVIN_RESOLUTION_MODE_VIDEO_4K_2K (4)
#endif

#ifndef _define___AMP_AVIN_RESOLUTION_MODE_SENSIO
#define _define___AMP_AVIN_RESOLUTION_MODE_SENSIO
#define AMP_AVIN_RESOLUTION_MODE_SENSIO (5)
#endif

#ifndef _typedef___AMP_AVIN_RESOLUTION_MODE
#define _typedef___AMP_AVIN_RESOLUTION_MODE
typedef CORBA_enum AMP_AVIN_RESOLUTION_MODE;
#endif /* _typedef___AMP_AVIN_RESOLUTION_MODE */

#ifndef _define___AMP_AVIN_COLOR_FORMAT_TYPE_RGB
#define _define___AMP_AVIN_COLOR_FORMAT_TYPE_RGB
#define AMP_AVIN_COLOR_FORMAT_TYPE_RGB (0)
#endif

#ifndef _define___AMP_AVIN_COLOR_FORMAT_TYPE_YUV444
#define _define___AMP_AVIN_COLOR_FORMAT_TYPE_YUV444
#define AMP_AVIN_COLOR_FORMAT_TYPE_YUV444 (1)
#endif

#ifndef _define___AMP_AVIN_COLOR_FORMAT_TYPE_YUV422
#define _define___AMP_AVIN_COLOR_FORMAT_TYPE_YUV422
#define AMP_AVIN_COLOR_FORMAT_TYPE_YUV422 (2)
#endif

#ifndef _define___AMP_AVIN_COLOR_FORMAT_TYPE_YUV420
#define _define___AMP_AVIN_COLOR_FORMAT_TYPE_YUV420
#define AMP_AVIN_COLOR_FORMAT_TYPE_YUV420 (3)
#endif

#ifndef _typedef___AMP_AVIN_COLOR_FORMAT_TYPE
#define _typedef___AMP_AVIN_COLOR_FORMAT_TYPE
typedef CORBA_enum AMP_AVIN_COLOR_FORMAT_TYPE;
#endif /* _typedef___AMP_AVIN_COLOR_FORMAT_TYPE */

#ifndef _define___AMP_AVIN_COLOR_PRI_RESERVED1
#define _define___AMP_AVIN_COLOR_PRI_RESERVED1
#define AMP_AVIN_COLOR_PRI_RESERVED1 (0)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_BT709
#define _define___AMP_AVIN_COLOR_PRI_BT709
#define AMP_AVIN_COLOR_PRI_BT709 (1)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_UNSPECIFIED
#define _define___AMP_AVIN_COLOR_PRI_UNSPECIFIED
#define AMP_AVIN_COLOR_PRI_UNSPECIFIED (2)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_RESERVED2
#define _define___AMP_AVIN_COLOR_PRI_RESERVED2
#define AMP_AVIN_COLOR_PRI_RESERVED2 (3)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_BT470M
#define _define___AMP_AVIN_COLOR_PRI_BT470M
#define AMP_AVIN_COLOR_PRI_BT470M (4)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_BT470BG
#define _define___AMP_AVIN_COLOR_PRI_BT470BG
#define AMP_AVIN_COLOR_PRI_BT470BG (5)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_SMPTE170M
#define _define___AMP_AVIN_COLOR_PRI_SMPTE170M
#define AMP_AVIN_COLOR_PRI_SMPTE170M (6)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_SMPTE470M
#define _define___AMP_AVIN_COLOR_PRI_SMPTE470M
#define AMP_AVIN_COLOR_PRI_SMPTE470M (7)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_FILM
#define _define___AMP_AVIN_COLOR_PRI_FILM
#define AMP_AVIN_COLOR_PRI_FILM (8)
#endif

#ifndef _define___AMP_AVIN_COLOR_PRI_BT2020
#define _define___AMP_AVIN_COLOR_PRI_BT2020
#define AMP_AVIN_COLOR_PRI_BT2020 (9)
#endif

#ifndef _typedef___AMP_AVIN_COLOR_PRI_TYPE
#define _typedef___AMP_AVIN_COLOR_PRI_TYPE
typedef CORBA_enum AMP_AVIN_COLOR_PRI_TYPE;
#endif /* _typedef___AMP_AVIN_COLOR_PRI_TYPE */

#ifndef _define___AMP_AVIN_COLOR_DEPTH_TYPE_8BIT
#define _define___AMP_AVIN_COLOR_DEPTH_TYPE_8BIT
#define AMP_AVIN_COLOR_DEPTH_TYPE_8BIT (0)
#endif

#ifndef _define___AMP_AVIN_COLOR_DEPTH_TYPE_10BIT
#define _define___AMP_AVIN_COLOR_DEPTH_TYPE_10BIT
#define AMP_AVIN_COLOR_DEPTH_TYPE_10BIT (1)
#endif

#ifndef _define___AMP_AVIN_COLOR_DEPTH_TYPE_12BIT
#define _define___AMP_AVIN_COLOR_DEPTH_TYPE_12BIT
#define AMP_AVIN_COLOR_DEPTH_TYPE_12BIT (2)
#endif

#ifndef _define___AMP_AVIN_COLOR_DEPTH_TYPE_16BIT
#define _define___AMP_AVIN_COLOR_DEPTH_TYPE_16BIT
#define AMP_AVIN_COLOR_DEPTH_TYPE_16BIT (3)
#endif

#ifndef _typedef___AMP_AVIN_COLOR_DEPTH_TYPE
#define _typedef___AMP_AVIN_COLOR_DEPTH_TYPE
typedef CORBA_enum AMP_AVIN_COLOR_DEPTH_TYPE;
#endif /* _typedef___AMP_AVIN_COLOR_DEPTH_TYPE */

#ifndef _define___AMP_AVIN_VID_ASPECT_RATIO_NONE
#define _define___AMP_AVIN_VID_ASPECT_RATIO_NONE
#define AMP_AVIN_VID_ASPECT_RATIO_NONE (0)
#endif

#ifndef _define___AMP_AVIN_VID_ASPECT_RATIO_4BY3
#define _define___AMP_AVIN_VID_ASPECT_RATIO_4BY3
#define AMP_AVIN_VID_ASPECT_RATIO_4BY3 (1)
#endif

#ifndef _define___AMP_AVIN_VID_ASPECT_RATIO_16BY9
#define _define___AMP_AVIN_VID_ASPECT_RATIO_16BY9
#define AMP_AVIN_VID_ASPECT_RATIO_16BY9 (2)
#endif

#ifndef _typedef___AMP_AVIN_VID_ASPECT_RATIO
#define _typedef___AMP_AVIN_VID_ASPECT_RATIO
typedef CORBA_enum AMP_AVIN_VID_ASPECT_RATIO;
#endif /* _typedef___AMP_AVIN_VID_ASPECT_RATIO */

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_NONE
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_NONE
#define AMP_AVIN_VID_ACTIVE_FORMAT_NONE (0)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_BOX16_BY_9_TOP
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_BOX16_BY_9_TOP
#define AMP_AVIN_VID_ACTIVE_FORMAT_BOX16_BY_9_TOP (1)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_BOX14_BY_9_TOP
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_BOX14_BY_9_TOP
#define AMP_AVIN_VID_ACTIVE_FORMAT_BOX14_BY_9_TOP (2)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_BOX16_BY_9_CENTER
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_BOX16_BY_9_CENTER
#define AMP_AVIN_VID_ACTIVE_FORMAT_BOX16_BY_9_CENTER (3)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_SAME_AS_PIC_AR
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_SAME_AS_PIC_AR
#define AMP_AVIN_VID_ACTIVE_FORMAT_SAME_AS_PIC_AR (4)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_4_BY_3_CENTER
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_4_BY_3_CENTER
#define AMP_AVIN_VID_ACTIVE_FORMAT_4_BY_3_CENTER (5)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_CENTER
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_CENTER
#define AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_CENTER (6)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_14_BY_9_CENTER
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_14_BY_9_CENTER
#define AMP_AVIN_VID_ACTIVE_FORMAT_14_BY_9_CENTER (7)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_4_BY_3_USE_14_BY_9
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_4_BY_3_USE_14_BY_9
#define AMP_AVIN_VID_ACTIVE_FORMAT_4_BY_3_USE_14_BY_9 (8)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_USE_14_BY_9
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_USE_14_BY_9
#define AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_USE_14_BY_9 (9)
#endif

#ifndef _define___AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_USE_4_BY_3
#define _define___AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_USE_4_BY_3
#define AMP_AVIN_VID_ACTIVE_FORMAT_16_BY_9_USE_4_BY_3 (10)
#endif

#ifndef _typedef___AMP_AVIN_VID_ACTIVE_FORMAT
#define _typedef___AMP_AVIN_VID_ACTIVE_FORMAT
typedef CORBA_enum AMP_AVIN_VID_ACTIVE_FORMAT;
#endif /* _typedef___AMP_AVIN_VID_ACTIVE_FORMAT */

#ifndef _define___AMP_AVIN_SCART_FB_RGB
#define _define___AMP_AVIN_SCART_FB_RGB
#define AMP_AVIN_SCART_FB_RGB (0)
#endif

#ifndef _define___AMP_AVIN_SCART_FB_CVBS
#define _define___AMP_AVIN_SCART_FB_CVBS
#define AMP_AVIN_SCART_FB_CVBS (1)
#endif

#ifndef _typedef___AMP_AVIN_SCART_FB_TYPE
#define _typedef___AMP_AVIN_SCART_FB_TYPE
typedef CORBA_enum AMP_AVIN_SCART_FB_TYPE;
#endif /* _typedef___AMP_AVIN_SCART_FB_TYPE */

#ifndef _define___AMP_AVIN_VIDEO_STD_eNTSC_GENERAL
#define _define___AMP_AVIN_VIDEO_STD_eNTSC_GENERAL
#define AMP_AVIN_VIDEO_STD_eNTSC_GENERAL (0)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_eNTSC443
#define _define___AMP_AVIN_VIDEO_STD_eNTSC443
#define AMP_AVIN_VIDEO_STD_eNTSC443 (1)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_ePAL_GENERAL
#define _define___AMP_AVIN_VIDEO_STD_ePAL_GENERAL
#define AMP_AVIN_VIDEO_STD_ePAL_GENERAL (2)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_ePALM
#define _define___AMP_AVIN_VIDEO_STD_ePALM
#define AMP_AVIN_VIDEO_STD_ePALM (3)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_ePALN
#define _define___AMP_AVIN_VIDEO_STD_ePALN
#define AMP_AVIN_VIDEO_STD_ePALN (4)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_eSECAM_GENERAL
#define _define___AMP_AVIN_VIDEO_STD_eSECAM_GENERAL
#define AMP_AVIN_VIDEO_STD_eSECAM_GENERAL (5)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_ePAL60
#define _define___AMP_AVIN_VIDEO_STD_ePAL60
#define AMP_AVIN_VIDEO_STD_ePAL60 (6)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_ePALNC
#define _define___AMP_AVIN_VIDEO_STD_ePALNC
#define AMP_AVIN_VIDEO_STD_ePALNC (7)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_eAUTO
#define _define___AMP_AVIN_VIDEO_STD_eAUTO
#define AMP_AVIN_VIDEO_STD_eAUTO (8)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_eUNKNOW
#define _define___AMP_AVIN_VIDEO_STD_eUNKNOW
#define AMP_AVIN_VIDEO_STD_eUNKNOW (9)
#endif

#ifndef _define___AMP_AVIN_VIDEO_STD_eMax
#define _define___AMP_AVIN_VIDEO_STD_eMax
#define AMP_AVIN_VIDEO_STD_eMax (10)
#endif

#ifndef _typedef___AMP_AVIN_VIDEO_STANDARD
#define _typedef___AMP_AVIN_VIDEO_STANDARD
typedef CORBA_enum AMP_AVIN_VIDEO_STANDARD;
#endif /* _typedef___AMP_AVIN_VIDEO_STANDARD */

#ifndef _define___AMP_AVIN_VIDEO_DO_STABLE_CHECK
#define _define___AMP_AVIN_VIDEO_DO_STABLE_CHECK
#define AMP_AVIN_VIDEO_DO_STABLE_CHECK (0)
#endif

#ifndef _define___AMP_AVIN_VIDEO_SKIP_STABLE_CHECK
#define _define___AMP_AVIN_VIDEO_SKIP_STABLE_CHECK
#define AMP_AVIN_VIDEO_SKIP_STABLE_CHECK (1)
#endif

#ifndef _define___AMP_AVIN_VIDEO_SKIP_INTERMEDIATE_CHECK
#define _define___AMP_AVIN_VIDEO_SKIP_INTERMEDIATE_CHECK
#define AMP_AVIN_VIDEO_SKIP_INTERMEDIATE_CHECK (2)
#endif

#ifndef _typedef___AMP_AVIN_VIDEO_STABLE_CHECK_TYPE
#define _typedef___AMP_AVIN_VIDEO_STABLE_CHECK_TYPE
typedef CORBA_enum AMP_AVIN_VIDEO_STABLE_CHECK_TYPE;
#endif /* _typedef___AMP_AVIN_VIDEO_STABLE_CHECK_TYPE */

#ifndef _define___AMP_AVIN_ATV_MODE_SCAN
#define _define___AMP_AVIN_ATV_MODE_SCAN
#define AMP_AVIN_ATV_MODE_SCAN (0)
#endif

#ifndef _define___AMP_AVIN_ATV_MODE_PLAY
#define _define___AMP_AVIN_ATV_MODE_PLAY
#define AMP_AVIN_ATV_MODE_PLAY (1)
#endif

#ifndef _typedef___AMP_AVIN_ATV_MODE
#define _typedef___AMP_AVIN_ATV_MODE
typedef CORBA_enum AMP_AVIN_ATV_MODE;
#endif /* _typedef___AMP_AVIN_ATV_MODE */

#ifndef _define___AMP_AVIN_CombFilter_e3DAdaptive
#define _define___AMP_AVIN_CombFilter_e3DAdaptive
#define AMP_AVIN_CombFilter_e3DAdaptive (0)
#endif

#ifndef _define___AMP_AVIN_CombFilter_eForce3D
#define _define___AMP_AVIN_CombFilter_eForce3D
#define AMP_AVIN_CombFilter_eForce3D (1)
#endif

#ifndef _define___AMP_AVIN_CombFilter_eForce2D
#define _define___AMP_AVIN_CombFilter_eForce2D
#define AMP_AVIN_CombFilter_eForce2D (2)
#endif

#ifndef _define___AMP_AVIN_CombFilter_eMax
#define _define___AMP_AVIN_CombFilter_eMax
#define AMP_AVIN_CombFilter_eMax (3)
#endif

#ifndef _typedef___AMP_AVIN_COMBFILTER_MODE
#define _typedef___AMP_AVIN_COMBFILTER_MODE
typedef CORBA_enum AMP_AVIN_COMBFILTER_MODE;
#endif /* _typedef___AMP_AVIN_COMBFILTER_MODE */

#ifndef _define___AMP_AVIN_COMBFILTER_BANDPASS
#define _define___AMP_AVIN_COMBFILTER_BANDPASS
#define AMP_AVIN_COMBFILTER_BANDPASS (0)
#endif

#ifndef _define___AMP_AVIN_COMBFILTER_NORCH
#define _define___AMP_AVIN_COMBFILTER_NORCH
#define AMP_AVIN_COMBFILTER_NORCH (1)
#endif

#ifndef _typedef___AMP_AVIN_COMBFILTER_TYPE
#define _typedef___AMP_AVIN_COMBFILTER_TYPE
typedef CORBA_enum AMP_AVIN_COMBFILTER_TYPE;
#endif /* _typedef___AMP_AVIN_COMBFILTER_TYPE */

#ifndef _define___AMP_AVIN_REG_8BIT
#define _define___AMP_AVIN_REG_8BIT
#define AMP_AVIN_REG_8BIT (0)
#endif

#ifndef _define___AMP_AVIN_REG_16BIT
#define _define___AMP_AVIN_REG_16BIT
#define AMP_AVIN_REG_16BIT (1)
#endif

#ifndef _define___AMP_AVIN_REG_32BIT
#define _define___AMP_AVIN_REG_32BIT
#define AMP_AVIN_REG_32BIT (2)
#endif

#ifndef _typedef___AMP_AVIN_REGISTER_DEPTH
#define _typedef___AMP_AVIN_REGISTER_DEPTH
typedef CORBA_enum AMP_AVIN_REGISTER_DEPTH;
#endif /* _typedef___AMP_AVIN_REGISTER_DEPTH */

#ifndef _typedef___AMP_AVIN_SIF_STANDARD
#define _typedef___AMP_AVIN_SIF_STANDARD
typedef UINT32 AMP_AVIN_SIF_STANDARD;
#endif /* _typedef___AMP_AVIN_SIF_STANDARD */

#ifndef _define___AMP_AVIN_SIF_STD_UNKNOWN
#define _define___AMP_AVIN_SIF_STD_UNKNOWN
#define AMP_AVIN_SIF_STD_UNKNOWN (0)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_MONO45
#define _define___AMP_AVIN_SIF_STD_MONO45
#define AMP_AVIN_SIF_STD_MONO45 (1)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_MONO55
#define _define___AMP_AVIN_SIF_STD_MONO55
#define AMP_AVIN_SIF_STD_MONO55 (2)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_MONO60
#define _define___AMP_AVIN_SIF_STD_MONO60
#define AMP_AVIN_SIF_STD_MONO60 (4)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_MONO65
#define _define___AMP_AVIN_SIF_STD_MONO65
#define AMP_AVIN_SIF_STD_MONO65 (8)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_MONO65_AML
#define _define___AMP_AVIN_SIF_STD_MONO65_AML
#define AMP_AVIN_SIF_STD_MONO65_AML (16)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_AUTO
#define _define___AMP_AVIN_SIF_STD_AUTO
#define AMP_AVIN_SIF_STD_AUTO (32)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_A2DK
#define _define___AMP_AVIN_SIF_STD_A2DK
#define AMP_AVIN_SIF_STD_A2DK (64)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_BTSC
#define _define___AMP_AVIN_SIF_STD_BTSC
#define AMP_AVIN_SIF_STD_BTSC (128)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_BTSCSAP
#define _define___AMP_AVIN_SIF_STD_BTSCSAP
#define AMP_AVIN_SIF_STD_BTSCSAP (256)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_KOREA
#define _define___AMP_AVIN_SIF_STD_KOREA
#define AMP_AVIN_SIF_STD_KOREA (512)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_BGNICAM
#define _define___AMP_AVIN_SIF_STD_BGNICAM
#define AMP_AVIN_SIF_STD_BGNICAM (1024)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_FMMONO_NICAM_CHINA
#define _define___AMP_AVIN_SIF_STD_FMMONO_NICAM_CHINA
#define AMP_AVIN_SIF_STD_FMMONO_NICAM_CHINA (2048)
#endif

#ifndef _define___AMP_AVIN_SIF_STD_PALI
#define _define___AMP_AVIN_SIF_STD_PALI
#define AMP_AVIN_SIF_STD_PALI (4096)
#endif

#ifndef _typedef___AMP_AVIN_AUD_SIF_MODE
#define _typedef___AMP_AVIN_AUD_SIF_MODE
typedef UINT32 AMP_AVIN_AUD_SIF_MODE;
#endif /* _typedef___AMP_AVIN_AUD_SIF_MODE */

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_UNKNOWN
#define _define___AMP_AVIN_AUD_SIF_MODE_UNKNOWN
#define AMP_AVIN_AUD_SIF_MODE_UNKNOWN (0)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_FM_MONO
#define _define___AMP_AVIN_AUD_SIF_MODE_FM_MONO
#define AMP_AVIN_AUD_SIF_MODE_FM_MONO (1)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_AM_MONO
#define _define___AMP_AVIN_AUD_SIF_MODE_AM_MONO
#define AMP_AVIN_AUD_SIF_MODE_AM_MONO (2)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_MONO
#define _define___AMP_AVIN_AUD_SIF_MODE_MONO
#define AMP_AVIN_AUD_SIF_MODE_MONO (4)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_STEREO
#define _define___AMP_AVIN_AUD_SIF_MODE_STEREO
#define AMP_AVIN_AUD_SIF_MODE_STEREO (8)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_SAP
#define _define___AMP_AVIN_AUD_SIF_MODE_SAP
#define AMP_AVIN_AUD_SIF_MODE_SAP (16)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_SAP_MONO
#define _define___AMP_AVIN_AUD_SIF_MODE_SAP_MONO
#define AMP_AVIN_AUD_SIF_MODE_SAP_MONO (32)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_MONO
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_MONO
#define AMP_AVIN_AUD_SIF_MODE_NICAM_MONO (256)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_STEREO
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_STEREO
#define AMP_AVIN_AUD_SIF_MODE_NICAM_STEREO (512)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1
#define AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1 (1024)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO2
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO2
#define AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO2 (2048)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1_MONO2
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1_MONO2
#define AMP_AVIN_AUD_SIF_MODE_NICAM_DUAL_MONO1_MONO2 (4096)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DATA704
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_DATA704
#define AMP_AVIN_AUD_SIF_MODE_NICAM_DATA704 (8192)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_NICAM_MONO_DATA352
#define _define___AMP_AVIN_AUD_SIF_MODE_NICAM_MONO_DATA352
#define AMP_AVIN_AUD_SIF_MODE_NICAM_MONO_DATA352 (16384)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_A2_MONO
#define _define___AMP_AVIN_AUD_SIF_MODE_A2_MONO
#define AMP_AVIN_AUD_SIF_MODE_A2_MONO (131072)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_A2_STEREO
#define _define___AMP_AVIN_AUD_SIF_MODE_A2_STEREO
#define AMP_AVIN_AUD_SIF_MODE_A2_STEREO (262144)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO1
#define _define___AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO1
#define AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO1 (524288)
#endif

#ifndef _define___AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2
#define _define___AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2
#define AMP_AVIN_AUD_SIF_MODE_A2_DUAL_MONO2 (1048576)
#endif

#ifndef _define___AMP_AVIN_SCREEN_MODE_BLUE
#define _define___AMP_AVIN_SCREEN_MODE_BLUE
#define AMP_AVIN_SCREEN_MODE_BLUE (0)
#endif

#ifndef _define___AMP_AVIN_SCREEN_MODE_GREEN
#define _define___AMP_AVIN_SCREEN_MODE_GREEN
#define AMP_AVIN_SCREEN_MODE_GREEN (1)
#endif

#ifndef _define___AMP_AVIN_SCREEN_MODE_RED
#define _define___AMP_AVIN_SCREEN_MODE_RED
#define AMP_AVIN_SCREEN_MODE_RED (2)
#endif

#ifndef _define___AMP_AVIN_SCREEN_MODE_BLACK
#define _define___AMP_AVIN_SCREEN_MODE_BLACK
#define AMP_AVIN_SCREEN_MODE_BLACK (3)
#endif

#ifndef _define___AMP_AVIN_SCREEN_MODE_AUTO
#define _define___AMP_AVIN_SCREEN_MODE_AUTO
#define AMP_AVIN_SCREEN_MODE_AUTO (4)
#endif

#ifndef _typedef___AMP_AVIN_SCREEN_MODE
#define _typedef___AMP_AVIN_SCREEN_MODE
typedef CORBA_enum AMP_AVIN_SCREEN_MODE;
#endif /* _typedef___AMP_AVIN_SCREEN_MODE */

#ifndef _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_SIZE
#define _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_SIZE
#define AMP_AVIN_VGA_AUTO_ADJUSTMENT_SIZE (0)
#endif

#ifndef _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_POSITION
#define _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_POSITION
#define AMP_AVIN_VGA_AUTO_ADJUSTMENT_POSITION (1)
#endif

#ifndef _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_PHASE
#define _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_PHASE
#define AMP_AVIN_VGA_AUTO_ADJUSTMENT_PHASE (2)
#endif

#ifndef _typedef___AMP_AVIN_VGA_AUTO_ADJUSTMENT_SELECT
#define _typedef___AMP_AVIN_VGA_AUTO_ADJUSTMENT_SELECT
typedef CORBA_enum AMP_AVIN_VGA_AUTO_ADJUSTMENT_SELECT;
#endif /* _typedef___AMP_AVIN_VGA_AUTO_ADJUSTMENT_SELECT */

#ifndef _define___AMP_AVIN_VBI_NONE
#define _define___AMP_AVIN_VBI_NONE
#define AMP_AVIN_VBI_NONE (0)
#endif

#ifndef _define___AMP_AVIN_VBI_NTSC_CC
#define _define___AMP_AVIN_VBI_NTSC_CC
#define AMP_AVIN_VBI_NTSC_CC (1)
#endif

#ifndef _define___AMP_AVIN_VBI_TELETEXT
#define _define___AMP_AVIN_VBI_TELETEXT
#define AMP_AVIN_VBI_TELETEXT (2)
#endif

#ifndef _define___AMP_AVIN_VBI_WSS
#define _define___AMP_AVIN_VBI_WSS
#define AMP_AVIN_VBI_WSS (3)
#endif

#ifndef _define___AMP_AVIN_VBI_PAL_CC
#define _define___AMP_AVIN_VBI_PAL_CC
#define AMP_AVIN_VBI_PAL_CC (4)
#endif

#ifndef _define___AMP_AVIN_VBI_CGMS
#define _define___AMP_AVIN_VBI_CGMS
#define AMP_AVIN_VBI_CGMS (5)
#endif

#ifndef _define___AMP_AVIN_VBI_DATA_MAX
#define _define___AMP_AVIN_VBI_DATA_MAX
#define AMP_AVIN_VBI_DATA_MAX (6)
#endif

#ifndef _typedef___AMP_AVIN_VBI_DATA_TYPE
#define _typedef___AMP_AVIN_VBI_DATA_TYPE
typedef CORBA_enum AMP_AVIN_VBI_DATA_TYPE;
#endif /* _typedef___AMP_AVIN_VBI_DATA_TYPE */

#ifndef _define___AMP_AVIN_SIGNAL_POLARITY_NEGATIVE
#define _define___AMP_AVIN_SIGNAL_POLARITY_NEGATIVE
#define AMP_AVIN_SIGNAL_POLARITY_NEGATIVE (0)
#endif

#ifndef _define___AMP_AVIN_SIGNAL_POLARITY_POSITIVE
#define _define___AMP_AVIN_SIGNAL_POLARITY_POSITIVE
#define AMP_AVIN_SIGNAL_POLARITY_POSITIVE (1)
#endif

#ifndef _typedef___AMP_AVIN_SIGNAL_POLARITY
#define _typedef___AMP_AVIN_SIGNAL_POLARITY
typedef CORBA_enum AMP_AVIN_SIGNAL_POLARITY;
#endif /* _typedef___AMP_AVIN_SIGNAL_POLARITY */

#ifndef _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_COMPLETED
#define _define___AMP_AVIN_VGA_AUTO_ADJUSTMENT_COMPLETED
#define AMP_AVIN_VGA_AUTO_ADJUSTMENT_COMPLETED (0)
#endif

#ifndef _typedef___AMP_AVIN_COMPLETED_JOB_TYPE
#define _typedef___AMP_AVIN_COMPLETED_JOB_TYPE
typedef CORBA_enum AMP_AVIN_COMPLETED_JOB_TYPE;
#endif /* _typedef___AMP_AVIN_COMPLETED_JOB_TYPE */

#ifndef _typedef___AMP_AVIN_DEBUG_DUMP_DATA_TYPE
#define _typedef___AMP_AVIN_DEBUG_DUMP_DATA_TYPE
typedef UINT32 AMP_AVIN_DEBUG_DUMP_DATA_TYPE;
#endif /* _typedef___AMP_AVIN_DEBUG_DUMP_DATA_TYPE */

#ifndef _define___AMP_AVIN_DEBUG_DUMP_DATA_AUDIO
#define _define___AMP_AVIN_DEBUG_DUMP_DATA_AUDIO
#define AMP_AVIN_DEBUG_DUMP_DATA_AUDIO (2)
#endif

#ifndef _define___AMP_AVIN_DEBUG_DUMP_DATA_VIDEO
#define _define___AMP_AVIN_DEBUG_DUMP_DATA_VIDEO
#define AMP_AVIN_DEBUG_DUMP_DATA_VIDEO (4)
#endif

#ifndef _define___AMP_AVIN_DEBUG_DUMP_DATA_VBI
#define _define___AMP_AVIN_DEBUG_DUMP_DATA_VBI
#define AMP_AVIN_DEBUG_DUMP_DATA_VBI (8)
#endif

#ifndef _typedef___AMP_AVIN_SOURCE_LIST
#define _typedef___AMP_AVIN_SOURCE_LIST
typedef struct AMP_AVIN_SOURCE_LIST {
  AMP_AVIN_SOURCE_TYPE source[19];
  UINT32 size;
} AMP_AVIN_SOURCE_LIST;
#endif /* _typedef___AMP_AVIN_SOURCE_LIST */

#ifndef _typedef___AMP_AVIN_VCR_PROPERTY
#define _typedef___AMP_AVIN_VCR_PROPERTY
typedef struct AMP_AVIN_VCR_PROPERTY {
  BOOL bDetected;
  BOOL bTrickMode;
} AMP_AVIN_VCR_PROPERTY;
#endif /* _typedef___AMP_AVIN_VCR_PROPERTY */

#ifndef _typedef___AMP_AVIN_HDMI_PKT_HDR_INFOFRM
#define _typedef___AMP_AVIN_HDMI_PKT_HDR_INFOFRM
typedef struct AMP_AVIN_HDMI_PKT_HDR_INFOFRM {
  UINT8 uiEOTF;
  UINT8 uiStatic_metadata_Des_ID;
  UINT16 uidisplay_primaries_x[3];
  UINT16 uidisplay_primaries_y[3];
  UINT16 uiwhite_point_x;
  UINT16 uiwhite_point_y;
  UINT16 uimax_display_mastering_lumi;
  UINT16 uimin_display_mastering_lumi;
  UINT16 uiMaximum_Content_Light_Lvl;
  UINT16 uiMaximum_Frame_Average_Light_Lvl;
} AMP_AVIN_HDMI_PKT_HDR_INFOFRM;
#endif /* _typedef___AMP_AVIN_HDMI_PKT_HDR_INFOFRM */

#ifndef _typedef___AMP_AVIN_HDMI_DV_INFO
#define _typedef___AMP_AVIN_HDMI_DV_INFO
typedef struct AMP_AVIN_HDMI_DV_INFO {
  AMP_AVIN_COLOR_FORMAT_TYPE colorFormat;
  UINT8 uiDvEnable;
} AMP_AVIN_HDMI_DV_INFO;
#endif /* _typedef___AMP_AVIN_HDMI_DV_INFO */

#ifndef _typedef___AMP_AVIN_INPUT_VIDEO_PARAMS
#define _typedef___AMP_AVIN_INPUT_VIDEO_PARAMS
typedef struct AMP_AVIN_INPUT_VIDEO_PARAMS {
  AMP_AVIN_COLOR_FORMAT_TYPE colorFormat;
  AMP_AVIN_COLOR_PRI_TYPE colorPrimary;
  AMP_AVIN_COLOR_DEPTH_TYPE colorDepth;
  AMP_AVIN_RESOLUTION_FORMAT resFormat;
  AMP_AVIN_RESOLUTION_MODE resMode;
  AMP_AVIN_VIDEO_STANDARD videoStandard;
  UINT16 videoWidth;
  UINT16 videoHeight;
  BOOL videoProgressive;
  CORBA_float videoRefrate;
  BOOL videoFullrange;
  UINT16 videoParaHTotal;
  UINT16 videoParaVTotal;
  UINT16 videoParaHFrontPorch;
  UINT16 videoParaVFrontPorch;
  UINT16 videoParaHSyncWidth;
  UINT16 videoParaVSyncWidth;
  AMP_AVIN_SIGNAL_POLARITY videoParaHpol;
  AMP_AVIN_SIGNAL_POLARITY videoParaVpol;
  BOOL isHdmiMode;
  AMP_AVIN_VID_ASPECT_RATIO aspRatio;
  AMP_AVIN_VID_ACTIVE_FORMAT vidActFmt;
  AMP_AVIN_SCART_FB_TYPE scartFB;
  struct AMP_AVIN_VCR_PROPERTY vcrProp;
  UINT8 hdmiVSIF[32];
  AMP_AVIN_COLOR_FORMAT_TYPE outputColorFormat;
  AMP_AVIN_COLOR_DEPTH_TYPE outputColorDepth;
  UINT8 hdmiAVIF[16];
  UINT8 hdmiAudioIF[8];
  UINT8 hdmiChStatus[6];
  UINT8 hdmiSPDIF[28];
  struct AMP_AVIN_HDMI_PKT_HDR_INFOFRM hdmiHDRIF;
} AMP_AVIN_INPUT_VIDEO_PARAMS;
#endif /* _typedef___AMP_AVIN_INPUT_VIDEO_PARAMS */

#ifndef _typedef___AMP_AVIN_INPUT_AUDIO_PARAMS
#define _typedef___AMP_AVIN_INPUT_AUDIO_PARAMS
typedef struct AMP_AVIN_INPUT_AUDIO_PARAMS {
  AMP_ADEC_FORMAT audFormat;
  UINT32 bitDepth;
  UINT32 sampleRate;
  UINT32 channels;
  BOOL isHBR;
} AMP_AVIN_INPUT_AUDIO_PARAMS;
#endif /* _typedef___AMP_AVIN_INPUT_AUDIO_PARAMS */

#ifndef _typedef___AMP_AVIN_INPUT_PARAMS
#define _typedef___AMP_AVIN_INPUT_PARAMS
typedef struct AMP_AVIN_INPUT_PARAMS {
  AMP_AVIN_SOURCE_STATE state;
  struct AMP_AVIN_INPUT_VIDEO_PARAMS videoParams;
  struct AMP_AVIN_INPUT_AUDIO_PARAMS audioParams;
} AMP_AVIN_INPUT_PARAMS;
#endif /* _typedef___AMP_AVIN_INPUT_PARAMS */

#ifndef _typedef___AMP_AVIN_HDMI_PKT_GMD
#define _typedef___AMP_AVIN_HDMI_PKT_GMD
typedef struct AMP_AVIN_HDMI_PKT_GMD {
  UINT8 nextField;
  UINT8 noCurGBD;
  INT32 gdbProfile;
  INT32 seqInfo;
  UINT8 affectedGamutSeqNum;
  UINT8 curGamutSeqNum;
  UINT8 gbdData[28];
} AMP_AVIN_HDMI_PKT_GMD;
#endif /* _typedef___AMP_AVIN_HDMI_PKT_GMD */

#ifndef _typedef___AMP_AVIN_EDID_INFO
#define _typedef___AMP_AVIN_EDID_INFO
typedef struct AMP_AVIN_EDID_INFO {
  AMP_AVIN_SOURCE_TYPE source;
  UINT8 data[256];
  UINT32 size;
} AMP_AVIN_EDID_INFO;
#endif /* _typedef___AMP_AVIN_EDID_INFO */

#ifndef _typedef___AMP_AVIN_VIDEO_PARAMS_CHANGED_INFO
#define _typedef___AMP_AVIN_VIDEO_PARAMS_CHANGED_INFO
typedef struct AMP_AVIN_INPUT_VIDEO_PARAMS AMP_AVIN_VIDEO_PARAMS_CHANGED_INFO;
#endif /* _typedef___AMP_AVIN_VIDEO_PARAMS_CHANGED_INFO */

#ifndef _typedef___AMP_AVIN_AUDIO_PARAMS_CHANGED_INFO
#define _typedef___AMP_AVIN_AUDIO_PARAMS_CHANGED_INFO
typedef struct AMP_AVIN_INPUT_AUDIO_PARAMS AMP_AVIN_AUDIO_PARAMS_CHANGED_INFO;
#endif /* _typedef___AMP_AVIN_AUDIO_PARAMS_CHANGED_INFO */

#ifndef _typedef___AMP_AVIN_CONNECT_STATUS_CHANGED_INFO
#define _typedef___AMP_AVIN_CONNECT_STATUS_CHANGED_INFO
typedef struct AMP_AVIN_CONNECT_STATUS_CHANGED_INFO {
  AMP_AVIN_SOURCE_TYPE source;
  BOOL srcConnected;
} AMP_AVIN_CONNECT_STATUS_CHANGED_INFO;
#endif /* _typedef___AMP_AVIN_CONNECT_STATUS_CHANGED_INFO */

#ifndef _typedef___AMP_AVIN_LOCK_STATUS_CHANGED_INFO
#define _typedef___AMP_AVIN_LOCK_STATUS_CHANGED_INFO
typedef struct AMP_AVIN_LOCK_STATUS_CHANGED_INFO {
  AMP_AVIN_SOURCE_STATE status;
} AMP_AVIN_LOCK_STATUS_CHANGED_INFO;
#endif /* _typedef___AMP_AVIN_LOCK_STATUS_CHANGED_INFO */

#ifndef _typedef___AMP_AVIN_AVMUTE_CHANGED_INFO
#define _typedef___AMP_AVIN_AVMUTE_CHANGED_INFO
typedef struct AMP_AVIN_AVMUTE_CHANGED_INFO {
  BOOL avMute;
} AMP_AVIN_AVMUTE_CHANGED_INFO;
#endif /* _typedef___AMP_AVIN_AVMUTE_CHANGED_INFO */

#ifndef _typedef___AMP_AVIN_INPUT_SOURCES
#define _typedef___AMP_AVIN_INPUT_SOURCES
typedef struct AMP_AVIN_INPUT_SOURCES {
  BOOL bAllSources[19];
} AMP_AVIN_INPUT_SOURCES;
#endif /* _typedef___AMP_AVIN_INPUT_SOURCES */

#ifndef _typedef___AMP_AVIN_SIF_STANDARD_EXT
#define _typedef___AMP_AVIN_SIF_STANDARD_EXT
typedef struct AMP_AVIN_SIF_STANDARD_EXT {
  AMP_AVIN_SIF_STANDARD standard;
  INT32 offset;
} AMP_AVIN_SIF_STANDARD_EXT;
#endif /* _typedef___AMP_AVIN_SIF_STANDARD_EXT */

#ifndef _typedef___AMP_AVIN_SIF_STATUS
#define _typedef___AMP_AVIN_SIF_STATUS
typedef struct AMP_AVIN_SIF_STATUS {
  AMP_AVIN_SIF_STANDARD standard;
  AMP_AVIN_AUD_SIF_MODE mode;
  UINT32 uiSupportedModes;
} AMP_AVIN_SIF_STATUS;
#endif /* _typedef___AMP_AVIN_SIF_STATUS */

#ifndef _typedef___AMP_AVIN_ADC_CALIB_DATA
#define _typedef___AMP_AVIN_ADC_CALIB_DATA
typedef struct AMP_AVIN_ADC_CALIB_DATA {
  CORBA_float gain[3];
  CORBA_float offset[3];
} AMP_AVIN_ADC_CALIB_DATA;
#endif /* _typedef___AMP_AVIN_ADC_CALIB_DATA */

#ifndef _typedef___AMP_AVIN_ADC_CALIB_DATA_EXT
#define _typedef___AMP_AVIN_ADC_CALIB_DATA_EXT
typedef struct AMP_AVIN_ADC_CALIB_DATA_EXT {
  UINT8 expected[3];
  UINT8 actual[3];
} AMP_AVIN_ADC_CALIB_DATA_EXT;
#endif /* _typedef___AMP_AVIN_ADC_CALIB_DATA_EXT */

#ifndef _typedef___AMP_AVIN_BLUESCREEN_SETTING
#define _typedef___AMP_AVIN_BLUESCREEN_SETTING
typedef struct AMP_AVIN_BLUESCREEN_SETTING {
  BOOL enable;
  UINT32 ulColor;
} AMP_AVIN_BLUESCREEN_SETTING;
#endif /* _typedef___AMP_AVIN_BLUESCREEN_SETTING */

#ifndef _typedef___AMP_AVIN_VGA_VIDEO_PROP
#define _typedef___AMP_AVIN_VGA_VIDEO_PROP
typedef struct AMP_AVIN_VGA_VIDEO_PROP {
  INT16 xshift;
  INT16 yshift;
  INT16 ulPhase;
} AMP_AVIN_VGA_VIDEO_PROP;
#endif /* _typedef___AMP_AVIN_VGA_VIDEO_PROP */

#ifndef _typedef___AMP_AVIN_HDMI_ARC_SETTING
#define _typedef___AMP_AVIN_HDMI_ARC_SETTING
typedef struct AMP_AVIN_HDMI_ARC_SETTING {
  BOOL bArc_ON;
  UINT32 hdmi_port;
} AMP_AVIN_HDMI_ARC_SETTING;
#endif /* _typedef___AMP_AVIN_HDMI_ARC_SETTING */

#ifndef _typedef___AMP_AVIN_INPUT_MUX
#define _typedef___AMP_AVIN_INPUT_MUX
typedef struct AMP_AVIN_INPUT_MUX {
  AMP_AVIN_SOURCE_TYPE source;
  UINT8 videoInputName[50];
  UINT8 audioInputName[50];
} AMP_AVIN_INPUT_MUX;
#endif /* _typedef___AMP_AVIN_INPUT_MUX */

#ifndef _typedef___AMP_AVIN_PLATFORM_CONFIG
#define _typedef___AMP_AVIN_PLATFORM_CONFIG
typedef struct AMP_AVIN_PLATFORM_CONFIG {
  UINT8 chipName[50];
  UINT8 chip_name[50];
  struct AMP_AVIN_INPUT_MUX inputMux[19];
  UINT8 audioOutputName[50];
  UINT32 uChipResetPinAddress;
  UINT32 uCvbsDetectPinAddress;
  UINT32 uComponentDetectPinAddress;
  UINT32 uScartFBDetectPinAddress;
  UINT32 uHdmirx1HPDAddress;
  UINT32 uHdmirx2HPDAddress;
  UINT32 uHdmirx3HPDAddress;
  UINT32 uHdmirx4HPDAddress;
  BOOL bEnableInternalEdid;
  UINT8 internalEdid[256];
  BOOL bEnableComponentVBI;
  BOOL bEnableCvbsVBI;
  BOOL bEnable50HzTiming;
  BOOL bEnable3DCombFilter;
  BOOL bEnableHdcpRepeaterMode;
  AMP_AVIN_VIDEO_STANDARD defaultCvbsStandard;
} AMP_AVIN_PLATFORM_CONFIG;
#endif /* _typedef___AMP_AVIN_PLATFORM_CONFIG */

#ifndef _typedef___AMP_AVIN_REGISTER
#define _typedef___AMP_AVIN_REGISTER
typedef struct AMP_AVIN_REGISTER {
  UINT32 addr;
  UINT32 value;
  AMP_AVIN_REGISTER_DEPTH depth;
} AMP_AVIN_REGISTER;
#endif /* _typedef___AMP_AVIN_REGISTER */

#ifndef _typedef___AMP_AVIN_HDCP_KEYS
#define _typedef___AMP_AVIN_HDCP_KEYS
typedef struct AMP_AVIN_HDCP_KEYS {
  AMP_AVIN_SOURCE_TYPE source;
  UINT8 data[255];
  UINT8 length;
} AMP_AVIN_HDCP_KEYS;
#endif /* _typedef___AMP_AVIN_HDCP_KEYS */

#ifndef _typedef___AMP_AVIN_FETG_PARAMS
#define _typedef___AMP_AVIN_FETG_PARAMS
typedef struct AMP_AVIN_FETG_PARAMS {
  UINT32 start_x;
  UINT32 end_x;
  UINT32 start_y;
  UINT32 end_y;
  UINT32 vsamp;
  UINT32 htotal;
  UINT32 vtotal;
  UINT32 field_flag;
  UINT32 mode_3d;
  UINT32 sync_type;
  UINT32 pixel_freq;
  UINT32 refresh_rate;
} AMP_AVIN_FETG_PARAMS;
#endif /* _typedef___AMP_AVIN_FETG_PARAMS */

#ifndef _typedef___AMP_AVIN_RECT
#define _typedef___AMP_AVIN_RECT
typedef struct AMP_AVIN_RECT {
  INT16 x;
  INT16 y;
  INT16 w;
  INT16 h;
} AMP_AVIN_RECT;
#endif /* _typedef___AMP_AVIN_RECT */

#ifndef _typedef___AMP_AVIN_VBI_CONFIG
#define _typedef___AMP_AVIN_VBI_CONFIG
typedef struct AMP_AVIN_VBI_CONFIG {
  BOOL bEnTTX;
  BOOL bEnWSS;
  BOOL bEnNtscCC;
  BOOL bEnPalCC;
  BOOL bEnCGMS;
} AMP_AVIN_VBI_CONFIG;
#endif /* _typedef___AMP_AVIN_VBI_CONFIG */

#ifndef _define___AMP_AVIN_VBI_CONFIG_MAX_LINES
#define _define___AMP_AVIN_VBI_CONFIG_MAX_LINES
#define AMP_AVIN_VBI_CONFIG_MAX_LINES (26)
#endif

#ifndef _typedef___AMP_AVIN_VBI_LINES_CONFIG
#define _typedef___AMP_AVIN_VBI_LINES_CONFIG
typedef struct AMP_AVIN_VBI_LINES_CONFIG {
  BOOL bLinesTTX[26];
  BOOL bLinesWSS[26];
  BOOL bLinesNtscCC[26];
  BOOL bLinesPalCC[26];
  BOOL bLinesCGMS[26];
} AMP_AVIN_VBI_LINES_CONFIG;
#endif /* _typedef___AMP_AVIN_VBI_LINES_CONFIG */

#ifndef _typedef___AMP_AVIN_DUMP_INFO
#define _typedef___AMP_AVIN_DUMP_INFO
typedef struct AMP_AVIN_DUMP_INFO {
  UINT32 dumpId;
  INT8 dumpPath[1024];
} AMP_AVIN_DUMP_INFO;
#endif /* _typedef___AMP_AVIN_DUMP_INFO */

#ifndef _typedef___AMP_AVIN_COMPLETED_JOB_INFO
#define _typedef___AMP_AVIN_COMPLETED_JOB_INFO
typedef struct AMP_AVIN_COMPLETED_JOB_INFO {
  AMP_AVIN_COMPLETED_JOB_TYPE jobType;
} AMP_AVIN_COMPLETED_JOB_INFO;
#endif /* _typedef___AMP_AVIN_COMPLETED_JOB_INFO */

#ifndef _typedef___AMP_AVIN_COMBFILTER_COEFF
#define _typedef___AMP_AVIN_COMBFILTER_COEFF
typedef struct AMP_AVIN_COMBFILTER_COEFF {
  UINT16 uCoeff[32];
  UINT8 uSize;
} AMP_AVIN_COMBFILTER_COEFF;
#endif /* _typedef___AMP_AVIN_COMBFILTER_COEFF */

HRESULT AMP_AVIN_SetAdcGainOffset(AMP_AVIN _obj, struct AMP_AVIN_ADC_CALIB_DATA *pADCParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetAdcGainOffset(AMP_AVIN _obj, struct AMP_AVIN_ADC_CALIB_DATA *pADCParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetAdcGainExt(AMP_AVIN _obj, struct AMP_AVIN_ADC_CALIB_DATA *pADCParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetAdcGainExt(AMP_AVIN _obj, struct AMP_AVIN_ADC_CALIB_DATA *pADCParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_CalibrateAdc(AMP_AVIN _obj, struct AMP_AVIN_ADC_CALIB_DATA_EXT *pADCParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVgaPhase(AMP_AVIN _obj, INT8 phase, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVgaClock(AMP_AVIN _obj, INT8 clock, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVgaHShift(AMP_AVIN _obj, INT8 hshift, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVgaVShift(AMP_AVIN _obj, INT8 vshift, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVgaAutoAdjustment(AMP_AVIN _obj, struct AMP_AVIN_VGA_VIDEO_PROP *pParam, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetVgaAutoAdjustment(AMP_AVIN _obj, struct AMP_AVIN_VGA_VIDEO_PROP *pParam, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetStandardManual(AMP_AVIN _obj, AMP_AVIN_VIDEO_STANDARD standard, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetAtvMode(AMP_AVIN _obj, AMP_AVIN_ATV_MODE mode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Set3DComb(AMP_AVIN _obj, AMP_AVIN_COMBFILTER_MODE mode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Set3DCombFilterCoeff(AMP_AVIN _obj, AMP_AVIN_VIDEO_STANDARD eVidStandard, AMP_AVIN_COMBFILTER_TYPE eCombFilterType, struct AMP_AVIN_COMBFILTER_COEFF *pCombFilterCoeff, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Get3DCombFilterCoeff(AMP_AVIN _obj, AMP_AVIN_VIDEO_STANDARD eVidStandard, AMP_AVIN_COMBFILTER_TYPE eCombFilterType, struct AMP_AVIN_COMBFILTER_COEFF *pCombFilterCoeff, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetRegister(AMP_AVIN _obj, struct AMP_AVIN_REGISTER *pReg, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetRegister(AMP_AVIN _obj, struct AMP_AVIN_REGISTER *pReg, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetScreenMode(AMP_AVIN _obj, AMP_AVIN_SCREEN_MODE mode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetPlatformConfig(AMP_AVIN _obj, struct AMP_AVIN_PLATFORM_CONFIG *pPltConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetPlatformConfig(AMP_AVIN _obj, struct AMP_AVIN_PLATFORM_CONFIG *pPltConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetHdmiArc(AMP_AVIN _obj, BOOL arc_on, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetHdcpKeys(AMP_AVIN _obj, struct AMP_AVIN_HDCP_KEYS *pHdcpKey, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetActiveSourceList(AMP_AVIN _obj, struct AMP_AVIN_SOURCE_LIST *pSourceList, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetSifStandard(AMP_AVIN _obj, AMP_AVIN_SIF_STANDARD SifStandard, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetSifMode(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE sifMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetNicamMode(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE SifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetBtscSifMode(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE SifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetBtscSapSwitch(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE *pSifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetSifStandard(AMP_AVIN _obj, struct AMP_AVIN_SIF_STANDARD_EXT *pSifStandard, CORBA_Environment *_ev);

HRESULT AMP_AVIN_DetectSifStandard(AMP_AVIN _obj, AMP_AVIN_SIF_STANDARD sifStd, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetSifStatus(AMP_AVIN _obj, struct AMP_AVIN_SIF_STATUS *pSifStatus, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetNicamMode(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE *pSifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetBtscSifMode(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE *pSifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetBtscSapSifMode(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE *pSifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetSapPresence(AMP_AVIN _obj, AMP_AVIN_AUD_SIF_MODE *pSifAudioMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetSupportedSourceList(AMP_AVIN _obj, struct AMP_AVIN_SOURCE_LIST *pSourceList, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetCurrentSource(AMP_AVIN _obj, AMP_AVIN_SOURCE_TYPE *pSource, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetCurrentSource(AMP_AVIN _obj, AMP_AVIN_SOURCE_TYPE source, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetInputParams(AMP_AVIN _obj, struct AMP_AVIN_INPUT_PARAMS *pParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetAudioParams(AMP_AVIN _obj, struct AMP_AVIN_INPUT_AUDIO_PARAMS *pParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_UpdateEdid(AMP_AVIN _obj, struct AMP_AVIN_EDID_INFO *pEdidInfo, CORBA_Environment *_ev);

HRESULT AMP_AVIN_ToggleHpd(AMP_AVIN _obj, UINT32 hpd, CORBA_Environment *_ev);

HRESULT AMP_AVIN_ToggleHpdBySource(AMP_AVIN _obj, AMP_AVIN_SOURCE_TYPE source, UINT32 hpd, CORBA_Environment *_ev);

HRESULT AMP_AVIN_ReadEdid(AMP_AVIN _obj, struct AMP_AVIN_EDID_INFO *pEdidInfo, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetGMDPkt(AMP_AVIN _obj, struct AMP_AVIN_HDMI_PKT_GMD *pGmdPkt, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetFetgParams(AMP_AVIN _obj, struct AMP_AVIN_FETG_PARAMS *pParams, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Freeze(AMP_AVIN _obj, BOOL enable, CORBA_Environment *_ev);

HRESULT AMP_AVIN_FreeRunning(AMP_AVIN _obj, AMP_AVIN_VIDEO_STABLE_CHECK_TYPE type, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetVipState(AMP_AVIN _obj, INT16 *pState, CORBA_Environment *_ev);

HRESULT AMP_AVIN_ReadPixel(AMP_AVIN _obj, UINT32 x, UINT32 y, UINT32 *pPixelValue, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetVideoFrame(AMP_AVIN _obj, struct AMP_AVIN_RECT *pRect, UINT32 hShm, UINT32 *pSize, CORBA_Environment *_ev);

HRESULT AMP_AVIN_DumpFrame(AMP_AVIN _obj, struct AMP_AVIN_DUMP_INFO *pDumpInfo, CORBA_Environment *_ev);

HRESULT AMP_AVIN_PowerDown(AMP_AVIN _obj, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVbiConfig(AMP_AVIN _obj, struct AMP_AVIN_VBI_CONFIG *pVbiConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetVbiConfig(AMP_AVIN _obj, struct AMP_AVIN_VBI_CONFIG *pVbiConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetVbiLinesConfig(AMP_AVIN _obj, struct AMP_AVIN_VBI_LINES_CONFIG *pVbiConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetVbiLinesConfig(AMP_AVIN _obj, struct AMP_AVIN_VBI_LINES_CONFIG *pVbiConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetHdmiEqualizerGain(AMP_AVIN _obj, UINT8 u8EqGain, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetHdmiEqualizerGain(AMP_AVIN _obj, UINT8 *pEqGain, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetHdmiTermination(AMP_AVIN _obj, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetHdmiTermination(AMP_AVIN _obj, BOOL *pEnable, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetHdmiTmdsClockRate(AMP_AVIN _obj, AMP_AVIN_SOURCE_TYPE source, UINT32 *pClockRate, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetOutputPortAttribute(AMP_AVIN _obj, UINT32 uiPortIdx, struct AMP_AVIN_PORT_ATTRIB *pPortAttrib, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetPCMode(AMP_AVIN _obj, BOOL bIsPcMode, CORBA_Environment *_ev);

HRESULT AMP_AVIN_EnableScaler(AMP_AVIN _obj, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_AVIN_EnablePassthrough(AMP_AVIN _obj, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetHdmiAudCAInfo(AMP_AVIN _obj, UINT16 *pAudCA, CORBA_Environment *_ev);

HRESULT AMP_AVIN_EnableLowLatency(AMP_AVIN _obj, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetOutputPlane(AMP_AVIN _obj, UINT32 uiPlane, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetOutputComp(AMP_AVIN _obj, UINT32 uiOutputComp, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Open(AMP_AVIN _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Close(AMP_AVIN _obj, CORBA_Environment *_ev);

HRESULT AMP_AVIN_Destroy(AMP_AVIN _obj, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetState(AMP_AVIN _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_AVIN_GetState(AMP_AVIN _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_AVIN_QueryInfo(AMP_AVIN _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_AVIN_SetupPort(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_AVIN_DynamicSetupPort(AMP_AVIN _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AVIN_DisconnectPorts(AMP_AVIN _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AVIN_ClearPort(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AVIN_QueryPort(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_AVIN_ClearPortBuf(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_AVIN_DebugDump(AMP_AVIN _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_AVIN_RegisterNotify(AMP_AVIN _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AVIN_UnregisterNotify(AMP_AVIN _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AVIN_RegisterNotifyDetail(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AVIN_UnregisterNotifyDetail(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_AVIN_RegisterBD(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AVIN_UnregisterBD(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_AVIN_PushBD(AMP_AVIN _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_VOUT
#define _typedef___AMP_VOUT
typedef CORBA_Object AMP_VOUT;
#endif /* _typedef___AMP_VOUT */

#ifndef _define___AMP_VOUT_SHOWBLACKSCREEN
#define _define___AMP_VOUT_SHOWBLACKSCREEN
#define AMP_VOUT_SHOWBLACKSCREEN (0)
#endif

#ifndef _define___AMP_VOUT_SHOWBLUESCREEN
#define _define___AMP_VOUT_SHOWBLUESCREEN
#define AMP_VOUT_SHOWBLUESCREEN (1)
#endif

#ifndef _define___AMP_VOUT_SHOWBLUEWHITESCREEN
#define _define___AMP_VOUT_SHOWBLUEWHITESCREEN
#define AMP_VOUT_SHOWBLUEWHITESCREEN (2)
#endif

#ifndef _define___AMP_VOUT_REPEATLASTFRAME
#define _define___AMP_VOUT_REPEATLASTFRAME
#define AMP_VOUT_REPEATLASTFRAME (3)
#endif

#ifndef _define___AMP_VOUT_KEEPLASTFRAME
#define _define___AMP_VOUT_KEEPLASTFRAME
#define AMP_VOUT_KEEPLASTFRAME (4)
#endif

#ifndef _define___AMP_VOUT_MAXLASTFRAMEMODE
#define _define___AMP_VOUT_MAXLASTFRAMEMODE
#define AMP_VOUT_MAXLASTFRAMEMODE (5)
#endif

#ifndef _typedef___AMP_VOUT_LastFrameAction
#define _typedef___AMP_VOUT_LastFrameAction
typedef CORBA_enum AMP_VOUT_LastFrameAction;
#endif /* _typedef___AMP_VOUT_LastFrameAction */

#ifndef _define___AMP_VOUT_FRAME_DISPLAYED
#define _define___AMP_VOUT_FRAME_DISPLAYED
#define AMP_VOUT_FRAME_DISPLAYED (1)
#endif

#ifndef _define___AMP_VOUT_FRAME_DROPPED
#define _define___AMP_VOUT_FRAME_DROPPED
#define AMP_VOUT_FRAME_DROPPED (0)
#endif

#ifndef _define___AMP_VOUT_FRAME_TYPE_I
#define _define___AMP_VOUT_FRAME_TYPE_I
#define AMP_VOUT_FRAME_TYPE_I (0)
#endif

#ifndef _define___AMP_VOUT_FRAME_TYPE_P
#define _define___AMP_VOUT_FRAME_TYPE_P
#define AMP_VOUT_FRAME_TYPE_P (1)
#endif

#ifndef _define___AMP_VOUT_FRAME_TYPE_B
#define _define___AMP_VOUT_FRAME_TYPE_B
#define AMP_VOUT_FRAME_TYPE_B (2)
#endif

#ifndef _define___AMP_VOUT_NORMAL_MODE
#define _define___AMP_VOUT_NORMAL_MODE
#define AMP_VOUT_NORMAL_MODE (0)
#endif

#ifndef _define___AMP_VOUT_LOW_LATENCY_MODE
#define _define___AMP_VOUT_LOW_LATENCY_MODE
#define AMP_VOUT_LOW_LATENCY_MODE (1)
#endif

#ifndef _typedef___AMP_VOUT_DISP_FRAME_INFO
#define _typedef___AMP_VOUT_DISP_FRAME_INFO
typedef struct AMP_VOUT_DISP_FRAME_INFO {
  UINT32 displayed;
  UINT32 pts_l;
  UINT32 pts_h;
  AMP_PTS pts64;
  UINT32 frame_type;
  UINT32 reserved[3];
} AMP_VOUT_DISP_FRAME_INFO;
#endif /* _typedef___AMP_VOUT_DISP_FRAME_INFO */

#ifndef _typedef___AMP_VOUT_CAP_FRAME_INFO
#define _typedef___AMP_VOUT_CAP_FRAME_INFO
typedef struct AMP_VOUT_CAP_FRAME_INFO {
  UINT32 uSuccess;
  UINT32 uCapWidth;
  UINT32 uCapHeight;
  UINT32 uCapPts_l;
  UINT32 uCapPts_h;
  AMP_PTS uCapPts64;
  UINT32 uFrameWidth;
  UINT32 uFrameHeight;
} AMP_VOUT_CAP_FRAME_INFO;
#endif /* _typedef___AMP_VOUT_CAP_FRAME_INFO */

#ifndef _define___AMP_VOUT_CURSOR_MOVE_UP
#define _define___AMP_VOUT_CURSOR_MOVE_UP
#define AMP_VOUT_CURSOR_MOVE_UP (0)
#endif

#ifndef _define___AMP_VOUT_CURSOR_MOVE_DOWN
#define _define___AMP_VOUT_CURSOR_MOVE_DOWN
#define AMP_VOUT_CURSOR_MOVE_DOWN (1)
#endif

#ifndef _define___AMP_VOUT_CURSOR_MOVE_LEFT
#define _define___AMP_VOUT_CURSOR_MOVE_LEFT
#define AMP_VOUT_CURSOR_MOVE_LEFT (2)
#endif

#ifndef _define___AMP_VOUT_CURSOR_MOVE_RIGHT
#define _define___AMP_VOUT_CURSOR_MOVE_RIGHT
#define AMP_VOUT_CURSOR_MOVE_RIGHT (3)
#endif

#ifndef _typedef___AMP_VOUT_CURSOR_MOVE_DIRECTION
#define _typedef___AMP_VOUT_CURSOR_MOVE_DIRECTION
typedef CORBA_enum AMP_VOUT_CURSOR_MOVE_DIRECTION;
#endif /* _typedef___AMP_VOUT_CURSOR_MOVE_DIRECTION */

HRESULT AMP_VOUT_SetLastFrameMode(AMP_VOUT _obj, AMP_VOUT_LastFrameAction eAction, CORBA_Environment *_ev);

HRESULT AMP_VOUT_GetDecodedFrameCount(AMP_VOUT _obj, UINT32 uiPortIdx, UINT32 *count, CORBA_Environment *_ev);

HRESULT AMP_VOUT_CaptureScreen(AMP_VOUT _obj, UINT32 uiPortIdx, UINT32 hFrameBuf, CORBA_Environment *_ev);

HRESULT AMP_VOUT_GetCurrentPTS(AMP_VOUT _obj, UINT32 *uiPtsHigh, UINT32 *uiPtsLow, CORBA_Environment *_ev);

HRESULT AMP_VOUT_SetLowLatency(AMP_VOUT _obj, UINT32 uiMode, UINT32 uiThr, CORBA_Environment *_ev);

HRESULT AMP_VOUT_GetPlaneState(AMP_VOUT _obj, UINT32 vPlane, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_VOUT_SetSREnable(AMP_VOUT _obj, BOOL enable, CORBA_Environment *_ev);

HRESULT AMP_VOUT_SetSBSEnable(AMP_VOUT _obj, BOOL enable, CORBA_Environment *_ev);

HRESULT AMP_VOUT_MoveSBSWin(AMP_VOUT _obj, AMP_VOUT_CURSOR_MOVE_DIRECTION direction, UINT32 pixel, CORBA_Environment *_ev);

HRESULT AMP_VOUT_Open(AMP_VOUT _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_VOUT_Close(AMP_VOUT _obj, CORBA_Environment *_ev);

HRESULT AMP_VOUT_Destroy(AMP_VOUT _obj, CORBA_Environment *_ev);

HRESULT AMP_VOUT_SetState(AMP_VOUT _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_VOUT_GetState(AMP_VOUT _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_VOUT_QueryInfo(AMP_VOUT _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_VOUT_SetupPort(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_VOUT_DynamicSetupPort(AMP_VOUT _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VOUT_DisconnectPorts(AMP_VOUT _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VOUT_ClearPort(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VOUT_QueryPort(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_VOUT_ClearPortBuf(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VOUT_DebugDump(AMP_VOUT _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_VOUT_RegisterNotify(AMP_VOUT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VOUT_UnregisterNotify(AMP_VOUT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VOUT_RegisterNotifyDetail(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VOUT_UnregisterNotifyDetail(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VOUT_RegisterBD(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VOUT_UnregisterBD(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VOUT_PushBD(AMP_VOUT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DEINT
#define _typedef___AMP_DEINT
typedef CORBA_Object AMP_DEINT;
#endif /* _typedef___AMP_DEINT */

#ifndef _typedef___AMP_DEINT_RES_CHANGE_INFO
#define _typedef___AMP_DEINT_RES_CHANGE_INFO
typedef struct AMP_DEINT_RES_CHANGE_INFO {
  UINT32 uiWidth;
  UINT32 uiHeight;
  UINT32 uiBitDepth;
  UINT32 uiSrcFmt;
  UINT32 uiSize;
} AMP_DEINT_RES_CHANGE_INFO;
#endif /* _typedef___AMP_DEINT_RES_CHANGE_INFO */

#ifndef _define___AMP_DEINT_EVENT_NONE_MASK
#define _define___AMP_DEINT_EVENT_NONE_MASK
#define AMP_DEINT_EVENT_NONE_MASK (0)
#endif

#ifndef _define___AMP_DEINT_EVENT_ERROR_HANDLE
#define _define___AMP_DEINT_EVENT_ERROR_HANDLE
#define AMP_DEINT_EVENT_ERROR_HANDLE (1)
#endif

#ifndef _define___AMP_DEINT_EVENT_RESOLUTION_CHANGE
#define _define___AMP_DEINT_EVENT_RESOLUTION_CHANGE
#define AMP_DEINT_EVENT_RESOLUTION_CHANGE (2)
#endif

#ifndef _define___AMP_DEINT_EVENT_ALL_MASK
#define _define___AMP_DEINT_EVENT_ALL_MASK
#define AMP_DEINT_EVENT_ALL_MASK (3)
#endif

#ifndef _define___AMP_DEINT_MODE_TWO_FRAMES
#define _define___AMP_DEINT_MODE_TWO_FRAMES
#define AMP_DEINT_MODE_TWO_FRAMES (0)
#endif

#ifndef _define___AMP_DEINT_MODE_ONE_FRAME
#define _define___AMP_DEINT_MODE_ONE_FRAME
#define AMP_DEINT_MODE_ONE_FRAME (1)
#endif

#ifndef _define___AMP_DEINT_MODE_TWO_FIELDS
#define _define___AMP_DEINT_MODE_TWO_FIELDS
#define AMP_DEINT_MODE_TWO_FIELDS (2)
#endif

#ifndef _define___AMP_DEINT_MODE_ONE_FIELD
#define _define___AMP_DEINT_MODE_ONE_FIELD
#define AMP_DEINT_MODE_ONE_FIELD (3)
#endif

#ifndef _define___AMP_DEINT_MODE_MAX
#define _define___AMP_DEINT_MODE_MAX
#define AMP_DEINT_MODE_MAX (4)
#endif

#ifndef _typedef___AMP_DEINT_MODE
#define _typedef___AMP_DEINT_MODE
typedef CORBA_enum AMP_DEINT_MODE;
#endif /* _typedef___AMP_DEINT_MODE */

HRESULT AMP_DEINT_SetOutputMode(AMP_DEINT _obj, AMP_DEINT_MODE mode, CORBA_Environment *_ev);

HRESULT AMP_DEINT_GetOutputMode(AMP_DEINT _obj, AMP_DEINT_MODE *mode, CORBA_Environment *_ev);

HRESULT AMP_DEINT_Open(AMP_DEINT _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DEINT_Close(AMP_DEINT _obj, CORBA_Environment *_ev);

HRESULT AMP_DEINT_Destroy(AMP_DEINT _obj, CORBA_Environment *_ev);

HRESULT AMP_DEINT_SetState(AMP_DEINT _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DEINT_GetState(AMP_DEINT _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DEINT_QueryInfo(AMP_DEINT _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DEINT_SetupPort(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DEINT_DynamicSetupPort(AMP_DEINT _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEINT_DisconnectPorts(AMP_DEINT _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEINT_ClearPort(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEINT_QueryPort(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DEINT_ClearPortBuf(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEINT_DebugDump(AMP_DEINT _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DEINT_RegisterNotify(AMP_DEINT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEINT_UnregisterNotify(AMP_DEINT _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEINT_RegisterNotifyDetail(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEINT_UnregisterNotifyDetail(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEINT_RegisterBD(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DEINT_UnregisterBD(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DEINT_PushBD(AMP_DEINT _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_OVPP
#define _typedef___AMP_OVPP
typedef CORBA_Object AMP_OVPP;
#endif /* _typedef___AMP_OVPP */

#ifndef _typedef___AMP_OVPP_RES_CHANGE_INFO
#define _typedef___AMP_OVPP_RES_CHANGE_INFO
typedef struct AMP_OVPP_RES_CHANGE_INFO {
  UINT32 uiWidth;
  UINT32 uiHeight;
  UINT32 uiBitDepth;
  UINT32 uiSrcFmt;
  UINT32 uiSize;
} AMP_OVPP_RES_CHANGE_INFO;
#endif /* _typedef___AMP_OVPP_RES_CHANGE_INFO */

#ifndef _define___AMP_OVPP_EVENT_NONE_MASK
#define _define___AMP_OVPP_EVENT_NONE_MASK
#define AMP_OVPP_EVENT_NONE_MASK (0)
#endif

#ifndef _define___AMP_OVPP_EVENT_ERROR_HANDLE
#define _define___AMP_OVPP_EVENT_ERROR_HANDLE
#define AMP_OVPP_EVENT_ERROR_HANDLE (1)
#endif

#ifndef _define___AMP_OVPP_EVENT_RESOLUTION_CHANGE
#define _define___AMP_OVPP_EVENT_RESOLUTION_CHANGE
#define AMP_OVPP_EVENT_RESOLUTION_CHANGE (2)
#endif

#ifndef _define___AMP_OVPP_EVENT_ALL_MASK
#define _define___AMP_OVPP_EVENT_ALL_MASK
#define AMP_OVPP_EVENT_ALL_MASK (3)
#endif

#ifndef _define___AMP_OVPP_MODE_TWO_FRAMES
#define _define___AMP_OVPP_MODE_TWO_FRAMES
#define AMP_OVPP_MODE_TWO_FRAMES (0)
#endif

#ifndef _define___AMP_OVPP_MODE_ONE_FRAME
#define _define___AMP_OVPP_MODE_ONE_FRAME
#define AMP_OVPP_MODE_ONE_FRAME (1)
#endif

#ifndef _define___AMP_OVPP_MODE_TWO_FIELDS
#define _define___AMP_OVPP_MODE_TWO_FIELDS
#define AMP_OVPP_MODE_TWO_FIELDS (2)
#endif

#ifndef _define___AMP_OVPP_MODE_ONE_FIELD
#define _define___AMP_OVPP_MODE_ONE_FIELD
#define AMP_OVPP_MODE_ONE_FIELD (3)
#endif

#ifndef _define___AMP_OVPP_MODE_MAX
#define _define___AMP_OVPP_MODE_MAX
#define AMP_OVPP_MODE_MAX (4)
#endif

#ifndef _typedef___AMP_OVPP_MODE
#define _typedef___AMP_OVPP_MODE
typedef CORBA_enum AMP_OVPP_MODE;
#endif /* _typedef___AMP_OVPP_MODE */

HRESULT AMP_OVPP_SetOutputMode(AMP_OVPP _obj, AMP_OVPP_MODE mode, CORBA_Environment *_ev);

HRESULT AMP_OVPP_GetOutputMode(AMP_OVPP _obj, AMP_OVPP_MODE *mode, CORBA_Environment *_ev);

HRESULT AMP_OVPP_Open(AMP_OVPP _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_OVPP_Close(AMP_OVPP _obj, CORBA_Environment *_ev);

HRESULT AMP_OVPP_Destroy(AMP_OVPP _obj, CORBA_Environment *_ev);

HRESULT AMP_OVPP_SetState(AMP_OVPP _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_OVPP_GetState(AMP_OVPP _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_OVPP_QueryInfo(AMP_OVPP _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_OVPP_SetupPort(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_OVPP_DynamicSetupPort(AMP_OVPP _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_OVPP_DisconnectPorts(AMP_OVPP _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_OVPP_ClearPort(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_OVPP_QueryPort(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_OVPP_ClearPortBuf(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_OVPP_DebugDump(AMP_OVPP _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_OVPP_RegisterNotify(AMP_OVPP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_OVPP_UnregisterNotify(AMP_OVPP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_OVPP_RegisterNotifyDetail(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_OVPP_UnregisterNotifyDetail(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_OVPP_RegisterBD(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_OVPP_UnregisterBD(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_OVPP_PushBD(AMP_OVPP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_SCALER
#define _typedef___AMP_SCALER
typedef CORBA_Object AMP_SCALER;
#endif /* _typedef___AMP_SCALER */

#ifndef _define___AMP_SCALER_EVENT_FRAME_DROPPED
#define _define___AMP_SCALER_EVENT_FRAME_DROPPED
#define AMP_SCALER_EVENT_FRAME_DROPPED (0)
#endif

#ifndef _define___AMP_SCALER_EVENT_ERROR_INVALID_FORMAT
#define _define___AMP_SCALER_EVENT_ERROR_INVALID_FORMAT
#define AMP_SCALER_EVENT_ERROR_INVALID_FORMAT (1)
#endif

#ifndef _define___AMP_SCALER_EVENT_FIRST_FRAME_RESCALED
#define _define___AMP_SCALER_EVENT_FIRST_FRAME_RESCALED
#define AMP_SCALER_EVENT_FIRST_FRAME_RESCALED (2)
#endif

#ifndef _define___AMP_SCALER_EVENT_FRAME_RESCALED
#define _define___AMP_SCALER_EVENT_FRAME_RESCALED
#define AMP_SCALER_EVENT_FRAME_RESCALED (4)
#endif

#ifndef _define___AMP_SCALER_EVENT_ALL_MASK
#define _define___AMP_SCALER_EVENT_ALL_MASK
#define AMP_SCALER_EVENT_ALL_MASK (255)
#endif

#ifndef _define___AMP_SCALER_INPUT_MAX_1080
#define _define___AMP_SCALER_INPUT_MAX_1080
#define AMP_SCALER_INPUT_MAX_1080 (0)
#endif

#ifndef _define___AMP_SCALER_INPUT_MAX_2160
#define _define___AMP_SCALER_INPUT_MAX_2160
#define AMP_SCALER_INPUT_MAX_2160 (1)
#endif

#ifndef _define___AMP_SCALER_INPUT_MODE_MAX
#define _define___AMP_SCALER_INPUT_MODE_MAX
#define AMP_SCALER_INPUT_MODE_MAX (2)
#endif

#ifndef _typedef___AMP_SCALER_INPUT_SIZE
#define _typedef___AMP_SCALER_INPUT_SIZE
typedef CORBA_enum AMP_SCALER_INPUT_SIZE;
#endif /* _typedef___AMP_SCALER_INPUT_SIZE */

HRESULT AMP_SCALER_Configure_MaxInputSize(AMP_SCALER _obj, AMP_SCALER_INPUT_SIZE maxInputSize, CORBA_Environment *_ev);

HRESULT AMP_SCALER_SetOutputResolution(AMP_SCALER _obj, UINT32 uiWidth, UINT32 uiHeight, CORBA_Environment *_ev);

HRESULT AMP_SCALER_GetOutputResolution(AMP_SCALER _obj, UINT32 *uiWidth, UINT32 *uiHeight, CORBA_Environment *_ev);

HRESULT AMP_SCALER_ReduceFrameRate(AMP_SCALER _obj, UINT32 uiDivisor, CORBA_Environment *_ev);

HRESULT AMP_SCALER_Open(AMP_SCALER _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_SCALER_Close(AMP_SCALER _obj, CORBA_Environment *_ev);

HRESULT AMP_SCALER_Destroy(AMP_SCALER _obj, CORBA_Environment *_ev);

HRESULT AMP_SCALER_SetState(AMP_SCALER _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_SCALER_GetState(AMP_SCALER _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_SCALER_QueryInfo(AMP_SCALER _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_SCALER_SetupPort(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_SCALER_DynamicSetupPort(AMP_SCALER _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SCALER_DisconnectPorts(AMP_SCALER _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SCALER_ClearPort(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SCALER_QueryPort(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_SCALER_ClearPortBuf(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SCALER_DebugDump(AMP_SCALER _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_SCALER_RegisterNotify(AMP_SCALER _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SCALER_UnregisterNotify(AMP_SCALER _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SCALER_RegisterNotifyDetail(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SCALER_UnregisterNotifyDetail(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SCALER_RegisterBD(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_SCALER_UnregisterBD(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_SCALER_PushBD(AMP_SCALER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_VCAP
#define _typedef___AMP_VCAP
typedef CORBA_Object AMP_VCAP;
#endif /* _typedef___AMP_VCAP */

#ifndef _define___AMP_VCAP_EVENT_NONE_MASK
#define _define___AMP_VCAP_EVENT_NONE_MASK
#define AMP_VCAP_EVENT_NONE_MASK (0)
#endif

#ifndef _define___AMP_VCAP_EVENT_FIRST_FRAME_CAPTURED
#define _define___AMP_VCAP_EVENT_FIRST_FRAME_CAPTURED
#define AMP_VCAP_EVENT_FIRST_FRAME_CAPTURED (1)
#endif

#ifndef _define___AMP_VCAP_EVENT_ONE_FRAME_CAPTURED
#define _define___AMP_VCAP_EVENT_ONE_FRAME_CAPTURED
#define AMP_VCAP_EVENT_ONE_FRAME_CAPTURED (2)
#endif

#ifndef _define___AMP_VCAP_EVENT_FR_CHANGE
#define _define___AMP_VCAP_EVENT_FR_CHANGE
#define AMP_VCAP_EVENT_FR_CHANGE (3)
#endif

#ifndef _define___AMP_VCAP_EVENT_CAPTURE_DONE
#define _define___AMP_VCAP_EVENT_CAPTURE_DONE
#define AMP_VCAP_EVENT_CAPTURE_DONE (4)
#endif

#ifndef _define___AMP_VCAP_EVENT_CAPTURE_UNDERFLOW
#define _define___AMP_VCAP_EVENT_CAPTURE_UNDERFLOW
#define AMP_VCAP_EVENT_CAPTURE_UNDERFLOW (5)
#endif

#ifndef _define___AMP_VCAP_EVENT_ALL_MASK
#define _define___AMP_VCAP_EVENT_ALL_MASK
#define AMP_VCAP_EVENT_ALL_MASK (255)
#endif

HRESULT AMP_VCAP_SetFrameRate(AMP_VCAP _obj, UINT16 uiFR, CORBA_Environment *_ev);

HRESULT AMP_VCAP_GetFrameRate(AMP_VCAP _obj, UINT16 *uiFR, CORBA_Environment *_ev);

HRESULT AMP_VCAP_SetEventMask(AMP_VCAP _obj, UINT32 uiMaskBit, CORBA_Environment *_ev);

HRESULT AMP_VCAP_GetEventMask(AMP_VCAP _obj, UINT32 *uiMaskBit, CORBA_Environment *_ev);

HRESULT AMP_VCAP_Open(AMP_VCAP _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_VCAP_Close(AMP_VCAP _obj, CORBA_Environment *_ev);

HRESULT AMP_VCAP_Destroy(AMP_VCAP _obj, CORBA_Environment *_ev);

HRESULT AMP_VCAP_SetState(AMP_VCAP _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_VCAP_GetState(AMP_VCAP _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_VCAP_QueryInfo(AMP_VCAP _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_VCAP_SetupPort(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_VCAP_DynamicSetupPort(AMP_VCAP _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VCAP_DisconnectPorts(AMP_VCAP _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VCAP_ClearPort(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VCAP_QueryPort(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_VCAP_ClearPortBuf(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VCAP_DebugDump(AMP_VCAP _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_VCAP_RegisterNotify(AMP_VCAP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VCAP_UnregisterNotify(AMP_VCAP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VCAP_RegisterNotifyDetail(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VCAP_UnregisterNotifyDetail(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VCAP_RegisterBD(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VCAP_UnregisterBD(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VCAP_PushBD(AMP_VCAP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_SR
#define _typedef___AMP_SR
typedef CORBA_Object AMP_SR;
#endif /* _typedef___AMP_SR */

#ifndef _define___AMP_SR_EVENT_NONE_MASK
#define _define___AMP_SR_EVENT_NONE_MASK
#define AMP_SR_EVENT_NONE_MASK (0)
#endif

#ifndef _define___AMP_SR_EVENT_SCALING_BY_NPU
#define _define___AMP_SR_EVENT_SCALING_BY_NPU
#define AMP_SR_EVENT_SCALING_BY_NPU (1)
#endif

#ifndef _define___AMP_SR_EVENT_SCALING_BY_HW
#define _define___AMP_SR_EVENT_SCALING_BY_HW
#define AMP_SR_EVENT_SCALING_BY_HW (2)
#endif

#ifndef _define___AMP_SR_EVENT_NPU_NOT_AVAILABLE
#define _define___AMP_SR_EVENT_NPU_NOT_AVAILABLE
#define AMP_SR_EVENT_NPU_NOT_AVAILABLE (3)
#endif

#ifndef _define___AMP_SR_EVENT_INPUT_FRAME_PARAMS_INVALID
#define _define___AMP_SR_EVENT_INPUT_FRAME_PARAMS_INVALID
#define AMP_SR_EVENT_INPUT_FRAME_PARAMS_INVALID (4)
#endif

#ifndef _define___AMP_SR_EVENT_INPUT_OUTPUT_INCOMPATIBLE
#define _define___AMP_SR_EVENT_INPUT_OUTPUT_INCOMPATIBLE
#define AMP_SR_EVENT_INPUT_OUTPUT_INCOMPATIBLE (5)
#endif

#ifndef _define___AMP_SR_EVENT_ONE_FRAME_UPSCALING_DONE
#define _define___AMP_SR_EVENT_ONE_FRAME_UPSCALING_DONE
#define AMP_SR_EVENT_ONE_FRAME_UPSCALING_DONE (6)
#endif

#ifndef _define___AMP_SR_EVENT_UPSCALING_ERR
#define _define___AMP_SR_EVENT_UPSCALING_ERR
#define AMP_SR_EVENT_UPSCALING_ERR (7)
#endif

#ifndef _define___AMP_SR_EVENT_ALL_MASK
#define _define___AMP_SR_EVENT_ALL_MASK
#define AMP_SR_EVENT_ALL_MASK (255)
#endif

#ifndef _typedef___AMP_SR_STATESET
#define _typedef___AMP_SR_STATESET
typedef struct AMP_SR_STATESET {
  UINT32 uiInputWidth;
  UINT32 uiInputHeight;
  UINT32 uiOutputWidth;
  UINT32 uiOutputHeight;
  CORBA_float fFrameRate;
  UINT32 uiFrameFormat;
  BOOL bNPUAvailable;
  BOOL bScalingByNPU;
} AMP_SR_STATESET;
#endif /* _typedef___AMP_SR_STATESET */

HRESULT AMP_SR_GetScalingState(AMP_SR _obj, struct AMP_SR_STATESET *curState, CORBA_Environment *_ev);

HRESULT AMP_SR_Open(AMP_SR _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_SR_Close(AMP_SR _obj, CORBA_Environment *_ev);

HRESULT AMP_SR_Destroy(AMP_SR _obj, CORBA_Environment *_ev);

HRESULT AMP_SR_SetState(AMP_SR _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_SR_GetState(AMP_SR _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_SR_QueryInfo(AMP_SR _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_SR_SetupPort(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_SR_DynamicSetupPort(AMP_SR _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SR_DisconnectPorts(AMP_SR _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SR_ClearPort(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SR_QueryPort(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_SR_ClearPortBuf(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SR_DebugDump(AMP_SR _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_SR_RegisterNotify(AMP_SR _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SR_UnregisterNotify(AMP_SR _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SR_RegisterNotifyDetail(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SR_UnregisterNotifyDetail(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SR_RegisterBD(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_SR_UnregisterBD(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_SR_PushBD(AMP_SR _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DVS
#define _typedef___AMP_DVS
typedef CORBA_Object AMP_DVS;
#endif /* _typedef___AMP_DVS */

#ifndef _typedef___AMP_DVS_HANDLE
#define _typedef___AMP_DVS_HANDLE
typedef UINT32 AMP_DVS_HANDLE;
#endif /* _typedef___AMP_DVS_HANDLE */

#ifndef _define___AMP_DVS_DV_IN_0
#define _define___AMP_DVS_DV_IN_0
#define AMP_DVS_DV_IN_0 (0)
#endif

#ifndef _define___AMP_DVS_DV_IN_1
#define _define___AMP_DVS_DV_IN_1
#define AMP_DVS_DV_IN_1 (1)
#endif

#ifndef _define___AMP_DVS_DV_IN_MAX
#define _define___AMP_DVS_DV_IN_MAX
#define AMP_DVS_DV_IN_MAX (2)
#endif

#ifndef _define___AMP_DVS_DV_IN_ALL
#define _define___AMP_DVS_DV_IN_ALL
#define AMP_DVS_DV_IN_ALL (3)
#endif

#ifndef _typedef___AMP_DVS_INPUT_PORT
#define _typedef___AMP_DVS_INPUT_PORT
typedef CORBA_enum AMP_DVS_INPUT_PORT;
#endif /* _typedef___AMP_DVS_INPUT_PORT */

#ifndef _define___AMP_DVS_DV_OUTPUT_BL
#define _define___AMP_DVS_DV_OUTPUT_BL
#define AMP_DVS_DV_OUTPUT_BL (0)
#endif

#ifndef _define___AMP_DVS_DV_OUTPUT_EL
#define _define___AMP_DVS_DV_OUTPUT_EL
#define AMP_DVS_DV_OUTPUT_EL (1)
#endif

#ifndef _define___AMP_DVS_DV_OUTPUT_RPU
#define _define___AMP_DVS_DV_OUTPUT_RPU
#define AMP_DVS_DV_OUTPUT_RPU (2)
#endif

#ifndef _define___AMP_DVS_DV_OUTPUT_MAX
#define _define___AMP_DVS_DV_OUTPUT_MAX
#define AMP_DVS_DV_OUTPUT_MAX (3)
#endif

#ifndef _define___AMP_DVS_DV_OUTPUT_ALL
#define _define___AMP_DVS_DV_OUTPUT_ALL
#define AMP_DVS_DV_OUTPUT_ALL (4)
#endif

#ifndef _typedef___AMP_DVS_OUTPUT_PORT
#define _typedef___AMP_DVS_OUTPUT_PORT
typedef CORBA_enum AMP_DVS_OUTPUT_PORT;
#endif /* _typedef___AMP_DVS_OUTPUT_PORT */

#ifndef _define___AMP_DVS_DV_LAYER_BL
#define _define___AMP_DVS_DV_LAYER_BL
#define AMP_DVS_DV_LAYER_BL (0)
#endif

#ifndef _define___AMP_DVS_DV_LAYER_EL
#define _define___AMP_DVS_DV_LAYER_EL
#define AMP_DVS_DV_LAYER_EL (1)
#endif

#ifndef _define___AMP_DVS_DV_LAYER_MAX
#define _define___AMP_DVS_DV_LAYER_MAX
#define AMP_DVS_DV_LAYER_MAX (2)
#endif

#ifndef _typedef___AMP_DVS_DV_LAYER
#define _typedef___AMP_DVS_DV_LAYER
typedef CORBA_enum AMP_DVS_DV_LAYER;
#endif /* _typedef___AMP_DVS_DV_LAYER */

#ifndef _define___AMP_DVS_DV_AVC_CODEC
#define _define___AMP_DVS_DV_AVC_CODEC
#define AMP_DVS_DV_AVC_CODEC (0)
#endif

#ifndef _define___AMP_DVS_DV_HEVC_CODEC
#define _define___AMP_DVS_DV_HEVC_CODEC
#define AMP_DVS_DV_HEVC_CODEC (1)
#endif

#ifndef _define___AMP_DVS_DV_AV1_CODEC
#define _define___AMP_DVS_DV_AV1_CODEC
#define AMP_DVS_DV_AV1_CODEC (2)
#endif

#ifndef _define___AMP_DVS_DV_MAX_CODEC
#define _define___AMP_DVS_DV_MAX_CODEC
#define AMP_DVS_DV_MAX_CODEC (3)
#endif

#ifndef _typedef___AMP_DVS_DV_ES_CODEC
#define _typedef___AMP_DVS_DV_ES_CODEC
typedef CORBA_enum AMP_DVS_DV_ES_CODEC;
#endif /* _typedef___AMP_DVS_DV_ES_CODEC */

HRESULT AMP_DVS_SetLayerCodec(AMP_DVS _obj, AMP_DVS_DV_LAYER ulayer, AMP_DVS_DV_ES_CODEC uCodec, CORBA_Environment *_ev);

HRESULT AMP_DVS_SetInputFrameInMode(AMP_DVS _obj, AMP_DVS_INPUT_PORT uiPortIdx, BOOL uIsFrameIn, CORBA_Environment *_ev);

HRESULT AMP_DVS_SetOutputBufferSecure(AMP_DVS _obj, AMP_DVS_OUTPUT_PORT uiPortIdx, BOOL uIsSecure, CORBA_Environment *_ev);

HRESULT AMP_DVS_Open(AMP_DVS _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DVS_Close(AMP_DVS _obj, CORBA_Environment *_ev);

HRESULT AMP_DVS_Destroy(AMP_DVS _obj, CORBA_Environment *_ev);

HRESULT AMP_DVS_SetState(AMP_DVS _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DVS_GetState(AMP_DVS _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DVS_QueryInfo(AMP_DVS _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DVS_SetupPort(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DVS_DynamicSetupPort(AMP_DVS _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DVS_DisconnectPorts(AMP_DVS _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DVS_ClearPort(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DVS_QueryPort(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DVS_ClearPortBuf(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DVS_DebugDump(AMP_DVS _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DVS_RegisterNotify(AMP_DVS _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DVS_UnregisterNotify(AMP_DVS _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DVS_RegisterNotifyDetail(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DVS_UnregisterNotifyDetail(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DVS_RegisterBD(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DVS_UnregisterBD(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DVS_PushBD(AMP_DVS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DATA_EXPORTER
#define _typedef___AMP_DATA_EXPORTER
typedef CORBA_Object AMP_DATA_EXPORTER;
#endif /* _typedef___AMP_DATA_EXPORTER */

#ifndef _define___AMP_DATA_EXPORTER_EVENT_NONE_MASK
#define _define___AMP_DATA_EXPORTER_EVENT_NONE_MASK
#define AMP_DATA_EXPORTER_EVENT_NONE_MASK (0)
#endif

#ifndef _define___AMP_DATA_EXPORTER_EVENT_ERROR_UNKNOWN_DATA
#define _define___AMP_DATA_EXPORTER_EVENT_ERROR_UNKNOWN_DATA
#define AMP_DATA_EXPORTER_EVENT_ERROR_UNKNOWN_DATA (1)
#endif

#ifndef _define___AMP_DATA_EXPORTER_EVENT_FIRST_TELETEXT
#define _define___AMP_DATA_EXPORTER_EVENT_FIRST_TELETEXT
#define AMP_DATA_EXPORTER_EVENT_FIRST_TELETEXT (2)
#endif

#ifndef _define___AMP_DATA_EXPORTER_EVENT_FIRST_SUBTITLE
#define _define___AMP_DATA_EXPORTER_EVENT_FIRST_SUBTITLE
#define AMP_DATA_EXPORTER_EVENT_FIRST_SUBTITLE (4)
#endif

#ifndef _define___AMP_DATA_EXPORTER_EVENT_FIRST_SECTION
#define _define___AMP_DATA_EXPORTER_EVENT_FIRST_SECTION
#define AMP_DATA_EXPORTER_EVENT_FIRST_SECTION (8)
#endif

#ifndef _define___AMP_DATA_EXPORTER_EVENT_ALL_MASK
#define _define___AMP_DATA_EXPORTER_EVENT_ALL_MASK
#define AMP_DATA_EXPORTER_EVENT_ALL_MASK (255)
#endif

#ifndef _define___AMP_DATA_EXPORTER_MODE_TELETEXT
#define _define___AMP_DATA_EXPORTER_MODE_TELETEXT
#define AMP_DATA_EXPORTER_MODE_TELETEXT (0)
#endif

#ifndef _define___AMP_DATA_EXPORTER_MODE_DVB_SUBTITLES
#define _define___AMP_DATA_EXPORTER_MODE_DVB_SUBTITLES
#define AMP_DATA_EXPORTER_MODE_DVB_SUBTITLES (1)
#endif

#ifndef _define___AMP_DATA_EXPORTER_MODE_PSI_SECTION
#define _define___AMP_DATA_EXPORTER_MODE_PSI_SECTION
#define AMP_DATA_EXPORTER_MODE_PSI_SECTION (2)
#endif

#ifndef _define___AMP_DATA_EXPORTER_MODE_MAX
#define _define___AMP_DATA_EXPORTER_MODE_MAX
#define AMP_DATA_EXPORTER_MODE_MAX (3)
#endif

#ifndef _typedef___AMP_DATA_EXPORTER_MODE
#define _typedef___AMP_DATA_EXPORTER_MODE
typedef CORBA_enum AMP_DATA_EXPORTER_MODE;
#endif /* _typedef___AMP_DATA_EXPORTER_MODE */

HRESULT AMP_DATA_EXPORTER_SetOutputMode(AMP_DATA_EXPORTER _obj, AMP_DATA_EXPORTER_MODE mode, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_GetOutputMode(AMP_DATA_EXPORTER _obj, AMP_DATA_EXPORTER_MODE *mode, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_Open(AMP_DATA_EXPORTER _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_Close(AMP_DATA_EXPORTER _obj, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_Destroy(AMP_DATA_EXPORTER _obj, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_SetState(AMP_DATA_EXPORTER _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_GetState(AMP_DATA_EXPORTER _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_QueryInfo(AMP_DATA_EXPORTER _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_SetupPort(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_DynamicSetupPort(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_DisconnectPorts(AMP_DATA_EXPORTER _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_ClearPort(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_QueryPort(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_ClearPortBuf(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_DebugDump(AMP_DATA_EXPORTER _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_RegisterNotify(AMP_DATA_EXPORTER _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_UnregisterNotify(AMP_DATA_EXPORTER _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_RegisterNotifyDetail(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_UnregisterNotifyDetail(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_RegisterBD(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_UnregisterBD(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DATA_EXPORTER_PushBD(AMP_DATA_EXPORTER _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_ISP
#define _typedef___AMP_ISP
typedef CORBA_Object AMP_ISP;
#endif /* _typedef___AMP_ISP */

#ifndef _typedef___AMP_ISP_HANDLE
#define _typedef___AMP_ISP_HANDLE
typedef UINT32 AMP_ISP_HANDLE;
#endif /* _typedef___AMP_ISP_HANDLE */

#ifndef _define___AMP_ISP_SENSOR_MAX
#define _define___AMP_ISP_SENSOR_MAX
#define AMP_ISP_SENSOR_MAX (2)
#endif

#ifndef _define___AMP_ISP_IN
#define _define___AMP_ISP_IN
#define AMP_ISP_IN (0)
#endif

#ifndef _define___AMP_ISP_IN_MAX
#define _define___AMP_ISP_IN_MAX
#define AMP_ISP_IN_MAX (1)
#endif

#ifndef _typedef___AMP_ISP_INPUT_PORT
#define _typedef___AMP_ISP_INPUT_PORT
typedef CORBA_enum AMP_ISP_INPUT_PORT;
#endif /* _typedef___AMP_ISP_INPUT_PORT */

#ifndef _define___AMP_ISP_OUT_MP
#define _define___AMP_ISP_OUT_MP
#define AMP_ISP_OUT_MP (0)
#endif

#ifndef _define___AMP_ISP_OUT_SP1
#define _define___AMP_ISP_OUT_SP1
#define AMP_ISP_OUT_SP1 (1)
#endif

#ifndef _define___AMP_ISP_OUT_SP2
#define _define___AMP_ISP_OUT_SP2
#define AMP_ISP_OUT_SP2 (2)
#endif

#ifndef _define___AMP_ISP_OUT_MAX
#define _define___AMP_ISP_OUT_MAX
#define AMP_ISP_OUT_MAX (3)
#endif

#ifndef _typedef___AMP_ISP_OUTPUT_PORT
#define _typedef___AMP_ISP_OUTPUT_PORT
typedef CORBA_enum AMP_ISP_OUTPUT_PORT;
#endif /* _typedef___AMP_ISP_OUTPUT_PORT */

#ifndef _define___AMP_ISP_MODE_INLINE
#define _define___AMP_ISP_MODE_INLINE
#define AMP_ISP_MODE_INLINE (0)
#endif

#ifndef _define___AMP_ISP_MODE_MCM_STREAM
#define _define___AMP_ISP_MODE_MCM_STREAM
#define AMP_ISP_MODE_MCM_STREAM (1)
#endif

#ifndef _define___AMP_ISP_MODE_MCM_ISP_STREAM
#define _define___AMP_ISP_MODE_MCM_ISP_STREAM
#define AMP_ISP_MODE_MCM_ISP_STREAM (2)
#endif

#ifndef _typedef___AMP_ISP_MODE
#define _typedef___AMP_ISP_MODE
typedef CORBA_enum AMP_ISP_MODE;
#endif /* _typedef___AMP_ISP_MODE */

#ifndef _define___AMP_ISP_FRAME_MODE_CAPTURE
#define _define___AMP_ISP_FRAME_MODE_CAPTURE
#define AMP_ISP_FRAME_MODE_CAPTURE (0)
#endif

#ifndef _define___AMP_ISP_FRAME_MODE_STREAMING
#define _define___AMP_ISP_FRAME_MODE_STREAMING
#define AMP_ISP_FRAME_MODE_STREAMING (1)
#endif

#ifndef _typedef___AMP_ISP_FRAME_MODE
#define _typedef___AMP_ISP_FRAME_MODE
typedef CORBA_enum AMP_ISP_FRAME_MODE;
#endif /* _typedef___AMP_ISP_FRAME_MODE */

#ifndef _define___AMP_ISP_PQ_CMD_AEC_START
#define _define___AMP_ISP_PQ_CMD_AEC_START
#define AMP_ISP_PQ_CMD_AEC_START (0)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_STOP
#define _define___AMP_ISP_PQ_CMD_AEC_STOP
#define AMP_ISP_PQ_CMD_AEC_STOP (1)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_RESET
#define _define___AMP_ISP_PQ_CMD_AEC_RESET
#define AMP_ISP_PQ_CMD_AEC_RESET (2)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_STATUS
#define _define___AMP_ISP_PQ_CMD_AEC_STATUS
#define AMP_ISP_PQ_CMD_AEC_STATUS (3)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_AEC_CONFIGURE
#define AMP_ISP_PQ_CMD_AEC_CONFIGURE (4)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_GET_HISTOGR
#define _define___AMP_ISP_PQ_CMD_AEC_GET_HISTOGR
#define AMP_ISP_PQ_CMD_AEC_GET_HISTOGR (5)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_GET_LUMA
#define _define___AMP_ISP_PQ_CMD_AEC_GET_LUMA
#define AMP_ISP_PQ_CMD_AEC_GET_LUMA (6)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEC_GET_OBJ_REG
#define _define___AMP_ISP_PQ_CMD_AEC_GET_OBJ_REG
#define AMP_ISP_PQ_CMD_AEC_GET_OBJ_REG (7)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AF_IS_AVAILABLE
#define _define___AMP_ISP_PQ_CMD_AF_IS_AVAILABLE
#define AMP_ISP_PQ_CMD_AF_IS_AVAILABLE (8)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AF_STATUS
#define _define___AMP_ISP_PQ_CMD_AF_STATUS
#define AMP_ISP_PQ_CMD_AF_STATUS (9)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AF_ONE_SHOT
#define _define___AMP_ISP_PQ_CMD_AF_ONE_SHOT
#define AMP_ISP_PQ_CMD_AF_ONE_SHOT (10)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AF_START
#define _define___AMP_ISP_PQ_CMD_AF_START
#define AMP_ISP_PQ_CMD_AF_START (11)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AF_STOP
#define _define___AMP_ISP_PQ_CMD_AF_STOP
#define AMP_ISP_PQ_CMD_AF_STOP (12)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWB_STATUS
#define _define___AMP_ISP_PQ_CMD_AWB_STATUS
#define AMP_ISP_PQ_CMD_AWB_STATUS (13)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWB_START
#define _define___AMP_ISP_PQ_CMD_AWB_START
#define AMP_ISP_PQ_CMD_AWB_START (14)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWB_STOP
#define _define___AMP_ISP_PQ_CMD_AWB_STOP
#define AMP_ISP_PQ_CMD_AWB_STOP (15)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWB_RESET
#define _define___AMP_ISP_PQ_CMD_AWB_RESET
#define AMP_ISP_PQ_CMD_AWB_RESET (16)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWB_GET_ILLU_PROF
#define _define___AMP_ISP_PQ_CMD_AWB_GET_ILLU_PROF
#define AMP_ISP_PQ_CMD_AWB_GET_ILLU_PROF (17)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_BLS_GET
#define _define___AMP_ISP_PQ_CMD_BLS_GET
#define AMP_ISP_PQ_CMD_BLS_GET (18)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_BLS_SET
#define _define___AMP_ISP_PQ_CMD_BLS_SET
#define AMP_ISP_PQ_CMD_BLS_SET (19)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CAC_STATUS
#define _define___AMP_ISP_PQ_CMD_CAC_STATUS
#define AMP_ISP_PQ_CMD_CAC_STATUS (20)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CAC_ENABLE
#define _define___AMP_ISP_PQ_CMD_CAC_ENABLE
#define AMP_ISP_PQ_CMD_CAC_ENABLE (21)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CAC_DISABLE
#define _define___AMP_ISP_PQ_CMD_CAC_DISABLE
#define AMP_ISP_PQ_CMD_CAC_DISABLE (22)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CPROC_STATUS
#define _define___AMP_ISP_PQ_CMD_CPROC_STATUS
#define AMP_ISP_PQ_CMD_CPROC_STATUS (23)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CPROC_SET
#define _define___AMP_ISP_PQ_CMD_CPROC_SET
#define AMP_ISP_PQ_CMD_CPROC_SET (24)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CPROC_ENABLE
#define _define___AMP_ISP_PQ_CMD_CPROC_ENABLE
#define AMP_ISP_PQ_CMD_CPROC_ENABLE (25)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CPROC_DISABLE
#define _define___AMP_ISP_PQ_CMD_CPROC_DISABLE
#define AMP_ISP_PQ_CMD_CPROC_DISABLE (26)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_CONFIG
#define _define___AMP_ISP_PQ_CMD_A2DNR_CONFIG
#define AMP_ISP_PQ_CMD_A2DNR_CONFIG (27)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_STATUS
#define _define___AMP_ISP_PQ_CMD_A2DNR_STATUS
#define AMP_ISP_PQ_CMD_A2DNR_STATUS (28)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_START
#define _define___AMP_ISP_PQ_CMD_A2DNR_START
#define AMP_ISP_PQ_CMD_A2DNR_START (29)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_STOP
#define _define___AMP_ISP_PQ_CMD_A2DNR_STOP
#define AMP_ISP_PQ_CMD_A2DNR_STOP (30)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_SET_AUTO_LEVEL
#define _define___AMP_ISP_PQ_CMD_A2DNR_SET_AUTO_LEVEL
#define AMP_ISP_PQ_CMD_A2DNR_SET_AUTO_LEVEL (31)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_GET_AUTO_LEVEL
#define _define___AMP_ISP_PQ_CMD_A2DNR_GET_AUTO_LEVEL
#define AMP_ISP_PQ_CMD_A2DNR_GET_AUTO_LEVEL (32)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_A2DNR_SET_AUTO_TABLE
#define _define___AMP_ISP_PQ_CMD_A2DNR_SET_AUTO_TABLE
#define AMP_ISP_PQ_CMD_A2DNR_SET_AUTO_TABLE (33)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_CONFIG
#define _define___AMP_ISP_PQ_CMD_3DNR_CONFIG
#define AMP_ISP_PQ_CMD_3DNR_CONFIG (34)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_STATUS
#define _define___AMP_ISP_PQ_CMD_3DNR_STATUS
#define AMP_ISP_PQ_CMD_3DNR_STATUS (35)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_ENABLE
#define _define___AMP_ISP_PQ_CMD_3DNR_ENABLE
#define AMP_ISP_PQ_CMD_3DNR_ENABLE (36)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_DISABLE
#define _define___AMP_ISP_PQ_CMD_3DNR_DISABLE
#define AMP_ISP_PQ_CMD_3DNR_DISABLE (37)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_SET_AUTO_LEVEL
#define _define___AMP_ISP_PQ_CMD_3DNR_SET_AUTO_LEVEL
#define AMP_ISP_PQ_CMD_3DNR_SET_AUTO_LEVEL (38)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_GET_AUTO_LEVEL
#define _define___AMP_ISP_PQ_CMD_3DNR_GET_AUTO_LEVEL
#define AMP_ISP_PQ_CMD_3DNR_GET_AUTO_LEVEL (39)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_3DNR_SET_AUTO_TABLE
#define _define___AMP_ISP_PQ_CMD_3DNR_SET_AUTO_TABLE
#define AMP_ISP_PQ_CMD_3DNR_SET_AUTO_TABLE (40)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPCC_STATUS
#define _define___AMP_ISP_PQ_CMD_ADPCC_STATUS
#define AMP_ISP_PQ_CMD_ADPCC_STATUS (41)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPCC_START
#define _define___AMP_ISP_PQ_CMD_ADPCC_START
#define AMP_ISP_PQ_CMD_ADPCC_START (42)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPCC_STOP
#define _define___AMP_ISP_PQ_CMD_ADPCC_STOP
#define AMP_ISP_PQ_CMD_ADPCC_STOP (43)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPCC_GET_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_ADPCC_GET_CONFIGURE
#define AMP_ISP_PQ_CMD_ADPCC_GET_CONFIGURE (44)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPCC_SET_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_ADPCC_SET_CONFIGURE
#define AMP_ISP_PQ_CMD_ADPCC_SET_CONFIGURE (45)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPF_STATUS
#define _define___AMP_ISP_PQ_CMD_ADPF_STATUS
#define AMP_ISP_PQ_CMD_ADPF_STATUS (46)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPF_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_ADPF_CONFIGURE
#define AMP_ISP_PQ_CMD_ADPF_CONFIGURE (47)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPF_START
#define _define___AMP_ISP_PQ_CMD_ADPF_START
#define AMP_ISP_PQ_CMD_ADPF_START (48)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ADPF_STOP
#define _define___AMP_ISP_PQ_CMD_ADPF_STOP
#define AMP_ISP_PQ_CMD_ADPF_STOP (49)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_ECM_SET
#define _define___AMP_ISP_PQ_CMD_ECM_SET
#define AMP_ISP_PQ_CMD_ECM_SET (50)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEE_STATUS
#define _define___AMP_ISP_PQ_CMD_AEE_STATUS
#define AMP_ISP_PQ_CMD_AEE_STATUS (51)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEE_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_AEE_CONFIGURE
#define AMP_ISP_PQ_CMD_AEE_CONFIGURE (52)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEE_START
#define _define___AMP_ISP_PQ_CMD_AEE_START
#define AMP_ISP_PQ_CMD_AEE_START (53)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEE_STOP
#define _define___AMP_ISP_PQ_CMD_AEE_STOP
#define AMP_ISP_PQ_CMD_AEE_STOP (54)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AEE_SET_AUTO_TABLE
#define _define___AMP_ISP_PQ_CMD_AEE_SET_AUTO_TABLE
#define AMP_ISP_PQ_CMD_AEE_SET_AUTO_TABLE (55)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_FILTER_STATUS
#define _define___AMP_ISP_PQ_CMD_FILTER_STATUS
#define AMP_ISP_PQ_CMD_FILTER_STATUS (56)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_FILTER_SET
#define _define___AMP_ISP_PQ_CMD_FILTER_SET
#define AMP_ISP_PQ_CMD_FILTER_SET (57)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_FILTER_ENABLE
#define _define___AMP_ISP_PQ_CMD_FILTER_ENABLE
#define AMP_ISP_PQ_CMD_FILTER_ENABLE (58)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_FILTER_DISABLE
#define _define___AMP_ISP_PQ_CMD_FILTER_DISABLE
#define AMP_ISP_PQ_CMD_FILTER_DISABLE (59)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CNR_STATUS
#define _define___AMP_ISP_PQ_CMD_CNR_STATUS
#define AMP_ISP_PQ_CMD_CNR_STATUS (60)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CNR_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_CNR_CONFIGURE
#define AMP_ISP_PQ_CMD_CNR_CONFIGURE (61)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CNR_ENABLE
#define _define___AMP_ISP_PQ_CMD_CNR_ENABLE
#define AMP_ISP_PQ_CMD_CNR_ENABLE (62)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_CNR_DISABLE
#define _define___AMP_ISP_PQ_CMD_CNR_DISABLE
#define AMP_ISP_PQ_CMD_CNR_DISABLE (63)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_IE_ENABLE
#define _define___AMP_ISP_PQ_CMD_IE_ENABLE
#define AMP_ISP_PQ_CMD_IE_ENABLE (64)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_IE_DISABLE
#define _define___AMP_ISP_PQ_CMD_IE_DISABLE
#define AMP_ISP_PQ_CMD_IE_DISABLE (65)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_JPE_ENABLE
#define _define___AMP_ISP_PQ_CMD_JPE_ENABLE
#define AMP_ISP_PQ_CMD_JPE_ENABLE (66)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_JPE_DISABLE
#define _define___AMP_ISP_PQ_CMD_JPE_DISABLE
#define AMP_ISP_PQ_CMD_JPE_DISABLE (67)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SET_ORIENTATION
#define _define___AMP_ISP_PQ_CMD_SET_ORIENTATION
#define AMP_ISP_PQ_CMD_SET_ORIENTATION (68)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SET_ACQ_RESOLUTION
#define _define___AMP_ISP_PQ_CMD_SET_ACQ_RESOLUTION
#define AMP_ISP_PQ_CMD_SET_ACQ_RESOLUTION (69)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SEARCH_AND_LOCK
#define _define___AMP_ISP_PQ_CMD_SEARCH_AND_LOCK
#define AMP_ISP_PQ_CMD_SEARCH_AND_LOCK (70)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_UNLOCK
#define _define___AMP_ISP_PQ_CMD_UNLOCK
#define AMP_ISP_PQ_CMD_UNLOCK (71)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SIMP_ENABLE
#define _define___AMP_ISP_PQ_CMD_SIMP_ENABLE
#define AMP_ISP_PQ_CMD_SIMP_ENABLE (72)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SIMP_DISABLE
#define _define___AMP_ISP_PQ_CMD_SIMP_DISABLE
#define AMP_ISP_PQ_CMD_SIMP_DISABLE (73)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WB_GET_CC_MATRIX
#define _define___AMP_ISP_PQ_CMD_WB_GET_CC_MATRIX
#define AMP_ISP_PQ_CMD_WB_GET_CC_MATRIX (74)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WB_GET_CC_OFFSET
#define _define___AMP_ISP_PQ_CMD_WB_GET_CC_OFFSET
#define AMP_ISP_PQ_CMD_WB_GET_CC_OFFSET (75)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WB_GET_GAINS
#define _define___AMP_ISP_PQ_CMD_WB_GET_GAINS
#define AMP_ISP_PQ_CMD_WB_GET_GAINS (76)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WB_SET_CC_MATRIX
#define _define___AMP_ISP_PQ_CMD_WB_SET_CC_MATRIX
#define AMP_ISP_PQ_CMD_WB_SET_CC_MATRIX (77)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WB_SET_CC_OFFSET
#define _define___AMP_ISP_PQ_CMD_WB_SET_CC_OFFSET
#define AMP_ISP_PQ_CMD_WB_SET_CC_OFFSET (78)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WB_SET_GAINS
#define _define___AMP_ISP_PQ_CMD_WB_SET_GAINS
#define AMP_ISP_PQ_CMD_WB_SET_GAINS (79)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_DEMOSAIC_GET
#define _define___AMP_ISP_PQ_CMD_DEMOSAIC_GET
#define AMP_ISP_PQ_CMD_DEMOSAIC_GET (80)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_DEMOSAIC_SET
#define _define___AMP_ISP_PQ_CMD_DEMOSAIC_SET
#define AMP_ISP_PQ_CMD_DEMOSAIC_SET (81)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR_SET_CURVE
#define _define___AMP_ISP_PQ_CMD_WDR_SET_CURVE
#define AMP_ISP_PQ_CMD_WDR_SET_CURVE (82)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR2_SET_STRENGTH
#define _define___AMP_ISP_PQ_CMD_WDR2_SET_STRENGTH
#define AMP_ISP_PQ_CMD_WDR2_SET_STRENGTH (83)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR3_STATUS
#define _define___AMP_ISP_PQ_CMD_WDR3_STATUS
#define AMP_ISP_PQ_CMD_WDR3_STATUS (84)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR3_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_WDR3_CONFIGURE
#define AMP_ISP_PQ_CMD_WDR3_CONFIGURE (85)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWDR3_START
#define _define___AMP_ISP_PQ_CMD_AWDR3_START
#define AMP_ISP_PQ_CMD_AWDR3_START (86)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWDR3_STOP
#define _define___AMP_ISP_PQ_CMD_AWDR3_STOP
#define AMP_ISP_PQ_CMD_AWDR3_STOP (87)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWDR3_SET_AUTO_LEVEL
#define _define___AMP_ISP_PQ_CMD_AWDR3_SET_AUTO_LEVEL
#define AMP_ISP_PQ_CMD_AWDR3_SET_AUTO_LEVEL (88)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWDR3_GET_AUTO_LEVEL
#define _define___AMP_ISP_PQ_CMD_AWDR3_GET_AUTO_LEVEL
#define AMP_ISP_PQ_CMD_AWDR3_GET_AUTO_LEVEL (89)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AWDR3_SET_AUTO_TABLE
#define _define___AMP_ISP_PQ_CMD_AWDR3_SET_AUTO_TABLE
#define AMP_ISP_PQ_CMD_AWDR3_SET_AUTO_TABLE (90)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR_ENABLE
#define _define___AMP_ISP_PQ_CMD_WDR_ENABLE
#define AMP_ISP_PQ_CMD_WDR_ENABLE (91)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR_DISABLE
#define _define___AMP_ISP_PQ_CMD_WDR_DISABLE
#define AMP_ISP_PQ_CMD_WDR_DISABLE (92)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR2_ENABLE
#define _define___AMP_ISP_PQ_CMD_WDR2_ENABLE
#define AMP_ISP_PQ_CMD_WDR2_ENABLE (93)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR2_DISABLE
#define _define___AMP_ISP_PQ_CMD_WDR2_DISABLE
#define AMP_ISP_PQ_CMD_WDR2_DISABLE (94)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR3_DISABLE
#define _define___AMP_ISP_PQ_CMD_WDR3_DISABLE
#define AMP_ISP_PQ_CMD_WDR3_DISABLE (95)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WDR3_ENABLE
#define _define___AMP_ISP_PQ_CMD_WDR3_ENABLE
#define AMP_ISP_PQ_CMD_WDR3_ENABLE (96)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AFLT_STATUS
#define _define___AMP_ISP_PQ_CMD_AFLT_STATUS
#define AMP_ISP_PQ_CMD_AFLT_STATUS (97)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AFLT_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_AFLT_CONFIGURE
#define AMP_ISP_PQ_CMD_AFLT_CONFIGURE (98)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AFLT_START
#define _define___AMP_ISP_PQ_CMD_AFLT_START
#define AMP_ISP_PQ_CMD_AFLT_START (99)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AFLT_STOP
#define _define___AMP_ISP_PQ_CMD_AFLT_STOP
#define AMP_ISP_PQ_CMD_AFLT_STOP (100)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_AFLT_SET_AUTO_TABLE
#define _define___AMP_ISP_PQ_CMD_AFLT_SET_AUTO_TABLE
#define AMP_ISP_PQ_CMD_AFLT_SET_AUTO_TABLE (101)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_GC_SET_CURVE
#define _define___AMP_ISP_PQ_CMD_GC_SET_CURVE
#define AMP_ISP_PQ_CMD_GC_SET_CURVE (102)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_GC_STATUS
#define _define___AMP_ISP_PQ_CMD_GC_STATUS
#define AMP_ISP_PQ_CMD_GC_STATUS (103)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_GC_ENABLE
#define _define___AMP_ISP_PQ_CMD_GC_ENABLE
#define AMP_ISP_PQ_CMD_GC_ENABLE (104)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_GC_DISABLE
#define _define___AMP_ISP_PQ_CMD_GC_DISABLE
#define AMP_ISP_PQ_CMD_GC_DISABLE (105)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_LSC_STATUS
#define _define___AMP_ISP_PQ_CMD_LSC_STATUS
#define AMP_ISP_PQ_CMD_LSC_STATUS (106)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_LSC_ENABLE
#define _define___AMP_ISP_PQ_CMD_LSC_ENABLE
#define AMP_ISP_PQ_CMD_LSC_ENABLE (107)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_LSC_DISABLE
#define _define___AMP_ISP_PQ_CMD_LSC_DISABLE
#define AMP_ISP_PQ_CMD_LSC_DISABLE (108)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_FILES_SET
#define _define___AMP_ISP_PQ_CMD_SENSOR_FILES_SET
#define AMP_ISP_PQ_CMD_SENSOR_FILES_SET (109)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_CAPS_GET
#define _define___AMP_ISP_PQ_CMD_SENSOR_CAPS_GET
#define AMP_ISP_PQ_CMD_SENSOR_CAPS_GET (110)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_RES_SET
#define _define___AMP_ISP_PQ_CMD_SENSOR_RES_SET
#define AMP_ISP_PQ_CMD_SENSOR_RES_SET (111)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_DUMP_REGS
#define _define___AMP_ISP_PQ_CMD_SENSOR_DUMP_REGS
#define AMP_ISP_PQ_CMD_SENSOR_DUMP_REGS (112)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_READ_REGS
#define _define___AMP_ISP_PQ_CMD_SENSOR_READ_REGS
#define AMP_ISP_PQ_CMD_SENSOR_READ_REGS (113)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_WRITE_REGS
#define _define___AMP_ISP_PQ_CMD_SENSOR_WRITE_REGS
#define AMP_ISP_PQ_CMD_SENSOR_WRITE_REGS (114)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_ACTIVATE_TEST
#define _define___AMP_ISP_PQ_CMD_SENSOR_ACTIVATE_TEST
#define AMP_ISP_PQ_CMD_SENSOR_ACTIVATE_TEST (115)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_GAIN
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_GAIN
#define AMP_ISP_PQ_CMD_SENSOR_GET_GAIN (116)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_INTEGR_TIME
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_INTEGR_TIME
#define AMP_ISP_PQ_CMD_SENSOR_GET_INTEGR_TIME (117)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_SET_GAIN
#define _define___AMP_ISP_PQ_CMD_SENSOR_SET_GAIN
#define AMP_ISP_PQ_CMD_SENSOR_SET_GAIN (118)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_SET_INTEGR_TIME
#define _define___AMP_ISP_PQ_CMD_SENSOR_SET_INTEGR_TIME
#define AMP_ISP_PQ_CMD_SENSOR_SET_INTEGR_TIME (119)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_GAIN_LIMITS
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_GAIN_LIMITS
#define AMP_ISP_PQ_CMD_SENSOR_GET_GAIN_LIMITS (120)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_GAIN_INCR
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_GAIN_INCR
#define AMP_ISP_PQ_CMD_SENSOR_GET_GAIN_INCR (121)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_INTERG_TIME_LIMITS
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_INTERG_TIME_LIMITS
#define AMP_ISP_PQ_CMD_SENSOR_GET_INTERG_TIME_LIMITS (122)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_INTERG_TIME_INCR
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_INTERG_TIME_INCR
#define AMP_ISP_PQ_CMD_SENSOR_GET_INTERG_TIME_INCR (123)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_CHECK_CONN
#define _define___AMP_ISP_PQ_CMD_SENSOR_CHECK_CONN
#define AMP_ISP_PQ_CMD_SENSOR_CHECK_CONN (124)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_GET_REVISION
#define _define___AMP_ISP_PQ_CMD_SENSOR_GET_REVISION
#define AMP_ISP_PQ_CMD_SENSOR_GET_REVISION (125)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SENSOR_SETUP
#define _define___AMP_ISP_PQ_CMD_SENSOR_SETUP
#define AMP_ISP_PQ_CMD_SENSOR_SETUP (126)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_READ_CORE_REGS
#define _define___AMP_ISP_PQ_CMD_READ_CORE_REGS
#define AMP_ISP_PQ_CMD_READ_CORE_REGS (127)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_WRITE_CORE_REGS
#define _define___AMP_ISP_PQ_CMD_WRITE_CORE_REGS
#define AMP_ISP_PQ_CMD_WRITE_CORE_REGS (128)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_HDR_START
#define _define___AMP_ISP_PQ_CMD_HDR_START
#define AMP_ISP_PQ_CMD_HDR_START (129)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_HDR_STOP
#define _define___AMP_ISP_PQ_CMD_HDR_STOP
#define AMP_ISP_PQ_CMD_HDR_STOP (130)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_HDR_CONFIGURE
#define _define___AMP_ISP_PQ_CMD_HDR_CONFIGURE
#define AMP_ISP_PQ_CMD_HDR_CONFIGURE (131)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_HDR_STATUS
#define _define___AMP_ISP_PQ_CMD_HDR_STATUS
#define AMP_ISP_PQ_CMD_HDR_STATUS (132)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_SET_COLOR_CONVERSION_RANGE
#define _define___AMP_ISP_PQ_CMD_SET_COLOR_CONVERSION_RANGE
#define AMP_ISP_PQ_CMD_SET_COLOR_CONVERSION_RANGE (133)
#endif

#ifndef _define___AMP_ISP_PQ_CMD_MAX
#define _define___AMP_ISP_PQ_CMD_MAX
#define AMP_ISP_PQ_CMD_MAX (134)
#endif

#ifndef _typedef___AMP_ISP_PQ_COMMAND
#define _typedef___AMP_ISP_PQ_COMMAND
typedef CORBA_enum AMP_ISP_PQ_COMMAND;
#endif /* _typedef___AMP_ISP_PQ_COMMAND */

#ifndef _typedef___AMP_ISP_SENSOR_CONFIG
#define _typedef___AMP_ISP_SENSOR_CONFIG
typedef struct AMP_ISP_SENSOR_CONFIG {
  UINT16 uiWidth;
  UINT16 uiHeight;
  UINT32 uiDphy;
} AMP_ISP_SENSOR_CONFIG;
#endif /* _typedef___AMP_ISP_SENSOR_CONFIG */

HRESULT AMP_ISP_SetInputSource(AMP_ISP _obj, AMP_ISP_INPUT_SOURCE eInSource, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetMode(AMP_ISP _obj, AMP_ISP_MODE eIspMode, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetInputFormat(AMP_ISP _obj, AMP_ISP_INPUT_PORT ePort, AMP_DISP_SRCFMT eFmtType, AMP_DISP_ORDER eOrder, UINT16 uWidth, UINT16 uHeight, UINT8 uDepth, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetOutputFormat(AMP_ISP _obj, AMP_ISP_OUTPUT_PORT ePort, AMP_DISP_SRCFMT eFmtType, AMP_DISP_ORDER eOrder, UINT16 uWidth, UINT16 uHeight, UINT8 uDepth, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetFrameMode(AMP_ISP _obj, AMP_ISP_FRAME_MODE eFrmMode, UINT32 uFrmCount, CORBA_Environment *_ev);

HRESULT AMP_ISP_SelectSensor(AMP_ISP _obj, UINT8 index, CORBA_char *sensorName, CORBA_char *sensorCalibFile, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetSensor(AMP_ISP _obj, UINT8 index, struct AMP_ISP_SENSOR_CONFIG *pSensorCfg, CORBA_Environment *_ev);

HRESULT AMP_ISP_GetFrameSize(AMP_ISP _obj, AMP_DISP_SRCFMT eFmtType, AMP_DISP_ORDER eOrder, UINT16 uWidth, UINT16 uHeight, UINT8 uDepth, UINT32 *uYSize, UINT32 *uUVSize, CORBA_Environment *_ev);

HRESULT AMP_ISP_PQControl(AMP_ISP _obj, AMP_ISP_PQ_COMMAND eCmd, UINT32 hShm, CORBA_Environment *_ev);

HRESULT AMP_ISP_Open(AMP_ISP _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_ISP_Close(AMP_ISP _obj, CORBA_Environment *_ev);

HRESULT AMP_ISP_Destroy(AMP_ISP _obj, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetState(AMP_ISP _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_ISP_GetState(AMP_ISP _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_ISP_QueryInfo(AMP_ISP _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_ISP_SetupPort(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_ISP_DynamicSetupPort(AMP_ISP _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ISP_DisconnectPorts(AMP_ISP _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ISP_ClearPort(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ISP_QueryPort(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_ISP_ClearPortBuf(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ISP_DebugDump(AMP_ISP _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_ISP_RegisterNotify(AMP_ISP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ISP_UnregisterNotify(AMP_ISP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ISP_RegisterNotifyDetail(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ISP_UnregisterNotifyDetail(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ISP_RegisterBD(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_ISP_UnregisterBD(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_ISP_PushBD(AMP_ISP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DEWARP
#define _typedef___AMP_DEWARP
typedef CORBA_Object AMP_DEWARP;
#endif /* _typedef___AMP_DEWARP */

#ifndef _typedef___AMP_DEWARP_HANDLE
#define _typedef___AMP_DEWARP_HANDLE
typedef UINT32 AMP_DEWARP_HANDLE;
#endif /* _typedef___AMP_DEWARP_HANDLE */

#ifndef _define___AMP_DEWARP_IN
#define _define___AMP_DEWARP_IN
#define AMP_DEWARP_IN (0)
#endif

#ifndef _define___AMP_DEWARP_IN_MAX
#define _define___AMP_DEWARP_IN_MAX
#define AMP_DEWARP_IN_MAX (1)
#endif

#ifndef _typedef___AMP_DEWARP_INPUT_PORT
#define _typedef___AMP_DEWARP_INPUT_PORT
typedef CORBA_enum AMP_DEWARP_INPUT_PORT;
#endif /* _typedef___AMP_DEWARP_INPUT_PORT */

#ifndef _define___AMP_DEWARP_OUT
#define _define___AMP_DEWARP_OUT
#define AMP_DEWARP_OUT (0)
#endif

#ifndef _define___AMP_DEWARP_OUT_MAX
#define _define___AMP_DEWARP_OUT_MAX
#define AMP_DEWARP_OUT_MAX (1)
#endif

#ifndef _typedef___AMP_DEWARP_OUTPUT_PORT
#define _typedef___AMP_DEWARP_OUTPUT_PORT
typedef CORBA_enum AMP_DEWARP_OUTPUT_PORT;
#endif /* _typedef___AMP_DEWARP_OUTPUT_PORT */

HRESULT AMP_DEWARP_SetInputFormat(AMP_DEWARP _obj, UINT32 uiWidth, UINT32 uiHeight, UINT8 uiDepth, AMP_DISP_SRCFMT eFmtType, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_SetOutputFormat(AMP_DEWARP _obj, UINT32 uiWidth, UINT32 uiHeight, UINT8 uiDepth, AMP_DISP_SRCFMT eFmtType, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_GetOutputFrameSize(AMP_DEWARP _obj, UINT32 uiInWidth, UINT32 uiInHeight, UINT8 uiInDepth, AMP_DISP_SRCFMT eInFmtType, UINT32 uiOutWidth, UINT32 uiOutHeight, UINT8 uiOutDepth, AMP_DISP_SRCFMT eOutFmtType, UINT32 *uiYSize, UINT32 *uiUVSize, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_ReduceFrameRate(AMP_DEWARP _obj, UINT32 uiDivisor, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_Open(AMP_DEWARP _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_Close(AMP_DEWARP _obj, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_Destroy(AMP_DEWARP _obj, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_SetState(AMP_DEWARP _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_GetState(AMP_DEWARP _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_QueryInfo(AMP_DEWARP _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_SetupPort(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_DynamicSetupPort(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_DisconnectPorts(AMP_DEWARP _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_ClearPort(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_QueryPort(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_ClearPortBuf(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_DebugDump(AMP_DEWARP _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_RegisterNotify(AMP_DEWARP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_UnregisterNotify(AMP_DEWARP _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_RegisterNotifyDetail(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_UnregisterNotifyDetail(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_RegisterBD(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_UnregisterBD(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DEWARP_PushBD(AMP_DEWARP _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_ROTATE
#define _typedef___AMP_ROTATE
typedef CORBA_Object AMP_ROTATE;
#endif /* _typedef___AMP_ROTATE */

#ifndef _typedef___AMP_ROTATE_HANDLE
#define _typedef___AMP_ROTATE_HANDLE
typedef UINT32 AMP_ROTATE_HANDLE;
#endif /* _typedef___AMP_ROTATE_HANDLE */

#ifndef _define___AMP_ROTATE_IN
#define _define___AMP_ROTATE_IN
#define AMP_ROTATE_IN (0)
#endif

#ifndef _define___AMP_ROTATE_IN_MAX
#define _define___AMP_ROTATE_IN_MAX
#define AMP_ROTATE_IN_MAX (1)
#endif

#ifndef _typedef___AMP_ROTATE_INPUT_PORT
#define _typedef___AMP_ROTATE_INPUT_PORT
typedef CORBA_enum AMP_ROTATE_INPUT_PORT;
#endif /* _typedef___AMP_ROTATE_INPUT_PORT */

#ifndef _define___AMP_ROTATE_OUT
#define _define___AMP_ROTATE_OUT
#define AMP_ROTATE_OUT (0)
#endif

#ifndef _define___AMP_ROTATE_OUT_MAX
#define _define___AMP_ROTATE_OUT_MAX
#define AMP_ROTATE_OUT_MAX (1)
#endif

#ifndef _typedef___AMP_ROTATE_OUTPUT_PORT
#define _typedef___AMP_ROTATE_OUTPUT_PORT
typedef CORBA_enum AMP_ROTATE_OUTPUT_PORT;
#endif /* _typedef___AMP_ROTATE_OUTPUT_PORT */

#ifndef _define___AMP_ROTATE_ANGLE_0
#define _define___AMP_ROTATE_ANGLE_0
#define AMP_ROTATE_ANGLE_0 (0)
#endif

#ifndef _define___AMP_ROTATE_ANGLE_90
#define _define___AMP_ROTATE_ANGLE_90
#define AMP_ROTATE_ANGLE_90 (1)
#endif

#ifndef _define___AMP_ROTATE_ANGLE_180
#define _define___AMP_ROTATE_ANGLE_180
#define AMP_ROTATE_ANGLE_180 (2)
#endif

#ifndef _define___AMP_ROTATE_ANGLE_270
#define _define___AMP_ROTATE_ANGLE_270
#define AMP_ROTATE_ANGLE_270 (3)
#endif

#ifndef _define___AMP_ROTATE_H_FLIP
#define _define___AMP_ROTATE_H_FLIP
#define AMP_ROTATE_H_FLIP (4)
#endif

#ifndef _define___AMP_ROTATE_V_FLIP
#define _define___AMP_ROTATE_V_FLIP
#define AMP_ROTATE_V_FLIP (5)
#endif

#ifndef _define___AMP_ROTATE_ANGLE_MAX
#define _define___AMP_ROTATE_ANGLE_MAX
#define AMP_ROTATE_ANGLE_MAX (6)
#endif

#ifndef _typedef___AMP_ROTATE_OPERATION
#define _typedef___AMP_ROTATE_OPERATION
typedef CORBA_enum AMP_ROTATE_OPERATION;
#endif /* _typedef___AMP_ROTATE_OPERATION */

HRESULT AMP_ROTATE_SetOutputFormat(AMP_ROTATE _obj, AMP_ROTATE_OPERATION eAngle, UINT32 uiWidth, UINT32 uiHeight, UINT8 uDepth, AMP_DISP_SRCFMT eFmtType, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_ReduceFrameRate(AMP_ROTATE _obj, UINT32 uiDivisor, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_GetFrameSize(AMP_ROTATE _obj, UINT32 uiWidth, UINT32 uiHeight, UINT8 uiDepth, AMP_DISP_SRCFMT eFmtType, UINT32 *uiYSize, UINT32 *uiUVSize, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_Open(AMP_ROTATE _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_Close(AMP_ROTATE _obj, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_Destroy(AMP_ROTATE _obj, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_SetState(AMP_ROTATE _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_GetState(AMP_ROTATE _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_QueryInfo(AMP_ROTATE _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_SetupPort(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_DynamicSetupPort(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_DisconnectPorts(AMP_ROTATE _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_ClearPort(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_QueryPort(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_ClearPortBuf(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_DebugDump(AMP_ROTATE _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_RegisterNotify(AMP_ROTATE _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_UnregisterNotify(AMP_ROTATE _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_RegisterNotifyDetail(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_UnregisterNotifyDetail(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_RegisterBD(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_UnregisterBD(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_ROTATE_PushBD(AMP_ROTATE _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_DNS
#define _typedef___AMP_DNS
typedef CORBA_Object AMP_DNS;
#endif /* _typedef___AMP_DNS */

HRESULT AMP_DNS_SetOutputFormat(AMP_DNS _obj, UINT32 uiWidth, UINT32 uiHeight, UINT32 uiDepth, AMP_DISP_SRCFMT eFmtType, CORBA_Environment *_ev);

HRESULT AMP_DNS_SetSensorType(AMP_DNS _obj, AMP_ISP_INPUT_SOURCE eInSource, CORBA_Environment *_ev);

HRESULT AMP_DNS_GetOutputResolution(AMP_DNS _obj, UINT32 *uiWidth, UINT32 *uiHeight, CORBA_Environment *_ev);

HRESULT AMP_DNS_ReduceFrameRate(AMP_DNS _obj, UINT32 uiDivisor, CORBA_Environment *_ev);

HRESULT AMP_DNS_GetFrameSize(AMP_DNS _obj, UINT32 uiWidth, UINT32 uiHeight, UINT8 uiDepth, AMP_DISP_SRCFMT eFmtType, UINT32 *uiYSize, UINT32 *uiUVSize, CORBA_Environment *_ev);

HRESULT AMP_DNS_Open(AMP_DNS _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_DNS_Close(AMP_DNS _obj, CORBA_Environment *_ev);

HRESULT AMP_DNS_Destroy(AMP_DNS _obj, CORBA_Environment *_ev);

HRESULT AMP_DNS_SetState(AMP_DNS _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_DNS_GetState(AMP_DNS _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_DNS_QueryInfo(AMP_DNS _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_DNS_SetupPort(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_DNS_DynamicSetupPort(AMP_DNS _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DNS_DisconnectPorts(AMP_DNS _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DNS_ClearPort(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DNS_QueryPort(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_DNS_ClearPortBuf(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_DNS_DebugDump(AMP_DNS _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_DNS_RegisterNotify(AMP_DNS _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DNS_UnregisterNotify(AMP_DNS _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DNS_RegisterNotifyDetail(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DNS_UnregisterNotifyDetail(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_DNS_RegisterBD(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DNS_UnregisterBD(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_DNS_PushBD(AMP_DNS _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_PIPELINE
#define _typedef___AMP_PIPELINE
typedef CORBA_Object AMP_PIPELINE;
#endif /* _typedef___AMP_PIPELINE */

#ifndef _typedef___AMP_DISP_3D_Structure
#define _typedef___AMP_DISP_3D_Structure
typedef INT32 AMP_DISP_3D_Structure;
#endif /* _typedef___AMP_DISP_3D_Structure */

#ifndef _define___AMP_DISP_3D_Structure_FRAME_PACKING
#define _define___AMP_DISP_3D_Structure_FRAME_PACKING
#define AMP_DISP_3D_Structure_FRAME_PACKING (0)
#endif

#ifndef _define___AMP_DISP_3D_Structure_FIELD_ALTERNATIVE
#define _define___AMP_DISP_3D_Structure_FIELD_ALTERNATIVE
#define AMP_DISP_3D_Structure_FIELD_ALTERNATIVE (1)
#endif

#ifndef _define___AMP_DISP_3D_Structure_LINE_ALTERNATIVE
#define _define___AMP_DISP_3D_Structure_LINE_ALTERNATIVE
#define AMP_DISP_3D_Structure_LINE_ALTERNATIVE (2)
#endif

#ifndef _define___AMP_DISP_3D_Structure_SIDE_BY_SIDE_FULL
#define _define___AMP_DISP_3D_Structure_SIDE_BY_SIDE_FULL
#define AMP_DISP_3D_Structure_SIDE_BY_SIDE_FULL (3)
#endif

#ifndef _define___AMP_DISP_3D_Structure_L_DEPTH
#define _define___AMP_DISP_3D_Structure_L_DEPTH
#define AMP_DISP_3D_Structure_L_DEPTH (4)
#endif

#ifndef _define___AMP_DISP_3D_Structure_L_DEPTH_GFX_GDEPTH
#define _define___AMP_DISP_3D_Structure_L_DEPTH_GFX_GDEPTH
#define AMP_DISP_3D_Structure_L_DEPTH_GFX_GDEPTH (5)
#endif

#ifndef _define___AMP_DISP_3D_Structure_TOP_AND_BOTTOM
#define _define___AMP_DISP_3D_Structure_TOP_AND_BOTTOM
#define AMP_DISP_3D_Structure_TOP_AND_BOTTOM (6)
#endif

#ifndef _define___AMP_DISP_3D_Structure_RESERVED2
#define _define___AMP_DISP_3D_Structure_RESERVED2
#define AMP_DISP_3D_Structure_RESERVED2 (7)
#endif

#ifndef _define___AMP_DISP_3D_Structure_SIDE_BY_SIDE_HALF
#define _define___AMP_DISP_3D_Structure_SIDE_BY_SIDE_HALF
#define AMP_DISP_3D_Structure_SIDE_BY_SIDE_HALF (8)
#endif

#ifndef _define___AMP_DISP_3D_Structure_RESERVED3
#define _define___AMP_DISP_3D_Structure_RESERVED3
#define AMP_DISP_3D_Structure_RESERVED3 (9)
#endif

#ifndef _typedef___AMP_DISP_DDD_Format
#define _typedef___AMP_DISP_DDD_Format
typedef INT32 AMP_DISP_DDD_Format;
#endif /* _typedef___AMP_DISP_DDD_Format */

#ifndef _define___AMP_DISP_DDD_Format_BYPASS
#define _define___AMP_DISP_DDD_Format_BYPASS
#define AMP_DISP_DDD_Format_BYPASS (0)
#endif

#ifndef _define___AMP_DISP_DDD_Format_SOFT_BYPASS
#define _define___AMP_DISP_DDD_Format_SOFT_BYPASS
#define AMP_DISP_DDD_Format_SOFT_BYPASS (1)
#endif

#ifndef _define___AMP_DISP_DDD_Format_SBS
#define _define___AMP_DISP_DDD_Format_SBS
#define AMP_DISP_DDD_Format_SBS (2)
#endif

#ifndef _define___AMP_DISP_DDD_Format_LI_LA
#define _define___AMP_DISP_DDD_Format_LI_LA
#define AMP_DISP_DDD_Format_LI_LA (3)
#endif

#ifndef _define___AMP_DISP_DDD_Format_FS_FP
#define _define___AMP_DISP_DDD_Format_FS_FP
#define AMP_DISP_DDD_Format_FS_FP (4)
#endif

#ifndef _define___AMP_DISP_DDD_Format_TB
#define _define___AMP_DISP_DDD_Format_TB
#define AMP_DISP_DDD_Format_TB (5)
#endif

#ifndef _typedef___AMP_DISP
#define _typedef___AMP_DISP
typedef CORBA_Object AMP_DISP;
#endif /* _typedef___AMP_DISP */

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0 (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0 (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0 (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0 (3)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0 (4)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0 (5)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0 (6)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0 (7)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0 (8)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0 (9)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0 (10)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0 (11)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0 (12)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0 (13)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0 (14)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0 (15)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0 (16)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0 (17)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0 (18)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0 (19)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0 (20)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0 (21)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0 (22)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0 (23)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0 (24)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0 (25)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0 (26)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0 (27)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0 (28)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0 (29)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0 (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0 (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0 (32)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0 (33)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0 (34)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0 (35)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0 (36)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0 (37)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0 (38)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0 (39)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0 (40)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0 (41)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0 (42)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0 (43)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0 (44)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0 (45)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0 (46)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0 (47)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0 (48)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0 (49)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0 (50)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0 (51)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0 (52)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0 (53)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0 (54)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0 (55)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1 (2097152)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1 (2097153)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1 (2097154)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1 (2097155)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1 (2097156)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1 (2097157)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1 (2097158)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1 (2097159)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1 (2097160)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1 (2097161)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1 (2097162)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1 (2097163)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1 (2097164)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1 (2097165)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1 (2097166)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1 (2097167)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1 (2097168)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1 (2097169)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1 (2097170)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1 (2097171)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1 (2097172)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1 (2097173)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1 (2097174)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1 (2097175)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1 (2097176)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1 (2097177)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1 (2097178)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1 (2097179)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1 (2097180)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1 (2097181)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1 (2097182)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1 (2097183)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1 (2097184)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1 (2097185)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1 (2097186)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1 (2097187)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1 (2097188)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1 (2097189)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1 (2097190)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1 (2097191)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1 (2097192)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1 (2097193)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1 (2097194)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1 (2097195)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1 (2097196)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1 (2097197)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1 (2097198)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1 (2097199)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1 (2097200)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1 (2097201)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1 (2097202)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1 (2097203)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1 (2097204)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1 (2097205)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1 (2097206)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1 (2097207)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0 (16777216)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0 (16777217)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0 (16777218)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0 (16777219)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0 (16777220)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0 (16777221)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0 (16777222)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0 (16777223)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0 (16777224)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0 (16777225)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0 (16777226)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0 (16777227)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0 (16777228)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0 (16777229)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0 (16777230)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0 (16777231)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0 (33554432)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0 (33554433)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0 (33554434)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0 (33554435)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0 (33554436)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0 (33554437)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0 (33554438)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0 (33554439)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0 (33554440)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0 (33554441)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0 (33554442)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0 (33554443)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0 (33554444)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0 (33554445)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0 (33554446)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0 (33554447)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0 (33554448)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1 (35651584)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1 (35651585)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1 (35651586)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1 (35651587)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1 (35651588)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1 (35651589)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1 (35651590)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1 (35651591)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1 (35651592)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1 (35651593)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1 (35651594)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1 (35651595)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1 (35651596)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1 (35651597)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1 (35651598)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1 (35651599)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1 (35651600)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0 (50331648)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0 (50331649)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0 (50331650)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0 (50331651)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0 (50331652)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0 (50331653)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0 (50331654)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0 (50331655)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0 (50331656)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0 (50331657)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0 (50331658)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0 (50331659)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0 (50331660)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0 (50331661)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0 (50331662)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0 (50331663)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0 (50331664)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0 (50331665)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0 (67108864)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0 (67108865)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0 (67108866)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0 (67108867)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0 (67108868)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0 (67108869)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0 (67108870)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0 (67108871)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0 (67108872)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0 (67108873)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0 (67108874)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0 (67108875)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0 (67108876)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0 (67108877)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0 (83886080)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0 (83886081)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0 (83886082)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0 (100663296)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0 (117440512)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0 (117440513)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0 (117440514)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0 (117440515)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0 (117440516)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0 (117440517)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0 (117440518)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0 (117440519)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0 (134217728)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0 (134217729)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0 (134217730)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0 (134217731)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0 (134217732)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0 (134217733)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0 (134217734)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0 (134217735)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0 (134217736)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0 (134217737)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0 (134217738)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0 (134217739)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0 (134217740)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0 (150994944)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0 (150994945)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0 (150994946)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0 (150994947)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0 (150994948)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0 (150994949)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0 (150994950)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0 (150994951)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0 (150994952)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0 (150994953)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0 (150994954)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0 (167772160)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0 (167772161)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0 (167772162)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0 (167772163)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1 (169869312)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1 (169869313)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1 (169869314)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1 (169869315)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0
#define AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0 (184552448)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0 (201326592)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0 (201326593)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0 (201326594)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0 (201326595)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0 (201326596)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0 (201326597)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1_MIN (416)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1_MIN
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1_MIN
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0_MIN
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0_MIN
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0_MIN (0)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_0_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_0_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_ENABLE_1_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX1_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX2_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX3_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX4_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX5_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX6_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX7_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX8_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX9_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX10_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX11_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX12_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX13_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_AX14_1_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H1_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H2_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H3_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H4_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H5_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H6_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H7_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H8_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H9_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H10_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H11_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H12_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H13_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_H14_1_MAX (30)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S1_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S2_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S3_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S4_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S5_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S6_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S7_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S8_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S9_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S10_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S11_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S12_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S13_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_S14_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG1_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG2_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG3_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG4_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG5_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG6_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG7_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG8_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG9_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG10_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG11_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG12_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ICR_HS_M_BRG13_1_MAX (608)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_GAIN_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_GAIN_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_LTI_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_CTI_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HF_MODE_0_MAX (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VF_MODE_0_MAX (3)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE1_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_NOISE2_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN1_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_GAIN2_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_VLPF_LEVEL_0_MAX (127)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_EE_DET_HLPF_LEVEL_0_MAX (127)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_0_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_0_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_0_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_0_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_0_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_0_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_0_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_ENABLE_1_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG1_1_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG2_1_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_WFG3_1_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS1_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS2_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_MS3_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH0_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH1_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH2_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH3_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH4_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_TH5_1_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP0_1_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_OP5_1_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB2_1_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_ACE_GB3_1_MAX (1023)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_MODE_0_MAX (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_YMODE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CMODE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_VNR_CHROMA_MODE_0_MAX (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YMODE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CMODE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_MODE_0_MAX (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_ADAPT_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_ADAPT_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_YTH_0_MAX (63)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_SNR_CTH_0_MAX (63)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YMODE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CMODE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_ADAPT_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_ADAPT_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_YBETA_0_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_VNR_TNR_CBETA_0_MAX (31)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_CAR_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_TH_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_STR_0_MAX (16)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_MNR_2D_FILTER_K_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_NSBD_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_MODE_0_MAX (3)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_TH_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_TH_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_H_FILTER_K_0_MAX (3)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_1D_V_FILTER_K_0_MAX (3)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_TH_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_CAR_BNR_2D_FILTER_K_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_STRENGTH_0_MAX (4)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_QTC_DETECTION_REFINEMENT_0_MAX (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_BR_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_ENABLE_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_LOW_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_INNER_HUE_LIMIT_HIGH_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_LOW_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_OUTER_HUE_LIMIT_HIGH_0_MAX (360)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_LOW_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_MID_0_MAX (4095)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_FTDC_CHROMA_LIMIT_HIGH_0_MAX (4095)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_MODE_0_MAX (2)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VECTOR_INTERPOLATION_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_MOTION_DETECTION_MODE_0_MAX (5)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_LTH_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_VIDEO_MOTION_DETECTION_HTH_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE22_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2224_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE2332_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE32322_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE55_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE64_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_DEINT_CADENCE87_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_X_0_MAX (1920)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_Y_0_MAX (1080)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_W_0_MAX (1920)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_SRC_H_0_MAX (1080)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_X_0_MAX (1920)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_Y_0_MAX (1080)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_W_0_MAX (1920)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_DEST_H_0_MAX (1080)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_EN_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_W_0_MAX (255)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_SCALE_NONLIEAR_CENTRAL_RATIO_0_MAX (64)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_0_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_0_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_0_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_0_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_BRIGHTNESS_1_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_CONTRAST_1_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_SATURATION_1_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1_MAX
#define _define___AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1_MAX
#define AMP_DISP_PQ_PARA_ID_VPP_PIC_TINT_1_MAX (1000)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_GAMMA_BYPASS_0_MAX (1)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_R_GAIN_0_MAX (16383)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_G_GAIN_0_MAX (16383)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_B_GAIN_0_MAX (16383)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_R_OFFSET_0_MAX (8191)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_G_OFFSET_0_MAX (8191)
#endif

#ifndef _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0_MAX
#define _define___AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0_MAX
#define AMP_DISP_PQ_PARA_ID_LVDS_WB_B_OFFSET_0_MAX (8191)
#endif

#ifndef _define___AMP_DISP_COEFF_H_PSHARP
#define _define___AMP_DISP_COEFF_H_PSHARP
#define AMP_DISP_COEFF_H_PSHARP (4)
#endif

#ifndef _define___AMP_DISP_COEFF_V_5TAPS
#define _define___AMP_DISP_COEFF_V_5TAPS
#define AMP_DISP_COEFF_V_5TAPS (31)
#endif

#ifndef _define___AMP_DISP_SCL_UNIT_MAIN
#define _define___AMP_DISP_SCL_UNIT_MAIN
#define AMP_DISP_SCL_UNIT_MAIN (0)
#endif

#ifndef _define___AMP_DISP_SCL_UNIT_PIP
#define _define___AMP_DISP_SCL_UNIT_PIP
#define AMP_DISP_SCL_UNIT_PIP (1)
#endif

#ifndef _define___AMP_DISP_WATERMARK_INIT_CMD
#define _define___AMP_DISP_WATERMARK_INIT_CMD
#define AMP_DISP_WATERMARK_INIT_CMD (0)
#endif

#ifndef _define___AMP_DISP_WATERMARK_OPEN_CMD
#define _define___AMP_DISP_WATERMARK_OPEN_CMD
#define AMP_DISP_WATERMARK_OPEN_CMD (1)
#endif

#ifndef _define___AMP_DISP_WATERMARK_UPDATE_CMD
#define _define___AMP_DISP_WATERMARK_UPDATE_CMD
#define AMP_DISP_WATERMARK_UPDATE_CMD (2)
#endif

#ifndef _define___AMP_DISP_WATERMARK_CLOSE_CMD
#define _define___AMP_DISP_WATERMARK_CLOSE_CMD
#define AMP_DISP_WATERMARK_CLOSE_CMD (3)
#endif

#ifndef _define___AMP_DISP_WATERMARK_DEINIT_CMD
#define _define___AMP_DISP_WATERMARK_DEINIT_CMD
#define AMP_DISP_WATERMARK_DEINIT_CMD (4)
#endif

#ifndef _typedef___AMP_DISP_PLANE_ID
#define _typedef___AMP_DISP_PLANE_ID
typedef INT32 AMP_DISP_PLANE_ID;
#endif /* _typedef___AMP_DISP_PLANE_ID */

#ifndef _define___AMP_DISP_PLANE_FIRST
#define _define___AMP_DISP_PLANE_FIRST
#define AMP_DISP_PLANE_FIRST (0)
#endif

#ifndef _define___AMP_DISP_PLANE_MAIN
#define _define___AMP_DISP_PLANE_MAIN
#define AMP_DISP_PLANE_MAIN (0)
#endif

#ifndef _define___AMP_DISP_PLANE_PIP
#define _define___AMP_DISP_PLANE_PIP
#define AMP_DISP_PLANE_PIP (1)
#endif

#ifndef _define___AMP_DISP_PLANE_GFX0
#define _define___AMP_DISP_PLANE_GFX0
#define AMP_DISP_PLANE_GFX0 (2)
#endif

#ifndef _define___AMP_DISP_PLANE_GFX1
#define _define___AMP_DISP_PLANE_GFX1
#define AMP_DISP_PLANE_GFX1 (3)
#endif

#ifndef _define___AMP_DISP_PLANE_GFX2
#define _define___AMP_DISP_PLANE_GFX2
#define AMP_DISP_PLANE_GFX2 (4)
#endif

#ifndef _define___AMP_DISP_PLANE_PG
#define _define___AMP_DISP_PLANE_PG
#define AMP_DISP_PLANE_PG (5)
#endif

#ifndef _define___AMP_DISP_PLANE_BG
#define _define___AMP_DISP_PLANE_BG
#define AMP_DISP_PLANE_BG (6)
#endif

#ifndef _define___AMP_DISP_PLANE_AUX
#define _define___AMP_DISP_PLANE_AUX
#define AMP_DISP_PLANE_AUX (7)
#endif

#ifndef _define___AMP_DISP_PLANE_4K_BYPASS
#define _define___AMP_DISP_PLANE_4K_BYPASS
#define AMP_DISP_PLANE_4K_BYPASS (8)
#endif

#ifndef _define___AMP_DISP_PLANE_4K_SCL
#define _define___AMP_DISP_PLANE_4K_SCL
#define AMP_DISP_PLANE_4K_SCL (9)
#endif

#ifndef _define___AMP_DISP_PLANE_MAX
#define _define___AMP_DISP_PLANE_MAX
#define AMP_DISP_PLANE_MAX (10)
#endif

#ifndef _typedef___AMP_DISP_TG_ID
#define _typedef___AMP_DISP_TG_ID
typedef INT32 AMP_DISP_TG_ID;
#endif /* _typedef___AMP_DISP_TG_ID */

#ifndef _define___AMP_DISP_TG_FIRST
#define _define___AMP_DISP_TG_FIRST
#define AMP_DISP_TG_FIRST (0)
#endif

#ifndef _define___AMP_DISP_TG_PRI
#define _define___AMP_DISP_TG_PRI
#define AMP_DISP_TG_PRI (0)
#endif

#ifndef _define___AMP_DISP_TG_SEC
#define _define___AMP_DISP_TG_SEC
#define AMP_DISP_TG_SEC (1)
#endif

#ifndef _define___AMP_DISP_TG_AUX
#define _define___AMP_DISP_TG_AUX
#define AMP_DISP_TG_AUX (2)
#endif

#ifndef _define___AMP_DISP_TG_MAX
#define _define___AMP_DISP_TG_MAX
#define AMP_DISP_TG_MAX (3)
#endif

#ifndef _typedef___AMP_DISP_OUT_ID
#define _typedef___AMP_DISP_OUT_ID
typedef INT32 AMP_DISP_OUT_ID;
#endif /* _typedef___AMP_DISP_OUT_ID */

#ifndef _define___AMP_DISP_OUT_FIRST
#define _define___AMP_DISP_OUT_FIRST
#define AMP_DISP_OUT_FIRST (0)
#endif

#ifndef _define___AMP_DISP_OUT_HDMI
#define _define___AMP_DISP_OUT_HDMI
#define AMP_DISP_OUT_HDMI (0)
#endif

#ifndef _define___AMP_DISP_OUT_HD_ENC
#define _define___AMP_DISP_OUT_HD_ENC
#define AMP_DISP_OUT_HD_ENC (1)
#endif

#ifndef _define___AMP_DISP_OUT_SD_ENC
#define _define___AMP_DISP_OUT_SD_ENC
#define AMP_DISP_OUT_SD_ENC (2)
#endif

#ifndef _define___AMP_DISP_OUT_TTL24
#define _define___AMP_DISP_OUT_TTL24
#define AMP_DISP_OUT_TTL24 (3)
#endif

#ifndef _define___AMP_DISP_OUT_TTL30
#define _define___AMP_DISP_OUT_TTL30
#define AMP_DISP_OUT_TTL30 (4)
#endif

#ifndef _define___AMP_DISP_OUT_LVDS
#define _define___AMP_DISP_OUT_LVDS
#define AMP_DISP_OUT_LVDS (5)
#endif

#ifndef _define___AMP_DISP_OUT_DSI
#define _define___AMP_DISP_OUT_DSI
#define AMP_DISP_OUT_DSI (6)
#endif

#ifndef _define___AMP_DISP_OUT_MAX
#define _define___AMP_DISP_OUT_MAX
#define AMP_DISP_OUT_MAX (7)
#endif

#ifndef _typedef___AMP_DISP_PORT_ID
#define _typedef___AMP_DISP_PORT_ID
typedef UINT32 AMP_DISP_PORT_ID;
#endif /* _typedef___AMP_DISP_PORT_ID */

#ifndef _define___AMP_DISP_PORT_FIRST
#define _define___AMP_DISP_PORT_FIRST
#define AMP_DISP_PORT_FIRST (0)
#endif

#ifndef _define___AMP_DISP_PORT_COMPONENT
#define _define___AMP_DISP_PORT_COMPONENT
#define AMP_DISP_PORT_COMPONENT (0)
#endif

#ifndef _define___AMP_DISP_PORT_CVBS
#define _define___AMP_DISP_PORT_CVBS
#define AMP_DISP_PORT_CVBS (1)
#endif

#ifndef _define___AMP_DISP_PORT_SVIDEO
#define _define___AMP_DISP_PORT_SVIDEO
#define AMP_DISP_PORT_SVIDEO (2)
#endif

#ifndef _define___AMP_DISP_PORT_DVIO
#define _define___AMP_DISP_PORT_DVIO
#define AMP_DISP_PORT_DVIO (3)
#endif

#ifndef _define___AMP_DISP_PORT_HDMI
#define _define___AMP_DISP_PORT_HDMI
#define AMP_DISP_PORT_HDMI (4)
#endif

#ifndef _define___AMP_DISP_PORT_TTL24
#define _define___AMP_DISP_PORT_TTL24
#define AMP_DISP_PORT_TTL24 (5)
#endif

#ifndef _define___AMP_DISP_PORT_TTL30
#define _define___AMP_DISP_PORT_TTL30
#define AMP_DISP_PORT_TTL30 (6)
#endif

#ifndef _define___AMP_DISP_PORT_LVDS
#define _define___AMP_DISP_PORT_LVDS
#define AMP_DISP_PORT_LVDS (7)
#endif

#ifndef _define___AMP_DISP_PORT_MAX
#define _define___AMP_DISP_PORT_MAX
#define AMP_DISP_PORT_MAX (8)
#endif

#ifndef _typedef___AMP_DISP_LDC_FW_LUT
#define _typedef___AMP_DISP_LDC_FW_LUT
typedef INT32 AMP_DISP_LDC_FW_LUT;
#endif /* _typedef___AMP_DISP_LDC_FW_LUT */

#ifndef _define___AMP_DISP_LDC_LUT_LPSF
#define _define___AMP_DISP_LDC_LUT_LPSF
#define AMP_DISP_LDC_LUT_LPSF (0)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_GPSF
#define _define___AMP_DISP_LDC_LUT_GPSF
#define AMP_DISP_LDC_LUT_GPSF (1)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_SLPFCOEF
#define _define___AMP_DISP_LDC_LUT_SLPFCOEF
#define AMP_DISP_LDC_LUT_SLPFCOEF (2)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_BLPOFF
#define _define___AMP_DISP_LDC_LUT_BLPOFF
#define AMP_DISP_LDC_LUT_BLPOFF (3)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_GWVTR
#define _define___AMP_DISP_LDC_LUT_GWVTR
#define AMP_DISP_LDC_LUT_GWVTR (4)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_LWVTR
#define _define___AMP_DISP_LDC_LUT_LWVTR
#define AMP_DISP_LDC_LUT_LWVTR (5)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_AVTR
#define _define___AMP_DISP_LDC_LUT_AVTR
#define AMP_DISP_LDC_LUT_AVTR (6)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_GNORM
#define _define___AMP_DISP_LDC_LUT_GNORM
#define AMP_DISP_LDC_LUT_GNORM (7)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_LNORM
#define _define___AMP_DISP_LDC_LUT_LNORM
#define AMP_DISP_LDC_LUT_LNORM (8)
#endif

#ifndef _define___AMP_DISP_LDC_LUT_CORRECT
#define _define___AMP_DISP_LDC_LUT_CORRECT
#define AMP_DISP_LDC_LUT_CORRECT (9)
#endif

#ifndef _typedef___AMP_DISP_PWM_CHANNEL
#define _typedef___AMP_DISP_PWM_CHANNEL
typedef INT32 AMP_DISP_PWM_CHANNEL;
#endif /* _typedef___AMP_DISP_PWM_CHANNEL */

#ifndef _define___AMP_DISP_PWM_CHANNEL_0
#define _define___AMP_DISP_PWM_CHANNEL_0
#define AMP_DISP_PWM_CHANNEL_0 (0)
#endif

#ifndef _define___AMP_DISP_PWM_CHANNEL_1
#define _define___AMP_DISP_PWM_CHANNEL_1
#define AMP_DISP_PWM_CHANNEL_1 (1)
#endif

#ifndef _define___AMP_DISP_PWM_CHANNEL_2
#define _define___AMP_DISP_PWM_CHANNEL_2
#define AMP_DISP_PWM_CHANNEL_2 (2)
#endif

#ifndef _define___AMP_DISP_PWM_CHANNEL_3
#define _define___AMP_DISP_PWM_CHANNEL_3
#define AMP_DISP_PWM_CHANNEL_3 (3)
#endif

#ifndef _define___AMP_DISP_PWM_CHANNEL_AVIO
#define _define___AMP_DISP_PWM_CHANNEL_AVIO
#define AMP_DISP_PWM_CHANNEL_AVIO (4)
#endif

#ifndef _typedef___AMP_DISP_PWM_FREQUENCY
#define _typedef___AMP_DISP_PWM_FREQUENCY
typedef INT32 AMP_DISP_PWM_FREQUENCY;
#endif /* _typedef___AMP_DISP_PWM_FREQUENCY */

#ifndef _define___AMP_DISP_PWM_FREQUENCY_60HZ
#define _define___AMP_DISP_PWM_FREQUENCY_60HZ
#define AMP_DISP_PWM_FREQUENCY_60HZ (0)
#endif

#ifndef _define___AMP_DISP_PWM_FREQUENCY_192HZ
#define _define___AMP_DISP_PWM_FREQUENCY_192HZ
#define AMP_DISP_PWM_FREQUENCY_192HZ (1)
#endif

#ifndef _define___AMP_DISP_PWM_FREQUENCY_480HZ
#define _define___AMP_DISP_PWM_FREQUENCY_480HZ
#define AMP_DISP_PWM_FREQUENCY_480HZ (2)
#endif

#ifndef _typedef___AMP_DISP_BACKLIGHT_CONTROL
#define _typedef___AMP_DISP_BACKLIGHT_CONTROL
typedef INT32 AMP_DISP_BACKLIGHT_CONTROL;
#endif /* _typedef___AMP_DISP_BACKLIGHT_CONTROL */

#ifndef _define___AMP_DISP_BACKLIGHT_CONTROL_PWM
#define _define___AMP_DISP_BACKLIGHT_CONTROL_PWM
#define AMP_DISP_BACKLIGHT_CONTROL_PWM (0)
#endif

#ifndef _define___AMP_DISP_BACKLIGHT_CONTROL_SPI
#define _define___AMP_DISP_BACKLIGHT_CONTROL_SPI
#define AMP_DISP_BACKLIGHT_CONTROL_SPI (1)
#endif

#ifndef _define___AMP_DISP_LED_BLOCK_NUM_MAX
#define _define___AMP_DISP_LED_BLOCK_NUM_MAX
#define AMP_DISP_LED_BLOCK_NUM_MAX (64)
#endif

#ifndef _typedef___AMP_DISP_OUT_RES
#define _typedef___AMP_DISP_OUT_RES
typedef UINT32 AMP_DISP_OUT_RES;
#endif /* _typedef___AMP_DISP_OUT_RES */

#ifndef _define___AMP_DISP_OUT_RES_FIRST
#define _define___AMP_DISP_OUT_RES_FIRST
#define AMP_DISP_OUT_RES_FIRST (0)
#endif

#ifndef _define___AMP_DISP_OUT_RES_NTSC_M
#define _define___AMP_DISP_OUT_RES_NTSC_M
#define AMP_DISP_OUT_RES_NTSC_M (0)
#endif

#ifndef _define___AMP_DISP_OUT_RES_NTSC_J
#define _define___AMP_DISP_OUT_RES_NTSC_J
#define AMP_DISP_OUT_RES_NTSC_J (1)
#endif

#ifndef _define___AMP_DISP_OUT_RES_PAL_M
#define _define___AMP_DISP_OUT_RES_PAL_M
#define AMP_DISP_OUT_RES_PAL_M (2)
#endif

#ifndef _define___AMP_DISP_OUT_RES_PAL_BGH
#define _define___AMP_DISP_OUT_RES_PAL_BGH
#define AMP_DISP_OUT_RES_PAL_BGH (3)
#endif

#ifndef _define___AMP_DISP_OUT_RES_525I60
#define _define___AMP_DISP_OUT_RES_525I60
#define AMP_DISP_OUT_RES_525I60 (4)
#endif

#ifndef _define___AMP_DISP_OUT_RES_525I5994
#define _define___AMP_DISP_OUT_RES_525I5994
#define AMP_DISP_OUT_RES_525I5994 (5)
#endif

#ifndef _define___AMP_DISP_OUT_RES_625I50
#define _define___AMP_DISP_OUT_RES_625I50
#define AMP_DISP_OUT_RES_625I50 (6)
#endif

#ifndef _define___AMP_DISP_OUT_RES_525P60
#define _define___AMP_DISP_OUT_RES_525P60
#define AMP_DISP_OUT_RES_525P60 (7)
#endif

#ifndef _define___AMP_DISP_OUT_RES_525P5994
#define _define___AMP_DISP_OUT_RES_525P5994
#define AMP_DISP_OUT_RES_525P5994 (8)
#endif

#ifndef _define___AMP_DISP_OUT_RES_625P50
#define _define___AMP_DISP_OUT_RES_625P50
#define AMP_DISP_OUT_RES_625P50 (9)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P30
#define _define___AMP_DISP_OUT_RES_720P30
#define AMP_DISP_OUT_RES_720P30 (10)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P2997
#define _define___AMP_DISP_OUT_RES_720P2997
#define AMP_DISP_OUT_RES_720P2997 (11)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P25
#define _define___AMP_DISP_OUT_RES_720P25
#define AMP_DISP_OUT_RES_720P25 (12)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P60
#define _define___AMP_DISP_OUT_RES_720P60
#define AMP_DISP_OUT_RES_720P60 (13)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P5994
#define _define___AMP_DISP_OUT_RES_720P5994
#define AMP_DISP_OUT_RES_720P5994 (14)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P50
#define _define___AMP_DISP_OUT_RES_720P50
#define AMP_DISP_OUT_RES_720P50 (15)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080I60
#define _define___AMP_DISP_OUT_RES_1080I60
#define AMP_DISP_OUT_RES_1080I60 (16)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080I5994
#define _define___AMP_DISP_OUT_RES_1080I5994
#define AMP_DISP_OUT_RES_1080I5994 (17)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080I50
#define _define___AMP_DISP_OUT_RES_1080I50
#define AMP_DISP_OUT_RES_1080I50 (18)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P30
#define _define___AMP_DISP_OUT_RES_1080P30
#define AMP_DISP_OUT_RES_1080P30 (19)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P2997
#define _define___AMP_DISP_OUT_RES_1080P2997
#define AMP_DISP_OUT_RES_1080P2997 (20)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P25
#define _define___AMP_DISP_OUT_RES_1080P25
#define AMP_DISP_OUT_RES_1080P25 (21)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P24
#define _define___AMP_DISP_OUT_RES_1080P24
#define AMP_DISP_OUT_RES_1080P24 (22)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P2398
#define _define___AMP_DISP_OUT_RES_1080P2398
#define AMP_DISP_OUT_RES_1080P2398 (23)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P60
#define _define___AMP_DISP_OUT_RES_1080P60
#define AMP_DISP_OUT_RES_1080P60 (24)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P5994
#define _define___AMP_DISP_OUT_RES_1080P5994
#define AMP_DISP_OUT_RES_1080P5994 (25)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P50
#define _define___AMP_DISP_OUT_RES_1080P50
#define AMP_DISP_OUT_RES_1080P50 (26)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1080P48
#define _define___AMP_DISP_OUT_RES_LVDS_1080P48
#define AMP_DISP_OUT_RES_LVDS_1080P48 (27)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1080P50
#define _define___AMP_DISP_OUT_RES_LVDS_1080P50
#define AMP_DISP_OUT_RES_LVDS_1080P50 (28)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1080P60
#define _define___AMP_DISP_OUT_RES_LVDS_1080P60
#define AMP_DISP_OUT_RES_LVDS_1080P60 (29)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_2160P12
#define _define___AMP_DISP_OUT_RES_LVDS_2160P12
#define AMP_DISP_OUT_RES_LVDS_2160P12 (30)
#endif

#ifndef _define___AMP_DISP_OUT_RES_480P60
#define _define___AMP_DISP_OUT_RES_480P60
#define AMP_DISP_OUT_RES_480P60 (31)
#endif

#ifndef _define___AMP_DISP_OUT_RES_480P5994
#define _define___AMP_DISP_OUT_RES_480P5994
#define AMP_DISP_OUT_RES_480P5994 (32)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_FIRST
#define _define___AMP_DISP_OUT_RES_3D_FIRST
#define AMP_DISP_OUT_RES_3D_FIRST (33)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_720P50
#define _define___AMP_DISP_OUT_RES_3D_720P50
#define AMP_DISP_OUT_RES_3D_720P50 (33)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_720P60
#define _define___AMP_DISP_OUT_RES_3D_720P60
#define AMP_DISP_OUT_RES_3D_720P60 (34)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_720P5994
#define _define___AMP_DISP_OUT_RES_3D_720P5994
#define AMP_DISP_OUT_RES_3D_720P5994 (35)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080P24
#define _define___AMP_DISP_OUT_RES_3D_1080P24
#define AMP_DISP_OUT_RES_3D_1080P24 (36)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080P2398
#define _define___AMP_DISP_OUT_RES_3D_1080P2398
#define AMP_DISP_OUT_RES_3D_1080P2398 (37)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080P30
#define _define___AMP_DISP_OUT_RES_3D_1080P30
#define AMP_DISP_OUT_RES_3D_1080P30 (38)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080P2997
#define _define___AMP_DISP_OUT_RES_3D_1080P2997
#define AMP_DISP_OUT_RES_3D_1080P2997 (39)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080P25
#define _define___AMP_DISP_OUT_RES_3D_1080P25
#define AMP_DISP_OUT_RES_3D_1080P25 (40)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080I60
#define _define___AMP_DISP_OUT_RES_3D_1080I60
#define AMP_DISP_OUT_RES_3D_1080I60 (41)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080I5994
#define _define___AMP_DISP_OUT_RES_3D_1080I5994
#define AMP_DISP_OUT_RES_3D_1080I5994 (42)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1080I50
#define _define___AMP_DISP_OUT_RES_3D_1080I50
#define AMP_DISP_OUT_RES_3D_1080I50 (43)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1920X540P60
#define _define___AMP_DISP_OUT_RES_3D_1920X540P60
#define AMP_DISP_OUT_RES_3D_1920X540P60 (44)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1920X540P30
#define _define___AMP_DISP_OUT_RES_3D_1920X540P30
#define AMP_DISP_OUT_RES_3D_1920X540P30 (45)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_1920X540P24
#define _define___AMP_DISP_OUT_RES_3D_1920X540P24
#define AMP_DISP_OUT_RES_3D_1920X540P24 (46)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_720P100
#define _define___AMP_DISP_OUT_RES_LVDS_3D_720P100
#define AMP_DISP_OUT_RES_LVDS_3D_720P100 (47)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_720P120
#define _define___AMP_DISP_OUT_RES_LVDS_3D_720P120
#define AMP_DISP_OUT_RES_LVDS_3D_720P120 (48)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_1080P48
#define _define___AMP_DISP_OUT_RES_LVDS_3D_1080P48
#define AMP_DISP_OUT_RES_LVDS_3D_1080P48 (49)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_1080P50
#define _define___AMP_DISP_OUT_RES_LVDS_3D_1080P50
#define AMP_DISP_OUT_RES_LVDS_3D_1080P50 (50)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_1080P60
#define _define___AMP_DISP_OUT_RES_LVDS_3D_1080P60
#define AMP_DISP_OUT_RES_LVDS_3D_1080P60 (51)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_1920X540P100
#define _define___AMP_DISP_OUT_RES_LVDS_3D_1920X540P100
#define AMP_DISP_OUT_RES_LVDS_3D_1920X540P100 (52)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_1920X540P120
#define _define___AMP_DISP_OUT_RES_LVDS_3D_1920X540P120
#define AMP_DISP_OUT_RES_LVDS_3D_1920X540P120 (53)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_960X1080P100
#define _define___AMP_DISP_OUT_RES_LVDS_3D_960X1080P100
#define AMP_DISP_OUT_RES_LVDS_3D_960X1080P100 (54)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_3D_960X1080P120
#define _define___AMP_DISP_OUT_RES_LVDS_3D_960X1080P120
#define AMP_DISP_OUT_RES_LVDS_3D_960X1080P120 (55)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_MAX
#define _define___AMP_DISP_OUT_RES_3D_MAX
#define AMP_DISP_OUT_RES_3D_MAX (55)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K_MIN
#define _define___AMP_DISP_OUT_RES_4Kx2K_MIN
#define AMP_DISP_OUT_RES_4Kx2K_MIN (56)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP2398
#define _define___AMP_DISP_OUT_RES_4Kx2KP2398
#define AMP_DISP_OUT_RES_4Kx2KP2398 (56)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP24
#define _define___AMP_DISP_OUT_RES_4Kx2KP24
#define AMP_DISP_OUT_RES_4Kx2KP24 (57)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP24_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2KP24_SMPTE
#define AMP_DISP_OUT_RES_4Kx2KP24_SMPTE (58)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP25
#define _define___AMP_DISP_OUT_RES_4Kx2KP25
#define AMP_DISP_OUT_RES_4Kx2KP25 (59)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP2997
#define _define___AMP_DISP_OUT_RES_4Kx2KP2997
#define AMP_DISP_OUT_RES_4Kx2KP2997 (60)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP30
#define _define___AMP_DISP_OUT_RES_4Kx2KP30
#define AMP_DISP_OUT_RES_4Kx2KP30 (61)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP50
#define _define___AMP_DISP_OUT_RES_4Kx2KP50
#define AMP_DISP_OUT_RES_4Kx2KP50 (62)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP5994
#define _define___AMP_DISP_OUT_RES_4Kx2KP5994
#define AMP_DISP_OUT_RES_4Kx2KP5994 (63)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP60
#define _define___AMP_DISP_OUT_RES_4Kx2KP60
#define AMP_DISP_OUT_RES_4Kx2KP60 (64)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP30_HDMI
#define _define___AMP_DISP_OUT_RES_4Kx2KP30_HDMI
#define AMP_DISP_OUT_RES_4Kx2KP30_HDMI (65)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2KP120
#define _define___AMP_DISP_OUT_RES_4Kx2KP120
#define AMP_DISP_OUT_RES_4Kx2KP120 (66)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K_MAX
#define _define___AMP_DISP_OUT_RES_4Kx2K_MAX
#define AMP_DISP_OUT_RES_4Kx2K_MAX (66)
#endif

#ifndef _define___AMP_DISP_OUT_RES_3D_720P_4Kx1K120
#define _define___AMP_DISP_OUT_RES_3D_720P_4Kx1K120
#define AMP_DISP_OUT_RES_3D_720P_4Kx1K120 (67)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P100
#define _define___AMP_DISP_OUT_RES_720P100
#define AMP_DISP_OUT_RES_720P100 (68)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P11988
#define _define___AMP_DISP_OUT_RES_720P11988
#define AMP_DISP_OUT_RES_720P11988 (69)
#endif

#ifndef _define___AMP_DISP_OUT_RES_720P120
#define _define___AMP_DISP_OUT_RES_720P120
#define AMP_DISP_OUT_RES_720P120 (70)
#endif

#ifndef _define___AMP_DISP_OUT_RES_HFR_MIN
#define _define___AMP_DISP_OUT_RES_HFR_MIN
#define AMP_DISP_OUT_RES_HFR_MIN (71)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P100
#define _define___AMP_DISP_OUT_RES_1080P100
#define AMP_DISP_OUT_RES_1080P100 (71)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P11988
#define _define___AMP_DISP_OUT_RES_1080P11988
#define AMP_DISP_OUT_RES_1080P11988 (72)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P120
#define _define___AMP_DISP_OUT_RES_1080P120
#define AMP_DISP_OUT_RES_1080P120 (73)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P8991
#define _define___AMP_DISP_OUT_RES_1080P8991
#define AMP_DISP_OUT_RES_1080P8991 (74)
#endif

#ifndef _define___AMP_DISP_OUT_RES_1080P90
#define _define___AMP_DISP_OUT_RES_1080P90
#define AMP_DISP_OUT_RES_1080P90 (75)
#endif

#ifndef _define___AMP_DISP_OUT_RES_HFR_MAX
#define _define___AMP_DISP_OUT_RES_HFR_MAX
#define AMP_DISP_OUT_RES_HFR_MAX (75)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K2398_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K2398_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K2398_SMPTE (76)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K25_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K25_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K25_SMPTE (77)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K2997_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K2997_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K2997_SMPTE (78)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K30_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K30_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K30_SMPTE (79)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K50_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K50_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K50_SMPTE (80)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K5994_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K5994_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K5994_SMPTE (81)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K60_SMPTE
#define _define___AMP_DISP_OUT_RES_4Kx2K60_SMPTE
#define AMP_DISP_OUT_RES_4Kx2K60_SMPTE (82)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K50_420
#define _define___AMP_DISP_OUT_RES_4Kx2K50_420
#define AMP_DISP_OUT_RES_4Kx2K50_420 (83)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K5994_420
#define _define___AMP_DISP_OUT_RES_4Kx2K5994_420
#define AMP_DISP_OUT_RES_4Kx2K5994_420 (84)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K60_420
#define _define___AMP_DISP_OUT_RES_4Kx2K60_420
#define AMP_DISP_OUT_RES_4Kx2K60_420 (85)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K2398_3D
#define _define___AMP_DISP_OUT_RES_4Kx2K2398_3D
#define AMP_DISP_OUT_RES_4Kx2K2398_3D (86)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K24_3D
#define _define___AMP_DISP_OUT_RES_4Kx2K24_3D
#define AMP_DISP_OUT_RES_4Kx2K24_3D (87)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K25_3D
#define _define___AMP_DISP_OUT_RES_4Kx2K25_3D
#define AMP_DISP_OUT_RES_4Kx2K25_3D (88)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K2997_3D
#define _define___AMP_DISP_OUT_RES_4Kx2K2997_3D
#define AMP_DISP_OUT_RES_4Kx2K2997_3D (89)
#endif

#ifndef _define___AMP_DISP_OUT_RES_4Kx2K30_3D
#define _define___AMP_DISP_OUT_RES_4Kx2K30_3D
#define AMP_DISP_OUT_RES_4Kx2K30_3D (90)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1088P60
#define _define___AMP_DISP_OUT_RES_LVDS_1088P60
#define AMP_DISP_OUT_RES_LVDS_1088P60 (91)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1366X768P60
#define _define___AMP_DISP_OUT_RES_LVDS_1366X768P60
#define AMP_DISP_OUT_RES_LVDS_1366X768P60 (92)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1366X768P5994
#define _define___AMP_DISP_OUT_RES_LVDS_1366X768P5994
#define AMP_DISP_OUT_RES_LVDS_1366X768P5994 (93)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1366X768P50
#define _define___AMP_DISP_OUT_RES_LVDS_1366X768P50
#define AMP_DISP_OUT_RES_LVDS_1366X768P50 (94)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1366X768P48
#define _define___AMP_DISP_OUT_RES_LVDS_1366X768P48
#define AMP_DISP_OUT_RES_LVDS_1366X768P48 (95)
#endif

#ifndef _define___AMP_DISP_OUT_RES_LVDS_1366X768P4796
#define _define___AMP_DISP_OUT_RES_LVDS_1366X768P4796
#define AMP_DISP_OUT_RES_LVDS_1366X768P4796 (96)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_MIN
#define _define___AMP_DISP_OUT_RES_DSI_MIN
#define AMP_DISP_OUT_RES_DSI_MIN (97)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_540x960P60
#define _define___AMP_DISP_OUT_RES_DSI_540x960P60
#define AMP_DISP_OUT_RES_DSI_540x960P60 (97)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_1920x1200P60
#define _define___AMP_DISP_OUT_RES_DSI_1920x1200P60
#define AMP_DISP_OUT_RES_DSI_1920x1200P60 (98)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_800x1280P60
#define _define___AMP_DISP_OUT_RES_DSI_800x1280P60
#define AMP_DISP_OUT_RES_DSI_800x1280P60 (99)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_1920x1080P5994
#define _define___AMP_DISP_OUT_RES_DSI_1920x1080P5994
#define AMP_DISP_OUT_RES_DSI_1920x1080P5994 (100)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_WNC_800x1280P60
#define _define___AMP_DISP_OUT_RES_DSI_WNC_800x1280P60
#define AMP_DISP_OUT_RES_DSI_WNC_800x1280P60 (101)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_CUSTOM
#define _define___AMP_DISP_OUT_RES_DSI_CUSTOM
#define AMP_DISP_OUT_RES_DSI_CUSTOM (102)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_UBQ_800x1280P60
#define _define___AMP_DISP_OUT_RES_DSI_UBQ_800x1280P60
#define AMP_DISP_OUT_RES_DSI_UBQ_800x1280P60 (103)
#endif

#ifndef _define___AMP_DISP_OUT_RES_DSI_MAX
#define _define___AMP_DISP_OUT_RES_DSI_MAX
#define AMP_DISP_OUT_RES_DSI_MAX (103)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_MIN
#define _define___AMP_DISP_OUT_RES_CVT_RB_MIN
#define AMP_DISP_OUT_RES_CVT_RB_MIN (104)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_3840_1600P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_3840_1600P60
#define AMP_DISP_OUT_RES_CVT_RB_3840_1600P60 (104)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_1920_1600P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_1920_1600P60
#define AMP_DISP_OUT_RES_CVT_RB_1920_1600P60 (105)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_3440_1440P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_3440_1440P60
#define AMP_DISP_OUT_RES_CVT_RB_3440_1440P60 (106)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_1720_1440P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_1720_1440P60
#define AMP_DISP_OUT_RES_CVT_RB_1720_1440P60 (107)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_1920_2160P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_1920_2160P60
#define AMP_DISP_OUT_RES_CVT_RB_1920_2160P60 (108)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_1920x1200P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_1920x1200P60
#define AMP_DISP_OUT_RES_CVT_RB_1920x1200P60 (109)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CUSTOM_2880x1440P60
#define _define___AMP_DISP_OUT_RES_CUSTOM_2880x1440P60
#define AMP_DISP_OUT_RES_CUSTOM_2880x1440P60 (110)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_800x600P60
#define _define___AMP_DISP_OUT_RES_VESA_800x600P60
#define AMP_DISP_OUT_RES_VESA_800x600P60 (111)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1280x768P60
#define _define___AMP_DISP_OUT_RES_VESA_1280x768P60
#define AMP_DISP_OUT_RES_VESA_1280x768P60 (112)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1360x768P60
#define _define___AMP_DISP_OUT_RES_VESA_1360x768P60
#define AMP_DISP_OUT_RES_VESA_1360x768P60 (113)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1024x768P60
#define _define___AMP_DISP_OUT_RES_VESA_1024x768P60
#define AMP_DISP_OUT_RES_VESA_1024x768P60 (114)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CUSTOM_1280x800P60
#define _define___AMP_DISP_OUT_RES_CUSTOM_1280x800P60
#define AMP_DISP_OUT_RES_CUSTOM_1280x800P60 (115)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1280x1024P60
#define _define___AMP_DISP_OUT_RES_VESA_1280x1024P60
#define AMP_DISP_OUT_RES_VESA_1280x1024P60 (116)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1440X900P60
#define _define___AMP_DISP_OUT_RES_VESA_1440X900P60
#define AMP_DISP_OUT_RES_VESA_1440X900P60 (117)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_1400x1050P60
#define _define___AMP_DISP_OUT_RES_CVT_1400x1050P60
#define AMP_DISP_OUT_RES_CVT_1400x1050P60 (118)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1680x1050P60
#define _define___AMP_DISP_OUT_RES_VESA_1680x1050P60
#define AMP_DISP_OUT_RES_VESA_1680x1050P60 (119)
#endif

#ifndef _define___AMP_DISP_OUT_RES_VESA_1600x1200P60
#define _define___AMP_DISP_OUT_RES_VESA_1600x1200P60
#define AMP_DISP_OUT_RES_VESA_1600x1200P60 (120)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CUSTOM_1366X768P60
#define _define___AMP_DISP_OUT_RES_CUSTOM_1366X768P60
#define AMP_DISP_OUT_RES_CUSTOM_1366X768P60 (121)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_2560X1440P60
#define _define___AMP_DISP_OUT_RES_CVT_RB_2560X1440P60
#define AMP_DISP_OUT_RES_CVT_RB_2560X1440P60 (122)
#endif

#ifndef _define___AMP_DISP_OUT_RES_CVT_RB_MAX
#define _define___AMP_DISP_OUT_RES_CVT_RB_MAX
#define AMP_DISP_OUT_RES_CVT_RB_MAX (122)
#endif

#ifndef _define___AMP_DISP_OUT_RES_RESET
#define _define___AMP_DISP_OUT_RES_RESET
#define AMP_DISP_OUT_RES_RESET (123)
#endif

#ifndef _define___AMP_DISP_OUT_RES_MAX
#define _define___AMP_DISP_OUT_RES_MAX
#define AMP_DISP_OUT_RES_MAX (124)
#endif

#ifndef _typedef___AMP_DISP_OUT_REFRESH_RATE
#define _typedef___AMP_DISP_OUT_REFRESH_RATE
typedef UINT32 AMP_DISP_OUT_REFRESH_RATE;
#endif /* _typedef___AMP_DISP_OUT_REFRESH_RATE */

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_23P98
#define _define___AMP_DISP_OUT_REFRESH_RATE_23P98
#define AMP_DISP_OUT_REFRESH_RATE_23P98 (0)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_24
#define _define___AMP_DISP_OUT_REFRESH_RATE_24
#define AMP_DISP_OUT_REFRESH_RATE_24 (1)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_25
#define _define___AMP_DISP_OUT_REFRESH_RATE_25
#define AMP_DISP_OUT_REFRESH_RATE_25 (2)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_29P97
#define _define___AMP_DISP_OUT_REFRESH_RATE_29P97
#define AMP_DISP_OUT_REFRESH_RATE_29P97 (3)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_30
#define _define___AMP_DISP_OUT_REFRESH_RATE_30
#define AMP_DISP_OUT_REFRESH_RATE_30 (4)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_47P96
#define _define___AMP_DISP_OUT_REFRESH_RATE_47P96
#define AMP_DISP_OUT_REFRESH_RATE_47P96 (5)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_48
#define _define___AMP_DISP_OUT_REFRESH_RATE_48
#define AMP_DISP_OUT_REFRESH_RATE_48 (6)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_50
#define _define___AMP_DISP_OUT_REFRESH_RATE_50
#define AMP_DISP_OUT_REFRESH_RATE_50 (7)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_59P94
#define _define___AMP_DISP_OUT_REFRESH_RATE_59P94
#define AMP_DISP_OUT_REFRESH_RATE_59P94 (8)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_60
#define _define___AMP_DISP_OUT_REFRESH_RATE_60
#define AMP_DISP_OUT_REFRESH_RATE_60 (9)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_100
#define _define___AMP_DISP_OUT_REFRESH_RATE_100
#define AMP_DISP_OUT_REFRESH_RATE_100 (10)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_119P88
#define _define___AMP_DISP_OUT_REFRESH_RATE_119P88
#define AMP_DISP_OUT_REFRESH_RATE_119P88 (11)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_120
#define _define___AMP_DISP_OUT_REFRESH_RATE_120
#define AMP_DISP_OUT_REFRESH_RATE_120 (12)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_89P91
#define _define___AMP_DISP_OUT_REFRESH_RATE_89P91
#define AMP_DISP_OUT_REFRESH_RATE_89P91 (13)
#endif

#ifndef _define___AMP_DISP_OUT_REFRESH_RATE_90
#define _define___AMP_DISP_OUT_REFRESH_RATE_90
#define AMP_DISP_OUT_REFRESH_RATE_90 (14)
#endif

#ifndef _typedef___AMP_DISP_OUT_RESOLUTION_DESCRIPTION
#define _typedef___AMP_DISP_OUT_RESOLUTION_DESCRIPTION
typedef struct AMP_DISP_OUT_RESOLUTION_DESCRIPTION {
  UINT32 uiActiveWidth;
  UINT32 uiActiveHeight;
  UINT32 uiWidth;
  UINT32 uiHeight;
  AMP_DISP_OUT_REFRESH_RATE uiRefreshRate;
  UINT32 uiIsInterlaced;
  UINT32 uiIsDDD;
  CORBA_double dfps;
} AMP_DISP_OUT_RESOLUTION_DESCRIPTION;
#endif /* _typedef___AMP_DISP_OUT_RESOLUTION_DESCRIPTION */

#ifndef _typedef___AMP_DISP_OUT_MODE
#define _typedef___AMP_DISP_OUT_MODE
typedef INT32 AMP_DISP_OUT_MODE;
#endif /* _typedef___AMP_DISP_OUT_MODE */

#ifndef _define___AMP_DISP_OUT_PROGRESSIVE
#define _define___AMP_DISP_OUT_PROGRESSIVE
#define AMP_DISP_OUT_PROGRESSIVE (0)
#endif

#ifndef _define___AMP_DISP_OUT_INTERLACED
#define _define___AMP_DISP_OUT_INTERLACED
#define AMP_DISP_OUT_INTERLACED (1)
#endif

#ifndef _define___AMP_DISP_OUT_AUTO_SELECT
#define _define___AMP_DISP_OUT_AUTO_SELECT
#define AMP_DISP_OUT_AUTO_SELECT (2)
#endif

#ifndef _typedef___AMP_DISP_OUT_CLR_FMT
#define _typedef___AMP_DISP_OUT_CLR_FMT
typedef UINT32 AMP_DISP_OUT_CLR_FMT;
#endif /* _typedef___AMP_DISP_OUT_CLR_FMT */

#ifndef _define___AMP_DISP_OUT_CLR_FMT_FIRST
#define _define___AMP_DISP_OUT_CLR_FMT_FIRST
#define AMP_DISP_OUT_CLR_FMT_FIRST (0)
#endif

#ifndef _define___AMP_DISP_OUT_CLR_FMT_RGB888
#define _define___AMP_DISP_OUT_CLR_FMT_RGB888
#define AMP_DISP_OUT_CLR_FMT_RGB888 (0)
#endif

#ifndef _define___AMP_DISP_OUT_CLR_FMT_YCBCR444
#define _define___AMP_DISP_OUT_CLR_FMT_YCBCR444
#define AMP_DISP_OUT_CLR_FMT_YCBCR444 (1)
#endif

#ifndef _define___AMP_DISP_OUT_CLR_FMT_YCBCR422
#define _define___AMP_DISP_OUT_CLR_FMT_YCBCR422
#define AMP_DISP_OUT_CLR_FMT_YCBCR422 (2)
#endif

#ifndef _define___AMP_DISP_OUT_CLR_FMT_YCBCR420
#define _define___AMP_DISP_OUT_CLR_FMT_YCBCR420
#define AMP_DISP_OUT_CLR_FMT_YCBCR420 (3)
#endif

#ifndef _define___AMP_DISP_OUT_CLR_FMT_sRGB888
#define _define___AMP_DISP_OUT_CLR_FMT_sRGB888
#define AMP_DISP_OUT_CLR_FMT_sRGB888 (4)
#endif

#ifndef _define___AMP_DISP_OUT_CLR_FMT_MAX
#define _define___AMP_DISP_OUT_CLR_FMT_MAX
#define AMP_DISP_OUT_CLR_FMT_MAX (5)
#endif

#ifndef _typedef___AMP_DISP_OUT_BIT_DEP
#define _typedef___AMP_DISP_OUT_BIT_DEP
typedef UINT32 AMP_DISP_OUT_BIT_DEP;
#endif /* _typedef___AMP_DISP_OUT_BIT_DEP */

#ifndef _define___AMP_DISP_OUT_BIT_DPE_FIRST
#define _define___AMP_DISP_OUT_BIT_DPE_FIRST
#define AMP_DISP_OUT_BIT_DPE_FIRST (0)
#endif

#ifndef _define___AMP_DISP_OUT_BIT_DPE_12
#define _define___AMP_DISP_OUT_BIT_DPE_12
#define AMP_DISP_OUT_BIT_DPE_12 (0)
#endif

#ifndef _define___AMP_DISP_OUT_BIT_DPE_10
#define _define___AMP_DISP_OUT_BIT_DPE_10
#define AMP_DISP_OUT_BIT_DPE_10 (1)
#endif

#ifndef _define___AMP_DISP_OUT_BIT_DPE_8
#define _define___AMP_DISP_OUT_BIT_DPE_8
#define AMP_DISP_OUT_BIT_DPE_8 (2)
#endif

#ifndef _define___AMP_DISP_OUT_BIT_DPE_MAX
#define _define___AMP_DISP_OUT_BIT_DPE_MAX
#define AMP_DISP_OUT_BIT_DPE_MAX (3)
#endif

#ifndef _typedef___AMP_DISP_OUT_ASPECT_RATIO
#define _typedef___AMP_DISP_OUT_ASPECT_RATIO
typedef INT32 AMP_DISP_OUT_ASPECT_RATIO;
#endif /* _typedef___AMP_DISP_OUT_ASPECT_RATIO */

#ifndef _define___AMP_DISP_OUT_FIRST_ASPECT_RATIO
#define _define___AMP_DISP_OUT_FIRST_ASPECT_RATIO
#define AMP_DISP_OUT_FIRST_ASPECT_RATIO (0)
#endif

#ifndef _define___AMP_DISP_OUT_ASPECT_RATIO_NONE
#define _define___AMP_DISP_OUT_ASPECT_RATIO_NONE
#define AMP_DISP_OUT_ASPECT_RATIO_NONE (0)
#endif

#ifndef _define___AMP_DISP_OUT_ASPECT_RATIO_4BY3
#define _define___AMP_DISP_OUT_ASPECT_RATIO_4BY3
#define AMP_DISP_OUT_ASPECT_RATIO_4BY3 (1)
#endif

#ifndef _define___AMP_DISP_OUT_ASPECT_RATIO_16BY9
#define _define___AMP_DISP_OUT_ASPECT_RATIO_16BY9
#define AMP_DISP_OUT_ASPECT_RATIO_16BY9 (2)
#endif

#ifndef _define___AMP_DISP_OUT_MAX_ASPECT_RATIO
#define _define___AMP_DISP_OUT_MAX_ASPECT_RATIO
#define AMP_DISP_OUT_MAX_ASPECT_RATIO (3)
#endif

#ifndef _typedef___AMP_DISP_EVENT_HDMI
#define _typedef___AMP_DISP_EVENT_HDMI
typedef INT32 AMP_DISP_EVENT_HDMI;
#endif /* _typedef___AMP_DISP_EVENT_HDMI */

#ifndef _define___AMP_DISP_EVENT_HDMI_SINK_CONNECTED
#define _define___AMP_DISP_EVENT_HDMI_SINK_CONNECTED
#define AMP_DISP_EVENT_HDMI_SINK_CONNECTED (0)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_SINK_DISCONNECTED
#define _define___AMP_DISP_EVENT_HDMI_SINK_DISCONNECTED
#define AMP_DISP_EVENT_HDMI_SINK_DISCONNECTED (1)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_VIDEO_CFG_ERR
#define _define___AMP_DISP_EVENT_HDMI_VIDEO_CFG_ERR
#define AMP_DISP_EVENT_HDMI_VIDEO_CFG_ERR (2)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_AUDIO_CFG_ERR
#define _define___AMP_DISP_EVENT_HDMI_AUDIO_CFG_ERR
#define AMP_DISP_EVENT_HDMI_AUDIO_CFG_ERR (3)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_HDCP_ERR
#define _define___AMP_DISP_EVENT_HDMI_HDCP_ERR
#define AMP_DISP_EVENT_HDMI_HDCP_ERR (4)
#endif

#ifndef _define___AMP_DISP_VPP_EVENT_HDMI_RESUME
#define _define___AMP_DISP_VPP_EVENT_HDMI_RESUME
#define AMP_DISP_VPP_EVENT_HDMI_RESUME (5)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_HDCP_AUTH_DONE
#define _define___AMP_DISP_EVENT_HDMI_HDCP_AUTH_DONE
#define AMP_DISP_EVENT_HDMI_HDCP_AUTH_DONE (6)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_EDID_READ_ERR
#define _define___AMP_DISP_EVENT_HDMI_EDID_READ_ERR
#define AMP_DISP_EVENT_HDMI_EDID_READ_ERR (7)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_START
#define _define___AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_START
#define AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_START (8)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_STOP
#define _define___AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_STOP
#define AMP_DISP_EVENT_HDMI_HDR_PLAYBACK_STOP (9)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_HDCP_VER_1_x
#define _define___AMP_DISP_EVENT_HDMI_HDCP_VER_1_x
#define AMP_DISP_EVENT_HDMI_HDCP_VER_1_x (10)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_HDCP_VER_2_x
#define _define___AMP_DISP_EVENT_HDMI_HDCP_VER_2_x
#define AMP_DISP_EVENT_HDMI_HDCP_VER_2_x (11)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_DV_PLAYBACK_START
#define _define___AMP_DISP_EVENT_HDMI_DV_PLAYBACK_START
#define AMP_DISP_EVENT_HDMI_DV_PLAYBACK_START (12)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_DV_PLAYBACK_STOP
#define _define___AMP_DISP_EVENT_HDMI_DV_PLAYBACK_STOP
#define AMP_DISP_EVENT_HDMI_DV_PLAYBACK_STOP (13)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_DV_ON
#define _define___AMP_DISP_EVENT_HDMI_DV_ON
#define AMP_DISP_EVENT_HDMI_DV_ON (14)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_DV_OFF
#define _define___AMP_DISP_EVENT_HDMI_DV_OFF
#define AMP_DISP_EVENT_HDMI_DV_OFF (15)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_SMP_PASS
#define _define___AMP_DISP_EVENT_HDMI_SMP_PASS
#define AMP_DISP_EVENT_HDMI_SMP_PASS (16)
#endif

#ifndef _define___AMP_DISP_EVENT_HDMI_SMP_FAIL
#define _define___AMP_DISP_EVENT_HDMI_SMP_FAIL
#define AMP_DISP_EVENT_HDMI_SMP_FAIL (17)
#endif

#ifndef _typedef___AMP_DISP_HDMI_HDCP_VER
#define _typedef___AMP_DISP_HDMI_HDCP_VER
typedef INT32 AMP_DISP_HDMI_HDCP_VER;
#endif /* _typedef___AMP_DISP_HDMI_HDCP_VER */

#ifndef _define___AMP_DISP_HDMI_HDCP_VER_ERR
#define _define___AMP_DISP_HDMI_HDCP_VER_ERR
#define AMP_DISP_HDMI_HDCP_VER_ERR (-1)
#endif

#ifndef _define___AMP_DISP_HDMI_HDCP_VER_NONE
#define _define___AMP_DISP_HDMI_HDCP_VER_NONE
#define AMP_DISP_HDMI_HDCP_VER_NONE (0)
#endif

#ifndef _define___AMP_DISP_HDMI_HDCP_VER_1_x
#define _define___AMP_DISP_HDMI_HDCP_VER_1_x
#define AMP_DISP_HDMI_HDCP_VER_1_x (1)
#endif

#ifndef _define___AMP_DISP_HDMI_HDCP_VER_2_2
#define _define___AMP_DISP_HDMI_HDCP_VER_2_2
#define AMP_DISP_HDMI_HDCP_VER_2_2 (2)
#endif

#ifndef _typedef___AMP_DISP_EVENT_HDCP
#define _typedef___AMP_DISP_EVENT_HDCP
typedef UINT32 AMP_DISP_EVENT_HDCP;
#endif /* _typedef___AMP_DISP_EVENT_HDCP */

#ifndef _define___AMP_DISP_EVENT_HDCP_DEV_NOT_RDY
#define _define___AMP_DISP_EVENT_HDCP_DEV_NOT_RDY
#define AMP_DISP_EVENT_HDCP_DEV_NOT_RDY (268435456)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_TXRX_ERR
#define _define___AMP_DISP_EVENT_HDCP_TXRX_ERR
#define AMP_DISP_EVENT_HDCP_TXRX_ERR (536870912)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_ERR (805306368)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_SUCCESS
#define _define___AMP_DISP_EVENT_HDCP_AUTH_SUCCESS
#define AMP_DISP_EVENT_HDCP_AUTH_SUCCESS (1073741824)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_DS_DEV_REPEATER
#define _define___AMP_DISP_EVENT_HDCP_DS_DEV_REPEATER
#define AMP_DISP_EVENT_HDCP_DS_DEV_REPEATER (1342177280)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_DS_REAUTH_REQ
#define _define___AMP_DISP_EVENT_HDCP_DS_REAUTH_REQ
#define AMP_DISP_EVENT_HDCP_DS_REAUTH_REQ (1610612736)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_ENGINE_INIT
#define _define___AMP_DISP_EVENT_HDCP_ENGINE_INIT
#define AMP_DISP_EVENT_HDCP_ENGINE_INIT (1879048192)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_MASK_FLAG
#define _define___AMP_DISP_EVENT_HDCP_MASK_FLAG
#define AMP_DISP_EVENT_HDCP_MASK_FLAG (-268435456)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_GENERIC
#define _define___AMP_DISP_EVENT_HDCP_AUTH_GENERIC
#define AMP_DISP_EVENT_HDCP_AUTH_GENERIC (0)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_RI_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_RI_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_RI_ERR (1)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_R0_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_R0_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_R0_ERR (2)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_NO_KEYS_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_NO_KEYS_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_NO_KEYS_ERR (3)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_RD_BKSV_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_RD_BKSV_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_RD_BKSV_ERR (4)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_REP_VI_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_REP_VI_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_REP_VI_ERR (5)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_REP_TOP_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_REP_TOP_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_REP_TOP_ERR (6)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_REP_DELAY_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_REP_DELAY_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_REP_DELAY_ERR (7)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_KEY_REVOKED_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_KEY_REVOKED_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_KEY_REVOKED_ERR (8)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_SRM_INTG_ERR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_SRM_INTG_ERR
#define AMP_DISP_EVENT_HDCP_AUTH_SRM_INTG_ERR (9)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_I2C_ERROR
#define _define___AMP_DISP_EVENT_HDCP_AUTH_I2C_ERROR
#define AMP_DISP_EVENT_HDCP_AUTH_I2C_ERROR (10)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_REAUTH_REQ
#define _define___AMP_DISP_EVENT_HDCP_AUTH_REAUTH_REQ
#define AMP_DISP_EVENT_HDCP_AUTH_REAUTH_REQ (11)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_AUTH_ERR_MASK_FLAG
#define _define___AMP_DISP_EVENT_HDCP_AUTH_ERR_MASK_FLAG
#define AMP_DISP_EVENT_HDCP_AUTH_ERR_MASK_FLAG (15)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_NONE
#define _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_NONE
#define AMP_DISP_EVENT_HDCP_SRC_VERSION_NONE (0)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_1_x
#define _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_1_x
#define AMP_DISP_EVENT_HDCP_SRC_VERSION_1_x (1)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_2_2
#define _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_2_2
#define AMP_DISP_EVENT_HDCP_SRC_VERSION_2_2 (2)
#endif

#ifndef _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_MASK
#define _define___AMP_DISP_EVENT_HDCP_SRC_VERSION_MASK
#define AMP_DISP_EVENT_HDCP_SRC_VERSION_MASK (15)
#endif

#ifndef _typedef___AMP_DISP_EVENT_CEC
#define _typedef___AMP_DISP_EVENT_CEC
typedef INT32 AMP_DISP_EVENT_CEC;
#endif /* _typedef___AMP_DISP_EVENT_CEC */

#ifndef _define___AMP_DISP_EVENT_CEC_LOG_ADDR_STS
#define _define___AMP_DISP_EVENT_CEC_LOG_ADDR_STS
#define AMP_DISP_EVENT_CEC_LOG_ADDR_STS (0)
#endif

#ifndef _define___AMP_DISP_EVENT_CEC_MSG_TX_STS
#define _define___AMP_DISP_EVENT_CEC_MSG_TX_STS
#define AMP_DISP_EVENT_CEC_MSG_TX_STS (1)
#endif

#ifndef _define___AMP_DISP_EVENT_CEC_MSG_RX_STS
#define _define___AMP_DISP_EVENT_CEC_MSG_RX_STS
#define AMP_DISP_EVENT_CEC_MSG_RX_STS (2)
#endif

#ifndef _typedef___AMP_DISP_HDMI_AUX_PKT_ID
#define _typedef___AMP_DISP_HDMI_AUX_PKT_ID
typedef INT32 AMP_DISP_HDMI_AUX_PKT_ID;
#endif /* _typedef___AMP_DISP_HDMI_AUX_PKT_ID */

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_ACP
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_ACP
#define AMP_DISP_HDMI_AUX_PKT_ID_ACP (4)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_ISRC1
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_ISRC1
#define AMP_DISP_HDMI_AUX_PKT_ID_ISRC1 (5)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_ISRC2
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_ISRC2
#define AMP_DISP_HDMI_AUX_PKT_ID_ISRC2 (6)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_HDR_INFOFRAME
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_HDR_INFOFRAME
#define AMP_DISP_HDMI_AUX_PKT_ID_HDR_INFOFRAME (7)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_GAMUT_METADATA
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_GAMUT_METADATA
#define AMP_DISP_HDMI_AUX_PKT_ID_GAMUT_METADATA (10)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_VENDOR_INFOFRAME
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_VENDOR_INFOFRAME
#define AMP_DISP_HDMI_AUX_PKT_ID_VENDOR_INFOFRAME (129)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_AVI_INFOFRAME
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_AVI_INFOFRAME
#define AMP_DISP_HDMI_AUX_PKT_ID_AVI_INFOFRAME (130)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_SPD_INFOFRAME
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_SPD_INFOFRAME
#define AMP_DISP_HDMI_AUX_PKT_ID_SPD_INFOFRAME (131)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_AUDIO_INFOFRAME
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_AUDIO_INFOFRAME
#define AMP_DISP_HDMI_AUX_PKT_ID_AUDIO_INFOFRAME (132)
#endif

#ifndef _define___AMP_DISP_HDMI_AUX_PKT_ID_MPEG_SRC_INFOFRAME
#define _define___AMP_DISP_HDMI_AUX_PKT_ID_MPEG_SRC_INFOFRAME
#define AMP_DISP_HDMI_AUX_PKT_ID_MPEG_SRC_INFOFRAME (133)
#endif

#ifndef _typedef___AMP_DISP_NGWM_PIPE_SOURCE_ID
#define _typedef___AMP_DISP_NGWM_PIPE_SOURCE_ID
typedef INT32 AMP_DISP_NGWM_PIPE_SOURCE_ID;
#endif /* _typedef___AMP_DISP_NGWM_PIPE_SOURCE_ID */

#ifndef _define___AMP_DISP_NGWM_PIPE_SOURCE_MAIN
#define _define___AMP_DISP_NGWM_PIPE_SOURCE_MAIN
#define AMP_DISP_NGWM_PIPE_SOURCE_MAIN (0)
#endif

#ifndef _define___AMP_DISP_NGWM_PIPE_SOURCE_OVPSCL
#define _define___AMP_DISP_NGWM_PIPE_SOURCE_OVPSCL
#define AMP_DISP_NGWM_PIPE_SOURCE_OVPSCL (1)
#endif

#ifndef _define___AMP_DISP_NGWM_PIPE_SOURCE_MAX
#define _define___AMP_DISP_NGWM_PIPE_SOURCE_MAX
#define AMP_DISP_NGWM_PIPE_SOURCE_MAX (2)
#endif

#ifndef _define___AMP_DISP_NGWM_PIPE_SOURCE_GLOBAL
#define _define___AMP_DISP_NGWM_PIPE_SOURCE_GLOBAL
#define AMP_DISP_NGWM_PIPE_SOURCE_GLOBAL (-1)
#endif

#ifndef _typedef___AMP_DISP_ZORDER
#define _typedef___AMP_DISP_ZORDER
typedef struct AMP_DISP_ZORDER {
  INT32 iMain;
  INT32 iPip;
  INT32 iGfx0;
  INT32 iGfx1;
  INT32 iGfx2;
  INT32 iPg;
  INT32 iBg;
  INT32 iAux;
} AMP_DISP_ZORDER;
#endif /* _typedef___AMP_DISP_ZORDER */

#ifndef _typedef___AMP_DISP_WIN
#define _typedef___AMP_DISP_WIN
typedef struct AMP_DISP_WIN {
  INT32 iX;
  INT32 iY;
  INT32 iWidth;
  INT32 iHeight;
} AMP_DISP_WIN;
#endif /* _typedef___AMP_DISP_WIN */

#ifndef _typedef___AMP_DISP_VPP_WIN_ATTR
#define _typedef___AMP_DISP_VPP_WIN_ATTR
typedef struct AMP_DISP_VPP_WIN_ATTR {
  INT32 bgcolor;
  INT32 alpha;
  INT32 globalAlphaFlag;
} AMP_DISP_VPP_WIN_ATTR;
#endif /* _typedef___AMP_DISP_VPP_WIN_ATTR */

#ifndef _typedef___AMP_DISP_CMU_EE_DEFAULT_MODE_STRUCT
#define _typedef___AMP_DISP_CMU_EE_DEFAULT_MODE_STRUCT
typedef struct AMP_DISP_CMU_EE_DEFAULT_MODE_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_DEFAULT_MODE_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_DEFAULT_MODE_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_SOFT_BYPASS_MODE_STRUCT
#define _typedef___AMP_DISP_CMU_EE_SOFT_BYPASS_MODE_STRUCT
typedef struct AMP_DISP_CMU_EE_SOFT_BYPASS_MODE_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_SOFT_BYPASS_MODE_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_SOFT_BYPASS_MODE_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_ONLY_UPS_ENABLED_STRUCT
#define _typedef___AMP_DISP_CMU_EE_ONLY_UPS_ENABLED_STRUCT
typedef struct AMP_DISP_CMU_EE_ONLY_UPS_ENABLED_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_ONLY_UPS_ENABLED_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_ONLY_UPS_ENABLED_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT_STRUCT
#define _typedef___AMP_DISP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT_STRUCT
typedef struct AMP_DISP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_OLD_LTI_CTI_WITH_ALL_STRUCT
#define _typedef___AMP_DISP_CMU_EE_OLD_LTI_CTI_WITH_ALL_STRUCT
typedef struct AMP_DISP_CMU_EE_OLD_LTI_CTI_WITH_ALL_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_OLD_LTI_CTI_WITH_ALL_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_OLD_LTI_CTI_WITH_ALL_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER_STRUCT
#define _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER_STRUCT
typedef struct AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_ALL_STRUCT
#define _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_ALL_STRUCT
typedef struct AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_ALL_STRUCT {
  UINT32 ui32Ctrlparam;
} AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_ALL_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_ALL_STRUCT */

#ifndef _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL_STRUCT
#define _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL_STRUCT
typedef struct AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL_STRUCT {
  UINT8 u8Ltigain;
  UINT8 u8Ctigain;
  UINT16 ui16Rsrvd;
} AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL_STRUCT;
#endif /* _typedef___AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL_STRUCT */

#ifndef _define___AMP_DISP_VPP_CMU_EE_DEFAULT_MODE
#define _define___AMP_DISP_VPP_CMU_EE_DEFAULT_MODE
#define AMP_DISP_VPP_CMU_EE_DEFAULT_MODE (0)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_SOFT_BYPASS_MODE
#define _define___AMP_DISP_VPP_CMU_EE_SOFT_BYPASS_MODE
#define AMP_DISP_VPP_CMU_EE_SOFT_BYPASS_MODE (1)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_ONLY_UPS_ENABLED
#define _define___AMP_DISP_VPP_CMU_EE_ONLY_UPS_ENABLED
#define AMP_DISP_VPP_CMU_EE_ONLY_UPS_ENABLED (2)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT
#define _define___AMP_DISP_VPP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT
#define AMP_DISP_VPP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT (3)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_OLD_LTI_CTI_WITH_ALL
#define _define___AMP_DISP_VPP_CMU_EE_OLD_LTI_CTI_WITH_ALL
#define AMP_DISP_VPP_CMU_EE_OLD_LTI_CTI_WITH_ALL (4)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER
#define _define___AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER
#define AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER (5)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_ALL
#define _define___AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_ALL
#define AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_ALL (6)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL
#define _define___AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL
#define AMP_DISP_VPP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL (7)
#endif

#ifndef _define___AMP_DISP_VPP_CMU_EE_MAX_MODE
#define _define___AMP_DISP_VPP_CMU_EE_MAX_MODE
#define AMP_DISP_VPP_CMU_EE_MAX_MODE (8)
#endif

#ifndef _typedef___AMP_DISP_ENUM_VPP_CMU_EE_MODE
#define _typedef___AMP_DISP_ENUM_VPP_CMU_EE_MODE
typedef CORBA_enum AMP_DISP_ENUM_VPP_CMU_EE_MODE;
#endif /* _typedef___AMP_DISP_ENUM_VPP_CMU_EE_MODE */

#ifndef _typedef___AMP_DISP_VPP_CMU_EE_CTRL_PARAM
#define _typedef___AMP_DISP_VPP_CMU_EE_CTRL_PARAM
typedef struct AMP_DISP_VPP_CMU_EE_CTRL_PARAM {
  UINT32 _d;
  union {
    struct AMP_DISP_CMU_EE_DEFAULT_MODE_STRUCT stVppCmuDefaultCtrlParam;
    struct AMP_DISP_CMU_EE_SOFT_BYPASS_MODE_STRUCT stVppCmuSoftBypassCtrlParam;
    struct AMP_DISP_CMU_EE_ONLY_UPS_ENABLED_STRUCT stVppCmuUpsEnParam;
    struct AMP_DISP_CMU_EE_ONLY_UPS_WITH_RINGING_ADAPT_STRUCT stVppCmuUpsEnRingParam;
    struct AMP_DISP_CMU_EE_OLD_LTI_CTI_WITH_ALL_STRUCT stVppCmuOldLtiCtiCtrlParam;
    struct AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_WITHOUT_DETAILER_STRUCT stVppCmuNewLtiCtiNoDetailerCtrlParam;
    struct AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_ALL_STRUCT stVppCmuNewLtiCtiCtrlParam;
    struct AMP_DISP_CMU_EE_NEW_LTI_CTI_WITH_CUSTOM_GAIN_CONTROL_STRUCT stVppCmuLtiCtiCustomCtrlParam;
  } _u;
} AMP_DISP_VPP_CMU_EE_CTRL_PARAM;
#endif /* _typedef___AMP_DISP_VPP_CMU_EE_CTRL_PARAM */

#ifndef _define___AMP_DISP_COLOR_PRI_RESERVED1
#define _define___AMP_DISP_COLOR_PRI_RESERVED1
#define AMP_DISP_COLOR_PRI_RESERVED1 (0)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_BT709
#define _define___AMP_DISP_COLOR_PRI_BT709
#define AMP_DISP_COLOR_PRI_BT709 (1)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_UNSPECIFIED
#define _define___AMP_DISP_COLOR_PRI_UNSPECIFIED
#define AMP_DISP_COLOR_PRI_UNSPECIFIED (2)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_RESERVED2
#define _define___AMP_DISP_COLOR_PRI_RESERVED2
#define AMP_DISP_COLOR_PRI_RESERVED2 (3)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_BT470M
#define _define___AMP_DISP_COLOR_PRI_BT470M
#define AMP_DISP_COLOR_PRI_BT470M (4)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_BT470BG
#define _define___AMP_DISP_COLOR_PRI_BT470BG
#define AMP_DISP_COLOR_PRI_BT470BG (5)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_SMPTE170M
#define _define___AMP_DISP_COLOR_PRI_SMPTE170M
#define AMP_DISP_COLOR_PRI_SMPTE170M (6)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_SMPTE470M
#define _define___AMP_DISP_COLOR_PRI_SMPTE470M
#define AMP_DISP_COLOR_PRI_SMPTE470M (7)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_FILM
#define _define___AMP_DISP_COLOR_PRI_FILM
#define AMP_DISP_COLOR_PRI_FILM (8)
#endif

#ifndef _define___AMP_DISP_COLOR_PRI_BT2020
#define _define___AMP_DISP_COLOR_PRI_BT2020
#define AMP_DISP_COLOR_PRI_BT2020 (9)
#endif

#ifndef _typedef___AMP_DISP_COLOR_PRI_TYPE
#define _typedef___AMP_DISP_COLOR_PRI_TYPE
typedef CORBA_enum AMP_DISP_COLOR_PRI_TYPE;
#endif /* _typedef___AMP_DISP_COLOR_PRI_TYPE */

#ifndef _define___AMP_DISP_VID_3D_FMT_NONE
#define _define___AMP_DISP_VID_3D_FMT_NONE
#define AMP_DISP_VID_3D_FMT_NONE (0)
#endif

#ifndef _define___AMP_DISP_VID_3D_FMT_SBS
#define _define___AMP_DISP_VID_3D_FMT_SBS
#define AMP_DISP_VID_3D_FMT_SBS (1)
#endif

#ifndef _define___AMP_DISP_VID_3D_FMT_TAB
#define _define___AMP_DISP_VID_3D_FMT_TAB
#define AMP_DISP_VID_3D_FMT_TAB (2)
#endif

#ifndef _define___AMP_DISP_VID_3D_FMT_FP
#define _define___AMP_DISP_VID_3D_FMT_FP
#define AMP_DISP_VID_3D_FMT_FP (3)
#endif

#ifndef _typedef___AMP_DISP_VID_3D_FMT_TYPE
#define _typedef___AMP_DISP_VID_3D_FMT_TYPE
typedef CORBA_enum AMP_DISP_VID_3D_FMT_TYPE;
#endif /* _typedef___AMP_DISP_VID_3D_FMT_TYPE */

#ifndef _define___AMP_DISP_OUTPUT_YUV422_8BITS_UYVY
#define _define___AMP_DISP_OUTPUT_YUV422_8BITS_UYVY
#define AMP_DISP_OUTPUT_YUV422_8BITS_UYVY (0)
#endif

#ifndef _define___AMP_DISP_OUTPUT_YUV422_8BITS_VYUY
#define _define___AMP_DISP_OUTPUT_YUV422_8BITS_VYUY
#define AMP_DISP_OUTPUT_YUV422_8BITS_VYUY (1)
#endif

#ifndef _define___AMP_DISP_OUTPUT_YUV422_8BITS_YUYV
#define _define___AMP_DISP_OUTPUT_YUV422_8BITS_YUYV
#define AMP_DISP_OUTPUT_YUV422_8BITS_YUYV (2)
#endif

#ifndef _define___AMP_DISP_OUTPUT_YUV422_8BITS_YVYU
#define _define___AMP_DISP_OUTPUT_YUV422_8BITS_YVYU
#define AMP_DISP_OUTPUT_YUV422_8BITS_YVYU (3)
#endif

#ifndef _define___AMP_DISP_OUTPUT_ARGB_8BITS_ARGB
#define _define___AMP_DISP_OUTPUT_ARGB_8BITS_ARGB
#define AMP_DISP_OUTPUT_ARGB_8BITS_ARGB (4)
#endif

#ifndef _define___AMP_DISP_OUTPUT_ARGB_8BITS_ABGR
#define _define___AMP_DISP_OUTPUT_ARGB_8BITS_ABGR
#define AMP_DISP_OUTPUT_ARGB_8BITS_ABGR (5)
#endif

#ifndef _define___AMP_DISP_OUTPUT_ARGB_8BITS_RGBA
#define _define___AMP_DISP_OUTPUT_ARGB_8BITS_RGBA
#define AMP_DISP_OUTPUT_ARGB_8BITS_RGBA (6)
#endif

#ifndef _define___AMP_DISP_OUTPUT_ARGB_8BITS_BGRA
#define _define___AMP_DISP_OUTPUT_ARGB_8BITS_BGRA
#define AMP_DISP_OUTPUT_ARGB_8BITS_BGRA (7)
#endif

#ifndef _define___AMP_DISP_OUTPUT_UNSUPPORT_FORMAT
#define _define___AMP_DISP_OUTPUT_UNSUPPORT_FORMAT
#define AMP_DISP_OUTPUT_UNSUPPORT_FORMAT (8)
#endif

#ifndef _typedef___AMP_DISP_COLOR_OUTPUT_FMT_TYPE
#define _typedef___AMP_DISP_COLOR_OUTPUT_FMT_TYPE
typedef CORBA_enum AMP_DISP_COLOR_OUTPUT_FMT_TYPE;
#endif /* _typedef___AMP_DISP_COLOR_OUTPUT_FMT_TYPE */

#ifndef _define___AMP_DISP_PLANE_MODE_SHARED
#define _define___AMP_DISP_PLANE_MODE_SHARED
#define AMP_DISP_PLANE_MODE_SHARED (0)
#endif

#ifndef _define___AMP_DISP_PLANE_MODE_EXCLUSIVE
#define _define___AMP_DISP_PLANE_MODE_EXCLUSIVE
#define AMP_DISP_PLANE_MODE_EXCLUSIVE (1)
#endif

#ifndef _define___AMP_DISP_PLANE_MODE_UNDEF
#define _define___AMP_DISP_PLANE_MODE_UNDEF
#define AMP_DISP_PLANE_MODE_UNDEF (2)
#endif

#ifndef _typedef___AMP_DISP_PLANE_MODE
#define _typedef___AMP_DISP_PLANE_MODE
typedef CORBA_enum AMP_DISP_PLANE_MODE;
#endif /* _typedef___AMP_DISP_PLANE_MODE */

#ifndef _define___AMP_DISP_ROTATION_MODE_0D
#define _define___AMP_DISP_ROTATION_MODE_0D
#define AMP_DISP_ROTATION_MODE_0D (0)
#endif

#ifndef _define___AMP_DISP_ROTATION_MODE_90D
#define _define___AMP_DISP_ROTATION_MODE_90D
#define AMP_DISP_ROTATION_MODE_90D (1)
#endif

#ifndef _define___AMP_DISP_ROTATION_MODE_180D
#define _define___AMP_DISP_ROTATION_MODE_180D
#define AMP_DISP_ROTATION_MODE_180D (2)
#endif

#ifndef _define___AMP_DISP_ROTATION_MODE_270D
#define _define___AMP_DISP_ROTATION_MODE_270D
#define AMP_DISP_ROTATION_MODE_270D (3)
#endif

#ifndef _define___AMP_DISP_ROTATION_MODE_MAX
#define _define___AMP_DISP_ROTATION_MODE_MAX
#define AMP_DISP_ROTATION_MODE_MAX (4)
#endif

#ifndef _typedef___AMP_DISP_ROTATION_MODE
#define _typedef___AMP_DISP_ROTATION_MODE
typedef CORBA_enum AMP_DISP_ROTATION_MODE;
#endif /* _typedef___AMP_DISP_ROTATION_MODE */

#ifndef _typedef___AMP_DISP_CAP_FRM_SETTING
#define _typedef___AMP_DISP_CAP_FRM_SETTING
typedef struct AMP_DISP_CAP_FRM_SETTING {
  AMP_DISP_PLANE_ID PlaneId;
  struct AMP_DISP_WIN CapWin;
  UINT32 pFrmBuf;
  UINT32 FrmBufSize;
} AMP_DISP_CAP_FRM_SETTING;
#endif /* _typedef___AMP_DISP_CAP_FRM_SETTING */

#ifndef _typedef___AMP_DISP_CAP_FRM_INFO
#define _typedef___AMP_DISP_CAP_FRM_INFO
typedef struct AMP_DISP_CAP_FRM_INFO {
  UINT16 uiWidth;
  UINT16 uiHeight;
  UINT16 uiStride;
  AMP_DISP_COLOR_PRI_TYPE ColorPrimary;
  AMP_DISP_COLOR_OUTPUT_FMT_TYPE ColorFormat;
  AMP_DISP_VID_3D_FMT_TYPE Video3DFormat;
} AMP_DISP_CAP_FRM_INFO;
#endif /* _typedef___AMP_DISP_CAP_FRM_INFO */

#ifndef _typedef___AMP_DISP_PLANE_CAP_INFO
#define _typedef___AMP_DISP_PLANE_CAP_INFO
typedef struct AMP_DISP_PLANE_CAP_INFO {
  struct AMP_DISP_CAP_FRM_INFO VideoInfo;
  UINT16 uiFrameRate;
} AMP_DISP_PLANE_CAP_INFO;
#endif /* _typedef___AMP_DISP_PLANE_CAP_INFO */

#ifndef _typedef___AMP_DISP_WIN_RATIO
#define _typedef___AMP_DISP_WIN_RATIO
typedef struct AMP_DISP_WIN_RATIO {
  UINT16 uiTop;
  UINT16 uiBottom;
  UINT16 uiLeft;
  UINT16 uiRight;
} AMP_DISP_WIN_RATIO;
#endif /* _typedef___AMP_DISP_WIN_RATIO */

#ifndef _typedef___AMP_DISP_BUILD_IN_FRAME_TYPE
#define _typedef___AMP_DISP_BUILD_IN_FRAME_TYPE
typedef INT32 AMP_DISP_BUILD_IN_FRAME_TYPE;
#endif /* _typedef___AMP_DISP_BUILD_IN_FRAME_TYPE */

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_TYPE_FIRST
#define _define___AMP_DISP_BUILD_IN_FRAME_TYPE_FIRST
#define AMP_DISP_BUILD_IN_FRAME_TYPE_FIRST (0)
#endif

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_BLACK
#define _define___AMP_DISP_BUILD_IN_FRAME_BLACK
#define AMP_DISP_BUILD_IN_FRAME_BLACK (0)
#endif

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_BLUE
#define _define___AMP_DISP_BUILD_IN_FRAME_BLUE
#define AMP_DISP_BUILD_IN_FRAME_BLUE (1)
#endif

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_BLUE_WHITE_SQUARE
#define _define___AMP_DISP_BUILD_IN_FRAME_BLUE_WHITE_SQUARE
#define AMP_DISP_BUILD_IN_FRAME_BLUE_WHITE_SQUARE (2)
#endif

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_TEST_PATTERN
#define _define___AMP_DISP_BUILD_IN_FRAME_TEST_PATTERN
#define AMP_DISP_BUILD_IN_FRAME_TEST_PATTERN (3)
#endif

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_REPEAT_LAST
#define _define___AMP_DISP_BUILD_IN_FRAME_REPEAT_LAST
#define AMP_DISP_BUILD_IN_FRAME_REPEAT_LAST (4)
#endif

#ifndef _define___AMP_DISP_BUILD_IN_FRAME_TYPE_MAX
#define _define___AMP_DISP_BUILD_IN_FRAME_TYPE_MAX
#define AMP_DISP_BUILD_IN_FRAME_TYPE_MAX (5)
#endif

#ifndef _typedef___AMP_DISP_BOOL
#define _typedef___AMP_DISP_BOOL
typedef UINT8 AMP_DISP_BOOL;
#endif /* _typedef___AMP_DISP_BOOL */

#ifndef _typedef___AMP_DISP_HDMI_RES_INFO
#define _typedef___AMP_DISP_HDMI_RES_INFO
typedef struct AMP_DISP_HDMI_RES_INFO {
  INT32 ihActive;
  INT32 ivActive;
  INT32 iRefreshRate;
  INT32 iInterlaced;
  INT32 iAspRatio;
} AMP_DISP_HDMI_RES_INFO;
#endif /* _typedef___AMP_DISP_HDMI_RES_INFO */

#ifndef _typedef___AMP_DISP_HDMI_PIXEL_REPT_INFO
#define _typedef___AMP_DISP_HDMI_PIXEL_REPT_INFO
typedef struct AMP_DISP_HDMI_PIXEL_REPT_INFO {
  UINT32 uiResInfo;
} AMP_DISP_HDMI_PIXEL_REPT_INFO;
#endif /* _typedef___AMP_DISP_HDMI_PIXEL_REPT_INFO */

#ifndef _typedef___AMP_DISP_HDMI_CALORIMETRY_INFO
#define _typedef___AMP_DISP_HDMI_CALORIMETRY_INFO
typedef struct AMP_DISP_HDMI_CALORIMETRY_INFO {
  UINT16 CalorimetryInfo;
} AMP_DISP_HDMI_CALORIMETRY_INFO;
#endif /* _typedef___AMP_DISP_HDMI_CALORIMETRY_INFO */

#ifndef _typedef___AMP_DISP_HDMI_SPKR_ALLOC
#define _typedef___AMP_DISP_HDMI_SPKR_ALLOC
typedef struct AMP_DISP_HDMI_SPKR_ALLOC {
  UINT8 SpkrAllocInfo;
} AMP_DISP_HDMI_SPKR_ALLOC;
#endif /* _typedef___AMP_DISP_HDMI_SPKR_ALLOC */

#ifndef _typedef___AMP_DISP_HDMI_AUDIO_FREQ_SPRT
#define _typedef___AMP_DISP_HDMI_AUDIO_FREQ_SPRT
typedef struct AMP_DISP_HDMI_AUDIO_FREQ_SPRT {
  UINT8 uiAudioFreqSprtInfo;
} AMP_DISP_HDMI_AUDIO_FREQ_SPRT;
#endif /* _typedef___AMP_DISP_HDMI_AUDIO_FREQ_SPRT */

#ifndef _typedef___AMP_DISP_HDMI_AUDIO_WDLEN_SPRT
#define _typedef___AMP_DISP_HDMI_AUDIO_WDLEN_SPRT
typedef struct AMP_DISP_HDMI_AUDIO_WDLEN_SPRT {
  UINT8 uiAudioWdlenSprtInfo;
} AMP_DISP_HDMI_AUDIO_WDLEN_SPRT;
#endif /* _typedef___AMP_DISP_HDMI_AUDIO_WDLEN_SPRT */

#ifndef _typedef___AMP_DISP_HDMI_AUDIO_INFO
#define _typedef___AMP_DISP_HDMI_AUDIO_INFO
typedef struct AMP_DISP_HDMI_AUDIO_INFO {
  INT32 iAudioFmt;
  struct AMP_DISP_HDMI_AUDIO_FREQ_SPRT FreqSprt;
  UINT32 uiMaxBitRate;
  struct AMP_DISP_HDMI_AUDIO_WDLEN_SPRT WdLenSprt;
  UINT8 uiMaxNumChnls;
} AMP_DISP_HDMI_AUDIO_INFO;
#endif /* _typedef___AMP_DISP_HDMI_AUDIO_INFO */

#ifndef _typedef___AMP_DISP_HDMI_CAPABILITY_INFO
#define _typedef___AMP_DISP_HDMI_CAPABILITY_INFO
typedef struct AMP_DISP_HDMI_CAPABILITY_INFO {
  UINT8 uiCapabilityInfo;
} AMP_DISP_HDMI_CAPABILITY_INFO;
#endif /* _typedef___AMP_DISP_HDMI_CAPABILITY_INFO */

#ifndef _typedef___AMP_DISP_HDMI_YCbCr420_CAP_INFO
#define _typedef___AMP_DISP_HDMI_YCbCr420_CAP_INFO
typedef struct AMP_DISP_HDMI_YCbCr420_CAP_INFO {
  UINT8 uiVideoIdCode[16];
} AMP_DISP_HDMI_YCbCr420_CAP_INFO;
#endif /* _typedef___AMP_DISP_HDMI_YCbCr420_CAP_INFO */

#ifndef _typedef___AMP_DISP_HDMI_YCbCr420_CAP_MAP_INFO
#define _typedef___AMP_DISP_HDMI_YCbCr420_CAP_MAP_INFO
typedef struct AMP_DISP_HDMI_YCbCr420_CAP_MAP_INFO {
  UINT8 uiVideoIdCode[31];
} AMP_DISP_HDMI_YCbCr420_CAP_MAP_INFO;
#endif /* _typedef___AMP_DISP_HDMI_YCbCr420_CAP_MAP_INFO */

#ifndef _typedef___AMP_DISP_EDID_CEA_HFVSDB_FEATURE_SUPPORT
#define _typedef___AMP_DISP_EDID_CEA_HFVSDB_FEATURE_SUPPORT
typedef struct AMP_DISP_EDID_CEA_HFVSDB_FEATURE_SUPPORT {
  UINT8 uiHFVSDBFeatureSupported;
} AMP_DISP_EDID_CEA_HFVSDB_FEATURE_SUPPORT;
#endif /* _typedef___AMP_DISP_EDID_CEA_HFVSDB_FEATURE_SUPPORT */

#ifndef _typedef___AMP_DISP_EDID_CEA_HFVSDB_DEEPCOL_SUPPORT
#define _typedef___AMP_DISP_EDID_CEA_HFVSDB_DEEPCOL_SUPPORT
typedef struct AMP_DISP_EDID_CEA_HFVSDB_DEEPCOL_SUPPORT {
  UINT8 uiHFVSDBDeepcolsupported;
} AMP_DISP_EDID_CEA_HFVSDB_DEEPCOL_SUPPORT;
#endif /* _typedef___AMP_DISP_EDID_CEA_HFVSDB_DEEPCOL_SUPPORT */

#ifndef _typedef___AMP_DISP_HDMI_DOLBYVISION_CAP_INFO
#define _typedef___AMP_DISP_HDMI_DOLBYVISION_CAP_INFO
typedef struct AMP_DISP_HDMI_DOLBYVISION_CAP_INFO {
  UINT8 uiDolbyVisionSupport;
  UINT32 uiMaxLum;
  UINT32 uiMinLum;
} AMP_DISP_HDMI_DOLBYVISION_CAP_INFO;
#endif /* _typedef___AMP_DISP_HDMI_DOLBYVISION_CAP_INFO */

#ifndef _typedef___AMP_DISP_EDID_CEA_HFVSDB
#define _typedef___AMP_DISP_EDID_CEA_HFVSDB
typedef struct AMP_DISP_EDID_CEA_HFVSDB {
  UINT8 uiVendorSpecificTagCode;
  UINT8 uiLength;
  UINT8 uiIEEEOUIThirdOctet;
  UINT8 uiIEEEOUISecondOctet;
  UINT8 uiIEEEOUIFirstOctet;
  UINT8 uiVersion;
  UINT8 uiMAXTMDSCharacterRate;
  struct AMP_DISP_EDID_CEA_HFVSDB_FEATURE_SUPPORT HfvsdbFeatures;
  struct AMP_DISP_EDID_CEA_HFVSDB_DEEPCOL_SUPPORT HfVsdbDeepCol;
} AMP_DISP_EDID_CEA_HFVSDB;
#endif /* _typedef___AMP_DISP_EDID_CEA_HFVSDB */

#ifndef _typedef___AMP_DISP_HDMI_HDR_METADATA_INFO
#define _typedef___AMP_DISP_HDMI_HDR_METADATA_INFO
typedef struct AMP_DISP_HDMI_HDR_METADATA_INFO {
  UINT8 uiEOTF_ET_0;
  UINT8 uiEOTF_SM_0;
  UINT32 uiDesCntMaxLuminanceData;
  UINT32 uiDesCntMaxFrmAvgLuminanceData;
  UINT32 uiDesCntMinLuminanceData;
} AMP_DISP_HDMI_HDR_METADATA_INFO;
#endif /* _typedef___AMP_DISP_HDMI_HDR_METADATA_INFO */

#ifndef _typedef___AMP_DISP_HDMI_HDR_CAP_INFO
#define _typedef___AMP_DISP_HDMI_HDR_CAP_INFO
typedef struct AMP_DISP_HDMI_HDR_CAP_INFO {
  UINT8 uiHdmiHdrCapInfo;
  struct AMP_DISP_HDMI_HDR_METADATA_INFO uihdr10Info;
  struct AMP_DISP_HDMI_DOLBYVISION_CAP_INFO uiDolbyCapInfo;
} AMP_DISP_HDMI_HDR_CAP_INFO;
#endif /* _typedef___AMP_DISP_HDMI_HDR_CAP_INFO */

#ifndef _define___AMP_DISP_HDMI_LATENCY_STS_NOT_PRESENT
#define _define___AMP_DISP_HDMI_LATENCY_STS_NOT_PRESENT
#define AMP_DISP_HDMI_LATENCY_STS_NOT_PRESENT (0)
#endif

#ifndef _define___AMP_DISP_HDMI_LATENCY_STS_INVALID
#define _define___AMP_DISP_HDMI_LATENCY_STS_INVALID
#define AMP_DISP_HDMI_LATENCY_STS_INVALID (1)
#endif

#ifndef _define___AMP_DISP_HDMI_LATENCY_STS_UNSUPP_AV
#define _define___AMP_DISP_HDMI_LATENCY_STS_UNSUPP_AV
#define AMP_DISP_HDMI_LATENCY_STS_UNSUPP_AV (2)
#endif

#ifndef _define___AMP_DISP_HDMI_LATENCY_STS_VALID
#define _define___AMP_DISP_HDMI_LATENCY_STS_VALID
#define AMP_DISP_HDMI_LATENCY_STS_VALID (3)
#endif

#ifndef _typedef___AMP_DISP_HDMI_LATENCY_STS_TYPE
#define _typedef___AMP_DISP_HDMI_LATENCY_STS_TYPE
typedef CORBA_enum AMP_DISP_HDMI_LATENCY_STS_TYPE;
#endif /* _typedef___AMP_DISP_HDMI_LATENCY_STS_TYPE */

#ifndef _typedef___AMP_DISP_HDMI_LATENCY_INFO
#define _typedef___AMP_DISP_HDMI_LATENCY_INFO
typedef struct AMP_DISP_HDMI_LATENCY_INFO {
  AMP_DISP_HDMI_LATENCY_STS_TYPE prgVidLatencySts;
  UINT32 prgVidLatencyValue;
  AMP_DISP_HDMI_LATENCY_STS_TYPE prgAudLatencySts;
  UINT32 prgAudLatencyValue;
  AMP_DISP_HDMI_LATENCY_STS_TYPE intVidLatencySts;
  UINT32 intVidLatencyValue;
  AMP_DISP_HDMI_LATENCY_STS_TYPE intAudLatencySts;
  UINT32 intAudLatencyValue;
} AMP_DISP_HDMI_LATENCY_INFO;
#endif /* _typedef___AMP_DISP_HDMI_LATENCY_INFO */

#ifndef _typedef___AMP_DISP_HDMI_DOLBY_AUD_CAP
#define _typedef___AMP_DISP_HDMI_DOLBY_AUD_CAP
typedef struct AMP_DISP_HDMI_DOLBY_AUD_CAP {
  UINT8 uiVsadbPresent;
  UINT8 uiVsadbConfig;
  UINT8 uiSinkDevCap;
} AMP_DISP_HDMI_DOLBY_AUD_CAP;
#endif /* _typedef___AMP_DISP_HDMI_DOLBY_AUD_CAP */

#ifndef _typedef___AMP_DISP_HDMI_SINK_CAPS
#define _typedef___AMP_DISP_HDMI_SINK_CAPS
typedef struct AMP_DISP_HDMI_SINK_CAPS {
  AMP_DISP_BOOL bValidEdid;
  UINT8 uiMonitorName[14];
  UINT8 uiMnfrName[4];
  UINT8 uiProductCode[5];
  UINT8 uiMnfrWeek;
  UINT16 uiMnfrYear;
  UINT32 uiSerialNum;
  UINT8 uiVersion;
  UINT8 uiRevision;
  UINT8 uiMinVRate;
  UINT8 uiMaxVRate;
  UINT8 uiMinHRate;
  UINT8 uiMaxHRate;
  UINT32 uiMaxPixelClock;
  UINT16 uiMaxHSize;
  UINT16 uiMaxVSize;
  CORBA_float Gamma;
  UINT32 uiResCnt;
  struct AMP_DISP_HDMI_RES_INFO ResInfo[64];
  AMP_DISP_BOOL bHdmiMode;
  UINT32 uiSprtedDispRes;
  AMP_DISP_BOOL bSupport3D;
  UINT32 uiSprted3DDispRes;
  UINT32 uiSprted3DStructures[32];
  UINT32 uiHdmiVICLen;
  UINT32 uiSprtedHdmiVICRes;
  struct AMP_DISP_HDMI_RES_INFO PrefResInfo;
  UINT8 uiYCbCr444Sprt;
  UINT8 uiYCbCr422Sprt;
  UINT8 uiDC30bitSprt;
  UINT8 uiDC36bitSprt;
  UINT8 uiYCbCr444AtDC;
  UINT8 uiAISprt;
  struct AMP_DISP_HDMI_PIXEL_REPT_INFO PrInfo[4];
  struct AMP_DISP_HDMI_CALORIMETRY_INFO CalInfo;
  struct AMP_DISP_HDMI_YCbCr420_CAP_INFO YCbCr420CapInfo;
  struct AMP_DISP_HDMI_SPKR_ALLOC SpkrAlloc;
  struct AMP_DISP_HDMI_AUDIO_INFO AudioInfo[15];
  UINT16 uiCecPhyAddr;
  UINT16 uiMaxTMDSClock;
  struct AMP_DISP_EDID_CEA_HFVSDB CeaHdmiVSDB;
  struct AMP_DISP_HDMI_YCbCr420_CAP_MAP_INFO YCbCr420capmapInfo;
  UINT16 uiSvdBlockLen;
  UINT16 uiSvdVics[31];
  struct AMP_DISP_HDMI_LATENCY_INFO latencyInfo;
  struct AMP_DISP_HDMI_HDR_CAP_INFO uiHdrCapInfo;
  struct AMP_DISP_HDMI_DOLBY_AUD_CAP DolbyAudioCap;
} AMP_DISP_HDMI_SINK_CAPS;
#endif /* _typedef___AMP_DISP_HDMI_SINK_CAPS */

#ifndef _typedef___AMP_DISP_HDMI_RAW_EDID
#define _typedef___AMP_DISP_HDMI_RAW_EDID
typedef struct AMP_DISP_HDMI_RAW_EDID {
  UINT8 uiDataBuf[128];
  UINT8 uiDataBufExt[128];
  UINT8 uiIsValid;
} AMP_DISP_HDMI_RAW_EDID;
#endif /* _typedef___AMP_DISP_HDMI_RAW_EDID */

#ifndef _typedef___AMP_DISP_HDMI_PKT_ACP
#define _typedef___AMP_DISP_HDMI_PKT_ACP
typedef struct AMP_DISP_HDMI_PKT_ACP {
  INT32 iType;
  UINT8 uiDataLen;
  UINT8 uiDataBuf[16];
} AMP_DISP_HDMI_PKT_ACP;
#endif /* _typedef___AMP_DISP_HDMI_PKT_ACP */

#ifndef _typedef___AMP_DISP_HDMI_PKT_GMD
#define _typedef___AMP_DISP_HDMI_PKT_GMD
typedef struct AMP_DISP_HDMI_PKT_GMD {
  AMP_DISP_BOOL bNextField;
  AMP_DISP_BOOL bNoCurGBD;
  INT32 iGdbProfile;
  INT32 iSeqInfo;
  UINT8 uiAffectedGamutSeqNum;
  UINT8 uiCurGamutSeqNum;
  UINT8 uiGbdData[28];
} AMP_DISP_HDMI_PKT_GMD;
#endif /* _typedef___AMP_DISP_HDMI_PKT_GMD */

#ifndef _typedef___AMP_DISP_HDMI_PKT_ISRC1
#define _typedef___AMP_DISP_HDMI_PKT_ISRC1
typedef struct AMP_DISP_HDMI_PKT_ISRC1 {
  AMP_DISP_BOOL bCont;
  INT32 iSts;
  AMP_DISP_BOOL bValid;
  UINT8 uiUpcEanFld[16];
} AMP_DISP_HDMI_PKT_ISRC1;
#endif /* _typedef___AMP_DISP_HDMI_PKT_ISRC1 */

#ifndef _typedef___AMP_DISP_HDMI_PKT_ISRC2
#define _typedef___AMP_DISP_HDMI_PKT_ISRC2
typedef struct AMP_DISP_HDMI_PKT_ISRC2 {
  UINT8 uiUpcEanFld[16];
} AMP_DISP_HDMI_PKT_ISRC2;
#endif /* _typedef___AMP_DISP_HDMI_PKT_ISRC2 */

#ifndef _typedef___AMP_DISP_HDMI_PKT_SPD_INFOFRM
#define _typedef___AMP_DISP_HDMI_PKT_SPD_INFOFRM
typedef struct AMP_DISP_HDMI_PKT_SPD_INFOFRM {
  UINT8 uiVendorName[8];
  UINT8 uiProdDescChar[16];
  INT32 iSrcDev;
} AMP_DISP_HDMI_PKT_SPD_INFOFRM;
#endif /* _typedef___AMP_DISP_HDMI_PKT_SPD_INFOFRM */

#ifndef _typedef___AMP_DISP_HDMI_PKT_MPEG_SRC_INFOFRM
#define _typedef___AMP_DISP_HDMI_PKT_MPEG_SRC_INFOFRM
typedef struct AMP_DISP_HDMI_PKT_MPEG_SRC_INFOFRM {
  UINT32 uiBitRate;
  INT32 iMpegFrameType;
  AMP_DISP_BOOL bRepeatedField;
} AMP_DISP_HDMI_PKT_MPEG_SRC_INFOFRM;
#endif /* _typedef___AMP_DISP_HDMI_PKT_MPEG_SRC_INFOFRM */

#ifndef _typedef___AMP_DISP_HDMI_PKT_VNDRSPEC_INFOFRM
#define _typedef___AMP_DISP_HDMI_PKT_VNDRSPEC_INFOFRM
typedef struct AMP_DISP_HDMI_PKT_VNDRSPEC_INFOFRM {
  UINT32 uiIeeeRegID;
  UINT8 uiPacketLength;
  UINT8 uiHdmiVideoFmt;
  UINT8 uiHdmiVic;
  UINT8 uiHdmi3DStructure;
  AMP_DISP_BOOL bHdmi3DMetaPresent;
  UINT8 uiHdmi3DExtData;
  UINT8 uiHdmi3DMetadataType;
  UINT8 uiHdmi3DMetadataLength;
  UINT8 uiHdmi3DMetadata[16];
} AMP_DISP_HDMI_PKT_VNDRSPEC_INFOFRM;
#endif /* _typedef___AMP_DISP_HDMI_PKT_VNDRSPEC_INFOFRM */

#ifndef _typedef___AMP_DISP_HDMI_PKT_HDR_INFOFRM
#define _typedef___AMP_DISP_HDMI_PKT_HDR_INFOFRM
typedef struct AMP_DISP_HDMI_PKT_HDR_INFOFRM {
  UINT8 uiEOTF;
  UINT8 uiStatic_metadata_Des_ID;
  UINT16 uidisplay_primaries_x[3];
  UINT16 uidisplay_primaries_y[3];
  UINT16 uiwhite_point_x;
  UINT16 uiwhite_point_y;
  UINT16 uimax_display_mastering_lumi;
  UINT16 uimin_display_mastering_lumi;
  UINT16 uiMaximum_Content_Light_Lvl;
  UINT16 uiMaximum_Frame_Average_Light_Lvl;
} AMP_DISP_HDMI_PKT_HDR_INFOFRM;
#endif /* _typedef___AMP_DISP_HDMI_PKT_HDR_INFOFRM */

#ifndef _typedef___AMP_DISP_HDMI_PKT
#define _typedef___AMP_DISP_HDMI_PKT
typedef struct AMP_DISP_HDMI_PKT {
  UINT32 _d;
  union {
    struct AMP_DISP_HDMI_PKT_ACP AcpPkt;
    struct AMP_DISP_HDMI_PKT_GMD GmdPkt;
    struct AMP_DISP_HDMI_PKT_ISRC1 Isrc1Pkt;
    struct AMP_DISP_HDMI_PKT_ISRC2 Isrc2Pkt;
    struct AMP_DISP_HDMI_PKT_SPD_INFOFRM SpdInfoFrm;
    struct AMP_DISP_HDMI_PKT_MPEG_SRC_INFOFRM MpegSrcInfoFrm;
    struct AMP_DISP_HDMI_PKT_VNDRSPEC_INFOFRM VndrSpecInfoFrm;
    struct AMP_DISP_HDMI_PKT_HDR_INFOFRM HdrInfoFrm;
  } _u;
} AMP_DISP_HDMI_PKT;
#endif /* _typedef___AMP_DISP_HDMI_PKT */

#ifndef _typedef___AMP_DISP_HDMI_PHY_SETTING
#define _typedef___AMP_DISP_HDMI_PHY_SETTING
typedef struct AMP_DISP_HDMI_PHY_SETTING {
  UINT16 uiDAMP;
  UINT16 uiEAMP;
  UINT16 uiIDRV;
  UINT8 uiTXDRVX2;
} AMP_DISP_HDMI_PHY_SETTING;
#endif /* _typedef___AMP_DISP_HDMI_PHY_SETTING */

#ifndef _typedef___AMP_DISP_HDMI_PKT_RAW
#define _typedef___AMP_DISP_HDMI_PKT_RAW
typedef struct AMP_DISP_HDMI_PKT_RAW {
  UINT8 uipkt[31];
} AMP_DISP_HDMI_PKT_RAW;
#endif /* _typedef___AMP_DISP_HDMI_PKT_RAW */

#ifndef _typedef___AMP_DISP_HDMI_REG_DUMP
#define _typedef___AMP_DISP_HDMI_REG_DUMP
typedef struct AMP_DISP_HDMI_REG_DUMP {
  UINT32 uiRegAddr[425];
  UINT32 uiRegVal[425];
} AMP_DISP_HDMI_REG_DUMP;
#endif /* _typedef___AMP_DISP_HDMI_REG_DUMP */

#ifndef _typedef___AMP_DISP_HDCP_BSTATUS
#define _typedef___AMP_DISP_HDCP_BSTATUS
typedef struct AMP_DISP_HDCP_BSTATUS {
  UINT16 uiDepth;
  UINT16 uiMaxDeviceExceeded;
  UINT16 uiMaxCascadeExceeded;
} AMP_DISP_HDCP_BSTATUS;
#endif /* _typedef___AMP_DISP_HDCP_BSTATUS */

#ifndef _typedef___AMP_DISP_HDCP_AKSV_DATA
#define _typedef___AMP_DISP_HDCP_AKSV_DATA
typedef struct AMP_DISP_HDCP_AKSV_DATA {
  UINT8 uiAKsvBuf[5];
} AMP_DISP_HDCP_AKSV_DATA;
#endif /* _typedef___AMP_DISP_HDCP_AKSV_DATA */

#ifndef _typedef___AMP_DISP_HDCP_KSV_DATA
#define _typedef___AMP_DISP_HDCP_KSV_DATA
typedef struct AMP_DISP_HDCP_KSV_DATA {
  UINT8 uiKsvBuf[640];
  UINT32 uiKsvLen;
  struct AMP_DISP_HDCP_BSTATUS BStatus;
} AMP_DISP_HDCP_KSV_DATA;
#endif /* _typedef___AMP_DISP_HDCP_KSV_DATA */

#ifndef _typedef___AMP_DISP_HDCP_SRM_DATA
#define _typedef___AMP_DISP_HDCP_SRM_DATA
typedef struct AMP_DISP_HDCP_SRM_DATA {
  UINT32 hdataShm;
  UINT32 uiBufLen;
} AMP_DISP_HDCP_SRM_DATA;
#endif /* _typedef___AMP_DISP_HDCP_SRM_DATA */

#ifndef _typedef___AMP_DISP_HDCP_SRM_ERROR
#define _typedef___AMP_DISP_HDCP_SRM_ERROR
typedef UINT32 AMP_DISP_HDCP_SRM_ERROR;
#endif /* _typedef___AMP_DISP_HDCP_SRM_ERROR */

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_NONE
#define _define___AMP_DISP_HDCP_SRM_ERROR_NONE
#define AMP_DISP_HDCP_SRM_ERROR_NONE (0)
#endif

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_UNSUPP_GENERATION
#define _define___AMP_DISP_HDCP_SRM_ERROR_UNSUPP_GENERATION
#define AMP_DISP_HDCP_SRM_ERROR_UNSUPP_GENERATION (1)
#endif

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_INTEGRITY_CHK_FAIL
#define _define___AMP_DISP_HDCP_SRM_ERROR_INTEGRITY_CHK_FAIL
#define AMP_DISP_HDCP_SRM_ERROR_INTEGRITY_CHK_FAIL (2)
#endif

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_LOW_VERSION
#define _define___AMP_DISP_HDCP_SRM_ERROR_LOW_VERSION
#define AMP_DISP_HDCP_SRM_ERROR_LOW_VERSION (3)
#endif

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_INVALID_DATA
#define _define___AMP_DISP_HDCP_SRM_ERROR_INVALID_DATA
#define AMP_DISP_HDCP_SRM_ERROR_INVALID_DATA (4)
#endif

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_INVALID_STATE
#define _define___AMP_DISP_HDCP_SRM_ERROR_INVALID_STATE
#define AMP_DISP_HDCP_SRM_ERROR_INVALID_STATE (5)
#endif

#ifndef _define___AMP_DISP_HDCP_SRM_ERROR_NO_MEMORY
#define _define___AMP_DISP_HDCP_SRM_ERROR_NO_MEMORY
#define AMP_DISP_HDCP_SRM_ERROR_NO_MEMORY (6)
#endif

#ifndef _typedef___AMP_DISP_HDCP_SRM_INFO
#define _typedef___AMP_DISP_HDCP_SRM_INFO
typedef struct AMP_DISP_HDCP_SRM_INFO {
  UINT8 valid;
  UINT8 id;
  UINT16 version;
  UINT16 generation;
  UINT32 updated;
  UINT32 errorInfo;
} AMP_DISP_HDCP_SRM_INFO;
#endif /* _typedef___AMP_DISP_HDCP_SRM_INFO */

#ifndef _define___AMP_DISP_EARC_STATUS_MODE_UPDATE_EVENT
#define _define___AMP_DISP_EARC_STATUS_MODE_UPDATE_EVENT
#define AMP_DISP_EARC_STATUS_MODE_UPDATE_EVENT (0)
#endif

#ifndef _define___AMP_DISP_EARC_STATUS_OUTPUT_MUTED_EVENT
#define _define___AMP_DISP_EARC_STATUS_OUTPUT_MUTED_EVENT
#define AMP_DISP_EARC_STATUS_OUTPUT_MUTED_EVENT (1)
#endif

#ifndef _define___AMP_DISP_EARC_STATUS_ERROR_EVENT
#define _define___AMP_DISP_EARC_STATUS_ERROR_EVENT
#define AMP_DISP_EARC_STATUS_ERROR_EVENT (2)
#endif

#ifndef _typedef___AMP_DISP_EARC_STATUS_EVENT_TYPE
#define _typedef___AMP_DISP_EARC_STATUS_EVENT_TYPE
typedef CORBA_enum AMP_DISP_EARC_STATUS_EVENT_TYPE;
#endif /* _typedef___AMP_DISP_EARC_STATUS_EVENT_TYPE */

#ifndef _define___AMP_DISP_EARC_DISCOVERY_TIMEDOUT
#define _define___AMP_DISP_EARC_DISCOVERY_TIMEDOUT
#define AMP_DISP_EARC_DISCOVERY_TIMEDOUT (0)
#endif

#ifndef _typedef___AMP_DISP_EARC_STATUS_ERROR_EVENT_CODES
#define _typedef___AMP_DISP_EARC_STATUS_ERROR_EVENT_CODES
typedef CORBA_enum AMP_DISP_EARC_STATUS_ERROR_EVENT_CODES;
#endif /* _typedef___AMP_DISP_EARC_STATUS_ERROR_EVENT_CODES */

#ifndef _define___AMP_DISP_EARC_MODE_NONE
#define _define___AMP_DISP_EARC_MODE_NONE
#define AMP_DISP_EARC_MODE_NONE (0)
#endif

#ifndef _define___AMP_DISP_EARC_MODE_ARC
#define _define___AMP_DISP_EARC_MODE_ARC
#define AMP_DISP_EARC_MODE_ARC (1)
#endif

#ifndef _define___AMP_DISP_EARC_MODE_EARC
#define _define___AMP_DISP_EARC_MODE_EARC
#define AMP_DISP_EARC_MODE_EARC (2)
#endif

#ifndef _typedef___AMP_DISP_EARC_MODE
#define _typedef___AMP_DISP_EARC_MODE
typedef CORBA_enum AMP_DISP_EARC_MODE;
#endif /* _typedef___AMP_DISP_EARC_MODE */

#ifndef _typedef___AMP_DISP_EARC_STATUS_EVENT_DESC
#define _typedef___AMP_DISP_EARC_STATUS_EVENT_DESC
typedef struct AMP_DISP_EARC_STATUS_EVENT_DESC {
  UINT32 _d;
  union {
    AMP_DISP_EARC_MODE UpdatedMode;
    AMP_DISP_BOOL OutputMuted;
    UINT32 ErrorCode;
  } _u;
} AMP_DISP_EARC_STATUS_EVENT_DESC;
#endif /* _typedef___AMP_DISP_EARC_STATUS_EVENT_DESC */

#ifndef _typedef___AMP_DISP_EARC_STATUS_EVENT_INFO
#define _typedef___AMP_DISP_EARC_STATUS_EVENT_INFO
typedef struct AMP_DISP_EARC_STATUS_EVENT_INFO {
  AMP_DISP_EARC_STATUS_EVENT_TYPE Status_Type;
  struct AMP_DISP_EARC_STATUS_EVENT_DESC Status_Desc;
} AMP_DISP_EARC_STATUS_EVENT_INFO;
#endif /* _typedef___AMP_DISP_EARC_STATUS_EVENT_INFO */

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_16
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_16
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_16 (0)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_17
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_17
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_17 (1)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_18
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_18
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_18 (2)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_19
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_19
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_19 (3)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_20
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_20
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_20 (4)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_21
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_21
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_21 (5)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_22
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_22
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_22 (6)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_23
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_23
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_23 (7)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_24
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_24
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_24 (8)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_NOT_INDICATED
#define _define___AMP_DISP_EARC_AUDIO_DATAWIDTH_NOT_INDICATED
#define AMP_DISP_EARC_AUDIO_DATAWIDTH_NOT_INDICATED (9)
#endif

#ifndef _typedef___AMP_DISP_EARC_AUDIO_DATAWIDTH
#define _typedef___AMP_DISP_EARC_AUDIO_DATAWIDTH
typedef CORBA_enum AMP_DISP_EARC_AUDIO_DATAWIDTH;
#endif /* _typedef___AMP_DISP_EARC_AUDIO_DATAWIDTH */

#ifndef _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_2
#define _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_2
#define AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_2 (0)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_4
#define _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_4
#define AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_4 (1)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_6
#define _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_6
#define AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_6 (2)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_8
#define _define___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_8
#define AMP_DISP_EARC_AUDIO_CHANNEL_COUNT_8 (3)
#endif

#ifndef _typedef___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT
#define _typedef___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT
typedef CORBA_enum AMP_DISP_EARC_AUDIO_CHANNEL_COUNT;
#endif /* _typedef___AMP_DISP_EARC_AUDIO_CHANNEL_COUNT */

#ifndef _define___AMP_DISP_EARC_OUT_SPDIF
#define _define___AMP_DISP_EARC_OUT_SPDIF
#define AMP_DISP_EARC_OUT_SPDIF (0)
#endif

#ifndef _define___AMP_DISP_EARC_OUT_I2S
#define _define___AMP_DISP_EARC_OUT_I2S
#define AMP_DISP_EARC_OUT_I2S (1)
#endif

#ifndef _define___AMP_DISP_EARC_OUT_DSD
#define _define___AMP_DISP_EARC_OUT_DSD
#define AMP_DISP_EARC_OUT_DSD (2)
#endif

#ifndef _typedef___AMP_DISP_EARC_OUT_AUDIO_TYPE
#define _typedef___AMP_DISP_EARC_OUT_AUDIO_TYPE
typedef CORBA_enum AMP_DISP_EARC_OUT_AUDIO_TYPE;
#endif /* _typedef___AMP_DISP_EARC_OUT_AUDIO_TYPE */

#ifndef _typedef___AMP_DISP_EARC_OUT_AUDIO_COMMON_DATA
#define _typedef___AMP_DISP_EARC_OUT_AUDIO_COMMON_DATA
typedef struct AMP_DISP_EARC_OUT_AUDIO_COMMON_DATA {
  AMP_DISP_BOOL IsCompressed;
  UINT32 SampleFreq;
  UINT8 SpeakerAlloc;
  UINT8 DmInhLsv;
  AMP_DISP_EARC_AUDIO_DATAWIDTH DataWidth;
} AMP_DISP_EARC_OUT_AUDIO_COMMON_DATA;
#endif /* _typedef___AMP_DISP_EARC_OUT_AUDIO_COMMON_DATA */

#ifndef _typedef___AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA_Audio_Desc_I2s_t
#define _typedef___AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA_Audio_Desc_I2s_t
typedef struct AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA_Audio_Desc_I2s_t {
  AMP_DISP_BOOL SpdifOnI2s;
  AMP_DISP_EARC_AUDIO_CHANNEL_COUNT ChannelCount;
} AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA_Audio_Desc_I2s_t;
#endif /* _typedef___AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA_Audio_Desc_I2s_t */

#ifndef _typedef___AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA
#define _typedef___AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA
typedef struct AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA {
  UINT32 _d;
  union {
    struct AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA_Audio_Desc_I2s_t Audio_Desc_I2s;
  } _u;
} AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA;
#endif /* _typedef___AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA */

#ifndef _typedef___AMP_DISP_EARC_OUT_AUDIO_DESC
#define _typedef___AMP_DISP_EARC_OUT_AUDIO_DESC
typedef struct AMP_DISP_EARC_OUT_AUDIO_DESC {
  struct AMP_DISP_EARC_OUT_AUDIO_COMMON_DATA Common;
  struct AMP_DISP_EARC_OUT_AUDIO_SPECIFIC_DATA TypeSpecific;
} AMP_DISP_EARC_OUT_AUDIO_DESC;
#endif /* _typedef___AMP_DISP_EARC_OUT_AUDIO_DESC */

#ifndef _typedef___AMP_DISP_EARC_AUDIO_EVENT_INFO
#define _typedef___AMP_DISP_EARC_AUDIO_EVENT_INFO
typedef struct AMP_DISP_EARC_AUDIO_EVENT_INFO {
  AMP_DISP_EARC_OUT_AUDIO_TYPE Audio_Type;
  struct AMP_DISP_EARC_OUT_AUDIO_DESC Audio_Desc;
} AMP_DISP_EARC_AUDIO_EVENT_INFO;
#endif /* _typedef___AMP_DISP_EARC_AUDIO_EVENT_INFO */

#ifndef _define___AMP_DISP_EARC_STATUS_UPDATE
#define _define___AMP_DISP_EARC_STATUS_UPDATE
#define AMP_DISP_EARC_STATUS_UPDATE (0)
#endif

#ifndef _define___AMP_DISP_EARC_AUDIO_CFG
#define _define___AMP_DISP_EARC_AUDIO_CFG
#define AMP_DISP_EARC_AUDIO_CFG (1)
#endif

#ifndef _define___AMP_DISP_EARC_LATENCY_UPDATE_REQ
#define _define___AMP_DISP_EARC_LATENCY_UPDATE_REQ
#define AMP_DISP_EARC_LATENCY_UPDATE_REQ (2)
#endif

#ifndef _typedef___AMP_DISP_EARC_INFO_ID
#define _typedef___AMP_DISP_EARC_INFO_ID
typedef CORBA_enum AMP_DISP_EARC_INFO_ID;
#endif /* _typedef___AMP_DISP_EARC_INFO_ID */

#ifndef _typedef___AMP_DISP_EARC_EVENT_INFO_EARC_EVENT_PAYLOAD
#define _typedef___AMP_DISP_EARC_EVENT_INFO_EARC_EVENT_PAYLOAD
typedef struct AMP_DISP_EARC_EVENT_INFO_EARC_EVENT_PAYLOAD {
  UINT32 _d;
  union {
    struct AMP_DISP_EARC_STATUS_EVENT_INFO StatusEvtInfo;
    struct AMP_DISP_EARC_AUDIO_EVENT_INFO AudioCfgEvtInfo;
    UINT8 ReqLatency;
  } _u;
} AMP_DISP_EARC_EVENT_INFO_EARC_EVENT_PAYLOAD;
#endif /* _typedef___AMP_DISP_EARC_EVENT_INFO_EARC_EVENT_PAYLOAD */

#ifndef _typedef___AMP_DISP_EARC_EVENT_INFO
#define _typedef___AMP_DISP_EARC_EVENT_INFO
typedef struct AMP_DISP_EARC_EVENT_INFO {
  AMP_DISP_EARC_INFO_ID InfoID;
  struct AMP_DISP_EARC_EVENT_INFO_EARC_EVENT_PAYLOAD EvtInfo;
} AMP_DISP_EARC_EVENT_INFO;
#endif /* _typedef___AMP_DISP_EARC_EVENT_INFO */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_FEATURE_ABORT
#define _typedef___AMP_DISP_CEC_OPERAND_FEATURE_ABORT
typedef struct AMP_DISP_CEC_OPERAND_FEATURE_ABORT {
  UINT8 uiFeatureOpcode;
  INT32 iAbortReason;
} AMP_DISP_CEC_OPERAND_FEATURE_ABORT;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_FEATURE_ABORT */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_REPORT_PHY_ADDR
#define _typedef___AMP_DISP_CEC_OPERAND_REPORT_PHY_ADDR
typedef struct AMP_DISP_CEC_OPERAND_REPORT_PHY_ADDR {
  UINT16 uiPhyAddr;
  INT32 iDeviceType;
} AMP_DISP_CEC_OPERAND_REPORT_PHY_ADDR;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_REPORT_PHY_ADDR */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_ARIB_DATA
#define _typedef___AMP_DISP_CEC_OPERAND_ARIB_DATA
typedef struct AMP_DISP_CEC_OPERAND_ARIB_DATA {
  UINT16 uiTxStreamID;
  UINT16 uiContent_srvID;
  UINT16 uiOrigNetworkID;
} AMP_DISP_CEC_OPERAND_ARIB_DATA;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_ARIB_DATA */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_ATSC_DATA
#define _typedef___AMP_DISP_CEC_OPERAND_ATSC_DATA
typedef struct AMP_DISP_CEC_OPERAND_ATSC_DATA {
  UINT16 uiTxStreamID;
  UINT16 uiContent_srvID;
} AMP_DISP_CEC_OPERAND_ATSC_DATA;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_ATSC_DATA */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_DVB_DATA
#define _typedef___AMP_DISP_CEC_OPERAND_DVB_DATA
typedef struct AMP_DISP_CEC_OPERAND_DVB_DATA {
  UINT16 uiTxStreamID;
  UINT16 uiContentSrvID;
  UINT16 uiOrigNetworkID;
} AMP_DISP_CEC_OPERAND_DVB_DATA;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_DVB_DATA */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_CHN_IDEN
#define _typedef___AMP_DISP_CEC_OPERAND_CHN_IDEN
typedef struct AMP_DISP_CEC_OPERAND_CHN_IDEN {
  UINT32 uiCecOperandChnIden;
} AMP_DISP_CEC_OPERAND_CHN_IDEN;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_CHN_IDEN */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_DIG_SRV_ID_CEC_SRC_IDEN
#define _typedef___AMP_DISP_CEC_OPERAND_DIG_SRV_ID_CEC_SRC_IDEN
typedef struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID_CEC_SRC_IDEN {
  UINT32 _d;
  union {
    struct AMP_DISP_CEC_OPERAND_ARIB_DATA AribData;
    struct AMP_DISP_CEC_OPERAND_ATSC_DATA AtscData;
    struct AMP_DISP_CEC_OPERAND_DVB_DATA DvbData;
    struct AMP_DISP_CEC_OPERAND_CHN_IDEN ChnData;
  } _u;
} AMP_DISP_CEC_OPERAND_DIG_SRV_ID_CEC_SRC_IDEN;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_DIG_SRV_ID_CEC_SRC_IDEN */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_DIG_SRV_ID
#define _typedef___AMP_DISP_CEC_OPERAND_DIG_SRV_ID
typedef struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID {
  INT32 iSrvIdenType;
  INT32 iBroadcastSys;
  struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID_CEC_SRC_IDEN SrvIden;
} AMP_DISP_CEC_OPERAND_DIG_SRV_ID;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_DIG_SRV_ID */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID
#define _typedef___AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID
typedef struct AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID {
  INT32 iBroadcastType;
  UINT16 uiFrequency;
  INT32 iBroadcastSystem;
} AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_REC_SRC_CEC_REC_SRC_IDEN
#define _typedef___AMP_DISP_CEC_OPERAND_REC_SRC_CEC_REC_SRC_IDEN
typedef struct AMP_DISP_CEC_OPERAND_REC_SRC_CEC_REC_SRC_IDEN {
  UINT32 _d;
  union {
    struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID DigSrvIden;
    struct AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID AnaSrvIden;
    UINT8 uiExtPlugNum;
    UINT16 uiExtPhyAddr;
  } _u;
} AMP_DISP_CEC_OPERAND_REC_SRC_CEC_REC_SRC_IDEN;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_REC_SRC_CEC_REC_SRC_IDEN */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_REC_SRC
#define _typedef___AMP_DISP_CEC_OPERAND_REC_SRC
typedef struct AMP_DISP_CEC_OPERAND_REC_SRC {
  INT32 iRecSrc;
  struct AMP_DISP_CEC_OPERAND_REC_SRC_CEC_REC_SRC_IDEN RecSrcIden;
} AMP_DISP_CEC_OPERAND_REC_SRC;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_REC_SRC */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_OSD_STRING
#define _typedef___AMP_DISP_CEC_OPERAND_OSD_STRING
typedef struct AMP_DISP_CEC_OPERAND_OSD_STRING {
  INT32 iDispCtrl;
  UINT8 uiOsdStringLen;
  UINT8 uiOsdString[13];
} AMP_DISP_CEC_OPERAND_OSD_STRING;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_OSD_STRING */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_OSD_NAME
#define _typedef___AMP_DISP_CEC_OPERAND_OSD_NAME
typedef struct AMP_DISP_CEC_OPERAND_OSD_NAME {
  UINT8 uiOsdNameLen;
  UINT8 uiOsdName[14];
} AMP_DISP_CEC_OPERAND_OSD_NAME;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_OSD_NAME */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_VENDOR_SPEC_DATA
#define _typedef___AMP_DISP_CEC_OPERAND_VENDOR_SPEC_DATA
typedef struct AMP_DISP_CEC_OPERAND_VENDOR_SPEC_DATA {
  UINT8 uiDataLength;
  UINT32 uiVendorID;
  UINT8 uiVendorData[14];
} AMP_DISP_CEC_OPERAND_VENDOR_SPEC_DATA;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_VENDOR_SPEC_DATA */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO_CEC_SRV_IDEN
#define _typedef___AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO_CEC_SRV_IDEN
typedef struct AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO_CEC_SRV_IDEN {
  UINT32 _d;
  union {
    struct AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID AnalogueSrvIden;
    struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID DigSrvIden;
  } _u;
} AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO_CEC_SRV_IDEN;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO_CEC_SRV_IDEN */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO
#define _typedef___AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO
typedef struct AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO {
  INT32 RecordingFlag;
  INT32 TunerDispInfo;
  struct AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO_CEC_SRV_IDEN SrvIden;
} AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_ANALOGUE_TIMER_INFO
#define _typedef___AMP_DISP_CEC_OPERAND_ANALOGUE_TIMER_INFO
typedef struct AMP_DISP_CEC_OPERAND_ANALOGUE_TIMER_INFO {
  UINT8 uiDayOfMonth;
  UINT8 uiMonthOfYear;
  UINT8 uiStartTimeHour;
  UINT8 uiStartTimeMin;
  UINT8 uiDurationHour;
  UINT8 uiDurationMin;
  INT32 iRecSequence;
  INT32 iBroadcastType;
  UINT16 uiFrequency;
  INT32 iBroadcastSystem;
} AMP_DISP_CEC_OPERAND_ANALOGUE_TIMER_INFO;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_ANALOGUE_TIMER_INFO */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_DIGITAL_TIMER_INFO
#define _typedef___AMP_DISP_CEC_OPERAND_DIGITAL_TIMER_INFO
typedef struct AMP_DISP_CEC_OPERAND_DIGITAL_TIMER_INFO {
  UINT8 uiDayOfMonth;
  UINT8 uiMonthOfYear;
  UINT8 uiStartTimeHour;
  UINT8 uiStartTimeMin;
  UINT8 uiDurationHour;
  UINT8 uiDurationMin;
  INT32 iRecSequence;
  struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID DigSrvID;
} AMP_DISP_CEC_OPERAND_DIGITAL_TIMER_INFO;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_DIGITAL_TIMER_INFO */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_EXT_TIMER_INFO
#define _typedef___AMP_DISP_CEC_OPERAND_EXT_TIMER_INFO
typedef struct AMP_DISP_CEC_OPERAND_EXT_TIMER_INFO {
  UINT8 uiDayOfMonth;
  UINT8 uiMonthOfYear;
  UINT8 uiStartTimeHour;
  UINT8 uiStartTimeMin;
  UINT8 uiDurationHour;
  UINT8 uiDurationMin;
  INT32 iRecSequence;
  INT32 iExtSrcSpec;
  UINT8 uiExtPlugNum;
  UINT16 uiExtPhyAddr;
} AMP_DISP_CEC_OPERAND_EXT_TIMER_INFO;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_EXT_TIMER_INFO */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_TIMER_STATUS_DATA
#define _typedef___AMP_DISP_CEC_OPERAND_TIMER_STATUS_DATA
typedef struct AMP_DISP_CEC_OPERAND_TIMER_STATUS_DATA {
  INT32 iTimerOverLapWarning;
  INT32 iMediaInfo;
  INT32 iPgmedIndicator;
  INT32 iPgmedInfo;
  INT32 iNotPgmedErrInfo;
  UINT8 uiDurationAvailable;
  UINT8 uiAvailableDurHour;
  UINT8 uiAvailableDurMin;
} AMP_DISP_CEC_OPERAND_TIMER_STATUS_DATA;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_TIMER_STATUS_DATA */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_AUDIO_MODE_REQUEST
#define _typedef___AMP_DISP_CEC_OPERAND_AUDIO_MODE_REQUEST
typedef struct AMP_DISP_CEC_OPERAND_AUDIO_MODE_REQUEST {
  AMP_DISP_BOOL AudioMode;
  UINT16 uiPhyAddr;
} AMP_DISP_CEC_OPERAND_AUDIO_MODE_REQUEST;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_AUDIO_MODE_REQUEST */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_AUDIO_STATUS
#define _typedef___AMP_DISP_CEC_OPERAND_AUDIO_STATUS
typedef struct AMP_DISP_CEC_OPERAND_AUDIO_STATUS {
  AMP_DISP_BOOL bAudioMuteStatus;
  UINT8 uiAudioVolStatus;
} AMP_DISP_CEC_OPERAND_AUDIO_STATUS;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_AUDIO_STATUS */

#ifndef _typedef___AMP_DISP_CEC_AUDIO_FMT_WITH_ID
#define _typedef___AMP_DISP_CEC_AUDIO_FMT_WITH_ID
typedef struct AMP_DISP_CEC_AUDIO_FMT_WITH_ID {
  UINT8 uiAudioFmtID;
  UINT8 uiAudioFmtCode;
} AMP_DISP_CEC_AUDIO_FMT_WITH_ID;
#endif /* _typedef___AMP_DISP_CEC_AUDIO_FMT_WITH_ID */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_AUDIO_FMT_WITH_ID
#define _typedef___AMP_DISP_CEC_OPERAND_AUDIO_FMT_WITH_ID
typedef struct AMP_DISP_CEC_OPERAND_AUDIO_FMT_WITH_ID {
  INT32 iNum;
  struct AMP_DISP_CEC_AUDIO_FMT_WITH_ID AudioFmt[4];
} AMP_DISP_CEC_OPERAND_AUDIO_FMT_WITH_ID;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_AUDIO_FMT_WITH_ID */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_AUDIO_SHORT_DESC
#define _typedef___AMP_DISP_CEC_OPERAND_AUDIO_SHORT_DESC
typedef struct AMP_DISP_CEC_OPERAND_AUDIO_SHORT_DESC {
  INT32 iNum;
  UINT8 uiAudioShortDesc[12];
} AMP_DISP_CEC_OPERAND_AUDIO_SHORT_DESC;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_AUDIO_SHORT_DESC */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_ROUTING_CHANGE
#define _typedef___AMP_DISP_CEC_OPERAND_ROUTING_CHANGE
typedef struct AMP_DISP_CEC_OPERAND_ROUTING_CHANGE {
  UINT16 uiOrigPhyAddr;
  UINT16 uiNewPhyAddr;
} AMP_DISP_CEC_OPERAND_ROUTING_CHANGE;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_ROUTING_CHANGE */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_STRING
#define _typedef___AMP_DISP_CEC_OPERAND_STRING
typedef struct AMP_DISP_CEC_OPERAND_STRING {
  UINT8 uiStringLen;
  UINT8 uiStringName[14];
} AMP_DISP_CEC_OPERAND_STRING;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_STRING */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_UI_COMMAND_CEC_FnOperand
#define _typedef___AMP_DISP_CEC_OPERAND_UI_COMMAND_CEC_FnOperand
typedef struct AMP_DISP_CEC_OPERAND_UI_COMMAND_CEC_FnOperand {
  UINT32 _d;
  union {
    INT32 iPlayMode;
    struct AMP_DISP_CEC_OPERAND_CHN_IDEN ChnIden;
    UINT8 uiFnMedia;
    UINT8 uiFnSelAVIn;
    UINT8 uiFnSelAudIn;
    UINT8 uiFnSelSoundCtrl;
    UINT8 uiFnSelBrdCstIn;
  } _u;
} AMP_DISP_CEC_OPERAND_UI_COMMAND_CEC_FnOperand;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_UI_COMMAND_CEC_FnOperand */

#ifndef _typedef___AMP_DISP_CEC_OPERAND_UI_COMMAND
#define _typedef___AMP_DISP_CEC_OPERAND_UI_COMMAND
typedef struct AMP_DISP_CEC_OPERAND_UI_COMMAND {
  INT32 iUserControlCode;
  INT32 iFnOperandPresent;
  struct AMP_DISP_CEC_OPERAND_UI_COMMAND_CEC_FnOperand FnOperand;
} AMP_DISP_CEC_OPERAND_UI_COMMAND;
#endif /* _typedef___AMP_DISP_CEC_OPERAND_UI_COMMAND */

#ifndef _typedef___AMP_DISP_CEC_MSG_OPERAND
#define _typedef___AMP_DISP_CEC_MSG_OPERAND
typedef struct AMP_DISP_CEC_MSG_OPERAND {
  UINT32 _d;
  union {
    UINT16 uiOpPhyAddr;
    UINT8 uiOpLanguage[3];
    struct AMP_DISP_CEC_OPERAND_FEATURE_ABORT OpFeatureAbort;
    struct AMP_DISP_CEC_OPERAND_REPORT_PHY_ADDR OpRepPhyAddr;
    INT32 iOpMenuRequest;
    INT32 iOpMenuState;
    struct AMP_DISP_CEC_OPERAND_UI_COMMAND OpUICommand;
    struct AMP_DISP_CEC_OPERAND_REC_SRC OpRecSrc;
    INT32 iOpRecStsInfo;
    INT32 iOpDeckControlMode;
    INT32 iOpDeckInfo;
    INT32 iOpStatusRequest;
    INT32 iOpPlayMode;
    struct AMP_DISP_CEC_OPERAND_OSD_STRING OpOsdString;
    struct AMP_DISP_CEC_OPERAND_OSD_NAME OpDevOsdName;
    INT32 iOpPwrSts;
    UINT32 uiOpVendorID;
    UINT8 uiOpVendorRCCode;
    struct AMP_DISP_CEC_OPERAND_VENDOR_SPEC_DATA OpVendorSpecData;
    struct AMP_DISP_CEC_OPERAND_DIG_SRV_ID OpDigSrvIden;
    struct AMP_DISP_CEC_OPERAND_ANALOGUE_SRV_ID OpAnalogueSrvIden;
    struct AMP_DISP_CEC_OPERAND_TUNER_DEVICE_INFO OpTunerDeviceInfo;
    struct AMP_DISP_CEC_OPERAND_ANALOGUE_TIMER_INFO OpAnalogueTimerInfo;
    struct AMP_DISP_CEC_OPERAND_DIGITAL_TIMER_INFO OpDigitalTimerInfo;
    struct AMP_DISP_CEC_OPERAND_EXT_TIMER_INFO OpExtTimerInfo;
    INT32 iOpTimerClearedStsData;
    struct AMP_DISP_CEC_OPERAND_TIMER_STATUS_DATA OpTimerStatusData;
    struct AMP_DISP_CEC_OPERAND_STRING OpPgmTitleString;
    struct AMP_DISP_CEC_OPERAND_AUDIO_MODE_REQUEST OpSysAudioModeRequest;
    INT32 iOpAudioRateControl;
    struct AMP_DISP_CEC_OPERAND_AUDIO_STATUS OpAudioStatus;
    AMP_DISP_BOOL bOpSysAudioStatus;
    struct AMP_DISP_CEC_OPERAND_AUDIO_FMT_WITH_ID OpAudioFormatWithID;
    struct AMP_DISP_CEC_OPERAND_AUDIO_SHORT_DESC OpAudioShortDesc;
    INT32 iOpVersion;
    struct AMP_DISP_CEC_OPERAND_ROUTING_CHANGE OpRoutingChange;
  } _u;
} AMP_DISP_CEC_MSG_OPERAND;
#endif /* _typedef___AMP_DISP_CEC_MSG_OPERAND */

#ifndef _typedef___AMP_DISP_CEC_MSG
#define _typedef___AMP_DISP_CEC_MSG
typedef struct AMP_DISP_CEC_MSG {
  UINT8 uiSrcAddr;
  UINT8 uiDestAddr;
  UINT16 uiOpCode;
  struct AMP_DISP_CEC_MSG_OPERAND Operand;
} AMP_DISP_CEC_MSG;
#endif /* _typedef___AMP_DISP_CEC_MSG */

#ifndef _typedef___AMP_DISP_WSS_DATA
#define _typedef___AMP_DISP_WSS_DATA
typedef struct AMP_DISP_WSS_DATA {
  INT8 iARMode;
  INT8 iSourceMode;
  INT8 iSubinTT;
  INT8 iSubMode;
  INT8 iSoundMode;
  INT8 iCopyright;
  INT8 iGeneration;
} AMP_DISP_WSS_DATA;
#endif /* _typedef___AMP_DISP_WSS_DATA */

#ifndef _typedef___AMP_DISP_CGMS_DATA
#define _typedef___AMP_DISP_CGMS_DATA
typedef struct AMP_DISP_CGMS_DATA {
  INT8 iARMode;
  INT8 iCopyInfoTrans;
  INT8 iCopyInfoMode;
  INT8 iAPSMode;
  INT8 iSourceMode;
} AMP_DISP_CGMS_DATA;
#endif /* _typedef___AMP_DISP_CGMS_DATA */

#ifndef _typedef___AMP_DISP_CEC_EVENT_LOG_ADDR
#define _typedef___AMP_DISP_CEC_EVENT_LOG_ADDR
typedef struct AMP_DISP_CEC_EVENT_LOG_ADDR {
  INT32 iDeviceType;
  INT32 iLogAddrAllocSts;
  INT32 iLogAddr;
} AMP_DISP_CEC_EVENT_LOG_ADDR;
#endif /* _typedef___AMP_DISP_CEC_EVENT_LOG_ADDR */

#ifndef _typedef___AMP_DISP_CEC_EVENT_TX_STATUS
#define _typedef___AMP_DISP_CEC_EVENT_TX_STATUS
typedef struct AMP_DISP_CEC_EVENT_TX_STATUS {
  INT32 iTxMsgOpcode;
  INT32 iTxStatus;
} AMP_DISP_CEC_EVENT_TX_STATUS;
#endif /* _typedef___AMP_DISP_CEC_EVENT_TX_STATUS */

#ifndef _typedef___AMP_DISP_CEC_EVENT_RX_STATUS
#define _typedef___AMP_DISP_CEC_EVENT_RX_STATUS
typedef struct AMP_DISP_CEC_EVENT_RX_STATUS {
  INT32 iRxStatus;
  struct AMP_DISP_CEC_MSG CecMsg;
} AMP_DISP_CEC_EVENT_RX_STATUS;
#endif /* _typedef___AMP_DISP_CEC_EVENT_RX_STATUS */

#ifndef _typedef___AMP_DISP_CEC_EVENT_INFO_CEC_EVENT_PAYLOAD
#define _typedef___AMP_DISP_CEC_EVENT_INFO_CEC_EVENT_PAYLOAD
typedef struct AMP_DISP_CEC_EVENT_INFO_CEC_EVENT_PAYLOAD {
  UINT32 _d;
  union {
    struct AMP_DISP_CEC_EVENT_LOG_ADDR LogAddrStsEvtInfo;
    struct AMP_DISP_CEC_EVENT_TX_STATUS TxStatusEvtInfo;
    struct AMP_DISP_CEC_EVENT_RX_STATUS RxStatusEvtInfo;
  } _u;
} AMP_DISP_CEC_EVENT_INFO_CEC_EVENT_PAYLOAD;
#endif /* _typedef___AMP_DISP_CEC_EVENT_INFO_CEC_EVENT_PAYLOAD */

#ifndef _typedef___AMP_DISP_CEC_EVENT_INFO
#define _typedef___AMP_DISP_CEC_EVENT_INFO
typedef struct AMP_DISP_CEC_EVENT_INFO {
  INT32 iInfoId;
  struct AMP_DISP_CEC_EVENT_INFO_CEC_EVENT_PAYLOAD EvtInfo;
} AMP_DISP_CEC_EVENT_INFO;
#endif /* _typedef___AMP_DISP_CEC_EVENT_INFO */

#ifndef _typedef___AMP_DISP_VPP_AUX_FRAME_INFO
#define _typedef___AMP_DISP_VPP_AUX_FRAME_INFO
typedef struct AMP_DISP_VPP_AUX_FRAME_INFO {
  UINT32 uSuccess;
  UINT32 uSize;
  UINT32 viewID;
} AMP_DISP_VPP_AUX_FRAME_INFO;
#endif /* _typedef___AMP_DISP_VPP_AUX_FRAME_INFO */

#ifndef _typedef___AMP_DISP_RES_CHANGE_EVENT_INFO
#define _typedef___AMP_DISP_RES_CHANGE_EVENT_INFO
typedef struct AMP_DISP_RES_CHANGE_EVENT_INFO {
  UINT32 uiTgId;
  UINT32 ResId;
  UINT32 uiBitDepth;
  UINT32 uiOriginalResId;
  UINT32 uiTargetResId;
} AMP_DISP_RES_CHANGE_EVENT_INFO;
#endif /* _typedef___AMP_DISP_RES_CHANGE_EVENT_INFO */

#ifndef _define___AMP_DISP_REG_GROUP_ITEM_MAX
#define _define___AMP_DISP_REG_GROUP_ITEM_MAX
#define AMP_DISP_REG_GROUP_ITEM_MAX (4)
#endif

#ifndef _typedef___AMP_DISP_VPP_REG_GROUP
#define _typedef___AMP_DISP_VPP_REG_GROUP
typedef struct AMP_DISP_VPP_REG_GROUP {
  UINT32 uiRegNum;
  UINT32 uiRegGroup[8];
} AMP_DISP_VPP_REG_GROUP;
#endif /* _typedef___AMP_DISP_VPP_REG_GROUP */

#ifndef _typedef___AMP_DISP_LVDS_GAMMA_LUT_TABLE
#define _typedef___AMP_DISP_LVDS_GAMMA_LUT_TABLE
typedef struct AMP_DISP_LVDS_GAMMA_LUT_TABLE {
  UINT16 uiGammaLutTable[1024];
} AMP_DISP_LVDS_GAMMA_LUT_TABLE;
#endif /* _typedef___AMP_DISP_LVDS_GAMMA_LUT_TABLE */

#ifndef _typedef___AMP_DISP_SCALER_FILTER_TABLE
#define _typedef___AMP_DISP_SCALER_FILTER_TABLE
typedef struct AMP_DISP_SCALER_FILTER_TABLE {
  UINT16 uiScalerFilterTable[396];
} AMP_DISP_SCALER_FILTER_TABLE;
#endif /* _typedef___AMP_DISP_SCALER_FILTER_TABLE */

#ifndef _typedef___AMP_DISP_LVDS_MAPPING_TABLE
#define _typedef___AMP_DISP_LVDS_MAPPING_TABLE
typedef struct AMP_DISP_LVDS_MAPPING_TABLE {
  UINT8 uiLvdsMappingTable[168];
} AMP_DISP_LVDS_MAPPING_TABLE;
#endif /* _typedef___AMP_DISP_LVDS_MAPPING_TABLE */

#ifndef _typedef___AMP_DISP_LVDS_ICSC_COEFFS
#define _typedef___AMP_DISP_LVDS_ICSC_COEFFS
typedef struct AMP_DISP_LVDS_ICSC_COEFFS {
  UINT32 uiIcscCoeffs[12];
} AMP_DISP_LVDS_ICSC_COEFFS;
#endif /* _typedef___AMP_DISP_LVDS_ICSC_COEFFS */

#ifndef _typedef___AMP_DISP_LVDS_SSC_CONFIG
#define _typedef___AMP_DISP_LVDS_SSC_CONFIG
typedef struct AMP_DISP_LVDS_SSC_CONFIG {
  UINT8 uiSscEnable;
  UINT8 uiSscMode;
  UINT8 uiSscFreq;
  UINT8 uiSscAmplitude;
} AMP_DISP_LVDS_SSC_CONFIG;
#endif /* _typedef___AMP_DISP_LVDS_SSC_CONFIG */

#ifndef _typedef___AMP_DISP_VBO_LANE_MAP_TABLE
#define _typedef___AMP_DISP_VBO_LANE_MAP_TABLE
typedef struct AMP_DISP_VBO_LANE_MAP_TABLE {
  UINT16 uiVboLaneMapTable[24];
} AMP_DISP_VBO_LANE_MAP_TABLE;
#endif /* _typedef___AMP_DISP_VBO_LANE_MAP_TABLE */

#ifndef _typedef___AMP_DISP_CLK_GATE_SIGNATURE
#define _typedef___AMP_DISP_CLK_GATE_SIGNATURE
typedef struct AMP_DISP_CLK_GATE_SIGNATURE {
  UINT32 signature0;
  UINT32 signature1;
  UINT32 signature2;
  UINT32 signature3;
  UINT32 signature4;
  UINT32 signature5;
} AMP_DISP_CLK_GATE_SIGNATURE;
#endif /* _typedef___AMP_DISP_CLK_GATE_SIGNATURE */

#ifndef _typedef___AMP_DISP_DV_VSDB
#define _typedef___AMP_DISP_DV_VSDB
typedef struct AMP_DISP_DV_VSDB {
  UINT8 buf[32];
  UINT32 size;
} AMP_DISP_DV_VSDB;
#endif /* _typedef___AMP_DISP_DV_VSDB */

#ifndef _typedef___AMP_DISP_OUT_PARAMS
#define _typedef___AMP_DISP_OUT_PARAMS
typedef struct AMP_DISP_OUT_PARAMS {
  AMP_DISP_OUT_ID uiDispId;
  AMP_DISP_OUT_RES uiResId;
  UINT32 uiBitDepth;
  UINT32 uiColorFmt;
  INT32 iPixelRepeat;
  UINT32 uiDisplayMode;
} AMP_DISP_OUT_PARAMS;
#endif /* _typedef___AMP_DISP_OUT_PARAMS */

#ifndef _typedef___AMP_DISP_OUT_TYPE
#define _typedef___AMP_DISP_OUT_TYPE
typedef UINT32 AMP_DISP_OUT_TYPE;
#endif /* _typedef___AMP_DISP_OUT_TYPE */

#ifndef _define___AMP_DISP_OUT_SINGLE_MODE_PRI
#define _define___AMP_DISP_OUT_SINGLE_MODE_PRI
#define AMP_DISP_OUT_SINGLE_MODE_PRI (0)
#endif

#ifndef _define___AMP_DISP_OUT_SINGLE_MODE_SEC
#define _define___AMP_DISP_OUT_SINGLE_MODE_SEC
#define AMP_DISP_OUT_SINGLE_MODE_SEC (1)
#endif

#ifndef _define___AMP_DISP_OUT_DUAL_MODE_PIP
#define _define___AMP_DISP_OUT_DUAL_MODE_PIP
#define AMP_DISP_OUT_DUAL_MODE_PIP (2)
#endif

#ifndef _define___AMP_DISP_OUT_DUAL_MODE_GFX0
#define _define___AMP_DISP_OUT_DUAL_MODE_GFX0
#define AMP_DISP_OUT_DUAL_MODE_GFX0 (3)
#endif

#ifndef _define___AMP_DISP_OUT_DUAL_MODE_GFX1
#define _define___AMP_DISP_OUT_DUAL_MODE_GFX1
#define AMP_DISP_OUT_DUAL_MODE_GFX1 (4)
#endif

#ifndef _define___AMP_DISP_OUT_TYPE_MAX
#define _define___AMP_DISP_OUT_TYPE_MAX
#define AMP_DISP_OUT_TYPE_MAX (5)
#endif

HRESULT AMP_DISP_Destroy(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_Suspend(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_Resume(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetPlaneMute(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiMute, CORBA_Environment *_ev);

HRESULT AMP_DISP_GetPlaneMute(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 *pMute, CORBA_Environment *_ev);

HRESULT AMP_DISP_MuteActiveVideo(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiMute, CORBA_Environment *_ev);

HRESULT AMP_DISP_CaptureAUXFrame(AMP_DISP _obj, UINT32 pAddr, CORBA_Environment *_ev);

HRESULT AMP_DISP_CaptureVideoFrame(AMP_DISP _obj, struct AMP_DISP_CAP_FRM_SETTING *pCapFrmSetting, struct AMP_DISP_CAP_FRM_INFO *pVidCapInfo, CORBA_Environment *_ev);

HRESULT AMP_DISP_GetCaptureInfo(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_DISP_PLANE_CAP_INFO *pVidInfo, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetPlaneZOrder(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, struct AMP_DISP_ZORDER *pZorder, CORBA_Environment *_ev);

HRESULT AMP_DISP_GetPlaneZOrder(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, struct AMP_DISP_ZORDER *pZorder, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetScale(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_DISP_WIN *pSrcWin, struct AMP_DISP_WIN *pDstWin, CORBA_Environment *_ev);

HRESULT AMP_DISP_GetScale(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_DISP_WIN *pSrcWin, struct AMP_DISP_WIN *pDstWin, CORBA_Environment *_ev);

HRESULT AMP_DISP_LoadCustScalerFilter(AMP_DISP _obj, UINT8 coeffID, UINT8 hv, UINT8 sclUnit, struct AMP_DISP_SCALER_FILTER_TABLE *pCoeffs, CORBA_Environment *_ev);

HRESULT AMP_DISP_LoadSystemScalerFilter(AMP_DISP _obj, UINT8 coeffID, UINT8 hv, UINT8 sclUnit, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetScale_RatioCtrl(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_DISP_WIN_RATIO *pSrcWinRatio, struct AMP_DISP_WIN *pDstWin, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetScalerNonLinearControl(AMP_DISP _obj, AMP_DISP_PLANE_ID PlaneId, UINT32 uiEnable, UINT32 uiCenterFrac, UINT32 uiCenterRatio, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetScalerSubPixel(AMP_DISP _obj, UINT32 uiScalerId, UINT32 uiHv, UINT32 uiSubPixel, CORBA_Environment *_ev);

HRESULT AMP_DISP_Make_BuildInFrame(AMP_DISP _obj, UINT32 uiWidth, UINT32 uiHeight, UINT32 uiShmOffset, AMP_DISP_BUILD_IN_FRAME_TYPE FrameType, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetPlaneBgColor(AMP_DISP _obj, AMP_DISP_PLANE_ID PlaneId, INT32 Color, CORBA_Environment *_ev);

HRESULT AMP_DISP_GetPlaneBgColor(AMP_DISP _obj, AMP_DISP_PLANE_ID PlaneId, INT32 *pColor, CORBA_Environment *_ev);

HRESULT AMP_DISP_ReadRegGroup(AMP_DISP _obj, struct AMP_DISP_VPP_REG_GROUP *pRegGroup, CORBA_Environment *_ev);

HRESULT AMP_DISP_WriteRegGroup(AMP_DISP _obj, struct AMP_DISP_VPP_REG_GROUP *pRegGroup, CORBA_Environment *_ev);

HRESULT AMP_DISP_VID_PopFrame(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_BD_ST *pBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_VID_PushFrame(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_BD_ST *pBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetPortPower(AMP_DISP _obj, UINT32 uiPort, UINT32 uiEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_Push_BuildInFrame(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, AMP_DISP_BUILD_IN_FRAME_TYPE FrameType, CORBA_Environment *_ev);

HRESULT AMP_DISP_DDD_SetFormat(AMP_DISP _obj, AMP_DISP_DDD_Format dddFormat, AMP_DISP_BOOL enableFlag, CORBA_Environment *_ev);

HRESULT AMP_DISP_DDD_SetParameters(AMP_DISP _obj, INT32 strength, INT32 focalPoint, CORBA_Environment *_ev);

HRESULT AMP_DISP_DDD_GetParameters(AMP_DISP _obj, AMP_DISP_DDD_Format *pDddFormat, INT32 *pStrength, INT32 *pFocalPoint, CORBA_Environment *_ev);

HRESULT AMP_DISP_GFX_PushBD(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_BD_ST *hBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_GFX_PopBD(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_BD_ST *phBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_GFX_WaitBDFree(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_BD_ST *hBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_GFX_WaitVSync(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, CORBA_Environment *_ev);

HRESULT AMP_DISP_SetGfxCsc(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiCscMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetResolution(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, AMP_DISP_OUT_RES uiResId, UINT32 uiBitDepth, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_GetResolution(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, AMP_DISP_OUT_RES *pResId, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_GetResolutionDescription(AMP_DISP _obj, AMP_DISP_OUT_RES ResId, struct AMP_DISP_OUT_RESOLUTION_DESCRIPTION *ResDesc, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetMute(AMP_DISP _obj, AMP_DISP_OUT_ID OutPath, UINT32 uiMute, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetMute_Ext(AMP_DISP _obj, AMP_DISP_OUT_ID OutPath, AMP_DISP_BOOL uiMute, AMP_DISP_BOOL bVideo, AMP_DISP_BOOL bAudio, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetSource(AMP_DISP _obj, AMP_DISP_OUT_ID OutPath, AMP_DISP_TG_ID TgId, AMP_DISP_OUT_MODE OutMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetTVType(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, UINT32 uiTVType, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_GetTVType(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, UINT32 *pTVType, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetFlipMode(AMP_DISP _obj, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_WatermarkCTL(AMP_DISP _obj, UINT32 WatermarkPlaneID, UINT32 subcmd, UINT32 pAddrHandle, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetFormat(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, struct AMP_DISP_OUT_PARAMS *pDispParams, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_GetFormat(AMP_DISP _obj, AMP_DISP_TG_ID uiTgId, struct AMP_DISP_OUT_PARAMS *pDispParams, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_SetVidFmt(AMP_DISP _obj, INT32 iHdmiColorFmt, INT32 iHdmiBitDepth, INT32 iHdmiPixelRept, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetVidFmt(AMP_DISP _obj, AMP_DISP_OUT_BIT_DEP *pHdmiColorFmt, AMP_DISP_OUT_CLR_FMT *pHdmiBitDepth, INT32 *pHdmiPixelRept, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_SetVidInfo(AMP_DISP _obj, INT32 iAspRatio, INT32 iScanInfo, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetSinkCaps(AMP_DISP _obj, struct AMP_DISP_HDMI_SINK_CAPS *pCaps, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetRawEdid(AMP_DISP _obj, struct AMP_DISP_HDMI_RAW_EDID *pEdid, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_SetMode(AMP_DISP _obj, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_eARC_SetMode(AMP_DISP _obj, AMP_DISP_EARC_MODE arcMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_eARC_ResetMode(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_SetSRM(AMP_DISP _obj, struct AMP_DISP_HDCP_SRM_DATA *pSrmData, struct AMP_DISP_HDCP_SRM_INFO *pSrmInfo, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetSRMVersion(AMP_DISP _obj, UINT16 *pHdcp22SrmVersion, UINT16 *pHdcp14SrmVersion, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetHdcp2xDebugDump(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_SendAuxPkt(AMP_DISP _obj, AMP_DISP_HDMI_AUX_PKT_ID uiPktType, struct AMP_DISP_HDMI_PKT *pAuxPkt, AMP_DISP_BOOL RepeatPkt, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_StopAuxPkt(AMP_DISP _obj, AMP_DISP_HDMI_AUX_PKT_ID uiPktType, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetAuxPkt(AMP_DISP _obj, AMP_DISP_HDMI_AUX_PKT_ID uiPktType, struct AMP_DISP_HDMI_PKT_RAW *pAuxPkt, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_EnablexvYCC(AMP_DISP _obj, UINT32 uiEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_SetxvYCCMode(AMP_DISP _obj, INT32 iMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_GetState(AMP_DISP _obj, INT32 *pState, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_DumpRegisters(AMP_DISP _obj, struct AMP_DISP_HDMI_REG_DUMP *pHdmiRegDump, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_GetAKsv(AMP_DISP _obj, struct AMP_DISP_HDCP_AKSV_DATA *pAKsvData, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_GetKsv(AMP_DISP _obj, struct AMP_DISP_HDCP_KSV_DATA *pKsvData, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_SetRepeaterMode(AMP_DISP _obj, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_LoadKeys(AMP_DISP _obj, UINT32 uiHdcpKeys, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_Enable(AMP_DISP _obj, UINT8 uiEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_GetVersion(AMP_DISP _obj, AMP_DISP_HDMI_HDCP_VER *pHDCPVersion, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDCP_SetVersion(AMP_DISP _obj, AMP_DISP_HDMI_HDCP_VER iHDCPVersion, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_GetSinkHdcpCapability(AMP_DISP _obj, AMP_DISP_HDMI_HDCP_VER *pHDCPVersion, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HPD_GetStatus(AMP_DISP _obj, UINT32 *pHPDStatus, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDR10_GetPlaybackStatus(AMP_DISP _obj, AMP_DISP_BOOL *pIsHdrPlaying, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_CEC_Enable(AMP_DISP _obj, INT32 iEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_CEC_SetPhysicalAddr(AMP_DISP _obj, INT32 iPhyAddr, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_CEC_AllocLogicalAddr(AMP_DISP _obj, AMP_DISP_BOOL Alloc, UINT8 uiDevType, INT32 iIniPollAddr, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_CEC_SendMessage(AMP_DISP _obj, struct AMP_DISP_CEC_MSG *pCecMsg, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_CEC_GetSupportedFeature(AMP_DISP _obj, INT32 *pFeature, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_CEC_SetSupportedFeature(AMP_DISP _obj, INT32 iFeature, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_ANALOG_SetMacrovision(AMP_DISP _obj, UINT32 uiOutPath, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_ANALOG_SetWSSData(AMP_DISP _obj, UINT32 uiOutPath, struct AMP_DISP_WSS_DATA *pWssData, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_ANALOG_SetCGMSData(AMP_DISP _obj, UINT32 uiOutPath, struct AMP_DISP_CGMS_DATA *pCgmsData, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_ANALOG_SetCCData(AMP_DISP _obj, UINT32 uiData, UINT32 uiLength, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetSSOffsetMode(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiOffsetmode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetSSOffsetSeqIDRef(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiSeqId, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetSSDefaultOffset(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiOffset, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetSSType(AMP_DISP _obj, UINT32 uiChannelMask, UINT32 uiPresenttypes, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetHDMIStructure(AMP_DISP _obj, AMP_DISP_3D_Structure StructureID, UINT32 uiExtData, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetConvertMode(AMP_DISP _obj, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_3D_SetInvertMode(AMP_DISP _obj, AMP_DISP_BOOL uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetDefaultMapping(AMP_DISP _obj, UINT8 uiIndex, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetMapping(AMP_DISP _obj, struct AMP_DISP_LVDS_MAPPING_TABLE *pMapTab, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_GetSCLCoeffModes(AMP_DISP _obj, INT32 iUnitNum, INT32 *pHSCLCoeffMode, INT32 *pVSCLCoeffMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_LoadIcscCustCoeffs(AMP_DISP _obj, UINT8 uiDeGammaGamma, struct AMP_DISP_LVDS_ICSC_COEFFS *pCoeffsTab, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetIcsc(AMP_DISP _obj, UINT8 uiDeGammaGamma, UINT8 uiIcscMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_GetIcsc(AMP_DISP _obj, UINT8 uiDeGammaGamma, UINT8 *pIcscMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_LoadGammaLUT(AMP_DISP _obj, UINT8 uiDeGammaGamma, UINT8 uiOptBGR, struct AMP_DISP_LVDS_GAMMA_LUT_TABLE *pLUT, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_VBO_SetScramlerEn(AMP_DISP _obj, UINT8 uiEnable, UINT8 uiBypassMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_VBO_SetEncoderEn(AMP_DISP _obj, UINT8 uiEnable, UINT8 uiBypassMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_VBO_SetEmphasis(AMP_DISP _obj, UINT8 uiEnable, UINT8 uiTx_swing, UINT8 uiEmph_swing, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_VBO_GetRecieverStatus(AMP_DISP _obj, UINT8 *pLockN, UINT8 *pPwrdN, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_VBO_SetLaneMapping(AMP_DISP _obj, struct AMP_DISP_VBO_LANE_MAP_TABLE *pLaneMap, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_WriteGammaLUT(AMP_DISP _obj, UINT8 uiDeGammaGamma, UINT8 uiGammaBitDepth, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_ReadGammaLUT(AMP_DISP _obj, UINT8 uiDeGammaGamma, UINT8 uiOptBGR, struct AMP_DISP_LVDS_GAMMA_LUT_TABLE *pLUT, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_BypassDeGammaGamma(AMP_DISP _obj, UINT8 uiDeGammaGamma, UINT8 uiBypass, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetWBGain(AMP_DISP _obj, UINT8 uiOptBGR, UINT32 uiGain, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_GetWBGain(AMP_DISP _obj, UINT8 uiOptBGR, UINT32 *pGain, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetWBOffset(AMP_DISP _obj, UINT8 uiOptBGR, INT32 iOffset, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_GetWBOffset(AMP_DISP _obj, UINT8 uiOptBGR, INT32 *pOffset, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetSSC(AMP_DISP _obj, struct AMP_DISP_LVDS_SSC_CONFIG *pLvdsSscCfg, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetPower(AMP_DISP _obj, UINT8 uiPowerOnOff, UINT8 uiPanelType, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_Config(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_OpenPanel(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_ClosePanel(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetBacklight(AMP_DISP _obj, UINT8 uiPwmBL, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetPwm(AMP_DISP _obj, UINT8 uiEnable, UINT8 uiPwmCh, UINT8 uiPwmBL, UINT32 uiTermCount, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetLed(AMP_DISP _obj, UINT8 uiPort, UINT8 uiOnOff, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetGpio(AMP_DISP _obj, UINT8 uiMode, UINT8 uiPort, UINT8 uiOnOff, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_InitMemcCmd(AMP_DISP _obj, UINT8 uiFrc, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_SetMemcMode(AMP_DISP _obj, UINT8 uiFrc, UINT8 uiCtrlMode, UINT8 uiMemcMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_GetMemcMode(AMP_DISP _obj, UINT8 uiFrc, UINT8 uiCtrlMode, UINT8 *pMemcMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_LVDS_Set3dMode(AMP_DISP _obj, UINT8 uiFrc, UINT8 uiCtrlMode, UINT8 ui3dMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_PQ_SetProperty(AMP_DISP _obj, UINT32 uiParaID, UINT32 uiParaVal, CORBA_Environment *_ev);

HRESULT AMP_DISP_PQ_GetProperties(AMP_DISP _obj, UINT32 uiParaID, UINT32 uiCount, UINT32 *pParaVal, CORBA_Environment *_ev);

HRESULT AMP_DISP_PQ_LoadProperty(AMP_DISP _obj, UINT32 uiParaID, UINT32 uiParaVal, CORBA_Environment *_ev);

HRESULT AMP_DISP_PQ_CommitProperties(AMP_DISP _obj, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_EnableHDR(AMP_DISP _obj, UINT32 uiEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetRotationAngle(AMP_DISP _obj, UINT32 uiPlaneID, AMP_DISP_ROTATION_MODE eRotationAngle, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_GetRotationAngle(AMP_DISP _obj, UINT32 uiPlaneID, AMP_DISP_ROTATION_MODE *peRotationAngle, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_GetClkGateSignature(AMP_DISP _obj, UINT32 *puiSignatureCount, struct AMP_DISP_CLK_GATE_SIGNATURE *pSignature, UINT32 uiOption, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetClkGateSignature(AMP_DISP _obj, struct AMP_DISP_CLK_GATE_SIGNATURE *pSignatureValue, struct AMP_DISP_CLK_GATE_SIGNATURE *pSignatureMask, UINT32 uiOption, CORBA_Environment *_ev);

HRESULT AMP_DISP_ConfigWinAttr(AMP_DISP _obj, AMP_DISP_PLANE_ID uiPlaneId, struct AMP_DISP_VPP_WIN_ATTR *winAttr, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_HDMI_EnableEDR(AMP_DISP _obj, UINT32 uiEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetThdrVitmParams(AMP_DISP _obj, INT32 iCmdId, INT32 iValue, CORBA_Environment *_ev);

HRESULT AMP_DISP_DCMPQ_SetProperty(AMP_DISP _obj, INT32 iPlaneID, INT32 iHue, INT32 iSaturation, INT32 iLightness, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_DV_LoadVSDB(AMP_DISP _obj, struct AMP_DISP_DV_VSDB *pVsdb, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_DV_SetPriorityMode(AMP_DISP _obj, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_DV_EnableLowLatency(AMP_DISP _obj, UINT32 uiEnable, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_DV_SetDovi2HdrMap(AMP_DISP _obj, UINT32 uiEn, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_DV_SetHdrFeature(AMP_DISP _obj, UINT32 uiFeature, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_DV_SetGfxInfo(AMP_DISP _obj, UINT32 type, UINT32 gfx_min, UINT32 gfx_max, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetHdmiOutputMode(AMP_DISP _obj, UINT32 uiMode, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetColorTransformGain(AMP_DISP _obj, CORBA_float Rgain, CORBA_float Ggain, CORBA_float Bgain, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetColorTemperature(AMP_DISP _obj, UINT32 temp, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetGammaMode(AMP_DISP _obj, UINT32 vopID, UINT32 GmMode, struct AMP_BD_ST *phBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetScalingPreference(AMP_DISP _obj, UINT32 uiScalingPrefer, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_SetEEMode(AMP_DISP _obj, UINT32 mode, struct AMP_DISP_VPP_CMU_EE_CTRL_PARAM *CmuEeCtrlParam, CORBA_Environment *_ev);

HRESULT AMP_DISP_NGWM_StubConfigure(AMP_DISP _obj, struct AMP_BD_ST *pBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_NGWM_StubConfigureByPipe(AMP_DISP _obj, UINT32 pipeSourceId, struct AMP_BD_ST *pBD, CORBA_Environment *_ev);

HRESULT AMP_DISP_OUT_Debug(AMP_DISP _obj, UINT32 uiCmd, UINT32 uiCmdParam, struct AMP_BD_ST *phInBD, struct AMP_BD_ST *phOutBD, CORBA_Environment *_ev);

#ifndef _define___AMP_AUD_STREAMS_MAX
#define _define___AMP_AUD_STREAMS_MAX
#define AMP_AUD_STREAMS_MAX (16)
#endif

#ifndef _define___AMP_AUD_SINKS_MAX
#define _define___AMP_AUD_SINKS_MAX
#define AMP_AUD_SINKS_MAX (6)
#endif

#ifndef _define___AMP_AUD_PORT_STAT_UPDATED
#define _define___AMP_AUD_PORT_STAT_UPDATED
#define AMP_AUD_PORT_STAT_UPDATED (1)
#endif

#ifndef _define___AMP_AUD_PORT_STAT_UNDERFLOW
#define _define___AMP_AUD_PORT_STAT_UNDERFLOW
#define AMP_AUD_PORT_STAT_UNDERFLOW (2)
#endif

#ifndef _typedef___AMP_TIMESPEC
#define _typedef___AMP_TIMESPEC
typedef struct AMP_TIMESPEC {
  UINT32 tv_sec;
  UINT32 tv_nsec;
} AMP_TIMESPEC;
#endif /* _typedef___AMP_TIMESPEC */

#ifndef _typedef___AMP_CHANMASK
#define _typedef___AMP_CHANMASK
typedef struct AMP_CHANMASK {
  UINT32 uiChanMask[10];
} AMP_CHANMASK;
#endif /* _typedef___AMP_CHANMASK */

#ifndef _typedef___AMP_AUD_PORT_STAT
#define _typedef___AMP_AUD_PORT_STAT
typedef struct AMP_AUD_PORT_STAT {
  INT32 iStreamId;
  UINT32 uiStatFlag;
  UINT32 uiSampFullness;
  UINT32 uiSampRate;
  UINT32 uiIdleBDNr;
  UINT32 uiBusyBDNr;
  UINT32 uiPtsHigh;
  UINT32 uiPtsLow;
  UINT32 uiDelayInMs;
  UINT32 uiPtsUpdateMask;
  UINT64 ullStreamPos;
  INT32 iRatio;
  struct AMP_TIMESPEC stStreamPos;
  struct AMP_TIMESPEC stSysTime;
  UINT32 uiBufFullnessMs;
} AMP_AUD_PORT_STAT;
#endif /* _typedef___AMP_AUD_PORT_STAT */

#ifndef _define___AMP_BTOFF
#define _define___AMP_BTOFF
#define AMP_BTOFF (0)
#endif

#ifndef _define___AMP_BTON
#define _define___AMP_BTON
#define AMP_BTON (1)
#endif

#ifndef _define___AMP_BTAUDIO
#define _define___AMP_BTAUDIO
#define AMP_BTAUDIO (2)
#endif

#ifndef _define___AMP_BTSUBWOOFER
#define _define___AMP_BTSUBWOOFER
#define AMP_BTSUBWOOFER (3)
#endif

#ifndef _define___AMP_BTDISABLE
#define _define___AMP_BTDISABLE
#define AMP_BTDISABLE (4)
#endif

#ifndef _typedef___AMP_BTSWITCH
#define _typedef___AMP_BTSWITCH
typedef CORBA_enum AMP_BTSWITCH;
#endif /* _typedef___AMP_BTSWITCH */

#ifndef _define___AMP_AUDIO_SPDIF_COPYRIGHT_NOT_PROTECT
#define _define___AMP_AUDIO_SPDIF_COPYRIGHT_NOT_PROTECT
#define AMP_AUDIO_SPDIF_COPYRIGHT_NOT_PROTECT (0)
#endif

#ifndef _define___AMP_AUDIO_SPDIF_COPYRIGHT_ONETIME
#define _define___AMP_AUDIO_SPDIF_COPYRIGHT_ONETIME
#define AMP_AUDIO_SPDIF_COPYRIGHT_ONETIME (1)
#endif

#ifndef _define___AMP_AUDIO_SPDIF_COPYRIGHT_PROTECTED
#define _define___AMP_AUDIO_SPDIF_COPYRIGHT_PROTECTED
#define AMP_AUDIO_SPDIF_COPYRIGHT_PROTECTED (2)
#endif

#ifndef _define___AMP_AUDIO_SPDIF_COPYRIGHT_MAX
#define _define___AMP_AUDIO_SPDIF_COPYRIGHT_MAX
#define AMP_AUDIO_SPDIF_COPYRIGHT_MAX (3)
#endif

#ifndef _typedef___AMP_AUDIO_SPDIF_COPY_RIGHT
#define _typedef___AMP_AUDIO_SPDIF_COPY_RIGHT
typedef CORBA_enum AMP_AUDIO_SPDIF_COPY_RIGHT;
#endif /* _typedef___AMP_AUDIO_SPDIF_COPY_RIGHT */

#ifndef _define___AMP_AOUT_DUMPSTOP
#define _define___AMP_AOUT_DUMPSTOP
#define AMP_AOUT_DUMPSTOP (0)
#endif

#ifndef _define___AMP_AOUT_DUMPSTART
#define _define___AMP_AOUT_DUMPSTART
#define AMP_AOUT_DUMPSTART (1)
#endif

#ifndef _define___AMP_AOUT_DUMPPAUSE
#define _define___AMP_AOUT_DUMPPAUSE
#define AMP_AOUT_DUMPPAUSE (2)
#endif

#ifndef _typedef___AMP_AOUT_DUMP_STATE
#define _typedef___AMP_AOUT_DUMP_STATE
typedef CORBA_enum AMP_AOUT_DUMP_STATE;
#endif /* _typedef___AMP_AOUT_DUMP_STATE */

#ifndef _typedef___AMP_AUD_PIPELINE_STAT
#define _typedef___AMP_AUD_PIPELINE_STAT
typedef struct AMP_AUD_PIPELINE_STAT {
  struct AMP_AUD_PORT_STAT stSrcInPortStat;
  struct AMP_AUD_PORT_STAT stSrcOutPortStat;
  struct AMP_AUD_PORT_STAT stSrcMulInPortStat;
  struct AMP_AUD_PORT_STAT stSrcMulOutPortStat;
  struct AMP_AUD_PORT_STAT stSrcBTInPortStat;
  struct AMP_AUD_PORT_STAT stSrcBTOutPortStat;
  struct AMP_AUD_PORT_STAT stTSInPortStat;
  struct AMP_AUD_PORT_STAT stTSOutPortStat;
  struct AMP_AUD_PORT_STAT stTSMulInPortStat;
  struct AMP_AUD_PORT_STAT stTSMulOutPortStat;
  struct AMP_AUD_PORT_STAT stTSBTInPortStat;
  struct AMP_AUD_PORT_STAT stTSBTOutPortStat;
  struct AMP_AUD_PORT_STAT stMixerInPortStat;
  struct AMP_AUD_PORT_STAT stMixerOutPortStat;
  struct AMP_AUD_PORT_STAT stMixerMulInPortStat;
  struct AMP_AUD_PORT_STAT stMixerMulOutPortStat;
  struct AMP_AUD_PORT_STAT stSrsInPortStat;
  struct AMP_AUD_PORT_STAT stSrsOutPortStat;
  struct AMP_AUD_PORT_STAT stNeuralInPortStat;
  struct AMP_AUD_PORT_STAT stNeuralOutPortStat;
  struct AMP_AUD_PORT_STAT stMS12InPortStat;
  struct AMP_AUD_PORT_STAT stMS12OutPortStat;
  struct AMP_AUD_PORT_STAT stSndExpStat;
  struct AMP_AUD_PORT_STAT stAoutStat;
  UINT32 uiPipeLineMs;
  UINT64 ullStreamPos;
  UINT32 uiPtsHigh;
  UINT32 uiPtsLow;
  struct AMP_TIMESPEC stStreamPos;
  struct AMP_TIMESPEC stSysTime;
  BOOL bPipeLineMsValid;
  UINT32 uiSuggestPipeLineMs;
  UINT32 uiInputPipelineMs;
} AMP_AUD_PIPELINE_STAT;
#endif /* _typedef___AMP_AUD_PIPELINE_STAT */

#ifndef _typedef___AMP_SNDSINK_HDMI_CAP
#define _typedef___AMP_SNDSINK_HDMI_CAP
typedef struct AMP_SNDSINK_HDMI_CAP {
  BOOL bValidEdid;
  UINT32 uiSNDHDMIAudioFmt[15];
  UINT32 ucSNDHDMISinkCapFreq[15];
  UINT32 uiMaxBitRate[15];
  UINT32 uiMaxSampleRate;
  UINT32 ucSNDHDMISinkCapWdLen[15];
  UINT32 uiMaxChanNum[15];
  UINT32 uiSNDHDMIChanMask[15];
  UINT32 uiHDMIUserFmt;
  UINT32 uiHDMIWorkFmt;
  UINT32 uiSPDIFUserFmt;
  UINT32 uiSPDIFWorkFmt;
} AMP_SNDSINK_HDMI_CAP;
#endif /* _typedef___AMP_SNDSINK_HDMI_CAP */

#ifndef _typedef___AMP_SNDSINK
#define _typedef___AMP_SNDSINK
typedef CORBA_Object AMP_SNDSINK;
#endif /* _typedef___AMP_SNDSINK */

HRESULT AMP_SNDSINK_SetupPort2(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 *puiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetAPPParameters(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_APP_PARAIDX eParamIdx, struct AMP_APP_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetAPPParameters(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, AMP_APP_PARAIDX eParamIdx, struct AMP_APP_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetHDMIFormat(AMP_SNDSINK _obj, UINT32 uiFormat, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetSpdifCopyRight(AMP_SNDSINK _obj, UINT32 CopyRight, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetSpdifCopyRight(AMP_SNDSINK _obj, UINT32 *CopyRight, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetHDMIFormat(AMP_SNDSINK _obj, UINT32 *uiFormat, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetSpdifFormat(AMP_SNDSINK _obj, UINT32 uiFormat, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetSpdifFormat(AMP_SNDSINK _obj, UINT32 *uiFormat, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_PausePort(AMP_SNDSINK _obj, UINT32 puiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_StartPort(AMP_SNDSINK _obj, UINT32 puiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_StopPort(AMP_SNDSINK _obj, UINT32 puiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_QueryPortStat(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_AUD_PIPELINE_STAT *pParam, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Register_BT_Service(AMP_SNDSINK _obj, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_BT_Switch(AMP_SNDSINK _obj, UINT32 uiSwitch, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Aout_Data_Dump(AMP_SNDSINK _obj, UINT32 uiPortIdx, UINT32 State, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetEncoder(AMP_SNDSINK _obj, UINT32 Path, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetEncoder(AMP_SNDSINK _obj, UINT32 Path, BOOL *pEnable, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_CreateAudioPatch(AMP_SNDSINK _obj, struct AMP_APP_PARAST *pParam, UINT32 *uiPatchAddr, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_ReleaseAudioPatch(AMP_SNDSINK _obj, UINT32 uiPatchAddr, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_DeviceAttach(AMP_SNDSINK _obj, struct AMP_APP_PARAST *pParaSt, UINT32 *pDeviceAttachAddr, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_DeviceDetach(AMP_SNDSINK _obj, UINT32 uiDeviceAttachAddr, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SinkAttach(AMP_SNDSINK _obj, struct AMP_APP_PARAST *pParaSt, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SinkDetach(AMP_SNDSINK _obj, struct AMP_APP_PARAST *pParaSt, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Aout_SetAIO_Mute(AMP_SNDSINK _obj, UINT32 uiPath, BOOL bMute, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Aout_GetAIO_Mute(AMP_SNDSINK _obj, UINT32 uiPath, BOOL *pMute, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetAoutParameters(AMP_SNDSINK _obj, UINT32 uiPortIdx, AMP_AOUT_PARAIDX eParamIdx, struct AMP_AOUT_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SyncStrmPos(AMP_SNDSINK _obj, UINT32 uiPath, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetSrsEnable(AMP_SNDSINK _obj, UINT32 uiPath, BOOL bEnable, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetSrsEnable(AMP_SNDSINK _obj, UINT32 uiPath, BOOL *pbEnable, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetHDMICap(AMP_SNDSINK _obj, struct AMP_SNDSINK_HDMI_CAP *pSndHDMISinkCap, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetHDMICap(AMP_SNDSINK _obj, struct AMP_SNDSINK_HDMI_CAP *pSndHDMISinkCap, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetChanMask(AMP_SNDSINK _obj, struct AMP_CHANMASK *pChanMask, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetAudioRoute(AMP_SNDSINK _obj, struct AMP_APP_PARAST *pParam, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_BTStandBy(AMP_SNDSINK _obj, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_BTResume(AMP_SNDSINK _obj, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_LoopBack_Switch(AMP_SNDSINK _obj, BOOL eState, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetAoutMultiDelay(AMP_SNDSINK _obj, UINT32 uiDelaySample, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Open(AMP_SNDSINK _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Close(AMP_SNDSINK _obj, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_Destroy(AMP_SNDSINK _obj, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetState(AMP_SNDSINK _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_GetState(AMP_SNDSINK _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_QueryInfo(AMP_SNDSINK _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_SetupPort(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_DynamicSetupPort(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_DisconnectPorts(AMP_SNDSINK _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_ClearPort(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_QueryPort(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_ClearPortBuf(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_DebugDump(AMP_SNDSINK _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_RegisterNotify(AMP_SNDSINK _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_UnregisterNotify(AMP_SNDSINK _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_RegisterNotifyDetail(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_UnregisterNotifyDetail(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_RegisterBD(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_UnregisterBD(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_SNDSINK_PushBD(AMP_SNDSINK _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_SND
#define _typedef___AMP_SND
typedef CORBA_Object AMP_SND;
#endif /* _typedef___AMP_SND */

#ifndef _typedef___AMP_VA
#define _typedef___AMP_VA
typedef CORBA_Object AMP_VA;
#endif /* _typedef___AMP_VA */

#ifndef _typedef___CORBA_sequence_octet
#define _typedef___CORBA_sequence_octet
typedef struct CORBA_sequence_octet {
  CORBA_unsigned_long _maximum;
  CORBA_unsigned_long _length;
  CORBA_octet *_buffer;
  CORBA_boolean _release;
} CORBA_sequence_octet;
#endif /* _typedef___CORBA_sequence_octet */

#ifndef _typedef___AMP_VA_blob
#define _typedef___AMP_VA_blob
typedef struct CORBA_sequence_octet AMP_VA_blob;
#endif /* _typedef___AMP_VA_blob */

HRESULT AMP_VA_SetModelParameter(AMP_VA _obj, UINT8 inputNumber, AMP_VA_blob *data, CORBA_Environment *_ev);

HRESULT AMP_VA_SetMinimumInterval(AMP_VA _obj, UINT32 interval, CORBA_Environment *_ev);

HRESULT AMP_VA_Open(AMP_VA _obj, struct AMP_COMPONENT_CONFIG *pConfig, CORBA_Environment *_ev);

HRESULT AMP_VA_Close(AMP_VA _obj, CORBA_Environment *_ev);

HRESULT AMP_VA_Destroy(AMP_VA _obj, CORBA_Environment *_ev);

HRESULT AMP_VA_SetState(AMP_VA _obj, AMP_STATE eState, CORBA_Environment *_ev);

HRESULT AMP_VA_GetState(AMP_VA _obj, AMP_STATE *eState, CORBA_Environment *_ev);

HRESULT AMP_VA_QueryInfo(AMP_VA _obj, struct AMP_COMPONENT_INFO *pInfo, CORBA_Environment *_ev);

HRESULT AMP_VA_SetupPort(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_CONN *pClientConn, CORBA_Environment *_ev);

HRESULT AMP_VA_DynamicSetupPort(AMP_VA _obj, AMP_PORT_IO ePortIo, AMP_PORT_TYPE ePortType, struct AMP_PORT_CONN *pClientConn, UINT32 *uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VA_DisconnectPorts(AMP_VA _obj, UINT32 uiThisPortIdx, AMP_COMPONENT hDownComponent, UINT32 uiDownPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VA_ClearPort(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VA_QueryPort(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_PORT_INFO *pPort, CORBA_Environment *_ev);

HRESULT AMP_VA_ClearPortBuf(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, CORBA_Environment *_ev);

HRESULT AMP_VA_DebugDump(AMP_VA _obj, UINT32 uiDumpId, CORBA_Environment *_ev);

HRESULT AMP_VA_RegisterNotify(AMP_VA _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VA_UnregisterNotify(AMP_VA _obj, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VA_RegisterNotifyDetail(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VA_UnregisterNotifyDetail(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, UINT32 uiFilter, UINT32 uiServiceId, UINT32 uiEventType, CORBA_Environment *_ev);

HRESULT AMP_VA_RegisterBD(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VA_UnregisterBD(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

HRESULT AMP_VA_PushBD(AMP_VA _obj, AMP_PORT_IO ePortIo, UINT32 uiPortIdx, struct AMP_BD_ST *hBufDesc, CORBA_Environment *_ev);

#ifndef _typedef___AMP_FACTORY
#define _typedef___AMP_FACTORY
typedef CORBA_Object AMP_FACTORY;
#endif /* _typedef___AMP_FACTORY */

#ifndef _typedef___CORBA_sequence_fd
#define _typedef___CORBA_sequence_fd
typedef struct CORBA_sequence_fd {
  CORBA_unsigned_long _maximum;
  CORBA_unsigned_long _length;
  CORBA_fd *_buffer;
  CORBA_boolean _release;
} CORBA_sequence_fd;
#endif /* _typedef___CORBA_sequence_fd */

#ifndef _typedef___AMP_FILE_DESCRIPTORS
#define _typedef___AMP_FILE_DESCRIPTORS
typedef struct CORBA_sequence_fd AMP_FILE_DESCRIPTORS;
#endif /* _typedef___AMP_FILE_DESCRIPTORS */

#ifndef _typedef___AMP_FD
#define _typedef___AMP_FD
typedef CORBA_Object AMP_FD;
#endif /* _typedef___AMP_FD */

#ifndef _define___AMP_FD_SWAP_FILE_TYPE_LOCAL
#define _define___AMP_FD_SWAP_FILE_TYPE_LOCAL
#define AMP_FD_SWAP_FILE_TYPE_LOCAL (0)
#endif

#ifndef _define___AMP_FD_SWAP_FILE_TYPE_ION
#define _define___AMP_FD_SWAP_FILE_TYPE_ION
#define AMP_FD_SWAP_FILE_TYPE_ION (1)
#endif

#ifndef _typedef___AMP_FD_SWAP_FILE_TYPE
#define _typedef___AMP_FD_SWAP_FILE_TYPE
typedef CORBA_enum AMP_FD_SWAP_FILE_TYPE;
#endif /* _typedef___AMP_FD_SWAP_FILE_TYPE */

HRESULT AMP_FD_Swap(AMP_FD _obj, AMP_FD_SWAP_FILE_TYPE file_type, CORBA_fd fdIn, CORBA_fd *fdOut, CORBA_Environment *_ev);

HRESULT AMP_FD_SwapMulti(AMP_FD _obj, AMP_FD_SWAP_FILE_TYPE file_type, AMP_FILE_DESCRIPTORS *fdIn, AMP_FILE_DESCRIPTORS **fdOut, CORBA_Environment *_ev);

HRESULT AMP_FD_Send_Multi(AMP_FD _obj, AMP_FILE_DESCRIPTORS *fd1, AMP_FILE_DESCRIPTORS *fd2, CORBA_Environment *_ev);

HRESULT AMP_FD_Send(AMP_FD _obj, AMP_FILE_DESCRIPTORS *fdIn, CORBA_Environment *_ev);

#ifndef _typedef___AMP_FACTORY
#define _typedef___AMP_FACTORY
typedef CORBA_Object AMP_FACTORY;
#endif /* _typedef___AMP_FACTORY */

HRESULT AMP_FACTORY_Ping(AMP_FACTORY _obj, UINT32 input, UINT32 *output, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreateComponent(AMP_FACTORY _obj, AMP_COMPONENT_TYPE eCompType, UINT32 uiCompIdx, AMP_COMPONENT *hComponent, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreatePipeline(AMP_FACTORY _obj, AMP_PIPELINE *hPipeline, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreateDisplayService(AMP_FACTORY _obj, AMP_DISP *hService, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreateSoundService(AMP_FACTORY _obj, AMP_SND *hService, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreateDrmService(AMP_FACTORY _obj, AMP_DRM *hService, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreateLibrary(AMP_FACTORY _obj, AMP_LIB *hLibrary, CORBA_Environment *_ev);

HRESULT AMP_FACTORY_CreateFd(AMP_FACTORY _obj, AMP_FD *hFd, CORBA_Environment *_ev);

#ifdef __cplusplus
}
#endif

#endif /* _amp_client_rpc_h_ */
