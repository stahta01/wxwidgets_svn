/////////////////////////////////////////////////////////////////////////////
// Name:        protocol.cpp
// Purpose:     Implement protocol base class
// Author:      Guilhem Lavaux
// Modified by:
// Created:     07/07/1997
// RCS-ID:      $Id$
// Copyright:   (c) 1997, 1998 Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "protocol.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_SOCKETS

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/protocol/protocol.h"
#include "wx/url.h"
#include "wx/module.h"

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
{
  m_protoname = name;
  m_servname = serv;
  m_cinfo = info;
  m_needhost = need_host1;
  next = wxURL::g_protocols;
  wxURL::g_protocols = this;
}

/////////////////////////////////////////////////////////////////
// wxProtocol ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

IMPLEMENT_ABSTRACT_CLASS(wxProtocol, wxSocketClient)

wxProtocol::wxProtocol()
 : wxSocketClient()
{
}

bool wxProtocol::Reconnect()
{
  wxIPV4address addr;

  if (!GetPeer(addr)) {
    Close();
    return FALSE;
  }
  if (!Close())
    return FALSE;
  if (!Connect(addr))
    return FALSE;
  return TRUE;
}

wxProtocolError GetLine(wxSocketBase *sock, wxString& result) {
#define PROTO_BSIZE 2048
  size_t avail, size;
  char tmp_buf[PROTO_BSIZE], tmp_str[PROTO_BSIZE];
  char *ret;
  bool found;

  avail = sock->Read(tmp_buf, PROTO_BSIZE).LastCount();
  if (sock->LastError() != 0 || avail == 0)
    return wxPROTO_NETERR;

  memcpy(tmp_str, tmp_buf, avail);

// Not implemented on all systems
// ret = (char *)memccpy(tmp_str, tmp_buf, '\n', avail);
  found = FALSE;
  for (ret=tmp_str;ret < (tmp_str+avail); ret++)
     if (*ret == '\n') {
       found = TRUE;
       break;
     }

  if (!found)
    return wxPROTO_PROTERR;
  *ret = 0;

  result = tmp_str;
  result = result.Left(result.Length()-1);

  size = ret-tmp_str+1;
  sock->CreatePushbackBefore(&tmp_buf[size], avail-size);
  return wxPROTO_NOERR;
#undef PROTO_BSIZE
}

// ----------------------------------------------------------------------
// Module
// ----------------------------------------------------------------------

class wxProtocolModule: public wxModule {
  DECLARE_DYNAMIC_CLASS(wxProtocolModule)
public:
  wxProtocolModule() {}
  bool OnInit();
  void OnExit();
};

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxProtocolModule, wxModule)
#endif

bool wxProtocolModule::OnInit()
{
  wxURL::g_proxy = new wxHTTP();
  return TRUE;
}

void wxProtocolModule::OnExit()
{
  delete wxURL::g_proxy;
  wxURL::g_proxy = NULL;
}

#endif
  // wxUSE_SOCKETS
