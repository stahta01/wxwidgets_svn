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

#include "wx/button.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxButton, wxControl)
#endif

#include <wx/mac/uma.h>
// Button


bool wxButton::Create(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
	Rect bounds ;
	Str255 title ;
	m_macHorizontalBorder = 2 ; // additional pixels around the real control
	m_macVerticalBorder = 2 ;
	
	MacPreControlCreate( parent , id ,  label , pos , size ,style, validator , name , &bounds , title ) ;

	m_macControl = UMANewControl( parent->GetMacRootWindow() , &bounds , title , true , 0 , 0 , 1, 
	  	kControlPushButtonProc , (long) this ) ;
	wxASSERT_MSG( m_macControl != NULL , "No valid mac control" ) ;
	
	MacPostControlCreate() ;

  return TRUE;
}

void wxButton::SetDefault()
{
  wxWindow *parent = (wxWindow *)GetParent();
  if (parent)
      parent->SetDefaultItem(this);

  if ( m_macControl )
  {
		UMASetControlData( m_macControl , kControlButtonPart , kControlPushButtonDefaultTag , sizeof( Boolean ) , (char*)((Boolean)1) ) ;
	}
}

void wxButton::Command (wxCommandEvent & event)
{
    ProcessCommand (event);
}

void wxButton::MacHandleControlClick( ControlHandle control , SInt16 controlpart ) 
{
    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, m_windowId );
    event.SetEventObject(this);
    ProcessCommand(event);
}

