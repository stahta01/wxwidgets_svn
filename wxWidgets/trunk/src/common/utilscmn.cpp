/////////////////////////////////////////////////////////////////////////////
// Name:        utilscmn.cpp
// Purpose:     Miscellaneous utility functions and classes
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "utils.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#include "wx/utils.h"
#include "wx/window.h"
#include "wx/menu.h"
#include "wx/frame.h"
#endif

#if USE_IOSTREAMH
#include <iostream.h>
#else
#include <iostream>
#endif

#include <fstream.h>
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
#include <sys/types.h>
#include <sys/stat.h>

// Pattern matching code.
// Yes, this path is deliberate (for Borland compilation)
#ifdef wx_mac /* MATTHEW: [5] Mac doesn't like paths with "/" */
#include "glob.inc"
#else
#include "../common/glob.inc"
#endif

#ifdef __WXMSW__
#include "windows.h"
#endif

#define _MAXPATHLEN 500

extern char *wxBuffer;

#ifdef __VMS__
// we have no strI functions under VMS, therefore I have implemented
// an inefficient but portable version: convert copies of strings to lowercase
// and then use the normal comparison
static void myLowerString(char *s)
{
  while(*s){
    if(isalpha(*s)) *s = (char)tolower(*s);
    s++;
  }
}

int strcasecmp(const char *str_1, const char *str_2)
{
  char *temp1 = new char[strlen(str_1)+1];
  char *temp2 = new char[strlen(str_2)+1];
  strcpy(temp1,str_1);
  strcpy(temp2,str_2);
  myLowerString(temp1);
  myLowerString(temp2);

  int result = strcmp(temp1,temp2);
  delete[] temp1;
  delete[] temp2;

  return(result);
}

int strncasecmp(const char *str_1, const char *str_2, size_t maxchar)
{
  char *temp1 = new char[strlen(str_1)+1];
  char *temp2 = new char[strlen(str_2)+1];
  strcpy(temp1,str_1);
  strcpy(temp2,str_2);
  myLowerString(temp1);
  myLowerString(temp2);

  int result = strncmp(temp1,temp2,maxchar);
  delete[] temp1;
  delete[] temp2;

  return(result);
}
#endif

#ifdef __WINDOWS__

#ifndef __GNUWIN32__
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

#ifdef _MSC_VER
#pragma warning (disable : 4245)
#endif

#ifdef _MSC_VER
#pragma warning (default : 4245)
#endif

#else
// This declaration is missing in SunOS!
// (Yes, I know it is NOT ANSI-C but its in BSD libc)
#if defined(__xlC) || defined(__AIX__) || defined(__GNUG__)
extern "C"
{
  int strcasecmp (const char *, const char *);
  int strncasecmp (const char *, const char *, size_t);
}
#endif
#endif				/* __WXMSW__ */


char *
copystring (const char *s)
{
  if (s == NULL) s = "";
  size_t len = strlen (s) + 1;

  char *news = new char[len];
  memcpy (news, s, len);	// Should be the fastest

  return news;
}

// Id generation
static long wxCurrentId = 100;

long 
wxNewId (void)
{
  return wxCurrentId++;
}

long
wxGetCurrentId(void) { return wxCurrentId; }

void 
wxRegisterId (long id)
{
  if (id >= wxCurrentId)
    wxCurrentId = id + 1;
}

void 
StringToFloat (char *s, float *number)
{
  if (s && *s && number)
    *number = (float) strtod (s, NULL);
}

void 
StringToDouble (char *s, double *number)
{
  if (s && *s && number)
    *number = strtod (s, NULL);
}

char *
FloatToString (float number, const char *fmt)
{
  static char buf[256];

//  sprintf (buf, "%.2f", number);
  sprintf (buf, fmt, number);
  return buf;
}

char *
DoubleToString (double number, const char *fmt)
{
  static char buf[256];

  sprintf (buf, fmt, number);
  return buf;
}

void 
StringToInt (char *s, int *number)
{
  if (s && *s && number)
    *number = (int) strtol (s, NULL, 10);
}

void 
StringToLong (char *s, long *number)
{
  if (s && *s && number)
    *number = strtol (s, NULL, 10);
}

char *
IntToString (int number)
{
  static char buf[20];

  sprintf (buf, "%d", number);
  return buf;
}

char *
LongToString (long number)
{
  static char buf[20];

  sprintf (buf, "%ld", number);
  return buf;
}

// Array used in DecToHex conversion routine.
static char hexArray[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
  'C', 'D', 'E', 'F' };

// Convert 2-digit hex number to decimal
int wxHexToDec(char *buf)
{
  int firstDigit, secondDigit;
  
  if (buf[0] >= 'A')
    firstDigit = buf[0] - 'A' + 10;
  else
    firstDigit = buf[0] - '0';

  if (buf[1] >= 'A')
    secondDigit = buf[1] - 'A' + 10;
  else
    secondDigit = buf[1] - '0';
    
  return firstDigit * 16 + secondDigit;
}

// Convert decimal integer to 2-character hex string
void wxDecToHex(int dec, char *buf)
{
  int firstDigit = (int)(dec/16.0);
  int secondDigit = (int)(dec - (firstDigit*16.0));
  buf[0] = hexArray[firstDigit];
  buf[1] = hexArray[secondDigit];
  buf[2] = 0;
}

// Match a string INDEPENDENT OF CASE
bool 
StringMatch (char *str1, char *str2, bool subString, bool exact)
{
  if (str1 == NULL || str2 == NULL)
    return FALSE;
  if (str1 == str2)
    return TRUE;

  if (subString)
    {
      int len1 = strlen (str1);
      int len2 = strlen (str2);
      int i;

      // Search for str1 in str2
      // Slow .... but acceptable for short strings
      for (i = 0; i <= len2 - len1; i++)
	{
	  if (strncasecmp (str1, str2 + i, len1) == 0)
	    return TRUE;
	}
    }
  else if (exact)
    {
      if (strcasecmp (str1, str2) == 0)
	return TRUE;
    }
  else
    {
      int len1 = strlen (str1);
      int len2 = strlen (str2);

      if (strncasecmp (str1, str2, wxMin (len1, len2)) == 0)
	return TRUE;
    }

  return FALSE;
}

// Return the current date/time
// [volatile]
wxString wxNow( void )
{
  time_t now = time(NULL);
  char *date = ctime(&now); 
  date[24] = '\0';
  return wxString(date);
}

/* Get Full RFC822 style email address */
bool
wxGetEmailAddress (char *address, int maxSize)
{
  char host[65];
  char user[65];

  if (wxGetHostName(host, 64) == FALSE)
    return FALSE;
  if (wxGetUserId(user, 64) == FALSE)
    return FALSE;

  char tmp[130];
  strcpy(tmp, user);
  strcat(tmp, "@");
  strcat(tmp, host);

  strncpy(address, tmp, maxSize - 1);
  address[maxSize-1] = '\0';
  return TRUE;
}

/*
 * Strip out any menu codes
 */

char *wxStripMenuCodes (char *in, char *out)
{
  if (!in)
    return NULL;
    
  if (!out)
    out = copystring(in);

  char *tmpOut = out;
  
  while (*in)
    {
      if (*in == '&')
	{
	  // Check && -> &, &x -> x
	  if (*++in == '&')
	    *out++ = *in++;
	}
      else if (*in == '\t')
	{
          // Remove all stuff after \t in X mode, and let the stuff as is
          // in Windows mode.
          // Accelerators are handled in wx_item.cc for Motif, and are not
          // YET supported in XView
	  break;
	}
      else
	*out++ = *in++;
    }				// while

  *out = '\0';

  return tmpOut;
}


/*
 * Window search functions
 *
 */

/*
 * If parent is non-NULL, look through children for a label or title
 * matching the specified string. If NULL, look through all top-level windows.
 *
 */

static wxWindow *wxFindWindowByLabel1 (const wxString& title, wxWindow * parent);

wxWindow *
wxFindWindowByLabel (const wxString& title, wxWindow * parent)
{
  if (parent)
    {
      return wxFindWindowByLabel1 (title, parent);
    }
  else
    {
      for (wxNode * node = wxTopLevelWindows.First (); node; node = node->Next ())
	{
	  wxWindow *win = (wxWindow *) node->Data ();
	  wxWindow *retwin = wxFindWindowByLabel1 (title, win);
	  if (retwin)
	    return retwin;
	}			// for()

    }
  return NULL;
}

// Recursive
static wxWindow *
wxFindWindowByLabel1 (const wxString& title, wxWindow * parent)
{
  if (parent)
    {
      if (parent->GetLabel() == title)
		return parent;
    }

  if (parent)
    {
      for (wxNode * node = parent->GetChildren()->First (); node; node = node->Next ())
	{
	  wxWindow *win = (wxWindow *) node->Data ();
	  wxWindow *retwin = wxFindWindowByLabel1 (title, win);
	  if (retwin)
	    return retwin;
	}			// for()

    }

  return NULL;			// Not found

}

/*
 * If parent is non-NULL, look through children for a name
 * matching the specified string. If NULL, look through all top-level windows.
 *
 */

static wxWindow *wxFindWindowByName1 (const wxString& title, wxWindow * parent);

wxWindow *
wxFindWindowByName (const wxString& title, wxWindow * parent)
{
  if (parent)
    {
      return wxFindWindowByName1 (title, parent);
    }
  else
    {
      for (wxNode * node = wxTopLevelWindows.First (); node; node = node->Next ())
	{
	  wxWindow *win = (wxWindow *) node->Data ();
	  wxWindow *retwin = wxFindWindowByName1 (title, win);
	  if (retwin)
	    return retwin;
	}			// for()

    }
  // Failed? Try by label instead.
  return wxFindWindowByLabel(title, parent);
}

// Recursive
static wxWindow *
wxFindWindowByName1 (const wxString& title, wxWindow * parent)
{
  if (parent)
    {
    	if ( parent->GetName() == title )
			return parent;
    }

  if (parent)
    {
      for (wxNode * node = parent->GetChildren()->First (); node; node = node->Next ())
	{
	  wxWindow *win = (wxWindow *) node->Data ();
	  wxWindow *retwin = wxFindWindowByName1 (title, win);
	  if (retwin)
	    return retwin;
	}			// for()

    }

  return NULL;			// Not found

}

// Returns menu item id or -1 if none.
int 
wxFindMenuItemId (wxFrame * frame, const wxString& menuString, const wxString& itemString)
{
  wxMenuBar *menuBar = frame->GetMenuBar ();
  if (!menuBar)
    return -1;
  return menuBar->FindMenuItem (menuString, itemString);
}

/*
 * wxDebugStreamBuf
 */
#if !defined(_WINDLL)
 
wxDebugStreamBuf::wxDebugStreamBuf(void)
{
  if (allocate()) setp(base(),ebuf());
}

int wxDebugStreamBuf::overflow(int WXUNUSED(i))
{
  int len = pptr() - pbase();
  char *txt = new char[len+1];
  strncpy(txt, pbase(), len);
  txt[len] = '\0';
#ifdef __WXMSW__
  OutputDebugString((LPCSTR)txt);
#else
  fprintf(stderr, txt);
#endif
  setp(pbase(), epptr());
  delete[] txt;
  return EOF;
}

int wxDebugStreamBuf::sync(void)
{
  int len = pptr() - pbase();
  char *txt = new char[len+1];
  strncpy(txt, pbase(), len);
  txt[len] = '\0';
#ifdef __WXMSW__
  OutputDebugString((LPCSTR)txt);
#else
  fprintf(stderr, txt);
#endif
  setp(pbase(), epptr());
  delete[] txt;
  return 0;
}

#endif

/*
On Fri, 21 Jul 1995, Paul Craven wrote:

> Is there a way to find the path of running program's executable? I can get
> my home directory, and the current directory, but I don't know how to get the
> executable directory.
> 

The code below (warty as it is), does what you want on most Unix,
DOS, and Mac platforms (it's from the ALS Prolog main).

|| Ken Bowen      Applied Logic Systems, Inc.         PO Box 180,     
||====            Voice:  +1 (617)965-9191            Newton Centre,
||                FAX:    +1 (617)965-1636            MA  02159  USA
                  Email:  ken@als.com        WWW: http://www.als.com
------------------------------------------------------------------------
*/

// This code is commented out but it may be integrated with wxWin at
// a later date, after testing. Thanks Ken!
#if 0

/*--------------------------------------------------------------------*
 | whereami is given a filename f in the form:  whereami(argv[0])
 | It returns the directory in which the executable file (containing 
 | this code [main.c] ) may be found.  A dot will be returned to indicate 
 | the current directory.
 *--------------------------------------------------------------------*/

static void
whereami(name)
    char *name;
{
    register char *cutoff = NULL;	/* stifle -Wall */
    register char *s;
    register char *t;
    int   cc;
    char  ebuf[4096];

    /*
     * See if the file is accessible either through the current directory
     * or through an absolute path.
     */

    if (access(name, R_OK) == 0) {

	/*-------------------------------------------------------------*
	 * The file was accessible without any other work.  But the current
	 * working directory might change on us, so if it was accessible
	 * through the cwd, then we should get it for later accesses.
	 *-------------------------------------------------------------*/

	t = imagedir;
	if (!absolute_pathname(name)) {
#if defined(DOS) || defined(__WIN32__)
	    int   drive;
	    char *newrbuf;

	    newrbuf = imagedir;
#ifndef __DJGPP__
	    if (*(name + 1) == ':') {
		if (*name >= 'a' && *name <= 'z')
		    drive = (int) (*name - 'a' + 1);
		else
		    drive = (int) (*name - 'A' + 1);
		*newrbuf++ = *name;
		*newrbuf++ = *(name + 1);
		*newrbuf++ = DIR_SEPARATOR;
	    }
	    else {
		drive = 0;
		*newrbuf++ = DIR_SEPARATOR;
	    }
	    if (getcwd(newrbuf, drive) == 0) {	/* } */
#else
	    if (getcwd(newrbuf, 1024) == 0) {	/* } */
#endif
#else  /* DOS */
#ifdef HAVE_GETWD
	    if (getwd(imagedir) == 0) {		/* } */
#else  /* !HAVE_GETWD */
	    if (getcwd(imagedir, 1024) == 0) {
#endif /* !HAVE_GETWD */
#endif /* DOS */
		fatal_error(FE_GETCWD, 0);
	    }
	    for (; *t; t++)	/* Set t to end of buffer */
		;
	    if (*(t - 1) == DIR_SEPARATOR)	/* leave slash if already
						 * last char
						 */
		cutoff = t - 1;
	    else {
		cutoff = t;	/* otherwise put one in */
		*t++ = DIR_SEPARATOR;
	    }
	}
#if (!defined(__MAC__) && !defined(__DJGPP__) && !defined(__GO32__) && !defined(__WIN32__))
	else
		(*t++ = DIR_SEPARATOR);
#endif

	/*-------------------------------------------------------------*
	 * Copy the rest of the string and set the cutoff if it was not
	 * already set.  If the first character of name is a slash, cutoff
	 * is not presently set but will be on the first iteration of the
	 * loop below.
	 *-------------------------------------------------------------*/

	for ((*name == DIR_SEPARATOR ? (s = name+1) : (s = name));;) {
	    if (*s == DIR_SEPARATOR)
			cutoff = t;
	    if (!(*t++ = *s++))
			break;
	}

    }
    else {

	/*-------------------------------------------------------------*
	 * Get the path list from the environment.  If the path list is
	 * inaccessible for any reason, leave with fatal error.
	 *-------------------------------------------------------------*/

#ifdef __MAC__
	if ((s = getenv("Commands")) == (char *) 0)
#else
	if ((s = getenv("PATH")) == (char *) 0)
#endif
	    fatal_error(FE_PATH, 0);

	/*
	 * Copy path list into ebuf and set the source pointer to the
	 * beginning of this buffer.
	 */

	strcpy(ebuf, s);
	s = ebuf;

	for (;;) {
	    t = imagedir;
	    while (*s && *s != PATH_SEPARATOR)
		*t++ = *s++;
	    if (t > imagedir && *(t - 1) == DIR_SEPARATOR) 
		;		/* do nothing -- slash already is in place */
	    else
		*t++ = DIR_SEPARATOR;	/* put in the slash */
	    cutoff = t - 1;	/* set cutoff */
	    strcpy(t, name);
	    if (access(imagedir, R_OK) == 0)
		break;

	    if (*s)
		s++;		/* advance source pointer */
	    else
		fatal_error(FE_INFND, 0);
	}

    }

    /*-------------------------------------------------------------*
     | At this point the full pathname should exist in imagedir and
     | cutoff should be set to the final slash.  We must now determine
     | whether the file name is a symbolic link or not and chase it down
     | if it is.  Note that we reuse ebuf for getting the link.
     *-------------------------------------------------------------*/

#ifdef HAVE_SYMLINK
    while ((cc = readlink(imagedir, ebuf, 512)) != -1) {
	ebuf[cc] = 0;
	s = ebuf;
	if (*s == DIR_SEPARATOR) {
	    t = imagedir;
	}
	else {
	    t = cutoff + 1;
	}
	for (;;) {
	    if (*s == DIR_SEPARATOR)
		cutoff = t;	/* mark the last slash seen */
	    if (!(*t++ = *s++))	/* copy the character */
		break;
	}
    }

#endif /* HAVE_SYMLINK */

    strcpy(imagename, cutoff + 1);	/* keep the image name */
    *(cutoff + 1) = 0;		/* chop off the filename part */
}

#endif

