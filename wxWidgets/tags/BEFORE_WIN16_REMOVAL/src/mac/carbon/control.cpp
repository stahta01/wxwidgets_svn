/////////////////////////////////////////////////////////////////////////////
// Name:        control.cpp
// Purpose:     wxControl class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "control.h"
#endif

#include "wx/defs.h"

#include "wx/control.h"
#include "wx/panel.h"
#include "wx/app.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "wx/notebook.h"
#include "wx/tabctrl.h"
#include "wx/radiobox.h"
#include "wx/spinbutt.h"
#include "wx/scrolbar.h"
#include "wx/button.h"
#include "wx/dialog.h"
#include "wx/statbox.h"
#include "wx/sizer.h"
#include "wx/stattext.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)
#endif

#include "wx/mac/uma.h"
#include "wx/mac/private.h"

// Item members

wxControl::wxControl()
{
}

bool wxControl::Create(wxWindow *parent, wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size, long style,
                       const wxValidator& validator,
                       const wxString& name)
{
    bool rval = wxWindow::Create(parent, id, pos, size, style, name);

#if 0
    // no automatic inheritance as we most often need transparent backgrounds
    if ( parent )
    {
        m_backgroundColour = parent->GetBackgroundColour() ;
        m_foregroundColour = parent->GetForegroundColour() ;
    }
#endif

    if (rval) 
    {
#if wxUSE_VALIDATORS
        SetValidator(validator);
#endif
    }
    return rval;
}

wxControl::~wxControl()
{
    m_isBeingDeleted = TRUE;
}

bool wxControl::ProcessCommand (wxCommandEvent & event)
{
    // Tries:
    // 1) OnCommand, starting at this window and working up parent hierarchy
    // 2) OnCommand then calls ProcessEvent to search the event tables.
    return GetEventHandler()->ProcessEvent(event);
}

void  wxControl::OnKeyDown( wxKeyEvent &event ) 
{
    if ( (ControlRef) m_macControl == NULL )
        return ;
    
#if TARGET_CARBON

    char charCode ;
    UInt32 keyCode ;    
    UInt32 modifiers ;

    GetEventParameter( (EventRef) wxTheApp->MacGetCurrentEvent(), kEventParamKeyMacCharCodes, typeChar, NULL,sizeof(char), NULL,&charCode );
    GetEventParameter( (EventRef) wxTheApp->MacGetCurrentEvent(), kEventParamKeyCode, typeUInt32, NULL,  sizeof(UInt32), NULL, &keyCode );
       GetEventParameter((EventRef) wxTheApp->MacGetCurrentEvent(), kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifiers);

    ::HandleControlKey( (ControlRef) m_macControl , keyCode , charCode , modifiers ) ;
    
#else
    EventRecord *ev = (EventRecord*) wxTheApp->MacGetCurrentEvent() ;
    short keycode ;
    short keychar ;
    keychar = short(ev->message & charCodeMask);
    keycode = short(ev->message & keyCodeMask) >> 8 ;

    ::HandleControlKey( (ControlRef) m_macControl , keycode , keychar , ev->modifiers ) ;
#endif
}

