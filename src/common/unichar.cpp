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

#include "wx/unichar.h"

// ===========================================================================
// implementation
// ===========================================================================

/* static */
wxUniChar::unicode_type wxUniChar::From8bit(char c)
{
    // all supported charsets have the first 128 characters same as ASCII:
    if ( (unsigned char)c < 0x80 )
        return c;

    wchar_t buf[2];
    if ( wxConvLibc.ToWChar(buf, 2, &c, 1) != 2 )
        return wxT('?'); // FIXME-UTF8: what to use as failure character?
    return buf[0];
}

/* static */
char wxUniChar::To8bit(wxUniChar::unicode_type c)
{
    // all supported charsets have the first 128 characters same as ASCII:
    if ( c < 0x80 )
        return c;

    wchar_t in = c;
    char buf[2];
    if ( wxConvLibc.FromWChar(buf, 2, &in, 1) != 2 )
        return '?'; // FIXME-UTF8: what to use as failure character?
    return buf[0];
}
