/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/scrolwin.h
// Purpose:     wxGenericScrolledWindow class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_SCROLLWIN_H_
#define _WX_GENERIC_SCROLLWIN_H_

#ifdef __GNUG__
    #pragma interface "genscrolwin.h"
#endif

// ----------------------------------------------------------------------------
// headers and constants
// ----------------------------------------------------------------------------

#include "wx/window.h"
#include "wx/panel.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxPanelNameStr;

// default scrolled window style
#ifndef wxScrolledWindowStyle
#define wxScrolledWindowStyle (wxHSCROLL | wxVSCROLL)
#endif

// avoid triggering this stupid VC++ warning
#ifdef __VISUALC__
    #pragma warning(disable:4355) // 'this' used in base member initializer list
#endif

// ----------------------------------------------------------------------------
// wxGenericScrolledWindow
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxGenericScrolledWindow : public wxPanel,
                                            public wxScrollHelper
{
 public:
    wxGenericScrolledWindow() : wxScrollHelper(this) { }
    wxGenericScrolledWindow(wxWindow *parent,
                     wxWindowID id = -1,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxScrolledWindowStyle,
                     const wxString& name = wxPanelNameStr)
        : wxScrollHelper(this)
    {
        Create(parent, id, pos, size, style, name);
    }

    virtual ~wxGenericScrolledWindow();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxScrolledWindowStyle,
                const wxString& name = wxPanelNameStr);

    virtual void PrepareDC(wxDC& dc) { DoPrepareDC(dc); }

private:
    DECLARE_ABSTRACT_CLASS(wxGenericScrolledWindow)
};

#ifdef __VISUALC__
    #pragma warning(default:4355)
#endif

#endif
    // _WX_GENERIC_SCROLLWIN_H_
