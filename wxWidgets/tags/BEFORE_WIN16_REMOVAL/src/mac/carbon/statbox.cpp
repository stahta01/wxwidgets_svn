/////////////////////////////////////////////////////////////////////////////
// Name:        statbox.cpp
// Purpose:     wxStaticBox
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "statbox.h"
#endif

#include "wx/defs.h"

#include "wx/statbox.h"
#include "wx/mac/uma.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxStaticBox, wxControl)
#endif

/*
 * Static box
 */
 
bool wxStaticBox::Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    m_macIsUserPane = FALSE ;
    
    if ( !wxControl::Create(parent, id, pos, size,
                            style, wxDefaultValidator, name) )
        return false;

    m_label = label ;

    Rect bounds = wxMacGetBoundsForControl( this , pos , size ) ;
    
    m_macControl = (WXWidget) ::NewControl( MAC_WXHWND(parent->MacGetTopLevelWindowRef()) , &bounds , "\p" , true , 0 , 0 , 1, 
        kControlGroupBoxTextTitleProc , (long) this ) ;
    
    MacPostControlCreate(pos,size) ;
    
    return TRUE;
}
