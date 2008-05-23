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
WXDLLIMPEXP_CORE const wxChar *wxCanvasClassName = NULL;
WXDLLIMPEXP_CORE const wxChar *wxCanvasClassNameNR = NULL;
#endif
WXDLLIMPEXP_CORE const wxChar *wxMDIFrameClassName = NULL;
WXDLLIMPEXP_CORE const wxChar *wxMDIFrameClassNameNoRedraw = NULL;
WXDLLIMPEXP_CORE const wxChar *wxMDIChildFrameClassName = NULL;
WXDLLIMPEXP_CORE const wxChar *wxMDIChildFrameClassNameNoRedraw = NULL;

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

// ---------------------------------------------------------------------------
// Stuff for using console from the GUI applications
// ---------------------------------------------------------------------------

#ifndef __WXWINCE__

#include <wx/dynlib.h>

namespace
{

/*
    Helper class to manipulate console from a GUI app.

    Notice that console output is available in the GUI app only if:
    - AttachConsole() returns TRUE (which means it never works under pre-XP)
    - we have a valid STD_ERROR_HANDLE
    - command history hasn't been changed since our startup

    To check if all these conditions are verified, you need to simple call
    IsOkToUse(). It will check the first two conditions above the first time it
    is called (and if this fails, the subsequent calls will return immediately)
    and also recheck the last one every time it is called.
 */
class wxConsoleStderr
{
public:
    // default ctor does nothing, call Init() before using this class
    wxConsoleStderr()
    {
        m_hStderr = INVALID_HANDLE_VALUE;
        m_historyLen =
        m_dataLen =
        m_dataLine = 0;

        m_ok = -1;
    }

    ~wxConsoleStderr()
    {
        if ( m_hStderr != INVALID_HANDLE_VALUE )
        {
            if ( !::FreeConsole() )
            {
                wxLogLastError(_T("FreeConsole"));
            }
        }
    }

    // return true if we were successfully initialized and there had been no
    // console activity which would interfere with our output since then
    bool IsOkToUse() const
    {
        if ( m_ok == -1 )
        {
            wxConsoleStderr * const self = wx_const_cast(wxConsoleStderr *, this);
            self->m_ok = self->DoInit();

            // no need to call IsHistoryUnchanged() as we just initialized
            // m_history anyhow
            return m_ok == 1;
        }

        return m_ok && IsHistoryUnchanged();
    }


    // output the provided text on the console, return true if ok
    bool Write(const wxString& text);

private:
    // called by Init() once only to do the real initialization
    bool DoInit();

    // retrieve the command line history into the provided buffer and return
    // its length
    int GetCommandHistory(wxWxCharBuffer& buf) const;

    // check if the console history has changed
    bool IsHistoryUnchanged() const;

    int m_ok;                   // initially -1, set to true or false by Init()

    wxDynamicLibrary m_dllKernel32;

    HANDLE m_hStderr;           // console handle, if it's valid we must call
                                // FreeConsole() (even if m_ok != 1)

    wxWxCharBuffer m_history;   // command history on startup
    int m_historyLen;           // length command history buffer

    wxCharBuffer m_data;        // data between empty line and cursor position
    int m_dataLen;              // length data buffer
    int m_dataLine;             // line offset

    typedef DWORD (WINAPI *GetConsoleCommandHistory_t)(LPTSTR sCommands,
                                                       DWORD nBufferLength,
                                                       LPCTSTR sExeName);
    typedef DWORD (WINAPI *GetConsoleCommandHistoryLength_t)(LPCTSTR sExeName);

    GetConsoleCommandHistory_t m_pfnGetConsoleCommandHistory;
    GetConsoleCommandHistoryLength_t m_pfnGetConsoleCommandHistoryLength;

    DECLARE_NO_COPY_CLASS(wxConsoleStderr)
};

bool wxConsoleStderr::DoInit()
{
    HANDLE hStderr = ::GetStdHandle(STD_ERROR_HANDLE);

    if ( hStderr == INVALID_HANDLE_VALUE || !hStderr )
        return false;

    if ( !m_dllKernel32.Load(_T("kernel32.dll")) )
        return false;

    typedef BOOL (WINAPI *AttachConsole_t)(DWORD dwProcessId);
    AttachConsole_t wxDL_INIT_FUNC(pfn, AttachConsole, m_dllKernel32);

    if ( !pfnAttachConsole || !pfnAttachConsole(ATTACH_PARENT_PROCESS) )
        return false;

    // console attached, set m_hStderr now to ensure that we free it in the
    // dtor
    m_hStderr = hStderr;

    wxDL_INIT_FUNC_AW(m_pfn, GetConsoleCommandHistory, m_dllKernel32);
    if ( !m_pfnGetConsoleCommandHistory )
        return false;

    wxDL_INIT_FUNC_AW(m_pfn, GetConsoleCommandHistoryLength, m_dllKernel32);
    if ( !m_pfnGetConsoleCommandHistoryLength )
        return false;

    // remember the current command history to be able to compare with it later
    // in IsHistoryUnchanged()
    m_historyLen = GetCommandHistory(m_history);
    if ( !m_history )
        return false;


    // now find the first blank line above the current position
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if ( !::GetConsoleScreenBufferInfo(m_hStderr, &csbi) )
    {
        wxLogLastError(_T("GetConsoleScreenBufferInfo"));
        return false;
    }

    COORD pos;
    pos.X = 0;
    pos.Y = csbi.dwCursorPosition.Y + 1;

    // we decide that a line is empty if first 4 characters are spaces
    DWORD ret;
    char buf[4];
    do
    {
        pos.Y--;
        if ( !::ReadConsoleOutputCharacterA(m_hStderr, buf, WXSIZEOF(buf),
                                            pos, &ret) )
        {
            wxLogLastError(_T("ReadConsoleOutputCharacterA"));
            return false;
        }
    } while ( wxStrncmp("    ", buf, WXSIZEOF(buf)) != 0 );

    // calculate line offset and length of data
    m_dataLine = csbi.dwCursorPosition.Y - pos.Y;
    m_dataLen = m_dataLine*csbi.dwMaximumWindowSize.X + csbi.dwCursorPosition.X;

    if ( m_dataLen > 0 )
    {
        m_data.extend(m_dataLen);
        if ( !::ReadConsoleOutputCharacterA(m_hStderr, m_data.data(), m_dataLen,
                                            pos, &ret) )
        {
            wxLogLastError(_T("ReadConsoleOutputCharacterA"));
            return false;
        }
    }

    return true;
}

int wxConsoleStderr::GetCommandHistory(wxWxCharBuffer& buf) const
{
    // these functions are internal and may only be called by cmd.exe
    static const wxChar *CMD_EXE = _T("cmd.exe");

    const int len = m_pfnGetConsoleCommandHistoryLength(CMD_EXE);
    if ( len )
    {
        buf.extend(len);
        const int len2 = m_pfnGetConsoleCommandHistory(buf.data(), len, CMD_EXE);
        wxASSERT_MSG( len2 == len, _T("failed getting history?") );
    }

    return len;
}

bool wxConsoleStderr::IsHistoryUnchanged() const
{
    wxASSERT_MSG( m_ok == 1, _T("shouldn't be called if not initialized") );

    // get (possibly changed) command history
    wxWxCharBuffer history;
    const int historyLen = GetCommandHistory(history);

    // and compare it with the original one
    return historyLen == m_historyLen && history &&
                memcmp(m_history, history, historyLen) == 0;
}

bool wxConsoleStderr::Write(const wxString& text)
{
    wxASSERT_MSG( m_hStderr != INVALID_HANDLE_VALUE,
                    _T("should only be called if Init() returned true") );

    // get current position
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( !::GetConsoleScreenBufferInfo(m_hStderr, &csbi) )
    {
        wxLogLastError(_T("GetConsoleScreenBufferInfo"));
        return false;
    }

    // and calculate new position (where is empty line)
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y -= m_dataLine;

    if ( !::SetConsoleCursorPosition(m_hStderr, csbi.dwCursorPosition) )
    {
        wxLogLastError(_T("SetConsoleCursorPosition"));
        return false;
    }

    DWORD ret;
    if ( !::FillConsoleOutputCharacter(m_hStderr, _T(' '), m_dataLen,
                                       csbi.dwCursorPosition, &ret) )
    {
        wxLogLastError(_T("FillConsoleOutputCharacter"));
        return false;
    }

    if ( !::WriteConsole(m_hStderr, text.wx_str(), text.length(), &ret, NULL) )
    {
        wxLogLastError(_T("WriteConsole"));
        return false;
    }

    WriteConsoleA(m_hStderr, m_data, m_dataLen, &ret, 0);

    return true;
}

wxConsoleStderr s_consoleStderr;

} // anonymous namespace

bool wxGUIAppTraits::CanUseStderr()
{
    return s_consoleStderr.IsOkToUse();
}

bool wxGUIAppTraits::WriteToStderr(const wxString& text)
{
    return s_consoleStderr.IsOkToUse() && s_consoleStderr.Write(text);
}

#endif // !__WXWINCE__

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

// This function registers the given class name and stores a pointer to a
// heap-allocated copy of it at the specified location, it must be deleted
// later.
static void RegisterAndStoreClassName(const wxString& uniqueClassName,
                                      const wxChar **className,
                                      WNDCLASS *lpWndClass)
{
    const size_t length = uniqueClassName.length() + 1; // for trailing NUL
    wxChar *newChars = new wxChar[length];
    wxStrncpy(newChars, uniqueClassName, length);
    *className = newChars;
    lpWndClass->lpszClassName = *className;

    if ( !::RegisterClass(lpWndClass) )
    {
        wxLogLastError(wxString::Format(wxT("RegisterClass(%s)"), newChars));
    }
}

// This function registers the class defined by the provided WNDCLASS struct
// contents using a unique name constructed from the specified base name and
// and a suffix unique to this library instance. It also stores the generated
// unique names for normal and "no redraw" versions of the class in the
// provided variables, caller must delete their contents later.
static void RegisterClassWithUniqueNames(const wxString& baseName,
                                         const wxChar **className,
                                         const wxChar **classNameNR,
                                         WNDCLASS *lpWndClass)
{
    // for each class we register one with CS_(V|H)REDRAW style and one
    // without for windows created with wxNO_FULL_REDRAW_ON_REPAINT flag
    static const long styleNormal = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    static const long styleNoRedraw = CS_DBLCLKS;

    const wxString uniqueSuffix(wxString::Format(wxT("@%p"), className));

    wxString uniqueClassName(baseName + uniqueSuffix);
    lpWndClass->style = styleNormal;
    RegisterAndStoreClassName(uniqueClassName, className, lpWndClass);

    // NB: remember that code elsewhere supposes that no redraw class names
    //     use the same names as normal classes with "NR" suffix so we must put
    //     "NR" at the end instead of using more natural baseName+"NR"+suffix
    wxString uniqueClassNameNR(uniqueClassName + wxT("NR"));
    lpWndClass->style = styleNoRedraw;
    RegisterAndStoreClassName(uniqueClassNameNR, classNameNR, lpWndClass);
}

// TODO we should only register classes really used by the app. For this it
//      would be enough to just delay the class registration until an attempt
//      to create a window of this class is made.
bool wxApp::RegisterWindowClasses()
{
    WNDCLASS wndclass;
    wxZeroMemory(wndclass);

    // the fields which are common to all classes
    wndclass.lpfnWndProc   = (WNDPROC)wxWndProc;
    wndclass.hInstance     = wxhInstance;
    wndclass.hCursor       = ::LoadCursor((HINSTANCE)NULL, IDC_ARROW);

    // register the class for all normal windows and "no redraw" frames
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    RegisterClassWithUniqueNames(wxT("wxWindowClass"),
                                 &wxCanvasClassName,
                                 &wxCanvasClassNameNR,
                                 &wndclass);

    // Register the MDI frame window class and "no redraw" MDI frame
    wndclass.hbrBackground = (HBRUSH)NULL; // paint MDI frame ourselves
    RegisterClassWithUniqueNames(wxT("wxMDIFrameClass"),
                                 &wxMDIFrameClassName,
                                 &wxMDIFrameClassNameNoRedraw,
                                 &wndclass);

    // Register the MDI child frame window class and "no redraw" MDI child frame
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassWithUniqueNames(wxT("wxMDIChildFrameClass"),
                                 &wxMDIChildFrameClassName,
                                 &wxMDIChildFrameClassNameNoRedraw,
                                 &wndclass);

    return true;
}

// ---------------------------------------------------------------------------
// UnregisterWindowClasses
// ---------------------------------------------------------------------------

// This function unregisters the class with the given name and frees memory
// allocated for it by RegisterAndStoreClassName().
static bool UnregisterAndFreeClassName(const wxChar **ppClassName)
{
    bool retval = true;

    if ( !::UnregisterClass(*ppClassName, wxhInstance) )
    {
        wxLogLastError(
                wxString::Format(wxT("UnregisterClass(%s)"), *ppClassName));

        retval = false;
    }

    delete [] (wxChar*) *ppClassName;
    *ppClassName = NULL;

    return retval;
}

bool wxApp::UnregisterWindowClasses()
{
    bool retval = true;

#ifndef __WXMICROWIN__
    if ( !UnregisterAndFreeClassName(&wxMDIFrameClassName) )
        retval = false;

    if ( !UnregisterAndFreeClassName(&wxMDIFrameClassNameNoRedraw) )
        retval = false;

    if ( !UnregisterAndFreeClassName(&wxMDIChildFrameClassName) )
        retval = false;

    if ( !UnregisterAndFreeClassName(&wxMDIChildFrameClassNameNoRedraw) )
        retval = false;

    if ( !UnregisterAndFreeClassName(&wxCanvasClassName) )
        retval = false;

    if ( !UnregisterAndFreeClassName(&wxCanvasClassNameNR) )
        retval = false;
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
    wxWindow * const topWindow = wxTheApp->GetTopWindow();
    if ( topWindow )
    {
        HWND hwndTop = GetHwndOf(topWindow);

        // Do not post WM_NULL if there's already a pending WM_NULL to avoid
        // overflowing the message queue.
        //
        // Notice that due to a limitation of PeekMessage() API (which handles
        // 0,0 range specially), we have to check the range from 0-1 instead.
        // This still makes it possible to overflow the queue with WM_NULLs by
        // interspersing the calles to WakeUpIdle() with windows creation but
        // it should be rather hard to do it accidentally.
        MSG msg;
        if ( !::PeekMessage(&msg, hwndTop, 0, 1, PM_NOREMOVE) ||
              ::PeekMessage(&msg, hwndTop, 1, 1, PM_NOREMOVE) )
        {
            if ( !::PostMessage(hwndTop, WM_NULL, 0, 0) )
            {
                // should never happen
                wxLogLastError(wxT("PostMessage(WM_NULL)"));
            }
        }
    }
}

// ----------------------------------------------------------------------------
// other wxApp event hanlders
// ----------------------------------------------------------------------------

void wxApp::OnEndSession(wxCloseEvent& WXUNUSED(event))
{
    // Windows will terminate the process soon after we return from
    // WM_ENDSESSION handler or when we delete our last window, so make sure we
    // at least execute our cleanup code before

    // prevent the window from being destroyed when the corresponding wxTLW is
    // destroyed: this will result in a leak of a HWND, of course, but who
    // cares when the process is being killed anyhow
    if ( !wxTopLevelWindows.empty() )
        wxTopLevelWindows[0]->SetHWND(0);

    const int rc = OnExit();

    wxEntryCleanup();

    // calling exit() instead of ExitProcess() or not doing anything at all and
    // being killed by Windows has the advantage of executing the dtors of
    // global objects
    exit(rc);
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
    wxEventLoopGuarantor dummyLoopIfNeeded;
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
