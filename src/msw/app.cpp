/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/app.cpp
// Purpose:     wxApp
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/msw/wrapcctl.h"
    #include "wx/dynarray.h"
    #include "wx/frame.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/gdicmn.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/cursor.h"
    #include "wx/icon.h"
    #include "wx/palette.h"
    #include "wx/dc.h"
    #include "wx/dialog.h"
    #include "wx/msgdlg.h"
    #include "wx/intl.h"
    #include "wx/crt.h"
    #include "wx/log.h"
    #include "wx/module.h"
#endif

#include "wx/apptrait.h"
#include "wx/filename.h"
#include "wx/dynlib.h"
#include "wx/evtloop.h"
#include "wx/thread.h"

#include "wx/msw/private.h"
#include "wx/msw/dc.h"
#include "wx/msw/ole/oleutils.h"
#include "wx/msw/private/timer.h"

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif // wxUSE_TOOLTIPS

// OLE is used for drag-and-drop, clipboard, OLE Automation..., but some
// compilers don't support it (missing headers, libs, ...)
#if defined(__GNUWIN32_OLD__) || defined(__SYMANTEC__)
    #undef wxUSE_OLE

    #define  wxUSE_OLE 0
#endif // broken compilers

#if defined(__POCKETPC__) || defined(__SMARTPHONE__)
    #include <ole2.h>
    #include <aygshell.h>
#endif

#if wxUSE_OLE
    #include <ole2.h>
#endif

#include <string.h>
#include <ctype.h>

#include "wx/msw/missing.h"

// instead of including <shlwapi.h> which is not part of the core SDK and not
// shipped at all with other compilers, we always define the parts of it we
// need here ourselves
//
// NB: DLLVER_PLATFORM_WINDOWS will be defined if shlwapi.h had been somehow
//     included already
#ifndef DLLVER_PLATFORM_WINDOWS
    // hopefully we don't need to change packing as DWORDs should be already
    // correctly aligned
    struct DLLVERSIONINFO
    {
        DWORD cbSize;
        DWORD dwMajorVersion;                   // Major version
        DWORD dwMinorVersion;                   // Minor version
        DWORD dwBuildNumber;                    // Build number
        DWORD dwPlatformID;                     // DLLVER_PLATFORM_*
    };

    typedef HRESULT (CALLBACK* DLLGETVERSIONPROC)(DLLVERSIONINFO *);
#endif // defined(DLLVERSIONINFO)


// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
extern void wxSetKeyboardHook(bool doIt);
#endif

// NB: all "NoRedraw" classes must have the same names as the "normal" classes
//     with NR suffix - wxWindow::MSWCreate() supposes this
#ifdef __WXWINCE__
WXDLLIMPEXP_CORE       wxChar *wxCanvasClassName;
WXDLLIMPEXP_CORE       wxChar *wxCanvasClassNameNR;
#else
WXDLLIMPEXP_CORE const wxChar *wxCanvasClassName        = wxT("wxWindowClass");
WXDLLIMPEXP_CORE const wxChar *wxCanvasClassNameNR      = wxT("wxWindowClassNR");
#endif
WXDLLIMPEXP_CORE const wxChar *wxMDIFrameClassName      = wxT("wxMDIFrameClass");
WXDLLIMPEXP_CORE const wxChar *wxMDIFrameClassNameNoRedraw = wxT("wxMDIFrameClassNR");
WXDLLIMPEXP_CORE const wxChar *wxMDIChildFrameClassName = wxT("wxMDIChildFrameClass");
WXDLLIMPEXP_CORE const wxChar *wxMDIChildFrameClassNameNoRedraw = wxT("wxMDIChildFrameClassNR");

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

LRESULT WXDLLEXPORT APIENTRY wxWndProc(HWND, UINT, WPARAM, LPARAM);

// ===========================================================================
// wxGUIAppTraits implementation
// ===========================================================================

// private class which we use to pass parameters from BeforeChildWaitLoop() to
// AfterChildWaitLoop()
struct ChildWaitLoopData
{
    ChildWaitLoopData(wxWindowDisabler *wd_, wxWindow *winActive_)
    {
        wd = wd_;
        winActive = winActive_;
    }

    wxWindowDisabler *wd;
    wxWindow *winActive;
};

void *wxGUIAppTraits::BeforeChildWaitLoop()
{
    /*
       We use a dirty hack here to disable all application windows (which we
       must do because otherwise the calls to wxYield() could lead to some very
       unexpected reentrancies in the users code) but to avoid losing
       focus/activation entirely when the child process terminates which would
       happen if we simply disabled everything using wxWindowDisabler. Indeed,
       remember that Windows will never activate a disabled window and when the
       last childs window is closed and Windows looks for a window to activate
       all our windows are still disabled. There is no way to enable them in
       time because we don't know when the childs windows are going to be
       closed, so the solution we use here is to keep one special tiny frame
       enabled all the time. Then when the child terminates it will get
       activated and when we close it below -- after reenabling all the other
       windows! -- the previously active window becomes activated again and
       everything is ok.
     */
    wxBeginBusyCursor();

    // first disable all existing windows
    wxWindowDisabler *wd = new wxWindowDisabler;

    // then create an "invisible" frame: it has minimal size, is positioned
    // (hopefully) outside the screen and doesn't appear on the taskbar
    wxWindow *winActive = new wxFrame
                    (
                        wxTheApp->GetTopWindow(),
                        wxID_ANY,
                        wxEmptyString,
                        wxPoint(32600, 32600),
                        wxSize(1, 1),
                        wxDEFAULT_FRAME_STYLE | wxFRAME_NO_TASKBAR
                    );
    winActive->Show();

    return new ChildWaitLoopData(wd, winActive);
}

void wxGUIAppTraits::AlwaysYield()
{
    wxYield();
}

void wxGUIAppTraits::AfterChildWaitLoop(void *dataOrig)
{
    wxEndBusyCursor();

    ChildWaitLoopData * const data = (ChildWaitLoopData *)dataOrig;

    delete data->wd;

    // finally delete the dummy frame and, as wd has been already destroyed and
    // the other windows reenabled, the activation is going to return to the
    // window which had had it before
    data->winActive->Destroy();

    // also delete the temporary data object itself
    delete data;
}

bool wxGUIAppTraits::DoMessageFromThreadWait()
{
    // we should return false only if the app should exit, i.e. only if
    // Dispatch() determines that the main event loop should terminate
    wxEventLoopBase * const evtLoop = wxEventLoop::GetActive();
    if ( !evtLoop || !evtLoop->Pending() )
    {
        // no events means no quit event
        return true;
    }

    return evtLoop->Dispatch();
}

DWORD wxGUIAppTraits::WaitForThread(WXHANDLE hThread)
{
    // if we don't have a running event loop, we shouldn't wait for the
    // messages as we never remove them from the message queue and so we enter
    // an infinite loop as MsgWaitForMultipleObjects() keeps returning
    // WAIT_OBJECT_0 + 1
    if ( !wxEventLoop::GetActive() )
        return DoSimpleWaitForThread(hThread);

    return ::MsgWaitForMultipleObjects
             (
               1,                   // number of objects to wait for
               (HANDLE *)&hThread,  // the objects
               false,               // wait for any objects, not all
               INFINITE,            // no timeout
               QS_ALLINPUT |        // return as soon as there are any events
               QS_ALLPOSTMESSAGE
             );
}

wxPortId wxGUIAppTraits::GetToolkitVersion(int *majVer, int *minVer) const
{
    OSVERSIONINFO info;
    wxZeroMemory(info);

    // on Windows, the toolkit version is the same of the OS version
    // as Windows integrates the OS kernel with the GUI toolkit.
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if ( ::GetVersionEx(&info) )
    {
        if ( majVer )
            *majVer = info.dwMajorVersion;
        if ( minVer )
            *minVer = info.dwMinorVersion;
    }

#if defined(__WXHANDHELD__) || defined(__WXWINCE__)
    return wxPORT_WINCE;
#else
    return wxPORT_MSW;
#endif
}

#if wxUSE_TIMER

wxTimerImpl *wxGUIAppTraits::CreateTimerImpl(wxTimer *timer)
{
    return new wxMSWTimerImpl(timer);
}

#endif // wxUSE_TIMER

wxEventLoopBase* wxGUIAppTraits::CreateEventLoop()
{
    return new wxEventLoop;
}

// ===========================================================================
// wxApp implementation
// ===========================================================================

int wxApp::m_nCmdShow = SW_SHOWNORMAL;

// ---------------------------------------------------------------------------
// wxWin macros
// ---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)

BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
    EVT_END_SESSION(wxApp::OnEndSession)
    EVT_QUERY_END_SESSION(wxApp::OnQueryEndSession)
END_EVENT_TABLE()

// class to ensure that wxAppBase::CleanUp() is called if our Initialize()
// fails
class wxCallBaseCleanup
{
public:
    wxCallBaseCleanup(wxApp *app) : m_app(app) { }
    ~wxCallBaseCleanup() { if ( m_app ) m_app->wxAppBase::CleanUp(); }

    void Dismiss() { m_app = NULL; }

private:
    wxApp *m_app;
};

//// Initialize
bool wxApp::Initialize(int& argc, wxChar **argv)
{
    if ( !wxAppBase::Initialize(argc, argv) )
        return false;

    // ensure that base cleanup is done if we return too early
    wxCallBaseCleanup callBaseCleanup(this);

#ifdef __WXWINCE__
    wxString tmp = GetAppName();
    tmp += wxT("ClassName");
    wxCanvasClassName = wxStrdup( tmp.wc_str() );
    tmp += wxT("NR");
    wxCanvasClassNameNR = wxStrdup( tmp.wc_str() );
    HWND hWnd = FindWindow( wxCanvasClassNameNR, NULL );
    if (hWnd)
    {
        SetForegroundWindow( (HWND)(((DWORD)hWnd)|0x01) );
        return false;
    }
#endif

#if !defined(__WXMICROWIN__)
    InitCommonControls();
#endif // !defined(__WXMICROWIN__)

#if defined(__SMARTPHONE__) || defined(__POCKETPC__)
    SHInitExtraControls();
#endif

#ifndef __WXWINCE__
    // Don't show a message box if a function such as SHGetFileInfo
    // fails to find a device.
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
#endif

    wxOleInitialize();

    RegisterWindowClasses();

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
    wxSetKeyboardHook(true);
#endif

    callBaseCleanup.Dismiss();

    return true;
}

// ---------------------------------------------------------------------------
// RegisterWindowClasses
// ---------------------------------------------------------------------------

// TODO we should only register classes really used by the app. For this it
//      would be enough to just delay the class registration until an attempt
//      to create a window of this class is made.
bool wxApp::RegisterWindowClasses()
{
    WNDCLASS wndclass;
    wxZeroMemory(wndclass);

    // for each class we register one with CS_(V|H)REDRAW style and one
    // without for windows created with wxNO_FULL_REDRAW_ON_REPAINT flag
    static const long styleNormal = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    static const long styleNoRedraw = CS_DBLCLKS;

    // the fields which are common to all classes
    wndclass.lpfnWndProc   = (WNDPROC)wxWndProc;
    wndclass.hInstance     = wxhInstance;
    wndclass.hCursor       = ::LoadCursor((HINSTANCE)NULL, IDC_ARROW);

    // register the class for all normal windows
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszClassName = wxCanvasClassName;
    wndclass.style         = styleNormal;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(frame)"));
    }

    // "no redraw" frame
    wndclass.lpszClassName = wxCanvasClassNameNR;
    wndclass.style         = styleNoRedraw;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(no redraw frame)"));
    }

    // Register the MDI frame window class.
    wndclass.hbrBackground = (HBRUSH)NULL; // paint MDI frame ourselves
    wndclass.lpszClassName = wxMDIFrameClassName;
    wndclass.style         = styleNormal;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(MDI parent)"));
    }

    // "no redraw" MDI frame
    wndclass.lpszClassName = wxMDIFrameClassNameNoRedraw;
    wndclass.style         = styleNoRedraw;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(no redraw MDI parent frame)"));
    }

    // Register the MDI child frame window class.
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszClassName = wxMDIChildFrameClassName;
    wndclass.style         = styleNormal;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(MDI child)"));
    }

    // "no redraw" MDI child frame
    wndclass.lpszClassName = wxMDIChildFrameClassNameNoRedraw;
    wndclass.style         = styleNoRedraw;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(no redraw MDI child)"));
    }

    return true;
}

// ---------------------------------------------------------------------------
// UnregisterWindowClasses
// ---------------------------------------------------------------------------

bool wxApp::UnregisterWindowClasses()
{
    bool retval = true;

#ifndef __WXMICROWIN__
    // MDI frame window class.
    if ( !::UnregisterClass(wxMDIFrameClassName, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(MDI parent)"));

        retval = false;
    }

    // "no redraw" MDI frame
    if ( !::UnregisterClass(wxMDIFrameClassNameNoRedraw, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(no redraw MDI parent frame)"));

        retval = false;
    }

    // MDI child frame window class.
    if ( !::UnregisterClass(wxMDIChildFrameClassName, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(MDI child)"));

        retval = false;
    }

    // "no redraw" MDI child frame
    if ( !::UnregisterClass(wxMDIChildFrameClassNameNoRedraw, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(no redraw MDI child)"));

        retval = false;
    }

    // canvas class name
    if ( !::UnregisterClass(wxCanvasClassName, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(canvas)"));

        retval = false;
    }

    if ( !::UnregisterClass(wxCanvasClassNameNR, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(no redraw canvas)"));

        retval = false;
    }
#endif // __WXMICROWIN__

    return retval;
}

void wxApp::CleanUp()
{
    // all objects pending for deletion must be deleted first, otherwise
    // UnregisterWindowClasses() call wouldn't succeed (because windows
    // using the classes being unregistered still exist), so call the base
    // class method first and only then do our clean up
    wxAppBase::CleanUp();

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
    wxSetKeyboardHook(false);
#endif

    wxOleUninitialize();

    // for an EXE the classes are unregistered when it terminates but DLL may
    // be loaded several times (load/unload/load) into the same process in
    // which case the registration will fail after the first time if we don't
    // unregister the classes now
    UnregisterWindowClasses();

#ifdef __WXWINCE__
    free( wxCanvasClassName );
    free( wxCanvasClassNameNR );
#endif
}

// ----------------------------------------------------------------------------
// wxApp ctor/dtor
// ----------------------------------------------------------------------------

wxApp::wxApp()
{
    m_printMode = wxPRINT_WINDOWS;
}

wxApp::~wxApp()
{
}

// ----------------------------------------------------------------------------
// wxApp idle handling
// ----------------------------------------------------------------------------

void wxApp::OnIdle(wxIdleEvent& WXUNUSED(event))
{
#if wxUSE_DC_CACHEING
    // automated DC cache management: clear the cached DCs and bitmap
    // if it's likely that the app has finished with them, that is, we
    // get an idle event and we're not dragging anything.
    if (!::GetKeyState(MK_LBUTTON) && !::GetKeyState(MK_MBUTTON) && !::GetKeyState(MK_RBUTTON))
        wxMSWDCImpl::ClearCache();
#endif // wxUSE_DC_CACHEING
}

void wxApp::WakeUpIdle()
{
    // Send the top window a dummy message so idle handler processing will
    // start up again.  Doing it this way ensures that the idle handler
    // wakes up in the right thread (see also wxWakeUpMainThread() which does
    // the same for the main app thread only)
    wxWindow *topWindow = wxTheApp->GetTopWindow();
    if ( topWindow )
    {
        if ( !::PostMessage(GetHwndOf(topWindow), WM_NULL, 0, 0) )
        {
            // should never happen
            wxLogLastError(wxT("PostMessage(WM_NULL)"));
        }
    }
}

// ----------------------------------------------------------------------------
// other wxApp event hanlders
// ----------------------------------------------------------------------------

void wxApp::OnEndSession(wxCloseEvent& WXUNUSED(event))
{
    if (GetTopWindow())
        GetTopWindow()->Close(true);
}

// Default behaviour: close the application with prompts. The
// user can veto the close, and therefore the end session.
void wxApp::OnQueryEndSession(wxCloseEvent& event)
{
    if (GetTopWindow())
    {
        if (!GetTopWindow()->Close(!event.CanVeto()))
            event.Veto(true);
    }
}

// ----------------------------------------------------------------------------
// system DLL versions
// ----------------------------------------------------------------------------

// these functions have trivial inline implementations for CE
#ifndef __WXWINCE__

#if wxUSE_DYNLIB_CLASS

namespace
{

// helper function: retrieve the DLL version by using DllGetVersion(), returns
// 0 if the DLL doesn't export such function
int CallDllGetVersion(wxDynamicLibrary& dll)
{
    // now check if the function is available during run-time
    wxDYNLIB_FUNCTION( DLLGETVERSIONPROC, DllGetVersion, dll );
    if ( !pfnDllGetVersion )
        return 0;

    DLLVERSIONINFO dvi;
    dvi.cbSize = sizeof(dvi);

    HRESULT hr = (*pfnDllGetVersion)(&dvi);
    if ( FAILED(hr) )
    {
        wxLogApiError(_T("DllGetVersion"), hr);

        return 0;
    }

    return 100*dvi.dwMajorVersion + dvi.dwMinorVersion;
}

} // anonymous namespace

/* static */
int wxApp::GetComCtl32Version()
{
    // cache the result
    //
    // NB: this is MT-ok as in the worst case we'd compute s_verComCtl32 twice,
    //     but as its value should be the same both times it doesn't matter
    static int s_verComCtl32 = -1;

    if ( s_verComCtl32 == -1 )
    {
        // we're prepared to handle the errors
        wxLogNull noLog;

        // the DLL should really be available
        wxDynamicLibrary dllComCtl32(_T("comctl32.dll"), wxDL_VERBATIM);
        if ( !dllComCtl32.IsLoaded() )
        {
            s_verComCtl32 = 0;
            return 0;
        }

        // try DllGetVersion() for recent DLLs
        s_verComCtl32 = CallDllGetVersion(dllComCtl32);

        // if DllGetVersion() is unavailable either during compile or
        // run-time, try to guess the version otherwise
        if ( !s_verComCtl32 )
        {
            // InitCommonControlsEx is unique to 4.70 and later
            void *pfn = dllComCtl32.GetSymbol(_T("InitCommonControlsEx"));
            if ( !pfn )
            {
                // not found, must be 4.00
                s_verComCtl32 = 400;
            }
            else // 4.70+
            {
                // many symbols appeared in comctl32 4.71, could use any of
                // them except may be DllInstall()
                pfn = dllComCtl32.GetSymbol(_T("InitializeFlatSB"));
                if ( !pfn )
                {
                    // not found, must be 4.70
                    s_verComCtl32 = 470;
                }
                else
                {
                    // found, must be 4.71 or later
                    s_verComCtl32 = 471;
                }
            }
        }
    }

    return s_verComCtl32;
}

/* static */
int wxApp::GetShell32Version()
{
    static int s_verShell32 = -1;
    if ( s_verShell32 == -1 )
    {
        // we're prepared to handle the errors
        wxLogNull noLog;

        wxDynamicLibrary dllShell32(_T("shell32.dll"), wxDL_VERBATIM);
        if ( dllShell32.IsLoaded() )
        {
            s_verShell32 = CallDllGetVersion(dllShell32);

            if ( !s_verShell32 )
            {
                // there doesn't seem to be any way to distinguish between 4.00
                // and 4.70 (starting from 4.71 we have DllGetVersion()) so
                // just assume it is 4.0
                s_verShell32 = 400;
            }
        }
        else // failed load the DLL?
        {
            s_verShell32 = 0;
        }
    }

    return s_verShell32;
}

#else // !wxUSE_DYNLIB_CLASS

/* static */
int wxApp::GetComCtl32Version()
{
    return 0;
}

/* static */
int wxApp::GetShell32Version()
{
    return 0;
}

#endif // wxUSE_DYNLIB_CLASS/!wxUSE_DYNLIB_CLASS

#endif // !__WXWINCE__

// ----------------------------------------------------------------------------
// Yield to incoming messages
// ----------------------------------------------------------------------------

bool wxApp::Yield(bool onlyIfNeeded)
{
    // MT-FIXME
    static bool s_inYield = false;

#if wxUSE_LOG
    // disable log flushing from here because a call to wxYield() shouldn't
    // normally result in message boxes popping up &c
    wxLog::Suspend();
#endif // wxUSE_LOG

    if ( s_inYield )
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return false;
    }

    s_inYield = true;

    // we don't want to process WM_QUIT from here - it should be processed in
    // the main event loop in order to stop it
    MSG msg;
    while ( PeekMessage(&msg, (HWND)0, 0, 0, PM_NOREMOVE) &&
            msg.message != WM_QUIT )
    {
#if wxUSE_THREADS
        wxMutexGuiLeaveOrEnter();
#endif // wxUSE_THREADS

        if ( !wxTheApp->Dispatch() )
            break;
    }

    // if there are pending events, we must process them.
    ProcessPendingEvents();

#if wxUSE_LOG
    // let the logs be flashed again
    wxLog::Resume();
#endif // wxUSE_LOG

    s_inYield = false;

    return true;
}

#if wxUSE_EXCEPTIONS

// ----------------------------------------------------------------------------
// exception handling
// ----------------------------------------------------------------------------

bool wxApp::OnExceptionInMainLoop()
{
    // ask the user about what to do: use the Win32 API function here as it
    // could be dangerous to use any wxWidgets code in this state
    switch (
            ::MessageBox
              (
                NULL,
                _T("An unhandled exception occurred. Press \"Abort\" to \
terminate the program,\r\n\
\"Retry\" to exit the program normally and \"Ignore\" to try to continue."),
                _T("Unhandled exception"),
                MB_ABORTRETRYIGNORE |
                MB_ICONERROR|
                MB_TASKMODAL
              )
           )
    {
        case IDABORT:
            throw;

        default:
            wxFAIL_MSG( _T("unexpected MessageBox() return code") );
            // fall through

        case IDRETRY:
            return false;

        case IDIGNORE:
            return true;
    }
}

#endif // wxUSE_EXCEPTIONS
