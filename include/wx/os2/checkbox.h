/////////////////////////////////////////////////////////////////////////////
// Name:        checkbox.h
// Purpose:     wxCheckBox class
// Author:      David Webster
// Modified by:
// Created:     10/13/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKBOX_H_
#define _WX_CHECKBOX_H_

#include "wx/control.h"

WXDLLEXPORT_DATA(extern const char*) wxCheckBoxNameStr;

// Checkbox item (single checkbox)
class WXDLLEXPORT wxBitmap;
class WXDLLEXPORT wxCheckBox: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxCheckBox)

 public:
  inline wxCheckBox() { }
  inline wxCheckBox(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
#if wxUSE_VALIDATORS
           const wxValidator& validator = wxDefaultValidator,
#endif
           const wxString& name = wxCheckBoxNameStr)
  {
      Create(parent, id, label, pos, size, style, validator, name);
  }

  bool Create(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
#if wxUSE_VALIDATORS
           const wxValidator& validator = wxDefaultValidator,
#endif
           const wxString& name = wxCheckBoxNameStr);

  virtual void SetValue(bool);
  virtual bool GetValue() const ;

  virtual bool OS2Command(WXUINT param, WXWORD id);
  virtual void SetLabel(const wxString& label);
  virtual void Command(wxCommandEvent& event);
  virtual WXHBRUSH OnCtlColor(WXHDC pDC, WXHWND pWnd, WXUINT nCtlColor,
            WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

protected:
  virtual wxSize DoGetBestSize();
};

class WXDLLEXPORT wxBitmapCheckBox: public wxCheckBox
{
  DECLARE_DYNAMIC_CLASS(wxBitmapCheckBox)

 public:
  int checkWidth ;
  int checkHeight ;

  inline wxBitmapCheckBox() { checkWidth = -1; checkHeight = -1; }
  inline wxBitmapCheckBox(wxWindow *parent, wxWindowID id, const wxBitmap *label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
#if wxUSE_VALIDATORS
           const wxValidator& validator = wxDefaultValidator,
#endif
           const wxString& name = wxCheckBoxNameStr)
  {
      Create(parent, id, label, pos, size, style, validator, name);
  }

  bool Create(wxWindow *parent, wxWindowID id, const wxBitmap *bitmap,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
#if wxUSE_VALIDATORS
           const wxValidator& validator = wxDefaultValidator,
#endif
           const wxString& name = wxCheckBoxNameStr);

  virtual void SetLabel(const wxBitmap& bitmap);
private:
  virtual void SetLabel(const wxString& string)
  { wxCheckBox::SetLabel(string); };
};
#endif
    // _WX_CHECKBOX_H_
