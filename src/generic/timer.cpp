/////////////////////////////////////////////////////////////////////////////
// Name:        mgl/timer.cpp
// Purpose:     wxTimer implementation
// Author:      Vaclav Slavik
// Id:          $Id$
// Copyright:   (c) Vaclav Slavik
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "timer.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// ----------------------------------------------------------------------------
// NB: when using generic wxTimer implementation in your port, you *must* call 
//     wxTimer::NotifyTimers() often enough. The ideal place for this 
//     is in wxEventLoop::Dispatch().
// ----------------------------------------------------------------------------

#include "wx/timer.h"

#if wxUSE_TIMER

#include "wx/log.h"
#include "wx/module.h"

// ----------------------------------------------------------------------------
// Time input function
// ----------------------------------------------------------------------------

#ifdef __WXMGL__
    // We take advantage of wxMGL's _EVT_getTicks because it is faster
    // (especially under MS-DOS!) and more precise than wxGetLocalTimeMillis
    // if we are unlucky and the latter combines information from two sources.
    #include "wx/mgl/private.h"
    extern "C" ulong _EVT_getTicks();
    #define GetMillisecondsTime _EVT_getTicks

    typedef ulong wxTimerTick_t;

    #define wxTimerTickFmtSpec _T("lu")
    #define wxTimerTickPrintfArg(tt) (tt)
#else // !__WXMGL__
    #define GetMillisecondsTime wxGetLocalTimeMillis

    typedef wxLongLong wxTimerTick_t;

    #if wxUSE_LONGLONG_WX
        #define wxTimerTickFmtSpec wxLongLongFmtSpec _T("d")
        #define wxTimerTickPrintfArg(tt) (tt.GetValue())
    #else // using native wxLongLong
        #define wxTimerTickFmtSpec _T("s")
        #define wxTimerTickPrintfArg(tt) (tt.ToString().c_str())
    #endif // wx/native long long
#endif // __WXMGL__/!__WXMGL__

// ----------------------------------------------------------------------------
// helper structures and wxTimerScheduler
// ----------------------------------------------------------------------------

class wxTimerDesc
{
public:
    wxTimerDesc(wxTimer *t) : 
        timer(t), running(FALSE), next(NULL), prev(NULL), 
        shotTime(0), deleteFlag(NULL) {}

    wxTimer         *timer;
    bool             running;
    wxTimerDesc     *next, *prev;
    wxTimerTick_t    shotTime;  
    volatile bool   *deleteFlag; // see comment in ~wxTimer
};

class wxTimerScheduler
{
public:
    wxTimerScheduler() : m_timers(NULL) {}

    void QueueTimer(wxTimerDesc *desc, wxTimerTick_t when = 0);
    void RemoveTimer(wxTimerDesc *desc);
    void NotifyTimers();
   
private:
    wxTimerDesc *m_timers;
};

void wxTimerScheduler::QueueTimer(wxTimerDesc *desc, wxTimerTick_t when)
{
    if ( desc->running )
        return; // already scheduled
        
    if ( when == 0 )
        when = GetMillisecondsTime() + desc->timer->GetInterval();
    desc->shotTime = when;
    desc->running = TRUE;

    wxLogTrace( wxT("timer"),
                wxT("queued timer %p at tick %") wxTimerTickFmtSpec, 
               desc->timer,  wxTimerTickPrintfArg(when));

    if ( m_timers )
    {
        wxTimerDesc *d = m_timers;
        while ( d->next && d->next->shotTime < when ) d = d->next;
        desc->next = d->next;
        desc->prev = d;
        if ( d->next )
            d->next->prev = desc;
        d->next = desc;
    }
    else
    {
        m_timers = desc;
        desc->prev = desc->next = NULL;
    }
}

void wxTimerScheduler::RemoveTimer(wxTimerDesc *desc)
{
    desc->running = FALSE;
    if ( desc == m_timers )
        m_timers = desc->next;
    if ( desc->prev )
        desc->prev->next = desc->next;
    if ( desc->next )
        desc->next->prev = desc->prev;
    desc->prev = desc->next = NULL;
}

void wxTimerScheduler::NotifyTimers()
{
    if ( m_timers )
    {
        bool oneShot;
        volatile bool timerDeleted;
        wxTimerTick_t now = GetMillisecondsTime();
        wxTimerDesc *desc;

        while ( m_timers && m_timers->shotTime <= now )
        {
            desc = m_timers;
            oneShot = desc->timer->IsOneShot();
            RemoveTimer(desc);

            timerDeleted = FALSE;
            desc->deleteFlag = &timerDeleted;
            desc->timer->Notify();
            
            if ( !timerDeleted )
            {
                wxLogTrace( wxT("timer"),
                            wxT("notified timer %p sheduled for %")
                            wxTimerTickFmtSpec, 
                            desc->timer,
                            wxTimerTickPrintfArg(desc->shotTime) );

                desc->deleteFlag = NULL;
                if ( !oneShot )
                    QueueTimer(desc, now + desc->timer->GetInterval());
            }
        }
    }
}


// ----------------------------------------------------------------------------
// wxTimer
// ----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxTimer, wxEvtHandler)

wxTimerScheduler *gs_scheduler = NULL;

void wxTimer::Init()
{
    if ( !gs_scheduler )
        gs_scheduler = new wxTimerScheduler;
    m_desc = new wxTimerDesc(this);
}

wxTimer::~wxTimer()
{
    wxLogTrace( wxT("timer"), wxT("destroying timer %p..."), this);
    if ( IsRunning() )
        Stop();

    // NB: this is a hack: wxTimerScheduler must have some way of knowing
    //     that wxTimer object was deleted under its hands -- this may 
    //     happen if somebody is really nasty and deletes the timer
    //     from wxTimer::Notify()
    if ( m_desc->deleteFlag != NULL )
        *m_desc->deleteFlag = TRUE;

    delete m_desc;
    wxLogTrace( wxT("timer"), wxT("    ...done destroying timer %p..."), this);
}

bool wxTimer::IsRunning() const
{
    return m_desc->running;
}

bool wxTimer::Start(int millisecs, bool oneShot)
{
    wxLogTrace( wxT("timer"), wxT("started timer %p: %i ms, oneshot=%i"), 
               this, millisecs, oneShot);

    if ( !wxTimerBase::Start(millisecs, oneShot) )
        return FALSE;
    
    gs_scheduler->QueueTimer(m_desc);
    return TRUE;
}

void wxTimer::Stop()
{
    if ( !m_desc->running ) return;
    
    gs_scheduler->RemoveTimer(m_desc);
}

/*static*/ void wxTimer::NotifyTimers()
{
    if ( gs_scheduler )
        gs_scheduler->NotifyTimers();
}



// A module to deallocate memory properly:
class wxTimerModule: public wxModule
{
DECLARE_DYNAMIC_CLASS(wxTimerModule)
public:
    wxTimerModule() {}
    bool OnInit() { return TRUE; }
    void OnExit() { delete gs_scheduler; gs_scheduler = NULL; }
};

IMPLEMENT_DYNAMIC_CLASS(wxTimerModule, wxModule)


#endif //wxUSE_TIMER
