/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/statline.mm
// Purpose:     wxStaticLine
// Author:      David Elliott
// Modified by:
// Created:     2003/02/15
// RCS-ID:      $Id: 
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/app.h"
#include "wx/statline.h"

#import <AppKit/NSView.h>

IMPLEMENT_DYNAMIC_CLASS(wxStaticLine, wxControl)
BEGIN_EVENT_TABLE(wxStaticLine, wxStaticLineBase)
END_EVENT_TABLE()
// WX_IMPLEMENT_COCOA_OWNER(wxStaticLine,NSTextField,NSControl,NSView)

bool wxStaticLine::Create(wxWindow *parent, wxWindowID winid,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    if(!CreateControl(parent,winid,pos,size,style,wxDefaultValidator,name))
        return false;
    SetNSView([[NSView alloc] initWithFrame: MakeDefaultNSRect(size)]);
    [m_cocoaNSView release];
    if(m_parent)
        m_parent->CocoaAddChild(this);
    SetInitialFrameRect(pos,size);

    return true;
}

wxStaticLine::~wxStaticLine()
{
}

