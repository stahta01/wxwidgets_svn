/////////////////////////////////////////////////////////////////////////////
// Name:        progdlgg.h
// Purpose:     wxProgressDialog class
// Author:      Karsten Ball�der
// Modified by:
// Created:     09.05.1999
// RCS-ID:      $Id$
// Copyright:   (c) Karsten Ball�der
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "progdlgg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/frame.h"
    #include "wx/button.h"
    #include "wx/stattext.h"
    #include "wx/layout.h"
    #include "wx/event.h"
    #include "wx/gauge.h"
    #include "wx/intl.h"
    #include "wx/settings.h"
    #include "wx/dcclient.h"
#endif

#if wxUSE_PROGRESSDLG

#include "wx/generic/progdlgg.h"

#define LAYOUT_X_MARGIN 8
#define LAYOUT_Y_MARGIN 8

// wxTextEntryDialog

#if !USE_SHARED_LIBRARY
    BEGIN_EVENT_TABLE(wxProgressDialog, wxFrame)
       EVT_BUTTON(-1, wxProgressDialog::OnCancel)
       EVT_CLOSE(wxProgressDialog::OnClose)
    END_EVENT_TABLE()

    IMPLEMENT_CLASS(wxProgressDialog, wxFrame)
#endif

wxProgressDialog::wxProgressDialog(wxString const &title,
                                 wxString const &message,
                                 int maximum,
                                 wxWindow *parent,
                                 int style)
{
   bool hasAbortButton = (style & wxPD_CAN_ABORT) != 0;
   m_state = hasAbortButton ? Continue : Uncancelable;
   m_disableParentOnly = (style & wxPD_APP_MODAL) == 0;
   m_parent = parent;
   m_maximum = maximum;

   wxFrame::Create(m_parent, -1, title, wxDefaultPosition,
                   wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
   SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));

   wxLayoutConstraints *c;

   wxClientDC dc(this);
   dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));
   long widthText;
   dc.GetTextExtent(message, &widthText, NULL);

   m_msg = new wxStaticText(this, -1, message);
   c = new wxLayoutConstraints;
   c->left.SameAs(this, wxLeft, 10);
   c->top.SameAs(this, wxTop, 10);
   c->width.AsIs();
   c->height.AsIs();
   m_msg->SetConstraints(c);

   if ( maximum > 0 )
   {
      m_gauge = new wxGauge(this, -1, maximum,
                            wxDefaultPosition, wxDefaultSize,
                            wxGA_HORIZONTAL | wxRAISED_BORDER);
      c = new wxLayoutConstraints;
      c->left.SameAs(this, wxLeft, 2*LAYOUT_X_MARGIN);
      c->top.Below(m_msg, 2*LAYOUT_Y_MARGIN);
      c->right.SameAs(this, wxRight, 2*LAYOUT_X_MARGIN);
      c->height.AsIs();
      m_gauge->SetConstraints(c);
      m_gauge->SetValue(0);
   }
   else
      m_gauge = (wxGauge *)NULL;

   if ( hasAbortButton )
   {
      m_btnAbort = new wxButton(this, -1, _("Cancel"));
      c = new wxLayoutConstraints;
      c->centreX.SameAs(this, wxCentreX);
      if(m_gauge)
         c->top.Below(m_gauge, 2*LAYOUT_Y_MARGIN);
      else
         c->top.Below(m_btnAbort, 2*LAYOUT_Y_MARGIN);
      c->width.AsIs();
      c->height.AsIs();
      m_btnAbort->SetConstraints(c);
   }
   else
      m_btnAbort = (wxButton *)NULL;

   SetAutoLayout(TRUE);
   Layout();

   // calc the height of the dialog
   Fit();
   // and set the width from it - unfortunately, Fit() makes the dialog way too
   // wide under Windows, so try to find a reasonable value for the width, not
   // too big and not too small
   wxSize size = GetClientSize();
   size.x = 2*widthText;
   if ( size.x < 2*size.y )
      SetClientSize(2*size.y, size.y);

   Show(TRUE);
   Centre(wxCENTER_FRAME | wxBOTH);

   if(m_disableParentOnly)
      m_parent->Enable(FALSE);
   else
      wxEnableTopLevelWindows(FALSE);

   Enable(TRUE); // enable this window
   wxYield();
}


bool
wxProgressDialog::Update(int value, const wxString& newmsg)
{
   wxASSERT_MSG( value == -1 || m_gauge, _T("can't update non existent dialog") );
   wxASSERT_MSG( value < m_maximum, _T("invalid progress value") );


   if( m_gauge )
      m_gauge->SetValue(value + 1);

   if( !newmsg.IsEmpty() )
      m_msg->SetLabel(newmsg);

   if ( (value == m_maximum - 1) && !(GetWindowStyleFlag() & wxPD_AUTO_HIDE) )
   {
       if ( m_btnAbort )
       {
           // tell the user what he should do...
           m_btnAbort->SetLabel(_("Close"));
       }

       if ( !newmsg )
       {
           // also provide the finishing message if the application didn't
           m_msg->SetLabel(_("Done."));
       }

       m_state = Finished;

       // so that we return TRUE below
       m_state = Finished;
   }

   return m_state != Canceled;
}

void wxProgressDialog::OnClose(wxCloseEvent& event)
{
   if ( m_state == Uncancelable )
      event.Veto(TRUE);
   else
      m_state = Canceled;
}


wxProgressDialog::~wxProgressDialog()
{
   if ( m_disableParentOnly )
      m_parent->Enable(TRUE);
   else
      wxEnableTopLevelWindows(TRUE);
}

#endif
