/////////////////////////////////////////////////////////////////////////////
// Name:        thread.h
// Purpose:     Thread API
// Author:      Guilhem Lavaux
// Modified by: Vadim Zeitlin (modifications partly inspired by omnithreads
//              package from Olivetti & Oracle Research Laboratory)
// Created:     04/13/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __THREADH__
#define __THREADH__

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// get the value of wxUSE_THREADS configuration flag
#include "wx/setup.h"

#if wxUSE_THREADS

// only for wxUSE_THREADS - otherwise we'd get undefined symbols
#ifdef __GNUG__
    #pragma interface "thread.h"
#endif

// Windows headers define it
#ifdef Yield
    #undef Yield
#endif

#include "wx/module.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

enum wxMutexError
{
    wxMUTEX_NO_ERROR = 0,
    wxMUTEX_DEAD_LOCK,      // Mutex has been already locked by THE CALLING thread
    wxMUTEX_BUSY,           // Mutex has been already locked by ONE thread
    wxMUTEX_UNLOCKED,
    wxMUTEX_MISC_ERROR
};

enum wxThreadError
{
    wxTHREAD_NO_ERROR = 0,      // No error
    wxTHREAD_NO_RESOURCE,       // No resource left to create a new thread
    wxTHREAD_RUNNING,           // The thread is already running
    wxTHREAD_NOT_RUNNING,       // The thread isn't running
    wxTHREAD_KILLED,            // Thread we waited for had to be killed
    wxTHREAD_MISC_ERROR         // Some other error
};

enum wxThreadKind
{
    wxTHREAD_DETACHED,
    wxTHREAD_JOINABLE
};

// defines the interval of priority
enum
{
    WXTHREAD_MIN_PRIORITY      = 0u,
    WXTHREAD_DEFAULT_PRIORITY  = 50u,
    WXTHREAD_MAX_PRIORITY      = 100u
};

// ----------------------------------------------------------------------------
// A mutex object is a synchronization object whose state is set to signaled
// when it is not owned by any thread, and nonsignaled when it is owned. Its
// name comes from its usefulness in coordinating mutually-exclusive access to
// a shared resource. Only one thread at a time can own a mutex object.
// ----------------------------------------------------------------------------

// you should consider wxMutexLocker whenever possible instead of directly
// working with wxMutex class - it is safer
class WXDLLEXPORT wxConditionInternal;
class WXDLLEXPORT wxMutexInternal;
class WXDLLEXPORT wxMutex
{
public:
    // constructor & destructor
    wxMutex();
    ~wxMutex();

    // Lock the mutex.
    wxMutexError Lock();
    // Try to lock the mutex: if it can't, returns immediately with an error.
    wxMutexError TryLock();
    // Unlock the mutex.
    wxMutexError Unlock();

    // Returns true if the mutex is locked.
    bool IsLocked() const { return (m_locked > 0); }

protected:
    // no assignment operator nor copy ctor
    wxMutex(const wxMutex&);
    wxMutex& operator=(const wxMutex&);

    int m_locked;
    wxMutexInternal *m_internal;

    friend class wxConditionInternal;
};

// a helper class which locks the mutex in the ctor and unlocks it in the dtor:
// this ensures that mutex is always unlocked, even if the function returns or
// throws an exception before it reaches the end
class WXDLLEXPORT wxMutexLocker
{
public:
    // lock the mutex in the ctor
    wxMutexLocker(wxMutex& mutex) : m_mutex(mutex)
        { m_isOk = m_mutex.Lock() == wxMUTEX_NO_ERROR; }

    // returns TRUE if mutex was successfully locked in ctor
    bool IsOk() const
        { return m_isOk; }

    // unlock the mutex in dtor
    ~wxMutexLocker()
        { if ( IsOk() ) m_mutex.Unlock(); }

private:
    // no assignment operator nor copy ctor
    wxMutexLocker(const wxMutexLocker&);
    wxMutexLocker& operator=(const wxMutexLocker&);

    bool     m_isOk;
    wxMutex& m_mutex;
};

// ----------------------------------------------------------------------------
// Critical section: this is the same as mutex but is only visible to the
// threads of the same process. For the platforms which don't have native
// support for critical sections, they're implemented entirely in terms of
// mutexes.
//
// NB: wxCriticalSection object does not allocate any memory in its ctor
//     which makes it possible to have static globals of this class
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxCriticalSectionInternal;

// in order to avoid any overhead under platforms where critical sections are
// just mutexes make all wxCriticalSection class functions inline
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #define WXCRITICAL_INLINE   inline

    #define wxCRITSECT_IS_MUTEX 1
#else // MSW || OS2
    #define WXCRITICAL_INLINE

    #define wxCRITSECT_IS_MUTEX 0
#endif // MSW/!MSW

// you should consider wxCriticalSectionLocker whenever possible instead of
// directly working with wxCriticalSection class - it is safer
class WXDLLEXPORT wxCriticalSection
{
public:
    // ctor & dtor
    WXCRITICAL_INLINE wxCriticalSection();
    WXCRITICAL_INLINE ~wxCriticalSection();

    // enter the section (the same as locking a mutex)
    WXCRITICAL_INLINE void Enter();
    // leave the critical section (same as unlocking a mutex)
    WXCRITICAL_INLINE void Leave();

private:
    // no assignment operator nor copy ctor
    wxCriticalSection(const wxCriticalSection&);
    wxCriticalSection& operator=(const wxCriticalSection&);

#if wxCRITSECT_IS_MUTEX
    wxMutex m_mutex;
#elif defined(__WXMSW__)
    // we can't allocate any memory in the ctor, so use placement new -
    // unfortunately, we have to hardcode the sizeof() here because we can't
    // include windows.h from this public header
    char m_buffer[24];
#elif !defined(__WXPM__)
    wxCriticalSectionInternal *m_critsect;
#else
    // nothing for OS/2
#endif // !Unix/Unix
};

// keep your preprocessor name space clean
#undef WXCRITICAL_INLINE

// wxCriticalSectionLocker is the same to critical sections as wxMutexLocker is
// to th mutexes
class WXDLLEXPORT wxCriticalSectionLocker
{
public:
    inline wxCriticalSectionLocker(wxCriticalSection& critsect);
    inline ~wxCriticalSectionLocker();

private:
    // no assignment operator nor copy ctor
    wxCriticalSectionLocker(const wxCriticalSectionLocker&);
    wxCriticalSectionLocker& operator=(const wxCriticalSectionLocker&);

    wxCriticalSection& m_critsect;
};

// ----------------------------------------------------------------------------
// wxCondition models a POSIX condition variable which allows one (or more)
// thread(s) to wait until some condition is fulfilled
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxCondition
{
public:
    // constructor and destructor

    // Each wxCondition object is associated with with a wxMutex object The
    // mutex object MUST be locked before calling Wait()
    wxCondition(wxMutex& mutex);

    // dtor is not virtual, don't use this class polymorphically
    ~wxCondition();

    // NB: the associated mutex MUST be locked beforehand by the calling thread
    // 
    // it atomically releases the lock on the associated mutex
    // and starts waiting to be woken up by a Signal()/Broadcast()
    // once its signaled, then it will wait until it can reacquire
    // the lock on the associated mutex object, before returning.
    void Wait();

    // exactly as Wait() except that it may also return if the specified
    // timeout ellapses even if the condition hasn't been signalled: in this
    // case, the return value is FALSE, otherwise (i.e. in case of a normal
    // return) it is TRUE
    // 
    // the timeeout parameter specifies a interval that needs to be waited in
    // milliseconds
    bool Wait( unsigned long timeout_millis );

    // NB: the associated mutex may or may not be locked by the calling thread
    //
    // this method unblocks one thread if any are blocking on the condition.
    // if no thread is blocking in Wait(), then the signal is NOT remembered
    // The thread which was blocking on Wait(), will then reacquire the lock
    // on the associated mutex object before returning
    void Signal();

    // NB: the associated mutex may or may not be locked by the calling thread
    //
    // this method unblocks all threads if any are blocking on the condition.
    // if no thread is blocking in Wait(), then the signal is NOT remembered
    // The threads which were blocking on Wait(), will then reacquire the lock
    // on the associated mutex object before returning.
    void Broadcast();

private:
    wxConditionInternal *m_internal;
};

// ----------------------------------------------------------------------------
// wxSemaphore: a counter limiting the number of threads concurrently accessing
//              a shared resource
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxSemaphoreInternal;
class WXDLLEXPORT wxSemaphore
{
public:
    // specifying a maxcount of 0 actually makes wxSemaphore behave as if there
    // is no upper limit, if maxcount is 1 the semaphore behaves as a mutex
    wxSemaphore( int initialcount = 0, int maxcount = 0 );

    // dtor is not virtual, don't use this class polymorphically
    ~wxSemaphore();

    // wait indefinitely, until the semaphore count goes beyond 0
    // and then decrement it and return (this method might have been called
    // Acquire())
    void Wait();

    // same as Wait(), but does not block, returns TRUE if successful and
    // FALSE if the count is zero
    bool TryWait();

    // same as Wait(), but as a timeout limit, returns TRUE if the semaphore
    // was acquired and FALSE if the timeout has ellapsed
    bool Wait( unsigned long timeout_millis );

    // increments the semaphore count and signals one of the waiting threads
    void Post();

private:
    wxSemaphoreInternal *m_internal;
};

// ----------------------------------------------------------------------------
// wxThread: class encpasulating a thread of execution
// ----------------------------------------------------------------------------

// there are two different kinds of threads: joinable and detached (default)
// ones. Only joinable threads can return a return code and only detached
// threads auto-delete themselves - the user should delete the joinable
// threads manually.

// NB: in the function descriptions the words "this thread" mean the thread
//     created by the wxThread object while "main thread" is the thread created
//     during the process initialization (a.k.a. the GUI thread)

// On VMS thread pointers are 64 bits (also needed for other systems???
#ifdef __VMS
   typedef unsigned long long wxThreadIdType;
#else
   typedef unsigned long wxThreadIdType;
#endif

class wxThreadInternal;
class WXDLLEXPORT wxThread
{
public:
    // the return type for the thread function
    typedef void *ExitCode;

    // static functions
        // Returns the wxThread object for the calling thread. NULL is returned
        // if the caller is the main thread (but it's recommended to use
        // IsMain() and only call This() for threads other than the main one
        // because NULL is also returned on error). If the thread wasn't
        // created with wxThread class, the returned value is undefined.
    static wxThread *This();

        // Returns true if current thread is the main thread.
    static bool IsMain();

        // Release the rest of our time slice leting the other threads run
    static void Yield();

        // Sleep during the specified period of time in milliseconds
        //
        // NB: at least under MSW worker threads can not call ::wxSleep()!
    static void Sleep(unsigned long milliseconds);

        // get the number of system CPUs - useful with SetConcurrency()
        // (the "best" value for it is usually number of CPUs + 1)
        //
        // Returns -1 if unknown, number of CPUs otherwise
    static int GetCPUCount();

        // Get the platform specific thread ID and return as a long.  This
        // can be used to uniquely identify threads, even if they are not
        // wxThreads.  This is used by wxPython.
   static wxThreadIdType GetCurrentId();

        // sets the concurrency level: this is, roughly, the number of threads
        // the system tries to schedule to run in parallel. 0 means the
        // default value (usually acceptable, but may not yield the best
        // performance for this process)
        //
        // Returns TRUE on success, FALSE otherwise (if not implemented, for
        // example)
    static bool SetConcurrency(size_t level);

    // constructor only creates the C++ thread object and doesn't create (or
    // start) the real thread
    wxThread(wxThreadKind kind = wxTHREAD_DETACHED);

    // functions that change the thread state: all these can only be called
    // from _another_ thread (typically the thread that created this one, e.g.
    // the main thread), not from the thread itself

        // create a new thread and optionally set the stack size on
        // platforms that support that - call Run() to start it
        // (special cased for watcom which won't accept 0 default)

    wxThreadError Create(unsigned int stackSize = 0);

        // starts execution of the thread - from the moment Run() is called
        // the execution of wxThread::Entry() may start at any moment, caller
        // shouldn't suppose that it starts after (or before) Run() returns.
    wxThreadError Run();

        // stops the thread if it's running and deletes the wxThread object if
        // this is a detached thread freeing its memory - otherwise (for
        // joinable threads) you still need to delete wxThread object
        // yourself.
        //
        // this function only works if the thread calls TestDestroy()
        // periodically - the thread will only be deleted the next time it
        // does it!
        //
        // will fill the rc pointer with the thread exit code if it's !NULL
    wxThreadError Delete(ExitCode *rc = (ExitCode *)NULL);

        // waits for a joinable thread to finish and returns its exit code
        //
        // Returns (ExitCode)-1 on error (for example, if the thread is not
        // joinable)
    ExitCode Wait();

        // kills the thread without giving it any chance to clean up - should
        // not be used in normal circumstances, use Delete() instead. It is a
        // dangerous function that should only be used in the most extreme
        // cases!
        //
        // The wxThread object is deleted by Kill() if the thread is
        // detachable, but you still have to delete it manually for joinable
        // threads.
    wxThreadError Kill();

        // pause a running thread: as Delete(), this only works if the thread
        // calls TestDestroy() regularly
    wxThreadError Pause();

        // resume a paused thread
    wxThreadError Resume();

    // priority
        // Sets the priority to "prio": see WXTHREAD_XXX_PRIORITY constants
        //
        // NB: the priority can only be set before the thread is created
    void SetPriority(unsigned int prio);

        // Get the current priority.
    unsigned int GetPriority() const;

    // thread status inquiries
        // Returns true if the thread is alive: i.e. running or suspended
    bool IsAlive() const;
        // Returns true if the thread is running (not paused, not killed).
    bool IsRunning() const;
        // Returns true if the thread is suspended
    bool IsPaused() const;

        // is the thread of detached kind?
    bool IsDetached() const { return m_isDetached; }

    // Get the thread ID - a platform dependent number which uniquely
    // identifies a thread inside a process
    wxThreadIdType GetId() const;

    // called when the thread exits - in the context of this thread
    //
    // NB: this function will not be called if the thread is Kill()ed
    virtual void OnExit() { }

    // dtor is public, but the detached threads should never be deleted - use
    // Delete() instead (or leave the thread terminate by itself)
    virtual ~wxThread();

protected:
    // Returns TRUE if the thread was asked to terminate: this function should
    // be called by the thread from time to time, otherwise the main thread
    // will be left forever in Delete()!
    bool TestDestroy();

    // exits from the current thread - can be called only from this thread
    void Exit(ExitCode exitcode = 0);

    // entry point for the thread - called by Run() and executes in the context
    // of this thread.
    virtual void *Entry() = 0;

private:
    // no copy ctor/assignment operator
    wxThread(const wxThread&);
    wxThread& operator=(const wxThread&);

    friend class wxThreadInternal;

    // the (platform-dependent) thread class implementation
    wxThreadInternal *m_internal;

    // protects access to any methods of wxThreadInternal object
    wxCriticalSection m_critsect;

    // true if the thread is detached, false if it is joinable
    bool m_isDetached;
};

// ----------------------------------------------------------------------------
// Automatic initialization
// ----------------------------------------------------------------------------

// GUI mutex handling.
void WXDLLEXPORT wxMutexGuiEnter();
void WXDLLEXPORT wxMutexGuiLeave();

// macros for entering/leaving critical sections which may be used without
// having to take them inside "#if wxUSE_THREADS"
#define wxENTER_CRIT_SECT(cs)   (cs).Enter()
#define wxLEAVE_CRIT_SECT(cs)   (cs).Leave()
#define wxCRIT_SECT_DECLARE(cs) static wxCriticalSection cs
#define wxCRIT_SECT_LOCKER(name, cs)  wxCriticalSectionLocker name(cs)

#else // !wxUSE_THREADS

#include "wx/defs.h" // for WXDLLEXPORT

// no thread support
inline void WXDLLEXPORT wxMutexGuiEnter() { }
inline void WXDLLEXPORT wxMutexGuiLeave() { }

// macros for entering/leaving critical sections which may be used without
// having to take them inside "#if wxUSE_THREADS"
#define wxENTER_CRIT_SECT(cs)
#define wxLEAVE_CRIT_SECT(cs)
#define wxCRIT_SECT_DECLARE(cs)
#define wxCRIT_SECT_LOCKER(name, cs)

#endif // wxUSE_THREADS

// automatically unlock GUI mutex in dtor
class WXDLLEXPORT wxMutexGuiLocker
{
public:
    wxMutexGuiLocker() { wxMutexGuiEnter(); }
   ~wxMutexGuiLocker() { wxMutexGuiLeave(); }
};

// -----------------------------------------------------------------------------
// implementation only until the end of file
// -----------------------------------------------------------------------------

#if wxUSE_THREADS

#if defined(__WXMSW__)
    // unlock GUI if there are threads waiting for and lock it back when
    // there are no more of them - should be called periodically by the main
    // thread
    extern void WXDLLEXPORT wxMutexGuiLeaveOrEnter();

    // returns TRUE if the main thread has GUI lock
    extern bool WXDLLEXPORT wxGuiOwnedByMainThread();

    // wakes up the main thread if it's sleeping inside ::GetMessage()
    extern void WXDLLEXPORT wxWakeUpMainThread();

    // return TRUE if the main thread is waiting for some other to terminate:
    // wxApp then should block all "dangerous" messages
    extern bool WXDLLEXPORT wxIsWaitingForThread();
#elif defined(__WXMAC__)
   extern void WXDLLEXPORT wxMutexGuiLeaveOrEnter();

    // returns TRUE if the main thread has GUI lock
    extern bool WXDLLEXPORT wxGuiOwnedByMainThread();

    // wakes up the main thread if it's sleeping inside ::GetMessage()
    extern void WXDLLEXPORT wxWakeUpMainThread();

    // return TRUE if the main thread is waiting for some other to terminate:
    // wxApp then should block all "dangerous" messages
    extern bool WXDLLEXPORT wxIsWaitingForThread();

    // implement wxCriticalSection using mutexes
    inline wxCriticalSection::wxCriticalSection() { }
    inline wxCriticalSection::~wxCriticalSection() { }

    inline void wxCriticalSection::Enter() { (void)m_mutex.Lock(); }
    inline void wxCriticalSection::Leave() { (void)m_mutex.Unlock(); }
#elif defined(__WXPM__)
    // unlock GUI if there are threads waiting for and lock it back when
    // there are no more of them - should be called periodically by the main
    // thread
    extern void WXDLLEXPORT wxMutexGuiLeaveOrEnter();

    // returns TRUE if the main thread has GUI lock
    extern bool WXDLLEXPORT wxGuiOwnedByMainThread();

    // return TRUE if the main thread is waiting for some other to terminate:
    // wxApp then should block all "dangerous" messages
    extern bool WXDLLEXPORT wxIsWaitingForThread();

#else // !MSW && !PM
    // implement wxCriticalSection using mutexes
    inline wxCriticalSection::wxCriticalSection() { }
    inline wxCriticalSection::~wxCriticalSection() { }

    inline void wxCriticalSection::Enter() { (void)m_mutex.Lock(); }
    inline void wxCriticalSection::Leave() { (void)m_mutex.Unlock(); }
#endif // MSW/!MSW

    // we can define these inline functions now (they should be defined after
    // wxCriticalSection::Enter/Leave)
    inline
    wxCriticalSectionLocker:: wxCriticalSectionLocker(wxCriticalSection& cs)
        : m_critsect(cs) { m_critsect.Enter(); }
    inline
    wxCriticalSectionLocker::~wxCriticalSectionLocker() { m_critsect.Leave(); }
#endif // wxUSE_THREADS

#endif // __THREADH__

