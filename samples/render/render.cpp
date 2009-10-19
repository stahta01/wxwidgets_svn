/////////////////////////////////////////////////////////////////////////////
// Name:        render.cpp
// Purpose:     Render wxWidgets sample
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04.08.03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
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
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/panel.h"
    #include "wx/menu.h"
    #include "wx/textdlg.h"
    #include "wx/log.h"
    #include "wx/msgdlg.h"
    #include "wx/icon.h"
    #include "wx/image.h"
#endif

#include "wx/apptrait.h"
#include "wx/renderer.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// A renderer class draws the header buttons in a "special" way
class MyRenderer : public wxDelegateRendererNative
{
public:
    MyRenderer() : wxDelegateRendererNative(wxRendererNative::GetDefault()) { }

    virtual int DrawHeaderButton(wxWindow *WXUNUSED(win),
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int WXUNUSED(flags) = 0,
                                  wxHeaderSortIconType WXUNUSED(sortArrow) = wxHDR_SORT_ICON_NONE,
                                  wxHeaderButtonParams* WXUNUSED(params) = NULL)
    {
        wxDCBrushChanger setBrush(dc, *wxBLUE_BRUSH);
        wxDCTextColourChanger setFgCol(dc, *wxWHITE);
        dc.DrawRoundedRectangle(rect, 5);
        dc.DrawLabel(wxT("MyRenderer"), wxNullBitmap, rect, wxALIGN_CENTER);
        return rect.width;
    }
};

// To use a different renderer from the very beginning we must override
// wxAppTraits method creating the renderer (another, simpler, alternative is
// to call wxRendererNative::Set() a.s.a.p. which should work in 99% of the
// cases, but we show this here just for completeness)
class MyTraits : public wxGUIAppTraits
{
    virtual wxRendererNative *CreateRenderer()
    {
        // it will be deleted on program shutdown by wxWidgets itself
        return new MyRenderer;
    }
};

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    virtual bool OnInit();

    // if we want MyTraits to be used we must override CreateTraits()
    virtual wxAppTraits *CreateTraits() { return new MyTraits; }
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame();
    virtual ~MyFrame();

private:
    // event handlers (these functions should _not_ be virtual)
    void OnDrawDisabled(wxCommandEvent& event)
        { OnToggleDrawFlag(event, wxCONTROL_DISABLED); }
    void OnDrawFocused(wxCommandEvent& event)
        { OnToggleDrawFlag(event, wxCONTROL_FOCUSED); }
    void OnDrawPressed(wxCommandEvent& event)
        { OnToggleDrawFlag(event, wxCONTROL_PRESSED); }
    void OnDrawChecked(wxCommandEvent& event)
        { OnToggleDrawFlag(event, wxCONTROL_CHECKED); }
    void OnDrawHot(wxCommandEvent& event)
        { OnToggleDrawFlag(event, wxCONTROL_CURRENT); }

#if wxUSE_DYNLIB_CLASS
    void OnLoad(wxCommandEvent& event);
    void OnUnload(wxCommandEvent& event);
#endif // wxUSE_DYNLIB_CLASS
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnToggleDrawFlag(wxCommandEvent& event, int flag);

    class MyPanel *m_panel;

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// a very simple class just to have something to draw on
class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow *parent) : wxPanel(parent) { m_flags = 0; }

    int GetFlags() const { return m_flags; }
    void SetFlags(int flags) { m_flags = flags; }

private:
    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(this);

        wxRendererNative& renderer = wxRendererNative::Get();

        int x1 = 10,    // text offset
            x2 = 200,   // drawing offset
            y = 10;

        const int lineHeight = dc.GetCharHeight();
        dc.DrawText("Demonstration of various wxRenderer functions:", x1, y);
        y += lineHeight;
        wxString flagsString;
        if ( m_flags & wxCONTROL_DISABLED )
            flagsString += "wxCONTROL_DISABLED ";
        if ( m_flags & wxCONTROL_FOCUSED )
            flagsString += "wxCONTROL_FOCUSED ";
        if ( m_flags & wxCONTROL_PRESSED )
            flagsString += "wxCONTROL_PRESSED ";
        if ( m_flags & wxCONTROL_CURRENT )
            flagsString += "wxCONTROL_CURRENT ";
        if ( m_flags & wxCONTROL_CHECKED )
            flagsString += "wxCONTROL_CHECKED ";
        if ( flagsString.empty() )
            flagsString = "(none)";
        dc.DrawText("Using flags: " + flagsString, x1, y);
        y += lineHeight*3;

        dc.DrawText("DrawHeaderButton() (overridden)", x1, y);
        const wxCoord heightHdr = renderer.GetHeaderButtonHeight(this);
        renderer.DrawHeaderButton(this, dc,
                                  wxRect(x2, y, 100, heightHdr), m_flags);
        y += lineHeight + heightHdr;

        dc.DrawText("DrawCheckBox()", x1, y);
        const wxSize sizeCheck = renderer.GetCheckBoxSize(this);
        renderer.DrawCheckBox(this, dc,
                              wxRect(wxPoint(x2, y), sizeCheck), m_flags);
        y += lineHeight + sizeCheck.y;

        dc.DrawText("DrawRadioBitmap()", x1, y);
        renderer.DrawRadioBitmap(this, dc,
                                 wxRect(wxPoint(x2, y), sizeCheck), m_flags);
        y += lineHeight + sizeCheck.y;

        dc.DrawText("DrawTreeItemButton()", x1, y);
        renderer.DrawTreeItemButton(this, dc,
                                    wxRect(x2, y, 20, 20), m_flags);
        y += lineHeight + 20;

#ifdef wxHAS_DRAW_TITLE_BAR_BITMAP
        dc.DrawText("DrawTitleBarBitmap()", x1, y);
        wxRect rBtn(x2, y, 21, 21);
        renderer.DrawTitleBarBitmap(this, dc, rBtn,
                                    wxTITLEBAR_BUTTON_HELP, m_flags);
        rBtn.x += 2*rBtn.width;
        renderer.DrawTitleBarBitmap(this, dc, rBtn,
                                    wxTITLEBAR_BUTTON_ICONIZE, m_flags);
        rBtn.x += 2*rBtn.width;
        renderer.DrawTitleBarBitmap(this, dc, rBtn,
                                    wxTITLEBAR_BUTTON_RESTORE, m_flags);
        rBtn.x += 2*rBtn.width;
        renderer.DrawTitleBarBitmap(this, dc, rBtn,
                                    wxTITLEBAR_BUTTON_MAXIMIZE, m_flags);
        rBtn.x += 2*rBtn.width;
        renderer.DrawTitleBarBitmap(this, dc, rBtn,
                                    wxTITLEBAR_BUTTON_CLOSE, m_flags);

        y += lineHeight + rBtn.height;
#endif // wxHAS_DRAW_TITLE_BAR_BITMAP
    }

    int m_flags;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
    EVT_PAINT(MyPanel::OnPaint)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // our menu items
    Render_DrawDisabled = 100,
    Render_DrawFocused,
    Render_DrawPressed,
    Render_DrawChecked,
    Render_DrawHot,

#if wxUSE_DYNLIB_CLASS
    Render_Load,
    Render_Unload,
#endif // wxUSE_DYNLIB_CLASS

    // standard menu items
    Render_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Render_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Render_DrawDisabled, MyFrame::OnDrawDisabled)
    EVT_MENU(Render_DrawFocused, MyFrame::OnDrawFocused)
    EVT_MENU(Render_DrawPressed, MyFrame::OnDrawPressed)
    EVT_MENU(Render_DrawChecked, MyFrame::OnDrawChecked)
    EVT_MENU(Render_DrawHot, MyFrame::OnDrawHot)

#if wxUSE_DYNLIB_CLASS
    EVT_MENU(Render_Load,  MyFrame::OnLoad)
    EVT_MENU(Render_Unload,MyFrame::OnUnload)
#endif // wxUSE_DYNLIB_CLASS
    EVT_MENU(Render_Quit,  MyFrame::OnQuit)

    EVT_MENU(Render_About, MyFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

#ifdef __WXOSX__
    // currently the images used by DrawTitleBarBitmap() are hard coded as PNG
    // images inside the library itself so we need to enable PNG support to use
    // this function
    wxImage::AddHandler(new wxPNGHandler);
#endif // OS X

    // create the main application window
    new MyFrame;

    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame()
       : wxFrame(NULL,
                 wxID_ANY,
                 wxT("Render wxWidgets Sample"),
                 wxPoint(50, 50),
                 wxSize(450, 340))
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->AppendCheckItem(Render_DrawDisabled,
                              "Draw in &disabled state\tCtrl-D");
    menuFile->AppendCheckItem(Render_DrawFocused,
                              "Draw in &focused state\tCtrl-F");
    menuFile->AppendCheckItem(Render_DrawPressed,
                              "Draw in &pressed state\tCtrl-P");
    menuFile->AppendCheckItem(Render_DrawChecked,
                              "Draw in &checked state\tCtrl-C");
    menuFile->AppendCheckItem(Render_DrawHot,
                              "Draw in &hot state\tCtrl-H");
    menuFile->AppendSeparator();
#if wxUSE_DYNLIB_CLASS
    menuFile->Append(Render_Load, wxT("&Load renderer...\tCtrl-L"));
    menuFile->Append(Render_Unload, wxT("&Unload renderer\tCtrl-U"));
    menuFile->AppendSeparator();
#endif // wxUSE_DYNLIB_CLASS
    menuFile->Append(Render_Quit);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Render_About);

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

    m_panel = new MyPanel(this);

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to wxWidgets!"));
#endif // wxUSE_STATUSBAR

    Show();
}

MyFrame::~MyFrame()
{
    delete wxRendererNative::Set(NULL);
}


// event handlers

void MyFrame::OnToggleDrawFlag(wxCommandEvent& event, int flag)
{
    int flags = m_panel->GetFlags();
    if ( event.IsChecked() )
        flags |= flag;
    else
        flags &= ~flag;

    m_panel->SetFlags(flags);
    m_panel->Refresh();
}

#if wxUSE_DYNLIB_CLASS

void MyFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
    static wxString s_name = wxT("renddll");

    wxString name = wxGetTextFromUser
                    (
                        wxT("Name of the renderer to load:"),
                        wxT("Render wxWidgets Sample"),
                        s_name,
                        this
                    );
    if ( name.empty() )
    {
        // cancelled
        return;
    }

    s_name = name;

    wxRendererNative *renderer = wxRendererNative::Load(name);
    if ( !renderer )
    {
        wxLogError(wxT("Failed to load renderer \"%s\"."), name.c_str());
    }
    else // loaded ok
    {
        delete wxRendererNative::Set(renderer);

        m_panel->Refresh();

        wxLogStatus(this, wxT("Successfully loaded the renderer \"%s\"."),
                    name.c_str());
    }
}

void MyFrame::OnUnload(wxCommandEvent& WXUNUSED(event))
{
    wxRendererNative *renderer = wxRendererNative::Set(NULL);
    if ( renderer )
    {
        delete renderer;

        m_panel->Refresh();

        wxLogStatus(this, wxT("Unloaded the previously loaded renderer."));
    }
    else
    {
        wxLogWarning(wxT("No renderer to unload."));
    }
}

#endif // wxUSE_DYNLIB_CLASS

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Render sample shows how to use custom renderers.\n")
                 wxT("\n")
                 wxT("(c) 2003 Vadim Zeitlin"),
                 wxT("About Render wxWidgets Sample"),
                 wxOK | wxICON_INFORMATION, this);
}

