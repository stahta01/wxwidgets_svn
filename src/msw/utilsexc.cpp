/////////////////////////////////////////////////////////////////////////////
// Name:        msw/utilsexec.cpp
// Purpose:     Various utilities
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/app.h"
    #include "wx/intl.h"
#endif

#include "wx/log.h"

#ifdef __WIN32__
    #include "wx/process.h"
#endif

#include "wx/msw/private.h"

#include <ctype.h>

#if !defined(__GNUWIN32__) && !defined(__WXWINE__) && !defined(__SALFORDC__)
    #include <direct.h>
#ifndef __MWERKS__
    #include <dos.h>
#endif
#endif

#if defined(__GNUWIN32__) && !defined(__TWIN32__)
    #include <sys/unistd.h>
    #include <sys/stat.h>
#endif

#if defined(__WIN32__) && !defined(__WXWINE__)
#include <io.h>

#ifndef __GNUWIN32__
#include <shellapi.h>
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __WATCOMC__
    #if !(defined(_MSC_VER) && (_MSC_VER > 800))
        #include <errno.h>
    #endif
#endif
#include <stdarg.h>

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// this message is sent when the process we're waiting for terminates
#define wxWM_PROC_TERMINATED (WM_USER + 10000)

// ----------------------------------------------------------------------------
// this module globals
// ----------------------------------------------------------------------------

// we need to create a hidden window to receive the process termination
// notifications and for this we need a (Win) class name for it which we will
// register the first time it's needed
static const wxChar *gs_classForHiddenWindow = NULL;

// ----------------------------------------------------------------------------
// private types
// ----------------------------------------------------------------------------

// structure describing the process we're being waiting for
struct wxExecuteData
{
public:
    ~wxExecuteData()
    {
#ifndef __WIN16__
        if ( !::CloseHandle(hProcess) )
        {
            wxLogLastError("CloseHandle(hProcess)");
        }
#endif
    }

    HWND       hWnd;          // window to send wxWM_PROC_TERMINATED to
    HANDLE     hProcess;      // handle of the process
    DWORD      dwProcessId;   // pid of the process
    wxProcess *handler;
    DWORD      dwExitCode;    // the exit code of the process
    bool       state;         // set to FALSE when the process finishes
};

// ============================================================================
// implementation
// ============================================================================

#ifdef __WIN32__
static DWORD wxExecuteThread(wxExecuteData *data)
{
    WaitForSingleObject(data->hProcess, INFINITE);

    // get the exit code
    if ( !GetExitCodeProcess(data->hProcess, &data->dwExitCode) )
    {
        wxLogLastError("GetExitCodeProcess");
    }

    wxASSERT_MSG( data->dwExitCode != STILL_ACTIVE,
                  wxT("process should have terminated") );

    // send a message indicating process termination to the window
    SendMessage(data->hWnd, wxWM_PROC_TERMINATED, 0, (LPARAM)data);

    return 0;
}

// window procedure of a hidden window which is created just to receive
// the notification message when a process exits
LRESULT APIENTRY _EXPORT wxExecuteWindowCbk(HWND hWnd, UINT message,
                                            WPARAM wParam, LPARAM lParam)
{
    if ( message == wxWM_PROC_TERMINATED )
    {
        DestroyWindow(hWnd);    // we don't need it any more

        wxExecuteData *data = (wxExecuteData *)lParam;
        if ( data->handler )
        {
            data->handler->OnTerminate((int)data->dwProcessId,
                                       (int)data->dwExitCode);
        }

        if ( data->state )
        {
            // we're executing synchronously, tell the waiting thread
            // that the process finished
            data->state = 0;
        }
        else
        {
            // asynchronous execution - we should do the clean up
            delete data;
        }

        return 0;
    }
    else
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
#endif

long wxExecute(const wxString& command, bool sync, wxProcess *handler)
{
    wxCHECK_MSG( !!command, 0, wxT("empty command in wxExecute") );

#if defined(__WIN32__) && !defined(__TWIN32__)
    // the old code is disabled because we really need a process handle
    // if we want to execute it asynchronously or even just get its
    // return code and for this we must use CreateProcess() and not
    // ShellExecute()
#if 0
    // isolate command and arguments
    wxString commandName;
    bool insideQuotes = FALSE;
    const char *pc;
    for ( pc = command.c_str(); *pc != '\0'; pc++ )
    {
        switch ( *pc )
        {
            case ' ':
            case '\t':
                if ( !insideQuotes )
                    break;
                // fall through

            case '"':
                insideQuotes = !insideQuotes;
                // fall through

            default:
                commandName += *pc;
                continue;   // skip the next break
        }

        // only reached for space not inside quotes
        break;
    }

    wxString commandArgs = pc;

    wxWindow *winTop = wxTheApp->GetTopWindow();
    HWND hwndTop = (HWND)(winTop ? winTop->GetHWND() : 0);

    HANDLE result;
#ifdef __GNUWIN32__
    result = ShellExecute(hwndTop,
                          (const wchar_t)"open",
                          (const wchar_t)commandName,
                          (const wchar_t)commandArgs,
                          (const wchar_t)NULL,
                          SW_SHOWNORMAL);
#else // !GNUWIN32
    result = ShellExecute(hwndTop, "open", commandName,
                          commandArgs, NULL, SW_SHOWNORMAL);
#endif // GNUWIN32

    if ( ((long)result) <= 32 )
        wxLogSysError(_("Can't execute command '%s'"), command.c_str());

    return result;
#else // 1
    // create the process
    STARTUPINFO si;
    wxZeroMemory(si);

    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;

    if ( ::CreateProcess(
                         NULL,       // application name (use only cmd line)
                         (wxChar *)command.c_str(),  // full command line
                         NULL,       // security attributes: defaults for both
                         NULL,       //   the process and its main thread
                         FALSE,      // don't inherit handles
                         CREATE_DEFAULT_ERROR_MODE |
                         CREATE_SUSPENDED,           // flags
                         NULL,       // environment (use the same)
                         NULL,       // current directory (use the same)
                         &si,        // startup info (unused here)
                         &pi         // process info
                        ) == 0 )
    {
        wxLogSysError(_("Execution of command '%s' failed"), command.c_str());

        return 0;
    }

    // register the class for the hidden window used for the notifications
    if ( !gs_classForHiddenWindow )
    {
        gs_classForHiddenWindow = _T("wxHiddenWindow");

        WNDCLASS wndclass;
        wxZeroMemory(wndclass);
        wndclass.lpfnWndProc   = (WNDPROC)wxExecuteWindowCbk;
        wndclass.hInstance     = wxGetInstance();
        wndclass.lpszClassName = gs_classForHiddenWindow;

        if ( !::RegisterClass(&wndclass) )
        {
            wxLogLastError("RegisterClass(hidden window)");
        }
    }

    // create a hidden window to receive notification about process
    // termination
    HWND hwnd = ::CreateWindow(gs_classForHiddenWindow, NULL,
                               WS_OVERLAPPEDWINDOW,
                               0, 0, 0, 0, NULL,
                               (HMENU)NULL, wxGetInstance(), 0);
    wxASSERT_MSG( hwnd, wxT("can't create a hidden window for wxExecute") );

    // Alloc data
    wxExecuteData *data = new wxExecuteData;
    data->hProcess    = pi.hProcess;
    data->dwProcessId = pi.dwProcessId;
    data->hWnd        = hwnd;
    data->state       = sync;
    if ( sync )
    {
        wxASSERT_MSG( !handler, wxT("wxProcess param ignored for sync execution") );

        data->handler = NULL;
    }
    else
    {
        // may be NULL or not
        data->handler = handler;
    }

    DWORD tid;
    HANDLE hThread = ::CreateThread(NULL,
                                    0,
                                    (LPTHREAD_START_ROUTINE)wxExecuteThread,
                                    (void *)data,
                                    0,
                                    &tid);

    // resume process we created now - whether the thread creation succeeded or
    // not
    if ( ::ResumeThread(pi.hThread) == (DWORD)-1 )
    {
        // ignore it - what can we do?
        wxLogLastError("ResumeThread in wxExecute");
    }

    // close unneeded handle
    if ( !::CloseHandle(pi.hThread) )
        wxLogLastError("CloseHandle(hThread)");

    if ( !hThread )
    {
        wxLogLastError("CreateThread in wxExecute");

        DestroyWindow(hwnd);
        delete data;

        // the process still started up successfully...
        return pi.dwProcessId;
    }

    if ( !sync )
    {
        // clean up will be done when the process terminates

        // return the pid
        return pi.dwProcessId;
    }

    // waiting until command executed (disable everything while doing it)
#if wxUSE_GUI
    wxBeginBusyCursor();
    wxEnableTopLevelWindows(FALSE);
#endif // wxUSE_GUI

    while ( data->state )
        wxYield();

#if wxUSE_GUI
    wxEnableTopLevelWindows(TRUE);
    wxEndBusyCursor();
#endif // wxUSE_GUI

    DWORD dwExitCode = data->dwExitCode;
    delete data;

    // return the exit code
    return dwExitCode;
#endif // 0/1
#else // Win16
    long instanceID = WinExec((LPCSTR) WXSTRINGCAST command, SW_SHOW);
    if (instanceID < 32) return(0);

    if (sync) {
        int running;
        do {
            wxYield();
            running = GetModuleUsage((HINSTANCE)instanceID);
        } while (running);
    }

    return(instanceID);
#endif // Win16/32
}

long wxExecute(char **argv, bool sync, wxProcess *handler)
{
    wxString command;

    while ( *argv != NULL )
    {
        command << *argv++ << ' ';
    }

    command.RemoveLast();

    return wxExecute(command, sync, handler);
}

// ----------------------------------------------------------------------------
// Metafile helpers
// ----------------------------------------------------------------------------

extern void PixelToHIMETRIC(LONG *x, LONG *y)
{
    ScreenHDC hdcRef;

    int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE),
        iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE),
        iWidthPels = GetDeviceCaps(hdcRef, HORZRES),
        iHeightPels = GetDeviceCaps(hdcRef, VERTRES);

    *x *= (iWidthMM * 100);
    *x /= iWidthPels;
    *y *= (iHeightMM * 100);
    *y /= iHeightPels;
}

extern void HIMETRICToPixel(LONG *x, LONG *y)
{
    ScreenHDC hdcRef;

    int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE),
        iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE),
        iWidthPels = GetDeviceCaps(hdcRef, HORZRES),
        iHeightPels = GetDeviceCaps(hdcRef, VERTRES);

    *x *= iWidthPels;
    *x /= (iWidthMM * 100);
    *y *= iHeightPels;
    *y /= (iHeightMM * 100);
}
