/////////////////////////////////////////////////////////////////////////////
// Name:        filedlg.h
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKFILEDLGH__
#define __GTKFILEDLGH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/dialog.h"

//-------------------------------------------------------------------------
// File selector
//-------------------------------------------------------------------------

extern const char *wxFileSelectorPromptStr;
extern const char *wxFileSelectorDefaultWildcardStr;

class wxFileDialog: public wxDialog
{

  DECLARE_DYNAMIC_CLASS(wxFileDialog)

  friend void gtk_filedialog_ok_callback( GtkWidget *widget, gpointer data );

  public:

    wxFileDialog() {};

    wxFileDialog(wxWindow *parent, const wxString& message = wxFileSelectorPromptStr,
        const wxString& defaultDir = "", const wxString& defaultFile = "",
  const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
        long style = 0, const wxPoint& pos = wxDefaultPosition);

    inline void SetMessage(const wxString& message) { m_message = message; }
    inline void SetPath(const wxString& path) { m_path = path; }
    inline void SetDirectory(const wxString& dir) { m_dir = dir; }
    inline void SetFilename(const wxString& name) { m_fileName = name; }
    inline void SetWildcard(const wxString& wildCard) { m_wildCard = wildCard; }
    inline void SetStyle(long style) { m_dialogStyle = style; }
    inline void SetFilterIndex(int filterIndex) { m_filterIndex = filterIndex; }

    inline wxString GetMessage(void) const { return m_message; }
    inline wxString GetPath(void) const { return m_path; }
    inline wxString GetDirectory(void) const { return m_dir; }
    inline wxString GetFilename(void) const { return m_fileName; }
    inline wxString GetWildcard(void) const { return m_wildCard; }
    inline long GetStyle(void) const { return m_dialogStyle; }
    inline int GetFilterIndex(void) const { return m_filterIndex ; }

    int ShowModal(void);

  protected:

    wxString    m_message;
    long        m_dialogStyle;
    wxWindow *  m_parent;
    wxString    m_dir;
    wxString    m_path; // Full path
    wxString    m_fileName;
    wxString    m_wildCard;
    int         m_filterIndex;
};

#define wxOPEN 1
#define wxSAVE 2
#define wxOVERWRITE_PROMPT 4
#define wxHIDE_READONLY 8
#define wxFILE_MUST_EXIST 16

// File selector - backward compatibility

char* wxFileSelector(const char *message = wxFileSelectorPromptStr, const char *default_path = NULL,
         const char *default_filename = NULL, const char *default_extension = NULL,
         const char *wildcard = wxFileSelectorDefaultWildcardStr, int flags = 0,
         wxWindow *parent = NULL, int x = -1, int y = -1);

char* wxLoadFileSelector(const char *what, const char *extension, const char *default_name = NULL,
         wxWindow *parent = NULL);

char* wxSaveFileSelector(const char *what, const char *extension, const char *default_name = NULL,
         wxWindow *parent = NULL);



#endif
    // __GTKFILEDLGH__
