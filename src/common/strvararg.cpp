///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/strvararg.cpp
// Purpose:     macros for implementing type-safe vararg passing of strings
// Author:      Vaclav Slavik
// Created:     2007-02-19
// RCS-ID:      $Id$
// Copyright:   (c) 2007 REA Elektronik GmbH
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/strvararg.h"
#include "wx/buffer.h"
#include "wx/strconv.h"
#include "wx/string.h"

// ============================================================================
// implementation
// ============================================================================

const wxStringCharType *wxArgNormalizer<const wxCStrData&>::get() const
{
    return m_value;
}

const wxStringCharType *wxArgNormalizer<const wxString&>::get() const
{
    return m_value.wx_str();
}

#if wxUSE_UNICODE_WCHAR

wxArgNormalizer<const char*>::wxArgNormalizer(const char *value)
{
    m_value = new wxWCharBuffer(wxConvLibc.cMB2WC(value));
}

wxArgNormalizer<const char*>::~wxArgNormalizer()
{
    delete m_value;
}

const wchar_t *wxArgNormalizer<const char*>::get() const
{
    return m_value->data();
}

#elif wxUSE_WCHAR_T // !wxUSE_UNICODE_WCHAR && wxUSE_WCHAR_T

wxArgNormalizer<const wchar_t*>::wxArgNormalizer(const wchar_t *value)
{
    m_value = new wxCharBuffer(wxConvLibc.cWC2MB(value));
}

wxArgNormalizer<const wchar_t*>::~wxArgNormalizer()
{
    delete m_value;
}

const char *wxArgNormalizer<const wchar_t*>::get() const
{
    return m_value->data();
}

#endif // wxUSE_UNICODE_WCHAR / !wxUSE_UNICODE_WCHAR && wxUSE_WCHAR_T

// FIXME-UTF8: move this to the header once it's possible to include buffer.h
//             without including wxcrt.h

wxArgNormalizer<wxCharBuffer>::wxArgNormalizer(const wxCharBuffer& buf)
    : wxArgNormalizer<const char*>(buf.data())
{
}

wxArgNormalizer<wxWCharBuffer>::wxArgNormalizer(const wxWCharBuffer& buf)
    : wxArgNormalizer<const wchar_t*>(buf.data())
{
}
