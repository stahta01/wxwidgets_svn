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
#endif

#include "wx/module.h"

// ----------------------------------------------------------------------------
// global vars
// ----------------------------------------------------------------------------

wxApp * WXDLLEXPORT wxTheApp = NULL;

wxAppInitializerFunction
    wxAppBase::m_appInitFn = (wxAppInitializerFunction)NULL;

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class /* no WXDLLEXPORT */ wxConsoleApp : public wxApp
{
public:
    virtual int OnRun() { wxFAIL_MSG(wxT("unreachable")); return 0; }
};

// ----------------------------------------------------------------------------
// private vars
// ----------------------------------------------------------------------------

static size_t gs_nInitCount = 0;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// stubs for some GUI functions
// ----------------------------------------------------------------------------

void WXDLLEXPORT wxExit()
{
    abort();
}

// Yield to other apps/messages
bool WXDLLEXPORT wxYield()
{
    // do nothing
    return TRUE;
}

// Yield to other apps/messages
void WXDLLEXPORT wxWakeUpIdle()
{
    // do nothing
}

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

    wxClassInfo::InitializeClasses();

    wxModule::RegisterModules();
    if ( !wxModule::InitializeModules() )
    {
        return FALSE;
    }

    wxTheApp = new wxConsoleApp;

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
        wxModule::CleanUpModules();

        wxClassInfo::CleanUpClasses();

        // delete the application object
        delete wxTheApp;
        wxTheApp = (wxApp *)NULL;
    }
}
