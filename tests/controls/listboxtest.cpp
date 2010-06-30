///////////////////////////////////////////////////////////////////////////////
// Name:        tests/controls/listbox.cpp
// Purpose:     wxListBox unit test
// Author:      Steven Lamerton
// Created:     2010-06-29
// RCS-ID:      $Id$
// Copyright:   (c) 2010 Steven Lamerton
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/listbox.h"
#endif // WX_PRECOMP

#include "itemcontainertest.h"

class ListBoxTestCase : public ItemContainerTestCase
{
public:
    ListBoxTestCase() { }

    virtual void setUp();
    virtual void tearDown();

private:
    virtual wxItemContainer *GetContainer() const { return m_list; }
    virtual wxWindow *GetContainerWindow() const { return m_list; }

    CPPUNIT_TEST_SUITE( ListBoxTestCase );
        wxITEM_CONTAINER_TESTS();
        CPPUNIT_TEST( Sort );
    CPPUNIT_TEST_SUITE_END();

    void Sort();

    wxListBox* m_list;

    DECLARE_NO_COPY_CLASS(ListBoxTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( ListBoxTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ListBoxTestCase, "ListBoxTestCase" );

void ListBoxTestCase::setUp()
{
    m_list = new wxListBox(wxTheApp->GetTopWindow(), wxID_ANY);
}

void ListBoxTestCase::tearDown()
{
    wxDELETE(m_list);
}

void ListBoxTestCase::Sort()
{
    wxDELETE(m_list);
    m_list = new wxListBox(wxTheApp->GetTopWindow(), wxID_ANY, 
                            wxDefaultPosition, wxDefaultSize, 0, 0,
                            wxLB_SORT);

    wxArrayString testitems;
    testitems.Add("aaa");
    testitems.Add("Aaa");
    testitems.Add("aba");
    testitems.Add("aaab");
    testitems.Add("aab");
    testitems.Add("AAA");

    m_list->Append(testitems);

    CPPUNIT_ASSERT_EQUAL("AAA", m_list->GetString(0));
    CPPUNIT_ASSERT_EQUAL("Aaa", m_list->GetString(1));
    CPPUNIT_ASSERT_EQUAL("aaa", m_list->GetString(2));
    CPPUNIT_ASSERT_EQUAL("aaab", m_list->GetString(3));
    CPPUNIT_ASSERT_EQUAL("aab", m_list->GetString(4));
    CPPUNIT_ASSERT_EQUAL("aba", m_list->GetString(5));

    m_list->Append("a");

    CPPUNIT_ASSERT_EQUAL("a", m_list->GetString(0));
}

