///////////////////////////////////////////////////////////////////////////////
// Name:        wx/evtloop.h
// Purpose:     declares wxEventLoop class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.06.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_EVTLOOP_H_
#define _WX_EVTLOOP_H_

#include "wx/utils.h"

class WXDLLEXPORT wxEventLoop;

// ----------------------------------------------------------------------------
// wxEventLoop: a GUI event loop
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxEventLoopBase
{
public:
    // trivial, but needed (because of wxEventLoopBase) ctor
    wxEventLoopBase() { }

    // dtor
    virtual ~wxEventLoopBase() { }

    // start the event loop, return the exit code when it is finished
    virtual int Run() = 0;

    // exit from the loop with the given exit code
    virtual void Exit(int rc = 0) = 0;

    // return true if any events are available
    virtual bool Pending() const = 0;

    // dispatch a single event, return false if we should exit from the loop
    virtual bool Dispatch() = 0;

    // return currently active (running) event loop, may be NULL
    static wxEventLoop *GetActive() { return ms_activeLoop; }

    // set currently active (running) event loop
    static void SetActive(wxEventLoop* loop) { ms_activeLoop = loop; }

    // is this event loop running now?
    //
    // notice that even if this event loop hasn't terminated yet but has just
    // spawned a nested (e.g. modal) event loop, this would return false
    bool IsRunning() const;

protected:
    // this function should be called before the event loop terminates, whether
    // this happens normally (because of Exit() call) or abnormally (because of
    // an exception thrown from inside the loop)
    virtual void OnExit() { }


    // the pointer to currently active loop
    static wxEventLoop *ms_activeLoop;

    DECLARE_NO_COPY_CLASS(wxEventLoopBase)
};

// we're moving away from old m_impl wxEventLoop model as otherwise the user
// code doesn't have access to platform-specific wxEventLoop methods and this
// can sometimes be very useful (e.g. under MSW this is necessary for
// integration with MFC) but currently this is done for MSW only, other ports
// should follow a.s.a.p.
#if defined(__WXPALMOS__)
    #include "wx/palmos/evtloop.h"
#elif defined(__WXMSW__)
    #include "wx/msw/evtloop.h"
#else

class WXDLLEXPORT wxEventLoopImpl;

class WXDLLEXPORT wxEventLoop : public wxEventLoopBase
{
public:
    wxEventLoop() { m_impl = NULL; }
    virtual ~wxEventLoop();

    virtual int Run();
    virtual void Exit(int rc = 0);
    virtual bool Pending() const;
    virtual bool Dispatch();

protected:
    // the pointer to the port specific implementation class
    wxEventLoopImpl *m_impl;

    DECLARE_NO_COPY_CLASS(wxEventLoop)
        };

#endif // __WXMSW__/!__WXMSW__

inline bool wxEventLoopBase::IsRunning() const { return GetActive() == this; }

// ----------------------------------------------------------------------------
// wxModalEventLoop
// ----------------------------------------------------------------------------

// this is a naive generic implementation which uses wxWindowDisabler to
// implement modality, we will surely need platform-specific implementations
// too, this generic implementation is here only temporarily to see how it
// works
class WXDLLEXPORT wxModalEventLoop : public wxEventLoop
{
public:
    wxModalEventLoop(wxWindow *winModal)
    {
        m_windowDisabler = new wxWindowDisabler(winModal);
    }

protected:
    virtual void OnExit()
    {
        delete m_windowDisabler;
        m_windowDisabler = NULL;

        wxEventLoop::OnExit();
    }

private:
    wxWindowDisabler *m_windowDisabler;
};

// ----------------------------------------------------------------------------
// wxEventLoopActivator: helper class for wxEventLoop implementations
// ----------------------------------------------------------------------------

// this object sets the wxEventLoop given to the ctor as the currently active
// one and unsets it in its dtor, this is especially useful in presence of
// exceptions but is more tidy even when we don't use them
class wxEventLoopActivator
{
public:
    wxEventLoopActivator(wxEventLoop *evtLoop)
    {
        m_evtLoopOld = wxEventLoop::GetActive();
        wxEventLoop::SetActive(evtLoop);
    }

    ~wxEventLoopActivator()
    {
        // restore the previously active event loop
        wxEventLoop::SetActive(m_evtLoopOld);
    }

private:
    wxEventLoop *m_evtLoopOld;
};

#endif // _WX_EVTLOOP_H_
