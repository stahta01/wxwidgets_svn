/////////////////////////////////////////////////////////////////////////////
// Name:        strconv.cpp
// Purpose:     Unicode conversion classes
// Author:      Ove Kaaven, Robert Roebling, Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Ove Kaaven, Robert Roebling, Vadim Zeitlin
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
  #pragma implementation "strconv.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifdef __WXMSW__
  #include "wx/msw/private.h"
#endif

#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#ifdef __SALFORDC__
  #include <clib.h>
#endif

#ifdef HAVE_ICONV_H
  #include <iconv.h>
#endif

#ifdef __WXMSW__
  #include <windows.h>
#endif

#include "wx/debug.h"
#include "wx/strconv.h"
#include "wx/intl.h"
#include "wx/log.h"

#if defined(WORDS_BIGENDIAN) || defined(__STDC_ISO_10646__)
#define BSWAP_UCS4(str, len)
#define BSWAP_UCS2(str, len)
#else
#define BSWAP_UCS4(str, len) { unsigned _c; for (_c=0; _c<len; _c++) str[_c]=wxUINT32_SWAP_ALWAYS(str[_c]); }
#define BSWAP_UCS2(str, len) { unsigned _c; for (_c=0; _c<len; _c++) str[_c]=wxUINT16_SWAP_ALWAYS(str[_c]); }
#define WC_NEED_BSWAP
#endif
#define BSWAP_UTF32(str, len) BSWAP_UCS4(str, len)
#define BSWAP_UTF16(str, len) BSWAP_UCS2(str, len)

// under Unix SIZEOF_WCHAR_T is defined by configure, but under other platforms
// it might be not defined - assume the most common value
#ifndef SIZEOF_WCHAR_T
    #define SIZEOF_WCHAR_T 2
#endif // !defined(SIZEOF_WCHAR_T)

#if SIZEOF_WCHAR_T == 4
    #define WC_NAME "UCS4"
    #define WC_BSWAP BSWAP_UCS4
#elif SIZEOF_WCHAR_T == 2
    #define WC_NAME "UTF16"
    #define WC_BSWAP BSWAP_UTF16
    #define WC_UTF16
#else // sizeof(wchar_t) != 2 nor 4
    // I don't know what to do about this
    #error "Weird sizeof(wchar_t): please report your platform details to wx-users mailing list"
#endif

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxMBConv *) wxConvCurrent = &wxConvLibc;

// ============================================================================
// implementation
// ============================================================================

#if wxUSE_WCHAR_T

#ifdef WC_UTF16

static size_t encode_utf16(wxUint32 input, wchar_t *output)
{
    if (input<=0xffff)
    {
        if (output) *output++ = input;
        return 1;
    }
    else if (input>=0x110000)
    {
        return (size_t)-1;
    }
    else
    {
        if (output)
        {
            *output++ = (input >> 10)+0xd7c0;
            *output++ = (input&0x3ff)+0xdc00;
        }
        return 2;
    }
}

static size_t decode_utf16(const wchar_t* input, wxUint32& output)
{
    if ((*input<0xd800) || (*input>0xdfff))
    {
        output = *input;
        return 1;
    }
    else if ((input[1]<0xdc00) || (input[1]>=0xdfff))
    {
        output = *input;
        return (size_t)-1;
    }
    else
    {
        output = ((input[0] - 0xd7c0) << 10) + (input[1] - 0xdc00);
        return 2;
    }
}

#endif // WC_UTF16

// ----------------------------------------------------------------------------
// wxMBConv
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxMBConv) wxConvLibc;

size_t wxMBConv::MB2WC(wchar_t *buf, const char *psz, size_t n) const
{
    return wxMB2WC(buf, psz, n);
}

size_t wxMBConv::WC2MB(char *buf, const wchar_t *psz, size_t n) const
{
    return wxWC2MB(buf, psz, n);
}

const wxWCharBuffer wxMBConv::cMB2WC(const char *psz) const
{
    if (psz)
    {
        size_t nLen = MB2WC((wchar_t *) NULL, psz, 0);
        if (nLen == (size_t)-1)
            return wxWCharBuffer((wchar_t *) NULL);
        wxWCharBuffer buf(nLen);
        MB2WC((wchar_t *)(const wchar_t *) buf, psz, nLen);
        return buf;
    }
    else
        return wxWCharBuffer((wchar_t *) NULL);
}

const wxCharBuffer wxMBConv::cWC2MB(const wchar_t *psz) const
{
    if (psz)
    {
        size_t nLen = WC2MB((char *) NULL, psz, 0);
        if (nLen == (size_t)-1)
            return wxCharBuffer((char *) NULL);
        wxCharBuffer buf(nLen);
        WC2MB((char *)(const char *) buf, psz, nLen);
        return buf;
    }
    else
        return wxCharBuffer((char *) NULL);
}

// ----------------------------------------------------------------------------
// standard file conversion
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxMBConvFile) wxConvFile;

// just use the libc conversion for now
size_t wxMBConvFile::MB2WC(wchar_t *buf, const char *psz, size_t n) const
{
    return wxMB2WC(buf, psz, n);
}

size_t wxMBConvFile::WC2MB(char *buf, const wchar_t *psz, size_t n) const
{
    return wxWC2MB(buf, psz, n);
}

// ----------------------------------------------------------------------------
// standard gdk conversion
// ----------------------------------------------------------------------------

#ifdef __WXGTK12__

WXDLLEXPORT_DATA(wxMBConvGdk) wxConvGdk;

#include <gdk/gdk.h>

size_t wxMBConvGdk::MB2WC(wchar_t *buf, const char *psz, size_t n) const
{
    if (buf)
    {
        return gdk_mbstowcs((GdkWChar *)buf, psz, n);
    }
    else
    {
        GdkWChar *nbuf = new GdkWChar[n=strlen(psz)];
        size_t len = gdk_mbstowcs(nbuf, psz, n);
        delete[] nbuf;
        return len;
    }
}

size_t wxMBConvGdk::WC2MB(char *buf, const wchar_t *psz, size_t n) const
{
    char *mbstr = gdk_wcstombs((GdkWChar *)psz);
    size_t len = mbstr ? strlen(mbstr) : 0;
    if (buf)
    {
        if (len > n)
            len = n;
        memcpy(buf, psz, len);
        if (len < n)
            buf[len] = 0;
    }
    return len;
}

#endif // GTK > 1.0

// ----------------------------------------------------------------------------
// UTF-7
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxMBConvUTF7) wxConvUTF7;

#if 0
static char utf7_setD[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789'(),-./:?";
static char utf7_setO[]="!\"#$%&*;<=>@[]^_`{|}";
static char utf7_setB[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";
#endif

// TODO: write actual implementations of UTF-7 here
size_t wxMBConvUTF7::MB2WC(wchar_t * WXUNUSED(buf),
                           const char * WXUNUSED(psz),
                           size_t WXUNUSED(n)) const
{
  return 0;
}

size_t wxMBConvUTF7::WC2MB(char * WXUNUSED(buf),
                           const wchar_t * WXUNUSED(psz),
                           size_t WXUNUSED(n)) const
{
  return 0;
}

// ----------------------------------------------------------------------------
// UTF-8
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxMBConvUTF8) wxConvUTF8;

static wxUint32 utf8_max[]=
    { 0x7f, 0x7ff, 0xffff, 0x1fffff, 0x3ffffff, 0x7fffffff, 0xffffffff };

size_t wxMBConvUTF8::MB2WC(wchar_t *buf, const char *psz, size_t n) const
{
    size_t len = 0;

    while (*psz && ((!buf) || (len < n)))
    {
        unsigned char cc = *psz++, fc = cc;
        unsigned cnt;
        for (cnt = 0; fc & 0x80; cnt++)
            fc <<= 1;
        if (!cnt)
        {
            // plain ASCII char
            if (buf)
                *buf++ = cc;
            len++;
        }
        else
        {
            cnt--;
            if (!cnt)
            {
                // invalid UTF-8 sequence
                return (size_t)-1;
            }
            else
            {
                unsigned ocnt = cnt - 1;
                wxUint32 res = cc & (0x3f >> cnt);
                while (cnt--)
                {
                    cc = *psz++;
                    if ((cc & 0xC0) != 0x80)
                    {
                        // invalid UTF-8 sequence
                        return (size_t)-1;
                    }
                    res = (res << 6) | (cc & 0x3f);
                }
                if (res <= utf8_max[ocnt])
                {
                    // illegal UTF-8 encoding
                    return (size_t)-1;
                }
#ifdef WC_UTF16
                size_t pa = encode_utf16(res, buf);
                if (pa == (size_t)-1)
                  return (size_t)-1;
                if (buf)
                    buf += pa;
                len += pa;
#else
                if (buf)
                    *buf++ = res;
                len++;
#endif
            }
        }
    }
    if (buf && (len < n))
        *buf = 0;
    return len;
}

size_t wxMBConvUTF8::WC2MB(char *buf, const wchar_t *psz, size_t n) const
{
    size_t len = 0;

    while (*psz && ((!buf) || (len < n)))
    {
        wxUint32 cc;
#ifdef WC_UTF16
        size_t pa = decode_utf16(psz, cc);
        psz += (pa == (size_t)-1) ? 1 : pa;
#else
        cc=(*psz++) & 0x7fffffff;
#endif
        unsigned cnt;
        for (cnt = 0; cc > utf8_max[cnt]; cnt++) {}
        if (!cnt)
        {
            // plain ASCII char
            if (buf)
                *buf++ = cc;
            len++;
        }

        else
        {
            len += cnt + 1;
            if (buf)
            {
                *buf++ = (-128 >> cnt) | ((cc >> (cnt * 6)) & (0x3f >> cnt));
                while (cnt--)
                    *buf++ = 0x80 | ((cc >> (cnt * 6)) & 0x3f);
            }
        }
    }

    if (buf && (len<n)) *buf = 0;
    return len;
}

// ----------------------------------------------------------------------------
// specified character set
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxCSConv) wxConvLocal((const wxChar *)NULL);

#include "wx/encconv.h"
#include "wx/fontmap.h"

// TODO: add some tables here
// - perhaps common encodings to common codepages (for Win32)
// - perhaps common encodings to objects ("UTF8" -> wxConvUTF8)
// - move wxEncodingConverter meat in here

#ifdef __WIN32__
#include "wx/msw/registry.h"
// this should work if M$ Internet Exploiter is installed
static long CharsetToCodepage(const wxChar *name)
{
    if (!name)
        return GetACP();

    long CP=-1;

    wxString cn(name);
    do {
        wxString path(wxT("MIME\\Database\\Charset\\"));
        path += cn;
        wxRegKey key(wxRegKey::HKCR, path);

        if (!key.Exists()) break;

        // two cases: either there's an AliasForCharset string,
        // or there are Codepage and InternetEncoding dwords.
        // The InternetEncoding gives us the actual encoding,
        // the Codepage just says which Windows character set to
        // use when displaying the data.
        if (key.HasValue(wxT("InternetEncoding")) &&
            key.QueryValue(wxT("InternetEncoding"), &CP)) break;

        // no encoding, see if it's an alias
        if (!key.HasValue(wxT("AliasForCharset")) ||
            !key.QueryValue(wxT("AliasForCharset"), cn)) break;
    } while (1);

    return CP;
}
#endif

class wxCharacterSet
{
public:
    wxCharacterSet(const wxChar*name)
        : cname(name) {}
    virtual ~wxCharacterSet()
        {}
    virtual size_t MB2WC(wchar_t *buf, const char *psz, size_t n)
        { return (size_t)-1; }
    virtual size_t WC2MB(char *buf, const wchar_t *psz, size_t n)
        { return (size_t)-1; }
    virtual bool usable()
        { return FALSE; }
public:
    const wxChar*cname;
};

class ID_CharSet : public wxCharacterSet
{
public:
    ID_CharSet(const wxChar *name,wxMBConv *cnv)
        : wxCharacterSet(name), work(cnv) {}

    size_t MB2WC(wchar_t *buf, const char *psz, size_t n)
        { return work ? work->MB2WC(buf,psz,n) : (size_t)-1; }

    size_t WC2MB(char *buf, const wchar_t *psz, size_t n)
        { return work ? work->WC2MB(buf,psz,n) : (size_t)-1; }

    bool usable()
        { return work!=NULL; }
public:
    wxMBConv*work;
};


#ifdef HAVE_ICONV_H

// VS: glibc 2.1.3 is broken in that iconv() conversion to/from UCS4 fails with E2BIG
//     if output buffer is _exactly_ as big as needed. Such case is (unless there's
//     yet another bug in glibc) the only case when iconv() returns with (size_t)-1
//     (which means error) and says there are 0 bytes left in the input buffer --
//     when _real_ error occurs, bytes-left-in-input buffer is non-zero. Hence,
//     this alternative test for iconv() failure.
//     [This bug does not appear in glibc 2.2.]
#if defined(__GLIBC__) && __GLIBC__ == 2 && __GLIBC_MINOR__ <= 1
#define ICONV_FAILED(cres, bufLeft) ((cres == (size_t)-1) && \
                                     (errno != E2BIG || bufLeft != 0))
#else
#define ICONV_FAILED(cres, bufLeft)  (cres == (size_t)-1)
#endif

class IC_CharSet : public wxCharacterSet
{
public:
    IC_CharSet(const wxChar *name)
        : wxCharacterSet(name)
    {
        m2w = iconv_open(WC_NAME, wxConvLibc.cWX2MB(cname));
        w2m = iconv_open(wxConvLibc.cWX2MB(cname), WC_NAME);
    }

    ~IC_CharSet()
    {
        if ( m2w != (iconv_t)-1 )
            iconv_close(m2w);
        if ( w2m != (iconv_t)-1 )
            iconv_close(w2m);
    }

    size_t MB2WC(wchar_t *buf, const char *psz, size_t n)
    {
        size_t inbuf = strlen(psz);
        size_t outbuf = n * SIZEOF_WCHAR_T;
        size_t res, cres;
        // VS: Use these instead of psz, buf because iconv() modifies its arguments:
        wchar_t *bufPtr = buf;
        const char *pszPtr = psz;

        if (buf)
        {
            // have destination buffer, convert there
#ifdef WX_ICONV_TAKES_CHAR
            cres = iconv(m2w, (char**)&pszPtr, &inbuf, (char**)&bufPtr, &outbuf);
#else
            cres = iconv(m2w, &pszPtr, &inbuf, (char**)&bufPtr, &outbuf);
#endif
            res = n - (outbuf / SIZEOF_WCHAR_T);
            // convert to native endianness
#ifdef WC_NEED_BSWAP
            WC_BSWAP(buf /* _not_ bufPtr */, res)
#endif
        }
        else
        {
            // no destination buffer... convert using temp buffer
            // to calculate destination buffer requirement
            wchar_t tbuf[8];
            res = 0;
            do {
                bufPtr = tbuf; outbuf = 8*SIZEOF_WCHAR_T;
#ifdef WX_ICONV_TAKES_CHAR
                cres = iconv( m2w, (char**)&pszPtr, &inbuf, (char**)&bufPtr, &outbuf );
#else
                cres = iconv( m2w, &pszPtr, &inbuf, (char**)&bufPtr, &outbuf );
#endif
                res += 8-(outbuf/SIZEOF_WCHAR_T);
            } while ((cres==(size_t)-1) && (errno==E2BIG));
        }

        if (ICONV_FAILED(cres, inbuf))
            return (size_t)-1;

        return res;
    }

    size_t WC2MB(char *buf, const wchar_t *psz, size_t n)
    {
#if defined(__BORLANDC__) && (__BORLANDC__ > 0x530)
        size_t inbuf = std::wcslen(psz) * SIZEOF_WCHAR_T;
#else
        size_t inbuf = ::wcslen(psz) * SIZEOF_WCHAR_T;
#endif
        size_t outbuf = n;
        size_t res, cres;

#ifdef WC_NEED_BSWAP
        // need to copy to temp buffer to switch endianness
        // this absolutely doesn't rock!
        // (no, doing WC_BSWAP twice on the original buffer won't help, as it
        //  could be in read-only memory, or be accessed in some other thread)
        wchar_t *tmpbuf=(wchar_t*)malloc((inbuf+1)*SIZEOF_WCHAR_T);
        memcpy(tmpbuf,psz,(inbuf+1)*SIZEOF_WCHAR_T);
        WC_BSWAP(tmpbuf, inbuf)
        psz=tmpbuf;
#endif
        if (buf)
        {
            // have destination buffer, convert there
#ifdef WX_ICONV_TAKES_CHAR
            cres = iconv( w2m, (char**)&psz, &inbuf, &buf, &outbuf );
#else
            cres = iconv( w2m, (const char**)&psz, &inbuf, &buf, &outbuf );
#endif
            res = n-outbuf;
        }
        else
        {
            // no destination buffer... convert using temp buffer
            // to calculate destination buffer requirement
            char tbuf[16];
            res = 0;
            do {
                buf = tbuf; outbuf = 16;
#ifdef WX_ICONV_TAKES_CHAR
                cres = iconv( w2m, (char**)&psz, &inbuf, &buf, &outbuf );
#else
                cres = iconv( w2m, (const char**)&psz, &inbuf, &buf, &outbuf );
#endif
                res += 16 - outbuf;
            } while ((cres==(size_t)-1) && (errno==E2BIG));
        }
#ifdef WC_NEED_BSWAP
        free(tmpbuf);
#endif
        if (ICONV_FAILED(cres, inbuf))
            return (size_t)-1;

        return res;
    }

    bool usable()
        { return (m2w != (iconv_t)-1) && (w2m != (iconv_t)-1); }

public:
    iconv_t m2w, w2m;
};
#endif

#ifdef __WIN32__
class CP_CharSet : public wxCharacterSet
{
public:
    CP_CharSet(const wxChar*name)
        : wxCharacterSet(name), CodePage(CharsetToCodepage(name)) {}

    size_t MB2WC(wchar_t *buf, const char *psz, size_t n)
    {
        size_t len =
            MultiByteToWideChar(CodePage, 0, psz, -1, buf, buf ? n : 0);
        //VS: returns # of written chars for buf!=NULL and *size* 
        //    needed buffer for buf==NULL
        return len ? (buf ? len : len-1) : (size_t)-1;
    }

    size_t WC2MB(char *buf, const wchar_t *psz, size_t n)
    {
        size_t len = WideCharToMultiByte(CodePage, 0, psz, -1, buf,
                                         buf ? n : 0, NULL, NULL);
        //VS: returns # of written chars for buf!=NULL and *size* 
        //    needed buffer for buf==NULL
        return len ? (buf ? len : len-1) : (size_t)-1;
    }

    bool usable()
        { return CodePage != -1; }

public:
    long CodePage;
};
#endif

class EC_CharSet : public wxCharacterSet
{
public:
    // temporarily just use wxEncodingConverter stuff,
    // so that it works while a better implementation is built
    EC_CharSet(const wxChar*name) : wxCharacterSet(name),
                                    enc(wxFONTENCODING_SYSTEM)
    {
        if (name)
            enc = wxTheFontMapper->CharsetToEncoding(name, FALSE);
        m2w.Init(enc, wxFONTENCODING_UNICODE);
        w2m.Init(wxFONTENCODING_UNICODE, enc);
    }

    size_t MB2WC(wchar_t *buf, const char *psz, size_t n)
    {
        size_t inbuf = strlen(psz);
        if (buf)
            m2w.Convert(psz,buf);
        return inbuf;
    }

    size_t WC2MB(char *buf, const wchar_t *psz, size_t n)
    {
#if defined(__BORLANDC__) && (__BORLANDC__ > 0x530)
        size_t inbuf = std::wcslen(psz);
#else
        size_t inbuf = ::wcslen(psz);
#endif
        if (buf)
            w2m.Convert(psz,buf);

        return inbuf;
    }

    bool usable()
        { return (enc!=wxFONTENCODING_SYSTEM) && (enc!=wxFONTENCODING_DEFAULT); }

public:
    wxFontEncoding enc;
    wxEncodingConverter m2w, w2m;
};

static wxCharacterSet *wxGetCharacterSet(const wxChar *name)
{
    wxCharacterSet *cset = NULL;
    if (name)
    {
        if (wxStricmp(name, wxT("UTF8")) == 0 || wxStricmp(name, wxT("UTF-8")) == 0)
        {
            cset = new ID_CharSet(name, &wxConvUTF8);
        }
        else
        {
#ifdef HAVE_ICONV_H
            cset = new IC_CharSet(name); // may not take NULL
#endif
        }
    }

    if (cset && cset->usable()) return cset;
    if (cset)
    {
        delete cset;
        cset = NULL;
    }

#ifdef __WIN32__
    cset = new CP_CharSet(name); // may take NULL
    if (cset->usable())
        return cset;

    delete cset;
#endif // __WIN32__

    cset = new EC_CharSet(name);
    if (cset->usable())
        return cset;

    delete cset;
    wxLogError(_("Unknown encoding '%s'!"), name);
    return NULL;
}

wxCSConv::wxCSConv(const wxChar *charset)
{
    m_name = (wxChar *)NULL;
    m_cset = (wxCharacterSet *) NULL;
    m_deferred = TRUE;

    SetName(charset);
}

wxCSConv::~wxCSConv()
{
    free(m_name);
    delete m_cset;
}

void wxCSConv::SetName(const wxChar *charset)
{
    if (charset)
    {
        m_name = wxStrdup(charset);
        m_deferred = TRUE;
    }
}

void wxCSConv::LoadNow()
{
    if (m_deferred)
    {
        if ( !m_name )
        {
            wxString name = wxLocale::GetSystemEncodingName();
            if ( !name.empty() )
                SetName(name);
        }

        m_cset = wxGetCharacterSet(m_name);
        m_deferred = FALSE;
    }
}

size_t wxCSConv::MB2WC(wchar_t *buf, const char *psz, size_t n) const
{
    ((wxCSConv *)this)->LoadNow(); // discard constness

    if (m_cset)
        return m_cset->MB2WC(buf, psz, n);

    // latin-1 (direct)
    size_t len = strlen(psz);

    if (buf)
    {
        for (size_t c = 0; c <= len; c++)
            buf[c] = (unsigned char)(psz[c]);
    }

    return len;
}

size_t wxCSConv::WC2MB(char *buf, const wchar_t *psz, size_t n) const
{
    ((wxCSConv *)this)->LoadNow(); // discard constness

    if (m_cset)
        return m_cset->WC2MB(buf, psz, n);

    // latin-1 (direct)
#if defined(__BORLANDC__) && (__BORLANDC__ > 0x530)
    size_t len=std::wcslen(psz);
#else
    size_t len=::wcslen(psz);
#endif
    if (buf)
    {
        for (size_t c = 0; c <= len; c++)
            buf[c] = (psz[c] > 0xff) ? '?' : psz[c];
    }

    return len;
}

#ifdef HAVE_ICONV_H

class IC_CharSetConverter
{
public:
    IC_CharSetConverter(IC_CharSet *from, IC_CharSet *to)
    {
        cnv = iconv_open(wxConvLibc.cWX2MB(to->cname),
                         wxConvLibc.cWX2MB(from->cname));
    }

    ~IC_CharSetConverter()
    {
        if (cnv != (iconv_t)-1)
            iconv_close(cnv);
    }

    size_t Convert(char *buf, const char *psz, size_t n)
    {
        size_t inbuf = strlen(psz);
        size_t outbuf = n;
#ifdef WX_ICONV_TAKES_CHAR
        size_t res = iconv( cnv, (char**)&psz, &inbuf, &buf, &outbuf );
#else
        size_t res = iconv( cnv, &psz, &inbuf, &buf, &outbuf );
#endif
        if (res == (size_t)-1)
            return (size_t)-1;
        return (n - outbuf);
    }

public:
    iconv_t cnv;
};

#endif // HAVE_ICONV_H

class EC_CharSetConverter
{
public:
    EC_CharSetConverter(EC_CharSet*from,EC_CharSet*to)
        { cnv.Init(from->enc,to->enc); }

    size_t Convert(char*buf, const char*psz, size_t n)
    {
        size_t inbuf = strlen(psz);
        if (buf) cnv.Convert(psz,buf);
        return inbuf;
    }

public:
    wxEncodingConverter cnv;
};

#else // !wxUSE_WCHAR_T

// ----------------------------------------------------------------------------
// stand-ins in absence of wchar_t
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(wxMBConv) wxConvLibc, wxConvFile;

#endif // wxUSE_WCHAR_T


