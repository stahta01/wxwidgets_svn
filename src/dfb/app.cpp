/////////////////////////////////////////////////////////////////////////////
// Name:        src/dfb/app.cpp
// Purpose:     wxApp implementation
// Author:      Vaclav Slavik
//              based on MGL implementation
// Created:     2006-08-16
// RCS-ID:      $Id$
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/app.h"

#include "wx/evtloop.h"
#include "wx/dfb/private.h"

//-----------------------------------------------------------------------------
// wxApp initialization
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)

BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxAppBase::OnIdle)
END_EVENT_TABLE()

wxApp::wxApp()
{
}

wxApp::~wxApp()
{
}

IDirectFBPtr wxApp::GetDirectFBInterface()
{
    return m_dfb;
}

bool wxApp::Initialize(int& argc, wxChar **argv)
{
    if ( !wxAppBase::Initialize(argc, argv) )
        return false;

    if ( !DFB_CALL( DirectFBInit(&argc, &argv) ) )
        return false;

    if ( !DFB_CALL( DirectFBCreate(&m_dfb) ) )
        return false;

    #warning "FIXME: theme override is temporary"
    wxTheme::Set(wxTheme::Create(_T("gtk")));

    return true;
}

void wxApp::CleanUp()
{
    wxAppBase::CleanUp();

    m_dfb.Reset();
}

//-----------------------------------------------------------------------------
// display mode
//-----------------------------------------------------------------------------

static wxVideoMode GetCurrentVideoMode()
{
    wxVideoMode m;

    IDirectFBSurfacePtr surface(wxDfbGetPrimarySurface());
    if ( !surface )
        return m; // invalid

    DFB_CALL( surface->GetSize(surface, &m.w, &m.h) );
    m.bpp = wxDfbGetSurfaceDepth(surface);

    return m;
}

wxVideoMode wxApp::GetDisplayMode() const
{
    if ( !m_videoMode.IsOk() )
        wxConstCast(this, wxApp)->m_videoMode = GetCurrentVideoMode();

    return m_videoMode;
}

bool wxApp::SetDisplayMode(const wxVideoMode& mode)
{
    if ( !DFB_CALL( m_dfb->SetVideoMode(m_dfb, mode.w, mode.h, mode.bpp) ) )
        return false;

    m_videoMode = mode;
    return true;
}

//-----------------------------------------------------------------------------
// events processing related
//-----------------------------------------------------------------------------

void wxApp::WakeUpIdle()
{
#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiEnter();
#endif

    wxEventLoop::GetActive()->WakeUp();

#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiLeave();
#endif
}


bool wxApp::Yield(bool onlyIfNeeded)
{
#if wxUSE_THREADS
    if ( !wxThread::IsMain() )
        return true; // can't process events from other threads
#endif // wxUSE_THREADS

    static bool s_inYield = false;

    if ( s_inYield )
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return false;
    }

    s_inYield = true;

    wxLog::Suspend();

    if ( wxEventLoop::GetActive() )
    {
        while (wxEventLoop::GetActive()->Pending())
            wxEventLoop::GetActive()->Dispatch();
    }

    // it's necessary to call ProcessIdle() to update the frames sizes which
    // might have been changed (it also will update other things set from
    // OnUpdateUI() which is a nice (and desired) side effect)
    while ( ProcessIdle() ) {}

    wxLog::Resume();

    s_inYield = false;

    return true;
}
