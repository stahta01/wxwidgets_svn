/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "app.h"
#endif

#include "wx/app.h"
#include "wx/gdicmn.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/log.h"
#include "wx/memory.h"
#include "wx/font.h"
#include "wx/settings.h"
#include "wx/dialog.h"

#if wxUSE_WX_RESOURCES
    #include "wx/resource.h"
#endif

#include "wx/module.h"
#include "wx/image.h"

#if wxUSE_THREADS
    #include "wx/thread.h"
#endif

#include <unistd.h>

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

wxApp *wxTheApp = (wxApp *)  NULL;
wxAppInitializerFunction wxAppBase::m_appInitFn = (wxAppInitializerFunction) NULL;

extern bool g_isIdle;

bool   g_mainThreadLocked = FALSE;
gint   g_pendingTag = 0;

static GtkWidget *gs_RootWindow = (GtkWidget*) NULL;

//-----------------------------------------------------------------------------
// local functions
//-----------------------------------------------------------------------------

/* forward declaration */
gint   wxapp_idle_callback( gpointer WXUNUSED(data) );
gint   wxapp_pending_callback( gpointer WXUNUSED(data) );
void   wxapp_install_idle_handler();

#if wxUSE_THREADS
gint   wxapp_wakeup_timerout_callback( gpointer WXUNUSED(data) );
#endif

//-----------------------------------------------------------------------------
// wxExit
//-----------------------------------------------------------------------------

void wxExit()
{
    gtk_main_quit();
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
        // can't call gtk_main_iteration() from other threads like this
        return TRUE;
    }
#endif // wxUSE_THREADS

#ifdef __WXDEBUG__
    if (gs_inYield)
        wxFAIL_MSG( wxT("wxYield called recursively" ) );
#endif

    gs_inYield = TRUE;

    if (!g_isIdle)
    {
        // We need to remove idle callbacks or the loop will
        // never finish.
        gtk_idle_remove( wxTheApp->m_idleTag );
        wxTheApp->m_idleTag = 0;
        g_isIdle = TRUE;
    }

    // disable log flushing from here because a call to wxYield() shouldn't
    // normally result in message boxes popping up &c
    wxLog::Suspend();

    while (gtk_events_pending())
        gtk_main_iteration();

    /* it's necessary to call ProcessIdle() to update the frames sizes which
       might have been changed (it also will update other things set from
       OnUpdateUI() which is a nice (and desired) side effect) */
    while (wxTheApp->ProcessIdle()) { }

    // let the logs be flashed again
    wxLog::Resume();

    gs_inYield = FALSE;

    return TRUE;
}

//-----------------------------------------------------------------------------
// wxYieldIfNeeded
// Like wxYield, but fails silently if the yield is recursive.
//-----------------------------------------------------------------------------

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

    if (g_isIdle)
        wxapp_install_idle_handler();

#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiLeave();
#endif
}

//-----------------------------------------------------------------------------
// local functions
//-----------------------------------------------------------------------------

gint wxapp_pending_callback( gpointer WXUNUSED(data) )
{
    if (!wxTheApp) return TRUE;

    // when getting called from GDK's time-out handler
    // we are no longer within GDK's grab on the GUI
    // thread so we must lock it here ourselves
    gdk_threads_enter();

    // Sent idle event to all who request them
    wxTheApp->ProcessPendingEvents();

    g_pendingTag = 0;

    /* flush the logged messages if any */
#if wxUSE_LOG
    wxLog::FlushActive();
#endif // wxUSE_LOG

    // Release lock again
    gdk_threads_leave();

    // Return FALSE to indicate that no more idle events are
    // to be sent (single shot instead of continuous stream)
    return FALSE;
}

gint wxapp_idle_callback( gpointer WXUNUSED(data) )
{
    if (!wxTheApp)
        return TRUE;

#ifdef __WXDEBUG__
    // don't generate the idle events while the assert modal dialog is shown,
    // this completely confuses the apps which don't expect to be reentered
    // from some safely-looking functions
    if ( wxTheApp->IsInAssert() )
    {
        return TRUE;
    }
#endif // __WXDEBUG__

    // when getting called from GDK's time-out handler
    // we are no longer within GDK's grab on the GUI
    // thread so we must lock it here ourselves
    gdk_threads_enter();

    /* Indicate that we are now in idle mode - even so deeply
       in idle mode that we don't get any idle events anymore.
       this is like wxMSW where an idle event is sent only
       once each time after the event queue has been completely
       emptied */
    g_isIdle = TRUE;
    wxTheApp->m_idleTag = 0;

    // Sent idle event to all who request them as long as they do
    while (wxTheApp->ProcessIdle())
        ;

    // Release lock again
    gdk_threads_leave();

    // Return FALSE to indicate that no more idle events are
    // to be sent (single shot instead of continuous stream)
    return FALSE;
}

void wxapp_install_idle_handler()
{
    wxASSERT_MSG( wxTheApp->m_idleTag == 0, wxT("attempt to install idle handler twice") );

    g_isIdle = FALSE;

    if (g_pendingTag == 0)
        g_pendingTag = gtk_idle_add_priority( 900, wxapp_pending_callback, (gpointer) NULL );

    /* This routine gets called by all event handlers
       indicating that the idle is over. It may also
       get called from other thread for sending events
       to the main thread (and processing these in
       idle time). Very low priority. */

    wxTheApp->m_idleTag = gtk_idle_add_priority( 1000, wxapp_idle_callback, (gpointer) NULL );
}

#if wxUSE_THREADS

static int g_threadUninstallLevel = 0;

void wxapp_install_thread_wakeup()
{
    g_threadUninstallLevel++;

    if (g_threadUninstallLevel != 1) return;

    if (wxTheApp->m_wakeUpTimerTag) return;

    wxTheApp->m_wakeUpTimerTag = gtk_timeout_add( 50, wxapp_wakeup_timerout_callback, (gpointer) NULL );
}

void wxapp_uninstall_thread_wakeup()
{
    g_threadUninstallLevel--;

    if (g_threadUninstallLevel != 0) return;

    if (!wxTheApp->m_wakeUpTimerTag) return;

    gtk_timeout_remove( wxTheApp->m_wakeUpTimerTag );
    wxTheApp->m_wakeUpTimerTag = 0;
}

gint wxapp_wakeup_timerout_callback( gpointer WXUNUSED(data) )
{
    // when getting called from GDK's time-out handler
    // we are no longer within GDK's grab on the GUI
    // thread so we must lock it here ourselves
    gdk_threads_enter();

    wxapp_uninstall_thread_wakeup();

    // unblock other threads wishing to do some GUI things
    wxMutexGuiLeave();

    g_mainThreadLocked = TRUE;

    // wake up other threads
    wxUsleep( 1 );

    // block other thread again
    wxMutexGuiEnter();

    g_mainThreadLocked = FALSE;

    wxapp_install_thread_wakeup();

    // release lock again
    gdk_threads_leave();

    return TRUE;
}

#endif // wxUSE_THREADS

//-----------------------------------------------------------------------------
// wxApp
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp,wxEvtHandler)

BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
END_EVENT_TABLE()

wxApp::wxApp()
{
    m_initialized = FALSE;
#ifdef __WXDEBUG__
    m_isInAssert = FALSE;
#endif // __WXDEBUG__

    m_idleTag = 0;
    wxapp_install_idle_handler();

#if wxUSE_THREADS
    m_wakeUpTimerTag = 0;
    wxapp_install_thread_wakeup();
#endif

    m_colorCube = (unsigned char*) NULL;
}

wxApp::~wxApp()
{
    if (m_idleTag) gtk_idle_remove( m_idleTag );

#if wxUSE_THREADS
    wxapp_uninstall_thread_wakeup();
#endif

    if (m_colorCube) free(m_colorCube);
}

bool wxApp::OnInitGui()
{
    if ( !wxAppBase::OnInitGui() )
        return FALSE;

    GdkVisual *visual = gdk_visual_get_system();

    /* on some machines, the default visual is just 256 colours, so
       we make sure we get the best. this can sometimes be wasteful,
       of course, but what do these guys pay $30.000 for? */

    if ((gdk_visual_get_best() != gdk_visual_get_system()) &&
        (m_useBestVisual))
    {
#ifdef __WXGTK20__
        /* seems gtk_widget_set_default_visual no longer exists? */
        GdkVisual* vis = gtk_widget_get_default_visual();
#else
        GdkVisual* vis = gdk_visual_get_best();
        gtk_widget_set_default_visual( vis );
#endif

        GdkColormap *colormap = gdk_colormap_new( vis, FALSE );
        gtk_widget_set_default_colormap( colormap );

        visual = vis;
    }

    /* Nothing to do for 15, 16, 24, 32 bit displays */
    if (visual->depth > 8) return TRUE;

    /* initialize color cube for 8-bit color reduction dithering */

    GdkColormap *cmap = gtk_widget_get_default_colormap();

    m_colorCube = (unsigned char*)malloc(32 * 32 * 32);

    for (int r = 0; r < 32; r++)
    {
        for (int g = 0; g < 32; g++)
        {
            for (int b = 0; b < 32; b++)
            {
                int rr = (r << 3) | (r >> 2);
                int gg = (g << 3) | (g >> 2);
                int bb = (b << 3) | (b >> 2);

                int index = -1;

                GdkColor *colors = cmap->colors;
                if (colors)
                {
                    int max = 3 * 65536;

                    for (int i = 0; i < cmap->size; i++)
                    {
                        int rdiff = ((rr << 8) - colors[i].red);
                        int gdiff = ((gg << 8) - colors[i].green);
                        int bdiff = ((bb << 8) - colors[i].blue);
                        int sum = ABS (rdiff) + ABS (gdiff) + ABS (bdiff);
                        if (sum < max)
                        {
                            index = i; max = sum;
                        }
                    }
                }
                else
                {
#if (GTK_MINOR_VERSION > 0)
                    /* assume 8-bit true or static colors. this really
                       exists. */
                    GdkVisual* vis = gdk_colormap_get_visual( cmap );
                    index = (r >> (5 - vis->red_prec)) << vis->red_shift;
                    index |= (g >> (5 - vis->green_prec)) << vis->green_shift;
                    index |= (b >> (5 - vis->blue_prec)) << vis->blue_shift;
#else
                    wxFAIL_MSG( wxT("Unsupported graphics hardware") );
#endif
                }
                m_colorCube[ (r*1024) + (g*32) + b ] = index;
            }
        }
    }

    return TRUE;
}

bool wxApp::ProcessIdle()
{
    wxIdleEvent event;
    event.SetEventObject( this );
    ProcessEvent( event );

    return event.MoreRequested();
}

void wxApp::OnIdle( wxIdleEvent &event )
{
    static bool s_inOnIdle = FALSE;

    /* Avoid recursion (via ProcessEvent default case) */
    if (s_inOnIdle)
        return;

    s_inOnIdle = TRUE;

    /* Resend in the main thread events which have been prepared in other
       threads */
    ProcessPendingEvents();

    /* 'Garbage' collection of windows deleted with Close(). */
    DeletePendingObjects();

    /* Send OnIdle events to all windows */
    bool needMore = SendIdleEvents();

    if (needMore)
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
        if (SendIdleEvents(win))
            needMore = TRUE;
        node = node->GetNext();
    }

    return needMore;
}

bool wxApp::SendIdleEvents( wxWindow* win )
{
    bool needMore = FALSE;

    wxIdleEvent event;
    event.SetEventObject(win);

    win->GetEventHandler()->ProcessEvent(event);

    win->OnInternalIdle();

    if (event.MoreRequested())
        needMore = TRUE;

    wxNode* node = win->GetChildren().First();
    while (node)
    {
        wxWindow* win = (wxWindow*) node->Data();
        if (SendIdleEvents(win))
            needMore = TRUE;

        node = node->Next();
    }
    return needMore ;
}

int wxApp::MainLoop()
{
    gtk_main();
    return 0;
}

void wxApp::ExitMainLoop()
{
    if (gtk_main_level() > 0)
        gtk_main_quit();
}

bool wxApp::Initialized()
{
    return m_initialized;
}

bool wxApp::Pending()
{
    return (gtk_events_pending() > 0);
}

void wxApp::Dispatch()
{
    gtk_main_iteration();
}

void wxApp::DeletePendingObjects()
{
    wxNode *node = wxPendingDelete.First();
    while (node)
    {
        wxObject *obj = (wxObject *)node->Data();

        delete obj;

        if (wxPendingDelete.Find(obj))
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
    wxPendingEvents = new wxList();
    wxPendingEventsLocker = new wxCriticalSection();
#endif

    wxTheColourDatabase = new wxColourDatabase( wxKEY_STRING );
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

void wxApp::CleanUp()
{
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
}

//-----------------------------------------------------------------------------
// Access to the root window global
//-----------------------------------------------------------------------------

GtkWidget* wxGetRootWindow()
{
    if (gs_RootWindow == NULL) {
        gs_RootWindow = gtk_window_new( GTK_WINDOW_TOPLEVEL );
        gtk_widget_realize( gs_RootWindow );
    }
    return gs_RootWindow;
}

//-----------------------------------------------------------------------------
// wxEntry
//-----------------------------------------------------------------------------


int wxEntryStart( int argc, char *argv[] )
{
#if wxUSE_THREADS
    /* GTK 1.2 up to version 1.2.3 has broken threads */
   if ((gtk_major_version == 1) &&
        (gtk_minor_version == 2) &&
        (gtk_micro_version < 4))
    {
        printf( "wxWindows warning: GUI threading disabled due to outdated GTK version\n" );
    }
    else
    {
        g_thread_init(NULL);
    }
#endif

    gtk_set_locale();

    // We should have the wxUSE_WCHAR_T test on the _outside_
#if wxUSE_WCHAR_T
#if defined(__WXGTK20__)
    // gtk+ 2.0 supports Unicode through UTF-8 strings
    wxConvCurrent = &wxConvUTF8;
#else
    if (!wxOKlibc()) wxConvCurrent = &wxConvLocal;
#endif
#else
    if (!wxOKlibc()) wxConvCurrent = (wxMBConv*) NULL;
#endif

    gdk_threads_enter();

    gtk_init( &argc, &argv );

    wxSetDetectableAutoRepeat( TRUE );

    if (!wxApp::Initialize())
    {
        gdk_threads_leave();
        return -1;
    }

    return 0;
}


int wxEntryInitGui()
{
    int retValue = 0;

    if ( !wxTheApp->OnInitGui() )
        retValue = -1;

    wxGetRootWindow();

    return retValue;
}


void wxEntryCleanup()
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

    wxApp::CleanUp();

    gdk_threads_leave();
}



int wxEntry( int argc, char *argv[] )
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
    if (err)
        return err;

    if (!wxTheApp)
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
    wxStripExtension( name );
    wxTheApp->SetAppName( name );

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

        wxTheApp->m_initialized = wxTopLevelWindows.GetCount() != 0;

        if (wxTheApp->Initialized())
        {
            wxTheApp->OnRun();

            wxWindow *topWindow = wxTheApp->GetTopWindow();
            if (topWindow)
            {
                /* Forcibly delete the window. */
                if (topWindow->IsKindOf(CLASSINFO(wxFrame)) ||
                    topWindow->IsKindOf(CLASSINFO(wxDialog)) )
                {
                    topWindow->Close( TRUE );
                    wxTheApp->DeletePendingObjects();
                }
                else
                {
                    delete topWindow;
                    wxTheApp->SetTopWindow( (wxWindow*) NULL );
                }
            }

            retValue = wxTheApp->OnExit();
        }
    }

    wxEntryCleanup();

    return retValue;
}

#include "wx/gtk/info.xpm"
#include "wx/gtk/error.xpm"
#include "wx/gtk/question.xpm"
#include "wx/gtk/warning.xpm"

wxIcon
wxApp::GetStdIcon(int which) const
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

#ifdef __WXDEBUG__

void wxApp::OnAssert(const wxChar *file, int line, const wxChar *msg)
{
    m_isInAssert = TRUE;

    wxAppBase::OnAssert(file, line, msg);

    m_isInAssert = FALSE;
}

#endif // __WXDEBUG__

