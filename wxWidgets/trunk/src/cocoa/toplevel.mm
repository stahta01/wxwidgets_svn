///////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/toplevel.mm
// Purpose:     implements wxTopLevelWindow for Cocoa
// Author:      David Elliott 
// Modified by:
// Created:     2002/11/27
// RCS-ID:      $Id:
// Copyright:   (c) 2002 David Elliott
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/toplevel.h"
    #include "wx/menuitem.h"
    #include "wx/frame.h"
    #include "wx/log.h"
    #include "wx/app.h"
#endif //WX_PRECOMP

#include "wx/cocoa/autorelease.h"
#include "wx/cocoa/string.h"
#include "wx/cocoa/mbarman.h"

#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>
// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

// list of all frames and modeless dialogs
wxWindowList       wxModelessWindows;

// ============================================================================
// wxTopLevelWindowCocoa implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxTopLevelWindowCocoa creation
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(wxTopLevelWindowCocoa,wxTopLevelWindowBase)
    EVT_CLOSE(wxTopLevelWindowCocoa::OnCloseWindow)
END_EVENT_TABLE()

void wxTopLevelWindowCocoa::Init()
{
    m_iconized =
    m_maximizeOnShow =
    m_closed = false;
}

bool wxTopLevelWindowCocoa::Create(wxWindow *parent,
                                 wxWindowID winid,
                                 const wxString& title,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxString& name)
{
    wxAutoNSAutoreleasePool pool;
    wxTopLevelWindows.Append(this);

    if(!CreateBase(parent,winid,pos,size,style,wxDefaultValidator,name))
        return FALSE;

    if ( parent )
        parent->AddChild(this);

    // TODO: get rect from given position/size
    NSRect cocoaRect =  NSMakeRect(100,100,200,200);

    // TODO: Set flags given wxWindows style
    unsigned int cocoaStyle = 0;
    cocoaStyle |= NSTitledWindowMask;
    cocoaStyle |= NSClosableWindowMask;
    cocoaStyle |= NSMiniaturizableWindowMask;
    cocoaStyle |= NSResizableWindowMask;

    m_cocoaNSWindow = NULL;
    m_cocoaNSView = NULL;
    SetNSWindow([[NSWindow alloc] initWithContentRect:cocoaRect styleMask:cocoaStyle backing:NSBackingStoreBuffered defer:NO]);
    // NOTE: SetNSWindow has retained the Cocoa object for this object.
    // Because we do not release on close, the following release matches the
    // above alloc and thus the retain count will be 1.
    [m_cocoaNSWindow release];

    [m_cocoaNSWindow setTitle:wxNSStringWithWxString(title)];
    return TRUE;
}

wxTopLevelWindowCocoa::~wxTopLevelWindowCocoa()
{
    wxAutoNSAutoreleasePool pool;
    DestroyChildren();
    SetNSWindow(NULL);
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowCocoa Cocoa Specifics
// ----------------------------------------------------------------------------

void wxTopLevelWindowCocoa::SetNSWindow(WX_NSWindow cocoaNSWindow)
{
    bool need_debug = cocoaNSWindow || m_cocoaNSWindow;
    if(need_debug) wxLogDebug("wxTopLevelWindowCocoa=%p::SetNSWindow [m_cocoaNSWindow=%p retainCount]=%d",this,m_cocoaNSWindow,[m_cocoaNSWindow retainCount]);
    DisassociateNSWindow(m_cocoaNSWindow);
    [cocoaNSWindow retain];
    [m_cocoaNSWindow release];
    m_cocoaNSWindow = cocoaNSWindow;
    if(m_cocoaNSWindow)
        SetNSView([m_cocoaNSWindow contentView]);
    else
        SetNSView(NULL);
    AssociateNSWindow(m_cocoaNSWindow);
    if(need_debug) wxLogDebug("wxTopLevelWindowCocoa=%p::SetNSWindow [cocoaNSWindow=%p retainCount]=%d",this,cocoaNSWindow,[cocoaNSWindow retainCount]);
}

void wxTopLevelWindowCocoa::CocoaReplaceView(WX_NSView oldView, WX_NSView newView)
{
    if([m_cocoaNSWindow contentView] == (id)oldView)
        [m_cocoaNSWindow setContentView:newView];
}

void wxTopLevelWindowCocoa::CocoaDelegate_windowDidBecomeKey(void)
{
    wxLogDebug("wxTopLevelWindowCocoa=%p::CocoaDelegate_windowDidBecomeKey",this);
    wxMenuBarManager::GetInstance()->WindowDidBecomeKey(this);
    wxActivateEvent event(wxEVT_ACTIVATE, TRUE, GetId());
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}

void wxTopLevelWindowCocoa::CocoaDelegate_windowDidResignKey(void)
{
    wxLogDebug("wxTopLevelWindowCocoa=%p::CocoaDelegate_windowDidResignKey",this);
    wxActivateEvent event(wxEVT_ACTIVATE, FALSE, GetId());
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
    wxMenuBarManager::GetInstance()->WindowDidResignKey(this);
}

void wxTopLevelWindowCocoa::CocoaDelegate_windowDidBecomeMain(void)
{
    wxLogDebug("wxTopLevelWindowCocoa=%p::CocoaDelegate_windowDidBecomeMain",this);
    wxMenuBarManager::GetInstance()->WindowDidBecomeMain(this);
}

void wxTopLevelWindowCocoa::CocoaDelegate_windowDidResignMain(void)
{
    wxLogDebug("wxTopLevelWindowCocoa=%p::CocoaDelegate_windowDidResignMain",this);
    wxMenuBarManager::GetInstance()->WindowDidResignMain(this);
}

void wxTopLevelWindowCocoa::Cocoa_close(void)
{
    m_closed = true;
    Destroy();
    /* Be SURE that idle events get ran.  If the window was not active when
    it was closed, then there will be no more events to trigger this and
    therefore it must be done here */
    wxTheApp->CocoaInstallRequestedIdleHandler();
}

bool wxTopLevelWindowCocoa::CocoaDelegate_windowShouldClose()
{
    return wxWindowBase::Close(false);
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowCocoa maximize/minimize
// ----------------------------------------------------------------------------

void wxTopLevelWindowCocoa::Maximize(bool maximize)
{
}

bool wxTopLevelWindowCocoa::IsMaximized() const
{
    return false ; 
}

void wxTopLevelWindowCocoa::Iconize(bool iconize)
{
}

bool wxTopLevelWindowCocoa::IsIconized() const
{
    return FALSE;
}

void wxTopLevelWindowCocoa::Restore()
{
}

bool wxTopLevelWindowCocoa::Show(bool show)
{
    if(m_isShown == show)
        return false;
    wxAutoNSAutoreleasePool pool;
    if(show)
    {
        // Send the window a size event because wxWindows apps expect it
        // NOTE: This should really only be done the first time a window
        // is shown.  I doubt this will cause any problems though.
        wxSizeEvent event(GetSize(), GetId());
        event.SetEventObject(this);
        GetEventHandler()->ProcessEvent(event);

        [m_cocoaNSWindow makeKeyAndOrderFront:m_cocoaNSWindow];
    }
    else
        [m_cocoaNSWindow orderOut:m_cocoaNSWindow];
    m_isShown = show;
    return true;
}

bool wxTopLevelWindowCocoa::Close(bool force)
{
    if(force)
        return wxWindowBase::Close(force);
    // performClose  will fake the user clicking the close button which
    // will invoke windowShouldClose which will call the base class version
    // of Close() which will NOT Destroy() the window (see below) but
    // if closing is not stopped, then performClose will go ahead and
    // close the window which will invoke Cocoa_close() setting m_closed
    // to true and Destroy()ing the window.
    [m_cocoaNSWindow performClose:m_cocoaNSWindow];
    return m_closed;
}

void wxTopLevelWindowCocoa::OnCloseWindow(wxCloseEvent& event)
{
    // If the event was forced, close the window which will Destroy() it
    if(!event.CanVeto())
        [m_cocoaNSWindow close];
    // if the event was not forced, it's probably because the user clicked
    // the close button, or Close(false) was called which (see above) is
    // redirected to performClose and thus Cocoa itself will close the window
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowCocoa misc
// ----------------------------------------------------------------------------

bool wxTopLevelWindowCocoa::ShowFullScreen(bool show, long style)
{
    return FALSE;
}

bool wxTopLevelWindowCocoa::IsFullScreen() const
{
    return FALSE;
}

void wxTopLevelWindowCocoa::CocoaSetWxWindowSize(int width, int height)
{
    // Set the NSView size by setting the frame size to enclose it
    unsigned int styleMask = [m_cocoaNSWindow styleMask];
    NSRect frameRect = [m_cocoaNSWindow frame];
    NSRect contentRect = [NSWindow
        contentRectForFrameRect: frameRect
        styleMask: styleMask];
    contentRect.size.width = width;
    contentRect.size.height = height;
    frameRect = [NSWindow
        frameRectForContentRect: contentRect
        styleMask: styleMask];
    [m_cocoaNSWindow setFrame: frameRect display: NO];
}

void wxTopLevelWindowCocoa::DoMoveWindow(int x, int y, int width, int height)
{
//    wxLogDebug("wxTopLevelWindow=%p::DoMoveWindow(%d,%d,%d,%d)",this,x,y,width,height);

    NSRect cocoaRect = NSMakeRect(x,y,width,height);
    [m_cocoaNSWindow setFrame: cocoaRect display:NO];
}

void wxTopLevelWindowCocoa::DoGetSize(int *w, int *h) const
{
    NSRect cocoaRect = [m_cocoaNSWindow frame];
    if(w)
        *w=(int)cocoaRect.size.width;
    if(h)
        *h=(int)cocoaRect.size.height;
//    wxLogDebug("wxTopLevelWindow=%p::DoGetSize = (%d,%d)",this,(int)cocoaRect.size.width,(int)cocoaRect.size.height);
}

void wxTopLevelWindowCocoa::DoGetPosition(int *x, int *y) const
{
    NSRect cocoaRect = [m_cocoaNSWindow frame];
    if(x)
        *x=(int)cocoaRect.origin.x;
    if(y)
        *y=(int)cocoaRect.origin.y;
//    wxLogDebug("wxTopLevelWindow=%p::DoGetPosition = (%d,%d)",this,(int)cocoaRect.origin.x,(int)cocoaRect.origin.y);
}

