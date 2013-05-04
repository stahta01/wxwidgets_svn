///////////////////////////////////////////////////////////////////////////////
// Name:        tests/events/propagation.cpp
// Purpose:     Test events propagation
// Author:      Vadim Zeitlin
// Created:     2009-01-16
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/event.h"
    #include "wx/scrolwin.h"
    #include "wx/window.h"
#endif // WX_PRECOMP

#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/scopedptr.h"
#include "wx/scopeguard.h"

namespace
{

// this string will record the execution of all handlers
wxString g_str;

// a custom event
wxDEFINE_EVENT(TEST_EVT, wxCommandEvent);

// a custom event handler tracing the propagation of the events of the
// specified types
template <class Event>
class TestEvtHandlerBase : public wxEvtHandler
{
public:
    TestEvtHandlerBase(wxEventType evtType, char tag)
        : m_evtType(evtType),
          m_tag(tag)
    {
        Connect(evtType,
                static_cast<wxEventFunction>(&TestEvtHandlerBase::OnTest));
    }

    // override ProcessEvent() to confirm that it is called for all event
    // handlers in the chain
    virtual bool ProcessEvent(wxEvent& event)
    {
        if ( event.GetEventType() == m_evtType )
            g_str += 'o'; // "o" == "overridden"

        return wxEvtHandler::ProcessEvent(event);
    }

private:
    void OnTest(wxEvent& event)
    {
        g_str += m_tag;

        event.Skip();
    }

    const wxEventType m_evtType;
    const char m_tag;

    wxDECLARE_NO_COPY_TEMPLATE_CLASS(TestEvtHandlerBase, Event);
};

struct TestEvtHandler : TestEvtHandlerBase<wxCommandEvent>
{
    TestEvtHandler(char tag)
        : TestEvtHandlerBase<wxCommandEvent>(TEST_EVT, tag)
    {
    }
};

struct TestMenuEvtHandler : TestEvtHandlerBase<wxCommandEvent>
{
    TestMenuEvtHandler(char tag)
        : TestEvtHandlerBase<wxCommandEvent>(wxEVT_MENU, tag)
    {
    }
};

struct TestPaintEvtHandler : TestEvtHandlerBase<wxPaintEvent>
{
    TestPaintEvtHandler(char tag)
        : TestEvtHandlerBase<wxPaintEvent>(wxEVT_PAINT, tag)
    {
    }
};

// a window handling the test event
class TestWindow : public wxWindow
{
public:
    TestWindow(wxWindow *parent, char tag)
        : wxWindow(parent, wxID_ANY),
          m_tag(tag)
    {
        Connect(TEST_EVT, wxCommandEventHandler(TestWindow::OnTest));
    }

private:
    void OnTest(wxCommandEvent& event)
    {
        g_str += m_tag;

        event.Skip();
    }

    const char m_tag;

    DECLARE_NO_COPY_CLASS(TestWindow)
};

// a scroll window handling paint event: we want to have a special test case
// for this because the event propagation is complicated even further than
// usual here by the presence of wxScrollHelperEvtHandler in the event handlers
// chain and the fact that OnDraw() virtual method must be called if EVT_PAINT
// is not handled
class TestScrollWindow : public wxScrolledWindow
{
public:
    TestScrollWindow(wxWindow *parent)
        : wxScrolledWindow(parent, wxID_ANY)
    {
        Connect(wxEVT_PAINT, wxPaintEventHandler(TestScrollWindow::OnPaint));
    }

    virtual void OnDraw(wxDC& WXUNUSED(dc))
    {
        g_str += 'D';   // draw
    }

private:
    void OnPaint(wxPaintEvent& event)
    {
        g_str += 'P';   // paint
        event.Skip();
    }

    wxDECLARE_NO_COPY_CLASS(TestScrollWindow);
};

int DoFilterEvent(wxEvent& event)
{
    if ( event.GetEventType() == TEST_EVT ||
            event.GetEventType() == wxEVT_MENU )
        g_str += 'a';

    return -1;
}

bool DoProcessEvent(wxEvent& event)
{
    if ( event.GetEventType() == TEST_EVT ||
            event.GetEventType() == wxEVT_MENU )
        g_str += 'A';

    return false;
}

} // anonymous namespace

// --------------------------------------------------------------------------
// test class
// --------------------------------------------------------------------------

class EventPropagationTestCase : public CppUnit::TestCase
{
public:
    EventPropagationTestCase() {}

    virtual void setUp();
    virtual void tearDown();

private:
    CPPUNIT_TEST_SUITE( EventPropagationTestCase );
        CPPUNIT_TEST( OneHandler );
        CPPUNIT_TEST( TwoHandlers );
        CPPUNIT_TEST( WindowWithoutHandler );
        CPPUNIT_TEST( WindowWithHandler );
        CPPUNIT_TEST( ForwardEvent );
        CPPUNIT_TEST( ScrollWindowWithoutHandler );
        CPPUNIT_TEST( ScrollWindowWithHandler );
        CPPUNIT_TEST( MenuEvent );
    CPPUNIT_TEST_SUITE_END();

    void OneHandler();
    void TwoHandlers();
    void WindowWithoutHandler();
    void WindowWithHandler();
    void ForwardEvent();
    void ScrollWindowWithoutHandler();
    void ScrollWindowWithHandler();
    void MenuEvent();

    DECLARE_NO_COPY_CLASS(EventPropagationTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( EventPropagationTestCase );

// also include in its own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventPropagationTestCase, "EventPropagationTestCase" );

void EventPropagationTestCase::setUp()
{
    SetFilterEventFunc(DoFilterEvent);
    SetProcessEventFunc(DoProcessEvent);

    g_str.clear();
}

void EventPropagationTestCase::tearDown()
{
    SetFilterEventFunc(NULL);
    SetProcessEventFunc(NULL);
}

void EventPropagationTestCase::OneHandler()
{
    wxCommandEvent event(TEST_EVT);
    TestEvtHandler h1('1');
    h1.ProcessEvent(event);
    CPPUNIT_ASSERT_EQUAL( "oa1A", g_str );
}

void EventPropagationTestCase::TwoHandlers()
{
    wxCommandEvent event(TEST_EVT);
    TestEvtHandler h1('1');
    TestEvtHandler h2('2');
    h1.SetNextHandler(&h2);
    h2.SetPreviousHandler(&h1);
    h1.ProcessEvent(event);
    CPPUNIT_ASSERT_EQUAL( "oa1o2A", g_str );
}

void EventPropagationTestCase::WindowWithoutHandler()
{
    wxCommandEvent event(TEST_EVT);
    TestWindow * const parent = new TestWindow(wxTheApp->GetTopWindow(), 'p');
    wxON_BLOCK_EXIT_OBJ0( *parent, wxWindow::Destroy );

    TestWindow * const child = new TestWindow(parent, 'c');

    child->GetEventHandler()->ProcessEvent(event);
    CPPUNIT_ASSERT_EQUAL( "acpA", g_str );
}

void EventPropagationTestCase::WindowWithHandler()
{
    wxCommandEvent event(TEST_EVT);
    TestWindow * const parent = new TestWindow(wxTheApp->GetTopWindow(), 'p');
    wxON_BLOCK_EXIT_OBJ0( *parent, wxWindow::Destroy );

    TestWindow * const child = new TestWindow(parent, 'c');

    TestEvtHandler h1('1');
    child->PushEventHandler(&h1);
    wxON_BLOCK_EXIT_OBJ1( *child, wxWindow::PopEventHandler, false );
    TestEvtHandler h2('2');
    child->PushEventHandler(&h2);
    wxON_BLOCK_EXIT_OBJ1( *child, wxWindow::PopEventHandler, false );

    child->HandleWindowEvent(event);
    CPPUNIT_ASSERT_EQUAL( "oa2o1cpA", g_str );
}

void EventPropagationTestCase::ForwardEvent()
{
    // The idea of this test is to check that the events explicitly forwarded
    // to another event handler still get pre/post-processed as usual as this
    // used to be broken by the fixes trying to avoid duplicate processing.
    TestWindow * const win = new TestWindow(wxTheApp->GetTopWindow(), 'w');
    wxON_BLOCK_EXIT_OBJ0( *win, wxWindow::Destroy );

    TestEvtHandler h1('1');
    win->PushEventHandler(&h1);
    wxON_BLOCK_EXIT_OBJ1( *win, wxWindow::PopEventHandler, false );

    class ForwardEvtHandler : public wxEvtHandler
    {
    public:
        ForwardEvtHandler(wxEvtHandler& h) : m_h(&h) { }

        virtual bool ProcessEvent(wxEvent& event)
        {
            g_str += 'f';

            return m_h->ProcessEvent(event);
        }

    private:
        wxEvtHandler *m_h;
    } f(h1);

    // First send the event directly to f.
    wxCommandEvent event1(TEST_EVT);
    f.ProcessEvent(event1);
    CPPUNIT_ASSERT_EQUAL( "foa1wA", g_str );
    g_str.clear();

    // And then also test sending it to f indirectly.
    wxCommandEvent event2(TEST_EVT);
    TestEvtHandler h2('2');
    h2.SetNextHandler(&f);
    h2.ProcessEvent(event2);
    CPPUNIT_ASSERT_EQUAL( "oa2fo1wAA", g_str );
}

void EventPropagationTestCase::ScrollWindowWithoutHandler()
{
    TestWindow * const parent = new TestWindow(wxTheApp->GetTopWindow(), 'p');
    wxON_BLOCK_EXIT_OBJ0( *parent, wxWindow::Destroy );

    TestScrollWindow * const win = new TestScrollWindow(parent);

#if !defined(__WXOSX__) && !defined(__WXGTK3__)
    wxPaintEvent event(win->GetId());
    win->ProcessWindowEvent(event);
    CPPUNIT_ASSERT_EQUAL( "PD", g_str );
#endif
    g_str.clear();
    wxCommandEvent eventCmd(TEST_EVT);
    win->HandleWindowEvent(eventCmd);
    CPPUNIT_ASSERT_EQUAL( "apA", g_str );
}

void EventPropagationTestCase::ScrollWindowWithHandler()
{
    TestWindow * const parent = new TestWindow(wxTheApp->GetTopWindow(), 'p');
    wxON_BLOCK_EXIT_OBJ0( *parent, wxWindow::Destroy );

    TestScrollWindow * const win = new TestScrollWindow(parent);

#if !defined(__WXOSX__) && !defined(__WXGTK3__)
    TestPaintEvtHandler h('h');
    win->PushEventHandler(&h);
    wxON_BLOCK_EXIT_OBJ1( *win, wxWindow::PopEventHandler, false );

    wxPaintEvent event(win->GetId());
    win->ProcessWindowEvent(event);
    CPPUNIT_ASSERT_EQUAL( "ohPD", g_str );
#endif

    g_str.clear();
    wxCommandEvent eventCmd(TEST_EVT);
    win->HandleWindowEvent(eventCmd);
    CPPUNIT_ASSERT_EQUAL( "apA", g_str );
}

// Create a menu bar with a single menu containing wxID_APPLY menu item and
// attach it to the specified frame.
wxMenu* CreateTestMenu(wxFrame* frame)
{
    wxMenu* const menu = new wxMenu;
    menu->Append(wxID_APPLY);
    wxMenuBar* const mb = new wxMenuBar;
    mb->Append(menu, "&Menu");
    frame->SetMenuBar(mb);

    return menu;
}

// Helper for checking that the menu event processing resulted in the expected
// output from the handlers.
//
// Notice that this is supposed to be used with ASSERT_MENU_EVENT_RESULT()
// macro to make the file name and line number of the caller appear in the
// failure messages.
void
CheckMenuEvent(wxMenu* menu, const char* result, CppUnit::SourceLine sourceLine)
{
    g_str.clear();

    // Trigger the menu event: this is more reliable than using
    // wxUIActionSimulator and currently works in all ports as they all call
    // wxMenuBase::SendEvent() from their respective menu event handlers.
    menu->SendEvent(wxID_APPLY);

    CPPUNIT_NS::assertEquals( result, g_str, sourceLine, "" );
}

#define ASSERT_MENU_EVENT_RESULT(menu, result) \
    CheckMenuEvent((menu), (result), CPPUNIT_SOURCELINE())

void EventPropagationTestCase::MenuEvent()
{
    wxFrame* const frame = static_cast<wxFrame*>(wxTheApp->GetTopWindow());

    // Create a minimal menu bar.
    wxMenu* const menu = CreateTestMenu(frame);
    wxMenuBar* const mb = menu->GetMenuBar();
    wxScopedPtr<wxMenuBar> ensureMenuBarDestruction(mb);
    wxON_BLOCK_EXIT_OBJ1( *frame, wxFrame::SetMenuBar, (wxMenuBar*)NULL );

    // Check that wxApp gets the event exactly once.
    ASSERT_MENU_EVENT_RESULT( menu, "aA" );


    // Check that the menu event handler is called.
    TestMenuEvtHandler hm('m'); // 'm' for "menu"
    menu->SetNextHandler(&hm);
    wxON_BLOCK_EXIT_OBJ1( *menu,
                          wxEvtHandler::SetNextHandler, (wxEvtHandler*)NULL );
    ASSERT_MENU_EVENT_RESULT( menu, "aomA" );


    // Test that the event handler associated with the menu bar gets the event.
    TestMenuEvtHandler hb('b'); // 'b' for "menu Bar"
    mb->PushEventHandler(&hb);
    wxON_BLOCK_EXIT_OBJ1( *mb, wxWindow::PopEventHandler, false );

    ASSERT_MENU_EVENT_RESULT( menu, "aomobA" );


    // Also test that the window to which the menu belongs gets the event.
    TestMenuEvtHandler hw('w'); // 'w' for "Window"
    frame->PushEventHandler(&hw);
    wxON_BLOCK_EXIT_OBJ1( *frame, wxWindow::PopEventHandler, false );

    ASSERT_MENU_EVENT_RESULT( menu, "aomobowA" );
}
