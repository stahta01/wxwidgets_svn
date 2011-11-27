///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/stopwatch.cpp
// Purpose:     wxStopWatch and other non-GUI stuff from wx/timer.h
// Author:
//    Original version by Julian Smart
//    Vadim Zeitlin got rid of all ifdefs (11.12.99)
//    Sylvain Bougnoux added wxStopWatch class
//    Guillermo Rodriguez <guille@iies.es> rewrote from scratch (Dic/99)
// Modified by:
// Created:     20.06.2003 (extracted from common/timercmn.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2003 wxWidgets Team
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

#include "wx/stopwatch.h"

#ifndef WX_PRECOMP
    #ifdef __WXMSW__
        #include "wx/msw/wrapwin.h"
    #endif
    #include "wx/intl.h"
    #include "wx/log.h"
#endif //WX_PRECOMP

// ----------------------------------------------------------------------------
// System headers
// ----------------------------------------------------------------------------

#if defined(__WIN32__) && !defined(HAVE_FTIME) && !defined(__MWERKS__) && !defined(__WXWINCE__)
    #define HAVE_FTIME
#endif

#if defined(__VISAGECPP__) && !defined(HAVE_FTIME)
    #define HAVE_FTIME
#  if __IBMCPP__ >= 400
    #  define ftime(x) _ftime(x)
#  endif
#endif

#if defined(__MWERKS__) && defined(__WXMSW__)
#   undef HAVE_FTIME
#   undef HAVE_GETTIMEOFDAY
#endif

#ifndef __WXPALMOS5__
#ifndef __WXWINCE__
#include <time.h>
#else
#include "wx/msw/private.h"
#include "wx/msw/wince/time.h"
#endif
#endif // __WXPALMOS5__


#if !defined(__WXMAC__) && !defined(__WXWINCE__)
    #include <sys/types.h>      // for time_t
#endif

#if defined(HAVE_GETTIMEOFDAY)
    #include <sys/time.h>
    #include <unistd.h>
#elif defined(HAVE_FTIME)
    #include <sys/timeb.h>
#endif

#ifdef __WXPALMOS__
    #include <DateTime.h>
    #include <TimeMgr.h>
    #include <SystemMgr.h>
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStopWatch
// ----------------------------------------------------------------------------

#if wxUSE_STOPWATCH

namespace
{

#ifdef __WXMSW__

struct PerfCounter
{
    PerfCounter()
    {
        init = false;
    }

    bool CanBeUsed() const
    {
        return freq.QuadPart != 0;
    }

    wxCriticalSection cs;
    LARGE_INTEGER freq;
    bool init;
} gs_perfCounter;

#endif // __WXMSW__

const int MILLISECONDS_PER_SECOND = 1000;
const int MICROSECONDS_PER_SECOND = 1000*1000;

} // anonymous namespace

void wxStopWatch::DoStart()
{
#ifdef __WXMSW__
    if ( !gs_perfCounter.init )
    {
        wxCriticalSectionLocker lock(gs_perfCounter.cs);
        ::QueryPerformanceFrequency(&gs_perfCounter.freq);

        // Just a sanity check: it's not supposed to happen but verify that
        // ::QueryPerformanceCounter() succeeds so that we can really use it.
        LARGE_INTEGER counter;
        if ( !::QueryPerformanceCounter(&counter) )
        {
            wxLogDebug("QueryPerformanceCounter() unexpected failed (%s), "
                       "will not use it.", wxSysErrorMsg());

            gs_perfCounter.freq.QuadPart = 0;
        }

        gs_perfCounter.init = true;
    }
#endif // __WXMSW__

    m_t0 = GetCurrentClockValue();
}

wxLongLong wxStopWatch::GetClockFreq() const
{
#ifdef __WXMSW__
    // Under MSW we use the high resolution performance counter timer which has
    // its own frequency (usually related to the CPU clock speed).
    if ( gs_perfCounter.CanBeUsed() )
        return gs_perfCounter.freq.QuadPart;
#endif // __WXMSW__

    // Currently milliseconds are used everywhere else.
    return MILLISECONDS_PER_SECOND;
}

void wxStopWatch::Start(long t0)
{
    DoStart();

    m_t0 -= (wxLongLong(t0)*GetClockFreq())/MILLISECONDS_PER_SECOND;
}

wxLongLong wxStopWatch::GetCurrentClockValue() const
{
#ifdef __WXMSW__
    if ( gs_perfCounter.CanBeUsed() )
    {
        LARGE_INTEGER counter;
        ::QueryPerformanceCounter(&counter);
        return counter.QuadPart;
    }
#endif // __WXMSW__

    return wxGetLocalTimeMillis();
}

wxLongLong wxStopWatch::TimeInMicro() const
{
    const wxLongLong elapsed(m_pauseCount ? m_elapsedBeforePause
                                          : GetCurrentClockValue() - m_t0);

    return (elapsed*MICROSECONDS_PER_SECOND)/GetClockFreq();
}

#endif // wxUSE_STOPWATCH

// ----------------------------------------------------------------------------
// old timer functions superceded by wxStopWatch
// ----------------------------------------------------------------------------

#if wxUSE_LONGLONG

static wxLongLong wxStartTime = 0l;

// starts the global timer
void wxStartTimer()
{
    wxStartTime = wxGetLocalTimeMillis();
}

// Returns elapsed time in milliseconds
long wxGetElapsedTime(bool resetTimer)
{
    wxLongLong oldTime = wxStartTime;
    wxLongLong newTime = wxGetLocalTimeMillis();

    if ( resetTimer )
        wxStartTime = newTime;

    return (newTime - oldTime).GetLo();
}

#endif // wxUSE_LONGLONG

// ----------------------------------------------------------------------------
// the functions to get the current time and timezone info
// ----------------------------------------------------------------------------

// Get local time as seconds since 00:00:00, Jan 1st 1970
long wxGetLocalTime()
{
    struct tm tm;
    time_t t0, t1;

    // This cannot be made static because mktime can overwrite it.
    //
    memset(&tm, 0, sizeof(tm));
    tm.tm_year  = 70;
    tm.tm_mon   = 0;
    tm.tm_mday  = 5;        // not Jan 1st 1970 due to mktime 'feature'
    tm.tm_hour  = 0;
    tm.tm_min   = 0;
    tm.tm_sec   = 0;
    tm.tm_isdst = -1;       // let mktime guess

    // Note that mktime assumes that the struct tm contains local time.
    //
    t1 = time(&t1);         // now
    t0 = mktime(&tm);       // origin

    // Return the difference in seconds.
    //
    if (( t0 != (time_t)-1 ) && ( t1 != (time_t)-1 ))
        return (long)difftime(t1, t0) + (60 * 60 * 24 * 4);

    wxLogSysError(_("Failed to get the local system time"));
    return -1;
}

// Get UTC time as seconds since 00:00:00, Jan 1st 1970
long wxGetUTCTime()
{
    return (long)time(NULL);
}

#if wxUSE_LONGLONG

// Get local time as milliseconds since 00:00:00, Jan 1st 1970
wxLongLong wxGetLocalTimeMillis()
{
    wxLongLong val = 1000l;

    // If possible, use a function which avoids conversions from
    // broken-up time structures to milliseconds

#if defined(__WXPALMOS__)
    DateTimeType thenst;
    thenst.second  = 0;
    thenst.minute  = 0;
    thenst.hour    = 0;
    thenst.day     = 1;
    thenst.month   = 1;
    thenst.year    = 1970;
    thenst.weekDay = 5;
    uint32_t now = TimGetSeconds();
    uint32_t then = TimDateTimeToSeconds (&thenst);
    return SysTimeToMilliSecs(SysTimeInSecs(now - then));
#elif defined(__WXMSW__) && (defined(__WINE__) || defined(__MWERKS__))
    // This should probably be the way all WXMSW compilers should do it
    // Go direct to the OS for time

    SYSTEMTIME thenst = { 1970, 1, 4, 1, 0, 0, 0, 0 };  // 00:00:00 Jan 1st 1970
    FILETIME thenft;
    SystemTimeToFileTime( &thenst, &thenft );
    wxLongLong then( thenft.dwHighDateTime, thenft.dwLowDateTime );   // time in 100 nanoseconds

    SYSTEMTIME nowst;
    GetLocalTime( &nowst );
    FILETIME nowft;
    SystemTimeToFileTime( &nowst, &nowft );
    wxLongLong now( nowft.dwHighDateTime, nowft.dwLowDateTime );   // time in 100 nanoseconds

    return ( now - then ) / 10000.0;  // time from 00:00:00 Jan 1st 1970 to now in milliseconds

#elif defined(HAVE_GETTIMEOFDAY)
    struct timeval tp;
    if ( wxGetTimeOfDay(&tp) != -1 )
    {
        val *= tp.tv_sec;
        return (val + (tp.tv_usec / 1000));
    }
    else
    {
        wxLogError(_("wxGetTimeOfDay failed."));
        return 0;
    }
#elif defined(HAVE_FTIME)
    struct timeb tp;

    // ftime() is void and not int in some mingw32 headers, so don't
    // test the return code (well, it shouldn't fail anyhow...)
    (void)::ftime(&tp);
    val *= tp.time;
    return (val + tp.millitm);
#else // no gettimeofday() nor ftime()
    // We use wxGetLocalTime() to get the seconds since
    // 00:00:00 Jan 1st 1970 and then whatever is available
    // to get millisecond resolution.
    //
    // NOTE that this might lead to a problem if the clocks
    // use different sources, so this approach should be
    // avoided where possible.

    val *= wxGetLocalTime();

// GRG: This will go soon as all WIN32 seem to have ftime
// JACS: unfortunately not. WinCE doesn't have it.
#if defined (__WIN32__)
    // If your platform/compiler needs to use two different functions
    // to get ms resolution, please do NOT just shut off these warnings,
    // drop me a line instead at <guille@iies.es>

    // FIXME
#ifndef __WXWINCE__
    #warning "Possible clock skew bug in wxGetLocalTimeMillis()!"
#endif

    SYSTEMTIME st;
    ::GetLocalTime(&st);
    val += st.wMilliseconds;
#else // !Win32
    // If your platform/compiler does not support ms resolution please
    // do NOT just shut off these warnings, drop me a line instead at
    // <guille@iies.es>

    #if defined(__VISUALC__) || defined (__WATCOMC__)
        #pragma message("wxStopWatch will be up to second resolution!")
    #elif defined(__BORLANDC__)
        #pragma message "wxStopWatch will be up to second resolution!"
    #else
        #warning "wxStopWatch will be up to second resolution!"
    #endif // compiler
#endif

    return val;

#endif // time functions
}

#else // !wxUSE_LONGLONG

double wxGetLocalTimeMillis(void)
{
    return (double(clock()) / double(CLOCKS_PER_SEC)) * 1000.0;
}

#endif // wxUSE_LONGLONG/!wxUSE_LONGLONG
