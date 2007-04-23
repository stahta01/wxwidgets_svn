/*
 * Name:        wx/wxcrt.h
 * Purpose:     Type-safe ANSI and Unicode builds compatible wrappers for
 *              CRT functions
 * Author:      Joel Farley, Ove K�ven
 * Modified by: Vadim Zeitlin, Robert Roebling, Ron Lee
 * Created:     1998/06/12
 * RCS-ID:      $Id$
 * Copyright:   (c) 1998-2006 wxWidgets dev team
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_WXCRT_H_
#define _WX_WXCRT_H_

#include "wx/chartype.h"

#include <stdio.h>  /* we use FILE below */

#ifdef __cplusplus
    #if wxUSE_UNICODE_UTF8
    // flag indicating whether the current locale uses UTF-8 or not; must be
    // updated every time the locale is changed!
    #if !wxUSE_UTF8_LOCALE_ONLY
    extern WXDLLIMPEXP_BASE bool wxLocaleIsUtf8;
    #endif
    // function used to update the flag:
    extern WXDLLIMPEXP_BASE void wxUpdateLocaleIsUtf8();
    #else // !wxUSE_UNICODE_UTF8
    inline WXDLLIMPEXP_BASE void wxUpdateLocaleIsUtf8() {}
    #endif // wxUSE_UNICODE_UTF8/!wxUSE_UNICODE_UTF8
#endif // __cplusplus

#if defined(HAVE_STRTOK_R) && defined(__DARWIN__) && defined(_MSL_USING_MW_C_HEADERS) && _MSL_USING_MW_C_HEADERS
    char *strtok_r(char *, const char *, char **);
#endif


/*
    Standard headers we need here.

    NB: don't include any wxWidgets headers here because almost all of them include
        this one!
 */

/* Almost all compiler have strdup(), but not quite all: CodeWarrior under Mac */
/* and VC++ for Windows CE don't provide it */
#if defined(__VISUALC__) && __VISUALC__ >= 1400
    #define wxStrdupA _strdup
#elif !(defined(__MWERKS__) && defined(__WXMAC__)) && !defined(__WXWINCE__)
    /* use #define, not inline wrapper, as it is tested with #ifndef below */
    #define wxStrdupA strdup
#endif

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

#ifdef wxHAVE_TCHAR_SUPPORT
    /* we surely have wchar_t if we have TCHAR have wcslen() */
    #ifndef HAVE_WCSLEN
        #define HAVE_WCSLEN
    #endif
#endif /* wxHAVE_TCHAR_SUPPORT */

/*
    define wxFoo() function for each standard foo() function whose signature
    (exceptionally including the return type) includes any mention of char:
    wxFoo() is going to be a Unicode-friendly version of foo(), i.e. will have
    the same signature but with char replaced by wxChar which allows us to
    use it in Unicode build as well
 */

#ifdef wxHAVE_TCHAR_SUPPORT
    #include <ctype.h>

    #if defined(__WATCOMC__) && defined(UNICODE)
      #define WXWCHAR_T_CAST(c) (wint_t)(c)
    #else
      #define WXWCHAR_T_CAST(c) c
    #endif

    /* ctype.h functions */
    #define  wxIsalnum(c)   _istalnum(WXWCHAR_T_CAST(c))
    #define  wxIsalpha(c)   _istalpha(WXWCHAR_T_CAST(c))
    #define  wxIscntrl(c)   _istcntrl(WXWCHAR_T_CAST(c))
    #define  wxIsdigit(c)   _istdigit(WXWCHAR_T_CAST(c))
    #define  wxIsgraph(c)   _istgraph(WXWCHAR_T_CAST(c))
    #define  wxIslower(c)   _istlower(WXWCHAR_T_CAST(c))
    #define  wxIsprint(c)   _istprint(WXWCHAR_T_CAST(c))
    #define  wxIspunct(c)   _istpunct(WXWCHAR_T_CAST(c))
    #define  wxIsspace(c)   _istspace(WXWCHAR_T_CAST(c))
    #define  wxIsupper(c)   _istupper(WXWCHAR_T_CAST(c))
    #define  wxIsxdigit(c)  _istxdigit(WXWCHAR_T_CAST(c))

    /*
       There is a bug in VC6 C RTL: toxxx() functions dosn't do anything with
       signed chars < 0, so "fix" it here.
     */
    #define  wxTolower(c) _totlower((wxUChar)(wxChar)(c))
    #define  wxToupper(c) _totupper((wxUChar)(wxChar)(c))

    /* locale.h functons */
    #define  wxSetlocale_ _tsetlocale

    /* string.h functions */
    #define  wxStrcat    _tcscat
    #define  wxStrchr    _tcschr
    #define  wxStrcmp    _tcscmp
    #define  wxStrcoll   _tcscoll
    #define  wxStrcpy    _tcscpy
    #define  wxStrcspn   _tcscspn
    #define  wxStrdupW   _wcsdup        /* notice the 'W'! */
    #define  wxStrftime  _tcsftime
    #define  wxStricmp   _tcsicmp
    #define  wxStrnicmp  _tcsnicmp
    #define  wxStrlen_   _tcslen        /* used in wxStrlen inline function */
    #define  wxStrncat   _tcsncat
    #define  wxStrncmp   _tcsncmp
    #define  wxStrncpy   _tcsncpy
    #define  wxStrpbrk   _tcspbrk
    #define  wxStrrchr   _tcsrchr
    #define  wxStrspn    _tcsspn
    #define  wxStrstr    _tcsstr
    #define  wxStrtod    _tcstod
    #define  wxStrtol    _tcstol
    #define  wxStrtoul   _tcstoul
    #ifdef __VISUALC__
        #if __VISUALC__ >= 1300 && !defined(__WXWINCE__)
            #define wxStrtoll  _tcstoi64
            #define wxStrtoull _tcstoui64
        #endif /* VC++ 7+ */
    #endif
    #define  wxStrxfrm   _tcsxfrm

    /* stdio.h functions */
    #define  wxFgetc     _fgettc
    #define  wxFgetchar  _fgettchar
    #define  wxFgets     _fgetts
    #if wxUSE_UNICODE_MSLU
        WXDLLIMPEXP_BASE FILE * wxMSLU__tfopen(const wxChar *name, const wxChar *mode);

        #define  wxFopen    wxMSLU__tfopen
    #else
        #define  wxFopen     _tfopen
    #endif
    #define  wxFputc     _fputtc
    #define  wxFputchar  _fputtchar
    #define  wxFputs     _fputts
    #define  wxFreopen   _tfreopen
    #define  wxGetc      _gettc
    #define  wxGetchar   _gettchar
    #define  wxGets      _getts
    #define  wxPerror    _tperror
    #define  wxPutc(c,f) _puttc(WXWCHAR_T_CAST(c),f)
    #define  wxPutchar   _puttchar
    #define  wxPuts      _putts

    #define  wxTmpnam    _ttmpnam
    #define  wxUngetc    _tungetc

    /* special case: these functions are missing under Win9x with Unicows so we */
    /* have to implement them ourselves */
    #if wxUSE_UNICODE_MSLU
        WXDLLIMPEXP_BASE int wxMSLU__trename(const wxChar *oldname, const wxChar *newname);
        WXDLLIMPEXP_BASE int wxMSLU__tremove(const wxChar *name);

        #define  wxRemove    wxMSLU__tremove
        #define  wxRename    wxMSLU__trename
    #else
        #ifdef __WXWINCE__
            /* carefully: wxRemove() must return 0 on success while DeleteFile()
               returns 0 on error, so don't just define one as the other */
            int wxRemove(const wxChar *path);
        #else
            #define  wxRemove    _tremove
            #define  wxRename    _trename
        #endif
    #endif

    /* stdlib.h functions */
    #define  wxAtoi      _ttoi
    #define  wxAtol      _ttol
    /* #define  wxAtof   _tttof -- notice that there is no such thing (why?) */
    /* there are no env vars at all under CE, so no _tgetenv neither */
    #ifdef __WXWINCE__
        /* can't define as inline function as this is a C file... */
        #define wxGetenv(name)  ((wxChar *)NULL)
    #else
        #define  wxGetenv    _tgetenv
    #endif
    #define  wxSystem    _tsystem

    /* time.h functions */
    #define  wxAsctime   _tasctime
    #define  wxCtime     _tctime

    #define wxMbstowcs mbstowcs
    #define wxWcstombs wcstombs
#else /* !TCHAR-aware compilers */
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
        WXDLLIMPEXP_BASE size_t wxMbstowcs (wchar_t *, const char *, size_t);
        WXDLLIMPEXP_BASE size_t wxWcstombs (char *, const wchar_t *, size_t);
    #else
        #define wxMbstowcs mbstowcs
        #define wxWcstombs wcstombs
    #endif

    /*
       The system C library on Mac OS X 10.2 and below does not support
       unicode: in other words all wide-character functions such as towupper et
       al. do simply not exist so we need to provide our own in that context,
       except for the wchar_t definition/typedef itself.

       We need to do this for both project builder and CodeWarrior as
       the latter uses the system C library in Mach builds for wide character
       support, which as mentioned does not exist on 10.2 and below.
    */
    #if wxUSE_UNICODE && \
        defined(__DARWIN__) && \
            ( MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_2 )
        /* we need everything! */
        #define wxNEED_WX_STRING_H
        #define wxNEED_WX_CTYPE_H

        #define  wxFgetchar(c)  wxFgetc(c, stdin)
        #define  wxFputc     wxPutc
        #define  wxFputchar(c)  wxPutc(c, stdout)
        #define  wxGetc      wxFgetc
        #define  wxGetchar(c)   wxFgetc(c, stdin)

        #include <stdio.h>

        #define wxNEED_FGETC
        #define wxNEED_FGETS
        #define wxNEED_GETS
        #define wxNEED_UNGETC

        #define wxNEED_FPUTS
        #define wxNEED_PUTS
        #define wxNEED_PUTC

        int wxFputs(const wxChar *ch, FILE *stream);
        int wxPuts(const wxChar *ws);
        int wxPutc(wxChar ch, FILE *stream);

        #ifdef __cplusplus
        extern "C" {
        #endif
            WXDLLIMPEXP_BASE size_t   wxStrlen_(const wxChar *s);
        #ifdef __cplusplus
        }
        #endif

        #define wxPutchar(wch) wxPutc(wch, stdout)

        #define wxNEED_PRINTF_CONVERSION
        #define wxNEED_WX_STDIO_H
        #define wxNEED_WX_STDLIB_H
        #define wxNEED_WX_TIME_H

    #elif wxUSE_UNICODE
        #include <wctype.h>

        /* this is probably glibc-specific */
        #if defined(__WCHAR_TYPE__) && !defined(__MWERKS__)
            /* ctype.h functions (wctype.h) */
            #define  wxIsalnum   iswalnum
            #define  wxIsalpha   iswalpha
            #define  wxIscntrl   iswcntrl
            #define  wxIsdigit   iswdigit
            #define  wxIsgraph   iswgraph
            #define  wxIslower   iswlower
            #define  wxIsprint   iswprint
            #define  wxIspunct   iswpunct
            #define  wxIsspace   iswspace
            #define  wxIsupper   iswupper
            #define  wxIsxdigit  iswxdigit

            #if defined(__GLIBC__) && (__GLIBC__ == 2) && (__GLIBC_MINOR__ == 0)
                /* /usr/include/wctype.h incorrectly declares translations */
                /* tables which provokes tons of compile-time warnings -- try */
                /* to correct this */
                #define  wxTolower(wc)   towctrans((wc), (wctrans_t)__ctype_tolower)
                #define  wxToupper(wc)   towctrans((wc), (wctrans_t)__ctype_toupper)
            #else /* !glibc 2.0 */
                #define  wxTolower   towlower
                #define  wxToupper   towupper
            #endif /* gcc/!gcc */

            /* string.h functions (wchar.h) */
            #define  wxStrcat    wcscat
            #define  wxStrchr    wcschr
            #define  wxStrcmp    wcscmp
            #define  wxStrcoll   wcscoll
            #define  wxStrcpy    wcscpy
            #define  wxStrcspn   wcscspn
            #define  wxStrlen_   wxWcslen /* wxStrlen_() is used in wxStrlen() */
            #define  wxStrncat   wcsncat
            #define  wxStrncmp   wcsncmp
            #define  wxStrncpy   wcsncpy
            #define  wxStrpbrk   wcspbrk
            #define  wxStrrchr   wcsrchr
            #define  wxStrspn    wcsspn
            #define  wxStrstr    wcsstr
            #define  wxStrtod    wcstod
            #define  wxStrtol    wcstol
            #define  wxStrtoul   wcstoul
            #ifdef HAVE_WCSTOULL
                /* assume that we have wcstoull(), which is also C99, too */
                #define  wxStrtoll   wcstoll
                #define  wxStrtoull  wcstoull
            #endif /* HAVE_WCSTOULL */
            #define  wxStrxfrm   wcsxfrm

            #define  wxFgetc     fgetwc
            #define  wxFgetchar  fgetwchar
            #define  wxFgets     fgetws
            #define  wxFputc     fputwc
            #define  wxFputchar  fputwchar
            #define  wxGetc      getwc
            #define  wxGetchar   getwchar
            #define  wxGets      getws
            #define  wxUngetc    ungetwc

            #ifdef HAVE_FPUTWS
                #define wxFputs     fputws
            #else
                #define wxNEED_FPUTS
                #include <stdio.h>
                int wxFputs(const wxChar *ch, FILE *stream);
            #endif

            #ifdef HAVE_WPUTC
                #define wxPutc      wputc
            #else
                #define wxNEED_PUTC
                #include <stdio.h>
                int wxPutc(wxChar ch, FILE *stream);
            #endif

            #ifdef HAVE_WPUTCHAR
                #define wxPutchar   wputchar
            #else
                #define wxPutchar(wch) wxPutc(wch, stdout)
            #endif

            #ifdef HAVE_PUTWS
                #define wxPuts      putws
            #else
                #define wxNEED_PUTS
                int wxPuts(const wxChar *ws);
            #endif

            /* we need %s to %ls conversion for printf and scanf etc */
            #define wxNEED_PRINTF_CONVERSION

            /* glibc doesn't have wide char equivalents of the other stuff so */
            /* use our own versions */
            #define wxNEED_WX_STDIO_H
            #define wxNEED_WX_STDLIB_H
            #define wxNEED_WX_TIME_H
        #elif defined(__MWERKS__) && ( defined(__MSL__) || defined(__MACH__) )
            /* ctype.h functions (wctype.h) */
            #define  wxIsalnum   iswalnum
            #define  wxIsalpha   iswalpha
            #define  wxIscntrl   iswcntrl
            #define  wxIsdigit   iswdigit
            #define  wxIsgraph   iswgraph
            #define  wxIslower   iswlower
            #define  wxIsprint   iswprint
            #define  wxIspunct   iswpunct
            #define  wxIsspace   iswspace
            #define  wxIsupper   iswupper
            #define  wxIsxdigit  iswxdigit
            #define  wxTolower   towlower
            #define  wxToupper   towupper

            /* string.h functions (wchar.h) */
            #define  wxStrcat    wcscat
            #define  wxStrchr    wcschr
            #define  wxStrcmp    wcscmp
            #define  wxStrcoll   wcscoll
            #define  wxStrcpy    wcscpy
            #define  wxStrcspn   wcscspn
            #define  wxStrlen_   wxWcslen /* wxStrlen_() is used in wxStrlen() */
            #define  wxStrncat   wcsncat
            #define  wxStrncmp   wcsncmp
            #define  wxStrncpy   wcsncpy
            #define  wxStrpbrk   wcspbrk
            #define  wxStrrchr   wcsrchr
            #define  wxStrspn    wcsspn
            #define  wxStrstr    wcsstr
            #define  wxStrtod    wcstod
            #define  wxStrtol    wcstol
            #define  wxStrtoul   wcstoul
            #define  wxStrxfrm   wcsxfrm

            #define  wxFgetc     fgetwc
            #define  wxFgetchar  fgetwchar
            #define  wxFgets     fgetws
            #define  wxFputc     fputwc
            #define  wxFputchar  fputwchar
            #define  wxGetc      getwc
            #define  wxGetchar   getwchar
            #define  wxGets      getws
            #define  wxUngetc    ungetwc

            #define wxNEED_PRINTF_CONVERSION

            #define  wxPutc      putwc
            #define  wxPutchar   putwchar
            #define  wxFputs     fputws

            /* stdio.h functions */

            #define wxNEED_WX_STDIO_H

            /* stdlib.h functions */
            #ifdef __MACH__
            #define wxNEED_WX_STDLIB_H
            #else
            #define  wxAtof      watof
            #define  wxAtoi      watoi
            #define  wxAtol      watol
            #define  wxGetenv(a)    ((wxChar*)NULL)
            #define  wxSystem(a)    ((int)NULL)
            #endif
            /* time.h functions */
            #define  wxAsctime   wasciitime
            #define  wxCtime     wctime
            /* #define  wxStrftime  wcsftime */

            #define wxNEED_WX_TIME_H
        #else /* !metrowerks for apple */
            #error  "Please define wide character functions for your environment"
        #endif
    #else /* ASCII */
        #include <ctype.h>
        #include <string.h>

        /* ctype.h functions */
        #define  wxIsalnum   isalnum
        #define  wxIsalpha   isalpha
        #define  wxIscntrl   iscntrl
        #define  wxIsdigit   isdigit
        #define  wxIsgraph   isgraph
        #define  wxIslower   islower
        #define  wxIsprint   isprint
        #define  wxIspunct   ispunct
        #define  wxIsspace   isspace
        #define  wxIsupper   isupper
        #define  wxIsxdigit  isxdigit
        #define  wxTolower   tolower
        #define  wxToupper   toupper

         /* locale.h functons */
        #define  wxSetlocale_ setlocale

         /* string.h functions */
        #define  wxStrcat    strcat
        #define  wxStrchr    strchr
        #define  wxStrcmp    strcmp
        #define  wxStrcoll   strcoll
        #define  wxStrcpy    strcpy
        #define  wxStrcspn   strcspn

        /* wxStricmp and wxStrnicmp are defined below */
        #define  wxStrlen_   strlen /* used in wxStrlen inline function */
        #define  wxStrncat   strncat
        #define  wxStrncmp   strncmp
        #define  wxStrncpy   strncpy
        #define  wxStrpbrk   strpbrk
        #define  wxStrrchr   strrchr
        #define  wxStrspn    strspn
        #define  wxStrstr    strstr
        #define  wxStrtod    strtod
        #ifdef HAVE_STRTOK_R
            #define  wxStrtok(str, sep, last)    strtok_r(str, sep, last)
        #endif
        #define  wxStrtol    strtol
        #define  wxStrtoul   strtoul
        #ifdef HAVE_STRTOULL
            /* assume that we have wcstoull(), which is also C99, too */
            #define  wxStrtoll   strtoll
            #define  wxStrtoull  strtoull
        #endif /* HAVE_WCSTOULL */
        #define  wxStrxfrm   strxfrm

        /* stdio.h functions */
        #define  wxFopen     fopen
        #define  wxFreopen   freopen
        #define  wxRemove    remove
        #define  wxRename    rename

        #define  wxPerror    perror
        #define  wxTmpnam    tmpnam

        #define  wxFgetc     fgetc
        #define  wxFgetchar  fgetchar
        #define  wxFgets     fgets
        #define  wxFputc     fputc
        #define  wxFputs     fputs
        #define  wxFputchar  fputchar
        #define  wxGetc      getc
        #define  wxGetchar   getchar
        #define  wxGets      gets
        #define  wxPutc      putc
        #define  wxPutchar   putchar
        #define  wxPuts      puts
        #define  wxUngetc    ungetc

        /* stdlib.h functions */
        #define  wxAtof      atof
        #define  wxAtoi      atoi
        #define  wxAtol      atol
        #define  wxGetenv    getenv
        #define  wxSystem    system

        /* time.h functions */
        #define  wxAsctime   asctime
        #define  wxCtime     ctime
        #define  wxStrftime  strftime
    #endif /* Unicode/ASCII */
#endif /* TCHAR-aware compilers/the others */

/*
    various special cases
 */

/* define wxStricmp and wxStrnicmp for various compilers */

/* note that in Unicode mode we definitely are going to need our own version */
#if !defined(wxStricmp) && !wxUSE_UNICODE
    #if defined(__BORLANDC__) || defined(__WATCOMC__) || \
            defined(__SALFORDC__) || defined(__VISAGECPP__) || \
            defined(__EMX__) || defined(__DJGPP__)
        #define wxStricmp stricmp
        #define wxStrnicmp strnicmp
    #elif defined(__WXPALMOS__)
        /* FIXME: There is no equivalent to strnicmp in the Palm OS API.  This
         * quick hack should do until one can be written.
         */
        #define wxStricmp StrCaselessCompare
        #define wxStrnicmp strnicmp
    #elif defined(__SYMANTEC__) || defined(__VISUALC__) || \
            (defined(__MWERKS__) && defined(__INTEL__))
        #define wxStricmp _stricmp
        #define wxStrnicmp _strnicmp
    #elif defined(__UNIX__) || defined(__GNUWIN32__)
        #define wxStricmp strcasecmp
        #define wxStrnicmp strncasecmp
    /* #else -- use wxWidgets implementation */
    #endif
#endif /* !defined(wxStricmp) */

/* define wxWcslen() which should be always available if wxUSE_WCHAR_T == 1 (as */
/* it's used in wx/buffer.h -- and also might be used just below by wxStrlen() */
/* when wxStrlen_() is #define'd as wxWcslen so do it before defining wxStrlen) */
#if wxUSE_WCHAR_T
    #ifdef HAVE_WCSLEN
        #define wxWcslen wcslen
    #else
        WXDLLIMPEXP_BASE size_t wxWcslen(const wchar_t *s);
    #endif
#endif /* wxUSE_WCHAR_T */

#ifdef __cplusplus
/* checks whether the passed in pointer is NULL and if the string is empty */
inline bool wxIsEmpty(const wxChar *p) { return !p || !*p; }

/* safe version of strlen() (returns 0 if passed NULL pointer) */
inline size_t wxStrlen(const wxChar *psz) { return psz ? wxStrlen_(psz) : 0; }
#endif

/*
    each of strdup() and wcsdup() may or may not be available but we need both
    of them anyhow for wx/buffer.h so we define the missing one(s) in
    wxchar.cpp and so we should always have both wxStrdupA and wxStrdupW
    defined -- if this is somehow not the case in some situations, please
    correct that and not the lines here
 */
#if wxUSE_UNICODE
    #define wxStrdup wxStrdupW
#else
    #define wxStrdup wxStrdupA
#endif

#ifdef __cplusplus
WXDLLIMPEXP_BASE bool wxOKlibc(); /* for internal use */
#endif

#ifndef wxStrtoll
    /* supply strtoll and strtoull, if needed */
    WXDLLIMPEXP_BASE wxLongLong_t wxStrtoll(const wxChar* nptr, wxChar** endptr, int base);
    WXDLLIMPEXP_BASE wxULongLong_t wxStrtoull(const wxChar* nptr, wxChar** endptr, int base);
#endif

#define wxHAS_STRTOLL

/*
   In Unicode mode we need to have all standard functions such as wprintf() and
   so on but not all systems have them so use our own implementations in this
   case.
 */
#if wxUSE_UNICODE && !defined(wxHAVE_TCHAR_SUPPORT) && !defined(HAVE_WPRINTF)
    #define wxNEED_WPRINTF
#endif

/*
    various functions which might not be available in libc and for which we
    provide our own replacements in wxchar.cpp
 */

/* ctype.h functions */

/* RN: Used only under OSX <= 10.2 currently */
#ifdef wxNEED_WX_CTYPE_H
    WXDLLIMPEXP_BASE int wxIsalnum(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsalpha(wxChar ch);
    WXDLLIMPEXP_BASE int wxIscntrl(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsdigit(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsgraph(wxChar ch);
    WXDLLIMPEXP_BASE int wxIslower(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsprint(wxChar ch);
    WXDLLIMPEXP_BASE int wxIspunct(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsspace(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsupper(wxChar ch);
    WXDLLIMPEXP_BASE int wxIsxdigit(wxChar ch);
    WXDLLIMPEXP_BASE int wxTolower(wxChar ch);
    WXDLLIMPEXP_BASE int wxToupper(wxChar ch);
#endif /* wxNEED_WX_CTYPE_H */

/* under VC++ 6.0 isspace() returns 1 for 8 bit chars which completely breaks */
/* the file parsing -- this may be true for 5.0 as well, update #ifdef then */
#if defined(__VISUALC__) && (__VISUALC__ >= 1200) && !wxUSE_UNICODE
    #undef wxIsspace
    #define wxIsspace(c) ((((unsigned)(wxChar)c) < 128) && isspace(c))
#endif /* VC++ */

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

/*
   we had goofed and defined wxIsctrl() instead of (correct) wxIscntrl() in the
   initial versions of this header -- now it is too late to remove it so
   although we fixed the function/macro name above, still provide the
   backwards-compatible synonym.
 */
#define wxIsctrl wxIscntrl

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

/* RN: Used only under OSX <= 10.2 currently
   The __cplusplus ifdefs are messy, but they are required to build
   the regex library, since c does not support function overloading
*/
#ifdef wxNEED_WX_STRING_H
# ifdef __cplusplus
    extern "C" {
# endif
        WXDLLIMPEXP_BASE wxChar * wxStrcat(wxChar *dest, const wxChar *src);
        WXDLLIMPEXP_BASE const wxChar * wxStrchr(const wxChar *s, wxChar c);
        WXDLLIMPEXP_BASE int      wxStrcmp(const wxChar *s1, const wxChar *s2);
        WXDLLIMPEXP_BASE int      wxStrcoll(const wxChar *s1, const wxChar *s2);
        WXDLLIMPEXP_BASE wxChar * wxStrcpy(wxChar *dest, const wxChar *src);
        WXDLLIMPEXP_BASE size_t   wxStrcspn(const wxChar *s, const wxChar *reject);
        WXDLLIMPEXP_BASE wxChar * wxStrncat(wxChar *dest, const wxChar *src, size_t n);
        WXDLLIMPEXP_BASE int      wxStrncmp(const wxChar *s1, const wxChar *s2, size_t n);
        WXDLLIMPEXP_BASE wxChar * wxStrncpy(wxChar *dest, const wxChar *src, size_t n);
        WXDLLIMPEXP_BASE const wxChar * wxStrpbrk(const wxChar *s, const wxChar *accept);
        WXDLLIMPEXP_BASE const wxChar * wxStrrchr(const wxChar *s, wxChar c);
        WXDLLIMPEXP_BASE size_t   wxStrspn(const wxChar *s, const wxChar *accept);
        WXDLLIMPEXP_BASE const wxChar * wxStrstr(const wxChar *haystack, const wxChar *needle);
# ifdef __cplusplus
    }
# endif

    /* These functions use C++, so we can't c extern them */
    WXDLLIMPEXP_BASE double   wxStrtod(const wxChar *nptr, wxChar **endptr);
    WXDLLIMPEXP_BASE long int wxStrtol(const wxChar *nptr, wxChar **endptr, int base);
    WXDLLIMPEXP_BASE unsigned long int wxStrtoul(const wxChar *nptr, wxChar **endptr, int base);
    WXDLLIMPEXP_BASE size_t   wxStrxfrm(wxChar *dest, const wxChar *src, size_t n);

    /* inlined versions */
    #ifdef __cplusplus
        inline wxChar * wxStrchr(wxChar *s, wxChar c)
            { return (wxChar *)wxStrchr((const wxChar *)s, c); }
        inline wxChar * wxStrpbrk(wxChar *s, const wxChar *accept)
            { return (wxChar *)wxStrpbrk((const wxChar *)s, accept); }
        inline wxChar * wxStrrchr(wxChar *s, wxChar c)
            { return (wxChar *)wxStrrchr((const wxChar *)s, c); }
        inline wxChar *wxStrstr(wxChar *haystack, const wxChar *needle)
            { return (wxChar *)wxStrstr((const wxChar *)haystack, needle); }
    #endif

#endif /* wxNEED_WX_STRING_H */

#ifndef wxStrdupA
WXDLLIMPEXP_BASE char *wxStrdupA(const char *psz);
#endif

#ifndef wxStrdupW
WXDLLIMPEXP_BASE wchar_t *wxStrdupW(const wchar_t *pwz);
#endif

#ifndef wxStricmp
WXDLLIMPEXP_BASE int wxStricmp(const wxChar *psz1, const wxChar *psz2);
#endif

#ifndef wxStrnicmp
WXDLLIMPEXP_BASE int wxStrnicmp(const wxChar *psz1, const wxChar *psz2, size_t len);
#endif

#ifndef wxStrtok
WXDLLIMPEXP_BASE wxChar * wxStrtok(wxChar *psz, const wxChar *delim, wxChar **save_ptr);
#endif

#ifdef __cplusplus
    #ifndef wxSetlocale_
    class WXDLLIMPEXP_BASE wxWCharBuffer;
    WXDLLIMPEXP_BASE wxWCharBuffer wxSetlocale_(int category, const wxChar *locale);
    WXDLLIMPEXP_BASE wxWCharBuffer wxSetlocale(int category, const wxChar *locale);
    #else
    WXDLLIMPEXP_BASE wxChar *wxSetlocale(int category, const wxChar *locale);
    #endif // defined(wxSetlocale_)
#endif // __cplusplus

/* stdio.h functions */
#ifdef wxNEED_WX_STDIO_H
    #include <stdio.h>
    WXDLLIMPEXP_BASE FILE *   wxFopen(const wxChar *path, const wxChar *mode);
    WXDLLIMPEXP_BASE FILE *   wxFreopen(const wxChar *path, const wxChar *mode, FILE *stream);
    WXDLLIMPEXP_BASE int      wxRemove(const wxChar *path);
    WXDLLIMPEXP_BASE int      wxRename(const wxChar *oldpath, const wxChar *newpath);

    /* *printf() family is handled separately */
#endif /* wxNEED_WX_STDIO_H */


/* stdlib.h functions */
#ifndef wxAtof
WXDLLIMPEXP_BASE double   wxAtof(const wxChar *psz);
#endif

#ifdef wxNEED_WX_STDLIB_H
WXDLLIMPEXP_BASE int      wxAtoi(const wxChar *psz);
WXDLLIMPEXP_BASE long     wxAtol(const wxChar *psz);
WXDLLIMPEXP_BASE wxChar * wxGetenv(const wxChar *name);
WXDLLIMPEXP_BASE int      wxSystem(const wxChar *psz);
#endif


/* time.h functions */
#ifdef wxNEED_WX_TIME_H
#if defined(__MWERKS__) && defined(macintosh)
    #include <time.h>
#endif
    /*silent gabby compilers*/
    struct tm;
    WXDLLIMPEXP_BASE size_t wxStrftime(wxChar *s, size_t max,
                                       const wxChar *fmt,
                                       const struct tm *tm);
#endif /* wxNEED_WX_TIME_H */

#ifndef wxCtime
#include <time.h>
WXDLLIMPEXP_BASE wxChar *wxCtime(const time_t *timep);
#endif


/* missing functions in some WinCE versions */
#ifdef _WIN32_WCE
#if (_WIN32_WCE < 300)
WXDLLIMPEXP_BASE void *calloc( size_t num, size_t size );
#endif
#endif /* _WIN32_WCE */

/* multibyte to wide char conversion functions and macros */

#if wxUSE_WCHAR_T
    /* multibyte<->widechar conversion */
    WXDLLIMPEXP_BASE size_t wxMB2WC(wchar_t *buf, const char *psz, size_t n);
    WXDLLIMPEXP_BASE size_t wxWC2MB(char *buf, const wchar_t *psz, size_t n);

    #if wxUSE_UNICODE
        #define wxMB2WX wxMB2WC
        #define wxWX2MB wxWC2MB
        #define wxWC2WX wxStrncpy
        #define wxWX2WC wxStrncpy
    #else
        #define wxMB2WX wxStrncpy
        #define wxWX2MB wxStrncpy
        #define wxWC2WX wxWC2MB
        #define wxWX2WC wxMB2WC
    #endif
#else /* !wxUSE_UNICODE */
/* Why is this here?
#error ha */
    /* No wxUSE_WCHAR_T: we have to do something (JACS) */
    #define wxMB2WC wxStrncpy
    #define wxWC2MB wxStrncpy
    #define wxMB2WX wxStrncpy
    #define wxWX2MB wxStrncpy
    #define wxWC2WX wxWC2MB
    #define wxWX2WC wxMB2WC
#endif

/*
    RN:  The following are not normal versions of memcpy et al., rather
    these are either char or widechar versions depending on
    if unicode is used or not.
*/

#ifdef __cplusplus

    //
    //  RN: We could do the usual tricky compiler detection here,
    //  and use their variant (such as wmemchr, etc.).  The problem
    //  is that these functions are quite rare, even though they are
    //  part of the current POSIX standard.  In addition, most compilers
    //  (including even MSC) inline them just like we do right in their
    //  headers.
    //
    #if wxUSE_UNICODE
        #include <string.h> //for mem funcs

        //implement our own wmem variants
        inline wxChar* wxTmemchr(const wxChar* s, wxChar c, size_t l)
        {
            for(;l && *s != c;--l, ++s) {}

            if(l)
                return (wxChar*)s;
            return NULL;
        }

        inline int wxTmemcmp(const wxChar* sz1, const wxChar* sz2, size_t len)
        {
            for(; *sz1 == *sz2 && len; --len, ++sz1, ++sz2) {}

            if(len)
                return *sz1 < *sz2 ? -1 : *sz1 > *sz2;
            else
                return 0;
        }

        inline wxChar* wxTmemcpy(wxChar* szOut, const wxChar* szIn, size_t len)
        {
            return (wxChar*) memcpy(szOut, szIn, len * sizeof(wxChar));
        }

        inline wxChar* wxTmemmove(wxChar* szOut, const wxChar* szIn, size_t len)
        {
            return (wxChar*) memmove(szOut, szIn, len * sizeof(wxChar));
        }

        inline wxChar* wxTmemset(wxChar* szOut, const wxChar cIn, size_t len)
        {
            wxChar* szRet = szOut;

            while (len--)
                *szOut++ = cIn;

            return szRet;
        }

    #else /* !wxUSE_UNICODE */
    #   define wxTmemchr memchr
    #   define wxTmemcmp memcmp
    #   define wxTmemcpy memcpy
    #   define wxTmemmove memmove
    #   define wxTmemset memset
    #endif /* wxUSE_UNICODE/!wxUSE_UNICODE */

#endif /*__cplusplus*/

#endif /* _WX_WXCRT_H_ */
