/*
 *  tests_textctrl.cpp
 *  testios
 *
 *  Created by Linas Valiukas on 2011-08-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "tests_textctrl.h"

BEGIN_EVENT_TABLE(MobileTestsWxTextCtrlPanel, wxPanel)
    EVT_TEXT(wxID_ANY, MobileTestsWxTextCtrlPanel::OnTextCtrlText)
    EVT_TEXT_ENTER(wxID_ANY, MobileTestsWxTextCtrlPanel::OnTextCtrlTextEnter)
    EVT_TEXT_MAXLEN(wxID_ANY, MobileTestsWxTextCtrlPanel::OnTextCtrlTextMaxLen)
END_EVENT_TABLE()


bool MobileTestsWxTextCtrlPanel::CreateWithControls(wxWindow* parent,
                                                    wxWindowID id,
                                                    const wxPoint& pos,
                                                    const wxSize& size,
                                                    long style,
                                                    const wxString& name)
{
    if ( !wxPanel::Create(parent, id, pos, size, style, name)) {
        wxFAIL_MSG("Unable to create parent wxPanel");
        return false;
    }
    
    if ( !CreateControls()) {
        wxFAIL_MSG("Unable to create controls for this test panel");
        return false;
    }
    
    return true;
}

bool MobileTestsWxTextCtrlPanel::CreateControls()
{
    wxBoxSizer* textCtrlSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(textCtrlSizer);
    
    //
    // Various text controls
    // 
    
    m_textCtrlNormal = new wxTextCtrl(this, wxID_ANY, "Normal", wxDefaultPosition, wxDefaultSize);
    textCtrlSizer->Add(m_textCtrlNormal, 0, wxEXPAND|wxALL, 5);
    
    m_textCtrlPassword = new wxTextCtrl(this, wxID_ANY, "Password", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    textCtrlSizer->Add(m_textCtrlPassword, 0, wxEXPAND|wxALL, 5);
    
    m_textCtrlReadOnly = new wxTextCtrl(this, wxID_ANY, "Read only", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    textCtrlSizer->Add(m_textCtrlReadOnly, 0, wxEXPAND|wxALL, 5);
    
    m_textCtrlLimitedTo10Chars = new wxTextCtrl(this, wxID_ANY, "10 chars", wxDefaultPosition, wxDefaultSize);
    m_textCtrlLimitedTo10Chars->SetMaxLength(10);
    textCtrlSizer->Add(m_textCtrlLimitedTo10Chars, 0, wxEXPAND|wxALL, 5);
    
    m_textCtrlMultiLine = new wxTextCtrl(this, wxID_ANY, "Multi line", wxDefaultPosition, wxSize(wxDefaultCoord,80), wxTE_MULTILINE);
    textCtrlSizer->Add(m_textCtrlMultiLine, 0, wxEXPAND|wxALL, 5);
    
    m_textCtrlProcessEnter = new wxTextCtrl(this, wxID_ANY, "Processes 'Enter'", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    textCtrlSizer->Add(m_textCtrlProcessEnter, 0, wxEXPAND|wxALL, 5);
    
    m_textCtrlCapitalize = new wxTextCtrl(this, wxID_ANY, "Capitalizes text", wxDefaultPosition, wxDefaultSize, wxTE_CAPITALIZE);
    textCtrlSizer->Add(m_textCtrlCapitalize, 0, wxEXPAND|wxALL, 5);    
    
    return true;
}

void MobileTestsWxTextCtrlPanel::OnTextCtrlText(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("Event wxEVT_TEXT");
}

void MobileTestsWxTextCtrlPanel::OnTextCtrlTextEnter(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("Event wxEVT_TEXT_ENTER");    
}

void MobileTestsWxTextCtrlPanel::OnTextCtrlTextMaxLen(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("Event wxEVT_TEXT_MAXLEN");    
}
