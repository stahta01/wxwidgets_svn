/////////////////////////////////////////////////////////////////////////////
// Name:        ftp.cpp
// Purpose:     FTP protocol
// Author:      Guilhem Lavaux
// Modified by: Mark Johnson, wxWindows@mj10777.de
//              20000917 : RmDir, GetLastResult, GetList
//              Vadim Zeitlin (numerous fixes and rewrites to all part of the
//              code, support ASCII/Binary modes, better error reporting, more
//              robust Abort(), support for arbitrary FTP commands, ...)
// Created:     07/07/1997
// RCS-ID:      $Id$
// Copyright:   (c) 1997, 1998 Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

#ifdef __GNUG__
  #pragma implementation "ftp.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_SOCKETS && wxUSE_STREAMS

#ifndef WX_PRECOMP
    #include <stdlib.h>
    #include "wx/string.h"
    #include "wx/utils.h"
    #include "wx/log.h"
    #include "wx/intl.h"
#endif // WX_PRECOMP

#include "wx/sckaddr.h"
#include "wx/socket.h"
#include "wx/url.h"
#include "wx/sckstrm.h"
#include "wx/protocol/protocol.h"
#include "wx/protocol/ftp.h"

#if defined(__WXMAC__)
    #include "/wx/mac/macsock.h"
#endif

#ifndef __MWERKS__
    #include <memory.h>
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the length of FTP status code (3 digits)
static const size_t LEN_CODE = 3;

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFTP, wxProtocol)
IMPLEMENT_PROTOCOL(wxFTP, wxT("ftp"), wxT("ftp"), TRUE)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxFTP constructor and destructor
// ----------------------------------------------------------------------------

wxFTP::wxFTP()
{
    m_lastError = wxPROTO_NOERR;
    m_streaming = FALSE;
    m_modeSet = FALSE;

    m_user = wxT("anonymous");
    m_passwd << wxGetUserId() << wxT('@') << wxGetFullHostName();

    SetNotify(0);
    SetFlags(wxSOCKET_NONE);
}

wxFTP::~wxFTP()
{
    if ( m_streaming )
    {
        (void)Abort();
    }

    Close();
}

// ----------------------------------------------------------------------------
// wxFTP connect and login methods
// ----------------------------------------------------------------------------

bool wxFTP::Connect(wxSockAddress& addr, bool WXUNUSED(wait))
{
    if ( !wxProtocol::Connect(addr) )
    {
        m_lastError = wxPROTO_NETERR;
        return FALSE;
    }

    if ( !m_user )
    {
        m_lastError = wxPROTO_CONNERR;
        return FALSE;
    }

    // we should have 220 welcome message
    if ( !CheckResult('2') )
    {
        Close();
        return FALSE;
    }

    wxString command;
    command.Printf(wxT("USER %s"), m_user.c_str());
    char rc = SendCommand(command);
    if ( rc == '2' )
    {
        // 230 return: user accepted without password
        return TRUE;
    }

    if ( rc != '3' )
    {
        Close();
        return FALSE;
    }

    command.Printf(wxT("PASS %s"), m_passwd.c_str());
    if ( !CheckCommand(command, '2') )
    {
        Close();
        return FALSE;
    }

    return TRUE;
}

bool wxFTP::Connect(const wxString& host)
{
    wxIPV4address addr;
    addr.Hostname(host);
    addr.Service(wxT("ftp"));

    return Connect(addr);
}

bool wxFTP::Close()
{
    if ( m_streaming )
    {
        m_lastError = wxPROTO_STREAMING;
        return FALSE;
    }

    if ( IsConnected() )
    {
        if ( !CheckCommand(wxT("QUIT"), '2') )
        {
            wxLogDebug(_T("Failed to close connection gracefully."));
        }
    }

    return wxSocketClient::Close();
}

// ============================================================================
// low level methods
// ============================================================================

// ----------------------------------------------------------------------------
// Send command to FTP server
// ----------------------------------------------------------------------------

char wxFTP::SendCommand(const wxString& command)
{
    if ( m_streaming )
    {
        m_lastError = wxPROTO_STREAMING;
        return 0;
    }

    wxString tmp_str = command + wxT("\r\n");
    const wxWX2MBbuf tmp_buf = tmp_str.mb_str();
    if ( Write(wxMBSTRINGCAST tmp_buf, strlen(tmp_buf)).Error())
    {
        m_lastError = wxPROTO_NETERR;
        return 0;
    }

    wxLogTrace(_T("ftp"), _T("==> %s"), command.c_str());

    return GetResult();
}

// ----------------------------------------------------------------------------
// Recieve servers reply
// ----------------------------------------------------------------------------

char wxFTP::GetResult()
{
    wxString code;

    // m_lastResult will contain the entire server response, possibly on
    // multiple lines
    m_lastResult.clear();

    // we handle multiline replies here according to RFC 959: it says that a
    // reply may either be on 1 line of the form "xyz ..." or on several lines
    // in whuch case it looks like
    //      xyz-...
    //      ...
    //      xyz ...
    // and the intermeidate lines may start with xyz or not
    bool badReply = FALSE;
    bool firstLine = TRUE;
    bool endOfReply = FALSE;
    while ( !endOfReply && !badReply )
    {
        wxString line;
        m_lastError = ReadLine(line);
        if ( m_lastError )
            return 0;

        if ( !m_lastResult.empty() )
        {
            // separate from last line
            m_lastResult += _T('\n');
        }

        m_lastResult += line;

        // unless this is an intermediate line of a multiline reply, it must
        // contain the code in the beginning and '-' or ' ' following it
        if ( line.Len() < LEN_CODE + 1 )
        {
            if ( firstLine )
            {
                badReply = TRUE;
            }
            else
            {
                wxLogTrace(_T("ftp"), _T("<== %s %s"),
                           code.c_str(), line.c_str());
            }
        }
        else // line has at least 4 chars
        {
            // this is the char which tells us what we're dealing with
            wxChar chMarker = line.GetChar(LEN_CODE);

            if ( firstLine )
            {
                code = wxString(line, LEN_CODE);
                wxLogTrace(_T("ftp"), _T("<== %s %s"),
                           code.c_str(), line.c_str() + LEN_CODE + 1);

                switch ( chMarker )
                {
                    case _T(' '):
                        endOfReply = TRUE;
                        break;

                    case _T('-'):
                        firstLine = FALSE;
                        break;

                    default:
                        // unexpected
                        badReply = TRUE;
                }
            }
            else // subsequent line of multiline reply
            {
                if ( wxStrncmp(line, code, LEN_CODE) == 0 )
                {
                    if ( chMarker == _T(' ') )
                    {
                        endOfReply = TRUE;
                    }

                    wxLogTrace(_T("ftp"), _T("<== %s %s"),
                               code.c_str(), line.c_str() + LEN_CODE + 1);
                }
                else
                {
                    // just part of reply
                    wxLogTrace(_T("ftp"), _T("<== %s %s"),
                               code.c_str(), line.c_str());
                }
            }
        }
    }

    if ( badReply )
    {
        wxLogDebug(_T("Broken FTP server: '%s' is not a valid reply."),
                   m_lastResult.c_str());

        m_lastError = wxPROTO_PROTERR;

        return 0;
    }

    // if we got here we must have a non empty code string
    return code[0u];
}

// ----------------------------------------------------------------------------
// wxFTP simple commands
// ----------------------------------------------------------------------------

bool wxFTP::SetTransferMode(TransferMode transferMode)
{
    wxString mode;
    switch ( transferMode )
    {
        default:
            wxFAIL_MSG(_T("unknown FTP transfer mode"));
            // fall through

        case BINARY:
            mode = _T('I');
            break;

        case ASCII:
            mode = _T('A');
            break;
    }

    if ( !DoSimpleCommand(_T("TYPE"), mode) )
    {
        wxLogError(_("Failed to set FTP transfer mode to %s."),
                   transferMode == ASCII ? _("ASCII") : _("binary"));

        return FALSE;
    }

    m_modeSet = TRUE;

    return TRUE;
}

bool wxFTP::DoSimpleCommand(const wxChar *command, const wxString& arg)
{
    wxString fullcmd = command;
    if ( !arg.empty() )
    {
        fullcmd << _T(' ') << arg;
    }

    if ( !CheckCommand(fullcmd, '2') )
    {
        wxLogDebug(_T("FTP command '%s' failed."), fullcmd.c_str());

        return FALSE;
    }

    return TRUE;
}

bool wxFTP::ChDir(const wxString& dir)
{
    // some servers might not understand ".." if they use different directory
    // tree conventions, but they always understand CDUP - should we use it if
    // dir == ".."? OTOH, do such servers (still) exist?

    return DoSimpleCommand(_T("CWD"), dir);
}

bool wxFTP::MkDir(const wxString& dir)
{
    return DoSimpleCommand(_T("MKD"), dir);
}

bool wxFTP::RmDir(const wxString& dir)
{
    return DoSimpleCommand(_T("RMD"), dir);
}

wxString wxFTP::Pwd()
{
    wxString path;

    if ( CheckCommand(wxT("PWD"), '2') )
    {
        // the result is at least that long if CheckCommand() succeeded
        const wxChar *p = m_lastResult.c_str() + LEN_CODE + 1;
        if ( *p != _T('"') )
        {
            wxLogDebug(_T("Missing starting quote in reply for PWD: %s"), p);
        }
        else
        {
            for ( p++; *p; p++ )
            {
                if ( *p == _T('"') )
                {
                    // check if the quote is doubled
                    p++;
                    if ( !*p || *p != _T('"') )
                    {
                        // no, this is the end
                        break;
                    }
                    //else: yes, it is: this is an embedded quote in the
                    //      filename, treat as normal char
                }

                path += *p;
            }

            if ( !*p )
            {
                wxLogDebug(_T("Missing ending quote in reply for PWD: %s"),
                           m_lastResult.c_str() + LEN_CODE + 1);
            }
        }
    }
    else
    {
        wxLogDebug(_T("FTP PWD command failed."));
    }

    return path;
}

bool wxFTP::Rename(const wxString& src, const wxString& dst)
{
    wxString str;

    str = wxT("RNFR ") + src;
    if ( !CheckCommand(str, '3') )
        return FALSE;

    str = wxT("RNTO ") + dst;

    return CheckCommand(str, '2');
}

bool wxFTP::RmFile(const wxString& path)
{
    wxString str;
    str = wxT("DELE ") + path;

    return CheckCommand(str, '2');
}

// ----------------------------------------------------------------------------
// wxFTP download and upload
// ----------------------------------------------------------------------------

class wxInputFTPStream : public wxSocketInputStream
{
public:
    wxInputFTPStream(wxFTP *ftp_clt, wxSocketBase *sock)
        : wxSocketInputStream(*sock), m_ftp(ftp_clt)
    {
    }

    size_t GetSize() const { return m_ftpsize; }

    virtual ~wxInputFTPStream()
    {
        if ( LastError() == wxStream_NOERROR )
        {
            // wait for "226 transfer completed"
            m_ftp->CheckResult('2');

            m_ftp->m_streaming = FALSE;
        }
        else
        {
            m_ftp->Abort();
        }

        delete m_i_socket;
    }

    wxFTP *m_ftp;
    size_t m_ftpsize;
};

class wxOutputFTPStream : public wxSocketOutputStream
{
public:
    wxOutputFTPStream(wxFTP *ftp_clt, wxSocketBase *sock)
        : wxSocketOutputStream(*sock), m_ftp(ftp_clt)
    {
    }

    virtual ~wxOutputFTPStream(void)
    {
        if ( IsOk() )
        {
            // close data connection first, this will generate "transfer
            // completed" reply
            delete m_o_socket;

            // read this reply
            m_ftp->CheckResult('2');

            m_ftp->m_streaming = FALSE;
        }
        else
        {
            // abort data connection first
            m_ftp->Abort();

            // and close it after
            delete m_o_socket;
        }
    }

    wxFTP *m_ftp;
};

wxSocketClient *wxFTP::GetPort()
{
    int a[6];

    if ( !DoSimpleCommand(_T("PASV")) )
    {
        wxLogError(_("The FTP server doesn't support passive mode."));

        return NULL;
    }

    const char *addrStart = wxStrchr(m_lastResult, _T('('));
    if ( !addrStart )
    {
        m_lastError = wxPROTO_PROTERR;

        return NULL;
    }

    const char *addrEnd = wxStrchr(addrStart, _T(')'));
    if ( !addrEnd )
    {
        m_lastError = wxPROTO_PROTERR;

        return NULL;
    }

    wxString straddr(addrStart + 1, addrEnd);

    wxSscanf(straddr, wxT("%d,%d,%d,%d,%d,%d"),
             &a[2],&a[3],&a[4],&a[5],&a[0],&a[1]);

    wxUint32 hostaddr = (wxUint16)a[5] << 24 |
                        (wxUint16)a[4] << 16 |
                        (wxUint16)a[3] << 8 |
                        a[2];
    wxUint16 port = (wxUint16)a[0] << 8 | a[1];

    wxIPV4address addr;
    addr.Hostname(hostaddr);
    addr.Service(port);

    wxSocketClient *client = new wxSocketClient();
    if ( !client->Connect(addr) )
    {
        delete client;
        return NULL;
    }

    client->Notify(FALSE);

    return client;
}

bool wxFTP::Abort()
{
    if ( !m_streaming )
        return TRUE;

    m_streaming = FALSE;
    if ( !CheckCommand(wxT("ABOR"), '4') )
        return FALSE;

    return CheckResult('2');
}

wxInputStream *wxFTP::GetInputStream(const wxString& path)
{
    int pos_size;
    wxInputFTPStream *in_stream;

    if ( !m_modeSet && !SetTransferMode(BINARY) )
        return NULL;

    wxSocketClient *sock = GetPort();

    if ( !sock )
    {
        m_lastError = wxPROTO_NETERR;
        return NULL;
    }

    wxString tmp_str = wxT("RETR ") + wxURL::ConvertFromURI(path);
    if ( !CheckCommand(tmp_str, '1') )
        return NULL;

    m_streaming = TRUE;

    in_stream = new wxInputFTPStream(this, sock);

    pos_size = m_lastResult.Index(wxT('('));
    if ( pos_size != wxNOT_FOUND )
    {
        wxString str_size = m_lastResult(pos_size+1, m_lastResult.Index(wxT(')'))-1);

        in_stream->m_ftpsize = wxAtoi(WXSTRINGCAST str_size);
    }

    sock->SetFlags(wxSOCKET_WAITALL);

    return in_stream;
}

wxOutputStream *wxFTP::GetOutputStream(const wxString& path)
{
    if ( !m_modeSet && !SetTransferMode(BINARY) )
        return NULL;

    wxSocketClient *sock = GetPort();

    wxString tmp_str = wxT("STOR ") + path;
    if ( !CheckCommand(tmp_str, '1') )
        return FALSE;

    m_streaming = TRUE;

    return new wxOutputFTPStream(this, sock);
}

// ----------------------------------------------------------------------------
// FTP directory listing
// ----------------------------------------------------------------------------

bool wxFTP::GetList(wxArrayString& files,
                    const wxString& wildcard,
                    bool details)
{
    wxSocketBase *sock = GetPort();
    if (!sock)
        return FALSE;

    // NLST : List of Filenames (including Directory's !)
    // LIST : depending on BS of FTP-Server
    //        - Unix    : result like "ls" command
    //        - Windows : like "dir" command
    //        - others  : ?
    wxString line(details ? _T("LIST") : _T("NLST"));
    if ( !!wildcard )
    {
        line << _T(' ') << wildcard;
    }

    if (!CheckCommand(line, '1'))
    {
        return FALSE;
    }
    files.Empty();
    while ( ReadLine(sock, line) == wxPROTO_NOERR )
    {
        files.Add(line);
    }
    delete sock;

    // the file list should be terminated by "226 Transfer complete""
    if ( !CheckResult('2') )
        return FALSE;

    return TRUE;
}

#ifdef WXWIN_COMPATIBILITY_2
// deprecated
wxList *wxFTP::GetList(const wxString& wildcard, bool details)
{
 wxSocketBase *sock = GetPort();
 if (!sock)
  return FALSE;
 wxList *file_list = new wxList;
 wxString line;
 // NLST : List of Filenames (including Directory's !)
 // LIST : depending on BS of FTP-Server
 //        - Unix    : result like "ls" command
 //        - Windows : like "dir" command
 //        - others  : ?
 if (!details)
  line = _T("NLST");   // Default
 else
  line = _T("LIST");
 if (!wildcard.IsNull())
  line += wildcard;
 if (!CheckCommand(line, '1'))
 {
  delete sock;
  delete file_list;
  return NULL;
 }
 while (GetLine(sock, line) == wxPROTO_NOERR)
 {
  file_list->Append((wxObject *)(new wxString(line)));
 }
 if (!CheckResult('2'))
 {
  delete sock;
  file_list->DeleteContents(TRUE);
  delete file_list;
  return NULL;
 }
 return file_list;
}
#endif // WXWIN_COMPATIBILITY_2

#endif
  // wxUSE_SOCKETS
