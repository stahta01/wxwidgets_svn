///////////////////////////////////////////////////////////////////////////////
// Name:        unix/snglinst.cpp
// Purpose:     implements wxSingleInstanceChecker class for Unix using
//              lock files with fcntl(2) or flock(2)
// Author:      Vadim Zeitlin
// Modified by:
// Created:     09.06.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "snglinst.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_SNGLINST_CHECKER

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/file.h"
#endif //WX_PRECOMP

#include "wx/utils.h"           // wxGetHomeDir()

#include "wx/snglinst.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>             // for kill()

#ifdef HAVE_FCNTL
    #include <fcntl.h>
#elif defined(HAVE_FLOCK)
    #include <sys/file.h>
#else
    // normally, wxUSE_SNGLINST_CHECKER must have been reset by configure
    #error "wxSingleInstanceChecker can't be compiled on this platform"
#endif // fcntl()/flock()

// ----------------------------------------------------------------------------
// private functions: (exclusively) lock/unlock the file
// ----------------------------------------------------------------------------

enum LockOperation
{
    LOCK,
    UNLOCK
};

#ifdef HAVE_FCNTL

static int wxLockFile(int fd, LockOperation lock)
{
    // init the flock parameter struct
    struct flock fl;
    fl.l_type = lock == LOCK ? F_WRLCK : F_UNLCK;

    // lock the entire file
    fl.l_whence =
    fl.l_start =
    fl.l_len = 0;

    // is this needed?
    fl.l_pid = getpid();

    return fcntl(fd, F_SETLK, &fl);
}

#else // HAVE_FLOCK

static int wxLockFile(int fd, LockOperation lock)
{
    return flock(fd, lock == LOCK ? LOCK_EX | LOCK_NB : LOCK_UN);
}

#endif // fcntl()/flock()

// ----------------------------------------------------------------------------
// wxSingleInstanceCheckerImpl: the real implementation class
// ----------------------------------------------------------------------------

class wxSingleInstanceCheckerImpl
{
public:
    wxSingleInstanceCheckerImpl()
    {
        m_fdLock = -1;
        m_pidLocker = 0;
    }

    bool Create(const wxString& name);

    pid_t GetLockerPID() const { return m_pidLocker; }

    ~wxSingleInstanceCheckerImpl() { Unlock(); }

private:
    // try to create and lock the file
    bool CreateLockFile();

    // unlock and remove the lock file
    void Unlock();

    // the descriptor of our lock file, -1 if none
    int m_fdLock;

    // pid of the process owning the lock file
    pid_t m_pidLocker;

    // the name of the lock file
    wxString m_nameLock;
};

// ============================================================================
// wxSingleInstanceCheckerImpl implementation
// ============================================================================

bool wxSingleInstanceCheckerImpl::CreateLockFile()
{
    // try to open the file
    m_fdLock = open(m_nameLock,
                    O_WRONLY | O_CREAT | O_EXCL,
                    S_IREAD | S_IWRITE);

    if ( m_fdLock != -1 )
    {
        // try to lock it
        if ( wxLockFile(m_fdLock, LOCK) == 0 )
        {
            // fine, we have the exclusive lock to the file, write our PID
            // into it
            m_pidLocker = getpid();

            // use char here, not wxChar!
            char buf[256]; // enough for any PID size
            int len = sprintf(buf, "%d", m_pidLocker) + 1;

            if ( write(m_fdLock, buf, len) != len )
            {
                wxLogSysError(_("Failed to write to lock file '%s'"),
                              m_nameLock.c_str());

                Unlock();

                return FALSE;
            }

            fsync(m_fdLock);

            return TRUE;
        }

        // couldn't lock: this might have happened because of a race
        // condition: maybe another instance opened and locked the file
        // between our calls to open() and flock()
        close(m_fdLock);
        m_fdLock = -1;
    }

    // we didn't create and lock the file
    return FALSE;
}

bool wxSingleInstanceCheckerImpl::Create(const wxString& name)
{
    m_nameLock = name;

    if ( CreateLockFile() )
    {
        // nothing more to do
        return TRUE;
    }

    // try to open the file for reading and get the PID of the process
    // which has it
    wxFile file(name, wxFile::read);
    if ( !file.IsOpened() )
    {
        // well, this is really weird - file doesn't exist and we can't
        // create it
        //
        // normally, this just means that we don't have write access to
        // the directory where we try to create it, so return failure,
        // even it might also be a rare case of a race condition when
        // another process managed to open and lock the file and terminate
        // (erasing it) before we got here, but this should happen so
        // rarely in practice that we don't care
        wxLogError(_("Failed to access lock file."));

        return FALSE;
    }

    char buf[256];
    off_t count = file.Read(buf, WXSIZEOF(buf));
    if ( count == wxInvalidOffset )
    {
        wxLogError(_("Failed to read PID from lock file."));
    }
    else
    {
        if ( sscanf(buf, "%d", &m_pidLocker) == 1 )
        {
            if ( kill(m_pidLocker, 0) != 0 )
            {
                if ( unlink(name) != 0 )
                {
                    wxLogError(_("Failed to remove stale lock file '%s'."),
                               name.c_str());

                    // return TRUE in this case for now...
                }
                else
                {
                    wxLogMessage(_("Deleted stale lock file '%s'."),
                                 name.c_str());

                    // retry now
                    (void)CreateLockFile();
                }
            }
            //else: the other process is running
        }
        else
        {
            wxLogWarning(_("Invalid lock file '%s'."));
        }
    }

    // return TRUE if we could get the PID of the process owning the lock file
    // (whether it is still running or not), FALSE otherwise as it is
    // unexpected
    return m_pidLocker != 0;
}

void wxSingleInstanceCheckerImpl::Unlock()
{
    if ( m_fdLock != -1 )
    {
        if ( unlink(m_nameLock) != 0 )
        {
            wxLogSysError(_("Failed to remove lock file '%s'"),
                          m_nameLock.c_str());
        }

        if ( wxLockFile(m_fdLock, UNLOCK) != 0 )
        {
            wxLogSysError(_("Failed to unlock lock file '%s'"),
                          m_nameLock.c_str());
        }

        if ( close(m_fdLock) != 0 )
        {
            wxLogSysError(_("Failed to close lock file '%s'"),
                          m_nameLock.c_str());
        }
    }

    m_pidLocker = 0;
}

// ============================================================================
// wxSingleInstanceChecker implementation
// ============================================================================

bool wxSingleInstanceChecker::Create(const wxString& name,
                                     const wxString& path)
{
    wxASSERT_MSG( !m_impl,
                  _T("calling wxSingleInstanceChecker::Create() twice?") );

    // must have the file name to create a lock file
    wxASSERT_MSG( !name.empty(), _T("lock file name can't be empty") );

    m_impl = new wxSingleInstanceCheckerImpl;

    wxString fullname = path;
    if ( fullname.empty() )
    {
        fullname << wxGetHomeDir() << _T('/');
    }

    fullname << name;

    return m_impl->Create(fullname);
}

bool wxSingleInstanceChecker::IsAnotherRunning() const
{
    wxCHECK_MSG( m_impl, FALSE, _T("must call Create() first") );

    // if another instance is running, it must own the lock file - otherwise
    // we have it and the locker PID is ours one
    return m_impl->GetLockerPID() != getpid();
}

wxSingleInstanceChecker::~wxSingleInstanceChecker()
{
    delete m_impl;
}

#endif // wxUSE_SNGLINST_CHECKER

