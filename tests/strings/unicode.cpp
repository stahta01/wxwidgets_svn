///////////////////////////////////////////////////////////////////////////////
// Name:        tests/strings/unicode.cpp
// Purpose:     Unicode unit test
// Author:      Vadim Zeitlin, Wlodzimierz ABX Skiba
// Created:     2004-04-28
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Vadim Zeitlin, Wlodzimierz Skiba
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif // WX_PRECOMP

// helper class holding the matching MB and WC strings
//
// either str or wcs (but not both) may be NULL, this means that the conversion
// to it should fail
struct StringConversionData
{
    const char *str;
    const wchar_t *wcs;

    enum
    {
        TEST_BOTH  = 0, // test both str -> wcs and wcs -> str
        ONLY_MB2WC = 1  // only test str -> wcs conversion
    };

    int flags;

    // test that the conversion between str and wcs (subject to flags) succeeds
    //
    // the first argument is the index in the test array and is used solely for
    // diagnostics
    void Test(size_t n, wxMBConv& conv) const
    {
        if ( str )
        {
            wxWCharBuffer wbuf = conv.cMB2WC(str);

            if ( wcs )
            {
                CPPUNIT_ASSERT_MESSAGE
                (
                    Message(n, "MB2WC failed"),
                    wbuf.data()
                );

                CPPUNIT_ASSERT_MESSAGE
                (
                    Message(n, "MB2WC", wbuf, wcs),
                    wxStrcmp(wbuf, wcs) == 0
                );
            }
            else // conversion is supposed to fail
            {
                CPPUNIT_ASSERT_MESSAGE
                (
                    Message(n, "MB2WC succeeded"),
                    !wbuf.data()
                );
            }
        }

        if ( wcs && !(flags & ONLY_MB2WC) )
        {
            wxCharBuffer buf = conv.cWC2MB(wcs);

            if ( str )
            {
                CPPUNIT_ASSERT_MESSAGE
                (
                    Message(n, "WC2MB failed"),
                    buf.data()
                );

                CPPUNIT_ASSERT_MESSAGE
                (
                    Message(n, "WC2MB", buf, str),
                    strcmp(buf, str) == 0
                );
            }
            else
            {
                CPPUNIT_ASSERT_MESSAGE
                (
                    Message(n, "WC2MB succeeded"),
                    !buf.data()
                );
            }
        }
    }

private:
    static std::string
    Message(size_t n, const wxString& msg)
    {
        return std::string(wxString::Format("#%lu: %s", (unsigned long)n, msg));
    }

    template <typename T>
    static std::string
    Message(size_t n,
            const char *func,
            const wxCharTypeBuffer<T>& actual,
            const T *expected)
    {
        return Message(n,
                       wxString::Format("%s returned \"%s\", expected \"%s\"",
                                        func, actual.data(), expected));
    }
};

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class UnicodeTestCase : public CppUnit::TestCase
{
public:
    UnicodeTestCase();

private:
    CPPUNIT_TEST_SUITE( UnicodeTestCase );
        CPPUNIT_TEST( ToFromAscii );
        CPPUNIT_TEST( ConstructorsWithConversion );
        CPPUNIT_TEST( ConversionEmpty );
        CPPUNIT_TEST( ConversionWithNULs );
        CPPUNIT_TEST( ConversionUTF7 );
        CPPUNIT_TEST( ConversionUTF8 );
        CPPUNIT_TEST( ConversionUTF16 );
        CPPUNIT_TEST( ConversionUTF32 );
        CPPUNIT_TEST( IsConvOk );
#if wxUSE_UNICODE
        CPPUNIT_TEST( Iteration );
#endif
    CPPUNIT_TEST_SUITE_END();

    void ToFromAscii();
    void ConstructorsWithConversion();
    void ConversionEmpty();
    void ConversionWithNULs();
    void ConversionUTF7();
    void ConversionUTF8();
    void ConversionUTF16();
    void ConversionUTF32();
    void IsConvOk();
#if wxUSE_UNICODE
    void Iteration();
#endif

    DECLARE_NO_COPY_CLASS(UnicodeTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( UnicodeTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( UnicodeTestCase, "UnicodeTestCase" );

UnicodeTestCase::UnicodeTestCase()
{
}

void UnicodeTestCase::ToFromAscii()
{

#define TEST_TO_FROM_ASCII(txt)                              \
    {                                                        \
        static const char *msg = txt;                        \
        wxString s = wxString::FromAscii(msg);               \
        CPPUNIT_ASSERT( strcmp( s.ToAscii() , msg ) == 0 );  \
    }

    TEST_TO_FROM_ASCII( "Hello, world!" );
    TEST_TO_FROM_ASCII( "additional \" special \t test \\ component \n :-)" );
}

void UnicodeTestCase::ConstructorsWithConversion()
{
    // the string "D�j�" in UTF-8 and wchar_t:
    const unsigned char utf8Buf[] = {0x44,0xC3,0xA9,0x6A,0xC3,0xA0,0};
    const unsigned char utf8subBuf[] = {0x44,0xC3,0xA9,0x6A,0}; // just "D�j"
    const char *utf8 = (char *)utf8Buf;
    const char *utf8sub = (char *)utf8subBuf;

    wxString s1(utf8, wxConvUTF8);

#if wxUSE_UNICODE
    const wchar_t wchar[] = {0x44,0xE9,0x6A,0xE0,0};
    WX_ASSERT_STR_EQUAL( wchar, s1 );

    wxString s2(wchar);
    WX_ASSERT_STR_EQUAL( wchar, s2 );
    WX_ASSERT_STR_EQUAL( utf8, s2 );
#else
    WX_ASSERT_STR_EQUAL( utf8, s1 );
#endif

    wxString sub(utf8sub, wxConvUTF8); // "Dej" substring
    wxString s3(utf8, wxConvUTF8, 4);
    CPPUNIT_ASSERT_EQUAL( sub, s3 );

#if wxUSE_UNICODE
    wxString s4(wchar, wxConvUTF8, 3);
    CPPUNIT_ASSERT_EQUAL( sub, s4 );

    // conversion should stop with failure at pos 35
    wxString s("\t[pl]open.format.Sformatuj dyskietk�=gfloppy %f", wxConvUTF8);
    CPPUNIT_ASSERT( s.empty() );
#endif // wxUSE_UNICODE


    // test using Unicode strings together with char* strings (this must work
    // in ANSI mode as well, of course):
    wxString s5("ascii");
    WX_ASSERT_STR_EQUAL( "ascii", s5 );

    s5 += " value";

    CPPUNIT_ASSERT( strcmp(s5.mb_str(), "ascii value") == 0 );
    WX_ASSERT_STR_EQUAL( "ascii value", s5 );
    CPPUNIT_ASSERT( s5 != "SomethingElse" );
}

void UnicodeTestCase::ConversionEmpty()
{
    size_t len;

#if wxUSE_UNICODE
    wxCharBuffer buf = wxConvLibc.cWC2MB(L"", 0, &len);
#else // !wxUSE_UNICODE
    wxWCharBuffer wbuf = wxConvLibc.cMB2WC("", 0, &len);
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

    CPPUNIT_ASSERT(len == 0);
}

void UnicodeTestCase::ConversionWithNULs()
{
#if wxUSE_UNICODE
    static const size_t lenNulString = 10;

    wxString szTheString(L"The\0String", wxConvLibc, lenNulString);
    wxCharBuffer theBuffer = szTheString.mb_str();

    CPPUNIT_ASSERT( memcmp(theBuffer.data(), "The\0String",
                    lenNulString + 1) == 0 );

    wxString szTheString2("The\0String", wxConvLocal, lenNulString);
    CPPUNIT_ASSERT_EQUAL( lenNulString, szTheString2.length() );
    CPPUNIT_ASSERT( wxTmemcmp(szTheString2.c_str(), L"The\0String",
                    lenNulString + 1) == 0 );
#else // !wxUSE_UNICODE
    wxString szTheString("TheString");
    szTheString.insert(3, 1, '\0');
    wxWCharBuffer theBuffer = szTheString.wc_str(wxConvLibc);

    CPPUNIT_ASSERT( memcmp(theBuffer.data(), L"The\0String", 11 * sizeof(wchar_t)) == 0 );

    wxString szLocalTheString("TheString");
    szLocalTheString.insert(3, 1, '\0');
    wxWCharBuffer theLocalBuffer = szLocalTheString.wc_str(wxConvLocal);

    CPPUNIT_ASSERT( memcmp(theLocalBuffer.data(), L"The\0String", 11 * sizeof(wchar_t)) == 0 );
#endif // wxUSE_UNICODE/!wxUSE_UNICODE
}

void UnicodeTestCase::ConversionUTF7()
{
    static const StringConversionData utf7data[] =
    {
        // normal fragments
        { "+AKM-", L"\xa3" },
        { "+AOk-t+AOk-", L"\xe9t\xe9" },

        // this one is an alternative valid encoding of the same string
        { "+AOk-t+AOk", L"\xe9t\xe9", StringConversionData::ONLY_MB2WC },

        // some special cases
        { "+-", L"+" },
        { "+--", L"+-" },

        // the following are invalid UTF-7 sequences
        { "\xa3", NULL },
        { "+", NULL },
        { "+~", NULL },
        { "a+", NULL },
    };

    for ( size_t n = 0; n < WXSIZEOF(utf7data); n++ )
    {
        const StringConversionData& d = utf7data[n];

        // converting to/from UTF-7 using iconv() currently doesn't work
        // because of several problems:
        //  - GetMBNulLen() doesn't return correct result (iconv converts L'\0'
        //    to an incomplete and anyhow nonsensical "+AA" string)
        //  - iconv refuses to convert "+-" (although it converts "+-\n" just
        //    fine, go figure)
        //
        // I have no idea how to fix this so just disable the test for now
#if 0
        d.Test(n, wxCSConv("utf-7"));
#endif
        d.Test(n, wxConvUTF7);
    }
}

void UnicodeTestCase::ConversionUTF8()
{
    static const StringConversionData utf8data[] =
    {
#ifdef wxHAVE_U_ESCAPE
        { "\xc2\xa3", L"\u00a3" },
#endif
        { "\xc2", NULL },
    };

    wxCSConv conv(_T("utf-8"));
    for ( size_t n = 0; n < WXSIZEOF(utf8data); n++ )
    {
        const StringConversionData& d = utf8data[n];
        d.Test(n, conv);
        d.Test(n, wxConvUTF8);
    }
}

void UnicodeTestCase::ConversionUTF16()
{
    static const StringConversionData utf16data[] =
    {
#ifdef wxHAVE_U_ESCAPE
        { "\x04\x1f\x04\x40\x04\x38\x04\x32\x04\x35\x04\x42\0\0",
          L"\u041f\u0440\u0438\u0432\u0435\u0442" },
        { "\x01\0\0b\x01\0\0a\x01\0\0r\0\0", L"\u0100b\u0100a\u0100r" },
#endif
        { "\0f\0o\0o\0\0", L"foo" },
    };

    wxCSConv conv(wxFONTENCODING_UTF16BE);
    for ( size_t n = 0; n < WXSIZEOF(utf16data); n++ )
    {
        const StringConversionData& d = utf16data[n];
        d.Test(n, conv);
    }

    // special case: this string has consecutive NULs inside it which don't
    // terminate the string, this exposed a bug in our conversion code which
    // got confused in this case
    size_t len;
    wxWCharBuffer wbuf(conv.cMB2WC("\x01\0\0B\0C" /* A macron BC */, 6, &len));
    CPPUNIT_ASSERT_EQUAL( (size_t)3, len );
}

void UnicodeTestCase::ConversionUTF32()
{
    static const StringConversionData utf32data[] =
    {
#ifdef wxHAVE_U_ESCAPE
        {
            "\0\0\x04\x1f\0\0\x04\x40\0\0\x04\x38\0\0\x04\x32\0\0\x04\x35\0\0\x04\x42\0\0\0\0",
          L"\u041f\u0440\u0438\u0432\u0435\u0442" },
#endif
        { "\0\0\0f\0\0\0o\0\0\0o\0\0\0\0", L"foo" },
    };

    wxCSConv conv(wxFONTENCODING_UTF32BE);
    for ( size_t n = 0; n < WXSIZEOF(utf32data); n++ )
    {
        const StringConversionData& d = utf32data[n];
        d.Test(n, conv);
    }

    size_t len;
    wxWCharBuffer wbuf(conv.cMB2WC("\0\0\x01\0\0\0\0B\0\0\0C" /* A macron BC */,
                                   12, &len));
    CPPUNIT_ASSERT_EQUAL( (size_t)3, len );
}

void UnicodeTestCase::IsConvOk()
{
    CPPUNIT_ASSERT( wxCSConv(wxFONTENCODING_SYSTEM).IsOk() );
    CPPUNIT_ASSERT( wxCSConv(_T("UTF-8")).IsOk() );
    CPPUNIT_ASSERT( !wxCSConv(_T("NoSuchConversion")).IsOk() );

#ifdef __WINDOWS__
    CPPUNIT_ASSERT( wxCSConv(_T("WINDOWS-437")).IsOk() );
#endif
}

#if wxUSE_UNICODE
void UnicodeTestCase::Iteration()
{
    // "czech" in Czech ("cestina"):
    static const char *textUTF8 = "\304\215e\305\241tina";
    static const wchar_t textUTF16[] = {0x10D, 0x65, 0x161, 0x74, 0x69, 0x6E, 0x61, 0};

    wxString text(wxString::FromUTF8(textUTF8));
    CPPUNIT_ASSERT( wxStrcmp(text.wc_str(), textUTF16) == 0 );

    // verify the string was decoded correctly:
    {
        size_t idx = 0;
        for ( wxString::const_iterator i = text.begin(); i != text.end(); ++i, ++idx )
        {
            CPPUNIT_ASSERT( *i == textUTF16[idx] );
        }
    }

    // overwrite the string with something that is shorter in UTF-8:
    {
        for ( wxString::iterator i = text.begin(); i != text.end(); ++i )
            *i = 'x';
    }

    // restore the original text now:
    {
        wxString::iterator end1 = text.end();
        wxString::const_iterator end2 = text.end();

        size_t idx = 0;
        for ( wxString::iterator i = text.begin(); i != text.end(); ++i, ++idx )
        {
            *i = textUTF16[idx];

            CPPUNIT_ASSERT( end1 == text.end() );
            CPPUNIT_ASSERT( end2 == text.end() );
        }

        CPPUNIT_ASSERT( end1 == text.end() );
        CPPUNIT_ASSERT( end2 == text.end() );
    }

    // and verify it again:
    {
        size_t idx = 0;
        for ( wxString::const_iterator i = text.begin(); i != text.end(); ++i, ++idx )
        {
            CPPUNIT_ASSERT( *i == textUTF16[idx] );
        }
    }
}
#endif // wxUSE_UNICODE
