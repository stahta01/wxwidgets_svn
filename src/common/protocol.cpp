/////////////////////////////////////////////////////////////////////////////
// Name:        protocol.cpp
// Purpose:     Implement protocol base class
// Author:      Guilhem Lavaux
// Modified by:
// Created:     07/07/1997
// RCS-ID:      $Id$
// Copyright:   (c) 1997, 1998 Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "protocol.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_PROTOCOL

#include "wx/protocol/protocol.h"
#include "wx/url.h"
#include "wx/module.h"

#include <stdlib.h>

/////////////////////////////////////////////////////////////////
// wxProtoInfo
/////////////////////////////////////////////////////////////////

/*
 * --------------------------------------------------------------
 * --------- wxProtoInfo CONSTRUCTOR ----------------------------
 * --------------------------------------------------------------
 */

wxProtoInfo::wxProtoInfo(const wxChar *name, const wxChar *serv,
                         const bool need_host1, wxClassInfo *info)
           : m_protoname(name),
             m_servname(serv)
{
    m_cinfo = info;
    m_needhost = need_host1;
    next = wxURL::ms_protocols;
    wxURL::ms_protocols = this;
}

/////////////////////////////////////////////////////////////////
// wxProtocol ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#if wxUSE_SOCKETS
IMPLEMENT_ABSTRACT_CLASS(wxProtocol, wxSocketClient)
#else
IMPLEMENT_ABSTRACT_CLASS(wxProtocol, wxObject)
#endif

wxProtocol::wxProtocol()
#if wxUSE_SOCKETS
 : wxSocketClient()
#endif
{
}

#if wxUSE_SOCKETS
bool wxProtocol::Reconnect()
{
    wxIPV4address addr;

    if (!GetPeer(addr))
    {
        Close();
        return FALSE;
    }

    if (!Close())
        return FALSE;
        
    if (!Connect(addr))
        return FALSE;

    return TRUE;
}

// ----------------------------------------------------------------------------
// Read a line from socket
// ----------------------------------------------------------------------------

/* static */
wxProtocolError wxProtocol::ReadLine(wxSocketBase *socket, wxString& result)
{
    static const int LINE_BUF = 4095;

    result.clear();

    wxCharBuffer buf(LINE_BUF);
    while ( sock->WaitForRead() )
    {
        // peek at the socket to see if there is a CRLF
        sock->Peek(buf.data(), LINE_BUF);

        size_t nRead = sock->LastCount();
        if ( !nRead && sock->Error() )
            return wxPROTO_NETERR;

        // look for "\r\n" paying attention to a special case: "\r\n" could
        // have been split by buffer boundary, so check also for \r at the end
        // of the last chunk and \n at the beginning of this one
        buf.data()[nRead] = '\0';
        const char *eol = strchr(buf, '\n');

        // if we found '\n', is there a '\r' as well?
        if ( eol )
        {
            if ( eol == buf.data() )
            {
                // check for case of "\r\n" being split
                if ( result.empty() || result.Last() != _T('\r') )
                {
                    // ignore the stray '\n'
                    eol = NULL;
                }
                //else: ok, got real EOL

                // read just this '\n' and restart
                nRead = 1;
            }
            else // '\n' in the middle of the buffer
            {
                // in any case, read everything up to and including '\n'
                nRead = eol - buf + 1;

                if ( eol[-1] != '\r' )
                {
                    // as above, simply ignore stray '\n'
                    eol = NULL;
                }
            }
        }

        sock->Read(buf.data(), nRead);
        if ( sock->LastCount() != nRead )
            return wxPROTO_NETERR;

        buf.data()[nRead] = '\0';
        result += wxString::FromAscii(buf);

        if ( eol )
        {
            // remove trailing "\r\n"
            result.RemoveLast(2);

            return wxPROTO_NOERR;
        }
    }

    return wxPROTO_NETERR;
}

wxProtocolError wxProtocol::ReadLine(wxString& result)
{
    return ReadLine(this, result);
}

// old function which only chops '\n' and not '\r\n'
wxProtocolError GetLine(wxSocketBase *sock, wxString& result)
{
#define PROTO_BSIZE 2048
    size_t avail, size;
    char tmp_buf[PROTO_BSIZE], tmp_str[PROTO_BSIZE];
    char *ret;
    bool found;

    avail = sock->Read(tmp_buf, PROTO_BSIZE).LastCount();
    if (sock->Error() || avail == 0)
        return wxPROTO_NETERR;

    memcpy(tmp_str, tmp_buf, avail);

    // Not implemented on all systems
    // ret = (char *)memccpy(tmp_str, tmp_buf, '\n', avail);
    found = FALSE;
    for (ret=tmp_str;ret < (tmp_str+avail); ret++)
        if (*ret == '\n') 
        {
            found = TRUE;
            break;
        }

    if (!found)
        return wxPROTO_PROTERR;
        
    *ret = 0;

    result = wxString::FromAscii( tmp_str );
    result = result.Left(result.Length()-1);

    size = ret-tmp_str+1;
    sock->Unread(&tmp_buf[size], avail-size);
  
    return wxPROTO_NOERR;
#undef PROTO_BSIZE
}
#endif // wxUSE_SOCKETS

#endif // wxUSE_PROTOCOL

