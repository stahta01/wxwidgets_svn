/////////////////////////////////////////////////////////////////////////////
// Name:        file.cpp
// Purpose:     wxFile - encapsulates low-level "file descriptor"
//              wxTempFile
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "file.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_FILE

// standard
#if defined(__WXMSW__) && !defined(__GNUWIN32__) && !defined(__WXWINE__)
  #include  <io.h>

#ifndef __SALFORDC__
    #define   WIN32_LEAN_AND_MEAN
    #define   NOSERVICE
    #define   NOIME
    #define   NOATOM
    #define   NOGDI
    #define   NOGDICAPMASKS
    #define   NOMETAFILE
    #define   NOMINMAX
    #define   NOMSG
    #define   NOOPENFILE
    #define   NORASTEROPS
    #define   NOSCROLL
    #define   NOSOUND
    #define   NOSYSMETRICS
    #define   NOTEXTMETRIC
    #define   NOWH
    #define   NOCOMM
    #define   NOKANJI
    #define   NOCRYPT
    #define   NOMCX
#endif

    #include  <windows.h>     // for GetTempFileName
#elif (defined(__UNIX__) || defined(__GNUWIN32__))
    #include  <unistd.h>
    #ifdef __GNUWIN32__
        #include <windows.h>
    #endif
#elif (defined(__WXPM__))
    #include <io.h>
    #include <direct.h>
    #define   W_OK        2
    #define   R_OK        4
#elif (defined(__WXSTUBS__))
    // Have to ifdef this for different environments
    #include <io.h>
#elif (defined(__WXMAC__))
    int access( const char *path, int mode ) { return 0 ; }
    char* mktemp( char * path ) { return path ;}
    #include  <unistd.h>
    #include  <unix.h>
    #define   W_OK        2
    #define   R_OK        4
#else
    #error  "Please specify the header with file functions declarations."
#endif  //Win/UNIX

#include  <stdio.h>       // SEEK_xxx constants
#include  <fcntl.h>       // O_RDONLY &c

#ifndef __MWERKS__
    #include  <sys/types.h>   // needed for stat
    #include  <sys/stat.h>    // stat
#endif

// Microsoft compiler loves underscores, feed them to it
#ifdef  __VISUALC__
    // functions
    #define   open        _open
    #define   close       _close
    #define   read        _read
    #define   write       _write
    #define   lseek       _lseek
    #define   fsync       _commit
    #define   access      _access
    #define   eof         _eof

    // types
    #define   stat        _stat

    // constants

    #define   O_RDONLY    _O_RDONLY
    #define   O_WRONLY    _O_WRONLY
    #define   O_RDWR      _O_RDWR
    #define   O_EXCL      _O_EXCL
    #define   O_CREAT     _O_CREAT
    #define   O_BINARY    _O_BINARY

    #define   S_IFDIR     _S_IFDIR
    #define   S_IFREG     _S_IFREG
#else
    #define   tell(fd)    lseek(fd, 0, SEEK_CUR)
#endif  // VC++

#if defined(__BORLANDC__) || defined(_MSC_VER)
    #define   W_OK        2
    #define   R_OK        4
#endif

// there is no distinction between text and binary files under Unix
#ifdef    __UNIX__
    #define   O_BINARY    (0)
#endif  //__UNIX__

#ifdef __SALFORDC__
    #include <unix.h>
#endif

// wxWindows
#include  <wx/string.h>
#include  <wx/intl.h>
#include  <wx/file.h>
#include  <wx/log.h>

#ifndef MAX_PATH
    #define MAX_PATH 512
#endif

#ifdef __WXMAC__
    char gwxMacFileName[ MAX_PATH ] ;
    char gwxMacFileName2[ MAX_PATH ] ;
    char gwxMacFileName3[ MAX_PATH ] ;
#endif

// some broken compilers don't have 3rd argument in open() and creat()
#ifdef __SALFORDC__
    #define ACCESS(access)
    #define stat    _stat
#else // normal compiler
    #define ACCESS(access)  , (access)
#endif // Salford C

// ============================================================================
// implementation of wxFile
// ============================================================================

// ----------------------------------------------------------------------------
// static functions
// ----------------------------------------------------------------------------
bool wxFile::Exists(const wxChar *name)
{
    struct stat st;
#if wxUSE_UNICODE && wxMBFILES
    wxCharBuffer fname = wxConvFile.cWC2MB(name);

    return !access(fname, 0) &&
           !stat(MBSTRINGCAST fname, &st) &&
           (st.st_mode & S_IFREG);
#else
    return !access(name, 0) &&
           !stat((wxChar*) name, &st) &&
           (st.st_mode & S_IFREG);
#endif
}

bool wxFile::Access(const wxChar *name, OpenMode mode)
{
    int how = 0;

    switch ( mode ) {
        case read:
            how = R_OK;
            break;

        case write:
            how = W_OK;
            break;

        default:
            wxFAIL_MSG(_T("bad wxFile::Access mode parameter."));
    }

    return access(wxFNCONV(name), how) == 0;
}

// ----------------------------------------------------------------------------
// opening/closing
// ----------------------------------------------------------------------------

// ctors
wxFile::wxFile(const wxChar *szFileName, OpenMode mode)
{
    m_fd = fd_invalid;
    m_error = FALSE;

    Open(szFileName, mode);
}

// create the file, fail if it already exists and bOverwrite
bool wxFile::Create(const wxChar *szFileName, bool bOverwrite, int accessMode)
{
    // if bOverwrite we create a new file or truncate the existing one,
    // otherwise we only create the new file and fail if it already exists
    int fd = open(wxFNCONV(szFileName),
                  O_WRONLY | O_CREAT | (bOverwrite ? O_TRUNC : O_EXCL)
                  ACCESS(accessMode));

    if ( fd == -1 ) {
        wxLogSysError(_("can't create file '%s'"), szFileName);
        return FALSE;
    }
    else {
        Attach(fd);
        return TRUE;
    }
}

// open the file
bool wxFile::Open(const wxChar *szFileName, OpenMode mode, int accessMode)
{
    int flags = O_BINARY;

    switch ( mode ) {
        case read:
            flags |= O_RDONLY;
            break;

        case write:
            flags |= O_WRONLY | O_CREAT | O_TRUNC;
            break;

        case write_append:
            flags |= O_WRONLY | O_APPEND;
            break;

        case read_write:
            flags |= O_RDWR;
            break;
    }

    int fd = open(wxFNCONV(szFileName), flags ACCESS(accessMode));

    if ( fd == -1 ) {
        wxLogSysError(_("can't open file '%s'"), szFileName);
        return FALSE;
    }
    else {
        Attach(fd);
        return TRUE;
    }
}

// close
bool wxFile::Close()
{
    if ( IsOpened() ) {
        if ( close(m_fd) == -1 ) {
            wxLogSysError(_("can't close file descriptor %d"), m_fd);
            m_fd = fd_invalid;
            return FALSE;
        }
        else
            m_fd = fd_invalid;
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// read/write
// ----------------------------------------------------------------------------

// read
off_t wxFile::Read(void *pBuf, off_t nCount)
{
    wxCHECK( (pBuf != NULL) && IsOpened(), 0 );

#ifdef __MWERKS__
    int iRc = ::read(m_fd, (char*) pBuf, nCount);
#else
    int iRc = ::read(m_fd, pBuf, nCount);
#endif
    if ( iRc == -1 ) {
        wxLogSysError(_("can't read from file descriptor %d"), m_fd);
        return wxInvalidOffset;
    }
    else
        return (size_t)iRc;
}

// write
size_t wxFile::Write(const void *pBuf, size_t nCount)
{
    wxCHECK( (pBuf != NULL) && IsOpened(), 0 );

#ifdef __MWERKS__
    int iRc = ::write(m_fd, (const char*) pBuf, nCount);
#else
    int iRc = ::write(m_fd, pBuf, nCount);
#endif
    if ( iRc == -1 ) {
        wxLogSysError(_("can't write to file descriptor %d"), m_fd);
        m_error = TRUE;
        return 0;
    }
    else
        return iRc;
}

// flush
bool wxFile::Flush()
{
    if ( IsOpened() ) {
#if defined(__VISUALC__) || wxHAVE_FSYNC
        if ( fsync(m_fd) == -1 )
        {
            wxLogSysError(_("can't flush file descriptor %d"), m_fd);
            return FALSE;
        }
#else // no fsync
        // just do nothing
#endif // fsync
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// seek
// ----------------------------------------------------------------------------

// seek
off_t wxFile::Seek(off_t ofs, wxSeekMode mode)
{
    wxASSERT( IsOpened() );

    int origin;
    switch ( mode ) {
        default:
            wxFAIL_MSG(_("unknown seek origin"));

        case wxFromStart:
            origin = SEEK_SET;
            break;

        case wxFromCurrent:
            origin = SEEK_CUR;
            break;

        case wxFromEnd:
            origin = SEEK_END;
            break;
    }

    int iRc = lseek(m_fd, ofs, origin);
    if ( iRc == -1 ) {
        wxLogSysError(_("can't seek on file descriptor %d"), m_fd);
        return wxInvalidOffset;
    }
    else
        return (off_t)iRc;
}

// get current off_t
off_t wxFile::Tell() const
{
    wxASSERT( IsOpened() );

    int iRc = tell(m_fd);
    if ( iRc == -1 ) {
        wxLogSysError(_("can't get seek position on file descriptor %d"), m_fd);
        return wxInvalidOffset;
    }
    else
        return (off_t)iRc;
}

// get current file length
off_t wxFile::Length() const
{
    wxASSERT( IsOpened() );

#ifdef __VISUALC__
    int iRc = _filelength(m_fd);
#else // !VC++
    int iRc = tell(m_fd);
    if ( iRc != -1 ) {
        // @ have to use const_cast :-(
        int iLen = ((wxFile *)this)->SeekEnd();
        if ( iLen != -1 ) {
            // restore old position
            if ( ((wxFile *)this)->Seek(iRc) == -1 ) {
                // error
                iLen = -1;
            }
        }

        iRc = iLen;
    }
#endif  // VC++

    if ( iRc == -1 ) {
        wxLogSysError(_("can't find length of file on file descriptor %d"), m_fd);
        return wxInvalidOffset;
    }
    else
        return (off_t)iRc;
}

// is end of file reached?
bool wxFile::Eof() const
{
    wxASSERT( IsOpened() );

    int iRc;

#if defined(__UNIX__) || defined(__GNUWIN32__) || defined( __MWERKS__ ) || defined(__SALFORDC__)
    // @@ this doesn't work, of course, on unseekable file descriptors
    off_t ofsCur = Tell(),
    ofsMax = Length();
    if ( ofsCur == wxInvalidOffset || ofsMax == wxInvalidOffset )
        iRc = -1;
    else
        iRc = ofsCur == ofsMax;
#else  // Windows and "native" compiler
    iRc = eof(m_fd);
#endif // Windows/Unix

    switch ( iRc ) {
        case 1:
            break;

        case 0:
            return FALSE;

        case -1:
            wxLogSysError(_("can't determine if the end of file is reached on \
                descriptor %d"), m_fd);
                break;

        default:
            wxFAIL_MSG(_("invalid eof() return value."));
    }

    return TRUE;
}

// ============================================================================
// implementation of wxTempFile
// ============================================================================

// ----------------------------------------------------------------------------
// construction
// ----------------------------------------------------------------------------
wxTempFile::wxTempFile(const wxString& strName)
{
    Open(strName);
}

bool wxTempFile::Open(const wxString& strName)
{
    m_strName = strName;

    // we want to create the file in the same directory as strName because
    // otherwise rename() in Commit() might not work (if the files are on
    // different partitions for example). Unfortunately, the only standard
    // (POSIX) temp file creation function tmpnam() can't do it.
#if defined(__UNIX__) || defined(__WXSTUBS__)|| defined( __WXMAC__ )
    static const wxChar *szMktempSuffix = _T("XXXXXX");
    m_strTemp << strName << szMktempSuffix;
    mktemp(MBSTRINGCAST m_strTemp.mb_str()); // will do because length doesn't change
#elif  defined(__WXPM__)
    // for now just create a file
    // future enhancements can be to set some extended attributes for file systems
    // OS/2 supports that have them (HPFS, FAT32) and security (HPFS386)
    static const wxChar *szMktempSuffix = _T("XXX");
    m_strTemp << strName << szMktempSuffix;
    mkdir(m_strTemp.GetWriteBuf(MAX_PATH));
#else // Windows
    wxString strPath;
    wxSplitPath(strName, &strPath, NULL, NULL);
    if ( strPath.IsEmpty() )
        strPath = _T('.');  // GetTempFileName will fail if we give it empty string
#ifdef __WIN32__
    if ( !GetTempFileName(strPath, _T("wx_"),0, m_strTemp.GetWriteBuf(MAX_PATH)) )
#else
        // Not sure why MSVC++ 1.5 header defines first param as BYTE - bug?
        if ( !GetTempFileName((BYTE) (const wxChar*) strPath, _T("wx_"),0, m_strTemp.GetWriteBuf(MAX_PATH)) )
#endif
            wxLogLastError(_T("GetTempFileName"));
    m_strTemp.UngetWriteBuf();
#endif  // Windows/Unix

    int access = wxS_DEFAULT;
#ifdef __UNIX__
    // create the file with the same mode as the original one under Unix
    mode_t umaskOld = 0; // just to suppress compiler warning
    bool changedUmask;

    struct stat st;
    if ( stat(strName.fn_str(), &st) == 0 )
    {
        // this assumes that only lower bits of st_mode contain the access
        // rights, but it's true for at least all Unices which have S_IXXXX()
        // macros, so should not be less portable than using (not POSIX)
        // S_IFREG &c
        access = st.st_mode & 0777;

        // we want to create the file with exactly the same access rights as
        // the original one, so disable the user's umask for the moment
        umaskOld = umask(0);
        changedUmask = TRUE;
    }
    else
    {
        // file probably didn't exist, just create with default mode _using_
        // user's umask (new files creation should respet umask)
        changedUmask = FALSE;
    }
#endif // Unix

    bool ok = m_file.Open(m_strTemp, wxFile::write, access);

#ifdef __UNIX__
    if ( changedUmask )
    {
        // restore umask now that the file is created
        (void)umask(umaskOld);
    }
#endif // Unix

    return ok;
}

// ----------------------------------------------------------------------------
// destruction
// ----------------------------------------------------------------------------

wxTempFile::~wxTempFile()
{
    if ( IsOpened() )
        Discard();
}

bool wxTempFile::Commit()
{
    m_file.Close();

    if ( wxFile::Exists(m_strName) && remove(m_strName.fn_str()) != 0 ) {
        wxLogSysError(_("can't remove file '%s'"), m_strName.c_str());
        return FALSE;
    }

    if ( rename(m_strTemp.fn_str(), m_strName.fn_str()) != 0 ) {
        wxLogSysError(_("can't commit changes to file '%s'"), m_strName.c_str());
        return FALSE;
    }

    return TRUE;
}

void wxTempFile::Discard()
{
    m_file.Close();
    if ( remove(m_strTemp.fn_str()) != 0 )
        wxLogSysError(_("can't remove temporary file '%s'"), m_strTemp.c_str());
}

#endif

