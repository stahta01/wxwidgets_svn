/////////////////////////////////////////////////////////////////////////////
// Name:        dirdlg.h
// Purpose:     wxDirDialog class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __DIRDLGH__
#define __DIRDLGH__

#ifdef __GNUG__
#pragma interface "dirdlg.h"
#endif

#include "wx/dialog.h"

class WXDLLEXPORT wxDirDialog: public wxDialog
{
DECLARE_DYNAMIC_CLASS(wxDirDialog)
public:
    wxDirDialog(wxWindow *parent, const wxString& message = wxFileSelectorPromptStr,
        const wxString& defaultPath = "",
        long style = 0, const wxPoint& pos = wxDefaultPosition);

    inline void SetMessage(const wxString& message) { m_message = message; }
    inline void SetPath(const wxString& path) { m_path = path; }
    inline void SetStyle(long style) { m_dialogStyle = style; }

    inline wxString GetMessage(void) const { return m_message; }
    inline wxString GetPath(void) const { return m_path; }
    inline long GetStyle(void) const { return m_dialogStyle; }

    int ShowModal(void);

protected:
    wxString    m_message;
    long        m_dialogStyle;
    wxWindow *  m_parent;
    wxString    m_path;
};

#endif
    // __DIRDLGH__
