/////////////////////////////////////////////////////////////////////////////
// Name:        sckstrm.h
// Purpose:     wxSocket*Stream
// Author:      Guilhem Lavaux
// Modified by:
// Created:     17/07/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
#pragma implementation "sckstrm.h"
#endif

#include "wx/stream.h"
#include "wx/socket.h"
#include "wx/sckstrm.h"

// ---------------------------------------------------------------------------
// wxSocketOutputStream
// ---------------------------------------------------------------------------

wxSocketOutputStream::wxSocketOutputStream(wxSocketBase& s)
  : m_o_socket(&s)
{
}

wxSocketOutputStream::~wxSocketOutputStream()
{
}

wxOutputStream& wxSocketOutputStream::Write(const void *buffer, size_t size)
{
  m_o_socket->Write((const char *)buffer, size);
  return *this;
}

// ---------------------------------------------------------------------------
// wxSocketInputStream
// ---------------------------------------------------------------------------

wxSocketInputStream::wxSocketInputStream(wxSocketBase& s)
 : m_i_socket(&s)
{
}

wxSocketInputStream::~wxSocketInputStream()
{
}

wxInputStream& wxSocketInputStream::Read(void *buffer, size_t size)
{
  m_i_socket->Read((char *)buffer, size);
  return *this;
}

// ---------------------------------------------------------------------------
// wxSocketStream (IO)
// ---------------------------------------------------------------------------
wxSocketStream::wxSocketStream(wxSocketBase& i_s, wxSocketBase& o_s)
  : wxSocketInputStream(i_s), wxSocketOutputStream(o_s)
{
}

wxSocketStream::wxSocketStream(wxSocketBase& s)
  : wxSocketInputStream(s), wxSocketOutputStream(s)
{
}
