/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/NSWindow.mm
// Purpose:     wxCocoaNSWindow
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id:
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/menuitem.h"
#endif // WX_PRECOMP

#include "wx/cocoa/ObjcPose.h"
#include "wx/cocoa/NSWindow.h"

#import <AppKit/NSWindow.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSString.h>

// ============================================================================
// @class wxNSWindowDelegate
// ============================================================================
@interface wxNSWindowDelegate : NSObject
{
}

- (void)windowDidBecomeKey: (NSNotification *)notification;
- (void)windowDidResignKey: (NSNotification *)notification;
- (void)windowDidBecomeMain: (NSNotification *)notification;
- (void)windowDidResignMain: (NSNotification *)notification;
- (BOOL)windowShouldClose: (id)sender;
- (void)windowWillClose: (NSNotification *)notification;
@end //interface wxNSWindowDelegate

@implementation wxNSWindowDelegate : NSObject

- (void)windowDidBecomeKey: (NSNotification *)notification
{
    wxCocoaNSWindow *win = wxCocoaNSWindow::GetFromCocoa([notification object]);
    wxCHECK_RET(win,wxT("notificationDidBecomeKey received but no wxWindow exists"));
    win->CocoaDelegate_windowDidBecomeKey();
}

- (void)windowDidResignKey: (NSNotification *)notification
{
    wxCocoaNSWindow *win = wxCocoaNSWindow::GetFromCocoa([notification object]);
    wxCHECK_RET(win,wxT("notificationDidResignKey received but no wxWindow exists"));
    win->CocoaDelegate_windowDidResignKey();
}

- (void)windowDidBecomeMain: (NSNotification *)notification
{
    wxCocoaNSWindow *win = wxCocoaNSWindow::GetFromCocoa([notification object]);
    wxCHECK_RET(win,wxT("notificationDidBecomeMain received but no wxWindow exists"));
    win->CocoaDelegate_windowDidBecomeMain();
}

- (void)windowDidResignMain: (NSNotification *)notification
{
    wxCocoaNSWindow *win = wxCocoaNSWindow::GetFromCocoa([notification object]);
    wxCHECK_RET(win,wxT("notificationDidResignMain received but no wxWindow exists"));
    win->CocoaDelegate_windowDidResignMain();
}

- (BOOL)windowShouldClose: (id)sender
{
    wxLogDebug(wxT("windowShouldClose"));
    wxCocoaNSWindow *tlw = wxCocoaNSWindow::GetFromCocoa(sender);
    if(tlw && !tlw->CocoaDelegate_windowShouldClose())
    {
        wxLogDebug(wxT("Window will not be closed"));
        return NO;
    }
    wxLogDebug(wxT("Window will be closed"));
    return YES;
}

- (void)windowWillClose: (NSNotification *)notification
{
    wxCocoaNSWindow *win = wxCocoaNSWindow::GetFromCocoa([notification object]);
    wxCHECK_RET(win,wxT("windowWillClose received but no wxWindow exists"));
    win->CocoaDelegate_windowWillClose();
}

@end //implementation wxNSWindowDelegate

// ============================================================================
// class wxCocoaNSWindow
// ============================================================================

WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(NSWindow)

struct objc_object *wxCocoaNSWindow::sm_cocoaDelegate = [[wxNSWindowDelegate alloc] init];

void wxCocoaNSWindow::AssociateNSWindow(WX_NSWindow cocoaNSWindow)
{
    if(cocoaNSWindow)
    {
        [cocoaNSWindow setReleasedWhenClosed: NO];
        sm_cocoaHash.insert(wxCocoaNSWindowHash::value_type(cocoaNSWindow,this));
        [cocoaNSWindow setDelegate: sm_cocoaDelegate];
    }
}

void wxCocoaNSWindow::DisassociateNSWindow(WX_NSWindow cocoaNSWindow)
{
    if(cocoaNSWindow)
    {
        [cocoaNSWindow setDelegate: nil];
        sm_cocoaHash.erase(cocoaNSWindow);
    }
}

wxMenuBar* wxCocoaNSWindow::GetAppMenuBar(wxCocoaNSWindow *win)
{
    return NULL;
}

// ============================================================================
// @class wxPoserNSWindow
// ============================================================================
@interface wxPoserNSWindow : NSWindow
{
}

- (BOOL)canBecomeMainWindow;
@end // wxPoserNSwindow

WX_IMPLEMENT_POSER(wxPoserNSWindow);
@implementation wxPoserNSWindow : NSWindow

- (BOOL)canBecomeMainWindow
{
    bool canBecome = false;
    wxCocoaNSWindow *tlw = wxCocoaNSWindow::GetFromCocoa(self);
    if(!tlw || !tlw->Cocoa_canBecomeMainWindow(canBecome))
        canBecome = [super canBecomeMainWindow];
    return canBecome;
}

@end // implementation wxPoserNSWindow

