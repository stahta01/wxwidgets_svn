/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Author:      Vaclav Slavik
//              based on GTK and MSW implementations
// Id:          $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "app.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/settings.h"
    #include "wx/module.h"
    #include "wx/evtloop.h"
    #include "wx/frame.h"
    #include "wx/dialog.h"
    #include "wx/intl.h"
#endif

#include "wx/app.h"
#include "wx/mgl/private.h"

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------

wxApp *wxTheApp = NULL;
wxAppInitializerFunction wxAppBase::m_appInitFn = (wxAppInitializerFunction) NULL;

static wxEventLoop *gs_mainEventLoop = NULL;


//-----------------------------------------------------------------------------
// wxExit
//-----------------------------------------------------------------------------

void wxExit()
{
    MGL_exit();
    exit(0);
}

//-----------------------------------------------------------------------------
// wxYield
//-----------------------------------------------------------------------------

static bool gs_inYield = FALSE;

bool wxYield()
{
#if wxUSE_THREADS
    if ( !wxThread::IsMain() )
    {
        // can't process events from other threads, MGL is thread-unsafe
        return TRUE;
    }
#endif // wxUSE_THREADS

    gs_inYield = TRUE;

    wxLog::Suspend();

    while (gs_mainEventLoop->Pending())
        gs_mainEventLoop->Dispatch();
        
    /* it's necessary to call ProcessIdle() to update the frames sizes which
       might have been changed (it also will update other things set from
       OnUpdateUI() which is a nice (and desired) side effect) */
    while (wxTheApp->ProcessIdle()) { }

    wxLog::Resume();

    gs_inYield = FALSE;

    return TRUE;
}

bool wxYieldIfNeeded()
{
    if (gs_inYield)
        return FALSE;

    return wxYield();
}


//-----------------------------------------------------------------------------
// wxWakeUpIdle
//-----------------------------------------------------------------------------

void wxWakeUpIdle()
{
#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiEnter();
#endif

    while (wxTheApp->ProcessIdle()) {} 

#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiLeave();
#endif
}

//-----------------------------------------------------------------------------
// wxApp
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp,wxEvtHandler)

BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
END_EVENT_TABLE()


wxApp::wxApp()
{
}

wxApp::~wxApp()
{
}

bool wxApp::OnInitGui()
{
    if ( MGL_init(".", NULL) == 0 )
        return FALSE;
    if ( !wxCreateMGL_WM() )
        return FALSE;

    // This has to be done *after* wxCreateMGL_WM() because it initializes 
    // wxUniv's themes
    if ( !wxAppBase::OnInitGui() )
        return FALSE;

    return TRUE;
}

bool wxApp::ProcessIdle()
{
    wxIdleEvent event;
    event.SetEventObject(this);
    ProcessEvent(event);

    return event.MoreRequested();
}

void wxApp::OnIdle(wxIdleEvent &event)
{
    static bool s_inOnIdle = FALSE;

    /* Avoid recursion (via ProcessEvent default case) */
    if (s_inOnIdle)
        return;

    s_inOnIdle = TRUE;

    /* Resend in the main thread events which have been prepared in other
       threads */
    ProcessPendingEvents();

    // 'Garbage' collection of windows deleted with Close().
    DeletePendingObjects();

    // Send OnIdle events to all windows
    if ( SendIdleEvents() )
        event.RequestMore(TRUE);

    s_inOnIdle = FALSE;
}

bool wxApp::SendIdleEvents()
{
    bool needMore = FALSE;

    wxWindowList::Node* node = wxTopLevelWindows.GetFirst();
    while (node)
    {
        wxWindow* win = node->GetData();
        if ( SendIdleEvents(win) )
            needMore = TRUE;
        node = node->GetNext();
    }

    return needMore;
}

bool wxApp::SendIdleEvents(wxWindow* win)
{
    bool needMore = FALSE;

    wxIdleEvent event;
    event.SetEventObject(win);

    win->GetEventHandler()->ProcessEvent(event);

#if 0 // FIXME_MGL - what the hell it is?
    win->OnInternalIdle();

    if ( event.MoreRequested() )
        needMore = TRUE;
#endif

    wxNode* node = win->GetChildren().First();
    while (node)
    {
        wxWindow* win = (wxWindow*) node->Data();
        if ( SendIdleEvents(win) )
            needMore = TRUE;

        node = node->Next();
    }
    return needMore;
}

int wxApp::MainLoop()
{
    gs_mainEventLoop = new wxEventLoop;
    return gs_mainEventLoop->Run();
    delete gs_mainEventLoop;
    gs_mainEventLoop = NULL;
}

void wxApp::ExitMainLoop()
{
    gs_mainEventLoop->Exit(0);
}

bool wxApp::Initialized()
{
    return (GetTopWindow() != NULL);
}

bool wxApp::Pending()
{
    return gs_mainEventLoop->Pending();
}

void wxApp::Dispatch()
{
    gs_mainEventLoop->Dispatch();
}

void wxApp::DeletePendingObjects()
{
    wxNode *node = wxPendingDelete.First();
    while (node)
    {
        wxObject *obj = (wxObject *)node->Data();

        delete obj;

        if ( wxPendingDelete.Find(obj) )
            delete node;

        node = wxPendingDelete.First();
    }
}

bool wxApp::Initialize()
{
    wxBuffer = new wxChar[BUFSIZ + 512];

    wxClassInfo::InitializeClasses();

    wxSystemSettings::Init();
    
#if wxUSE_INTL
    wxFont::SetDefaultEncoding(wxLocale::GetSystemEncoding());
#endif

    // GL: I'm annoyed ... I don't know where to put this and I don't want to
    // create a module for that as it's part of the core.
#if wxUSE_THREADS
    wxPendingEvents = new wxList;
    wxPendingEventsLocker = new wxCriticalSection;
#endif

    wxTheColourDatabase = new wxColourDatabase(wxKEY_STRING);
    wxTheColourDatabase->Initialize();

    wxInitializeStockLists();
    wxInitializeStockObjects();

#if wxUSE_WX_RESOURCES
    wxInitializeResourceSystem();
#endif

    wxModule::RegisterModules();
    if (!wxModule::InitializeModules()) return FALSE;

    return TRUE;
}

#include "info.xpm"
#include "error.xpm"
#include "question.xpm"
#include "warning.xpm"

wxIcon wxApp::GetStdIcon(int which) const
{
    switch(which)
    {
        case wxICON_INFORMATION:
            return wxIcon(info_xpm);
        case wxICON_QUESTION:
            return wxIcon(question_xpm);
        case wxICON_EXCLAMATION:
            return wxIcon(warning_xpm);
        default:
            wxFAIL_MSG(wxT("requested non existent standard icon"));
            // still fall through
        case wxICON_HAND:
            return wxIcon(error_xpm);
    }
}

void wxApp::CleanUp()
{
#if wxUSE_LOG
    // flush the logged messages if any
    wxLog *log = wxLog::GetActiveTarget();
    if (log != NULL && log->HasPendingMessages())
        log->Flush();

    // continuing to use user defined log target is unsafe from now on because
    // some resources may be already unavailable, so replace it by something
    // more safe
    wxLog *oldlog = wxLog::SetActiveTarget(new wxLogStderr);
    if ( oldlog )
        delete oldlog;
#endif // wxUSE_LOG

    wxModule::CleanUpModules();

#if wxUSE_WX_RESOURCES
    wxCleanUpResourceSystem();
#endif

    if (wxTheColourDatabase)
        delete wxTheColourDatabase;

    wxTheColourDatabase = (wxColourDatabase*) NULL;

    wxDeleteStockObjects();

    wxDeleteStockLists();

    delete wxTheApp;
    wxTheApp = (wxApp*) NULL;

    // GL: I'm annoyed ... I don't know where to put this and I don't want to
    // create a module for that as it's part of the core.
#if wxUSE_THREADS
    delete wxPendingEvents;
    delete wxPendingEventsLocker;
#endif

    wxSystemSettings::Done();

    delete[] wxBuffer;

    wxClassInfo::CleanUpClasses();

    // check for memory leaks
#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
    if (wxDebugContext::CountObjectsLeft(TRUE) > 0)
    {
        wxLogDebug(wxT("There were memory leaks.\n"));
        wxDebugContext::Dump();
        wxDebugContext::PrintStatistics();
    }
#endif // Debug

#if wxUSE_LOG
    // do this as the very last thing because everything else can log messages
    wxLog::DontCreateOnDemand();

    wxLog *oldLog = wxLog::SetActiveTarget( (wxLog*) NULL );
    if (oldLog)
        delete oldLog;
#endif // wxUSE_LOG

    wxDestroyMGL_WM();
    MGL_exit();
}


int wxEntryStart(int argc, char *argv[])
{
    return wxApp::Initialize() ? 0 : -1;
}


int wxEntryInitGui()
{
    return wxTheApp->OnInitGui() ? 0 : -1;
}


void wxEntryCleanup()
{
    wxApp::CleanUp();
}



int wxEntry(int argc, char *argv[])
{
#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
    // This seems to be necessary since there are 'rogue'
    // objects present at this point (perhaps global objects?)
    // Setting a checkpoint will ignore them as far as the
    // memory checking facility is concerned.
    // Of course you may argue that memory allocated in globals should be
    // checked, but this is a reasonable compromise.
    wxDebugContext::SetCheckpoint();
#endif
    int err = wxEntryStart(argc, argv);
    if ( err )
        return err;

    if ( !wxTheApp )
    {
        wxCHECK_MSG( wxApp::GetInitializerFunction(), -1,
                     wxT("wxWindows error: No initializer - use IMPLEMENT_APP macro.\n") );

        wxAppInitializerFunction app_ini = wxApp::GetInitializerFunction();

        wxObject *test_app = app_ini();

        wxTheApp = (wxApp*) test_app;
    }

    wxCHECK_MSG( wxTheApp, -1, wxT("wxWindows error: no application object") );

    wxTheApp->argc = argc;
#if wxUSE_UNICODE
    wxTheApp->argv = new wxChar*[argc+1];
    int mb_argc = 0;
    while (mb_argc < argc)
    {
        wxTheApp->argv[mb_argc] = wxStrdup(wxConvLibc.cMB2WX(argv[mb_argc]));
        mb_argc++;
    }
    wxTheApp->argv[mb_argc] = (wxChar *)NULL;
#else
    wxTheApp->argv = argv;
#endif

    wxString name(wxFileNameFromPath(argv[0]));
    wxStripExtension(name);
    wxTheApp->SetAppName(name);

    int retValue;
    retValue = wxEntryInitGui();

    // Here frames insert themselves automatically into wxTopLevelWindows by
    // getting created in OnInit().
    if ( retValue == 0 )
    {
        if ( !wxTheApp->OnInit() )
            retValue = -1;
    }

    if ( retValue == 0 )
    {
        /* delete pending toplevel windows (typically a single
           dialog) so that, if there isn't any left, we don't
           call OnRun() */
        wxTheApp->DeletePendingObjects();

        if ( wxTheApp->Initialized() &&
             wxTopLevelWindows.GetCount() != 0 )
        {
            wxTheApp->OnRun();

            wxWindow *topWindow = wxTheApp->GetTopWindow();
            if ( topWindow )
            {
                /* Forcibly delete the window. */
                if (topWindow->IsKindOf(CLASSINFO(wxFrame)) ||
                    topWindow->IsKindOf(CLASSINFO(wxDialog)) )
                {
                    topWindow->Close(TRUE);
                    wxTheApp->DeletePendingObjects();
                }
                else
                {
                    delete topWindow;
                    wxTheApp->SetTopWindow((wxWindow*) NULL);
                }
            }

            retValue = wxTheApp->OnExit();
        }
    }

    wxEntryCleanup();

    return retValue;
}
