/////////////////////////////////////////////////////////////////////////////
// Name:        chartype.h
// Purpose:     interface of global functions
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/** @ingroup group_funcmacro_string */
//@{

/**
    This macro can be used with character and string literals (in other words,
    @c 'x' or @c "foo") to automatically convert them to wide strings in Unicode
    builds of wxWidgets. This macro simply returns the value passed to it
    without changes in ASCII build. In fact, its definition is:

@code
#ifdef UNICODE
#   define wxT(x)  L##x
#else // !Unicode
#   define wxT(x)  x
#endif
@endcode

    @see @ref overview_unicode, wxS()

    @header{wx/chartype.h}
*/
#define wxT(string)

/**
    wxS is macro which can be used with character and string literals (in other words,
    @c 'x' or @c "foo") to either convert them to wide characters or wide strings
    in @c wchar_t-based (UTF-16) builds or keep them unchanged in @c char-based
    (UTF-8) builds.

    The use of this macro is optional as the translation will always be done at
    run-time even if there is a mismatch between the kind of the literal used
    and the string or character type used in the current build.
    However using it can be beneficial in <b>performance-sensitive code</b> to
    do the conversion at compile-time instead.

    @see @ref overview_unicode, wxT()

    @header{wx/chartype.h}
*/
#define wxS(string)

/**
    This macro is exactly the same as wxT() and is defined in wxWidgets simply
    because it may be more intuitive for Windows programmers as the standard
    Win32 headers also define it (as well as yet another name for the same
    macro which is _TEXT()).

    Don't confuse this macro with _()!

    @header{wx/chartype.h}
*/
#define _T(string)

/**
    wxChar is defined to be
    - @c char when <tt>wxUSE_UNICODE==0</tt>
    - @c wchar_t when <tt>wxUSE_UNICODE==1</tt> (the default).
*/
typedef wxUSE_UNICODE_dependent wxChar;

/**
    wxSChar is defined to be
    - <tt>signed char</tt> when <tt>wxUSE_UNICODE==0</tt>
    - @c wchar_t when <tt>wxUSE_UNICODE==1</tt> (the default).
*/
typedef wxUSE_UNICODE_dependent wxSChar;

/**
    wxUChar is defined to be
    - <tt>unsigned char</tt> when <tt>wxUSE_UNICODE==0</tt>
    - @c wchar_t when <tt>wxUSE_UNICODE==1</tt> (the default).
*/
typedef wxUSE_UNICODE_dependent wxUChar;

/**
    wxStringCharType is defined to be:
    - @c char when <tt>wxUSE_UNICODE==0</tt>
    - @c char when <tt>wxUSE_UNICODE_WCHAR==0</tt> and <tt>wxUSE_UNICODE==1</tt>
    - @c wchar_t when <tt>wxUSE_UNICODE_WCHAR==1</tt> and <tt>wxUSE_UNICODE==1</tt>

    The @c wxUSE_UNICODE_WCHAR symbol is defined to @c 1 when building on
    Windows while it's defined to @c 0 when building on Unix, Linux or OS X.

    Note that wxStringCharType is the type used by wxString for internal storage
    of the characters.
*/
typedef wxUSE_UNICODE_WCHAR_dependent wxStringCharType;

//@}
