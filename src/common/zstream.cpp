//////////////////////////////////////////////////////////////////////////////
// Name:        zstream.cpp
// Purpose:     Compressed stream classes
// Author:      Guilhem Lavaux
// Modified by: Mike Wetherell
// Created:     11/07/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "zstream.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_ZLIB && wxUSE_STREAMS

#include "wx/zstream.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/log.h"

// normally, the compiler options should contain -I../zlib, but it is
// apparently not the case for all MSW makefiles and so, unless we use
// configure (which defines __WX_SETUP_H__) or it is explicitly overridden by
// the user (who can define wxUSE_ZLIB_H_IN_PATH), we hardcode the path here
#if defined(__WXMSW__) && !defined(__WX_SETUP_H__) && !defined(wxUSE_ZLIB_H_IN_PATH)
   #include "../zlib/zlib.h"
#else
   #include "zlib.h"
#endif

enum {
    ZSTREAM_BUFFER_SIZE = 16384
};

//////////////////////
// wxZlibInputStream
//////////////////////

wxZlibInputStream::wxZlibInputStream(wxInputStream& stream, int flags)
  : wxFilterInputStream(stream)
{
  m_inflate = NULL;
  m_z_buffer = new unsigned char[ZSTREAM_BUFFER_SIZE];
  m_z_size = ZSTREAM_BUFFER_SIZE;
  m_pos = 0;

  if (m_z_buffer) {
    m_inflate = new z_stream_s;

    if (m_inflate) {
      m_inflate->zalloc = (alloc_func)0;
      m_inflate->zfree = (free_func)0;
      m_inflate->opaque = (voidpf)0;
      m_inflate->avail_in = 0;
      m_inflate->next_in = NULL;
      m_inflate->next_out = NULL;

      wxASSERT((flags & ~(wxZLIB_ZLIB | wxZLIB_GZIP)) == 0);
 
      // when autodetecting between gzip & zlib, silently drop gzip flag
      // if the version of zlib doesn't support it
      if (flags == (wxZLIB_ZLIB | wxZLIB_GZIP)
              && strcmp(zlib_version, "1.2.") < 0)
        flags &= ~wxZLIB_GZIP;

      int bits = flags ? MAX_WBITS : -MAX_WBITS;
      if (flags & wxZLIB_GZIP)
        bits |= (flags & wxZLIB_ZLIB) ? 0x20 : 0x10;

      if (inflateInit2(m_inflate, bits) == Z_OK)
        return;
    }
  }

  wxLogError(_("Can't initialize zlib inflate stream."));
  m_lasterror = wxSTREAM_READ_ERROR;
}

wxZlibInputStream::~wxZlibInputStream()
{
  inflateEnd(m_inflate);
  delete m_inflate;

  delete [] m_z_buffer;
}

size_t wxZlibInputStream::OnSysRead(void *buffer, size_t size)
{
  wxASSERT_MSG(m_inflate && m_z_buffer, wxT("Inflate stream not open"));

  if (!m_inflate || !m_z_buffer)
    m_lasterror = wxSTREAM_READ_ERROR;
  if (!IsOk() || !size)
    return 0;

  int err = Z_OK;
  m_inflate->next_out = (unsigned char *)buffer;
  m_inflate->avail_out = size;

  while (err == Z_OK && m_inflate->avail_out > 0) {
    if (m_inflate->avail_in == 0) {
      m_parent_i_stream->Read(m_z_buffer, m_z_size);
      m_inflate->next_in = m_z_buffer;
      m_inflate->avail_in = m_parent_i_stream->LastRead();

      if (m_inflate->avail_in == 0) {
        if (m_parent_i_stream->Eof())
          wxLogError(_("Can't read inflate stream: unexpected EOF in underlying stream."));
        m_lasterror = wxSTREAM_READ_ERROR;
        break;
      }
    }
    err = inflate(m_inflate, Z_NO_FLUSH);
  }

  if (err == Z_STREAM_END) {
    // Unread any data taken from past the end of the deflate stream, so that
    // any additional data can be read from the underlying stream (the crc
    // in a gzip for example)
    if (m_inflate->avail_in) {
      m_parent_i_stream->Ungetch(m_inflate->next_in, m_inflate->avail_in);
      m_inflate->avail_in = 0;
    }
    m_lasterror = wxSTREAM_EOF;
  } else if (err != Z_OK) {
    wxString msg(m_inflate->msg, *wxConvCurrent);
    if (!msg)
      msg.Format(_("zlib error %d"), err);
    wxLogError(_("Can't read from inflate stream: %s\n"), msg.c_str());
    m_lasterror = wxSTREAM_READ_ERROR;
  }

  size -= m_inflate->avail_out;
  m_pos += size;
  return size;
}

//////////////////////
// wxZlibOutputStream
//////////////////////

wxZlibOutputStream::wxZlibOutputStream(wxOutputStream& stream,
                                       int level,
                                       int flags)
 : wxFilterOutputStream(stream)
{
  m_deflate = NULL;
  m_z_buffer = new unsigned char[ZSTREAM_BUFFER_SIZE];
  m_z_size = ZSTREAM_BUFFER_SIZE;
  m_pos = 0;

  if ( level == -1 )
  {
    level = Z_DEFAULT_COMPRESSION;
  }
  else
  {
    wxASSERT_MSG(level >= 0 && level <= 9, wxT("wxZlibOutputStream compression level must be between 0 and 9!"));
  }

  if (m_z_buffer) {
     m_deflate = new z_stream_s;

     if (m_deflate) {
        m_deflate->zalloc = (alloc_func)0;
        m_deflate->zfree = (free_func)0;
        m_deflate->opaque = (voidpf)0;
        m_deflate->avail_in = 0;
        m_deflate->next_out = m_z_buffer;
        m_deflate->avail_out = m_z_size;

        wxASSERT(flags == 0 || flags == wxZLIB_ZLIB || flags == wxZLIB_GZIP);
 
        int bits = flags ? MAX_WBITS : -MAX_WBITS;
        if (flags & wxZLIB_GZIP)
            bits |= 0x10;

        if (deflateInit2(m_deflate, level, Z_DEFLATED, bits, 
                         8, Z_DEFAULT_STRATEGY) == Z_OK)
          return;
     }
  }

  wxLogError(_("Can't initialize zlib deflate stream."));
  m_lasterror = wxSTREAM_WRITE_ERROR;
}

wxZlibOutputStream::~wxZlibOutputStream()
{
  if (m_deflate && m_z_buffer)
    DoFlush(true);
  deflateEnd(m_deflate);
  delete m_deflate;

  delete[] m_z_buffer;
}

void wxZlibOutputStream::DoFlush(bool final)
{
  wxASSERT_MSG(m_deflate && m_z_buffer, wxT("Deflate stream not open"));

  if (!m_deflate || !m_z_buffer)
    m_lasterror = wxSTREAM_WRITE_ERROR;
  if (!IsOk())
    return;

  int err = Z_OK;
  bool done = false;

  while (err == Z_OK || err == Z_STREAM_END) {
    size_t len = m_z_size  - m_deflate->avail_out;
    if (len) {
      if (m_parent_o_stream->Write(m_z_buffer, len).LastWrite() != len) {
        m_lasterror = wxSTREAM_WRITE_ERROR;
        wxLogDebug(wxT("wxZlibOutputStream: Error writing to underlying stream"));
        break;
      }
      m_deflate->next_out = m_z_buffer;
      m_deflate->avail_out = m_z_size;
    }

    if (done)
      break;
    err = deflate(m_deflate, final ? Z_FINISH : Z_FULL_FLUSH);
    done = m_deflate->avail_out != 0 || err == Z_STREAM_END;
  }
}

size_t wxZlibOutputStream::OnSysWrite(const void *buffer, size_t size)
{
  wxASSERT_MSG(m_deflate && m_z_buffer, wxT("Deflate stream not open"));

  if (!m_deflate || !m_z_buffer)
    m_lasterror = wxSTREAM_WRITE_ERROR;
  if (!IsOk() || !size)
    return 0;

  int err = Z_OK;
  m_deflate->next_in = (unsigned char *)buffer;
  m_deflate->avail_in = size;

  while (err == Z_OK && m_deflate->avail_in > 0) {
    if (m_deflate->avail_out == 0) {
      m_parent_o_stream->Write(m_z_buffer, m_z_size);
      if (m_parent_o_stream->LastWrite() != m_z_size) {
        m_lasterror = wxSTREAM_WRITE_ERROR;
        wxLogDebug(wxT("wxZlibOutputStream: Error writing to underlying stream"));
        break;
      }

      m_deflate->next_out = m_z_buffer;
      m_deflate->avail_out = m_z_size;
    }

    err = deflate(m_deflate, Z_NO_FLUSH);
  }

  if (err != Z_OK) {
    m_lasterror = wxSTREAM_WRITE_ERROR;
    wxString msg(m_deflate->msg, *wxConvCurrent);
    if (!msg)
      msg.Format(_("zlib error %d"), err);
    wxLogError(_("Can't write to deflate stream: %s\n"), msg.c_str());
  }

  size -= m_deflate->avail_in;
  m_pos += size;
  return size;
}

#endif
  // wxUSE_ZLIB && wxUSE_STREAMS
  
