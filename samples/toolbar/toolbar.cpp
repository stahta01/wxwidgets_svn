/////////////////////////////////////////////////////////////////////////////
// Name:        toolbar.cpp
// Purpose:     wxToolBar sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
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

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/toolbar.h>
#include <wx/log.h>
#include <wx/image.h>
#include <wx/spinctrl.h>

// define this to 1 to use wxToolBarSimple instead of the native one
#define USE_GENERIC_TBAR 0

// define this to use XPMs everywhere (by default, BMPs are used under Win)
// BMPs use less space, but aren't compiled into the executable on other platforms
#ifdef __WXMSW__
    #define USE_XPM_BITMAPS 0
#else
    #define USE_XPM_BITMAPS 1
#endif

#if USE_GENERIC_TBAR
    #if !wxUSE_TOOLBAR_SIMPLE
        #error wxToolBarSimple is not compiled in, set wxUSE_TOOLBAR_SIMPLE \
               to 1 in setup.h and recompile the library.
    #else
        #include <wx/tbarsmpl.h>
    #endif
#endif // USE_GENERIC_TBAR

#if USE_XPM_BITMAPS && defined(__WXMSW__) && !wxUSE_XPM_IN_MSW
    #error You need to enable XPM support to use XPM bitmaps with toolbar!
#endif // USE_XPM_BITMAPS

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#if USE_XPM_BITMAPS
    #include "mondrian.xpm"
    #include "bitmaps/new.xpm"
    #include "bitmaps/open.xpm"
    #include "bitmaps/save.xpm"
    #include "bitmaps/copy.xpm"
    #include "bitmaps/cut.xpm"
    #include "bitmaps/preview.xpm"  // paste XPM
    #include "bitmaps/print.xpm"
    #include "bitmaps/help.xpm"
#endif // USE_XPM_BITMAPS

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new application
class MyApp : public wxApp
{
public:
    bool OnInit();
};

// Define a new frame
class MyFrame: public wxFrame
{
public:
    MyFrame(wxFrame *parent,
            wxWindowID id = -1,
            const wxString& title = "wxToolBar Sample",
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE);

    void RecreateToolbar();

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSize(wxSizeEvent& event);

    void OnToggleAnotherToolbar(wxCommandEvent& event);

    void OnToggleToolbarSize(wxCommandEvent& event);
    void OnToggleToolbarOrient(wxCommandEvent& event);
    void OnToggleToolbarRows(wxCommandEvent& event);

    void OnEnablePrint(wxCommandEvent& WXUNUSED(event)) { DoEnablePrint(); }
    void OnDeletePrint(wxCommandEvent& WXUNUSED(event)) { DoDeletePrint(); }
    void OnInsertPrint(wxCommandEvent& event);
    void OnChangeToolTip(wxCommandEvent& event);
    void OnToggleHelp(wxCommandEvent& WXUNUSED(event)) { DoToggleHelp(); }

    void OnToolbarStyle(wxCommandEvent& event);

    void OnToolLeftClick(wxCommandEvent& event);
    void OnToolEnter(wxCommandEvent& event);

    void OnCombo(wxCommandEvent& event);

    void OnUpdateCopyAndCut(wxUpdateUIEvent& event);

#if USE_GENERIC_TBAR
    virtual wxToolBar *OnCreateToolBar(long style,
                                       wxWindowID id,
                                       const wxString& name );
#endif // USE_GENERIC_TBAR

private:
    void DoEnablePrint();
    void DoDeletePrint();
    void DoToggleHelp();

    void LayoutChildren();

    bool                m_smallToolbar,
                        m_horzToolbar;
    size_t              m_rows;             // 1 or 2 only

    wxTextCtrl         *m_textWindow;

    wxToolBar          *m_tbar;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

const int ID_TOOLBAR = 500;

static const long TOOLBAR_STYLE = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT ;
// static const long TOOLBAR_STYLE = 0;

enum
{
    IDM_TOOLBAR_TOGGLETOOLBARSIZE = 200,
    IDM_TOOLBAR_TOGGLETOOLBARORIENT,
    IDM_TOOLBAR_TOGGLETOOLBARROWS,
    IDM_TOOLBAR_ENABLEPRINT,
    IDM_TOOLBAR_DELETEPRINT,
    IDM_TOOLBAR_INSERTPRINT,
    IDM_TOOLBAR_TOGGLEHELP,
    IDM_TOOLBAR_TOGGLE_ANOTHER_TOOLBAR,
    IDM_TOOLBAR_CHANGE_TOOLTIP,
    IDM_TOOLBAR_SHOW_TEXT,
    IDM_TOOLBAR_SHOW_ICONS,
    IDM_TOOLBAR_SHOW_BOTH,

    ID_COMBO = 1000
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

// Notice that wxID_HELP will be processed for the 'About' menu and the toolbar
// help button.

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_SIZE(MyFrame::OnSize)

    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
    EVT_MENU(wxID_HELP, MyFrame::OnAbout)

    EVT_MENU(IDM_TOOLBAR_TOGGLE_ANOTHER_TOOLBAR, MyFrame::OnToggleAnotherToolbar)

    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLBARSIZE, MyFrame::OnToggleToolbarSize)
    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLBARORIENT, MyFrame::OnToggleToolbarOrient)
    EVT_MENU(IDM_TOOLBAR_TOGGLETOOLBARROWS, MyFrame::OnToggleToolbarRows)

    EVT_MENU(IDM_TOOLBAR_ENABLEPRINT, MyFrame::OnEnablePrint)
    EVT_MENU(IDM_TOOLBAR_DELETEPRINT, MyFrame::OnDeletePrint)
    EVT_MENU(IDM_TOOLBAR_INSERTPRINT, MyFrame::OnInsertPrint)
    EVT_MENU(IDM_TOOLBAR_TOGGLEHELP, MyFrame::OnToggleHelp)
    EVT_MENU(IDM_TOOLBAR_CHANGE_TOOLTIP, MyFrame::OnChangeToolTip)

    EVT_MENU_RANGE(IDM_TOOLBAR_SHOW_TEXT, IDM_TOOLBAR_SHOW_BOTH,
                   MyFrame::OnToolbarStyle)

    EVT_MENU(-1, MyFrame::OnToolLeftClick)

    EVT_COMBOBOX(ID_COMBO, MyFrame::OnCombo)

    EVT_TOOL_ENTER(ID_TOOLBAR, MyFrame::OnToolEnter)

    EVT_UPDATE_UI(wxID_COPY, MyFrame::OnUpdateCopyAndCut)
    EVT_UPDATE_UI(wxID_CUT, MyFrame::OnUpdateCopyAndCut)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

// The `main program' equivalent, creating the windows and returning the
// main frame
bool MyApp::OnInit()
{
    // Create the main frame window
    MyFrame* frame = new MyFrame((wxFrame *) NULL, -1,
                                 "wxToolBar Sample",
                                 wxPoint(100, 100), wxSize(550, 300));

    frame->Show(TRUE);

    frame->SetStatusText("Hello, wxWindows");

    SetTopWindow(frame);

    return TRUE;
}

void MyFrame::RecreateToolbar()
{
    // delete and recreate the toolbar
    wxToolBarBase *toolBar = GetToolBar();
    long style = toolBar ? toolBar->GetWindowStyle() : TOOLBAR_STYLE;

    delete toolBar;

    SetToolBar(NULL);

    style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL);
    style |= m_horzToolbar ? wxTB_HORIZONTAL : wxTB_VERTICAL;

    toolBar = CreateToolBar(style, ID_TOOLBAR);
    //toolBar->SetMargins( 4, 4 );

    // Set up toolbar
    wxBitmap toolBarBitmaps[8];

#if USE_XPM_BITMAPS
    toolBarBitmaps[0] = wxBitmap(new_xpm);
    toolBarBitmaps[1] = wxBitmap(open_xpm);
    toolBarBitmaps[2] = wxBitmap(save_xpm);
    toolBarBitmaps[3] = wxBitmap(copy_xpm);
    toolBarBitmaps[4] = wxBitmap(cut_xpm);
    toolBarBitmaps[5] = wxBitmap(paste_xpm);
    toolBarBitmaps[6] = wxBitmap(print_xpm);
    toolBarBitmaps[7] = wxBitmap(help_xpm);
#else // !USE_XPM_BITMAPS
    toolBarBitmaps[0] = wxBITMAP(new);
    toolBarBitmaps[1] = wxBITMAP(open);
    toolBarBitmaps[2] = wxBITMAP(save);
    toolBarBitmaps[3] = wxBITMAP(copy);
    toolBarBitmaps[4] = wxBITMAP(cut);
    toolBarBitmaps[5] = wxBITMAP(paste);
    toolBarBitmaps[6] = wxBITMAP(print);
    toolBarBitmaps[7] = wxBITMAP(help);
#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS

    if ( !m_smallToolbar )
    {
        int w = 2*toolBarBitmaps[0].GetWidth(),
            h = 2*toolBarBitmaps[0].GetHeight();
        for ( size_t n = 0; n < WXSIZEOF(toolBarBitmaps); n++ )
        {
            toolBarBitmaps[n] =
                wxBitmap(toolBarBitmaps[n].ConvertToImage().Scale(w, h));
        }

        toolBar->SetToolBitmapSize(wxSize(w, h));
    }

    toolBar->AddTool(wxID_NEW, _T("New"), toolBarBitmaps[0], _T("New file"));
    toolBar->AddTool(wxID_OPEN, _T("Open"), toolBarBitmaps[1], _T("Open file"));

    // neither the generic nor Motif native toolbars really support this
#if (wxUSE_TOOLBAR_NATIVE && !USE_GENERIC_TBAR) && !defined(__WXMOTIF__) && !defined(__WXX11__)
    // adding a combo to a vertical toolbar is not very smart
    if ( m_horzToolbar )
    {
        wxComboBox *combo = new wxComboBox(toolBar, ID_COMBO, "", wxDefaultPosition, wxSize(200,-1) );
        combo->Append("This");
        combo->Append("is a");
        combo->Append("combobox");
        combo->Append("in a");
        combo->Append("toolbar");
        toolBar->AddControl(combo);
    }
#endif // toolbars which don't support controls

    toolBar->AddTool(wxID_SAVE, _T("Save"), toolBarBitmaps[2], _T("Toggle button 1"), wxITEM_CHECK);
    toolBar->AddTool(wxID_COPY, _T("Copy"), toolBarBitmaps[3], _T("Toggle button 2"), wxITEM_CHECK);
    toolBar->AddTool(wxID_CUT, _T("Cut"), toolBarBitmaps[4], _T("Toggle/Untoggle help button"));
    toolBar->AddTool(wxID_PASTE, _T("Paste"), toolBarBitmaps[5], _T("Paste"));
    toolBar->AddTool(wxID_PRINT, _T("Print"), toolBarBitmaps[6], _T("Delete this tool"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_HELP, _T("Help"), toolBarBitmaps[7], _T("Help button"), wxITEM_CHECK);

    // after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes
    toolBar->Realize();

    toolBar->SetRows(m_horzToolbar ? m_rows : 10 / m_rows);
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

// Define my frame constructor
MyFrame::MyFrame(wxFrame* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
       : wxFrame(parent, id, title, pos, size, style)
{
    m_tbar = NULL;
    m_textWindow = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(-1, -1), wxTE_MULTILINE);

    m_smallToolbar = TRUE;
    m_horzToolbar = TRUE;
    m_rows = 1;

    // Give it a status line
    CreateStatusBar();

    // Give it an icon
    SetIcon(wxICON(mondrian));

    // Make a menubar
    wxMenu *tbarMenu = new wxMenu;
    tbarMenu->Append(IDM_TOOLBAR_TOGGLE_ANOTHER_TOOLBAR,
                     "Toggle &another toolbar\tCtrl-A",
                     "Show/hide another test toolbar",
                     TRUE);

    tbarMenu->Append(IDM_TOOLBAR_TOGGLETOOLBARSIZE,
                     "&Toggle toolbar size\tCtrl-S",
                     "Toggle between big/small toolbar",
                     TRUE);
    tbarMenu->Append(IDM_TOOLBAR_TOGGLETOOLBARORIENT,
                     "Toggle toolbar &orientation\tCtrl-O",
                     "Toggle toolbar orientation",
                     TRUE);
    tbarMenu->Append(IDM_TOOLBAR_TOGGLETOOLBARROWS,
                     "Toggle number of &rows\tCtrl-R",
                     "Toggle number of toolbar rows between 1 and 2",
                     TRUE);

    tbarMenu->AppendSeparator();

    tbarMenu->Append(IDM_TOOLBAR_ENABLEPRINT, "&Enable print button\tCtrl-E", "");
    tbarMenu->Append(IDM_TOOLBAR_DELETEPRINT, "&Delete print button\tCtrl-D", "");
    tbarMenu->Append(IDM_TOOLBAR_INSERTPRINT, "&Insert print button\tCtrl-I", "");
    tbarMenu->Append(IDM_TOOLBAR_TOGGLEHELP, "Toggle &help button\tCtrl-T", "");
    tbarMenu->AppendSeparator();
    tbarMenu->Append(IDM_TOOLBAR_CHANGE_TOOLTIP, "Change tool tip", "");
    tbarMenu->AppendSeparator();
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_SHOW_TEXT, "Show &text\tAlt-T");
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_SHOW_ICONS, "Show &icons\tAlt-I");
    tbarMenu->AppendRadioItem(IDM_TOOLBAR_SHOW_BOTH, "Show &both\tAlt-B");

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit toolbar sample" );

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, "&About", "About toolbar sample");

    wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(tbarMenu, "&Toolbar");
    menuBar->Append(helpMenu, "&Help");

    // Associate the menu bar with the frame
    SetMenuBar(menuBar);

    menuBar->Check(IDM_TOOLBAR_SHOW_BOTH, TRUE);

    // Create the toolbar
    RecreateToolbar();
}

#if USE_GENERIC_TBAR

wxToolBar* MyFrame::OnCreateToolBar(long style,
                                    wxWindowID id,
                                    const wxString& name)
{
    return (wxToolBar *)new wxToolBarSimple(this, id,
                                            wxDefaultPosition, wxDefaultSize,
                                            style, name);
}

#endif // USE_GENERIC_TBAR

void MyFrame::LayoutChildren()
{
    wxSize size = GetClientSize();

    int offset;
    if ( m_tbar )
    {
        m_tbar->SetSize(-1, size.y);
        m_tbar->Move(0, 0);

        offset = m_tbar->GetSize().x;
    }
    else
    {
        offset = 0;
    }

    m_textWindow->SetSize(offset, 0, size.x - offset, size.y);
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    if ( m_tbar )
    {
        LayoutChildren();
    }
    else
    {
        event.Skip();
    }
}

void MyFrame::OnToggleAnotherToolbar(wxCommandEvent& WXUNUSED(event))
{
    if ( m_tbar )
    {
        delete m_tbar;
        m_tbar = NULL;
    }
    else
    {
        long style = GetToolBar()->GetWindowStyle();
        style &= ~wxTB_HORIZONTAL;
        style |= wxTB_VERTICAL;

        m_tbar = new wxToolBar(this, -1,
                               wxDefaultPosition, wxDefaultSize,
                               style);

        m_tbar->AddRadioTool(wxID_NEW, _T("First"), wxBITMAP(new));
        m_tbar->AddRadioTool(wxID_OPEN, _T("Second"), wxBITMAP(open));
        m_tbar->AddRadioTool(wxID_SAVE, _T("Third"), wxBITMAP(save));
        m_tbar->AddSeparator();
        m_tbar->AddTool(wxID_HELP, _T("Help"), wxBITMAP(help));

        m_tbar->Realize();
    }

    LayoutChildren();
}

void MyFrame::OnToggleToolbarSize(wxCommandEvent& WXUNUSED(event))
{
    m_smallToolbar = !m_smallToolbar;

    RecreateToolbar();
}

void MyFrame::OnToggleToolbarRows(wxCommandEvent& WXUNUSED(event))
{
    // m_rows may be only 1 or 2
    m_rows = 3 - m_rows;

    GetToolBar()->SetRows(m_horzToolbar ? m_rows : 10 / m_rows);

    //RecreateToolbar(); -- this is unneeded
}

void MyFrame::OnToggleToolbarOrient(wxCommandEvent& WXUNUSED(event))
{
    m_horzToolbar = !m_horzToolbar;

    RecreateToolbar();
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
        if ( event.GetExtraLong() != 0 )
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

    if (event.GetId() == wxID_PRINT)
    {
        DoDeletePrint();
    }
}

void MyFrame::OnCombo(wxCommandEvent& event)
{
    wxLogStatus(_T("Combobox string '%s' selected"), event.GetString().c_str());
}

void MyFrame::DoEnablePrint()
{
    wxToolBarBase *tb = GetToolBar();
    if (tb->GetToolEnabled(wxID_PRINT))
        tb->EnableTool( wxID_PRINT, FALSE );
    else
        tb->EnableTool( wxID_PRINT, TRUE );
}

void MyFrame::DoDeletePrint()
{
    wxToolBarBase *tb = GetToolBar();

    tb->DeleteTool( wxID_PRINT );
}

void MyFrame::DoToggleHelp()
{
    wxToolBarBase *tb = GetToolBar();
    tb->ToggleTool( wxID_HELP, !tb->GetToolState( wxID_HELP ) );
}

void MyFrame::OnUpdateCopyAndCut(wxUpdateUIEvent& event)
{
    event.Enable( m_textWindow->CanCopy() );
}

void MyFrame::OnChangeToolTip(wxCommandEvent& WXUNUSED(event))
{
    GetToolBar()->SetToolShortHelp(wxID_NEW, _T("New toolbar button"));
}

void MyFrame::OnToolbarStyle(wxCommandEvent& event)
{
    long style = GetToolBar()->GetWindowStyle();
    style &= ~(wxTB_NOICONS | wxTB_TEXT);

    switch ( event.GetId() )
    {
        case IDM_TOOLBAR_SHOW_TEXT:
            style |= wxTB_NOICONS | wxTB_TEXT;
            break;

        case IDM_TOOLBAR_SHOW_ICONS:
            // nothing to do
            break;

        case IDM_TOOLBAR_SHOW_BOTH:
            style |= wxTB_TEXT;
    }

    GetToolBar()->SetWindowStyle(style);
}

void MyFrame::OnInsertPrint(wxCommandEvent& WXUNUSED(event))
{
    wxBitmap bmp = wxBITMAP(print);

    GetToolBar()->InsertTool(0, wxID_PRINT, bmp, wxNullBitmap,
                             FALSE, (wxObject *) NULL,
                             "Delete this tool",
                             "This button was inserted into the toolbar");

    GetToolBar()->Realize();
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

