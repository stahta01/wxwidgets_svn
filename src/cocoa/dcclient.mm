/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/dcclient.mm
// Purpose:     wxWindowDC, wxPaintDC, and wxClientDC classes
// Author:      David Elliott
// Modified by:
// Created:     2003/04/01
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/window.h"
    #include "wx/dcclient.h"
#endif //WX_PRECOMP

#import <AppKit/NSView.h>
#import <AppKit/NSAffineTransform.h>
#import <AppKit/NSColor.h>
#import <AppKit/NSGraphicsContext.h>
#import <AppKit/NSBezierPath.h>
#import <AppKit/NSWindow.h>

/*
 * wxWindowDC
 */
IMPLEMENT_DYNAMIC_CLASS(wxWindowDC, wxDC)

wxWindowDC::wxWindowDC(void)
:   m_window(NULL)
,   m_lockedNSView(NULL)
{
};

wxWindowDC::wxWindowDC( wxWindow *window )
:   m_window(window)
,   m_lockedNSView(NULL)
{
    wxLogDebug(wxT("non-client window DC's are not supported, oh well"));
};

wxWindowDC::~wxWindowDC(void)
{
    CocoaUnwindStackAndLoseFocus();
};

bool wxWindowDC::CocoaLockFocusOnNSView(WX_NSView nsview)
{
    if([nsview lockFocusIfCanDraw])
    {
        sm_cocoaDCStack.Insert(this);
        m_cocoaFlipped = [nsview isFlipped];
        m_cocoaHeight = [nsview bounds].size.height;
        CocoaApplyTransformations();
        m_lockedNSView = nsview;
        return true;
    }
    wxLogDebug(wxT("focus lock failed!"));
    return false;
}

bool wxWindowDC::CocoaUnlockFocusOnNSView()
{
    [[m_lockedNSView window] flushWindow];
    [m_lockedNSView unlockFocus];
    m_lockedNSView = NULL;
    return true;
}

bool wxWindowDC::CocoaLockFocus()
{
    wxLogDebug(wxT("Locking focus on wxWindowDC=%p, NSView=%p"),this, m_window->GetNonClientNSView());
    return CocoaLockFocusOnNSView(m_window->GetNonClientNSView());
}

bool wxWindowDC::CocoaUnlockFocus()
{
    wxLogDebug(wxT("Unlocking focus on wxWindowDC=%p, NSView=%p"),this, m_window->GetNonClientNSView());
    return CocoaUnlockFocusOnNSView();
}

void wxWindowDC::Clear()
{
    if(!CocoaTakeFocus()) return;

    NSGraphicsContext *context = [NSGraphicsContext currentContext];
    [context saveGraphicsState];

    [m_backgroundBrush.GetNSColor() set];
    [NSBezierPath fillRect:[m_lockedNSView bounds]];

    [context restoreGraphicsState];
}

/*
 * wxClientDC
 */
IMPLEMENT_DYNAMIC_CLASS(wxClientDC, wxWindowDC)

wxClientDC::wxClientDC(void)
{
};

wxClientDC::wxClientDC( wxWindow *window )
{
    m_window = window;
};

wxClientDC::~wxClientDC(void)
{
    CocoaUnwindStackAndLoseFocus();
};

bool wxClientDC::CocoaLockFocus()
{
    wxLogDebug(wxT("Locking focus on wxClientDC=%p, NSView=%p"),this, m_window->GetNSView());
    return CocoaLockFocusOnNSView(m_window->GetNSView());
}

bool wxClientDC::CocoaUnlockFocus()
{
    wxLogDebug(wxT("Unlocking focus on wxClientDC=%p, NSView=%p"),this, m_window->GetNSView());
    return CocoaUnlockFocusOnNSView();
}

/*
 * wxPaintDC
 */
IMPLEMENT_DYNAMIC_CLASS(wxPaintDC, wxWindowDC)

wxPaintDC::wxPaintDC(void)
{
};

wxPaintDC::wxPaintDC( wxWindow *window )
{
    m_window = window;
    wxASSERT_MSG([NSView focusView]==window->GetNSView(), wxT("PaintDC's NSView does not have focus.  Please use wxPaintDC only as the first DC created in a paint handler"));
    sm_cocoaDCStack.Insert(this);
    m_lockedNSView = window->GetNSView();
    m_cocoaFlipped = [window->GetNSView() isFlipped];
    m_cocoaHeight = [window->GetNSView() bounds].size.height;
    CocoaApplyTransformations();
};

wxPaintDC::~wxPaintDC(void)
{
    CocoaUnwindStackAndLoseFocus();
};

bool wxPaintDC::CocoaLockFocus()
{
    wxFAIL_MSG(wxT("wxPaintDC cannot be asked to lock focus!"));
    return false;
}

bool wxPaintDC::CocoaUnlockFocus()
{
    // wxPaintDC focus can never be unlocked.
    return false;
}

