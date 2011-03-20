/////////////////////////////////////////////////////////////////////////////
// Name:        wx/panel.h
// Purpose:     Base header for wxPanel
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
//              (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PANEL_H_BASE_
#define _WX_PANEL_H_BASE_

// ----------------------------------------------------------------------------
// headers and forward declarations
// ----------------------------------------------------------------------------

#include "wx/window.h"
#include "wx/containr.h"

class WXDLLIMPEXP_FWD_CORE wxControlContainer;

extern WXDLLIMPEXP_DATA_CORE(const char) wxPanelNameStr[];

// ----------------------------------------------------------------------------
// wxPanel contains other controls and implements TAB traversal between them
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPanelBase : public wxWindow
{
public:
    wxPanelBase();

    // Derived classes should also provide this constructor:
    /*
    wxPanelBase(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = wxPanelNameStr);
    */

    // Pseudo ctor
    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = wxPanelNameStr);


    // implementation from now on
    // --------------------------

    virtual void InitDialog();

    WX_DECLARE_CONTROL_CONTAINER();

private:
    wxDECLARE_EVENT_TABLE();

    wxDECLARE_NO_COPY_CLASS(wxPanelBase);
};

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/panel.h"
#elif defined(__WXMSW__)
    #include "wx/msw/panel.h"
#else
    #include "wx/generic/panelg.h"
#endif

#endif // _WX_PANELH_BASE_
