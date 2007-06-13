///////////////////////////////////////////////////////////////////////////////
// Name:        tests/strings/crt.cpp
// Purpose:     Test for wx C runtime functions wrappers
// Author:      Vaclav Slavik
// Created:     2004-06-03
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Vaclav Slavik 
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

#include "wx/textfile.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const char *strMB = "hello, world";
static const wchar_t *strWC = L"hello, world";
static const wxString strWX("hello, world");

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class CrtTestCase : public CppUnit::TestCase
{
public:
    CrtTestCase() {}

private:
    CPPUNIT_TEST_SUITE( CrtTestCase );
        CPPUNIT_TEST( SetGetEnv );
        CPPUNIT_TEST( Strcmp );
        CPPUNIT_TEST( Strspn );
        CPPUNIT_TEST( Strcspn );
    CPPUNIT_TEST_SUITE_END();

    void SetGetEnv();
    void Strcmp();
    void Strspn();
    void Strcspn();

    DECLARE_NO_COPY_CLASS(CrtTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( CrtTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( CrtTestCase, "CrtTestCase" );

void CrtTestCase::SetGetEnv()
{
    wxString val;
    wxSetEnv(_T("TESTVAR"), _T("value"));
    CPPUNIT_ASSERT( wxGetEnv(_T("TESTVAR"), &val) == true );
    CPPUNIT_ASSERT( val == _T("value") );
    wxSetEnv(_T("TESTVAR"), _T("something else"));
    CPPUNIT_ASSERT( wxGetEnv(_T("TESTVAR"), &val) );
    CPPUNIT_ASSERT( val == _T("something else") );
    CPPUNIT_ASSERT( wxUnsetEnv(_T("TESTVAR")) );
    CPPUNIT_ASSERT( wxGetEnv(_T("TESTVAR"), NULL) == false );
}

void CrtTestCase::Strcmp()
{
    // this code tests if all possible ways of calling wxStrcmp() compile:
    const char * const char1 = "first";
    const wchar_t * const wchar1 = L"first";
    wxString str1("first");
    wxCStrData cstr1(str1.c_str());
    wxCharBuffer charbuf1(char1);
    wxWCharBuffer wcharbuf1(wchar1);

    const char * const char2 = "last";
    const wchar_t * const wchar2 = L"last";
    wxString str2("last");
    wxCStrData cstr2(str2.c_str());
    wxCharBuffer charbuf2(char2);
    wxWCharBuffer wcharbuf2(wchar2);

    CPPUNIT_ASSERT( wxStrcmp(char1, char2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(char1, wchar2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(char1, str2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(char1, cstr2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(char1, charbuf2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(char1, wcharbuf2) < 0 );

    CPPUNIT_ASSERT( wxStrcmp(wchar1, char2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wchar1, wchar2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wchar1, str2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wchar1, cstr2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wchar1, charbuf2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wchar1, wcharbuf2) < 0 );

    CPPUNIT_ASSERT( wxStrcmp(str1, char2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(str1, wchar2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(str1, str2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(str1, cstr2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(str1, charbuf2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(str1, wcharbuf2) < 0 );

    CPPUNIT_ASSERT( wxStrcmp(cstr1, char2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(cstr1, wchar2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(cstr1, str2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(cstr1, cstr2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(cstr1, charbuf2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(cstr1, wcharbuf2) < 0 );

    CPPUNIT_ASSERT( wxStrcmp(charbuf1, char2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(charbuf1, wchar2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(charbuf1, str2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(charbuf1, cstr2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(charbuf1, charbuf2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(charbuf1, wcharbuf2) < 0 );

    CPPUNIT_ASSERT( wxStrcmp(wcharbuf1, char2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wcharbuf1, wchar2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wcharbuf1, str2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wcharbuf1, cstr2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wcharbuf1, charbuf2) < 0 );
    CPPUNIT_ASSERT( wxStrcmp(wcharbuf1, wcharbuf2) < 0 );
}

void CrtTestCase::Strspn()
{
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strMB, "xyz") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strWC, "xyz") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strWX, "xyz") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strMB, L"xyz") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strWC, L"xyz") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strWX, L"xyz") );

    CPPUNIT_ASSERT_EQUAL( (size_t)5, wxStrspn(strMB, "hleo") );
    CPPUNIT_ASSERT_EQUAL( (size_t)5, wxStrspn(strWC, "hleo") );
    CPPUNIT_ASSERT_EQUAL( (size_t)5, wxStrspn(strWX, "hleo") );

    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strMB, "ld") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strWC, "ld") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrspn(strWX, "ld") );

    CPPUNIT_ASSERT_EQUAL( strWX.length(), wxStrspn(strMB, strWC) );
    CPPUNIT_ASSERT_EQUAL( strWX.length(), wxStrspn(strWC, strWX) );
    CPPUNIT_ASSERT_EQUAL( strWX.length(), wxStrspn(strWX, strMB) );
}

void CrtTestCase::Strcspn()
{
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrcspn(strMB, strWX) );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrcspn(strWC, strMB) );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrcspn(strWX, strWC) );

    CPPUNIT_ASSERT_EQUAL( (size_t)5, wxStrcspn(strMB, ", ") );
    CPPUNIT_ASSERT_EQUAL( (size_t)5, wxStrcspn(strWC, ", ") );
    CPPUNIT_ASSERT_EQUAL( (size_t)5, wxStrcspn(strWX, ", ") );

    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrcspn(strMB, "hel") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrcspn(strWC, "hel") );
    CPPUNIT_ASSERT_EQUAL( (size_t)0, wxStrcspn(strWX, "hel") );

    CPPUNIT_ASSERT_EQUAL( strWX.length(), wxStrcspn(strMB, "xy") );
    CPPUNIT_ASSERT_EQUAL( strWX.length(), wxStrcspn(strWC, "xy") );
    CPPUNIT_ASSERT_EQUAL( strWX.length(), wxStrcspn(strWX, "xy") );
}

