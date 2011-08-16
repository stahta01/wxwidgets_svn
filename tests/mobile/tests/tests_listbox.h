/*
 *  tests_listbox.h
 *  testios
 *
 *  Created by Linas Valiukas on 2011-08-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

//
// TESTS:
// 
// events:
//   + wxEVT_COMMAND_LISTBOX_SELECTED
// + wxListBox (wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, const wxArrayString &strings=wxArrayString(), long style=wxLB_SINGLE, const wxValidator &validator=wxDefaultValidator, const wxString &name=wxMoVListBoxNameStr)
// - bool   Create (wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, const wxArrayString &strings=wxArrayString(), long style=wxLB_SINGLE, const wxValidator &validator=wxDefaultValidator, const wxString &name=wxMoVListBoxNameStr)
// - void   Append (const wxString &str)
// - int    Insert (const wxString &str, int pos)
// - void   Set (const wxArrayString &strings)
// - void   Clear ()
// - wxString      GetStringSelection () const
// - void   SetStringSelection (const wxString &str)
// - wxString      GetString (int n) const
// - void   SetString (int n, const wxString &str)
// - wxArrayString GetStrings () const
// - int    GetCount () const
// - void   InsertItems (const wxArrayString &items, int pos)
// + virtual void  SetSelection (int selection)
// - void     Deselect (int n)
// + virtual int   GetSelections (wxArrayInt &selections) const 
// - void    InsertItems (unsigned int nItems, const wxString *items, unsigned int pos)
// - void    InsertItems (const wxArrayString &items, unsigned int pos)
// + virtual bool  IsSelected (int n) const 
// - void    Set (unsigned int n, const wxString *choices, void *clientData)
// - void    Set (const wxArrayString &choices, void *clientData)
// - void    SetFirstItem (int n)
// - void    SetFirstItem (const wxString &string)

#ifndef testios_tests_listbox_h
#define testios_tests_listbox_h

#include "testpanel.h"
#include "wx/listbox.h"

class MobileTestsWxListBoxPanel : public MobileTestsWxPanel {
    
public:
    bool CreateWithControls(wxWindow* parent,
                            wxWindowID id = wxID_ANY,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            long style = wxTAB_TRAVERSAL,
                            const wxString& name = _("wxListBox Mobile"));
    
    // Event tests
    void OnListBoxSelected(wxCommandEvent& WXUNUSED(event));
    
protected:
    bool CreateControls();
    
    DECLARE_EVENT_TABLE()
    
private:
    wxListBox* m_listBox;
};

#endif
