/////////////////////////////////////////////////////////////////////////////
// Name:        dialogs.cpp
// Purpose:     Common dialogs demo
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
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

#include "wx/colordlg.h"
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "wx/fontdlg.h"
#include "wx/choicdlg.h"
#include "wx/tipdlg.h"
#include "wx/progdlg.h"
#include "wx/fdrepdlg.h"
#include "wx/busyinfo.h"

#define wxTEST_GENERIC_DIALOGS_IN_MSW 0

#if defined(__WXMSW__) && wxTEST_GENERIC_DIALOGS_IN_MSW
#include "wx/generic/colrdlgg.h"
#include "wx/generic/fontdlgg.h"
#endif

#define wxUSE_DIRDLGG 0

#if !defined(__WXMSW__) || defined(wxUSE_DIRDLGG) && wxUSE_DIRDLGG
#include "wx/generic/dirdlgg.h"
#endif

#include "dialogs.h"

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(DIALOGS_CHOOSE_COLOUR,                 MyFrame::ChooseColour)
    EVT_MENU(DIALOGS_CHOOSE_FONT,                   MyFrame::ChooseFont)
    EVT_MENU(DIALOGS_LOG_DIALOG,                    MyFrame::LogDialog)
    EVT_MENU(DIALOGS_MESSAGE_BOX,                   MyFrame::MessageBox)
    EVT_MENU(DIALOGS_TEXT_ENTRY,                    MyFrame::TextEntry)
    EVT_MENU(DIALOGS_PASSWORD_ENTRY,                MyFrame::PasswordEntry)
    EVT_MENU(DIALOGS_NUM_ENTRY,                     MyFrame::NumericEntry)
    EVT_MENU(DIALOGS_SINGLE_CHOICE,                 MyFrame::SingleChoice)
    EVT_MENU(DIALOGS_MULTI_CHOICE,                  MyFrame::MultiChoice)
    EVT_MENU(DIALOGS_FILE_OPEN,                     MyFrame::FileOpen)
    EVT_MENU(DIALOGS_FILE_OPEN2,                    MyFrame::FileOpen2)
    EVT_MENU(DIALOGS_FILES_OPEN,                    MyFrame::FilesOpen)
    EVT_MENU(DIALOGS_FILE_SAVE,                     MyFrame::FileSave)
    EVT_MENU(DIALOGS_DIR_CHOOSE,                    MyFrame::DirChoose)
    EVT_MENU(DIALOGS_GENERIC_DIR_CHOOSE,            MyFrame::GenericDirChoose)
    EVT_MENU(DIALOGS_MODAL,                         MyFrame::ModalDlg)
    EVT_MENU(DIALOGS_MODELESS,                      MyFrame::ModelessDlg)
    EVT_MENU(DIALOGS_TIP,                           MyFrame::ShowTip)
#if defined(__WXMSW__) && wxTEST_GENERIC_DIALOGS_IN_MSW
    EVT_MENU(DIALOGS_CHOOSE_COLOUR_GENERIC,         MyFrame::ChooseColourGeneric)
    EVT_MENU(DIALOGS_CHOOSE_FONT_GENERIC,           MyFrame::ChooseFontGeneric)
#endif

#if wxUSE_PROGRESSDLG
    EVT_MENU(DIALOGS_PROGRESS,                      MyFrame::ShowProgress)
#endif // wxUSE_PROGRESSDLG

#if wxUSE_BUSYINFO
    EVT_MENU(DIALOGS_BUSYINFO,                      MyFrame::ShowBusyInfo)
#endif // wxUSE_BUSYINFO

#if wxUSE_FINDREPLDLG
    EVT_MENU(DIALOGS_FIND,                          MyFrame::ShowFindDialog)
    EVT_MENU(DIALOGS_REPLACE,                       MyFrame::ShowReplaceDialog)

    EVT_FIND(-1, MyFrame::OnFindDialog)
    EVT_FIND_NEXT(-1, MyFrame::OnFindDialog)
    EVT_FIND_REPLACE(-1, MyFrame::OnFindDialog)
    EVT_FIND_REPLACE_ALL(-1, MyFrame::OnFindDialog)
    EVT_FIND_CLOSE(-1, MyFrame::OnFindDialog)
#endif // wxUSE_FINDREPLDLG
    EVT_MENU(wxID_EXIT,                             MyFrame::OnExit)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyModalDialog, wxDialog)
    EVT_BUTTON(-1, MyModalDialog::OnButton)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyModelessDialog, wxDialog)
    EVT_BUTTON(DIALOGS_MODELESS_BTN, MyModelessDialog::OnButton)

    EVT_CLOSE(MyModelessDialog::OnClose)
END_EVENT_TABLE()

MyCanvas *myCanvas = (MyCanvas *) NULL;

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
#if defined(__WXGTK__) && defined(wxUSE_UNICODE)
  wxConvCurrent = &wxConvLibc;
#endif

  m_canvasTextColour = wxColour("BLACK");
  m_canvasFont = *wxNORMAL_FONT;

  // Create the main frame window
  MyFrame *frame = new MyFrame((wxFrame *) NULL, (char *) "wxWindows dialogs example", wxPoint(20, 20), wxSize(400, 300));

  // Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append(DIALOGS_CHOOSE_COLOUR, "&Choose colour");

#if defined(__WXMSW__) && wxTEST_GENERIC_DIALOGS_IN_MSW
  file_menu->Append(DIALOGS_CHOOSE_COLOUR_GENERIC, "Choose colour (&generic)");
#endif

  file_menu->AppendSeparator();
  file_menu->Append(DIALOGS_CHOOSE_FONT, "Choose &font");

#if defined(__WXMSW__) && wxTEST_GENERIC_DIALOGS_IN_MSW
  file_menu->Append(DIALOGS_CHOOSE_FONT_GENERIC, "Choose f&ont (generic)");

#endif
  file_menu->AppendSeparator();
  file_menu->Append(DIALOGS_LOG_DIALOG, "&Log dialog\tCtrl-L");
  file_menu->Append(DIALOGS_MESSAGE_BOX, "&Message box\tCtrl-M");
  file_menu->Append(DIALOGS_TEXT_ENTRY,  "Text &entry\tCtrl-E");
  file_menu->Append(DIALOGS_PASSWORD_ENTRY,  "&Password entry\tCtrl-P");
  file_menu->Append(DIALOGS_NUM_ENTRY, "&Numeric entry\tCtrl-N");
  file_menu->Append(DIALOGS_SINGLE_CHOICE,  "&Single choice\tCtrl-C");
  file_menu->Append(DIALOGS_MULTI_CHOICE,  "M&ultiple choice\tCtrl-U");
  file_menu->AppendSeparator();
  file_menu->Append(DIALOGS_TIP,  "&Tip of the day\tCtrl-T");
  file_menu->AppendSeparator();
  file_menu->Append(DIALOGS_FILE_OPEN,  "&Open file\tCtrl-O");
  file_menu->Append(DIALOGS_FILE_OPEN2,  "&Second open file\tCtrl-2");
  file_menu->Append(DIALOGS_FILES_OPEN,  "Open &files\tCtrl-Q");
  file_menu->Append(DIALOGS_FILE_SAVE,  "Sa&ve file\tCtrl-S");
  file_menu->Append(DIALOGS_DIR_CHOOSE,  "&Choose a directory\tCtrl-D");
  file_menu->Append(DIALOGS_GENERIC_DIR_CHOOSE,  "&Choose a directory (generic implementation)");
#if wxUSE_PROGRESSDLG
  file_menu->Append(DIALOGS_PROGRESS, "Pro&gress dialog\tCtrl-G");
#endif // wxUSE_PROGRESSDLG
#if wxUSE_BUSYINFO
  file_menu->Append(DIALOGS_BUSYINFO, "&Busy info dialog\tCtrl-B");
#endif // wxUSE_BUSYINFO
#if wxUSE_FINDREPLDLG
  file_menu->Append(DIALOGS_FIND, "&Find dialog\tCtrl-F", "", TRUE);
  file_menu->Append(DIALOGS_REPLACE, "Find and &replace dialog\tShift-Ctrl-F", "", TRUE);
#endif // wxUSE_FINDREPLDLG
  file_menu->AppendSeparator();
  file_menu->Append(DIALOGS_MODAL, "Mo&dal dialog\tCtrl-W");
  file_menu->Append(DIALOGS_MODELESS, "Modeless &dialog\tCtrl-Z", "", TRUE);
  file_menu->AppendSeparator();
  file_menu->Append(wxID_EXIT, "E&xit\tAlt-X");
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, "&File");
  frame->SetMenuBar(menu_bar);

  myCanvas = new MyCanvas(frame);
  myCanvas->SetBackgroundColour(*wxWHITE);

  frame->Centre(wxBOTH);

  // Show the frame
  frame->Show(TRUE);

  SetTopWindow(frame);

  return TRUE;
}

// My frame constructor
MyFrame::MyFrame(wxWindow *parent,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size)
       : wxFrame(parent, -1, title, pos, size)
{
    m_dialog = (MyModelessDialog *)NULL;

#if wxUSE_FINDREPLDLG
    m_dlgFind =
    m_dlgReplace = NULL;
#endif
   
    CreateStatusBar();
}

void MyFrame::ChooseColour(wxCommandEvent& WXUNUSED(event) )
{
    wxColour col = myCanvas->GetBackgroundColour();

    wxColourData data;
    data.SetColour(col);
    data.SetChooseFull(TRUE);
    for (int i = 0; i < 16; i++)
    {
        wxColour colour(i*16, i*16, i*16);
        data.SetCustomColour(i, colour);
    }

    wxColourDialog dialog(this, &data);
    dialog.SetTitle("Choose the background colour");
    if (dialog.ShowModal() == wxID_OK)
    {
        wxColourData retData = dialog.GetColourData();
        col = retData.GetColour();
        myCanvas->SetBackgroundColour(col);
        myCanvas->Clear();
        myCanvas->Refresh();
    }
}

void MyFrame::ChooseFont(wxCommandEvent& WXUNUSED(event) )
{
      wxFontData data;
      data.SetInitialFont(wxGetApp().m_canvasFont);
      data.SetColour(wxGetApp().m_canvasTextColour);

      wxFontDialog *dialog = new wxFontDialog(this, &data);
      if (dialog->ShowModal() == wxID_OK)
      {
        wxFontData retData = dialog->GetFontData();
        wxGetApp().m_canvasFont = retData.GetChosenFont();
        wxGetApp().m_canvasTextColour = retData.GetColour();
        myCanvas->Refresh();
      }
      dialog->Destroy();
}

#if defined(__WXMSW__) && wxTEST_GENERIC_DIALOGS_IN_MSW
void MyFrame::ChooseColourGeneric(wxCommandEvent& WXUNUSED(event))
{
      wxColourData data;
      data.SetChooseFull(TRUE);
      for (int i = 0; i < 16; i++)
      {
        wxColour colour(i*16, i*16, i*16);
        data.SetCustomColour(i, colour);
      }

      wxGenericColourDialog *dialog = new wxGenericColourDialog(this, &data);
      if (dialog->ShowModal() == wxID_OK)
      {
        wxColourData retData = dialog->GetColourData();
        wxColour col = retData.GetColour();
//        wxBrush *brush = wxTheBrushList->FindOrCreateBrush(&col, wxSOLID);
        myCanvas->SetBackgroundColour(col);
        myCanvas->Clear();
        myCanvas->Refresh();
      }
      dialog->Destroy();
}

void MyFrame::ChooseFontGeneric(wxCommandEvent& WXUNUSED(event) )
{
      wxFontData data;
      data.SetInitialFont(wxGetApp().m_canvasFont);
      data.SetColour(wxGetApp().m_canvasTextColour);

      wxGenericFontDialog *dialog = new wxGenericFontDialog(this, &data);
      if (dialog->ShowModal() == wxID_OK)
      {
        wxFontData retData = dialog->GetFontData();
        wxGetApp().m_canvasFont = retData.GetChosenFont();
        wxGetApp().m_canvasTextColour = retData.GetColour();
        myCanvas->Refresh();
      }
      dialog->Destroy();
}
#endif // wxTEST_GENERIC_DIALOGS_IN_MSW

void MyFrame::LogDialog(wxCommandEvent& event)
{
    // calling wxYield() (as ~wxBusyCursor does) shouldn't result in messages
    // being flushed -- test it
    {
        wxBusyCursor bc;
        wxLogMessage(wxT("This is some message - everything is ok so far."));
        wxLogMessage(wxT("Another message...\n... this one is on multiple lines"));
        wxLogWarning(wxT("And then something went wrong!"));

        // and if ~wxBusyCursor doesn't do it, then call it manually
        wxYield();
    }

    wxLogError(wxT("Intermediary error handler decided to abort."));
    wxLogError(wxT("The top level caller detected an unrecoverable error."));

    wxLog::FlushActive();

    wxLogMessage(wxT("And this is the same dialog but with only one message."));
}

void MyFrame::MessageBox(wxCommandEvent& WXUNUSED(event) )
{
  wxMessageDialog dialog( NULL, "This is a message box\nA long, long string to test out the message box properly",
      "Message box text", wxYES_NO|wxCANCEL|wxICON_INFORMATION);

  dialog.ShowModal();
}

void MyFrame::NumericEntry(wxCommandEvent& WXUNUSED(event) )
{
    long res = wxGetNumberFromUser( "This is some text, actually a lot of text.\n"
                                    "Even two rows of text.",
                                    "Enter a number:", "Numeric input test",
                                     50, 0, 100, this );

    wxString msg;
    int icon;
    if ( res == -1 )
    {
        msg = "Invalid number entered or dialog cancelled.";
        icon = wxICON_HAND;
    }
    else
    {
        msg.Printf(_T("You've entered %lu"), res );
        icon = wxICON_INFORMATION;
    }

    wxMessageBox(msg, "Numeric test result", wxOK | icon, this);
}

void MyFrame::PasswordEntry(wxCommandEvent& WXUNUSED(event))
{
    wxString pwd = wxGetPasswordFromUser("Enter password:",
                                         "Password entry dialog",
                                         "",
                                         this);
    if ( !!pwd )
    {
        wxMessageBox(wxString::Format(wxT("Your password is '%s'"), pwd.c_str()),
                     "Got password", wxOK | wxICON_INFORMATION, this);
    }
}

void MyFrame::TextEntry(wxCommandEvent& WXUNUSED(event))
{
  wxTextEntryDialog dialog(this,
                           "This is a small sample\n"
                           "A long, long string to test out the text entrybox",
                           "Please enter a string",
                           "Default value",
                           wxOK | wxCANCEL);

  if (dialog.ShowModal() == wxID_OK)
  {
    wxMessageDialog dialog2(this, dialog.GetValue(), "Got string");
    dialog2.ShowModal();
  }
}

void MyFrame::SingleChoice(wxCommandEvent& WXUNUSED(event) )
{
    const wxString choices[] = { "One", "Two", "Three", "Four", "Five" } ;

    wxSingleChoiceDialog dialog(this,
                                "This is a small sample\n"
                                "A single-choice convenience dialog",
                                "Please select a value",
                                WXSIZEOF(choices), choices);

    dialog.SetSelection(2);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxMessageDialog dialog2(this, dialog.GetStringSelection(), "Got string");
        dialog2.ShowModal();
    }
}

void MyFrame::MultiChoice(wxCommandEvent& WXUNUSED(event) )
{
    const wxString choices[] =
    {
        "One", "Two", "Three", "Four", "Five",
        "Six", "Seven", "Eight", "Nine", "Ten",
        "Eleven", "Twelve", "Seventeen",
    };

    wxArrayInt selections;
    size_t count = wxGetMultipleChoices(selections,
                                        "This is a small sample\n"
                                        "A multi-choice convenience dialog",
                                        "Please select a value",
                                        WXSIZEOF(choices), choices,
                                        this);
    if ( count )
    {
        wxString msg;
        msg.Printf(wxT("You selected %u items:\n"), count);
        for ( size_t n = 0; n < count; n++ )
        {
            msg += wxString::Format(wxT("\t%u: %u (%s)\n"), n, selections[n],
                                    choices[selections[n]].c_str());
        }
        wxLogMessage(msg);
    }
    //else: cancelled or nothing selected
}

void MyFrame::FileOpen(wxCommandEvent& WXUNUSED(event) )
{
    wxFileDialog dialog
                 (
                    this,
                    _T("Testing open file dialog"),
                    _T(""),
                    _T(""),
                    _T("C++ files (*.h;*.cpp)|*.h;*.cpp")
                 );

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString info;
        info.Printf(_T("Full file name: %s\n")
                    _T("Path: %s\n")
                    _T("Name: %s"),
                    dialog.GetPath().c_str(),
                    dialog.GetDirectory().c_str(),
                    dialog.GetFilename().c_str());
        wxMessageDialog dialog2(this, info, "Selected file");
        dialog2.ShowModal();
    }
}

// this shows how to take advantage of specifying a default extension in the
// call to wxFileSelector: it is remembered after each new call and the next
// one will use it by default
void MyFrame::FileOpen2(wxCommandEvent& WXUNUSED(event) )
{
    static wxString s_extDef;
    wxString path = wxFileSelector(
                                    _T("Select the file to load"),
                                    _T(""), _T(""),
                                    s_extDef,
                                    _T("Waveform (*.wav)|*.wav|Plain text (*.txt)|*.txt|All files (*.*)|*.*"),
                                    wxCHANGE_DIR,
                                    this
                                   );

    if ( !path )
        return;

    // it is just a sample, would use wxSplitPath in real program
    s_extDef = path.AfterLast(_T('.'));

    wxLogMessage(_T("You selected the file '%s', remembered extension '%s'"),
                 (const wxChar*) path, (const wxChar*) s_extDef);
}

void MyFrame::FilesOpen(wxCommandEvent& WXUNUSED(event) )
{
    wxFileDialog dialog(this, "Testing open multiple file dialog",
                        "", "", wxFileSelectorDefaultWildcardStr,
                        wxMULTIPLE);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxArrayString paths, filenames;

        dialog.GetPaths(paths);
        dialog.GetFilenames(filenames);

        wxString msg, s;
        size_t count = paths.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            s.Printf(_T("File %d: %s (%s)\n"),
                     n, paths[n].c_str(), filenames[n].c_str());

            msg += s;
        }

        wxMessageDialog dialog2(this, msg, "Selected files");
        dialog2.ShowModal();
    }
}

void MyFrame::FileSave(wxCommandEvent& WXUNUSED(event) )
{
    wxFileDialog dialog(this, "Testing save file dialog", "", "myletter.txt",
        "Text files (*.txt)|*.txt|Document files (*.doc)|*.doc",
        wxSAVE|wxOVERWRITE_PROMPT);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxChar buf[400];
        wxSprintf(buf, _T("%s, filter %d"), (const wxChar*)dialog.GetPath(), dialog.GetFilterIndex());
        wxMessageDialog dialog2(this, wxString(buf), "Selected path");
        dialog2.ShowModal();
    }
}

void MyFrame::DirChoose(wxCommandEvent& WXUNUSED(event) )
{
    // pass some initial dir to wxDirDialog
    wxString dirHome;
    wxGetHomeDir(&dirHome);

    wxDirDialog dialog(this, "Testing directory picker", dirHome);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxMessageDialog dialog2(this, dialog.GetPath(), "Selected path");
        dialog2.ShowModal();
    }
}

void MyFrame::GenericDirChoose(wxCommandEvent& WXUNUSED(event) )
{
#if !defined(__WXMSW__) || defined(wxUSE_DIRDLGG) && wxUSE_DIRDLGG
    // pass some initial dir to wxDirDialog
    wxString dirHome;
    wxGetHomeDir(&dirHome);

    wxGenericDirDialog dialog(this, "Testing generic directory picker", dirHome);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxMessageDialog dialog2(this, dialog.GetPath(), "Selected path");
        dialog2.ShowModal();
    }
#else
    wxLogError(wxT("Sorry, generic dir dialog not available:\n")
               wxT("set wxUSE_DIRDLGG to 1 and recompile"));
#endif
}

void MyFrame::ModalDlg(wxCommandEvent& WXUNUSED(event))
{
    MyModalDialog dlg(this);
    dlg.ShowModal();
}

void MyFrame::ModelessDlg(wxCommandEvent& event)
{
    bool show = GetMenuBar()->IsChecked(event.GetId());

    if ( show )
    {
        if ( !m_dialog )
        {
            m_dialog = new MyModelessDialog(this);
        }

        m_dialog->Show(TRUE);
    }
    else // hide
    {
        m_dialog->Hide();
    }
}

void MyFrame::ShowTip(wxCommandEvent& event)
{
#if wxUSE_STARTUP_TIPS
    static size_t s_index = (size_t)-1;

    if ( s_index == (size_t)-1 )
    {
        srand(time(NULL));

        // this is completely bogus, we don't know how many lines are there
        // in the file, but who cares, it's a demo only...
        s_index = rand() % 5;
    }

    wxTipProvider *tipProvider = wxCreateFileTipProvider("tips.txt", s_index);

    bool showAtStartup = wxShowTip(this, tipProvider);

    if ( showAtStartup )
    {
        wxMessageBox("Will show tips on startup", "Tips dialog",
                     wxOK | wxICON_INFORMATION, this);
    }

    s_index = tipProvider->GetCurrentTip();
    delete tipProvider;
#endif
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event) )
{
    Close(TRUE);
}

#if wxUSE_PROGRESSDLG

void MyFrame::ShowProgress( wxCommandEvent& WXUNUSED(event) )
{
    static const int max = 10;

    wxProgressDialog dialog("Progress dialog example",
                            "An informative message",
                            max,    // range
                            this,   // parent
                            wxPD_CAN_ABORT |
                            wxPD_APP_MODAL |
                            wxPD_ELAPSED_TIME |
                            wxPD_ESTIMATED_TIME |
                            wxPD_REMAINING_TIME);

    bool cont = TRUE;
    for ( int i = 0; i <= max && cont; i++ )
    {
        wxSleep(1);
        if ( i == max )
        {
            cont = dialog.Update(i, "That's all, folks!");
        }
        else if ( i == max / 2 )
        {
            cont = dialog.Update(i, "Only a half left (very long message)!");
        }
        else
        {
            cont = dialog.Update(i);
        }
    }

    if ( !cont )
    {
        wxLogStatus(wxT("Progress dialog aborted!"));
    }
    else
    {
        wxLogStatus(wxT("Countdown from %d finished"), max);
    }
}

#endif // wxUSE_PROGRESSDLG

#if wxUSE_BUSYINFO

void MyFrame::ShowBusyInfo(wxCommandEvent& WXUNUSED(event))
{
    wxWindowDisabler disableAll;

    wxBusyInfo info("Working, please wait...", this);

    for ( int i = 0; i < 30; i++ )
    {
        wxUsleep(100);
        wxTheApp->Yield();
    }
}

#endif // wxUSE_BUSYINFO

#if wxUSE_FINDREPLDLG

void MyFrame::ShowReplaceDialog( wxCommandEvent& WXUNUSED(event) )
{
    if ( m_dlgReplace )
    {
        delete m_dlgReplace;
        m_dlgReplace = NULL;
    }
    else
    {
        m_dlgReplace = new wxFindReplaceDialog
                           (
                            this,
                            &m_findData,
                            "Find and replace dialog",
                            wxFR_REPLACEDIALOG
                           );

        m_dlgReplace->Show(TRUE);
    }
}

void MyFrame::ShowFindDialog( wxCommandEvent& WXUNUSED(event) )
{
    if ( m_dlgFind )
    {
        delete m_dlgFind;
        m_dlgFind = NULL;
    }
    else
    {
        m_dlgFind = new wxFindReplaceDialog
                        (
                            this,
                            &m_findData,
                            "Find dialog",
                            // just for testing
                            wxFR_NOWHOLEWORD
                        );

        m_dlgFind->Show(TRUE);
    }
}

static wxString DecodeFindDialogEventFlags(int flags)
{
    wxString str;
    str << (flags & wxFR_DOWN ? "down" : "up") << ", "
        << (flags & wxFR_WHOLEWORD ? "whole words only, " : "")
        << (flags & wxFR_MATCHCASE ? "" : "not ")
        << "case sensitive";

    return str;
}

void MyFrame::OnFindDialog(wxFindDialogEvent& event)
{
    wxEventType type = event.GetEventType();

    if ( type == wxEVT_COMMAND_FIND || type == wxEVT_COMMAND_FIND_NEXT )
    {
        wxLogMessage(wxT("Find %s'%s' (flags: %s)"),
                     type == wxEVT_COMMAND_FIND_NEXT ? "next " : "",
                     event.GetFindString().c_str(),
                     DecodeFindDialogEventFlags(event.GetFlags()).c_str());
    }
    else if ( type == wxEVT_COMMAND_FIND_REPLACE ||
                type == wxEVT_COMMAND_FIND_REPLACE_ALL )
    {
        wxLogMessage(wxT("Replace %s'%s' with '%s' (flags: %s)"),
                     type == wxEVT_COMMAND_FIND_REPLACE_ALL ? "all " : "",
                     event.GetFindString().c_str(),
                     event.GetReplaceString().c_str(),
                     DecodeFindDialogEventFlags(event.GetFlags()).c_str());
    }
    else if ( type == wxEVT_COMMAND_FIND_CLOSE )
    {
        wxFindReplaceDialog *dlg = event.GetDialog();

        const wxChar *txt;
        if ( dlg == m_dlgFind )
        {
            txt = _T("Find");
            m_dlgFind = NULL;
        }
        else if ( dlg == m_dlgReplace )
        {
            txt = _T("Replace");
            m_dlgReplace = NULL;
        }
        else
        {
            txt = _T("Unknown");

            wxFAIL_MSG( _T("unexecpted event") );
        }

        wxLogMessage(wxT("%s dialog is being closed."), txt),

        dlg->Destroy();
    }
    else
    {
        wxLogError(wxT("Unknown find dialog event!"));
    }
}

#endif // wxUSE_FINDREPLDLG

// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event) )
{
    wxPaintDC dc(this);
    dc.SetFont(wxGetApp().m_canvasFont);
    dc.SetTextForeground(wxGetApp().m_canvasTextColour);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.DrawText("wxWindows common dialogs test application", 10, 10);
}

// ----------------------------------------------------------------------------
// MyModelessDialog
// ----------------------------------------------------------------------------

MyModelessDialog::MyModelessDialog(wxWindow *parent)
                : wxDialog(parent, -1, wxString("Modeless dialog"))
{
    wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

    wxButton *btn = new wxButton(this, DIALOGS_MODELESS_BTN, "Press me");
    wxCheckBox *check = new wxCheckBox(this, -1, "Should be disabled");
    check->Disable();

    sizerTop->Add(btn, 1, wxEXPAND | wxALL, 5);
    sizerTop->Add(check, 1, wxEXPAND | wxALL, 5);

    SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
}

void MyModelessDialog::OnButton(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("Button pressed in modeless dialog", "Info",
                 wxOK | wxICON_INFORMATION, this);
}

void MyModelessDialog::OnClose(wxCloseEvent& event)
{
    if ( event.CanVeto() )
    {
        wxMessageBox("Use the menu item to close this dialog",
                     "Modeless dialog",
                     wxOK | wxICON_INFORMATION, this);

        event.Veto();
    }
}

// ----------------------------------------------------------------------------
// MyModalDialog
// ----------------------------------------------------------------------------

MyModalDialog::MyModalDialog(wxWindow *parent)
             : wxDialog(parent, -1, wxString("Modal dialog"))
{
    wxBoxSizer *sizerTop = new wxBoxSizer(wxHORIZONTAL);

    m_btnFocused = new wxButton(this, -1, "Default button");
    m_btnDelete = new wxButton(this, -1, "&Delete button");
    wxButton *btnOk = new wxButton(this, wxID_CANCEL, "&Close");
    sizerTop->Add(m_btnFocused, 0, wxALIGN_CENTER | wxALL, 5);
    sizerTop->Add(m_btnDelete, 0, wxALIGN_CENTER | wxALL, 5);
    sizerTop->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 5);

    SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);

    m_btnFocused->SetFocus();
    m_btnFocused->SetDefault();
}

void MyModalDialog::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() == m_btnDelete )
    {
        delete m_btnFocused;
        m_btnFocused = NULL;

        m_btnDelete->Disable();
    }
    else if ( event.GetEventObject() == m_btnFocused )
    {
        wxGetTextFromUser("Dummy prompt", "Modal dialog called from dialog",
                          "", this);
    }
    else
    {
        event.Skip();
    }
}
