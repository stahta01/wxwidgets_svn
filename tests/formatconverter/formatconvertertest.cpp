///////////////////////////////////////////////////////////////////////////////
// Name:        tests/formatconverter/formatconverter.cpp
// Purpose:     Test wxFormatConverter
// Author:      Mike Wetherell
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Mike Wetherell
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

//
// Notes:
//
// The conversions wxFormatConverter currently does are as follows:
//
//    %s, %lS   ->  %ls
//    %S, %hS   ->  %s
//    %c, %lC   ->  %lc
//    %C, %hC   ->  %c
//
// %hs and %hc stay the same.
//
// %S and %C aren't actually in the ISO C or C++ standards, but they can be
// avoided when writing portable code.
//
// Nor are %hs or %hc in the standards, which means wxWidgets currently doesn't
// have a specifier for 'char' types that is ok for all builds and platforms.
//
// The effect of using %hs/%hc is undefined, though RTLs are quite likely
// to just ignore the 'h', so maybe it works as required even though it's
// not legal.
//
// I've put in some checks, such as this which will flag up any platforms
// where this is not the case:
//
//  CPPUNIT_ASSERT(wxString::Format(_T("%hs"), "test") == _T("test"));
//

// For compilers that support precompilation, includes "wx/wx.h".
#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using CppUnit::TestCase;
using std::string;

///////////////////////////////////////////////////////////////////////////////
// The test case
//
// wxFormatConverter only changes %s, %c, %S and %C, all others are treated
// equally, therefore it is enough to choose just one other for testing, %d
// will do.

class FormatConverterTestCase : public TestCase
{
    CPPUNIT_TEST_SUITE(FormatConverterTestCase);
        CPPUNIT_TEST(format_d);
        CPPUNIT_TEST(format_hd);
        CPPUNIT_TEST(format_ld);
        CPPUNIT_TEST(format_s);
        CPPUNIT_TEST(format_hs);
        CPPUNIT_TEST(format_ls);
        CPPUNIT_TEST(format_c);
        CPPUNIT_TEST(format_hc);
        CPPUNIT_TEST(format_lc);
        CPPUNIT_TEST(format_S);
        CPPUNIT_TEST(format_hS);
        CPPUNIT_TEST(format_lS);
        CPPUNIT_TEST(format_C);
        CPPUNIT_TEST(format_hC);
        CPPUNIT_TEST(format_lC);
        CPPUNIT_TEST(testLonger);
    CPPUNIT_TEST_SUITE_END();

    void format_d();
    void format_hd();
    void format_ld();
    void format_s();
    void format_hs();
    void format_ls();
    void format_c();
    void format_hc();
    void format_lc();

    void format_S();
    void format_hS();
    void format_lS();
    void format_C();
    void format_hC();
    void format_lC();
    void testLonger();

    void doTest(const char *input, const char *expectedScanf,
                                   const char *expectedUtf8,
                                   const char *expectedWcharUnix,
                                   const char *expectedWcharWindows);
    void check(const wxString& input, const wxString& expectedScanf,
                                      const wxString& expectedUtf8,
                                      const wxString& expectedWcharUnix,
                                      const wxString& expectedWcharWindows);
};

void FormatConverterTestCase::format_d()
{
    doTest("d", "d", "d", "d", "d");
    CPPUNIT_ASSERT(wxString::Format(_T("%d"), 255) == _T("255"));
    CPPUNIT_ASSERT(wxString::Format(_T("%05d"), 255) == _T("00255"));
    CPPUNIT_ASSERT(wxString::Format(_T("% 5d"), 255) == _T("  255"));
    CPPUNIT_ASSERT(wxString::Format(_T("% 5d"), -255) == _T(" -255"));
    CPPUNIT_ASSERT(wxString::Format(_T("%-5d"), -255) == _T("-255 "));
    CPPUNIT_ASSERT(wxString::Format(_T("%+5d"), 255) == _T(" +255"));
    CPPUNIT_ASSERT(wxString::Format(_T("%*d"), 5, 255) == _T("  255"));
}

void FormatConverterTestCase::format_hd()
{
    doTest("hd", "hd", "hd", "hd", "hd");
    short s = 32767;
    CPPUNIT_ASSERT(wxString::Format(_T("%hd"), s) == _T("32767"));
}

void FormatConverterTestCase::format_ld()
{
    doTest("ld", "ld", "ld", "ld", "ld");
    long l = 2147483647L;
    CPPUNIT_ASSERT(wxString::Format(_T("%ld"), l) == _T("2147483647"));
}

void FormatConverterTestCase::format_s()
{
    doTest("s", "ls", "s", "ls", "s");
    CPPUNIT_ASSERT(wxString::Format(_T("%s!"), _T("test")) == _T("test!"));
    CPPUNIT_ASSERT(wxString::Format(_T("%6s!"), _T("test")) == _T("  test!"));
    CPPUNIT_ASSERT(wxString::Format(_T("%-6s!"), _T("test")) == _T("test  !"));
    CPPUNIT_ASSERT(wxString::Format(_T("%.6s!"), _T("test")) == _T("test!"));
    CPPUNIT_ASSERT(wxString::Format(_T("%6.4s!"), _T("testing")) == _T("  test!"));
}

void FormatConverterTestCase::format_hs()
{
    doTest("hs", "hs", "s", "ls", "s");
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%hs!")), "test") == _T("test!"));
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%6hs!")), "test") == _T("  test!"));
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%-6hs!")), "test") == _T("test  !"));
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%.6hs!")), "test") == _T("test!"));
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%6.4hs!")), "testing") == _T("  test!"));
}

void FormatConverterTestCase::format_ls()
{
    doTest("ls", "ls", "s", "ls", "s");
    CPPUNIT_ASSERT(wxString::Format(_T("%ls!"), L"test") == _T("test!"));
    CPPUNIT_ASSERT(wxString::Format(_T("%6ls!"), L"test") == _T("  test!"));
    CPPUNIT_ASSERT(wxString::Format(_T("%-6ls!"), L"test") == _T("test  !"));
    CPPUNIT_ASSERT(wxString::Format(_T("%.6ls!"), L"test") == _T("test!"));
    CPPUNIT_ASSERT(wxString::Format(_T("%6.4ls!"), L"testing") == _T("  test!"));
}

void FormatConverterTestCase::format_c()
{
    doTest("c", "lc", "s", "lc", "c");
    CPPUNIT_ASSERT(wxString::Format(_T("%c"), _T('x')) == _T("x"));
    CPPUNIT_ASSERT(wxString::Format(_T("%2c"), _T('x')) == _T(" x"));
    CPPUNIT_ASSERT(wxString::Format(_T("%-2c"), _T('x')) == _T("x "));
}

void FormatConverterTestCase::format_hc()
{
    doTest("hc", "hc", "s", "lc", "c");
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%hc")), 'x') == _T("x"));
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%2hc")), 'x') == _T(" x"));
    CPPUNIT_ASSERT(wxString::Format(wxString(_T("%-2hc")), 'x') == _T("x "));
}

void FormatConverterTestCase::format_lc()
{
    doTest("lc", "lc", "s", "lc", "c");
    CPPUNIT_ASSERT(wxString::Format(_T("%lc"), L'x') == _T("x"));
    CPPUNIT_ASSERT(wxString::Format(_T("%2lc"), L'x') == _T(" x"));
    CPPUNIT_ASSERT(wxString::Format(_T("%-2lc"), L'x') == _T("x "));
}


void FormatConverterTestCase::format_S()
    { doTest("S",  "s", "s", "ls", "s");  }
void FormatConverterTestCase::format_hS()
    { doTest("hS", "s", "s", "ls", "s");  }
void FormatConverterTestCase::format_lS()
    { doTest("lS", "ls", "s", "ls", "s"); }

void FormatConverterTestCase::format_C()
    { doTest("C",  "c", "s", "lc", "c");  }
void FormatConverterTestCase::format_hC()
    { doTest("hC", "c", "s", "lc", "c");  }
void FormatConverterTestCase::format_lC()
    { doTest("lC", "lc", "s", "lc", "c"); }

// It's possible that although a format converts correctly alone, it leaves
// the converter in a bad state that will affect subsequent formats, so
// check with a selection of longer patterns.
//
void FormatConverterTestCase::testLonger()
{
    struct {
        const char *input;
        const char *expectedScanf;
        const char *expectedWcharUnix;
        const char *expectedWcharWindows;
        const char *expectedUtf8;
    } formats[] = {
        { "%d",     "%d",     "%d",     "%d",    "%d"    },
        { "%*hd",   "%*hd",   "%*hd",   "%*hd",  "%*hd"  },
        { "%.4ld",  "%.4ld",  "%.4ld",  "%.4ld", "%.4ld" },
        { "%-.*s",  "%-.*ls", "%-.*ls", "%-.*s", "%-.*s" },
        { "%.*hs",  "%.*hs",  "%.*ls",  "%.*s",  "%.*s"  },
        { "%-.9ls", "%-.9ls", "%-.9ls", "%-.9s", "%-.9s" },
        { "%-*c",   "%-*lc",  "%-*lc",  "%-*c",  "%-*s"  },
        { "%3hc",   "%3hc",   "%3lc",   "%3c",   "%3s"   },
        { "%-5lc",  "%-5lc",  "%-5lc",  "%-5c",  "%-5s"  }
    };
    size_t i, j;

    // test all possible pairs of the above patterns
    for (i = 0; i < WXSIZEOF(formats); i++) {
        if (formats[i].input) {
            wxString input(formats[i].input);
            wxString expectedScanf(formats[i].expectedScanf);
            wxString expectedUtf8(formats[i].expectedUtf8);
            wxString expectedWcharUnix(formats[i].expectedWcharUnix);
            wxString expectedWcharWindows(formats[i].expectedWcharWindows);

            for (j = 0; j < WXSIZEOF(formats); j++)
                if (formats[j].input)
                    check(input + formats[j].input,
                          expectedScanf + formats[j].expectedScanf,
                          expectedUtf8 + formats[j].expectedUtf8,
                          expectedWcharUnix + formats[j].expectedWcharUnix,
                          expectedWcharWindows + formats[j].expectedWcharWindows);
        }
    }
}

void FormatConverterTestCase::doTest(const char *input,
                                     const char *expectedScanf,
                                     const char *expectedUtf8,
                                     const char *expectedWcharUnix,
                                     const char *expectedWcharWindows)
{
    static const wxChar *flag_width[] =
        { _T(""), _T("*"), _T("10"), _T("-*"), _T("-10"), NULL };
    static const wxChar *precision[] =
        { _T(""), _T(".*"), _T(".10"), NULL };
    static const wxChar *empty[] =
        { _T(""), NULL };

    // no precision for %c or %C
    const wxChar **precs = wxTolower(input[wxStrlen(input)-1]) == _T('c') ?
        empty : precision;

    wxString fmt(_T("%"));

    // try the test for a variety of combinations of flag, width and precision
    for (const wxChar **prec = precs; *prec; prec++)
        for (const wxChar **width = flag_width; *width; width++)
            check(fmt + *width + *prec + input,
                  fmt + *width + *prec + expectedScanf,
                  fmt + *width + *prec + expectedUtf8,
                  fmt + *width + *prec + expectedWcharUnix,
                  fmt + *width + *prec + expectedWcharWindows);
}

void FormatConverterTestCase::check(const wxString& input,
                                    const wxString& expectedScanf,
                                    const wxString& expectedUtf8,
                                    const wxString& expectedWcharUnix,
                                    const wxString& expectedWcharWindows)
{
    // all of them are unused in some build configurations
    wxUnusedVar(expectedScanf);
    wxUnusedVar(expectedUtf8);
    wxUnusedVar(expectedWcharUnix);
    wxUnusedVar(expectedWcharWindows);

    wxString result, msg;

#ifndef __WINDOWS__
    // on windows, wxScanf() string needs no modifications
    result = wxScanfConvertFormatW(input.wc_str());

    msg = _T("input: '") + input +
          _T("', result (scanf): '") + result +
          _T("', expected: '") + expectedScanf + _T("'");
    CPPUNIT_ASSERT_MESSAGE(string(msg.mb_str()), result == expectedScanf);
#endif // !__WINDOWS__

#if wxUSE_UNICODE_UTF8
    result = (const char*)wxFormatString(input);

    msg = _T("input: '") + input +
          _T("', result (UTF-8): '") + result +
          _T("', expected: '") + expectedUtf8 + _T("'");
    CPPUNIT_ASSERT_MESSAGE(string(msg.mb_str()), result == expectedUtf8);
#endif // wxUSE_UNICODE_UTF8

#if wxUSE_UNICODE && !wxUSE_UTF8_LOCALE_ONLY
    result = (const wchar_t*)wxFormatString(input);

#ifdef __WINDOWS__
    wxString expectedWchar(expectedWcharWindows);
#else
    wxString expectedWchar(expectedWcharUnix);
#endif

    msg = _T("input: '") + input +
          _T("', result (wchar_t): '") + result +
          _T("', expected: '") + expectedWchar + _T("'");
    CPPUNIT_ASSERT_MESSAGE(string(msg.mb_str()), result == expectedWchar);
#endif // wxUSE_UNICODE && !wxUSE_UTF8_LOCALE_ONLY
}


// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION(FormatConverterTestCase);

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FormatConverterTestCase,
                                      "FormatConverterTestCase");

