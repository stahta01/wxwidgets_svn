/////////////////////////////////////////////////////////////////////////////
// Name:        helpwce.h
// Purpose:     Help system: Windows CE help implementation
// Author:      Julian Smart
// Modified by:
// Created:     2003-07-12
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HELPWCE_H_
#define _WX_HELPWCE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "helpwce.cpp"
#endif

#if wxUSE_HELP

#include "wx/helpbase.h"

class WXDLLEXPORT wxWinceHelpController : public wxHelpControllerBase
{
public:
    wxWinceHelpController() {}
    virtual ~wxWinceHelpController() {}

    // Must call this to set the filename
    virtual bool Initialize(const wxString& file);

    // If file is "", reloads file given in Initialize
    virtual bool LoadFile(const wxString& file = wxEmptyString);
    virtual bool DisplayContents();
    virtual bool DisplaySection(int sectionNo);
    virtual bool DisplaySection(const wxString& section);
    virtual bool DisplayBlock(long blockNo);
    virtual bool DisplayContextPopup(int contextId);
    virtual bool DisplayTextPopup(const wxString& text, const wxPoint& pos);
    virtual bool KeywordSearch(const wxString& k,
                               wxHelpSearchMode mode = wxHELP_SEARCH_ALL);
    virtual bool Quit();

    wxString GetHelpFile() const { return m_helpFile; }

protected:
    // Append extension if necessary.
    wxString GetValidFilename(const wxString& file) const;

    // View topic, or just the HTML file
    bool ViewURL(const wxString& topic = wxEmptyString);

protected:
    wxString m_helpFile;

    DECLARE_CLASS(wxWinceHelpController)
};

#endif // wxUSE_MS_HTML_HELP

#endif
// _WX_HELPWCE_H_

