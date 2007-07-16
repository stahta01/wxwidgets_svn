/*
 * Name:        wx/wxcrtbase.h
 * Purpose:     Type-safe ANSI and Unicode builds compatible wrappers for
 *              CRT functions
 * Author:      Joel Farley, Ove K�ven
 * Modified by: Vadim Zeitlin, Robert Roebling, Ron Lee
 * Created:     1998/06/12
 * RCS-ID:      $Id: wxcrtbase.h 47238 2007-07-08 19:06:03Z VS $
 * Copyright:   (c) 1998-2006 wxWidgets dev team
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_WXCRTBASE_H_
#define _WX_WXCRTBASE_H_

/* -------------------------------------------------------------------------
                        headers and missing declarations
   ------------------------------------------------------------------------- */

#include "wx/chartype.h"

/*
    Standard headers we need here.

    NB: don't include any wxWidgets headers here because almost all of them
        include this one!
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <time.h>

#ifdef __WINDOWS__
#include <io.h>
#endif

#if defined(HAVE_STRTOK_R) && defined(__DARWIN__) && defined(_MSL_USING_MW_C_HEADERS) && _MSL_USING_MW_C_HEADERS
    char *strtok_r(char *, const char *, char **);
#endif

/*
   a few compilers don't have the (non standard but common) isascii function,
   define it ourselves for them
 */
#ifndef isascii
    #if defined(__MWERKS__)
        #define wxNEED_ISASCII
    #elif defined(_WIN32_WCE)
        #if _WIN32_WCE <= 211
            #define wxNEED_ISASCII
        #endif
    #endif
#endif /* isascii */

#ifdef wxNEED_ISASCII
    inline int isascii(int c) { return (unsigned)c < 0x80; }
#endif

#ifdef _WIN32_WCE
    #if _WIN32_WCE <= 211
        #define isspace(c) ((c) == _T(' ') || (c) == _T('\t'))
    #endif
#endif /* _WIN32_WCE */

/* string.h functions */
#ifndef strdup
    #if defined(__MWERKS__) && !defined(__MACH__) && (__MSL__ < 0x00008000)
        #define wxNEED_STRDUP
    #elif defined(__WXWINCE__)
        #if _WIN32_WCE <= 211
            #define wxNEED_STRDUP
        #endif
    #endif
#endif /* strdup */

#ifdef wxNEED_STRDUP
    WXDLLIMPEXP_BASE char *strdup(const char* s);
#endif

/* missing functions in some WinCE versions */
#ifdef _WIN32_WCE
#if (_WIN32_WCE < 300)
WXDLLIMPEXP_BASE void *calloc( size_t num, size_t size );
#endif
#endif /* _WIN32_WCE */


#if defined(__MWERKS__)
    /* Metrowerks only has wide char support for OS X >= 10.3 */
    #if !defined(__DARWIN__) || \
         (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)
        #define wxHAVE_MWERKS_UNICODE
    #endif

    #ifdef wxHAVE_MWERKS_UNICODE
        #define HAVE_WPRINTF   1
        #define HAVE_WCSRTOMBS 1
        #define HAVE_VSWPRINTF 1
    #endif
#endif /* __MWERKS__ */


/* -------------------------------------------------------------------------
                            UTF-8 locale handling
   ------------------------------------------------------------------------- */

#ifdef __cplusplus
    #if wxUSE_UNICODE_UTF8
        /* flag indicating whether the current locale uses UTF-8 or not; must be
           updated every time the locale is changed! */
        #if wxUSE_UTF8_LOCALE_ONLY
        #define wxLocaleIsUtf8 true
        #else
        extern WXDLLIMPEXP_BASE bool wxLocaleIsUtf8;
        #endif
        /* function used to update the flag: */
        extern WXDLLIMPEXP_BASE void wxUpdateLocaleIsUtf8();
    #else /* !wxUSE_UNICODE_UTF8 */
        inline void wxUpdateLocaleIsUtf8() {}
    #endif /* wxUSE_UNICODE_UTF8/!wxUSE_UNICODE_UTF8 */
#endif /* __cplusplus */


/* -------------------------------------------------------------------------
                                 string.h
   ------------------------------------------------------------------------- */

#define wxCRT_StrcatA    strcat
#define wxCRT_StrchrA    strchr
#define wxCRT_StrcmpA    strcmp
#define wxCRT_StrcollA   strcoll
#define wxCRT_StrcpyA    strcpy
#define wxCRT_StrcspnA   strcspn
#define wxCRT_StrlenA    strlen
#define wxCRT_StrncatA   strncat
#define wxCRT_StrncmpA   strncmp
#define wxCRT_StrncpyA   strncpy
#define wxCRT_StrpbrkA   strpbrk
#define wxCRT_StrrchrA   strrchr
#define wxCRT_StrspnA    strspn
#define wxCRT_StrstrA    strstr
#define wxCRT_StrxfrmA   strxfrm

#define wxCRT_StrcatW    wcscat
#define wxCRT_StrchrW    wcschr
#define wxCRT_StrcmpW    wcscmp
#define wxCRT_StrcollW   wcscoll
#define wxCRT_StrcpyW    wcscpy
#define wxCRT_StrcspnW   wcscspn
#define wxCRT_StrncatW   wcsncat
#define wxCRT_StrncmpW   wcsncmp
#define wxCRT_StrncpyW   wcsncpy
#define wxCRT_StrpbrkW   wcspbrk
#define wxCRT_StrrchrW   wcsrchr
#define wxCRT_StrspnW    wcsspn
#define wxCRT_StrstrW    wcsstr
#define wxCRT_StrxfrmW   wcsxfrm

/* Almost all compiler have strdup(), but not quite all: CodeWarrior under
   Mac and VC++ for Windows CE don't provide it; additionally, gcc under
   Mac and OpenVMS do not have wcsdup: */
#if defined(__VISUALC__) && __VISUALC__ >= 1400
    #define wxCRT_StrdupA _strdup
#elif !(defined(__MWERKS__) && defined(__WXMAC__)) && !defined(__WXWINCE__)
    #define wxCRT_StrdupA strdup
#endif
#if defined(__WINDOWS__)
    #define wxCRT_StrdupW _wcsdup
#elif defined(HAVE_WCSDUP)
    #define wxCRT_StrdupW wcsdup
#endif

#ifdef wxHAVE_TCHAR_SUPPORT
    /* we surely have wchar_t if we have TCHAR have wcslen() */
    #ifndef HAVE_WCSLEN
        #define HAVE_WCSLEN
    #endif
#endif /* wxHAVE_TCHAR_SUPPORT */

#ifdef HAVE_WCSLEN
    #define wxCRT_StrlenW wcslen
#endif

#define wxCRT_StrtodA    strtod
#define wxCRT_StrtolA    strtol
#define wxCRT_StrtoulA   strtoul
#define wxCRT_StrtodW    wcstod
#define wxCRT_StrtolW    wcstol
#define wxCRT_StrtoulW   wcstoul

#ifdef __VISUALC__
    #if __VISUALC__ >= 1300 && !defined(__WXWINCE__)
        #define wxCRT_StrtollA   _strtoi64
        #define wxCRT_StrtoullA  _strtoui64
        #define wxCRT_StrtollW   _wcstoi64
        #define wxCRT_StrtoullW  _wcstoui64
    #endif /* VC++ 7+ */
#else
    #ifdef HAVE_STRTOULL
        #define wxCRT_StrtollA   strtoll
        #define wxCRT_StrtoullA  strtoull
    #endif /* HAVE_STRTOULL */
    #ifdef HAVE_WCSTOULL
        /* assume that we have wcstoull(), which is also C99, too */
        #define wxCRT_StrtollW   wcstoll
        #define wxCRT_StrtoullW  wcstoull
    #endif /* HAVE_WCSTOULL */
#endif


/* define wxCRT_StricmpA/W and wxCRT_StrnicmpA/W for various compilers */

/* note that we definitely are going to need our own version for widechar
 * versions */
#if !defined(wxCRT_StricmpA)
    #if defined(__BORLANDC__) || defined(__WATCOMC__) || \
            defined(__SALFORDC__) || defined(__VISAGECPP__) || \
            defined(__EMX__) || defined(__DJGPP__)
        #define wxCRT_StricmpA stricmp
        #define wxCRT_StrnicmpA strnicmp
    #elif defined(__WXPALMOS__)
        /* FIXME: There is no equivalent to strnicmp in the Palm OS API.  This
         * quick hack should do until one can be written.
         */
        #define wxCRT_StricmpA StrCaselessCompare
        #define wxCRT_StrnicmpA strnicmp
    #elif defined(__SYMANTEC__) || defined(__VISUALC__) || \
            (defined(__MWERKS__) && defined(__INTEL__))
        #define wxCRT_StricmpA _stricmp
        #define wxCRT_StrnicmpA _strnicmp
    #elif defined(__UNIX__) || defined(__GNUWIN32__)
        #define wxCRT_StricmpA strcasecmp
        #define wxCRT_StrnicmpA strncasecmp
    /* #else -- use wxWidgets implementation */
    #endif
#endif /* !defined(wxCRT_StricmpA) */
/* FIXME-UTF8: use wcs(n)casecmp if available for *W versions */

#ifdef HAVE_STRTOK_R
    #define  wxCRT_StrtokA(str, sep, last)    strtok_r(str, sep, last)
#endif
/* FIXME-UTF8: detect and use wcstok() if available for wxCRT_StrtokW */

/* these are extern "C" because they are used by regex lib: */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef wxCRT_StrlenW
WXDLLIMPEXP_BASE size_t wxCRT_StrlenW(const wchar_t *s);
#endif

#ifndef wxCRT_StrncmpW
WXDLLIMPEXP_BASE int wxCRT_StrncmpW(const wchar_t *s1, const wchar_t *s2, size_t n);
#endif

#ifdef __cplusplus
}
#endif

/* FIXME-UTF8: remove this once we are Unicode only */
#if wxUSE_UNICODE
    #define wxCRT_StrlenNative  wxCRT_StrlenW
    #define wxCRT_StrncmpNative wxCRT_StrncmpW
    #define wxCRT_ToupperNative wxCRT_ToupperW
    #define wxCRT_TolowerNative wxCRT_TolowerW
#else
    #define wxCRT_StrlenNative  wxCRT_StrlenA
    #define wxCRT_StrncmpNative wxCRT_StrncmpA
    #define wxCRT_ToupperNative toupper
    #define wxCRT_TolowerNative tolower
#endif

#ifndef wxCRT_StrcatW
WXDLLIMPEXP_BASE wchar_t *wxCRT_StrcatW(wchar_t *dest, const wchar_t *src);
#endif

#ifndef wxCRT_StrchrW
WXDLLIMPEXP_BASE const wchar_t *wxCRT_StrchrW(const wchar_t *s, wchar_t c);
#endif

#ifndef wxCRT_StrcmpW
WXDLLIMPEXP_BASE int wxCRT_StrcmpW(const wchar_t *s1, const wchar_t *s2);
#endif

#ifndef wxCRT_StrcollW
WXDLLIMPEXP_BASE int wxCRT_StrcollW(const wchar_t *s1, const wchar_t *s2);
#endif

#ifndef wxCRT_StrcpyW
WXDLLIMPEXP_BASE wchar_t *wxCRT_StrcpyW(wchar_t *dest, const wchar_t *src);
#endif

#ifndef wxCRT_StrcspnW
WXDLLIMPEXP_BASE size_t wxCRT_StrcspnW(const wchar_t *s, const wchar_t *reject);
#endif

#ifndef wxCRT_StrncatW
WXDLLIMPEXP_BASE wchar_t *wxCRT_StrncatW(wchar_t *dest, const wchar_t *src, size_t n);
#endif

#ifndef wxCRT_StrncpyW
WXDLLIMPEXP_BASE wchar_t *wxCRT_StrncpyW(wchar_t *dest, const wchar_t *src, size_t n);
#endif

#ifndef wxCRT_StrpbrkW
WXDLLIMPEXP_BASE const wchar_t *wxCRT_StrpbrkW(const wchar_t *s, const wchar_t *accept);
#endif

#ifndef wxCRT_StrrchrW
WXDLLIMPEXP_BASE const wchar_t *wxCRT_StrrchrW(const wchar_t *s, wchar_t c);
#endif

#ifndef wxCRT_StrspnW
WXDLLIMPEXP_BASE size_t wxCRT_StrspnW(const wchar_t *s, const wchar_t *accept);
#endif

#ifndef wxCRT_StrstrW
WXDLLIMPEXP_BASE const wchar_t *wxCRT_StrstrW(const wchar_t *haystack, const wchar_t *needle);
#endif

#ifndef wxCRT_StrtodW
WXDLLIMPEXP_BASE double wxCRT_StrtodW(const wchar_t *nptr, wchar_t **endptr);
#endif

#ifndef wxCRT_StrtolW
WXDLLIMPEXP_BASE long int wxCRT_StrtolW(const wchar_t *nptr, wchar_t **endptr, int base);
#endif

#ifndef wxCRT_StrtoulW
WXDLLIMPEXP_BASE unsigned long int wxCRT_StrtoulW(const wchar_t *nptr, wchar_t **endptr, int base);
#endif

#ifndef wxCRT_StrxfrmW
WXDLLIMPEXP_BASE size_t wxCRT_StrxfrmW(wchar_t *dest, const wchar_t *src, size_t n);
#endif

#ifndef wxCRT_StrdupA
WXDLLIMPEXP_BASE char *wxCRT_StrdupA(const char *psz);
#endif

#ifndef wxCRT_StrdupW
WXDLLIMPEXP_BASE wchar_t *wxCRT_StrdupW(const wchar_t *pwz);
#endif

#ifndef wxCRT_StricmpA
WXDLLIMPEXP_BASE int wxCRT_StricmpA(const char *psz1, const char *psz2);
#endif

#ifndef wxCRT_StricmpW
WXDLLIMPEXP_BASE int wxCRT_StricmpW(const wchar_t *psz1, const wchar_t *psz2);
#endif

#ifndef wxCRT_StrnicmpA
WXDLLIMPEXP_BASE int wxCRT_StrnicmpA(const char *psz1, const char *psz2, size_t len);
#endif

#ifndef wxCRT_StrnicmpW
WXDLLIMPEXP_BASE int wxCRT_StrnicmpW(const wchar_t *psz1, const wchar_t *psz2, size_t len);
#endif

#ifndef wxCRT_StrtokA
WXDLLIMPEXP_BASE char *wxCRT_StrtokA(char *psz, const char *delim, char **save_ptr);
#endif

#ifndef wxCRT_StrtokW
WXDLLIMPEXP_BASE wchar_t *wxCRT_StrtokW(wchar_t *psz, const wchar_t *delim, wchar_t **save_ptr);
#endif

/* supply strtoll and strtoull, if needed */
#ifdef wxLongLong_t
    #ifndef wxCRT_StrtollA
        WXDLLIMPEXP_BASE wxLongLong_t wxCRT_StrtollA(const char* nptr,
                                                     char** endptr,
                                                     int base);
        WXDLLIMPEXP_BASE wxULongLong_t wxCRT_StrtoullA(const char* nptr,
                                                       char** endptr,
                                                       int base);
    #endif
    #ifndef wxCRT_StrtollW
        WXDLLIMPEXP_BASE wxLongLong_t wxCRT_StrtollW(const wchar_t* nptr,
                                                     wchar_t** endptr,
                                                     int base);
        WXDLLIMPEXP_BASE wxULongLong_t wxCRT_StrtoullW(const wchar_t* nptr,
                                                       wchar_t** endptr,
                                                       int base);
    #endif
#endif // wxLongLong_t


/* -------------------------------------------------------------------------
                                  stdio.h
   ------------------------------------------------------------------------- */

#if defined(__UNIX__) || defined(__WXMAC__)
    #define wxMBFILES 1
#else
    #define wxMBFILES 0
#endif


/* these functions are only needed in the form used for filenames (i.e. char*
   on Unix, wchar_t* on Windows), so we don't need to use A/W suffix: */
#if wxMBFILES || !wxUSE_UNICODE /* ANSI filenames */

    #define wxCRT_Fopen   fopen
    #define wxCRT_Freopen freopen
    #define wxCRT_Remove  remove
    #define wxCRT_Rename  rename

#else /* Unicode filenames */

    /* special case: these functions are missing under Win9x with Unicows so we
       have to implement them ourselves */
    #if wxUSE_UNICODE_MSLU
            WXDLLIMPEXP_BASE FILE* wxMSLU__wfopen(const wchar_t *name, const wchar_t *mode);
            WXDLLIMPEXP_BASE FILE* wxMSLU__wfreopen(const wchar_t *name, const wchar_t *mode, FILE *stream);
            WXDLLIMPEXP_BASE int wxMSLU__wrename(const wchar_t *oldname, const wchar_t *newname);
            WXDLLIMPEXP_BASE int wxMSLU__wremove(const wchar_t *name);
            #define wxCRT_Fopen     wxMSLU__wfopen
            #define wxCRT_Freopen   wxMSLU__wfreopen
            #define wxCRT_Remove    wxMSLU__wremove
            #define wxCRT_Rename    wxMSLU__wrename
    #else
        #define wxCRT_Rename   _wrename
        #define wxCRT_Fopen    _wfopen
        #define wxCRT_Freopen  _wfreopen
        #ifdef __WXWINCE__
            /* carefully: wxCRT_Remove() must return 0 on success while
               DeleteFile() returns 0 on error, so don't just define one as
               the other */
            int wxCRT_Remove(const wchar_t *path);
        #else
            #define wxCRT_Remove _wremove
        #endif
    #endif

#endif /* wxMBFILES/!wxMBFILES */

#define wxCRT_PutsA       puts
#define wxCRT_FputsA      fputs
#define wxCRT_FgetsA      fgets
#define wxCRT_FputcA      fputc
#define wxCRT_FgetcA      fgetc
#define wxCRT_UngetcA     ungetc

#ifdef wxHAVE_TCHAR_SUPPORT
    #define wxCRT_PutsW   _putws
    #define wxCRT_FputsW  fputws
    #define wxCRT_FputcW  fputwc
#endif
#ifdef HAVE_FPUTWS
    #define wxCRT_FputsW  fputws
#endif
#ifdef HAVE_PUTWS
    #define wxCRT_PutsW   putws
#endif
#ifdef HAVE_FPUTWC
    #define wxCRT_FputcW  fputwc
#endif
#define wxCRT_FgetsW  fgetws

#ifndef wxCRT_PutsW
WXDLLIMPEXP_BASE int wxCRT_PutsW(const wchar_t *ws);
#endif

#ifndef wxCRT_FputsW
WXDLLIMPEXP_BASE int wxCRT_FputsW(const wchar_t *ch, FILE *stream);
#endif

#ifndef wxCRT_FputcW
WXDLLIMPEXP_BASE int wxCRT_FputcW(wchar_t wc, FILE *stream);
#endif

/*
   NB: tmpnam() is unsafe and thus is not wrapped!
       Use other wxWidgets facilities instead:
        wxFileName::CreateTempFileName, wxTempFile, or wxTempFileOutputStream
*/
#define wxTmpnam(x)         wxTmpnam_is_insecure_use_wxTempFile_instead

#define wxCRT_PerrorA   perror
#ifdef wxHAVE_TCHAR_SUPPORT
    #define wxCRT_PerrorW _wperror
#endif

/* -------------------------------------------------------------------------
                                  stdlib.h
   ------------------------------------------------------------------------- */

/* there are no env vars at all under CE, so no _tgetenv neither */
#ifdef __WXWINCE__
    /* can't define as inline function as this is a C file... */
    #define wxCRT_GetenvA(name)     ((char*)NULL)
    #define wxCRT_GetenvW(name)     ((wchar_t*)NULL)
#else
    #define wxCRT_GetenvA           getenv
    #ifdef _tgetenv
        #define wxCRT_GetenvW       _wgetenv
    #endif
#endif

#ifndef wxCRT_GetenvW
WXDLLIMPEXP_BASE wchar_t * wxCRT_GetenvW(const wchar_t *name);
#endif


#define wxCRT_SystemA               system
/* mingw32 doesn't provide _tsystem() or _wsystem(): */
#if defined(_tsystem)
    #define  wxCRT_SystemW          _wsystem
#endif

#define wxCRT_AtofA                 atof
#define wxCRT_AtoiA                 atoi
#define wxCRT_AtolA                 atol

#if defined(__MWERKS__)
    #if defined(__MSL__)
        #define wxCRT_AtofW         watof
        #define wxCRT_AtoiW         watoi
        #define wxCRT_AtolW         watol
    /* else: use ANSI versions */
    #endif
#elif defined(wxHAVE_TCHAR_SUPPORT)
    #define  wxCRT_AtoiW           _wtoi
    #define  wxCRT_AtolW           _wtol
    /* _wtof doesn't exist */
#else
#ifndef __VMS
    #define wxCRT_AtofW(s)         wcstof(s, NULL)
#endif
    #define wxCRT_AtolW(s)         wcstol(s, NULL, 10)
    /* wcstoi doesn't exist */
#endif

/*
    There are 2 unrelated problems with these functions under Mac:
        a) Metrowerks MSL CRT implements them strictly in C99 sense and
           doesn't support (very common) extension of allowing to call
           mbstowcs(NULL, ...) which makes it pretty useless as you can't
           know the size of the needed buffer
        b) OS X <= 10.2 declares and even defined these functions but
           doesn't really implement them -- they always return an error

    So use our own replacements in both cases.
 */
#if defined(__MWERKS__) && defined(__MSL__)
    #define wxNEED_WX_MBSTOWCS
#endif

#ifdef __DARWIN__
    #if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_2
        #define wxNEED_WX_MBSTOWCS
    #endif
#endif

#ifdef wxNEED_WX_MBSTOWCS
    /* even though they are defined and "implemented", they are bad and just
       stubs so we need our own - we need these even in ANSI builds!! */
    WXDLLIMPEXP_BASE size_t wxMbstowcs(wchar_t *, const char *, size_t);
    WXDLLIMPEXP_BASE size_t wxWcstombs(char *, const wchar_t *, size_t);
#else
    #define wxMbstowcs mbstowcs
    #define wxWcstombs wcstombs
#endif



/* -------------------------------------------------------------------------
                                time.h
   ------------------------------------------------------------------------- */

#define wxCRT_StrftimeA  strftime
/* FIXME-UTF8: when is this available? */
#define wxCRT_StrftimeW  wcsftime

#ifndef wxCRT_StrftimeW
WXDLLIMPEXP_BASE size_t wxCRT_StrftimeW(wchar_t *s, size_t max,
                                        const wchar_t *fmt,
                                        const struct tm *tm);
#endif



/* -------------------------------------------------------------------------
                                ctype.h
   ------------------------------------------------------------------------- */

#ifdef __WATCOMC__
  #define WXWCHAR_T_CAST(c) (wint_t)(c)
#else
  #define WXWCHAR_T_CAST(c) c
#endif

#define wxCRT_IsalnumW(c)   iswalnum(WXWCHAR_T_CAST(c))
#define wxCRT_IsalphaW(c)   iswalpha(WXWCHAR_T_CAST(c))
#define wxCRT_IscntrlW(c)   iswcntrl(WXWCHAR_T_CAST(c))
#define wxCRT_IsdigitW(c)   iswdigit(WXWCHAR_T_CAST(c))
#define wxCRT_IsgraphW(c)   iswgraph(WXWCHAR_T_CAST(c))
#define wxCRT_IslowerW(c)   iswlower(WXWCHAR_T_CAST(c))
#define wxCRT_IsprintW(c)   iswprint(WXWCHAR_T_CAST(c))
#define wxCRT_IspunctW(c)   iswpunct(WXWCHAR_T_CAST(c))
#define wxCRT_IsspaceW(c)   iswspace(WXWCHAR_T_CAST(c))
#define wxCRT_IsupperW(c)   iswupper(WXWCHAR_T_CAST(c))
#define wxCRT_IsxdigitW(c)  iswxdigit(WXWCHAR_T_CAST(c))

#ifdef __GLIBC__
    #if defined(__GLIBC__) && (__GLIBC__ == 2) && (__GLIBC_MINOR__ == 0)
        /* /usr/include/wctype.h incorrectly declares translations */
        /* tables which provokes tons of compile-time warnings -- try */
        /* to correct this */
        #define wxCRT_TolowerW(wc) towctrans((wc), (wctrans_t)__ctype_tolower)
        #define wxCRT_ToupperW(wc) towctrans((wc), (wctrans_t)__ctype_toupper)
    #else /* !glibc 2.0 */
        #define wxCRT_TolowerW   towlower
        #define wxCRT_ToupperW   towupper
    #endif
#else /* !__GLIBC__ */
    /* There is a bug in VC6 C RTL: toxxx() functions dosn't do anything
       with signed chars < 0, so "fix" it here. */
    #define wxCRT_TolowerW(c)   towlower((wxUChar)(wxChar)(c))
    #define wxCRT_ToupperW(c)   towupper((wxUChar)(wxChar)(c))
#endif /* __GLIBC__/!__GLIBC__ */





/* -------------------------------------------------------------------------
       wx wrappers for CRT functions in both char* and wchar_t* versions
   ------------------------------------------------------------------------- */

#ifdef __cplusplus

/* NB: this belongs to wxcrt.h and not this header, but it makes life easier
 *     for buffer.h and stringimpl.h (both of which must be included before
 *     string.h, which is required by wxcrt.h) to have them here: */

/* safe version of strlen() (returns 0 if passed NULL pointer) */
inline size_t wxStrlen(const char *s) { return s ? wxCRT_StrlenA(s) : 0; }
inline size_t wxStrlen(const wchar_t *s) { return s ? wxCRT_StrlenW(s) : 0; }
#define wxWcslen wxCRT_StrlenW

#define wxStrdupA wxCRT_StrdupA
#define wxStrdupW wxCRT_StrdupW
inline char* wxStrdup(const char *s) { return wxCRT_StrdupA(s); }
inline wchar_t* wxStrdup(const wchar_t *s) { return wxCRT_StrdupW(s); }

#endif /* __cplusplus */

#endif /* _WX_WXCRTBASE_H_ */
