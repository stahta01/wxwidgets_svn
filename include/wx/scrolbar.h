/////////////////////////////////////////////////////////////////////////////
// Name:        scrolbar.h
// Purpose:     wxScrollBar base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SCROLBAR_H_BASE_
#define _WX_SCROLBAR_H_BASE_

#if wxUSE_SCROLLBAR

#include "wx/control.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxScrollBarNameStr;

// ----------------------------------------------------------------------------
// wxScrollBar: a scroll bar control
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxScrollBarBase : public wxControl
{
public:
    // scrollbar construction
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSB_HORIZONTAL,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxScrollBarNameStr);

    // accessors
    virtual int GetThumbPosition() const = 0;
    virtual int GetThumbSize() const = 0;
    virtual int GetPageSize() const = 0;
    virtual int GetRange() const = 0;

    bool IsVertical() const { return (m_windowStyle & wxVERTICAL) != 0; }

    // operations
    virtual void SetThumbPosition(int viewStart) = 0;
    virtual void SetScrollbar(int position, int thumbSize,
                              int range, int pageSize,
                              bool refresh = TRUE) = 0;
};

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/scrolbar.h"
#elif defined(__WXMSW__)
    #include "wx/msw/scrolbar.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/scrolbar.h"
#elif defined(__WXGTK__)
    #include "wx/gtk/scrolbar.h"
#elif defined(__WXMAC__)
    #include "wx/mac/scrolbar.h"
#elif defined(__WXPM__)
    #include "wx/os2/scrolbar.h"
#elif defined(__WXSTUBS__)
    #include "wx/stubs/scrolbar.h"
#endif

#endif // wxUSE_SCROLLBAR

#endif
    // _WX_SCROLBAR_H_BASE_
