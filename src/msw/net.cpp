/////////////////////////////////////////////////////////////////////////////
// Name:        msw/net.cpp
// Purpose:     MSW implementation of network/dialup classes and functions
// Author:      Vadim Zeitlin
// Modified by:
// Created:     07.07.99
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

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// these functions require Win32
#if defined(__WIN16__) && wxUSE_DIALUP_MANAGER
    #undef wxUSE_DIALUP_MANAGER
    #define wxUSE_DIALUP_MANAGER 0
#endif // wxUSE_DIALUP_MANAGER && Win16

#if wxUSE_DIALUP_MANAGER

#ifndef WX_PRECOMP
    #include "wx/log.h"
#endif

#include "wx/dynlib.h"

#include "wx/net.h"

#include <ras.h>
#include <raserror.h>

#include "wx/msw/private.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// this message is sent by the secondary thread when RAS status changes
#define wxWM_RAS_STATUS_CHANGED (WM_USER + 10010)

// ----------------------------------------------------------------------------
// types
// ----------------------------------------------------------------------------

// the signatures of RAS functions: all this is quite heavy, but we must do it
// to allow running wxWin programs on machine which don't have RAS installed
// (this does exist) - if we link with rasapi32.lib, the program will fail on
// startup because of the missing DLL...

#ifndef UNICODE
    typedef DWORD (* RASDIAL)( LPRASDIALEXTENSIONS, LPCSTR, LPRASDIALPARAMSA, DWORD, LPVOID, LPHRASCONN );
    typedef DWORD (* RASENUMCONNECTIONS)( LPRASCONNA, LPDWORD, LPDWORD );
    typedef DWORD (* RASENUMENTRIES)( LPCSTR, LPCSTR, LPRASENTRYNAMEA, LPDWORD, LPDWORD );
    typedef DWORD (* RASGETCONNECTSTATUS)( HRASCONN, LPRASCONNSTATUSA );
    typedef DWORD (* RASGETERRORSTRING)( UINT, LPSTR, DWORD );
    typedef DWORD (* RASHANGUP)( HRASCONN );
    typedef DWORD (* RASGETPROJECTIONINFO)( HRASCONN, RASPROJECTION, LPVOID, LPDWORD );
    typedef DWORD (* RASCREATEPHONEBOOKENTRY)( HWND, LPCSTR );
    typedef DWORD (* RASEDITPHONEBOOKENTRY)( HWND, LPCSTR, LPCSTR );
    typedef DWORD (* RASSETENTRYDIALPARAMS)( LPCSTR, LPRASDIALPARAMSA, BOOL );
    typedef DWORD (* RASGETENTRYDIALPARAMS)( LPCSTR, LPRASDIALPARAMSA, LPBOOL );
    typedef DWORD (* RASENUMDEVICES)( LPRASDEVINFOA, LPDWORD, LPDWORD );
    typedef DWORD (* RASGETCOUNTRYINFO)( LPRASCTRYINFOA, LPDWORD );
    typedef DWORD (* RASGETENTRYPROPERTIES)( LPCSTR, LPCSTR, LPRASENTRYA, LPDWORD, LPBYTE, LPDWORD );
    typedef DWORD (* RASSETENTRYPROPERTIES)( LPCSTR, LPCSTR, LPRASENTRYA, DWORD, LPBYTE, DWORD );
    typedef DWORD (* RASRENAMEENTRY)( LPCSTR, LPCSTR, LPCSTR );
    typedef DWORD (* RASDELETEENTRY)( LPCSTR, LPCSTR );
    typedef DWORD (* RASVALIDATEENTRYNAME)( LPCSTR, LPCSTR );
    typedef DWORD (* RASCONNECTIONNOTIFICATION)( HRASCONN, HANDLE, DWORD );

    static const char gs_funcSuffix = 'A';
#else // Unicode
    typedef DWORD (* RASDIAL)( LPRASDIALEXTENSIONS, LPCWSTR, LPRASDIALPARAMSW, DWORD, LPVOID, LPHRASCONN );
    typedef DWORD (* RASENUMCONNECTIONS)( LPRASCONNW, LPDWORD, LPDWORD );
    typedef DWORD (* RASENUMENTRIES)( LPCWSTR, LPCWSTR, LPRASENTRYNAMEW, LPDWORD, LPDWORD );
    typedef DWORD (* RASGETCONNECTSTATUS)( HRASCONN, LPRASCONNSTATUSW );
    typedef DWORD (* RASGETERRORSTRING)( UINT, LPWSTR, DWORD );
    typedef DWORD (* RASHANGUP)( HRASCONN );
    typedef DWORD (* RASGETPROJECTIONINFO)( HRASCONN, RASPROJECTION, LPVOID, LPDWORD );
    typedef DWORD (* RASCREATEPHONEBOOKENTRY)( HWND, LPCWSTR );
    typedef DWORD (* RASEDITPHONEBOOKENTRY)( HWND, LPCWSTR, LPCWSTR );
    typedef DWORD (* RASSETENTRYDIALPARAMS)( LPCWSTR, LPRASDIALPARAMSW, BOOL );
    typedef DWORD (* RASGETENTRYDIALPARAMS)( LPCWSTR, LPRASDIALPARAMSW, LPBOOL );
    typedef DWORD (* RASENUMDEVICES)( LPRASDEVINFOW, LPDWORD, LPDWORD );
    typedef DWORD (* RASGETCOUNTRYINFO)( LPRASCTRYINFOW, LPDWORD );
    typedef DWORD (* RASGETENTRYPROPERTIES)( LPCWSTR, LPCWSTR, LPRASENTRYW, LPDWORD, LPBYTE, LPDWORD );
    typedef DWORD (* RASSETENTRYPROPERTIES)( LPCWSTR, LPCWSTR, LPRASENTRYW, DWORD, LPBYTE, DWORD );
    typedef DWORD (* RASRENAMEENTRY)( LPCWSTR, LPCWSTR, LPCWSTR );
    typedef DWORD (* RASDELETEENTRY)( LPCWSTR, LPCWSTR );
    typedef DWORD (* RASVALIDATEENTRYNAME)( LPCWSTR, LPCWSTR );
    typedef DWORD (* RASCONNECTIONNOTIFICATION)( HRASCONN, HANDLE, DWORD );

    static const char gs_funcSuffix = 'W';
#endif // ASCII/Unicode

// structure passed to the secondary thread
struct wxRasThreadData
{
    wxRasThreadData()
    {
        hWnd = 0;
        hEventRas = hEventQuit = INVALID_HANDLE_VALUE;
        dialUpManager = NULL;
    }

    HWND    hWnd;       // window to send notifications to
    HANDLE  hEventRas,  // event which RAS signals when status changes
            hEventQuit; // event which we signal when we terminate

    class WXDLLEXPORT wxDialUpManagerMSW *dialUpManager;  // the owner
};

// ----------------------------------------------------------------------------
// wxDialUpManager class for MSW
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxDialUpManagerMSW : public wxDialUpManager
{
public:
    // ctor & dtor
    wxDialUpManagerMSW();
    virtual ~wxDialUpManagerMSW();

    // implement base class pure virtuals
    virtual bool IsOk() const;
    virtual bool Dial(const wxString& nameOfISP,
                      const wxString& username,
                      const wxString& password);
    virtual bool HangUp();
    virtual bool IsOnline();
    virtual void SetOnlineStatus(bool isOnline = TRUE);
    virtual bool EnableAutoCheckOnlineStatus(size_t nSeconds);
    virtual void DisableAutoCheckOnlineStatus();
    virtual void SetWellKnownHost(const wxString& hostname);

    // for RasTimer
    void CheckRasStatus(bool firstTime = FALSE);

    // for wxRasStatusWindowProc
    void OnConnectStatusChange();

private:
    // return the error string for the given RAS error code
    wxString GetErrorString(DWORD error);

    // find the (first) handle of the active connection
    HRASCONN FindActiveConnection();

    // notify the application about status change
    void NotifyApp(bool connected) const;

    // destroy the thread data and the thread itself
    void CleanUpThreadData();

    // timer used for polling RAS status
    class RasTimer : public wxTimer
    {
    public:
        RasTimer(wxDialUpManagerMSW *dialUpManager)
            { m_dialUpManager = dialUpManager; }

        virtual void Notify() { m_dialUpManager->CheckRasStatus(); }

    private:
        wxDialUpManagerMSW *m_dialUpManager;
    } m_timerStatusPolling;

    // thread handle for the thread sitting on connection change event
    HANDLE m_hThread;

    // data used by this thread and our hidden window to send messages between
    // each other
    wxRasThreadData m_data;

    // the handle of the connection we initiated or 0 if none
    static HRASCONN ms_hRasConnection;

    // the use count of rasapi32.dll
    static int ms_nDllCount;

    // the handle of rasapi32.dll when it's loaded
    static wxDllType ms_dllRas;

    // the pointers to RAS functions
    static RASDIAL ms_pfnRasDial;
    static RASENUMCONNECTIONS ms_pfnRasEnumConnections;
    static RASENUMENTRIES ms_pfnRasEnumEntries;
    static RASGETCONNECTSTATUS ms_pfnRasGetConnectStatus;
    static RASGETERRORSTRING ms_pfnRasGetErrorString;
    static RASHANGUP ms_pfnRasHangUp;
    static RASGETPROJECTIONINFO ms_pfnRasGetProjectionInfo;
    static RASCREATEPHONEBOOKENTRY ms_pfnRasCreatePhonebookEntry;
    static RASEDITPHONEBOOKENTRY ms_pfnRasEditPhonebookEntry;
    static RASSETENTRYDIALPARAMS ms_pfnRasSetEntryDialParams;
    static RASGETENTRYDIALPARAMS ms_pfnRasGetEntryDialParams;
    static RASENUMDEVICES ms_pfnRasEnumDevices;
    static RASGETCOUNTRYINFO ms_pfnRasGetCountryInfo;
    static RASGETENTRYPROPERTIES ms_pfnRasGetEntryProperties;
    static RASSETENTRYPROPERTIES ms_pfnRasSetEntryProperties;
    static RASRENAMEENTRY ms_pfnRasRenameEntry;
    static RASDELETEENTRY ms_pfnRasDeleteEntry;
    static RASVALIDATEENTRYNAME ms_pfnRasValidateEntryName;

    // this function is not supported by Win95
    static RASCONNECTIONNOTIFICATION ms_pfnRasConnectionNotification;

    // if this flag is different from -1, it overrides IsOnline()
    static int ms_userSpecifiedOnlineStatus;
};

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static LRESULT APIENTRY wxRasStatusWindowProc(HWND hWnd, UINT message,
                                              WPARAM wParam, LPARAM lParam);

static DWORD wxRasMonitorThread(wxRasThreadData *data);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// init the static variables
// ----------------------------------------------------------------------------

wxDialUpManager *wxDialUpManager::ms_dialUpManager = NULL;

HRASCONN wxDialUpManagerMSW::ms_hRasConnection = 0;

int wxDialUpManagerMSW::ms_nDllCount = 0;
wxDllType wxDialUpManagerMSW::ms_dllRas = 0;

RASDIAL wxDialUpManagerMSW::ms_pfnRasDial = 0;
RASENUMCONNECTIONS wxDialUpManagerMSW::ms_pfnRasEnumConnections = 0;
RASENUMENTRIES wxDialUpManagerMSW::ms_pfnRasEnumEntries = 0;
RASGETCONNECTSTATUS wxDialUpManagerMSW::ms_pfnRasGetConnectStatus = 0;
RASGETERRORSTRING wxDialUpManagerMSW::ms_pfnRasGetErrorString = 0;
RASHANGUP wxDialUpManagerMSW::ms_pfnRasHangUp = 0;
RASGETPROJECTIONINFO wxDialUpManagerMSW::ms_pfnRasGetProjectionInfo = 0;
RASCREATEPHONEBOOKENTRY wxDialUpManagerMSW::ms_pfnRasCreatePhonebookEntry = 0;
RASEDITPHONEBOOKENTRY wxDialUpManagerMSW::ms_pfnRasEditPhonebookEntry = 0;
RASSETENTRYDIALPARAMS wxDialUpManagerMSW::ms_pfnRasSetEntryDialParams = 0;
RASGETENTRYDIALPARAMS wxDialUpManagerMSW::ms_pfnRasGetEntryDialParams = 0;
RASENUMDEVICES wxDialUpManagerMSW::ms_pfnRasEnumDevices = 0;
RASGETCOUNTRYINFO wxDialUpManagerMSW::ms_pfnRasGetCountryInfo = 0;
RASGETENTRYPROPERTIES wxDialUpManagerMSW::ms_pfnRasGetEntryProperties = 0;
RASSETENTRYPROPERTIES wxDialUpManagerMSW::ms_pfnRasSetEntryProperties = 0;
RASRENAMEENTRY wxDialUpManagerMSW::ms_pfnRasRenameEntry = 0;
RASDELETEENTRY wxDialUpManagerMSW::ms_pfnRasDeleteEntry = 0;
RASVALIDATEENTRYNAME wxDialUpManagerMSW::ms_pfnRasValidateEntryName = 0;
RASCONNECTIONNOTIFICATION wxDialUpManagerMSW::ms_pfnRasConnectionNotification = 0;

int wxDialUpManagerMSW::ms_userSpecifiedOnlineStatus = -1;

// ----------------------------------------------------------------------------
// ctor and dtor: the dynamic linking happens here
// ----------------------------------------------------------------------------

// the static creator function is implemented here
wxDialUpManager *wxDialUpManager::Create()
{
    return new wxDialUpManagerMSW;
}

#ifdef __VISUALC__
    // warning about "'this' : used in base member initializer list" - so what?
    #pragma warning(disable:4355)
#endif // VC++

wxDialUpManagerMSW::wxDialUpManagerMSW()
                  : m_timerStatusPolling(this)
{
    if ( !ms_nDllCount++ )
    {
        // load the RAS library
        ms_dllRas = wxDllLoader::LoadLibrary("RASAPI32");
        if ( !ms_dllRas )
        {
            wxLogError(_("Dial up functions are unavailable because the "
                         "remote access service (RAS) is not installed "
                         "on this machine. Please install it."));
        }
        else
        {
            // resolve the functions we need

            // this will contain the name of the function we failed to resolve
            // if any at the end
            const char *funcName = NULL;

            // get the function from rasapi32.dll and abort if it's not found
            #define RESOLVE_RAS_FUNCTION(type, name)                    \
                ms_pfn##name = (type)wxDllLoader::GetSymbol(ms_dllRas,  \
                               wxString(#name) + gs_funcSuffix);        \
                if ( !ms_pfn##name )                                    \
                {                                                       \
                    funcName = #name;                                   \
                    goto exit;                                          \
                }

            // a variant of above macro which doesn't abort if the function is
            // not found in the DLL
            #define RESOLVE_OPTIONAL_RAS_FUNCTION(type, name)           \
                ms_pfn##name = (type)wxDllLoader::GetSymbol(ms_dllRas,  \
                               wxString(#name) + gs_funcSuffix);

            RESOLVE_RAS_FUNCTION(RASDIAL, RasDial);
            RESOLVE_RAS_FUNCTION(RASENUMCONNECTIONS, RasEnumConnections);
            RESOLVE_RAS_FUNCTION(RASENUMENTRIES, RasEnumEntries);
            RESOLVE_RAS_FUNCTION(RASGETCONNECTSTATUS, RasGetConnectStatus);
            RESOLVE_RAS_FUNCTION(RASGETERRORSTRING, RasGetErrorString);
            RESOLVE_RAS_FUNCTION(RASHANGUP, RasHangUp);
            RESOLVE_RAS_FUNCTION(RASGETPROJECTIONINFO, RasGetProjectionInfo);
            RESOLVE_RAS_FUNCTION(RASCREATEPHONEBOOKENTRY, RasCreatePhonebookEntry);
            RESOLVE_RAS_FUNCTION(RASEDITPHONEBOOKENTRY, RasEditPhonebookEntry);
            RESOLVE_RAS_FUNCTION(RASSETENTRYDIALPARAMS, RasSetEntryDialParams);
            RESOLVE_RAS_FUNCTION(RASGETENTRYDIALPARAMS, RasGetEntryDialParams);
            RESOLVE_RAS_FUNCTION(RASENUMDEVICES, RasEnumDevices);
            RESOLVE_RAS_FUNCTION(RASGETCOUNTRYINFO, RasGetCountryInfo);
            RESOLVE_RAS_FUNCTION(RASGETENTRYPROPERTIES, RasGetEntryProperties);
            RESOLVE_RAS_FUNCTION(RASSETENTRYPROPERTIES, RasSetEntryProperties);
            RESOLVE_RAS_FUNCTION(RASRENAMEENTRY, RasRenameEntry);
            RESOLVE_RAS_FUNCTION(RASDELETEENTRY, RasDeleteEntry);
            RESOLVE_RAS_FUNCTION(RASVALIDATEENTRYNAME, RasValidateEntryName);

            RESOLVE_OPTIONAL_RAS_FUNCTION(RASCONNECTIONNOTIFICATION, RasConnectionNotification);

            // keep your preprocessor name space clean
            #undef RESOLVE_RAS_FUNCTION
            #undef RESOLVE_OPTIONAL_RAS_FUNCTION

exit:
            if ( funcName )
            {
                wxLogError(_("The version of remote access service (RAS) "
                             "installed on this machine is too old, please "
                             "upgrade (the following required function is "
                             "missing: %s)."), funcName);

                wxDllLoader::UnloadLibrary(ms_dllRas);
                ms_dllRas = 0;
            }
        }
    }

    // initialize our data
    m_hThread = 0;

    // enable auto check by default
    EnableAutoCheckOnlineStatus(0);
}

wxDialUpManagerMSW::~wxDialUpManagerMSW()
{
    CleanUpThreadData();

    if ( !--ms_nDllCount )
    {
        // unload the RAS library
        wxDllLoader::UnloadLibrary(ms_dllRas);
        ms_dllRas = 0;
    }
}

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

wxString wxDialUpManagerMSW::GetErrorString(DWORD error)
{
    wxCHECK_MSG( IsOk(), "", _T("using uninitialized wxDialUpManager") );

    wxChar buffer[512]; // this should be more than enough according to MS docs
    DWORD dwRet = ms_pfnRasGetErrorString(error, buffer, WXSIZEOF(buffer));
    switch ( dwRet )
    {
        case ERROR_INVALID_PARAMETER:
            // this was a standard Win32 error probably
            return wxString(wxSysErrorMsg(error));

        default:
            {
                wxLogSysError(dwRet, _("Failed to retrieve text of RAS "
                                       "error message"));

                wxString msg;
                msg.Printf(_("unknown error (error code %08x)."), error);
                return msg;
            }

        case 0:
            // we want the error message to start from a lower case letter
            buffer[0] = wxTolower(buffer[0]);

            return wxString(buffer);
    }
}

HRASCONN wxDialUpManagerMSW::FindActiveConnection()
{
    wxCHECK_MSG( IsOk(), 0, _T("using uninitialized wxDialUpManager") );

    // enumerate connections
    DWORD cbBuf = sizeof(RASCONN);
    LPRASCONN lpRasConn = (LPRASCONN)malloc(cbBuf);
    if ( !lpRasConn )
    {
        // out of memory
        return 0;
    }

    lpRasConn->dwSize = sizeof(RASCONN);

    DWORD nConnections = 0;
    DWORD dwRet = ERROR_BUFFER_TOO_SMALL;

    while ( dwRet == ERROR_BUFFER_TOO_SMALL )
    {
        dwRet = ms_pfnRasEnumConnections(lpRasConn, &cbBuf, &nConnections);

        if ( dwRet == ERROR_BUFFER_TOO_SMALL )
        {
            LPRASCONN lpRasConnOld = lpRasConn;
            lpRasConn = (LPRASCONN)realloc(lpRasConn, cbBuf);
            if ( !lpRasConn )
            {
                // out of memory
                free(lpRasConnOld);

                return 0;
            }
        }
        else if ( dwRet == 0 )
        {
            // ok, success
            break;
        }
        else
        {
            // an error occured
            wxLogError(_("Cannot find active dialup connection: %s"),
                       GetErrorString(dwRet));
            return 0;
        }
    }

    HRASCONN hrasconn;

    switch ( nConnections )
    {
        case 0:
            // no connections
            hrasconn = 0;
            break;

        default:
            // more than 1 connection - we don't know what to do with this
            // case, so give a warning but continue (taking the first
            // connection) - the warning is really needed because this function
            // is used, for example, to select the connection to hang up and so
            // we may hang up the wrong connection here...
            wxLogWarning(_("Several active dialup connections found, "
                           "choosing one randomly."));
            // fall through

        case 1:
            // exactly 1 connection, great
            hrasconn = lpRasConn->hrasconn;
    }

    free(lpRasConn);

    return hrasconn;
}

void wxDialUpManagerMSW::CleanUpThreadData()
{
    if ( m_hThread )
    {
        if ( !SetEvent(m_data.hEventQuit) )
        {
            wxLogLastError("SetEvent(RasThreadQuit)");
        }

        CloseHandle(m_hThread);

        m_hThread = 0;
    }

    if ( m_data.hWnd )
    {
        DestroyWindow(m_data.hWnd);

        m_data.hWnd = 0;
    }

    if ( m_data.hEventQuit )
    {
        CloseHandle(m_data.hEventQuit);

        m_data.hEventQuit = 0;
    }

    if ( m_data.hEventRas )
    {
        CloseHandle(m_data.hEventRas);

        m_data.hEventRas = 0;
    }
}

// ----------------------------------------------------------------------------
// connection status
// ----------------------------------------------------------------------------

void wxDialUpManagerMSW::CheckRasStatus(bool firstTime)
{
    static int s_connected = -1;

    // use int, not bool to compare with -1
    int connected = FindActiveConnection() != 0;
    if ( connected != s_connected )
    {
        if ( (s_connected == -1) || firstTime )
        {
            // it's the first time we're called, just update the flag
        }
        else
        {
            // notify the program
            NotifyApp(connected != 0);
        }

        s_connected = connected;
    }
}

void wxDialUpManagerMSW::NotifyApp(bool connected) const
{
    wxDialUpEvent event(connected);
    (void)wxTheApp->ProcessEvent(event);
}

void wxDialUpManagerMSW::OnConnectStatusChange()
{
    // we know that status changed, but we don't know whether we're connected
    // or not - so find it out
    NotifyApp(FindActiveConnection() != 0);
}

// ----------------------------------------------------------------------------
// implementation of wxDialUpManager functions
// ----------------------------------------------------------------------------

bool wxDialUpManagerMSW::IsOk() const
{
    return ms_nDllCount != 0;
}

bool wxDialUpManagerMSW::Dial(const wxString& nameOfISP,
                              const wxString& username,
                              const wxString& password)
{
    wxCHECK_MSG( IsOk(), FALSE, _T("using uninitialized wxDialUpManager") );

    RASDIALPARAMS rasDialParams;
    rasDialParams.dwSize = sizeof(rasDialParams);
    strncpy(rasDialParams.szEntryName, nameOfISP, RAS_MaxEntryName);
    rasDialParams.szPhoneNumber[0] = '\0';
    rasDialParams.szCallbackNumber[0] = '*';
    rasDialParams.szCallbackNumber[0] = '\0';

    rasDialParams.szUserName[0] = '\0';
    rasDialParams.szPassword[0] = '\0';
    rasDialParams.szDomain[0] = '*';
    rasDialParams.szDomain[1] = '\0';

    wxString phoneBook;
    if ( wxGetOsVersion() == wxWINDOWS_NT )
    {
        // first get the length
        UINT nLen = ::GetSystemDirectory(NULL, 0);
        nLen++;

        if ( !::GetSystemDirectory(phoneBook.GetWriteBuf(nLen), nLen) )
        {
            wxLogSysError(_("Cannot find the location of address book file"));
        }

        phoneBook.UngetWriteBuf();

        // this is the default phone book
        phoneBook << "\\ras\\rasphone.pbk";
    }

    ms_hRasConnection = 0;
    DWORD dwRet = ms_pfnRasDial
                  (
                   (LPRASDIALEXTENSIONS)NULL,
                   phoneBook,               // phone book file (NT only)
                   &rasDialParams,
                   0,       // ignored because of next entry
                   NULL,    // no notifications - synchronous operations
                   &ms_hRasConnection
                  );

    if ( dwRet != 0 )
    {
        wxLogError(_("Failed to establish dialup connection: %s"),
                   GetErrorString(dwRet));

        return FALSE;
    }

    return TRUE;
}

bool wxDialUpManagerMSW::HangUp()
{
    wxCHECK_MSG( IsOk(), FALSE, _T("using uninitialized wxDialUpManager") );

    // we may terminate either the connection we initiated or another one which
    // is active now
    HRASCONN hRasConn = ms_hRasConnection ? ms_hRasConnection
                                          : FindActiveConnection();

    if ( !hRasConn )
    {
        wxLogError(_("Cannot hang up - no active dialup connection."));

        return FALSE;
    }

    DWORD dwRet = ms_pfnRasHangUp(hRasConn);
    if ( dwRet != 0 )
    {
        wxLogError(_("Failed to terminate the dialup connection: %s"),
                   GetErrorString(dwRet));
    }

    return TRUE;
}

bool wxDialUpManagerMSW::IsOnline()
{
    wxCHECK_MSG( IsOk(), FALSE, _T("using uninitialized wxDialUpManager") );

    if ( ms_userSpecifiedOnlineStatus != -1 )
    {
        // user specified flag overrides our logic
        return ms_userSpecifiedOnlineStatus != 0;
    }
    else
    {
        // return TRUE if there is at least one active connection
        return FindActiveConnection() != 0;
    }
}

void wxDialUpManagerMSW::SetOnlineStatus(bool isOnline)
{
    wxCHECK_RET( IsOk(), _T("using uninitialized wxDialUpManager") );

    ms_userSpecifiedOnlineStatus = isOnline;
}

bool wxDialUpManagerMSW::EnableAutoCheckOnlineStatus(size_t nSeconds)
{
    wxCHECK_MSG( IsOk(), FALSE, _T("using uninitialized wxDialUpManager") );

    bool ok = ms_pfnRasConnectionNotification != 0;

    if ( ok )
    {
        // we're running under NT 4.0, Windows 98 or later and can use
        // RasConnectionNotification() to be notified by a secondary thread

        // first, see if we don't have this thread already running
        if ( m_hThread != 0 )
        {
            DWORD dwSuspendCount = 2;
            while ( dwSuspendCount > 1 )
            {
                dwSuspendCount = ResumeThread(m_hThread);
                if ( dwSuspendCount == (DWORD)-1 )
                {
                    wxLogLastError("ResumeThread(RasThread)");

                    ok = FALSE;
                }
            }

            if ( ok )
            {
                return TRUE;
            }
        }
    }

    // create all the stuff we need to be notified about RAS connection
    // status change

    if ( ok )
    {
        // first create an event to wait on
        m_data.hEventRas = CreateEvent
                           (
                             NULL,      // security attribute (default)
                             FALSE,     // manual reset (not)
                             FALSE,     // initial state (not signaled)
                             NULL       // name (no)
                           );
        if ( !m_data.hEventRas )
        {
            wxLogLastError("CreateEvent(RasStatus)");

            ok = FALSE;
        }
    }

    if ( ok )
    {
        // create the event we use to quit the thread
        m_data.hEventQuit = CreateEvent(NULL, FALSE, FALSE, NULL);
        if ( !m_data.hEventQuit )
        {
            wxLogLastError("CreateEvent(RasThreadQuit)");

            CleanUpThreadData();

            ok = FALSE;
        }
    }

    if ( ok )
    {
        // create a hidden window to receive notification about connections
        // status change
        extern wxChar wxPanelClassName[];
        m_data.hWnd = ::CreateWindow(wxPanelClassName, NULL,
                                     0, 0, 0, 0,
                                     0, NULL,
                                     (HMENU)NULL, wxGetInstance(), 0);
        if ( !m_data.hWnd )
        {
            wxLogLastError("CreateWindow(RasHiddenWindow)");

            CleanUpThreadData();

            ok = FALSE;
        }

        // and subclass it
        FARPROC windowProc = MakeProcInstance
                             (
                              (FARPROC)wxRasStatusWindowProc,
                              wxGetInstance()
                             );

        ::SetWindowLong(m_data.hWnd, GWL_WNDPROC, (LONG) windowProc);
    }

    if ( ok )
    {
        // start the secondary thread
        m_data.dialUpManager = this;

        DWORD tid;
        m_hThread = CreateThread
                    (
                     NULL,
                     0,
                     (LPTHREAD_START_ROUTINE)wxRasMonitorThread,
                     (void *)&m_data,
                     0,
                     &tid
                    );

        if ( !m_hThread )
        {
            wxLogLastError("CreateThread(RasStatusThread)");

            CleanUpThreadData();
        }
    }

    if ( ok )
    {
        // start receiving RAS notifications
        DWORD dwRet = ms_pfnRasConnectionNotification
                      (
                        (HRASCONN)INVALID_HANDLE_VALUE,
                        m_data.hEventRas,
                        3 /* RASCN_Connection | RASCN_Disconnection */
                      );

        if ( dwRet != 0 )
        {
            wxLogDebug(_T("RasConnectionNotification() failed: %s"),
                       GetErrorString(dwRet));

            CleanUpThreadData();
        }
        else
        {
            return TRUE;
        }
    }

    // we're running under Windows 95 and have to poll ourselves
    // (or, alternatively, the code above for NT/98 failed)
    CheckRasStatus(TRUE /* first time */);
    m_timerStatusPolling.Stop();
    if ( nSeconds == 0 )
    {
        // default value
        nSeconds = 60;
    }
    m_timerStatusPolling.Start(nSeconds * 1000);

    return TRUE;
}

void wxDialUpManagerMSW::DisableAutoCheckOnlineStatus()
{
    wxCHECK_RET( IsOk(), _T("using uninitialized wxDialUpManager") );

    if ( m_hThread )
    {
        // we have running secondary thread, it's just enough to suspend it
        if ( SuspendThread(m_hThread) == (DWORD)-1 )
        {
            wxLogLastError("SuspendThread(RasThread)");
        }
    }
    else
    {
        // even simpler - just stop the timer
        m_timerStatusPolling.Stop();
    }
}

void wxDialUpManagerMSW::SetWellKnownHost(const wxString& WXUNUSED(hostname))
{
    wxCHECK_RET( IsOk(), _T("using uninitialized wxDialUpManager") );

    // nothing to do - we don't use this
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static DWORD wxRasMonitorThread(wxRasThreadData *data)
{
    HANDLE handles[2];
    handles[0] = data->hEventRas;
    handles[1] = data->hEventQuit;

    bool cont = TRUE;
    while ( cont )
    {
        DWORD dwRet = WaitForMultipleObjects(2, handles, FALSE, INFINITE);

        switch ( dwRet )
        {
            case WAIT_OBJECT_0:
                // RAS connection status changed
                SendMessage(data->hWnd, wxWM_RAS_STATUS_CHANGED,
                            0, (LPARAM)data);
                break;

            case WAIT_OBJECT_0 + 1:
                cont = FALSE;
                break;

            case WAIT_FAILED:
                wxLogLastError("WaitForMultipleObjects(RasMonitor)");
                break;
        }
    }

    return 0;
}

static LRESULT APIENTRY wxRasStatusWindowProc(HWND hWnd, UINT message,
                                              WPARAM wParam, LPARAM lParam)
{
    if ( message == wxWM_RAS_STATUS_CHANGED )
    {
        wxRasThreadData *data = (wxRasThreadData *)lParam;
        data->dialUpManager->OnConnectStatusChange();
    }

    return 0;
}

#endif // wxUSE_DIALUP_MANAGER
