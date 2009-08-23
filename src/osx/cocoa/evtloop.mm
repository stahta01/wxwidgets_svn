///////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/cocoa/evtloop.mm
// Purpose:     implementation of wxEventLoop for OS X
// Author:      Vadim Zeitlin, Stefan Csomor
// Modified by:
// Created:     2006-01-12
// RCS-ID:      $Id: evtloop.cpp 54845 2008-07-30 14:52:41Z SC $
// Copyright:   (c) 2006 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/evtloop.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif // WX_PRECOMP

#include "wx/log.h"

#include "wx/osx/private.h"

// ============================================================================
// wxEventLoop implementation
// ============================================================================

/*
static int CalculateNSEventMaskFromEventCategory(wxEventCategory cat)
{
	NSLeftMouseDownMask	|
	NSLeftMouseUpMask |
	NSRightMouseDownMask |
	NSRightMouseUpMask		= 1 << NSRightMouseUp,
	NSMouseMovedMask		= 1 << NSMouseMoved,
	NSLeftMouseDraggedMask		= 1 << NSLeftMouseDragged,
	NSRightMouseDraggedMask		= 1 << NSRightMouseDragged,
	NSMouseEnteredMask		= 1 << NSMouseEntered,
	NSMouseExitedMask		= 1 << NSMouseExited,
        NSScrollWheelMask		= 1 << NSScrollWheel,
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
	NSTabletPointMask		= 1 << NSTabletPoint,
	NSTabletProximityMask		= 1 << NSTabletProximity,
#endif
	NSOtherMouseDownMask		= 1 << NSOtherMouseDown,
	NSOtherMouseUpMask		= 1 << NSOtherMouseUp,
	NSOtherMouseDraggedMask		= 1 << NSOtherMouseDragged,



	NSKeyDownMask			= 1 << NSKeyDown,
	NSKeyUpMask			= 1 << NSKeyUp,
	NSFlagsChangedMask		= 1 << NSFlagsChanged,

	NSAppKitDefinedMask		= 1 << NSAppKitDefined,
	NSSystemDefinedMask		= 1 << NSSystemDefined,
	NSApplicationDefinedMask	= 1 << NSApplicationDefined,
	NSPeriodicMask			= 1 << NSPeriodic,
	NSCursorUpdateMask		= 1 << NSCursorUpdate,

	NSAnyEventMask			= 0xffffffffU
}
*/

wxGUIEventLoop::wxGUIEventLoop()
{
    m_sleepTime = 0.0;
}

void wxGUIEventLoop::WakeUp()
{
    extern void wxMacWakeUp();

    wxMacWakeUp();
}

//-----------------------------------------------------------------------------
// event loop sources operations
//-----------------------------------------------------------------------------

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
extern "C"
{
struct wx_cffd_data
{
    wxEventLoopSourceHandler* handler;
    int flags;
};

static void wx_cffiledescriptor_callback(CFFileDescriptorRef cffd,
                                         CFOptionFlags flags, void* ctxData)
{
    wxLogTrace(wxTRACE_EVT_SOURCE, "CFFileDescriptor Callback");

    wx_cffd_data* data = static_cast<wx_cffd_data*>(ctxData);
    wxEventLoopSourceHandler* handler = data->handler;
    if (flags & kCFFileDescriptorReadCallBack)
        handler->OnReadWaiting();
    if (flags & kCFFileDescriptorWriteCallBack)
        handler->OnWriteWaiting();

    // reenable callbacks
    if (data->flags & wxEVENT_SOURCE_INPUT)
        CFFileDescriptorEnableCallBacks(cffd, kCFFileDescriptorReadCallBack);
    if (data->flags & wxEVENT_SOURCE_OUTPUT)
        CFFileDescriptorEnableCallBacks(cffd, kCFFileDescriptorWriteCallBack);
}
}

wxMacEventLoopSource* wxGUIEventLoop::CreateSource(int fd,
                                            wxEventLoopSourceHandler* handler,
                                            int flags) const
{
    wxMacEventLoopSource* source = new wxMacEventLoopSource();
    // FIXME this is currently a leak :-)
    wx_cffd_data* data = new wx_cffd_data;
    data->handler = handler;
    data->flags = flags;
    CFFileDescriptorContext ctx = { 0, data, NULL, NULL, NULL };
    CFFileDescriptorRef cffd = CFFileDescriptorCreate(kCFAllocatorDefault, fd,
                                     true, wx_cffiledescriptor_callback, &ctx);

    if (flags & wxEVENT_SOURCE_INPUT)
        CFFileDescriptorEnableCallBacks(cffd, kCFFileDescriptorReadCallBack);
    if (flags & wxEVENT_SOURCE_OUTPUT)
        CFFileDescriptorEnableCallBacks(cffd, kCFFileDescriptorWriteCallBack);

    source->SetResource(
            CFFileDescriptorCreateRunLoopSource(kCFAllocatorDefault, cffd, 0));
    source->SetHandler(handler);
    source->SetFlags(flags);
    return source;
}
#elif
wxMacEventLoopSource* wxGUIEventLoop::CreateSource(int fd,
                                            wxEventLoopSourceHandler* handler,
                                            int flags) const
{
    return NULL;
}
#endif

bool wxGUIEventLoop::DoAddSource(wxAbstractEventLoopSource* src)
{
    Source* source = dynamic_cast<Source*>(src);
    wxCHECK_MSG( source, false, "Invalid source type" );

    wxLogTrace(wxTRACE_EVT_SOURCE,
                "wxGUIEventLoop::AddSource() source=%d",
                source->GetResource());

    NSRunLoop* nsloop = [NSRunLoop currentRunLoop];
    CFRunLoopRef cfloop = [nsloop getCFRunLoop];
    CFRunLoopAddSource(cfloop, source->GetResource(), kCFRunLoopDefaultMode);
    
    return true;
}

bool wxGUIEventLoop::DoRemoveSource(wxAbstractEventLoopSource* src)
{
    Source* source = dynamic_cast<Source*>(src);
    wxCHECK_MSG( source, false, "Invalid source type" );

    wxLogTrace(wxTRACE_EVT_SOURCE,
                "wxGUIEventLoop::RemoveSource() source=%d",
                source->GetResource());

    NSRunLoop* nsloop = [NSRunLoop currentRunLoop];
    CFRunLoopRef cfloop = [nsloop getCFRunLoop];
    CFRunLoopRemoveSource(cfloop, source->GetResource(), kCFRunLoopDefaultMode);

    return true;
}

//-----------------------------------------------------------------------------
// events dispatch and loop handling
//-----------------------------------------------------------------------------

bool wxGUIEventLoop::Pending() const
{
    wxMacAutoreleasePool autoreleasepool;
    // a pointer to the event is returned if there is one, or nil if not
    return [[NSApplication sharedApplication]
            nextEventMatchingMask: NSAnyEventMask
            untilDate: nil
            inMode: NSDefaultRunLoopMode
            dequeue: NO];
}

bool wxGUIEventLoop::Dispatch()
{
    if ( !wxTheApp )
        return false;

    wxMacAutoreleasePool autoreleasepool;

    if(NSEvent *event = [NSApp
                nextEventMatchingMask:NSAnyEventMask
                untilDate:[NSDate dateWithTimeIntervalSinceNow: m_sleepTime]
                inMode:NSDefaultRunLoopMode
                dequeue: YES])
    {
        m_sleepTime = 0.0;
        [NSApp sendEvent: event];
    }
    else
    {
        if ( wxTheApp->ProcessIdle() )
            m_sleepTime = 0.0 ;
        else
        {
            m_sleepTime = 1.0;
#if wxUSE_THREADS
            wxMutexGuiLeave();
            wxMilliSleep(20);
            wxMutexGuiEnter();
#endif
        }
    }

    return true;
}

bool wxGUIEventLoop::YieldFor(long eventsToProcess)
{
#if wxUSE_THREADS
    // Yielding from a non-gui thread needs to bail out, otherwise we end up
    // possibly sending events in the thread too.
    if ( !wxThread::IsMain() )
    {
        return true;
    }
#endif // wxUSE_THREADS

    m_isInsideYield = true;
    m_eventsToProcessInsideYield = eventsToProcess;

#if wxUSE_LOG
    // disable log flushing from here because a call to wxYield() shouldn't
    // normally result in message boxes popping up &c
    wxLog::Suspend();
#endif // wxUSE_LOG

    // process all pending events:
    while ( Pending() )
        Dispatch();

    // it's necessary to call ProcessIdle() to update the frames sizes which
    // might have been changed (it also will update other things set from
    // OnUpdateUI() which is a nice (and desired) side effect)
    while ( ProcessIdle() ) {}

#if wxUSE_LOG
    wxLog::Resume();
#endif // wxUSE_LOG
    m_isInsideYield = false;

    return true;
}

int wxGUIEventLoop::DispatchTimeout(unsigned long timeout)
{
    wxMacAutoreleasePool autoreleasepool;

    NSEvent *event = [NSApp
                nextEventMatchingMask:NSAnyEventMask
                untilDate:[NSDate dateWithTimeIntervalSinceNow: timeout/1000]
                inMode:NSDefaultRunLoopMode
                dequeue: YES];
    if ( !event )
        return -1;

    [NSApp sendEvent: event];

    return true;
}
