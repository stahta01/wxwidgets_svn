/////////////////////////////////////////////////////////////////////////////
// Name:        utils.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


//#ifdef __GNUG__
//#pragma implementation "utils.h"
//#endif

#include "wx/utils.h"
#include "wx/string.h"

#include <stdarg.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <netdb.h>

#ifdef __SVR4__
#include <sys/systeminfo.h>
#endif

//------------------------------------------------------------------------
// misc.
//------------------------------------------------------------------------

void wxBell(void)
{
  gdk_beep();
};

//------------------------------------------------------------------------
// user and home routines
//------------------------------------------------------------------------

char* wxGetHomeDir( char *dest )
{
  wxString tmp = wxGetUserHome( wxString() );
  if (tmp.IsNull())
    strcpy( wxBuffer, "/" );
  else
    strcpy( wxBuffer, tmp );
  if (dest) strcpy( dest, WXSTRINGCAST tmp );
  return wxBuffer;
};

char *wxGetUserHome( const wxString &user )
{
  struct passwd *who = NULL;

  if (user.IsNull() || (user== "")) 
  {
	register char *ptr;

	if ((ptr = getenv("HOME")) != NULL) 
	    return ptr;
	if ((ptr = getenv("USER")) != NULL
	|| (ptr = getenv("LOGNAME")) != NULL) {
	    who = getpwnam(ptr);
	}
	// We now make sure the the user exists!
	if (who == NULL)
	    who = getpwuid(getuid());
  } 
  else
    who = getpwnam (user);
    
  return who ? who->pw_dir : (char*)NULL;
};

//------------------------------------------------------------------------
// id routines
//------------------------------------------------------------------------

bool wxGetHostName(char *buf, int sz)
{
    *buf = '\0';
#if defined(__SVR4__) && !defined(__sgi)
    return (sysinfo(SI_HOSTNAME, buf, sz) != -1);
#else /* BSD Sockets */
    char name[255];
    struct hostent *h;
    // Get hostname
    if (gethostname(name, sizeof(name)/sizeof(char)-1) == -1)
	return FALSE;
    // Get official full name of host
    strncpy(buf, (h=gethostbyname(name))!=NULL ? h->h_name : name, sz-1);
    return TRUE;
#endif
}

bool wxGetUserId(char *buf, int sz)
{
    struct passwd *who;

    *buf = '\0';
    if ((who = getpwuid(getuid ())) != NULL) {
	strncpy (buf, who->pw_name, sz-1);
	return TRUE;
    }
    return FALSE;
}

bool wxGetUserName(char *buf, int sz)
{
    struct passwd *who;

    *buf = '\0';
    if ((who = getpwuid (getuid ())) != NULL) {
	strncpy (buf, who->pw_gecos, sz - 1);
	return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------
// error and debug output routines
//------------------------------------------------------------------------

void wxDebugMsg( const char *format, ... )
{
  va_list ap;
  va_start( ap, format );
  vfprintf( stderr, format, ap ); 
  fflush( stderr );
  va_end(ap);
};

void wxError( const wxString &msg, const wxString &title )
{
  fprintf( stderr, "Error " );
  if (!title.IsNull()) fprintf( stderr, "%s ", WXSTRINGCAST(title) );
  if (!msg.IsNull()) fprintf( stderr, ": %s", WXSTRINGCAST(msg) );
  fprintf( stderr, ".\n" );
};

void wxFatalError( const wxString &msg, const wxString &title )
{
  fprintf( stderr, "Error " );
  if (!title.IsNull()) fprintf( stderr, "%s ", WXSTRINGCAST(title) );
  if (!msg.IsNull()) fprintf( stderr, ": %s", WXSTRINGCAST(msg) );
  fprintf( stderr, ".\n" );
  exit(1);
};

//------------------------------------------------------------------------
// directory routines
//------------------------------------------------------------------------

bool wxDirExists( const wxString& dir )
{
  char buf[500];
  strcpy( buf, WXSTRINGCAST(dir) );
  struct stat sbuf;
  return ((stat(buf, &sbuf) != -1) && S_ISDIR(sbuf.st_mode) ? TRUE : FALSE);
};

//------------------------------------------------------------------------
// wild character routines
//------------------------------------------------------------------------

bool wxIsWild( const wxString& pattern )
{
  wxString tmp = pattern;
  char *pat = WXSTRINGCAST(tmp);
    while (*pat) {
	switch (*pat++) {
	case '?': case '*': case '[': case '{':
	    return TRUE;
	case '\\':
	    if (!*pat++)
		return FALSE;
	}
    }
    return FALSE;
};


bool wxMatchWild( const wxString& pat, const wxString& text, bool dot_special )
{
  wxString tmp1 = pat;
  char *pattern = WXSTRINGCAST(tmp1);
  wxString tmp2 = text;
  char *str = WXSTRINGCAST(tmp2);
    char c;
    char *cp;
    bool done = FALSE, ret_code, ok;
    // Below is for vi fans
    const char OB = '{', CB = '}';

    // dot_special means '.' only matches '.'
    if (dot_special && *str == '.' && *pattern != *str)
	return FALSE;

    while ((*pattern != '\0') && (!done)
    && (((*str=='\0')&&((*pattern==OB)||(*pattern=='*')))||(*str!='\0'))) {
	switch (*pattern) {
	case '\\':
	    pattern++;
	    if (*pattern != '\0')
		pattern++;
	    break;
	case '*':
	    pattern++;
	    ret_code = FALSE;
	    while ((*str!='\0')
	    && (!(ret_code=wxMatchWild(pattern, str++, FALSE))))
		/*loop*/;
	    if (ret_code) {
		while (*str != '\0')
		    str++;
		while (*pattern != '\0')
		    pattern++;
	    }
	    break;
	case '[':
	    pattern++;
	  repeat:
	    if ((*pattern == '\0') || (*pattern == ']')) {
		done = TRUE;
		break;
	    }
	    if (*pattern == '\\') {
		pattern++;
		if (*pattern == '\0') {
		    done = TRUE;
		    break;
		}
	    }
	    if (*(pattern + 1) == '-') {
		c = *pattern;
		pattern += 2;
		if (*pattern == ']') {
		    done = TRUE;
		    break;
		}
		if (*pattern == '\\') {
		    pattern++;
		    if (*pattern == '\0') {
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
	    while ((*pattern != ']') && (*pattern != '\0')) {
		if ((*pattern == '\\') && (*(pattern + 1) != '\0'))
		    pattern++;
		pattern++;
	    }
	    if (*pattern != '\0') {
		pattern++, str++;
	    }
	    break;
	case '?':
	    pattern++;
	    str++;
	    break;
	case OB:
	    pattern++;
	    while ((*pattern != CB) && (*pattern != '\0')) {
		cp = str;
		ok = TRUE;
		while (ok && (*cp != '\0') && (*pattern != '\0')
		&&  (*pattern != ',') && (*pattern != CB)) {
		    if (*pattern == '\\')
			pattern++;
		    ok = (*pattern++ == *cp++);
		}
		if (*pattern == '\0') {
		    ok = FALSE;
		    done = TRUE;
		    break;
		} else if (ok) {
		    str = cp;
		    while ((*pattern != CB) && (*pattern != '\0')) {
			if (*++pattern == '\\') {
			    if (*++pattern == CB)
				pattern++;
			}
		    }
		} else {
		    while (*pattern!=CB && *pattern!=',' && *pattern!='\0') {
			if (*++pattern == '\\') {
                            if (*++pattern == CB || *pattern == ',')
				pattern++;
			}
		    }
		}
		if (*pattern != '\0')
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
    while (*pattern == '*')
	pattern++;
    return ((*str == '\0') && (*pattern == '\0'));
};

//------------------------------------------------------------------------
// subprocess routines
//------------------------------------------------------------------------

long wxExecute( char **argv, bool Async )
{
    if (*argv == NULL)
	return FALSE;

    /* fork the process */
#if defined(sun) || defined(__ultrix) || defined(__bsdi__)
    pid_t pid = vfork();
#else
    pid_t pid = fork();
#endif
    if (pid == -1) {
	perror ("fork failed");
	return FALSE;
    } else if (pid == 0) {
	/* child */
#ifdef _AIX
	execvp ((const char *)*argv, (const char **)argv);
#else
	execvp (*argv, argv);
#endif
	if (errno == ENOENT)
	    wxError("command not found", *argv);
	else
	    perror (*argv);
	wxError("could not execute", *argv);
	_exit (-1);
    }

    // Code below is NOT really acceptable!
    // One should NEVER use wait under X
    // Ideas? A Sleep idle callback?
    // WARNING: WARNING: WARNING: WARNING:
    // The CODE BELOW IS BAD BAD BAD BAD!
    if (Async) {
	int status;
/*
	wxSleep(2);		// Give a little time
*/
#if !defined(DG) && \
    !defined(__AIX__) && \
    !defined(__xlC__) && \
    !defined(__SVR4__) && \
    !defined(__SUN__) && \
    !defined(__ALPHA__) && \
    !defined(__SGI__) && \
    !defined(__HPUX__) && \
    !defined(__SUNPRO_CC) && \
    !defined(__FreeBSD__)
        while (wait((union wait*)&status) != pid)
#else
	while (wait(&status) != pid)
#endif
      {};
/*
	    wxSleep(3);	// 3 sec?
*/
    };
    return TRUE;
};

long wxExecute( const wxString& command, bool Async )
{
    if (command.IsNull() || command == "") return FALSE;

    int argc = 0;
    char *argv[127];
    char tmp[1024];
    const char *IFS = " \t\n";

    strncpy (tmp, command, sizeof(tmp) / sizeof(char) - 1);
    tmp[sizeof (tmp) / sizeof (char) - 1] = '\0';
    argv[argc++] = strtok (tmp, IFS);
    while ((argv[argc++] = strtok(NULL, IFS)) != NULL)
	/* loop */ ;
    return wxExecute(argv, Async);
};

