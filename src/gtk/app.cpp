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

#include "wx/thread.h"

#include "unistd.h"

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

wxApp *wxTheApp = (wxApp *)  NULL;
wxAppInitializerFunction wxApp::m_appInitFn = (wxAppInitializerFunction) NULL;

extern wxList wxPendingDelete;
#if wxUSE_THREADS
extern wxList wxPendingEvents;
extern wxCriticalSection wxPendingEventsLocker;
#endif
extern wxResourceCache *wxTheResourceCache;

unsigned char g_palette[64*3] =
{
  0x0,  0x0,  0x0,
  0xff, 0xff, 0xff,
  0xff, 0x0,  0x0,
  0xff, 0xff, 0x0,
  0x0,  0xff, 0x0,
  0x0,  0x0,  0xff,
  0x0,  0xff, 0xff,
  0x99, 0x99, 0x99,
  0xff, 0x88, 0x0,
  0x88, 0x0,  0x0,
  0x0,  0x88, 0x88,
  0x88, 0x88, 0x0,
  0xff, 0xcc, 0x97,
  0xbb, 0xbb, 0xbb,
  0x9f, 0x6b, 0x42,
  0x55, 0x55, 0x55,
  0xdd, 0xdd, 0xdd,
  0x77, 0x77, 0x77,
  0x33, 0x33, 0x33,
  0xcc, 0x0,  0x0,
  0xff, 0x44, 0x0,
  0xff, 0xcc, 0x0,
  0xcc, 0xcc, 0x0,
  0x60, 0x60, 0x0,
  0x0,  0x43, 0x0,
  0x0,  0x7f, 0x0,
  0x0,  0xcc, 0x0,
  0x0,  0x44, 0x44,
  0x0,  0x0,  0x44,
  0x0,  0x0,  0x88,
  0xef, 0xb1, 0x7b,
  0xdf, 0x98, 0x5f,
  0xbf, 0x87, 0x56,
  0x7f, 0x57, 0x26,
  0x5f, 0x39, 0xc,
  0x3f, 0x1c, 0x0,
  0x21, 0x0,  0x0,
  0x0,  0x43, 0x87,
  0x2d, 0x70, 0xaf,
  0x5a, 0x9e, 0xd7,
  0x87, 0xcc, 0xff,
  0xff, 0xe0, 0xba,
  0x21, 0x43, 0xf,
  0x3d, 0x5d, 0x25,
  0x59, 0x78, 0x3a,
  0x75, 0x93, 0x4f,
  0x91, 0xae, 0x64,
  0xad, 0xc8, 0x7a,
  0xf0, 0xa8, 0xef,
  0xd0, 0x88, 0xd0,
  0xaf, 0x66, 0xaf,
  0x8e, 0x44, 0x8e,
  0x6d, 0x22, 0x6d,
  0x4b, 0x0,  0x4b,
  0xff, 0xc0, 0xbc,
  0xff, 0x93, 0x91,
  0xff, 0x66, 0x67,
  0xd8, 0xf2, 0xbf,
  0xff, 0xc9, 0x68,
  0xff, 0x96, 0x67,
  0xa5, 0x60, 0xff,
  0x51, 0xff, 0x99,
  0x3f, 0xa5, 0x63,
  0x98, 0x90, 0x67
};

//-----------------------------------------------------------------------------
// local functions
//-----------------------------------------------------------------------------

extern void wxFlushResources(void);

//-----------------------------------------------------------------------------
// global functions
//-----------------------------------------------------------------------------

void wxExit()
{
    gtk_main_quit();
}

bool wxYield()
{
    // it's necessary to call ProcessIdle() to update the frames sizes which
    // might have been changed (it also will update other things set from
    // OnUpdateUI() which is a nice (and desired) side effect)
    for ( wxWindowList::Node *node = wxTopLevelWindows.GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *win = node->GetData();
        win->OnInternalIdle();
    }

    while (gtk_events_pending() > 0)
        gtk_main_iteration();

    return TRUE;
}

//-----------------------------------------------------------------------------
// wxApp
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp,wxEvtHandler)

BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
END_EVENT_TABLE()

gint wxapp_idle_callback( gpointer WXUNUSED(data) )
{
    if (wxTheApp)
    {
        while (wxTheApp->ProcessIdle())
        {
        }
    }

    wxMutexGuiLeave();
    wxUsleep(10);
    wxMutexGuiEnter();

    return TRUE;
}

wxApp::wxApp()
{
    wxTheApp = this;

    m_topWindow = (wxWindow *) NULL;
    m_exitOnFrameDelete = TRUE;

    m_idleTag = gtk_idle_add( wxapp_idle_callback, (gpointer) NULL );

    m_colorCube = (unsigned char*) NULL;
}

wxApp::~wxApp()
{
    gtk_idle_remove( m_idleTag );

    if (m_colorCube) free(m_colorCube);
}

bool wxApp::OnInitGui()
{
    /* Nothing to do for 15, 16, 24, 32 bit displays */

    GdkVisual *visual = gdk_visual_get_system();
    if (visual->depth > 8) return TRUE;

        /* this initiates the standard palette as defined by GdkImlib
           in the GNOME libraries. it ensures that all GNOME applications
           use the same 64 colormap entries on 8-bit displays so you
           can use several rather graphics-heavy applications at the
           same time.
	   NOTE: this doesn't really seem to work this way... */

        /*
        GdkColormap *cmap = gdk_colormap_new( gdk_visual_get_system(), TRUE );

        for (int i = 0; i < 64; i++)
        {
            GdkColor col;
            col.red    = g_palette[i*3 + 0] << 8;
            col.green  = g_palette[i*3 + 1] << 8;
            col.blue   = g_palette[i*3 + 2] << 8;
            col.pixel  = 0;

            gdk_color_alloc( cmap, &col );
        }

        gtk_widget_set_default_colormap( cmap );
        */

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

                GdkColor *colors = cmap->colors;
                int max = 3 * (65536);
                int index = -1;

                for (int i = 0; i < cmap->size; i++)
                {
                    int rdiff = ((rr << 8) - colors[i].red);
                    int gdiff = ((gg << 8)- colors[i].green);
                    int bdiff = ((bb << 8)- colors[i].blue);
                    int sum = ABS (rdiff) + ABS (gdiff) + ABS (bdiff);
                    if (sum < max) { index = i; max = sum; }
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
    static bool inOnIdle = FALSE;

    /* Avoid recursion (via ProcessEvent default case) */
    if (inOnIdle)
        return;

    inOnIdle = TRUE;

#if wxUSE_THREADS
    /* Resend in the main thread events which have been prepared in other
       threads */
    ProcessPendingEvents();
#endif

    /* 'Garbage' collection of windows deleted with Close(). */
    DeletePendingObjects();

    /* flush the logged messages if any */
    wxLog *log = wxLog::GetActiveTarget();
    if (log != NULL && log->HasPendingMessages())
        log->Flush();

    /* Send OnIdle events to all windows */
    bool needMore = SendIdleEvents();

    if (needMore)
        event.RequestMore(TRUE);

    inOnIdle = FALSE;
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

    win->OnInternalIdle();

    win->ProcessEvent(event);

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
    gtk_main_quit();
}

bool wxApp::Initialized()
{
    return m_initialized;
}

bool wxApp::Pending()
{
    return FALSE;
}

void wxApp::Dispatch()
{
}

#if wxUSE_THREADS
void wxApp::ProcessPendingEvents()
{
    wxNode *node = wxPendingEvents.First();
    wxCriticalSectionLocker locker(wxPendingEventsLocker);

    while (node)
    {
        wxEvtHandler *handler = (wxEvtHandler *)node->Data();

        handler->ProcessPendingEvents();

        delete node;

        node = wxPendingEvents.First();
    }
}
#endif

void wxApp::DeletePendingObjects()
{
    wxNode *node = wxPendingDelete.First();
    while (node)
    {
        wxObject *obj = (wxObject *)node->Data();

        delete obj;

        if (wxPendingDelete.Member(obj))
        delete node;

        node = wxPendingDelete.First();
    }
}

wxWindow *wxApp::GetTopWindow()
{
    if (m_topWindow)
        return m_topWindow;
    else if (wxTopLevelWindows.GetCount() > 0)
        return wxTopLevelWindows.GetFirst()->GetData();
    else
        return NULL;
}

void wxApp::SetTopWindow( wxWindow *win )
{
    m_topWindow = win;
}

bool wxApp::Initialize()
{
    wxBuffer = new char[BUFSIZ + 512];

    wxClassInfo::InitializeClasses();

    wxSystemSettings::Init();

/*
    wxTheFontNameDirectory =  new wxFontNameDirectory;
    wxTheFontNameDirectory->Initialize();
*/

    wxTheColourDatabase = new wxColourDatabase( wxKEY_STRING );
    wxTheColourDatabase->Initialize();

    wxInitializeStockLists();
    wxInitializeStockObjects();

#if wxUSE_WX_RESOURCES
    wxTheResourceCache = new wxResourceCache( wxKEY_STRING );

    wxInitializeResourceSystem();
#endif

    wxImage::InitStandardHandlers();

    /* no global cursor under X
       g_globalCursor = new wxCursor; */

    wxModule::RegisterModules();
    if (!wxModule::InitializeModules()) return FALSE;

    return TRUE;
}

void wxApp::CleanUp()
{
    wxModule::CleanUpModules();

#if wxUSE_WX_RESOURCES
    wxFlushResources();

    if (wxTheResourceCache)
        delete wxTheResourceCache;
    wxTheResourceCache = (wxResourceCache*) NULL;

    wxCleanUpResourceSystem();
#endif

    if (wxTheColourDatabase)
        delete wxTheColourDatabase;
    wxTheColourDatabase = (wxColourDatabase*) NULL;

/*
    if (wxTheFontNameDirectory) delete wxTheFontNameDirectory;
    wxTheFontNameDirectory = (wxFontNameDirectory*) NULL;
*/

    wxDeleteStockObjects();

    wxDeleteStockLists();

    wxImage::CleanUpHandlers();

    delete wxTheApp;
    wxTheApp = (wxApp*) NULL;

    wxSystemSettings::Done();

    delete[] wxBuffer;

    wxClassInfo::CleanUpClasses();

    // check for memory leaks
#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
    if (wxDebugContext::CountObjectsLeft() > 0)
    {
        wxLogDebug("There were memory leaks.\n");
        wxDebugContext::Dump();
        wxDebugContext::PrintStatistics();
    }
#endif

    // do this as the very last thing because everything else can log messages
    wxLog::DontCreateOnDemand();

    wxLog *oldLog = wxLog::SetActiveTarget( (wxLog*) NULL );
    if (oldLog)
        delete oldLog;
}

wxLog *wxApp::CreateLogTarget()
{
    return new wxLogGui;
}

//-----------------------------------------------------------------------------
// wxEntry
//-----------------------------------------------------------------------------

int wxEntry( int argc, char *argv[] )
{
    gtk_set_locale();

    gtk_init( &argc, &argv );

    if (!wxApp::Initialize())
        return -1;

    if (!wxTheApp)
    {
        wxCHECK_MSG( wxApp::GetInitializerFunction(), -1,
                     "wxWindows error: No initializer - use IMPLEMENT_APP macro.\n" );

        wxAppInitializerFunction app_ini = wxApp::GetInitializerFunction();

        wxObject *test_app = app_ini();

        wxTheApp = (wxApp*) test_app;
    }

    wxCHECK_MSG( wxTheApp, -1, "wxWindows error: no application object" );

    wxTheApp->argc = argc;
    wxTheApp->argv = argv;

    char name[200];
    strcpy( name, argv[0] );
    strcpy( name, wxFileNameFromPath(name) );
    wxStripExtension( name );
    wxTheApp->SetAppName( name );

    if (!wxTheApp->OnInitGui())
        return 0;

    /* Here frames insert themselves automatically
     * into wxTopLevelWindows by getting created
     * in OnInit(). */

    if (!wxTheApp->OnInit())
        return 0;

    wxTheApp->m_initialized = wxTopLevelWindows.GetCount() != 0;

    int retValue = 0;

    if (wxTheApp->Initialized())
        retValue = wxTheApp->OnRun();

    wxWindow *topWindow = wxTheApp->GetTopWindow();
    if (topWindow)
    {
        // Forcibly delete the window.
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

    wxTheApp->OnExit();

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

    wxApp::CleanUp();

    return retValue;
}

