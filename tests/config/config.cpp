///////////////////////////////////////////////////////////////////////////////
// Name:        tests/config/config.cpp
// Purpose:     wxConfig unit test
// Author:      Marcin Wojdyr
// Created:     2007-07-07
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Marcin Wojdyr
///////////////////////////////////////////////////////////////////////////////

// see also tests/fileconf/fileconftest.cpp for wxFileConfig specific tests

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

#include "wx/config.h"
#include "wx/colour.h"

// --------------------------------------------------------------------------
// test class
// --------------------------------------------------------------------------

class ConfigTestCase : public CppUnit::TestCase
{
public:
    ConfigTestCase() {}

private:
    CPPUNIT_TEST_SUITE( ConfigTestCase );
        CPPUNIT_TEST( ReadWriteLocalTest );
        CPPUNIT_TEST( RecordingDefaultsTest );
    CPPUNIT_TEST_SUITE_END();

    void ReadWriteLocalTest();
    void RecordingDefaultsTest();
    void ReadValues(wxConfig *config, bool has_values);

    DECLARE_NO_COPY_CLASS(ConfigTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( ConfigTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConfigTestCase, "ConfigTestCase" );

void ConfigTestCase::ReadWriteLocalTest()
{
    wxString app = wxT("wxConfigTestCase");
    wxString vendor = wxT("wxWidgets");
    wxConfig *config = new wxConfig(app, vendor, wxT(""), wxT(""),
                                    wxCONFIG_USE_LOCAL_FILE);
    config->DeleteAll();
    config->Write(wxT("string1"), wxT("abc"));
    config->Write(wxT("string2"), wxString(wxT("def")));
    config->Write(wxT("int1"), 123);
    config->Write(wxString(wxT("long1")), 234L);
    config->Write(wxT("double1"), 345.67);
    config->Write(wxT("bool1"), true);
    config->Write(wxT("color1"), wxColour(11,22,33,44));
    config->Flush();
    delete config;

    config = new wxConfig(app, vendor, wxT(""), wxT(""),
                                    wxCONFIG_USE_LOCAL_FILE);
    wxString string1 = config->Read(wxT("string1"));
    CPPUNIT_ASSERT_EQUAL( string1, wxString(wxT("abc")) );
    string1 = config->Read(wxT("string1"), wxT("defaultvalue"));
    CPPUNIT_ASSERT_EQUAL( string1, wxString(wxT("abc")) );

    wxString string2;
    bool r = config->Read(wxT("string2"), &string2);
    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT_EQUAL( string2, wxString(wxT("def")) );

    r = config->Read(wxT("string2"), &string2, wxT("defaultvalue"));
    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT_EQUAL( string2, wxString(wxT("def")) );

    int int1 = config->Read(wxT("int1"), 5);
    CPPUNIT_ASSERT_EQUAL( int1, 123 );

    long long1;
    r = config->Read(wxT("long1"), &long1);
    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT_EQUAL( long1, 234L );

    bool bool1;
    r = config->Read(wxT("foo"), &bool1);
    CPPUNIT_ASSERT( !r );

    r = config->Read(wxT("bool1"), &bool1);
    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT_EQUAL( bool1, true );

    wxColour color1;
    r = config->Read(wxT("color1"), &color1);
    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT( color1 == wxColour(11,22,33,44) );

    config->DeleteAll();
    delete config;
}

void ConfigTestCase::ReadValues(wxConfig *config, bool has_values)
{
    bool r;
    wxString string1 = config->Read(wxT("string1"), wxT("abc"));
    wxString string2 = config->Read(wxT("string2"), wxString(wxT("def")));
    wxString string3, string4;
    r = config->Read(wxT("string3"), &string3, wxT("abc"));
    CPPUNIT_ASSERT_EQUAL( r, has_values );
    r = config->Read(wxT("string4"), &string4, wxString(wxT("def")));
    CPPUNIT_ASSERT_EQUAL( r, has_values );
    int int1;
    r = config->Read(wxT("int1"), &int1, 123);
    CPPUNIT_ASSERT_EQUAL( r, has_values );
    int int2 = config->Read(wxT("int2"), 1234);
    CPPUNIT_ASSERT_EQUAL( int2, 1234 );
    long long1;
    r = config->Read(wxString(wxT("long1")), &long1, 234L);
    CPPUNIT_ASSERT_EQUAL( r, has_values );
    double double1;
    r = config->Read(wxT("double1"), &double1, 345.67);
    CPPUNIT_ASSERT_EQUAL( r, has_values );
    bool bool1;
    r = config->Read(wxT("bool1"), &bool1, true);
    CPPUNIT_ASSERT_EQUAL( r, has_values );
    wxColour color1;
    r = config->Read(wxT("color1"), &color1, wxColour(11,22,33,44));
    CPPUNIT_ASSERT_EQUAL( r, has_values );
}


void ConfigTestCase::RecordingDefaultsTest()
{
    wxString app = wxT("wxConfigTestCaseRD");
    wxString vendor = wxT("wxWidgets");
    wxConfig *config = new wxConfig(app, vendor, wxT(""), wxT(""),
                                    wxCONFIG_USE_LOCAL_FILE);
    config->DeleteAll();
    config->SetRecordDefaults(false); // by default it is false
    ReadValues(config, false);
    CPPUNIT_ASSERT_EQUAL( (int) config->GetNumberOfEntries(), 0 );
    config->SetRecordDefaults(true);
    ReadValues(config, false);
    CPPUNIT_ASSERT_EQUAL( (int) config->GetNumberOfEntries(), 10 );
    ReadValues(config, true);
    config->DeleteAll();
    delete config;
}


