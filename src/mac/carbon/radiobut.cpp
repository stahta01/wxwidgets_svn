/////////////////////////////////////////////////////////////////////////////
// Name:        radiobut.cpp
// Purpose:     wxRadioButton
// Author:      AUTHOR
// Modified by: JS Lair (99/11/15) adding the cyclic groupe notion for radiobox
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:       wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "radiobut.h"
#endif

#include "wx/defs.h"

#include "wx/radiobut.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxRadioButton, wxControl)
#endif

#include "wx/mac/uma.h"

bool wxRadioButton::Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
    m_macIsUserPane = FALSE ;
    
    if ( !wxControl::Create(parent, id, pos, size, style, validator, name) )
        return false;
    
    m_label = label ;

    Rect bounds = wxMacGetBoundsForControl( this , pos , size ) ;
    
    verify_noerr ( CreateRadioButtonControl( MAC_WXHWND(parent->MacGetTopLevelWindowRef()) , &bounds , CFSTR("") , 
        0 , false /* no autotoggle */ , (ControlRef*) &m_macControl ) ) ;

    MacPostControlCreate(pos,size) ;

  m_cycle = this ;
  
  if (HasFlag(wxRB_GROUP))
  {
      AddInCycle( NULL ) ;
  }
  else
  {
    /* search backward for last group start */
    wxRadioButton *chief = (wxRadioButton*) NULL;
    wxWindowList::Node *node = parent->GetChildren().GetLast();
    while (node)
    {
      wxWindow *child = node->GetData();
      if (child->IsKindOf( CLASSINFO( wxRadioButton ) ) )
      {
          chief = (wxRadioButton*) child;
         if (child->HasFlag(wxRB_GROUP)) break;
      }
      node = node->GetPrevious();
    }
    AddInCycle( chief ) ;
  }
    return TRUE;
}

void wxRadioButton::SetValue(bool val)
{
    wxRadioButton *cycle;
      if ( GetControl32BitValue( (ControlRef) m_macControl ) == val )
        return ;
        
   ::SetControl32BitValue( (ControlRef) m_macControl , val ) ;
   if (val) 
   {
           cycle=this->NextInCycle();
          if (cycle!=NULL) {
               while (cycle!=this) {
                   cycle->SetValue(false);
                   cycle=cycle->NextInCycle();
                   }
               }
           }
   MacRedrawControl() ;
}

bool wxRadioButton::GetValue() const
{
    return ::GetControl32BitValue( (ControlRef) m_macControl ) ;
}

void wxRadioButton::Command (wxCommandEvent & event)
{
  SetValue ( (event.GetInt() != 0) );
  ProcessCommand (event);
}

wxInt32 wxRadioButton::MacControlHit(WXEVENTHANDLERREF WXUNUSED(handler) , WXEVENTREF WXUNUSED(event) )  
{
    // if already set -> no action
    if ( GetValue() )
      return noErr;
      
      wxRadioButton *cycle, *old = NULL ;
    cycle=this->NextInCycle();
    if (cycle!=NULL) {
          while (cycle!=this) {
            if ( cycle->GetValue() ) {
              old = cycle ;
                cycle->SetValue(false);
              }
              cycle=cycle->NextInCycle();
            }
    }

      SetValue(true) ;

    if ( old ) {
      wxCommandEvent event(wxEVT_COMMAND_RADIOBUTTON_SELECTED, old->m_windowId );
      event.SetEventObject(old);
      event.SetInt( false );
      old->ProcessCommand(event);
    }
    wxCommandEvent event2(wxEVT_COMMAND_RADIOBUTTON_SELECTED, m_windowId );
    event2.SetEventObject(this);
    event2.SetInt( true );
    ProcessCommand(event2);
    return noErr ;
}

wxRadioButton *wxRadioButton::AddInCycle(wxRadioButton *cycle)
{
    wxRadioButton *next,*current;
        
    if (cycle==NULL) {
        m_cycle=this;
        return(this);
        }
    else {
        current=cycle;
          while ((next=current->m_cycle)!=cycle) 
            current=current->m_cycle;
          m_cycle=cycle;
          current->m_cycle=this;
          return(cycle);
      }
}  
