/////////////////////////////////////////////////////////////////////////////
// Name:        helpctrl.cpp
// Purpose:     wxHtmlHelpController
// Notes:       Based on htmlhelp.cpp, implementing a monolithic
//              HTML Help controller class,  by Vaclav Slavik
// Author:      Harm van der Heijden and Vaclav Slavik
// RCS-ID:      $Id$
// Copyright:   (c) Harm van der Heijden and Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/defs.h"

#if wxUSE_HTML

#include "wx/html/helpctrl.h"
#include "wx/wx.h"
#include "wx/busyinfo.h"

IMPLEMENT_DYNAMIC_CLASS(wxHtmlHelpController, wxHelpControllerBase)

#if 0
BEGIN_EVENT_TABLE(wxHtmlHelpController, wxEvtHandler)
EVT_CLOSE(wxHtmlHelpController::OnCloseFrame)
END_EVENT_TABLE()
#endif

wxHtmlHelpController::wxHtmlHelpController(int style)
{
    m_helpFrame = NULL;
    m_Config = NULL;
    m_ConfigRoot = wxEmptyString;
    m_titleFormat = _("Help: %s");
    m_FrameStyle = style;
}

wxHtmlHelpController::~wxHtmlHelpController()
{
    WriteCustomization(m_Config, m_ConfigRoot);
    if (m_helpFrame)
        DestroyHelpWindow();
}


void wxHtmlHelpController::DestroyHelpWindow()
{
    //if (m_Config) WriteCustomization(m_Config, m_ConfigRoot);
    if (m_helpFrame)
        m_helpFrame->Destroy();
}

void wxHtmlHelpController::OnCloseFrame(wxCloseEvent& evt)
{
    evt.Skip();

    OnQuit();

    m_helpFrame->SetController((wxHelpControllerBase*) NULL);
    m_helpFrame = NULL;
}

void wxHtmlHelpController::SetTitleFormat(const wxString& title)
{
    m_titleFormat = title;
    if (m_helpFrame)
        m_helpFrame->SetTitleFormat(title);
}


bool wxHtmlHelpController::AddBook(const wxString& book, bool show_wait_msg)
{
    wxBusyCursor cur;
#if wxUSE_BUSYINFO
    wxBusyInfo* busy = NULL;
    wxString info;
    if (show_wait_msg) {
        info.Printf(_("Adding book %s"), book.c_str());
        busy = new wxBusyInfo(info);
    }
#endif
    bool retval = m_helpData.AddBook(book);
#if wxUSE_BUSYINFO
    if (show_wait_msg)
        delete busy;
#endif 
    return retval;
}



wxHtmlHelpFrame *wxHtmlHelpController::CreateHelpFrame(wxHtmlHelpData *data)
{
    return new wxHtmlHelpFrame(data);
}


void wxHtmlHelpController::CreateHelpWindow()
{
    if (m_helpFrame) {
        m_helpFrame->Raise();
        return ;
    }

    if (m_Config == NULL) 
    {
        m_Config = wxConfigBase::Get(FALSE);
        if (m_Config != NULL)
            m_ConfigRoot = _T("wxWindows/wxHtmlHelpController");
    }

    m_helpFrame = CreateHelpFrame(&m_helpData);
    m_helpFrame->SetController(this);
//    m_helpFrame->PushEventHandler(this);

    if (m_Config)
        m_helpFrame->UseConfig(m_Config, m_ConfigRoot);

    m_helpFrame->Create(NULL, wxID_HTML_HELPFRAME, wxEmptyString, m_FrameStyle);
    m_helpFrame->SetTitleFormat(m_titleFormat);
    m_helpFrame->Show(TRUE);
}

void wxHtmlHelpController::ReadCustomization(wxConfigBase* cfg, const wxString& path)
{
    /* should not be called by the user; call UseConfig, and the controller
     * will do the rest */
    if (m_helpFrame)
        m_helpFrame->ReadCustomization(cfg, path);
}

void wxHtmlHelpController::WriteCustomization(wxConfigBase* cfg, const wxString& path)
{
    /* typically called by the controllers OnCloseFrame handler */
    if (m_helpFrame)
        m_helpFrame->WriteCustomization(cfg, path);
}

void wxHtmlHelpController::UseConfig(wxConfigBase *config, const wxString& rootpath)
{
    m_Config = config;
    m_ConfigRoot = rootpath;
    if (m_helpFrame) m_helpFrame -> UseConfig(config, rootpath);
    ReadCustomization(config, rootpath);
}

//// Backward compatibility with wxHelpController API

bool wxHtmlHelpController::Initialize(const wxString& file)
{
    wxString dir, filename, ext;
    wxSplitPath(file, & dir, & filename, & ext);

    if (!dir.IsEmpty())
        dir = dir + wxString(_("/"));

    // Try to find a suitable file
    wxString actualFilename = dir + filename + wxString(_(".zip"));
    if (!wxFileExists(actualFilename))
    {
        actualFilename = dir + filename + wxString(_(".htb"));
        if (!wxFileExists(actualFilename))
        {
            actualFilename = dir + filename + wxString(_(".hhp"));
            if (!wxFileExists(actualFilename))
                return FALSE;
        }
    }

    return AddBook(actualFilename);
}

bool wxHtmlHelpController::LoadFile(const wxString& WXUNUSED(file))
{
    // Don't reload the file or we'll have it appear again, presumably.
    return TRUE;
}

bool wxHtmlHelpController::DisplaySection(int sectionNo)
{
    return Display(sectionNo);
}

void wxHtmlHelpController::SetFrameParameters(const wxString& title,
                                   const wxSize& size,
                                   const wxPoint& pos,
                                   bool WXUNUSED(newFrameEachTime))
{
    SetTitleFormat(title);
    if (m_helpFrame)
    {
        m_helpFrame->SetSize(pos.x, pos.y, size.x, size.y);
    }
}

wxFrame* wxHtmlHelpController::GetFrameParameters(wxSize *size,
                                   wxPoint *pos,
                                   bool *newFrameEachTime)
{
    if (newFrameEachTime)
        (* newFrameEachTime) = FALSE;
    if (size && m_helpFrame)
        (* size) = m_helpFrame->GetSize();
    if (pos && m_helpFrame)
        (* pos) = m_helpFrame->GetPosition();
    return m_helpFrame;
}

bool wxHtmlHelpController::Quit()
{
    DestroyHelpWindow();
    return TRUE;
}


#endif
