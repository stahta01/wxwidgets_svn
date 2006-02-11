///////////////////////////////////////////////////////////////////////////////
// Name:        tests/uris/uris.cpp
// Purpose:     wxTextXXXStream unit test
// Author:      Ryan Norton
// Created:     2004-08-14
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Ryan Norton
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

#include "wx/txtstrm.h"
#include "wx/wfstream.h"

#if wxUSE_LONGLONG
#include "wx/longlong.h"
#endif

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class TextStreamTestCase : public CppUnit::TestCase
{
public:
    TextStreamTestCase();

private:
    CPPUNIT_TEST_SUITE( TextStreamTestCase );
        CPPUNIT_TEST( Endline );
#if wxUSE_LONGLONG
        CPPUNIT_TEST( TestLongLong );
        CPPUNIT_TEST( TestLongLong );
#endif
    CPPUNIT_TEST_SUITE_END();

    void Endline();
#if wxUSE_LONGLONG
    void TestLongLong();
    void TestULongLong();
#endif // wxUSE_LONGLONG


    DECLARE_NO_COPY_CLASS(TextStreamTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( TextStreamTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TextStreamTestCase, "TextStreamTestCase" );

TextStreamTestCase::TextStreamTestCase()
{
}

#if defined(__WXMSW__) || defined(__WXPM__)
#   define NEWLINE "\r\n"
#   define NEWLINELEN 2
#elif defined(__WXMAC__) && !defined(__DARWIN__)
#   define NEWLINE "\r"
#   define NEWLINELEN 1
#else
#   define NEWLINE "\n"
#   define NEWLINELEN 1
#endif

void TextStreamTestCase::Endline()
{
    wxFileOutputStream* pOutFile = new wxFileOutputStream(_T("test.txt"));
    wxTextOutputStream* pOutText = new wxTextOutputStream(*pOutFile);
    *pOutText   << _T("Test text") << endl 
                << _T("More Testing Text (There should be newline before this)");

    delete pOutText;
    delete pOutFile;

    wxFileInputStream* pInFile = new wxFileInputStream(_T("test.txt"));

    char szIn[9 + NEWLINELEN];

    pInFile->Read(szIn, 9 + NEWLINELEN);

    CPPUNIT_ASSERT( memcmp(&szIn[9], NEWLINE, NEWLINELEN) == 0 );

    delete pInFile;
}

#if wxUSE_LONGLONG

template <typename T>
static void DoTestRoundTrip(const T *values, size_t numValues)
{
    {
        wxFileOutputStream fileOut(_T("test.txt"));
        wxTextOutputStream textOut(fileOut);

        for ( size_t n = 0; n < numValues; n++ )
        {
            textOut << values[n] << endl;
        }
    }

    {
        wxFileInputStream fileIn(_T("test.txt"));
        wxTextInputStream textIn(fileIn);

        T value;
        for ( size_t n = 0; n < numValues; n++ )
        {
            textIn >> value;

            CPPUNIT_ASSERT( value == values[n] );
        }
    }
}

void TextStreamTestCase::TestLongLong()
{
    static const wxLongLong llvalues[] =
    {
        0,
        1,
        -1,
        0x12345678l,
        -0x12345678l,
        wxLL(0x123456789abcdef0),
        wxLL(-0x123456789abcdef0),
    };

    DoTestRoundTrip(llvalues, WXSIZEOF(llvalues));
}

void TextStreamTestCase::TestULongLong()
{
    static const wxULongLong ullvalues[] =
    {
        0,
        1,
        0x12345678l,
        wxULL(0x123456789abcdef0),
    };

    DoTestRoundTrip(ullvalues, WXSIZEOF(ullvalues));
}

#endif // wxUSE_LONGLONG

