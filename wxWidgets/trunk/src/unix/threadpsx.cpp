/////////////////////////////////////////////////////////////////////////////
// Name:        threadpsx.cpp
// Purpose:     wxThread (Posix) Implementation
// Author:      Original from Wolfram Gloger/Guilhem Lavaux
// Modified by:
// Created:     04/22/98
// RCS-ID:      $Id$
// Copyright:   (c) Wolfram Gloger (1996, 1997)
//                  Guilhem Lavaux (1998)
//                  Vadim Zeitlin (1999)
//                  Robert Roebling (1999)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declaration
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "thread.h"
#endif

// With simple makefiles, we must ignore the file body if not using
// threads.
#include "wx/setup.h"

#if wxUSE_THREADS

#include "wx/thread.h"
#include "wx/module.h"
#include "wx/utils.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/dynarray.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#if HAVE_SCHED_H
    #include <sched.h>
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the possible states of the thread and transitions from them
enum wxThreadState
{
    STATE_NEW,          // didn't start execution yet (=> RUNNING)
    STATE_RUNNING,      // running (=> PAUSED or EXITED)
    STATE_PAUSED,       // suspended (=> RUNNING or EXITED)
    STATE_EXITED        // thread doesn't exist any more
};

// ----------------------------------------------------------------------------
// types
// ----------------------------------------------------------------------------

WX_DEFINE_ARRAY(wxThread *, wxArrayThread);

// -----------------------------------------------------------------------------
// global data
// -----------------------------------------------------------------------------

// we keep the list of all threads created by the application to be able to
// terminate them on exit if there are some left - otherwise the process would
// be left in memory
static wxArrayThread gs_allThreads;

// the id of the main thread
static pthread_t gs_tidMain;

// the key for the pointer to the associated wxThread object
static pthread_key_t gs_keySelf;

// this mutex must be acquired before any call to a GUI function
static wxMutex *gs_mutexGui;

// ============================================================================
// implementation
// ============================================================================

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
    pthread_mutexattr_t attr_type;

    pthread_mutexattr_settype( &attr_type, PTHREAD_MUTEX_FAST_NP );

    p_internal = new wxMutexInternal;
    pthread_mutex_init( &(p_internal->p_mutex), (const pthread_mutexattr_t*) &attr_type );
    m_locked = 0;
}

wxMutex::~wxMutex()
{
    if (m_locked > 0)
        wxLogDebug(_T("Freeing a locked mutex (%d locks)"), m_locked);

    pthread_mutex_destroy( &(p_internal->p_mutex) );
    delete p_internal;
}

wxMutexError wxMutex::Lock()
{
    int err = pthread_mutex_lock( &(p_internal->p_mutex) );
    if (err == EDEADLK)
    {
        wxLogDebug(_T("Locking this mutex would lead to deadlock!"));

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
        wxLogDebug(_T("Unlocking not locked mutex."));

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
    wxThreadInternal();
    ~wxThreadInternal();

    // thread entry function
    static void *PthreadStart(void *ptr);

#if HAVE_THREAD_CLEANUP_FUNCTIONS
    // thread exit function
    static void PthreadCleanup(void *ptr);
#endif

    // thread actions
        // start the thread
    wxThreadError Run();
        // ask the thread to terminate
    void Wait();
        // wake up threads waiting for our termination
    void SignalExit();
        // go to sleep until Resume() is called
    void Pause();
        // resume the thread
    void Resume();

    // accessors
        // priority
    int GetPriority() const { return m_prio; }
    void SetPriority(int prio) { m_prio = prio; }
        // state
    wxThreadState GetState() const { return m_state; }
    void SetState(wxThreadState state) { m_state = state; }
        // id
    pthread_t GetId() const { return m_threadId; }
    pthread_t *GetIdPtr() { return &m_threadId; }
        // "cancelled" flag
    void SetCancelFlag() { m_cancelled = TRUE; }
    bool WasCancelled() const { return m_cancelled; }

private:
    pthread_t     m_threadId;   // id of the thread
    wxThreadState m_state;      // see wxThreadState enum
    int           m_prio;       // in wxWindows units: from 0 to 100

    // set when the thread should terminate
    bool m_cancelled;

    // this (mutex, cond) pair is used to synchronize the main thread and this
    // thread in several situations:
    //  1. The thread function blocks until condition is signaled by Run() when
    //     it's initially created - this allows thread creation in "suspended"
    //     state
    //  2. The Delete() function blocks until the condition is signaled when the
    //     thread exits.
    // GL: On Linux, this may fail because we can have a deadlock in either
    //     SignalExit() or Wait(): so we add m_end_mutex for the finalization.
    wxMutex     m_mutex, m_end_mutex;
    wxCondition m_cond;

    // another (mutex, cond) pair for Pause()/Resume() usage
    //
    // VZ: it's possible that we might reuse the mutex and condition from above
    //     for this too, but as I'm not at all sure that it won't create subtle
    //     problems with race conditions between, say, Pause() and Delete() I
    //     prefer this may be a bit less efficient but much safer solution
    wxMutex     m_mutexSuspend;
    wxCondition m_condSuspend;
};

void *wxThreadInternal::PthreadStart(void *ptr)
{
    wxThread *thread = (wxThread *)ptr;
    wxThreadInternal *pthread = thread->p_internal;
    void *status;

    int rc = pthread_setspecific(gs_keySelf, thread);
    if ( rc != 0 )
    {
        wxLogSysError(rc, _("Cannot start thread: error writing TLS"));

        return (void *)-1;
    }
#if HAVE_THREAD_CLEANUP_FUNCTIONS
    // Install the cleanup handler.
    pthread_cleanup_push(wxThreadInternal::PthreadCleanup, ptr);
#endif

    // wait for the condition to be signaled from Run()
    // mutex state: currently locked by the thread which created us
    pthread->m_cond.Wait(pthread->m_mutex);
    // mutex state: locked again on exit of Wait()

    // call the main entry
    status = thread->Entry();

#if HAVE_THREAD_CLEANUP_FUNCTIONS
    pthread_cleanup_pop(FALSE);
#endif

    // terminate the thread
    thread->Exit(status);

    wxFAIL_MSG(_T("wxThread::Exit() can't return."));

    return NULL;
}

#if HAVE_THREAD_CLEANUP_FUNCTIONS
// Only called when the thread is explicitely killed.

void wxThreadInternal::PthreadCleanup(void *ptr)
{
    wxThread *thread = (wxThread *) ptr;

    // The thread is already considered as finished.
    if (thread->p_internal->GetState() == STATE_EXITED)
      return;

    // first call user-level clean up code
    thread->OnExit();

    // next wake up the threads waiting for us (OTOH, this function won't retur
    // until someone waited for us!)
    thread->p_internal->SetState(STATE_EXITED);

    thread->p_internal->SignalExit();
}
#endif

wxThreadInternal::wxThreadInternal()
{
    m_state = STATE_NEW;
    m_cancelled = FALSE;

    // this mutex is locked during almost all thread lifetime - it will only be
    // unlocked in the very end
    m_mutex.Lock();
 
    // this mutex is used by wxThreadInternal::Wait() and by
    // wxThreadInternal::SignalExit(). We don't use m_mutex because of a
    // possible deadlock in either Wait() or SignalExit().
    m_end_mutex.Lock();

    // this mutex is used in Pause()/Resume() and is also locked all the time
    // unless the thread is paused
    m_mutexSuspend.Lock();
}

wxThreadInternal::~wxThreadInternal()
{
    // GL: moved to SignalExit
    // m_mutexSuspend.Unlock();

    // note that m_mutex will be unlocked by the thread which waits for our
    // termination

    // In the case, we didn't start the thread, all these mutex are locked:
    // we must unlock them.
    if (m_mutex.IsLocked())
      m_mutex.Unlock();

    if (m_end_mutex.IsLocked())
      m_end_mutex.Unlock();

    if (m_mutexSuspend.IsLocked())
      m_mutexSuspend.Unlock();
}

wxThreadError wxThreadInternal::Run()
{
    wxCHECK_MSG( GetState() == STATE_NEW, wxTHREAD_RUNNING,
                 _T("thread may only be started once after successful Create()") );

    // the mutex was locked on Create(), so we will be able to lock it again
    // only when the thread really starts executing and enters the wait -
    // otherwise we might signal the condition before anybody is waiting for it
    wxMutexLocker lock(m_mutex);
    m_cond.Signal();

    m_state = STATE_RUNNING;

    return wxTHREAD_NO_ERROR;

    // now the mutex is unlocked back - but just to allow Wait() function to
    // terminate by relocking it, so the net result is that the worker thread
    // starts executing and the mutex is still locked
}

void wxThreadInternal::Wait()
{
    wxCHECK_RET( WasCancelled(), _T("thread should have been cancelled first") );

    // if the thread we're waiting for is waiting for the GUI mutex, we will
    // deadlock so make sure we release it temporarily
    if ( wxThread::IsMain() )
        wxMutexGuiLeave();

    // entering Wait() releases the mutex thus allowing SignalExit() to acquire
    // it and to signal us its termination
    m_cond.Wait(m_end_mutex);

    // mutex is still in the locked state - relocked on exit from Wait(), so
    // unlock it - we don't need it any more, the thread has already terminated
    m_end_mutex.Unlock();

    // After that, we wait for the real end of the other thread.
    pthread_join(GetId(), NULL);

    // reacquire GUI mutex
    if ( wxThread::IsMain() )
        wxMutexGuiEnter();
}

void wxThreadInternal::SignalExit()
{
    // GL: Unlock mutexSuspend here.
    m_mutexSuspend.Unlock();

    // as mutex is currently locked, this will block until some other thread
    // (normally the same which created this one) unlocks it by entering Wait()
    m_end_mutex.Lock();

    // wake up all the threads waiting for our termination
    m_cond.Broadcast();

    // after this call mutex will be finally unlocked
    m_end_mutex.Unlock();
}

void wxThreadInternal::Pause()
{
    // the state is set from the thread which pauses us first, this function
    // is called later so the state should have been already set
    wxCHECK_RET( m_state == STATE_PAUSED,
                 _T("thread must first be paused with wxThread::Pause().") );

    // don't pause the thread which is being terminated - this would lead to
    // deadlock if the thread is paused after Delete() had called Resume() but
    // before it had time to call Wait()
    if ( WasCancelled() )
        return;

    // wait until the condition is signaled from Resume()
    m_condSuspend.Wait(m_mutexSuspend);
}

void wxThreadInternal::Resume()
{
    wxCHECK_RET( m_state == STATE_PAUSED,
                 _T("can't resume thread which is not suspended.") );

    // we will be able to lock this mutex only when Pause() starts waiting
    wxMutexLocker lock(m_mutexSuspend);
    m_condSuspend.Signal();

    SetState(STATE_RUNNING);
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
    return (bool)pthread_equal(pthread_self(), gs_tidMain);
}

void wxThread::Yield()
{
    sched_yield();
}

void wxThread::Sleep(unsigned long milliseconds)
{
    wxUsleep(milliseconds);
}

// -----------------------------------------------------------------------------
// creating thread
// -----------------------------------------------------------------------------

wxThread::wxThread()
{
    // add this thread to the global list of all threads
    gs_allThreads.Add(this);

    p_internal = new wxThreadInternal();
}

wxThreadError wxThread::Create()
{
    if (p_internal->GetState() != STATE_NEW)
        return wxTHREAD_RUNNING;

    // set up the thread attribute: right now, we only set thread priority
    pthread_attr_t attr;
    pthread_attr_init(&attr);

#ifdef HAVE_THREAD_PRIORITY_FUNCTIONS
    int prio;
    if ( pthread_attr_getschedpolicy(&attr, &prio) != 0 )
    {
        wxLogError(_("Cannot retrieve thread scheduling policy."));
    }

    int min_prio = sched_get_priority_min(prio),
        max_prio = sched_get_priority_max(prio);

    if ( min_prio == -1 || max_prio == -1 )
    {
        wxLogError(_("Cannot get priority range for scheduling policy %d."),
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
#endif // HAVE_THREAD_PRIORITY_FUNCTIONS

#ifdef HAVE_PTHREAD_ATTR_SETSCOPE
    // this will make the threads created by this process really concurrent
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
#endif // HAVE_PTHREAD_ATTR_SETSCOPE

    // create the new OS thread object
    int rc = pthread_create(p_internal->GetIdPtr(), &attr,
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
    wxCHECK_RET( ((int)WXTHREAD_MIN_PRIORITY <= (int)prio) &&
                 ((int)prio <= (int)WXTHREAD_MAX_PRIORITY),
                 _T("invalid thread priority") );

    wxCriticalSectionLocker lock(m_critsect);

    switch ( p_internal->GetState() )
    {
        case STATE_NEW:
            // thread not yet started, priority will be set when it is
            p_internal->SetPriority(prio);
            break;

        case STATE_RUNNING:
        case STATE_PAUSED:
#ifdef HAVE_THREAD_PRIORITY_FUNCTIONS
            {
                struct sched_param sparam;
                sparam.sched_priority = prio;

                if ( pthread_setschedparam(p_internal->GetId(),
                                           SCHED_OTHER, &sparam) != 0 )
                {
                    wxLogError(_("Failed to set thread priority %d."), prio);
                }
            }
#endif // HAVE_THREAD_PRIORITY_FUNCTIONS
            break;

        case STATE_EXITED:
        default:
            wxFAIL_MSG(_T("impossible to set thread priority in this state"));
    }
}

unsigned int wxThread::GetPriority() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect);

    return p_internal->GetPriority();
}

unsigned long wxThread::GetID() const
{
    return (unsigned long)p_internal->GetId();
}

// -----------------------------------------------------------------------------
// pause/resume
// -----------------------------------------------------------------------------

wxThreadError wxThread::Pause()
{
    wxCriticalSectionLocker lock(m_critsect);

    if ( p_internal->GetState() != STATE_RUNNING )
    {
        wxLogDebug(_T("Can't pause thread which is not running."));

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
        m_critsect.Leave();
        p_internal->Resume();
        m_critsect.Enter();

        return wxTHREAD_NO_ERROR;
    }
    else
    {
        wxLogDebug(_T("Attempt to resume a thread which is not paused."));

        return wxTHREAD_MISC_ERROR;
    }
}

// -----------------------------------------------------------------------------
// exiting thread
// -----------------------------------------------------------------------------

wxThread::ExitCode wxThread::Delete()
{
    if (IsPaused())
      Resume();

    m_critsect.Enter();
    wxThreadState state = p_internal->GetState();

    // ask the thread to stop
    p_internal->SetCancelFlag();

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
            // wait until the thread stops
            p_internal->Wait();
    }
    //GL: As we must auto-destroy, the destruction must happen here.
    delete this;

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
#ifdef HAVE_PTHREAD_CANCEL 
            if ( pthread_cancel(p_internal->GetId()) != 0 )
#endif
            {
                wxLogError(_("Failed to terminate a thread."));

                return wxTHREAD_MISC_ERROR;
            }
	    //GL: As we must auto-destroy, the destruction must happen here (2).
	    delete this;

            return wxTHREAD_NO_ERROR;
    }
}

void wxThread::Exit(void *status)
{
    // first call user-level clean up code
    OnExit();

    // next wake up the threads waiting for us (OTOH, this function won't return
    // until someone waited for us!)
    p_internal->SignalExit();

    p_internal->SetState(STATE_EXITED);

    // delete both C++ thread object and terminate the OS thread object
    // GL: This is very ugly and buggy ...
//    delete this;
    pthread_exit(status);
}

// also test whether we were paused
bool wxThread::TestDestroy()
{
    wxCriticalSectionLocker lock(m_critsect);

    if ( p_internal->GetState() == STATE_PAUSED )
    {
        // leave the crit section or the other threads will stop too if they try
        // to call any of (seemingly harmless) IsXXX() functions while we sleep
        m_critsect.Leave();

        p_internal->Pause();

        // enter it back before it's finally left in lock object dtor
        m_critsect.Enter();
    }

    return p_internal->WasCancelled();
}

wxThread::~wxThread()
{
    m_critsect.Enter();
    if (p_internal->GetState() != STATE_EXITED &&
        p_internal->GetState() != STATE_NEW)
      wxLogDebug(_T("The thread is being destroyed althought it is still running ! The application may crash."));

    m_critsect.Leave();

    delete p_internal;
    // remove this thread from the global array
    gs_allThreads.Remove(this);
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

bool wxThread::IsPaused() const
{
    wxCriticalSectionLocker lock((wxCriticalSection&)m_critsect);

    return (p_internal->GetState() == STATE_PAUSED);
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
    int rc = pthread_key_create(&gs_keySelf, NULL /* dtor function */);
    if ( rc != 0 )
    {
        wxLogSysError(rc, _("Thread module initialization failed: "
                            "failed to create thread key"));

        return FALSE;
    }

    gs_mutexGui = new wxMutex();

    gs_tidMain = pthread_self();

    gs_mutexGui->Lock();

    return TRUE;
}

void wxThreadModule::OnExit()
{
    wxASSERT_MSG( wxThread::IsMain(), _T("only main thread can be here") );

    // terminate any threads left
    size_t count = gs_allThreads.GetCount();
    if ( count != 0u )
        wxLogDebug(_T("Some threads were not terminated by the application."));

    for ( size_t n = 0u; n < count; n++ )
    {
        gs_allThreads[n]->Delete();
    }

    // destroy GUI mutex
    gs_mutexGui->Unlock();

    delete gs_mutexGui;

    // and free TLD slot
    (void)pthread_key_delete(gs_keySelf);
}

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

void wxMutexGuiEnter()
{
  gs_mutexGui->Lock();
}

void wxMutexGuiLeave()
{
  gs_mutexGui->Unlock();
}

#endif
  // wxUSE_THREADS
