/////////////////////////////////////////////////////////////////////////////
// Name:        mstream.h
// Purpose:     Memory stream classes
// Author:      Guilhem Lavaux
// Modified by:
// Created:     11/07/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_WXMMSTREAM_H__
#define _WX_WXMMSTREAM_H__

#include <wx/stream.h>

#if wxUSE_STREAMS

class wxMemoryInputStream: public wxInputStream {
 private:
  size_t m_length;
  
 public:
  wxMemoryInputStream(const char *data, size_t length);
  virtual ~wxMemoryInputStream();
  virtual size_t StreamSize() const { return m_length; }

  char Peek();

  wxStreamBuffer *InputStreamBuffer() const { return m_i_streambuf; }

 protected:
  wxStreamBuffer *m_i_streambuf;

 protected:
  size_t OnSysRead(void *buffer, size_t nbytes);
  off_t OnSysSeek(off_t pos, wxSeekMode mode);
  off_t OnSysTell() const;
};

class wxMemoryOutputStream:  public wxOutputStream {
 public:
  wxMemoryOutputStream(char *data = NULL, size_t length = 0);
  virtual ~wxMemoryOutputStream();
  virtual size_t StreamSize() const { return m_o_streambuf->GetLastAccess(); }

  wxStreamBuffer *OutputStreamBuffer() const { return m_o_streambuf; }

 protected:
  wxStreamBuffer *m_o_streambuf;

 protected:
  size_t OnSysWrite(const void *buffer, size_t nbytes);
  off_t OnSysSeek(off_t pos, wxSeekMode mode);
  off_t OnSysTell() const;
};

#endif
  // wxUSE_STREAMS

#endif
  // _WX_WXMMSTREAM_H__

