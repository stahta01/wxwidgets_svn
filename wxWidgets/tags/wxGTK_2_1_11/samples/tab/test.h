/////////////////////////////////////////////////////////////////////////////
// Name:        test.h
// Purpose:     Tab demo
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// Define a new application
class MyApp: public wxApp
{
public:
    bool OnInit(void);
    void InitTabView(wxPanelTabView* view, wxWindow* window);

    wxButton*   m_okButton;
    wxButton*   m_cancelButton;
    wxButton*   m_helpButton;
};

DECLARE_APP(MyApp)

class MyDialog: public wxTabbedDialog
{
public:
    MyDialog(wxWindow* parent, const wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, const long windowStyle = wxDEFAULT_DIALOG_STYLE);

    void OnOK(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void Init(void);

DECLARE_EVENT_TABLE()
};

class MyFrame: public wxFrame
{
public:
    MyFrame(wxFrame* parent, const wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, const long windowStyle = wxDEFAULT_FRAME_STYLE);

    void OnOK(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void OnSize(wxSizeEvent& event);
    void Init(void);
protected:
    wxTabbedPanel*      m_panel;
    wxPanelTabView*     m_view;

DECLARE_EVENT_TABLE()
};


class MyTabbedPanel: public wxTabbedPanel
{
public:
    MyTabbedPanel(wxWindow* parent, const wxWindowID id,
        const wxPoint& pos, const wxSize& size, const long windowStyle = 0);

    void OnOK(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
//    void OnAbout(wxCommandEvent& event);
    void Init(void);

DECLARE_EVENT_TABLE()
};

// File ids
#define TEST_ABOUT          2

// Tab ids
#define TEST_TAB_DOG        1
#define TEST_TAB_CAT        2
#define TEST_TAB_GOAT       3
#define TEST_TAB_GUINEAPIG  4
#define TEST_TAB_ANTEATER   5
#define TEST_TAB_HUMMINGBIRD 6
#define TEST_TAB_SHEEP      7
#define TEST_TAB_COW        8
#define TEST_TAB_HORSE      9
#define TEST_TAB_PIG        10
#define TEST_TAB_OSTRICH    11
#define TEST_TAB_AARDVARK   12

