//////
/// don't edit! auto-generated by docc: tsen.h
////////////////////////////////////////////////////////////
#ifndef tsen_h
#define tsen_h (){}


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
/// $INTERFACE TSEN                                     (4,4)
///     # # ----------------------------------------------------------
///     @ 0x00000 CTRL                 (P-)
///               ###
///               * PLL Control register
///               ###
///               %unsigned 1  ENA                       0x0
///                                    ###
///                                    * An asynchronous control signal commanding the sensor to re-evaluate the local die PVT parameter (active High). When ENA goes from 0 to 1, all control signals: VSAMPLE, PSAMPLE[1:0], TRIMG[4:0] and TRIMO[5:0], will be sampled.
///                                    ###
///               %unsigned 1  CLK_EN                    0x0
///                                    ###
///                                    * Enable to the divby20 clock divider circuit to generate clock TSEN_CLK
///                                    ###
///               %unsigned 2  PSAMPLE                   0x0
///                                    ###
///                                    * Select for Process sample mode
///                                    * ENA =0, VSAMPLE=PSAMPLE0=PSAMPLE1=X => Reset
///                                    * ENA=1, VSAMPLE=PSAMPLE0=PSAMPLE1=0   => Temperature
///                                    * ENA=1, VSAMPLE=1, PSAMPLE0=PSAMPLE1=X => Voltage evaluation.
///                                    * ENA=1, VSAMPLE=0, PSAMPLE0=1, PSAMPLE1=0 => Process evaluation (LVT)
///                                    * ENA=1, VSAMPLE=0, PSAMPLE0=0, PSAMPLE1=1 => Process evaluation (ULVT)
///                                    * ENA=1, VSAMPLE=0, PSAMPLE0=1, PSAMPLE1=1 => Process evaluation (SLVT)
///                                    ###
///               %unsigned 1  VSAMPLE                   0x0
///                                    ###
///                                    * Select for voltage sample mode (active High)
///                                    ###
///               %unsigned 5  TRIMG                     0xF
///                                    ###
///                                    * Trim gain bits. Optional programmable inputs which may be used to improve the absolute accuracy of the temperature sensor.
///                                    ###
///               %unsigned 6  TRIMO                     0x0
///                                    ###
///                                    * Trim offset bits. Optional programmable inputs which may be used to improve the absolute accuracy of the temperature sensor.
///                                    ###
///               %unsigned 5  TSEN_DAT_LT               0x0
///                                    ###
///                                    * Delay interms of pclk after data ready at which the data is to be latched
///                                    ###
///               %%        11         # Stuffing bits...
///     @ 0x00004 STATUS               (RW)
///               ###
///               * TSEN status register
///               ###
///               %unsigned 1  DATA_RDY                  0x0
///                                    ###
///                                    * ADC data ready, write 0 to clear interrupt, write 1 won't effect anything
///                                    * 0: no valid data
///                                    * 1: data ready to read
///                                    ###
///               %unsigned 1  INT_EN                    0x0
///                                    ###
///                                    * ADC interrupt enable
///                                    * 0: disable
///                                    * 1: enable
///                                    ###
///               %%        30         # Stuffing bits...
///     @ 0x00008 DATA                 (R-)
///               %unsigned 10 DATA                      
///                                    ###
///                                    * A 10 bit output representing the sensed temperature or voltage.
///                                    ###
///               %%        22         # Stuffing bits...
///     @ 0x0000C TSEN_CHK_CTRL        (P-)
///               %unsigned 10 TSEN_DATA_MAX             0x3FF
///                                    ###
///                                    * Maximum value used to check against TSEN_DATA
///                                    ###
///               %unsigned 10 TSEN_DATA_MIN             0x0
///                                    ###
///                                    * Minimum value used to check against TSEN_DATA
///                                    ###
///               %unsigned 1  TSEN_OVERHEAT_SEL         0x0
///                                    ###
///                                    * 0: Overheat flag uses TSEN_MAX_FAIL
///                                    * 1: Overheat flag uses TSEN_MIN_FAIL
///                                    ###
///               %%        11         # Stuffing bits...
///     @ 0x00010 TSEN_DATA_STATUS     (R-)
///               ###
///               * SM TSEN Data Threshold Check Status
///               ###
///               %unsigned 1  TSEN_MAX_FAIL             0x0
///                                    ###
///                                    * 0: TSEN_DATA <= TSEN_DATA_MAX
///                                    * 1: TSEN_DATA > TSEN_DATA_MAX
///                                    ###
///               %unsigned 1  TSEN_MIN_FAIL             0x0
///                                    ###
///                                    * 0: TSEN_DATA >= TSEN_DATA_MIN
///                                    * 1: TSEN_DATA < TSEN_DATA_MIN
///                                    ###
///               %%        30         # Stuffing bits...
///     # # ----------------------------------------------------------
/// $ENDOFINTERFACE  # size:      20B, bits:      56b, padding:     0B
////////////////////////////////////////////////////////////
#ifndef h_TSEN
#define h_TSEN (){}

    #define     RA_TSEN_CTRL                                   0x0000

    #define     BA_TSEN_CTRL_ENA                               0x0000
    #define     B16TSEN_CTRL_ENA                               0x0000
    #define   LSb32TSEN_CTRL_ENA                                  0
    #define   LSb16TSEN_CTRL_ENA                                  0
    #define       bTSEN_CTRL_ENA                               1
    #define   MSK32TSEN_CTRL_ENA                                  0x00000001

    #define     BA_TSEN_CTRL_CLK_EN                            0x0000
    #define     B16TSEN_CTRL_CLK_EN                            0x0000
    #define   LSb32TSEN_CTRL_CLK_EN                               1
    #define   LSb16TSEN_CTRL_CLK_EN                               1
    #define       bTSEN_CTRL_CLK_EN                            1
    #define   MSK32TSEN_CTRL_CLK_EN                               0x00000002

    #define     BA_TSEN_CTRL_PSAMPLE                           0x0000
    #define     B16TSEN_CTRL_PSAMPLE                           0x0000
    #define   LSb32TSEN_CTRL_PSAMPLE                              2
    #define   LSb16TSEN_CTRL_PSAMPLE                              2
    #define       bTSEN_CTRL_PSAMPLE                           2
    #define   MSK32TSEN_CTRL_PSAMPLE                              0x0000000C

    #define     BA_TSEN_CTRL_VSAMPLE                           0x0000
    #define     B16TSEN_CTRL_VSAMPLE                           0x0000
    #define   LSb32TSEN_CTRL_VSAMPLE                              4
    #define   LSb16TSEN_CTRL_VSAMPLE                              4
    #define       bTSEN_CTRL_VSAMPLE                           1
    #define   MSK32TSEN_CTRL_VSAMPLE                              0x00000010

    #define     BA_TSEN_CTRL_TRIMG                             0x0000
    #define     B16TSEN_CTRL_TRIMG                             0x0000
    #define   LSb32TSEN_CTRL_TRIMG                                5
    #define   LSb16TSEN_CTRL_TRIMG                                5
    #define       bTSEN_CTRL_TRIMG                             5
    #define   MSK32TSEN_CTRL_TRIMG                                0x000003E0

    #define     BA_TSEN_CTRL_TRIMO                             0x0001
    #define     B16TSEN_CTRL_TRIMO                             0x0000
    #define   LSb32TSEN_CTRL_TRIMO                                10
    #define   LSb16TSEN_CTRL_TRIMO                                10
    #define       bTSEN_CTRL_TRIMO                             6
    #define   MSK32TSEN_CTRL_TRIMO                                0x0000FC00

    #define     BA_TSEN_CTRL_TSEN_DAT_LT                       0x0002
    #define     B16TSEN_CTRL_TSEN_DAT_LT                       0x0002
    #define   LSb32TSEN_CTRL_TSEN_DAT_LT                          16
    #define   LSb16TSEN_CTRL_TSEN_DAT_LT                          0
    #define       bTSEN_CTRL_TSEN_DAT_LT                       5
    #define   MSK32TSEN_CTRL_TSEN_DAT_LT                          0x001F0000
    ///////////////////////////////////////////////////////////
    #define     RA_TSEN_STATUS                                 0x0004

    #define     BA_TSEN_STATUS_DATA_RDY                        0x0004
    #define     B16TSEN_STATUS_DATA_RDY                        0x0004
    #define   LSb32TSEN_STATUS_DATA_RDY                           0
    #define   LSb16TSEN_STATUS_DATA_RDY                           0
    #define       bTSEN_STATUS_DATA_RDY                        1
    #define   MSK32TSEN_STATUS_DATA_RDY                           0x00000001

    #define     BA_TSEN_STATUS_INT_EN                          0x0004
    #define     B16TSEN_STATUS_INT_EN                          0x0004
    #define   LSb32TSEN_STATUS_INT_EN                             1
    #define   LSb16TSEN_STATUS_INT_EN                             1
    #define       bTSEN_STATUS_INT_EN                          1
    #define   MSK32TSEN_STATUS_INT_EN                             0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_TSEN_DATA                                   0x0008

    #define     BA_TSEN_DATA_DATA                              0x0008
    #define     B16TSEN_DATA_DATA                              0x0008
    #define   LSb32TSEN_DATA_DATA                                 0
    #define   LSb16TSEN_DATA_DATA                                 0
    #define       bTSEN_DATA_DATA                              10
    #define   MSK32TSEN_DATA_DATA                                 0x000003FF
    ///////////////////////////////////////////////////////////
    #define     RA_TSEN_TSEN_CHK_CTRL                          0x000C

    #define     BA_TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX            0x000C
    #define     B16TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX            0x000C
    #define   LSb32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX               0
    #define   LSb16TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX               0
    #define       bTSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX            10
    #define   MSK32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX               0x000003FF

    #define     BA_TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN            0x000D
    #define     B16TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN            0x000C
    #define   LSb32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN               10
    #define   LSb16TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN               10
    #define       bTSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN            10
    #define   MSK32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN               0x000FFC00

    #define     BA_TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL        0x000E
    #define     B16TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL        0x000E
    #define   LSb32TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL           20
    #define   LSb16TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL           4
    #define       bTSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL        1
    #define   MSK32TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL           0x00100000
    ///////////////////////////////////////////////////////////
    #define     RA_TSEN_TSEN_DATA_STATUS                       0x0010

    #define     BA_TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL         0x0010
    #define     B16TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL         0x0010
    #define   LSb32TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL            0
    #define   LSb16TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL            0
    #define       bTSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL         1
    #define   MSK32TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL            0x00000001

    #define     BA_TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL         0x0010
    #define     B16TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL         0x0010
    #define   LSb32TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL            1
    #define   LSb16TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL            1
    #define       bTSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL         1
    #define   MSK32TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL            0x00000002
    ///////////////////////////////////////////////////////////

    typedef struct SIE_TSEN {
    ///////////////////////////////////////////////////////////
    #define   GET32TSEN_CTRL_ENA(r32)                          _BFGET_(r32, 0, 0)
    #define   SET32TSEN_CTRL_ENA(r32,v)                        _BFSET_(r32, 0, 0,v)
    #define   GET16TSEN_CTRL_ENA(r16)                          _BFGET_(r16, 0, 0)
    #define   SET16TSEN_CTRL_ENA(r16,v)                        _BFSET_(r16, 0, 0,v)

    #define   GET32TSEN_CTRL_CLK_EN(r32)                       _BFGET_(r32, 1, 1)
    #define   SET32TSEN_CTRL_CLK_EN(r32,v)                     _BFSET_(r32, 1, 1,v)
    #define   GET16TSEN_CTRL_CLK_EN(r16)                       _BFGET_(r16, 1, 1)
    #define   SET16TSEN_CTRL_CLK_EN(r16,v)                     _BFSET_(r16, 1, 1,v)

    #define   GET32TSEN_CTRL_PSAMPLE(r32)                      _BFGET_(r32, 3, 2)
    #define   SET32TSEN_CTRL_PSAMPLE(r32,v)                    _BFSET_(r32, 3, 2,v)
    #define   GET16TSEN_CTRL_PSAMPLE(r16)                      _BFGET_(r16, 3, 2)
    #define   SET16TSEN_CTRL_PSAMPLE(r16,v)                    _BFSET_(r16, 3, 2,v)

    #define   GET32TSEN_CTRL_VSAMPLE(r32)                      _BFGET_(r32, 4, 4)
    #define   SET32TSEN_CTRL_VSAMPLE(r32,v)                    _BFSET_(r32, 4, 4,v)
    #define   GET16TSEN_CTRL_VSAMPLE(r16)                      _BFGET_(r16, 4, 4)
    #define   SET16TSEN_CTRL_VSAMPLE(r16,v)                    _BFSET_(r16, 4, 4,v)

    #define   GET32TSEN_CTRL_TRIMG(r32)                        _BFGET_(r32, 9, 5)
    #define   SET32TSEN_CTRL_TRIMG(r32,v)                      _BFSET_(r32, 9, 5,v)
    #define   GET16TSEN_CTRL_TRIMG(r16)                        _BFGET_(r16, 9, 5)
    #define   SET16TSEN_CTRL_TRIMG(r16,v)                      _BFSET_(r16, 9, 5,v)

    #define   GET32TSEN_CTRL_TRIMO(r32)                        _BFGET_(r32,15,10)
    #define   SET32TSEN_CTRL_TRIMO(r32,v)                      _BFSET_(r32,15,10,v)
    #define   GET16TSEN_CTRL_TRIMO(r16)                        _BFGET_(r16,15,10)
    #define   SET16TSEN_CTRL_TRIMO(r16,v)                      _BFSET_(r16,15,10,v)

    #define   GET32TSEN_CTRL_TSEN_DAT_LT(r32)                  _BFGET_(r32,20,16)
    #define   SET32TSEN_CTRL_TSEN_DAT_LT(r32,v)                _BFSET_(r32,20,16,v)
    #define   GET16TSEN_CTRL_TSEN_DAT_LT(r16)                  _BFGET_(r16, 4, 0)
    #define   SET16TSEN_CTRL_TSEN_DAT_LT(r16,v)                _BFSET_(r16, 4, 0,v)

    #define     w32TSEN_CTRL                                   {\
            UNSG32 uCTRL_ENA                                   :  1;\
            UNSG32 uCTRL_CLK_EN                                :  1;\
            UNSG32 uCTRL_PSAMPLE                               :  2;\
            UNSG32 uCTRL_VSAMPLE                               :  1;\
            UNSG32 uCTRL_TRIMG                                 :  5;\
            UNSG32 uCTRL_TRIMO                                 :  6;\
            UNSG32 uCTRL_TSEN_DAT_LT                           :  5;\
            UNSG32 RSVDx0_b21                                  : 11;\
          }
    union { UNSG32 u32TSEN_CTRL;
            struct w32TSEN_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSEN_STATUS_DATA_RDY(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32TSEN_STATUS_DATA_RDY(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16TSEN_STATUS_DATA_RDY(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16TSEN_STATUS_DATA_RDY(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32TSEN_STATUS_INT_EN(r32)                     _BFGET_(r32, 1, 1)
    #define   SET32TSEN_STATUS_INT_EN(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   GET16TSEN_STATUS_INT_EN(r16)                     _BFGET_(r16, 1, 1)
    #define   SET16TSEN_STATUS_INT_EN(r16,v)                   _BFSET_(r16, 1, 1,v)

    #define     w32TSEN_STATUS                                 {\
            UNSG32 uSTATUS_DATA_RDY                            :  1;\
            UNSG32 uSTATUS_INT_EN                              :  1;\
            UNSG32 RSVDx4_b2                                   : 30;\
          }
    union { UNSG32 u32TSEN_STATUS;
            struct w32TSEN_STATUS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSEN_DATA_DATA(r32)                         _BFGET_(r32, 9, 0)
    #define   SET32TSEN_DATA_DATA(r32,v)                       _BFSET_(r32, 9, 0,v)
    #define   GET16TSEN_DATA_DATA(r16)                         _BFGET_(r16, 9, 0)
    #define   SET16TSEN_DATA_DATA(r16,v)                       _BFSET_(r16, 9, 0,v)

    #define     w32TSEN_DATA                                   {\
            UNSG32 uDATA_DATA                                  : 10;\
            UNSG32 RSVDx8_b10                                  : 22;\
          }
    union { UNSG32 u32TSEN_DATA;
            struct w32TSEN_DATA;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX(r32)       _BFGET_(r32, 9, 0)
    #define   SET32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX(r32,v)     _BFSET_(r32, 9, 0,v)
    #define   GET16TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX(r16)       _BFGET_(r16, 9, 0)
    #define   SET16TSEN_TSEN_CHK_CTRL_TSEN_DATA_MAX(r16,v)     _BFSET_(r16, 9, 0,v)

    #define   GET32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN(r32)       _BFGET_(r32,19,10)
    #define   SET32TSEN_TSEN_CHK_CTRL_TSEN_DATA_MIN(r32,v)     _BFSET_(r32,19,10,v)

    #define   GET32TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL(r32)   _BFGET_(r32,20,20)
    #define   SET32TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL(r16)   _BFGET_(r16, 4, 4)
    #define   SET16TSEN_TSEN_CHK_CTRL_TSEN_OVERHEAT_SEL(r16,v) _BFSET_(r16, 4, 4,v)

    #define     w32TSEN_TSEN_CHK_CTRL                          {\
            UNSG32 uTSEN_CHK_CTRL_TSEN_DATA_MAX                : 10;\
            UNSG32 uTSEN_CHK_CTRL_TSEN_DATA_MIN                : 10;\
            UNSG32 uTSEN_CHK_CTRL_TSEN_OVERHEAT_SEL            :  1;\
            UNSG32 RSVDxC_b21                                  : 11;\
          }
    union { UNSG32 u32TSEN_TSEN_CHK_CTRL;
            struct w32TSEN_TSEN_CHK_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL(r32)    _BFGET_(r32, 0, 0)
    #define   SET32TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL(r16)    _BFGET_(r16, 0, 0)
    #define   SET16TSEN_TSEN_DATA_STATUS_TSEN_MAX_FAIL(r16,v)  _BFSET_(r16, 0, 0,v)

    #define   GET32TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL(r32)    _BFGET_(r32, 1, 1)
    #define   SET32TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   GET16TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL(r16)    _BFGET_(r16, 1, 1)
    #define   SET16TSEN_TSEN_DATA_STATUS_TSEN_MIN_FAIL(r16,v)  _BFSET_(r16, 1, 1,v)

    #define     w32TSEN_TSEN_DATA_STATUS                       {\
            UNSG32 uTSEN_DATA_STATUS_TSEN_MAX_FAIL             :  1;\
            UNSG32 uTSEN_DATA_STATUS_TSEN_MIN_FAIL             :  1;\
            UNSG32 RSVDx10_b2                                  : 30;\
          }
    union { UNSG32 u32TSEN_TSEN_DATA_STATUS;
            struct w32TSEN_TSEN_DATA_STATUS;
          };
    ///////////////////////////////////////////////////////////
    } SIE_TSEN;

    typedef union  T32TSEN_CTRL
          { UNSG32 u32;
            struct w32TSEN_CTRL;
                 } T32TSEN_CTRL;
    typedef union  T32TSEN_STATUS
          { UNSG32 u32;
            struct w32TSEN_STATUS;
                 } T32TSEN_STATUS;
    typedef union  T32TSEN_DATA
          { UNSG32 u32;
            struct w32TSEN_DATA;
                 } T32TSEN_DATA;
    typedef union  T32TSEN_TSEN_CHK_CTRL
          { UNSG32 u32;
            struct w32TSEN_TSEN_CHK_CTRL;
                 } T32TSEN_TSEN_CHK_CTRL;
    typedef union  T32TSEN_TSEN_DATA_STATUS
          { UNSG32 u32;
            struct w32TSEN_TSEN_DATA_STATUS;
                 } T32TSEN_TSEN_DATA_STATUS;
    ///////////////////////////////////////////////////////////

    typedef union  TTSEN_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32TSEN_CTRL;
                   };
                 } TTSEN_CTRL;
    typedef union  TTSEN_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32TSEN_STATUS;
                   };
                 } TTSEN_STATUS;
    typedef union  TTSEN_DATA
          { UNSG32 u32[1];
            struct {
            struct w32TSEN_DATA;
                   };
                 } TTSEN_DATA;
    typedef union  TTSEN_TSEN_CHK_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32TSEN_TSEN_CHK_CTRL;
                   };
                 } TTSEN_TSEN_CHK_CTRL;
    typedef union  TTSEN_TSEN_DATA_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32TSEN_TSEN_DATA_STATUS;
                   };
                 } TTSEN_TSEN_DATA_STATUS;

    ///////////////////////////////////////////////////////////
     SIGN32 TSEN_drvrd(SIE_TSEN *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TSEN_drvwr(SIE_TSEN *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TSEN_reset(SIE_TSEN *p);
     SIGN32 TSEN_cmp  (SIE_TSEN *p, SIE_TSEN *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TSEN_check(p,pie,pfx,hLOG) TSEN_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TSEN_print(p,    pfx,hLOG) TSEN_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif
//////
/// ENDOFINTERFACE: TSEN
////////////////////////////////////////////////////////////



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif
//////
/// ENDOFFILE: tsen.h
////////////////////////////////////////////////////////////
