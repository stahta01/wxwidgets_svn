/////////////////////////////////////////////////////////////////////////////
// Name:        test.cpp
// Purpose:     wxToolBar sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/toolbar.h"
#include <wx/log.h>

#include "test.h"

#if defined(__WXGTK__) || defined(__WXMOTIF__)
#include "mondrian.xpm"
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
// #include "bitmaps/paste.xpm"
#include "bitmaps/print.xpm"
#include "bitmaps/preview.xpm"
#include "bitmaps/help.xpm"
#endif

IMPLEMENT_APP(MyApp)

// The `main program' equivalent, creating the windows and returning the
// main frame
bool MyApp::OnInit()
{
    // Create the main frame window
    MyFrame* frame = new MyFrame((wxFrame *) NULL, -1, (const wxString) "wxToolBar Sample",
            wxPoint(100, 100), wxSize(450, 300));

    // Give it a status line
    frame->CreateStatusBar();

    // Give it an icon
    frame->SetIcon(wxICON(mondrian));

    // Make a menubar
    wxMenu *tbarMenu = new wxMenu;
    tbarMenu->Append(IDM_TOOLBAR_TOGGLETOOLBAR, "&Toggle toolbar", "Change the toolbar kind");
    tbarMenu->Append(IDM_TOOLBAR_ENABLEPRINT, "&Enable print button", "");
    tbarMenu->Append(IDM_TOOLBAR_TOGGLEHELP, "Toggle &help button", "");

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "E&xit", "Quit toolbar sample" );

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, "&About", "About toolbar sample");

    wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(tbarMenu, "&Toolbar");
    menuBar->Append(helpMenu, "&Help");

    // Associate the menu bar with the frame
    frame->SetMenuBar(menuBar);

    // Create the toolbar
    frame->CreateToolBar(wxNO_BORDER|wxTB_HORIZONTAL|wxTB_FLAT|wxTB_DOCKABLE, ID_TOOLBAR);

    frame->GetToolBar()->SetMargins( 2, 2 );

    InitToolbar(frame->GetToolBar());

    // Force a resize. This should probably be replaced by a call to a wxFrame
    // function that lays out default decorations and the remaining content window.
    wxSizeEvent event(wxSize(-1, -1), frame->GetId());
    frame->OnSize(event);
    frame->Show(TRUE);

    frame->SetStatusText("Hello, wxWindows");

    SetTopWindow(frame);

    return TRUE;
}

bool MyApp::InitToolbar(wxToolBar* toolBar, bool small)
{
  // Set up toolbar
  wxBitmap* toolBarBitmaps[8];

#ifdef __WXMSW__
  toolBarBitmaps[0] = new wxBitmap("icon1");
  toolBarBitmaps[1] = new wxBitmap("icon2");
  if ( !small )
  {
      toolBarBitmaps[2] = new wxBitmap("icon3");
      toolBarBitmaps[3] = new wxBitmap("icon4");
      toolBarBitmaps[4] = new wxBitmap("icon5");
      toolBarBitmaps[5] = new wxBitmap("icon6");
      toolBarBitmaps[6] = new wxBitmap("icon7");
      toolBarBitmaps[7] = new wxBitmap("icon8");
  }
#else
  toolBarBitmaps[0] = new wxBitmap( new_xpm );
  toolBarBitmaps[1] = new wxBitmap( open_xpm );
  if ( !small )
  {
      toolBarBitmaps[2] = new wxBitmap( save_xpm );
      toolBarBitmaps[3] = new wxBitmap( copy_xpm );
      toolBarBitmaps[4] = new wxBitmap( cut_xpm );
      toolBarBitmaps[5] = new wxBitmap( preview_xpm );
      toolBarBitmaps[6] = new wxBitmap( print_xpm );
      toolBarBitmaps[7] = new wxBitmap( help_xpm );
  }
#endif

#ifdef __WXMSW__
  int width = 24;
#else
  int width = 16;
#endif
  int currentX = 5;

  toolBar->AddTool(wxID_NEW, *(toolBarBitmaps[0]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "New file");
  currentX += width + 5;
  toolBar->AddTool(wxID_OPEN, *(toolBarBitmaps[1]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Open file");

  if ( !small )
  {
      currentX += width + 5;
      toolBar->AddTool(wxID_SAVE, *(toolBarBitmaps[2]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Save file");
      currentX += width + 5;
      toolBar->AddSeparator();
      toolBar->AddTool(wxID_COPY, *(toolBarBitmaps[3]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Disable/Enable print button");
      currentX += width + 5;
      toolBar->AddTool(wxID_CUT, *(toolBarBitmaps[4]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Toggle/Untoggle help button");
      currentX += width + 5;
      toolBar->AddTool(wxID_PASTE, *(toolBarBitmaps[5]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Paste");
      currentX += width + 5;
      toolBar->AddSeparator();
      toolBar->AddTool(wxID_PRINT, *(toolBarBitmaps[6]), wxNullBitmap, FALSE, currentX, -1, (wxObject *) NULL, "Print");
      currentX += width + 5;
      toolBar->AddSeparator();
      toolBar->AddTool(wxID_HELP, *(toolBarBitmaps[7]), *(toolBarBitmaps[6]), TRUE, currentX, -1, (wxObject *) NULL, "Help");
      
      toolBar->EnableTool( wxID_PRINT, FALSE );
  }

  toolBar->Realize();

  // Can delete the bitmaps since they're reference counted
  int i, max = small ? 2 : WXSIZEOF(toolBarBitmaps);
  for (i = 0; i < max; i++)
    delete toolBarBitmaps[i];

  return TRUE;
}

// wxID_HELP will be processed for the 'About' menu and the toolbar help button.

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
    EVT_MENU(wxID_HELP, MyFrame::OnAbout)

    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLBAR, OnToggleToolbar)
    EVT_MENU(IDM_TOOLBAR_ENABLEPRINT, OnEnablePrint)
    EVT_MENU(IDM_TOOLBAR_TOGGLEHELP, OnToggleHelp)

    EVT_MENU(-1, MyFrame::OnToolLeftClick)
    EVT_TOOL_ENTER(ID_TOOLBAR, MyFrame::OnToolEnter)
END_EVENT_TABLE()

// Define my frame constructor
MyFrame::MyFrame(wxFrame* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
       : wxFrame(parent, id, title, pos, size, style)
{
    m_textWindow = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(-1, -1), wxTE_MULTILINE);
    m_smallToolbar = FALSE;
}

void MyFrame::OnToggleToolbar(wxCommandEvent& event)
{
    delete GetToolBar();
    SetToolBar(NULL);
    CreateToolBar(wxNO_BORDER|wxTB_HORIZONTAL|wxTB_FLAT|wxTB_DOCKABLE, ID_TOOLBAR);

    m_smallToolbar = !m_smallToolbar;
    wxGetApp().InitToolbar(GetToolBar(), m_smallToolbar);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    (void)wxMessageBox("wxWindows toolbar sample", "About wxToolBar");
}

void MyFrame::OnToolLeftClick(wxCommandEvent& event)
{
    wxString str;
    str.Printf( _T("Clicked on tool %d\n"), event.GetId());
    m_textWindow->WriteText( str );
  
    if (event.GetId() == wxID_HELP)
    {
        if ((bool)event.GetExtraLong())
            m_textWindow->WriteText( _T("Help button down now.\n") );
        else
            m_textWindow->WriteText( _T("Help button up now.\n") );
    }
  
    if (event.GetId() == wxID_COPY)
    {
        DoEnablePrint();
    }
    
    if (event.GetId() == wxID_CUT)
    {
        DoToggleHelp();
    }
}

void MyFrame::DoEnablePrint()
{
    wxToolBar *tb = GetToolBar();
    if (tb->GetToolEnabled(wxID_PRINT))
        tb->EnableTool( wxID_PRINT, FALSE );
    else
        tb->EnableTool( wxID_PRINT, TRUE );
}

void MyFrame::DoToggleHelp()
{
    wxToolBar *tb = GetToolBar();
    tb->ToggleTool( wxID_HELP, !tb->GetToolState( wxID_HELP ) );
}

void MyFrame::OnToolEnter(wxCommandEvent& event)
{
  if (event.GetSelection() > -1)
  {
    wxString str;
    str.Printf(_T("This is tool number %d"), event.GetSelection());
    SetStatusText(str);
  }
  else
    SetStatusText("");
}

