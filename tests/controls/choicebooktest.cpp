///////////////////////////////////////////////////////////////////////////////
// Name:        tests/controls/choicebooktest.cpp
// Purpose:     wxChoicebook unit test
// Author:      Steven Lamerton
// Created:     2010-07-02
// RCS-ID:      $Id$
// Copyright:   (c) 2010 Steven Lamerton
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/panel.h"
#endif // WX_PRECOMP

#include "wx/choicebk.h"
#include "bookctrlbasetest.h"

class ChoicebookTestCase : public BookCtrlBaseTestCase
{
public:
    ChoicebookTestCase() { }

    virtual void setUp();
    virtual void tearDown();

private:
    virtual wxBookCtrlBase *GetBase() const { return m_choicebook; }

    virtual wxEventType GetChangedEvent() const
    { return wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED; }

    virtual wxEventType GetChangingEvent() const
    { return wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING; }

    CPPUNIT_TEST_SUITE( ChoicebookTestCase );
        wxBOOK_CTRL_BASE_TESTS();
    CPPUNIT_TEST_SUITE_END();

    wxChoicebook *m_choicebook;

    DECLARE_NO_COPY_CLASS(ChoicebookTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( ChoicebookTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ChoicebookTestCase, "ChoicebookTestCase" );

void ChoicebookTestCase::setUp()
{
    m_choicebook = new wxChoicebook(wxTheApp->GetTopWindow(), wxID_ANY);
    AddPanels();
}

void ChoicebookTestCase::tearDown()
{
    wxDELETE(m_choicebook);
}
