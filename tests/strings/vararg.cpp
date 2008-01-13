///////////////////////////////////////////////////////////////////////////////
// Name:        tests/strings/vararg.cpp
// Purpose:     Test for wx vararg look-alike macros
// Author:      Vaclav Slavik
// Created:     2007-02-20
// RCS-ID:      $Id$
// Copyright:   (c) 2007 REA Elektronik GmbH
// Licence:     wxWindows licence
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

#include "wx/string.h"

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class VarArgTestCase : public CppUnit::TestCase
{
public:
    VarArgTestCase() {}

private:
    CPPUNIT_TEST_SUITE( VarArgTestCase );
        CPPUNIT_TEST( StringPrintf );
        CPPUNIT_TEST( CharPrintf );
#if wxUSE_STD_STRING
        CPPUNIT_TEST( StdString );
#endif
        CPPUNIT_TEST( Sscanf );
    CPPUNIT_TEST_SUITE_END();

    void StringPrintf();
    void CharPrintf();
#if wxUSE_STD_STRING
    void StdString();
#endif
    void Sscanf();

    DECLARE_NO_COPY_CLASS(VarArgTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( VarArgTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( VarArgTestCase, "VarArgTestCase" );

void VarArgTestCase::StringPrintf()
{
    wxString s, s2;

    // test passing literals:
    s.Printf("%s %i", "foo", 42);
    CPPUNIT_ASSERT( s == "foo 42" );
    s.Printf("%s %s %i", _T("bar"), "=", 11);

    // test passing c_str():
    CPPUNIT_ASSERT( s == "bar = 11" );
    s2.Printf("(%s)", s.c_str());
    CPPUNIT_ASSERT( s2 == "(bar = 11)" );
    s2.Printf(_T("[%s](%s)"), s.c_str(), "str");
    CPPUNIT_ASSERT( s2 == "[bar = 11](str)" );

    s2.Printf("%s mailbox", wxString("Opening").c_str());
    CPPUNIT_ASSERT( s2 == "Opening mailbox" );

    // test passing wxString directly:
    s2.Printf(_T("[%s](%s)"), s, "str");
    CPPUNIT_ASSERT( s2 == "[bar = 11](str)" );

    // test passing wxCharBufferType<T>:
    s = "FooBar";
    s2.Printf(_T("(%s)"), s.mb_str());
    CPPUNIT_ASSERT( s2 == "(FooBar)" );
    s2.Printf(_T("value=%s;"), s.wc_str());
    CPPUNIT_ASSERT( s2 == "value=FooBar;" );

    // this tests correct passing of wxCStrData constructed from string
    // literal:
    bool cond = true;
    s2.Printf(_T("foo %s"), !cond ? s.c_str() : _T("bar"));
}

void VarArgTestCase::CharPrintf()
{
    wxString foo("foo");
    wxString s;

    // test using wchar_t:
    s.Printf("char=%c", L'c');
    WX_ASSERT_STR_EQUAL( "char=c", s );

    // test wxUniCharRef:
    s.Printf("string[1] is %c", foo[1]);
    WX_ASSERT_STR_EQUAL( "string[1] is o", s );

    // test char
    char c = 'z';
    s.Printf("%c to %c", 'a', c);
    WX_ASSERT_STR_EQUAL( "a to z", s );

    // test char used as integer:
    #ifdef _MSC_VER
        #pragma warning(disable:4305) // truncation of constant value in VC6
        #pragma warning(disable:4309) // truncation of constant value
    #endif
    c = 240;
    #ifdef _MSC_VER
        #pragma warning(default:4305) // truncation of constant value in VC6
        #pragma warning(default:4309)
    #endif
    s.Printf("value is %i (int)", c);
    WX_ASSERT_STR_EQUAL( wxString("value is -16 (int)"), s );

    unsigned char u = 240;
    s.Printf("value is %i (int)", u);
    WX_ASSERT_STR_EQUAL( wxString("value is 240 (int)"), s );
}

#if wxUSE_STD_STRING
void VarArgTestCase::StdString()
{
    // test passing std::[w]string
    wxString s;

    std::string mb("multi-byte");
    std::string wc("widechar");

    s.Printf("string %s(%i).", mb, 1);
    CPPUNIT_ASSERT( s == "string multi-byte(1)." );

    s.Printf("string %s(%i).", wc, 2);
    CPPUNIT_ASSERT( s == "string widechar(2)." );
}
#endif // wxUSE_STD_STRING

void VarArgTestCase::Sscanf()
{
    int i = 0;
    char str[20];
    wchar_t wstr[20];

    wxString input("42 test");

    wxSscanf(input, "%d %s", &i, &str);
    CPPUNIT_ASSERT( i == 42 );
    CPPUNIT_ASSERT( wxStrcmp(str, "test") == 0 );

    i = 0;
    wxSscanf(input, L"%d %s", &i, &wstr);
    CPPUNIT_ASSERT( i == 42 );
    CPPUNIT_ASSERT( wxStrcmp(wstr, "test") == 0 );
}
