/////////////////////////////////////////////////////////////////////////////
// Name:        msw/utilsexec.cpp
// Purpose:     wxExecute implementation for MSW
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2002 wxWindows dev team
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
    #include "wx/stream.h"
    #include "wx/process.h"
#endif

#include "wx/msw/private.h"

#include <ctype.h>

#if !defined(__GNUWIN32__) && !defined(__WXWINE__) && !defined(__SALFORDC__) && !defined(__WXMICROWIN__)
    #include <direct.h>
#ifndef __MWERKS__
    #include <dos.h>
#endif
#endif

#if defined(__GNUWIN32__) && !defined(__TWIN32__)
    #include <sys/unistd.h>
    #include <sys/stat.h>
#endif

#if defined(__WIN32__) && !defined(__WXWINE__) && !defined(__WXMICROWIN__)
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

#if wxUSE_IPC
    #include "wx/dde.h"         // for WX_DDE hack in wxExecute
#endif // wxUSE_IPC

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
            wxLogLastError(wxT("CloseHandle(hProcess)"));
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

#if defined(__WIN32__) && wxUSE_STREAMS

// ----------------------------------------------------------------------------
// wxPipeStreams
// ----------------------------------------------------------------------------

class wxPipeInputStream: public wxInputStream
{
public:
    wxPipeInputStream(HANDLE hInput);
    virtual ~wxPipeInputStream();

    // returns TRUE if the pipe is still opened
    bool IsOpened() const { return m_hInput != INVALID_HANDLE_VALUE; }

    // returns TRUE if there is any data to be read from the pipe
    bool IsAvailable() const;

protected:
    size_t OnSysRead(void *buffer, size_t len);

protected:
    HANDLE m_hInput;
};

class wxPipeOutputStream: public wxOutputStream
{
public:
    wxPipeOutputStream(HANDLE hOutput);
    virtual ~wxPipeOutputStream();

protected:
    size_t OnSysWrite(const void *buffer, size_t len);

protected:
    HANDLE m_hOutput;
};

// define this to let wxexec.cpp know that we know what we're doing
#define _WX_USED_BY_WXEXECUTE_
#include "../common/execcmn.cpp"

// ----------------------------------------------------------------------------
// wxPipe represents a Win32 anonymous pipe
// ----------------------------------------------------------------------------

class wxPipe
{
public:
    // the symbolic names for the pipe ends
    enum Direction
    {
        Read,
        Write
    };

    // default ctor doesn't do anything
    wxPipe() { m_handles[Read] = m_handles[Write] = INVALID_HANDLE_VALUE; }

    // create the pipe, return TRUE if ok, FALSE on error
    bool Create()
    {
        // default secutiry attributes
        SECURITY_ATTRIBUTES security;

        security.nLength              = sizeof(security);
        security.lpSecurityDescriptor = NULL;
        security.bInheritHandle       = TRUE; // to pass it to the child

        if ( !::CreatePipe(&m_handles[0], &m_handles[1], &security, 0) )
        {
            wxLogSysError(_("Failed to create an anonymous pipe"));

            return FALSE;
        }

        return TRUE;
    }

    // return TRUE if we were created successfully
    bool IsOk() const { return m_handles[Read] != INVALID_HANDLE_VALUE; }

    // return the descriptor for one of the pipe ends
    HANDLE operator[](Direction which) const
    {
        wxASSERT_MSG( which >= 0 && (size_t)which < WXSIZEOF(m_handles),
                      _T("invalid pipe index") );

        return m_handles[which];
    }

    // detach a descriptor, meaning that the pipe dtor won't close it, and
    // return it
    HANDLE Detach(Direction which)
    {
        wxASSERT_MSG( which >= 0 && (size_t)which < WXSIZEOF(m_handles),
                      _T("invalid pipe index") );

        HANDLE handle = m_handles[which];
        m_handles[which] = INVALID_HANDLE_VALUE;

        return handle;
    }

    // close the pipe descriptors
    void Close()
    {
        for ( size_t n = 0; n < WXSIZEOF(m_handles); n++ )
        {
            if ( m_handles[n] != INVALID_HANDLE_VALUE )
            {
                ::CloseHandle(m_handles[n]);
                m_handles[n] = INVALID_HANDLE_VALUE;
            }
        }
    }

    // dtor closes the pipe descriptors
    ~wxPipe() { Close(); }

private:
    HANDLE m_handles[2];
};

#endif // wxUSE_STREAMS

// ============================================================================
// implementation
// ============================================================================

#ifdef __WIN32__

// ----------------------------------------------------------------------------
// process termination detecting support
// ----------------------------------------------------------------------------

// thread function for the thread monitoring the process termination
static DWORD __stdcall wxExecuteThread(void *arg)
{
    wxExecuteData *data = (wxExecuteData*)arg;

    if ( ::WaitForSingleObject(data->hProcess, INFINITE) != WAIT_OBJECT_0 )
    {
        wxLogDebug(_T("Waiting for the process termination failed!"));
    }

    // get the exit code
    if ( !::GetExitCodeProcess(data->hProcess, &data->dwExitCode) )
    {
        wxLogLastError(wxT("GetExitCodeProcess"));
    }

    wxASSERT_MSG( data->dwExitCode != STILL_ACTIVE,
                  wxT("process should have terminated") );

    // send a message indicating process termination to the window
    ::SendMessage(data->hWnd, wxWM_PROC_TERMINATED, 0, (LPARAM)data);

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

// ============================================================================
// implementation of IO redirection support classes
// ============================================================================

#if wxUSE_STREAMS

// ----------------------------------------------------------------------------
// wxPipeInputStreams
// ----------------------------------------------------------------------------

wxPipeInputStream::wxPipeInputStream(HANDLE hInput)
{
    m_hInput = hInput;
}

wxPipeInputStream::~wxPipeInputStream()
{
    if ( m_hInput != INVALID_HANDLE_VALUE )
        ::CloseHandle(m_hInput);
}

bool wxPipeInputStream::IsAvailable() const
{
    // FIXME
#ifdef __WXWINE__
    return FALSE;
#else
    if ( !IsOpened() )
        return FALSE;

    DWORD nAvailable;

    // function name is misleading, it works with anon pipes as well
    DWORD rc = ::PeekNamedPipe
                    (
                      m_hInput,     // handle
                      NULL, 0,      // ptr to buffer and its size
                      NULL,         // [out] bytes read
                      &nAvailable,  // [out] bytes available
                      NULL          // [out] bytes left
                    );

    if ( !rc )
    {
        if ( ::GetLastError() != ERROR_BROKEN_PIPE )
        {
            // unexpected error
            wxLogLastError(_T("PeekNamedPipe"));
        }

        // don't try to continue reading from a pipe if an error occured or if
        // it had been closed
        ::CloseHandle(m_hInput);

        wxConstCast(this, wxPipeInputStream)->m_hInput = INVALID_HANDLE_VALUE;

        return FALSE;
    }

    return nAvailable != 0;
#endif
}

size_t wxPipeInputStream::OnSysRead(void *buffer, size_t len)
{
    // reading from a pipe may block if there is no more data, always check for
    // EOF first
    if ( !IsAvailable() )
    {
        m_lasterror = wxSTREAM_EOF;

        return 0;
    }

    m_lasterror = wxSTREAM_NOERROR;

    DWORD bytesRead;
    if ( !::ReadFile(m_hInput, buffer, len, &bytesRead, NULL) )
    {
        if ( ::GetLastError() == ERROR_BROKEN_PIPE )
            m_lasterror = wxSTREAM_EOF;
        else
            m_lasterror = wxSTREAM_READ_ERROR;
    }

    return bytesRead;
}

// ----------------------------------------------------------------------------
// wxPipeOutputStream
// ----------------------------------------------------------------------------

wxPipeOutputStream::wxPipeOutputStream(HANDLE hOutput)
{
    m_hOutput = hOutput;
}

wxPipeOutputStream::~wxPipeOutputStream()
{
    ::CloseHandle(m_hOutput);
}

size_t wxPipeOutputStream::OnSysWrite(const void *buffer, size_t len)
{
    DWORD bytesRead;

    m_lasterror = wxSTREAM_NOERROR;
    if ( !::WriteFile(m_hOutput, buffer, len, &bytesRead, NULL) )
    {
        if ( ::GetLastError() == ERROR_BROKEN_PIPE )
            m_lasterror = wxSTREAM_EOF;
        else
            m_lasterror = wxSTREAM_READ_ERROR;
    }

    return bytesRead;
}

#endif // wxUSE_STREAMS

#endif // Win32

// ============================================================================
// wxExecute functions family
// ============================================================================

#if wxUSE_IPC

// connect to the given server via DDE and ask it to execute the command
static bool wxExecuteDDE(const wxString& ddeServer,
                         const wxString& ddeTopic,
                         const wxString& ddeCommand)
{
    bool ok;

    wxDDEClient client;
    wxConnectionBase *conn = client.MakeConnection(_T(""),
                                                   ddeServer,
                                                   ddeTopic);
    if ( !conn )
    {
        ok = FALSE;
    }
    else // connected to DDE server
    {
        // the added complication here is that although most
        // programs use XTYP_EXECUTE for their DDE API, some
        // important ones - like IE and other MS stuff - use
        // XTYP_REQUEST!
        //
        // so we try it first and then the other one if it
        // failed
        {
            wxLogNull noErrors;
            ok = conn->Request(ddeCommand) != NULL;
        }

        if ( !ok )
        {
            // now try execute - but show the errors
            ok = conn->Execute(ddeCommand);
        }
    }

    return ok;
}

#endif // wxUSE_IPC

long wxExecute(const wxString& cmd, int flags, wxProcess *handler)
{
    wxCHECK_MSG( !!cmd, 0, wxT("empty command in wxExecute") );

    wxString command;

#if wxUSE_IPC
    // DDE hack: this is really not pretty, but we need to allow this for
    // transparent handling of DDE servers in wxMimeTypesManager. Usually it
    // returns the command which should be run to view/open/... a file of the
    // given type. Sometimes, however, this command just launches the server
    // and an additional DDE request must be made to really open the file. To
    // keep all this well hidden from the application, we allow a special form
    // of command: WX_DDE#<command>#DDE_SERVER#DDE_TOPIC#DDE_COMMAND in which
    // case we execute just <command> and process the rest below
    wxString ddeServer, ddeTopic, ddeCommand;
    static const size_t lenDdePrefix = 7;   // strlen("WX_DDE:")
    if ( cmd.Left(lenDdePrefix) == _T("WX_DDE#") )
    {
        // speed up the concatenations below
        ddeServer.reserve(256);
        ddeTopic.reserve(256);
        ddeCommand.reserve(256);

        const wxChar *p = cmd.c_str() + 7;
        while ( *p && *p != _T('#') )
        {
            command += *p++;
        }

        if ( *p )
        {
            // skip '#'
            p++;
        }
        else
        {
            wxFAIL_MSG(_T("invalid WX_DDE command in wxExecute"));
        }

        while ( *p && *p != _T('#') )
        {
            ddeServer += *p++;
        }

        if ( *p )
        {
            // skip '#'
            p++;
        }
        else
        {
            wxFAIL_MSG(_T("invalid WX_DDE command in wxExecute"));
        }

        while ( *p && *p != _T('#') )
        {
            ddeTopic += *p++;
        }

        if ( *p )
        {
            // skip '#'
            p++;
        }
        else
        {
            wxFAIL_MSG(_T("invalid WX_DDE command in wxExecute"));
        }

        while ( *p )
        {
            ddeCommand += *p++;
        }

        // if we want to just launch the program and not wait for its
        // termination, try to execute DDE command right now, it can succeed if
        // the process is already running - but as it fails if it's not
        // running, suppress any errors it might generate
        if ( !(flags & wxEXEC_SYNC) )
        {
            wxLogNull noErrors;
            if ( wxExecuteDDE(ddeServer, ddeTopic, ddeCommand) )
            {
                // a dummy PID - this is a hack, of course, but it's well worth
                // it as we don't open a new server each time we're called
                // which would be quite bad
                return -1;
            }
        }
    }
    else
#endif // wxUSE_IPC
    {
        // no DDE
        command = cmd;
    }

#if defined(__WIN32__) && !defined(__TWIN32__)

    // the IO redirection is only supported with wxUSE_STREAMS
    BOOL redirect = FALSE;

#if wxUSE_STREAMS
    wxPipe pipeIn, pipeOut, pipeErr;

    // we'll save here the copy of pipeIn[Write]
    HANDLE hpipeStdinWrite = INVALID_HANDLE_VALUE;

    // open the pipes to which child process IO will be redirected if needed
    if ( handler && handler->IsRedirected() )
    {
        // create pipes for redirecting stdin, stdout and stderr
        if ( !pipeIn.Create() || !pipeOut.Create() || !pipeErr.Create() )
        {
            wxLogSysError(_("Failed to redirect the child process IO"));

            // indicate failure: we need to return different error code
            // depending on the sync flag
            return flags & wxEXEC_SYNC ? -1 : 0;
        }

        redirect = TRUE;
    }
#endif // wxUSE_STREAMS

    // create the process
    STARTUPINFO si;
    wxZeroMemory(si);
    si.cb = sizeof(si);

#if wxUSE_STREAMS
    if ( redirect )
    {
        si.dwFlags = STARTF_USESTDHANDLES;

        si.hStdInput = pipeIn[wxPipe::Read];
        si.hStdOutput = pipeOut[wxPipe::Write];
        si.hStdError = pipeErr[wxPipe::Write];

        // when the std IO is redirected, we don't show the (console) process
        // window by default, but this can be overridden by the caller by
        // specifying wxEXEC_NOHIDE flag
        if ( !(flags & wxEXEC_NOHIDE) )
        {
            si.dwFlags |= STARTF_USESHOWWINDOW;
            si.wShowWindow = SW_HIDE;
        }

        // we must duplicate the handle to the write side of stdin pipe to make
        // it non inheritable: indeed, we must close the writing end of pipeIn
        // before launching the child process as otherwise this handle will be
        // inherited by the child which will never close it and so the pipe
        // will never be closed and the child will be left stuck in ReadFile()
        HANDLE pipeInWrite = pipeIn.Detach(wxPipe::Write);
        if ( !::DuplicateHandle
                (
                    ::GetCurrentProcess(),
                    pipeInWrite,
                    ::GetCurrentProcess(),
                    &hpipeStdinWrite,
                    0,                      // desired access: unused here
                    FALSE,                  // not inherited
                    DUPLICATE_SAME_ACCESS   // same access as for src handle
                ) )
        {
            wxLogLastError(_T("DuplicateHandle"));
        }

        ::CloseHandle(pipeInWrite);
    }
#endif // wxUSE_STREAMS

    PROCESS_INFORMATION pi;
    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

    bool ok = ::CreateProcess
                (
                 NULL,              // application name (use only cmd line)
                 (wxChar *)
                 command.c_str(),   // full command line
                 NULL,              // security attributes: defaults for both
                 NULL,              //   the process and its main thread
                 redirect,          // inherit handles if we use pipes
                 dwFlags,           // process creation flags
                 NULL,              // environment (use the same)
                 NULL,              // current directory (use the same)
                 &si,               // startup info (unused here)
                 &pi                // process info
                ) != 0;

#if wxUSE_STREAMS
    // we can close the pipe ends used by child anyhow
    if ( redirect )
    {
        ::CloseHandle(pipeIn.Detach(wxPipe::Read));
        ::CloseHandle(pipeOut.Detach(wxPipe::Write));
        ::CloseHandle(pipeErr.Detach(wxPipe::Write));
    }
#endif // wxUSE_STREAMS

    if ( !ok )
    {
#if wxUSE_STREAMS
        // close the other handles too
        if ( redirect )
        {
            ::CloseHandle(pipeOut.Detach(wxPipe::Read));
            ::CloseHandle(pipeErr.Detach(wxPipe::Read));
        }
#endif // wxUSE_STREAMS

        wxLogSysError(_("Execution of command '%s' failed"), command.c_str());

        return flags & wxEXEC_SYNC ? -1 : 0;
    }

#if wxUSE_STREAMS
    // the input buffer bufOut is connected to stdout, this is why it is
    // called bufOut and not bufIn
    wxStreamTempInputBuffer bufOut,
                            bufErr;

    if ( redirect )
    {
        // We can now initialize the wxStreams
        wxPipeInputStream *
            outStream = new wxPipeInputStream(pipeOut.Detach(wxPipe::Read));
        wxPipeInputStream *
            errStream = new wxPipeInputStream(pipeErr.Detach(wxPipe::Read));
        wxPipeOutputStream *
            inStream = new wxPipeOutputStream(hpipeStdinWrite);

        handler->SetPipeStreams(outStream, inStream, errStream);

        bufOut.Init(outStream);
        bufErr.Init(errStream);
    }
#endif // wxUSE_STREAMS

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
            wxLogLastError(wxT("RegisterClass(hidden window)"));
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
    data->state       = (flags & wxEXEC_SYNC) != 0;
    if ( flags & wxEXEC_SYNC )
    {
        // handler may be !NULL for capturing program output, but we don't use
        // it wxExecuteData struct in this case
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
                                    wxExecuteThread,
                                    (void *)data,
                                    0,
                                    &tid);

    // resume process we created now - whether the thread creation succeeded or
    // not
    if ( ::ResumeThread(pi.hThread) == (DWORD)-1 )
    {
        // ignore it - what can we do?
        wxLogLastError(wxT("ResumeThread in wxExecute"));
    }

    // close unneeded handle
    if ( !::CloseHandle(pi.hThread) )
        wxLogLastError(wxT("CloseHandle(hThread)"));

    if ( !hThread )
    {
        wxLogLastError(wxT("CreateThread in wxExecute"));

        DestroyWindow(hwnd);
        delete data;

        // the process still started up successfully...
        return pi.dwProcessId;
    }

    ::CloseHandle(hThread);

#if wxUSE_IPC
    // second part of DDE hack: now establish the DDE conversation with the
    // just launched process
    if ( !ddeServer.empty() )
    {
        bool ok;

        // give the process the time to init itself
        //
        // we use a very big timeout hoping that WaitForInputIdle() will return
        // much sooner, but not INFINITE just in case the process hangs
        // completely - like this we will regain control sooner or later
        switch ( ::WaitForInputIdle(pi.hProcess, 10000 /* 10 seconds */) )
        {
            default:
                wxFAIL_MSG( _T("unexpected WaitForInputIdle() return code") );
                // fall through

            case -1:
                wxLogLastError(_T("WaitForInputIdle() in wxExecute"));

            case WAIT_TIMEOUT:
                wxLogDebug(_T("Timeout too small in WaitForInputIdle"));

                ok = FALSE;
                break;

            case 0:
                // ok, process ready to accept DDE requests
                ok = wxExecuteDDE(ddeServer, ddeTopic, ddeCommand);
        }
    }
#endif // wxUSE_IPC

    if ( !(flags & wxEXEC_SYNC) )
    {
        // clean up will be done when the process terminates

        // return the pid
        return pi.dwProcessId;
    }

    // waiting until command executed (disable everything while doing it)
#if wxUSE_GUI
    {
        wxBusyCursor bc;

        wxWindowDisabler wd;
#endif // wxUSE_GUI

        // wait until the child process terminates
        while ( data->state )
        {
#if wxUSE_STREAMS
            bufOut.Update();
            bufErr.Update();
#endif // wxUSE_STREAMS

            // don't eat 100% of the CPU -- ugly but anything else requires
            // real async IO which we don't have for the moment
            ::Sleep(50);

            // repaint the GUI
            wxYield();
        }

#if wxUSE_GUI
    }
#endif // wxUSE_GUI

    DWORD dwExitCode = data->dwExitCode;
    delete data;

    // return the exit code
    return dwExitCode;
#else // Win16
    long instanceID = WinExec((LPCSTR) WXSTRINGCAST command, SW_SHOW);
    if (instanceID < 32)
        return flags & wxEXEC_SYNC ? -1 : 0;

    if ( flags & wxEXEC_SYNC )
    {
        int running;
        do
        {
            wxYield();
            running = GetModuleUsage((HINSTANCE)instanceID);
        } while (running);
    }

    return instanceID;
#endif // Win16/32
}

long wxExecute(wxChar **argv, int flags, wxProcess *handler)
{
    wxString command;

    for ( ;; )
    {
        command += *argv++;
        if ( !*argv )
            break;

        command += _T(' ');
    }

    return wxExecute(command, flags, handler);
}

