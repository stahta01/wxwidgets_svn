/////////////////////////////////////////////////////////////////////////////
// Name:        spinbutt.cpp
// Purpose:     wxSpinButton
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "spinbutt.h"
#pragma implementation "spinbuttbase.h"
#endif

#include "wx/spinbutt.h"
#include "wx/mac/uma.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
    IMPLEMENT_DYNAMIC_CLASS(wxSpinButton, wxControl)
    IMPLEMENT_DYNAMIC_CLASS(wxSpinEvent, wxScrollEvent)
#endif

wxSpinButton::wxSpinButton()
   : wxSpinButtonBase()
{
}

bool wxSpinButton::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
        long style, const wxString& name)
{
    m_macIsUserPane = FALSE ;
    
    if ( !wxSpinButtonBase::Create(parent, id, pos, size,
                                   style, wxDefaultValidator, name) )
        return false;

    m_min = 0;
    m_max = 100;
    
    if (!parent)
        return FALSE;
    
    Rect bounds = wxMacGetBoundsForControl( this , pos , size ) ;
        
    verify_noerr ( CreateLittleArrowsControl( MAC_WXHWND(parent->MacGetTopLevelWindowRef()) , &bounds , 0 , m_min , m_max , 1 ,
     (ControlRef*) &m_macControl ) ) ;
    
    MacPostControlCreate(pos,size) ;
    
    return TRUE;
}
    
wxSpinButton::~wxSpinButton()
{
}

// Attributes
////////////////////////////////////////////////////////////////////////////

int wxSpinButton::GetMin() const
{
    return m_min;
}

int wxSpinButton::GetMax() const
{
    return m_max;
}

int wxSpinButton::GetValue() const
{
    return m_value;
}

void wxSpinButton::SetValue(int val)
{
    m_value = val ;
}

void wxSpinButton::SetRange(int minVal, int maxVal)
{
    m_min = minVal;
    m_max = maxVal;
    SetControl32BitMaximum( (ControlRef) m_macControl , maxVal ) ;
    SetControl32BitMinimum((ControlRef) m_macControl , minVal ) ;
}

void wxSpinButton::MacHandleValueChanged( int inc )
{
    
    wxEventType scrollEvent = wxEVT_NULL;
    int oldValue = m_value ;
    
    m_value = oldValue + inc;
    
    if (m_value < m_min)
    {
        if ( m_windowStyle & wxSP_WRAP )
            m_value = m_max;
        else
            m_value = m_min;
    }
    
    if (m_value > m_max)
    {
        if ( m_windowStyle & wxSP_WRAP )
            m_value = m_min;
        else
            m_value = m_max;
    }
    
    if ( m_value - oldValue == -1 )
        scrollEvent = wxEVT_SCROLL_LINEDOWN ;
    else if ( m_value - oldValue == 1 )
        scrollEvent = wxEVT_SCROLL_LINEUP ;
    else
        scrollEvent = wxEVT_SCROLL_THUMBTRACK ;
    
    wxSpinEvent event(scrollEvent, m_windowId);
    
    event.SetPosition(m_value);
    event.SetEventObject( this );
    if ((GetEventHandler()->ProcessEvent( event )) &&
        !event.IsAllowed() )
    {
        m_value = oldValue ;
    }
    SetControl32BitValue( (ControlRef) m_macControl , m_value ) ;
    
    /* always send a thumbtrack event */
    if (scrollEvent != wxEVT_SCROLL_THUMBTRACK)
    {
        scrollEvent = wxEVT_SCROLL_THUMBTRACK;
        wxSpinEvent event2( scrollEvent, GetId());
        event2.SetPosition( m_value );
        event2.SetEventObject( this );
        GetEventHandler()->ProcessEvent( event2 );
    }
}

wxInt32 wxSpinButton::MacControlHit(WXEVENTHANDLERREF WXUNUSED(handler) , WXEVENTREF event )  
{
    int nScrollInc = 0;
    wxMacCarbonEvent cEvent( (EventRef) event ) ;
    
    switch( cEvent.GetParameter<ControlPartCode>(kEventParamControlPart,typeControlPartCode) )
    {
    case kControlUpButtonPart :
        nScrollInc = 1;
        break ;
    case kControlDownButtonPart :
        nScrollInc = -1;
        break ;
    }
    MacHandleValueChanged( nScrollInc ) ;
    return noErr ;
}

// ----------------------------------------------------------------------------
// size calculation
// ----------------------------------------------------------------------------

wxSize wxSpinButton::DoGetBestSize() const
{
    return wxSize(16,24);
}

