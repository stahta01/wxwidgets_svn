/////////////////////////////////////////////////////////////////////////////
// Name:        wx/stream.h
// Purpose:     "wxWindows stream" base classes
// Author:      Guilhem Lavaux
// Modified by:
// Created:     11/07/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WXSTREAM_H__
#define _WX_WXSTREAM_H__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"

#if wxUSE_STREAMS

#include <stdio.h>
#include "wx/object.h"
#include "wx/string.h"
#include "wx/filefn.h"  // for off_t, wxInvalidOffset and wxSeekMode

class WXDLLEXPORT wxStreamBase;
class WXDLLEXPORT wxInputStream;
class WXDLLEXPORT wxOutputStream;

typedef wxInputStream& (*__wxInputManip)(wxInputStream&);
typedef wxOutputStream& (*__wxOutputManip)(wxOutputStream&);

WXDLLEXPORT wxOutputStream& wxEndL(wxOutputStream& o_stream);

// ---------------------------------------------------------------------------
// wxStream: base classes
// ---------------------------------------------------------------------------

enum wxStreamError
{
    wxSTREAM_NO_ERROR = 0,
    wxSTREAM_NO_ERR = wxSTREAM_NO_ERROR,
    wxSTREAM_NOERROR = wxSTREAM_NO_ERROR,

    wxSTREAM_EOF,

    wxSTREAM_WRITE_ERROR,
    wxSTREAM_WRITE_ERR = wxSTREAM_WRITE_ERROR,

    wxSTREAM_READ_ERROR,
    wxSTREAM_READ_ERR = wxSTREAM_READ_ERROR
};

// compatibility
#define wxStream_NOERROR    wxSTREAM_NOERROR
#define wxStream_EOF        wxSTREAM_EOF
#define wxStream_WRITE_ERR  wxSTREAM_WRITE_ERROR
#define wxStream_READ_ERR   wxSTREAM_READ_ERROR

class WXDLLEXPORT wxStreamBase
{
public:
    wxStreamBase();
    virtual ~wxStreamBase();

    // error testing
    wxStreamError GetLastError() const { return m_lasterror; }
    bool IsOk() const { return LastError() == wxSTREAM_NOERROR; }
    bool operator!() const { return LastError() != wxSTREAM_NOERROR; }

    virtual size_t GetSize() const { return 0; }

    // deprecated, for compatibility only
    wxStreamError LastError() const { return m_lasterror; }
    size_t StreamSize() const { return GetSize(); }

protected:
    // VZ: these functions are really pure virtual and shouldn't be declared
    //     in the base class because it creates ambiguties in stream classes
    //     deriving from both wxInputStream and wxOutputStream
#if 0
    virtual size_t OnSysRead(void *buffer, size_t bufsize);
    virtual size_t OnSysWrite(const void *buffer, size_t bufsize);
#endif

    virtual off_t OnSysSeek(off_t seek, wxSeekMode mode);
    virtual off_t OnSysTell() const;

    friend class wxStreamBuffer;

    size_t m_lastcount;
    wxStreamError m_lasterror;
};

class WXDLLEXPORT wxInputStream : /* virtual */ public wxStreamBase
{
public:
    wxInputStream();
    virtual ~wxInputStream();

    // is the stream at EOF?
    virtual bool Eof() const;

    // IO functions
    virtual char Peek();
    char GetC();
    virtual wxInputStream& Read(void *buffer, size_t size);
    wxInputStream& Read(wxOutputStream& stream_out);

    // Position functions
    virtual off_t SeekI(off_t pos, wxSeekMode mode = wxFromStart);
    virtual off_t TellI() const;

    // State functions
    virtual size_t LastRead() { return wxStreamBase::m_lastcount; }

    // Ungetch
    size_t Ungetch(const void *buffer, size_t size);
    bool Ungetch(char c);

    // Operators
    wxInputStream& operator>>(wxOutputStream& out) { return Read(out); }
    wxInputStream& operator>>( __wxInputManip func) { return func(*this); }

protected:
    // to be implemented in the derived classes (it should have been pure
    // virtual)
    virtual size_t OnSysRead(void *buffer, size_t bufsize);

    // Ungetch managers
    char *m_wback;
    size_t m_wbacksize;
    size_t m_wbackcur;

    char *AllocSpaceWBack(size_t needed_size);
    size_t GetWBack(void *buf, size_t bsize);

    friend class wxStreamBuffer;
};

class WXDLLEXPORT wxOutputStream : /* virtual */ public wxStreamBase
{
public:
    wxOutputStream();
    virtual ~wxOutputStream();

    void PutC(char c);
    virtual wxOutputStream& Write(const void *buffer, size_t size);
    wxOutputStream& Write(wxInputStream& stream_in);

    virtual off_t SeekO(off_t pos, wxSeekMode mode = wxFromStart);
    virtual off_t TellO() const;

    virtual size_t LastWrite() const { return wxStreamBase::m_lastcount; }

    virtual void Sync();

    wxOutputStream& operator<<(wxInputStream& out) { return Write(out); }
    wxOutputStream& operator<<( __wxOutputManip func) { return func(*this); }

protected:
    // to be implemented in the derived classes (it should have been pure
    // virtual)
    virtual size_t OnSysWrite(const void *buffer, size_t bufsize);

    friend class wxStreamBuffer;
};

// ---------------------------------------------------------------------------
// A stream for measuring streamed output
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxCountingOutputStream : public wxOutputStream
{
public:
    wxCountingOutputStream();

    size_t GetSize() const;
    bool Ok() const { return TRUE; }

protected:
    virtual size_t OnSysWrite(const void *buffer, size_t size);
    virtual off_t OnSysSeek(off_t pos, wxSeekMode mode);
    virtual off_t OnSysTell() const;

    size_t m_currentPos;
};

// ---------------------------------------------------------------------------
// "Filter" streams
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxFilterInputStream : public wxInputStream
{
public:
    wxFilterInputStream();
    wxFilterInputStream(wxInputStream& stream);
    virtual ~wxFilterInputStream();

    char Peek() { return m_parent_i_stream->Peek(); }

    size_t GetSize() const { return m_parent_i_stream->GetSize(); }

    wxInputStream *GetFilterInputStream() const { return m_parent_i_stream; }

protected:
    wxInputStream *m_parent_i_stream;
};

class WXDLLEXPORT wxFilterOutputStream : public wxOutputStream
{
public:
    wxFilterOutputStream();
    wxFilterOutputStream(wxOutputStream& stream);
    virtual ~wxFilterOutputStream();

    size_t GetSize() const { return m_parent_o_stream->GetSize(); }

    wxOutputStream *GetFilterOutputStream() const { return m_parent_o_stream; }

protected:
    wxOutputStream *m_parent_o_stream;
};

// ---------------------------------------------------------------------------
// Stream buffer: this class can be derived from and passed to
// wxBufferedStreams to implement custom buffering
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxStreamBuffer
{
public:
    enum BufMode
    {
        read,
        write,
        read_write
    };

    wxStreamBuffer(wxStreamBase& stream, BufMode mode);
    wxStreamBuffer(BufMode mode);
    wxStreamBuffer(const wxStreamBuffer& buf);
    virtual ~wxStreamBuffer();

    // Filtered IO
    virtual size_t Read(void *buffer, size_t size);
    size_t Read(wxStreamBuffer *buf);
    virtual size_t Write(const void *buffer, size_t size);
    size_t Write(wxStreamBuffer *buf);

    virtual char Peek();
    virtual char GetChar();
    virtual void PutChar(char c);
    virtual off_t Tell() const;
    virtual off_t Seek(off_t pos, wxSeekMode mode);

    // Buffer control
    void ResetBuffer();

    // NB: the buffer must always be allocated with malloc() if takeOwn is
    //     TRUE as it will be deallocated by free()
    void SetBufferIO(void *start, void *end, bool takeOwnership = FALSE);
    void SetBufferIO(void *start, size_t len, bool takeOwnership = FALSE);
    void SetBufferIO(size_t bufsize);
    void *GetBufferStart() const { return m_buffer_start; }
    void *GetBufferEnd() const { return m_buffer_end; }
    void *GetBufferPos() const { return m_buffer_pos; }
    size_t GetBufferSize() const { return m_buffer_size; }
    size_t GetIntPosition() const { return m_buffer_pos - m_buffer_start; }
    void SetIntPosition(size_t pos) { m_buffer_pos = m_buffer_start + pos; }
    size_t GetLastAccess() const { return m_buffer_end - m_buffer_start; }
    size_t GetBytesLeft() const { return m_buffer_end - m_buffer_pos; }

    void Fixed(bool fixed) { m_fixed = fixed; }
    void Flushable(bool f) { m_flushable = f; }

    bool FlushBuffer();
    bool FillBuffer();
    size_t GetDataLeft();

    // misc accessors
    wxStreamBase *GetStream() const { return m_stream; }
    bool HasBuffer() const { return m_buffer_size != 0; }

    bool IsFixed() const { return m_fixed; }
    bool IsFlushable() const { return m_flushable; }

    // only for input/output buffers respectively, returns NULL otherwise
    wxInputStream *GetInputStream() const;
    wxOutputStream *GetOutputStream() const;

    // deprecated, for compatibility only
    wxStreamBase *Stream() { return m_stream; }

protected:
    void GetFromBuffer(void *buffer, size_t size);
    void PutToBuffer(const void *buffer, size_t size);

    // set the last error to the specified value if we didn't have it before
    void SetError(wxStreamError err);

    // common part of several ctors
    void Init();

    // init buffer variables to be empty
    void InitBuffer();

    // free the buffer (always safe to call)
    void FreeBuffer();

    // the buffer itself: the pointers to its start and end and the current
    // position in the buffer
    char *m_buffer_start,
         *m_buffer_end,
         *m_buffer_pos;

    // the buffer size
    // FIXME: isn't it the same as m_buffer_end - m_buffer_start? (VZ)
    size_t m_buffer_size;

    // the stream we're associated with
    wxStreamBase *m_stream;

    // its mode
    BufMode m_mode;

    // flags
    bool m_destroybuf,      // deallocate buffer?
         m_destroystream,   // delete associated stream?
         m_fixed,
         m_flushable;
};

// ---------------------------------------------------------------------------
// wxBufferedStreams
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxBufferedInputStream : public wxFilterInputStream
{
public:
    // if a non NULL buffer is given to the stream, it will be deleted by it
    wxBufferedInputStream(wxInputStream& stream,
                          wxStreamBuffer *buffer = NULL);
    virtual ~wxBufferedInputStream();

    char Peek();
    wxInputStream& Read(void *buffer, size_t size);

    // Position functions
    off_t SeekI(off_t pos, wxSeekMode mode = wxFromStart);
    off_t TellI() const;

    // the buffer given to the stream will be deleted by it
    void SetInputStreamBuffer(wxStreamBuffer *buffer);
    wxStreamBuffer *GetInputStreamBuffer() const { return m_i_streambuf; }

    // deprecated, for compatibility only
    wxStreamBuffer *InputStreamBuffer() const { return m_i_streambuf; }

protected:
    virtual size_t OnSysRead(void *buffer, size_t bufsize);
    virtual off_t OnSysSeek(off_t seek, wxSeekMode mode);
    virtual off_t OnSysTell() const;

    wxStreamBuffer *m_i_streambuf;
};

class WXDLLEXPORT wxBufferedOutputStream : public wxFilterOutputStream
{
public:
    // if a non NULL buffer is given to the stream, it will be deleted by it
    wxBufferedOutputStream(wxOutputStream& stream,
                           wxStreamBuffer *buffer = NULL);
    virtual ~wxBufferedOutputStream();

    wxOutputStream& Write(const void *buffer, size_t size);

    // Position functions
    off_t SeekO(off_t pos, wxSeekMode mode = wxFromStart);
    off_t TellO() const;

    void Sync();

    size_t GetSize() const;

    // the buffer given to the stream will be deleted by it
    void SetOutputStreamBuffer(wxStreamBuffer *buffer);
    wxStreamBuffer *GetOutputStreamBuffer() const { return m_o_streambuf; }

    // deprecated, for compatibility only
    wxStreamBuffer *OutputStreamBuffer() const { return m_o_streambuf; }

protected:
    virtual size_t OnSysWrite(const void *buffer, size_t bufsize);
    virtual off_t OnSysSeek(off_t seek, wxSeekMode mode);
    virtual off_t OnSysTell() const;

    wxStreamBuffer *m_o_streambuf;
};

#endif // wxUSE_STREAMS

#endif // _WX_WXSTREAM_H__

