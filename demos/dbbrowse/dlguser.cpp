//---------------------------------------------------------------------------
// Name:        DlgUser.h,cpp
// Purpose:     Dialog mit Variable Gestaltung durch DlgUser.wxr
// Author:      Mark Johnson, mj10777@gmx.net
// Modified by: 19991105.mj10777
// Created:     19991105
// Copyright:   (c) Mark Johnson
// Licence:     wxWindows license
//---------------------------------------------------------------------------
//-- all #ifdefs that the whole Project needs. ------------------------------
//---------------------------------------------------------------------------
#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif
//---------------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
//---------------------------------------------------------------------------
//-- all #includes that every .cpp needs             ----19990807.mj10777 ---
//---------------------------------------------------------------------------
#include "dlguser.h"
//---------------------------------------------------------------------------
DlgUser::DlgUser(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long WXUNUSED(style) ) :
  wxDialog(parent, ID_DIALOG_DSN, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL)
{
  SetBackgroundColour("wheat");
  wxButton *but1 = new wxButton(this, wxID_OK, "OK", wxPoint(55,110), wxSize(80, 30));
  wxButton *but2 = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(210,110), wxSize(80, 30));
  (void)new wxStaticText(this, -1,_("User ID"), wxPoint(20, 40), wxSize(50, 20),wxALIGN_LEFT);
  (void)new wxStaticText(this, -1,_("Password"), wxPoint(20, 80), wxSize(50, 20),wxALIGN_LEFT);
  // but1->SetFocus();
  but1->SetDefault();
}
void DlgUser::OnInit()
{
  wxString Temp; Temp.Printf(_(">>> %s <<< "),s_DSN.c_str());
  (void)new wxStaticText(this, -1, Temp, wxPoint(10, 10), wxSize(300, 20),wxALIGN_CENTRE );
  tc_User     = new wxTextCtrl(this, ID_USER, s_User, wxPoint(75, 35), wxSize(200, 25), 0, wxDefaultValidator);
  tc_Password = new wxTextCtrl(this, ID_PASSWORD, s_Password, wxPoint(75, 75), wxSize(200, 25),wxTE_PASSWORD, wxDefaultValidator);
  tc_User->SetFocus();
}
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(DlgUser, wxDialog)
  EVT_BUTTON(wxID_OK, DlgUser::OnOk)
  EVT_BUTTON(wxID_CANCEL, DlgUser::OnCancel)
  END_EVENT_TABLE()
  //---------------------------------------------------------------------------
  void DlgUser::OnOk(wxCommandEvent& WXUNUSED(event) )
{
  canceled = FALSE;
  s_User    = tc_User->GetValue();
  s_Password = tc_Password->GetValue();
  EndModal(wxID_OK);
}
//---------------------------------------------------------------------------
void DlgUser::OnCancel(wxCommandEvent& WXUNUSED(event) )
{
  canceled = TRUE;
  EndModal(wxID_CANCEL);
}
//---------------------------------------------------------------------------
