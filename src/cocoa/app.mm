/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/app.mm
// Purpose:     wxApp
// Author:      David Elliott
// Modified by:
// Created:     2002/11/27
// RCS-ID:      $Id$
// Copyright:   (c) David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/dialog.h"
    #include "wx/dc.h"
    #include "wx/intl.h"
    #include "wx/log.h"
#endif

#include "wx/module.h"

#include "wx/cocoa/ObjcPose.h"
#include "wx/cocoa/autorelease.h"
#include "wx/cocoa/mbarman.h"

#if wxUSE_WX_RESOURCES
#  include "wx/resource.h"
#endif

#import <AppKit/NSApplication.h>
#import <Foundation/NSRunLoop.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSAutoreleasePool.h>
#import <Foundation/NSThread.h>
#import <AppKit/NSEvent.h>

// ========================================================================
// wxPoseAsInitializer
// ========================================================================
wxPoseAsInitializer *wxPoseAsInitializer::sm_first = NULL;

// ========================================================================
// wxPoserNSApplication
// ========================================================================
@interface wxPoserNSApplication : NSApplication
{
}

- (void)sendEvent: (NSEvent*)anEvent;
@end // wxPoserNSApplication

WX_IMPLEMENT_POSER(wxPoserNSApplication);

@implementation wxPoserNSApplication : NSApplication

- (void)sendEvent: (NSEvent*)anEvent
{
    wxLogDebug("SendEvent");
    wxTheApp->CocoaInstallRequestedIdleHandler();
    [super sendEvent: anEvent];
}

@end // wxPoserNSApplication

// ========================================================================
// wxNSApplicationDelegate
// ========================================================================
@interface wxNSApplicationDelegate : NSObject
{
}

- (void)doIdle: (id)data;
// Delegate methods
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (void)applicationWillBecomeActive:(NSNotification *)notification;
- (void)applicationDidBecomeActive:(NSNotification *)notification;
- (void)applicationWillResignActive:(NSNotification *)notification;
- (void)applicationDidResignActive:(NSNotification *)notification;
@end // interface wxNSApplicationDelegate : NSObject

@implementation wxNSApplicationDelegate : NSObject

- (void)doIdle: (id)data
{
    wxASSERT(wxTheApp);
    wxASSERT(wxMenuBarManager::GetInstance());
    wxLogDebug("doIdle called");
#ifdef __WXDEBUG__
    if(wxTheApp->IsInAssert())
    {
        wxLogDebug("Idle events ignored durring assertion dialog");
    }
    else
#endif
    {
        NSRunLoop *rl = [NSRunLoop currentRunLoop];
        // runMode: beforeDate returns YES if something was done
        while(wxTheApp->ProcessIdle()) // FIXME: AND NO EVENTS ARE PENDING
        {
            wxLogDebug("Looping for idle events");
            #if 1
            if( [rl runMode:[rl currentMode] beforeDate:[NSDate distantPast]])
            {
                wxLogDebug("Found actual work to do");
                break;
            }
            #endif
        }
    }
    wxLogDebug("Idle processing complete, requesting next idle event");
    // Add ourself back into the run loop (on next event) if necessary
    wxTheApp->CocoaRequestIdle();
}

// NOTE: Terminate means that the event loop does NOT return and thus
// cleanup code doesn't properly execute.  Furthermore, wxWindows has its
// own exit on frame delete mechanism.
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return NO;
}

- (void)applicationWillBecomeActive:(NSNotification *)notification
{
    wxTheApp->CocoaDelegate_applicationWillBecomeActive();
}

- (void)applicationDidBecomeActive:(NSNotification *)notification
{
    wxTheApp->CocoaDelegate_applicationDidBecomeActive();
}

- (void)applicationWillResignActive:(NSNotification *)notification
{
    wxTheApp->CocoaDelegate_applicationWillResignActive();
}

- (void)applicationDidResignActive:(NSNotification *)notification
{
    wxTheApp->CocoaDelegate_applicationDidResignActive();
}

@end // implementation wxNSApplicationDelegate : NSObject

// ========================================================================
// wxApp
// ========================================================================

// ----------------------------------------------------------------------------
// wxApp Static member initialization
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)
BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxAppBase::OnIdle)
//    EVT_END_SESSION(wxApp::OnEndSession)
//    EVT_QUERY_END_SESSION(wxApp::OnQueryEndSession)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxApp initialization/cleanup
// ----------------------------------------------------------------------------
bool wxApp::Initialize(int& argc, wxChar **argv)
{
    wxAutoNSAutoreleasePool pool;
    m_cocoaMainThread = [NSThread currentThread];
    // Mac OS X passes a process serial number command line argument when
    // the application is launched from the Finder. This argument must be
    // removed from the command line arguments before being handled by the
    // application (otherwise applications would need to handle it)
    if ( argc > 1 )
    {
        static const wxChar *ARG_PSN = _T("-psn_");
        if ( wxStrncmp(argv[1], ARG_PSN, strlen(ARG_PSN)) == 0 )
        {
            // remove this argument
            --argc;
            memmove(argv + 1, argv + 2, argc * sizeof(char *));
        }
    }

    // Posing must be completed before any instances of the Objective-C
    // classes being posed as are created.
    wxPoseAsInitializer::InitializePosers();

    return wxAppBase::Initialize(argc, argv);
}

void wxApp::CleanUp()
{
    wxAutoNSAutoreleasePool pool;

    wxDC::CocoaShutdownTextSystem();
    wxMenuBarManager::DestroyInstance();

    [m_cocoaApp setDelegate:nil];
    [m_cocoaAppDelegate release];
    m_cocoaAppDelegate = NULL;

    wxAppBase::CleanUp();
}

// ----------------------------------------------------------------------------
// wxApp creation
// ----------------------------------------------------------------------------
wxApp::wxApp()
{
    m_topWindow = NULL;

    m_isIdle = true;
#if WXWIN_COMPATIBILITY_2_2
    m_wantDebugOutput = TRUE;
#endif
#ifdef __WXDEBUG__
    m_isInAssert = FALSE;
#endif // __WXDEBUG__

    argc = 0;
    argv = NULL;
    m_cocoaApp = NULL;
    m_cocoaAppDelegate = NULL;
}

void wxApp::CocoaInstallIdleHandler()
{
    // If we're not the main thread, don't install the idle handler
    if(m_cocoaMainThread != [NSThread currentThread])
    {
        wxLogDebug("Attempt to install idle handler from secondary thread");
        return;
    }
    // If we're supposed to be stopping, don't add more idle events
    if(![m_cocoaApp isRunning])
        return;
    wxLogDebug("wxApp::CocoaInstallIdleHandler");
    m_isIdle = false;
    // Call doIdle for EVERYTHING dammit
// We'd need Foundation/NSConnection.h for this next constant, do we need it?
    [[ NSRunLoop currentRunLoop ] performSelector:@selector(doIdle:) target:m_cocoaAppDelegate argument:NULL order:0 modes:[NSArray arrayWithObjects:NSDefaultRunLoopMode, /* NSConnectionReplyRunLoopMode,*/ NSModalPanelRunLoopMode, /**/NSEventTrackingRunLoopMode,/**/ nil] ];
    /* Notes:
    In the Mac OS X implementation of Cocoa, the above method schedules
    doIdle: to be called from *within* [NSApplication
    -nextEventMatchingMask:untilDate:inMode:dequeue:].  That is, no
    NSEvent object is generated and control does not return from that
    method.  In fact, control will only return from that method for the
    usual reasons (e.g. a real event is received or the untilDate is reached).
    This has implications when trying to stop the event loop and return to
    its caller.  See wxEventLoop::Exit
    */
}

void wxApp::CocoaDelegate_applicationWillBecomeActive()
{
}

void wxApp::CocoaDelegate_applicationDidBecomeActive()
{
}

void wxApp::CocoaDelegate_applicationWillResignActive()
{
}

void wxApp::CocoaDelegate_applicationDidResignActive()
{
}

bool wxApp::OnInitGui()
{
    wxAutoNSAutoreleasePool pool;
    if(!wxAppBase::OnInitGui())
        return FALSE;

    // Create the app using the sharedApplication method
    m_cocoaApp = [NSApplication sharedApplication];
    m_cocoaAppDelegate = [[wxNSApplicationDelegate alloc] init];
    [m_cocoaApp setDelegate:m_cocoaAppDelegate];

    wxMenuBarManager::CreateInstance();

    wxDC::CocoaInitializeTextSystem();
//    [ m_cocoaApp setDelegate:m_cocoaApp ];
    #if 0
    wxLogDebug("Just for kicks");
    [ m_cocoaAppDelegate performSelector:@selector(doIdle:) withObject:NULL ];
    wxLogDebug("okay.. done now");
    #endif
    return TRUE;
}

bool wxApp::CallOnInit()
{
//    wxAutoNSAutoreleasePool pool;
    return OnInit();
}

bool wxApp::OnInit()
{
    if(!wxAppBase::OnInit())
        return FALSE;

    return TRUE;
}

void wxApp::Exit()
{
    wxApp::CleanUp();

    wxAppConsole::Exit();
}

// Yield to other processes
bool wxApp::Yield(bool onlyIfNeeded)
{
    // MT-FIXME
    static bool s_inYield = false;

#if wxUSE_LOG
    // disable log flushing from here because a call to wxYield() shouldn't
    // normally result in message boxes popping up &c
    wxLog::Suspend();
#endif // wxUSE_LOG

    if (s_inYield)
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return false;
    }

    s_inYield = true;

    // Run the event loop until it is out of events
    while(NSEvent *event = [GetNSApplication()
                nextEventMatchingMask:NSAnyEventMask
                untilDate:[NSDate distantPast]
                inMode:NSDefaultRunLoopMode
                dequeue: YES])
    {
        [GetNSApplication() sendEvent: event];
    }

#if wxUSE_LOG
    // let the logs be flashed again
    wxLog::Resume();
#endif // wxUSE_LOG

    s_inYield = false;

    return true;
}

#ifdef __WXDEBUG__
void wxApp::OnAssert(const wxChar *file, int line, const wxChar* cond, const wxChar *msg)
{
    m_isInAssert = TRUE;
    wxAppBase::OnAssert(file, line, cond, msg);
    m_isInAssert = FALSE;
}
#endif // __WXDEBUG__

