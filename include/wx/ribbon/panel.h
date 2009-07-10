///////////////////////////////////////////////////////////////////////////////
// Name:        wx/ribbon/panel.h
// Purpose:     Ribbon-style container for a group of related tools / controls
// Author:      Peter Cawley
// Modified by:
// Created:     2009-05-25
// RCS-ID:      $Id$
// Copyright:   (C) Peter Cawley
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef _WX_RIBBON_PANEL_H_
#define _WX_RIBBON_PANEL_H_

#include "wx/defs.h"

#if wxUSE_RIBBON

#include "wx/bitmap.h"
#include "wx/ribbon/control.h"

enum wxRibbonPanelOption
{
    wxRIBBON_PANEL_NO_AUTO_MINIMISE    = 1 << 0,
    wxRIBBON_PANEL_EXT_BUTTON        = 1 << 3,
    wxRIBBON_PANEL_MINIMISE_BUTTON    = 1 << 4,
    
    wxRIBBON_PANEL_DEFAULT_STYLE = 0,
};

class WXDLLIMPEXP_RIBBON wxRibbonPanel : public wxRibbonControl
{
public:
    wxRibbonPanel();

    wxRibbonPanel(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxString& label = wxEmptyString,
                  const wxBitmap& minimised_icon = wxNullBitmap,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxRIBBON_PANEL_DEFAULT_STYLE);

    virtual ~wxRibbonPanel();

    bool Create(wxWindow* parent,
                wxWindowID id = wxID_ANY,
                const wxString& label = wxEmptyString,
                const wxBitmap& icon = wxNullBitmap,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxRIBBON_PANEL_DEFAULT_STYLE);

    wxBitmap& GetMinimisedIcon() {return m_minimised_icon;}
    const wxBitmap& GetMinimisedIcon() const {return m_minimised_icon;}
    bool IsMinimised() const;
    bool IsMinimised(wxSize at_size) const;
    bool IsHovered() const;
    bool CanAutoMinimise() const;

    void SetArtProvider(wxRibbonArtProvider* art);

    virtual bool Realize();
    virtual bool Layout();
    virtual wxSize GetMinSize() const;
    virtual wxSize DoGetBestSize() const;

    virtual bool IsSizingContinuous() const;

    virtual void AddChild(wxWindowBase *child);
    virtual void RemoveChild(wxWindowBase *child);

protected:
    wxBorder GetDefaultBorder() const { return wxBORDER_NONE; }
    wxSize GetMinNotMinimisedSize() const;

    virtual wxSize DoGetNextSmallerSize(wxOrientation direction,
                                      wxSize relative_to) const;
    virtual wxSize DoGetNextLargerSize(wxOrientation direction,
                                     wxSize relative_to) const;

    void DoSetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
    void OnSize(wxSizeEvent& evt);
    void OnEraseBackground(wxEraseEvent& evt);
    void OnPaint(wxPaintEvent& evt);
    void OnMouseEnter(wxMouseEvent& evt);
    void OnMouseLeave(wxMouseEvent& evt);

    void CommonInit(const wxString& label, const wxBitmap& icon, long style);

    wxBitmap m_minimised_icon;
    wxBitmap m_minimised_icon_resized;
    wxSize m_minimised_size;
    long m_flags;
    long m_hovered_count;
    bool m_minimised;

#ifndef SWIG
    DECLARE_CLASS(wxRibbonPanel)
    DECLARE_EVENT_TABLE()
#endif
};

#endif // wxUSE_RIBBON

#endif // _WX_RIBBON_PANEL_H_
