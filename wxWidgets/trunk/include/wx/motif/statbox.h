/////////////////////////////////////////////////////////////////////////////
// Name:        statbox.h
// Purpose:     wxStaticBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBOX_H_
#define _WX_STATBOX_H_

#ifdef __GNUG__
#pragma interface "statbox.h"
#endif

#include "wx/control.h"

WXDLLEXPORT_DATA(extern const char*) wxStaticBoxNameStr;

// Group box
class WXDLLEXPORT wxStaticBox: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxStaticBox)

 public:
  wxStaticBox();
  inline wxStaticBox(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxStaticBoxNameStr)
  {
    Create(parent, id, label, pos, size, style, name);
  }
  ~wxStaticBox();

  bool Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxStaticBoxNameStr);

  virtual void Command(wxCommandEvent& WXUNUSED(event)) {};
  virtual void ProcessCommand(wxCommandEvent& WXUNUSED(event)) {};

  void SetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
  void SetSize(const wxRect& rect, int sizeFlags = wxSIZE_AUTO)
    { wxWindow::SetSize(rect, sizeFlags); }
  void SetSize(const wxSize& size) { wxWindow::SetSize(size); }
  virtual void SetSize(int width, int height) { SetSize(-1, -1, width, height, wxSIZE_USE_EXISTING); }

  void SetLabel(const wxString& label);
  wxString GetLabel() const;

// Implementation
  virtual void ChangeFont(bool keepOriginalSize = TRUE);
  virtual void ChangeBackgroundColour();
  virtual void ChangeForegroundColour();
  WXWidget GetTopWidget() const { return m_formWidget; }
  WXWidget GetLabelWidget() const { return m_labelWidget; }

DECLARE_EVENT_TABLE()

protected:
  // Motif-specific
  WXWidget  m_formWidget;
  WXWidget  m_labelWidget;
};

#endif
    // _WX_STATBOX_H_
