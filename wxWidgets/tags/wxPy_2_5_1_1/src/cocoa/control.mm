/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/control.mm
// Purpose:     wxControl class
// Author:      David Elliiott
// Modified by:
// Created:     2003/02/15
// RCS-ID:      $Id: 
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/control.h"
    #include "wx/log.h"
#endif

#include "wx/cocoa/autorelease.h"

#import <AppKit/NSControl.h>

@interface wxNonControlNSControl : NSControl
{
}

- (void)drawRect: (NSRect)rect;
@end // wxNonControlNSControl

@implementation wxNonControlNSControl : NSControl
- (void)drawRect: (NSRect)rect
{
    wxCocoaNSView *win = wxCocoaNSView::GetFromCocoa(self);
    if( !win || !win->Cocoa_drawRect(rect) )
        [super drawRect:rect];
}
@end // wxNonControlNSControl

IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)
BEGIN_EVENT_TABLE(wxControl, wxControlBase)
END_EVENT_TABLE()
WX_IMPLEMENT_COCOA_OWNER(wxControl,NSControl,NSView,NSView)

bool wxControl::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos, const wxSize& size, long style,
            const wxValidator& validator, const wxString& name)
{
    wxLogTrace(wxTRACE_COCOA,wxT("Creating control with id=%d"),winid);
    if(!CreateControl(parent,winid,pos,size,style,validator,name))
        return false;
    wxLogTrace(wxTRACE_COCOA,wxT("Created control with id=%d"),GetId());
    m_cocoaNSView = NULL;
    SetNSControl([[wxNonControlNSControl alloc] initWithFrame: MakeDefaultNSRect(size)]);
    // NOTE: YES we want to release this (to match the alloc).
    // DoAddChild(this) will retain us again since addSubView doesn't.
    [m_cocoaNSView release];

    [GetNSControl() sizeToFit];

    if(m_parent)
        m_parent->CocoaAddChild(this);
    SetInitialFrameRect(pos,size);

    return true;
}

wxControl::~wxControl()
{
    DisassociateNSControl(GetNSControl());
}

wxSize wxControl::DoGetBestSize() const
{
    wxAutoNSAutoreleasePool pool;
    wxASSERT(m_cocoaNSView);
    NSRect storedRect = [m_cocoaNSView frame];
    [GetNSControl() sizeToFit];
    NSRect cocoaRect = [m_cocoaNSView frame];
    wxSize size((int)cocoaRect.size.width+10,(int)cocoaRect.size.height);
    [m_cocoaNSView setFrame: storedRect];
    wxLogTrace(wxTRACE_COCOA_Window_Size,wxT("wxControl=%p::DoGetBestSize()==(%d,%d)"),this,size.x,size.y);
    return size;
}

bool wxControl::ProcessCommand(wxCommandEvent& event)
{
    return GetEventHandler()->ProcessEvent(event);
}

void wxControl::CocoaSetEnabled(bool enable)
{
    [GetNSControl() setEnabled: enable];
}

