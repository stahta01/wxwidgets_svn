/////////////////////////////////////////////////////////////////////////////
// Name:        thread.cpp
// Purpose:     wxThread Implementation
// Author:      Original from Wolfram Gloger/Guilhem Lavaux
// Modified by: Vadim Zeitlin to make it work :-)
// Created:     04/22/98
// RCS-ID:      $Id$
// Copyright:   (c) Wolfram Gloger (1996, 1997); Guilhem Lavaux (1998),
//                  Vadim Zeitlin (1999)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "thread.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#    include "wx/wx.h"
#endif

#if wxUSE_THREADS

#include "wx/msw/private.h"

#include "wx/module.h"
#include "wx/thread.h"

#ifdef Yield
#    undef Yield
#endif

// must have this symbol defined to get _beginthread/_endthread declarations
#ifndef _MT
    #define _MT
#endif

#if defined(__BORLANDC__)
    #if !defined(__MT__)
        // I can't set -tWM in the IDE (anyone?) so have to do this
        #define __MT__
    #endif

    #if !defined(__MFC_COMPAT__)
        // Needed to know about _beginthreadex etc..
        #define __MFC_COMPAT__
    #endif
#endif // BC++

// define wxUSE_BEGIN_THREAD if the compiler has _beginthreadex() function
// which should be used instead of Win32 ::CreateThread() if possible
#if defined(__VISUALC__) || \
    (defined(__BORLANDC__) && (__BORLANDC__ >= 0x500)) || \
    (defined(__GNUG__) && defined(__MSVCRT__)) || \
    defined(__WATCOMC__) || defined(__MWERKS__)

    #undef wxUSE_BEGIN_THREAD
    #define wxUSE_BEGIN_THREAD
#endif

#ifdef wxUSE_BEGIN_THREAD
    // this is where _beginthreadex() is declared
    #include <process.h>

    // the return type of the thread function entry point
    typedef unsigned THREAD_RETVAL;

    // the calling convention of the thread function entry point
    #define THREAD_CALLCONV __stdcall
#else
    // the settings for CreateThread()
    typedef DWORD THREAD_RETVAL;
    #define THREAD_CALLCONV WINAPI
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

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
// this module globals
// ----------------------------------------------------------------------------

// TLS index of the slot where we store the pointer to the current thread
static DWORD gs_tlsThisThread = 0xFFFFFFFF;

// id of the main thread - the one which can call GUI functions without first
// calling wxMutexGuiEnter()
static DWORD gs_idMainThread = 0;

// if it's FALSE, some secondary thread is holding the GUI lock
static bool gs_bGuiOwnedByMainThread = TRUE;

// critical section which controls access to all GUI functions: any secondary
// thread (i.e. except the main one) must enter this crit section before doing
// any GUI calls
static wxCriticalSection *gs_critsectGui = NULL;

// critical section which protects gs_nWaitingForGui variable
static wxCriticalSection *gs_critsectWaitingForGui = NULL;

// number of threads waiting for GUI in wxMutexGuiEnter()
static size_t gs_nWaitingForGui = 0;

// are we waiting for a thread termination?
static bool gs_waitingForThread = FALSE;

// ============================================================================
// Windows implementation of thread classes
// ============================================================================

// ----------------------------------------------------------------------------
// wxMutex implementation
// ----------------------------------------------------------------------------

class wxMutexInternal
{
public:
    wxMutexInternal()
    {
        m_mutex = ::CreateMutex(NULL, FALSE, NULL);
        if ( !m_mutex )
        {
            wxLogSysError(_("Can not create mutex"));
        }
    }

    ~wxMutexInternal() { if ( m_mutex ) ::CloseHandle(m_mutex); }

public:
    HANDLE m_mutex;
};

wxMutex::wxMutex()
{
    m_internal = new wxMutexInternal;

    m_locked = 0;
}

wxMutex::~wxMutex()
{
    if ( m_locked > 0 )
    {
        wxLogDebug(_T("Warning: freeing a locked mutex (%d locks)."), m_locked);
    }

    delete m_internal;
}

wxMutexError wxMutex::Lock()
{
    DWORD ret;

    ret = WaitForSingleObject(m_internal->m_mutex, INFINITE);
    switch ( ret )
    {
        case WAIT_ABANDONED:
            return wxMUTEX_BUSY;

        case WAIT_OBJECT_0:
            // ok
            break;

        case WAIT_FAILED:
            wxLogSysError(_("Couldn't acquire a mutex lock"));
            return wxMUTEX_MISC_ERROR;

        case WAIT_TIMEOUT:
        default:
            wxFAIL_MSG(wxT("impossible return value in wxMutex::Lock"));
    }

    m_locked++;
    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::TryLock()
{
    DWORD ret;

    ret = WaitForSingleObject(m_internal->m_mutex, 0);
    if (ret == WAIT_TIMEOUT || ret == WAIT_ABANDONED)
        return wxMUTEX_BUSY;

    m_locked++;
    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::Unlock()
{
    if (m_locked > 0)
        m_locked--;

    BOOL ret = ReleaseMutex(m_internal->m_mutex);
    if ( ret == 0 )
    {
        wxLogSysError(_("Couldn't release a mutex"));
        return wxMUTEX_MISC_ERROR;
    }

    return wxMUTEX_NO_ERROR;
}

// ==========================================================================
// wxSemaphore
// ==========================================================================

// --------------------------------------------------------------------------
// wxSemaphoreInternal
// --------------------------------------------------------------------------

class wxSemaphoreInternal
{
public:
    wxSemaphoreInternal( int initialcount = 0, int maxcount = 0 );
    ~wxSemaphoreInternal();

    void Wait();
    bool TryWait();

    bool Wait( unsigned long timeout_millis );

    void Post();

private:
    HANDLE m_semaphore;
};

wxSemaphoreInternal::wxSemaphoreInternal( int initialcount, int maxcount )
{
    if ( maxcount == 0 )
    {
        // make it practically infinite
        maxcount = INT_MAX;
    }

    m_semaphore = ::CreateSemaphore( NULL, initialcount, maxcount, NULL );
    if ( !m_semaphore )
    {
        wxLogLastError(_T("CreateSemaphore()"));
    }
}

wxSemaphoreInternal::~wxSemaphoreInternal()
{
    CloseHandle( m_semaphore );
}

void wxSemaphoreInternal::Wait()
{
    if ( ::WaitForSingleObject( m_semaphore, INFINITE ) != WAIT_OBJECT_0 )
    {
        wxLogLastError(_T("WaitForSingleObject"));
    }
}

bool wxSemaphoreInternal::TryWait()
{
    return Wait(0);
}

bool wxSemaphoreInternal::Wait( unsigned long timeout_millis )
{
    DWORD result = ::WaitForSingleObject( m_semaphore, timeout_millis );

    switch ( result )
    {
        case WAIT_OBJECT_0:
           return TRUE;

        case WAIT_TIMEOUT:
           break;

        default:
            wxLogLastError(_T("WaitForSingleObject()"));
    }

    return FALSE;
}

void wxSemaphoreInternal::Post()
{
    if ( !::ReleaseSemaphore( m_semaphore, 1, NULL ) )
    {
        wxLogLastError(_T("ReleaseSemaphore"));
    }
}

// --------------------------------------------------------------------------
// wxSemaphore
// --------------------------------------------------------------------------

wxSemaphore::wxSemaphore( int initialcount, int maxcount )
{
    m_internal = new wxSemaphoreInternal( initialcount, maxcount );
}

wxSemaphore::~wxSemaphore()
{
    delete m_internal;
}

void wxSemaphore::Wait()
{
    m_internal->Wait();
}

bool wxSemaphore::TryWait()
{
    return m_internal->TryWait();
}

bool wxSemaphore::Wait( unsigned long timeout_millis )
{
    return m_internal->Wait( timeout_millis );
}

void wxSemaphore::Post()
{
    m_internal->Post();
}


// ==========================================================================
// wxCondition
// ==========================================================================

// --------------------------------------------------------------------------
// wxConditionInternal
// --------------------------------------------------------------------------

class wxConditionInternal
{
public:
    wxConditionInternal(wxMutex& mutex);

    void Wait();

    bool Wait( unsigned long timeout_millis );

    void Signal();

    void Broadcast();

private:
    int m_numWaiters;
    wxMutex m_mutexNumWaiters;

    wxMutex& m_mutex;

    wxSemaphore m_semaphore;

    DECLARE_NO_COPY_CLASS(wxConditionInternal)
};

wxConditionInternal::wxConditionInternal(wxMutex& mutex)
                   : m_mutex(mutex)
{

    m_numWaiters = 0;
}

void wxConditionInternal::Wait()
{
    // increment the number of waiters
    m_mutexNumWaiters.Lock();
    m_numWaiters++;
    m_mutexNumWaiters.Unlock();

    m_mutex.Unlock();

    // a potential race condition can occur here
    //
    // after a thread increments nwaiters, and unlocks the mutex and before the
    // semaphore.Wait() is called, if another thread can cause a signal to be
    // generated
    //
    // this race condition is handled by using a semaphore and incrementing the
    // semaphore only if 'nwaiters' is greater that zero since the semaphore,
    // can 'remember' signals the race condition will not occur

    // wait ( if necessary ) and decrement semaphore
    m_semaphore.Wait();

    m_mutex.Lock();
}

bool wxConditionInternal::Wait( unsigned long timeout_millis )
{
    m_mutexNumWaiters.Lock();
    m_numWaiters++;
    m_mutexNumWaiters.Unlock();

    m_mutex.Unlock();

    // a race condition can occur at this point in the code
    //
    // please see the comments in Wait(), for details

    bool success = TRUE;

    bool result = m_semaphore.Wait( timeout_millis );

    if ( !result )
    {
        // another potential race condition exists here it is caused when a
        // 'waiting' thread timesout, and returns from WaitForSingleObject, but
        // has not yet decremented 'nwaiters'.
        //
        // at this point if another thread calls signal() then the semaphore
        // will be incremented, but the waiting thread will miss it.
        //
        // to handle this particular case, the waiting thread calls
        // WaitForSingleObject again with a timeout of 0, after locking
        // 'nwaiters_mutex'. this call does not block because of the zero
        // timeout, but will allow the waiting thread to catch the missed
        // signals.
        m_mutexNumWaiters.Lock();
        result = m_semaphore.Wait( 0 );

        if ( !result )
        {
            m_numWaiters--;
            success = FALSE;
        }

        m_mutexNumWaiters.Unlock();
    }

    m_mutex.Lock();

    return success;
}

void wxConditionInternal::Signal()
{
    m_mutexNumWaiters.Lock();

    if ( m_numWaiters > 0 )
    {
        // increment the semaphore by 1
        m_semaphore.Post();

        m_numWaiters--;
    }

    m_mutexNumWaiters.Unlock();
}

void wxConditionInternal::Broadcast()
{
    m_mutexNumWaiters.Lock();

    while ( m_numWaiters > 0 )
    {
        m_semaphore.Post();
        m_numWaiters--;
    }

    m_mutexNumWaiters.Unlock();
}

// ----------------------------------------------------------------------------
// wxCondition implementation
// ----------------------------------------------------------------------------

wxCondition::wxCondition(wxMutex& mutex)
{
    m_internal = new wxConditionInternal( mutex );
}

wxCondition::~wxCondition()
{
    delete m_internal;
}

void wxCondition::Wait()
{
    m_internal->Wait();
}

bool wxCondition::Wait( unsigned long timeout_millis )
{
    return m_internal->Wait(timeout_millis);
}

void wxCondition::Signal()
{
    m_internal->Signal();
}

void wxCondition::Broadcast()
{
    m_internal->Broadcast();
}

// ----------------------------------------------------------------------------
// wxCriticalSection implementation
// ----------------------------------------------------------------------------

wxCriticalSection::wxCriticalSection()
{
#ifdef __WXDEBUG__
    // Done this way to stop warnings during compilation about statement
    // always being FALSE
    int csSize = sizeof(CRITICAL_SECTION);
    int bSize  = sizeof(m_buffer);
    wxASSERT_MSG( csSize <= bSize,
                  _T("must increase buffer size in wx/thread.h") );
#endif

    ::InitializeCriticalSection((CRITICAL_SECTION *)m_buffer);
}

wxCriticalSection::~wxCriticalSection()
{
    ::DeleteCriticalSection((CRITICAL_SECTION *)m_buffer);
}

void wxCriticalSection::Enter()
{
    ::EnterCriticalSection((CRITICAL_SECTION *)m_buffer);
}

void wxCriticalSection::Leave()
{
    ::LeaveCriticalSection((CRITICAL_SECTION *)m_buffer);
}

// ----------------------------------------------------------------------------
// wxThread implementation
// ----------------------------------------------------------------------------

// wxThreadInternal class
// ----------------------

class wxThreadInternal
{
public:
    wxThreadInternal()
    {
        m_hThread = 0;
        m_state = STATE_NEW;
        m_priority = WXTHREAD_DEFAULT_PRIORITY;
    }

    ~wxThreadInternal()
    {
        Free();
    }

    void Free()
    {
        if ( m_hThread )
        {
            if ( !::CloseHandle(m_hThread) )
            {
                wxLogLastError(wxT("CloseHandle(thread)"));
            }

            m_hThread = 0;
        }
    }

    // create a new (suspended) thread (for the given thread object)
    bool Create(wxThread *thread, unsigned int stackSize);

    // suspend/resume/terminate
    bool Suspend();
    bool Resume();
    void Cancel() { m_state = STATE_CANCELED; }

    // thread state
    void SetState(wxThreadState state) { m_state = state; }
    wxThreadState GetState() const { return m_state; }

    // thread priority
    void SetPriority(unsigned int priority);
    unsigned int GetPriority() const { return m_priority; }

    // thread handle and id
    HANDLE GetHandle() const { return m_hThread; }
    DWORD  GetId() const { return m_tid; }

    // thread function
    static THREAD_RETVAL THREAD_CALLCONV WinThreadStart(void *thread);

private:
    HANDLE        m_hThread;    // handle of the thread
    wxThreadState m_state;      // state, see wxThreadState enum
    unsigned int  m_priority;   // thread priority in "wx" units
    DWORD         m_tid;        // thread id
};

THREAD_RETVAL THREAD_CALLCONV wxThreadInternal::WinThreadStart(void *param)
{
    THREAD_RETVAL rc;
    bool wasCancelled;

    // first of all, check whether we hadn't been cancelled already and don't
    // start the user code at all then
    wxThread *thread = (wxThread *)param;
    if ( thread->m_internal->GetState() == STATE_EXITED )
    {
        rc = (THREAD_RETVAL)-1;
        wasCancelled = TRUE;
    }
    else // do run thread
    {
        // store the thread object in the TLS
        if ( !::TlsSetValue(gs_tlsThisThread, thread) )
        {
            wxLogSysError(_("Can not start thread: error writing TLS."));

            return (DWORD)-1;
        }

        rc = (THREAD_RETVAL)thread->Entry();

        // enter m_critsect before changing the thread state
        thread->m_critsect.Enter();
        wasCancelled = thread->m_internal->GetState() == STATE_CANCELED;
        thread->m_internal->SetState(STATE_EXITED);
        thread->m_critsect.Leave();
    }

    thread->OnExit();

    // if the thread was cancelled (from Delete()), then its handle is still
    // needed there
    if ( thread->IsDetached() && !wasCancelled )
    {
        // auto delete
        delete thread;
    }
    //else: the joinable threads handle will be closed when Wait() is done

    return rc;
}

void wxThreadInternal::SetPriority(unsigned int priority)
{
    m_priority = priority;

    // translate wxWindows priority to the Windows one
    int win_priority;
    if (m_priority <= 20)
        win_priority = THREAD_PRIORITY_LOWEST;
    else if (m_priority <= 40)
        win_priority = THREAD_PRIORITY_BELOW_NORMAL;
    else if (m_priority <= 60)
        win_priority = THREAD_PRIORITY_NORMAL;
    else if (m_priority <= 80)
        win_priority = THREAD_PRIORITY_ABOVE_NORMAL;
    else if (m_priority <= 100)
        win_priority = THREAD_PRIORITY_HIGHEST;
    else
    {
        wxFAIL_MSG(wxT("invalid value of thread priority parameter"));
        win_priority = THREAD_PRIORITY_NORMAL;
    }

    if ( !::SetThreadPriority(m_hThread, win_priority) )
    {
        wxLogSysError(_("Can't set thread priority"));
    }
}

bool wxThreadInternal::Create(wxThread *thread, unsigned int stackSize)
{
    // for compilers which have it, we should use C RTL function for thread
    // creation instead of Win32 API one because otherwise we will have memory
    // leaks if the thread uses C RTL (and most threads do)
#ifdef wxUSE_BEGIN_THREAD

    // Watcom is reported to not like 0 stack size (which means "use default"
    // for the other compilers and is also the default value for stackSize)
#ifdef __WATCOMC__
    if ( !stackSize )
        stackSize = 10240;
#endif // __WATCOMC__

    m_hThread = (HANDLE)_beginthreadex
                        (
                          NULL,                             // default security
                          stackSize,
                          wxThreadInternal::WinThreadStart, // entry point
                          thread,
                          CREATE_SUSPENDED,
                          (unsigned int *)&m_tid
                        );
#else // compiler doesn't have _beginthreadex
    m_hThread = ::CreateThread
                  (
                    NULL,                               // default security
                    stackSize,                          // stack size
                    wxThreadInternal::WinThreadStart,   // thread entry point
                    (LPVOID)thread,                     // parameter
                    CREATE_SUSPENDED,                   // flags
                    &m_tid                              // [out] thread id
                  );
#endif // _beginthreadex/CreateThread

    if ( m_hThread == NULL )
    {
        wxLogSysError(_("Can't create thread"));

        return FALSE;
    }

    if ( m_priority != WXTHREAD_DEFAULT_PRIORITY )
    {
        SetPriority(m_priority);
    }

    return TRUE;
}

bool wxThreadInternal::Suspend()
{
    DWORD nSuspendCount = ::SuspendThread(m_hThread);
    if ( nSuspendCount == (DWORD)-1 )
    {
        wxLogSysError(_("Can not suspend thread %x"), m_hThread);

        return FALSE;
    }

    m_state = STATE_PAUSED;

    return TRUE;
}

bool wxThreadInternal::Resume()
{
    DWORD nSuspendCount = ::ResumeThread(m_hThread);
    if ( nSuspendCount == (DWORD)-1 )
    {
        wxLogSysError(_("Can not resume thread %x"), m_hThread);

        return FALSE;
    }

    // don't change the state from STATE_EXITED because it's special and means
    // we are going to terminate without running any user code - if we did it,
    // the codei n Delete() wouldn't work
    if ( m_state != STATE_EXITED )
    {
        m_state = STATE_RUNNING;
    }

    return TRUE;
}

// static functions
// ----------------

wxThread *wxThread::This()
{
    wxThread *thread = (wxThread *)::TlsGetValue(gs_tlsThisThread);

    // be careful, 0 may be a valid return value as well
    if ( !thread && (::GetLastError() != NO_ERROR) )
    {
        wxLogSysError(_("Couldn't get the current thread pointer"));

        // return NULL...
    }

    return thread;
}

bool wxThread::IsMain()
{
    return ::GetCurrentThreadId() == gs_idMainThread;
}

#ifdef Yield
#undef Yield
#endif

void wxThread::Yield()
{
    // 0 argument to Sleep() is special and means to just give away the rest of
    // our timeslice
    ::Sleep(0);
}

void wxThread::Sleep(unsigned long milliseconds)
{
    ::Sleep(milliseconds);
}

int wxThread::GetCPUCount()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    return si.dwNumberOfProcessors;
}

unsigned long wxThread::GetCurrentId()
{
    return (unsigned long)::GetCurrentThreadId();
}

bool wxThread::SetConcurrency(size_t level)
{
    wxASSERT_MSG( IsMain(), _T("should only be called from the main thread") );

    // ok only for the default one
    if ( level == 0 )
        return 0;

    // get system affinity mask first
    HANDLE hProcess = ::GetCurrentProcess();
    DWORD dwProcMask, dwSysMask;
    if ( ::GetProcessAffinityMask(hProcess, &dwProcMask, &dwSysMask) == 0 )
    {
        wxLogLastError(_T("GetProcessAffinityMask"));

        return FALSE;
    }

    // how many CPUs have we got?
    if ( dwSysMask == 1 )
    {
        // don't bother with all this complicated stuff - on a single
        // processor system it doesn't make much sense anyhow
        return level == 1;
    }

    // calculate the process mask: it's a bit vector with one bit per
    // processor; we want to schedule the process to run on first level
    // CPUs
    DWORD bit = 1;
    while ( bit )
    {
        if ( dwSysMask & bit )
        {
            // ok, we can set this bit
            dwProcMask |= bit;

            // another process added
            if ( !--level )
            {
                // and that's enough
                break;
            }
        }

        // next bit
        bit <<= 1;
    }

    // could we set all bits?
    if ( level != 0 )
    {
        wxLogDebug(_T("bad level %u in wxThread::SetConcurrency()"), level);

        return FALSE;
    }

    // set it: we can't link to SetProcessAffinityMask() because it doesn't
    // exist in Win9x, use RT binding instead

    typedef BOOL (*SETPROCESSAFFINITYMASK)(HANDLE, DWORD);

    // can use static var because we're always in the main thread here
    static SETPROCESSAFFINITYMASK pfnSetProcessAffinityMask = NULL;

    if ( !pfnSetProcessAffinityMask )
    {
        HMODULE hModKernel = ::LoadLibrary(_T("kernel32"));
        if ( hModKernel )
        {
            pfnSetProcessAffinityMask = (SETPROCESSAFFINITYMASK)
                ::GetProcAddress(hModKernel, "SetProcessAffinityMask");
        }

        // we've discovered a MT version of Win9x!
        wxASSERT_MSG( pfnSetProcessAffinityMask,
                      _T("this system has several CPUs but no SetProcessAffinityMask function?") );
    }

    if ( !pfnSetProcessAffinityMask )
    {
        // msg given above - do it only once
        return FALSE;
    }

    if ( pfnSetProcessAffinityMask(hProcess, dwProcMask) == 0 )
    {
        wxLogLastError(_T("SetProcessAffinityMask"));

        return FALSE;
    }

    return TRUE;
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

wxThreadError wxThread::Create(unsigned int stackSize)
{
    wxCriticalSectionLocker lock(m_critsect);

    if ( !m_internal->Create(this, stackSize) )
        return wxTHREAD_NO_RESOURCE;

    return wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Run()
{
    wxCriticalSectionLocker lock(m_critsect);

    if ( m_internal->GetState() != STATE_NEW )
    {
        // actually, it may be almost any state at all, not only STATE_RUNNING
        return wxTHREAD_RUNNING;
    }

    // the thread has just been created and is still suspended - let it run
    return Resume();
}

// suspend/resume thread
// ---------------------

wxThreadError wxThread::Pause()
{
    wxCriticalSectionLocker lock(m_critsect);

    return m_internal->Suspend() ? wxTHREAD_NO_ERROR : wxTHREAD_MISC_ERROR;
}

wxThreadError wxThread::Resume()
{
    wxCriticalSectionLocker lock(m_critsect);

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

    return rc;
}

wxThreadError wxThread::Delete(ExitCode *pRc)
{
    ExitCode rc = 0;

    // Delete() is always safe to call, so consider all possible states

    // we might need to resume the thread, but we might also not need to cancel
    // it if it doesn't run yet
    bool shouldResume = FALSE,
         shouldCancel = TRUE,
         isRunning = FALSE;

    // check if the thread already started to run
    {
        wxCriticalSectionLocker lock(m_critsect);

        if ( m_internal->GetState() == STATE_NEW )
        {
            // WinThreadStart() will see it and terminate immediately, no need
            // to cancel the thread - but we still need to resume it to let it
            // run
            m_internal->SetState(STATE_EXITED);

            Resume();   // it knows about STATE_EXITED special case

            shouldCancel = FALSE;
            isRunning = TRUE;

            // shouldResume is correctly set to FALSE here
        }
        else
        {
            shouldResume = IsPaused();
        }
    }

    // resume the thread if it is paused
    if ( shouldResume )
        Resume();

    HANDLE hThread = m_internal->GetHandle();

    // does is still run?
    if ( isRunning || IsRunning() )
    {
        if ( IsMain() )
        {
            // set flag for wxIsWaitingForThread()
            gs_waitingForThread = TRUE;

#if wxUSE_GUI
            wxBeginBusyCursor();
#endif // wxUSE_GUI
        }

        // ask the thread to terminate
        if ( shouldCancel )
        {
            wxCriticalSectionLocker lock(m_critsect);

            m_internal->Cancel();
        }

#if wxUSE_GUI
        // we can't just wait for the thread to terminate because it might be
        // calling some GUI functions and so it will never terminate before we
        // process the Windows messages that result from these functions
        DWORD result;
        do
        {
            result = ::MsgWaitForMultipleObjects
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

    // although the thread might be already in the EXITED state it might not
    // have terminated yet and so we are not sure that it has actually
    // terminated if the "if" above hadn't been taken
    do
    {
        if ( !::GetExitCodeThread(hThread, (LPDWORD)&rc) )
        {
            wxLogLastError(wxT("GetExitCodeThread"));

            rc = (ExitCode)-1;
        }
    } while ( (DWORD)rc == STILL_ACTIVE );

    if ( IsDetached() )
    {
        // if the thread exits normally, this is done in WinThreadStart, but in
        // this case it would have been too early because
        // MsgWaitForMultipleObject() would fail if the thread handle was
        // closed while we were waiting on it, so we must do it here
        delete this;
    }

    if ( pRc )
        *pRc = rc;

    return rc == (ExitCode)-1 ? wxTHREAD_MISC_ERROR : wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Kill()
{
    if ( !IsRunning() )
        return wxTHREAD_NOT_RUNNING;

    if ( !::TerminateThread(m_internal->GetHandle(), (DWORD)-1) )
    {
        wxLogSysError(_("Couldn't terminate thread"));

        return wxTHREAD_MISC_ERROR;
    }

    m_internal->Free();

    if ( IsDetached() )
    {
        delete this;
    }

    return wxTHREAD_NO_ERROR;
}

void wxThread::Exit(ExitCode status)
{
    m_internal->Free();

    if ( IsDetached() )
    {
        delete this;
    }

#ifdef wxUSE_BEGIN_THREAD
    _endthreadex((unsigned)status);
#else // !VC++
    ::ExitThread((DWORD)status);
#endif // VC++/!VC++

    wxFAIL_MSG(wxT("Couldn't return from ExitThread()!"));
}

// priority setting
// ----------------

void wxThread::SetPriority(unsigned int prio)
{
    wxCriticalSectionLocker lock(m_critsect);

    m_internal->SetPriority(prio);
}

unsigned int wxThread::GetPriority() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect); // const_cast

    return m_internal->GetPriority();
}

unsigned long wxThread::GetId() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect); // const_cast

    return (unsigned long)m_internal->GetId();
}

bool wxThread::IsRunning() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect); // const_cast

    return m_internal->GetState() == STATE_RUNNING;
}

bool wxThread::IsAlive() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect); // const_cast

    return (m_internal->GetState() == STATE_RUNNING) ||
           (m_internal->GetState() == STATE_PAUSED);
}

bool wxThread::IsPaused() const
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect); // const_cast

    return m_internal->GetState() == STATE_PAUSED;
}

bool wxThread::TestDestroy()
{
    wxCriticalSectionLocker lock((wxCriticalSection &)m_critsect); // const_cast

    return m_internal->GetState() == STATE_CANCELED;
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
    // allocate TLS index for storing the pointer to the current thread
    gs_tlsThisThread = ::TlsAlloc();
    if ( gs_tlsThisThread == 0xFFFFFFFF )
    {
        // in normal circumstances it will only happen if all other
        // TLS_MINIMUM_AVAILABLE (>= 64) indices are already taken - in other
        // words, this should never happen
        wxLogSysError(_("Thread module initialization failed: impossible to allocate index in thread local storage"));

        return FALSE;
    }

    // main thread doesn't have associated wxThread object, so store 0 in the
    // TLS instead
    if ( !::TlsSetValue(gs_tlsThisThread, (LPVOID)0) )
    {
        ::TlsFree(gs_tlsThisThread);
        gs_tlsThisThread = 0xFFFFFFFF;

        wxLogSysError(_("Thread module initialization failed: can not store value in thread local storage"));

        return FALSE;
    }

    gs_critsectWaitingForGui = new wxCriticalSection();

    gs_critsectGui = new wxCriticalSection();
    gs_critsectGui->Enter();

    // no error return for GetCurrentThreadId()
    gs_idMainThread = ::GetCurrentThreadId();

    return TRUE;
}

void wxThreadModule::OnExit()
{
    if ( !::TlsFree(gs_tlsThisThread) )
    {
        wxLogLastError(wxT("TlsFree failed."));
    }

    if ( gs_critsectGui )
    {
        gs_critsectGui->Leave();
        delete gs_critsectGui;
        gs_critsectGui = NULL;
    }

    delete gs_critsectWaitingForGui;
    gs_critsectWaitingForGui = NULL;
}

// ----------------------------------------------------------------------------
// under Windows, these functions are implemented using a critical section and
// not a mutex, so the names are a bit confusing
// ----------------------------------------------------------------------------

void WXDLLEXPORT wxMutexGuiEnter()
{
    // this would dead lock everything...
    wxASSERT_MSG( !wxThread::IsMain(),
                  wxT("main thread doesn't want to block in wxMutexGuiEnter()!") );

    // the order in which we enter the critical sections here is crucial!!

    // set the flag telling to the main thread that we want to do some GUI
    {
        wxCriticalSectionLocker enter(*gs_critsectWaitingForGui);

        gs_nWaitingForGui++;
    }

    wxWakeUpMainThread();

    // now we may block here because the main thread will soon let us in
    // (during the next iteration of OnIdle())
    gs_critsectGui->Enter();
}

void WXDLLEXPORT wxMutexGuiLeave()
{
    wxCriticalSectionLocker enter(*gs_critsectWaitingForGui);

    if ( wxThread::IsMain() )
    {
        gs_bGuiOwnedByMainThread = FALSE;
    }
    else
    {
        // decrement the number of threads waiting for GUI access now
        wxASSERT_MSG( gs_nWaitingForGui > 0,
                      wxT("calling wxMutexGuiLeave() without entering it first?") );

        gs_nWaitingForGui--;

        wxWakeUpMainThread();
    }

    gs_critsectGui->Leave();
}

void WXDLLEXPORT wxMutexGuiLeaveOrEnter()
{
    wxASSERT_MSG( wxThread::IsMain(),
                  wxT("only main thread may call wxMutexGuiLeaveOrEnter()!") );

    wxCriticalSectionLocker enter(*gs_critsectWaitingForGui);

    if ( gs_nWaitingForGui == 0 )
    {
        // no threads are waiting for GUI - so we may acquire the lock without
        // any danger (but only if we don't already have it)
        if ( !wxGuiOwnedByMainThread() )
        {
            gs_critsectGui->Enter();

            gs_bGuiOwnedByMainThread = TRUE;
        }
        //else: already have it, nothing to do
    }
    else
    {
        // some threads are waiting, release the GUI lock if we have it
        if ( wxGuiOwnedByMainThread() )
        {
            wxMutexGuiLeave();
        }
        //else: some other worker thread is doing GUI
    }
}

bool WXDLLEXPORT wxGuiOwnedByMainThread()
{
    return gs_bGuiOwnedByMainThread;
}

// wake up the main thread if it's in ::GetMessage()
void WXDLLEXPORT wxWakeUpMainThread()
{
    // sending any message would do - hopefully WM_NULL is harmless enough
    if ( !::PostThreadMessage(gs_idMainThread, WM_NULL, 0, 0) )
    {
        // should never happen
        wxLogLastError(wxT("PostThreadMessage(WM_NULL)"));
    }
}

bool WXDLLEXPORT wxIsWaitingForThread()
{
    return gs_waitingForThread;
}

#endif // wxUSE_THREADS

// vi:sts=4:sw=4:et
