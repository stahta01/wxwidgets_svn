/////////////////////////////////////////////////////////////////////////////
// Name:        taborder.cpp
// Purpose:     Sample for testing TAB navigation
// Author:      Vadim Zeitlin
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Vadim Zeitlin
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/menu.h"

    #include "wx/panel.h"

    #include "wx/button.h"
    #include "wx/listbox.h"
    #include "wx/textctrl.h"
#endif

#include "wx/notebook.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// menu commands and controls ids
enum
{
    // file menu
    TabOrder_Quit = wxID_EXIT,
    TabOrder_About = wxID_ABOUT,

    // navigation menu
    TabOrder_TabForward = 200,
    TabOrder_TabBackward,

    TabOrder_Max
};

// status panes: first one is for temporary messages, the second one shows
// current focus
enum
{
    StatusPane_Default,
    StatusPane_Focus,
    StatusPane_Max
};

// ----------------------------------------------------------------------------
// declarations of the classes used in this sample
// ----------------------------------------------------------------------------

// the main application class
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

// and the main sample window
class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);

    void OnTabForward(wxCommandEvent& event);
    void OnTabBackward(wxCommandEvent& event);

    void OnIdle(wxIdleEvent& event);

    void DoNavigate(long flags)
    {
        wxNavigationKeyEvent event;
        event.SetFlags(flags);
        if ( m_panel->ProcessEvent(event) )
            wxLogStatus(this, _T("Navigation event processed"));
        else
            wxLogStatus(this, _T("Navigation event ignored"));
    }

    wxPanel *m_panel;

    DECLARE_EVENT_TABLE()
};

// and the panel taking up MyFrame client area
class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow *parent);

private:
    wxWindow *CreateButtonPage(wxWindow *parent);
    wxWindow *CreateTextPage(wxWindow *parent);
};

// a text control which checks if processing Tab presses in controls with
// wxTE_PROCESS_TAB style really works
class MyTabTextCtrl : public wxTextCtrl
{
public:
    MyTabTextCtrl(wxWindow *parent, const wxString& value, int flags = 0)
        : wxTextCtrl(parent, wxID_ANY, value,
                     wxDefaultPosition, wxDefaultSize,
                     wxTE_PROCESS_TAB)
    {
        Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyTabTextCtrl::OnKeyDown));
    }

private:
    void OnKeyDown(wxKeyEvent& event)
    {
        if ( event.GetKeyCode() == WXK_TAB &&
                wxMessageBox
                (
                    _T("Let the Tab be used for navigation?"),
                    _T("wxWidgets TabOrder sample: Tab key pressed"),
                    wxICON_QUESTION | wxYES_NO,
                    this
                ) != wxYES )
        {
            // skip Skip() below: we consume the Tab press ourselves and so the
            // focus shouldn't change
            return;
        }

        event.Skip();
    }
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame = new MyFrame;
    frame->Show(true);

    return true;
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(TabOrder_Quit,   MyFrame::OnQuit)
    EVT_MENU(TabOrder_About,  MyFrame::OnAbout)

    EVT_MENU(TabOrder_TabForward, MyFrame::OnTabForward)
    EVT_MENU(TabOrder_TabBackward, MyFrame::OnTabBackward)

    EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

MyFrame::MyFrame()
       : wxFrame(NULL, wxID_ANY, _T("TabOrder wxWidgets Sample"),
                 wxDefaultPosition, wxSize(700, 450))
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(TabOrder_About);
    menuFile->AppendSeparator();
    menuFile->Append(TabOrder_Quit);

    wxMenu *menuNav = new wxMenu;
    menuNav->Append(TabOrder_TabForward, _T("Tab &forward\tCtrl-F"),
                    _T("Emulate a <Tab> press"));
    menuNav->Append(TabOrder_TabBackward, _T("Tab &backward\tCtrl-B"),
                    _T("Emulate a <Shift-Tab> press"));

    wxMenuBar *mbar = new wxMenuBar;
    mbar->Append(menuFile, _T("&File"));
    mbar->Append(menuNav, _T("&Navigate"));

    SetMenuBar(mbar);

    m_panel = new MyPanel(this);

    CreateStatusBar(StatusPane_Max);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Tab navigation sample\n(c) 2007 Vadim Zeitlin"),
                 _T("About TabOrder wxWidgets Sample"), wxOK, this);
}

void MyFrame::OnTabForward(wxCommandEvent& WXUNUSED(event))
{
    DoNavigate(wxNavigationKeyEvent::IsForward | wxNavigationKeyEvent::FromTab);
}

void MyFrame::OnTabBackward(wxCommandEvent& WXUNUSED(event))
{
    DoNavigate(wxNavigationKeyEvent::IsBackward | wxNavigationKeyEvent::FromTab);
}

void MyFrame::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
    // track the window which has the focus in the status bar
    static wxWindow *s_windowFocus = NULL;
    wxWindow *focus = wxWindow::FindFocus();
    if ( focus != s_windowFocus )
    {
        s_windowFocus = focus;

        wxString msg;
        if ( focus )
        {
            msg.Printf(_T("Focus is at %s"), s_windowFocus->GetName().c_str());
        }
        else
        {
            msg = _T("No focus");
        }

        SetStatusText(msg, StatusPane_Focus);
    }
}

// ----------------------------------------------------------------------------
// MyPanel
// ----------------------------------------------------------------------------

MyPanel::MyPanel(wxWindow *parent)
       : wxPanel(parent, wxID_ANY)
{
    wxNotebook *notebook = new wxNotebook(this, wxID_ANY);
    notebook->AddPage(CreateButtonPage(notebook), _T("Button"));
    notebook->AddPage(CreateTextPage(notebook), _T("Text"));

    wxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    sizerV->Add(notebook, wxSizerFlags(1).Expand());

    wxListBox *lbox = new wxListBox(this, wxID_ANY);
    lbox->AppendString(_T("Just a"));
    lbox->AppendString(_T("simple"));
    lbox->AppendString(_T("listbox"));
    sizerV->Add(lbox, wxSizerFlags(1).Expand());

    SetSizerAndFit(sizerV);
}

wxWindow *MyPanel::CreateButtonPage(wxWindow *parent)
{
    wxSizerFlags flagsBorder = wxSizerFlags().Border().Centre();

    wxPanel *page = new wxPanel(parent);
    wxSizer *sizerPage = new wxBoxSizer(wxHORIZONTAL);
    sizerPage->Add(new wxButton(page, wxID_ANY, _T("&First")), flagsBorder);
    sizerPage->Add(new wxStaticText(page, wxID_ANY, _T("[st&atic]")),
                   flagsBorder);
    sizerPage->Add(new wxButton(page, wxID_ANY, _T("&Second")), flagsBorder);

    page->SetSizer(sizerPage);

    return page;
}

wxWindow *MyPanel::CreateTextPage(wxWindow *parent)
{
    wxSizerFlags flagsBorder = wxSizerFlags().Border();

    wxSizer *sizerPage = new wxBoxSizer(wxVERTICAL);
    wxPanel *page = new wxPanel(parent);

    wxSizer *sizerH = new wxBoxSizer(wxHORIZONTAL);
    sizerH->Add(new wxStaticText(page, wxID_ANY, _T("&Label:")), flagsBorder);
    sizerH->Add(new MyTabTextCtrl(page, _T("TAB ignored here")), flagsBorder);
    sizerPage->Add(sizerH, wxSizerFlags(1).Expand());

    sizerH = new wxBoxSizer(wxHORIZONTAL);
    sizerH->Add(new wxStaticText(page, wxID_ANY, _T("&Another one::")), flagsBorder);
    sizerH->Add(new MyTabTextCtrl(page, _T("press Tab here"), wxTE_PROCESS_TAB),
                flagsBorder);
    sizerPage->Add(sizerH, wxSizerFlags(1).Expand());

    page->SetSizer(sizerPage);

    return page;
}

