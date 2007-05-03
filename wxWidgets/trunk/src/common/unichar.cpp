/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/unichar.cpp
// Purpose:     wxUniChar and wxUniCharRef classes
// Author:      Vaclav Slavik
// Created:     2007-03-19
// RCS-ID:      $Id$
// Copyright:   (c) 2007 REA Elektronik GmbH
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// headers
// ===========================================================================

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/strconv.h"  // wxConvLibc
#endif

#include "wx/unichar.h"
#include "wx/stringops.h"

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// wxUniChar
// ---------------------------------------------------------------------------

/* static */
wxUniChar::value_type wxUniChar::From8bit(char c)
{
    // all supported charsets have the first 128 characters same as ASCII:
    if ( (unsigned char)c < 0x80 )
        return c;

#if wxUSE_UTF8_LOCALE_ONLY
    wxFAIL_MSG( _T("invalid UTF-8 character") );
    return wxT('?'); // FIXME-UTF8: what to use as failure character?
#else
    wchar_t buf[2];
    if ( wxConvLibc.ToWChar(buf, 2, &c, 1) != 2 )
        return wxT('?'); // FIXME-UTF8: what to use as failure character?
    return buf[0];
#endif
}

/* static */
char wxUniChar::To8bit(wxUniChar::value_type c)
{
    // all supported charsets have the first 128 characters same as ASCII:
    if ( c < 0x80 )
        return c;

#if wxUSE_UTF8_LOCALE_ONLY
    wxFAIL_MSG( _T("character cannot be converted to single UTF-8 byte") );
    return '?'; // FIXME-UTF8: what to use as failure character?
#else
    wchar_t in = c;
    char buf[2];
    if ( wxConvLibc.FromWChar(buf, 2, &in, 1) != 2 )
        return '?'; // FIXME-UTF8: what to use as failure character?
    return buf[0];
#endif
}


// ---------------------------------------------------------------------------
// wxUniCharRef
// ---------------------------------------------------------------------------

#if wxUSE_UNICODE_UTF8
wxUniChar wxUniCharRef::UniChar() const
{
    return wxStringOperations::DecodeChar(m_pos);
}

wxUniCharRef& wxUniCharRef::operator=(const wxUniChar& c)
{
    wxStringOperations::Utf8CharBuffer utf(wxStringOperations::EncodeChar(c));
    size_t lenOld = wxStringOperations::GetUtf8CharLength(*m_pos);
    size_t lenNew = wxStringOperations::GetUtf8CharLength(utf[0]);

    if ( lenNew == lenOld )
    {
        iterator pos(m_pos);
        for ( size_t i = 0; i < lenNew; ++i, ++pos )
            *pos = utf[i];
    }
    else
    {
        size_t idx = m_pos - m_str.begin();

        m_str.replace(m_pos, m_pos + lenOld, utf, lenNew);

        // this is needed to keep m_pos valid:
        m_pos = m_str.begin() + idx;
    }

    return *this;
}
#endif // wxUSE_UNICODE_UTF8
