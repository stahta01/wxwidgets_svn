/////////////////////////////////////////////////////////////////////////////
// Name:        filedlg.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKFILEDLGH__
#define __GTKFILEDLGH__

#include "wx/generic/filedlgg.h"

//-------------------------------------------------------------------------
// wxFileDialog
//-------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxFileDialog: public wxGenericFileDialog
{
public:
    wxFileDialog() { }

    wxFileDialog(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = wxEmptyString,
                 const wxString& defaultFile = wxEmptyString,
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = 0,
                 const wxPoint& pos = wxDefaultPosition);

    virtual ~wxFileDialog();

    virtual wxString GetPath() const;
    virtual void GetPaths(wxArrayString& paths) const;
    virtual wxString GetDirectory() const;
    virtual wxString GetFilename() const;
    virtual void GetFilenames(wxArrayString& files) const;
    virtual int GetFilterIndex() const;

    virtual void SetMessage(const wxString& message);
    virtual void SetPath(const wxString& path);
    virtual void SetDirectory(const wxString& dir);
    virtual void SetFilename(const wxString& name);
    virtual void SetWildcard(const wxString& wildCard);
    virtual void SetFilterIndex(int filterIndex);

    virtual int ShowModal();
    virtual bool Show( bool show = true );

//private:
    bool m_destroyed_by_delete;

protected:
    // override this from wxTLW since the native
    // form doesn't have any m_wxwindow
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);


private:
    DECLARE_DYNAMIC_CLASS(wxFileDialog)
    DECLARE_EVENT_TABLE()
    void OnFakeOk( wxCommandEvent &event );
};

#endif // __GTKFILEDLGH__
