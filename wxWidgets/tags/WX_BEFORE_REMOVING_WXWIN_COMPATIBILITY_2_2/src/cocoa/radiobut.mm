/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/radiobut.mm
// Purpose:     wxRadioButton
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id: 
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/app.h"
#include "wx/radiobut.h"
#include "wx/log.h"

#import <AppKit/NSButton.h>
#include "wx/cocoa/string.h"
#include "wx/cocoa/autorelease.h"

IMPLEMENT_DYNAMIC_CLASS(wxRadioButton, wxControl)
// wxRadioButtonBase == wxControl
BEGIN_EVENT_TABLE(wxRadioButton, wxControl)
END_EVENT_TABLE()
WX_IMPLEMENT_COCOA_OWNER(wxRadioButton,NSButton,NSControl,NSView)

bool wxRadioButton::Create(wxWindow *parent, wxWindowID winid,
           const wxString& label,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxValidator& validator,
           const wxString& name)
{
    wxAutoNSAutoreleasePool pool;
    if(!CreateControl(parent,winid,pos,size,style,validator,name))
        return false;
    m_cocoaNSView = NULL;
    SetNSButton([[NSButton alloc] initWithFrame: MakeDefaultNSRect(size)]);
    [m_cocoaNSView release];
    [GetNSButton() setButtonType: NSRadioButton];
    [GetNSButton() setTitle:wxNSStringWithWxString(label)];
    [GetNSControl() sizeToFit];

    if(m_parent)
        m_parent->CocoaAddChild(this);
    SetInitialFrameRect(pos,size);

    return true;
}

wxRadioButton::~wxRadioButton()
{
    DisassociateNSButton(m_cocoaNSView);
}

void wxRadioButton::SetValue(bool)
{
}

bool wxRadioButton::GetValue() const
{
    return false;
}

void wxRadioButton::Cocoa_wxNSButtonAction(void)
{
    wxLogDebug("wxRadioButton");
}

