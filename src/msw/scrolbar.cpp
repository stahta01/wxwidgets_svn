/////////////////////////////////////////////////////////////////////////////
// Name:        msw/scrolbar.cpp
// Purpose:     wxScrollBar
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "scrolbar.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_SCROLLBAR

#ifndef WX_PRECOMP
    #include "wx/utils.h"
#endif

#include "wx/scrolbar.h"
#include "wx/msw/private.h"
#include "wx/settings.h"

#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxScrollBarStyle )

wxBEGIN_FLAGS( wxScrollBarStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)
    
    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    wxFLAGS_MEMBER(wxSB_HORIZONTAL)
    wxFLAGS_MEMBER(wxSB_VERTICAL)

wxEND_FLAGS( wxScrollBarStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxScrollBar, wxControl,"wx/scrolbar.h")

wxBEGIN_PROPERTIES_TABLE(wxScrollBar)
    wxEVENT_RANGE_PROPERTY( Scroll , wxEVT_SCROLL_TOP , wxEVT_SCROLL_ENDSCROLL , wxScrollEvent )

    wxPROPERTY( ThumbPosition , int , SetThumbPosition, GetThumbPosition, 0 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
	wxPROPERTY( Range , int , SetRange, GetRange, 0 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
	wxPROPERTY( ThumbSize , int , SetThumbSize, GetThumbSize, 0 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
	wxPROPERTY( PageSize , int , SetPageSize, GetPageSize, 0 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY_FLAGS( WindowStyle , wxScrollBarStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , EMPTY_MACROVALUE , 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxScrollBar)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_5( wxScrollBar , wxWindow* , Parent , wxWindowID , Id , wxPoint , Position , wxSize , Size , long , WindowStyle )
#else
IMPLEMENT_DYNAMIC_CLASS(wxScrollBar, wxControl)
#endif

// Scrollbar
bool wxScrollBar::Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& wxVALIDATOR_PARAM(validator),
           const wxString& name)
{
    if (!parent)
        return FALSE;
    parent->AddChild(this);
    SetName(name);
#if wxUSE_VALIDATORS
    SetValidator(validator);
#endif // wxUSE_VALIDATORS

    if ((style & wxBORDER_MASK) == wxBORDER_DEFAULT)
        style |= wxNO_BORDER;

    SetBackgroundColour(parent->GetBackgroundColour()) ;
    SetForegroundColour(parent->GetForegroundColour()) ;
    m_windowStyle = style;

  if ( id == -1 )
      m_windowId = (int)NewControlId();
  else
    m_windowId = id;

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;

    if (width == -1)
    {
      if (style & wxHORIZONTAL)
        width = 140;
      else
        width = 14;
    }
    if (height == -1)
    {
      if (style & wxVERTICAL)
        height = 140;
      else
        height = 14;
    }

    WXDWORD exStyle = 0;
    WXDWORD wstyle = MSWGetStyle(style, & exStyle) ;

    // Now create scrollbar
    DWORD _direction = (style & wxHORIZONTAL) ?
                        SBS_HORZ: SBS_VERT;
    HWND scroll_bar = CreateWindowEx(exStyle, wxT("SCROLLBAR"), wxT("scrollbar"),
                         _direction | wstyle,
                         0, 0, 0, 0, (HWND) parent->GetHWND(), (HMENU)m_windowId,
                         wxGetInstance(), NULL);

    m_pageSize = 1;
    m_viewSize = 1;
    m_objectSize = 1;

    ::SetScrollRange(scroll_bar, SB_CTL, 0, 1, FALSE);
    ::SetScrollPos(scroll_bar, SB_CTL, 0, FALSE);
    ShowWindow(scroll_bar, SW_SHOW);

    m_hWnd = (WXHWND)scroll_bar;

    // Subclass again for purposes of dialog editing mode
    SubclassWin((WXHWND) scroll_bar);

    SetSize(x, y, width, height);

    return TRUE;
}

wxScrollBar::~wxScrollBar(void)
{
}

bool wxScrollBar::MSWOnScroll(int WXUNUSED(orientation), WXWORD wParam,
                              WXWORD pos, WXHWND WXUNUSED(control))
{
    // current and max positions
    int position,
        maxPos, trackPos = pos;

    wxUnusedVar(trackPos);

    // when we're dragging the scrollbar we can't use pos parameter because it
    // is limited to 16 bits
    // JACS: now always using GetScrollInfo, since there's no reason
    // not to
//    if ( wParam == SB_THUMBPOSITION || wParam == SB_THUMBTRACK )
    {
        SCROLLINFO scrollInfo;
        wxZeroMemory(scrollInfo);
        scrollInfo.cbSize = sizeof(SCROLLINFO);

        // also get the range if we call GetScrollInfo() anyhow -- this is less
        // expensive than call it once here and then call GetScrollRange()
        // below
        scrollInfo.fMask = SIF_RANGE | SIF_POS | SIF_TRACKPOS;

        if ( !::GetScrollInfo(GetHwnd(), SB_CTL, &scrollInfo) )
        {
            wxLogLastError(_T("GetScrollInfo"));
        }

        trackPos = scrollInfo.nTrackPos;
        position = scrollInfo.nPos;
        maxPos = scrollInfo.nMax;
    }
#if 0
    else
    {
        position = ::GetScrollPos((HWND) control, SB_CTL);
        int minPos;
        ::GetScrollRange((HWND) control, SB_CTL, &minPos, &maxPos);
    }
#endif

#if defined(__WIN95__)
    // A page size greater than one has the effect of reducing the effective
    // range, therefore the range has already been boosted artificially - so
    // reduce it again.
    if ( m_pageSize > 1 )
        maxPos -= (m_pageSize - 1);
#endif // __WIN95__

    wxEventType scrollEvent = wxEVT_NULL;

    int nScrollInc;
    switch ( wParam )
    {
        case SB_BOTTOM:
            nScrollInc = maxPos - position;
            scrollEvent = wxEVT_SCROLL_TOP;
            break;

        case SB_TOP:
            nScrollInc = -position;
            scrollEvent = wxEVT_SCROLL_BOTTOM;
            break;

        case SB_LINEUP:
            nScrollInc = -1;
            scrollEvent = wxEVT_SCROLL_LINEUP;
            break;

        case SB_LINEDOWN:
            nScrollInc = 1;
            scrollEvent = wxEVT_SCROLL_LINEDOWN;
            break;

        case SB_PAGEUP:
            nScrollInc = -GetPageSize();
            scrollEvent = wxEVT_SCROLL_PAGEUP;
            break;

        case SB_PAGEDOWN:
            nScrollInc = GetPageSize();
            scrollEvent = wxEVT_SCROLL_PAGEDOWN;
            break;

        case SB_THUMBPOSITION:
            nScrollInc = trackPos - position;
            scrollEvent = wxEVT_SCROLL_THUMBRELEASE;
            break;

        case SB_THUMBTRACK:
            nScrollInc = trackPos - position;
            scrollEvent = wxEVT_SCROLL_THUMBTRACK;
            break;

        case SB_ENDSCROLL:
            nScrollInc = 0;
            scrollEvent = wxEVT_SCROLL_ENDSCROLL;
            break;

        default:
            nScrollInc = 0;
    }

    if ( nScrollInc )
    {
        position += nScrollInc;

        if ( position < 0 )
            position = 0;
        if ( position > maxPos )
            position = maxPos;

        SetThumbPosition(position);
    }
    else if ( scrollEvent != wxEVT_SCROLL_THUMBRELEASE &&
                scrollEvent != wxEVT_SCROLL_ENDSCROLL )
    {
        // don't process the event if there is no displacement,
        // unless this is a thumb release or end scroll event.
        return FALSE;
    }

    wxScrollEvent event(scrollEvent, m_windowId);
    event.SetOrientation(IsVertical() ? wxVERTICAL : wxHORIZONTAL);
    event.SetPosition(position);
    event.SetEventObject( this );

    return GetEventHandler()->ProcessEvent(event);
}

void wxScrollBar::SetThumbPosition(int viewStart)
{
#if defined(__WIN95__)
    SCROLLINFO info;
    info.cbSize = sizeof(SCROLLINFO);
    info.nPage = 0;
    info.nMin = 0;
    info.nPos = viewStart;
    info.fMask = SIF_POS ;

    ::SetScrollInfo((HWND) GetHWND(), SB_CTL, &info, TRUE);
#else
    ::SetScrollPos((HWND) GetHWND(), SB_CTL, viewStart, TRUE);
#endif
}

int wxScrollBar::GetThumbPosition(void) const
{
    SCROLLINFO scrollInfo;
    wxZeroMemory(scrollInfo);
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_POS;

    if ( !::GetScrollInfo(GetHwnd(), SB_CTL, &scrollInfo) )
    {
        wxLogLastError(_T("GetScrollInfo"));
    }
    return scrollInfo.nPos;
//    return ::GetScrollPos((HWND)m_hWnd, SB_CTL);
}

void wxScrollBar::SetScrollbar(int position, int thumbSize, int range, int pageSize,
    bool refresh)
{
  m_viewSize = pageSize;
  m_pageSize = thumbSize;
  m_objectSize = range;

  // The range (number of scroll steps) is the
  // object length minus the page size.
  int range1 = wxMax((m_objectSize - m_pageSize), 0) ;

#if defined(__WIN95__)
  // Try to adjust the range to cope with page size > 1
  // (see comment for SetPageLength)
  if ( m_pageSize > 1 )
  {
    range1 += (m_pageSize - 1);
  }

  SCROLLINFO info;
  info.cbSize = sizeof(SCROLLINFO);
  info.nPage = m_pageSize;
  info.nMin = 0;
  info.nMax = range1;
  info.nPos = position;

  info.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;

  ::SetScrollInfo((HWND) GetHWND(), SB_CTL, &info, refresh);
#else
  ::SetScrollPos((HWND)m_hWnd, SB_CTL, position, TRUE);
  ::SetScrollRange((HWND)m_hWnd, SB_CTL, 0, range1, TRUE);
#endif
}


WXHBRUSH wxScrollBar::OnCtlColor(WXHDC WXUNUSED(pDC), WXHWND WXUNUSED(pWnd), WXUINT WXUNUSED(nCtlColor),
            WXUINT WXUNUSED(message), WXWPARAM WXUNUSED(wParam), WXLPARAM WXUNUSED(lParam))
{
  return 0;
}

void wxScrollBar::Command(wxCommandEvent& event)
{
    SetThumbPosition(event.m_commandInt);
    ProcessCommand(event);
}

wxSize wxScrollBar::DoGetBestSize() const
{
    int w = 100;
    int h = 100;

    if ( IsVertical() )
    {
        w = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
    }
    else
    {
        h = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
    }

    return wxSize(w, h);
}

#endif // wxUSE_SCROLLBAR
