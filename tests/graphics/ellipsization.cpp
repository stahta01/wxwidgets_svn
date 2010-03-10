///////////////////////////////////////////////////////////////////////////////
// Name:        tests/graphics/ellipsization.cpp
// Purpose:     wxControlBase::*Ellipsize* unit test
// Author:      Francesco Montorsi
// Created:     2010-03-10
// RCS-ID:      $Id$
// Copyright:   (c) 2010 Francesco Montorsi
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/control.h"
#include "wx/dcmemory.h"

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class EllipsizationTestCase : public CppUnit::TestCase
{
public:
    EllipsizationTestCase() { }

private:
    CPPUNIT_TEST_SUITE( EllipsizationTestCase );
        CPPUNIT_TEST( Ellipsize );
    CPPUNIT_TEST_SUITE_END();

    void Ellipsize();

    DECLARE_NO_COPY_CLASS(EllipsizationTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( EllipsizationTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EllipsizationTestCase, "EllipsizationTestCase" );

void EllipsizationTestCase::Ellipsize()
{
    wxMemoryDC dc;

    wxString stringsToTest[] = 
    { 
        "N", ".", "x", "foobar", wxS("\u03B1"), "Another test", "a very very very very very very very long string",
        "\xCE\xB1\xCE\xB2\xCE\xB3\xCE\xB4\xCE\xB5\xCE\xB6\xCE\xB7\xCE\xB8\xCE\xB9", 
            // alpha+beta+gamma+delta+epsilon+zeta+eta+theta+iota
        "\t", "\t\t\t\t\t", "a\tstring\twith\ttabs",
        "\n", "\n\n\n\n\n", "a\nstring\nwith\nnewlines",
        "&", "&&&&&&&", "a&string&with&newlines",
        "\t\n&", "a\t\n&string\t\n&with\t\n&many\t\n&chars"
    };
    int flagsToTest[] = { 0, wxELLIPSIZE_FLAGS_PROCESS_MNEMONICS, wxELLIPSIZE_FLAGS_EXPAND_TABS, 
                          wxELLIPSIZE_FLAGS_PROCESS_MNEMONICS|wxELLIPSIZE_FLAGS_EXPAND_TABS };
    wxEllipsizeMode modesToTest[] = { wxELLIPSIZE_START, wxELLIPSIZE_MIDDLE, wxELLIPSIZE_END };
    int widthsToTest[] = { 0, 1, 2, 3, 10, 20, 100 };

    for (unsigned int s=0; s<WXSIZEOF(stringsToTest); s++)
        for (unsigned int f=0; f<WXSIZEOF(flagsToTest); f++)
            for (unsigned int m=0; m<WXSIZEOF(modesToTest); m++)
                for (unsigned int w=0; w<WXSIZEOF(widthsToTest); w++)
                {
                    wxString ret = wxControlBase::Ellipsize(stringsToTest[s], dc, modesToTest[m], 
                                                            widthsToTest[w], flagsToTest[f]);

                    CPPUNIT_ASSERT_MESSAGE((std::string)("invalid ellipsization for: " + stringsToTest[s]),
                                           dc.GetMultiLineTextExtent(ret).GetWidth() <= widthsToTest[w]);
                }
}
