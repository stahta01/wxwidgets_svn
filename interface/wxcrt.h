/////////////////////////////////////////////////////////////////////////////
// Name:        wxcrt.h
// Purpose:     documentation for global functions
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
Returns a negative value, 0, or positive value if @a p1 is less than, equal
to or greater than @e p2. The comparison is case-sensitive.
This function complements the standard C function @e stricmp() which performs
case-insensitive comparison.
*/
int wxStrcmp(const char* p1, const char* p2);


/**
@b NB: This function is obsolete, use wxString instead.
A macro defined as:

@code
#define wxStringEq(s1, s2) (s1 && s2 && (strcmp(s1, s2) == 0))
@endcode
*/
bool wxStringEq(const wxString& s1, const wxString& s2);

/**
    @b NB: This function is obsolete, use wxString::Find instead.
    Returns @true if the substring @a s1 is found within @e s2,
    ignoring case if @a exact is @false. If @a subString is @false,
    no substring matching is done.
*/
bool wxStringMatch(const wxString& s1, const wxString& s2,
                   bool subString = true,
                   bool exact = false);

/**
    This function replaces the dangerous standard function @c sprintf() and is
    like @c snprintf() available on some platforms. The only difference with
    sprintf() is that an additional argument - buffer size - is taken and the
    buffer is never overflowed.
    Returns the number of characters copied to the buffer or -1 if there is not
    enough space.

    @see wxVsnprintf, wxString::Printf
*/
int wxSnprintf(wxChar* buf, size_t len, const wxChar* format,
               ...);

/**
    This is a convenience function wrapping
    wxStringTokenizer which simply returns all tokens
    found in the given @a str in an array.
    Please see
    wxStringTokenizer::wxStringTokenizer
    for the description of the other parameters.
*/
wxArrayString wxStringTokenize(const wxString& str,
                               const wxString& delims = wxDEFAULT_DELIMITERS,
                               wxStringTokenizerMode mode = wxTOKEN_DEFAULT);

/**
    This is a safe version of standard function @e strlen(): it does exactly the
    same thing (i.e. returns the length of the string) except that it returns 0 if
    @a p is the @NULL pointer.
*/
size_t wxStrlen(const char* p);

/**
    The same as wxSnprintf but takes a @c va_list
    argument instead of arbitrary number of parameters.
    Note that if @c wxUSE_PRINTF_POS_PARAMS is set to 1, then this function supports
    positional arguments (see wxString::Printf for more information).
    However other functions of the same family (wxPrintf, wxSprintf, wxFprintf,
    wxVfprintf,
    wxVfprintf, wxVprintf, wxVsprintf) currently do not to support positional
    parameters
    even when @c wxUSE_PRINTF_POS_PARAMS is 1.

    @see wxSnprintf, wxString::PrintfV
*/
int wxVsnprintf(wxChar* buf, size_t len, const wxChar* format,
                va_list argPtr);

/**
    Returns @true if the pointer is either @NULL or points to an empty
    string, @false otherwise.
*/
bool wxIsEmpty(const char* p);

/**
    Returns a negative value, 0, or positive value if @a p1 is less than, equal
    to or greater than @e p2. The comparison is case-insensitive.
    This function complements the standard C function @e strcmp() which performs
    case-sensitive comparison.
*/
int wxStricmp(const char* p1, const char* p2);

