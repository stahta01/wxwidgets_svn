/////////////////////////////////////////////////////////////////////////////
// Name:        statbmp.cpp
// Purpose:     wxStaticBitmap
// Author:      David Webster
// Modified by:
// Created:     11/27/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "statbmp.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/window.h"
#include "wx/os2/private.h"

#ifndef WX_PRECOMP
    #include "wx/icon.h"
    #include "wx/statbmp.h"
#endif

#include <stdio.h>

// ---------------------------------------------------------------------------
// macors
// ---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxStaticBitmap, wxControl)

static wxGDIImage* ConvertImage(
  const wxGDIImage&                 rBitmap
)
{
    bool                            bIsIcon = rBitmap.IsKindOf( CLASSINFO(wxIcon) );

    if(!bIsIcon )
    {
        wxASSERT_MSG( wxDynamicCast(&rBitmap, wxBitmap),
                      _T("not an icon and not a bitmap?") );

        const wxBitmap&             rBmp = (const wxBitmap&)rBitmap;
        wxMask*                     pMask = rBmp.GetMask();

        if (pMask && pMask->GetMaskBitmap())
        {
            wxIcon*                 pIcon = new wxIcon;

            pIcon->CopyFromBitmap(rBmp);
            return pIcon;
        }
        return new wxBitmap(rBmp);
    }

    // copying a bitmap is a cheap operation
    return new wxIcon( (const wxIcon&)rBitmap );
} // end of ConvertImage

// ---------------------------------------------------------------------------
//  wxStaticBitmap
// ---------------------------------------------------------------------------

bool wxStaticBitmap::Create(
  wxWindow*                         pParent
, wxWindowID                        nId
, const wxGDIImage&                 rBitmap
, const wxPoint&                    rPos
, const wxSize&                     rSize
, long                              lStyle
, const wxString&                   rName
)
{
    ERRORID                         vError;
    wxString                        sError;

    Init();

    SetName(rName);
    if (pParent)
        pParent->AddChild(this);

    if (nId == -1)
        m_windowId = (int)NewControlId();
    else
        m_windowId = nId;

    m_windowStyle = lStyle;

    int                             nX= rPos.x;
    int                             nY = rPos.y;
    int                             nWidth = rSize.x;
    int                             nHeight = rSize.y;
    char                            zId[16];

    m_windowStyle = lStyle;

    m_bIsIcon = rBitmap.IsKindOf(CLASSINFO(wxIcon));

    //
    // For now we only support an ICON
    //
    int                             nWinstyle = SS_ICON;

    sprintf(zId, "#%d", rBitmap.GetId());
    m_hWnd = (WXHWND)::WinCreateWindow( pParent->GetHWND()
                                       ,WC_STATIC
                                       ,zId
                                       ,nWinstyle | WS_VISIBLE
                                       ,0,0,0,0
                                       ,pParent->GetHWND()
                                       ,HWND_TOP
                                       ,m_windowId
                                       ,NULL
                                       ,NULL
                                      );
    if (!m_hWnd)
    {
        vError = ::WinGetLastError(wxGetInstance());
        sError = wxPMErrorToStr(vError);
        return FALSE;
    }
    wxCHECK_MSG( m_hWnd, FALSE, wxT("Failed to create static bitmap") );
    m_pImage = ConvertImage(rBitmap);
    m_pImage->SetHandle((WXHWND)::WinSendMsg(m_hWnd, SM_QUERYHANDLE, (MPARAM)0, (MPARAM)0));

    // Subclass again for purposes of dialog editing mode
    SubclassWin(m_hWnd);
    return(TRUE);
} // end of wxStaticBitmap::Create

bool wxStaticBitmap::ImageIsOk() const
{
    return(m_pImage && m_pImage->Ok());
}

void wxStaticBitmap::Free()
{
    if (m_pImage)
        delete m_pImage;
    m_pImage = NULL;
} // end of wxStaticBitmap::Free

wxSize wxStaticBitmap::DoGetBestSize() const
{
    //
    // Reuse the current size (as wxWindow does) instead of using some
    // arbitrary default size (as wxControl, our immediate base class, does)
    //
    return wxWindow::DoGetBestSize();
}

void wxStaticBitmap::SetImage(
  const wxGDIImage&                 rBitmap
)
{
    int                             nX = 0;
    int                             nY = 0;
    int                             nWidth = 0;
    int                             nHeight = 0;

    Free();
    ::WinSendMsg( GetHwnd()
                 ,SM_SETHANDLE
                 ,MPFROMHWND(rBitmap.GetHandle())
                 ,NULL
                );
    m_pImage = ConvertImage(rBitmap);

    GetPosition(&nX, &nY);
    GetSize(&nWidth, &nHeight);

    RECTL                           vRect;

    vRect.xLeft   = nX;
    vRect.yTop    = nY;
    vRect.xRight  = nX + nWidth;
    vRect.yBottom = nY + nHeight;

    ::WinInvalidateRect(GetHwndOf(GetParent()), &vRect, TRUE);
}

MRESULT wxStaticBitmap::OS2WindowProc(
  WXUINT                            uMsg
, WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    return wxWindow::OS2WindowProc(uMsg, wParam, lParam);
} // end of wxStaticBitmap::OS2WindowProc
