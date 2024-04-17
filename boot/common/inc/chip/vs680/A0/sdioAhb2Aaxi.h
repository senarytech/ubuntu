//////
/// don't edit! auto-generated by docc: sdioAhb2Aaxi.h
////////////////////////////////////////////////////////////
#ifndef sdioAhb2Aaxi_h
#define sdioAhb2Aaxi_h (){}


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
/// $INTERFACE oneReg                                   (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (R-)
///               %unsigned 32 0x00000000                
///                                    ###
///                                    * One Register in an external IP block
///                                    ###
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:       4B, bits:      32b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_oneReg
#define h_oneReg (){}

    #define     BA_oneReg_0x00000000                           0x0000
    #define     B16oneReg_0x00000000                           0x0000
    #define   LSb32oneReg_0x00000000                              0
    #define   LSb16oneReg_0x00000000                              0
    #define       boneReg_0x00000000                           32
    #define   MSK32oneReg_0x00000000                              0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_oneReg {
    ///////////////////////////////////////////////////////////
    #define   GET32oneReg_0x00000000(r32)                      _BFGET_(r32,31, 0)
    #define   SET32oneReg_0x00000000(r32,v)                    _BFSET_(r32,31, 0,v)

            UNSG32 u_0x00000000                                : 32;
    ///////////////////////////////////////////////////////////
    } SIE_oneReg;

    ///////////////////////////////////////////////////////////
     SIGN32 oneReg_drvrd(SIE_oneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 oneReg_drvwr(SIE_oneReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void oneReg_reset(SIE_oneReg *p);
     SIGN32 oneReg_cmp  (SIE_oneReg *p, SIE_oneReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define oneReg_check(p,pie,pfx,hLOG) oneReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define oneReg_print(p,    pfx,hLOG) oneReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: oneReg
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE SDIO_REG                                 (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (P)
///     # 0x00000 dummy                
///               $oneReg              dummy             REG [4194304]
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:16777216B, bits:134217728b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_SDIO_REG
#define h_SDIO_REG (){}

    #define     RA_SDIO_REG_dummy                              0x0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_SDIO_REG {
    ///////////////////////////////////////////////////////////
              SIE_oneReg                                       ie_dummy[4194304];
    ///////////////////////////////////////////////////////////
    } SIE_SDIO_REG;

    ///////////////////////////////////////////////////////////
     SIGN32 SDIO_REG_drvrd(SIE_SDIO_REG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SDIO_REG_drvwr(SIE_SDIO_REG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SDIO_REG_reset(SIE_SDIO_REG *p);
     SIGN32 SDIO_REG_cmp  (SIE_SDIO_REG *p, SIE_SDIO_REG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SDIO_REG_check(p,pie,pfx,hLOG) SDIO_REG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SDIO_REG_print(p,    pfx,hLOG) SDIO_REG_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: SDIO_REG
////////////////////////////////////////////////////////////

//////
/// 
/// $INTERFACE sdio                    biu              (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000                      (R-)
///     # 0x00000 cfgReg               
///               $SDIO_REG            cfgReg            MEM          
///                                    ###
///                                    * Register Space, 64KB. (Hier:3)
///                                    ###
///     @ 0x1000000 dummy1               (R-)
///               %unsigned 32 STS                       0x0
///                                    ###
///                                    * Dummy register for ISC to generate the BIU.
///                                    ###
///     @ 0x1000004                      (W-)
///     #         # Stuffing bytes...
///               %% 134217696
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:33554432B, bits:      64b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_sdio
#define h_sdio (){}

    #define     RA_sdio_cfgReg                                 0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_sdio_dummy1                                 0x1000000

    #define     BA_sdio_dummy1_STS                             0x1000000
    #define     B16sdio_dummy1_STS                             0x1000000
    #define   LSb32sdio_dummy1_STS                                0
    #define   LSb16sdio_dummy1_STS                                0
    #define       bsdio_dummy1_STS                             32
    #define   MSK32sdio_dummy1_STS                                0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_sdio {
    ///////////////////////////////////////////////////////////
              SIE_SDIO_REG                                     ie_cfgReg;
    ///////////////////////////////////////////////////////////
    #define   GET32sdio_dummy1_STS(r32)                        _BFGET_(r32,31, 0)
    #define   SET32sdio_dummy1_STS(r32,v)                      _BFSET_(r32,31, 0,v)

    #define     w32sdio_dummy1                                 {\
            UNSG32 udummy1_STS                                 : 32;\
          }
    union { UNSG32 u32sdio_dummy1;
            struct w32sdio_dummy1;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx1000004                                [16777212];
    ///////////////////////////////////////////////////////////
    } SIE_sdio;

    typedef union  T32sdio_dummy1
          { UNSG32 u32;
            struct w32sdio_dummy1;
                 } T32sdio_dummy1;
    ///////////////////////////////////////////////////////////

    typedef union  Tsdio_dummy1
          { UNSG32 u32[1];
            struct {
            struct w32sdio_dummy1;
                   };
                 } Tsdio_dummy1;

    ///////////////////////////////////////////////////////////
     SIGN32 sdio_drvrd(SIE_sdio *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 sdio_drvwr(SIE_sdio *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void sdio_reset(SIE_sdio *p);
     SIGN32 sdio_cmp  (SIE_sdio *p, SIE_sdio *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define sdio_check(p,pie,pfx,hLOG) sdio_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define sdio_print(p,    pfx,hLOG) sdio_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: sdio
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: sdioAhb2Aaxi.h
////////////////////////////////////////////////////////////
