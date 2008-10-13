/////////////////////////////////////////////////////////////////////////////
// Name:        screenshot_main.cpp
// Purpose:     Implement the Application Frame
// Author:      Utensil Candel (UtensilCandel@@gmail.com)
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include <wx/filename.h>
    #include <wx/dcbuffer.h>
    #include <wx/colordlg.h>
    #include <wx/fontdlg.h>
    #include <wx/filedlg.h>
    #include <wx/dirdlg.h>
#endif

#include <wx/dir.h>
#include <wx/aboutdlg.h>
#include <wx/msgdlg.h>
#include <wx/dcscreen.h>
#include <wx/filesys.h>
#include <wx/utils.h>

#include "screenshot_main.h"
#include "ctrlmaskout.h"
#include "autocapture.h"

#include "bitmaps/play.xpm"
#include "bitmaps/stop.xpm"


// ----------------------------------------------------------------------------
// ScreenshotFrame
// ----------------------------------------------------------------------------

ScreenshotFrame::ScreenshotFrame(wxFrame *frame)
#if SCREENSHOTGEN_USE_AUI
: AuiGUIFrame(frame)
#else
: GUIFrame(frame)
#endif
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("Welcome to the Automatic Screenshot Generator!"), 0);
#endif

    // We will hold one ctrlmaskout during the whole life time of the main frame
    m_maskout = new CtrlMaskOut();

    // At the begining, we are not specifying the rect region
    capturingRect = false;

    // Do some further customization on some controls generated by wxFormBuilder
    InitFBControls();
#if SCREENSHOTGEN_USE_AUI
    // Somehow it will be very small after I move to Aui
    SetSize(600, 600);
    // Maximize(true);
#endif
}

ScreenshotFrame::~ScreenshotFrame()
{
    delete m_maskout;
}

/*
    Do some further customization on some controls generated by wxFormBuilder.

    wxFormBuilder does not allow customizations on some controls;
    e.g. you cannot load a richtext file in a wxRichtextCtrl during initialization.

    Those customizations will be done here.


    NB: under wxGTK for the radio button "unchecked" to be unchecked, it's
        important to put the wxRB_GROUP style on the first wxRadioButton
        (the one "checked") and no flags on the second one.
*/
void ScreenshotFrame::InitFBControls()
{
    // For some reason, wxFormBuilder does not set the scrollbar range
    m_scrollBar1->SetScrollbar(50, 1, 100, 1);

    // Do the default selection for wxComboBox
    m_comboBox1->Select(0);

    // To look better under gtk
#ifdef __WXGTK__
    m_comboBox1->Delete(4);
#endif

    // Add a root and some nodes for wxTreeCtrl
    wxTreeItemId root = m_treeCtrl1->AddRoot(_("wxTreeCtrl"));
    m_treeCtrl1->AppendItem(root, _("Node1"));
    wxTreeItemId node2 = m_treeCtrl1->AppendItem(root, _("Node2"));
    m_treeCtrl1->AppendItem(node2, _("Node3"));
    m_treeCtrl1->ExpandAll();

    // Add items into wxListCtrl
	m_listCtrl1->InsertColumn(0, "Names");
	m_listCtrl1->InsertColumn(1, "Values");
	for(long index = 0; index < 5; index++) {
        m_listCtrl1->InsertItem( index, wxString::Format(_("Item%d"),index));
		m_listCtrl1->SetItem(index, 1, wxString::Format("%d", index));
	}

	// Init file and dir pickers
	wxString file, dir;
#if defined(__WXMSW__)
	file = "C:\\Windows\\explorer.exe";
	dir = "C:\\Windows";
#else
	file = "/bin/bash";
	dir = "/home";
#endif
	m_filePicker1->SetPath(file);
	m_dirPicker1->SetPath(dir);

    // Check the first item in wxCheckListBox
    m_checkList1->Check(0);

    // Load richtext.xml into wxRichtextCtrl
    m_richText1->LoadFile(_T("richtext.xml"));
    //m_richText1->ShowPosition(335);

    // select first page in the main notebook ctrl
    m_notebook1->ChangeSelection(0);

    // set minimum size hints
    GetSizer()->SetSizeHints(this);

    // add bitmaps to the menus
    m_menuCapRect->SetBitmap( wxIcon(play_xpm) );
    m_menuEndCapRect->SetBitmap( wxIcon(stop_xpm) );
}



// ----------------------------------------------------------------------------
// ScreenshotFrame - event handlers
// ----------------------------------------------------------------------------

void ScreenshotFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void ScreenshotFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

void ScreenshotFrame::OnSeeScreenshots(wxCommandEvent& WXUNUSED(event))
{
    wxString defaultDir = m_maskout->GetDefaultDirectory();

    // Check if defaultDir already existed
    if (!wxDirExists(defaultDir))
        wxMkdir(defaultDir);

    // Use the native file browser to open defaultDir
    wxLaunchDefaultBrowser(wxFileSystem::FileNameToURL(defaultDir));
}

void ScreenshotFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetName(_("Automatic Screenshot Generator"));
    info.SetVersion(_("1.0"));
    info.SetDescription(_("This utility automatically creates screenshots of wxWidgets controls for use in wxWidgets documentation."));
    info.SetCopyright(_T("(C) 2008 Utensil Candel"));

    wxAboutBox(info);
}

void ScreenshotFrame::OnCaptureFullScreen(wxCommandEvent& WXUNUSED(event))
{
    // Create a DC for the whole screen area
    wxScreenDC dcScreen;

    // Get the size of the screenDC
    wxCoord screenWidth, screenHeight;
    dcScreen.GetSize(&screenWidth, &screenHeight);

    m_maskout->Capture(0, 0, screenWidth, screenHeight, _T("fullscreen"));

    // Inform the user
    wxMessageBox(_("A screenshot of the entire screen was saved as:\n\n  ") +
                 m_maskout->GetDefaultDirectoryAbsPath() + wxFileName::GetPathSeparator() + "fullscreen.png",
                 _("Full screen capture"), wxICON_INFORMATION|wxOK, this);
}

void ScreenshotFrame::OnCaptureRect(wxCommandEvent& WXUNUSED(event))
{
    capturingRect = true;
    wxMenuBar * menubar = this->GetMenuBar();
    menubar->FindItem(idMenuCapRect)->Enable(false);
    menubar->FindItem(idMenuEndCapRect)->Enable(true);

    wxWindow * thePage = m_notebook1->GetPage(m_notebook1->GetSelection());

    thePage->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonDown ), NULL, m_maskout);
    thePage->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonUp ), NULL, m_maskout);
    thePage->Connect( wxEVT_MOTION, wxMouseEventHandler( CtrlMaskOut::OnMouseMoving ), NULL, m_maskout);
}

void ScreenshotFrame::OnEndCaptureRect(wxCommandEvent& WXUNUSED(event))
{
    capturingRect = false;
    wxMenuBar * menubar = this->GetMenuBar();
    menubar->FindItem(idMenuCapRect)->Enable(true);
    menubar->FindItem(idMenuEndCapRect)->Enable(false);

    wxWindow * thePage = m_notebook1->GetPage(m_notebook1->GetSelection());

    thePage->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonDown ), NULL, m_maskout);
    thePage->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonUp ), NULL, m_maskout);
    thePage->Disconnect( wxEVT_MOTION, wxMouseEventHandler( CtrlMaskOut::OnMouseMoving ), NULL, m_maskout);
}

void ScreenshotFrame::OnNotebookPageChanging(
#if SCREENSHOTGEN_USE_AUI
wxAuiNotebookEvent& event
#else
wxNotebookEvent& event
#endif
)
{
    if (!capturingRect)
    {
        event.Skip();
        return;
    }

    wxWindow * thePage = m_notebook1->GetPage(event.GetOldSelection());

    thePage->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonDown ), NULL, m_maskout);
    thePage->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonUp ), NULL, m_maskout);
    thePage->Disconnect( wxEVT_MOTION, wxMouseEventHandler( CtrlMaskOut::OnMouseMoving ), NULL, m_maskout);

    event.Skip();
}

void ScreenshotFrame::OnNotebookPageChanged(
#if SCREENSHOTGEN_USE_AUI
wxAuiNotebookEvent& event
#else
wxNotebookEvent& event
#endif
)
{
    if (!capturingRect)
    {
        event.Skip();
        return;
    }

    wxWindow *thePage = m_notebook1->GetPage(event.GetSelection());

    thePage->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonDown ), NULL, m_maskout);
    thePage->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( CtrlMaskOut::OnLeftButtonUp ), NULL, m_maskout);
    thePage->Connect( wxEVT_MOTION, wxMouseEventHandler( CtrlMaskOut::OnMouseMoving ), NULL, m_maskout);

    event.Skip();
}

void ScreenshotFrame::OnCaptureAllControls(wxCommandEvent& WXUNUSED(event))
{
    wxString dir = m_maskout->GetDefaultDirectoryAbsPath();

    // check if there are other screenshots taken before
    if (wxFileName::DirExists(dir))
    {
        int choice = wxMessageBox(_("It seems that you have already generated some screenshots.\n\nClick YES to delete them all (recommended) or NO to preserve them.\nClick CANCEL to cancel this auto-capture operation."),
                            _("Delete existing screenshots?"),
                            wxYES_NO|wxCANCEL|wxICON_QUESTION, this);
        switch(choice)
        {
            case wxYES:
            {
                wxArrayString files;
                wxDir::GetAllFiles(dir, &files, wxT("*.png"), wxDIR_FILES);

                // remove all PNG files from the screenshots folder
                int n = files.GetCount();
                for (int i = 0; i < n; ++i)
                    wxRemoveFile(files[i]);
            }
            break;

            case wxNO: break;
            case wxCANCEL: return;
        }
    }

    // proceed with the automatic screenshot capture

    this->Maximize();

    AutoCaptureMechanism auto_cap(m_notebook1);

    auto_cap.RegisterControl(m_button1);
    auto_cap.RegisterControl(m_staticText1);
    auto_cap.RegisterControl(m_checkBox1, AJ_Union);
    auto_cap.RegisterControl(m_checkBox2, AJ_UnionEnd);
    auto_cap.RegisterControl(m_radioBtn1, AJ_Union);
    auto_cap.RegisterControl(m_radioBtn2, AJ_UnionEnd);
    auto_cap.RegisterControl(m_bpButton1);
    auto_cap.RegisterControl(m_bitmap1);
    auto_cap.RegisterControl(m_gauge1, wxT("wxGauge"));
    auto_cap.RegisterControl(m_slider1);
    auto_cap.RegisterControl(m_toggleBtn1, AJ_Union);
    auto_cap.RegisterControl(m_toggleBtn2, AJ_UnionEnd);
    auto_cap.RegisterControl(m_hyperlink1);
    auto_cap.RegisterControl(m_spinCtrl1, AJ_RegionAdjust);
    auto_cap.RegisterControl(m_spinBtn1);
    auto_cap.RegisterControl(m_scrollBar1);

    auto_cap.RegisterPageTurn();

    auto_cap.RegisterControl(m_checkList1);
    auto_cap.RegisterControl(m_listBox1);
    auto_cap.RegisterControl(m_radioBox1);
    auto_cap.RegisterControl(m_staticBox1);
    auto_cap.RegisterControl(m_treeCtrl1);
    auto_cap.RegisterControl(m_listCtrl1, wxT("wxListCtrl"));

    auto_cap.RegisterControl(m_animationCtrl1);
    auto_cap.RegisterControl(m_collPane1, wxT("wxCollapsiblePane"), AJ_Union);
    auto_cap.RegisterControl(m_collPane2, AJ_UnionEnd);

    auto_cap.RegisterPageTurn();

    auto_cap.RegisterControl(m_textCtrl1, AJ_Union);
    auto_cap.RegisterControl(m_textCtrl2, AJ_UnionEnd);
    auto_cap.RegisterControl(m_richText1);

    auto_cap.RegisterPageTurn();

    auto_cap.RegisterControl(m_colourPicker1, wxT("wxColourPickerCtrl"));
    auto_cap.RegisterControl(m_fontPicker1, wxT("wxFontPickerCtrl"));
    auto_cap.RegisterControl(m_filePicker1, wxT("wxFilePickerCtrl"), AJ_RegionAdjust);
    auto_cap.RegisterControl(m_calendar1, wxT("wxCalendarCtrl"), AJ_RegionAdjust);
    auto_cap.RegisterControl(m_datePicker1, wxT("wxDatePickerCtrl"));
    auto_cap.RegisterControl(m_genericDirCtrl1, wxT("wxGenericDirCtrl"));
    auto_cap.RegisterControl(m_dirPicker1, wxT("wxDirPickerCtrl"), AJ_RegionAdjust);

    auto_cap.RegisterPageTurn();

    auto_cap.RegisterControl(m_choice1, AJ_Dropdown);
    auto_cap.RegisterControl(m_comboBox1, AJ_Dropdown);
    auto_cap.RegisterControl(m_bmpComboBox1, AJ_Dropdown);
    auto_cap.RegisterControl(m_ownerDrawnComboBox1, AJ_Dropdown);
    auto_cap.RegisterControl(m_comboCtrl1, AJ_Dropdown|AJ_Union);
    auto_cap.RegisterControl(m_comboCtrl2, AJ_Dropdown|AJ_UnionEnd);

    auto_cap.CaptureAll();

    wxMessageBox(_("All screenshots were generated successfully in the folder:\n  ") + dir,
                 _("Success"), wxOK|wxICON_INFORMATION, this);
}
