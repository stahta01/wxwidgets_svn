    ///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/evtloop.h
// Purpose:     declares wxEventLoop class
// Author:      Lukasz Michalski (lm@zork.pl)
// Created:     2007-05-07
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Lukasz Michalski
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_EVTLOOP_H_
#define _WX_UNIX_EVTLOOP_H_

#if wxUSE_CONSOLE_EVENTLOOP

// ----------------------------------------------------------------------------
// wxConsoleEventLoop
// ----------------------------------------------------------------------------

class wxFDIODispatcher;

namespace wxPrivate
{
    class PipeIOHandler;
}

class WXDLLIMPEXP_BASE wxConsoleEventLoop : public wxEventLoopManual
{
public:
    typedef wxUnixEventLoopSource Source;

    // initialize the event loop, use IsOk() to check if we were successful
    wxConsoleEventLoop();
    virtual ~wxConsoleEventLoop();

    // implement base class pure virtuals
    virtual bool Pending() const;
    virtual bool Dispatch();
    virtual int DispatchTimeout(unsigned long timeout);
    virtual void WakeUp();
    virtual bool IsOk() const { return m_dispatcher != NULL; }
    virtual bool YieldFor(long WXUNUSED(eventsToProcess)) { return true; }

#if wxUSE_EVENTLOOP_SOURCE
    virtual wxUnixEventLoopSource* CreateSource() const
    {
        return new wxUnixEventLoopSource();
    }

    virtual wxUnixEventLoopSource* CreateSource(int res,
                                           wxEventLoopSourceHandler* handler,
                                           int flags) const
    {
        return new wxUnixEventLoopSource(res, handler, flags);
    }
#endif

protected:
#if wxUSE_EVENTLOOP_SOURCE
    // adding/removing sources
    virtual bool DoAddSource(wxAbstractEventLoopSource* source);
    virtual bool DoRemoveSource(wxAbstractEventLoopSource* source);
#endif

    virtual void OnNextIteration();

private:
    // pipe used for wake up messages: when a child thread wants to wake up
    // the event loop in the main thread it writes to this pipe
    wxPrivate::PipeIOHandler *m_wakeupPipe;

    // either wxSelectDispatcher or wxEpollDispatcher
    wxFDIODispatcher *m_dispatcher;

    wxDECLARE_NO_COPY_CLASS(wxConsoleEventLoop);
};

#endif // wxUSE_CONSOLE_EVENTLOOP

#endif // _WX_UNIX_EVTLOOP_H_
