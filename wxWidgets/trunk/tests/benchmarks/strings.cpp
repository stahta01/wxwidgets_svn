/////////////////////////////////////////////////////////////////////////////
// Name:        tests/benchmarks/strings.cpp
// Purpose:     String-related benchmarks
// Author:      Vadim Zeitlin
// Created:     2008-07-19
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/string.h"

#include "bench.h"

static const char asciistr[] =
    "This is just the first line of a very long 7 bit ASCII string"
    "This is just the second line of a very long 7 bit ASCII string"
    "This is just the third line of a very long 7 bit ASCII string"
    "This is just the fourth line of a very long 7 bit ASCII string"
    "This is just the fifth line of a very long 7 bit ASCII string"
    "This is just the sixth line of a very long 7 bit ASCII string"
    "This is just the seventh line of a very long 7 bit ASCII string"
    "This is just the eighth line of a very long 7 bit ASCII string"
    "This is just the ninth line of a very long 7 bit ASCII string"
    "This is just the tenth line of a very long 7 bit ASCII string"
    ;

static const char utf8str[] =
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 0"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 1"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 2"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 3"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 4"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 5"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 6"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 7"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 8"
    "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5 \xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE 9"
    ;

// this is just a baseline
BENCHMARK_FUNC(Strlen)
{
    if ( strlen(utf8str) != WXSIZEOF(utf8str) - 1 )
        return false;

    if ( strlen(asciistr) != WXSIZEOF(asciistr) - 1 )
        return false;

    return true;
}

// ----------------------------------------------------------------------------
// FromUTF8() benchmarks
// ----------------------------------------------------------------------------

BENCHMARK_FUNC(FromUTF8)
{
    wxString s = wxString::FromUTF8(utf8str);
    if ( s.empty() )
        return false;

    s = wxString::FromUTF8(asciistr);
    if ( s.empty() )
        return false;

    return true;
}

BENCHMARK_FUNC(FromUTF8WithNpos)
{
    wxString s = wxString::FromUTF8(utf8str, wxString::npos);
    if ( s.empty() )
        return false;

    s = wxString::FromUTF8(asciistr, wxString::npos);
    if ( s.empty() )
        return false;

    return true;
}

BENCHMARK_FUNC(FromUTF8WithLen)
{
    wxString s = wxString::FromUTF8(utf8str, WXSIZEOF(utf8str));
    if ( s.empty() )
        return false;

    s = wxString::FromUTF8(asciistr, WXSIZEOF(asciistr));
    if ( s.empty() )
        return false;

    return true;
}

// ----------------------------------------------------------------------------
// FromUTF8Unchecked() benchmarks
// ----------------------------------------------------------------------------

BENCHMARK_FUNC(FromUTF8Unchecked)
{
    wxString s = wxString::FromUTF8Unchecked(utf8str);
    if ( s.empty() )
        return false;

    s = wxString::FromUTF8Unchecked(asciistr);
    if ( s.empty() )
        return false;

    return true;
}

BENCHMARK_FUNC(FromUTF8UncheckedWithNpos)
{
    wxString s = wxString::FromUTF8Unchecked(utf8str, wxString::npos);
    if ( s.empty() )
        return false;

    s = wxString::FromUTF8Unchecked(asciistr, wxString::npos);
    if ( s.empty() )
        return false;

    return true;
}

BENCHMARK_FUNC(FromUTF8UncheckedWithLen)
{
    wxString s = wxString::FromUTF8Unchecked(utf8str, WXSIZEOF(utf8str));
    if ( s.empty() )
        return false;

    s = wxString::FromUTF8Unchecked(asciistr, WXSIZEOF(asciistr));
    if ( s.empty() )
        return false;

    return true;
}

// ----------------------------------------------------------------------------
// FromAscii() benchmarks
// ----------------------------------------------------------------------------

BENCHMARK_FUNC(FromAscii)
{
    wxString s = wxString::FromAscii(asciistr);
    if ( s.empty() )
        return false;

    return true;
}

BENCHMARK_FUNC(FromAsciiWithNpos)
{
    wxString s = wxString::FromAscii(asciistr);
    if ( s.empty() )
        return false;

    return true;
}

BENCHMARK_FUNC(FromAsciiWithLen)
{
    wxString s = wxString::FromAscii(asciistr, WXSIZEOF(asciistr));
    if ( s.empty() )
        return false;

    return true;
}

// ----------------------------------------------------------------------------
// simple string iteration
// ----------------------------------------------------------------------------

// baseline
BENCHMARK_FUNC(ForCString)
{
    for ( size_t n = 0; n < WXSIZEOF(asciistr); n++ )
    {
        if ( asciistr[n] == '~' )
            return false;
    }

    return true;
}

BENCHMARK_FUNC(ForStringIndex)
{
    const wxString s = wxString::FromAscii(asciistr);
    const size_t len = s.length();
    for ( size_t n = 0; n < len; n++ )
    {
        if ( s[n] == '~' )
            return false;
    }

    return true;
}

BENCHMARK_FUNC(ForStringIter)
{
    const wxString s = wxString::FromAscii(asciistr);
    const wxString::const_iterator end = s.end();
    for ( wxString::const_iterator i = s.begin(); i != end; ++i )
    {
        if ( *i == '~' )
            return false;
    }

    return true;
}

BENCHMARK_FUNC(ForStringRIter)
{
    const wxString s = wxString::FromAscii(asciistr);
    const wxString::const_reverse_iterator rend = s.rend();
    for ( wxString::const_reverse_iterator i = s.rbegin(); i != rend; ++i )
    {
        if ( *i == '~' )
            return false;
    }

    return true;
}

// ----------------------------------------------------------------------------
// wxString::Replace()
// ----------------------------------------------------------------------------

const size_t ASCIISTR_LEN = strlen(asciistr);

BENCHMARK_FUNC(ReplaceLoop)
{
    wxString str('x', ASCIISTR_LEN);
    for ( size_t n = 0; n < ASCIISTR_LEN; n++ )
    {
        if ( str[n] == 'a' )
            str[n] = 'z';
    }

    return str.length() != 0;
}

BENCHMARK_FUNC(ReplaceNone)
{
    wxString str('x', ASCIISTR_LEN);
    return str.Replace("a", "z") == 0;
}

BENCHMARK_FUNC(ReplaceSome)
{
    wxString str(asciistr);
    return str.Replace("7", "8") != 0;
}

BENCHMARK_FUNC(ReplaceAll)
{
    wxString str('x', ASCIISTR_LEN);
    return str.Replace("x", "y") != 0;
}


// ----------------------------------------------------------------------------
// string buffers: wx[W]CharBuffer
// ----------------------------------------------------------------------------

BENCHMARK_FUNC(CharBuffer)
{
    wxString str(asciistr);

    // NB: wxStrlen() is here to simulate some use of the returned buffer.
    //     Both mb_str() and wc_str() are used so that this code does something
    //     nontrivial in any build.
    return wxStrlen(str.mb_str()) == ASCIISTR_LEN &&
           wxStrlen(str.wc_str()) == ASCIISTR_LEN;
}
