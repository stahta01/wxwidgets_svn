/////////////////////////////////////////////////////////////////////////////
// Name:        internat.cpp
// Purpose:     Demonstrates internationalisation (i18n) support
// Author:      Vadim Zeitlin/Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/intl.h"
#include "wx/file.h"
#include "wx/log.h"

#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__)
#include "mondrian.xpm"
#endif

// Define a new application type
class MyApp: public wxApp
{
public:
    virtual bool OnInit();

protected:
    wxLocale m_locale; // locale we'll be using
};

// Define a new frame type
class MyFrame: public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxChar *title,
        const wxPoint& pos, const wxSize& size, wxLocale& m_locale);

public:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPlay(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

    wxLocale& m_locale;
};

// ID for the menu commands
enum
{
    MINIMAL_QUIT = 1,
    MINIMAL_TEXT,
    MINIMAL_TEST,
    MINIMAL_OPEN
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(MINIMAL_QUIT, MyFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(MINIMAL_TEST, MyFrame::OnPlay)
    EVT_MENU(MINIMAL_OPEN, MyFrame::OnOpen)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)


// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
    const wxString langs[] =
    {
        _T("(System default)"),
        _T("French"),
        _T("German"),
        _T("Russian"),
        _T("English"),
        _T("English (U.S.)")
    };

    SetExitOnFrameDelete(FALSE);
    int lng = wxGetSingleChoiceIndex(_T("Please choose language:"), _T("Language"), 
                                   WXSIZEOF(langs), langs);
    SetExitOnFrameDelete(TRUE);

    switch (lng)
    {
        case 0 : m_locale.Init(wxLANGUAGE_DEFAULT); break;
        case 1 : m_locale.Init(wxLANGUAGE_FRENCH); break;
        case 2 : m_locale.Init(wxLANGUAGE_GERMAN); break;
        case 3 : m_locale.Init(wxLANGUAGE_RUSSIAN); break;
        case 4 : m_locale.Init(wxLANGUAGE_ENGLISH); break;
        case -1:
        case 5 : m_locale.Init(wxLANGUAGE_ENGLISH_US); break;
    }

    // Initialize the catalogs we'll be using
    /* not needed any more, done in wxLocale ctor
    m_locale.AddCatalog("wxstd");      // 1) for library messages
    */
    m_locale.AddCatalog(wxT("internat"));      // 2) our private one
    /* this catalog is installed in standard location on Linux systems,
     it might not be installed on yours - just ignore the errrors
     or comment out this line then */
#ifdef __LINUX__
    {
    wxLogNull noLog;
    m_locale.AddCatalog(_T("fileutils"));  // 3) and another just for testing
    }
#endif

    // Create the main frame window
    MyFrame *frame = new MyFrame( (wxFrame *) NULL,
        _("International wxWindows App"), wxPoint(50, 50), wxSize(350, 60),
        m_locale);

    // Give it an icon
    frame->SetIcon(wxICON(mondrian));

    // Make a menubar
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_ABOUT, _("&About..."));
    file_menu->AppendSeparator();
    file_menu->Append(MINIMAL_QUIT, _("E&xit"));

    wxMenu *test_menu = new wxMenu;
    test_menu->Append(MINIMAL_OPEN, _("&Open bogus file"));
    test_menu->Append(MINIMAL_TEST, _("&Play a game"));

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, _("&File"));
    menu_bar->Append(test_menu, _("&Test"));
    frame->SetMenuBar(menu_bar);

    // Show the frame
    frame->Show(TRUE);
    SetTopWindow(frame);

    return TRUE;
}

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, const wxChar *title,
    const wxPoint& pos, const wxSize& size, wxLocale& l)
       : wxFrame(frame, -1, title, pos, size),
         m_locale(l)
{
    // Empty
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event) )
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString localeInfo;
    wxString locale = m_locale.GetLocale();
    wxString sysname = m_locale.GetSysName();
    wxString canname = m_locale.GetCanonicalName();

    localeInfo.Printf( _("Language: %s\nSystem locale name: %s\nCanonical locale name: %s\n"),
        locale.c_str(), sysname.c_str(), canname.c_str() );

    wxMessageDialog(this, wxString(_("I18n sample\n(c) 1998, 1999 Vadim Zeitlin and Julian Smart"))
        + wxT("\n\n") + localeInfo,
        _("About Internat"), wxOK | wxICON_INFORMATION).ShowModal();
}

void MyFrame::OnPlay(wxCommandEvent& WXUNUSED(event))
{
    wxString str = wxGetTextFromUser(_("Enter your number:"),
        _("Try to guess my number!"), wxEmptyString, this);

    if ( str.IsEmpty() ) return;

    int num;
    wxSscanf(str, wxT("%d"), &num);
    if ( num == 0 )
        str = _("You've probably entered an invalid number.");
    else if ( num == 9 )  // this message is not translated (not in catalog)
        str = _T("You've found a bug in this program!");
    else if ( num != 17 ) // a more implicit way to write _()
        str = wxGetTranslation(wxT("Bad luck! try again..."));
    else
    {
        str.Empty();
        // string must be split in two -- otherwise the translation won't be found
        str << _("Congratulations! you've won. Here is the magic phrase:")
            << _("cannot create fifo `%s'");
    }

    wxMessageBox(str, _("Result"), wxOK | wxICON_INFORMATION);
}

void MyFrame::OnOpen(wxCommandEvent&)
{
    // open a bogus file -- the error message should be also translated if you've
    // got wxstd.mo somewhere in the search path
    wxFile file(wxT("NOTEXIST.ING"));
}
