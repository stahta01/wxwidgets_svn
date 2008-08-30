/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/cocoa/checkbox.mm
// Purpose:     wxCheckBox
// Author:      Stefan Csomor
// Modified by:
// Created:     2008-08-20
// RCS-ID:      $Id: checkbox.mm 54129 2008-06-11 19:30:52Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_CHECKBOX

#include "wx/checkbox.h"
#include "wx/osx/private.h"

wxWidgetImplType* wxWidgetImpl::CreateCheckBox( wxWindowMac* wxpeer, 
                                    wxWindowMac* parent, 
                                    wxWindowID id, 
                                    const wxString& label,
                                    const wxPoint& pos, 
                                    const wxSize& size,
                                    long style, 
                                    long extraStyle) 
{
    NSView* sv = (wxpeer->GetParent()->GetHandle() );
    
    NSRect r = wxOSXGetFrameForControl( wxpeer, pos , size ) ;
    wxNSButton* v = [[wxNSButton alloc] initWithFrame:r];

    [v setButtonType:NSSwitchButton];    
    if (style & wxCHK_3STATE)
        [v setAllowsMixedState:YES];
        
    [sv addSubview:v];
    wxWidgetCocoaImpl* c = new wxWidgetCocoaImpl( wxpeer, v );
    [v setImplementation:c];
    return c;
}

#endif
