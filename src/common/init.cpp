/////////////////////////////////////////////////////////////////////////////
// Name:        common/init.cpp
// Purpose:     initialisation for the library
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04.10.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef    __BORLANDC__
  #pragma hdrstop
#endif  //__BORLANDC__

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/debug.h"
    #include "wx/filefn.h"
    #include "wx/log.h"
#endif

#include "wx/module.h"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// we need a dummy app object if the user doesn't want to create a real one
class wxDummyConsoleApp : public wxApp
{
public:
    virtual int OnRun() { wxFAIL_MSG( _T("unreachable code") ); return 0; }
};

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static bool DoInit();
static void DoCleanUp();

// ----------------------------------------------------------------------------
// private vars
// ----------------------------------------------------------------------------

static size_t gs_nInitCount = 0;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxBase-specific functions
// ----------------------------------------------------------------------------

bool WXDLLEXPORT wxInitialize()
{
    if ( gs_nInitCount )
    {
        // already initialized
        return TRUE;
    }

    wxASSERT_MSG( !wxTheApp,
                  wxT("either call wxInitialize or create app, not both!") );

    if ( !DoInit() )
    {
        return FALSE;
    }

    wxTheApp = new wxDummyConsoleApp;

    if ( !wxTheApp )
    {
        return FALSE;
    }

    gs_nInitCount++;

    return TRUE;
}

void WXDLLEXPORT wxUninitialize()
{
    if ( !--gs_nInitCount )
    {
        DoCleanUp();
    }
}

int wxEntry(int argc, char **argv)
{
    // library initialization
    if ( !DoInit() )
    {
        return -1;
    }

    // create the app
    if ( !wxTheApp )
    {
        wxCHECK_MSG( wxApp::GetInitializerFunction(), -1,
                wxT("No application object: use IMPLEMENT_APP macro.") );

        wxAppInitializerFunction fnCreate = wxApp::GetInitializerFunction();

        wxTheApp = (wxApp *)fnCreate();
    }

    wxCHECK_MSG( wxTheApp, -1, wxT("wxWindows error: no application object") );

    // app preinitialization
    wxTheApp->argc = argc;

#if wxUSE_UNICODE
    wxTheApp->argv = new wxChar*[argc+1];
    for ( int mb_argc = 0; mb_argc < argc; mb_argc++ )
    {
        wxTheApp->argv[mb_argc] = wxStrdup(wxConvLocal.cMB2WX(argv[mb_argc]));
    }
    wxTheApp->argv[mb_argc] = (wxChar *)NULL;
#else
    wxTheApp->argv = argv;
#endif

    wxString name = wxFileNameFromPath(wxTheApp->argv[0]);
    wxStripExtension(name);
    wxTheApp->SetAppName(name);

    int retValue = 0;

    // app initialization
    if ( !wxTheApp->OnInit() )
        retValue = -1;

    // app execution
    if ( retValue == 0 )
    {
        retValue = wxTheApp->OnRun();

        // app clean up
        wxTheApp->OnExit();
    }

    // library clean up
    DoCleanUp();

    return retValue;
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static bool DoInit()
{
    wxClassInfo::InitializeClasses();

    wxModule::RegisterModules();
    if ( !wxModule::InitializeModules() )
    {
        return FALSE;
    }

    return TRUE;
}

static void DoCleanUp()
{
#if wxUSE_LOG
    // flush the logged messages if any
    wxLog *log = wxLog::GetActiveTarget();
    if (log != NULL && log->HasPendingMessages())
        log->Flush();

    // continuing to use user defined log target is unsafe from now on because
    // some resources may be already unavailable, so replace it by something
    // more safe
    wxLog::DontCreateOnDemand();
    delete wxLog::SetActiveTarget(new wxLogStderr);
#endif // wxUSE_LOG

    wxModule::CleanUpModules();

    wxClassInfo::CleanUpClasses();

    // TODO: this should really be done in ~wxApp
#if wxUSE_UNICODE
    for ( int mb_argc = 0; mb_argc < wxTheApp->argc; mb_argc++ )
    {
        free(wxTheApp->argv[mb_argc]);
    }
#endif // wxUSE_UNICODE

    // delete the application object
    delete wxTheApp;
    wxTheApp = (wxApp *)NULL;

#if wxUSE_LOG
    // and now delete the last logger as well
    delete wxLog::SetActiveTarget(NULL);
#endif // wxUSE_LOG
}

