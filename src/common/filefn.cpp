/////////////////////////////////////////////////////////////////////////////
// Name:        filefn.cpp
// Purpose:     File- and directory-related functions
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "filefn.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/defs.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/file.h"
#include "wx/filename.h"

// there are just too many of those...
#ifdef __VISUALC__
    #pragma warning(disable:4706)   // assignment within conditional expression
#endif // VC++

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(__WATCOMC__)
    #if !(defined(_MSC_VER) && (_MSC_VER > 800))
        #include <errno.h>
    #endif
#endif

#include <time.h>

#ifndef __MWERKS__
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #include <stat.h>
    #include <unistd.h>
    #include <unix.h>
#endif

#ifdef __UNIX__
    #include <unistd.h>
    #include <dirent.h>
#endif

#ifdef __WXPM__
    #include <process.h>
    #include "wx/os2/private.h"
#endif
#ifdef __WINDOWS__
#if !defined( __GNUWIN32__ ) && !defined( __MWERKS__ ) && !defined(__SALFORDC__)
    #include <direct.h>
    #include <dos.h>
    #include <io.h>
#endif // __WINDOWS__
#endif // native Win compiler

#ifdef __GNUWIN32__
    #include <wchar.h>
    #ifndef __TWIN32__
        #include <sys/unistd.h>
    #endif
#endif

#ifdef __BORLANDC__ // Please someone tell me which version of Borland needs
                    // this (3.1 I believe) and how to test for it.
                    // If this works for Borland 4.0 as well, then no worries.
    #include <dir.h>
#endif

#ifdef __SALFORDC__
    #include <dir.h>
    #include <unix.h>
#endif

#include "wx/setup.h"
#include "wx/log.h"

// No, Cygwin doesn't appear to have fnmatch.h after all.
#if defined(HAVE_FNMATCH_H)
    #include "fnmatch.h"
#endif

#ifdef __WINDOWS__
    #include <windows.h>
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#define _MAXPATHLEN 500

extern wxChar *wxBuffer;

#ifdef __WXMAC__
#    include "MoreFiles.h"
#    include "MoreFilesExtras.h"
#    include "FullPath.h"
#    include "FSpCompat.h"
#endif

IMPLEMENT_DYNAMIC_CLASS(wxPathList, wxStringList)

// ----------------------------------------------------------------------------
// private globals
// ----------------------------------------------------------------------------

static wxChar wxFileFunctionsBuffer[4*_MAXPATHLEN];

#if defined(__VISAGECPP__) && __IBMCPP__ >= 400
//
// VisualAge C++ V4.0 cannot have any external linkage const decs
// in headers included by more than one primary source
//
const off_t wxInvalidOffset = (off_t)-1;
#endif

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// we need to translate Mac filenames before passing them to OS functions
#define OS_FILENAME(s) (s.fn_str())

// ============================================================================
// implementation
// ============================================================================

void wxPathList::Add (const wxString& path)
{
    wxStringList::Add (WXSTRINGCAST path);
}

// Add paths e.g. from the PATH environment variable
void wxPathList::AddEnvList (const wxString& envVariable)
{
  static const wxChar PATH_TOKS[] =
#ifdef __WINDOWS__
        wxT(" ;"); // Don't seperate with colon in DOS (used for drive)
#else
        wxT(" :;");
#endif

  wxChar *val = wxGetenv (WXSTRINGCAST envVariable);
  if (val && *val)
    {
      wxChar *s = copystring (val);
      wxChar *save_ptr, *token = wxStrtok (s, PATH_TOKS, &save_ptr);

      if (token)
      {
          Add (copystring (token));
          while (token)
          {
              if ((token = wxStrtok ((wxChar *) NULL, PATH_TOKS, &save_ptr)) != NULL)
                  Add (wxString(token));
          }
      }

      // suppress warning about unused variable save_ptr when wxStrtok() is a
      // macro which throws away its third argument
      save_ptr = token;

      delete [] s;
    }
}

// Given a full filename (with path), ensure that that file can
// be accessed again USING FILENAME ONLY by adding the path
// to the list if not already there.
void wxPathList::EnsureFileAccessible (const wxString& path)
{
    wxString path_only(wxPathOnly(path));
    if ( !path_only.IsEmpty() )
    {
        if ( !Member(path_only) )
            Add(path_only);
    }
}

bool wxPathList::Member (const wxString& path)
{
  for (wxNode * node = First (); node != NULL; node = node->Next ())
  {
      wxString path2((wxChar *) node->Data ());
      if (
#if defined(__WINDOWS__) || defined(__VMS__) || defined (__WXMAC__)
      // Case INDEPENDENT
          path.CompareTo (path2, wxString::ignoreCase) == 0
#else
      // Case sensitive File System
          path.CompareTo (path2) == 0
#endif
        )
        return TRUE;
  }
  return FALSE;
}

wxString wxPathList::FindValidPath (const wxString& file)
{
  if (wxFileExists (wxExpandPath(wxFileFunctionsBuffer, file)))
    return wxString(wxFileFunctionsBuffer);

  wxChar buf[_MAXPATHLEN];
  wxStrcpy(buf, wxFileFunctionsBuffer);

  wxChar *filename = (wxChar*) NULL; /* shut up buggy egcs warning */
  filename = IsAbsolutePath (buf) ? wxFileNameFromPath (buf) : (wxChar *)buf;

  for (wxNode * node = First (); node; node = node->Next ())
    {
      wxChar *path = (wxChar *) node->Data ();
      wxStrcpy (wxFileFunctionsBuffer, path);
      wxChar ch = wxFileFunctionsBuffer[wxStrlen(wxFileFunctionsBuffer)-1];
      if (ch != wxT('\\') && ch != wxT('/'))
        wxStrcat (wxFileFunctionsBuffer, wxT("/"));
      wxStrcat (wxFileFunctionsBuffer, filename);
#ifdef __WINDOWS__
      Unix2DosFilename (wxFileFunctionsBuffer);
#endif
      if (wxFileExists (wxFileFunctionsBuffer))
      {
        return wxString(wxFileFunctionsBuffer);        // Found!
      }
    }                                // for()

  return wxString(wxT(""));                    // Not found
}

wxString wxPathList::FindAbsoluteValidPath (const wxString& file)
{
    wxString f = FindValidPath(file);
    if ( wxIsAbsolutePath(f) )
        return f;

    wxString buf;
    wxGetWorkingDirectory(buf.GetWriteBuf(_MAXPATHLEN), _MAXPATHLEN - 1);
    buf.UngetWriteBuf();
    if ( !wxEndsWithPathSeparator(buf) )
    {
        buf += wxFILE_SEP_PATH;
    }
    buf += f;

    return buf;
}

bool
wxFileExists (const wxString& filename)
{
#ifdef __WINDOWS__
    // GetFileAttributes can copy with network paths
    DWORD ret = GetFileAttributes(filename);
    DWORD isDir = (ret & FILE_ATTRIBUTE_DIRECTORY);
    return ((ret != 0xffffffff) && (isDir == 0));
#else
    wxStructStat stbuf;
    if ( !filename.empty() && wxStat (OS_FILENAME(filename), &stbuf) == 0 )
        return TRUE;

    return FALSE;
#endif
}

bool
wxIsAbsolutePath (const wxString& filename)
{
    if (filename != wxT(""))
    {
#if defined(__WXMAC__) && !defined(__DARWIN__)
        // Classic or Carbon CodeWarrior like
        // Carbon with Apple DevTools is Unix like
        
        // This seems wrong to me, but there is no fix. since
        // "MacOS:MyText.txt" is absolute whereas "MyDir:MyText.txt"
        // is not. Or maybe ":MyDir:MyText.txt" has to be used? RR.
        if (filename.Find(':') != wxNOT_FOUND && filename[0] != ':')
            return TRUE ;
#else
        // Unix like or Windows
        if (filename[0] == wxT('/'))
            return TRUE;
#endif
#ifdef __VMS__
        if ((filename[0] == wxT('[') && filename[1] != wxT('.')))
            return TRUE;
#endif
#ifdef __WINDOWS__
        // MSDOS like
        if (filename[0] == wxT('\\') || (wxIsalpha (filename[0]) && filename[1] == wxT(':')))
            return TRUE;
#endif
    }
    return FALSE ;
}

/*
 * Strip off any extension (dot something) from end of file,
 * IF one exists. Inserts zero into buffer.
 *
 */

void wxStripExtension(wxChar *buffer)
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

void wxStripExtension(wxString& buffer)
{
  size_t len = buffer.Length();
  size_t i = len-1;
  while (i > 0)
  {
    if (buffer.GetChar(i) == wxT('.'))
    {
      buffer = buffer.Left(i);
      break;
    }
    i --;
  }
}

// Destructive removal of /./ and /../ stuff
wxChar *wxRealPath (wxChar *path)
{
#ifdef __WXMSW__
  static const wxChar SEP = wxT('\\');
  Unix2DosFilename(path);
#else
  static const wxChar SEP = wxT('/');
#endif
  if (path[0] && path[1]) {
    /* MATTHEW: special case "/./x" */
    wxChar *p;
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
                wxChar *q;
                for (q = p - 1; q >= path && *q != SEP; q--);
                if (q[0] == SEP && (q[1] != wxT('.') || q[2] != wxT('.') || q[3] != SEP)
                    && (q - 1 <= path || q[-1] != SEP))
                  {
                    wxStrcpy (q, p + 3);
                    if (path[0] == wxT('\0'))
                      {
                        path[0] = SEP;
                        path[1] = wxT('\0');
                      }
#ifdef __WXMSW__
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

// Must be destroyed
wxChar *wxCopyAbsolutePath(const wxString& filename)
{
  if (filename == wxT(""))
    return (wxChar *) NULL;

  if (! IsAbsolutePath(wxExpandPath(wxFileFunctionsBuffer, filename))) {
    wxChar  buf[_MAXPATHLEN];
    buf[0] = wxT('\0');
    wxGetWorkingDirectory(buf, WXSIZEOF(buf));
    wxChar ch = buf[wxStrlen(buf) - 1];
#ifdef __WXMSW__
    if (ch != wxT('\\') && ch != wxT('/'))
        wxStrcat(buf, wxT("\\"));
#else
    if (ch != wxT('/'))
        wxStrcat(buf, wxT("/"));
#endif
    wxStrcat(buf, wxFileFunctionsBuffer);
    return copystring( wxRealPath(buf) );
  }
  return copystring( wxFileFunctionsBuffer );
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

wxChar *wxExpandPath(wxChar *buf, const wxChar *name)
{
    register wxChar *d, *s, *nm;
    wxChar          lnm[_MAXPATHLEN];
    int             q;

    // Some compilers don't like this line.
//    const wxChar    trimchars[] = wxT("\n \t");

    wxChar      trimchars[4];
    trimchars[0] = wxT('\n');
    trimchars[1] = wxT(' ');
    trimchars[2] = wxT('\t');
    trimchars[3] = 0;

#ifdef __WXMSW__
     const wxChar     SEP = wxT('\\');
#else
     const wxChar     SEP = wxT('/');
#endif
    buf[0] = wxT('\0');
    if (name == NULL || *name == wxT('\0'))
        return buf;
    nm = copystring(name); // Make a scratch copy
    wxChar *nm_tmp = nm;

    /* Skip leading whitespace and cr */
    while (wxStrchr((wxChar *)trimchars, *nm) != NULL)
        nm++;
    /* And strip off trailing whitespace and cr */
    s = nm + (q = wxStrlen(nm)) - 1;
    while (q-- && wxStrchr((wxChar *)trimchars, *s) != NULL)
        *s = wxT('\0');

    s = nm;
    d = lnm;
#ifdef __WXMSW__
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
#  ifndef __WXMSW__
        if (*s == wxT('\\')) {
            if ((*(d - 1) = *++s)) {
                s++;
                continue;
            } else
                break;
        } else
#  endif
#endif
#ifdef __WXMSW__
        if (*s++ == wxT('$') && (*s == wxT('{') || *s == wxT(')')))
#else
        if (*s++ == wxT('$'))
#endif
        {
            register wxChar  *start = d;
            register int     braces = (*s == wxT('{') || *s == wxT('('));
            register wxChar  *value;
            while ((*d++ = *s) != 0)
                if (braces ? (*s == wxT('}') || *s == wxT(')')) : !(wxIsalnum(*s) || *s == wxT('_')) )
                    break;
                else
                    s++;
            *--d = 0;
            value = wxGetenv(braces ? start + 1 : start);
            if (value) {
                for ((d = start - 1); (*d++ = *value++) != 0;);
                d--;
                if (braces && *s)
                    s++;
            }
        }
    }

    /* Expand ~ and ~user */
    nm = lnm;
    s = wxT("");
    if (nm[0] == wxT('~') && !q)
    {
        /* prefix ~ */
        if (nm[1] == SEP || nm[1] == 0)
        {        /* ~/filename */
        // FIXME: wxGetUserHome could return temporary storage in Unicode mode
            if ((s = WXSTRINGCAST wxGetUserHome(wxT(""))) != NULL) {
                if (*++nm)
                    nm++;
            }
        } else
        {                /* ~user/filename */
            register wxChar  *nnm;
            register wxChar  *home;
            for (s = nm; *s && *s != SEP; s++);
            int was_sep; /* MATTHEW: Was there a separator, or NULL? */
            was_sep = (*s == SEP);
            nnm = *s ? s + 1 : s;
            *s = 0;
        // FIXME: wxGetUserHome could return temporary storage in Unicode mode
            if ((home = WXSTRINGCAST wxGetUserHome(wxString(nm + 1))) == NULL) {
               if (was_sep) /* replace only if it was there: */
                   *s = SEP;
                s = wxT("");
            } else {
                nm = nnm;
                s = home;
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
    while ((*d++ = *s++) != 0);
    delete[] nm_tmp; // clean up alloc
    /* Now clean up the buffer */
    return wxRealPath(buf);
}

/* Contract Paths to be build upon an environment variable
   component:

   example: "/usr/openwin/lib", OPENWINHOME --> ${OPENWINHOME}/lib

   The call wxExpandPath can convert these back!
 */
wxChar *
wxContractPath (const wxString& filename, const wxString& envname, const wxString& user)
{
  static wxChar dest[_MAXPATHLEN];

  if (filename == wxT(""))
    return (wxChar *) NULL;

  wxStrcpy (dest, WXSTRINGCAST filename);
#ifdef __WXMSW__
  Unix2DosFilename(dest);
#endif

  // Handle environment
  const wxChar *val = (const wxChar *) NULL;
  wxChar *tcp = (wxChar *) NULL;
  if (envname != WXSTRINGCAST NULL && (val = wxGetenv (WXSTRINGCAST envname)) != NULL &&
     (tcp = wxStrstr (dest, val)) != NULL)
    {
        wxStrcpy (wxFileFunctionsBuffer, tcp + wxStrlen (val));
        *tcp++ = wxT('$');
        *tcp++ = wxT('{');
        wxStrcpy (tcp, WXSTRINGCAST envname);
        wxStrcat (tcp, wxT("}"));
        wxStrcat (tcp, wxFileFunctionsBuffer);
    }

  // Handle User's home (ignore root homes!)
  size_t len = 0;
  if ((val = wxGetUserHome (user)) != NULL &&
      (len = wxStrlen(val)) > 2 &&
      wxStrncmp(dest, val, len) == 0)
    {
      wxStrcpy(wxFileFunctionsBuffer, wxT("~"));
      if (user != wxT(""))
             wxStrcat(wxFileFunctionsBuffer, (const wxChar*) user);
#ifdef __WXMSW__
//      strcat(wxFileFunctionsBuffer, "\\");
#else
//      strcat(wxFileFunctionsBuffer, "/");
#endif
      wxStrcat(wxFileFunctionsBuffer, dest + len);
      wxStrcpy (dest, wxFileFunctionsBuffer);
    }

  return dest;
}

// Return just the filename, not the path
// (basename)
wxChar *wxFileNameFromPath (wxChar *path)
{
    if (path)
    {
        register wxChar *tcp;
        
        tcp = path + wxStrlen (path);
        while (--tcp >= path)
        {
#if defined(__WXMAC__) && !defined(__DARWIN__)
            // Classic or Carbon CodeWarrior like
            // Carbon with Apple DevTools is Unix like
            if (*tcp == wxT(':'))
                return tcp + 1;
#else
            // Unix like or Windows
            if (*tcp == wxT('/') || *tcp == wxT('\\'))
                return tcp + 1;
#endif
#ifdef __VMS__
            if (*tcp == wxT(':') || *tcp == wxT(']'))
                return tcp + 1;
#endif
        } /* while */
#if defined(__WXMSW__) || defined(__WXPM__)
        // MSDOS like
        if (wxIsalpha (*path) && *(path + 1) == wxT(':'))
            return path + 2;
#endif
    }
    return path;
}

wxString wxFileNameFromPath (const wxString& path1)
{
    if (path1 != wxT(""))
    {
        wxChar *path = WXSTRINGCAST path1 ;
        register wxChar *tcp;
        
        tcp = path + wxStrlen (path);
        while (--tcp >= path)
        {
#if defined(__WXMAC__) && !defined(__DARWIN__)
            // Classic or Carbon CodeWarrior like
            // Carbon with Apple DevTools is Unix like
            if (*tcp == wxT(':') )
                return wxString(tcp + 1);
#else
            // Unix like or Windows
            if (*tcp == wxT('/') || *tcp == wxT('\\'))
                return wxString(tcp + 1);
#endif
#ifdef __VMS__
            if (*tcp == wxT(':') || *tcp == wxT(']'))
                return wxString(tcp + 1);
#endif
        } /* while */
#if defined(__WXMSW__) || defined(__WXPM__)
        // MSDOS like
        if (wxIsalpha (*path) && *(path + 1) == wxT(':'))
            return wxString(path + 2);
#endif
    }
    // Yes, this should return the path, not an empty string, otherwise
    // we get "thing.txt" -> "".
    return path1;
}

// Return just the directory, or NULL if no directory
wxChar *
wxPathOnly (wxChar *path)
{
    if (path && *path)
    {
        static wxChar buf[_MAXPATHLEN];
        
        // Local copy
        wxStrcpy (buf, path);
        
        int l = wxStrlen(path);
        int i = l - 1;
        
        // Search backward for a backward or forward slash
        while (i > -1)
        {
#if defined(__WXMAC__) && !defined(__DARWIN__)
            // Classic or Carbon CodeWarrior like
            // Carbon with Apple DevTools is Unix like
            if (path[i] == wxT(':') )
            {
                buf[i] = 0;
                return buf;
            }
#else
            // Unix like or Windows
            if (path[i] == wxT('/') || path[i] == wxT('\\'))
            {
                buf[i] = 0;
                return buf;
            }
#endif
#ifdef __VMS__
            if (path[i] == wxT(']'))
            {
                buf[i+1] = 0;
                return buf;
            }
#endif
            i --;
        }
        
#if defined(__WXMSW__) || defined(__WXPM__)
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
    return (wxChar *) NULL;
}

// Return just the directory, or NULL if no directory
wxString wxPathOnly (const wxString& path)
{
    if (path != wxT(""))
    {
        wxChar buf[_MAXPATHLEN];
        
        // Local copy
        wxStrcpy (buf, WXSTRINGCAST path);
        
        int l = path.Length();
        int i = l - 1;

        // Search backward for a backward or forward slash
        while (i > -1)
        {
#if defined(__WXMAC__) && !defined(__DARWIN__)
            // Classic or Carbon CodeWarrior like
            // Carbon with Apple DevTools is Unix like
            if (path[i] == wxT(':') )
            {
                buf[i] = 0;
                return wxString(buf);
            }
#else
            // Unix like or Windows
            if (path[i] == wxT('/') || path[i] == wxT('\\'))
            {
                buf[i] = 0;
                return wxString(buf);
            }
#endif
#ifdef __VMS__
            if (path[i] == wxT(']'))
            {
                buf[i+1] = 0;
                return wxString(buf);
            }
#endif
            i --;
        }
        
#if defined(__WXMSW__) || defined(__WXPM__)
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
    return wxString(wxT(""));
}

// Utility for converting delimiters in DOS filenames to UNIX style
// and back again - or we get nasty problems with delimiters.
// Also, convert to lower case, since case is significant in UNIX.

#if defined(__WXMAC__)
wxString wxMacFSSpec2MacFilename( const FSSpec *spec )
{
    Handle    myPath ;
    short     length ;

    FSpGetFullPath( spec , &length , &myPath ) ;
    ::SetHandleSize( myPath , length + 1 ) ;
    ::HLock( myPath ) ;
    (*myPath)[length] = 0 ;
    if ( length > 0 && (*myPath)[length-1] ==':' )
        (*myPath)[length-1] = 0 ;

#ifdef __DARWIN__
    wxString result( wxMac2UnixFilename((char*) *myPath) ) ;
#else
    wxString result( (char*) *myPath ) ;
#endif
    ::HUnlock( myPath ) ;
    ::DisposeHandle( myPath ) ;
    return result ;
}

void wxMacFilename2FSSpec( const char *path , FSSpec *spec )
{
#ifdef __DARWIN__
    const char *s = wxUnix2MacFilename(path);
    FSpLocationFromFullPath( strlen(s) , s , spec ) ;
#else
    FSpLocationFromFullPath( strlen(path) , path , spec ) ;
#endif
}

static char sMacFileNameConversion[ 1000 ] ;

wxString wxMac2UnixFilename (const char *str)
{
    char *s = sMacFileNameConversion ;
    strcpy( s , str ) ;
  if (s)
  {
    memmove( s+1 , s ,strlen( s ) + 1) ;
    if ( *s == ':' )
            *s = '.' ;
    else
            *s = '/' ;

    while (*s)
    {
            if (*s == ':')
              *s = '/';
            else
              *s = wxTolower (*s);        // Case INDEPENDENT
            s++;
    }
  }
  return wxString (sMacFileNameConversion) ;
}

wxString wxUnix2MacFilename (const char *str)
{
    char *s = sMacFileNameConversion ;
    strcpy( s , str ) ;
  if (s)
  {
    if ( *s == '.' )
    {
      // relative path , since it goes on with slash which is translated to a :
      memmove( s , s+1 ,strlen( s ) ) ;
    }
    else if ( *s == '/' )
    {
      // absolute path -> on mac just start with the drive name
      memmove( s , s+1 ,strlen( s ) ) ;
    }
    else
    {
      wxASSERT_MSG( 1 , "unkown path beginning" ) ;
    }
    while (*s)
    {
      if (*s == '/' || *s == '\\')
      {
          // convert any back-directory situations
          if ( *(s+1) == '.' && *(s+2) == '.' && ( (*(s+3) == '/' || *(s+3) == '\\') ) )
          {
          *s = ':';
                memmove( s+1 , s+3 ,strlen( s+3 ) + 1 ) ;
          }
          else
          *s = ':';
      }

      s++ ;
    }
  }
  return wxString (sMacFileNameConversion) ;
}

wxString wxMacFSSpec2UnixFilename( const FSSpec *spec )
{
    return wxMac2UnixFilename( wxMacFSSpec2MacFilename( spec) ) ;
}

void wxUnixFilename2FSSpec( const char *path , FSSpec *spec )
{
    wxString var = wxUnix2MacFilename( path ) ;
    wxMacFilename2FSSpec( var , spec ) ;
}

#endif // __WXMAC__

void
wxDos2UnixFilename (char *s)
{
  if (s)
    while (*s)
      {
        if (*s == '\\')
          *s = '/';
#ifdef __WXMSW__
        else
          *s = wxTolower (*s);        // Case INDEPENDENT
#endif
        s++;
      }
}

void
#if defined(__WXMSW__) || defined(__WXPM__)
wxUnix2DosFilename (wxChar *s)
#else
wxUnix2DosFilename (wxChar *WXUNUSED(s) )
#endif
{
// Yes, I really mean this to happen under DOS only! JACS
#if defined(__WXMSW__) || defined(__WXPM__)
  if (s)
    while (*s)
      {
        if (*s == wxT('/'))
          *s = wxT('\\');
        s++;
      }
#endif
}

// Concatenate two files to form third
bool
wxConcatFiles (const wxString& file1, const wxString& file2, const wxString& file3)
{
  wxString outfile;
  if ( !wxGetTempFileName("cat", outfile) )
      return FALSE;

  FILE *fp1 = (FILE *) NULL;
  FILE *fp2 = (FILE *) NULL;
  FILE *fp3 = (FILE *) NULL;
  // Open the inputs and outputs
  if ((fp1 = wxFopen (OS_FILENAME( file1 ), wxT("rb"))) == NULL ||
      (fp2 = wxFopen (OS_FILENAME( file2 ), wxT("rb"))) == NULL ||
      (fp3 = wxFopen (OS_FILENAME( outfile ), wxT("wb"))) == NULL)
    {
      if (fp1)
        fclose (fp1);
      if (fp2)
        fclose (fp2);
      if (fp3)
        fclose (fp3);
      return FALSE;
    }

  int ch;
  while ((ch = getc (fp1)) != EOF)
    (void) putc (ch, fp3);
  fclose (fp1);

  while ((ch = getc (fp2)) != EOF)
    (void) putc (ch, fp3);
  fclose (fp2);

  fclose (fp3);
  bool result = wxRenameFile(outfile, file3);
  return result;
}

// Copy files
bool
wxCopyFile (const wxString& file1, const wxString& file2, bool overwrite)
{
#if defined(__WIN32__) && !defined(__WXMICROWIN__)
    // CopyFile() copies file attributes and modification time too, so use it
    // instead of our code if available
    //
    // NB: 3rd parameter is bFailIfExists i.e. the inverse of overwrite
    return ::CopyFile(file1, file2, !overwrite) != 0;
#elif defined(__WXPM__)
    if (::DosCopy(file2, file2, overwrite ? DCPY_EXISTING : 0) == 0)
        return TRUE;
    else
        return FALSE;
#else // !Win32
    wxStructStat fbuf;

    // get permissions of file1
    if ( wxStat(OS_FILENAME(file1), &fbuf) != 0 )
    {
        // the file probably doesn't exist or we haven't the rights to read
        // from it anyhow
        wxLogSysError(_("Impossible to get permissions for file '%s'"),
                      file1.c_str());
        return FALSE;
    }

    // open file1 for reading
    wxFile fileIn(file1, wxFile::read);
    if ( !fileIn.IsOpened() )
        return FALSE;

    // remove file2, if it exists. This is needed for creating
    // file2 with the correct permissions in the next step
    if ( wxFileExists(file2)  && (!overwrite || !wxRemoveFile(file2)))
    {
        wxLogSysError(_("Impossible to overwrite the file '%s'"),
                      file2.c_str());
        return FALSE;
    }

#ifdef __UNIX__
    // reset the umask as we want to create the file with exactly the same
    // permissions as the original one
    mode_t oldUmask = umask( 0 );
#endif // __UNIX__

    // create file2 with the same permissions than file1 and open it for
    // writing
    wxFile fileOut;
    if ( !fileOut.Create(file2, overwrite, fbuf.st_mode & 0777) )
        return FALSE;

#ifdef __UNIX__
    /// restore the old umask
    umask(oldUmask);
#endif // __UNIX__

    // copy contents of file1 to file2
    char buf[4096];
    size_t count;
    for ( ;; )
    {
        count = fileIn.Read(buf, WXSIZEOF(buf));
        if ( fileIn.Error() )
            return FALSE;

        // end of file?
        if ( !count )
            break;

        if ( fileOut.Write(buf, count) < count )
            return FALSE;
    }

    // we can expect fileIn to be closed successfully, but we should ensure
    // that fileOut was closed as some write errors (disk full) might not be
    // detected before doing this
    if ( !fileIn.Close() || !fileOut.Close() )
        return FALSE;

#if !defined(__VISAGECPP__) && !defined(__WXMAC__) || defined(__UNIX__)
    // no chmod in VA.  Should be some permission API for HPFS386 partitions
    // however
    if ( chmod(OS_FILENAME(file2), fbuf.st_mode) != 0 )
    {
        wxLogSysError(_("Impossible to set permissions for the file '%s'"),
                      file2.c_str());
        return FALSE;
    }
#endif // OS/2 || Mac

    return TRUE;
#endif // __WXMSW__ && __WIN32__
}

bool
wxRenameFile (const wxString& file1, const wxString& file2)
{
  // Normal system call
  if ( wxRename (file1, file2) == 0 )
    return TRUE;

  // Try to copy
  if (wxCopyFile(file1, file2)) {
    wxRemoveFile(file1);
    return TRUE;
  }
  // Give up
  return FALSE;
}

bool wxRemoveFile(const wxString& file)
{
#if defined(__VISUALC__) \
 || defined(__BORLANDC__) \
 || defined(__WATCOMC__) \
 || defined(__GNUWIN32__)
  int res = wxRemove(file);
#else
  int res = unlink(OS_FILENAME(file));
#endif

  return res == 0;
}

bool wxMkdir(const wxString& dir, int perm)
{
#if defined(__WXMAC__) && !defined(__UNIX__)
  return (mkdir( dir , 0 ) == 0);
#else // !Mac
    const wxChar *dirname = dir.c_str();

    // assume mkdir() has 2 args on non Windows-OS/2 platforms and on Windows too
    // for the GNU compiler
#if (!(defined(__WXMSW__) || defined(__WXPM__))) || (defined(__GNUWIN32__) && !defined(__MINGW32__)) || defined(__WXWINE__) || defined(__WXMICROWIN__)
    if ( mkdir(wxFNCONV(dirname), perm) != 0 )
#elif defined(__WXPM__)
    if (::DosCreateDir((PSZ)dirname, NULL) != 0) // enhance for EAB's??
#else  // !MSW and !OS/2 VAC++
    (void)perm;
    if ( wxMkDir(wxFNSTRINGCAST wxFNCONV(dirname)) != 0 )
#endif // !MSW/MSW
    {
        wxLogSysError(_("Directory '%s' couldn't be created"), dirname);

        return FALSE;
    }

    return TRUE;
#endif // Mac/!Mac
}

bool wxRmdir(const wxString& dir, int WXUNUSED(flags))
{
#ifdef __VMS__
  return FALSE; //to be changed since rmdir exists in VMS7.x
#elif defined(__WXPM__)
  return (::DosDeleteDir((PSZ)dir.c_str()) == 0);
#else

#ifdef __SALFORDC__
  return FALSE; // What to do?
#else
  return (wxRmDir(OS_FILENAME(dir)) == 0);
#endif

#endif
}

// does the path exists? (may have or not '/' or '\\' at the end)
bool wxPathExists(const wxChar *pszPathName)
{
    wxString strPath(pszPathName);
#ifdef __WINDOWS__
    // Windows fails to find directory named "c:\dir\" even if "c:\dir" exists,
    // so remove all trailing backslashes from the path - but don't do this for
    // the pathes "d:\" (which are different from "d:") nor for just "\"
    while ( wxEndsWithPathSeparator(strPath) )
    {
        size_t len = strPath.length();
        if ( len == 1 || (len == 3 && strPath[len - 2] == _T(':')) )
            break;

        strPath.Truncate(len - 1);
    }
#endif // __WINDOWS__

#ifdef __WINDOWS__
    // Stat can't cope with network paths
    DWORD ret = GetFileAttributes(strPath.c_str());
    DWORD isDir = (ret & FILE_ATTRIBUTE_DIRECTORY);
    return ((ret != 0xffffffff) && (isDir != 0));
#else

    wxStructStat st;
#ifndef __VISAGECPP__
    return wxStat(wxFNSTRINGCAST strPath.fn_str(), &st) == 0 &&
        ((st.st_mode & S_IFMT) == S_IFDIR);
#else
    // S_IFMT not supported in VA compilers.. st_mode is a 2byte value only
    return wxStat(wxFNSTRINGCAST strPath.fn_str(), &st) == 0 &&
        (st.st_mode == S_IFDIR);
#endif

#endif
}

// Get a temporary filename, opening and closing the file.
wxChar *wxGetTempFileName(const wxString& prefix, wxChar *buf)
{
#if defined(__WINDOWS__) && !defined(__WXMICROWIN__)

#ifndef        __WIN32__
  wxChar tmp[144];
  ::GetTempFileName(0, WXSTRINGCAST prefix, 0, tmp);
#else
  wxChar tmp[MAX_PATH];
  wxChar tmpPath[MAX_PATH];
  ::GetTempPath(MAX_PATH, tmpPath);
  ::GetTempFileName(tmpPath, WXSTRINGCAST prefix, 0, tmp);
#endif
  if (buf) wxStrcpy(buf, tmp);
  else buf = copystring(tmp);
  return buf;

#else
  static short last_temp = 0;        // cache last to speed things a bit
  // At most 1000 temp files to a process! We use a ring count.
  wxChar tmp[100]; // FIXME static buffer

  for (short suffix = last_temp + 1; suffix != last_temp; ++suffix %= 1000)
    {
      wxSprintf (tmp, wxT("/tmp/%s%d.%03x"), WXSTRINGCAST prefix, (int) getpid (), (int) suffix);
      if (!wxFileExists( tmp ))
        {
          // Touch the file to create it (reserve name)
          FILE *fd = fopen (wxFNCONV(tmp), "w");
          if (fd)
            fclose (fd);
          last_temp = suffix;
          if (buf)
            wxStrcpy( buf, tmp);
          else
            buf = copystring( tmp );
          return buf;
        }
    }
  wxLogError( _("wxWindows: error finding temporary file name.\n") );
  if (buf) buf[0] = 0;
  return (wxChar *) NULL;
#endif
}

bool wxGetTempFileName(const wxString& prefix, wxString& buf)
{
    wxChar buf2[512];
    if (wxGetTempFileName(prefix, buf2) != (wxChar*) NULL)
    {
        buf = buf2;
        return TRUE;
    }
    else
        return FALSE;
}

// Get first file name matching given wild card.

#if defined(__UNIX__) && !defined(__WXMAC__)

// Get first file name matching given wild card.
// Flags are reserved for future use.

#if !defined( __VMS__ ) || ( __VMS_VER >= 70000000 )
    static DIR *gs_dirStream = (DIR *) NULL;
    static wxString gs_strFileSpec;
    static int gs_findFlags = 0;
#endif

wxString wxFindFirstFile(const wxChar *spec, int flags)
{
    wxString result;
#ifdef __VMS
   wxChar *specvms = NULL;
#endif

#if !defined( __VMS__ ) || ( __VMS_VER >= 70000000 )
    if (gs_dirStream)
        closedir(gs_dirStream); // edz 941103: better housekeping

    gs_findFlags = flags;

    gs_strFileSpec = spec;

    // Find path only so we can concatenate
    // found file onto path
    wxString path(wxPathOnly(gs_strFileSpec));

    // special case: path is really "/"
    if ( !path && gs_strFileSpec[0u] == wxT('/') )
#ifdef __VMS
     {
        wxStrcpy( specvms , wxT( "[000000]" ) );
        gs_strFileSpec = specvms;
        wxString path_vms(wxPathOnly(gs_strFileSpec));
        path = path_vms;
     }
#else
     path = wxT('/');
#endif
   // path is empty => Local directory
    if ( !path )
#ifdef __VMS
     {
        wxStrcpy( specvms , wxT( "[]" ) );
        gs_strFileSpec = specvms;
        wxString path_vms1(wxPathOnly(gs_strFileSpec));
        path = path_vms1;
     }
#else
     path = wxT('.');
#endif

    gs_dirStream = opendir(path.fn_str());
    if ( !gs_dirStream )
    {
        wxLogSysError(_("Can not enumerate files in directory '%s'"),
                      path.c_str());
    }
    else
    {
        result = wxFindNextFile();
    }
#endif // !VMS6.x or earlier

    return result;
}

wxString wxFindNextFile()
{
    wxString result;

#if !defined( __VMS__ ) || ( __VMS_VER >= 70000000 )
    wxCHECK_MSG( gs_dirStream, result, wxT("must call wxFindFirstFile first") );

    // Find path only so we can concatenate
    // found file onto path
    wxString path(wxPathOnly(gs_strFileSpec));
    wxString name(wxFileNameFromPath(gs_strFileSpec));

    /* MATTHEW: special case: path is really "/" */
    if ( !path && gs_strFileSpec[0u] == wxT('/'))
        path = wxT('/');

    // Do the reading
    struct dirent *nextDir;
    for ( nextDir = readdir(gs_dirStream);
          nextDir != NULL;
          nextDir = readdir(gs_dirStream) )
    {
        if (wxMatchWild(name, nextDir->d_name, FALSE) &&   // RR: added FALSE to find hidden files
        strcmp(nextDir->d_name, ".") &&
        strcmp(nextDir->d_name, "..") )
        {
            result.Empty();
            if ( !path.IsEmpty() )
            {
                result = path;
                if ( path != wxT('/') )
                    result += wxT('/');
            }

            result += nextDir->d_name;

            // Only return "." and ".." when they match
            bool isdir;
            if ( (strcmp(nextDir->d_name, ".") == 0) ||
                 (strcmp(nextDir->d_name, "..") == 0))
            {
                if ( (gs_findFlags & wxDIR) != 0 )
                    isdir = TRUE;
                else
                    continue;
            }
            else
                isdir = wxDirExists(result);

            // and only return directories when flags & wxDIR
            if ( !gs_findFlags ||
                 ((gs_findFlags & wxDIR) && isdir) ||
                 ((gs_findFlags & wxFILE) && !isdir) )
            {
                return result;
            }
        }
    }

    result.Empty(); // not found

    closedir(gs_dirStream);
    gs_dirStream = (DIR *) NULL;
#endif // !VMS6.2 or earlier

    return result;
}

#elif defined(__WXMAC__)

struct MacDirectoryIterator
{
    CInfoPBRec            m_CPB ;
    wxInt16                m_index ;
    long                m_dirId ;
    Str255                m_name ;
} ;

static int g_iter_flags ;

static MacDirectoryIterator g_iter ;

wxString wxFindFirstFile(const wxChar *spec, int flags)
{
    wxString result;

    g_iter_flags = flags; /* MATTHEW: [5] Remember flags */

    // Find path only so we can concatenate found file onto path
    wxString path(wxPathOnly(spec));
    if ( !path.IsEmpty() )
        result << path << wxT('\\');

    FSSpec fsspec ;

    wxMacFilename2FSSpec( result , &fsspec ) ;
    g_iter.m_CPB.hFileInfo.ioVRefNum = fsspec.vRefNum ;
    g_iter.m_CPB.hFileInfo.ioNamePtr = g_iter.m_name ;
    g_iter.m_index = 0 ;

    Boolean isDir ;
    FSpGetDirectoryID( &fsspec , &g_iter.m_dirId , &isDir ) ;
    if ( !isDir )
        return wxEmptyString ;

    return wxFindNextFile( ) ;
}

wxString wxFindNextFile()
{
    wxString result;

    short err = noErr ;

    while ( err == noErr )
    {
        g_iter.m_index++ ;
        g_iter.m_CPB.dirInfo.ioFDirIndex = g_iter.m_index;
        g_iter.m_CPB.dirInfo.ioDrDirID = g_iter.m_dirId;    /* we need to do this every time */
        err = PBGetCatInfoSync((CInfoPBPtr)&g_iter.m_CPB);
        if ( err != noErr )
            break ;

        if ( ( g_iter.m_CPB.dirInfo.ioFlAttrib & ioDirMask) != 0 && (g_iter_flags & wxDIR) ) //  we have a directory
            break ;

        if ( ( g_iter.m_CPB.dirInfo.ioFlAttrib & ioDirMask) == 0 && !(g_iter_flags & wxFILE ) )
            continue ;

        // hit !
        break ;
    }
    if ( err != noErr )
    {
        return wxEmptyString ;
    }
    FSSpec spec ;

    FSMakeFSSpecCompat(g_iter.m_CPB.hFileInfo.ioVRefNum,
                                   g_iter.m_dirId,
                                   g_iter.m_name,
                                   &spec) ;

    return wxMacFSSpec2MacFilename( &spec ) ;
}

#elif defined(__WXMSW__)

#ifdef __WIN32__
    static HANDLE gs_hFileStruct = INVALID_HANDLE_VALUE;
    static WIN32_FIND_DATA gs_findDataStruct;
#else // Win16
    #ifdef __BORLANDC__
        static struct ffblk gs_findDataStruct;
    #else
        static struct _find_t gs_findDataStruct;
    #endif // Borland
#endif // Win32/16

static wxString gs_strFileSpec;
static int gs_findFlags = 0;

wxString wxFindFirstFile(const wxChar *spec, int flags)
{
    wxString result;

    gs_strFileSpec = spec;
    gs_findFlags = flags; /* MATTHEW: [5] Remember flags */

    // Find path only so we can concatenate found file onto path
    wxString path(wxPathOnly(gs_strFileSpec));
    if ( !path.IsEmpty() )
        result << path << wxT('\\');

#ifdef __WIN32__
    if ( gs_hFileStruct != INVALID_HANDLE_VALUE )
        FindClose(gs_hFileStruct);

    gs_hFileStruct = ::FindFirstFile(WXSTRINGCAST spec, &gs_findDataStruct);

    if ( gs_hFileStruct == INVALID_HANDLE_VALUE )
    {
        result.Empty();

        return result;
    }

    bool isdir = !!(gs_findDataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

    if (isdir && !(flags & wxDIR))
        return wxFindNextFile();
    else if (!isdir && flags && !(flags & wxFILE))
        return wxFindNextFile();

    result += gs_findDataStruct.cFileName;

    return result;
#else // !Win32
    int flag = _A_NORMAL;
    if (flags & wxDIR)
        flag = _A_SUBDIR;

#ifdef __BORLANDC__
    if (findfirst(WXSTRINGCAST spec, &gs_findDataStruct, flag) == 0)
#else
        if (_dos_findfirst(WXSTRINGCAST spec, flag, &gs_findDataStruct) == 0)
#endif
        {
            char attrib;

#ifdef __BORLANDC__
            attrib = gs_findDataStruct.ff_attrib;
#else
            attrib = gs_findDataStruct.attrib;
#endif

            if (attrib & _A_SUBDIR) {
                if (!(gs_findFlags & wxDIR))
                    return wxFindNextFile();
            } else if (gs_findFlags && !(gs_findFlags & wxFILE))
                return wxFindNextFile();

            result +=
#ifdef __BORLANDC__
                    gs_findDataStruct.ff_name
#else
                    gs_findDataStruct.name
#endif
                    ;
        }

    return result;
#endif // __WIN32__
}


wxString wxFindNextFile()
{
    wxString result;

    // Find path only so we can concatenate found file onto path
    wxString path(wxPathOnly(gs_strFileSpec));

try_again:

#ifdef __WIN32__
    if (gs_hFileStruct == INVALID_HANDLE_VALUE)
        return result;

    bool success = (FindNextFile(gs_hFileStruct, &gs_findDataStruct) != 0);
    if (!success)
    {
        FindClose(gs_hFileStruct);
        gs_hFileStruct = INVALID_HANDLE_VALUE;
    }
    else
    {
        bool isdir = !!(gs_findDataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

        if (isdir && !(gs_findFlags & wxDIR))
            goto try_again;
        else if (!isdir && gs_findFlags && !(gs_findFlags & wxFILE))
            goto try_again;

        if ( !path.IsEmpty() )
            result << path << wxT('\\');
        result << gs_findDataStruct.cFileName;
    }

    return result;
#else // Win16

#ifdef __BORLANDC__
    if (findnext(&gs_findDataStruct) == 0)
#else
        if (_dos_findnext(&gs_findDataStruct) == 0)
#endif
        {
            /* MATTHEW: [5] Check directory flag */
            char attrib;

#ifdef __BORLANDC__
            attrib = gs_findDataStruct.ff_attrib;
#else
            attrib = gs_findDataStruct.attrib;
#endif

            if (attrib & _A_SUBDIR) {
                if (!(gs_findFlags & wxDIR))
                    goto try_again;
            } else if (gs_findFlags && !(gs_findFlags & wxFILE))
                goto try_again;


            result +=
#ifdef __BORLANDC__
                      gs_findDataStruct.ff_name
#else
                      gs_findDataStruct.name
#endif
                      ;
        }

    return result;
#endif // Win32/16
}

#elif defined(__WXPM__)

wxString wxFindFirstFile(const wxChar *spec, int flags)
{
    wxString result;

    /*
    // TODO: figure something out here for OS/2
    gs_strFileSpec = spec;
    gs_findFlags = flags;

    // Find path only so we can concatenate found file onto path
    wxString path(wxPathOnly(gs_strFileSpec));
    if ( !path.IsEmpty() )
        result << path << wxT('\\');

    int flag = _A_NORMAL;
    if (flags & wxDIR)
        flag = _A_SUBDIR;

    if (_dos_findfirst(WXSTRINGCAST spec, flag, &gs_findDataStruct) == 0)
    {
        char attrib;
        attrib = gs_findDataStruct.attrib;

        if (attrib & _A_SUBDIR) {
            if (!(gs_findFlags & wxDIR))
                return wxFindNextFile();
        } else if (gs_findFlags && !(gs_findFlags & wxFILE))
            return wxFindNextFile();

        result += gs_findDataStruct.name;
    }
    */
    return result;
}

wxString wxFindNextFile()
{
    wxString result;
    // TODO:
    return result;
}

#endif // Unix/Windows/OS/2

// Get current working directory.
// If buf is NULL, allocates space using new, else
// copies into buf.
wxChar *wxGetWorkingDirectory(wxChar *buf, int sz)
{
  if (!buf)
    buf = new wxChar[sz+1];
#if wxUSE_UNICODE
  char *cbuf = new char[sz+1];
#ifdef _MSC_VER
  if (_getcwd(cbuf, sz) == NULL) {
#elif defined(__WXMAC__) && !defined(__DARWIN__)
    enum
    {
        SFSaveDisk = 0x214, CurDirStore = 0x398
    };
    FSSpec cwdSpec ;

    FSMakeFSSpec( - *(short *) SFSaveDisk , *(long *) CurDirStore , NULL , &cwdSpec ) ;
    wxString res = wxMacFSSpec2UnixFilename( &cwdSpec ) ;
    strcpy( buf , res ) ;
    if (0) {
#else
  if (getcwd(cbuf, sz) == NULL) {
#endif
    delete [] cbuf;
#else // wxUnicode
#ifdef _MSC_VER
  if (_getcwd(buf, sz) == NULL) {
#elif defined(__WXMAC__) && !defined(__DARWIN__)
    FSSpec cwdSpec ;
    FCBPBRec pb;
    OSErr error;
    Str255  fileName ;
    pb.ioNamePtr = (StringPtr) &fileName;
    pb.ioVRefNum = 0;
    pb.ioRefNum = LMGetCurApRefNum();
    pb.ioFCBIndx = 0;
    error = PBGetFCBInfoSync(&pb);
    if ( error == noErr )
    {
        cwdSpec.vRefNum = pb.ioFCBVRefNum;
        cwdSpec.parID = pb.ioFCBParID;
        cwdSpec.name[0] = 0 ;
        wxString res = wxMacFSSpec2MacFilename( &cwdSpec ) ;

        strcpy( buf , res ) ;
        buf[res.length()]=0 ;
    }
    else
        buf[0] = 0 ;
    /*
    this version will not always give back the application directory on mac
    enum
    {
        SFSaveDisk = 0x214, CurDirStore = 0x398
    };
    FSSpec cwdSpec ;

    FSMakeFSSpec( - *(short *) SFSaveDisk , *(long *) CurDirStore , NULL , &cwdSpec ) ;
    wxString res = wxMacFSSpec2UnixFilename( &cwdSpec ) ;
    strcpy( buf , res ) ;
    */
    if (0) {
#elif defined(__VISAGECPP__) || (defined (__OS2__) && defined (__WATCOMC__))
    APIRET rc;
    rc = ::DosQueryCurrentDir( 0 // current drive
                              ,buf
                              ,(PULONG)&sz
                             );
    if (rc != 0) {
#else
  if (getcwd(buf, sz) == NULL) {
#endif
#endif
    buf[0] = wxT('.');
    buf[1] = wxT('\0');
  }
#if wxUSE_UNICODE
  else {
    wxConvFile.MB2WC(buf, cbuf, sz);
    delete [] cbuf;
  }
#endif
  return buf;
}

wxString wxGetCwd()
{
    static const size_t maxPathLen = 1024;

    wxString str;
    wxGetWorkingDirectory(str.GetWriteBuf(maxPathLen), maxPathLen);
    str.UngetWriteBuf();

    return str;
}

bool wxSetWorkingDirectory(const wxString& d)
{
#if defined( __UNIX__ ) || defined( __WXMAC__ )
  return (chdir(wxFNSTRINGCAST d.fn_str()) == 0);
#elif defined(__WXPM__)
  return (::DosSetCurrentDir((PSZ)d.c_str()) == 0);
#elif defined(__WINDOWS__)

#ifdef __WIN32__
  return (bool)(SetCurrentDirectory(d) != 0);
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
  bool success = (chdir(WXSTRINGCAST d) == 0);

  return success;
#endif

#endif
}

// Get the OS directory if appropriate (such as the Windows directory).
// On non-Windows platform, probably just return the empty string.
wxString wxGetOSDirectory()
{
#if defined(__WINDOWS__) && !defined(__WXMICROWIN__)
    wxChar buf[256];
    GetWindowsDirectory(buf, 256);
    return wxString(buf);
#else
    return wxEmptyString;
#endif
}

bool wxEndsWithPathSeparator(const wxChar *pszFileName)
{
  size_t len = wxStrlen(pszFileName);
  if ( len == 0 )
    return FALSE;
  else
    return wxIsPathSeparator(pszFileName[len - 1]);
}

// find a file in a list of directories, returns false if not found
bool wxFindFileInPath(wxString *pStr, const wxChar *pszPath, const wxChar *pszFile)
{
    // we assume that it's not empty
    wxCHECK_MSG( !wxIsEmpty(pszFile), FALSE,
                 _T("empty file name in wxFindFileInPath"));

    // skip path separator in the beginning of the file name if present
    if ( wxIsPathSeparator(*pszFile) )
        pszFile++;

    // copy the path (strtok will modify it)
    wxChar *szPath = new wxChar[wxStrlen(pszPath) + 1];
    wxStrcpy(szPath, pszPath);

    wxString strFile;
    wxChar *pc, *save_ptr;
    for ( pc = wxStrtok(szPath, wxPATH_SEP, &save_ptr);
          pc != NULL;
          pc = wxStrtok((wxChar *) NULL, wxPATH_SEP, &save_ptr) )
    {
        // search for the file in this directory
        strFile = pc;
        if ( !wxEndsWithPathSeparator(pc) )
            strFile += wxFILE_SEP_PATH;
        strFile += pszFile;

        if ( FileExists(strFile) ) {
            *pStr = strFile;
            break;
        }
    }

    // suppress warning about unused variable save_ptr when wxStrtok() is a
    // macro which throws away its third argument
    save_ptr = pc;

    delete [] szPath;

    return pc != NULL;  // if true => we breaked from the loop
}

void WXDLLEXPORT wxSplitPath(const wxChar *pszFileName,
                             wxString *pstrPath,
                             wxString *pstrName,
                             wxString *pstrExt)
{
    // it can be empty, but it shouldn't be NULL
    wxCHECK_RET( pszFileName, wxT("NULL file name in wxSplitPath") );

    wxFileName::SplitPath(pszFileName, pstrPath, pstrName, pstrExt);
}

time_t WXDLLEXPORT wxFileModificationTime(const wxString& filename)
{
    wxStructStat buf;

    wxStat(filename.fn_str(), &buf);
    return buf.st_mtime;
}


//------------------------------------------------------------------------
// wild character routines
//------------------------------------------------------------------------

bool wxIsWild( const wxString& pattern )
{
  wxString tmp = pattern;
  wxChar *pat = WXSTRINGCAST(tmp);
    while (*pat) {
        switch (*pat++) {
        case wxT('?'): case wxT('*'): case wxT('['): case wxT('{'):
            return TRUE;
        case wxT('\\'):
            if (!*pat++)
                return FALSE;
        }
    }
    return FALSE;
};

bool wxMatchWild( const wxString& pat, const wxString& text, bool dot_special )

#if defined(HAVE_FNMATCH_H)
{
// this probably won't work well for multibyte chars in Unicode mode?
   if(dot_special)
      return fnmatch(pat.fn_str(), text.fn_str(), FNM_PERIOD) == 0;
   else
      return fnmatch(pat.fn_str(), text.fn_str(), 0) == 0;
}
#else

// #pragma error Broken implementation of wxMatchWild() -- needs fixing!

   /*
    * WARNING: this code is broken!
    */
{
  wxString tmp1 = pat;
  wxChar *pattern = WXSTRINGCAST(tmp1);
  wxString tmp2 = text;
  wxChar *str = WXSTRINGCAST(tmp2);
    wxChar c;
    wxChar *cp;
    bool done = FALSE, ret_code, ok;
    // Below is for vi fans
    const wxChar OB = wxT('{'), CB = wxT('}');

    // dot_special means '.' only matches '.'
    if (dot_special && *str == wxT('.') && *pattern != *str)
        return FALSE;

    while ((*pattern != wxT('\0')) && (!done)
    && (((*str==wxT('\0'))&&((*pattern==OB)||(*pattern==wxT('*'))))||(*str!=wxT('\0')))) {
        switch (*pattern) {
        case wxT('\\'):
            pattern++;
            if (*pattern != wxT('\0'))
                pattern++;
            break;
        case wxT('*'):
            pattern++;
            ret_code = FALSE;
            while ((*str!=wxT('\0'))
            && ((ret_code=wxMatchWild(pattern, str++, FALSE)) == 0))
                /*loop*/;
            if (ret_code) {
                while (*str != wxT('\0'))
                    str++;
                while (*pattern != wxT('\0'))
                    pattern++;
            }
            break;
        case wxT('['):
            pattern++;
          repeat:
            if ((*pattern == wxT('\0')) || (*pattern == wxT(']'))) {
                done = TRUE;
                break;
            }
            if (*pattern == wxT('\\')) {
                pattern++;
                if (*pattern == wxT('\0')) {
                    done = TRUE;
                    break;
                }
            }
            if (*(pattern + 1) == wxT('-')) {
                c = *pattern;
                pattern += 2;
                if (*pattern == wxT(']')) {
                    done = TRUE;
                    break;
                }
                if (*pattern == wxT('\\')) {
                    pattern++;
                    if (*pattern == wxT('\0')) {
                        done = TRUE;
                        break;
                    }
                }
                if ((*str < c) || (*str > *pattern)) {
                    pattern++;
                    goto repeat;
                }
            } else if (*pattern != *str) {
                pattern++;
                goto repeat;
            }
            pattern++;
            while ((*pattern != wxT(']')) && (*pattern != wxT('\0'))) {
                if ((*pattern == wxT('\\')) && (*(pattern + 1) != wxT('\0')))
                    pattern++;
                pattern++;
            }
            if (*pattern != wxT('\0')) {
                pattern++, str++;
            }
            break;
        case wxT('?'):
            pattern++;
            str++;
            break;
        case OB:
            pattern++;
            while ((*pattern != CB) && (*pattern != wxT('\0'))) {
                cp = str;
                ok = TRUE;
                while (ok && (*cp != wxT('\0')) && (*pattern != wxT('\0'))
                &&  (*pattern != wxT(',')) && (*pattern != CB)) {
                    if (*pattern == wxT('\\'))
                        pattern++;
                    ok = (*pattern++ == *cp++);
                }
                if (*pattern == wxT('\0')) {
                    ok = FALSE;
                    done = TRUE;
                    break;
                } else if (ok) {
                    str = cp;
                    while ((*pattern != CB) && (*pattern != wxT('\0'))) {
                        if (*++pattern == wxT('\\')) {
                            if (*++pattern == CB)
                                pattern++;
                        }
                    }
                } else {
                    while (*pattern!=CB && *pattern!=wxT(',') && *pattern!=wxT('\0')) {
                        if (*++pattern == wxT('\\')) {
                            if (*++pattern == CB || *pattern == wxT(','))
                                pattern++;
                        }
                    }
                }
                if (*pattern != wxT('\0'))
                    pattern++;
            }
            break;
        default:
            if (*str == *pattern) {
                str++, pattern++;
            } else {
                done = TRUE;
            }
        }
    }
    while (*pattern == wxT('*'))
        pattern++;
    return ((*str == wxT('\0')) && (*pattern == wxT('\0')));
};

#endif

#ifdef __VISUALC__
    #pragma warning(default:4706)   // assignment within conditional expression
#endif // VC++
