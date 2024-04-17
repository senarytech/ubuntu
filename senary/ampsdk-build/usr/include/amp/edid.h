/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

/*******************************************************************************
 * \file     edid.h
 * \brief    API functions and Type definitions for EDID
 *           parsing, generating
 * \history
 *      Version    Date           Author
 *      1.00       Dec 19, 2011   yinxj@marvell.com
 ******************************************************************************/

#ifndef __EDID_H__
#define __EDID_H__

#include "com_type.h"

#define EDID_SIZE                128
#define EDID_DESCRIPTOR_SIZE     18
#define EDID_MAX_EXTBLOCK        7

#define EDID_MAX_DBC_NUM         7
#define EDID_MAX_DTD_NUM         16
#define CEA861_MAX_HDMI_VICS     127
/* maximum number of short video descriptors in one extension block */
#define CEA861_MAX_VIDEO_DESCRIPTORS 93
/* maximum number of audio descriptors in one extension block */
#define CEA861_MAX_AUDIO_DESCRIPTORS 32

#define ShiftBits(data, bits, shift) \
                ( (((UINT32)(data)) >> (shift)) & ((1 << (bits)) - 1) )

#define InsShiftBits(ptr, data, bits, shift) \
        *(ptr) = ( \
                        ((*(ptr)) & (~(((1 << (bits)) - 1) << (shift)))) | \
                        (((UINT32)(data) & ((1 << (bits)) - 1)) << (shift)) \
                 )
#define ShiftBool(data, shift) ShiftBits(data, 1, shift)

enum ColorSpace
{
        ColorSpace_None = 0,
        ColorSpace_RGB_16_235,
        ColorSpace_RGB_0_255,
        ColorSpace_YUV_601,
        ColorSpace_YUV_709,
        ColorSpace_YUV_601_0_255,
        ColorSpace_YUV_709_0_255,
        ColorSpace_xvYCC_601,
        ColorSpace_xvYCC_709,
        ColorSpace_xvYCC_601_0_255,
        ColorSpace_xvYCC_709_0_255,
        ColorSpace_BGR_0_255,
};

/*
 * CEA861-E 6.4 Format of Version 2 AVI InfoFrame
 * Table 18 Valid Pixel Repeat Values for Each Video Format Timing
 * VESA CVT/GTF not listed
 */
enum TVStandard
{
        TVStandard_HDMI_Custom,
        TVStandard_HDMI_640x480p59,
        TVStandard_HDMI_640x480p60,
        TVStandard_HDMI_480p59,
        TVStandard_HDMI_480p60,
        TVStandard_HDMI_720p59,
        TVStandard_HDMI_720p60,
        TVStandard_HDMI_1080i59,
        TVStandard_HDMI_1080i60,
        TVStandard_HDMI_480i59,
        TVStandard_HDMI_480i60,
        TVStandard_HDMI_1440x480i59,
        TVStandard_HDMI_1440x480i60,
        TVStandard_HDMI_240p59,
        TVStandard_HDMI_240p60,
        TVStandard_HDMI_1440x240p59,
        TVStandard_HDMI_1440x240p60,
        TVStandard_HDMI_2880x480i59,
        TVStandard_HDMI_2880x480i60,
        TVStandard_HDMI_2880x240p59,
        TVStandard_HDMI_2880x240p60,
        TVStandard_HDMI_1440x480p59,
        TVStandard_HDMI_1440x480p60,
        TVStandard_HDMI_1080p59,
        TVStandard_HDMI_1080p60,
        TVStandard_HDMI_576p50,
        TVStandard_HDMI_720p50,
        TVStandard_HDMI_1080i50,
        TVStandard_HDMI_576i50,
        TVStandard_HDMI_1440x576i50,
        TVStandard_HDMI_288p50,
        TVStandard_HDMI_1440x288p50,
        TVStandard_HDMI_2880x576i50,
        TVStandard_HDMI_2880x288p50,
        TVStandard_HDMI_1440x576p50,
        TVStandard_HDMI_1080p50,
        TVStandard_HDMI_1080p23,
        TVStandard_HDMI_1080p24,
        TVStandard_HDMI_1080p25,
        TVStandard_HDMI_1080p29,
        TVStandard_HDMI_1080p30,
        TVStandard_HDMI_2880x480p59,
        TVStandard_HDMI_2880x480p60,
        TVStandard_HDMI_2880x576p50,
        TVStandard_HDMI_1080i50_1250,
        TVStandard_HDMI_1080i100,
        TVStandard_HDMI_720p100,
        TVStandard_HDMI_576p100,
        TVStandard_HDMI_576i100,
        TVStandard_HDMI_1440x576i100,
        TVStandard_HDMI_1080i119,
        TVStandard_HDMI_1080i120,
        TVStandard_HDMI_720p119,
        TVStandard_HDMI_720p120,
        TVStandard_HDMI_480p119,
        TVStandard_HDMI_480p120,
        TVStandard_HDMI_480i119,
        TVStandard_HDMI_480i120,
        TVStandard_HDMI_1440x480i119,
        TVStandard_HDMI_1440x480i120,
        TVStandard_HDMI_576p200,
        TVStandard_HDMI_576i200,
        TVStandard_HDMI_1440x576i200,
        TVStandard_HDMI_480p239,
        TVStandard_HDMI_480p240,
        TVStandard_HDMI_480i239,
        TVStandard_HDMI_480i240,
        TVStandard_HDMI_1440x480i239,
        TVStandard_HDMI_1440x480i240,
        TVStandard_HDMI_720p23,
        TVStandard_HDMI_720p24,
        TVStandard_HDMI_720p25,
        TVStandard_HDMI_720p29,
        TVStandard_HDMI_720p30,
        TVStandard_HDMI_1080p119,
        TVStandard_HDMI_1080p120,
        TVStandard_HDMI_1080p100,
        TVStandard_HDMI_2160p29,
        TVStandard_HDMI_2160p30,
        TVStandard_HDMI_2160p25,
        TVStandard_HDMI_2160p23,
        TVStandard_HDMI_2160p24,
        TVStandard_HDMI_4K2Kp23,
        TVStandard_HDMI_4K2Kp24,
};

/*
 * Structure for CEA861-E Section 6.4 Format of Version 2 AVI InfoFrame
 */
struct CEA861_VideoFormat
{
        enum TVStandard Standard;
        /* Mode VFreq factor 1000/1001 23.98/24 29.98/30 59.94/60 119.88/120 239.76/240 */
        BOOLEAN         DropFrame;
        UINT32          AspX;      /* Aspect ratio X 4x3 16x9 */
        UINT32          AspY;      /* Aspect ratio Y */
        BOOLEAN         MultiAsp;  /* Support 4x3 and 16x9 with the same VIC */
        UINT32          PixelRepetition; /* Pixel Repetition 1/2/4 */
        /* CEA861-E Table 18 Valid Pixel Repeat Values for Each Video Format Timing */
        UINT32          CEA_VIC;
        UINT32          HDMI_VIC;  /* HDMI1.4a Section 8.2.3.1 */
        enum ColorSpace ColorSpace;/* Color space */
};


/*
 * EDID Block0 Data structures
 *
 */
struct CEA861_EDIDBlock0
{
        UINT8  Header[8];            /* 00,FF,FF,FF,FF,FF,FF,00 */
        CHAR   ManufacturerName[4];  /* Three uppercase letters, terminated with null char */
        UINT8  ProductCode[2];       /* LSB:[0], MSB:[1] */
        UINT8  SerialNumber[4];      /* LSB:[0], MSB:[3] */
        UINT8  ManufactureWeek;      /* 0xFF: only model year is used */
        UINT32 ManufactureYear;      /* Year - 1990 */
        UINT8  Version;
        UINT8  Revision;
        UINT8  VideoInputDefinition;
        UINT32 HSize_mm;
        UINT32 VSize_mm;
        UINT8  Gamma;                /* Gamma * 100 - 100 */
        UINT8  FeatureSupport;       /* Use EDID_FEATURESUPPORT_... defines */

        UINT32 Color_Red_x;          /* x * 2^10 */
        UINT32 Color_Red_y;          /* y * 2^10 */
        UINT32 Color_Green_x;        /* x * 2^10 */
        UINT32 Color_Green_y;        /* y * 2^10 */
        UINT32 Color_Blue_x;         /* x * 2^10 */
        UINT32 Color_Blue_y;         /* y * 2^10 */
        UINT32 Color_White_x;        /* x * 2^10 */
        UINT32 Color_White_y;        /* y * 2^10 */

        UINT8  Established_Timing[3];
        UINT8  Standard_Timing[8][2];
        UINT8  Descriptor[4][EDID_DESCRIPTOR_SIZE];

        UINT8  Extension;
        UINT8  Checksum;
};

/*
 * HDMI Audio Coding Type
 * CEA861-E Table 24 Audio InfoFrame Data Byte 1
 *          Table 26 Audio Format Code Extension (Data Byte 3)
 */
enum HDMI_AudioCodingType
{
        HDMI_AudioCodingType_FromStreamHeader = 0x0000,
        HDMI_AudioCodingType_PCM              = 0x0001,
        HDMI_AudioCodingType_AC3              = 0x0002,
        HDMI_AudioCodingType_MPEG1_12         = 0x0003,
        HDMI_AudioCodingType_MPEG1_3          = 0x0004,
        HDMI_AudioCodingType_MPEG2            = 0x0005,
        HDMI_AudioCodingType_AAC              = 0x0006,
        HDMI_AudioCodingType_DTS              = 0x0007,
        HDMI_AudioCodingType_ATRAC            = 0x0008,
        HDMI_AudioCodingType_OneBit           = 0x0009,
        HDMI_AudioCodingType_DDPlus           = 0x000A,
        HDMI_AudioCodingType_DTSHD            = 0x000B,
        HDMI_AudioCodingType_MLP              = 0x000C,
        HDMI_AudioCodingType_DST              = 0x000D,
        HDMI_AudioCodingType_WMAPro           = 0x000E,
        HDMI_AudioCodingType_HEAAC            = 0x010F,
        HDMI_AudioCodingType_HEAAC2           = 0x020F,
        HDMI_AudioCodingType_MPEGSurr         = 0x030F,
};

enum HDMI_3DSubSampling
{
        HDMI_3DSubSampling_All = 0,
        HDMI_3DSubSampling_AllHorizontal,
        HDMI_3DSubSampling_HorizontalOddLeftOddRight,
        HDMI_3DSubSampling_HorizontalOddLeftEvenRight,
        HDMI_3DSubSampling_HorizontalEvenLeftOddRight,
        HDMI_3DSubSampling_HorizontalEvenLeftEvenRight,
        HDMI_3DSubSampling_AllQuincunx,
        HDMI_3DSubSampling_QuincunxOddLeftOddRight,
        HDMI_3DSubSampling_QuincunxOddLeftEvenRight,
        HDMI_3DSubSampling_QuincunxEvenLeftOddRight,
        HDMI_3DSubSampling_QuincunxEvenLeftEvenRight
};

struct CEA861_MonitorRangeLimits
{
        UINT32 MinVerticalRate;    /* Hz */
        UINT32 MaxVerticalRate;    /* Hz */
        UINT32 MinHorizontalRate;  /* kHz */
        UINT32 MaxHorizontalRate;  /* kHz */
        UINT32 MaxPixelClock;      /* MHz */
};

struct CEA861_ShortAudioDescriptor
{
        enum HDMI_AudioCodingType AudioFormatCode;
        UINT8 MaxNumberOfChannels;
        UINT8 FrequencyMask;            /* See CEA861_AUDIO_FREQ_xxxx */
        union
        {
            /* Supported sample sizes for codec 1, see CEA861_AUDIO_PCM_XXX */
            UINT8 BitMask;
            UINT32 MaximumBitrate;  /* max_bitrate / 8 kHz for codecs 2..8 */
            UINT8 Option;           /* optional parameter for codecs 9..15 */
        } u;
};

#define HDMI_MAX_VIDEO_CODES 7  /* maximum number of HDMI 1.4 VICs */
#define HDMI_MAX_3D_LEN      31 /* maximum number of HDMI 1.4 3D structure bytes */

struct HDMI_3DExtDetail
{
        UINT8   HDMI_Multi_Present;

        UINT16  HDMI_3D_Structure_All; /* HDMI1.4a Table 8-19 */
        /* Where a bit is set(=1), for the corresponding VIC within the first 16 entries in
            the EDID, the Sink indicates 3D support as designated by 3D_Structure_All field. */
        UINT16  HDMI_3D_Mask;
        UINT8   HDMI_2D_VIC_Num;/* Number of 2D VICs */
        /* Value in this field is the pointer to particular VIC in EDID. */
        UINT8   HDMI_2D_VIC_Order[16];
        /* This field indicates 3D capability for corresponding VIC Code
            indicated by 2D_VIC_Order */
        UINT8   HDMI_3D_Structure[16];
        UINT8   HDMI_3D_Detail[16];
};

/*
 * Parsed info from one CEA extension block
 */
struct CEA861_DataBlockCollection
{
        UINT8   Version;

        /* With Version >= 2: */
        BOOLEAN DTVValid;    /* DTVSupport is valid */
        UINT8   DTVSupport;  /* See CEA861_DTV_... */

        /* With Version >= 3: */

        /*
         * Video Data Block
         */
        UINT8   nShortVideoDescriptor;
        UINT8   ShortVideoDescriptor[CEA861_MAX_VIDEO_DESCRIPTORS];

        /*
         * Audio Data Block
         */
        UINT8   nShortAudioDescriptor;
        struct CEA861_ShortAudioDescriptor ShortAudioDescriptor[CEA861_MAX_AUDIO_DESCRIPTORS];

        /*
         * Audio Speaker Allocation Block
         */
        BOOLEAN SpeakerValid;        /* SpeakerAllocation is valid */
        UINT8   SpeakerAllocation[3];/* See CEA861_SPEAKER_... and CEA 861-D 7.5.3 */

        /*
         * Extended Colorimetry Data Block
         */
        BOOLEAN ColorimetryValid;    /* ColorimetrySupport, MetaDataProfile are valid */
        UINT8   ColorimetrySupport;  /* See CEA861_COLORIMETRY_... and CEA 861-D 7.5.5 */
        UINT8   MetaDataProfile;     /* See CEA861_METADATAPROFILE_... and CEA 861-D 7.5.5 */

        /*
         * Extended Video Capability Data Block
         */
        BOOLEAN VideoValid;          /* VideoCapability is present */
        UINT8   VideoCapability;     /* See #define CEA861_VIDEO_... and CEA 861-D 7.5.6 */

        /*
         * Vendor Specific Data Block
         */
        /* Display conforms to HDMI and is capable to receive info frames,
            and HDMI_PhysAddr, CEASinkValid, CEATMDSValid and CEALatencyValid are valid */
        BOOLEAN HDMI;
        /* Physical position in DDC chain. A=[15:12],B=[11:8],C=[7:4],D=[3:0].
            See HDMI1.4a, sect. 8.7 */
        UINT16  HDMI_PhysAddr;

        BOOLEAN CEASinkValid;   /* SinkCapability is valid */
        UINT8   SinkCapability; /* See CEA861_SINK_... and HDMI1.4a sect. 8.3.2 */

        BOOLEAN CEATMDSValid;   /* MaxTMDSClock is valid */
        /* 0: not indicated, 1..255: TMDS supports clocks up to MaxTMDSClock * 5 MHz */
        UINT8   MaxTMDSClock;
        /* HDMIFlags8, LatencyPresent, VideoLatency, AudioLatency, InterlacedVideoLatency,
            InterlacedAudioLatency are valid */
        BOOLEAN CEALatencyValid;
        UINT8   LatencyPresent; /* See CEA861_LATENCY_... and HDMI1.4a sect. 8.3.2 */
        /* Video latency introduced by the sink, 0: unknown,
            255: video not supported, 1..251: (VideoLatency - 1) * 2 mSec
            (for progressive video only, when interlaced values are provided) */
        UINT8   VideoLatency;
        /* Audio latency introduced by the sink, 0: unknown,
            255: audio not supported, 1..251: (AudioLatency - 1) * 2 mSec ( " " ) */
        UINT8   AudioLatency;
        /* Video latency introduced by the sink, 0: unknown, 255: video not supported,
            1..251: (InterlacedVideoLatency - 1) * 2 mSec (for interlaced video) */
        UINT8   InterlacedVideoLatency;
        /* Audio latency introduced by the sink, 0: unknown, 255: audio not supported,
            1..251: (InterlacedAudioLatency - 1) * 2 mSec ( " " ) */
        UINT8   InterlacedAudioLatency;
        UINT8   HDMIFlags8;  /* See CEA861_FLAGS8_... and HDMI 1.4 sect. 8.3.2 */

        BOOLEAN HDMIVideoValid; /* HDMIFlags13, HDMIVIC and 3D values are valid */
        UINT8   HDMIFlags13;    /* See CEA861_FLAGS13_... and HDMI 1.4 sect. 8.3.2 */
        UINT32  nHDMIVIC;       /* Number of HDMI VICs in HDMIVIC array */
        UINT8   HDMIVIC[HDMI_MAX_VIDEO_CODES];

        UINT32  nStructure3D;   /* number of bytes in Structure3D[] array */
        /* Raw Data, See HDMI 1.4a HDMI_3D_STRUCTURE_... masks */
        UINT8   Structure3D[HDMI_MAX_3D_LEN];
        struct  HDMI_3DExtDetail  Structure3DDetails;
};

struct CEA861_DetailedTimingDescriptor
{
        UINT32  PixelClock;          /* in Hz */
        UINT16  HorizActivePixels;
        UINT16  HorizBlankingPixels;
        UINT16  VertActiveLines;
        UINT16  VertBlankingLines;
        UINT16  HorizSyncOffset;
        UINT16  HorizSyncPulseWidth;
        UINT16  VertSyncOffset;
        UINT16  VertSyncPulseWidth;
        UINT16  HorizImageSize_mm;
        UINT16  VertImageSize_mm;
        UINT8   HorizBorder;
        UINT8   VertBorder;
        BOOLEAN Interlaced;
        /* See E-EDID Standard Release A, Revision 1 (VESA) Table 3.17 page 19 */
        UINT8   StereoModeDecode;
        /* See E-EDID Standard Release A, Revision 1 (VESA) Table 3.18 page 19 */
        UINT8   SyncSignalDescription;
        UINT8   SyncSignalInfo;
        BOOLEAN Native;
};

struct CEA861_EDID
{
        struct CEA861_EDIDBlock0 Block0;               /* Basic EDID Block 0  */
        UINT32 nDBC;                                   /* Number of Data Block Collection */
        struct CEA861_DataBlockCollection DBCs[EDID_MAX_DBC_NUM];
        UINT32 nDTD;                                   /* Number of Detailed Timing Descriptor */
        struct CEA861_DetailedTimingDescriptor DTDs[EDID_MAX_DTD_NUM];
        /* Monitor Name parsed from basic block 0 */
        CHAR   MonitorName[14];
        BOOLEAN MonitorRangeLimitsValid;               /* Monitor Range Limits */
        struct CEA861_MonitorRangeLimits MonitorRangeLimits;
};


/*****************************************************************************
 * EDID Helper functions
 *    Checksum
 ****************************************************************************/
HRESULT EDID_Check_Checksum(UINT8 pEDID[EDID_SIZE]);
HRESULT EDID_Create_Checksum(UINT8 *pEDID, UINT32 nBlock);

/******************************************************************************
 * Translate functions from:
 *     VIC <-> TV standard
 ****************************************************************************/

HRESULT EDID_Tanslate_VIC_TO_TVStandard(
        UINT32  CEA_VIC,   /* CEA 861 VIC */
        UINT32  HDMI_VIC,  /* HDMI 1.4 "4k" VIC */
        /* use 24.00/30.00/60.00Hz instead of 23.97/29.97/59.94Hz mode, if applicable */
        BOOLEAN SixtyHertz,
        struct CEA861_VideoFormat *pVideoFormat);

HRESULT EDID_Translate_TVStandard_TO_VIC(
        enum TVStandard TVStandard,
        UINT32 AspX,
        UINT32 AspY,
        UINT32 *pCEA_VIC,
        UINT32 *pHDMI_VIC);

/*****************************************************************************
 * Blocks RAW Data -> Structures
 *     EDID Parsing Interface
 ****************************************************************************/

HRESULT EDID_Parse_Block0(
        UINT8                     pEDID[EDID_SIZE],
        struct CEA861_EDIDBlock0 *pEDIDBlock0);

HRESULT EDID_Parse_ExtBlock(
        UINT8 pEDID[EDID_SIZE],
        struct CEA861_DataBlockCollection *pDBC,
        UINT8 *pDescriptor, UINT32 *pnDescriptor, UINT32 *pnNativeDTD);

HRESULT EDID_Parse_DTD(
        UINT8           pDescriptor[EDID_DESCRIPTOR_SIZE],
        struct CEA861_DetailedTimingDescriptor *pDTD);

HRESULT EDID_Parse_MD_Name(
        UINT8 pDescriptor[EDID_DESCRIPTOR_SIZE],
        CHAR MonitorName[14]);

HRESULT EDID_Parse(
        UINT8 *pEDID, UINT32 nEDID,
        struct CEA861_EDID   *pParsedEDID);

/*****************************************************************************
 * Structures -> Blocks Raw Data
 *     EDID Generating Interface
 ****************************************************************************/

HRESULT EDID_Create_Block0(
        struct CEA861_EDIDBlock0 *pEDIDBlock0,
        UINT8                    *pEDID);

/*
 * Append a DTD to the last extension block, has to be called after EDID_Add_ExtDBC()
 */
HRESULT EDID_Append_ExtDTD(
        UINT8 *pEDID,
        struct CEA861_DetailedTimingDescriptor *pDTD);

/*
 * Add one 128 byte EDID extension data block from 'struct CEA861_DataBlockCollection'
 */
HRESULT EDID_Add_ExtDBC(
        UINT8 *pEDID,
        struct CEA861_DataBlockCollection *pDBC);

/*
 * Create 18 byte EDID 'monitor name' descriptor block
 */
HRESULT EDID_Create_MD_Name(
        UINT8 pDescriptor[EDID_DESCRIPTOR_SIZE],
        CHAR *MonitorName);

/*
 * Create 18 byte EDID 'monitor range limits' descriptor block
 */
HRESULT EDID_Create_MD_Range(
        UINT8 pDescriptor[EDID_DESCRIPTOR_SIZE],
        struct CEA861_MonitorRangeLimits *pRangeLimits);

HRESULT EDID_Create(
        UINT8 *pEDID, UINT8 nBlock,
        struct CEA861_EDID *pParsedEDID);

HRESULT Sample_Parse_EDID(UINT8 *pEDID, UINT32 nEDID);

#endif /* __EDID_H__ */
