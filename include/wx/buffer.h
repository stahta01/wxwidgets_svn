///////////////////////////////////////////////////////////////////////////////
// Name:        buffer.h
// Purpose:     auto buffer classes: buffers which automatically free memory
// Author:      Vadim Zeitlin
// Modified by:
// Created:     12.04.99
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// these classes are for private use only for now, they're not documented

#ifndef _WX_BUFFER_H
#define _WX_BUFFER_H

#include "wx/wxchar.h"

#include <string.h> // strdup

// ----------------------------------------------------------------------------
// Special classes for (wide) character strings: they use malloc/free instead
// of new/delete
// ----------------------------------------------------------------------------

class wxCharBuffer
{
public:
    wxCharBuffer(const char *str)
    {
        wxASSERT_MSG( str, wxT("NULL string in wxCharBuffer") );

        m_str = str ? wxStrdup(str) : (char *)NULL;
    }
    wxCharBuffer(size_t len)
    {
        m_str = (char *)malloc(len+1);
        m_str[len] = '\0';
    }
    // no need to check for NULL, free() does it
   ~wxCharBuffer() { free(m_str); }

   wxCharBuffer(const wxCharBuffer& src)
   {
     m_str = src.m_str;
     // no reference count yet...
     ((wxCharBuffer*)&src)->m_str = (char *)NULL;
   }
   wxCharBuffer& operator=(const wxCharBuffer& src)
   {
     m_str = src.m_str;
     // no reference count yet...
     ((wxCharBuffer*)&src)->m_str = (char *)NULL;
     return *this;
   }

   const char *data() const { return m_str; }
   operator const char *() const { return m_str; }
   char operator[](size_t n) const { return m_str[n]; }

private:
   char *m_str;
};

#if wxUSE_WCHAR_T
class wxWCharBuffer
{
public:
    wxWCharBuffer(const wchar_t *wcs)
    {
        wxASSERT_MSG( wcs, wxT("NULL string in wxWCharBuffer") );

        if (wcs) {
#if ( defined(__BORLANDC__) && (__BORLANDC__ > 0x530) ) \
    || ( defined(__MWERKS__) && defined(__WXMSW__) )
          size_t siz = (std::wcslen(wcs)+1)*sizeof(wchar_t);
#else
          size_t siz = (::wcslen(wcs)+1)*sizeof(wchar_t);
#endif
          m_wcs = (wchar_t *)malloc(siz);
          memcpy(m_wcs, wcs, siz);
        }
        else m_wcs = (wchar_t *)NULL;
    }
    wxWCharBuffer(size_t len)
    {
        m_wcs = (wchar_t *)malloc((len+1)*sizeof(wchar_t));
        m_wcs[len] = L'\0';
    }

    // no need to check for NULL, free() does it
   ~wxWCharBuffer() { free(m_wcs); }

   wxWCharBuffer(const wxWCharBuffer& src)
   {
     m_wcs = src.m_wcs;
     // no reference count yet...
     ((wxWCharBuffer*)&src)->m_wcs = (wchar_t *)NULL;
   }
   wxWCharBuffer& operator=(const wxWCharBuffer& src)
   {
     m_wcs = src.m_wcs;
     // no reference count yet...
     ((wxWCharBuffer*)&src)->m_wcs = (wchar_t *)NULL;
     return *this;
   }

   const wchar_t *data() const { return m_wcs; }
   operator const wchar_t *() const { return m_wcs; }
   wchar_t operator[](size_t n) const { return m_wcs[n]; }

private:
   wchar_t *m_wcs;
};
#endif

#if wxUSE_UNICODE
    #define wxMB2WXbuf wxWCharBuffer
    #define wxWX2MBbuf wxCharBuffer
    #define wxWC2WXbuf wxChar*
    #define wxWX2WCbuf wxChar*
#else // ANSI
    #define wxMB2WXbuf wxChar*
    #define wxWX2MBbuf wxChar*
    #define wxWC2WXbuf wxCharBuffer
    #define wxWX2WCbuf wxWCharBuffer
#endif // Unicode/ANSI

// ----------------------------------------------------------------------------
// template class for any kind of data
// ----------------------------------------------------------------------------

// TODO

#endif // _WX_BUFFER_H
