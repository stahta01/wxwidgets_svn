/////////////////////////////////////////////////////////////////////////////
// Name:        button.cpp
// Purpose:     wxButton
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "button.h"
#endif

#include "wx/defs.h"

#include "wx/button.h"
#include "wx/panel.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxButton, wxControl)
#endif

#include "wx/mac/uma.h"
// Button


bool wxButton::Create(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
	Rect bounds ;
	Str255 title ;
	
	MacPreControlCreate( parent , id ,  label , pos , size ,style, validator , name , &bounds , title ) ;

	m_macControl = ::NewControl( parent->MacGetRootWindow() , &bounds , title , false , 0 , 0 , 1, 
	  	kControlPushButtonProc , (long) this ) ;
	wxASSERT_MSG( m_macControl != NULL , "No valid mac control" ) ;
	
	MacPostControlCreate() ;

  return TRUE;
}

void wxButton::SetDefault()
{
    wxWindow *parent = GetParent();
    wxButton *btnOldDefault = NULL;
    if ( parent )
    {
        btnOldDefault = wxDynamicCast(parent->GetDefaultItem(),
           wxButton);
        parent->SetDefaultItem(this);
    }

	Boolean inData;
	if ( btnOldDefault && btnOldDefault->m_macControl )
	{
  		inData = 0;
		::SetControlData( btnOldDefault->m_macControl , kControlButtonPart ,
						   kControlPushButtonDefaultTag , sizeof( Boolean ) , (char*)(&inData) ) ;
	}
	if ( m_macControl )
	{
  		inData = 1;
		::SetControlData( m_macControl , kControlButtonPart ,
						   kControlPushButtonDefaultTag , sizeof( Boolean ) , (char*)(&inData) ) ;
	}
}

wxSize wxButton::DoGetBestSize() const
{
  wxSize sz = GetDefaultSize() ;
  
    int wBtn = m_label.Length() * 8 + 12 ;
	int hBtn = 20 ;
	
  if (wBtn > sz.x) sz.x = wBtn;
  if (hBtn > sz.y) sz.y = hBtn;
  
  return sz ;
}

wxSize wxButton::GetDefaultSize()
{
    int wBtn = 70 /* + 2 * m_macHorizontalBorder */ ; 
	int hBtn = 20 /* +  2 * m_macVerticalBorder */ ;

    return wxSize(wBtn, hBtn);
}

void wxButton::Command (wxCommandEvent & event)
{
	if ( m_macControl )
	{
		HiliteControl( m_macControl , kControlButtonPart ) ;
		unsigned long finalTicks ;
		Delay( 8 , &finalTicks ) ;
		HiliteControl( m_macControl , 0 ) ;
	}
    ProcessCommand (event);
}

void wxButton::MacHandleControlClick( ControlHandle control , SInt16 controlpart ) 
{
  if ( controlpart != kControlNoPart )
  {
    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, m_windowId );
    event.SetEventObject(this);
    ProcessCommand(event);
  }
}

