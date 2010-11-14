///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/pipe.h
// Purpose:     wxPipe class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     24.06.2003 (extracted from src/unix/utilsunx.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_PIPE_H_
#define _WX_UNIX_PIPE_H_

#include <unistd.h>

#include "wx/log.h"
#include "wx/intl.h"

// ----------------------------------------------------------------------------
// wxPipe: this class encapsulates pipe() system call
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

    enum
    {
        INVALID_FD = -1
    };

    // default ctor doesn't do anything
    wxPipe() { m_fds[Read] = m_fds[Write] = INVALID_FD; }

    // create the pipe, return TRUE if ok, FALSE on error
    bool Create()
    {
        if ( pipe(m_fds) == -1 )
        {
            wxLogSysError(_("Pipe creation failed"));

            return false;
        }

        return true;
    }

    // switch the given end of the pipe to non-blocking IO
    bool MakeNonBlocking(Direction which)
    {
        const int flags = fcntl(m_fds[which], F_GETFL, 0);
        if ( flags == -1 )
            return false;

        return fcntl(m_fds[which], F_SETFL, flags | O_NONBLOCK) == 0;
    }

    // return TRUE if we were created successfully
    bool IsOk() const { return m_fds[Read] != INVALID_FD; }

    // return the descriptor for one of the pipe ends
    int operator[](Direction which) const { return m_fds[which]; }

    // detach a descriptor, meaning that the pipe dtor won't close it, and
    // return it
    int Detach(Direction which)
    {
        int fd = m_fds[which];
        m_fds[which] = INVALID_FD;

        return fd;
    }

    // close the pipe descriptors
    void Close()
    {
        for ( size_t n = 0; n < WXSIZEOF(m_fds); n++ )
        {
            if ( m_fds[n] != INVALID_FD )
            {
                close(m_fds[n]);
                m_fds[n] = INVALID_FD;
            }
        }
    }

    // dtor closes the pipe descriptors
    ~wxPipe() { Close(); }

private:
    int m_fds[2];
};

#if wxUSE_STREAMS && wxUSE_FILE

#include "wx/wfstream.h"

// ----------------------------------------------------------------------------
// wxPipeInputStream: stream for reading from a pipe
// ----------------------------------------------------------------------------

class wxPipeInputStream : public wxFileInputStream
{
public:
    wxPipeInputStream(int fd) : wxFileInputStream(fd) { }

    // return TRUE if the pipe is still opened
    bool IsOpened() const { return !Eof(); }

    // return TRUE if we have anything to read, don't block
    virtual bool CanRead() const;
};

// ----------------------------------------------------------------------------
// wxPipeOutputStream: stream for writing to a pipe
// ----------------------------------------------------------------------------

class wxPipeOutputStream : public wxFileOutputStream
{
public:
    wxPipeOutputStream(int fd) : wxFileOutputStream(fd) { }

    // Override the base class version to ignore "pipe full" errors: this is
    // not an error for this class.
    size_t OnSysWrite(const void *buffer, size_t size);
};

#endif // wxUSE_STREAMS && wxUSE_FILE

#endif // _WX_UNIX_PIPE_H_

