/////////////////////////////////////////////////////////////////////////////
// Name:        thread.cpp
// Purpose:     wxThread Implementation. For Unix ports, see e.g. src/gtk
// Author:      Original from Wolfram Gloger/Guilhem Lavaux
// Modified by: David Webster
// Created:     04/22/98
// RCS-ID:      $Id$
// Copyright:   (c) Wolfram Gloger (1996, 1997); Guilhem Lavaux (1998)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_THREADS

#include <stdio.h>

#include "wx/module.h"
#include "wx/thread.h"

#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include <os2.h>
#include <bseerr.h>

// the possible states of the thread ("=>" shows all possible transitions from
// this state)
enum wxThreadState
{
    STATE_NEW,          // didn't start execution yet (=> RUNNING)
    STATE_RUNNING,      // thread is running (=> PAUSED, CANCELED)
    STATE_PAUSED,       // thread is temporarily suspended (=> RUNNING)
    STATE_CANCELED,     // thread should terminate a.s.a.p. (=> EXITED)
    STATE_EXITED        // thread is terminating
};

// ----------------------------------------------------------------------------
// static variables
// ----------------------------------------------------------------------------

// id of the main thread - the one which can call GUI functions without first
// calling wxMutexGuiEnter()
static ULONG                        s_ulIdMainThread = 0;
wxMutex*                            p_wxMainMutex;

// OS2 substitute for Tls pointer the current parent thread object
wxThread*                           m_pThread;    // pointer to the wxWindows thread object

// if it's FALSE, some secondary thread is holding the GUI lock
static bool s_bGuiOwnedByMainThread = TRUE;

// critical section which controls access to all GUI functions: any secondary
// thread (i.e. except the main one) must enter this crit section before doing
// any GUI calls
static wxCriticalSection *s_pCritsectGui = NULL;

// critical section which protects s_nWaitingForGui variable
static wxCriticalSection *s_pCritsectWaitingForGui = NULL;

// number of threads waiting for GUI in wxMutexGuiEnter()
static size_t s_nWaitingForGui = 0;

// are we waiting for a thread termination?
static bool s_bWaitingForThread = FALSE;

// ============================================================================
// OS/2 implementation of thread classes
// ============================================================================

// ----------------------------------------------------------------------------
// wxMutex implementation
// ----------------------------------------------------------------------------
class wxMutexInternal
{
public:
    HMTX                            m_vMutex;
};

wxMutex::wxMutex()
{
    APIRET                          ulrc;

    m_internal = new wxMutexInternal;
    ulrc = ::DosCreateMutexSem(NULL, &m_internal->m_vMutex, 0L, FALSE);
    if (ulrc != 0)
    {
        wxLogSysError(_("Can not create mutex."));
    }
    m_locked = 0;
}

wxMutex::~wxMutex()
{
    if (m_locked > 0)
        wxLogDebug(wxT("Warning: freeing a locked mutex (%d locks)."), m_locked);
    ::DosCloseMutexSem(m_internal->m_vMutex);
    m_internal->m_vMutex = NULL;
}

wxMutexError wxMutex::Lock()
{
    APIRET                          ulrc;

    ulrc = ::DosRequestMutexSem(m_internal->m_vMutex, SEM_INDEFINITE_WAIT);

    switch (ulrc)
    {
        case ERROR_TOO_MANY_SEM_REQUESTS:
            return wxMUTEX_BUSY;

        case NO_ERROR:
            // ok
            break;

        case ERROR_INVALID_HANDLE:
        case ERROR_INTERRUPT:
        case ERROR_SEM_OWNER_DIED:
            wxLogSysError(_("Couldn't acquire a mutex lock"));
            return wxMUTEX_MISC_ERROR;

        case ERROR_TIMEOUT:
        default:
            wxFAIL_MSG(wxT("impossible return value in wxMutex::Lock"));
    }
    m_locked++;
    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::TryLock()
{
    ULONG                           ulrc;

    ulrc = ::DosRequestMutexSem(m_internal->m_vMutex, SEM_IMMEDIATE_RETURN /*0L*/);
    if (ulrc == ERROR_TIMEOUT || ulrc == ERROR_TOO_MANY_SEM_REQUESTS)
        return wxMUTEX_BUSY;

    m_locked++;
    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::Unlock()
{
    APIRET                          ulrc;

    if (m_locked > 0)
        m_locked--;

    ulrc = ::DosReleaseMutexSem(m_internal->m_vMutex);
    if (ulrc != 0)
    {
        wxLogSysError(_("Couldn't release a mutex"));
        return wxMUTEX_MISC_ERROR;
    }
    return wxMUTEX_NO_ERROR;
}

// ----------------------------------------------------------------------------
// wxCondition implementation
// ----------------------------------------------------------------------------

class wxConditionInternal
{
public:
    inline wxConditionInternal ()
    {
        ::DosCreateEventSem(NULL, &m_vEvent, DC_SEM_SHARED, FALSE);
        if (!m_vEvent)
        {
            wxLogSysError(_("Can not create event semaphore."));
        }
        m_nWaiters = 0;
    }

    inline bool Wait(
      unsigned long                 ulTimeout
    )
    {
        APIRET                      ulrc;

        m_nWaiters++;
        ulrc = ::DosWaitEventSem(m_vEvent, ulTimeout);
        m_nWaiters--;
        return (ulrc != ERROR_TIMEOUT);
    }

    inline ~wxConditionInternal ()
    {
        APIRET                      ulrc;

        if (m_vEvent)
        {
            ulrc = ::DosCloseEventSem(m_vEvent);
            if (!ulrc)
            {
                wxLogLastError("DosCloseEventSem(m_vEvent)");
            }
        }
    }

    HEV                             m_vEvent;
    int                             m_nWaiters;
};

wxCondition::wxCondition()
{
    APIRET                          ulrc;
    ULONG                           ulCount;

    m_internal = new wxConditionInternal;
    ulrc = ::DosCreateEventSem(NULL, &m_internal->m_vEvent, 0L, FALSE);
    if (ulrc != 0)
    {
        wxLogSysError(_("Can not create event object."));
    }
    m_internal->m_nWaiters = 0;
    // ?? just for good measure?
    ::DosResetEventSem(m_internal->m_vEvent, &ulCount);
}

wxCondition::~wxCondition()
{
    ::DosCloseEventSem(m_internal->m_vEvent);
    delete m_internal;
    m_internal = NULL;
}

void wxCondition::Wait()
{
    (void)m_internal->Wait(SEM_INFINITE_WAIT);
}

bool wxCondition::Wait(
  unsigned long                     lSec
, unsigned long                     lNsec)
{
    return m_internal->Wait(lSec*1000 + lNsec/1000000);
}

void wxCondition::Signal()
{
    ::DosPostEventSem(m_internal->m_vEvent);
}

void wxCondition::Broadcast()
{
    int                             i;

    for (i = 0; i < m_internal->m_nWaiters; i++)
    {
        if (::DosPostEventSem(m_internal->m_vEvent) != 0)
        {
            wxLogSysError(_("Couldn't change the state of event object."));
        }
    }
}

// ----------------------------------------------------------------------------
// wxCriticalSection implementation
// ----------------------------------------------------------------------------

wxCriticalSection::wxCriticalSection()
{
}

wxCriticalSection::~wxCriticalSection()
{
}

void wxCriticalSection::Enter()
{
    ::DosEnterCritSec();
}

void wxCriticalSection::Leave()
{
    ::DosExitCritSec();
}

// ----------------------------------------------------------------------------
// wxThread implementation
// ----------------------------------------------------------------------------

// wxThreadInternal class
// ----------------------

class wxThreadInternal
{
public:
    inline wxThreadInternal()
    {
        m_hThread = 0;
        m_eState = STATE_NEW;
        m_nPriority = 0;
    }

    ~wxThreadInternal()
    {
        Free();
    }

    void Free()
    {
        if (m_hThread)
        {
            ::DosExit(0,0);
            m_hThread = 0;
        }
    }

    // create a new (suspended) thread (for the given thread object)
    bool Create(wxThread* pThread);

    // suspend/resume/terminate
    bool Suspend();
    bool Resume();
    inline void Cancel() { m_eState = STATE_CANCELED; }

    // thread state
    inline void SetState(wxThreadState eState) { m_eState = eState; }
    inline wxThreadState GetState() const { return m_eState; }

    // thread priority
    void                SetPriority(unsigned int nPriority);
    inline unsigned int GetPriority() const { return m_nPriority; }

    // thread handle and id
    inline TID GetHandle() const { return m_hThread; }
    TID  GetId() const { return m_hThread; }

    // thread function
    static DWORD OS2ThreadStart(wxThread *thread);

private:
    // Threads in OS/2 have only an ID, so m_hThread is both it's handle and ID
    // PM also has no real Tls mechanism to index pointers by so we'll just
    // keep track of the wxWindows parent object here.
    TID                             m_hThread;    // handle and ID of the thread
    wxThreadState                   m_eState;     // state, see wxThreadState enum
    unsigned int                    m_nPriority;  // thread priority in "wx" units
};

ULONG wxThreadInternal::OS2ThreadStart(
  wxThread*                         pThread
)
{
    m_pThread = pThread;

    DWORD                           dwRet = (DWORD)pThread->Entry();

    // enter m_critsect before changing the thread state
    pThread->m_critsect.Enter();

    bool                            bWasCancelled = thread->m_internal->GetState() == STATE_CANCELED;

    pThread->m_internal->SetState(STATE_EXITED);
    thread->m_critsect.Leave();

    pThread->OnExit();

    // if the thread was cancelled (from Delete()), then it the handle is still
    // needed there
    if (pThread->IsDetached() && !bWasCancelled)
    {
        // auto delete
        delete thread;
    }
    //else: the joinable threads handle will be closed when Wait() is done
    return dwRet;
}

void wxThreadInternal::SetPriority(
  unsigned int                      nPriority
)
{
    // translate wxWindows priority to the PM one
    ULONG                           ulOS2_Priority;

    m_nPriority = nPriority;

    if (m_nPriority <= 20)
        ulOS2_Priority = PRTYC_NOCHANGE;
    else if (m_nPriority <= 40)
        ulOS2_Priority = PRTYC_IDLETIME;
    else if (m_nPriority <= 60)
        ulOS2_Priority = PRTYC_REGULAR;
    else if (m_nPriority <= 80)
        ulOS2_Priority = PRTYC_TIMECRITICAL;
    else if (m_nPriority <= 100)
        ulOS2_Priority = PRTYC_FOREGROUNDSERVER;
    else
    {
        wxFAIL_MSG(wxT("invalid value of thread priority parameter"));
        ulOS2_Priority = PRTYC_REGULAR;
    }
    ulrc = ::DosSetPriority( PRTYS_THREAD
                            ,ulOS2_Priority
                            ,0
                            ,(ULONG)m_hThread
                           );
    if (ulrc != 0)
    {
        wxLogSysError(_("Can't set thread priority"));
    }
}

bool wxThreadInternal::Create(
  wxThread*                         pThread
)
{
    APIRET                          ulrc;

    ulrc = ::DosCreateThread( &m_hThread
                             ,(PFNTHREAD)wxThreadInternal::OS2ThreadStart
                             ,(ULONG)pThread
                             ,CREATE_SUSPENDED | STACK_SPARSE
                             ,8192L
                            );
    if(ulrc != 0)
    {
        wxLogSysError(_("Can't create thread"));

        return FALSE;
    }
    if (m_nPriority != WXTHREAD_DEFAULT_PRIORITY)
    {
        SetPriority(m_nPriority);
    }
    return(TRUE);
}

bool wxThreadInternal::Suspend()
{
    ULONG                           ulrc = ::DosSuspendThread(m_hThread);

    if (ulrc != 0)
    {
        wxLogSysError(_("Can not suspend thread %lu"), m_hThread);
        return FALSE;
    }
    m_eState = STATE_PAUSED;
    return TRUE;
}

bool wxThreadInternal::Resume()
{
    ULONG                           ulrc = ::DosResumeThread(m_hThread);

    if (ulrc != 0)
    {
        wxLogSysError(_("Can not suspend thread %lu"), m_hThread);
        return FALSE;
    }
    m_eState = STATE_PAUSED;
    return TRUE;
}

// static functions
// ----------------

wxThread *wxThread::This()
{
    wxThread*                       pThread = m_pThread;
    return pThread;
}

bool wxThread::IsMain()
{
    PTIB                            ptib;
    PPIB                            ppib;

    ::DosGetInfoBlocks(&ptib, &ppib);

    if (ptib->tib_ptib2->tib2_ultid == s_ulIdMainThread)
        return TRUE;
    return FALSE;
}

#ifdef Yield
    #undef Yield
#endif

void wxThread::Yield()
{
    ::DosSleep(0);
}

void wxThread::Sleep(
  unsigned long                     ulMilliseconds
)
{
    ::DosSleep(ulMilliseconds);
}

// ctor and dtor
// -------------

wxThread::wxThread(wxThreadKind kind)
{
    m_internal = new wxThreadInternal();

    m_isDetached = kind == wxTHREAD_DETACHED;
}

wxThread::~wxThread()
{
    delete m_internal;
}

// create/start thread
// -------------------

wxThreadError wxThread::Create()
{
    if ( !m_internal->Create(this) )
        return wxTHREAD_NO_RESOURCE;

    return wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Run()
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    if ( m_internal->GetState() != STATE_NEW )
    {
        // actually, it may be almost any state at all, not only STATE_RUNNING
        return wxTHREAD_RUNNING;
    }
    return Resume();
}

// suspend/resume thread
// ---------------------

wxThreadError wxThread::Pause()
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return m_internal->Suspend() ? wxTHREAD_NO_ERROR : wxTHREAD_MISC_ERROR;
}

wxThreadError wxThread::Resume()
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return m_internal->Resume() ? wxTHREAD_NO_ERROR : wxTHREAD_MISC_ERROR;
}

// stopping thread
// ---------------

wxThread::ExitCode wxThread::Wait()
{
    // although under Windows we can wait for any thread, it's an error to
    // wait for a detached one in wxWin API
    wxCHECK_MSG( !IsDetached(), (ExitCode)-1,
                 _T("can't wait for detached thread") );
    ExitCode rc = (ExitCode)-1;
    (void)Delete(&rc);
    m_internal->Free();
    return(rc);
}

wxThreadError wxThread::Delete(ExitCode *pRc)
{
    ExitCode rc = 0;

    // Delete() is always safe to call, so consider all possible states
    if (IsPaused())
        Resume();

    TID                             hThread = m_internal->GetHandle();

    if (IsRunning())
    {
        if (IsMain())
        {
            // set flag for wxIsWaitingForThread()
            gs_waitingForThread = TRUE;

#if wxUSE_GUI
            wxBeginBusyCursor();
#endif // wxUSE_GUI
        }

        // ask the thread to terminate
        {
            wxCriticalSectionLocker lock(m_critsect);
            m_internal->Cancel();
        }

#if wxUSE_GUI
        // we can't just wait for the thread to terminate because it might be
        // calling some GUI functions and so it will never terminate before we
        // process the Windows messages that result from these functions
        ULONG                       ulrc;
        do
        {
            ulrc = ::MsgWaitForMultipleObjects
                     (
                       1,              // number of objects to wait for
                       &hThread,       // the objects
                       FALSE,          // don't wait for all objects
                       INFINITE,       // no timeout
                       QS_ALLEVENTS    // return as soon as there are any events
                     );

            switch ( result )
            {
                case 0xFFFFFFFF:
                    // error
                    wxLogSysError(_("Can not wait for thread termination"));
                    Kill();
                    return wxTHREAD_KILLED;

                case WAIT_OBJECT_0:
                    // thread we're waiting for terminated
                    break;

                case WAIT_OBJECT_0 + 1:
                    // new message arrived, process it
                    if ( !wxTheApp->DoMessage() )
                    {
                        // WM_QUIT received: kill the thread
                        Kill();

                        return wxTHREAD_KILLED;
                    }

                    if ( IsMain() )
                    {
                        // give the thread we're waiting for chance to exit
                        // from the GUI call it might have been in
                        if ( (gs_nWaitingForGui > 0) && wxGuiOwnedByMainThread() )
                        {
                            wxMutexGuiLeave();
                        }
                    }

                    break;

                default:
                    wxFAIL_MSG(wxT("unexpected result of MsgWaitForMultipleObject"));
            }
        } while ( result != WAIT_OBJECT_0 );
#else // !wxUSE_GUI
        // simply wait for the thread to terminate
        //
        // OTOH, even console apps create windows (in wxExecute, for WinSock
        // &c), so may be use MsgWaitForMultipleObject() too here?
        if ( WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0 )
        {
            wxFAIL_MSG(wxT("unexpected result of WaitForSingleObject"));
        }
#endif // wxUSE_GUI/!wxUSE_GUI

        if ( IsMain() )
        {
            gs_waitingForThread = FALSE;

#if wxUSE_GUI
            wxEndBusyCursor();
#endif // wxUSE_GUI
        }
    }

    if ( !::GetExitCodeThread(hThread, (LPDWORD)&rc) )
    {
        wxLogLastError("GetExitCodeThread");

        rc = (ExitCode)-1;
    }

    if ( IsDetached() )
    {
        // if the thread exits normally, this is done in WinThreadStart, but in
        // this case it would have been too early because
        // MsgWaitForMultipleObject() would fail if the therad handle was
        // closed while we were waiting on it, so we must do it here
        delete this;
    }

    wxASSERT_MSG( (DWORD)rc != STILL_ACTIVE,
                  wxT("thread must be already terminated.") );

    if ( pRc )
        *pRc = rc;

    return rc == (ExitCode)-1 ? wxTHREAD_MISC_ERROR : wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Kill()
{
    if (!IsRunning())
        return wxTHREAD_NOT_RUNNING;

    ::DosKillThread(m_internal->GetHandle());
    delete this;
    return wxTHREAD_NO_ERROR;
}

void wxThread::Exit(
  void*                             pStatus
)
{
    delete this;
    ::DosExit(EXIT_THREAD, ULONG(pStatus));
    wxFAIL_MSG(wxT("Couldn't return from DosExit()!"));
}

void wxThread::SetPriority(
  unsigned int                      nPrio
)
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    m_internal->SetPriority(nPrio);
}

unsigned int wxThread::GetPriority() const
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return m_internal->GetPriority();
}

bool wxThread::IsRunning() const
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return m_internal->GetState() == STATE_RUNNING;
}

bool wxThread::IsAlive() const
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return (m_internal->GetState() == STATE_RUNNING) ||
           (m_internal->GetState() == STATE_PAUSED);
}

bool wxThread::IsPaused() const
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return (m_internal->GetState() == STATE_PAUSED);
}

bool wxThread::TestDestroy()
{
    wxCriticalSectionLocker         lock((wxCriticalSection &)m_critsect);

    return m_internal->GetState() == STATE_CANCELED;
}

wxThread::~wxThread()
{
    delete m_internal;
}

// ----------------------------------------------------------------------------
// Automatic initialization for thread module
// ----------------------------------------------------------------------------

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
    s_pCritsectWaitingForGui = new wxCriticalSection();

    s_pCritsectGui = new wxCriticalSection();
    s_pCritsectGui->Enter();

    PTIB                            ptib;
    PPIB                            ppib;

    ::DosGetInfoBlocks(&ptib, &ppib);

    s_ulIdMainThread = ptib->tib_ptib2->tib2_ultid;
    return TRUE;
}

void wxThreadModule::OnExit()
{
    if (s_pCritsectGui)
    {
        s_pCritsectGui->Leave();
        delete s_pCritsectGui;
        s_pCritsectGui = NULL;
    }

    wxDELETE(s_pCritsectWaitingForGui);
}

// ----------------------------------------------------------------------------
// Helper functions
// ----------------------------------------------------------------------------

// Does nothing under OS/2 [for now]
void WXDLLEXPORT wxWakeUpMainThread()
{
}

void WXDLLEXPORT wxMutexGuiLeave()
{
    wxCriticalSectionLocker enter(*s_pCritsectWaitingForGui);

    if ( wxThread::IsMain() )
    {
        s_bGuiOwnedByMainThread = FALSE;
    }
    else
    {
        // decrement the number of waiters now
        wxASSERT_MSG( s_nWaitingForGui > 0,
                      wxT("calling wxMutexGuiLeave() without entering it first?") );

        s_nWaitingForGui--;

        wxWakeUpMainThread();
    }

    s_pCritsectGui->Leave();
}

void WXDLLEXPORT wxMutexGuiLeaveOrEnter()
{
    wxASSERT_MSG( wxThread::IsMain(),
                  wxT("only main thread may call wxMutexGuiLeaveOrEnter()!") );

    wxCriticalSectionLocker enter(*s_pCritsectWaitingForGui);

    if ( s_nWaitingForGui == 0 )
    {
        // no threads are waiting for GUI - so we may acquire the lock without
        // any danger (but only if we don't already have it)
        if (!wxGuiOwnedByMainThread())
        {
            s_pCritsectGui->Enter();

            s_bGuiOwnedByMainThread = TRUE;
        }
        //else: already have it, nothing to do
    }
    else
    {
        // some threads are waiting, release the GUI lock if we have it
        if (wxGuiOwnedByMainThread())
        {
            wxMutexGuiLeave();
        }
        //else: some other worker thread is doing GUI
    }
}

bool WXDLLEXPORT wxGuiOwnedByMainThread()
{
    return s_bGuiOwnedByMainThread;
}

bool WXDLLEXPORT wxIsWaitingForThread()
{
    return s_bWaitingForThread;
}

#endif
  // wxUSE_THREADS
