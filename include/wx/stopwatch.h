/////////////////////////////////////////////////////////////////////////////
// Name:        wx/stopwatch.h
// Purpose:     wxStopWatch and global time-related functions
// Author:      Julian Smart (wxTimer), Sylvain Bougnoux (wxStopWatch)
// Created:     26.06.03 (extracted from wx/timer.h)
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2003 wxWindows team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STOPWATCH_H_
#define _WX_STOPWATCH_H_
//
// ----------------------------------------------------------------------------
// wxStopWatch: measure time intervals with up to 1ms resolution
// ----------------------------------------------------------------------------

#if wxUSE_STOPWATCH

class WXDLLEXPORT wxStopWatch
{
public:
    // ctor starts the stop watch
    wxStopWatch() { m_pauseCount = 0; Start(); }

    // start the stop watch at the moment t0
    void Start(long t0 = 0);

    // pause the stop watch
    void Pause()
    {
        if ( !m_pauseCount++ )
            m_pause = GetElapsedTime();
    }

    // resume it
    void Resume()
    {
        wxASSERT_MSG( m_pauseCount > 0,
                      _T("Resuming stop watch which is not paused") );

        if ( !--m_pauseCount )
            Start(m_pause);
    }

    // get elapsed time since the last Start() in milliseconds
    long Time() const;

protected:
    // returns the elapsed time since t0
    long GetElapsedTime() const;

private:
    // the time of the last Start()
    wxLongLong m_t0;

    // the time of the last Pause() (only valid if m_pauseCount > 0)
    long m_pause;

    // if > 0, the stop watch is paused, otherwise it is running
    int m_pauseCount;
};

#endif // wxUSE_STOPWATCH

#if wxUSE_LONGLONG

// Starts a global timer
// -- DEPRECATED: use wxStopWatch instead
void WXDLLEXPORT wxStartTimer();

// Gets elapsed milliseconds since last wxStartTimer or wxGetElapsedTime
// -- DEPRECATED: use wxStopWatch instead
long WXDLLEXPORT wxGetElapsedTime(bool resetTimer = TRUE);

#endif // wxUSE_LONGLONG

// ----------------------------------------------------------------------------
// global time functions
// ----------------------------------------------------------------------------

// Get number of seconds since local time 00:00:00 Jan 1st 1970.
extern long WXDLLEXPORT wxGetLocalTime();

// Get number of seconds since GMT 00:00:00, Jan 1st 1970.
extern long WXDLLEXPORT wxGetUTCTime();

#if wxUSE_LONGLONG
// Get number of milliseconds since local time 00:00:00 Jan 1st 1970
extern wxLongLong WXDLLEXPORT wxGetLocalTimeMillis();
#endif // wxUSE_LONGLONG

#define wxGetCurrentTime() wxGetLocalTime()

#endif // _WX_STOPWATCH_H_

