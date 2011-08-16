/*
 *  tests_listbox.cpp
 *  testios
 *
 *  Created by Linas Valiukas on 2011-08-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "tests_listbox.h"

BEGIN_EVENT_TABLE(MobileTestsWxListBoxPanel, wxPanel)
END_EVENT_TABLE()


bool MobileTestsWxListBoxPanel::CreateWithControls(wxWindow* parent,
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

bool MobileTestsWxListBoxPanel::CreateControls()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    //
    // List box
    // 
    
    wxArrayString listboxStrings;
    for (size_t i = 0; i < 20; i++) {
        listboxStrings.Add(wxString::Format(wxT("This is string %d"), (int) i));
    }
    
    m_listBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize(-1, 120), listboxStrings, wxBORDER_SIMPLE );
    m_listBox->SetSelection(1);   // 2nd element
    m_listBox->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(MobileTestsWxListBoxPanel::OnListBoxSelected), NULL, this);
    sizer->Add(m_listBox, 1, wxEXPAND|wxALL, 5);
    
    return true;
}

void MobileTestsWxListBoxPanel::OnListBoxSelected(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("Event EVT_LISTBOX");

}
