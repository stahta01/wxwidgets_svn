///////////////////////////////////////////////////////////////////////////////
// Name:        wx/persist/splitter.h
// Purpose:     Persistence support for wxSplitterWindow.
// Author:      Vadim Zeitlin
// Created:     2011-08-31
// RCS-ID:      $Id: wxhead.h,v 1.12 2010-04-22 12:44:51 zeitlin Exp $
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PERSIST_SPLITTER_H_
#define _WX_PERSIST_SPLITTER_H_

#include "wx/persist/window.h"

#include "wx/splitter.h"

// ----------------------------------------------------------------------------
// string constants used by wxPersistentSplitter
// ----------------------------------------------------------------------------

#define wxPERSIST_SPLITTER_KIND "Splitter"

// Special position value of -1 means the splitter is not split at all.
#define wxPERSIST_SPLITTER_POSITION "Position"

// ----------------------------------------------------------------------------
// wxPersistentSplitter: supports saving/restoring splitter position
// ----------------------------------------------------------------------------

class wxPersistentSplitter : public wxPersistentWindow<wxSplitterWindow>
{
public:
    wxPersistentSplitter(wxSplitterWindow* splitter)
        : wxPersistentWindow<wxSplitterWindow>(splitter)
    {
    }

    virtual void Save() const
    {
        wxSplitterWindow* const splitter = Get();

        int pos = splitter->IsSplit() ? splitter->GetSashPosition() : -1;
        SaveValue(wxPERSIST_SPLITTER_POSITION, pos);
    }

    virtual bool Restore()
    {
        int pos;
        if ( RestoreValue(wxPERSIST_SPLITTER_POSITION, &pos) )
        {
            if ( pos == -1 )
                Get()->Unsplit();
            else
                Get()->SetSashPosition(pos);
        }

        return false;
    }

    virtual wxString GetKind() const { return wxPERSIST_SPLITTER_KIND; }
};

inline wxPersistentObject *wxCreatePersistentObject(wxSplitterWindow* splitter)
{
    return new wxPersistentSplitter(splitter);
}

#endif // _WX_PERSIST_SPLITTER_H_
