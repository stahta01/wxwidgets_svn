///////////////////////////////////////////////////////////////////////////////
// Name:        wx/memtext.h
// Purpose:     wxMemoryText allows to use wxTextBuffer without a file
// Created:     14.11.01
// Author:      Morten Hanssen
// Copyright:   (c) 2001 wxWindows team
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MEMTEXT_H
#define _WX_MEMTEXT_H

#include "wx/defs.h"

// there is no separate setting for wxMemoryText, it's smallish anyhow
#if wxUSE_TEXTBUFFER

// ----------------------------------------------------------------------------
// wxMemoryText
// ----------------------------------------------------------------------------

class wxMemoryText : public wxTextBuffer
{
public:
    // Constructors.
    wxMemoryText() { }
    wxMemoryText(const wxString& name) : wxTextBuffer(name) { }

protected:
    virtual bool OnExists() const
        { return false; }

    virtual bool OnOpen(const wxString &strBufferName,
                        wxTextBufferOpenMode OpenMode)
        { return true; }

    virtual bool OnClose()
        { return true; }

    virtual bool OnRead(wxMBConv& conv)
        { return true; }

    virtual bool OnWrite(wxTextFileType typeNew,
                         wxMBConv& conv = wxConvLibc)
        { return true; }
};

#endif // wxUSE_TEXTBUFFER

#endif // _WX_MEMTEXT_H

