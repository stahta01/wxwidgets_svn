/////////////////////////////////////////////////////////////////////////////
// Name:        uiaction.cpp
// Purpose:     wxUIActionSimulator sample
// Author:      Kevin Ollivier
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Kevin Ollivier
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
 
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/uiaction.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    TheButton = 100,
    RunSimulation
};

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    void OnButtonPressed(wxCommandEvent&);
    void OnRunSimulation(wxCommandEvent&);
    
    bool ButtonPressed() const { return m_buttonPressed; }
    bool MenuSelected() const { return m_menuSelected; }
    
private:
    bool m_buttonPressed;
    bool m_menuSelected;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(TheButton, MyFrame::OnButtonPressed)
    EVT_MENU(RunSimulation, MyFrame::OnRunSimulation)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame = new MyFrame("wxUIActionSimulator sample application");
    frame->Show(true);
    
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    m_buttonPressed = false;
    m_menuSelected = false;

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    fileMenu->Append(wxID_NEW, "&New File...", "Open a new file");
    fileMenu->Append(RunSimulation, "&Run Simulation...", "Run the UI action simulation");

    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

    wxButton* button = new wxButton(this, TheButton, "Button");
    button->SetName("TheButton");
}


// event handlers

void MyFrame::OnRunSimulation(wxCommandEvent&)
{
    wxUIActionSimulator sim;
    wxWindow* button = FindWindow(wxString("TheButton"));
    wxPoint globalPoint = button->ClientToScreen(wxPoint(20, 10));
    sim.MouseMove(globalPoint.x, globalPoint.y);
    sim.MouseClick(wxMOUSE_BTN_LEFT);
    
    wxYield();
    
    if (ButtonPressed())
        wxMessageBox("Button automagically pressed!");
}

void MyFrame::OnButtonPressed(wxCommandEvent&)
{
    m_buttonPressed = true;
}
