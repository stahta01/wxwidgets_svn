/////////////////////////////////////////////////////////////////////////////
// Name:        textdlgg.h
// Purpose:     wxStatusBar class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __TEXTDLGH_G__
#define __TEXTDLGH_G__

#ifdef __GNUG__
    #pragma interface "textdlgg.h"
#endif

#include "wx/defs.h"

#include "wx/dialog.h"

class WXDLLEXPORT wxTextCtrl;

// Handy dialog functions (will be converted into classes at some point)
WXDLLEXPORT_DATA(extern const wxChar*) wxGetTextFromUserPromptStr;
WXDLLEXPORT_DATA(extern const wxChar*) wxEmptyString;

class WXDLLEXPORT wxTextEntryDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(wxTextEntryDialog)

public:
    wxTextEntryDialog(wxWindow *parent,
                      const wxString& message,
                      const wxString& caption = wxGetTextFromUserPromptStr,
                      const wxString& value = wxEmptyString,
                      long style = wxOK | wxCANCEL | wxCENTRE,
                      const wxPoint& pos = wxDefaultPosition);

    void SetValue(const wxString& val) { m_value = val; }
    wxString GetValue() const { return m_value; }

    // implementation only
    void OnOK(wxCommandEvent& event);

protected:
    wxTextCtrl *m_textctrl;
    wxString    m_value;

private:
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// functions to get a string or a number from user
// ----------------------------------------------------------------------------

wxString WXDLLEXPORT
wxGetTextFromUser(const wxString& message,
                  const wxString& caption = wxGetTextFromUserPromptStr,
                  const wxString& default_value = wxEmptyString,
                  wxWindow *parent = (wxWindow *) NULL,
                  int x = -1,
                  int y = -1,
                  bool centre = TRUE);

long WXDLLEXPORT
wxGetNumberFromUser(const wxString& message,
                    const wxString& prompt,
                    const wxString& caption,
                    long value = 0,
                    long min = 0,
                    long max = 100,
                    wxWindow *parent = (wxWindow *)NULL,
                    const wxPoint& pos = wxDefaultPosition);

#endif
    // __TEXTDLGH_G__
