/////////////////////////////////////////////////////////////////////////////
// Name:        threadpsx.cpp
// Purpose:     wxThread (Posix) Implementation
// Author:      Original from Wolfram Gloger/Guilhem Lavaux
// Modified by:
// Created:     04/22/98
// RCS-ID:      $Id$
// Copyright:   (c) Wolfram Gloger (1996, 1997)
//                  Guilhem Lavaux (1998)
//                  Robert Roebling (1999)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "thread.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#ifdef __linux__
    #include <sched.h>
#endif

#include "wx/thread.h"
#include "wx/module.h"
#include "wx/utils.h"
#include "wx/log.h"
#include "wx/intl.h"

#include "gdk/gdk.h"
#include "gtk/gtk.h"

enum thread_state
{
    STATE_NEW,          // didn't start execution yet (=> RUNNING)
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_CANCELED,
    STATE_EXITED
};

//--------------------------------------------------------------------
// global data
//--------------------------------------------------------------------

// the id of the main thread
static pthread_t gs_pidMain;

// the key for the pointer to the associated wxThread object
static pthread_key_t gs_keySelf;

// this mutex must be acquired before any call to a GUI function
static wxMutex *gs_mutexGui;

//--------------------------------------------------------------------
// common GUI thread code
//--------------------------------------------------------------------

#include "threadgui.inc"

//--------------------------------------------------------------------
// wxMutex (Posix implementation)
//--------------------------------------------------------------------

class wxMutexInternal
{
public:
  pthread_mutex_t p_mutex;
};

wxMutex::wxMutex()
{
    p_internal = new wxMutexInternal;
    pthread_mutex_init( &(p_internal->p_mutex), (const pthread_mutexattr_t*) NULL );
    m_locked = 0;
}

wxMutex::~wxMutex()
{
    if (m_locked > 0)
        wxLogDebug( "wxMutex warning: freeing a locked mutex (%d locks)", m_locked );

    pthread_mutex_destroy( &(p_internal->p_mutex) );
    delete p_internal;
}

wxMutexError wxMutex::Lock()
{
    int err = pthread_mutex_lock( &(p_internal->p_mutex) );
    if (err == EDEADLK)
    {
        return wxMUTEX_DEAD_LOCK;
    }

    m_locked++;

    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::TryLock()
{
    if (m_locked)
    {
        return wxMUTEX_BUSY;
    }

    int err = pthread_mutex_trylock( &(p_internal->p_mutex) );
    switch (err)
    {
        case EBUSY: return wxMUTEX_BUSY;
    }

    m_locked++;

    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::Unlock()
{
    if (m_locked > 0)
    {
        m_locked--;
    }
    else
    {
        return wxMUTEX_UNLOCKED;
    }

    pthread_mutex_unlock( &(p_internal->p_mutex) );

    return wxMUTEX_NO_ERROR;
}

//--------------------------------------------------------------------
// wxCondition (Posix implementation)
//--------------------------------------------------------------------

class wxConditionInternal
{
public:
  pthread_cond_t p_condition;
};

wxCondition::wxCondition()
{
    p_internal = new wxConditionInternal;
    pthread_cond_init( &(p_internal->p_condition), (const pthread_condattr_t *) NULL );
}

wxCondition::~wxCondition()
{
    pthread_cond_destroy( &(p_internal->p_condition) );

    delete p_internal;
}

void wxCondition::Wait(wxMutex& mutex)
{
    pthread_cond_wait( &(p_internal->p_condition), &(mutex.p_internal->p_mutex) );
}

bool wxCondition::Wait(wxMutex& mutex, unsigned long sec, unsigned long nsec)
{
    struct timespec tspec;

    tspec.tv_sec = time(0L)+sec;
    tspec.tv_nsec = nsec;
    return (pthread_cond_timedwait(&(p_internal->p_condition), &(mutex.p_internal->p_mutex), &tspec) != ETIMEDOUT);
}

void wxCondition::Signal()
{
    pthread_cond_signal( &(p_internal->p_condition) );
}

void wxCondition::Broadcast()
{
    pthread_cond_broadcast( &(p_internal->p_condition) );
}

//--------------------------------------------------------------------
// wxThread (Posix implementation)
//--------------------------------------------------------------------

class wxThreadInternal
{
public:
    wxThreadInternal() { m_state = STATE_NEW; }
    ~wxThreadInternal() {}

    // thread entry function
    static void *PthreadStart(void *ptr);

    // start the thread
    wxThreadError Run();

    // accessors
        // priority
    int GetPriority() const { return m_prio; }
    void SetPriority(int prio) { m_prio = prio; }
        // state
    thread_state GetState() const { return m_state; }
    void SetState(thread_state state) { m_state = state; }
        // id
    pthread_t GetId() const { return thread_id; }
        // "cancelled" flag
    void Cancel();
    bool WasCancelled() const { return m_cancelled; }

//private: -- should be!
    pthread_t thread_id;

private:
    thread_state m_state;    // see thread_state enum
    int          m_prio;     // in wxWindows units: from 0 to 100

    // set when the thread should terminate
    bool m_cancelled;

    // we start running when this condition becomes true
    wxMutex     m_mutexRun;
    wxCondition m_condRun;

    // this condition becomes true when we get back to PthreadStart() function
    wxMutex     m_mutexStop;
    wxCondition m_condStop;
};

void *wxThreadInternal::PthreadStart(void *ptr)
{
    wxThread *thread = (wxThread *)ptr;
    wxThreadInternal *pthread = thread->p_internal;

    if ( pthread_setspecific(gs_keySelf, thread) != 0 )
    {
        wxLogError(_("Can not start thread: error writing TLS."));

        return (void *)-1;
    }

    // wait for the condition to be signaled from Run()
    pthread->m_mutexRun.Lock();
    pthread->m_condRun.Wait(pthread->m_mutexRun);

    // call the main entry
    void* status = thread->Entry();

    pthread->m_mutexRun.Unlock();

    // wake up the pthread(s) waiting for our termination
    pthread->m_condStop.Broadcast();

    // terminate the thread
    thread->Exit(status);

    wxFAIL_MSG("wxThread::Exit() can't return.");

    return NULL;
}

wxThreadError wxThreadInternal::Run()
{
    wxCHECK_MSG( GetState() == STATE_NEW, wxTHREAD_RUNNING,
                 "thread may only be started once after successful Create()" );

    wxMutexLocker lock(&m_mutexRun);
    m_condRun.Signal();

    return wxTHREAD_NO_ERROR;
}

void wxThreadInternal::Cancel()
{
    wxMutexLocker lock(&m_mutexStop);
    m_cancelled = TRUE;
    m_condStop.Wait(m_mutexStop);
}

// -----------------------------------------------------------------------------
// static functions
// -----------------------------------------------------------------------------

wxThread *wxThread::This()
{
    return (wxThread *)pthread_getspecific(gs_keySelf);
}

bool wxThread::IsMain()
{
    return (bool)pthread_equal(pthread_self(), gs_pidMain);
}

void wxThread::Yield()
{
    sched_yield();
}

void wxThread::Sleep(unsigned long milliseconds)
{
    // FIXME how to test for nanosleep() availability?

    usleep(milliseconds * 1000); // usleep(3) wants microseconds
}

// -----------------------------------------------------------------------------
// creating thread
// -----------------------------------------------------------------------------

wxThread::wxThread()
{
    p_internal = new wxThreadInternal();
}

wxThreadError wxThread::Create()
{
    if (p_internal->GetState() != STATE_NEW)
        return wxTHREAD_RUNNING;

    // set up the thread attribute: right now, we only set thread priority
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int prio;
    if ( pthread_attr_getschedpolicy(&attr, &prio) != 0 )
    {
        wxLogError(_("Can not retrieve thread scheduling policy."));
    }

    int min_prio = sched_get_priority_min(prio),
        max_prio = sched_get_priority_max(prio);

    if ( min_prio == -1 || max_prio == -1 )
    {
        wxLogError(_("Can not get priority range for scheduling policy %d."),
                   prio);
    }
    else
    {
        struct sched_param sp;
        pthread_attr_getschedparam(&attr, &sp);
        sp.sched_priority = min_prio +
                           (p_internal->GetPriority()*(max_prio-min_prio))/100;
        pthread_attr_setschedparam(&attr, &sp);
    }

    // this is the point of no return
    int rc = pthread_create(&p_internal->thread_id, &attr,
                            wxThreadInternal::PthreadStart, (void *)this);
    pthread_attr_destroy(&attr);

    if ( rc != 0 )
    {
        p_internal->SetState(STATE_EXITED);
        return wxTHREAD_NO_RESOURCE;
    }

    return wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Run()
{
    return p_internal->Run();
}

// -----------------------------------------------------------------------------
// misc accessors
// -----------------------------------------------------------------------------

void wxThread::SetPriority(unsigned int prio)
{
    wxCHECK_RET( (WXTHREAD_MIN_PRIORITY <= prio) &&
                 (prio <= WXTHREAD_MAX_PRIORITY), "invalid thread priority" );

    wxCriticalSectionLocker lock(m_critsect);

    switch ( p_internal->GetState() )
    {
        case STATE_NEW:
            // thread not yet started, priority will be set when it is
            p_internal->SetPriority(prio);
            break;

        case STATE_RUNNING:
        case STATE_PAUSED:
            {
                struct sched_param sparam;
                sparam.sched_priority = prio;

                if ( pthread_setschedparam(p_internal->GetId(),
                                           SCHED_OTHER, &sparam) != 0 )
                {
                    wxLogError(_("Failed to set thread priority %d."), prio);
                }
            }
            break;

        case STATE_EXITED:
        default:
            wxFAIL_MSG("impossible to set thread priority in this state");
    }
}

unsigned int wxThread::GetPriority() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect);

    return p_internal->GetPriority();
}

unsigned long wxThread::GetID() const
{
    return (unsigned long)p_internal->thread_id;
}

// -----------------------------------------------------------------------------
// pause/resume
// -----------------------------------------------------------------------------

wxThreadError wxThread::Pause()
{
    wxCriticalSectionLocker lock(m_critsect);

    if ( p_internal->GetState() != STATE_RUNNING )
    {
        wxLogDebug("Can't pause thread which is not running.");

        return wxTHREAD_NOT_RUNNING;
    }

    p_internal->SetState(STATE_PAUSED);

    return wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Resume()
{
    wxCriticalSectionLocker lock(m_critsect);

    if ( p_internal->GetState() == STATE_PAUSED )
    {
        p_internal->SetState(STATE_RUNNING);

        return wxTHREAD_NO_ERROR;
    }
    else
    {
        wxLogDebug("Attempt to resume a thread which is not paused.");

        return wxTHREAD_MISC_ERROR;
    }
}

// -----------------------------------------------------------------------------
// exiting thread
// -----------------------------------------------------------------------------

wxThread::ExitCode wxThread::Delete()
{
    m_critsect.Enter();
    thread_state state = p_internal->GetState();
    m_critsect.Leave();

    switch ( state )
    {
        case STATE_NEW:
        case STATE_EXITED:
            // nothing to do
            break;

        case STATE_PAUSED:
            // resume the thread first
            Resume();

            // fall through

        default:
            // set the flag telling to the thread to stop and wait
            p_internal->Cancel();
    }

    return NULL;
}

wxThreadError wxThread::Kill()
{
    switch ( p_internal->GetState() )
    {
        case STATE_NEW:
        case STATE_EXITED:
            return wxTHREAD_NOT_RUNNING;

        default:
            if ( pthread_cancel(p_internal->GetId()) != 0 )
            {
                wxLogError(_("Failed to terminate a thread."));

                return wxTHREAD_MISC_ERROR;
            }

            return wxTHREAD_NO_ERROR;
    }
}

void wxThread::Exit(void *status)
{
    wxThread *ptr = this;
    THREAD_SEND_EXIT_MSG(ptr);

    OnExit();

    p_internal->SetState(STATE_EXITED);

    delete this;

    pthread_exit(status);
}

bool wxThread::TestDestroy() const
{
    wxCriticalSectionLocker lock((wxCriticalSection&)m_critsect);

    return p_internal->WasCancelled();
}

wxThread::~wxThread()
{
}

// -----------------------------------------------------------------------------
// state tests
// -----------------------------------------------------------------------------

bool wxThread::IsRunning() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect);

    return p_internal->GetState() == STATE_RUNNING;
}

bool wxThread::IsAlive() const
{
    wxCriticalSectionLocker lock((wxCriticalSection&)m_critsect);

    switch ( p_internal->GetState() )
    {
        case STATE_RUNNING:
        case STATE_PAUSED:
            return TRUE;

        default:
            return FALSE;
    }
}

//--------------------------------------------------------------------
// wxThreadModule
//--------------------------------------------------------------------

class wxThreadModule : public wxModule
{
public:
    virtual bool OnInit();
    virtual void OnExit();

private:
    DECLARE_DYNAMIC_CLASS(wxThreadModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxThreadModule, wxModule)

bool wxThreadModule::OnInit()
{
    if ( pthread_key_create(&gs_keySelf, NULL /* dtor function */) != 0 )
    {
        wxLogError(_("Thread module initialization failed: "
                     "failed to create pthread key."));

        return FALSE;
    }

    gs_mutexGui = new wxMutex();
    wxThreadGuiInit();
    gs_pidMain = (int)getpid();
    gs_mutexGui->Lock();

    return TRUE;
}

void wxThreadModule::OnExit()
{
    gs_mutexGui->Unlock();
    wxThreadGuiExit();
    delete gs_mutexGui;

    (void)pthread_key_delete(gs_keySelf);
}
