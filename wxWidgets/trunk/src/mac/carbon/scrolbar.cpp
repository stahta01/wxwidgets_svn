/////////////////////////////////////////////////////////////////////////////
// Name:        scrolbar.cpp
// Purpose:     wxScrollBar
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "scrolbar.h"
#endif

#include "wx/defs.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
#endif // WX_PRECOMP

#include "wx/scrolbar.h"
#include "wx/mac/uma.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxScrollBar, wxControl)

BEGIN_EVENT_TABLE(wxScrollBar, wxControl)
END_EVENT_TABLE()

#endif

extern ControlActionUPP wxMacLiveScrollbarActionUPP ;

// Scrollbar
bool wxScrollBar::Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
    m_macIsUserPane = FALSE ;
    
    if ( !wxControl::Create(parent, id, pos, size, style, validator, name) )
        return FALSE;

    Rect bounds = wxMacGetBoundsForControl( this , pos , size ) ;

    m_peer = new wxMacControl() ;
    verify_noerr ( CreateScrollBarControl( MAC_WXHWND(parent->MacGetTopLevelWindowRef()) , &bounds , 
    0 , 0 , 100 , 1 , true /* liveTracking */ , wxMacLiveScrollbarActionUPP , *m_peer ) );
    

    MacPostControlCreate(pos,size) ;

    return TRUE;
}

wxScrollBar::~wxScrollBar()
{
}

void wxScrollBar::SetThumbPosition(int viewStart)
{
    ::SetControl32BitValue( *m_peer , viewStart ) ;
}

int wxScrollBar::GetThumbPosition() const
{
    return ::GetControl32BitValue( *m_peer ) ;
}

void wxScrollBar::SetScrollbar(int position, int thumbSize, int range, int pageSize,
    bool refresh)
{
    m_pageSize = pageSize;
    m_viewSize = thumbSize;
    m_objectSize = range;

   int range1 = wxMax((m_objectSize - m_viewSize), 0) ;

    SetControl32BitMaximum( *m_peer , range1 ) ;
    SetControl32BitMinimum( *m_peer , 0 ) ;
    SetControl32BitValue( *m_peer , position ) ;
    SetControlViewSize( *m_peer , m_viewSize ) ;

    if ( refresh )
      MacRedrawControl() ;
}


void wxScrollBar::Command(wxCommandEvent& event)
{
    SetThumbPosition(event.m_commandInt);
    ProcessCommand(event);
}

void wxScrollBar::MacHandleControlClick( WXWidget control , wxInt16 controlpart , bool mouseStillDown ) 
{
    int position = GetControl32BitValue( *m_peer) ;
    int minPos = GetControl32BitMinimum( *m_peer) ;
    int maxPos = GetControl32BitMaximum( *m_peer) ;
    
    wxEventType scrollEvent = wxEVT_NULL;
    int nScrollInc = 0;
    
    // all events have already been reported during mouse down, except for THUMBRELEASE
    if ( !mouseStillDown && controlpart !=kControlIndicatorPart )
        return ;
    
    switch( controlpart )
    {
    case kControlUpButtonPart :
        nScrollInc = -1;
        scrollEvent = wxEVT_SCROLL_LINEUP;
        break ;
    case kControlDownButtonPart :
        nScrollInc = 1;
        scrollEvent = wxEVT_SCROLL_LINEDOWN;
        break ;
    case kControlPageUpPart :
        nScrollInc = -m_pageSize;
        scrollEvent = wxEVT_SCROLL_PAGEUP;
        break ;
    case kControlPageDownPart :
        nScrollInc = m_pageSize;
        scrollEvent = wxEVT_SCROLL_PAGEDOWN;
        break ;
    case kControlIndicatorPart :
        nScrollInc = 0 ;
        if ( mouseStillDown )
            scrollEvent = wxEVT_SCROLL_THUMBTRACK;
        else
            scrollEvent = wxEVT_SCROLL_THUMBRELEASE;
        break ;
    default :
        wxFAIL_MSG(wxT("illegal scrollbar selector"));
        break ;
    }
    
    int new_pos = position + nScrollInc;
    
    if (new_pos < minPos)
        new_pos = minPos;
    if (new_pos > maxPos)
        new_pos = maxPos;
    if ( nScrollInc )
        SetThumbPosition(new_pos);
    
    wxScrollEvent event(scrollEvent, m_windowId);
    if ( m_windowStyle & wxHORIZONTAL )
    {
        event.SetOrientation( wxHORIZONTAL ) ;
    }
    else
    {
        event.SetOrientation( wxVERTICAL ) ;
    }
    event.SetPosition(new_pos);
    event.SetEventObject( this );
    wxWindow* window = GetParent() ;
    if (window && window->MacIsWindowScrollbar(this) )
    {
        // this is hardcoded
        window->MacOnScroll(event);
    }
    else
        GetEventHandler()->ProcessEvent(event);
}

wxInt32 wxScrollBar::MacControlHit( WXEVENTHANDLERREF handler , WXEVENTREF mevent ) 
{
    int position = GetControl32BitValue( *m_peer) ;
    int minPos = GetControl32BitMinimum( *m_peer) ;
    int maxPos = GetControl32BitMaximum( *m_peer) ;
    
    wxEventType scrollEvent = wxEVT_NULL;
    int nScrollInc = 0;
    
    wxMacCarbonEvent cEvent( (EventRef) mevent ) ;
    ControlPartCode controlpart = cEvent.GetParameter<ControlPartCode>(kEventParamControlPart,typeControlPartCode) ;
    
    // all events have already been reported during mouse down, except for THUMBRELEASE
    if ( controlpart !=kControlIndicatorPart )
        return eventNotHandledErr ;
    
    switch( controlpart )
    {
    case kControlIndicatorPart :
        nScrollInc = 0 ;
        scrollEvent = wxEVT_SCROLL_THUMBRELEASE;
        break ;
    default :
        wxFAIL_MSG(wxT("illegal scrollbar selector"));
        break ;
    }
    
    int new_pos = position + nScrollInc;
    
    if (new_pos < minPos)
        new_pos = minPos;
    if (new_pos > maxPos)
        new_pos = maxPos;
    if ( nScrollInc )
        SetThumbPosition(new_pos);
    
    wxScrollEvent event(scrollEvent, m_windowId);
    if ( m_windowStyle & wxHORIZONTAL )
    {
        event.SetOrientation( wxHORIZONTAL ) ;
    }
    else
    {
        event.SetOrientation( wxVERTICAL ) ;
    }
    event.SetPosition(new_pos);
    event.SetEventObject( this );
    wxWindow* window = GetParent() ;
    if (window && window->MacIsWindowScrollbar(this) )
    {
        // this is hardcoded
        window->MacOnScroll(event);
    }
    else
        GetEventHandler()->ProcessEvent(event);
    return noErr ;
}


