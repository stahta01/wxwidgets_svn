/////////////////////////////////////////////////////////////////////////////
// Name:        windows.cpp
// Purpose:     wxWindow
// Author:      David Webster
// Modified by:
// Created:     10/12/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//
// For compilers that support precompilation, includes "wx.h".
//
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #define INCL_DOS
    #define INCL_PM
    #include <os2.h>
    #include "wx/window.h"
    #include "wx/accel.h"
    #include "wx/setup.h"
    #include "wx/menu.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/utils.h"
    #include "wx/app.h"
    #include "wx/panel.h"
    #include "wx/layout.h"
    #include "wx/dialog.h"
    #include "wx/frame.h"
    #include "wx/listbox.h"
    #include "wx/button.h"
    #include "wx/msgdlg.h"

    #include <stdio.h>
#endif

#if     wxUSE_OWNER_DRAWN
    #include "wx/ownerdrw.h"
#endif

#if     wxUSE_DRAG_AND_DROP
    #include "wx/dnd.h"
#endif

#include "wx/menuitem.h"
#include "wx/log.h"

#include "wx/os2/private.h"

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#if wxUSE_CARET
    #include "wx/caret.h"
#endif // wxUSE_CARET

#include "wx/intl.h"
#include "wx/log.h"


#include "wx/textctrl.h"

#include <string.h>

//
// Place compiler, OS specific includes here
//

//
// Standard macros -- these are for OS/2 PM, but most GUI's have something similar
//
#ifndef GET_X_LPARAM
//
//  SHORT1FROMMP -- LOWORD
//
    #define GET_X_LPARAM(mp) ((unsigned short)(unsigned long)(mp))
//
//  SHORT2FROMMP -- HIWORD
//
    #define GET_Y_LPARAM(mp) ((unsigned short)(unsigned long)(mp >> 16))
#endif // GET_X_LPARAM

#ifndef CW_USEDEFAULT
#  define CW_USEDEFAULT    ((int)0x80000000)
#endif

// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

//
// The last Windows message we got (MT-UNSAFE)
//
QMSG                      s_currentMsg;

wxMenu*                   wxCurrentPopupMenu = NULL;
extern wxList WXDLLEXPORT wxPendingDelete;
#if !defined(__VISAGECPP__) || (__IBMCPP__ < 400)
extern wxChar             wxCanvasClassName[];
#endif
wxList*                   wxWinHandleList = NULL;

// ---------------------------------------------------------------------------
// private functions
// ---------------------------------------------------------------------------

//
// the window proc for all our windows; most gui's have something similar
//
MRESULT EXPENTRY wxWndProc( HWND hWnd
                           ,ULONG message
                           ,MPARAM mp1
                           ,MPARAM mp2
                          );

#ifdef  __WXDEBUG__
    const char *wxGetMessageName(int message);
#endif  //__WXDEBUG__

void      wxRemoveHandleAssociation(wxWindow* pWin);
void      wxAssociateWinWithHandle( HWND      hWnd
                                   ,wxWindow* pWin
                                  );
wxWindow* wxFindWinFromHandle(WXHWND hWnd);

//
// This magical function is used to translate VK_APPS key presses to right
// mouse clicks
//
static void TranslateKbdEventToMouse( wxWindow* pWin
                                     ,int*      pX
                                     ,int*      pY
                                     ,MPARAM*   pFlags
                                    );

//
// get the current state of SHIFT/CTRL keys
//
static inline bool IsShiftDown() { return (::WinGetKeyState(HWND_DESKTOP, VK_SHIFT) & 0x8000) != 0; }
static inline bool IsCtrlDown() { return (::WinGetKeyState(HWND_DESKTOP, VK_CTRL) & 0x8000) != 0; }
// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

    IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowBase)

BEGIN_EVENT_TABLE(wxWindow, wxWindowBase)
    EVT_ERASE_BACKGROUND(wxWindowOS2::OnEraseBackground)
    EVT_SYS_COLOUR_CHANGED(wxWindowOS2::OnSysColourChanged)
    EVT_INIT_DIALOG(wxWindowOS2::OnInitDialog)
    EVT_IDLE(wxWindowOS2::OnIdle)
    EVT_SET_FOCUS(wxWindowOS2::OnSetFocus)
END_EVENT_TABLE()

// ===========================================================================
// implementation
// ===========================================================================

//
// Find an item given the PM Window id
//
wxWindow* wxWindowOS2::FindItem(
  long                              lId
) const
{
    wxControl*                      pItem = wxDynamicCast( this
                                                          ,wxControl
                                                         );

    if (pItem)
    {
        //
        // I it we or one of our "internal" children?
        //
        if (pItem->GetId() == lId ||
             (pItem->GetSubcontrols().Index(lId) != wxNOT_FOUND))
        {
            return pItem;
        }
    }

    wxWindowList::Node*             pCurrent = GetChildren().GetFirst();

    while (pCurrent)
    {
        wxWindow*                   pChildWin = pCurrent->GetData();
        wxWindow*                   pWnd = pChildWin->FindItem(lId);

        if (pWnd)
            return pWnd;

        pCurrent = pCurrent->GetNext();
    }
    return(NULL);
} // end of wxWindowOS2::FindItem

//
// Find an item given the PM Window handle
//
wxWindow* wxWindowOS2::FindItemByHWND(
  WXHWND                            hWnd
, bool                              bControlOnly
) const
{
    wxWindowList::Node*             pCurrent = GetChildren().GetFirst();

    while (pCurrent)
    {
        wxWindow*                   pParent = pCurrent->GetData();

        //
        // Do a recursive search.
        //
        wxWindow*                   pWnd = pParent->FindItemByHWND(hWnd);

        if (pWnd)
            return(pWnd);

        if (!bControlOnly || pParent->IsKindOf(CLASSINFO(wxControl)))
        {
            wxWindow*               pItem = pCurrent->GetData();

            if (pItem->GetHWND() == hWnd)
                return(pItem);
            else
            {
                if (pItem->ContainsHWND(hWnd))
                    return(pItem);
            }
        }
        pCurrent = pCurrent->GetNext();
    }
    return(NULL);
} // end of wxWindowOS2::FindItemByHWND

//
// Default command handler
//
bool wxWindowOS2::OS2Command(
  WXUINT                            WXUNUSED(uParam)
, WXWORD                            WXUNUSED(uId)
)
{
    return(FALSE);
}

// ----------------------------------------------------------------------------
// constructors and such
// ----------------------------------------------------------------------------

void wxWindowOS2::Init()
{
    //
    // Generic
    //
    InitBase();

    //
    // PM specific
    //
    m_bDoubleClickAllowed = 0;
    m_bWinCaptured = FALSE;

    m_isBeingDeleted = FALSE;
    m_fnOldWndProc = 0;
    m_bUseCtl3D = FALSE;
    m_bMouseInWindow = FALSE;

    //
    // wxWnd
    //
    m_hMenu             = 0L;
    m_hWnd              = 0L;
    m_hWndScrollBarHorz = 0L;
    m_hWndScrollBarVert = 0L;

    //
    // Pass WM_GETDLGCODE to DefWindowProc()
    m_lDlgCode = 0;

    m_nXThumbSize = 0;
    m_nYThumbSize = 0;
    m_bBackgroundTransparent = FALSE;

    //
    // As all windows are created with WS_VISIBLE style...
    //
    m_isShown = TRUE;

#if wxUSE_MOUSEEVENT_HACK
    m_lLastMouseX =
    m_lLastMouseY = -1;
    m_nLastMouseEvent = -1;
#endif // wxUSE_MOUSEEVENT_HACK
} // wxWindowOS2::Init

//
// Destructor
//
wxWindowOS2::~wxWindowOS2()
{
    m_isBeingDeleted = TRUE;

    OS2DetachWindowMenu();
    for (wxWindow* pWin = GetParent(); pWin; pWin = pWin->GetParent())
    {
        wxFrame*                    pFrame = wxDynamicCast(pWin, wxFrame);

        if (pFrame)
        {
            if (pFrame->GetLastFocus() == this)
                pFrame->SetLastFocus((wxWindow*)NULL);
        }
    }
    if (m_parent)
        m_parent->RemoveChild(this);
    DestroyChildren();

    if (m_hWnd)
    {
        if(!::WinDestroyWindow(GetHWND()))
            wxLogLastError(wxT("DestroyWindow"));
        //
        // remove hWnd <-> wxWindow association
        //
        wxRemoveHandleAssociation(this);
    }
} // end of wxWindowOS2::~wxWindowOS2

bool wxWindowOS2::Create(
  wxWindow*                         pParent
, wxWindowID                        vId
, const wxPoint&                    rPos
, const wxSize&                     rSize
, long                              lStyle
, const wxString&                   rName
)
{
    HWND                            hParent = NULLHANDLE;
    wxPoint                         vPos = rPos; // The OS/2 position
    ULONG                           ulCreateFlags = 0L;

    wxCHECK_MSG(pParent, FALSE, wxT("can't create wxWindow without parent"));

    if ( !CreateBase( pParent
                     ,vId
                     ,rPos
                     ,rSize
                     ,lStyle
                     ,wxDefaultValidator
                     ,rName
                    ))
        return(FALSE);

    if (pParent)
    {
        int                         nTempy;

        pParent->AddChild(this);
        hParent = GetWinHwnd(pParent);
        //
        // OS2 uses normal coordinates, no bassackwards Windows ones
        //
        if (pParent->IsKindOf(CLASSINFO(wxGenericScrolledWindow)) ||
            pParent->IsKindOf(CLASSINFO(wxScrolledWindow))
           )
        {
            wxWindow*               pGrandParent = NULL;

            pGrandParent = pParent->GetParent();
            if (pGrandParent)
                nTempy = pGrandParent->GetSize().y - (vPos.y + rSize.y);
            else
                nTempy = pParent->GetSize().y - (vPos.y + rSize.y);
        }
        else
            nTempy = pParent->GetSize().y - (vPos.y + rSize.y);
        vPos.y = nTempy;
        if ( pParent->IsKindOf(CLASSINFO(wxGenericScrolledWindow)) ||
             pParent->IsKindOf(CLASSINFO(wxScrolledWindow))
           )
            ulCreateFlags |= WS_CLIPSIBLINGS;
    }
    else
    {
        RECTL                   vRect;

        ::WinQueryWindowRect(HWND_DESKTOP, &vRect);
        hParent = HWND_DESKTOP;
        vPos.y = vRect.yTop - (vPos.y + rSize.y);
    }

    //
    // Most wxSTYLES are really PM Class specific styles and will be
    // set in those class create procs.  PM's basic windows styles are
    // very limited.
    //
    ulCreateFlags |=  WS_VISIBLE;


    if (lStyle & wxCLIP_SIBLINGS)
        ulCreateFlags |= WS_CLIPSIBLINGS;

    if (lStyle & wxCLIP_CHILDREN )
        ulCreateFlags |= WS_CLIPCHILDREN;

    //
    //
    //
    bool                            bWant3D;
    WXDWORD                         dwExStyle = Determine3DEffects( WS_EX_CLIENTEDGE
                                                                   ,&bWant3D
                                                                  );

    //
    // Add the simple border style as we'll use this to draw borders
    //
    if (lStyle & wxSIMPLE_BORDER)
        dwExStyle |= wxSIMPLE_BORDER;

    //
    // Generic OS/2 Windows are created with no owner, no Z Order, no Control data,
    // and no presentation parameters
    //
    OS2Create( hParent
              ,(PSZ)wxCanvasClassName
              ,rName.c_str()
              ,ulCreateFlags
              ,vPos.x
              ,vPos.y
              ,WidthDefault(rSize.x)
              ,HeightDefault(rSize.y)
              ,NULLHANDLE
              ,NULLHANDLE
              ,m_windowId
              ,NULL
              ,NULL
              ,dwExStyle
             );

    return(TRUE);
} // end of wxWindowOS2::Create

// ---------------------------------------------------------------------------
// basic operations
// ---------------------------------------------------------------------------

void wxWindowOS2::SetFocus()
{
    HWND                            hWnd = GetHwnd();

    if (hWnd)
        ::WinSetFocus(HWND_DESKTOP, hWnd);
} // end of wxWindowOS2::SetFocus

wxWindow* wxWindowBase::FindFocus()
{
    HWND                            hWnd = ::WinQueryFocus(HWND_DESKTOP);

    if (hWnd)
    {
        return wxFindWinFromHandle((WXHWND)hWnd);
    }
    return NULL;
} // wxWindowBase::FindFocus

bool wxWindowOS2::Enable(
  bool                              bEnable
)
{
    if (!wxWindowBase::Enable(bEnable))
        return(FALSE);

    HWND                            hWnd = GetHwnd();

    if ( hWnd )
        ::WinEnableWindow(hWnd, (BOOL)bEnable);

    wxWindowList::Node*             pNode = GetChildren().GetFirst();

    while (pNode)
    {
        wxWindow*                   pChild = pNode->GetData();

        pChild->Enable(bEnable);
        pNode = pNode->GetNext();
    }
    return(TRUE);
} // end of wxWindowOS2::Enable

bool wxWindowOS2::Show(
  bool                              bShow
)
{
    if (!wxWindowBase::Show(bShow))
        return(FALSE);

    HWND                            hWnd = GetHwnd();

    ::WinShowWindow(hWnd, bShow);

    if (bShow)
    {
        ::WinSetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);
    }
    return(TRUE);
} // end of wxWindowOS2::Show

void wxWindowOS2::Raise()
{
    ::WinSetWindowPos(GetHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_ZORDER | SWP_ACTIVATE);
} // end of wxWindowOS2::Raise

void wxWindowOS2::Lower()
{
    ::WinSetWindowPos(GetHwnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_ZORDER | SWP_DEACTIVATE);
} // end of wxWindowOS2::Lower

void wxWindowOS2::SetTitle(
  const wxString&                   rTitle
)
{
    ::WinSetWindowText(GetHwnd(), rTitle.c_str());
} // end of wxWindowOS2::SetTitle

wxString wxWindowOS2::GetTitle() const
{
    return wxGetWindowText(GetHWND());
} // end of wxWindowOS2::GetTitle

void wxWindowOS2::CaptureMouse()
{
    HWND                            hWnd = GetHwnd();

    if (hWnd && !m_bWinCaptured)
    {
        ::WinSetCapture(HWND_DESKTOP, hWnd);
        m_bWinCaptured = TRUE;
    }
} // end of wxWindowOS2::GetTitle

void wxWindowOS2::ReleaseMouse()
{
    if (m_bWinCaptured)
    {
        ::WinSetCapture(HWND_DESKTOP, NULLHANDLE);
        m_bWinCaptured = FALSE;
    }
} // end of wxWindowOS2::ReleaseMouse

/* static */ wxWindow* wxWindowBase::GetCapture()
{
    HWND hwnd = ::WinQueryCapture(HWND_DESKTOP);
    return hwnd ? wxFindWinFromHandle((WXHWND)hwnd) : (wxWindow *)NULL;
} // end of wxWindowBase::GetCapture

bool wxWindowOS2::SetFont(
  const wxFont&                     rFont
)
{
    if (!wxWindowBase::SetFont(rFont))
    {
        // nothing to do
        return(FALSE);
    }

    HWND                            hWnd = GetHwnd();

    if (hWnd != 0)
    {
        wxChar                      zFont[128];

        sprintf(zFont, "%d.%s", rFont.GetPointSize(), rFont.GetFaceName().c_str());
        return(::WinSetPresParam(hWnd, PP_FONTNAMESIZE, strlen(zFont), (PVOID)zFont));
    }
    return(TRUE);
}

bool wxWindowOS2::SetCursor(
  const wxCursor&                   rCursor
) // check if base implementation is OK
{
    if ( !wxWindowBase::SetCursor(rCursor))
    {
        // no change
        return FALSE;
    }

    wxASSERT_MSG( m_cursor.Ok(),
                  wxT("cursor must be valid after call to the base version"));

    HWND                            hWnd = GetHwnd();
    POINTL                          vPoint;
    RECTL                           vRect;
    HPS                             hPS;
    HRGN                            hRGN;

    hPS = ::WinGetPS(hWnd);

    ::WinQueryPointerPos(HWND_DESKTOP, &vPoint);
    ::WinQueryWindowRect(hWnd, &vRect);

    hRGN = ::GpiCreateRegion(hPS, 1L, &vRect);

    if ((::GpiPtInRegion(hPS, hRGN, &vPoint) == PRGN_INSIDE) && !wxIsBusy())
    {
        ::WinSetPointer(HWND_DESKTOP, (HPOINTER)m_cursor.GetHCURSOR());
    }
    return TRUE;
} // end of wxWindowOS2::SetCursor

void wxWindowOS2::WarpPointer(
  int                               nXPos
, int                               nYPos
)
{
    int                             nX = nXPos;
    int                             nY = nYPos;
    RECTL                           vRect;

    ::WinQueryWindowRect(GetHwnd(), &vRect);
    nX += vRect.xLeft;
    nY += vRect.yBottom;

    ::WinSetPointerPos(HWND_DESKTOP, (LONG)nX, (LONG)(nY));
} // end of wxWindowOS2::WarpPointer

#if WXWIN_COMPATIBILITY
void wxWindowOS2::OS2DeviceToLogical (float *x, float *y) const
{
}
#endif // WXWIN_COMPATIBILITY

// ---------------------------------------------------------------------------
// scrolling stuff
// ---------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY
void wxWindowOS2::SetScrollRange(
  int                               nOrient
, int                               nRange
, bool                              bRefresh
)
{
    int                             nRange1 = nRange;
    int                             nPageSize = GetScrollPage(nOrient);

    if (nPpageSize > 1 && nRange > 0)
    {
        nRange1 += (nPageSize - 1);
    }

    if (nOrient == wxHORIZONTAL)
    {
        ::WinSendMsg(m_hWndScrollBarHorz, SBM_SETSCROLLBAR, (MPARAM)0, MPFROM2SHORT(0, (SHORT)nRange1));
        ::WinSendMsg(m_hWndScrollBarHorz, SBM_SETTHUMBSIZE, MPFROM2SHORT((SHORT)nThumbVisible, (SHORT)nRange1), (MPARAM)0);
    }
    else
    {
        ::WinSendMsg(m_hWndScrollBarVert, SBM_SETSCROLLBAR, (MPARAM)0, MPFROM2SHORT(0, (SHORT)nRange1));
        ::WinSendMsg(m_hWndScrollBarVert, SBM_SETTHUMBSIZE, MPFROM2SHORT((SHORT)nThumbVisible, (SHORT)nRange1), (MPARAM)0);
    }
} // end of wxWindowOS2::SetScrollRange

void wxWindowOS2::SetScrollPage(
  int                               nOrient
, int                               nPage
, bool                              bRefresh
)
{
    if (nOrient == wxHORIZONTAL )
        m_nXThumbSize = nPage;
    else
        m_nYThumbSize = nPage;
} // end of wxWindowOS2::SetScrollPage

int wxWindowOS2::OldGetScrollRange(
  int                               nOrient
) const
{
    MRESULT                         mRc;
    HWND                            hWnd = GetHwnd();

    if (hWnd)
    {
        mRc = WinSendMsg(hWnd, SBM_QUERYRANGE, (MPARAM)0L, (MPARAM)0L);
        return(SHORT2FROMMR(mRc));
     }
     return 0;
} // end of wxWindowOS2::OldGetScrollRange

int  wxWindowOS2::GetScrollPage(
  int                               nOrient
) const
{
    if (nOrient == wxHORIZONTAL)
        return m_nXThumbSize;
    else
        return m_nYThumbSize;
} // end of wxWindowOS2::GetScrollPage
#endif // WXWIN_COMPATIBILITY

int  wxWindowOS2::GetScrollPos(
  int                               nOrient
) const
{
    if (nOrient == wxHORIZONTAL)
        return((int)::WinSendMsg(m_hWndScrollBarHorz, SBM_QUERYPOS, (MPARAM)NULL, (MPARAM)NULL));
    else
        return((int)::WinSendMsg(m_hWndScrollBarVert, SBM_QUERYPOS, (MPARAM)NULL, (MPARAM)NULL));
} // end of wxWindowOS2::GetScrollPos

int wxWindowOS2::GetScrollRange(
  int                               nOrient
) const
{
    MRESULT                         mr;

    if (nOrient == wxHORIZONTAL)
        mr = ::WinSendMsg(m_hWndScrollBarHorz, SBM_QUERYRANGE, (MPARAM)NULL, (MPARAM)NULL);
    else
        mr = ::WinSendMsg(m_hWndScrollBarVert, SBM_QUERYRANGE, (MPARAM)NULL, (MPARAM)NULL);
    return((int)SHORT2FROMMR(mr));
} // end of wxWindowOS2::GetScrollRange

int wxWindowOS2::GetScrollThumb(
  int                               nOrient
) const
{
    if (nOrient == wxHORIZONTAL )
        return m_nXThumbSize;
    else
        return m_nYThumbSize;
} // end of wxWindowOS2::GetScrollThumb

void wxWindowOS2::SetScrollPos(
  int                               nOrient
, int                               nPos
, bool                              bRefresh
)
{
    if (nOrient == wxHORIZONTAL )
        ::WinSendMsg(m_hWndScrollBarHorz, SBM_SETPOS, (MPARAM)nPos, (MPARAM)NULL);
    else
        ::WinSendMsg(m_hWndScrollBarVert, SBM_SETPOS, (MPARAM)nPos, (MPARAM)NULL);
} // end of wxWindowOS2::SetScrollPos(

void wxWindowOS2::SetScrollbar(
  int                               nOrient
, int                               nPos
, int                               nThumbVisible
, int                               nRange
, bool                              bRefresh
)
{
    int                             nOldRange = nRange - nThumbVisible;
    int                             nRange1 = nOldRange;
    int                             nPageSize = nThumbVisible;
    SBCDATA                         vInfo;
    HWND                            hWnd = GetHwnd();
    ULONG                           ulStyle = WS_VISIBLE | WS_SYNCPAINT;
    RECTL                           vRect;

    ::WinQueryWindowRect(hWnd, &vRect);
    if (nPageSize > 1 && nRange > 0)
    {
        nRange1 += (nPageSize - 1);
    }

    vInfo.cb = sizeof(SBCDATA);
    vInfo.posFirst = 0;
    vInfo.posLast = (SHORT)nRange1;
    vInfo.posThumb = nPos;

    if (nOrient == wxHORIZONTAL )
    {
        ulStyle |= SBS_HORZ;
        if (m_hWndScrollBarHorz == 0L)
        {
            //
            // We create the scrollbars with the desktop so that they are not
            // registered as child windows of the window in order that child
            // windows may be scrolled without scrolling the scrollbars themselves!
            //
            m_hWndScrollBarHorz = ::WinCreateWindow( hWnd
                                                    ,WC_SCROLLBAR
                                                    ,(PSZ)NULL
                                                    ,ulStyle
                                                    ,vRect.xLeft
                                                    ,vRect.yBottom
                                                    ,vRect.xRight - vRect.xLeft
                                                    ,20
                                                    ,hWnd
                                                    ,HWND_TOP
                                                    ,FID_HORZSCROLL
                                                    ,&vInfo
                                                    ,NULL
                                                   );
        }
        else
        {
            RECTL                   vRect2;

            //
            // Only want to resize the scrollbar if it changes, otherwise
            // we'd probably end up in a recursive loop until we crash the call stack
            // because this method is called in a ScrolledWindow OnSize event and SWP_MOVE | SWP_SIZE
            // generates those events.
            //
            ::WinQueryWindowRect(m_hWndScrollBarHorz, &vRect2);
            if (!(vRect2.xLeft == vRect.xLeft     &&
                  vRect2.xRight == vRect.xRight   &&
                  vRect2.yBottom == vRect.yBottom &&
                  vRect2.yTop == vRect.yTop
                ) )
            {
                ::WinSetWindowPos( m_hWndScrollBarHorz
                                  ,HWND_TOP
                                  ,vRect.xLeft
                                  ,vRect.yBottom
                                  ,vRect.xRight - vRect.xLeft
                                  ,20
                                  ,SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW
                                 );
            }
            ::WinSendMsg(m_hWndScrollBarHorz, SBM_SETSCROLLBAR, (MPARAM)nPos, MPFROM2SHORT(0, (SHORT)nRange1));
            ::WinSendMsg(m_hWndScrollBarHorz, SBM_SETTHUMBSIZE, MPFROM2SHORT((SHORT)nThumbVisible, (SHORT)nRange1), (MPARAM)0);
        }
    }
    else
    {
        ulStyle |= SBS_VERT;
        if (m_hWndScrollBarVert == 0L)
        {
            m_hWndScrollBarVert = ::WinCreateWindow( hWnd
                                                    ,WC_SCROLLBAR
                                                    ,(PSZ)NULL
                                                    ,ulStyle
                                                    ,vRect.xRight - 20
                                                    ,vRect.yBottom + 20
                                                    ,20
                                                    ,vRect.yTop - (vRect.yBottom + 20)
                                                    ,hWnd
                                                    ,HWND_TOP
                                                    ,FID_VERTSCROLL
                                                    ,&vInfo
                                                    ,NULL
                                                   );
        }
        else
        {
            RECTL                   vRect2;

            //
            // Only want to resize the scrollbar if it changes, otherwise
            // we'd probably end up in a recursive loop until we crash the call stack
            // because this method is called in a ScrolledWindow OnSize event and SWP_MOVE | SWP_SIZE
            // generates those events.
            //
            ::WinQueryWindowRect(m_hWndScrollBarVert, &vRect2);
            if (!(vRect2.xLeft == vRect.xLeft     &&
                  vRect2.xRight == vRect.xRight   &&
                  vRect2.yBottom == vRect.yBottom &&
                  vRect2.yTop == vRect.yTop
                ) )
            {
                ::WinSetWindowPos( m_hWndScrollBarVert
                                  ,HWND_TOP
                                  ,vRect.xRight - 20
                                  ,vRect.yBottom + 20
                                  ,20
                                  ,vRect.yTop - (vRect.yBottom + 20)
                                  ,SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW
                                 );
            }
            ::WinSendMsg(m_hWndScrollBarVert, SBM_SETSCROLLBAR, (MPARAM)nPos, MPFROM2SHORT(0, (SHORT)nRange1));
            ::WinSendMsg(m_hWndScrollBarVert, SBM_SETTHUMBSIZE, MPFROM2SHORT((SHORT)nThumbVisible, (SHORT)nRange1), (MPARAM)0);
        }
        m_nYThumbSize = nThumbVisible;
    }
} // end of wxWindowOS2::SetScrollbar

void wxWindowOS2::ScrollWindow(
  int                               nDx
, int                               nDy
, const wxRect*                     pRect
)
{
    RECTL                           vRect;
    RECTL                           vRect2;

    nDy *= -1; // flip the sign of Dy as OS/2 is opposite wxWin.
    if (pRect)
    {
        vRect2.xLeft   = pRect->x;
        vRect2.yTop    = pRect->y + pRect->height;
        vRect2.xRight  = pRect->x + pRect->width;
        vRect2.yBottom = pRect->y;
    }
    else
    {
        ::WinQueryWindowRect(GetHwnd(), &vRect2);
        ::WinQueryWindowRect(m_hWndScrollBarHorz, &vRect);
        vRect2.yBottom += vRect.yTop - vRect.yBottom;
        ::WinQueryWindowRect(m_hWndScrollBarVert, &vRect);
        vRect2.xRight -= vRect.xRight - vRect.xLeft;

    }
    if (pRect)
        ::WinScrollWindow( GetHwnd()
                          ,(LONG)nDx
                          ,(LONG)nDy
                          ,&vRect2
                          ,NULL
                          ,NULLHANDLE
                          ,NULL
                          ,SW_INVALIDATERGN
                         );
    else
        ::WinScrollWindow( GetHwnd()
                          ,nDx
                          ,nDy
                          ,NULL
                          ,NULL
                          ,NULLHANDLE
                          ,NULL
                          ,SW_INVALIDATERGN
                         );

    //
    // Move the children
    wxWindowList::Node*             pCurrent = GetChildren().GetFirst();
    SWP                             vSwp;

    while (pCurrent)
    {
        wxWindow*                   pChildWin = pCurrent->GetData();

        if (pChildWin->GetHWND() != NULLHANDLE)
        {
            ::WinQueryWindowPos(pChildWin->GetHWND(), &vSwp);
            ::WinQueryWindowRect(pChildWin->GetHWND(), &vRect);
            if (pChildWin->GetHWND() == m_hWndScrollBarVert ||
                pChildWin->GetHWND() == m_hWndScrollBarHorz)
            {
                ::WinSetWindowPos( pChildWin->GetHWND()
                                  ,HWND_TOP
                                  ,vSwp.x + nDx
                                  ,vSwp.y + nDy
                                  ,0
                                  ,0
                                  ,SWP_MOVE | SWP_SHOW | SWP_ZORDER
                                 );
            }
            else
            {
                ::WinSetWindowPos( pChildWin->GetHWND()
                                  ,HWND_BOTTOM
                                  ,vSwp.x + nDx
                                  ,vSwp.y + nDy
                                  ,0
                                  ,0
                                  ,SWP_MOVE | SWP_ZORDER
                                 );
                ::WinInvalidateRect(pChildWin->GetHWND(), &vRect, FALSE);
            }
        }
        pCurrent = pCurrent->GetNext();
    }
} // end of wxWindowOS2::ScrollWindow

// ---------------------------------------------------------------------------
// subclassing
// ---------------------------------------------------------------------------

void wxWindowOS2::SubclassWin(
  WXHWND                            hWnd
)
{
    HWND                            hwnd = (HWND)hWnd;

    wxASSERT_MSG( !m_fnOldWndProc, wxT("subclassing window twice?") );
    wxCHECK_RET(::WinIsWindow(vHabmain, hwnd), wxT("invalid HWND in SubclassWin") );
    m_fnOldWndProc = (WXFARPROC) ::WinSubclassWindow(hwnd, (PFNWP)wxWndProc);
} // end of wxWindowOS2::SubclassWin

void wxWindowOS2::UnsubclassWin()
{
    //
    // Restore old Window proc
    //
    HWND                            hwnd = GetHWND();

    if (m_hWnd)
    {
        wxCHECK_RET( ::WinIsWindow(vHabmain, hwnd), wxT("invalid HWND in UnsubclassWin") );

        PFNWP                       fnProc = (PFNWP)::WinQueryWindowPtr(hwnd, QWP_PFNWP);

        if ( (m_fnOldWndProc != 0) && (fnProc != (PFNWP) m_fnOldWndProc))
        {
            WinSubclassWindow(hwnd, (PFNWP)m_fnOldWndProc);
            m_fnOldWndProc = 0;
        }
    }
} // end of wxWindowOS2::UnsubclassWin

//
// Make a Windows extended style from the given wxWindows window style
//
WXDWORD wxWindowOS2::MakeExtendedStyle(
  long                              lStyle
, bool                              bEliminateBorders
)
{
   //
   // Simply fill out with wxWindow extended styles.  We'll conjure
   // something up in OS2Create and all window redrawing pieces later
   //
    WXDWORD                         dwStyle = 0;

    if (lStyle & wxTRANSPARENT_WINDOW )
        dwStyle |= wxTRANSPARENT_WINDOW;

    if (!bEliminateBorders)
    {
        if (lStyle & wxSUNKEN_BORDER)
            dwStyle |= wxSUNKEN_BORDER;
        if (lStyle & wxDOUBLE_BORDER)
            dwStyle |= wxDOUBLE_BORDER;
        if (lStyle & wxRAISED_BORDER )
            dwStyle |= wxRAISED_BORDER;
        if (lStyle & wxSTATIC_BORDER)
            dwStyle |= wxSTATIC_BORDER;
    }
    return dwStyle;
} // end of wxWindowOS2::MakeExtendedStyle

//
// Determines whether simulated 3D effects or CTL3D should be used,
// applying a default border style if required, and returning an extended
// style to pass to OS2Create.
//
WXDWORD wxWindowOS2::Determine3DEffects(
  WXDWORD                           dwDefaultBorderStyle
, bool*                             pbWant3D
) const
{
    WXDWORD                         dwStyle = 0L;

    //
    // Native PM does not have any specialize 3D effects like WIN32 does,
    // so we have to try and invent them.
    //

    //
    // If matches certain criteria, then assume no 3D effects
    // unless specifically requested (dealt with in MakeExtendedStyle)
    //
    if (!GetParent()                    ||
        !IsKindOf(CLASSINFO(wxControl)) ||
        (m_windowStyle & wxNO_BORDER)
       )
    {
        *pbWant3D = FALSE;
        return MakeExtendedStyle(m_windowStyle, FALSE);
    }

    //
    // 1) App can specify global 3D effects
    //
    *pbWant3D = wxTheApp->GetAuto3D();

    //
    // 2) If the parent is being drawn with user colours, or simple border
    //    specified, switch effects off.
    //
    if (GetParent() &&
        (GetParent()->GetWindowStyleFlag() & wxUSER_COLOURS) ||
        (m_windowStyle & wxSIMPLE_BORDER)
       )
        *pbWant3D = FALSE;

    //
    // 3) Control can override this global setting by defining
    //    a border style, e.g. wxSUNKEN_BORDER
    //
    if ((m_windowStyle & wxDOUBLE_BORDER) ||
        (m_windowStyle & wxRAISED_BORDER) ||
        (m_windowStyle & wxSTATIC_BORDER) ||
        (m_windowStyle & wxSUNKEN_BORDER)
       )
        *pbWant3D = TRUE;

    dwStyle = MakeExtendedStyle( m_windowStyle
                                ,FALSE
                               );

    //
    // If we want 3D, but haven't specified a border here,
    // apply the default border style specified.
    //
    if (dwDefaultBorderStyle && (*pbWant3D) &&
        !((m_windowStyle & wxDOUBLE_BORDER) ||
          (m_windowStyle & wxRAISED_BORDER) ||
          (m_windowStyle & wxSTATIC_BORDER) ||
          (m_windowStyle & wxSIMPLE_BORDER)
         )
        )
        dwStyle |= dwDefaultBorderStyle;
    return dwStyle;
} // end of wxWindowOS2::Determine3DEffects

#if WXWIN_COMPATIBILITY
void wxWindowOS2::OnCommand(
  wxWindow&                         rWin
, wxCommandEvent&                   rEvent
)
{
    if (GetEventHandler()->ProcessEvent(rEvent))
        return;
    if (m_parent)
        m_parent->GetEventHandler()->OnCommand( rWin
                                               ,rEvent
                                              );
} // end of wxWindowOS2::OnCommand

wxObject* wxWindowOS2::GetChild(
  int                               nNumber
) const
{
    //
    // Return a pointer to the Nth object in the Panel
    //
    wxNode*                         pNode = GetChildren().First();
    int                             n = nNumber;

    while (pNode && n--)
        pNode = pNode->Next();
    if (pNode)
    {
        wxObject*                   pObj = (wxObject*)pNode->Data();
        return(pObj);
    }
    else
        return NULL;
} // end of wxWindowOS2::GetChild

#endif // WXWIN_COMPATIBILITY

//
// Setup background and foreground colours correctly
//
void wxWindowOS2::SetupColours()
{
    if ( GetParent() )
        SetBackgroundColour(GetParent()->GetBackgroundColour());
} // end of wxWindowOS2::SetupColours

void wxWindowOS2::OnIdle(
  wxIdleEvent&                      rEvent
)
{
    //
    // Check if we need to send a LEAVE event
    //
    if (m_bMouseInWindow)
    {
        POINTL                      vPoint;

        ::WinQueryPointerPos(HWND_DESKTOP, &vPoint);
        if (::WinWindowFromPoint(HWND_DESKTOP, &vPoint, FALSE) != (HWND)GetHwnd())
        {
            //
            // Generate a LEAVE event
            //
            m_bMouseInWindow = FALSE;

            //
            // Unfortunately the mouse button and keyboard state may have changed
            // by the time the OnIdle function is called, so 'state' may be
            // meaningless.
            //
            int                     nState = 0;

            if (::WinGetKeyState(HWND_DESKTOP, VK_SHIFT) != 0)
                nState |= VK_SHIFT;
            if (::WinGetKeyState(HWND_DESKTOP, VK_CTRL) != 0);
                nState |= VK_CTRL;

            wxMouseEvent            rEvent(wxEVT_LEAVE_WINDOW);

            InitMouseEvent( rEvent
                           ,vPoint.x
                           ,vPoint.y
                           ,nState
                          );
            (void)GetEventHandler()->ProcessEvent(rEvent);
        }
    }
    UpdateWindowUI();
} // end of wxWindowOS2::OnIdle

//
// Set this window to be the child of 'parent'.
//
bool wxWindowOS2::Reparent(
  wxWindow*                         pParent
)
{
    if (!wxWindowBase::Reparent(pParent))
        return FALSE;

    HWND                            hWndChild = GetHwnd();
    HWND                            hWndParent = GetParent() ? GetWinHwnd(GetParent()) : (HWND)0;

    ::WinSetParent(hWndChild, hWndParent, TRUE);
    return TRUE;
} // end of wxWindowOS2::Reparent

void wxWindowOS2::Clear()
{
    wxClientDC                      vDc(this);
    wxBrush                         vBrush( GetBackgroundColour()
                                           ,wxSOLID
                                          );

    vDc.SetBackground(vBrush);
    vDc.Clear();
} // end of wxWindowOS2::Clear

void wxWindowOS2::Refresh(
  bool                              bEraseBack
, const wxRect*                     pRect
)
{
    HWND                            hWnd = GetHwnd();

    if (hWnd)
    {
        if (pRect)
        {
            RECTL                   vOs2Rect;

            vOs2Rect.xLeft   = pRect->x;
            vOs2Rect.yTop    = pRect->y;
            vOs2Rect.xRight  = pRect->x + pRect->width;
            vOs2Rect.yBottom = pRect->y + pRect->height;

            ::WinInvalidateRect(hWnd, &vOs2Rect, bEraseBack);
        }
        else
            ::WinInvalidateRect(hWnd, NULL, bEraseBack);
    }
} // end of wxWindowOS2::Refresh

// ---------------------------------------------------------------------------
// drag and drop
// ---------------------------------------------------------------------------

#if wxUSE_DRAG_AND_DROP
void wxWindowOS2::SetDropTarget(
  wxDropTarget*                     pDropTarget
)
{
    if (m_dropTarget != 0)
    {
        m_dropTarget->Revoke(m_hWnd);
        delete m_dropTarget;
    }
    m_dropTarget = pDropTarget;
    if (m_dropTarget != 0)
        m_dropTarget->Register(m_hWnd);
} // end of wxWindowOS2::SetDropTarget
#endif

//
// old style file-manager drag&drop support: we retain the old-style
// DragAcceptFiles in parallel with SetDropTarget.
//
void wxWindowOS2::DragAcceptFiles(
  bool                              bAccept
)
{
    HWND                            hWnd = GetHwnd();

    if (hWnd && bAccept)
        ::DrgAcceptDroppedFiles(hWnd, NULL, NULL, DO_COPY, 0L);
} // end of wxWindowOS2::DragAcceptFiles

// ----------------------------------------------------------------------------
// tooltips
// ----------------------------------------------------------------------------

#if wxUSE_TOOLTIPS

void wxWindowOS2::DoSetToolTip(
  wxToolTip*                        pTooltip
)
{
    wxWindowBase::DoSetToolTip(pTooltip);

    if (m_tooltip)
        m_tooltip->SetWindow(this);
} // end of wxWindowOS2::DoSetToolTip

#endif // wxUSE_TOOLTIPS

// ---------------------------------------------------------------------------
// moving and resizing
// ---------------------------------------------------------------------------

// Get total size
void wxWindowOS2::DoGetSize(
  int*                              pWidth
, int*                              pHeight
) const
{
    HWND                            hWnd = GetHwnd();
    RECTL                           vRect;

    ::WinQueryWindowRect(hWnd, &vRect);

    if (pWidth)
        *pWidth = vRect.xRight - vRect.xLeft;
    if (pHeight )
        // OS/2 PM is backwards from windows
        *pHeight = vRect.yTop - vRect.yBottom;
} // end of wxWindowOS2::DoGetSize

void wxWindowOS2::DoGetPosition(
  int*                              pX
, int*                              pY
) const
{
    HWND                            hWnd = GetHwnd();
    RECT                            vRect;
    POINTL                          vPoint;

    ::WinQueryWindowRect(hWnd, &vRect);

    vPoint.x = vRect.xLeft;
    vPoint.y = vRect.yBottom;

    //
    // We do the adjustments with respect to the parent only for the "real"
    // children, not for the dialogs/frames
    //
    if (!IsTopLevel())
    {
        HWND                        hParentWnd = 0;
        wxWindow*                   pParent = GetParent();

        if (pParent)
            hParentWnd = GetWinHwnd(pParent);

        //
        // Since we now have the absolute screen coords, if there's a parent we
        // must subtract its bottom left corner
        //
        if (hParentWnd)
        {
            RECTL                   vRect2;

            ::WinQueryWindowRect(hParentWnd, &vRect2);
            vPoint.x -= vRect.xLeft;
            vPoint.y -= vRect.yBottom;
        }

        //
        // We may be faking the client origin. So a window that's really at (0,
        // 30) may appear (to wxWin apps) to be at (0, 0).
        //
        wxPoint                     vPt(pParent->GetClientAreaOrigin());

        vPoint.x -= vPt.x;
        vPoint.y -= vPt.y;
    }

    if (pX)
        *pX = vPoint.x;
    if  (pY)
        *pY = vPoint.y;
} // end of wxWindowOS2::DoGetPosition

void wxWindowOS2::DoScreenToClient(
  int*                              pX
, int*                              pY
) const
{
    HWND                            hWnd = GetHwnd();
    SWP                             vSwp;

    ::WinQueryWindowPos(hWnd, &vSwp);

    if (pX)
        *pX -= vSwp.x;
    if (pY)
        *pY -= vSwp.y;
} // end of wxWindowOS2::DoScreenToClient

void wxWindowOS2::DoClientToScreen(
  int*                              pX
, int*                              pY
) const
{
    HWND                            hWnd = GetHwnd();
    SWP                             vSwp;

    ::WinQueryWindowPos(hWnd, &vSwp);

    if (pX)
        *pX += vSwp.x;
    if (pY)
        *pY += vSwp.y;
} // end of wxWindowOS2::DoClientToScreen

//
// Get size *available for subwindows* i.e. excluding menu bar etc.
// Must be a frame type window
//
void wxWindowOS2::DoGetClientSize(
  int*                              pWidth
, int*                              pHeight
) const
{
    HWND                            hWnd = GetHwnd();
    HWND                            hWndClient;
    RECTL                           vRect;

    if (IsKindOf(CLASSINFO(wxFrame)))
        hWndClient = ::WinWindowFromID(GetHwnd(), FID_CLIENT);
    else
        hWndClient = NULLHANDLE;
    if( hWndClient == NULLHANDLE)
       ::WinQueryWindowRect(GetHwnd(), &vRect);
    else
       ::WinQueryWindowRect(hWndClient, &vRect);

    if (pWidth)
        *pWidth  = vRect.xRight;
    if (pHeight)
        *pHeight = vRect.yTop;
} // end of wxWindowOS2::DoGetClientSize

void wxWindowOS2::DoMoveWindow(
  int                               nX
, int                               nY
, int                               nWidth
, int                               nHeight
)
{
    RECTL                           vRect;
    HWND                            hParent;
    wxWindow*                       pParent = GetParent();

    if (pParent)
        hParent = GetWinHwnd(pParent);
    else
        hParent = HWND_DESKTOP;
    ::WinQueryWindowRect(hParent, &vRect);
    nY = vRect.yTop - (nY + nHeight);

    if ( !::WinSetWindowPos( GetHwnd()
                            ,HWND_TOP
                            ,(LONG)nX
                            ,(LONG)nY
                            ,(LONG)nWidth
                            ,(LONG)nHeight
                            ,SWP_SIZE | SWP_MOVE
                           ))
    {
        wxLogLastError("MoveWindow");
    }
} // end of wxWindowOS2::DoMoveWindow

//
// Set the size of the window: if the dimensions are positive, just use them,
// but if any of them is equal to -1, it means that we must find the value for
// it ourselves (unless sizeFlags contains wxSIZE_ALLOW_MINUS_ONE flag, in
// which case -1 is a valid value for x and y)
//
// If sizeFlags contains wxSIZE_AUTO_WIDTH/HEIGHT flags (default), we calculate
// the width/height to best suit our contents, otherwise we reuse the current
// width/height
//
void wxWindowOS2::DoSetSize(
  int                               nX
, int                               nY
, int                               nWidth
, int                               nHeight
, int                               nSizeFlags
)
{
    //
    // Get the current size and position...
    //
    int                             nCurrentX;
    int                             nCurrentY;
    int                             nCurrentWidth;
    int                             nCurrentHeight;
    wxSize                          vSize(-1, -1);

    GetPosition( &nCurrentX
                ,&nCurrentY
               );
    GetSize( &nCurrentWidth
            ,&nCurrentHeight
           );

    //
    // ... and don't do anything (avoiding flicker) if it's already ok
    //
    if ( nX == nCurrentX &&
         nY == nCurrentY &&
         nWidth == nCurrentWidth &&
         nHeight == nCurrentHeight
       )
    {
        return;
    }

    if (nX == -1 && !(nSizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        nX = nCurrentX;
    if (nY == -1 && !(nSizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        nY = nCurrentY;

    AdjustForParentClientOrigin( nX
                                ,nY
                                ,nSizeFlags
                               );

    if (nWidth == -1)
    {
        if (nSizeFlags & wxSIZE_AUTO_WIDTH)
        {
            vSize  = DoGetBestSize();
            nWidth = vSize.x;
        }
        else
        {
            //
            // Just take the current one
            //
            nWidth = nCurrentWidth;
        }
    }

    if (nHeight == -1)
    {
        if (nSizeFlags & wxSIZE_AUTO_HEIGHT)
        {
            if (vSize.x == -1)
            {
                vSize = DoGetBestSize();
            }
            nHeight = vSize.y;
        }
        else
        {
            // just take the current one
            nHeight = nCurrentHeight;
        }
    }

    DoMoveWindow( nX
                 ,nY
                 ,nWidth
                 ,nHeight
                );
} // end of wxWindowOS2::DoSetSize

void wxWindowOS2::DoSetClientSize(
  int                               nWidth
, int                               nHeight
)
{
    wxWindow*                       pParent = GetParent();
    HWND                            hWnd = GetHwnd();
    HWND                            hParentWnd = (HWND)0;
    HWND                            hClientWnd = (HWND)0;
    RECTL                           vRect;
    RECT                            vRect2;
    RECT                            vRect3;

    hClientWnd = ::WinWindowFromID(GetHwnd(), FID_CLIENT);
    ::WinQueryWindowRect(hClientWnd, &vRect2);

    if (pParent)
        hParentWnd = (HWND) pParent->GetHWND();

    ::WinQueryWindowRect(hWnd, &vRect);
    ::WinQueryWindowRect(hParentWnd, &vRect3);
    //
    // Find the difference between the entire window (title bar and all)
    // and the client area; add this to the new client size to move the
    // window. OS/2 is backward from windows on height
    //
    int                             nActualWidth = vRect2.xRight - vRect2.xLeft - vRect.xRight + nWidth;
    int                             nActualHeight = vRect2.yTop - vRect2.yBottom - vRect.yTop + nHeight;

    //
    // If there's a parent, must subtract the parent's bottom left corner
    // since MoveWindow moves relative to the parent
    //
    POINTL                          vPoint;

    vPoint.x = vRect2.xLeft;
    vPoint.y = vRect2.yBottom;
    if (pParent)
    {
        vPoint.x -= vRect3.xLeft;
        vPoint.y -= vRect3.yBottom;
    }

    DoMoveWindow( vPoint.x
                 ,vPoint.y
                 ,nActualWidth
                 ,nActualHeight
                );

    wxSizeEvent                     vEvent( wxSize( nWidth
                                                  ,nHeight
                                                 )
                                           ,m_windowId
                                          );

    vEvent.SetEventObject(this);
    GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::DoSetClientSize

wxPoint wxWindowOS2::GetClientAreaOrigin() const
{
    return wxPoint(0, 0);
} // end of wxWindowOS2::GetClientAreaOrigin

void wxWindowOS2::AdjustForParentClientOrigin(
  int&                              rX
, int&                              rY
, int                               nSizeFlags
)
{
    //
    // Don't do it for the dialogs/frames - they float independently of their
    // parent
    //
    if (!IsTopLevel())
    {
        wxWindow*                   pParent = GetParent();

        if (!(nSizeFlags & wxSIZE_NO_ADJUSTMENTS) && pParent)
        {
            wxPoint                 vPoint(pParent->GetClientAreaOrigin());
            rX += vPoint.x;
            rY += vPoint.y;
        }
    }
} // end of wxWindowOS2::AdjustForParentClientOrigin

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

int wxWindowOS2::GetCharHeight() const
{
    HPS                             hPs;
    FONTMETRICS                     vFontMetrics;
    BOOL                            bRc;

    hPs = ::WinGetPS(GetHwnd());

    if(!GpiQueryFontMetrics(hPs, sizeof(FONTMETRICS), &vFontMetrics))
    {
        ::WinReleasePS(hPs);
        return (0);
    }
    ::WinReleasePS(hPs);
    return(vFontMetrics.lMaxAscender + vFontMetrics.lMaxDescender);
} // end of wxWindowOS2::GetCharHeight

int wxWindowOS2::GetCharWidth() const
{
    HPS                             hPs;
    FONTMETRICS                     vFontMetrics;

    hPs = ::WinGetPS(GetHwnd());

    if(!GpiQueryFontMetrics(hPs, sizeof(FONTMETRICS), &vFontMetrics))
    {
        ::WinReleasePS(hPs);
        return (0);
    }
    ::WinReleasePS(hPs);
    return(vFontMetrics.lAveCharWidth);
} // end of wxWindowOS2::GetCharWidth

void wxWindowOS2::GetTextExtent(
  const wxString&                   rString
, int*                              pX
, int*                              pY
, int*                              pDescent
, int*                              pExternalLeading
, const wxFont*                     pTheFont
) const
{
    const wxFont*                   pFontToUse = pTheFont;
    HPS                             hPs;

    hPs = ::WinGetPS(GetHwnd());
/*
// TODO: Will have to play with fonts later

    if (!pFontToUse)
        pFontToUse = &m_font;

    HFONT                           hFnt = 0;
    HFONT                           hFfontOld = 0;

    if (pFontToUse && pFontToUse->Ok())
    {
        ::GpiCreateLog
        hFnt = (HFONT)((wxFont *)pFontToUse)->GetResourceHandle(); // const_cast
        if (hFnt)
            hFontOld = (HFONT)SelectObject(dc,fnt);
    }

    SIZE sizeRect;
    TEXTMETRIC tm;
    GetTextExtentPoint(dc, string, (int)string.Length(), &sizeRect);
    GetTextMetrics(dc, &tm);

    if ( fontToUse && fnt && hfontOld )
        SelectObject(dc, hfontOld);

    ReleaseDC(hWnd, dc);

    if ( x )
        *x = sizeRect.cx;
    if ( y )
        *y = sizeRect.cy;
    if ( descent )
        *descent = tm.tmDescent;
    if ( externalLeading )
        *externalLeading = tm.tmExternalLeading;
*/
    ::WinReleasePS(hPs);
}

#if wxUSE_CARET && WXWIN_COMPATIBILITY
// ---------------------------------------------------------------------------
// Caret manipulation
// ---------------------------------------------------------------------------

void wxWindowOS2::CreateCaret(
  int                               nWidth
, int                               nHeight
)
{
    SetCaret(new wxCaret( this
                         ,nWidth
                         ,nHeight
                        ));
} // end of wxWindowOS2::CreateCaret

void wxWindowOS2::CreateCaret(
  const wxBitmap*                   pBitmap
)
{
    wxFAIL_MSG("not implemented");
} // end of wxWindowOS2::CreateCaret

void wxWindowOS2::ShowCaret(
  bool                              bShow
)
{
    wxCHECK_RET( m_caret, "no caret to show" );

    m_caret->Show(bShow);
} // end of wxWindowOS2::ShowCaret

void wxWindowOS2::DestroyCaret()
{
    SetCaret(NULL);
} // end of wxWindowOS2::DestroyCaret

void wxWindowOS2::SetCaretPos(
  int                               nX
, int                               nY)
{
    wxCHECK_RET( m_caret, "no caret to move" );

    m_caret->Move( nX
                  ,nY
                 );
} // end of wxWindowOS2::SetCaretPos

void wxWindowOS2::GetCaretPos(
  int*                              pX
, int*                              pY
) const
{
    wxCHECK_RET( m_caret, "no caret to get position of" );

    m_caret->GetPosition( pX
                         ,pY
                        );
} // end of wxWindowOS2::GetCaretPos

#endif //wxUSE_CARET

// ---------------------------------------------------------------------------
// popup menu
// ---------------------------------------------------------------------------

static void wxYieldForCommandsOnly()
{
    //
    // Peek all WM_COMMANDs (it will always return WM_QUIT too but we don't
    // want to process it here)
    //
    QMSG                            vMsg;

    while (::WinPeekMsg( vHabmain
                        ,&vMsg
                        ,(HWND)0
                        ,WM_COMMAND
                        ,WM_COMMAND
                        ,PM_REMOVE
                       ) && vMsg.msg != WM_QUIT)
    {
        wxTheApp->DoMessage((WXMSG*)&vMsg);
    }
}

bool wxWindowOS2::DoPopupMenu(
  wxMenu*                           pMenu
, int                               nX
, int                               nY
)
{
    HWND                            hWnd = GetHwnd();
    HWND                            hWndParent = GetParent() ? GetWinHwnd(GetParent()) : (HWND)0;
    HWND                            hMenu = GetHmenuOf(pMenu);

    pMenu->SetInvokingWindow(this);
    pMenu->UpdateUI();

    DoClientToScreen( &nX
                     ,&nY
                    );
    wxCurrentPopupMenu = pMenu;

    ::WinPopupMenu( hWndParent
                   ,hWnd
                   ,hMenu
                   ,nX
                   ,nY
                   ,0L
                   ,PU_MOUSEBUTTON2DOWN | PU_MOUSEBUTTON2 | PU_KEYBOARD
                  );
    // we need to do it righ now as otherwise the events are never going to be
    // sent to wxCurrentPopupMenu from HandleCommand()
    //
    // note that even eliminating (ugly) wxCurrentPopupMenu global wouldn't
    // help and we'd still need wxYieldForCommandsOnly() as the menu may be
    // destroyed as soon as we return (it can be a local variable in the caller
    // for example) and so we do need to process the event immediately
    wxYieldForCommandsOnly();
    wxCurrentPopupMenu = NULL;

    pMenu->SetInvokingWindow(NULL);
    return TRUE;
} // end of wxWindowOS2::DoPopupMenu

// ===========================================================================
// pre/post message processing
// ===========================================================================

MRESULT wxWindowOS2::OS2DefWindowProc(
  WXUINT                            uMsg
, WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    if (m_fnOldWndProc)
        return (MRESULT)m_fnOldWndProc(GetHWND(), (ULONG)uMsg, (MPARAM)wParam, (MPARAM)lParam);
    else
        return ::WinDefWindowProc(GetHWND(), (ULONG)uMsg, (MPARAM)wParam, (MPARAM)lParam);
} // end of wxWindowOS2::OS2DefWindowProc

bool wxWindowOS2::OS2ProcessMessage(
  WXMSG*                            pMsg
)
{
    QMSG*                           pQMsg = (QMSG*)pMsg;

    if (m_hWnd != 0 && (GetWindowStyleFlag() & wxTAB_TRAVERSAL))
    {
        //
        // Intercept dialog navigation keys
        //
        bool                        bProcess = TRUE;
        USHORT                      uKeyFlags = SHORT1FROMMP(pQMsg->mp1);

        if (uKeyFlags & KC_KEYUP)
            bProcess = FALSE;

        if (uKeyFlags & KC_ALT)
            bProcess = FALSE;

        if (!(uKeyFlags & KC_VIRTUALKEY))
            bProcess = FALSE;

        if (bProcess)
        {
            bool                    bCtrlDown = IsCtrlDown();
            bool                    bShiftDown = IsShiftDown();

            //
            // WM_QUERYDLGCODE: ask the control if it wants the key for itself,
            // don't process it if it's the case (except for Ctrl-Tab/Enter
            // combinations which are always processed)
            //
            ULONG                   ulDlgCode = 0;

            if (!bCtrlDown)
            {
                ulDlgCode = (ULONG)::WinSendMsg(pQMsg->hwnd, WM_QUERYDLGCODE, pQMsg, 0);
            }

            bool                    bForward = TRUE;
            bool                    bWindowChange = FALSE;

            switch (SHORT2FROMMP(pQMsg->mp2))
            {
                //
                // Going to make certain assumptions about specific types of controls
                // here, so we may have to alter some things later if they prove invalid
                //
                case VK_TAB:
                    //
                    // Shift tabl will always be a nav-key but tabs may be wanted
                    //
                    if (!bShiftDown)
                    {
                        bProcess = FALSE;
                    }
                    else
                    {
                        //
                        // Entry Fields want tabs for themselve usually
                        //
                        switch (ulDlgCode)
                        {
                            case DLGC_ENTRYFIELD:
                            case DLGC_MLE:
                                bProcess = TRUE;
                                break;

                            default:
                                bProcess = FALSE;
                        }

                        //
                        // Ctrl-Tab cycles thru notebook pages
                        //
                        bWindowChange = bCtrlDown;
                        bForward = !bShiftDown;
                    }
                    break;

                case VK_UP:
                case VK_LEFT:
                    if (bCtrlDown)
                        bProcess = FALSE;
                    else
                        bForward = FALSE;
                    break;

                case VK_DOWN:
                case VK_RIGHT:
                    if (bCtrlDown)
                        bProcess = FALSE;
                    break;

                case VK_ENTER:
                    {
                        if (bCtrlDown)
                        {
                            //
                            // ctrl-enter is not processed
                            //
                            return FALSE;
                        }
                        else if (ulDlgCode & DLGC_BUTTON)
                        {
                            //
                            // buttons want process Enter themselevs
                            //
                            bProcess = FALSE;
                        }
                        else
                        {
                            wxPanel*    pPanel = wxDynamicCast(this, wxPanel);
                            wxButton*   pBtn = NULL;

                            if (pPanel)
                            {
                                //
                                // Panel may have a default button which should
                                // be activated by Enter
                                //
                                pBtn = pPanel->GetDefaultItem();
                            }

                            if (pBtn && pBtn->IsEnabled())
                            {
                                //
                                // If we do have a default button, do press it
                                //
                                pBtn->OS2Command(BN_CLICKED, 0 /* unused */);
                                return TRUE;
                            }
                            // else: but if it does not it makes sense to make
                            //       it work like a TAB - and that's what we do.
                            //       Note that Ctrl-Enter always works this way.
                        }
                    }
                    break;

                default:
                    bProcess = FALSE;
            }

            if (bProcess)
            {
                wxNavigationKeyEvent    vEvent;

                vEvent.SetDirection(bForward);
                vEvent.SetWindowChange(bWindowChange);
                vEvent.SetEventObject(this);

                if (GetEventHandler()->ProcessEvent(vEvent))
                {
                    wxButton*       pBtn = wxDynamicCast(FindFocus(), wxButton);

                    if (pBtn)
                    {
                        //
                        // The button which has focus should be default
                        //
                        pBtn->SetDefault();
                    }
                    return TRUE;
                }
            }
        }
        //
        // Let Dialogs process
        //
        if (::WinSendMsg(pQMsg->hwnd, WM_QUERYDLGCODE, pQMsg, 0));
            return TRUE;
    }

#if wxUSE_TOOLTIPS
    if ( m_tooltip )
    {
        // relay mouse move events to the tooltip control
        QMSG*                       pQMsg = (QMSG*)pMsg;

        if (pQMsg->msg == WM_MOUSEMOVE )
            m_tooltip->RelayEvent(pMsg);
    }
#endif // wxUSE_TOOLTIPS

    return FALSE;
} // end of wxWindowOS2::OS2ProcessMessage

bool wxWindowOS2::OS2TranslateMessage(
  WXMSG*                            pMsg
)
{
#if wxUSE_ACCEL
  return m_acceleratorTable.Translate(m_hWnd, pMsg);
#else
  return FALSE;
#endif //wxUSE_ACCEL
} // end of wxWindowOS2::OS2TranslateMessage

// ---------------------------------------------------------------------------
// message params unpackers
// ---------------------------------------------------------------------------

void wxWindowOS2::UnpackCommand(
  WXWPARAM                          wParam
, WXLPARAM                          lParam
, WORD*                             pId
, WXHWND*                           phWnd
, WORD*                             pCmd
)
{
    *pId = LOWORD(wParam);
    *phWnd = NULL;  // or may be GetHWND() ?
    *pCmd = LOWORD(lParam);
} // end of wxWindowOS2::UnpackCommand

void wxWindowOS2::UnpackActivate(
  WXWPARAM                          wParam
, WXLPARAM                          lParam
, WXWORD*                           pState
, WXHWND*                           phWnd
)
{
    *pState     = LOWORD(wParam);
    *phWnd      = (WXHWND)lParam;
} // end of wxWindowOS2::UnpackActivate

void wxWindowOS2::UnpackScroll(
  WXWPARAM                          wParam
, WXLPARAM                          lParam
, WXWORD*                           pCode
, WXWORD*                           pPos
, WXHWND*                           phWnd
)
{
    ULONG                           ulId;
    HWND                            hWnd;

    ulId    = (ULONG)LONGFROMMP(wParam);
    hWnd = ::WinWindowFromID(GetHwnd(), ulId);
    if (hWnd == m_hWndScrollBarHorz || hWnd == m_hWndScrollBarVert)
        *phWnd = NULLHANDLE;
    else
        *phWnd = hWnd;

    *pPos  = SHORT1FROMMP(lParam);
    *pCode = SHORT2FROMMP(lParam);
} // end of wxWindowOS2::UnpackScroll

void wxWindowOS2::UnpackMenuSelect(
  WXWPARAM                          wParam
, WXLPARAM                          lParam
, WXWORD*                           pItem
, WXWORD*                           pFlags
, WXHMENU*                          phMenu
)
{
    *pItem = (WXWORD)LOWORD(wParam);
    *pFlags = HIWORD(wParam);
    *phMenu = (WXHMENU)lParam;
} // end of wxWindowOS2::UnpackMenuSelect

// ---------------------------------------------------------------------------
// Main wxWindows window proc and the window proc for wxWindow
// ---------------------------------------------------------------------------

//
// Hook for new window just as it's being created, when the window isn't yet
// associated with the handle
//
wxWindow*                           wxWndHook = NULL;

//
// Main window proc
//
MRESULT EXPENTRY wxWndProc(
  HWND                              hWnd
, ULONG                             ulMsg
, MPARAM                            wParam
, MPARAM                            lParam
)
{
    //
    // Trace all ulMsgs - useful for the debugging
    //
#ifdef __WXDEBUG__
    wxLogTrace(wxTraceMessages, wxT("Processing %s(wParam=%8lx, lParam=%8lx)"),
               wxGetMessageName(ulMsg), wParam, lParam);
#endif // __WXDEBUG__

    wxWindow*                       pWnd = wxFindWinFromHandle((WXHWND)hWnd);

    //
    // When we get the first message for the HWND we just created, we associate
    // it with wxWindow stored in wxWndHook
    //
    if (!pWnd && wxWndHook)
    {
        wxAssociateWinWithHandle(hWnd, wxWndHook);
        pWnd = wxWndHook;
        wxWndHook = NULL;
        pWnd->SetHWND((WXHWND)hWnd);
    }

    MRESULT                         rc = (MRESULT)0;


    //
    // Stop right here if we don't have a valid handle in our wxWindow object.
    //
    if (pWnd && !pWnd->GetHWND())
    {
        pWnd->SetHWND((WXHWND) hWnd);
        rc = pWnd->OS2DefWindowProc(ulMsg, wParam, lParam );
        pWnd->SetHWND(0);
    }
    else
    {
        if (pWnd)
            rc = pWnd->OS2WindowProc(ulMsg, wParam, lParam);
        else
            rc = ::WinDefWindowProc(hWnd, ulMsg, wParam, lParam);
    }

    return rc;
} // end of wxWndProc

//
// We will add (or delete) messages we need to handle at this default
// level as we go
//
MRESULT wxWindowOS2::OS2WindowProc(
  WXUINT                            uMsg
, WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    //
    // Did we process the uMsg?
    //
    bool                            bProcessed = FALSE;
    bool                            bAllow;
    MRESULT                         mResult;
    WXHICON                         hIcon;
    WXHBRUSH                        hBrush;

    //
    // For most messages we should return 0 when we do process the message
    //
    mResult = (MRESULT)0;

    switch (uMsg)
    {
        case WM_CREATE:
            {
                bool                bMayCreate;

                bProcessed = HandleCreate( (WXLPCREATESTRUCT)lParam
                                          ,&bMayCreate
                                         );
                if (bProcessed)
                {
                    //
                    // Return 0 to bAllow window creation
                    //
                    mResult = (MRESULT)(bMayCreate ? 0 : -1);
                }
            }
            break;

        case WM_DESTROY:
             HandleDestroy();
             bProcessed = TRUE;
             break;

        case WM_MOVE:
            bProcessed = HandleMove( LOWORD(lParam)
                                    ,HIWORD(lParam)
                                   );
            break;

        case WM_SIZE:
            bProcessed = HandleSize( LOWORD(lParam)
                                    ,HIWORD(lParam)
                                    ,(WXUINT)wParam
                                   );
            break;

        case WM_ACTIVATE:
            {
                WXWORD              wState;
                WXHWND              hWnd;

                UnpackActivate( wParam
                               ,lParam
                               ,&wState
                               ,&hWnd
                              );

                bProcessed = HandleActivate( wState
                                            ,(WXHWND)hWnd
                                           );
                bProcessed = FALSE;
            }
            break;

        case WM_SETFOCUS:
            if (SHORT1FROMMP((MPARAM)lParam) == TRUE)
                bProcessed = HandleSetFocus((WXHWND)(HWND)wParam);
            else
                bProcessed = HandleKillFocus((WXHWND)(HWND)wParam);
            break;

        case WM_PAINT:
            bProcessed = HandlePaint();
            break;

        case WM_CLOSE:
            //
            // Don't let the DefWindowProc() destroy our window - we'll do it
            // ourselves in ~wxWindow
            //
            bProcessed = TRUE;
            mResult = (MRESULT)TRUE;
            break;

        case WM_SHOW:
            bProcessed = HandleShow(wParam != 0, (int)lParam);
            break;

        //
        // Under OS2 PM Joysticks are treated just like mouse events
        // The "Motion" events will be prevelent in joysticks
        //
        case WM_MOUSEMOVE:
        case WM_BUTTON1DOWN:
        case WM_BUTTON1UP:
        case WM_BUTTON1DBLCLK:
        case WM_BUTTON1MOTIONEND:
        case WM_BUTTON1MOTIONSTART:
        case WM_BUTTON2DOWN:
        case WM_BUTTON2UP:
        case WM_BUTTON2DBLCLK:
        case WM_BUTTON2MOTIONEND:
        case WM_BUTTON2MOTIONSTART:
        case WM_BUTTON3DOWN:
        case WM_BUTTON3UP:
        case WM_BUTTON3DBLCLK:
        case WM_BUTTON3MOTIONEND:
        case WM_BUTTON3MOTIONSTART:
            {
                short x = LOWORD(lParam);
                short y = HIWORD(lParam);

                bProcessed = HandleMouseEvent(uMsg, x, y, (WXUINT)wParam);
            }
            break;
        case WM_SYSCOMMAND:
            bProcessed = HandleSysCommand(wParam, lParam);
            break;

        case WM_COMMAND:
            {
                WORD id, cmd;
                WXHWND hwnd;
                UnpackCommand(wParam, lParam, &id, &hwnd, &cmd);

                bProcessed = HandleCommand(id, cmd, hwnd);
            }
            break;

            //
            // For these messages we must return TRUE if process the message
            //
        case WM_DRAWITEM:
        case WM_MEASUREITEM:
            {
                int                 nIdCtrl = (UINT)wParam;

                if ( uMsg == WM_DRAWITEM )
                {
                    bProcessed = OS2OnDrawItem(nIdCtrl,
                                              (WXDRAWITEMSTRUCT *)lParam);
                }
                else
                {
                    bProcessed = OS2OnMeasureItem(nIdCtrl,
                                                 (WXMEASUREITEMSTRUCT *)lParam);
                }

                if ( bProcessed )
                    mResult = (MRESULT)TRUE;
            }
            break;

        case WM_QUERYDLGCODE:
            if ( m_lDlgCode )
            {
                mResult = (MRESULT)m_lDlgCode;
                bProcessed = TRUE;
            }
            //
            //else: get the dlg code from the DefWindowProc()
            //
            break;

        //
        // In OS/2 PM all keyboard events are of the WM_CHAR type.  Virtual key and key-up
        // and key-down events are obtained from the WM_CHAR params.
        //
        case WM_CHAR:
            {
                USHORT                  uKeyFlags = SHORT1FROMMP((MPARAM)wParam);

                if (uKeyFlags & KC_KEYUP)
                {
                    bProcessed = HandleKeyUp((WXDWORD)wParam, lParam);
                    break;
                }
                else // keydown event
                {
                    //
                    // If this has been processed by an event handler,
                    // return 0 now (we've handled it). DON't RETURN
                    // we still need to process further
                    //
                    HandleKeyDown((WXDWORD)wParam, lParam);
                    if (uKeyFlags & KC_VIRTUALKEY)
                    {
                        USHORT          uVk = SHORT2FROMMP((MPARAM)lParam);

                        //
                        // We consider these message "not interesting" to OnChar
                        //
                        if (uVk == VK_SHIFT || uVk == VK_CTRL )
                        {
                            bProcessed = TRUE;
                            break;
                        }
                        switch(uVk)
                        {
                            //
                            // Avoid duplicate messages to OnChar for these ASCII keys: they
                            // will be translated by TranslateMessage() and received in WM_CHAR
                            case VK_ESC:
                            case VK_SPACE:
                            case VK_ENTER:
                            case VK_BACKSPACE:
                            case VK_TAB:
                                // But set processed to FALSE, not TRUE to still pass them to
                                // the control's default window proc - otherwise built-in
                                // keyboard handling won't work
                                bProcessed = FALSE;
                                break;

                            case VK_LEFT:
                            case VK_RIGHT:
                            case VK_DOWN:
                            case VK_UP:
                            default:
                                bProcessed = HandleChar((WXDWORD)wParam, lParam);
                         }
                         break;
                    }
                    else // WM_CHAR -- Always an ASCII character
                    {
                        bProcessed = HandleChar((WXDWORD)wParam, lParam, TRUE);
                        break;
                    }
                }
            }

        case WM_HSCROLL:
        case WM_VSCROLL:
            {
                WXWORD              wCode;
                WXWORD              wPos;
                WXHWND              hWnd;
                UnpackScroll( wParam
                             ,lParam
                             ,&wCode
                             ,&wPos
                             ,&hWnd
                            );

                bProcessed = OS2OnScroll( uMsg == WM_HSCROLL ? wxHORIZONTAL
                                                             : wxVERTICAL
                                         ,wCode
                                         ,wPos
                                         ,hWnd
                                        );
            }
            break;

#if defined(__VISAGECPP__) && (__IBMCPP__ >= 400)
        case WM_CTLCOLORCHANGE:
            {
                bProcessed = HandleCtlColor(&hBrush);
            }
            break;
#endif
        case WM_ERASEBACKGROUND:
            //
            // Returning TRUE to requestw PM to paint the window background
            // in SYSCLR_WINDOW. We don't really want that
            //
            bProcessed = HandleEraseBkgnd((WXHDC)(HPS)wParam);
            mResult = (MRESULT)(FALSE);
            break;

        //
        // Instead of CTLCOLOR messages PM sends QUERYWINDOWPARAMS to
        // things such as colors and fonts and such
        //
        case WM_QUERYWINDOWPARAMS:
            {
                PWNDPARAMS          pWndParams = (PWNDPARAMS)wParam;

                bProcessed = HandleWindowParams( pWndParams
                                                ,lParam
                                               );
            }
            break;

            // the return value for this message is ignored
        case WM_SYSCOLORCHANGE:
            bProcessed = HandleSysColorChange();
            break;

        case WM_REALIZEPALETTE:
            bProcessed = HandlePaletteChanged();
            break;

        case WM_PRESPARAMCHANGED:
            bProcessed = HandlePresParamChanged(wParam);
            break;


        // move all drag and drops to wxDrg
        case WM_ENDDRAG:
            bProcessed = HandleEndDrag(wParam);
            break;

        case WM_INITDLG:
            bProcessed = HandleInitDialog((WXHWND)(HWND)wParam);

            if ( bProcessed )
            {
                // we never set focus from here
                mResult = FALSE;
            }
            break;

        // wxFrame specific message
        case WM_MINMAXFRAME:
            bProcessed = HandleGetMinMaxInfo((PSWP)wParam);
            break;

        case WM_SYSVALUECHANGED:
            // TODO: do something
            mResult = (MRESULT)TRUE;
            break;

        //
        // Comparable to WM_SETPOINTER for windows, only for just controls
        //
        case WM_CONTROLPOINTER:
            bProcessed = HandleSetCursor( SHORT1FROMMP(wParam) // Control ID
                                         ,(HWND)lParam         // Cursor Handle
                                        );
            if (bProcessed )
            {
                //
                // Returning TRUE stops the DefWindowProc() from further
                // processing this message - exactly what we need because we've
                // just set the cursor.
                //
                mResult = (MRESULT)TRUE;
            }
            break;
    }
    if (!bProcessed)
    {
#ifdef __WXDEBUG__
        wxLogTrace(wxTraceMessages, wxT("Forwarding %s to DefWindowProc."),
                   wxGetMessageName(uMsg));
#endif // __WXDEBUG__
        if (IsKindOf(CLASSINFO(wxFrame)))
            mResult = ::WinDefWindowProc(m_hWnd, uMsg, wParam, lParam);
        else
            mResult = OS2DefWindowProc(uMsg, wParam, lParam);
    }
    return mResult;
} // end of wxWindowOS2::OS2WindowProc

//
// Dialog window proc
//
MRESULT wxDlgProc(
  HWND                              hWnd
, UINT                              uMsg
, MPARAM                            wParam
, MPARAM                            lParam)
{
    if (uMsg == WM_INITDLG)
    {
        //
        // For this message, returning TRUE tells system to set focus to the
        // first control in the dialog box
        //
        return (MRESULT)TRUE;
    }
    else
    {
        //
        // For all the other ones, FALSE means that we didn't process the
        // message
        //
        return (MRESULT)0;
    }
} // end of wxDlgProc

wxWindow* wxFindWinFromHandle(
  WXHWND                            hWnd
)
{
    wxNode*                         pNode = wxWinHandleList->Find((long)hWnd);

    if (!pNode)
        return NULL;
    return (wxWindow *)pNode->Data();
} // end of wxFindWinFromHandle

void wxAssociateWinWithHandle(
  HWND                              hWnd
, wxWindow*                         pWin
)
{
    //
    // Adding NULL hWnd is (first) surely a result of an error and
    // (secondly) breaks menu command processing
    //
    wxCHECK_RET( hWnd != (HWND)NULL,
                 wxT("attempt to add a NULL hWnd to window list ignored") );


    wxWindow*                       pOldWin = wxFindWinFromHandle((WXHWND) hWnd);

    if (pOldWin && (pOldWin != pWin))
    {
        wxString                    str(pWin->GetClassInfo()->GetClassName());
        wxLogError( "Bug! Found existing HWND %X for new window of class %s"
                   ,(int)hWnd
                   ,(const char*)str
                  );
    }
    else if (!pOldWin)
    {
        wxWinHandleList->Append( (long)hWnd
                                ,pWin
                               );
    }
} // end of wxAssociateWinWithHandle

void wxRemoveHandleAssociation(
  wxWindow*                         pWin
)
{
    wxWinHandleList->DeleteObject(pWin);
} // end of wxRemoveHandleAssociation

//
// Default destroyer - override if you destroy it in some other way
// (e.g. with MDI child windows)
//
void wxWindowOS2::OS2DestroyWindow()
{
}

void wxWindowOS2::OS2DetachWindowMenu()
{
    if (m_hMenu)
    {
        HMENU                       hMenu = (HMENU)m_hMenu;

        int                         nN = (int)::WinSendMsg(hMenu, MM_QUERYITEMCOUNT, 0, 0);
        int                         i;

        for (i = 0; i < nN; i++)
        {
            wxChar                   zBuf[100];
            int                      nChars = (int)::WinSendMsg( hMenu
                                                                ,MM_QUERYITEMTEXT
                                                                ,MPFROM2SHORT(i, nN)
                                                                ,zBuf
                                                               );
            if (!nChars)
            {
                wxLogLastError(wxT("GetMenuString"));
                continue;
            }

            if (wxStrcmp(zBuf, wxT("&Window")) == 0)
            {
                ::WinSendMsg(hMenu, MM_DELETEITEM, MPFROM2SHORT(i, TRUE), 0);
                break;
            }
        }
    }
} // end of wxWindowOS2::OS2DetachWindowMenu

bool wxWindowOS2::OS2Create(
  WXHWND                            hParent
, PSZ                               zClass
, const wxChar*                     zTitle
, WXDWORD                           dwStyle
, long                              lX
, long                              lY
, long                              lWidth
, long                              lHeight
, WXHWND                            hOwner
, WXHWND                            hZOrder
, unsigned long                     ulId
, void*                             pCtlData
, void*                             pPresParams
, WXDWORD                           dwExStyle
)
{
    ERRORID                         vError;
    wxString                        sError;
    long                            lX1      = 0L;
    long                            lY1      = 0L;
    long                            lWidth1  = 20L;
    long                            lHeight1 = 20L;
    int                             nControlId = 0;
    int                             nNeedsubclass = 0;
    PCSZ                            pszClass = zClass;

    //
    // Find parent's size, if it exists, to set up a possible default
    // panel size the size of the parent window
    //
    RECTL                           vParentRect;
    HWND                            hWndClient;

    lX1 = lX;
    lY1 = lY;
    if (lWidth > -1L)
        lWidth1 = lWidth;
    if (lHeight > -1L)
        lHeight1 = lHeight;

    wxWndHook = this;

    //
    // check to see if the new window is a standard control
    //
    if ((ULONG)zClass == (ULONG)WC_BUTTON ||
        (ULONG)zClass == (ULONG)WC_COMBOBOX ||
        (ULONG)zClass == (ULONG)WC_CONTAINER ||
        (ULONG)zClass == (ULONG)WC_ENTRYFIELD ||
        (ULONG)zClass == (ULONG)WC_FRAME ||
        (ULONG)zClass == (ULONG)WC_LISTBOX ||
        (ULONG)zClass == (ULONG)WC_MENU ||
        (ULONG)zClass == (ULONG)WC_NOTEBOOK ||
        (ULONG)zClass == (ULONG)WC_SCROLLBAR ||
        (ULONG)zClass == (ULONG)WC_SPINBUTTON ||
        (ULONG)zClass == (ULONG)WC_STATIC ||
        (ULONG)zClass == (ULONG)WC_TITLEBAR ||
        (ULONG)zClass == (ULONG)WC_VALUESET
       )
    {
            nControlId = ulId;
    }
    else
    {
        // no standard controls
        if(wxString (wxT("wxFrameClass")) == wxString(zClass) )
        {
            pszClass =  WC_FRAME;
            nNeedsubclass = 1;
        }
        else
        {
            nControlId = ulId;
            if(nControlId < 0)
                nControlId = FID_CLIENT;
        }
    }

    //
    // We will either have a registered class via string name or a standard PM Class via a long
    //
    m_hWnd = (WXHWND)::WinCreateWindow( (HWND)hParent
                                       ,zClass
                                       ,(PSZ)zTitle ? zTitle : wxT("")
                                       ,(ULONG)dwStyle
                                       ,(LONG)lX1
                                       ,(LONG)lY1
                                       ,(LONG)lWidth
                                       ,(LONG)lHeight
                                       ,hOwner
                                       ,HWND_TOP
                                       ,(ULONG)nControlId
                                       ,pCtlData
                                       ,pPresParams
                                      );
    if (!m_hWnd)
    {
        vError = ::WinGetLastError(vHabmain);
        sError = wxPMErrorToStr(vError);
        wxLogError("Can't create window of class %s!. Error: %s\n", zClass, sError);
        return FALSE;
    }
    m_dwExStyle = dwExStyle;
    ::WinSetWindowULong(m_hWnd, QWL_USER, (ULONG) this);
    wxWndHook = NULL;

#ifdef __WXDEBUG__
    wxNode*                         pNode = wxWinHandleList->Member(this);

    if (pNode)
    {
        HWND                        hWnd = (HWND)pNode->GetKeyInteger();

        if (hWnd != (HWND)m_hWnd)

        {
            wxLogError("A second HWND association is being added for the same window!");
        }
    }
#endif
    wxAssociateWinWithHandle((HWND)m_hWnd
                             ,this
                            );
    //
    // Now need to subclass window.
    //
    if(!nNeedsubclass)
    {
         wxAssociateWinWithHandle((HWND)m_hWnd,this);
    }
    else
    {
        SubclassWin(GetHWND());
    }
    return TRUE;
} // end of wxWindowOS2::OS2Create

// ===========================================================================
// OS2 PM message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
// window creation/destruction
// ---------------------------------------------------------------------------

bool wxWindowOS2::HandleCreate(
  WXLPCREATESTRUCT                  vCs
, bool*                             pbMayCreate
)
{
    wxWindowCreateEvent             vEvent(this);

    (void)GetEventHandler()->ProcessEvent(vEvent);
    *pbMayCreate = TRUE;
    return TRUE;
} // end of wxWindowOS2::HandleCreate

bool wxWindowOS2::HandleDestroy()
{
    wxWindowDestroyEvent            vEvent(this);

    (void)GetEventHandler()->ProcessEvent(vEvent);

    //
    // Delete our drop target if we've got one
    //
#if wxUSE_DRAG_AND_DROP
    if (m_dropTarget != NULL)
    {
        m_dropTarget->Revoke(m_hWnd);
        delete m_dropTarget;
        m_dropTarget = NULL;
    }
#endif // wxUSE_DRAG_AND_DROP

    //
    // WM_DESTROY handled
    //
    return TRUE;
} // end of wxWindowOS2::HandleDestroy

// ---------------------------------------------------------------------------
// activation/focus
// ---------------------------------------------------------------------------
void wxWindowOS2::OnSetFocus(
  wxFocusEvent&                     rEvent
)
{
    //
    // Panel wants to track the window which was the last to have focus in it,
    // so we want to set ourselves as the window which last had focus
    //
    // Notice that it's also important to do it upwards the tree becaus
    // otherwise when the top level panel gets focus, it won't set it back to
    // us, but to some other sibling
    //
    wxWindow*                       pWin = this;

    while (pWin)
    {
        wxWindow*                   pParent = pWin->GetParent();
        wxPanel*                    pPanel = wxDynamicCast( pParent
                                                           ,wxPanel
                                                          );
        if (pPanel)
        {
            pPanel->SetLastFocus(pWin);
        }
        pWin = pParent;
    }

    wxLogTrace(_T("focus"), _T("%s (0x%08x) gets focus"),
               GetClassInfo()->GetClassName(), GetHandle());

    rEvent.Skip();
} // end of wxWindowOS2::OnSetFocus

bool wxWindowOS2::HandleActivate(
  int                               nState
, WXHWND                            WXUNUSED(hActivate)
)
{
    wxActivateEvent                 vEvent( wxEVT_ACTIVATE
                                           ,(bool)nState
                                           ,m_windowId
                                          );
    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleActivate

bool wxWindowOS2::HandleSetFocus(
  WXHWND                            WXUNUSED(hWnd)
)
{
#if wxUSE_CARET
    //
    // Deal with caret
    //
    if (m_caret)
    {
        m_caret->OnSetFocus();
    }
#endif // wxUSE_CARET

    //
    // Panel wants to track the window which was the last to have focus in it
    //
    wxPanel*                        pPanel = wxDynamicCast( GetParent()
                                                           ,wxPanel
                                                          );
    if (pPanel)
    {
        pPanel->SetLastFocus(this);
    }

    wxFocusEvent                    vEvent(wxEVT_SET_FOCUS, m_windowId);

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleSetFocus

bool wxWindowOS2::HandleKillFocus(
  WXHWND                            WXUNUSED(hWnd)
)
{
#if wxUSE_CARET
    //
    // Deal with caret
    //
    if (m_caret)
    {
        m_caret->OnKillFocus();
    }
#endif // wxUSE_CARET

    wxFocusEvent                    vEvent( wxEVT_KILL_FOCUS
                                           ,m_windowId
                                          );

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleKillFocus

// ---------------------------------------------------------------------------
// miscellaneous
// ---------------------------------------------------------------------------

bool wxWindowOS2::HandleShow(
  bool                              bShow
, int                               nStatus
)
{
    wxShowEvent                     vEvent( GetId()
                                           ,bShow
                                          );

    vEvent.m_eventObject = this;
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleShow

bool wxWindowOS2::HandleInitDialog(
  WXHWND                            WXUNUSED(hWndFocus)
)
{
    wxInitDialogEvent               vEvent(GetId());

    vEvent.m_eventObject = this;
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleInitDialog

bool wxWindowOS2::HandleEndDrag(WXWPARAM wParam)
{
   // TODO: We'll handle drag and drop later
    return FALSE;
}

bool wxWindowOS2::HandleSetCursor(
  USHORT                            vId
, WXHWND                            hPointer
)
{
    //
    // Under OS/2 PM this allows the pointer to be changed
    // as it passes over a control
    //
    ::WinSetPointer(HWND_DESKTOP, (HPOINTER)hPointer);
    return TRUE;
} // end of wxWindowOS2::HandleSetCursor

// ---------------------------------------------------------------------------
// owner drawn stuff
// ---------------------------------------------------------------------------
bool wxWindowOS2::OS2OnDrawItem(
  int                               vId
, WXDRAWITEMSTRUCT*                 pItemStruct
)
{
    wxDC                            vDc;

#if wxUSE_OWNER_DRAWN
    //
    // Is it a menu item?
    //
    if (vId == 0)
    {
        ERRORID                     vError;
        wxString                    sError;
        POWNERITEM                  pMeasureStruct = (POWNERITEM)pItemStruct;
        wxFrame*                    pFrame = (wxFrame*)this;
        wxMenuItem*                 pMenuItem = pFrame->GetMenuBar()->FindItem(pMeasureStruct->idItem, pMeasureStruct->hItem);
        HDC                         hDC = ::GpiQueryDevice(pMeasureStruct->hps);
        wxRect                      vRect( pMeasureStruct->rclItem.xLeft
                                          ,pMeasureStruct->rclItem.yBottom
                                          ,pMeasureStruct->rclItem.xRight - pMeasureStruct->rclItem.xLeft
                                          ,pMeasureStruct->rclItem.yTop - pMeasureStruct->rclItem.yBottom
                                         );
        vDc.SetHDC( hDC
                   ,FALSE
                  );
        vDc.SetHPS(pMeasureStruct->hps);
        //
        // Load the wxWindows Pallete and set to RGB mode
        //
        if (!::GpiCreateLogColorTable( pMeasureStruct->hps
                                      ,0L
                                      ,LCOLF_CONSECRGB
                                      ,0L
                                      ,(LONG)wxTheColourDatabase->m_nSize
                                      ,(PLONG)wxTheColourDatabase->m_palTable
                                     ))
        {
            vError = ::WinGetLastError(vHabmain);
            sError = wxPMErrorToStr(vError);
            wxLogError("Unable to set current color table. Error: %s\n", sError);
        }
        //
        // Set the color table to RGB mode
        //
        if (!::GpiCreateLogColorTable( pMeasureStruct->hps
                                      ,0L
                                      ,LCOLF_RGB
                                      ,0L
                                      ,0L
                                      ,NULL
                                     ))
        {
            vError = ::WinGetLastError(vHabmain);
            sError = wxPMErrorToStr(vError);
            wxLogError("Unable to set current color table. Error: %s\n", sError);
        }

        wxCHECK( pMenuItem->IsKindOf(CLASSINFO(wxMenuItem)), FALSE );


        int                         eAction = 0;
        int                         eStatus = 0;

        if (pMeasureStruct->fsAttribute == pMeasureStruct->fsAttributeOld)
        {
            //
            // Entire Item needs to be redrawn (either it has reappeared from
            // behind another window or is being displayed for the first time
            //
            eAction = wxOwnerDrawn::wxODDrawAll;

            if (pMeasureStruct->fsAttribute & MIA_HILITED)
            {
                //
                // If it is currently selected we let the system handle it
                //
                eStatus |= wxOwnerDrawn::wxODSelected;
            }
            if (pMeasureStruct->fsAttribute & MIA_CHECKED)
            {
                //
                // If it is currently checked we draw our own
                //
                eStatus |= wxOwnerDrawn::wxODChecked;
                pMeasureStruct->fsAttributeOld = pMeasureStruct->fsAttribute &= ~MIA_CHECKED;
            }
            if (pMeasureStruct->fsAttribute & MIA_DISABLED)
            {
                //
                // If it is currently disabled we let the system handle it
                //
                eStatus |= wxOwnerDrawn::wxODDisabled;
            }
            //
            // Don't really care about framed (indicationg focus) or NoDismiss
            //
        }
        else
        {
            if (pMeasureStruct->fsAttribute & MIA_HILITED)
            {
                eAction = wxOwnerDrawn::wxODDrawAll;
                eStatus |= wxOwnerDrawn::wxODSelected;
                //
                // Keep the system from trying to highlight with its bogus colors
                //
                pMeasureStruct->fsAttributeOld = pMeasureStruct->fsAttribute &= ~MIA_HILITED;
            }
            else if (!(pMeasureStruct->fsAttribute & MIA_HILITED))
            {
                eAction = wxOwnerDrawn::wxODDrawAll;
                eStatus = 0;
                //
                // Keep the system from trying to highlight with its bogus colors
                //
                pMeasureStruct->fsAttribute = pMeasureStruct->fsAttributeOld &= ~MIA_HILITED;
            }
            else
            {
                //
                // For now we don't care about anything else
                // just ignore the entire message!
                //
                return TRUE;
            }
        }
        //
        // Now redraw the item
        //
        return(pMenuItem->OnDrawItem( vDc
                                     ,vRect
                                     ,(wxOwnerDrawn::wxODAction)eAction
                                     ,(wxOwnerDrawn::wxODStatus)eStatus
                                    ));
        //
        // leave the fsAttribute and fsOldAttribute unchanged.  If different,
        // the system will do the highlight or fraeming or disabling for us,
        // otherwise, we'd have to do it ourselves.
        //
    }

    wxWindow*                       pItem = FindItem(vId);

    if (pItem && pItem->IsKindOf(CLASSINFO(wxControl)))
    {
        return ((wxControl *)pItem)->OS2OnDraw(pItemStruct);
    }
#endif
    return FALSE;
} // end of wxWindowOS2::OS2OnDrawItem

bool wxWindowOS2::OS2OnMeasureItem(
  int                               lId
, WXMEASUREITEMSTRUCT*              pItemStruct
)
{
    //
    // Is it a menu item?
    //
    if (lId == 65536) // I really don't like this...has to be a better indicator
    {
        if (IsKindOf(CLASSINFO(wxFrame))) // we'll assume if Frame then a menu
        {
            size_t                  nWidth;
            size_t                  nHeight;
            POWNERITEM              pMeasureStruct = (POWNERITEM)pItemStruct;
            wxFrame*                pFrame = (wxFrame*)this;
            wxMenuItem*             pMenuItem = pFrame->GetMenuBar()->FindItem(pMeasureStruct->idItem, pMeasureStruct->hItem);

            wxCHECK( pMenuItem->IsKindOf(CLASSINFO(wxMenuItem)), FALSE );
            nWidth  = 0L;
            nHeight = 0L;
            if (pMenuItem->OnMeasureItem( &nWidth
                                         ,&nHeight
                                        ))
            {
                pMeasureStruct->rclItem.xRight  = nWidth;
                pMeasureStruct->rclItem.xLeft   = 0L;
                pMeasureStruct->rclItem.yTop    = nHeight;
                pMeasureStruct->rclItem.yBottom = 0L;
                return TRUE;
            }
            return FALSE;
        }
    }
    wxWindow*                      pItem = FindItem(lId);

    if (pItem && pItem->IsKindOf(CLASSINFO(wxControl)))
    {
        return ((wxControl *)pItem)->OS2OnMeasure(pItemStruct);
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// colours and palettes
// ---------------------------------------------------------------------------

bool wxWindowOS2::HandleSysColorChange()
{
    wxSysColourChangedEvent         vEvent;

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleSysColorChange

bool wxWindowOS2::HandleCtlColor(
  WXHBRUSH*                         phBrush
)
{
    //
    // Not much provided with message. So not sure I can do anything with it
    //
    return TRUE;
} // end of wxWindowOS2::HandleCtlColor

bool wxWindowOS2::HandleWindowParams(
  PWNDPARAMS                        pWndParams
, WXLPARAM                          lParam
)
{
// TODO: I'll do something here, just not sure what yet
    return TRUE;
}

// Define for each class of dialog and control
WXHBRUSH wxWindowOS2::OnCtlColor(WXHDC hDC,
                              WXHWND hWnd,
                              WXUINT nCtlColor,
                              WXUINT message,
                              WXWPARAM wParam,
                              WXLPARAM lParam)
{
    return (WXHBRUSH)0;
}

bool wxWindowOS2::HandlePaletteChanged()
{
    // need to set this to something first
    WXHWND                          hWndPalChange = NULLHANDLE;

    wxPaletteChangedEvent           vEvent(GetId());

    vEvent.SetEventObject(this);
    vEvent.SetChangedWindow(wxFindWinFromHandle(hWndPalChange));

    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandlePaletteChanged

bool wxWindowOS2::HandlePresParamChanged(
  WXWPARAM                          wParam
)
{
    //
    // TODO:  Once again I'll do something here when I need it
    //
    //wxQueryNewPaletteEvent event(GetId());
    //event.SetEventObject(this);
    // if the background is erased
//            bProcessed = HandleEraseBkgnd((WXHDC)(HDC)wParam);

    return FALSE; //GetEventHandler()->ProcessEvent(event) && event.GetPaletteRealized();
}

//
// Responds to colour changes: passes event on to children.
//
void wxWindowOS2::OnSysColourChanged(
  wxSysColourChangedEvent&          rEvent
)
{
    wxNode*                         pNode = GetChildren().First();

    while (pNode)
    {
        //
        // Only propagate to non-top-level windows
        //
        wxWindow*                   pWin = (wxWindow *)pNode->Data();

        if (pWin->GetParent())
        {
            wxSysColourChangedEvent vEvent;

            rEvent.m_eventObject = pWin;
            pWin->GetEventHandler()->ProcessEvent(vEvent);
        }
        pNode = pNode->Next();
    }
} // end of wxWindowOS2::OnSysColourChanged

// ---------------------------------------------------------------------------
// painting
// ---------------------------------------------------------------------------

bool wxWindowOS2::HandlePaint()
{
    HRGN                            hRgn = NULLHANDLE;
    wxPaintEvent                    vEvent;
    HPS                             hPS;
    RECTL                           vRect;

    if (::WinQueryUpdateRegion(GetHwnd(), hRgn) == RGN_NULL)
    {
         wxLogLastError("CreateRectRgn");
         return FALSE;
    }

    m_updateRegion = wxRegion(hRgn);
    vEvent.SetEventObject(this);
    if (!GetEventHandler()->ProcessEvent(vEvent))
    {
        HPS                         hPS;

        hPS = ::WinBeginPaint( GetHwnd()
                              ,NULLHANDLE
                              ,&vRect
                             );
        if(hPS)
        {
            ::GpiCreateLogColorTable( hPS
                                     ,0L
                                     ,LCOLF_CONSECRGB
                                     ,0L
                                     ,(LONG)wxTheColourDatabase->m_nSize
                                     ,(PLONG)wxTheColourDatabase->m_palTable
                                    );
            ::GpiCreateLogColorTable( hPS
                                     ,0L
                                     ,LCOLF_RGB
                                     ,0L
                                     ,0L
                                     ,NULL
                                    );

            ::WinFillRect(hPS, &vRect,  GetBackgroundColour().GetPixel());

            if (m_dwExStyle)
            {
                LINEBUNDLE                      vLineBundle;

                vLineBundle.lColor     = 0x00000000; // Black
                vLineBundle.usMixMode  = FM_OVERPAINT;
                vLineBundle.fxWidth    = 1;
                vLineBundle.lGeomWidth = 1;
                vLineBundle.usType     = LINETYPE_SOLID;
                vLineBundle.usEnd      = 0;
                vLineBundle.usJoin     = 0;
                ::GpiSetAttrs( hPS
                              ,PRIM_LINE
                              ,LBB_COLOR | LBB_MIX_MODE | LBB_WIDTH | LBB_GEOM_WIDTH | LBB_TYPE
                              ,0L
                              ,&vLineBundle
                             );
                ::WinQueryWindowRect(GetHwnd(), &vRect);
                wxDrawBorder( hPS
                             ,vRect
                             ,m_dwExStyle
                            );
            }
            ::WinEndPaint(hPS);
        }
    }
    return (GetEventHandler()->ProcessEvent(vEvent));
} // end of wxWindowOS2::HandlePaint

bool wxWindowOS2::HandleEraseBkgnd(
  WXHDC                             hDC
)
{
    SWP                             vSwp;

    ::WinQueryWindowPos(GetHwnd(), &vSwp);
    if (vSwp.fl & SWP_MINIMIZE)
        return TRUE;

    wxDC                            vDC;

    vDC.m_hPS = (HPS)hDC; // this is really a PS
    vDC.SetWindow(this);
    vDC.BeginDrawing();

    wxEraseEvent                    vEvent(m_windowId, &vDC);

    vEvent.SetEventObject(this);

    bool                            rc = GetEventHandler()->ProcessEvent(vEvent);

    vDC.EndDrawing();
    vDC.m_hPS = NULLHANDLE;
    return TRUE;
} // end of wxWindowOS2::HandleEraseBkgnd

void wxWindowOS2::OnEraseBackground(
  wxEraseEvent&                     rEvent
)
{
    RECTL                           vRect;
    HPS                             hPS = rEvent.m_dc->m_hPS;

    ::WinQueryWindowRect(GetHwnd(), &vRect);
    ::WinFillRect(hPS, &vRect,  m_backgroundColour.GetPixel());
}  // end of wxWindowOS2::OnEraseBackground

// ---------------------------------------------------------------------------
// moving and resizing
// ---------------------------------------------------------------------------

bool wxWindowOS2::HandleMinimize()
{
    wxIconizeEvent                  vEvent(m_windowId);

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleMinimize

bool wxWindowOS2::HandleMaximize()
{
    wxMaximizeEvent                 vEvent(m_windowId);

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleMaximize

bool wxWindowOS2::HandleMove(
  int                               nX
, int                               nY
)
{
    wxMoveEvent                     vEvent( wxPoint( nX
                                                    ,nY
                                                   )
                                           ,m_windowId
                                          );

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
}  // end of wxWindowOS2::HandleMove

bool wxWindowOS2::HandleSize(
  int                               nWidth
, int                               nHeight
, WXUINT                            WXUNUSED(nFlag)
)
{
    wxSizeEvent                     vEvent( wxSize( nWidth
                                                   ,nHeight
                                                  )
                                           ,m_windowId
                                          );

    vEvent.SetEventObject(this);
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleSize

bool wxWindowOS2::HandleGetMinMaxInfo(
  PSWP                              pSwp
)
{
    bool                            bRc = FALSE;
    POINTL                          vPoint;

    switch(pSwp->fl)
    {
        case SWP_MAXIMIZE:
            ::WinGetMaxPosition(GetHwnd(), pSwp);
            m_maxWidth = pSwp->cx;
            m_maxHeight = pSwp->cy;
            break;

        case SWP_MINIMIZE:
            ::WinGetMinPosition(GetHwnd(), pSwp, &vPoint);
            m_minWidth = pSwp->cx;
            m_minHeight = pSwp->cy;
            break;

        default:
            return FALSE;
    }
    return TRUE;
} // end of wxWindowOS2::HandleGetMinMaxInfo

// ---------------------------------------------------------------------------
// command messages
// ---------------------------------------------------------------------------
bool wxWindowOS2::HandleCommand(
  WXWORD                            wId
, WXWORD                            wCmd
, WXHWND                            hControl
)
{
    if (wxCurrentPopupMenu)
    {
        wxMenu*                     pPopupMenu = wxCurrentPopupMenu;

        wxCurrentPopupMenu = NULL;
        return pPopupMenu->OS2Command(wCmd, wId);
    }

    wxWindow*                       pWin = FindItem(wId);

    if (!pWin)
    {
        pWin = wxFindWinFromHandle(hControl);
    }

    if (pWin)
        return pWin->OS2Command( wCmd
                                ,wId
                               );
    return FALSE;
} // end of wxWindowOS2::HandleCommand

bool wxWindowOS2::HandleSysCommand(
  WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    //
    // 4 bits are reserved
    //
    switch (SHORT1FROMMP(wParam))
    {
        case SC_MAXIMIZE:
            return HandleMaximize();

        case SC_MINIMIZE:
            return HandleMinimize();
    }
    return FALSE;
} // end of wxWindowOS2::HandleSysCommand

// ---------------------------------------------------------------------------
// mouse events
// ---------------------------------------------------------------------------

void wxWindowOS2::InitMouseEvent(
  wxMouseEvent&                     rEvent
, int                               nX
, int                               nY
, WXUINT                            uFlags
)
{
    rEvent.m_x           = nX;
    rEvent.m_y           = nY;
    rEvent.m_shiftDown   = ((uFlags & VK_SHIFT) != 0);
    rEvent.m_controlDown = ((uFlags & VK_CTRL) != 0);
    rEvent.m_leftDown    = ((uFlags & VK_BUTTON1) != 0);
    rEvent.m_middleDown  = ((uFlags & VK_BUTTON3) != 0);
    rEvent.m_rightDown   = ((uFlags & VK_BUTTON2) != 0);
    rEvent.SetTimestamp(s_currentMsg.time);
    rEvent.m_eventObject = this;

#if wxUSE_MOUSEEVENT_HACK
    m_lastMouseX = nX;
    m_lastMouseY = nY;
    m_lastMouseEvent = rEvent.GetEventType();
#endif // wxUSE_MOUSEEVENT_HACK
} // end of wxWindowOS2::InitMouseEvent

bool wxWindowOS2::HandleMouseEvent(
  WXUINT                            uMsg
, int                               nX
, int                               nY
, WXUINT                            uFlags
)
{
    //
    // The mouse events take consecutive IDs from WM_MOUSEFIRST to
    // WM_MOUSELAST, so it's enough to substract WM_MOUSEMOVE == WM_MOUSEFIRST
    // from the message id and take the value in the table to get wxWin event
    // id
    //
    static const wxEventType eventsMouse[] =
    {
        wxEVT_MOTION,
        wxEVT_LEFT_DOWN,
        wxEVT_LEFT_UP,
        wxEVT_LEFT_DCLICK,
        wxEVT_RIGHT_DOWN,
        wxEVT_RIGHT_UP,
        wxEVT_RIGHT_DCLICK,
        wxEVT_MIDDLE_DOWN,
        wxEVT_MIDDLE_UP,
        wxEVT_MIDDLE_DCLICK
    };

    wxMouseEvent                    vEvent(eventsMouse[uMsg - WM_MOUSEMOVE]);

    InitMouseEvent( vEvent
                   ,nX
                   ,nY
                   ,uFlags
                  );

    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::HandleMouseEvent

bool wxWindowOS2::HandleMouseMove(
  int                               nX
, int                               nY
, WXUINT                            uFlags
)
{
    if (!m_bMouseInWindow)
    {
        //
        // Generate an ENTER event
        //
        m_bMouseInWindow = TRUE;

        wxMouseEvent                vEvent(wxEVT_ENTER_WINDOW);

        InitMouseEvent( vEvent
                       ,nX
                       ,nY
                       ,uFlags
                      );

        (void)GetEventHandler()->ProcessEvent(vEvent);
    }
    return HandleMouseEvent( WM_MOUSEMOVE
                            ,nX
                            ,nY
                            ,uFlags
                           );
} // end of wxWindowOS2::HandleMouseMove

// ---------------------------------------------------------------------------
// keyboard handling
// ---------------------------------------------------------------------------

//
// Create the key event of the given type for the given key - used by
// HandleChar and HandleKeyDown/Up
//
wxKeyEvent wxWindowOS2::CreateKeyEvent(
  wxEventType                       eType
, int                               nId
, WXLPARAM                          lParam
) const
{
    wxKeyEvent                      vEvent(eType);

    vEvent.SetId(GetId());
    vEvent.m_shiftDown   = IsShiftDown();
    vEvent.m_controlDown = IsCtrlDown();
    vEvent.m_altDown     = (HIWORD(lParam) & KC_ALT) == KC_ALT;

    vEvent.m_eventObject = (wxWindow *)this; // const_cast
    vEvent.m_keyCode     = nId;
    vEvent.SetTimestamp(s_currentMsg.time);

    //
    // Translate the position to client coords
    //
    POINTL                          vPoint;
    RECTL                           vRect;

    ::WinQueryPointerPos(HWND_DESKTOP, &vPoint);
    ::WinQueryWindowRect( GetHwnd()
                         ,&vRect
                        );

    vPoint.x -= vRect.xLeft;
    vPoint.y -= vRect.yBottom;

    vEvent.m_x = vPoint.x;
    vEvent.m_y = vPoint.y;

    return vEvent;
} // end of wxWindowOS2::CreateKeyEvent

//
// isASCII is TRUE only when we're called from WM_CHAR handler and not from
// WM_KEYDOWN one
//
bool wxWindowOS2::HandleChar(
  WXWORD                            wParam
, WXLPARAM                          lParam
, bool                              isASCII
)
{
    bool                            bCtrlDown = FALSE;
    int                             vId;

    if (isASCII)
    {
        //
        // If 1 -> 26, translate to CTRL plus a letter.
        //
        vId = wParam;
        if ((vId > 0) && (vId < 27))
        {
            switch (vId)
            {
                case 13:
                    vId = WXK_RETURN;
                    break;

                case 8:
                    vId = WXK_BACK;
                    break;

                case 9:
                    vId = WXK_TAB;
                    break;

                default:
                    bCtrlDown = TRUE;
                    vId = vId + 96;
            }
        }
    }
    else if ( (vId = wxCharCodeOS2ToWX(wParam)) == 0)
    {
        //
        // It's ASCII and will be processed here only when called from
        // WM_CHAR (i.e. when isASCII = TRUE), don't process it now
        //
        vId = -1;
    }

    if (vId != -1)
    {
        wxKeyEvent                  vEvent(CreateKeyEvent( wxEVT_CHAR
                                                          ,vId
                                                          ,lParam
                                                         ));

        if (bCtrlDown)
        {
            vEvent.m_controlDown = TRUE;
        }

        if (GetEventHandler()->ProcessEvent(vEvent))
            return TRUE;
    }
    return FALSE;
}

bool wxWindowOS2::HandleKeyDown(
  WXWORD                            wParam
, WXLPARAM                          lParam
)
{
    int                             nId = wxCharCodeOS2ToWX(wParam);

    if (!nId)
    {
        //
        // Normal ASCII char
        //
        nId = wParam;
    }

    if (nId != -1)
    {
        wxKeyEvent                  vEvent(CreateKeyEvent( wxEVT_KEY_DOWN
                                                          ,nId
                                                          ,lParam
                                                         ));

        if (GetEventHandler()->ProcessEvent(vEvent))
        {
            return TRUE;
        }
    }
    return FALSE;
} // end of wxWindowOS2::HandleKeyDown

bool wxWindowOS2::HandleKeyUp(
  WXWORD                            wParam
, WXLPARAM                          lParam
)
{
    int                             nId = wxCharCodeOS2ToWX(wParam);

    if (!nId)
    {
        //
        // Normal ASCII char
        //
        nId = wParam;
    }

    if (nId != -1)
    {
        wxKeyEvent                  vEvent(CreateKeyEvent( wxEVT_KEY_UP
                                                          ,nId
                                                          ,lParam
                                                         ));

        if (GetEventHandler()->ProcessEvent(vEvent))
            return TRUE;
    }
    return FALSE;
} // end of wxWindowOS2::HandleKeyUp

// ---------------------------------------------------------------------------
// joystick
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// scrolling
// ---------------------------------------------------------------------------

bool wxWindowOS2::OS2OnScroll(
  int                               nOrientation
, WXWORD                            wParam
, WXWORD                            wPos
, WXHWND                            hControl
)
{
    if (hControl)
    {
        wxWindow*                   pChild = wxFindWinFromHandle(hControl);

        if (pChild )
            return pChild->OS2OnScroll( nOrientation
                                       ,wParam
                                       ,wPos
                                       ,hControl
                                      );
    }

    wxScrollWinEvent                vEvent;

    vEvent.SetPosition(wPos);
    vEvent.SetOrientation(nOrientation);
    vEvent.m_eventObject = this;

    switch (wParam)
    {
        case SB_LINEUP:
            vEvent.m_eventType = wxEVT_SCROLLWIN_LINEUP;
            break;

        case SB_LINEDOWN:
            vEvent.m_eventType = wxEVT_SCROLLWIN_LINEDOWN;
            break;

        case SB_PAGEUP:
            vEvent.m_eventType = wxEVT_SCROLLWIN_PAGEUP;
            break;

        case SB_PAGEDOWN:
            vEvent.m_eventType = wxEVT_SCROLLWIN_PAGEDOWN;
            break;

        case SB_SLIDERPOSITION:
            vEvent.m_eventType = wxEVT_SCROLLWIN_THUMBRELEASE;
            break;

        case SB_SLIDERTRACK:
            vEvent.m_eventType = wxEVT_SCROLLWIN_THUMBTRACK;
            break;

        default:
            return FALSE;
    }
    return GetEventHandler()->ProcessEvent(vEvent);
} // end of wxWindowOS2::OS2OnScroll

// ===========================================================================
// global functions
// ===========================================================================

void wxGetCharSize(
  WXHWND                            hWnd
, int*                              pX
, int*                              pY
,wxFont*                            pTheFont
)
{
  // TODO: we'll do this later
} // end of wxGetCharSize

//
// Returns 0 if was a normal ASCII value, not a special key. This indicates that
// the key should be ignored by WM_KEYDOWN and processed by WM_CHAR instead.
//
int wxCharCodeOS2ToWX(
  int                               nKeySym
)
{
    int                             nId = 0;

    switch (nKeySym)
    {
        case VK_BACKTAB:    nId = WXK_BACK; break;
        case VK_TAB:        nId = WXK_TAB; break;
        case VK_CLEAR:      nId = WXK_CLEAR; break;
        case VK_ENTER:      nId = WXK_RETURN; break;
        case VK_SHIFT:      nId = WXK_SHIFT; break;
        case VK_CTRL:       nId = WXK_CONTROL; break;
        case VK_PAUSE:      nId = WXK_PAUSE; break;
        case VK_SPACE:      nId = WXK_SPACE; break;
        case VK_ESC:        nId = WXK_ESCAPE; break;
        case VK_END:        nId = WXK_END; break;
        case VK_HOME :      nId = WXK_HOME; break;
        case VK_LEFT :      nId = WXK_LEFT; break;
        case VK_UP:         nId = WXK_UP; break;
        case VK_RIGHT:      nId = WXK_RIGHT; break;
        case VK_DOWN :      nId = WXK_DOWN; break;
        case VK_PRINTSCRN:  nId = WXK_PRINT; break;
        case VK_INSERT:     nId = WXK_INSERT; break;
        case VK_DELETE:     nId = WXK_DELETE; break;
        case VK_F1:         nId = WXK_F1; break;
        case VK_F2:         nId = WXK_F2; break;
        case VK_F3:         nId = WXK_F3; break;
        case VK_F4:         nId = WXK_F4; break;
        case VK_F5:         nId = WXK_F5; break;
        case VK_F6:         nId = WXK_F6; break;
        case VK_F7:         nId = WXK_F7; break;
        case VK_F8:         nId = WXK_F8; break;
        case VK_F9:         nId = WXK_F9; break;
        case VK_F10:        nId = WXK_F10; break;
        case VK_F11:        nId = WXK_F11; break;
        case VK_F12:        nId = WXK_F12; break;
        case VK_F13:        nId = WXK_F13; break;
        case VK_F14:        nId = WXK_F14; break;
        case VK_F15:        nId = WXK_F15; break;
        case VK_F16:        nId = WXK_F16; break;
        case VK_F17:        nId = WXK_F17; break;
        case VK_F18:        nId = WXK_F18; break;
        case VK_F19:        nId = WXK_F19; break;
        case VK_F20:        nId = WXK_F20; break;
        case VK_F21:        nId = WXK_F21; break;
        case VK_F22:        nId = WXK_F22; break;
        case VK_F23:        nId = WXK_F23; break;
        case VK_F24:        nId = WXK_F24; break;
        case VK_NUMLOCK:    nId = WXK_NUMLOCK; break;
        case VK_SCRLLOCK:   nId = WXK_SCROLL; break;
        default:
        {
            return 0;
        }
    }
    return nId;
} // end of wxCharCodeOS2ToWX

int wxCharCodeWXToOS2(
  int                               nId
, bool*                             bIsVirtual
)
{
    int                             nKeySym = 0;

    *bIsVirtual = TRUE;
    switch (nId)
    {
        case WXK_CLEAR:     nKeySym = VK_CLEAR; break;
        case WXK_SHIFT:     nKeySym = VK_SHIFT; break;
        case WXK_CONTROL:   nKeySym = VK_CTRL; break;
        case WXK_PAUSE:     nKeySym = VK_PAUSE; break;
        case WXK_END:       nKeySym = VK_END; break;
        case WXK_HOME :     nKeySym = VK_HOME; break;
        case WXK_LEFT :     nKeySym = VK_LEFT; break;
        case WXK_UP:        nKeySym = VK_UP; break;
        case WXK_RIGHT:     nKeySym = VK_RIGHT; break;
        case WXK_DOWN :     nKeySym = VK_DOWN; break;
        case WXK_PRINT:     nKeySym = VK_PRINTSCRN; break;
        case WXK_INSERT:    nKeySym = VK_INSERT; break;
        case WXK_DELETE:    nKeySym = VK_DELETE; break;
        case WXK_F1:        nKeySym = VK_F1; break;
        case WXK_F2:        nKeySym = VK_F2; break;
        case WXK_F3:        nKeySym = VK_F3; break;
        case WXK_F4:        nKeySym = VK_F4; break;
        case WXK_F5:        nKeySym = VK_F5; break;
        case WXK_F6:        nKeySym = VK_F6; break;
        case WXK_F7:        nKeySym = VK_F7; break;
        case WXK_F8:        nKeySym = VK_F8; break;
        case WXK_F9:        nKeySym = VK_F9; break;
        case WXK_F10:       nKeySym = VK_F10; break;
        case WXK_F11:       nKeySym = VK_F11; break;
        case WXK_F12:       nKeySym = VK_F12; break;
        case WXK_F13:       nKeySym = VK_F13; break;
        case WXK_F14:       nKeySym = VK_F14; break;
        case WXK_F15:       nKeySym = VK_F15; break;
        case WXK_F16:       nKeySym = VK_F16; break;
        case WXK_F17:       nKeySym = VK_F17; break;
        case WXK_F18:       nKeySym = VK_F18; break;
        case WXK_F19:       nKeySym = VK_F19; break;
        case WXK_F20:       nKeySym = VK_F20; break;
        case WXK_F21:       nKeySym = VK_F21; break;
        case WXK_F22:       nKeySym = VK_F22; break;
        case WXK_F23:       nKeySym = VK_F23; break;
        case WXK_F24:       nKeySym = VK_F24; break;
        case WXK_NUMLOCK:   nKeySym = VK_NUMLOCK; break;
        case WXK_SCROLL:    nKeySym = VK_SCRLLOCK; break;
        default:
        {
            *bIsVirtual = FALSE;
            nKeySym = nId;
            break;
        }
    }
    return nKeySym;
} // end of wxCharCodeWXToOS2

wxWindow* wxGetActiveWindow()
{
    HWND                            hWnd = ::WinQueryActiveWindow(HWND_DESKTOP);

    if (hWnd != 0)
    {
        return wxFindWinFromHandle((WXHWND)hWnd);
    }
    return NULL;
} // end of wxGetActiveWindow

#ifdef __WXDEBUG__
const char* wxGetMessageName(
  int                               nMessage)
{
    switch (nMessage)
    {
        case 0x0000: return "WM_NULL";
        case 0x0001: return "WM_CREATE";
        case 0x0002: return "WM_DESTROY";
        case 0x0004: return "WM_ENABLE";
        case 0x0005: return "WM_SHOW";
        case 0x0006: return "WM_MOVE";
        case 0x0007: return "WM_SIZE";
        case 0x0008: return "WM_ADJUSTWINDOWPOS";
        case 0x0009: return "WM_CALCVALIDRECTS";
        case 0x000A: return "WM_SETWINDOWPARAMS";
        case 0x000B: return "WM_QUERYWINDOWPARAMS";
        case 0x000C: return "WM_HITTEST";
        case 0x000D: return "WM_ACTIVATE";
        case 0x000F: return "WM_SETFOCUS";
        case 0x0010: return "WM_SETSELECTION";
        case 0x0011: return "WM_PPAINT";
        case 0x0012: return "WM_PSETFOCUS";
        case 0x0013: return "WM_PSYSCOLORCHANGE";
        case 0x0014: return "WM_PSIZE";
        case 0x0015: return "WM_PACTIVATE";
        case 0x0016: return "WM_PCONTROL";
        case 0x0020: return "WM_COMMAND";
        case 0x0021: return "WM_SYSCOMMAND";
        case 0x0022: return "WM_HELP";
        case 0x0023: return "WM_PAINT";
        case 0x0024: return "WM_TIMER";
        case 0x0025: return "WM_SEM1";
        case 0x0026: return "WM_SEM2";
        case 0x0027: return "WM_SEM3";
        case 0x0028: return "WM_SEM4";
        case 0x0029: return "WM_CLOSE";
        case 0x002A: return "WM_QUIT";
        case 0x002B: return "WM_SYSCOLORCHANGE";
        case 0x002D: return "WM_SYSVALUECHANGE";
        case 0x002E: return "WM_APPTERMINATENOTIFY";
        case 0x002F: return "WM_PRESPARAMCHANGED";
        // Control notification messages
        case 0x0030: return "WM_CONTROL";
        case 0x0031: return "WM_VSCROLL";
        case 0x0032: return "WM_HSCROLL";
        case 0x0033: return "WM_INITMENU";
        case 0x0034: return "WM_MENUSELECT";
        case 0x0035: return "WM_MENUSEND";
        case 0x0036: return "WM_DRAWITEM";
        case 0x0037: return "WM_MEASUREITEM";
        case 0x0038: return "WM_CONTROLPOINTER";
        case 0x003A: return "WM_QUERYDLGCODE";
        case 0x003B: return "WM_INITDLG";
        case 0x003C: return "WM_SUBSTITUTESTRING";
        case 0x003D: return "WM_MATCHMNEMONIC";
        case 0x003E: return "WM_SAVEAPPLICATION";
        case 0x0129: return "WM_CTLCOLORCHANGE";
        case 0x0130: return "WM_QUERYCTLTYPE";
        // Frame messages
        case 0x0040: return "WM_FLASHWINDOW";
        case 0x0041: return "WM_FORMATFRAME";
        case 0x0042: return "WM_UPDATEFRAME";
        case 0x0043: return "WM_FOCUSCHANGE";
        case 0x0044: return "WM_SETBORDERSIZE";
        case 0x0045: return "WM_TRACKFRAME";
        case 0x0046: return "WM_MINMAXFRAME";
        case 0x0047: return "WM_SETICON";
        case 0x0048: return "WM_QUERYICON";
        case 0x0049: return "WM_SETACCELTABLE";
        case 0x004A: return "WM_QUERYACCELTABLE";
        case 0x004B: return "WM_TRANSLATEACCEL";
        case 0x004C: return "WM_QUERYTRACKINFO";
        case 0x004D: return "WM_QUERYBORDERSIZE";
        case 0x004E: return "WM_NEXTMENU";
        case 0x004F: return "WM_ERASEBACKGROUND";
        case 0x0050: return "WM_QUERYFRAMEINFO";
        case 0x0051: return "WM_QUERYFOCUSCHAIN";
        case 0x0052: return "WM_OWNERPOSCHANGE";
        case 0x0053: return "WM_CACLFRAMERECT";
        case 0x0055: return "WM_WINDOWPOSCHANGED";
        case 0x0056: return "WM_ADJUSTFRAMEPOS";
        case 0x0059: return "WM_QUERYFRAMECTLCOUNT";
        case 0x005B: return "WM_QUERYHELPINFO";
        case 0x005C: return "WM_SETHELPINFO";
        case 0x005D: return "WM_ERROR";
        case 0x005E: return "WM_REALIZEPALETTE";
        // Clipboard messages
        case 0x0060: return "WM_RENDERFMT";
        case 0x0061: return "WM_RENDERALLFMTS";
        case 0x0062: return "WM_DESTROYCLIPBOARD";
        case 0x0063: return "WM_PAINTCLIPBOARD";
        case 0x0064: return "WM_SIZECLIPBOARD";
        case 0x0065: return "WM_HSCROLLCLIPBOARD";
        case 0x0066: return "WM_VSCROLLCLIPBOARD";
        case 0x0067: return "WM_DRAWCLIPBOARD";
        // mouse messages
        case 0x0070: return "WM_MOUSEMOVE";
        case 0x0071: return "WM_BUTTON1DOWN";
        case 0x0072: return "WM_BUTTON1UP";
        case 0x0073: return "WM_BUTTON1DBLCLK";
        case 0x0074: return "WM_BUTTON2DOWN";
        case 0x0075: return "WM_BUTTON2UP";
        case 0x0076: return "WM_BUTTON2DBLCLK";
        case 0x0077: return "WM_BUTTON3DOWN";
        case 0x0078: return "WM_BUTTON3UP";
        case 0x0079: return "WM_BUTTON3DBLCLK";
        case 0x007D: return "WM_MOUSEMAP";
        case 0x007E: return "WM_VRNDISABLED";
        case 0x007F: return "WM_VRNENABLED";
        case 0x0410: return "WM_CHORD";
        case 0x0411: return "WM_BUTTON1MOTIONSTART";
        case 0x0412: return "WM_BUTTON1MOTIONEND";
        case 0x0413: return "WM_BUTTON1CLICK";
        case 0x0414: return "WM_BUTTON2MOTIONSTART";
        case 0x0415: return "WM_BUTTON2MOTIONEND";
        case 0x0416: return "WM_BUTTON2CLICK";
        case 0x0417: return "WM_BUTTON3MOTIONSTART";
        case 0x0418: return "WM_BUTTON3MOTIONEND";
        case 0x0419: return "WM_BUTTON3CLICK";
        case 0x0420: return "WM_BEGINDRAG";
        case 0x0421: return "WM_ENDDRAG";
        case 0x0422: return "WM_SINGLESELECT";
        case 0x0423: return "WM_OPEN";
        case 0x0424: return "WM_CONTEXTMENU";
        case 0x0425: return "WM_CONTEXTHELP";
        case 0x0426: return "WM_TEXTEDIT";
        case 0x0427: return "WM_BEGINSELECT";
        case 0x0228: return "WM_ENDSELECT";
        case 0x0429: return "WM_PICKUP";
        case 0x04C0: return "WM_PENFIRST";
        case 0x04FF: return "WM_PENLAST";
        case 0x0500: return "WM_MMPMFIRST";
        case 0x05FF: return "WM_MMPMLAST";
        case 0x0600: return "WM_STDDLGFIRST";
        case 0x06FF: return "WM_STDDLGLAST";
        case 0x0BD0: return "WM_BIDI_FIRST";
        case 0x0BFF: return "WM_BIDI_LAST";
        // keyboard input
        case 0x007A: return "WM_CHAR";
        case 0x007B: return "WM_VIOCHAR";
        // DDE messages
        case 0x00A0: return "WM_DDE_INITIATE";
        case 0x00A1: return "WM_DDE_REQUEST";
        case 0x00A2: return "WM_DDE_ACK";
        case 0x00A3: return "WM_DDE_DATA";
        case 0x00A4: return "WM_DDE_ADVISE";
        case 0x00A5: return "WM_DDE_UNADVISE";
        case 0x00A6: return "WM_DDE_POKE";
        case 0x00A7: return "WM_DDE_EXECUTE";
        case 0x00A8: return "WM_DDE_TERMINATE";
        case 0x00A9: return "WM_DDE_INITIATEACK";
        case 0x00AF: return "WM_DDE_LAST";
        // Buttons
        case 0x0120: return "BM_CLICK";
        case 0x0121: return "BM_QUERYCHECKINDEX";
        case 0x0122: return "BM_QUERYHILITE";
        case 0x0123: return "BM_SETHILITE";
        case 0x0124: return "BM_QUERYCHECK";
        case 0x0125: return "BM_SETCHECK";
        case 0x0126: return "BM_SETDEFAULT";
        case 0x0128: return "BM_AUTOSIZE";
        // Combo boxes
        case 0x029A: return "CBID_LIST";
        case 0x029B: return "CBID_EDIT";
        case 0x0170: return "CBM_SHOWLIST";
        case 0x0171: return "CBM_HILITE";
        case 0x0172: return "CBM_ISLISTSHOWING";
        // Edit fields
        case 0x0140: return "EM_QUERYCHANGED";
        case 0x0141: return "EM_QUERYSEL";
        case 0x0142: return "EM_SETSEL";
        case 0x0143: return "EM_SETTEXTLIMIT";
        case 0x0144: return "EM_CUT";
        case 0x0145: return "EM_COPY";
        case 0x0146: return "EM_CLEAR";
        case 0x0147: return "EM_PASTE";
        case 0x0148: return "EM_QUERYFIRSTCHAR";
        case 0x0149: return "EM_SETFIRSTCHAR";
        case 0x014A: return "EM_QUERYREADONLY";
        case 0x014B: return "EM_SETREADONLY";
        case 0x014C: return "EM_SETINSERTMODE";
        // Listboxes
        case 0x0160: return "LM_QUERYITEMCOUNT";
        case 0x0161: return "LM_INSERTITEM";
        case 0x0162: return "LM_SETOPENINDEX";
        case 0x0163: return "LM_DELETEITEM";
        case 0x0164: return "LM_SELECTITEM";
        case 0x0165: return "LM_QUERYSELECTION";
        case 0x0166: return "LM_SETITEMTEXT";
        case 0x0167: return "LM_QUERYITEMTEXTLENGTH";
        case 0x0168: return "LM_QUERYITEMTEXT";
        case 0x0169: return "LM_SETITEMHANDLE";
        case 0x016A: return "LM_QUERYITEMHANDLE";
        case 0x016B: return "LM_SEARCHSTRING";
        case 0x016C: return "LM_SETITEMHEIGHT";
        case 0x016D: return "LM_QUERYTOPINDEX";
        case 0x016E: return "LM_DELETEALL";
        case 0x016F: return "LM_INSERTMULITEMS";
        case 0x0660: return "LM_SETITEMWIDTH";
        // Menus
        case 0x0180: return "MM_INSERTITEM";
        case 0x0181: return "MM_DELETEITEM";
        case 0x0182: return "MM_QUERYITEM";
        case 0x0183: return "MM_SETITEM";
        case 0x0184: return "MM_QUERYITEMCOUNT";
        case 0x0185: return "MM_STARTMENUMODE";
        case 0x0186: return "MM_ENDMENUMODE";
        case 0x0188: return "MM_REMOVEITEM";
        case 0x0189: return "MM_SELECTITEM";
        case 0x018A: return "MM_QUERYSELITEMID";
        case 0x018B: return "MM_QUERYITEMTEXT";
        case 0x018C: return "MM_QUERYITEMTEXTLENGTH";
        case 0x018D: return "MM_SETITEMHANDLE";
        case 0x018E: return "MM_SETITEMTEXT";
        case 0x018F: return "MM_ITEMPOSITIONFROMID";
        case 0x0190: return "MM_ITEMIDFROMPOSITION";
        case 0x0191: return "MM_QUERYITEMATTR";
        case 0x0192: return "MM_SETITEMATTR";
        case 0x0193: return "MM_ISITEMVALID";
        case 0x0194: return "MM_QUERYITEMRECT";
        case 0x0431: return "MM_QUERYDEFAULTITEMID";
        case 0x0432: return "MM_SETDEFAULTITEMID";
        // Scrollbars
        case 0x01A0: return "SBM_SETSCROLLBAR";
        case 0x01A1: return "SBM_SETPOS";
        case 0x01A2: return "SBM_QUERYPOS";
        case 0x01A3: return "SBM_QUERYRANGE";
        case 0x01A6: return "SBM_SETTHUMBSIZE";

        // Help messages
        case 0x0F00: return "WM_HELPBASE";
        case 0x0FFF: return "WM_HELPTOP";
        // Beginning of user defined messages
        case 0x1000: return "WM_USER";

        // wxWindows user defined types

        // listview
        // case 0x1000 + 0: return "LVM_GETBKCOLOR";
        case 0x1000 + 1: return "LVM_SETBKCOLOR";
        case 0x1000 + 2: return "LVM_GETIMAGELIST";
        case 0x1000 + 3: return "LVM_SETIMAGELIST";
        case 0x1000 + 4: return "LVM_GETITEMCOUNT";
        case 0x1000 + 5: return "LVM_GETITEMA";
        case 0x1000 + 75: return "LVM_GETITEMW";
        case 0x1000 + 6: return "LVM_SETITEMA";
        case 0x1000 + 76: return "LVM_SETITEMW";
        case 0x1000 + 7: return "LVM_INSERTITEMA";
        case 0x1000 + 77: return "LVM_INSERTITEMW";
        case 0x1000 + 8: return "LVM_DELETEITEM";
        case 0x1000 + 9: return "LVM_DELETEALLITEMS";
        case 0x1000 + 10: return "LVM_GETCALLBACKMASK";
        case 0x1000 + 11: return "LVM_SETCALLBACKMASK";
        case 0x1000 + 12: return "LVM_GETNEXTITEM";
        case 0x1000 + 13: return "LVM_FINDITEMA";
        case 0x1000 + 83: return "LVM_FINDITEMW";
        case 0x1000 + 14: return "LVM_GETITEMRECT";
        case 0x1000 + 15: return "LVM_SETITEMPOSITION";
        case 0x1000 + 16: return "LVM_GETITEMPOSITION";
        case 0x1000 + 17: return "LVM_GETSTRINGWIDTHA";
        case 0x1000 + 87: return "LVM_GETSTRINGWIDTHW";
        case 0x1000 + 18: return "LVM_HITTEST";
        case 0x1000 + 19: return "LVM_ENSUREVISIBLE";
        case 0x1000 + 20: return "LVM_SCROLL";
        case 0x1000 + 21: return "LVM_REDRAWITEMS";
        case 0x1000 + 22: return "LVM_ARRANGE";
        case 0x1000 + 23: return "LVM_EDITLABELA";
        case 0x1000 + 118: return "LVM_EDITLABELW";
        case 0x1000 + 24: return "LVM_GETEDITCONTROL";
        case 0x1000 + 25: return "LVM_GETCOLUMNA";
        case 0x1000 + 95: return "LVM_GETCOLUMNW";
        case 0x1000 + 26: return "LVM_SETCOLUMNA";
        case 0x1000 + 96: return "LVM_SETCOLUMNW";
        case 0x1000 + 27: return "LVM_INSERTCOLUMNA";
        case 0x1000 + 97: return "LVM_INSERTCOLUMNW";
        case 0x1000 + 28: return "LVM_DELETECOLUMN";
        case 0x1000 + 29: return "LVM_GETCOLUMNWIDTH";
        case 0x1000 + 30: return "LVM_SETCOLUMNWIDTH";
        case 0x1000 + 31: return "LVM_GETHEADER";
        case 0x1000 + 33: return "LVM_CREATEDRAGIMAGE";
        case 0x1000 + 34: return "LVM_GETVIEWRECT";
        case 0x1000 + 35: return "LVM_GETTEXTCOLOR";
        case 0x1000 + 36: return "LVM_SETTEXTCOLOR";
        case 0x1000 + 37: return "LVM_GETTEXTBKCOLOR";
        case 0x1000 + 38: return "LVM_SETTEXTBKCOLOR";
        case 0x1000 + 39: return "LVM_GETTOPINDEX";
        case 0x1000 + 40: return "LVM_GETCOUNTPERPAGE";
        case 0x1000 + 41: return "LVM_GETORIGIN";
        case 0x1000 + 42: return "LVM_UPDATE";
        case 0x1000 + 43: return "LVM_SETITEMSTATE";
        case 0x1000 + 44: return "LVM_GETITEMSTATE";
        case 0x1000 + 45: return "LVM_GETITEMTEXTA";
        case 0x1000 + 115: return "LVM_GETITEMTEXTW";
        case 0x1000 + 46: return "LVM_SETITEMTEXTA";
        case 0x1000 + 116: return "LVM_SETITEMTEXTW";
        case 0x1000 + 47: return "LVM_SETITEMCOUNT";
        case 0x1000 + 48: return "LVM_SORTITEMS";
        case 0x1000 + 49: return "LVM_SETITEMPOSITION32";
        case 0x1000 + 50: return "LVM_GETSELECTEDCOUNT";
        case 0x1000 + 51: return "LVM_GETITEMSPACING";
        case 0x1000 + 52: return "LVM_GETISEARCHSTRINGA";
        case 0x1000 + 117: return "LVM_GETISEARCHSTRINGW";
        case 0x1000 + 53: return "LVM_SETICONSPACING";
        case 0x1000 + 54: return "LVM_SETEXTENDEDLISTVIEWSTYLE";
        case 0x1000 + 55: return "LVM_GETEXTENDEDLISTVIEWSTYLE";
        case 0x1000 + 56: return "LVM_GETSUBITEMRECT";
        case 0x1000 + 57: return "LVM_SUBITEMHITTEST";
        case 0x1000 + 58: return "LVM_SETCOLUMNORDERARRAY";
        case 0x1000 + 59: return "LVM_GETCOLUMNORDERARRAY";
        case 0x1000 + 60: return "LVM_SETHOTITEM";
        case 0x1000 + 61: return "LVM_GETHOTITEM";
        case 0x1000 + 62: return "LVM_SETHOTCURSOR";
        case 0x1000 + 63: return "LVM_GETHOTCURSOR";
        case 0x1000 + 64: return "LVM_APPROXIMATEVIEWRECT";
        case 0x1000 + 65: return "LVM_SETWORKAREA";

        // tree view
        case 0x1100 + 0: return "TVM_INSERTITEMA";
        case 0x1100 + 50: return "TVM_INSERTITEMW";
        case 0x1100 + 1: return "TVM_DELETEITEM";
        case 0x1100 + 2: return "TVM_EXPAND";
        case 0x1100 + 4: return "TVM_GETITEMRECT";
        case 0x1100 + 5: return "TVM_GETCOUNT";
        case 0x1100 + 6: return "TVM_GETINDENT";
        case 0x1100 + 7: return "TVM_SETINDENT";
        case 0x1100 + 8: return "TVM_GETIMAGELIST";
        case 0x1100 + 9: return "TVM_SETIMAGELIST";
        case 0x1100 + 10: return "TVM_GETNEXTITEM";
        case 0x1100 + 11: return "TVM_SELECTITEM";
        case 0x1100 + 12: return "TVM_GETITEMA";
        case 0x1100 + 62: return "TVM_GETITEMW";
        case 0x1100 + 13: return "TVM_SETITEMA";
        case 0x1100 + 63: return "TVM_SETITEMW";
        case 0x1100 + 14: return "TVM_EDITLABELA";
        case 0x1100 + 65: return "TVM_EDITLABELW";
        case 0x1100 + 15: return "TVM_GETEDITCONTROL";
        case 0x1100 + 16: return "TVM_GETVISIBLECOUNT";
        case 0x1100 + 17: return "TVM_HITTEST";
        case 0x1100 + 18: return "TVM_CREATEDRAGIMAGE";
        case 0x1100 + 19: return "TVM_SORTCHILDREN";
        case 0x1100 + 20: return "TVM_ENSUREVISIBLE";
        case 0x1100 + 21: return "TVM_SORTCHILDRENCB";
        case 0x1100 + 22: return "TVM_ENDEDITLABELNOW";
        case 0x1100 + 23: return "TVM_GETISEARCHSTRINGA";
        case 0x1100 + 64: return "TVM_GETISEARCHSTRINGW";
        case 0x1100 + 24: return "TVM_SETTOOLTIPS";
        case 0x1100 + 25: return "TVM_GETTOOLTIPS";

        // header
        case 0x1200 + 0: return "HDM_GETITEMCOUNT";
        case 0x1200 + 1: return "HDM_INSERTITEMA";
        case 0x1200 + 10: return "HDM_INSERTITEMW";
        case 0x1200 + 2: return "HDM_DELETEITEM";
        case 0x1200 + 3: return "HDM_GETITEMA";
        case 0x1200 + 11: return "HDM_GETITEMW";
        case 0x1200 + 4: return "HDM_SETITEMA";
        case 0x1200 + 12: return "HDM_SETITEMW";
        case 0x1200 + 5: return "HDM_LAYOUT";
        case 0x1200 + 6: return "HDM_HITTEST";
        case 0x1200 + 7: return "HDM_GETITEMRECT";
        case 0x1200 + 8: return "HDM_SETIMAGELIST";
        case 0x1200 + 9: return "HDM_GETIMAGELIST";
        case 0x1200 + 15: return "HDM_ORDERTOINDEX";
        case 0x1200 + 16: return "HDM_CREATEDRAGIMAGE";
        case 0x1200 + 17: return "HDM_GETORDERARRAY";
        case 0x1200 + 18: return "HDM_SETORDERARRAY";
        case 0x1200 + 19: return "HDM_SETHOTDIVIDER";

        // tab control
        case 0x1300 + 2: return "TCM_GETIMAGELIST";
        case 0x1300 + 3: return "TCM_SETIMAGELIST";
        case 0x1300 + 4: return "TCM_GETITEMCOUNT";
        case 0x1300 + 5: return "TCM_GETITEMA";
        case 0x1300 + 60: return "TCM_GETITEMW";
        case 0x1300 + 6: return "TCM_SETITEMA";
        case 0x1300 + 61: return "TCM_SETITEMW";
        case 0x1300 + 7: return "TCM_INSERTITEMA";
        case 0x1300 + 62: return "TCM_INSERTITEMW";
        case 0x1300 + 8: return "TCM_DELETEITEM";
        case 0x1300 + 9: return "TCM_DELETEALLITEMS";
        case 0x1300 + 10: return "TCM_GETITEMRECT";
        case 0x1300 + 11: return "TCM_GETCURSEL";
        case 0x1300 + 12: return "TCM_SETCURSEL";
        case 0x1300 + 13: return "TCM_HITTEST";
        case 0x1300 + 14: return "TCM_SETITEMEXTRA";
        case 0x1300 + 40: return "TCM_ADJUSTRECT";
        case 0x1300 + 41: return "TCM_SETITEMSIZE";
        case 0x1300 + 42: return "TCM_REMOVEIMAGE";
        case 0x1300 + 43: return "TCM_SETPADDING";
        case 0x1300 + 44: return "TCM_GETROWCOUNT";
        case 0x1300 + 45: return "TCM_GETTOOLTIPS";
        case 0x1300 + 46: return "TCM_SETTOOLTIPS";
        case 0x1300 + 47: return "TCM_GETCURFOCUS";
        case 0x1300 + 48: return "TCM_SETCURFOCUS";
        case 0x1300 + 49: return "TCM_SETMINTABWIDTH";
        case 0x1300 + 50: return "TCM_DESELECTALL";

        // toolbar
        case WM_USER+1000+1: return "TB_ENABLEBUTTON";
        case WM_USER+1000+2: return "TB_CHECKBUTTON";
        case WM_USER+1000+3: return "TB_PRESSBUTTON";
        case WM_USER+1000+4: return "TB_HIDEBUTTON";
        case WM_USER+1000+5: return "TB_INDETERMINATE";
        case WM_USER+1000+9: return "TB_ISBUTTONENABLED";
        case WM_USER+1000+10: return "TB_ISBUTTONCHECKED";
        case WM_USER+1000+11: return "TB_ISBUTTONPRESSED";
        case WM_USER+1000+12: return "TB_ISBUTTONHIDDEN";
        case WM_USER+1000+13: return "TB_ISBUTTONINDETERMINATE";
        case WM_USER+1000+17: return "TB_SETSTATE";
        case WM_USER+1000+18: return "TB_GETSTATE";
        case WM_USER+1000+19: return "TB_ADDBITMAP";
        case WM_USER+1000+20: return "TB_ADDBUTTONS";
        case WM_USER+1000+21: return "TB_INSERTBUTTON";
        case WM_USER+1000+22: return "TB_DELETEBUTTON";
        case WM_USER+1000+23: return "TB_GETBUTTON";
        case WM_USER+1000+24: return "TB_BUTTONCOUNT";
        case WM_USER+1000+25: return "TB_COMMANDTOINDEX";
        case WM_USER+1000+26: return "TB_SAVERESTOREA";
        case WM_USER+1000+76: return "TB_SAVERESTOREW";
        case WM_USER+1000+27: return "TB_CUSTOMIZE";
        case WM_USER+1000+28: return "TB_ADDSTRINGA";
        case WM_USER+1000+77: return "TB_ADDSTRINGW";
        case WM_USER+1000+29: return "TB_GETITEMRECT";
        case WM_USER+1000+30: return "TB_BUTTONSTRUCTSIZE";
        case WM_USER+1000+31: return "TB_SETBUTTONSIZE";
        case WM_USER+1000+32: return "TB_SETBITMAPSIZE";
        case WM_USER+1000+33: return "TB_AUTOSIZE";
        case WM_USER+1000+35: return "TB_GETTOOLTIPS";
        case WM_USER+1000+36: return "TB_SETTOOLTIPS";
        case WM_USER+1000+37: return "TB_SETPARENT";
        case WM_USER+1000+39: return "TB_SETROWS";
        case WM_USER+1000+40: return "TB_GETROWS";
        case WM_USER+1000+42: return "TB_SETCMDID";
        case WM_USER+1000+43: return "TB_CHANGEBITMAP";
        case WM_USER+1000+44: return "TB_GETBITMAP";
        case WM_USER+1000+45: return "TB_GETBUTTONTEXTA";
        case WM_USER+1000+75: return "TB_GETBUTTONTEXTW";
        case WM_USER+1000+46: return "TB_REPLACEBITMAP";
        case WM_USER+1000+47: return "TB_SETINDENT";
        case WM_USER+1000+48: return "TB_SETIMAGELIST";
        case WM_USER+1000+49: return "TB_GETIMAGELIST";
        case WM_USER+1000+50: return "TB_LOADIMAGES";
        case WM_USER+1000+51: return "TB_GETRECT";
        case WM_USER+1000+52: return "TB_SETHOTIMAGELIST";
        case WM_USER+1000+53: return "TB_GETHOTIMAGELIST";
        case WM_USER+1000+54: return "TB_SETDISABLEDIMAGELIST";
        case WM_USER+1000+55: return "TB_GETDISABLEDIMAGELIST";
        case WM_USER+1000+56: return "TB_SETSTYLE";
        case WM_USER+1000+57: return "TB_GETSTYLE";
        case WM_USER+1000+58: return "TB_GETBUTTONSIZE";
        case WM_USER+1000+59: return "TB_SETBUTTONWIDTH";
        case WM_USER+1000+60: return "TB_SETMAXTEXTROWS";
        case WM_USER+1000+61: return "TB_GETTEXTROWS";
        case WM_USER+1000+41: return "TB_GETBITMAPFLAGS";

        default:
            static char s_szBuf[128];
            sprintf(s_szBuf, "<unknown message = %d>", nMessage);
            return s_szBuf;
    }
   return NULL;
} // end of wxGetMessageName

#endif // __WXDEBUG__

static void TranslateKbdEventToMouse(
  wxWindow*                         pWin
, int*                              pX
, int*                              pY
, ULONG*                            pFlags
)
{
    //
    // Construct the key mask
    ULONG&                          fwKeys = *pFlags;

    fwKeys = VK_BUTTON2;
    if ((::WinGetKeyState(HWND_DESKTOP, VK_CTRL) & 0x100) != 0)
        fwKeys |= VK_CTRL;
    if ((::WinGetKeyState(HWND_DESKTOP, VK_SHIFT) & 0x100) != 0)
        fwKeys |= VK_SHIFT;

    //
    // Simulate right mouse button click
    //
    POINTL                          vPoint;

    ::WinQueryMsgPos(vHabmain, &vPoint);
    *pX = vPoint.x;
    *pY = vPoint.y;

    pWin->ScreenToClient(pX, pY);
} // end of TranslateKbdEventToMouse

// Find the wxWindow at the current mouse position, returning the mouse
// position.
wxWindow* wxFindWindowAtPointer(
  wxPoint&                          rPt
)
{
    return wxFindWindowAtPoint(wxGetMousePosition());
}

wxWindow* wxFindWindowAtPoint(
  const wxPoint&                    rPt
)
{
    POINTL                          vPt2;

    vPt2.x = rPt.x;
    vPt2.y = rPt.y;

    HWND                            hWndHit = ::WinWindowFromPoint(HWND_DESKTOP, &vPt2, FALSE);
    wxWindow*                       pWin = wxFindWinFromHandle((WXHWND)hWndHit) ;
    HWND                            hWnd = hWndHit;

    //
    // Try to find a window with a wxWindow associated with it
    //
    while (!pWin && (hWnd != 0))
    {
        hWnd = ::WinQueryWindow(hWnd, QW_PARENT);
        pWin = wxFindWinFromHandle((WXHWND)hWnd) ;
    }
    return pWin;
}

// Get the current mouse position.
wxPoint wxGetMousePosition()
{
    POINTL                          vPt;

    ::WinQueryPointerPos(HWND_DESKTOP, &vPt);
    return wxPoint(vPt.x, vPt.y);
}

