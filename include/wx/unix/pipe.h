///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/pipe.h
// Purpose:     wxPipe class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     24.06.2003 (extracted from src/unix/utilsunx.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
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

            return FALSE;
        }

        return TRUE;
    }

    // return TRUE if we were created successfully
    bool IsOk() const { return m_fds[Read] != INVALID_FD; }

    // return the descriptor for one of the pipe ends
    int operator[](Direction which) const
    {
        wxASSERT_MSG( which >= 0 && (size_t)which < WXSIZEOF(m_fds),
                      _T("invalid pipe index") );

        return m_fds[which];
    }

    // detach a descriptor, meaning that the pipe dtor won't close it, and
    // return it
    int Detach(Direction which)
    {
        wxASSERT_MSG( which >= 0 && (size_t)which < WXSIZEOF(m_fds),
                      _T("invalid pipe index") );

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
                close(m_fds[n]);
        }
    }

    // dtor closes the pipe descriptors
    ~wxPipe() { Close(); }

private:
    int m_fds[2];
};

#endif // _WX_UNIX_PIPE_H_

