/////////////////////////////////////////////////////////////////////////////
// Name:        log.cpp
// Purpose:     Assorted wxLogXXX functions, and wxLog (sink for logs)
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
  #pragma implementation "log.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

// wxWindows
#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/intl.h"
    #include "wx/app.h"

    #if wxUSE_GUI
        #include "wx/window.h"
        #ifdef __WXMSW__
            #include "wx/msw/private.h"
        #endif
    #endif
#endif //WX_PRECOMP

#include  "wx/file.h"
#include  "wx/textfile.h"
#include  "wx/utils.h"
#include  "wx/wxchar.h"
#include  "wx/log.h"
#include  "wx/thread.h"

#if wxUSE_LOG

// other standard headers
#include  <errno.h>
#include  <stdlib.h>
#include  <time.h>

#if defined(__WXMSW__)
  #include  "wx/msw/private.h"      // includes windows.h for OutputDebugString
#endif

// ----------------------------------------------------------------------------
// non member functions
// ----------------------------------------------------------------------------

// define this to enable wrapping of log messages
//#define LOG_PRETTY_WRAP

#ifdef  LOG_PRETTY_WRAP
  static void wxLogWrap(FILE *f, const char *pszPrefix, const char *psz);
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

// log functions can't allocate memory (LogError("out of memory...") should
// work!), so we use a static buffer for all log messages
#define LOG_BUFFER_SIZE   (4096)

// static buffer for error messages
static wxChar s_szBuf[LOG_BUFFER_SIZE];

#if wxUSE_THREADS

// the critical section protecting the static buffer
static wxCriticalSection gs_csLogBuf;

#endif // wxUSE_THREADS

// return true if we have a non NULL non disabled log target
static inline bool IsLoggingEnabled()
{
    return wxLog::IsEnabled() && (wxLog::GetActiveTarget() != NULL);
}

// ----------------------------------------------------------------------------
// implementation of Log functions
//
// NB: unfortunately we need all these distinct functions, we can't make them
//     macros and not all compilers inline vararg functions.
// ----------------------------------------------------------------------------

// generic log function
void wxLogGeneric(wxLogLevel level, const wxChar *szFormat, ...)
{
    if ( IsLoggingEnabled() ) {
        wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);

        va_list argptr;
        va_start(argptr, szFormat);
        wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);
        va_end(argptr);

        wxLog::OnLog(level, s_szBuf, time(NULL));
    }
}

#define IMPLEMENT_LOG_FUNCTION(level)                               \
  void wxLog##level(const wxChar *szFormat, ...)                    \
  {                                                                 \
    if ( IsLoggingEnabled() ) {                                     \
      wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);                      \
                                                                    \
      va_list argptr;                                               \
      va_start(argptr, szFormat);                                   \
      wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);    \
      va_end(argptr);                                               \
                                                                    \
      wxLog::OnLog(wxLOG_##level, s_szBuf, time(NULL));             \
    }                                                               \
  }

IMPLEMENT_LOG_FUNCTION(FatalError)
IMPLEMENT_LOG_FUNCTION(Error)
IMPLEMENT_LOG_FUNCTION(Warning)
IMPLEMENT_LOG_FUNCTION(Message)
IMPLEMENT_LOG_FUNCTION(Info)
IMPLEMENT_LOG_FUNCTION(Status)

// same as info, but only if 'verbose' mode is on
void wxLogVerbose(const wxChar *szFormat, ...)
{
    if ( IsLoggingEnabled() ) {
        wxLog *pLog = wxLog::GetActiveTarget();
        if ( pLog != NULL && pLog->GetVerbose() ) {
            wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);

            va_list argptr;
            va_start(argptr, szFormat);
            wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);
            va_end(argptr);

            wxLog::OnLog(wxLOG_Info, s_szBuf, time(NULL));
        }
    }
}

// debug functions
#ifdef __WXDEBUG__
#define IMPLEMENT_LOG_DEBUG_FUNCTION(level)                         \
  void wxLog##level(const wxChar *szFormat, ...)                    \
  {                                                                 \
    if ( IsLoggingEnabled() ) {                                     \
      wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);                      \
                                                                    \
      va_list argptr;                                               \
      va_start(argptr, szFormat);                                   \
      wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);    \
      va_end(argptr);                                               \
                                                                    \
      wxLog::OnLog(wxLOG_##level, s_szBuf, time(NULL));             \
    }                                                               \
  }

  void wxLogTrace(const wxChar *mask, const wxChar *szFormat, ...)
  {
    if ( IsLoggingEnabled() && wxLog::IsAllowedTraceMask(mask) ) {
      wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);

      wxChar *p = s_szBuf;
      size_t len = WXSIZEOF(s_szBuf);
      wxStrncpy(s_szBuf, _T("("), len);
      len -= 1; // strlen("(")
      p += 1;
      wxStrncat(p, mask, len);
      size_t lenMask = wxStrlen(mask);
      len -= lenMask;
      p += lenMask;

      wxStrncat(p, _T(") "), len);
      len -= 2;
      p += 2;

      va_list argptr;
      va_start(argptr, szFormat);
      wxVsnprintf(p, len, szFormat, argptr);
      va_end(argptr);

      wxLog::OnLog(wxLOG_Trace, s_szBuf, time(NULL));
    }
  }

  void wxLogTrace(wxTraceMask mask, const wxChar *szFormat, ...)
  {
    // we check that all of mask bits are set in the current mask, so
    // that wxLogTrace(wxTraceRefCount | wxTraceOle) will only do something
    // if both bits are set.
    if ( IsLoggingEnabled() && ((wxLog::GetTraceMask() & mask) == mask) ) {
      wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);

      va_list argptr;
      va_start(argptr, szFormat);
      wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);
      va_end(argptr);

      wxLog::OnLog(wxLOG_Trace, s_szBuf, time(NULL));
    }
  }

#else // release
  #define IMPLEMENT_LOG_DEBUG_FUNCTION(level)
#endif

IMPLEMENT_LOG_DEBUG_FUNCTION(Debug)
IMPLEMENT_LOG_DEBUG_FUNCTION(Trace)

// wxLogSysError: one uses the last error code, for other  you must give it
// explicitly

// common part of both wxLogSysError
void wxLogSysErrorHelper(long lErrCode)
{
    wxChar szErrMsg[LOG_BUFFER_SIZE / 2];
    wxSnprintf(szErrMsg, WXSIZEOF(szErrMsg),
               _(" (error %ld: %s)"), lErrCode, wxSysErrorMsg(lErrCode));
    wxStrncat(s_szBuf, szErrMsg, WXSIZEOF(s_szBuf) - wxStrlen(s_szBuf));

    wxLog::OnLog(wxLOG_Error, s_szBuf, time(NULL));
}

void WXDLLEXPORT wxLogSysError(const wxChar *szFormat, ...)
{
    if ( IsLoggingEnabled() ) {
        wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);

        va_list argptr;
        va_start(argptr, szFormat);
        wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);
        va_end(argptr);

        wxLogSysErrorHelper(wxSysErrorCode());
    }
}

void WXDLLEXPORT wxLogSysError(long lErrCode, const wxChar *szFormat, ...)
{
    if ( IsLoggingEnabled() ) {
        wxCRIT_SECT_LOCKER(locker, gs_csLogBuf);

        va_list argptr;
        va_start(argptr, szFormat);
        wxVsnprintf(s_szBuf, WXSIZEOF(s_szBuf), szFormat, argptr);
        va_end(argptr);

        wxLogSysErrorHelper(lErrCode);
    }
}

// ----------------------------------------------------------------------------
// wxLog class implementation
// ----------------------------------------------------------------------------

wxLog::wxLog()
{
    m_bHasMessages = FALSE;
}

wxLog *wxLog::GetActiveTarget()
{
    if ( ms_bAutoCreate && ms_pLogger == NULL ) {
        // prevent infinite recursion if someone calls wxLogXXX() from
        // wxApp::CreateLogTarget()
        static bool s_bInGetActiveTarget = FALSE;
        if ( !s_bInGetActiveTarget ) {
            s_bInGetActiveTarget = TRUE;

            // ask the application to create a log target for us
            if ( wxTheApp != NULL )
                ms_pLogger = wxTheApp->CreateLogTarget();
            else
                ms_pLogger = new wxLogStderr;

            s_bInGetActiveTarget = FALSE;

            // do nothing if it fails - what can we do?
        }
    }

    return ms_pLogger;
}

wxLog *wxLog::SetActiveTarget(wxLog *pLogger)
{
    if ( ms_pLogger != NULL ) {
        // flush the old messages before changing because otherwise they might
        // get lost later if this target is not restored
        ms_pLogger->Flush();
    }

    wxLog *pOldLogger = ms_pLogger;
    ms_pLogger = pLogger;

    return pOldLogger;
}

void wxLog::DontCreateOnDemand()
{
    ms_bAutoCreate = FALSE;

    // this is usually called at the end of the program and we assume that it
    // is *always* called at the end - so we free memory here to avoid false
    // memory leak reports from wxWin  memory tracking code
    ClearTraceMasks();
}

void wxLog::RemoveTraceMask(const wxString& str)
{
    int index = ms_aTraceMasks.Index(str);
    if ( index != wxNOT_FOUND )
        ms_aTraceMasks.Remove((size_t)index);
}

void wxLog::ClearTraceMasks()
{
    ms_aTraceMasks.Clear();
}

void wxLog::TimeStamp(wxString *str)
{
    if ( ms_timestamp )
    {
        wxChar buf[256];
        time_t timeNow;
        (void)time(&timeNow);
        wxStrftime(buf, WXSIZEOF(buf), ms_timestamp, localtime(&timeNow));

        str->Empty();
        *str << buf << wxT(": ");
    }
}

void wxLog::DoLog(wxLogLevel level, const wxChar *szString, time_t t)
{
    switch ( level ) {
        case wxLOG_FatalError:
            DoLogString(wxString(_("Fatal error: ")) + szString, t);
            DoLogString(_("Program aborted."), t);
            Flush();
            abort();
            break;

        case wxLOG_Error:
            DoLogString(wxString(_("Error: ")) + szString, t);
            break;

        case wxLOG_Warning:
            DoLogString(wxString(_("Warning: ")) + szString, t);
            break;

        case wxLOG_Info:
            if ( GetVerbose() )
        case wxLOG_Message:
        case wxLOG_Status:
        default:    // log unknown log levels too
                DoLogString(szString, t);
            break;

        case wxLOG_Trace:
        case wxLOG_Debug:
#ifdef __WXDEBUG__
            {
                wxString msg = level == wxLOG_Trace ? wxT("Trace: ")
                                                    : wxT("Debug: ");
                msg << szString;
                DoLogString(msg, t);
            }
#endif // Debug
            break;
    }
}

void wxLog::DoLogString(const wxChar *WXUNUSED(szString), time_t WXUNUSED(t))
{
    wxFAIL_MSG(wxT("DoLogString must be overriden if it's called."));
}

void wxLog::Flush()
{
    // remember that we don't have any more messages to show
    m_bHasMessages = FALSE;
}

// ----------------------------------------------------------------------------
// wxLogStderr class implementation
// ----------------------------------------------------------------------------

wxLogStderr::wxLogStderr(FILE *fp)
{
    if ( fp == NULL )
        m_fp = stderr;
    else
        m_fp = fp;
}

#if defined(__WXMAC__) && !defined(__DARWIN__)
#define kDebuggerSignature        'MWDB'

static Boolean FindProcessBySignature(OSType signature, ProcessInfoRec* info)
{
    OSErr err;
    ProcessSerialNumber psn;
    Boolean found = false;
    psn.highLongOfPSN = 0;
    psn.lowLongOfPSN = kNoProcess;

    if (!info) return false;

    info->processInfoLength = sizeof(ProcessInfoRec);
    info->processName = NULL;
    info->processAppSpec = NULL;

    err = noErr;
    while (!found && err == noErr)
    {
        err = GetNextProcess(&psn);
        if (err == noErr)
        {
            err = GetProcessInformation(&psn, info);
            found = err == noErr && info->processSignature == signature;
        }
    }
    return found;
}

pascal Boolean MWDebuggerIsRunning(void)
{
    ProcessInfoRec info;
    return FindProcessBySignature(kDebuggerSignature, &info);
}

pascal OSErr AmIBeingMWDebugged(Boolean* result)
{
    OSErr err;
    ProcessSerialNumber psn;
    OSType sig = kDebuggerSignature;
    AppleEvent    theAE = {typeNull, NULL};
    AppleEvent    theReply = {typeNull, NULL};
    AEAddressDesc addr  = {typeNull, NULL};
    DescType actualType;
    Size actualSize;

    if (!result) return paramErr;

    err = AECreateDesc(typeApplSignature, &sig, sizeof(sig), &addr);
    if (err != noErr) goto exit;

    err = AECreateAppleEvent(kDebuggerSignature, 'Dbg?', &addr,
                kAutoGenerateReturnID, kAnyTransactionID, &theAE);
    if (err != noErr) goto exit;

    GetCurrentProcess(&psn);
    err = AEPutParamPtr(&theAE, keyDirectObject, typeProcessSerialNumber,
            &psn, sizeof(psn));
    if (err != noErr) goto exit;

    err = AESend(&theAE, &theReply, kAEWaitReply, kAENormalPriority,
                    kAEDefaultTimeout, NULL, NULL);
    if (err != noErr) goto exit;

    err = AEGetParamPtr(&theReply, keyAEResult, typeBoolean, &actualType, result,
                sizeof(Boolean), &actualSize);

exit:
    if (addr.dataHandle)
        AEDisposeDesc(&addr);
    if (theAE.dataHandle)
        AEDisposeDesc(&theAE);
    if (theReply.dataHandle)
        AEDisposeDesc(&theReply);

    return err;
}
#endif

void wxLogStderr::DoLogString(const wxChar *szString, time_t WXUNUSED(t))
{
    wxString str;
    TimeStamp(&str);
    str << szString;

    fputs(str.mb_str(), m_fp);
    fputc(_T('\n'), m_fp);
    fflush(m_fp);

    // under Windows, programs usually don't have stderr at all, so show the
    // messages also under debugger - unless it's a console program
#if defined(__WXMSW__) && wxUSE_GUI && !defined(__WXMICROWIN__)
    str += wxT("\r\n") ;
    OutputDebugString(str.c_str());
#endif // MSW
#if defined(__WXMAC__) && !defined(__DARWIN__) && wxUSE_GUI
    Str255 pstr ;
    strcpy( (char*) pstr , str.c_str() ) ;
    strcat( (char*) pstr , ";g" ) ;
    c2pstr( (char*) pstr ) ;
#if __WXDEBUG__
    Boolean running = false ;

/*
    if ( MWDebuggerIsRunning() )
    {
        AmIBeingMWDebugged( &running ) ;
    }
*/
    if (running)
    {
#ifdef __powerc
        DebugStr(pstr);
#else
        SysBreakStr(pstr);
#endif
    }
    else
#endif
    {
#ifdef __powerc
        DebugStr(pstr);
#else
        DebugStr(pstr);
#endif
    }
#endif // Mac
}

// ----------------------------------------------------------------------------
// wxLogStream implementation
// ----------------------------------------------------------------------------

#if wxUSE_STD_IOSTREAM
wxLogStream::wxLogStream(wxSTD ostream *ostr)
{
    if ( ostr == NULL )
        m_ostr = &wxSTD cerr;
    else
        m_ostr = ostr;
}

void wxLogStream::DoLogString(const wxChar *szString, time_t WXUNUSED(t))
{
    wxString str;
    TimeStamp(&str);
    (*m_ostr) << str << wxConvertWX2MB(szString) << wxSTD endl;
}
#endif // wxUSE_STD_IOSTREAM

// ----------------------------------------------------------------------------
// wxLogChain
// ----------------------------------------------------------------------------

wxLogChain::wxLogChain(wxLog *logger)
{
    m_logNew = logger;
    m_logOld = wxLog::SetActiveTarget(this);
}

void wxLogChain::SetLog(wxLog *logger)
{
    if ( m_logNew != this )
        delete m_logNew;

    wxLog::SetActiveTarget(logger);

    m_logNew = logger;
}

void wxLogChain::Flush()
{
    if ( m_logOld )
        m_logOld->Flush();

    // be careful to avoid inifinite recursion
    if ( m_logNew && m_logNew != this )
        m_logNew->Flush();
}

void wxLogChain::DoLog(wxLogLevel level, const wxChar *szString, time_t t)
{
    // let the previous logger show it
    if ( m_logOld && IsPassingMessages() )
    {
        // bogus cast just to access protected DoLog
        ((wxLogChain *)m_logOld)->DoLog(level, szString, t);
    }

    if ( m_logNew && m_logNew != this )
    {
        // as above...
        ((wxLogChain *)m_logNew)->DoLog(level, szString, t);
    }
}

// ----------------------------------------------------------------------------
// wxLogPassThrough
// ----------------------------------------------------------------------------

#ifdef __VISUALC__
    // "'this' : used in base member initializer list" - so what?
    #pragma warning(disable:4355)
#endif // VC++

wxLogPassThrough::wxLogPassThrough()
                : wxLogChain(this)
{
}

#ifdef __VISUALC__
    #pragma warning(default:4355)
#endif // VC++

// ============================================================================
// Global functions/variables
// ============================================================================

// ----------------------------------------------------------------------------
// static variables
// ----------------------------------------------------------------------------

wxLog          *wxLog::ms_pLogger      = (wxLog *)NULL;
bool            wxLog::ms_doLog        = TRUE;
bool            wxLog::ms_bAutoCreate  = TRUE;
bool            wxLog::ms_bVerbose     = FALSE;

size_t          wxLog::ms_suspendCount = 0;

#if wxUSE_GUI
    const wxChar *wxLog::ms_timestamp  = wxT("%X");  // time only, no date
#else
    const wxChar *wxLog::ms_timestamp  = NULL;       // save space
#endif

wxTraceMask     wxLog::ms_ulTraceMask  = (wxTraceMask)0;
wxArrayString   wxLog::ms_aTraceMasks;

// ----------------------------------------------------------------------------
// stdout error logging helper
// ----------------------------------------------------------------------------

// helper function: wraps the message and justifies it under given position
// (looks more pretty on the terminal). Also adds newline at the end.
//
// TODO this is now disabled until I find a portable way of determining the
//      terminal window size (ok, I found it but does anybody really cares?)
#ifdef LOG_PRETTY_WRAP
static void wxLogWrap(FILE *f, const char *pszPrefix, const char *psz)
{
    size_t nMax = 80; // FIXME
    size_t nStart = strlen(pszPrefix);
    fputs(pszPrefix, f);

    size_t n;
    while ( *psz != '\0' ) {
        for ( n = nStart; (n < nMax) && (*psz != '\0'); n++ )
            putc(*psz++, f);

        // wrapped?
        if ( *psz != '\0' ) {
            /*putc('\n', f);*/
            for ( n = 0; n < nStart; n++ )
                putc(' ', f);

            // as we wrapped, squeeze all white space
            while ( isspace(*psz) )
                psz++;
        }
    }

    putc('\n', f);
}
#endif  //LOG_PRETTY_WRAP

// ----------------------------------------------------------------------------
// error code/error message retrieval functions
// ----------------------------------------------------------------------------

// get error code from syste
unsigned long wxSysErrorCode()
{
#if defined(__WXMSW__) && !defined(__WXMICROWIN__)
#ifdef  __WIN32__
    return ::GetLastError();
#else   //WIN16
    // TODO what to do on Windows 3.1?
    return 0;
#endif  //WIN16/32
#else   //Unix
    return errno;
#endif  //Win/Unix
}

// get error message from system
const wxChar *wxSysErrorMsg(unsigned long nErrCode)
{
    if ( nErrCode == 0 )
        nErrCode = wxSysErrorCode();

#if defined(__WXMSW__) && !defined(__WXMICROWIN__)
#ifdef  __WIN32__
    static wxChar s_szBuf[LOG_BUFFER_SIZE / 2];

    // get error message from system
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, nErrCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

    // copy it to our buffer and free memory
    if( lpMsgBuf != 0 ) {
        wxStrncpy(s_szBuf, (const wxChar *)lpMsgBuf, WXSIZEOF(s_szBuf) - 1);
        s_szBuf[WXSIZEOF(s_szBuf) - 1] = wxT('\0');

        LocalFree(lpMsgBuf);

        // returned string is capitalized and ended with '\r\n' - bad
        s_szBuf[0] = (wxChar)wxTolower(s_szBuf[0]);
        size_t len = wxStrlen(s_szBuf);
        if ( len > 0 ) {
            // truncate string
            if ( s_szBuf[len - 2] == wxT('\r') )
                s_szBuf[len - 2] = wxT('\0');
        }
    }
    else {
        s_szBuf[0] = wxT('\0');
    }

    return s_szBuf;
#else   //Win16
    // TODO
    return NULL;
#endif // Win16/32
#else   // Unix
#if wxUSE_UNICODE
    static wxChar s_szBuf[LOG_BUFFER_SIZE / 2];
    wxConvCurrent->MB2WC(s_szBuf, strerror(nErrCode), WXSIZEOF(s_szBuf) -1);
    return s_szBuf;
#else
    return strerror((int)nErrCode);
#endif
#endif  // Win/Unix
}

#endif //wxUSE_LOG
