/////////////////////////////////////////////////////////////////////////////
// Name:        thread.cpp
// Purpose:     wxWindows thread sample
// Author:      Julian Smart(minimal)/Guilhem Lavaux(thread test)
// Modified by:
// Created:     06/16/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart, Markus Holzem, Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/*
    TODO: use worker threads to update progress controls instead of writing
          messages - it will be more visual
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !wxUSE_THREADS
    #error "This sample requires thread support!"
#endif // wxUSE_THREADS

#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/time.h"

#include "wx/progdlg.h"

#define USE_EXECUTE
#ifdef USE_EXECUTE
    #define EXEC(cmd) wxExecute((cmd), wxEXEC_SYNC)
#else
    #define EXEC(cmd) system(cmd)
#endif

class MyThread;
WX_DEFINE_ARRAY(wxThread *, wxArrayThread);

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit();

public:
    // all the threads currently alive - as soon as the thread terminates, it's
    // removed from the array
    wxArrayThread m_threads;

    // crit section protects access to all of the arrays below
    wxCriticalSection m_critsect;
};

// Create a new application object
IMPLEMENT_APP(MyApp)

// Define a new frame type
class MyFrame: public wxFrame
{
public:
    // ctor
    MyFrame(wxFrame *frame, const wxString& title, int x, int y, int w, int h);

    // operations
    void WriteText(const wxString& text) { m_txtctrl->WriteText(text); }

    // accessors for MyWorkerThread (called in its context!)
    bool Cancelled();

    // callbacks
    void OnQuit(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);

    void OnStartThread(wxCommandEvent& event);
    void OnStartThreads(wxCommandEvent& event);
    void OnStopThread(wxCommandEvent& event);
    void OnPauseThread(wxCommandEvent& event);
    void OnResumeThread(wxCommandEvent& event);

    void OnStartWorker(wxCommandEvent& event);
    void OnWorkerEvent(wxCommandEvent& event);
    void OnUpdateWorker(wxUpdateUIEvent& event);

    void OnExecMain(wxCommandEvent& event);
    void OnExecThread(wxCommandEvent& event);

    void OnShowCPUs(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnIdle(wxIdleEvent &event);

private:
    // helper function - creates a new thread (but doesn't run it)
    MyThread *CreateThread();

    // just some place to put our messages in
    wxTextCtrl *m_txtctrl;

    // remember the number of running threads and total number of threads
    size_t m_nRunning, m_nCount;

    // the progress dialog which we show while worker thread is running
    wxProgressDialog *m_dlgProgress;

    // was the worker thread cancelled by user?
    bool m_cancelled;

    // protects m_cancelled
    wxCriticalSection m_critsectWork;

    DECLARE_EVENT_TABLE()
};

// ID for the menu commands
enum
{
    THREAD_QUIT          = 1,
    THREAD_TEXT          = 101,
    THREAD_CLEAR,
    THREAD_START_THREAD  = 201,
    THREAD_START_THREADS,
    THREAD_STOP_THREAD,
    THREAD_PAUSE_THREAD,
    THREAD_RESUME_THREAD,
    THREAD_START_WORKER,

    THREAD_EXEC_MAIN,
    THREAD_EXEC_THREAD,

    THREAD_SHOWCPUS,
    THREAD_ABOUT,

    WORKER_EVENT    // this one gets sent from the worker thread
};

// ----------------------------------------------------------------------------
// GUI thread
// ----------------------------------------------------------------------------

class MyThread : public wxThread
{
public:
    MyThread(MyFrame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

    // write something to the text control
    void WriteText(const wxString& text);

public:
    size_t   m_count;
    MyFrame *m_frame;
};

MyThread::MyThread(MyFrame *frame)
        : wxThread()
{
    m_count = 0;
    m_frame = frame;
}

void MyThread::WriteText(const wxString& text)
{
    wxString msg;

    // before doing any GUI calls we must ensure that this thread is the only
    // one doing it!

    wxMutexGuiEnter();

    msg << text;
    m_frame->WriteText(msg);

    wxMutexGuiLeave();
}

void MyThread::OnExit()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);

    wxGetApp().m_threads.Remove(this);
}

void *MyThread::Entry()
{
    wxString text;

    text.Printf(wxT("Thread 0x%x started (priority = %d).\n"),
                GetId(), GetPriority());
    WriteText(text);
    // wxLogMessage(text); -- test wxLog thread safeness

    for ( m_count = 0; m_count < 10; m_count++ )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        text.Printf(wxT("[%u] Thread 0x%x here.\n"), m_count, GetId());
        WriteText(text);

        // wxSleep() can't be called from non-GUI thread!
        wxThread::Sleep(1000);
    }

    text.Printf(wxT("Thread 0x%x finished.\n"), GetId());
    WriteText(text);
    // wxLogMessage(text); -- test wxLog thread safeness

    return NULL;
}

// ----------------------------------------------------------------------------
// worker thread
// ----------------------------------------------------------------------------

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread(MyFrame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    MyFrame *m_frame;
    size_t   m_count;
};

MyWorkerThread::MyWorkerThread(MyFrame *frame)
        : wxThread()
{
    m_frame = frame;
    m_count = 0;
}

void MyWorkerThread::OnExit()
{
}

void *MyWorkerThread::Entry()
{
    for ( m_count = 0; !m_frame->Cancelled() && (m_count < 100); m_count++ )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        // create any type of command event here
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, WORKER_EVENT );
        event.SetInt( m_count );

        // send in a thread-safe way
        wxPostEvent( m_frame, event );

        // wxSleep() can't be called from non-main thread!
        wxThread::Sleep(200);
    }

    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, WORKER_EVENT );
    event.SetInt(-1); // that's all
    wxPostEvent( m_frame, event );

    return NULL;
}

// ----------------------------------------------------------------------------
// a thread which simply calls wxExecute
// ----------------------------------------------------------------------------

class MyExecThread : public wxThread
{
public:
    MyExecThread(const wxChar *command) : wxThread(wxTHREAD_JOINABLE),
                                          m_command(command)
    {
        Create();
    }

    virtual ExitCode Entry()
    {
        return (ExitCode)EXEC(m_command);
    }

private:
    wxString m_command;
};

// ----------------------------------------------------------------------------
// implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(THREAD_QUIT, MyFrame::OnQuit)
    EVT_MENU(THREAD_CLEAR, MyFrame::OnClear)
    EVT_MENU(THREAD_START_THREAD, MyFrame::OnStartThread)
    EVT_MENU(THREAD_START_THREADS, MyFrame::OnStartThreads)
    EVT_MENU(THREAD_STOP_THREAD, MyFrame::OnStopThread)
    EVT_MENU(THREAD_PAUSE_THREAD, MyFrame::OnPauseThread)
    EVT_MENU(THREAD_RESUME_THREAD, MyFrame::OnResumeThread)

    EVT_MENU(THREAD_EXEC_MAIN, MyFrame::OnExecMain)
    EVT_MENU(THREAD_EXEC_THREAD, MyFrame::OnExecThread)

    EVT_MENU(THREAD_SHOWCPUS, MyFrame::OnShowCPUs)
    EVT_MENU(THREAD_ABOUT, MyFrame::OnAbout)

    EVT_UPDATE_UI(THREAD_START_WORKER, MyFrame::OnUpdateWorker)
    EVT_MENU(THREAD_START_WORKER, MyFrame::OnStartWorker)
    EVT_MENU(WORKER_EVENT, MyFrame::OnWorkerEvent)

    EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
    // uncomment this to get some debugging messages from the trace code
    // on the console (or just set WXTRACE env variable to include "thread")
    //wxLog::AddTraceMask("thread");

    // Create the main frame window
    MyFrame *frame = new MyFrame((wxFrame *)NULL, "wxWindows threads sample",
                                 50, 50, 450, 340);

    // Make a menubar
    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(THREAD_CLEAR, "&Clear log\tCtrl-L");
    menuFile->AppendSeparator();
    menuFile->Append(THREAD_QUIT, "E&xit\tAlt-X");
    menuBar->Append(menuFile, "&File");

    wxMenu *menuThread = new wxMenu;
    menuThread->Append(THREAD_START_THREAD, "&Start a new thread\tCtrl-N");
    menuThread->Append(THREAD_START_THREADS, "Start &many threads at once");
    menuThread->Append(THREAD_STOP_THREAD, "S&top a running thread\tCtrl-S");
    menuThread->AppendSeparator();
    menuThread->Append(THREAD_PAUSE_THREAD, "&Pause a running thread\tCtrl-P");
    menuThread->Append(THREAD_RESUME_THREAD, "&Resume suspended thread\tCtrl-R");
    menuThread->AppendSeparator();
    menuThread->Append(THREAD_START_WORKER, "Start &worker thread\tCtrl-W");
    menuBar->Append(menuThread, "&Thread");

    wxMenu *menuExec = new wxMenu;
    menuExec->Append(THREAD_EXEC_MAIN, "&Launch a program from main thread\tF5");
    menuExec->Append(THREAD_EXEC_THREAD, "L&aunch a program from a thread\tCtrl-F5");
    menuBar->Append(menuExec, "&Execute");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(THREAD_SHOWCPUS, "&Show CPU count");
    menuHelp->AppendSeparator();
    menuHelp->Append(THREAD_ABOUT, "&About...");
    menuBar->Append(menuHelp, "&Help");

    frame->SetMenuBar(menuBar);

    // Show the frame
    frame->Show(TRUE);

    SetTopWindow(frame);

    return TRUE;
}

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title,
                 int x, int y, int w, int h)
       : wxFrame(frame, -1, title, wxPoint(x, y), wxSize(w, h))
{
    m_nRunning = m_nCount = 0;

    m_dlgProgress = (wxProgressDialog *)NULL;

    CreateStatusBar(2);

    m_txtctrl = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(0, 0),
                               wxTE_MULTILINE | wxTE_READONLY);

}

MyThread *MyFrame::CreateThread()
{
    MyThread *thread = new MyThread(this);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }

    wxCriticalSectionLocker enter(wxGetApp().m_critsect);
    wxGetApp().m_threads.Add(thread);

    return thread;
}

void MyFrame::OnStartThreads(wxCommandEvent& WXUNUSED(event) )
{
    static long s_num = 10;

    s_num = wxGetNumberFromUser("How many threads to start: ", "",
                                "wxThread sample", s_num, 1, 10000, this);
    if ( s_num == -1 )
    {
        s_num = 10;

        return;
    }

    size_t count = (size_t)s_num, n;

    wxArrayThread threads;

    // first create them all...
    for ( n = 0; n < count; n++ )
    {
        wxThread *thr = CreateThread();

        // we want to show the effect of SetPriority(): the first thread will
        // have the lowest priority, the second - the highest, all the rest
        // the normal one
        if ( n == 0 )
            thr->SetPriority(WXTHREAD_MIN_PRIORITY);
        else if ( n == 1 )
            thr->SetPriority(WXTHREAD_MAX_PRIORITY);
        else
            thr->SetPriority(WXTHREAD_DEFAULT_PRIORITY);

        threads.Add(thr);
    }

    wxString msg;
    msg.Printf(wxT("%d new threads created."), count);
    SetStatusText(msg, 1);

    // ...and then start them
    for ( n = 0; n < count; n++ )
    {
        threads[n]->Run();
    }
}

void MyFrame::OnStartThread(wxCommandEvent& WXUNUSED(event) )
{
    MyThread *thread = CreateThread();

    if ( thread->Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't start thread!"));
    }

    SetStatusText("New thread started.", 1);
}

void MyFrame::OnStopThread(wxCommandEvent& WXUNUSED(event) )
{
    wxGetApp().m_critsect.Enter();

    // stop the last thread
    if ( wxGetApp().m_threads.IsEmpty() )
    {
        wxLogError(wxT("No thread to stop!"));

        wxGetApp().m_critsect.Leave();
    }
    else
    {
        wxThread *thread = wxGetApp().m_threads.Last();

        // it's important to leave critical section before calling Delete()
        // because delete will (implicitly) call OnExit() which also tries
        // to enter the same crit section - would dead lock.
        wxGetApp().m_critsect.Leave();

        thread->Delete();

        SetStatusText("Thread stopped.", 1);
    }
}

void MyFrame::OnResumeThread(wxCommandEvent& WXUNUSED(event) )
{
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);

    // resume first suspended thread
    size_t n = 0, count = wxGetApp().m_threads.Count();
    while ( n < count && !wxGetApp().m_threads[n]->IsPaused() )
        n++;

    if ( n == count )
    {
        wxLogError(wxT("No thread to resume!"));
    }
    else
    {
        wxGetApp().m_threads[n]->Resume();

        SetStatusText("Thread resumed.", 1);
    }
}

void MyFrame::OnPauseThread(wxCommandEvent& WXUNUSED(event) )
{
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);

    // pause last running thread
    int n = wxGetApp().m_threads.Count() - 1;
    while ( n >= 0 && !wxGetApp().m_threads[n]->IsRunning() )
        n--;

    if ( n < 0 )
    {
        wxLogError(wxT("No thread to pause!"));
    }
    else
    {
        wxGetApp().m_threads[n]->Pause();

        SetStatusText("Thread paused.", 1);
    }
}

// set the frame title indicating the current number of threads
void MyFrame::OnIdle(wxIdleEvent &event)
{
    // update the counts of running/total threads
    size_t nRunning = 0,
           nCount = wxGetApp().m_threads.Count();
    for ( size_t n = 0; n < nCount; n++ )
    {
        if ( wxGetApp().m_threads[n]->IsRunning() )
            nRunning++;
    }

    if ( nCount != m_nCount || nRunning != m_nRunning )
    {
        m_nRunning = nRunning;
        m_nCount = nCount;

        wxLogStatus(this, wxT("%u threads total, %u running."), nCount, nRunning);
    }
    //else: avoid flicker - don't print anything
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event) )
{
    const wxArrayThread& threads = wxGetApp().m_threads;
    while ( !threads.IsEmpty() )
    {
        threads[0]->Delete();
    }

    Close(TRUE);
}

void MyFrame::OnExecMain(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage(wxT("The exit code from the main program is %ld"),
                 EXEC("/bin/echo \"main program\""));
}

void MyFrame::OnExecThread(wxCommandEvent& WXUNUSED(event))
{
    MyExecThread thread(wxT("/bin/echo \"child thread\""));
    thread.Run();

    wxLogMessage(wxT("The exit code from a child thread is %ld"),
                 (long)thread.Wait());
}

void MyFrame::OnShowCPUs(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;

    int nCPUs = wxThread::GetCPUCount();
    switch ( nCPUs )
    {
        case -1:
            msg = "Unknown number of CPUs";
            break;

        case 0:
            msg = "WARNING: you're running without any CPUs!";
            break;

        case 1:
            msg = "This system only has one CPU.";
            break;

        default:
            msg.Printf(wxT("This system has %d CPUs"), nCPUs);
    }
            
    wxLogMessage(msg);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    wxMessageDialog dialog(this, "wxWindows multithreaded application sample\n"
                                 "(c) 1998 Julian Smart, Guilhem Lavaux\n"
                                 "(c) 1999 Vadim Zeitlin\n"
                                 "(c) 2000 Robert Roebling",
                           "About wxThread sample",
                           wxOK | wxICON_INFORMATION);

    dialog.ShowModal();
}

void MyFrame::OnClear(wxCommandEvent& WXUNUSED(event))
{
    m_txtctrl->Clear();
}

void MyFrame::OnUpdateWorker(wxUpdateUIEvent& event)
{
    event.Enable( m_dlgProgress == NULL );
}

void MyFrame::OnStartWorker(wxCommandEvent& WXUNUSED(event))
{
    MyWorkerThread *thread = new MyWorkerThread(this);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }

    m_dlgProgress = new wxProgressDialog
                        (
                         "Progress dialog",
                         "Wait until the thread terminates or press [Cancel]",
                         100,
                         this,
                         wxPD_CAN_ABORT |
                         wxPD_APP_MODAL |
                         wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME
                        );

    // thread is not running yet, no need for crit sect
    m_cancelled = FALSE;

    thread->Run();
}

void MyFrame::OnWorkerEvent(wxCommandEvent& event)
{
#if 0
    WriteText( "Got message from worker thread: " );
    WriteText( event.GetString() );
    WriteText( "\n" );
#else
    int n = event.GetInt();
    if ( n == -1 )
    {
        m_dlgProgress->Destroy();
        m_dlgProgress = (wxProgressDialog *)NULL;

        // the dialog is aborted because the event came from another thread, so
        // we may need to wake up the main event loop for the dialog to be
        // really closed
        wxWakeUpIdle();
    }
    else
    {
        if ( !m_dlgProgress->Update(n) )
        {
            wxCriticalSectionLocker lock(m_critsectWork);

            m_cancelled = TRUE;
        }
    }
#endif
}

bool MyFrame::Cancelled()
{
    wxCriticalSectionLocker lock(m_critsectWork);

    return m_cancelled;
}
