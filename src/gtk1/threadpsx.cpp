/////////////////////////////////////////////////////////////////////////////
// Name:        threadpsx.cpp
// Purpose:     wxThread (Posix) Implementation
// Author:      Original from Wolfram Gloger/Guilhem Lavaux
// Modified by:
// Created:     04/22/98
// RCS-ID:      $Id$
// Copyright:   (c) Wolfram Gloger (1996, 1997); Guilhem Lavaux (1998)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
#pragma implementation "thread.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "wx/thread.h"
#include "wx/module.h"
#include "wx/utils.h"

enum thread_state {
  STATE_IDLE = 0,
  STATE_RUNNING,
  STATE_PAUSING,
  STATE_PAUSED,
  STATE_CANCELED,
  STATE_EXITED
};

/////////////////////////////////////////////////////////////////////////////
// Static variables
/////////////////////////////////////////////////////////////////////////////

static pthread_t p_mainid;
static wxMutex p_list_mutex;
static wxList p_threads_list;

wxMutex *wxMainMutex; // controls access to all GUI functions

/////////////////////////////////////////////////////////////////////////////
// GUI thread manager
/////////////////////////////////////////////////////////////////////////////
#include "threadgui.inc"

/////////////////////////////////////////////////////////////////////////////
// wxThread: Posix Thread implementation (Mutex)
/////////////////////////////////////////////////////////////////////////////

class wxMutexInternal {
public:
  pthread_mutex_t p_mutex;
};

wxMutex::wxMutex()
{
  p_internal = new wxMutexInternal;
  pthread_mutex_init(&(p_internal->p_mutex), NULL);
  m_locked = 0;
}

wxMutex::~wxMutex()
{
  if (m_locked > 0)
    wxDebugMsg("wxMutex warning: freeing a locked mutex (%d locks)\n",
               m_locked);

  pthread_mutex_destroy(&(p_internal->p_mutex));
  delete p_internal;
}

wxMutexError wxMutex::Lock()
{
  int err;

  err = pthread_mutex_lock(&(p_internal->p_mutex));
  if (err == EDEADLK)
    return MUTEX_DEAD_LOCK;
  m_locked++;
  return MUTEX_NO_ERROR;
}

wxMutexError wxMutex::TryLock()
{
  int err;

  if (m_locked)
    return MUTEX_BUSY;
  err = pthread_mutex_trylock(&(p_internal->p_mutex));
  switch (err) {
  case EBUSY: return MUTEX_BUSY;
  }
  m_locked++;
  return MUTEX_NO_ERROR;
}

wxMutexError wxMutex::Unlock()
{
  if (m_locked > 0)
    m_locked--;
  else
    return MUTEX_UNLOCKED;
  pthread_mutex_unlock(&(p_internal->p_mutex));
  return MUTEX_NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////
// wxThread: Posix Thread implementation (Condition)
/////////////////////////////////////////////////////////////////////////////

class wxConditionInternal {
public:
  pthread_cond_t p_condition;
};

wxCondition::wxCondition()
{
  p_internal = new wxConditionInternal;
  pthread_cond_init(&(p_internal->p_condition), NULL);
}

wxCondition::~wxCondition()
{
  pthread_cond_destroy(&(p_internal->p_condition));
  delete p_internal;
}

void wxCondition::Wait(wxMutex& mutex)
{
  pthread_cond_wait(&(p_internal->p_condition), &(mutex.p_internal->p_mutex));
}

bool wxCondition::Wait(wxMutex& mutex, unsigned long sec, unsigned long nsec)
{
  struct timespec tspec;

  tspec.tv_sec = time(NULL)+sec;
  tspec.tv_nsec = nsec;
  return (pthread_cond_timedwait(&(p_internal->p_condition), &(mutex.p_internal->p_mutex), &tspec) != ETIMEDOUT);
}

void wxCondition::Signal()
{
  pthread_cond_signal(&(p_internal->p_condition));
}

void wxCondition::Broadcast()
{
  pthread_cond_broadcast(&(p_internal->p_condition));
}

/////////////////////////////////////////////////////////////////////////////
// wxThread: Posix Thread implementation (Thread)
/////////////////////////////////////////////////////////////////////////////

class wxThreadInternal {
public:
  wxThreadInternal() { state = STATE_IDLE; }
  ~wxThreadInternal() {}
  static void *PthreadStart(void *ptr);
  pthread_t thread_id;
  int state;
  int prio;
  int defer_destroy;
  int id;
};

void *wxThreadInternal::PthreadStart(void *ptr)
{
  wxThread *thread = (wxThread *)ptr;

  // Add the current thread to the list
  p_list_mutex.Lock();
  thread->p_internal->id = p_threads_list.Number();
  p_threads_list.Append((wxObject *)thread);
  p_list_mutex.Unlock();

  // Call the main entry
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  void* status = thread->Entry();

  thread->Exit(status);

  return NULL;
}

wxThreadError wxThread::Create()
{
  pthread_attr_t a;
  int min_prio, max_prio, p;
  struct sched_param sp;

  if (p_internal->state != STATE_IDLE)
    return THREAD_RUNNING;

  // Change thread priority
  pthread_attr_init(&a);
  pthread_attr_getschedpolicy(&a, &p);

  min_prio = sched_get_priority_min(p);
  max_prio = sched_get_priority_max(p);

  pthread_attr_getschedparam(&a, &sp);
  sp.sched_priority = min_prio +
               (p_internal->prio*(max_prio-min_prio))/100;
  pthread_attr_setschedparam(&a, &sp);

  // this is the point of no return
  p_internal->state = STATE_RUNNING;
  if (pthread_create(&p_internal->thread_id, &a,
                     wxThreadInternal::PthreadStart, (void *)this) != 0) {
    p_internal->state = STATE_IDLE;
    pthread_attr_destroy(&a);
    return THREAD_NO_RESOURCE;
  }
  pthread_attr_destroy(&a);

  return THREAD_NO_ERROR;
}

void wxThread::SetPriority(int prio)
{
  if (p_internal->state == STATE_RUNNING)
    return;

  if (prio > 100)
    prio = 100;
  if (prio < 0)
    prio = 0;
  p_internal->prio = prio;
}

int wxThread::GetPriority() const
{
  return p_internal->prio;
}

void wxThread::DeferDestroy(bool on)
{
  if (on)
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  else
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
}

wxThreadError wxThread::Destroy()
{
  int res = 0;

  if (p_internal->state == STATE_RUNNING) {
    res = pthread_cancel(p_internal->thread_id);
    if (res == 0)
      p_internal->state = STATE_CANCELED;
  }

  return THREAD_NO_ERROR;
}

wxThreadError wxThread::Pause()
{
  if (p_internal->state != STATE_RUNNING)
    return THREAD_NOT_RUNNING;

  if (!p_internal->defer_destroy)
    return THREAD_MISC_ERROR;

  p_internal->state = STATE_PAUSING;
  return THREAD_NO_ERROR;
}

wxThreadError wxThread::Resume()
{
  if (p_internal->state == STATE_PAUSING || p_internal->state == STATE_PAUSED)
    p_internal->state = STATE_RUNNING;

  return THREAD_NO_ERROR;
}

void *wxThread::Join()
{
  void* status = 0;

  if (p_internal->state != STATE_IDLE) {
    bool do_unlock = wxThread::IsMain();

    while (p_internal->state == STATE_RUNNING)
      wxYield();

    if (do_unlock)
      wxMainMutex->Unlock();
    pthread_join(p_internal->thread_id, &status);
    if (do_unlock)
      wxMainMutex->Lock();

    p_list_mutex.Lock();
    delete p_threads_list.Nth(p_internal->id);
    p_list_mutex.Unlock();

    p_internal->state = STATE_IDLE;
  }
  return status;
}

unsigned long wxThread::GetID() const
{
  return p_internal->id;
}

wxThread *wxThread::GetThreadFromID(unsigned long id)
{
  wxNode *node = p_threads_list.Nth(id);

  if (!node)
    return NULL;
  return (wxThread *)node->Data();
}

void wxThread::Exit(void *status)
{
  wxThread* ptr = this;

  THREAD_SEND_EXIT_MSG(ptr);
  p_internal->state = STATE_EXITED;
  pthread_exit(status);
}

void wxThread::TestDestroy()
{
  if (p_internal->state == STATE_PAUSING) {
    p_internal->state = STATE_PAUSED;
    while (p_internal->state == STATE_PAUSED) {
      pthread_testcancel();
      usleep(1);
    }
  }
  pthread_testcancel();
}

bool wxThread::IsMain()
{
  return (bool)pthread_equal(pthread_self(), p_mainid);
}

bool wxThread::IsRunning() const
{
  return (p_internal->state == STATE_RUNNING);
}

bool wxThread::IsAlive() const
{
  return (p_internal->state == STATE_RUNNING) ||
         (p_internal->state == STATE_PAUSING) ||
         (p_internal->state == STATE_PAUSED);
}

wxThread::wxThread()
{
  p_internal = new wxThreadInternal();
}

wxThread::~wxThread()
{
  Destroy();
  Join();
  delete p_internal;
}

// The default callback just joins the thread and throws away the result.
void wxThread::OnExit()
{
  Join();
}

// Automatic initialization
class wxThreadModule : public wxModule {
  DECLARE_DYNAMIC_CLASS(wxThreadModule)
public:
  virtual bool OnInit() {
    wxMainMutex = new wxMutex();
    wxThreadGuiInit();
    p_mainid = pthread_self();
//    p_threads_list = wxList(wxKEY_INTEGER);
    wxMainMutex->Lock();

    return TRUE;
  }

  virtual void OnExit() {
    wxMainMutex->Unlock();
    wxThreadGuiExit();
    delete wxMainMutex;
  }
};

IMPLEMENT_DYNAMIC_CLASS(wxThreadModule, wxModule)
