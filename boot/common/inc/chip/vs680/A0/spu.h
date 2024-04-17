//////
/// don't edit! auto-generated by docc: spu.h
////////////////////////////////////////////////////////////
#ifndef spu_h
#define spu_h (){}


#include "ctypes.h"

#pragma pack(1)
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _DOCC_H_BITOPS_
#define _DOCC_H_BITOPS_ (){}

    #define _bSETMASK_(b)                                      ((b)<32 ? (1<<((b)&31)) : 0)
    #define _NSETMASK_(msb,lsb)                                (_bSETMASK_((msb)+1)-_bSETMASK_(lsb))
    #define _bCLRMASK_(b)                                      (~_bSETMASK_(b))
    #define _NCLRMASK_(msb,lsb)                                (~_NSETMASK_(msb,lsb))
    #define _BFGET_(r,msb,lsb)                                 (_NSETMASK_((msb)-(lsb),0)&((r)>>(lsb)))
    #define _BFSET_(r,msb,lsb,v)                               do{ (r)&=_NCLRMASK_(msb,lsb); (r)|=_NSETMASK_(msb,lsb)&((v)<<(lsb)); }while(0)

#endif



//////
/// 
/// $INTERFACE spuSrcInfo              biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 ddr                  (P)
///               %unsigned 3  adr                       
///                                    ###
///                                    * The stream start address inside DDR, only the lsb [2:0] are used. It sprcifies how many bytes need to be dropped from the first dHub word.
///                                    ###
///               %%        29         # Stuffing bits...
///     @ 0x00004 strm                 (P)
///               %unsigned 24 size                      
///                                    ###
///                                    * The stream size in byte.
///                                    ###
///               %%        8          # Stuffing bits...
///     @ 0x00008 total                (P)
///               %unsigned 16 x                         
///               %unsigned 16 y                         
///                                    ###
///                                    * The stream defined frame size.
///                                    ###
///     @ 0x0000C start                (P)
///               %unsigned 16 x                         0x0
///               %unsigned 16 y                         0x0
///                                    ###
///                                    * The start location of the cropping plane.
///                                    ###
///     @ 0x00010 size                 (P)
///               %unsigned 16 x                         0x64
///               %unsigned 16 y                         0x64
///                                    ###
///                                    * The cropping plane size.
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      20B, bits:     123b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_spuSrcInfo
#define h_spuSrcInfo (){}

    #define     RA_spuSrcInfo_ddr                              0x0000

    #define     BA_spuSrcInfo_ddr_adr                          0x0000
    #define     B16spuSrcInfo_ddr_adr                          0x0000
    #define   LSb32spuSrcInfo_ddr_adr                             0
    #define   LSb16spuSrcInfo_ddr_adr                             0
    #define       bspuSrcInfo_ddr_adr                          3
    #define   MSK32spuSrcInfo_ddr_adr                             0x00000007
    ///////////////////////////////////////////////////////////
    #define     RA_spuSrcInfo_strm                             0x0004

    #define     BA_spuSrcInfo_strm_size                        0x0004
    #define     B16spuSrcInfo_strm_size                        0x0004
    #define   LSb32spuSrcInfo_strm_size                           0
    #define   LSb16spuSrcInfo_strm_size                           0
    #define       bspuSrcInfo_strm_size                        24
    #define   MSK32spuSrcInfo_strm_size                           0x00FFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_spuSrcInfo_total                            0x0008

    #define     BA_spuSrcInfo_total_x                          0x0008
    #define     B16spuSrcInfo_total_x                          0x0008
    #define   LSb32spuSrcInfo_total_x                             0
    #define   LSb16spuSrcInfo_total_x                             0
    #define       bspuSrcInfo_total_x                          16
    #define   MSK32spuSrcInfo_total_x                             0x0000FFFF

    #define     BA_spuSrcInfo_total_y                          0x000A
    #define     B16spuSrcInfo_total_y                          0x000A
    #define   LSb32spuSrcInfo_total_y                             16
    #define   LSb16spuSrcInfo_total_y                             0
    #define       bspuSrcInfo_total_y                          16
    #define   MSK32spuSrcInfo_total_y                             0xFFFF0000
    ///////////////////////////////////////////////////////////
    #define     RA_spuSrcInfo_start                            0x000C

    #define     BA_spuSrcInfo_start_x                          0x000C
    #define     B16spuSrcInfo_start_x                          0x000C
    #define   LSb32spuSrcInfo_start_x                             0
    #define   LSb16spuSrcInfo_start_x                             0
    #define       bspuSrcInfo_start_x                          16
    #define   MSK32spuSrcInfo_start_x                             0x0000FFFF

    #define     BA_spuSrcInfo_start_y                          0x000E
    #define     B16spuSrcInfo_start_y                          0x000E
    #define   LSb32spuSrcInfo_start_y                             16
    #define   LSb16spuSrcInfo_start_y                             0
    #define       bspuSrcInfo_start_y                          16
    #define   MSK32spuSrcInfo_start_y                             0xFFFF0000
    ///////////////////////////////////////////////////////////
    #define     RA_spuSrcInfo_size                             0x0010

    #define     BA_spuSrcInfo_size_x                           0x0010
    #define     B16spuSrcInfo_size_x                           0x0010
    #define   LSb32spuSrcInfo_size_x                              0
    #define   LSb16spuSrcInfo_size_x                              0
    #define       bspuSrcInfo_size_x                           16
    #define   MSK32spuSrcInfo_size_x                              0x0000FFFF

    #define     BA_spuSrcInfo_size_y                           0x0012
    #define     B16spuSrcInfo_size_y                           0x0012
    #define   LSb32spuSrcInfo_size_y                              16
    #define   LSb16spuSrcInfo_size_y                              0
    #define       bspuSrcInfo_size_y                           16
    #define   MSK32spuSrcInfo_size_y                              0xFFFF0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_spuSrcInfo {
    ///////////////////////////////////////////////////////////
    #define   GET32spuSrcInfo_ddr_adr(r32)                     _BFGET_(r32, 2, 0)
    #define   SET32spuSrcInfo_ddr_adr(r32,v)                   _BFSET_(r32, 2, 0,v)
    #define   GET16spuSrcInfo_ddr_adr(r16)                     _BFGET_(r16, 2, 0)
    #define   SET16spuSrcInfo_ddr_adr(r16,v)                   _BFSET_(r16, 2, 0,v)

    #define     w32spuSrcInfo_ddr                              {\
            UNSG32 uddr_adr                                    :  3;\
            UNSG32 RSVDx0_b3                                   : 29;\
          }
    union { UNSG32 u32spuSrcInfo_ddr;
            struct w32spuSrcInfo_ddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuSrcInfo_strm_size(r32)                   _BFGET_(r32,23, 0)
    #define   SET32spuSrcInfo_strm_size(r32,v)                 _BFSET_(r32,23, 0,v)

    #define     w32spuSrcInfo_strm                             {\
            UNSG32 ustrm_size                                  : 24;\
            UNSG32 RSVDx4_b24                                  :  8;\
          }
    union { UNSG32 u32spuSrcInfo_strm;
            struct w32spuSrcInfo_strm;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuSrcInfo_total_x(r32)                     _BFGET_(r32,15, 0)
    #define   SET32spuSrcInfo_total_x(r32,v)                   _BFSET_(r32,15, 0,v)
    #define   GET16spuSrcInfo_total_x(r16)                     _BFGET_(r16,15, 0)
    #define   SET16spuSrcInfo_total_x(r16,v)                   _BFSET_(r16,15, 0,v)

    #define   GET32spuSrcInfo_total_y(r32)                     _BFGET_(r32,31,16)
    #define   SET32spuSrcInfo_total_y(r32,v)                   _BFSET_(r32,31,16,v)
    #define   GET16spuSrcInfo_total_y(r16)                     _BFGET_(r16,15, 0)
    #define   SET16spuSrcInfo_total_y(r16,v)                   _BFSET_(r16,15, 0,v)

    #define     w32spuSrcInfo_total                            {\
            UNSG32 utotal_x                                    : 16;\
            UNSG32 utotal_y                                    : 16;\
          }
    union { UNSG32 u32spuSrcInfo_total;
            struct w32spuSrcInfo_total;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuSrcInfo_start_x(r32)                     _BFGET_(r32,15, 0)
    #define   SET32spuSrcInfo_start_x(r32,v)                   _BFSET_(r32,15, 0,v)
    #define   GET16spuSrcInfo_start_x(r16)                     _BFGET_(r16,15, 0)
    #define   SET16spuSrcInfo_start_x(r16,v)                   _BFSET_(r16,15, 0,v)

    #define   GET32spuSrcInfo_start_y(r32)                     _BFGET_(r32,31,16)
    #define   SET32spuSrcInfo_start_y(r32,v)                   _BFSET_(r32,31,16,v)
    #define   GET16spuSrcInfo_start_y(r16)                     _BFGET_(r16,15, 0)
    #define   SET16spuSrcInfo_start_y(r16,v)                   _BFSET_(r16,15, 0,v)

    #define     w32spuSrcInfo_start                            {\
            UNSG32 ustart_x                                    : 16;\
            UNSG32 ustart_y                                    : 16;\
          }
    union { UNSG32 u32spuSrcInfo_start;
            struct w32spuSrcInfo_start;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuSrcInfo_size_x(r32)                      _BFGET_(r32,15, 0)
    #define   SET32spuSrcInfo_size_x(r32,v)                    _BFSET_(r32,15, 0,v)
    #define   GET16spuSrcInfo_size_x(r16)                      _BFGET_(r16,15, 0)
    #define   SET16spuSrcInfo_size_x(r16,v)                    _BFSET_(r16,15, 0,v)

    #define   GET32spuSrcInfo_size_y(r32)                      _BFGET_(r32,31,16)
    #define   SET32spuSrcInfo_size_y(r32,v)                    _BFSET_(r32,31,16,v)
    #define   GET16spuSrcInfo_size_y(r16)                      _BFGET_(r16,15, 0)
    #define   SET16spuSrcInfo_size_y(r16,v)                    _BFSET_(r16,15, 0,v)

    #define     w32spuSrcInfo_size                             {\
            UNSG32 usize_x                                     : 16;\
            UNSG32 usize_y                                     : 16;\
          }
    union { UNSG32 u32spuSrcInfo_size;
            struct w32spuSrcInfo_size;
          };
    ///////////////////////////////////////////////////////////
    } SIE_spuSrcInfo;

    typedef union  T32spuSrcInfo_ddr
          { UNSG32 u32;
            struct w32spuSrcInfo_ddr;
                 } T32spuSrcInfo_ddr;
    typedef union  T32spuSrcInfo_strm
          { UNSG32 u32;
            struct w32spuSrcInfo_strm;
                 } T32spuSrcInfo_strm;
    typedef union  T32spuSrcInfo_total
          { UNSG32 u32;
            struct w32spuSrcInfo_total;
                 } T32spuSrcInfo_total;
    typedef union  T32spuSrcInfo_start
          { UNSG32 u32;
            struct w32spuSrcInfo_start;
                 } T32spuSrcInfo_start;
    typedef union  T32spuSrcInfo_size
          { UNSG32 u32;
            struct w32spuSrcInfo_size;
                 } T32spuSrcInfo_size;
    ///////////////////////////////////////////////////////////

    typedef union  TspuSrcInfo_ddr
          { UNSG32 u32[1];
            struct {
            struct w32spuSrcInfo_ddr;
                   };
                 } TspuSrcInfo_ddr;
    typedef union  TspuSrcInfo_strm
          { UNSG32 u32[1];
            struct {
            struct w32spuSrcInfo_strm;
                   };
                 } TspuSrcInfo_strm;
    typedef union  TspuSrcInfo_total
          { UNSG32 u32[1];
            struct {
            struct w32spuSrcInfo_total;
                   };
                 } TspuSrcInfo_total;
    typedef union  TspuSrcInfo_start
          { UNSG32 u32[1];
            struct {
            struct w32spuSrcInfo_start;
                   };
                 } TspuSrcInfo_start;
    typedef union  TspuSrcInfo_size
          { UNSG32 u32[1];
            struct {
            struct w32spuSrcInfo_size;
                   };
                 } TspuSrcInfo_size;

    ///////////////////////////////////////////////////////////
     SIGN32 spuSrcInfo_drvrd(SIE_spuSrcInfo *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 spuSrcInfo_drvwr(SIE_spuSrcInfo *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void spuSrcInfo_reset(SIE_spuSrcInfo *p);
     SIGN32 spuSrcInfo_cmp  (SIE_spuSrcInfo *p, SIE_spuSrcInfo *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define spuSrcInfo_check(p,pie,pfx,hLOG) spuSrcInfo_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define spuSrcInfo_print(p,    pfx,hLOG) spuSrcInfo_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: spuSrcInfo
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE spuDstInfo              biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 ddr                  (P)
///               %unsigned 3  adr                       
///                                    ###
///                                    * 2D frame buffer address, H/W will write the cropped frame to this address. Could be any byte address, the lsb [2:0] are used for dHub word padding for each start of a line.
///                                    ###
///               %%        29         # Stuffing bits...
///     @ 0x00004 line                 (P)
///               %unsigned 16 stride                    
///                                    ###
///                                    * Line stride size in byte.
///                                    * It should be 64-bit aligned.
///                                    ###
///               %%        16         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:       8B, bits:      19b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_spuDstInfo
#define h_spuDstInfo (){}

    #define     RA_spuDstInfo_ddr                              0x0000

    #define     BA_spuDstInfo_ddr_adr                          0x0000
    #define     B16spuDstInfo_ddr_adr                          0x0000
    #define   LSb32spuDstInfo_ddr_adr                             0
    #define   LSb16spuDstInfo_ddr_adr                             0
    #define       bspuDstInfo_ddr_adr                          3
    #define   MSK32spuDstInfo_ddr_adr                             0x00000007
    ///////////////////////////////////////////////////////////
    #define     RA_spuDstInfo_line                             0x0004

    #define     BA_spuDstInfo_line_stride                      0x0004
    #define     B16spuDstInfo_line_stride                      0x0004
    #define   LSb32spuDstInfo_line_stride                         0
    #define   LSb16spuDstInfo_line_stride                         0
    #define       bspuDstInfo_line_stride                      16
    #define   MSK32spuDstInfo_line_stride                         0x0000FFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_spuDstInfo {
    ///////////////////////////////////////////////////////////
    #define   GET32spuDstInfo_ddr_adr(r32)                     _BFGET_(r32, 2, 0)
    #define   SET32spuDstInfo_ddr_adr(r32,v)                   _BFSET_(r32, 2, 0,v)
    #define   GET16spuDstInfo_ddr_adr(r16)                     _BFGET_(r16, 2, 0)
    #define   SET16spuDstInfo_ddr_adr(r16,v)                   _BFSET_(r16, 2, 0,v)

    #define     w32spuDstInfo_ddr                              {\
            UNSG32 uddr_adr                                    :  3;\
            UNSG32 RSVDx0_b3                                   : 29;\
          }
    union { UNSG32 u32spuDstInfo_ddr;
            struct w32spuDstInfo_ddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuDstInfo_line_stride(r32)                 _BFGET_(r32,15, 0)
    #define   SET32spuDstInfo_line_stride(r32,v)               _BFSET_(r32,15, 0,v)
    #define   GET16spuDstInfo_line_stride(r16)                 _BFGET_(r16,15, 0)
    #define   SET16spuDstInfo_line_stride(r16,v)               _BFSET_(r16,15, 0,v)

    #define     w32spuDstInfo_line                             {\
            UNSG32 uline_stride                                : 16;\
            UNSG32 RSVDx4_b16                                  : 16;\
          }
    union { UNSG32 u32spuDstInfo_line;
            struct w32spuDstInfo_line;
          };
    ///////////////////////////////////////////////////////////
    } SIE_spuDstInfo;

    typedef union  T32spuDstInfo_ddr
          { UNSG32 u32;
            struct w32spuDstInfo_ddr;
                 } T32spuDstInfo_ddr;
    typedef union  T32spuDstInfo_line
          { UNSG32 u32;
            struct w32spuDstInfo_line;
                 } T32spuDstInfo_line;
    ///////////////////////////////////////////////////////////

    typedef union  TspuDstInfo_ddr
          { UNSG32 u32[1];
            struct {
            struct w32spuDstInfo_ddr;
                   };
                 } TspuDstInfo_ddr;
    typedef union  TspuDstInfo_line
          { UNSG32 u32[1];
            struct {
            struct w32spuDstInfo_line;
                   };
                 } TspuDstInfo_line;

    ///////////////////////////////////////////////////////////
     SIGN32 spuDstInfo_drvrd(SIE_spuDstInfo *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 spuDstInfo_drvwr(SIE_spuDstInfo *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void spuDstInfo_reset(SIE_spuDstInfo *p);
     SIGN32 spuDstInfo_cmp  (SIE_spuDstInfo *p, SIE_spuDstInfo *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define spuDstInfo_check(p,pie,pfx,hLOG) spuDstInfo_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define spuDstInfo_print(p,    pfx,hLOG) spuDstInfo_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: spuDstInfo
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE spuReg                  biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (P)
///     # 0x00000 rleStrm              
///               $spuSrcInfo          rleStrm           REG          
///     @ 0x00014                      (P)
///     # 0x00014 frame                
///               $spuDstInfo          frame             REG          
///     @ 0x0001C start                (P)
///               %unsigned 1  en                        
///                                    ###
///                                    * Write '1' to this register will kick off the the spu engine start loading stream data from dHub, and the rest of the spu system will rely on the stream to start working.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00020 clr                  (P)
///               %unsigned 1  en                        
///                                    ###
///                                    * Write '1' to this register will reset all the internal state machine or FIFOs.
///                                    * This is used for S/W to reset the spu engine when something error happens inside spu. For example, spu detects illegal stream pattern.
///                                    ###
///               %%        31         # Stuffing bits...
///     @ 0x00024 st                   (WOC-)
///               ###
///               * The register will be cleared to '0' if writing '1' to the corresponding bit field.
///               ###
///               %unsigned 1  spu_busy                  0x0
///                                    ###
///                                    * SPU is busy (loading stream, decoding or driving data out to dHub)
///                                    ###
///               %unsigned 1  rleErr0                   0x0
///                                    ###
///                                    * The rest of the bits are not enough to compose any valid SE.
///                                    ###
///               %unsigned 1  rleErr1                   0x0
///                                    ###
///                                    * Illegal stream is detected. For example, the run-length_64_14k bit fields come out a value less than 64.
///                                    ###
///               %unsigned 1  rleErr2                   0x0
///                                    ###
///                                    * The decoded frame size is smaller than expected frame size.
///                                    ###
///               %unsigned 1  rleErr3                   0x0
///                                    ###
///                                    * The decoded frame size is bigger than expected frame size.
///                                    ###
///               %%        27         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      40B, bits:     149b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_spuReg
#define h_spuReg (){}

    #define     RA_spuReg_rleStrm                              0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_spuReg_frame                                0x0014
    ///////////////////////////////////////////////////////////
    #define     RA_spuReg_start                                0x001C

    #define     BA_spuReg_start_en                             0x001C
    #define     B16spuReg_start_en                             0x001C
    #define   LSb32spuReg_start_en                                0
    #define   LSb16spuReg_start_en                                0
    #define       bspuReg_start_en                             1
    #define   MSK32spuReg_start_en                                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_spuReg_clr                                  0x0020

    #define     BA_spuReg_clr_en                               0x0020
    #define     B16spuReg_clr_en                               0x0020
    #define   LSb32spuReg_clr_en                                  0
    #define   LSb16spuReg_clr_en                                  0
    #define       bspuReg_clr_en                               1
    #define   MSK32spuReg_clr_en                                  0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_spuReg_st                                   0x0024

    #define     BA_spuReg_st_spu_busy                          0x0024
    #define     B16spuReg_st_spu_busy                          0x0024
    #define   LSb32spuReg_st_spu_busy                             0
    #define   LSb16spuReg_st_spu_busy                             0
    #define       bspuReg_st_spu_busy                          1
    #define   MSK32spuReg_st_spu_busy                             0x00000001

    #define     BA_spuReg_st_rleErr0                           0x0024
    #define     B16spuReg_st_rleErr0                           0x0024
    #define   LSb32spuReg_st_rleErr0                              1
    #define   LSb16spuReg_st_rleErr0                              1
    #define       bspuReg_st_rleErr0                           1
    #define   MSK32spuReg_st_rleErr0                              0x00000002

    #define     BA_spuReg_st_rleErr1                           0x0024
    #define     B16spuReg_st_rleErr1                           0x0024
    #define   LSb32spuReg_st_rleErr1                              2
    #define   LSb16spuReg_st_rleErr1                              2
    #define       bspuReg_st_rleErr1                           1
    #define   MSK32spuReg_st_rleErr1                              0x00000004

    #define     BA_spuReg_st_rleErr2                           0x0024
    #define     B16spuReg_st_rleErr2                           0x0024
    #define   LSb32spuReg_st_rleErr2                              3
    #define   LSb16spuReg_st_rleErr2                              3
    #define       bspuReg_st_rleErr2                           1
    #define   MSK32spuReg_st_rleErr2                              0x00000008

    #define     BA_spuReg_st_rleErr3                           0x0024
    #define     B16spuReg_st_rleErr3                           0x0024
    #define   LSb32spuReg_st_rleErr3                              4
    #define   LSb16spuReg_st_rleErr3                              4
    #define       bspuReg_st_rleErr3                           1
    #define   MSK32spuReg_st_rleErr3                              0x00000010
    ///////////////////////////////////////////////////////////

    typedef struct SIE_spuReg {
    ///////////////////////////////////////////////////////////
              SIE_spuSrcInfo                                   ie_rleStrm;
    ///////////////////////////////////////////////////////////
              SIE_spuDstInfo                                   ie_frame;
    ///////////////////////////////////////////////////////////
    #define   GET32spuReg_start_en(r32)                        _BFGET_(r32, 0, 0)
    #define   SET32spuReg_start_en(r32,v)                      _BFSET_(r32, 0, 0,v)
    #define   GET16spuReg_start_en(r16)                        _BFGET_(r16, 0, 0)
    #define   SET16spuReg_start_en(r16,v)                      _BFSET_(r16, 0, 0,v)

    #define     w32spuReg_start                                {\
            UNSG32 ustart_en                                   :  1;\
            UNSG32 RSVDx1C_b1                                  : 31;\
          }
    union { UNSG32 u32spuReg_start;
            struct w32spuReg_start;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuReg_clr_en(r32)                          _BFGET_(r32, 0, 0)
    #define   SET32spuReg_clr_en(r32,v)                        _BFSET_(r32, 0, 0,v)
    #define   GET16spuReg_clr_en(r16)                          _BFGET_(r16, 0, 0)
    #define   SET16spuReg_clr_en(r16,v)                        _BFSET_(r16, 0, 0,v)

    #define     w32spuReg_clr                                  {\
            UNSG32 uclr_en                                     :  1;\
            UNSG32 RSVDx20_b1                                  : 31;\
          }
    union { UNSG32 u32spuReg_clr;
            struct w32spuReg_clr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32spuReg_st_spu_busy(r32)                     _BFGET_(r32, 0, 0)
    #define   SET32spuReg_st_spu_busy(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   GET16spuReg_st_spu_busy(r16)                     _BFGET_(r16, 0, 0)
    #define   SET16spuReg_st_spu_busy(r16,v)                   _BFSET_(r16, 0, 0,v)

    #define   GET32spuReg_st_rleErr0(r32)                      _BFGET_(r32, 1, 1)
    #define   SET32spuReg_st_rleErr0(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   GET16spuReg_st_rleErr0(r16)                      _BFGET_(r16, 1, 1)
    #define   SET16spuReg_st_rleErr0(r16,v)                    _BFSET_(r16, 1, 1,v)

    #define   GET32spuReg_st_rleErr1(r32)                      _BFGET_(r32, 2, 2)
    #define   SET32spuReg_st_rleErr1(r32,v)                    _BFSET_(r32, 2, 2,v)
    #define   GET16spuReg_st_rleErr1(r16)                      _BFGET_(r16, 2, 2)
    #define   SET16spuReg_st_rleErr1(r16,v)                    _BFSET_(r16, 2, 2,v)

    #define   GET32spuReg_st_rleErr2(r32)                      _BFGET_(r32, 3, 3)
    #define   SET32spuReg_st_rleErr2(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   GET16spuReg_st_rleErr2(r16)                      _BFGET_(r16, 3, 3)
    #define   SET16spuReg_st_rleErr2(r16,v)                    _BFSET_(r16, 3, 3,v)

    #define   GET32spuReg_st_rleErr3(r32)                      _BFGET_(r32, 4, 4)
    #define   SET32spuReg_st_rleErr3(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   GET16spuReg_st_rleErr3(r16)                      _BFGET_(r16, 4, 4)
    #define   SET16spuReg_st_rleErr3(r16,v)                    _BFSET_(r16, 4, 4,v)

    #define     w32spuReg_st                                   {\
            UNSG32 ust_spu_busy                                :  1;\
            UNSG32 ust_rleErr0                                 :  1;\
            UNSG32 ust_rleErr1                                 :  1;\
            UNSG32 ust_rleErr2                                 :  1;\
            UNSG32 ust_rleErr3                                 :  1;\
            UNSG32 RSVDx24_b5                                  : 27;\
          }
    union { UNSG32 u32spuReg_st;
            struct w32spuReg_st;
          };
    ///////////////////////////////////////////////////////////
    } SIE_spuReg;

    typedef union  T32spuReg_start
          { UNSG32 u32;
            struct w32spuReg_start;
                 } T32spuReg_start;
    typedef union  T32spuReg_clr
          { UNSG32 u32;
            struct w32spuReg_clr;
                 } T32spuReg_clr;
    typedef union  T32spuReg_st
          { UNSG32 u32;
            struct w32spuReg_st;
                 } T32spuReg_st;
    ///////////////////////////////////////////////////////////

    typedef union  TspuReg_start
          { UNSG32 u32[1];
            struct {
            struct w32spuReg_start;
                   };
                 } TspuReg_start;
    typedef union  TspuReg_clr
          { UNSG32 u32[1];
            struct {
            struct w32spuReg_clr;
                   };
                 } TspuReg_clr;
    typedef union  TspuReg_st
          { UNSG32 u32[1];
            struct {
            struct w32spuReg_st;
                   };
                 } TspuReg_st;

    ///////////////////////////////////////////////////////////
     SIGN32 spuReg_drvrd(SIE_spuReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 spuReg_drvwr(SIE_spuReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void spuReg_reset(SIE_spuReg *p);
     SIGN32 spuReg_cmp  (SIE_spuReg *p, SIE_spuReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define spuReg_check(p,pie,pfx,hLOG) spuReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define spuReg_print(p,    pfx,hLOG) spuReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: spuReg
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: spu.h
////////////////////////////////////////////////////////////

