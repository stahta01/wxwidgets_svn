/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/filedlg.h
// Purpose:     wxFileDialog class
// Author:      Ryan Norton
// Modified by:
// Created:     2004-10-02
// RCS-ID:      $Id$
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COCOA_FILEDLG_H_
#define _WX_COCOA_FILEDLG_H_

DECLARE_WXCOCOA_OBJC_CLASS(NSSavePanel);

#define wxFileDialog wxCocoaFileDialog
//-------------------------------------------------------------------------
// wxFileDialog
//-------------------------------------------------------------------------

class WXDLLEXPORT wxFileDialog: public wxFileDialogBase
{
    DECLARE_DYNAMIC_CLASS(wxFileDialog)
    DECLARE_NO_COPY_CLASS(wxFileDialog)
public:
    wxFileDialog(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = wxEmptyString,
                 const wxString& defaultFile = wxEmptyString,
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = 0,
                 const wxPoint& pos = wxDefaultPosition);
    ~wxFileDialog();

    virtual void SetPath(const wxString& path);
    virtual void GetPaths(wxArrayString& paths) const;
    virtual void GetFilenames(wxArrayString& files) const;

    virtual int ShowModal();
    
    inline WX_NSSavePanel GetNSSavePanel()
    {   return (WX_NSSavePanel)m_cocoaNSWindow; }

private:
    WX_NSMutableArray m_wildcards;
    wxArrayString m_fileNames;
};

#endif // _WX_FILEDLG_H_

