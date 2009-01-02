/////////////////////////////////////////////////////////////////////////////
// Name:        src/unix/evtloopunix.cpp
// Purpose:     wxEventLoop implementation
// Author:      Lukasz Michalski (lm@zork.pl)
// Created:     2007-05-07
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Zork Lukasz Michalski
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_CONSOLE_EVENTLOOP

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/log.h"
#endif

#include <errno.h>
#include "wx/apptrait.h"
#include "wx/evtloop.h"
#include "wx/thread.h"
#include "wx/module.h"
#include "wx/unix/private/timer.h"
#include "wx/unix/private/epolldispatcher.h"
#include "wx/private/selectdispatcher.h"

#define TRACE_EVENTS _T("events")

// ===========================================================================
// wxEventLoop::PipeIOHandler implementation
// ===========================================================================

// ----------------------------------------------------------------------------
// initialization
// ----------------------------------------------------------------------------

bool wxConsoleEventLoop::PipeIOHandler::Create()
{
    if ( !m_pipe.Create() )
    {
        wxLogError(_("Failed to create wake up pipe used by event loop."));
        return false;
    }

    const int fdRead = GetReadFd();

    int flags = fcntl(fdRead, F_GETFL, 0);
    if ( flags == -1 || fcntl(fdRead, F_SETFL, flags | O_NONBLOCK) == -1 )
    {
        wxLogSysError(_("Failed to switch wake up pipe to non-blocking mode"));
        return false;
    }

    wxLogTrace(TRACE_EVENTS, wxT("Wake up pipe (%d, %d) created"),
               fdRead, m_pipe[wxPipe::Write]);

    return true;
}

// ----------------------------------------------------------------------------
// wakeup handling
// ----------------------------------------------------------------------------

void wxConsoleEventLoop::PipeIOHandler::WakeUp()
{
    if ( write(m_pipe[wxPipe::Write], "s", 1) != 1 )
    {
        // don't use wxLog here, we can be in another thread and this could
        // result in dead locks
        perror("write(wake up pipe)");
    }
}

void wxConsoleEventLoop::PipeIOHandler::OnReadWaiting()
{
    // got wakeup from child thread: read all data available in pipe just to
    // make it empty (evevn though we write one byte at a time from WakeUp(),
    // it could have been called several times)
    char buf[4];
    for ( ;; )
    {
        const int size = read(GetReadFd(), buf, WXSIZEOF(buf));

        if ( size == 0 || (size == -1 && errno == EAGAIN) )
        {
            // nothing left in the pipe (EAGAIN is expected for an FD with
            // O_NONBLOCK)
            break;
        }

        if ( size == -1 )
        {
            wxLogSysError(_("Failed to read from wake-up pipe"));

            break;
        }
    }

    wxTheApp->ProcessPendingEvents();
}

// ===========================================================================
// wxEventLoop implementation
// ===========================================================================

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

wxConsoleEventLoop::wxConsoleEventLoop()
{
    if ( !m_wakeupPipe.Create() )
    {
        m_dispatcher = NULL;
        return;
    }

    m_dispatcher = wxFDIODispatcher::Get();
    if ( !m_dispatcher )
        return;

    m_dispatcher->RegisterFD
                  (
                    m_wakeupPipe.GetReadFd(),
                    &m_wakeupPipe,
                    wxFDIO_INPUT
                  );
}

//-----------------------------------------------------------------------------
// events dispatch and loop handling
//-----------------------------------------------------------------------------

bool wxConsoleEventLoop::Pending() const
{
    return wxTheApp->HasPendingEvents();
}

bool wxConsoleEventLoop::Dispatch()
{
    DispatchTimeout(wxFDIODispatcher::TIMEOUT_INFINITE);

    return true;
}

int wxConsoleEventLoop::DispatchTimeout(unsigned long timeout)
{
#if wxUSE_TIMER
    // check if we need to decrease the timeout to account for a timer
    wxUsecClock_t nextTimer;
    if ( wxTimerScheduler::Get().GetNext(&nextTimer) )
    {
        unsigned long timeUntilNextTimer = wxMilliClockToLong(nextTimer / 1000);
        if ( timeUntilNextTimer < timeout )
            timeout = timeUntilNextTimer;
    }
#endif // wxUSE_TIMER

    bool hadEvent = m_dispatcher->Dispatch(timeout);

#if wxUSE_TIMER
    if ( wxTimerScheduler::Get().NotifyExpired() )
        hadEvent = true;
#endif // wxUSE_TIMER

    return hadEvent ? 1 : -1;
}

void wxConsoleEventLoop::WakeUp()
{
    m_wakeupPipe.WakeUp();
}

void wxConsoleEventLoop::OnNextIteration()
{
    // call the signal handlers for any signals we caught recently
    wxTheApp->CheckSignal();
}


wxEventLoopBase *wxConsoleAppTraits::CreateEventLoop()
{
    return new wxEventLoop();
}

#endif // wxUSE_CONSOLE_EVENTLOOP
