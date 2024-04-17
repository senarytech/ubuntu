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

/*******************************************************************************
\file   gfx_api.h
\brief Graphics APIs Definitions.
Purpose:
    Define graphics APIs and related data structure.
Version, Date   and Author :
    V   1.00,        Dec 21 2012        Lili/Yifan
                                                            Initial Version.
Note:
*******************************************************************************/
#if !defined(_GFX_API_H_)
#define _GFX_API_H_

#if defined(__cplusplus)
extern "C" {
#endif

/************* Header file include ********************************************/
#include "ErrorCode.h"
#include "com_type.h"
#include "gfx_type.h"
#include "amp_client_support.h"

/************* Macros, type definitions****************************************/
#define GFX_IN
#define GFX_OUT
#define GFX_INOUT

typedef HANDLE MV_GFX_2D_HANDLE;                /** Generic Gfx handle */
typedef MV_GFX_2D_HANDLE MV_GFX_2D_HGFX;        /** Handle to a Gfx session */
typedef MV_GFX_2D_HANDLE MV_GFX_2D_HCEL;        /** Handle to a Gfx cel */
typedef MV_GFX_2D_HANDLE MV_GFX_2D_HPALETTE;    /** Handle to a Gfx palette */
typedef MV_GFX_2D_HANDLE MV_GFX_2D_HBRUSH;      /** Handle to a Gfx brush */

/************* Declaration of global functions ********************************/
/******************************************************************************
\fn HRESULT MV_GFX_2D_Init_CelInfo(GFX_INOUT MV_GFX_2D_CelInfo *pCelInfo)
\brief Initialize the information of cell.

\param pCelInfo(in) : pointer of a MV_GFX_2D_CelInfo

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_Init_CelInfo(GFX_INOUT MV_GFX_2D_CelInfo *pCelInfo);

/******************************************************************************
\fn HRESULT MV_GFX_2D_Open(GFX_IN const UINT8 *szAppName,
                       GFX_OUT MV_GFX_2D_HGFX *phGfx)
\brief Open a session with graphics API.

\param szAppName(in) : name of the gfx app
\param phGfx    (out): Handle for the gfx created.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_Open(GFX_IN const UINT8 *szAppName,
                       GFX_OUT MV_GFX_2D_HGFX *phGfx);


/******************************************************************************
\fn HRESULT MV_GFX_2D_Close(GFX_IN MV_GFX_2D_HGFX hGfx);
\brief Close a session.

\param phGfx    (in): Handle of the gfx need to be closed

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_Close(GFX_IN MV_GFX_2D_HGFX hGfx);

/******************************************************************************
\fn HRESULT MV_GFX_2D_CreateCel
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_CelInfo *pCelInfo,
        GFX_OUT MV_GFX_2D_HCEL *phCel
    )
\brief Create a cell.

\param hGfx (in): handle of the gfx instance
\param pCelInfo (in): handle of the cell info
\param phCel (out): Handle for the cell created.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_CreateCel
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_CelInfo *pCelInfo,
    GFX_OUT MV_GFX_2D_HCEL *phCel
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_SetCelInfo
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel,
        GFX_IN  MV_GFX_2D_CelInfo *pCelInfo
    )
\brief Set the information of a cell.

\param hGfx (in): handle of the gfx instance
\param hCel (in): Handle of the cell
\param pCelInfo (in): handle of the cell info

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_SetCelInfo
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel,
    GFX_IN  MV_GFX_2D_CelInfo *pCelInfo
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_SetCelCallBack
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel,
        GFX_IN  MV_GFX_2D_CelCallBack *pCallBack
    )
\brief Set cell destruction callback.

\param hGfx (in): handle of the gfx instance
\param hCel (in): handle of the cell
\param pCallBack (in): destruction callback

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_SetCelCallBack
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel,
    GFX_IN  MV_GFX_2D_CelCallBack *pCallBack
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_GetCelInfo
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel1,
        GFX_OUT MV_GFX_2D_CelInfo *pCelInfo
    )
\brief Get the information of a cell.

\param hGfx (in): handle of the gfx instance
\param phCel (in): Handle of the cell
\param pCelInfo (in): handle of the cell info

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_GetCelInfo
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel1,
    GFX_OUT MV_GFX_2D_CelInfo *pCelInfo
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_CreateSolidBrush
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_COLOR eColor,
        GFX_OUT MV_GFX_2D_HBRUSH *phBrush
    )
\brief Create a solid brush.

\param hGfx (in): handle of the gfx instance
\param eColor (in): color for the brush
\param phBrush (out): Handle for the brush created.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_CreateSolidBrush
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_COLOR eColor,
    GFX_OUT MV_GFX_2D_HBRUSH *phBrush
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_CreateMonochromeBrush
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_COLOR FgColor,
        GFX_IN  MV_GFX_2D_COLOR BgColor,
        GFX_IN  UINT64 uBits,
        GFX_OUT MV_GFX_2D_HBRUSH *phBrush
    )
\brief Create a monochrome brush.

\param hGfx (in): handle of the gfx instance
\param FgColor(in): Foreground colors of the brush.
\param BgColor (in): Background colors of the brush.
\param Bits(in): 64 bits of pixel bits. Each bit represents one pixel and is
                 used to choose between foreground and background colors. The
                 64 bits construct a 8x8 array.
\param phBrush (out): Handle for the brush created.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_CreateMonochromeBrush
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_COLOR FgColor,
    GFX_IN  MV_GFX_2D_COLOR BgColor,
    GFX_IN  UINT64 uBits,
    GFX_OUT MV_GFX_2D_HBRUSH *phBrush
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_CreateColorBrush
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  void *pBmpAddr,
        GFX_IN  MV_GFX_2D_PIXEL_FORMAT eSurfFormat,
        GFX_OUT MV_GFX_2D_HBRUSH *phBrush
    )
\brief Create a color brush.

\param hGfx (in): handle of the gfx instance
\param pBmpAddr (in): address of bmp picture
\param eSurfFormat (in): surface format of bmp picture
\param phBrush (out): Handle for the brush created.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_CreateColorBrush
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  void *pBmpAddr,
    GFX_IN  MV_GFX_2D_PIXEL_FORMAT eSurfFormat,
    GFX_OUT MV_GFX_2D_HBRUSH *phBrush
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_CreatePalette
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_COLOR *pColor,
        GFX_OUT MV_GFX_2D_HPALETTE *phPalette
    )
\brief Create a palette.

\param hGfx (in): handle of the gfx instance
\param pColor (in): color table for the palette
\param phPalette (out): Handle for the palette created.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_CreatePalette
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_COLOR *pColor,
    GFX_OUT MV_GFX_2D_HPALETTE *phPalette
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_SetPaletteEntries
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HPALETTE pPalette,
        GFX_IN  UINT16 byStartIdx,
        GFX_IN  UINT16 byNum,
        GFX_IN  MV_GFX_2D_COLOR *pEntries
    )
\brief Update some entries of color table of the palette object.

\param hGfx (in): handle of the gfx instance
\param pPalette (in): Handle of the palette.
\param byStartIdx (in): Start index of the color table need to be updated
\param byNum (in): Account of the entries need to be updated
\param pEntries (in): Entries need to be set to the color table

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_SetPaletteEntries
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HPALETTE pPalette,
    GFX_IN  UINT16 byStartIdx,
    GFX_IN  UINT16 byNum,
    GFX_IN  MV_GFX_2D_COLOR *pEntries
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_LoadPalette
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL pCel,
        GFX_IN  MV_GFX_2D_HPALETTE pPalette
    )
\brief Set palette to a cell to use.

\param hGfx (in): handle of the gfx instance
\param pCel (in): Handle to the cel, the pixel type of the cel has to be
                   MV_PE_GFX_PIX_I8XRGB, MV_PE_GFX_PIX_I8XYUV, or MV_PE_GFX_PIX_A8.
\param pPalette (in): handle to the palette

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_LoadPalette
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL pCel,
    GFX_IN  MV_GFX_2D_HPALETTE pPalette
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_DestroyObject
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HANDLE handle
    )
\brief Destroy a graphics object.

\param hGfx (in): handle of the gfx instance
\param handle (in): Handle of the object need to be destroyed

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_DestroyObject
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HANDLE handle
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_GetDisp(GFX_IN MV_GFX_2D_HGFX hGfx,
                              GFX_OUT AMP_DISP *pDisp);

\brief get amp gfx display handle.

\param hGfx (in): handle of the gfx instance
\param pDisp (out): Handle for the gfx display handle.

\return 0 if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_GetDisp(GFX_IN MV_GFX_2D_HGFX hGfx,
                          GFX_OUT AMP_DISP *pDisp);

/******************************************************************************
\fn HRESULT MV_GFX_2D_DisplayCel(GFX_IN MV_GFX_2D_HGFX hGfx,
                                 GFX_IN MV_GFX_2D_HCEL hCel,
                                 GFX_IN MV_GFX_2D_RECT *pSrcWin,
                                 GFX_IN MV_GFX_2D_RECT *pDesWin,
                                 GFX_IN MV_GFX_2D_CHANNEL eChannel)
\brief Display a cell.

\param hGfx (in): handle of the gfx instance
\param hCel (in): handle of the cell which stored the frame to be displayed
\param pSrcWin (in): the rectangle of the image need to be displayed
\param pDesWin (in): the position and size of the screen where the image
                      will be displayed to
\param eChannel (in): the channel to display to

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_DisplayCel(GFX_IN MV_GFX_2D_HGFX hGfx,
                             GFX_IN MV_GFX_2D_HCEL hCel,
                             GFX_IN MV_GFX_2D_RECT *pSrcWin,
                             GFX_IN MV_GFX_2D_RECT *pDesWin,
                             GFX_IN MV_GFX_2D_CHANNEL eChannel);

/******************************************************************************
\fn HRESULT MV_GFX_2D_Display3DCel(GFX_IN MV_GFX_2D_HGFX hGfx,
                                   GFX_IN MV_GFX_2D_HCEL hLeftCel,
                                   GFX_IN MV_GFX_2D_HCEL hRightCel,
                                   GFX_IN MV_GFX_2D_RECT *pSrcWin,
                                   GFX_IN MV_GFX_2D_RECT *pDesWin,
                                   GFX_IN MV_GFX_2D_CHANNEL uiPlaneId)

\brief Display a cell in 3D.

\param hGfx (in): handle of the gfx instance
\param hLeftCel (in): handle of the left cell to be displayed
\param hRightCel (in): handle of the right cell to be displayed
\param pSrcWin (in): the rectangle of the image need to be displayed
\param pDesWin (in): the position and size of the screen where the image
                      will be displayed to
\param uiPlaneId (in): the plane id to display to

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_Display3DCel(GFX_IN MV_GFX_2D_HGFX hGfx,
                               GFX_IN MV_GFX_2D_HCEL hLeftCel,
                               GFX_IN MV_GFX_2D_HCEL hRightCel,
                               GFX_IN MV_GFX_2D_RECT *pSrcWin,
                               GFX_IN MV_GFX_2D_RECT *pDesWin,
                               GFX_IN MV_GFX_2D_CHANNEL uiPlaneId);

/******************************************************************************
\fn HRESULT MV_GFX_2D_WaitCelFree(GFX_IN MV_GFX_2D_HGFX hGfx,
                              GFX_IN MV_GFX_2D_HCEL hCel,
                              GFX_IN_ MV_GFX_2D_CHANNEL channel);
\brief Wait a cell to be free.

\param hCel     (in): Handle to the cel to be waited.
\param channel  (in): Graphics plane
\return 0 if successful

\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_WaitCelFree(GFX_IN MV_GFX_2D_HGFX hGfx,
                              GFX_IN MV_GFX_2D_HCEL hCel,
                              GFX_IN MV_GFX_2D_CHANNEL channel);


/******************************************************************************
\fn HRESULT MV_GFX_2D_WaitVSync(GFX_IN MV_GFX_2D_HGFX hGfx);
\brief Wait for a vpp isr.

\param hGfx     (in): handle of the gfx instance
\param channel  (in): Graphics plane
\return 0 if successful

\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_WaitVSync(GFX_IN MV_GFX_2D_HGFX hGfx,
                            GFX_IN MV_GFX_2D_CHANNEL channel);

/******************************************************************************
\fn HRESULT MV_GFX_2D_ClearCel
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel1,
        GFX_IN  const MV_GFX_2D_RECT *pDstRect,
        GFX_IN  const MV_GFX_2D_RECT *pClipRect
    )
\brief Disable alpha blend.

\param hGfx (in): handle of the gfx instance
\param hCel1 (in): Handle to the cell
\param pDstRect (in): Rectangle in the cell, NULL means the whole cell
\param pClipRect (in): Clipping area in destination, NULL means none

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_ClearCel
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel1,
    GFX_IN  const MV_GFX_2D_RECT *pDstRect,
    GFX_IN  const MV_GFX_2D_RECT *pClipRect
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_FillCel
    (
        MV_GFX_2D_HGFX hGfx,
        MV_GFX_2D_HCEL hCel1,
        MV_GFX_2D_COLOR color,
        const MV_GFX_2D_RECT *pDstRect,
        const MV_GFX_2D_RECT *pClipRect
    )
\brief Fill a cell region with specified color.

\param hGfx (in): handle of the gfx instance
\param hCel1      (in): Handle to the cel
\param color (in): color used to fill
\param pDstRect (in): Rectangle in the cel, NULL means the whole cel
\param pClipRect (in): Clipping area in destination, NULL means none

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_FillCel
(
    MV_GFX_2D_HGFX hGfx,
    MV_GFX_2D_HCEL hCel1,
    MV_GFX_2D_COLOR color,
    const MV_GFX_2D_RECT *pDstRect,
    const MV_GFX_2D_RECT *pClipRect
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_AlphaBlend
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hSrc,
        GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
        GFX_IN  MV_GFX_2D_HCEL hDst,
        GFX_IN  const MV_GFX_2D_RECT *pDstRect,
        GFX_IN  const MV_GFX_2D_RECT *pClipRect,
        GFX_IN  MV_GFX_2D_BLEND_MODE eBlendSpec
    )
\brief Perform alphablend.

\param hGfx (in): handle of the gfx instance
\param hSrc      (in): handle to the source cell
\param pSrcRect  (in): rectangle in source cell, NULL means whole cell
\param hDst      (in): handle to the destination cell, NULL means parent cell
\param pDstRect (in): rectangle in destination cell, NULL means whole cell
\param pClipRect (in): clipping area in destination, NULL means none
\param ulBlendSpec   (in): alpha-blending spec, NULL means SRC_OVER

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_AlphaBlend
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hSrc,
    GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
    GFX_IN  MV_GFX_2D_HCEL hDst,
    GFX_IN  const MV_GFX_2D_RECT *pDstRect,
    GFX_IN  const MV_GFX_2D_RECT *pClipRect,
    GFX_IN  MV_GFX_2D_BLEND_MODE eBlendSpec
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_Blit
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hSrc,
        GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
        GFX_IN  MV_GFX_2D_HCEL hDst,
        GFX_IN  const MV_GFX_2D_POINT *pDstPoint,
        GFX_IN  const MV_GFX_2D_RECT *pClipRect,
        GFX_IN  MV_GFX_2D_ROP rop
    )
\brief Blit a region from source cell to destination cell.

\param hGfx (in): handle of the gfx instance
\param hSrc      (in): handle to the source cell
\param pSrcRect  (in): rectangle in source cell, NULL means whole cell
\param hDst      (in): handle to the destination cell, NULL means parent cell
\param pDstPoint (in): upper-left corner in destination cell
\param pClipRect (in): clipping area in destination, NULL means none
\param rop       (in): raster operation code

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_Blit
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hSrc,
    GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
    GFX_IN  MV_GFX_2D_HCEL hDst,
    GFX_IN  const MV_GFX_2D_POINT *pDstPoint,
    GFX_IN  const MV_GFX_2D_RECT *pClipRect,
    GFX_IN  MV_GFX_2D_ROP rop
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_StretchBlit
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hSrc,
        GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
        GFX_IN  MV_GFX_2D_HCEL hDst,
        GFX_IN  const MV_GFX_2D_RECT *pDstRect,
        GFX_IN  const MV_GFX_2D_RECT *pClipRect,
        GFX_IN  MV_GFX_2D_ROP rop
    )
\brief Stretch blit a region from source cell to destination cell.

\param hGfx (in): handle of the gfx instance
\param hSrc      (in): handle to the source cell
\param pSrcRect  (in): rectangle in source cell, NULL means whole cell
\param hDst      (in): handle to the destination cell, NULL means parent cell
\param pDstRect  (in): rectangle in dest cell, NULL means whole cell
\param pClipRect (in): clipping area in destination, NULL means none
\param rop       (in): raster operation code

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_StretchBlit
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hSrc,
    GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
    GFX_IN  MV_GFX_2D_HCEL hDst,
    GFX_IN  const MV_GFX_2D_RECT *pDstRect,
    GFX_IN  const MV_GFX_2D_RECT *pClipRect,
    GFX_IN  MV_GFX_2D_ROP rop
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_BatchBlit
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hSrc,
        GFX_IN  MV_GFX_2D_HCEL hDst,
        GFX_IN  UINT32 uiRectCount,
        GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
        GFX_IN  const MV_GFX_2D_RECT *pDstRect,
        GFX_IN  const MV_GFX_2D_RECT *pClipRect,
        GFX_IN  MV_GFX_2D_ROP rop
    )
\brief Batch blit several regions from source cell to destination cell.

\param hGfx (in): handle of the gfx instance
\param hSrc      (in): handle to the source cell
\param hDst      (in): handle to the destination cell, NULL means parent cell
\param uiRectCount    (in): account of rectangles
\param pSrcRect  (in): rectangles in source cell
\param pDstRect  (in): rectangles in destination cell
\param pClipRect (in): clipping area in destination, NULL means none
\param rop       (in): raster operation code

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_BatchBlit
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hSrc,
    GFX_IN  MV_GFX_2D_HCEL hDst,
    GFX_IN  UINT32 uiRectCount,
    GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
    GFX_IN  const MV_GFX_2D_RECT *pDstRect,
    GFX_IN  const MV_GFX_2D_RECT *pClipRect,
    GFX_IN  MV_GFX_2D_ROP rop
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_FilterBlit
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hSrc,
        GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
        GFX_IN  MV_GFX_2D_HCEL hDst,
        GFX_IN  const MV_GFX_2D_RECT *pDstRect,
        GFX_IN  const MV_GFX_2D_RECT *pClipRect
    )
\brief Filter blit a region from source cell to destination cell.

\param hGfx (in): handle of the gfx instance
\param hSrc      (in): handle to the source cell
\param pSrcRect  (in): rectangles in source cell
\param hDst      (in): handle to the destination cell, NULL means parent cell
\param pDstRect  (in): rectangles in destination cell
\param pClipRect (in): clipping area in destination, NULL means none

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_FilterBlit
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hSrc,
    GFX_IN  const MV_GFX_2D_RECT *pSrcRect,
    GFX_IN  MV_GFX_2D_HCEL hDst,
    GFX_IN  const MV_GFX_2D_RECT *pDstRect,
    GFX_IN  const MV_GFX_2D_RECT *pClipRect
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_DrawLines
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel1,
        GFX_IN  INT32 nLines,
        GFX_IN  const MV_GFX_2D_RECT *pPosRects,
        GFX_IN  MV_GFX_2D_HBRUSH hBrush,
        GFX_IN  MV_GFX_2D_RECT *pClipRect,
        GFX_IN  MV_GFX_2D_ROP rop
    )
\brief Draw lines with given brush.

\param hGfx (in): handle of the gfx instance
\param hCel1      (in): handle to the cel to be drawn
\param nLines    (in): number of lines
\param pPositions   (in): pointer to an array of positions
\param hBrush        (in): brush to be used
\param pClipRect (in): clipping area in destination, NULL means none
\param rop       (in): raster operation code

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_DrawLines
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel1,
    GFX_IN  INT32 nLines,
    GFX_IN  const MV_GFX_2D_RECT *pPosRects,
    GFX_IN  MV_GFX_2D_HBRUSH hBrush,
    GFX_IN  MV_GFX_2D_RECT *pClipRect,
    GFX_IN  MV_GFX_2D_ROP rop
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_DrawColorLines
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel1,
        GFX_IN  INT32 nLines,
        GFX_IN  const MV_GFX_2D_RECT*pPosRects,
        GFX_IN  MV_GFX_2D_COLOR color,
        GFX_IN  MV_GFX_2D_RECT *pClipRect,
        GFX_IN  MV_GFX_2D_ROP rop
    )
\brief Draw lines with specified color.

\param hGfx (in): handle of the gfx instance
\param hCel1      (in): handle to the cel to be drawn
\param nLines    (in): number of lines
\param pPositions   (in): pointer to an array of positions
\param color     (in): color to be used
\param pClipRect (in): clipping area in destination, NULL means none
\param rop       (in): raster operation code

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_DrawColorLines
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel1,
    GFX_IN  INT32 nLines,
    GFX_IN  const MV_GFX_2D_RECT*pPosRects,
    GFX_IN  MV_GFX_2D_COLOR color,
    GFX_IN  MV_GFX_2D_RECT *pClipRect,
    GFX_IN  MV_GFX_2D_ROP rop
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_Sync(MV_GFX_2D_HGFX hGfx, INT32 fSync)
\brief Synchronize graphics driver.

\param hGfx      (in): handle to the opened graphics instance.
\param fSync  (in): Flag for function blocking option.

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_Sync(MV_GFX_2D_HGFX hGfx, INT32 fSync);

/******************************************************************************
\fn HRESULT MV_GFX_2D_SetBlendMode
    (
        GFX_IN  MV_GFX_2D_BLEND_MODE eBlendSpec
    )
\brief Set blend mode.

\param eBlendSpec (in) specified alpha blend mode

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_SetBlendMode
(
    GFX_IN  MV_GFX_2D_BLEND_MODE eBlendSpec
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_DEBUG_DumpCell
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel1
    )
\brief Dump buffer of a cell.

\param hGfx      (in): handle to the opened graphics instance.
\param hCel1  (in): handle to the cell to be dumped
\param pFilePath (in): path of dumped file

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_DEBUG_DumpCell
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel1,
    const CHAR *pFilePath
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_LoadImage
    (
        GFX_IN  MV_GFX_2D_HGFX hGfx,
        GFX_IN  MV_GFX_2D_HCEL hCel1,
        GFX_IN  const CHAR *sPath,
        GFX_IN  UINT16 unWidth,
        GFX_IN  UINT16 unHeight,
        GFX_IN  MV_GFX_2D_PIXEL_FORMAT ePixFormat
    )
\brief Load raw image.

\param hGfx         (in): handle to the opened graphics instance.
\param hCel1        (in): handle to the cell to be dumped
\param sPath        (in): path of raw YUV image file
\param unWidth      (in): width of YUV image
\param unHeight     (in): height of YUV image
\param ePixelFormat (in): pixel format of YUV image

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_LoadImage
(
    GFX_IN  MV_GFX_2D_HGFX hGfx,
    GFX_IN  MV_GFX_2D_HCEL hCel1,
    GFX_IN  const CHAR *sPath,
    GFX_IN  UINT16 unWidth,
    GFX_IN  UINT16 unHeight,
    GFX_IN  MV_GFX_2D_PIXEL_FORMAT ePixFormat
);

/******************************************************************************
\fn HRESULT MV_GFX_2D_CompareCel
    (
        GFX_IN      MV_GFX_2D_HGFX hGfx,
        GFX_IN      MV_GFX_2D_HCEL hCelA,
        GFX_IN      MV_GFX_2D_HCEL hCelB,
        GFX_IN      BOOLEAN        bCheckAlign,
        GFX_OUT     INT32          *iResult
    )
\brief Compare the content of two cells.
\param hGfx         (in): handle to the opened graphics instance.
\param hCelA        (in): handle to the first cell
\param hCelB        (in): handle to the second cell
\param bCheckAlign  (in): Whether to compare strides and sizes
                          of the two cells
\param uiTolerance  (in): the differnece less than tolerance is ignored
\param iResult      (out): pointer of comparison result

\return S_OK if successful
\since 1.0
******************************************************************************/
HRESULT MV_GFX_2D_CompareCel
(
    GFX_IN      MV_GFX_2D_HGFX hGfx,
    GFX_IN      MV_GFX_2D_HCEL hCelA,
    GFX_IN      MV_GFX_2D_HCEL hCelB,
    GFX_IN      BOOLEAN        bCheckAlign,
    GFX_IN      UINT32         uiTolerance,
    GFX_OUT     INT32          *iResult
);

HRESULT MV_GFX_2D_GetRasterSettings
(
    GFX_IN      MV_GFX_2D_HGFX hGfx,
    GFX_OUT     MV_GFX_2D_RasterSettings *pSettings
);

HRESULT MV_GFX_2D_SetRasterSettings
(
    GFX_IN      MV_GFX_2D_HGFX hGfx,
    GFX_IN      MV_GFX_2D_RasterSettings *pSettings
);

HRESULT MV_GFX_PM_SetClock
(
    GFX_IN      MV_GFX_PM_ClockSettings *pSettings
);

HRESULT MV_GFX_PM_GetClock
(
    GFX_OUT     MV_GFX_PM_ClockSettings *pSettings
);

BOOL MV_GFX_2D_CheckCelFree(GFX_IN MV_GFX_2D_HGFX hGfx,
                              GFX_IN MV_GFX_2D_HCEL hCel);

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif // define _GFX_AGENT_H_
