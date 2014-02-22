/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/filefn.cpp
// Purpose:     File- and directory-related functions
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/filefn.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/utils.h"
    #include "wx/crt.h"
#endif

#include "wx/dynarray.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/dir.h"

#include "wx/scopedptr.h"
#include "wx/tokenzr.h"

// there are just too many of those...
#ifdef __VISUALC__
    #pragma warning(disable:4706)   // assignment within conditional expression
#endif // VC++

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !wxONLY_WATCOM_EARLIER_THAN(1,4)
    #if !(defined(_MSC_VER) && (_MSC_VER > 800))
        #include <errno.h>
    #endif
#endif

#if defined(__WXMAC__)
    #include  "wx/osx/private.h"  // includes mac headers
#endif

#ifdef __WINDOWS__
    #include "wx/msw/private.h"
    #include "wx/msw/missing.h"
    #include "wx/msw/mslu.h"

    // sys/cygwin.h is needed for cygwin_conv_to_full_win32_path()
    // and for cygwin_conv_path()
    //
    // note that it must be included after <windows.h>
    #ifdef __GNUWIN32__
        #ifdef __CYGWIN__
            #include <sys/cygwin.h>
            #include <cygwin/version.h>
        #endif
    #endif // __GNUWIN32__

    // io.h is needed for _get_osfhandle()
    // Already included by filefn.h for many Windows compilers
    #if defined __CYGWIN__
        #include <io.h>
    #endif
#endif // __WINDOWS__

#if defined(__VMS__)
    #include <fab.h>
#endif

// TODO: Borland probably has _wgetcwd as well?
#ifdef _MSC_VER
    #define HAVE_WGETCWD
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#ifndef _MAXPATHLEN
    #define _MAXPATHLEN 1024
#endif

// ----------------------------------------------------------------------------
// private globals
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_8
static wxChar wxFileFunctionsBuffer[4*_MAXPATHLEN];
#endif

#if defined(__VISAGECPP__) && __IBMCPP__ >= 400
//
// VisualAge C++ V4.0 cannot have any external linkage const decs
// in headers included by more than one primary source
//
const int wxInvalidOffset = -1;
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wrappers around standard POSIX functions
// ----------------------------------------------------------------------------

#if wxUSE_UNICODE && defined __BORLANDC__ \
    && __BORLANDC__ >= 0x550 && __BORLANDC__ <= 0x551

// BCC 5.5 and 5.5.1 have a bug in _wopen where files are created read only
// regardless of the mode parameter. This hack works around the problem by
// setting the mode with _wchmod.
//
int wxCRT_OpenW(const wchar_t *pathname, int flags, mode_t mode)
{
    int moreflags = 0;

    // we only want to fix the mode when the file is actually created, so
    // when creating first try doing it O_EXCL so we can tell if the file
    // was already there.
    if ((flags & O_CREAT) && !(flags & O_EXCL) && (mode & wxS_IWUSR) != 0)
        moreflags = O_EXCL;

    int fd = _wopen(pathname, flags | moreflags, mode);

    // the file was actually created and needs fixing
    if (fd != -1 && (flags & O_CREAT) != 0 && (mode & wxS_IWUSR) != 0)
    {
        close(fd);
        _wchmod(pathname, mode);
        fd = _wopen(pathname, flags & ~(O_EXCL | O_CREAT));
    }
    // the open failed, but it may have been because the added O_EXCL stopped
    // the opening of an existing file, so try again without.
    else if (fd == -1 && moreflags != 0)
    {
        fd = _wopen(pathname, flags & ~O_CREAT);
    }

    return fd;
}

#endif

// ----------------------------------------------------------------------------
// wxPathList
// ----------------------------------------------------------------------------

bool wxPathList::Add(const wxString& path)
{
    // add a path separator to force wxFileName to interpret it always as a directory
    // (i.e. if we are called with '/home/user' we want to consider it a folder and
    // not, as wxFileName would consider, a filename).
    wxFileName fn(path + wxFileName::GetPathSeparator());

    // add only normalized relative/absolute paths
    // NB: we won't do wxPATH_NORM_DOTS in order to avoid problems when trying to
    //     normalize paths which starts with ".." (which can be normalized only if
    //     we use also wxPATH_NORM_ABSOLUTE - which we don't want to use).
    if (!fn.Normalize(wxPATH_NORM_TILDE|wxPATH_NORM_LONG|wxPATH_NORM_ENV_VARS))
        return false;

    wxString toadd = fn.GetPath();
    if (Index(toadd) == wxNOT_FOUND)
        wxArrayString::Add(toadd);      // do not add duplicates

    return true;
}

void wxPathList::Add(const wxArrayString &arr)
{
    for (size_t j=0; j < arr.GetCount(); j++)
        Add(arr[j]);
}

// Add paths e.g. from the PATH environment variable
void wxPathList::AddEnvList (const wxString& WXUNUSED_IN_WINCE(envVariable))
{
    // No environment variables on WinCE
#ifndef __WXWINCE__

    // The space has been removed from the tokenizers, otherwise a
    // path such as "C:\Program Files" would be split into 2 paths:
    // "C:\Program" and "Files"; this is true for both Windows and Unix.

    static const wxChar PATH_TOKS[] =
#if defined(__WINDOWS__) || defined(__OS2__)
        wxT(";"); // Don't separate with colon in DOS (used for drive)
#else
        wxT(":;");
#endif

    wxString val;
    if ( wxGetEnv(envVariable, &val) )
    {
        // split into an array of string the value of the env var
        wxArrayString arr = wxStringTokenize(val, PATH_TOKS);
        WX_APPEND_ARRAY(*this, arr);
    }
#endif // !__WXWINCE__
}

// Given a full filename (with path), ensure that that file can
// be accessed again USING FILENAME ONLY by adding the path
// to the list if not already there.
bool wxPathList::EnsureFileAccessible (const wxString& path)
{
    return Add(wxPathOnly(path));
}

wxString wxPathList::FindValidPath (const wxString& file) const
{
    // normalize the given string as it could be a path + a filename
    // and not only a filename
    wxFileName fn(file);
    wxString strend;

    // NB: normalize without making absolute otherwise calling this function with
    //     e.g. "b/c.txt" would result in removing the directory 'b' and the for loop
    //     below would only add to the paths of this list the 'c.txt' part when doing
    //     the existence checks...
    // NB: we don't use wxPATH_NORM_DOTS here, too (see wxPathList::Add for more info)
    if (!fn.Normalize(wxPATH_NORM_TILDE|wxPATH_NORM_LONG|wxPATH_NORM_ENV_VARS))
        return wxEmptyString;

    wxASSERT_MSG(!fn.IsDir(), wxT("Cannot search for directories; only for files"));
    if (fn.IsAbsolute())
        strend = fn.GetFullName();      // search for the file name and ignore the path part
    else
        strend = fn.GetFullPath();

    for (size_t i=0; i<GetCount(); i++)
    {
        wxString strstart = Item(i);
        if (!strstart.IsEmpty() && strstart.Last() != wxFileName::GetPathSeparator())
            strstart += wxFileName::GetPathSeparator();

        if (wxFileExists(strstart + strend))
            return strstart + strend;        // Found!
    }

    return wxEmptyString;                    // Not found
}

wxString wxPathList::FindAbsoluteValidPath (const wxString& file) const
{
    wxString f = FindValidPath(file);
    if ( f.empty() || wxIsAbsolutePath(f) )
        return f;

    wxString buf = ::wxGetCwd();

    if ( !wxEndsWithPathSeparator(buf) )
    {
        buf += wxFILE_SEP_PATH;
    }
    buf += f;

    return buf;
}

// ----------------------------------------------------------------------------
// miscellaneous global functions
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_8
static inline wxChar* MYcopystring(const wxString& s)
{
    wxChar* copy = new wxChar[s.length() + 1];
    return wxStrcpy(copy, s.c_str());
}

template<typename CharType>
static inline CharType* MYcopystring(const CharType* s)
{
    CharType* copy = new CharType[wxStrlen(s) + 1];
    return wxStrcpy(copy, s);
}
#endif


bool
wxFileExists (const wxString& filename)
{
    return wxFileName::FileExists(filename);
}

bool
wxIsAbsolutePath (const wxString& filename)
{
    if (!filename.empty())
    {
        // Unix like or Windows
        if (filename[0] == wxT('/'))
            return true;
#ifdef __VMS__
        if ((filename[0] == wxT('[') && filename[1] != wxT('.')))
            return true;
#endif
#if defined(__WINDOWS__) || defined(__OS2__)
        // MSDOS like
        if (filename[0] == wxT('\\') || (wxIsalpha (filename[0]) && filename[1] == wxT(':')))
            return true;
#endif
    }
    return false ;
}

#if WXWIN_COMPATIBILITY_2_8
/*
 * Strip off any extension (dot something) from end of file,
 * IF one exists. Inserts zero into buffer.
 *
 */

template<typename T>
static void wxDoStripExtension(T *buffer)
{
    int len = wxStrlen(buffer);
    int i = len-1;
    while (i > 0)
    {
        if (buffer[i] == wxT('.'))
        {
            buffer[i] = 0;
            break;
        }
        i --;
    }
}

void wxStripExtension(char *buffer) { wxDoStripExtension(buffer); }
void wxStripExtension(wchar_t *buffer) { wxDoStripExtension(buffer); }

void wxStripExtension(wxString& buffer)
{
   buffer = wxFileName::StripExtension(buffer);
}

// Destructive removal of /./ and /../ stuff
template<typename CharType>
static CharType *wxDoRealPath (CharType *path)
{
  static const CharType SEP = wxFILE_SEP_PATH;
#ifdef __WINDOWS__
  wxUnix2DosFilename(path);
#endif
  if (path[0] && path[1]) {
    /* MATTHEW: special case "/./x" */
    CharType *p;
    if (path[2] == SEP && path[1] == wxT('.'))
      p = &path[0];
    else
      p = &path[2];
    for (; *p; p++)
      {
        if (*p == SEP)
          {
            if (p[1] == wxT('.') && p[2] == wxT('.') && (p[3] == SEP || p[3] == wxT('\0')))
              {
                CharType *q;
                for (q = p - 1; q >= path && *q != SEP; q--)
                {
                    // Empty
                }

                if (q[0] == SEP && (q[1] != wxT('.') || q[2] != wxT('.') || q[3] != SEP)
                    && (q - 1 <= path || q[-1] != SEP))
                  {
                    wxStrcpy (q, p + 3);
                    if (path[0] == wxT('\0'))
                      {
                        path[0] = SEP;
                        path[1] = wxT('\0');
                      }
#if defined(__WINDOWS__) || defined(__OS2__)
                    /* Check that path[2] is NULL! */
                    else if (path[1] == wxT(':') && !path[2])
                      {
                        path[2] = SEP;
                        path[3] = wxT('\0');
                      }
#endif
                    p = q - 1;
                  }
              }
            else if (p[1] == wxT('.') && (p[2] == SEP || p[2] == wxT('\0')))
              wxStrcpy (p, p + 2);
          }
      }
  }
  return path;
}

char *wxRealPath(char *path)
{
    return wxDoRealPath(path);
}

wchar_t *wxRealPath(wchar_t *path)
{
    return wxDoRealPath(path);
}

wxString wxRealPath(const wxString& path)
{
    wxChar *buf1=MYcopystring(path);
    wxChar *buf2=wxRealPath(buf1);
    wxString buf(buf2);
    delete [] buf1;
    return buf;
}


// Must be destroyed
wxChar *wxCopyAbsolutePath(const wxString& filename)
{
    if (filename.empty())
        return NULL;

    if (! wxIsAbsolutePath(wxExpandPath(wxFileFunctionsBuffer, filename)))
    {
        wxString buf = ::wxGetCwd();
        wxChar ch = buf.Last();
#ifdef __WINDOWS__
        if (ch != wxT('\\') && ch != wxT('/'))
            buf << wxT("\\");
#else
        if (ch != wxT('/'))
            buf << wxT("/");
#endif
        buf << wxFileFunctionsBuffer;
        buf = wxRealPath( buf );
        return MYcopystring( buf );
    }
    return MYcopystring( wxFileFunctionsBuffer );
}

/*-
 Handles:
   ~/ => home dir
   ~user/ => user's home dir
   If the environment variable a = "foo" and b = "bar" then:
   Unix:
        $a        =>        foo
        $a$b        =>        foobar
        $a.c        =>        foo.c
        xxx$a        =>        xxxfoo
        ${a}!        =>        foo!
        $(b)!        =>        bar!
        \$a        =>        \$a
   MSDOS:
        $a        ==>        $a
        $(a)        ==>        foo
        $(a)$b        ==>        foo$b
        $(a)$(b)==>        foobar
        test.$$        ==>        test.$$
 */

/* input name in name, pathname output to buf. */

template<typename CharType>
static CharType *wxDoExpandPath(CharType *buf, const wxString& name)
{
    register CharType *d, *s, *nm;
    CharType        lnm[_MAXPATHLEN];
    int             q;

    // Some compilers don't like this line.
//    const CharType    trimchars[] = wxT("\n \t");

    CharType      trimchars[4];
    trimchars[0] = wxT('\n');
    trimchars[1] = wxT(' ');
    trimchars[2] = wxT('\t');
    trimchars[3] = 0;

    static const CharType SEP = wxFILE_SEP_PATH;
#ifdef __WINDOWS__
    //wxUnix2DosFilename(path);
#endif

    buf[0] = wxT('\0');
    if (name.empty())
        return buf;
    nm = ::MYcopystring(static_cast<const CharType*>(name.c_str())); // Make a scratch copy
    CharType *nm_tmp = nm;

    /* Skip leading whitespace and cr */
    while (wxStrchr(trimchars, *nm) != NULL)
        nm++;
    /* And strip off trailing whitespace and cr */
    s = nm + (q = wxStrlen(nm)) - 1;
    while (q-- && wxStrchr(trimchars, *s) != NULL)
        *s = wxT('\0');

    s = nm;
    d = lnm;
#ifdef __WINDOWS__
    q = FALSE;
#else
    q = nm[0] == wxT('\\') && nm[1] == wxT('~');
#endif

    /* Expand inline environment variables */
#ifdef __VISAGECPP__
    while (*d)
    {
      *d++ = *s;
      if(*s == wxT('\\'))
      {
        *(d - 1) = *++s;
        if (*d)
        {
          s++;
          continue;
        }
        else
           break;
      }
      else
#else
    while ((*d++ = *s) != 0) {
#  ifndef __WINDOWS__
        if (*s == wxT('\\')) {
            if ((*(d - 1) = *++s)!=0) {
                s++;
                continue;
            } else
                break;
        } else
#  endif
#endif
            // No env variables on WinCE
#ifndef __WXWINCE__
#ifdef __WINDOWS__
        if (*s++ == wxT('$') && (*s == wxT('{') || *s == wxT(')')))
#else
        if (*s++ == wxT('$'))
#endif
        {
            register CharType  *start = d;
            register int     braces = (*s == wxT('{') || *s == wxT('('));
            register CharType  *value;
            while ((*d++ = *s) != 0)
                if (braces ? (*s == wxT('}') || *s == wxT(')')) : !(wxIsalnum(*s) || *s == wxT('_')) )
                    break;
                else
                    s++;
            *--d = 0;
            value = wxGetenv(braces ? start + 1 : start);
            if (value) {
                for ((d = start - 1); (*d++ = *value++) != 0;)
                {
                    // Empty
                }

                d--;
                if (braces && *s)
                    s++;
            }
        }
#endif
        // __WXWINCE__
    }

    /* Expand ~ and ~user */
    wxString homepath;
    nm = lnm;
    if (nm[0] == wxT('~') && !q)
    {
        /* prefix ~ */
        if (nm[1] == SEP || nm[1] == 0)
        {        /* ~/filename */
            homepath = wxGetUserHome(wxEmptyString);
            if (!homepath.empty()) {
                s = (CharType*)(const CharType*)homepath.c_str();
                if (*++nm)
                    nm++;
            }
        } else
        {                /* ~user/filename */
            register CharType  *nnm;
            for (s = nm; *s && *s != SEP; s++)
            {
                // Empty
            }
            int was_sep; /* MATTHEW: Was there a separator, or NULL? */
            was_sep = (*s == SEP);
            nnm = *s ? s + 1 : s;
            *s = 0;
            homepath = wxGetUserHome(wxString(nm + 1));
            if (homepath.empty())
            {
                if (was_sep) /* replace only if it was there: */
                    *s = SEP;
                s = NULL;
            }
            else
            {
                nm = nnm;
                s = (CharType*)(const CharType*)homepath.c_str();
            }
        }
    }

    d = buf;
    if (s && *s) { /* MATTHEW: s could be NULL if user '~' didn't exist */
        /* Copy home dir */
        while (wxT('\0') != (*d++ = *s++))
          /* loop */;
        // Handle root home
        if (d - 1 > buf && *(d - 2) != SEP)
          *(d - 1) = SEP;
    }
    s = nm;
    while ((*d++ = *s++) != 0)
    {
        // Empty
    }
    delete[] nm_tmp; // clean up alloc
    /* Now clean up the buffer */
    return wxRealPath(buf);
}

char *wxExpandPath(char *buf, const wxString& name)
{
    return wxDoExpandPath(buf, name);
}

wchar_t *wxExpandPath(wchar_t *buf, const wxString& name)
{
    return wxDoExpandPath(buf, name);
}


/* Contract Paths to be build upon an environment variable
   component:

   example: "/usr/openwin/lib", OPENWINHOME --> ${OPENWINHOME}/lib

   The call wxExpandPath can convert these back!
 */
wxChar *
wxContractPath (const wxString& filename,
                const wxString& WXUNUSED_IN_WINCE(envname),
                const wxString& user)
{
  static wxChar dest[_MAXPATHLEN];

  if (filename.empty())
    return NULL;

  wxStrcpy (dest, filename);
#ifdef __WINDOWS__
  wxUnix2DosFilename(dest);
#endif

  // Handle environment
  wxString val;
#ifndef __WXWINCE__
  wxChar *tcp;
  if (!envname.empty() && !(val = wxGetenv (envname)).empty() &&
     (tcp = wxStrstr (dest, val)) != NULL)
    {
        wxStrcpy (wxFileFunctionsBuffer, tcp + val.length());
        *tcp++ = wxT('$');
        *tcp++ = wxT('{');
        wxStrcpy (tcp, envname);
        wxStrcat (tcp, wxT("}"));
        wxStrcat (tcp, wxFileFunctionsBuffer);
    }
#endif

  // Handle User's home (ignore root homes!)
  val = wxGetUserHome (user);
  if (val.empty())
    return dest;

  const size_t len = val.length();
  if (len <= 2)
    return dest;

  if (wxStrncmp(dest, val, len) == 0)
  {
    wxStrcpy(wxFileFunctionsBuffer, wxT("~"));
    if (!user.empty())
           wxStrcat(wxFileFunctionsBuffer, user);
    wxStrcat(wxFileFunctionsBuffer, dest + len);
    wxStrcpy (dest, wxFileFunctionsBuffer);
  }

  return dest;
}

#endif // #if WXWIN_COMPATIBILITY_2_8

// Return just the filename, not the path (basename)
wxChar *wxFileNameFromPath (wxChar *path)
{
    wxString p = path;
    wxString n = wxFileNameFromPath(p);

    return path + p.length() - n.length();
}

wxString wxFileNameFromPath (const wxString& path)
{
    return wxFileName(path).GetFullName();
}

// Return just the directory, or NULL if no directory
wxChar *
wxPathOnly (wxChar *path)
{
    if (path && *path)
    {
        static wxChar buf[_MAXPATHLEN];

        int l = wxStrlen(path);
        int i = l - 1;
        if ( i >= _MAXPATHLEN )
            return NULL;

        // Local copy
        wxStrcpy (buf, path);

        // Search backward for a backward or forward slash
        while (i > -1)
        {
            // Unix like or Windows
            if (path[i] == wxT('/') || path[i] == wxT('\\'))
            {
                buf[i] = 0;
                return buf;
            }
#ifdef __VMS__
            if (path[i] == wxT(']'))
            {
                buf[i+1] = 0;
                return buf;
            }
#endif
            i --;
        }

#if defined(__WINDOWS__) || defined(__OS2__)
        // Try Drive specifier
        if (wxIsalpha (buf[0]) && buf[1] == wxT(':'))
        {
            // A:junk --> A:. (since A:.\junk Not A:\junk)
            buf[2] = wxT('.');
            buf[3] = wxT('\0');
            return buf;
        }
#endif
    }
    return NULL;
}

// Return just the directory, or NULL if no directory
wxString wxPathOnly (const wxString& path)
{
    if (!path.empty())
    {
        wxChar buf[_MAXPATHLEN];

        int l = path.length();
        int i = l - 1;

        if ( i >= _MAXPATHLEN )
            return wxString();

        // Local copy
        wxStrcpy(buf, path);

        // Search backward for a backward or forward slash
        while (i > -1)
        {
            // Unix like or Windows
            if (path[i] == wxT('/') || path[i] == wxT('\\'))
            {
                // Don't return an empty string
                if (i == 0)
                    i ++;
                buf[i] = 0;
                return wxString(buf);
            }
#ifdef __VMS__
            if (path[i] == wxT(']'))
            {
                buf[i+1] = 0;
                return wxString(buf);
            }
#endif
            i --;
        }

#if defined(__WINDOWS__) || defined(__OS2__)
        // Try Drive specifier
        if (wxIsalpha (buf[0]) && buf[1] == wxT(':'))
        {
            // A:junk --> A:. (since A:.\junk Not A:\junk)
            buf[2] = wxT('.');
            buf[3] = wxT('\0');
            return wxString(buf);
        }
#endif
    }
    return wxEmptyString;
}

// Utility for converting delimiters in DOS filenames to UNIX style
// and back again - or we get nasty problems with delimiters.
// Also, convert to lower case, since case is significant in UNIX.

#if defined(__WXMAC__) && !defined(__WXOSX_IPHONE__)

#define kDefaultPathStyle kCFURLPOSIXPathStyle

wxString wxMacFSRefToPath( const FSRef *fsRef , CFStringRef additionalPathComponent )
{
    CFURLRef fullURLRef;
    fullURLRef = CFURLCreateFromFSRef(NULL, fsRef);
    if ( fullURLRef == NULL)
        return wxEmptyString;
    
    if ( additionalPathComponent )
    {
        CFURLRef parentURLRef = fullURLRef ;
        fullURLRef = CFURLCreateCopyAppendingPathComponent(NULL, parentURLRef,
            additionalPathComponent,false);
        CFRelease( parentURLRef ) ;
    }
    wxCFStringRef cfString( CFURLCopyFileSystemPath(fullURLRef, kDefaultPathStyle ));
    CFRelease( fullURLRef ) ;

    return wxCFStringRef::AsStringWithNormalizationFormC(cfString);
}

OSStatus wxMacPathToFSRef( const wxString&path , FSRef *fsRef )
{
    OSStatus err = noErr ;
    CFMutableStringRef cfMutableString = CFStringCreateMutableCopy(NULL, 0, wxCFStringRef(path));
    CFStringNormalize(cfMutableString,kCFStringNormalizationFormD);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfMutableString , kDefaultPathStyle, false);
    CFRelease( cfMutableString );
    if ( NULL != url )
    {
        if ( CFURLGetFSRef(url, fsRef) == false )
            err = fnfErr ;
        CFRelease( url ) ;
    }
    else
    {
        err = fnfErr ;
    }
    return err ;
}

wxString wxMacHFSUniStrToString( ConstHFSUniStr255Param uniname )
{
    wxCFStringRef cfname( CFStringCreateWithCharacters( kCFAllocatorDefault,
                                                      uniname->unicode,
                                                      uniname->length ) );
    return wxCFStringRef::AsStringWithNormalizationFormC(cfname);
}

#ifndef __LP64__

wxString wxMacFSSpec2MacFilename( const FSSpec *spec )
{
    FSRef fsRef ;
    if ( FSpMakeFSRef( spec , &fsRef) == noErr )
    {
        return wxMacFSRefToPath( &fsRef ) ;
    }
    return wxEmptyString ;
}

void wxMacFilename2FSSpec( const wxString& path , FSSpec *spec )
{
    OSStatus err = noErr;
    FSRef fsRef;
    wxMacPathToFSRef( path , &fsRef );
    err = FSGetCatalogInfo(&fsRef, kFSCatInfoNone, NULL, NULL, spec, NULL);
    verify_noerr( err );
}
#endif

#endif // __WXMAC__


#if WXWIN_COMPATIBILITY_2_8

template<typename T>
static void wxDoDos2UnixFilename(T *s)
{
  if (s)
    while (*s)
      {
        if (*s == wxT('\\'))
          *s = wxT('/');
#ifdef __WINDOWS__
        else
          *s = wxTolower(*s);        // Case INDEPENDENT
#endif
        s++;
      }
}

void wxDos2UnixFilename(char *s) { wxDoDos2UnixFilename(s); }
void wxDos2UnixFilename(wchar_t *s) { wxDoDos2UnixFilename(s); }

template<typename T>
static void
#if defined(__WINDOWS__) || defined(__OS2__)
wxDoUnix2DosFilename(T *s)
#else
wxDoUnix2DosFilename(T *WXUNUSED(s) )
#endif
{
// Yes, I really mean this to happen under DOS only! JACS
#if defined(__WINDOWS__) || defined(__OS2__)
  if (s)
    while (*s)
      {
        if (*s == wxT('/'))
          *s = wxT('\\');
        s++;
      }
#endif
}

void wxUnix2DosFilename(char *s) { wxDoUnix2DosFilename(s); }
void wxUnix2DosFilename(wchar_t *s) { wxDoUnix2DosFilename(s); }

#endif // #if WXWIN_COMPATIBILITY_2_8

// Concatenate two files to form third
bool
wxConcatFiles (const wxString& file1, const wxString& file2, const wxString& file3)
{
#if wxUSE_FILE

    wxFile in1(file1), in2(file2);
    wxTempFile out(file3);

    if ( !in1.IsOpened() || !in2.IsOpened() || !out.IsOpened() )
        return false;

    ssize_t ofs;
    unsigned char buf[1024];

    for( int i=0; i<2; i++)
    {
        wxFile *in = i==0 ? &in1 : &in2;
        do{
            if ( (ofs = in->Read(buf,WXSIZEOF(buf))) == wxInvalidOffset ) return false;
            if ( ofs > 0 )
                if ( !out.Write(buf,ofs) )
                    return false;
        } while ( ofs == (ssize_t)WXSIZEOF(buf) );
    }

    return out.Commit();

#else

    wxUnusedVar(file1);
    wxUnusedVar(file2);
    wxUnusedVar(file3);
    return false;

#endif
}

// helper of generic implementation of wxCopyFile()
#if !(defined(__WIN32__) || defined(__OS2__)) && wxUSE_FILE

static bool
wxDoCopyFile(wxFile& fileIn,
             const wxStructStat& fbuf,
             const wxString& filenameDst,
             bool overwrite)
{
    // reset the umask as we want to create the file with exactly the same
    // permissions as the original one
    wxCHANGE_UMASK(0);

    // create file2 with the same permissions than file1 and open it for
    // writing

    wxFile fileOut;
    if ( !fileOut.Create(filenameDst, overwrite, fbuf.st_mode & 0777) )
        return false;

    // copy contents of file1 to file2
    char buf[4096];
    for ( ;; )
    {
        ssize_t count = fileIn.Read(buf, WXSIZEOF(buf));
        if ( count == wxInvalidOffset )
            return false;

        // end of file?
        if ( !count )
            break;

        if ( fileOut.Write(buf, count) < (size_t)count )
            return false;
    }

    // we can expect fileIn to be closed successfully, but we should ensure
    // that fileOut was closed as some write errors (disk full) might not be
    // detected before doing this
    return fileIn.Close() && fileOut.Close();
}

#endif // generic implementation of wxCopyFile

// Copy files
bool
wxCopyFile (const wxString& file1, const wxString& file2, bool overwrite)
{
#if defined(__WIN32__) && !defined(__WXMICROWIN__)
    // CopyFile() copies file attributes and modification time too, so use it
    // instead of our code if available
    //
    // NB: 3rd parameter is bFailIfExists i.e. the inverse of overwrite
    if ( !::CopyFile(file1.t_str(), file2.t_str(), !overwrite) )
    {
        wxLogSysError(_("Failed to copy the file '%s' to '%s'"),
                      file1.c_str(), file2.c_str());

        return false;
    }
#elif defined(__OS2__)
    if ( ::DosCopy(file1.c_str(), file2.c_str(), overwrite ? DCPY_EXISTING : 0) != 0 )
        return false;
#elif wxUSE_FILE // !Win32

    wxStructStat fbuf;
    // get permissions of file1
    if ( wxStat( file1, &fbuf) != 0 )
    {
        // the file probably doesn't exist or we haven't the rights to read
        // from it anyhow
        wxLogSysError(_("Impossible to get permissions for file '%s'"),
                      file1.c_str());
        return false;
    }

    // open file1 for reading
    wxFile fileIn(file1, wxFile::read);
    if ( !fileIn.IsOpened() )
        return false;

    // remove file2, if it exists. This is needed for creating
    // file2 with the correct permissions in the next step
    if ( wxFileExists(file2)  && (!overwrite || !wxRemoveFile(file2)))
    {
        wxLogSysError(_("Impossible to overwrite the file '%s'"),
                      file2.c_str());
        return false;
    }

    wxDoCopyFile(fileIn, fbuf, file2, overwrite);

#if defined(__WXMAC__) || defined(__WXCOCOA__)
    // copy the resource fork of the file too if it's present
    wxString pathRsrcOut;
    wxFile fileRsrcIn;

    {
        // suppress error messages from this block as resource forks don't have
        // to exist
        wxLogNull noLog;

        // it's not enough to check for file existence: it always does on HFS
        // but is empty for files without resources
        if ( fileRsrcIn.Open(file1 + wxT("/..namedfork/rsrc")) &&
                fileRsrcIn.Length() > 0 )
        {
            // we must be using HFS or another filesystem with resource fork
            // support, suppose that destination file system also is HFS[-like]
            pathRsrcOut = file2 + wxT("/..namedfork/rsrc");
        }
        else // check if we have resource fork in separate file (non-HFS case)
        {
            wxFileName fnRsrc(file1);
            fnRsrc.SetName(wxT("._") + fnRsrc.GetName());

            fileRsrcIn.Close();
            if ( fileRsrcIn.Open( fnRsrc.GetFullPath() ) )
            {
                fnRsrc = file2;
                fnRsrc.SetName(wxT("._") + fnRsrc.GetName());

                pathRsrcOut = fnRsrc.GetFullPath();
            }
        }
    }

    if ( !pathRsrcOut.empty() )
    {
        if ( !wxDoCopyFile(fileRsrcIn, fbuf, pathRsrcOut, overwrite) )
            return false;
    }
#endif // wxMac || wxCocoa

#if !defined(__VISAGECPP__) && !defined(__WXMAC__) || defined(__UNIX__)
    // no chmod in VA.  Should be some permission API for HPFS386 partitions
    // however
    if ( chmod(file2.fn_str(), fbuf.st_mode) != 0 )
    {
        wxLogSysError(_("Impossible to set permissions for the file '%s'"),
                      file2.c_str());
        return false;
    }
#endif // OS/2 || Mac

#else // !Win32 && ! wxUSE_FILE

    // impossible to simulate with wxWidgets API
    wxUnusedVar(file1);
    wxUnusedVar(file2);
    wxUnusedVar(overwrite);
    return false;

#endif // __WINDOWS__ && __WIN32__

    return true;
}

bool
wxRenameFile(const wxString& file1, const wxString& file2, bool overwrite)
{
    if ( !overwrite && wxFileExists(file2) )
    {
        wxLogSysError
        (
            _("Failed to rename the file '%s' to '%s' because the destination file already exists."),
            file1.c_str(), file2.c_str()
        );

        return false;
    }

#if !defined(__WXWINCE__)
    // Normal system call
  if ( wxRename (file1, file2) == 0 )
    return true;
#endif

  // Try to copy
  if (wxCopyFile(file1, file2, overwrite)) {
    wxRemoveFile(file1);
    return true;
  }
  // Give up
  wxLogSysError(_("File '%s' couldn't be renamed '%s'"), file1, file2);
  return false;
}

bool wxRemoveFile(const wxString& file)
{
#if defined(__VISUALC__) \
 || defined(__BORLANDC__) \
 || defined(__WATCOMC__) \
 || defined(__DMC__) \
 || defined(__GNUWIN32__)
    int res = wxRemove(file);
#elif defined(__WXMAC__)
    int res = unlink(file.fn_str());
#else
    int res = unlink(file.fn_str());
#endif
    if ( res )
    {
        wxLogSysError(_("File '%s' couldn't be removed"), file);
    }
    return res == 0;
}

bool wxMkdir(const wxString& dir, int perm)
{
#if defined(__WXMAC__) && !defined(__UNIX__)
    if ( mkdir(dir.fn_str(), 0) != 0 )

    // assume mkdir() has 2 args on non Windows-OS/2 platforms and on Windows too
    // for the GNU compiler
#elif (!(defined(__WINDOWS__) || defined(__OS2__) || defined(__DOS__))) || \
      (defined(__GNUWIN32__) && !defined(__MINGW32__)) ||                \
      defined(__WINE__) || defined(__WXMICROWIN__)
    const wxChar *dirname = dir.c_str();
  #if defined(MSVCRT)
    wxUnusedVar(perm);
    if ( mkdir(wxFNCONV(dirname)) != 0 )
  #else
    if ( mkdir(wxFNCONV(dirname), perm) != 0 )
  #endif
#elif defined(__OS2__)
    wxUnusedVar(perm);
    if (::DosCreateDir(dir.c_str(), NULL) != 0) // enhance for EAB's??
#elif defined(__DOS__)
    const wxChar *dirname = dir.c_str();
  #if defined(__WATCOMC__)
    (void)perm;
    if ( wxMkDir(wxFNSTRINGCAST wxFNCONV(dirname)) != 0 )
  #elif defined(__DJGPP__)
    if ( mkdir(wxFNCONV(dirname), perm) != 0 )
  #else
    #error "Unsupported DOS compiler!"
  #endif
#else  // !MSW, !DOS and !OS/2 VAC++
    wxUnusedVar(perm);
  #ifdef __WXWINCE__
    if ( CreateDirectory(dir.fn_str(), NULL) == 0 )
  #else
    if ( wxMkDir(dir.fn_str()) != 0 )
  #endif
#endif // !MSW/MSW
    {
        wxLogSysError(_("Directory '%s' couldn't be created"), dir);
        return false;
    }

    return true;
}

bool wxRmdir(const wxString& dir, int WXUNUSED(flags))
{
#if defined(__VMS__)
    return false; //to be changed since rmdir exists in VMS7.x
#else
  #if defined(__OS2__)
    if ( ::DosDeleteDir(dir.c_str()) != 0 )
  #elif defined(__WXWINCE__)
    if ( RemoveDirectory(dir.fn_str()) == 0 )
  #else
    if ( wxRmDir(dir.fn_str()) != 0 )
  #endif
    {
        wxLogSysError(_("Directory '%s' couldn't be deleted"), dir);
        return false;
    }

    return true;
#endif
}

// does the path exists? (may have or not '/' or '\\' at the end)
bool wxDirExists(const wxString& pathName)
{
    return wxFileName::DirExists(pathName);
}

#if WXWIN_COMPATIBILITY_2_8

// Get a temporary filename, opening and closing the file.
wxChar *wxGetTempFileName(const wxString& prefix, wxChar *buf)
{
    wxString filename;
    if ( !wxGetTempFileName(prefix, filename) )
        return NULL;

    if ( buf )
        wxStrcpy(buf, filename);
    else
        buf = MYcopystring(filename);

    return buf;
}

bool wxGetTempFileName(const wxString& prefix, wxString& buf)
{
#if wxUSE_FILE
    buf = wxFileName::CreateTempFileName(prefix);

    return !buf.empty();
#else // !wxUSE_FILE
    wxUnusedVar(prefix);
    wxUnusedVar(buf);

    return false;
#endif // wxUSE_FILE/!wxUSE_FILE
}

#endif // #if WXWIN_COMPATIBILITY_2_8

// Get first file name matching given wild card.

static wxScopedPtr<wxDir> gs_dir;
static wxString gs_dirPath;

wxString wxFindFirstFile(const wxString& spec, int flags)
{
    wxFileName::SplitPath(spec, &gs_dirPath, NULL, NULL);
    if ( gs_dirPath.empty() )
        gs_dirPath = wxT(".");
    if ( !wxEndsWithPathSeparator(gs_dirPath ) )
        gs_dirPath << wxFILE_SEP_PATH;

    gs_dir.reset(new wxDir(gs_dirPath));

    if ( !gs_dir->IsOpened() )
    {
        wxLogSysError(_("Cannot enumerate files '%s'"), spec);
        return wxEmptyString;
    }

    int dirFlags;
    switch (flags)
    {
        case wxDIR:  dirFlags = wxDIR_DIRS; break;
        case wxFILE: dirFlags = wxDIR_FILES; break;
        default:     dirFlags = wxDIR_DIRS | wxDIR_FILES; break;
    }

    wxString result;
    gs_dir->GetFirst(&result, wxFileNameFromPath(spec), dirFlags);
    if ( result.empty() )
        return result;

    return gs_dirPath + result;
}

wxString wxFindNextFile()
{
    wxCHECK_MSG( gs_dir, "", "You must call wxFindFirstFile before!" );

    wxString result;
    if ( !gs_dir->GetNext(&result) || result.empty() )
        return result;

    return gs_dirPath + result;
}


// Get current working directory.
// If buf is NULL, allocates space using new, else copies into buf.
// wxGetWorkingDirectory() is obsolete, use wxGetCwd()
// wxDoGetCwd() is their common core to be moved
// to wxGetCwd() once wxGetWorkingDirectory() will be removed.
// Do not expose wxDoGetCwd in headers!

wxChar *wxDoGetCwd(wxChar *buf, int sz)
{
#if defined(__WXWINCE__)
    // TODO
    if(buf && sz>0) buf[0] = wxT('\0');
    return buf;
#else
    if ( !buf )
    {
        buf = new wxChar[sz + 1];
    }

    bool ok = false;

    // for the compilers which have Unicode version of _getcwd(), call it
    // directly, for the others call the ANSI version and do the translation
#if !wxUSE_UNICODE
    #define cbuf buf
#else // wxUSE_UNICODE
    bool needsANSI = true;

    #if !defined(HAVE_WGETCWD) || wxUSE_UNICODE_MSLU
        char cbuf[_MAXPATHLEN];
    #endif

    #ifdef HAVE_WGETCWD
        #if wxUSE_UNICODE_MSLU
            if ( wxGetOsVersion() != wxOS_WINDOWS_9X )
        #else
            char *cbuf = NULL; // never really used because needsANSI will always be false
        #endif
            {
                ok = _wgetcwd(buf, sz) != NULL;
                needsANSI = false;
            }
    #endif

    if ( needsANSI )
#endif // wxUSE_UNICODE
    {
    #if defined(_MSC_VER) || defined(__MINGW32__)
        ok = _getcwd(cbuf, sz) != NULL;
    #elif defined(__OS2__)
        APIRET rc;
        ULONG ulDriveNum = 0;
        ULONG ulDriveMap = 0;
        rc = ::DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);
        ok = rc == 0;
        if (ok)
        {
            sz -= 3;
            rc = ::DosQueryCurrentDir( 0 // current drive
                                      ,(PBYTE)cbuf + 3
                                      ,(PULONG)&sz
                                     );
            cbuf[0] = char('A' + (ulDriveNum - 1));
            cbuf[1] = ':';
            cbuf[2] = '\\';
            ok = rc == 0;
        }
    #else // !Win32/VC++ !Mac !OS2
        ok = getcwd(cbuf, sz) != NULL;
    #endif // platform

    #if wxUSE_UNICODE
        // finally convert the result to Unicode if needed
        wxConvFile.MB2WC(buf, cbuf, sz);
    #endif // wxUSE_UNICODE
    }

    if ( !ok )
    {
        wxLogSysError(_("Failed to get the working directory"));

        // VZ: the old code used to return "." on error which didn't make any
        //     sense at all to me - empty string is a better error indicator
        //     (NULL might be even better but I'm afraid this could lead to
        //     problems with the old code assuming the return is never NULL)
        buf[0] = wxT('\0');
    }
    else // ok, but we might need to massage the path into the right format
    {
#ifdef __DJGPP__
        // VS: DJGPP is a strange mix of DOS and UNIX API and returns paths
        //     with / deliminers. We don't like that.
        for (wxChar *ch = buf; *ch; ch++)
        {
            if (*ch == wxT('/'))
                *ch = wxT('\\');
        }
#endif // __DJGPP__

// MBN: we hope that in the case the user is compiling a GTK+/Motif app,
//      he needs Unix as opposed to Win32 pathnames
#if defined( __CYGWIN__ ) && defined( __WINDOWS__ )
        // another example of DOS/Unix mix (Cygwin)
        wxString pathUnix = buf;
#if wxUSE_UNICODE
    #if CYGWIN_VERSION_DLL_MAJOR >= 1007
        cygwin_conv_path(CCP_POSIX_TO_WIN_W, pathUnix.mb_str(wxConvFile), buf, sz);
    #else
        char bufA[_MAXPATHLEN];
        cygwin_conv_to_full_win32_path(pathUnix.mb_str(wxConvFile), bufA);
        wxConvFile.MB2WC(buf, bufA, sz);
    #endif
#else
    #if CYGWIN_VERSION_DLL_MAJOR >= 1007
        cygwin_conv_path(CCP_POSIX_TO_WIN_A, pathUnix, buf, sz);
    #else
        cygwin_conv_to_full_win32_path(pathUnix, buf);
    #endif
#endif // wxUSE_UNICODE
#endif // __CYGWIN__
    }

    return buf;

#if !wxUSE_UNICODE
    #undef cbuf
#endif

#endif
    // __WXWINCE__
}

wxString wxGetCwd()
{
    wxString str;
    wxDoGetCwd(wxStringBuffer(str, _MAXPATHLEN), _MAXPATHLEN);
    return str;
}

bool wxSetWorkingDirectory(const wxString& d)
{
    bool success = false;
#if defined(__OS2__)
    if (d[1] == ':')
    {
        ::DosSetDefaultDisk(wxToupper(d[0]) - wxT('A') + 1);
    // do not call DosSetCurrentDir when just changing drive,
    // since it requires e.g. "d:." instead of "d:"!
    if (d.length() == 2)
        return true;
    }
    success = (::DosSetCurrentDir(d.c_str()) == 0);
#elif defined(__UNIX__) || defined(__WXMAC__) || defined(__DOS__)
    success = (chdir(wxFNSTRINGCAST d.fn_str()) == 0);
#elif defined(__WINDOWS__)

#ifdef __WIN32__
#ifdef __WXWINCE__
    // No equivalent in WinCE
    wxUnusedVar(d);
#else
    success = (SetCurrentDirectory(d.t_str()) != 0);
#endif
#else
    // Must change drive, too.
    bool isDriveSpec = ((strlen(d) > 1) && (d[1] == ':'));
    if (isDriveSpec)
    {
        wxChar firstChar = d[0];

        // To upper case
        if (firstChar > 90)
            firstChar = firstChar - 32;

        // To a drive number
        unsigned int driveNo = firstChar - 64;
        if (driveNo > 0)
        {
            unsigned int noDrives;
            _dos_setdrive(driveNo, &noDrives);
        }
    }
    success = (chdir(WXSTRINGCAST d) == 0);
#endif

#endif
    if ( !success )
    {
       wxLogSysError(_("Could not set current working directory"));
    }
    return success;
}

// Get the OS directory if appropriate (such as the Windows directory).
// On non-Windows platform, probably just return the empty string.
wxString wxGetOSDirectory()
{
#ifdef __WXWINCE__
    return wxString(wxT("\\Windows"));
#elif defined(__WINDOWS__) && !defined(__WXMICROWIN__)
    wxChar buf[MAX_PATH];
    if ( !GetWindowsDirectory(buf, MAX_PATH) )
    {
        wxLogLastError(wxS("GetWindowsDirectory"));
    }

    return wxString(buf);
#elif defined(__WXMAC__) && wxOSX_USE_CARBON
    return wxMacFindFolderNoSeparator(kOnSystemDisk, 'macs', false);
#else
    return wxEmptyString;
#endif
}

bool wxEndsWithPathSeparator(const wxString& filename)
{
    return !filename.empty() && wxIsPathSeparator(filename.Last());
}

// find a file in a list of directories, returns false if not found
bool wxFindFileInPath(wxString *pStr, const wxString& szPath, const wxString& szFile)
{
    // we assume that it's not empty
    wxCHECK_MSG( !szFile.empty(), false,
                 wxT("empty file name in wxFindFileInPath"));

    // skip path separator in the beginning of the file name if present
    wxString szFile2;
    if ( wxIsPathSeparator(szFile[0u]) )
        szFile2 = szFile.Mid(1);
    else
        szFile2 = szFile;

    wxStringTokenizer tkn(szPath, wxPATH_SEP);

    while ( tkn.HasMoreTokens() )
    {
        wxString strFile = tkn.GetNextToken();
        if ( !wxEndsWithPathSeparator(strFile) )
            strFile += wxFILE_SEP_PATH;
        strFile += szFile2;

        if ( wxFileExists(strFile) )
        {
            *pStr = strFile;
            return true;
        }
    }

    return false;
}

#if WXWIN_COMPATIBILITY_2_8
void WXDLLIMPEXP_BASE wxSplitPath(const wxString& fileName,
                             wxString *pstrPath,
                             wxString *pstrName,
                             wxString *pstrExt)
{
    wxFileName::SplitPath(fileName, pstrPath, pstrName, pstrExt);
}
#endif  // #if WXWIN_COMPATIBILITY_2_8

#if wxUSE_DATETIME

time_t WXDLLIMPEXP_BASE wxFileModificationTime(const wxString& filename)
{
    wxDateTime mtime;
    if ( !wxFileName(filename).GetTimes(NULL, &mtime, NULL) )
        return (time_t)-1;

    return mtime.GetTicks();
}

#endif // wxUSE_DATETIME


// Parses the filterStr, returning the number of filters.
// Returns 0 if none or if there's a problem.
// filterStr is in the form: "All files (*.*)|*.*|JPEG Files (*.jpeg)|*.jpeg"

int WXDLLIMPEXP_BASE wxParseCommonDialogsFilter(const wxString& filterStr,
                                           wxArrayString& descriptions,
                                           wxArrayString& filters)
{
    descriptions.Clear();
    filters.Clear();

    wxString str(filterStr);

    wxString description, filter;
    int pos = 0;
    while( pos != wxNOT_FOUND )
    {
        pos = str.Find(wxT('|'));
        if ( pos == wxNOT_FOUND )
        {
            // if there are no '|'s at all in the string just take the entire
            // string as filter and make description empty for later autocompletion
            if ( filters.IsEmpty() )
            {
                descriptions.Add(wxEmptyString);
                filters.Add(filterStr);
            }
            else
            {
                wxFAIL_MSG( wxT("missing '|' in the wildcard string!") );
            }

            break;
        }

        description = str.Left(pos);
        str = str.Mid(pos + 1);
        pos = str.Find(wxT('|'));
        if ( pos == wxNOT_FOUND )
        {
            filter = str;
        }
        else
        {
            filter = str.Left(pos);
            str = str.Mid(pos + 1);
        }

        descriptions.Add(description);
        filters.Add(filter);
    }

#if defined(__WXMOTIF__)
    // split it so there is one wildcard per entry
    for( size_t i = 0 ; i < descriptions.GetCount() ; i++ )
    {
        pos = filters[i].Find(wxT(';'));
        if (pos != wxNOT_FOUND)
        {
            // first split only filters
            descriptions.Insert(descriptions[i],i+1);
            filters.Insert(filters[i].Mid(pos+1),i+1);
            filters[i]=filters[i].Left(pos);

            // autoreplace new filter in description with pattern:
            //     C/C++ Files(*.cpp;*.c;*.h)|*.cpp;*.c;*.h
            // cause split into:
            //     C/C++ Files(*.cpp)|*.cpp
            //     C/C++ Files(*.c;*.h)|*.c;*.h
            // and next iteration cause another split into:
            //     C/C++ Files(*.cpp)|*.cpp
            //     C/C++ Files(*.c)|*.c
            //     C/C++ Files(*.h)|*.h
            for ( size_t k=i;k<i+2;k++ )
            {
                pos = descriptions[k].Find(filters[k]);
                if (pos != wxNOT_FOUND)
                {
                    wxString before = descriptions[k].Left(pos);
                    wxString after = descriptions[k].Mid(pos+filters[k].Len());
                    pos = before.Find(wxT('('),true);
                    if (pos>before.Find(wxT(')'),true))
                    {
                        before = before.Left(pos+1);
                        before << filters[k];
                        pos = after.Find(wxT(')'));
                        int pos1 = after.Find(wxT('('));
                        if (pos != wxNOT_FOUND && (pos<pos1 || pos1==wxNOT_FOUND))
                        {
                            before << after.Mid(pos);
                            descriptions[k] = before;
                        }
                    }
                }
            }
        }
    }
#endif

    // autocompletion
    for( size_t j = 0 ; j < descriptions.GetCount() ; j++ )
    {
        if ( descriptions[j].empty() && !filters[j].empty() )
        {
            descriptions[j].Printf(_("Files (%s)"), filters[j].c_str());
        }
    }

    return filters.GetCount();
}

#if defined(__WINDOWS__) && !(defined(__UNIX__) || defined(__OS2__))
static bool wxCheckWin32Permission(const wxString& path, DWORD access)
{
    // quoting the MSDN: "To obtain a handle to a directory, call the
    // CreateFile function with the FILE_FLAG_BACKUP_SEMANTICS flag", but this
    // doesn't work under Win9x/ME but then it's not needed there anyhow
    const DWORD dwAttr = ::GetFileAttributes(path.t_str());
    if ( dwAttr == INVALID_FILE_ATTRIBUTES )
    {
        // file probably doesn't exist at all
        return false;
    }

    if ( wxGetOsVersion() == wxOS_WINDOWS_9X )
    {
        // FAT directories always allow all access, even if they have the
        // readonly flag set, and FAT files can only be read-only
        return (dwAttr & FILE_ATTRIBUTE_DIRECTORY) ||
                    (access != GENERIC_WRITE ||
                        !(dwAttr & FILE_ATTRIBUTE_READONLY));
    }

    HANDLE h = ::CreateFile
                 (
                    path.t_str(),
                    access,
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                    NULL,
                    OPEN_EXISTING,
                    dwAttr & FILE_ATTRIBUTE_DIRECTORY
                        ? FILE_FLAG_BACKUP_SEMANTICS
                        : 0,
                    NULL
                 );
    if ( h != INVALID_HANDLE_VALUE )
        CloseHandle(h);

    return h != INVALID_HANDLE_VALUE;
}
#endif // __WINDOWS__

bool wxIsWritable(const wxString &path)
{
#if defined( __UNIX__ ) || defined(__OS2__)
    // access() will take in count also symbolic links
    return wxAccess(path.c_str(), W_OK) == 0;
#elif defined( __WINDOWS__ )
    return wxCheckWin32Permission(path, GENERIC_WRITE);
#else
    wxUnusedVar(path);
    // TODO
    return false;
#endif
}

bool wxIsReadable(const wxString &path)
{
#if defined( __UNIX__ ) || defined(__OS2__)
    // access() will take in count also symbolic links
    return wxAccess(path.c_str(), R_OK) == 0;
#elif defined( __WINDOWS__ )
    return wxCheckWin32Permission(path, GENERIC_READ);
#else
    wxUnusedVar(path);
    // TODO
    return false;
#endif
}

bool wxIsExecutable(const wxString &path)
{
#if defined( __UNIX__ ) || defined(__OS2__)
    // access() will take in count also symbolic links
    return wxAccess(path.c_str(), X_OK) == 0;
#elif defined( __WINDOWS__ )
   return wxCheckWin32Permission(path, GENERIC_EXECUTE);
#else
    wxUnusedVar(path);
    // TODO
    return false;
#endif
}

// Return the type of an open file
//
// Some file types on some platforms seem seekable but in fact are not.
// The main use of this function is to allow such cases to be detected
// (IsSeekable() is implemented as wxGetFileKind() == wxFILE_KIND_DISK).
//
// This is important for the archive streams, which benefit greatly from
// being able to seek on a stream, but which will produce corrupt archives
// if they unknowingly seek on a non-seekable stream.
//
// wxFILE_KIND_DISK is a good catch all return value, since other values
// disable features of the archive streams. Some other value must be returned
// for a file type that appears seekable but isn't.
//
// Known examples:
//   *  Pipes on Windows
//   *  Files on VMS with a record format other than StreamLF
//
wxFileKind wxGetFileKind(int fd)
{
#if defined __WINDOWS__ && !defined __WXWINCE__ && defined wxGetOSFHandle
    switch (::GetFileType(wxGetOSFHandle(fd)) & ~FILE_TYPE_REMOTE)
    {
        case FILE_TYPE_CHAR:
            return wxFILE_KIND_TERMINAL;
        case FILE_TYPE_DISK:
            return wxFILE_KIND_DISK;
        case FILE_TYPE_PIPE:
            return wxFILE_KIND_PIPE;
    }

    return wxFILE_KIND_UNKNOWN;

#elif defined(__UNIX__)
    if (isatty(fd))
        return wxFILE_KIND_TERMINAL;

    struct stat st;
    fstat(fd, &st);

    if (S_ISFIFO(st.st_mode))
        return wxFILE_KIND_PIPE;
    if (!S_ISREG(st.st_mode))
        return wxFILE_KIND_UNKNOWN;

    #if defined(__VMS__)
        if (st.st_fab_rfm != FAB$C_STMLF)
            return wxFILE_KIND_UNKNOWN;
    #endif

    return wxFILE_KIND_DISK;

#else
    #define wxFILEKIND_STUB
    (void)fd;
    return wxFILE_KIND_DISK;
#endif
}

wxFileKind wxGetFileKind(FILE *fp)
{
    // Note: The watcom rtl dll doesn't have fileno (the static lib does).
    //       Should be fixed in version 1.4.
#if defined(wxFILEKIND_STUB) || wxONLY_WATCOM_EARLIER_THAN(1,4)
    (void)fp;
    return wxFILE_KIND_DISK;
#elif defined(__WINDOWS__) && !defined(__CYGWIN__) && !defined(__WATCOMC__) && !defined(__WINE__)
    return fp ? wxGetFileKind(_fileno(fp)) : wxFILE_KIND_UNKNOWN;
#else
    return fp ? wxGetFileKind(fileno(fp)) : wxFILE_KIND_UNKNOWN;
#endif
}


//------------------------------------------------------------------------
// wild character routines
//------------------------------------------------------------------------

bool wxIsWild( const wxString& pattern )
{
    for ( wxString::const_iterator p = pattern.begin(); p != pattern.end(); ++p )
    {
        switch ( (*p).GetValue() )
        {
            case wxT('?'):
            case wxT('*'):
            case wxT('['):
            case wxT('{'):
                return true;

            case wxT('\\'):
                if ( ++p == pattern.end() )
                    return false;
        }
    }
    return false;
}

/*
* Written By Douglas A. Lewis <dalewis@cs.Buffalo.EDU>
*
* The match procedure is public domain code (from ircII's reg.c)
* but modified to suit our tastes (RN: No "%" syntax I guess)
*/

bool wxMatchWild( const wxString& pat, const wxString& text, bool dot_special )
{
    if (text.empty())
    {
        /* Match if both are empty. */
        return pat.empty();
    }

    const wxChar *m = pat.c_str(),
    *n = text.c_str(),
    *ma = NULL,
    *na = NULL;
    int just = 0,
    acount = 0,
    count = 0;

    if (dot_special && (*n == wxT('.')))
    {
        /* Never match so that hidden Unix files
         * are never found. */
        return false;
    }

    for (;;)
    {
        if (*m == wxT('*'))
        {
            ma = ++m;
            na = n;
            just = 1;
            acount = count;
        }
        else if (*m == wxT('?'))
        {
            m++;
            if (!*n++)
                return false;
        }
        else
        {
            if (*m == wxT('\\'))
            {
                m++;
                /* Quoting "nothing" is a bad thing */
                if (!*m)
                    return false;
            }
            if (!*m)
            {
                /*
                * If we are out of both strings or we just
                * saw a wildcard, then we can say we have a
                * match
                */
                if (!*n)
                    return true;
                if (just)
                    return true;
                just = 0;
                goto not_matched;
            }
            /*
            * We could check for *n == NULL at this point, but
            * since it's more common to have a character there,
            * check to see if they match first (m and n) and
            * then if they don't match, THEN we can check for
            * the NULL of n
            */
            just = 0;
            if (*m == *n)
            {
                m++;
                count++;
                n++;
            }
            else
            {

                not_matched:

                /*
                * If there are no more characters in the
                * string, but we still need to find another
                * character (*m != NULL), then it will be
                * impossible to match it
                */
                if (!*n)
                    return false;

                if (ma)
                {
                    m = ma;
                    n = ++na;
                    count = acount;
                }
                else
                    return false;
            }
        }
    }
}

#ifdef __VISUALC__
    #pragma warning(default:4706)   // assignment within conditional expression
#endif // VC++
