/////////////////////////////////////////////////////////////////////////////
// Name:        demo.cpp
// Purpose:     wxHelpController demo
// Author:      Karsten Ballueder
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Karsten Ballueder, Julian Smart
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
#   pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#   include "wx/image.h"
#   include "wx/help.h"

// define this to 1 to use HTML help even under Windows (by default, Windows
// version will use WinHelp).
// Please also see samples/html/helpview.

#define USE_HTML_HELP 1

// Use old-style HTML help if 1
#define USE_OLD_HTML_HELP 0

#if !wxUSE_HTML
#undef USE_HTML_HELP
#define USE_HTML_HELP 0
#endif

#if USE_HTML_HELP
#include <wx/filesys.h>
#include <wx/fs_zip.h>

#if USE_OLD_HTML_HELP
#include "wx/generic/helpwxht.h"
#endif

#include "wx/html/helpctrl.h"
#endif

#if wxUSE_MS_HTML_HELP
#include "wx/msw/helpchm.h"
#endif

// ----------------------------------------------------------------------------
// ressources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "mondrian.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    wxHelpController& GetHelpController() { return m_help; }

#if USE_HTML_HELP
#if USE_OLD_HTML_HELP
    wxHelpControllerHtml& GetHtmlHelpController() { return m_htmlHelp; }
#endif
    wxHtmlHelpController& GetAdvancedHtmlHelpController() { return m_advancedHtmlHelp; }
#endif
#if wxUSE_MS_HTML_HELP
    wxCHMHelpController& GetMSHtmlHelpController() { return m_msHtmlHelp; }
#endif

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnHtmlHelp(wxCommandEvent& event);
    void OnAdvancedHtmlHelp(wxCommandEvent& event);
    void OnMSHtmlHelp(wxCommandEvent& event);

    void OnContextHelp(wxHelpEvent& event);
    void OnShowContextHelp(wxCommandEvent& event);

    void ShowHelp(int commandId, wxHelpControllerBase& helpController);

private:
   wxHelpController         m_help;

#if USE_HTML_HELP
#if USE_OLD_HTML_HELP
   wxHelpControllerHtml     m_htmlHelp;
#endif
   wxHtmlHelpController     m_advancedHtmlHelp;
#endif

#if wxUSE_MS_HTML_HELP
    wxCHMHelpController     m_msHtmlHelp;
#endif

    // any class wishing to process wxWindows events must use this macro
   DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    HelpDemo_Quit = 1,
    HelpDemo_Help_Index,
    HelpDemo_Help_Classes,
    HelpDemo_Help_Functions,
    HelpDemo_Help_Help,
    HelpDemo_Help_Search,
    HelpDemo_Help_ContextHelp,

    HelpDemo_Html_Help_Index,
    HelpDemo_Html_Help_Classes,
    HelpDemo_Html_Help_Functions,
    HelpDemo_Html_Help_Help,
    HelpDemo_Html_Help_Search,

    HelpDemo_Advanced_Html_Help_Index,
    HelpDemo_Advanced_Html_Help_Classes,
    HelpDemo_Advanced_Html_Help_Functions,
    HelpDemo_Advanced_Html_Help_Help,
    HelpDemo_Advanced_Html_Help_Search,

    HelpDemo_MS_Html_Help_Index,
    HelpDemo_MS_Html_Help_Classes,
    HelpDemo_MS_Html_Help_Functions,
    HelpDemo_MS_Html_Help_Help,
    HelpDemo_MS_Html_Help_Search,

    HelpDemo_Help_KDE,
    HelpDemo_Help_GNOME,
    HelpDemo_Help_Netscape,
    // controls start here (the numbers are, of course, arbitrary)
    HelpDemo_Text = 1000,
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(HelpDemo_Quit,  MyFrame::OnQuit)
    EVT_MENU(HelpDemo_Help_Index, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_Classes, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_Functions, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_Help, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_Search, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_ContextHelp, MyFrame::OnShowContextHelp)

    EVT_HELP(-1, MyFrame::OnContextHelp)

    EVT_MENU(HelpDemo_Html_Help_Index, MyFrame::OnHtmlHelp)
    EVT_MENU(HelpDemo_Html_Help_Classes, MyFrame::OnHtmlHelp)
    EVT_MENU(HelpDemo_Html_Help_Functions, MyFrame::OnHtmlHelp)
    EVT_MENU(HelpDemo_Html_Help_Help, MyFrame::OnHtmlHelp)
    EVT_MENU(HelpDemo_Html_Help_Search, MyFrame::OnHtmlHelp)

    EVT_MENU(HelpDemo_Advanced_Html_Help_Index, MyFrame::OnAdvancedHtmlHelp)
    EVT_MENU(HelpDemo_Advanced_Html_Help_Classes, MyFrame::OnAdvancedHtmlHelp)
    EVT_MENU(HelpDemo_Advanced_Html_Help_Functions, MyFrame::OnAdvancedHtmlHelp)
    EVT_MENU(HelpDemo_Advanced_Html_Help_Help, MyFrame::OnAdvancedHtmlHelp)
    EVT_MENU(HelpDemo_Advanced_Html_Help_Search, MyFrame::OnAdvancedHtmlHelp)

    EVT_MENU(HelpDemo_MS_Html_Help_Index, MyFrame::OnMSHtmlHelp)
    EVT_MENU(HelpDemo_MS_Html_Help_Classes, MyFrame::OnMSHtmlHelp)
    EVT_MENU(HelpDemo_MS_Html_Help_Functions, MyFrame::OnMSHtmlHelp)
    EVT_MENU(HelpDemo_MS_Html_Help_Help, MyFrame::OnMSHtmlHelp)
    EVT_MENU(HelpDemo_MS_Html_Help_Search, MyFrame::OnMSHtmlHelp)

    EVT_MENU(HelpDemo_Help_KDE, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_GNOME, MyFrame::OnHelp)
    EVT_MENU(HelpDemo_Help_Netscape, MyFrame::OnHelp)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
#if wxUSE_HTML
#if wxUSE_GIF
    // Required for images in the online documentation
    wxImage::AddHandler(new wxGIFHandler);

    // Required for advanced HTML help
#if wxUSE_STREAMS && wxUSE_ZIPSTREAM && wxUSE_ZLIB
      wxFileSystem::AddHandler(new wxZipFSHandler);
#endif

#endif
#endif

    // Create the main application window
    MyFrame *frame = new MyFrame("HelpDemo wxWindows App",
                                 wxPoint(50, 50), wxSize(450, 340));

    frame->Show(TRUE);
    SetTopWindow(frame);

    // initialise the help system: this means that we'll use doc.hlp file under
    // Windows and that the HTML docs are in the subdirectory doc for platforms
    // using HTML help
    if ( !frame->GetHelpController().Initialize("doc") )
    {
        wxLogError("Cannot initialize the help system, aborting.");

        return FALSE;
    }

#if USE_HTML_HELP
    // initialise the standard HTML help system: this means that the HTML docs are in the
    // subdirectory doc for platforms using HTML help
#if USE_OLD_HTML_HELP
    if ( !frame->GetHtmlHelpController().Initialize("doc") )
    {
        wxLogError("Cannot initialize the HTML help system, aborting.");

        return FALSE;
    }
#endif

    // initialise the advanced HTML help system: this means that the HTML docs are in .htb
    // (zipped) form
    if ( !frame->GetAdvancedHtmlHelpController().Initialize("doc") )
    {
        wxLogError("Cannot initialize the advanced HTML help system, aborting.");

        return FALSE;
    }
#endif

#if wxUSE_MS_HTML_HELP
    if ( !frame->GetMSHtmlHelpController().Initialize("doc") )
    {
        wxLogError("Cannot initialize the MS HTML help system, aborting.");

        return FALSE;
    }
#endif

    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    menuFile->Append(HelpDemo_Help_Index, "&Help Index...");
    menuFile->Append(HelpDemo_Help_Classes, "&Help on Classes...");
    menuFile->Append(HelpDemo_Help_Functions, "&Help on Functions...");
    menuFile->Append(HelpDemo_Help_ContextHelp, "&Context Help...");
    menuFile->Append(HelpDemo_Help_Help, "&About Help Demo...");
    menuFile->Append(HelpDemo_Help_Search, "&Search help...");
#if USE_HTML_HELP
#if USE_OLD_HTML_HELP
    menuFile->AppendSeparator();
    menuFile->Append(HelpDemo_Html_Help_Index, "HTML &Help Index...");
    menuFile->Append(HelpDemo_Html_Help_Classes, "HTML &Help on Classes...");
    menuFile->Append(HelpDemo_Html_Help_Functions, "HTML &Help on Functions...");
    menuFile->Append(HelpDemo_Html_Help_Help, "HTML &About Help Demo...");
    menuFile->Append(HelpDemo_Html_Help_Search, "HTML &Search help...");
#endif
    menuFile->AppendSeparator();
    menuFile->Append(HelpDemo_Advanced_Html_Help_Index, "Advanced HTML &Help Index...");
    menuFile->Append(HelpDemo_Advanced_Html_Help_Classes, "Advanced HTML &Help on Classes...");
    menuFile->Append(HelpDemo_Advanced_Html_Help_Functions, "Advanced HTML &Help on Functions...");
    menuFile->Append(HelpDemo_Advanced_Html_Help_Help, "Advanced HTML &About Help Demo...");
    menuFile->Append(HelpDemo_Advanced_Html_Help_Search, "Advanced HTML &Search help...");
#endif

#if wxUSE_MS_HTML_HELP
    menuFile->AppendSeparator();
    menuFile->Append(HelpDemo_MS_Html_Help_Index, "MS HTML &Help Index...");
    menuFile->Append(HelpDemo_MS_Html_Help_Classes, "MS HTML &Help on Classes...");
    menuFile->Append(HelpDemo_MS_Html_Help_Functions, "MS HTML &Help on Functions...");
    menuFile->Append(HelpDemo_MS_Html_Help_Help, "MS HTML &About Help Demo...");
    menuFile->Append(HelpDemo_MS_Html_Help_Search, "MS HTML &Search help...");
#endif

#ifndef __WXMSW__
#if !wxUSE_HTML
    menuFile->AppendSeparator();
    menuFile->Append(HelpDemo_Help_KDE, "Use &KDE");
    menuFile->Append(HelpDemo_Help_GNOME, "Use &GNOME");
    menuFile->Append(HelpDemo_Help_Netscape, "Use &Netscape");
#endif
#endif
    menuFile->AppendSeparator();
    menuFile->Append(HelpDemo_Quit, "E&xit");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar();
    SetStatusText("Welcome to wxWindows!");

    // now create some controls

    // a panel first - if there were several controls, it would allow us to
    // navigate between them from the keyboard
    wxPanel *panel = new wxPanel(this, -1, wxPoint(0, 0), wxSize(400, 200));

    // and a static control whose parent is the panel
    (void)new wxStaticText(panel, -1, "Hello, world!", wxPoint(10, 10));
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnHelp(wxCommandEvent& event)
{
    ShowHelp(event.GetId(), m_help);
}

void MyFrame::OnShowContextHelp(wxCommandEvent& event)
{
    // This starts context help mode, then the user
    // clicks on a window to send a help message
    wxContextHelp contextHelp(this);
}

void MyFrame::OnContextHelp(wxHelpEvent& event)
{
    // In a real app, if we didn't recognise this ID, we should call event.Skip()
    wxString msg;
    msg.Printf(wxT("We should now display help for window %d"), event.GetId());
    wxMessageBox(msg);
}

void MyFrame::OnHtmlHelp(wxCommandEvent& event)
{
#if USE_HTML_HELP && USE_OLD_HTML_HELP
    ShowHelp(event.GetId(), m_htmlHelp);
#endif
}

void MyFrame::OnAdvancedHtmlHelp(wxCommandEvent& event)
{
#if USE_HTML_HELP
    ShowHelp(event.GetId(), m_advancedHtmlHelp);
#endif
}

void MyFrame::OnMSHtmlHelp(wxCommandEvent& event)
{
#if wxUSE_MS_HTML_HELP
    ShowHelp(event.GetId(), m_msHtmlHelp);
#endif
}

/*
 Notes: ShowHelp uses section ids for displaying particular topics,
 but you might want to use a unique keyword to display a topic, instead.

 Section ids are specified as follows for the different formats.

 WinHelp

   The [MAP] section specifies the topic to integer id mapping, e.g.

   [MAP]
   #define intro       100
   #define functions   1
   #define classes     2
   #define about       3

   The identifier name corresponds to the label used for that topic.
   You could also put these in a .h file and #include it in both the MAP
   section and your C++ source.

   Note that Tex2RTF doesn't currently generate the MAP section automatically.

 MS HTML Help

   The [MAP] section specifies the HTML filename root to integer id mapping, e.g.

   [MAP]
   #define doc1       100
   #define doc3       1
   #define doc2       2
   #define doc4       3

   The identifier name corresponds to the HTML filename used for that topic.
   You could also put these in a .h file and #include it in both the MAP
   section and your C++ source.

   Note that Tex2RTF doesn't currently generate the MAP section automatically.

 Simple wxHTML Help and External HTML Help

   A wxhelp.map file is used, for example:

   0 wx.htm             ; wxWindows: Help index; additional keywords like overview
   1 wx204.htm          ; wxWindows Function Reference
   2 wx34.htm           ; wxWindows Class Reference

   Note that Tex2RTF doesn't currently generate the MAP section automatically.

 Advanced HTML Help

   An extension to the .hhc file format is used, specifying a new parameter
   with name="ID":

   <OBJECT type="text/sitemap">
   <param name="Local" value="doc2.htm#classes">
   <param name="Name" value="Classes">
   <param name="ID" value=2>
   </OBJECT>

   Again, this is not generated automatically by Tex2RTF, though it could
   be added quite easily.

   Unfortunately adding the ID parameters appears to interfere with MS HTML Help,
   so you should not try to compile a .chm file from a .hhc file with
   this extension, or the contents will be messed up.
 */

void MyFrame::ShowHelp(int commandId, wxHelpControllerBase& helpController)
{
   switch(commandId)
   {
   case HelpDemo_Help_Classes:
   case HelpDemo_Html_Help_Classes:
   case HelpDemo_Advanced_Html_Help_Classes:
   case HelpDemo_MS_Html_Help_Classes:
      helpController.DisplaySection(2);
      //helpController.DisplaySection("Classes"); // An alternative form for most controllers

      break;
   case HelpDemo_Help_Functions:
   case HelpDemo_Html_Help_Functions:
   case HelpDemo_Advanced_Html_Help_Functions:
   case HelpDemo_MS_Html_Help_Functions:
      helpController.DisplaySection(1);
      //helpController.DisplaySection("Functions"); // An alternative form for most controllers
      break;
   case HelpDemo_Help_Help:
   case HelpDemo_Html_Help_Help:
   case HelpDemo_Advanced_Html_Help_Help:
   case HelpDemo_MS_Html_Help_Help:
      helpController.DisplaySection(3);
      //helpController.DisplaySection("About"); // An alternative form for most controllers
      break;

   case HelpDemo_Help_Search:
   case HelpDemo_Html_Help_Search:
   case HelpDemo_Advanced_Html_Help_Search:
   case HelpDemo_MS_Html_Help_Search:
   {
      wxString key = wxGetTextFromUser("Search for?",
                                       "Search help for keyword",
                                       "",
                                       this);
      if(! key.IsEmpty())
         helpController.KeywordSearch(key);
   }
   break;

   case HelpDemo_Help_Index:
   case HelpDemo_Html_Help_Index:
   case HelpDemo_Advanced_Html_Help_Index:
   case HelpDemo_MS_Html_Help_Index:
      helpController.DisplayContents();
      break;

   // These three calls are only used by wxExtHelpController

   case HelpDemo_Help_KDE:
      helpController.SetViewer("kdehelp");
      break;
   case HelpDemo_Help_GNOME:
      helpController.SetViewer("gnome-help-browser");
      break;
   case HelpDemo_Help_Netscape:
      helpController.SetViewer("netscape", wxHELP_NETSCAPE);
      break;

   default:
      break;
   }
}

