/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/dialog.mm
// Purpose:     wxDialog class
// Author:      David Elliott
// Modified by:
// Created:     2002/12/15
// RCS-ID:      $Id: 
// Copyright:   2002 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/app.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
#endif //WX_PRECOMP

#include "wx/cocoa/autorelease.h"
#include "wx/cocoa/string.h"

#import <AppKit/NSPanel.h>
#import <AppKit/NSApplication.h>
#import <AppKit/NSEvent.h>
#import <Foundation/NSRunLoop.h>

// Lists to keep track of windows, so we can disable/enable them
// for modal dialogs
static wxWindowList wxModalDialogs;

IMPLEMENT_DYNAMIC_CLASS(wxDialog, wxTopLevelWindow)

BEGIN_EVENT_TABLE(wxDialog, wxDialogBase)
  EVT_BUTTON(wxID_OK, wxDialog::OnOK)
  EVT_BUTTON(wxID_APPLY, wxDialog::OnApply)
  EVT_BUTTON(wxID_CANCEL, wxDialog::OnCancel)
  EVT_CLOSE(wxDialog::OnCloseWindow)
END_EVENT_TABLE()

WX_IMPLEMENT_COCOA_OWNER(wxDialog,NSPanel,NSWindow,NSWindow)

void wxDialog::Init()
{
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));
}

bool wxDialog::Create(wxWindow *parent, wxWindowID winid,
           const wxString& title,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    wxAutoNSAutoreleasePool pool;
    wxTopLevelWindows.Append(this);

    if(!CreateBase(parent,winid,pos,size,style,wxDefaultValidator,name))
        return false;

    if (parent)
        parent->AddChild(this);

    NSRect cocoaRect = NSMakeRect(300,300,200,200);

    unsigned int cocoaStyle = 0;
    cocoaStyle |= NSTitledWindowMask;
    cocoaStyle |= NSClosableWindowMask;
    cocoaStyle |= NSMiniaturizableWindowMask;
    cocoaStyle |= NSResizableWindowMask;

    m_cocoaNSWindow = NULL;
    SetNSPanel([[NSPanel alloc] initWithContentRect:cocoaRect styleMask:cocoaStyle backing:NSBackingStoreBuffered defer:NO]);
    // NOTE: SetNSWindow has retained the Cocoa object for this object.
    // Because we do not release on close, the following release matches the
    // above alloc and thus the retain count will be 1.
    [m_cocoaNSWindow release];
    wxLogDebug(wxT("wxDialog m_cocoaNSWindow retainCount=%d"),[m_cocoaNSWindow retainCount]);
    [m_cocoaNSWindow setTitle:wxNSStringWithWxString(title)];
    [m_cocoaNSWindow setHidesOnDeactivate:NO];

    return true;
}

wxDialog::~wxDialog()
{
    wxLogDebug(wxT("Destroying"));
    // setReleasedWhenClosed: NO
    [m_cocoaNSWindow close];
    DisassociateNSPanel(GetNSPanel());
}

void wxDialog::CocoaDelegate_windowWillClose(void)
{
    m_closed = true;
    /* Actually, this isn't true anymore */
    wxLogDebug(wxT("Woah: Dialogs are not generally closed"));
}

void wxDialog::SetModal(bool flag)
{
    if ( flag )
    {
        wxModelessWindows.DeleteObject(this);
        m_windowStyle |= wxDIALOG_MODAL ;
    }
    else
    {
        m_windowStyle &= ~wxDIALOG_MODAL ;
        wxModelessWindows.Append(this);
    }
}

bool wxDialog::Show(bool show)
{
    if(m_isShown == show)
        return false;
    if(show)
        InitDialog();
    if(IsModal())
    {
        m_isShown = show;
        if(show)
        {
            wxAutoNSAutoreleasePool pool;
            wxModalDialogs.Append(this);
            wxLogDebug(wxT("runModal"));
            NSApplication *theNSApp = wxTheApp->GetNSApplication();
            // If the app hasn't started, flush the event queue
            // If we don't do this, the Dock doesn't get the message that
            // the app has started so will refuse to activate it.
            if(![theNSApp isRunning])
            {
                while(NSEvent *event = [theNSApp
                            nextEventMatchingMask:NSAnyEventMask
                            untilDate:[NSDate distantPast]
                            inMode:NSDefaultRunLoopMode
                            dequeue: YES])
                {
                    [theNSApp sendEvent: event];
                }
            }
            [wxTheApp->GetNSApplication() runModalForWindow:m_cocoaNSWindow];
            wxLogDebug(wxT("runModal END"));
        }
        else
        {
            wxLogDebug(wxT("abortModal"));
            [wxTheApp->GetNSApplication() abortModal];
            wxModalDialogs.DeleteObject(this);
        }
    }
    else
        return wxTopLevelWindow::Show(show);
    return true;
}

// Replacement for Show(TRUE) for modal dialogs - returns return code
int wxDialog::ShowModal()
{
    if(!IsModal())
        SetModal(true);
    Show(true);
    return GetReturnCode();
}

// EndModal will work for any dialog
void wxDialog::EndModal(int retCode)
{
    SetReturnCode(retCode);
    Show(false);
}

bool wxDialog::IsModal() const
{
    return (GetWindowStyleFlag() & wxDIALOG_MODAL);
}

void wxDialog::OnCloseWindow(wxCloseEvent& event)
{
    // We'll send a Cancel message by default,
    // which may close the dialog.
    // Check for looping if the Cancel event handler calls Close().

    // Note that if a cancel button and handler aren't present in the dialog,
    // nothing will happen when you close the dialog via the window manager, or
    // via Close().
    // We wouldn't want to destroy the dialog by default, since the dialog may have been
    // created on the stack.
    // However, this does mean that calling dialog->Close() won't delete the dialog
    // unless the handler for wxID_CANCEL does so. So use Destroy() if you want to be
    // sure to destroy the dialog.
    // The default OnCancel (above) simply ends a modal dialog, and hides a modeless dialog.
    // ALWAYS VETO THIS EVENT!!!!
    event.Veto();

    static wxList closing;
    
    if ( closing.Member(this) )
    {
        wxLogDebug(wxT("WARNING: Attempting to recursively call Close for dialog"));
        return;
    }
    
    closing.Append(this);
    
    wxLogDebug(wxT("Sending Cancel Event"));
    wxCommandEvent cancelEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
    cancelEvent.SetEventObject( this );
    GetEventHandler()->ProcessEvent(cancelEvent); // This may close the dialog

    closing.DeleteObject(this);
}

// Standard buttons
void wxDialog::OnOK(wxCommandEvent& event)
{
    if ( Validate() && TransferDataFromWindow() )
    {
        EndModal(wxID_OK);
    }
}

void wxDialog::OnApply(wxCommandEvent& event)
{
	if (Validate())
		TransferDataFromWindow();
	// TODO probably need to disable the Apply button until things change again
}

void wxDialog::OnCancel(wxCommandEvent& event)
{
    wxLogDebug(wxT("Cancelled!"));
    EndModal(wxID_CANCEL);
}

