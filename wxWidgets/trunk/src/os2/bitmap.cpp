/////////////////////////////////////////////////////////////////////////////
// Name:        bitmap.cpp
// Purpose:     wxBitmap
// Author:      David Webster
// Modified by:
// Created:     08/08/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "bitmap.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include <stdio.h>

    #include "wx/list.h"
    #include "wx/utils.h"
    #include "wx/app.h"
    #include "wx/palette.h"
    #include "wx/dcmemory.h"
    #include "wx/bitmap.h"
    #include "wx/icon.h"
#endif

#include "wx/os2/private.h"
#include "wx/log.h"

//#include "wx/msw/dib.h"
#include "wx/image.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxBitmap, wxGDIObject)
IMPLEMENT_DYNAMIC_CLASS(wxMask, wxObject)

IMPLEMENT_DYNAMIC_CLASS(wxBitmapHandler, wxObject)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxBitmapRefData
// ----------------------------------------------------------------------------

wxBitmapRefData::wxBitmapRefData()
{
    m_nQuality      = 0;
    m_pSelectedInto = NULL;
    m_nNumColors    = 0;
    m_pBitmapMask   = NULL;
}

void wxBitmapRefData::Free()
{
    wxASSERT_MSG( !m_pSelectedInto,
                  wxT("deleting bitmap still selected into wxMemoryDC") );

    if (m_hBitmap)
    {
        if ( !::GpiDeleteBitmap((HBITMAP)m_hBitmap) )
        {
            wxLogLastError("GpiDeleteBitmap(hbitmap)");
        }
    }

    delete m_pBitmapMask;
    m_pBitmapMask = NULL;
}

// ----------------------------------------------------------------------------
// wxBitmap creation
// ----------------------------------------------------------------------------

// this function should be called from all wxBitmap ctors
void wxBitmap::Init()
{
    // m_refData = NULL; done in the base class ctor

    if (wxTheBitmapList)
        wxTheBitmapList->AddBitmap(this);
}

bool wxBitmap::CopyFromIconOrCursor(
  const wxGDIImage&                 rIcon
)
{
    wxBitmapRefData*                pRefData = new wxBitmapRefData;

    m_refData = pRefData;

    pRefData->m_nWidth = rIcon.GetWidth();
    pRefData->m_nHeight = rIcon.GetHeight();
    pRefData->m_nDepth = wxDisplayDepth();

    pRefData->m_hBitmap = (WXHBITMAP)rIcon.GetHandle();
    // no mask???
    pRefData->m_pBitmapMask = new wxMask();

#if WXWIN_COMPATIBILITY_2
    pRefData->m_bOk = TRUE;
#endif // WXWIN_COMPATIBILITY_2
    return(TRUE);
}

bool wxBitmap::CopyFromCursor(
  const wxCursor&                   rCursor
)
{
    UnRef();

    if (!rCursor.Ok())
        return(FALSE);
    return(CopyFromIconOrCursor(rCursor));
}

bool wxBitmap::CopyFromIcon(
  const wxIcon&                     rIcon
)
{
    UnRef();

    if (!rIcon.Ok())
        return(FALSE);

    return CopyFromIconOrCursor(rIcon);
}

wxBitmap::~wxBitmap()
{
    if (wxTheBitmapList)
        wxTheBitmapList->DeleteObject(this);
}

wxBitmap::wxBitmap(
  const char                        zBits[]
, int                               nTheWidth
, int                               nTheHeight
, int                               nNoBits
)
{
    Init();

    wxBitmapRefData*                pRefData = new wxBitmapRefData;
    BITMAPINFOHEADER2               vHeader;
    BITMAPINFO2                     vInfo;
    HDC                             hDc;
    HPS                             hPs;
    DEVOPENSTRUC                    vDop = { NULL, "DISPLAY", NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    SIZEL                           vSize = {0, 0};

    wxASSERT(vHabmain != NULL);

    hDc = ::DevOpenDC(vHabmain, OD_MEMORY, (PSZ)"*", 1L, (PDEVOPENDATA)&vDop, 0L);

    vHeader.cbFix           = sizeof(vHeader);
    vHeader.cx              = (USHORT)nTheWidth;
    vHeader.cy              = (USHORT)nTheHeight;
    vHeader.cPlanes         = 1L;
    vHeader.cBitCount       = nNoBits;
    vHeader.ulCompression   = BCA_UNCOMP;
    vHeader.cxResolution    = 0;
    vHeader.cyResolution    = 0;
    vHeader.cclrUsed        = 0;
    vHeader.cclrImportant   = 0;
    vHeader.usUnits         = BRU_METRIC;
    vHeader.usRecording     = BRA_BOTTOMUP;
    vHeader.usRendering     = BRH_NOTHALFTONED;
    vHeader.cSize1          = 0;
    vHeader.cSize2          = 0;
    vHeader.ulColorEncoding = 0;
    vHeader.ulIdentifier    = 0;

    hPs = ::GpiCreatePS(vHabmain, hDc, &vSize, GPIA_ASSOC | PU_PELS);
    if (hPs == 0)
    {
        wxLogLastError("GpiCreatePS Failure");
    }

    m_refData = pRefData;

    pRefData->m_nWidth = nTheWidth;
    pRefData->m_nHeight = nTheHeight;
    pRefData->m_nDepth = nNoBits;
    pRefData->m_nNumColors = 0;
    pRefData->m_pSelectedInto = NULL;

    HBITMAP hBmp = ::GpiCreateBitmap(hPs, &vHeader, 0L, NULL, &vInfo);
    if (!hBmp)
    {
        wxLogLastError("CreateBitmap");
    }
    SetHBITMAP((WXHBITMAP)hBmp);
}

// Create from XPM data
wxBitmap::wxBitmap(
  char**                            ppData
, wxControl*                        WXUNUSED(pAnItem))
{
    Init();

    (void)Create( (void *)ppData
                 ,wxBITMAP_TYPE_XPM_DATA
                 ,0
                 ,0
                 ,0
                );
}

wxBitmap::wxBitmap(
  int                               nW
, int                               nH
, int                               nD
)
{
    Init();

    (void)Create( nW
                 ,nH
                 ,nD
                );
}

wxBitmap::wxBitmap(
  void*                             pData
, long                              lType
, int                               nWidth
, int                               nHeight
, int                               nDepth
)
{
    Init();

    (void)Create( pData
                 ,lType
                 ,nWidth
                 ,nHeight
                 ,nDepth
                );
}

wxBitmap::wxBitmap(
  const wxString&                   rFilename
, long                              lType
)
{
    Init();

    LoadFile( rFilename
             ,(int)lType
            );
}

bool wxBitmap::Create(
  int                               nW
, int                               nH
, int                               nD
)
{
    HBITMAP                         hBmp;
    BITMAPINFOHEADER2               vHeader;
    BITMAPINFO2                     vInfo;
    HPS                             hpsScreen;
    HDC                             hdcScreen;
    DEVOPENSTRUC                    vDop = { NULL, "DISPLAY", NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    SIZEL                           vSize = {0, 0};
    LONG                            lBitCount;

    wxASSERT(vHabmain != NULL);

    hpsScreen = ::WinGetScreenPS(HWND_DESKTOP);
    hdcScreen = ::GpiQueryDevice(hpsScreen);
    ::DevQueryCaps(hdcScreen, CAPS_COLOR_BITCOUNT, 1L, &lBitCount);

    vHeader.cbFix           = sizeof(vHeader);
    vHeader.cx              = (USHORT)nW;
    vHeader.cy              = (USHORT)nH;
    vHeader.cPlanes         = (USHORT)nD;
    vHeader.cBitCount       = lBitCount;
    vHeader.ulCompression   = BCA_UNCOMP;
    vHeader.cxResolution    = 0;
    vHeader.cyResolution    = 0;
    vHeader.cclrUsed        = 0;
    vHeader.cclrImportant   = 0;
    vHeader.usUnits         = BRU_METRIC;
    vHeader.usRecording     = BRA_BOTTOMUP;
    vHeader.usRendering     = BRH_NOTHALFTONED;
    vHeader.cSize1          = 0;
    vHeader.cSize2          = 0;
    vHeader.ulColorEncoding = 0;
    vHeader.ulIdentifier    = 0;

    UnRef();
    m_refData = new wxBitmapRefData;

    GetBitmapData()->m_nWidth = nW;
    GetBitmapData()->m_nHeight = nH;
    GetBitmapData()->m_nDepth = nD;

    if (nD > 0)
    {
        hBmp = ::GpiCreateBitmap(hpsScreen, &vHeader, 0L, NULL, &vInfo);
        if (!hBmp)
        {
            wxLogLastError("CreateBitmap");
        }
    }
    else
    {
        LONG                            lPlanes;

        ::DevQueryCaps(hdcScreen, CAPS_COLOR_PLANES, 1L, &lPlanes);
        hBmp = ::GpiCreateBitmap(hpsScreen, &vHeader, 0L, NULL, &vInfo);
        if (!hBmp)
        {
            wxLogLastError("CreateBitmap");
        }
        GetBitmapData()->m_nDepth = wxDisplayDepth();
    }
    SetHBITMAP((WXHBITMAP)hBmp);

#if WXWIN_COMPATIBILITY_2
    GetBitmapData()->m_bOk = hBmp != 0;
#endif // WXWIN_COMPATIBILITY_2

    return Ok();
}

bool wxBitmap::LoadFile(
  const wxString&                   rFilename
, long                              lType
)
{
    HPS                             hPs = NULLHANDLE;

    UnRef();

    wxBitmapHandler*                pHandler = wxDynamicCast( FindHandler(lType)
                                                             ,wxBitmapHandler
                                                            );

    if (pHandler)
    {
        m_refData = new wxBitmapRefData;

        return(pHandler->LoadFile( this
                                  ,rFilename
                                  ,hPs
                                  ,lType
                                  , -1
                                  , -1
                                 ));
    }
    else
    {
        wxImage                     vImage;

        if (!vImage.LoadFile(rFilename, lType) || !vImage.Ok() )
            return(FALSE);

        *this = vImage.ConvertToBitmap();

        return(TRUE);
    }
}

bool wxBitmap::Create(
  void*                             pData
, long                              lType
, int                               nWidth
, int                               nHeight
, int                               nDepth
)
{
    UnRef();

    wxBitmapHandler*                pHandler = wxDynamicCast( FindHandler(lType)
                                                             ,wxBitmapHandler
                                                            );

    if (!pHandler)
    {
        wxLogDebug(wxT("Failed to create bitmap: no bitmap handler for "
                       "type %d defined."), lType);

        return(FALSE);
    }

    m_refData = new wxBitmapRefData;

    return(pHandler->Create( this
                            ,pData
                            ,lType
                            ,nWidth
                            ,nHeight
                            ,nDepth
                           ));
}

bool wxBitmap::SaveFile(
  const wxString&                   rFilename
, int                               lType
, const wxPalette*                  pPalette
)
{
    wxBitmapHandler*                pHandler = wxDynamicCast( FindHandler(lType)
                                                             ,wxBitmapHandler
                                                            );

    if (pHandler)
    {
        return pHandler->SaveFile( this
                                  ,rFilename
                                  ,lType
                                  ,pPalette
                                 );
    }
    else
    {
        // FIXME what about palette? shouldn't we use it?
        wxImage                     vImage(*this);

        if (!vImage.Ok())
            return(FALSE);

        return(vImage.SaveFile( rFilename
                               ,lType
                              ));
    }
}

// ----------------------------------------------------------------------------
// wxBitmap accessors
// ----------------------------------------------------------------------------

void wxBitmap::SetQuality(
  int                               nQ
)
{
    EnsureHasData();

    GetBitmapData()->m_nQuality = nQ;
}

#if WXWIN_COMPATIBILITY_2
void wxBitmap::SetOk(
  bool                              bOk
)
{
    EnsureHasData();

    GetBitmapData()->m_bOk = bOk;
}
#endif // WXWIN_COMPATIBILITY_2

void wxBitmap::SetPalette(
  const wxPalette&                  rPalette
)
{
    EnsureHasData();

    GetBitmapData()->m_vBitmapPalette = rPalette;
}

void wxBitmap::SetMask(
  wxMask*                           pMask
)
{
    EnsureHasData();

    GetBitmapData()->m_pBitmapMask = pMask;
}

// Will try something for OS/2 but not really sure how close
// to the msw intent this is.
wxBitmap wxBitmap::GetBitmapForDC(
  wxDC&                             rDc
) const
{
    wxMemoryDC                      vMemDC;
    wxBitmap                        vTmpBitmap( this->GetWidth()
                                               ,this->GetHeight()
                                               ,rDc.GetDepth()
                                              );
    WXHBITMAP                       vOldBitmap;
    HPS                             hMemoryPS;
    HPS                             hPs;
    POINTL                          vPoint[4];
    SIZEL                           vSize = {0,0};

    hMemoryPS = ::GpiCreatePS(vHabmain, (HDC)vMemDC.GetHDC(), &vSize, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
    hPs       = ::GpiCreatePS(vHabmain, (HDC)rDc.GetHDC(), &vSize, PU_PELS | GPIT_MICRO | GPIA_ASSOC);

    // TODO: Set the points

    vOldBitmap = (WXHBITMAP)::GpiSetBitmap(hPs, (HBITMAP)vTmpBitmap.GetHBITMAP());
    ::GpiBitBlt(hPs, hMemoryPS, 4L, vPoint, ROP_SRCCOPY, BBO_IGNORE);

    return(vTmpBitmap);
}

// ----------------------------------------------------------------------------
// wxMask
// ----------------------------------------------------------------------------

wxMask::wxMask()
{
    m_hMaskBitmap = 0;
}

// Construct a mask from a bitmap and a colour indicating
// the transparent area
wxMask::wxMask(
  const wxBitmap&                   rBitmap
, const wxColour&                   rColour
)
{
    m_hMaskBitmap = 0;
    Create( rBitmap
           ,rColour
          );
}

// Construct a mask from a bitmap and a palette index indicating
// the transparent area
wxMask::wxMask(
  const wxBitmap&                   rBitmap
, int                               nPaletteIndex
)
{
    m_hMaskBitmap = 0;
    Create( rBitmap
           ,nPaletteIndex
          );
}

// Construct a mask from a mono bitmap (copies the bitmap).
wxMask::wxMask(
  const wxBitmap&                   rBitmap
)
{
    m_hMaskBitmap = 0;
    Create(rBitmap);
}

wxMask::~wxMask()
{
    if (m_hMaskBitmap)
        ::GpiDeleteBitmap((HBITMAP)m_hMaskBitmap);
}

// Create a mask from a mono bitmap (copies the bitmap).
bool wxMask::Create(
  const wxBitmap&                   rBitmap
)
{
    BITMAPINFOHEADER2               vHeader;
    DEVOPENSTRUC                    vDop = { NULL, "DISPLAY", NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    SIZEL                           vSize = {0, 0};
    POINTL                          vPoint[4];

    if (m_hMaskBitmap)
    {
        ::GpiDeleteBitmap((HBITMAP) m_hMaskBitmap);
        m_hMaskBitmap = 0;
    }
    if (!rBitmap.Ok() || rBitmap.GetDepth() != 1)
    {
        return(FALSE);
    }
    vHeader.cbFix           = sizeof(vHeader);
    vHeader.cx              = (USHORT)rBitmap.GetWidth();
    vHeader.cy              = (USHORT)rBitmap.GetHeight();
    vHeader.cPlanes         = 1;
    vHeader.cBitCount       = 1;

    m_hMaskBitmap = (WXHBITMAP) ::GpiCreateBitmap( m_hPs
                                                  ,&vHeader
                                                  ,0L
                                                  ,NULL
                                                  ,NULL
                                                 );

    HPS srcPS = ::GpiCreatePS(vHabmain, m_hDc, &vSize, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
    ::GpiSetBitmap(srcPS, (HBITMAP)rBitmap.GetHBITMAP());
    HPS destPS = ::GpiCreatePS(vHabmain, m_hDc, &vSize, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
    ::GpiSetBitmap(srcPS, (HBITMAP)m_hMaskBitmap);
    // TODO: Set the point array
    ::GpiBitBlt(destPS, srcPS, 4L, vPoint, ROP_SRCCOPY , BBO_IGNORE);

    ::GpiDestroyPS(srcPS);
    ::GpiDestroyPS(destPS);
    return(TRUE);
}

// Create a mask from a bitmap and a palette index indicating
// the transparent area
bool wxMask::Create(
  const wxBitmap&                   rBitmap
, int                               nPaletteIndex
)
{
    if (m_hMaskBitmap)
    {
        ::GpiDeleteBitmap((HBITMAP) m_hMaskBitmap);
        m_hMaskBitmap = 0;
    }
    if (rBitmap.Ok() && rBitmap.GetPalette()->Ok())
    {
        unsigned char               cRed;
        unsigned char               cGreen;
        unsigned char               cBlue;

        if (rBitmap.GetPalette()->GetRGB( nPaletteIndex
                                         ,&cRed
                                         ,&cGreen
                                         ,&cBlue
                                        ))
        {
            wxColour                vTransparentColour( cRed
                                                       ,cGreen
                                                       ,cBlue
                                                      );

            return (Create( rBitmap
                           ,vTransparentColour
                          ));
        }
    }
    return(FALSE);
}

// Create a mask from a bitmap and a colour indicating
// the transparent area
bool wxMask::Create(
  const wxBitmap&                   rBitmap
, const wxColour&                   rColour
)
{
    BITMAPINFOHEADER2               vHeader;
    DEVOPENSTRUC                    vDop = { NULL, "DISPLAY", NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    SIZEL                           vSize = {0, 0};
    POINTL                          vPoint[4];

    if (m_hMaskBitmap)
    {
        ::GpiDeleteBitmap((HBITMAP) m_hMaskBitmap);
        m_hMaskBitmap = 0;
    }
    if (!rBitmap.Ok())
    {
        return(FALSE);
    }

    // scan the bitmap for the transparent colour and set
    // the corresponding pixels in the mask to BLACK and
    // the rest to WHITE
    COLORREF                        vMaskColour = OS2RGB(rColour.Red(), rColour.Green(), rColour.Blue());

    vHeader.cbFix           = sizeof(vHeader);
    vHeader.cx              = (USHORT)rBitmap.GetWidth();
    vHeader.cy              = (USHORT)rBitmap.GetHeight();
    vHeader.cPlanes         = 1;
    vHeader.cBitCount       = 1;

    m_hMaskBitmap = (WXHBITMAP) ::GpiCreateBitmap( m_hPs
                                                  ,&vHeader
                                                  ,0L
                                                  ,NULL
                                                  ,NULL
                                                 );

    HPS srcPS = ::GpiCreatePS(vHabmain, m_hDc, &vSize, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
    ::GpiSetBitmap(srcPS, (HBITMAP)rBitmap.GetHBITMAP());
    HPS destPS = ::GpiCreatePS(vHabmain, m_hDc, &vSize, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
    ::GpiSetBitmap(srcPS, (HBITMAP)m_hMaskBitmap);

    // this is not very efficient, but I can't think
    // of a better way of doing it
    for (int w = 0; w < rBitmap.GetWidth(); w++)
    {
        for (int h = 0; h < rBitmap.GetHeight(); h++)
        {
            POINTL                  vPoint;

            vPoint.x = w;
            vPoint.y = h;

            COLORREF                col = ::GpiQueryPel(srcPS, &vPoint);

            if (col == vMaskColour)
            {
                ::GpiSetColor(destPS, CLR_WHITE);
                ::GpiSetPel(destPS, &vPoint);
            }
            else
            {
                ::GpiSetColor(destPS, CLR_BLACK);
                ::GpiSetPel(destPS, &vPoint);
            }
        }
    }
    ::GpiDestroyPS(srcPS);
    ::GpiDestroyPS(destPS);
    return(TRUE);
}

// ----------------------------------------------------------------------------
// wxBitmapHandler
// ----------------------------------------------------------------------------

bool wxBitmapHandler::Create(
  wxGDIImage*                       pImage
, void*                             pData
, long                              lFlags
, int                               nWidth
, int                               nHeight
, int                               nDepth
)
{
    wxBitmap*                       pBitmap = wxDynamicCast( pImage
                                                            ,wxBitmap
                                                           );

    return(pBitmap ? Create( pBitmap
                            ,pData
                            ,nWidth
                            ,nHeight
                            ,nDepth
                           ) : FALSE);
}

bool wxBitmapHandler::Load(
  wxGDIImage*                       pImage
, const wxString&                   rName
, HPS                               hPs
, long                              lFlags
, int                               nWidth
, int                               nHeight
)
{
    wxBitmap*                       pBitmap = wxDynamicCast( pImage
                                                            ,wxBitmap
                                                           );

    return(pBitmap ? LoadFile( pBitmap
                              ,rName
                              ,hPs
                              ,lFlags
                              ,nWidth
                              ,nHeight
                             ) : FALSE);
}

bool wxBitmapHandler::Save(
  wxGDIImage*                       pImage
, const wxString&                   rName
, int                               lType
)
{
    wxBitmap*                       pBitmap = wxDynamicCast( pImage
                                                            ,wxBitmap
                                                           );

    return(pBitmap ? SaveFile( pBitmap
                              ,rName
                              ,lType
                             ) : FALSE);
}

bool wxBitmapHandler::Create(
  wxBitmap*                         WXUNUSED(pBitmap)
, void*                             WXUNUSED(pData)
, long                              WXUNUSED(lType)
, int                               WXUNUSED(nWidth)
, int                               WXUNUSED(nHeight)
, int                               WXUNUSED(nDepth)
)
{
    return(FALSE);
}

bool wxBitmapHandler::LoadFile(
  wxBitmap*                         WXUNUSED(pBitmap)
, const wxString&                   WXUNUSED(rName)
, HPS                               WXUNUSED(hPs)
, long                              WXUNUSED(lType)
, int                               WXUNUSED(nDesiredWidth)
, int                               WXUNUSED(nDesiredHeight)
)
{
    return(FALSE);
}

bool wxBitmapHandler::SaveFile(
  wxBitmap*                         WXUNUSED(pBitmap)
, const wxString&                   WXUNUSED(rName)
, int                               WXUNUSED(nType)
, const wxPalette*                  WXUNUSED(pPalette)
)
{
    return(FALSE);
}

