/////////////////////////////////////////////////////////////////////////////
// Name:        serctrl.h
// Purpose:     Serialization: control classes
// Author:      Guilhem Lavaux
// Modified by:
// Created:     July 1998
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __SERCTRL_H__
#define __SERCTRL_H__

#ifdef __GNUG__
#pragma interface
#endif

#include <wx/serbase.h>
#include "serwnd.h"

DECLARE_ALIAS_SERIAL_CLASS(wxControl, wxWindow)
DECLARE_SERIAL_CLASS(wxButton, wxControl)
#ifdef __WINDOWS__
DECLARE_SERIAL_CLASS(wxSlider95, wxControl)
DECLARE_SERIAL_CLASS(wxGauge95, wxControl)
#else
DECLARE_SERIAL_CLASS(wxSlider, wxControl)
DECLARE_SERIAL_CLASS(wxGauge, wxControl)
#endif
DECLARE_SERIAL_CLASS(wxCheckBox, wxControl)
DECLARE_SERIAL_CLASS(wxChoice, wxControl)
DECLARE_SERIAL_CLASS(wxComboBox, wxControl)
DECLARE_SERIAL_CLASS(wxListBox, wxControl)
DECLARE_SERIAL_CLASS(wxRadioBox, wxControl)
DECLARE_SERIAL_CLASS(wxRadioButton, wxControl)
DECLARE_SERIAL_CLASS(wxStaticText, wxControl)
DECLARE_SERIAL_CLASS(wxStaticBox, wxControl)


class WXSERIAL(wxNotebook) : public WXSERIAL(wxControl) {
  DECLARE_DYNAMIC_CLASS( wxNotebook_Serialize )
public:
  void StoreObject(wxObjectOutputStream& s);
  void LoadObject(wxObjectInputStream& s);

protected:
  int m_pcount;
  wxArrayString m_stringlist;
};

#endif
