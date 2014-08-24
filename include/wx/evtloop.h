///////////////////////////////////////////////////////////////////////////////
// Name:        wx/evtloop.h
// Purpose:     declares wxEventLoop class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.06.01
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_EVTLOOP_H_
#define _WX_EVTLOOP_H_

#include "wx/event.h"
#include "wx/utils.h"

// TODO: implement wxEventLoopSource for MSW (it should wrap a HANDLE and be
//       monitored using MsgWaitForMultipleObjects())
#if defined(__WXOSX__) || (defined(__UNIX__) && !defined(__WINDOWS__))
    #define wxUSE_EVENTLOOP_SOURCE 1
#else
    #define wxUSE_EVENTLOOP_SOURCE 0
#endif

#if wxUSE_EVENTLOOP_SOURCE
    class wxEventLoopSource;
    class wxEventLoopSourceHandler;
#endif

/*
    NOTE ABOUT wxEventLoopBase::YieldFor LOGIC
    ------------------------------------------

    The YieldFor() function helps to avoid re-entrancy problems and problems
    caused by out-of-order event processing
    (see "wxYield-like problems" and "wxProgressDialog+threading BUG" wx-dev threads).

    The logic behind YieldFor() is simple: it analyzes the queue of the native
    events generated by the underlying GUI toolkit and picks out and processes
    only those matching the given mask.

    It's important to note that YieldFor() is used to selectively process the
    events generated by the NATIVE toolkit.
    Events syntethized by wxWidgets code or by user code are instead selectively
    processed thanks to the logic built into wxEvtHandler::ProcessPendingEvents().
    In fact, when wxEvtHandler::ProcessPendingEvents gets called from inside a
    YieldFor() call, wxEventLoopBase::IsEventAllowedInsideYield is used to decide
    if the pending events for that event handler can be processed.
    If all the pending events associated with that event handler result as "not processable",
    the event handler "delays" itself calling wxEventLoopBase::DelayPendingEventHandler
    (so it's moved: m_handlersWithPendingEvents => m_handlersWithPendingDelayedEvents).
    Last, wxEventLoopBase::ProcessPendingEvents() before exiting moves the delayed
    event handlers back into the list of handlers with pending events
    (m_handlersWithPendingDelayedEvents => m_handlersWithPendingEvents) so that
    a later call to ProcessPendingEvents() (possibly outside the YieldFor() call)
    will process all pending events as usual.
*/

// ----------------------------------------------------------------------------
// wxEventLoopBase: interface for wxEventLoop
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxEventLoopBase
{
public:
    // trivial, but needed (because of wxEventLoopBase) ctor
    wxEventLoopBase();

    // dtor
    virtual ~wxEventLoopBase() { }

    // use this to check whether the event loop was successfully created before
    // using it
    virtual bool IsOk() const { return true; }

    // returns true if this is the main loop
    bool IsMain() const;

#if wxUSE_EVENTLOOP_SOURCE
    // create a new event loop source wrapping the given file descriptor and
    // monitor it for events occurring on this descriptor in all event loops
    static wxEventLoopSource *
      AddSourceForFD(int fd, wxEventLoopSourceHandler *handler, int flags);
#endif // wxUSE_EVENTLOOP_SOURCE

    // dispatch&processing
    // -------------------

    // start the event loop, return the exit code when it is finished
    //
    // notice that wx ports should override DoRun(), this method is virtual
    // only to allow overriding it in the user code for custom event loops
    virtual int Run();

    // is this event loop running now?
    //
    // notice that even if this event loop hasn't terminated yet but has just
    // spawned a nested (e.g. modal) event loop, this would return false
    bool IsRunning() const;

    // exit from the loop with the given exit code
    //
    // this can be only used to exit the currently running loop, use
    // ScheduleExit() if this might not be the case
    virtual void Exit(int rc = 0);

    // ask the event loop to exit with the given exit code, can be used even if
    // this loop is not running right now but the loop must have been started,
    // i.e. Run() should have been already called
    virtual void ScheduleExit(int rc = 0) = 0;

    // return true if any events are available
    virtual bool Pending() const = 0;

    // dispatch a single event, return false if we should exit from the loop
    virtual bool Dispatch() = 0;

    // same as Dispatch() but doesn't wait for longer than the specified (in
    // ms) timeout, return true if an event was processed, false if we should
    // exit the loop or -1 if timeout expired
    virtual int DispatchTimeout(unsigned long timeout) = 0;

    // implement this to wake up the loop: usually done by posting a dummy event
    // to it (can be called from non main thread)
    virtual void WakeUp() = 0;


    // idle handling
    // -------------

        // make sure that idle events are sent again
    virtual void WakeUpIdle();

        // this virtual function is called  when the application
        // becomes idle and by default it forwards to wxApp::ProcessIdle() and
        // while it can be overridden in a custom event loop, you must call the
        // base class version to ensure that idle events are still generated
        //
        // it should return true if more idle events are needed, false if not
    virtual bool ProcessIdle();


    // Yield-related hooks
    // -------------------

    // process all currently pending events right now
    //
    // it is an error to call Yield() recursively unless the value of
    // onlyIfNeeded is true
    //
    // WARNING: this function is dangerous as it can lead to unexpected
    //          reentrancies (i.e. when called from an event handler it
    //          may result in calling the same event handler again), use
    //          with _extreme_ care or, better, don't use at all!
    bool Yield(bool onlyIfNeeded = false);

    // more selective version of Yield()
    //
    // notice that it is virtual for backwards-compatibility but new code
    // should override DoYieldFor() and not YieldFor() itself
    virtual bool YieldFor(long eventsToProcess);

    // returns true if the main thread is inside a Yield() call
    virtual bool IsYielding() const
        { return m_isInsideYield; }

    // returns true if events of the given event category should be immediately
    // processed inside a wxApp::Yield() call or rather should be queued for
    // later processing by the main event loop
    virtual bool IsEventAllowedInsideYield(wxEventCategory cat) const
        { return (m_eventsToProcessInsideYield & cat) != 0; }

    // no SafeYield hooks since it uses wxWindow which is not available when wxUSE_GUI=0


    // active loop
    // -----------

    // return currently active (running) event loop, may be NULL
    static wxEventLoopBase *GetActive() { return ms_activeLoop; }

    // set currently active (running) event loop
    static void SetActive(wxEventLoopBase* loop);


protected:
    // real implementation of Run()
    virtual int DoRun() = 0;

    // And the real, port-specific, implementation of YieldFor().
    //
    // The base class version is pure virtual to ensure that it is overridden
    // in the derived classes but does have an implementation which processes
    // pending events in wxApp if eventsToProcess allows it, and so should be
    // called from the overridden version at an appropriate place (i.e. after
    // processing the native events but before doing anything else that could
    // be affected by pending events dispatching).
    virtual void DoYieldFor(long eventsToProcess) = 0;

    // this function should be called before the event loop terminates, whether
    // this happens normally (because of Exit() call) or abnormally (because of
    // an exception thrown from inside the loop)
    virtual void OnExit();

    // Return true if we're currently inside our Run(), even if another nested
    // event loop is currently running, unlike IsRunning() (which should have
    // been really called IsActive() but it's too late to change this now).
    bool IsInsideRun() const { return m_isInsideRun; }


    // the pointer to currently active loop
    static wxEventLoopBase *ms_activeLoop;

    // should we exit the loop?
    bool m_shouldExit;

    // YieldFor() helpers:
    bool m_isInsideYield;
    long m_eventsToProcessInsideYield;

private:
    // this flag is set on entry into Run() and reset before leaving it
    bool m_isInsideRun;

    wxDECLARE_NO_COPY_CLASS(wxEventLoopBase);
};

#if defined(__WINDOWS__) || defined(__WXMAC__) || defined(__WXDFB__) || (defined(__UNIX__) && !defined(__WXOSX__))

// this class can be used to implement a standard event loop logic using
// Pending() and Dispatch()
//
// it also handles idle processing automatically
class WXDLLIMPEXP_BASE wxEventLoopManual : public wxEventLoopBase
{
public:
    wxEventLoopManual();

    // sets the "should exit" flag and wakes up the loop so that it terminates
    // soon
    virtual void ScheduleExit(int rc = 0) wxOVERRIDE;

protected:
    // enters a loop calling OnNextIteration(), Pending() and Dispatch() and
    // terminating when Exit() is called
    virtual int DoRun() wxOVERRIDE;

    // may be overridden to perform some action at the start of each new event
    // loop iteration
    virtual void OnNextIteration() { }


    // the loop exit code
    int m_exitcode;

private:
    // process all already pending events and dispatch a new one (blocking
    // until it appears in the event queue if necessary)
    //
    // returns the return value of Dispatch()
    bool ProcessEvents();

    wxDECLARE_NO_COPY_CLASS(wxEventLoopManual);
};

#endif // platforms using "manual" loop

// we're moving away from old m_impl wxEventLoop model as otherwise the user
// code doesn't have access to platform-specific wxEventLoop methods and this
// can sometimes be very useful (e.g. under MSW this is necessary for
// integration with MFC) but currently this is not done for all ports yet (e.g.
// wxX11) so fall back to the old wxGUIEventLoop definition below for them

#if defined(__DARWIN__)
    // CoreFoundation-based event loop is currently in wxBase so include it in
    // any case too (although maybe it actually shouldn't be there at all)
    #include "wx/osx/core/evtloop.h"
#endif

// include the header defining wxConsoleEventLoop
#if defined(__UNIX__) && !defined(__WINDOWS__)
    #include "wx/unix/evtloop.h"
#elif defined(__WINDOWS__)
    #include "wx/msw/evtloopconsole.h"
#endif

#if wxUSE_GUI

// include the appropriate header defining wxGUIEventLoop

#if defined(__WXMSW__)
    #include "wx/msw/evtloop.h"
#elif defined(__WXOSX__)
    #include "wx/osx/evtloop.h"
#elif defined(__WXDFB__)
    #include "wx/dfb/evtloop.h"
#elif defined(__WXGTK20__)
    #include "wx/gtk/evtloop.h"
    #elif defined(__WXQT__)
    #include "wx/qt/evtloop.h"
#else // other platform

#include "wx/stopwatch.h"   // for wxMilliClock_t

class WXDLLIMPEXP_FWD_CORE wxEventLoopImpl;

class WXDLLIMPEXP_CORE wxGUIEventLoop : public wxEventLoopBase
{
public:
    wxGUIEventLoop() { m_impl = NULL; }
    virtual ~wxGUIEventLoop();

    virtual void ScheduleExit(int rc = 0);
    virtual bool Pending() const;
    virtual bool Dispatch();
    virtual int DispatchTimeout(unsigned long timeout)
    {
        // TODO: this is, of course, horribly inefficient and a proper wait with
        //       timeout should be implemented for all ports natively...
        const wxMilliClock_t timeEnd = wxGetLocalTimeMillis() + timeout;
        for ( ;; )
        {
            if ( Pending() )
                return Dispatch();

            if ( wxGetLocalTimeMillis() >= timeEnd )
                return -1;
        }
    }
    virtual void WakeUp() { }

protected:
    virtual int DoRun();
    virtual void DoYieldFor(long eventsToProcess);

    // the pointer to the port specific implementation class
    wxEventLoopImpl *m_impl;

    wxDECLARE_NO_COPY_CLASS(wxGUIEventLoop);
};

#endif // platforms

#endif // wxUSE_GUI

#if wxUSE_GUI
    // we use a class rather than a typedef because wxEventLoop is
    // forward-declared in many places
    class wxEventLoop : public wxGUIEventLoop { };
#else // !wxUSE_GUI
    // we can't define wxEventLoop differently in GUI and base libraries so use
    // a #define to still allow writing wxEventLoop in the user code
    #if wxUSE_CONSOLE_EVENTLOOP && (defined(__WINDOWS__) || defined(__UNIX__))
        #define wxEventLoop wxConsoleEventLoop
    #else // we still must define it somehow for the code below...
        #define wxEventLoop wxEventLoopBase
    #endif
#endif

inline bool wxEventLoopBase::IsRunning() const { return GetActive() == this; }

#if wxUSE_GUI && !defined(__WXOSX__)
// ----------------------------------------------------------------------------
// wxModalEventLoop
// ----------------------------------------------------------------------------

// this is a naive generic implementation which uses wxWindowDisabler to
// implement modality, we will surely need platform-specific implementations
// too, this generic implementation is here only temporarily to see how it
// works
class WXDLLIMPEXP_CORE wxModalEventLoop : public wxGUIEventLoop
{
public:
    wxModalEventLoop(wxWindow *winModal)
    {
        m_windowDisabler = new wxWindowDisabler(winModal);
    }

protected:
    virtual void OnExit() wxOVERRIDE
    {
        delete m_windowDisabler;
        m_windowDisabler = NULL;

        wxGUIEventLoop::OnExit();
    }

private:
    wxWindowDisabler *m_windowDisabler;
};

#endif //wxUSE_GUI

// ----------------------------------------------------------------------------
// wxEventLoopActivator: helper class for wxEventLoop implementations
// ----------------------------------------------------------------------------

// this object sets the wxEventLoop given to the ctor as the currently active
// one and unsets it in its dtor, this is especially useful in presence of
// exceptions but is more tidy even when we don't use them
class wxEventLoopActivator
{
public:
    wxEventLoopActivator(wxEventLoopBase *evtLoop)
    {
        m_evtLoopOld = wxEventLoopBase::GetActive();
        wxEventLoopBase::SetActive(evtLoop);
    }

    ~wxEventLoopActivator()
    {
        // restore the previously active event loop
        wxEventLoopBase::SetActive(m_evtLoopOld);
    }

private:
    wxEventLoopBase *m_evtLoopOld;
};

#if wxUSE_GUI || wxUSE_CONSOLE_EVENTLOOP

class wxEventLoopGuarantor
{
public:
    wxEventLoopGuarantor()
    {
        m_evtLoopNew = NULL;
        if (!wxEventLoop::GetActive())
        {
            m_evtLoopNew = new wxEventLoop;
            wxEventLoop::SetActive(m_evtLoopNew);
        }
    }

    ~wxEventLoopGuarantor()
    {
        if (m_evtLoopNew)
        {
            wxEventLoop::SetActive(NULL);
            delete m_evtLoopNew;
        }
    }

private:
    wxEventLoop *m_evtLoopNew;
};

#endif // wxUSE_GUI || wxUSE_CONSOLE_EVENTLOOP

#endif // _WX_EVTLOOP_H_
