///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/scrolbar.h
// Purpose:     wxScrollBar for wxUniversal
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_SCROLBAR_H_
#define _WX_UNIV_SCROLBAR_H_

#ifdef __GNUG__
    #pragma interface "univscrolbar.h"
#endif

class WXDLLEXPORT wxInputHandler;

// ----------------------------------------------------------------------------
// the actions supported by this control
// ----------------------------------------------------------------------------

// various parts of scrollbar may be highlighted
#define wxACTION_SCROLL_HIGHLIGHT_ARROW_UP _T("focusarrowup")
#define wxACTION_SCROLL_HIGHLIGHT_ARROW_DOWN _T("focusarrowdown")
#define wxACTION_SCROLL_HIGHLIGHT_THUMB _T("focusthumb")
#define wxACTION_SCROLL_HIGHLIGHT_BAR _T("focusbar")

// scroll the bar
#define wxACTION_SCROLL_START       _T("start")     // to the beginning
#define wxACTION_SCROLL_END         _T("end")       // to the end
#define wxACTION_SCROLL_LINE_UP     _T("lineup")    // one line up/left
#define wxACTION_SCROLL_PAGE_UP     _T("pageup")    // one page up/left
#define wxACTION_SCROLL_LINE_DOWN   _T("linedown")  // one line down/right
#define wxACTION_SCROLL_PAGE_DOWN   _T("pagedown")  // one page down/right

// the scrollbar thumb may be dragged
#define wxACTION_SCROLL_THUMB_DRAG      _T("thumbdrag")
#define wxACTION_SCROLL_THUMB_MOVE      _T("thumbmove")
#define wxACTION_SCROLL_THUMB_RELEASE   _T("thumbrelease")

// ----------------------------------------------------------------------------
// wxScrollBar
// ----------------------------------------------------------------------------

// class name
#define wxCONTROL_SCROLLBAR _T("scrollbar")

class WXDLLEXPORT wxScrollBar : public wxScrollBarBase
{
public:
    enum
    {
        // which part of scrollbar is currently highlighted?
        Highlight_Arrow1 = 0x0001,
        Highlight_Arrow2 = 0x0002,
        Highlight_Thumb  = 0x0004,
        Highlight_Bar    = 0x0008
    };

    wxScrollBar() { Init(); }
    wxScrollBar(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSB_HORIZONTAL,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxScrollBarNameStr)
    {
        Init();

        Create(parent, id, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSB_HORIZONTAL,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxScrollBarNameStr);

    virtual ~wxScrollBar();

    // implementate base class pure virtuals
    virtual int GetThumbPosition() const;
    virtual int GetThumbSize() const;
    virtual int GetPageSize() const;
    virtual int GetRange() const;

    virtual void SetThumbPosition(int viewStart);
    virtual void SetScrollbar(int position, int thumbSize,
                              int range, int pageSize,
                              bool refresh = TRUE);

    // wxScrollBar actions

protected:
    virtual wxInputHandler *CreateInputHandler() const;
    virtual bool PerformAction(const wxControlAction& action,
                               const wxEvent& event);
    virtual wxSize DoGetBestSize() const;
    virtual void DoDraw(wxControlRenderer *renderer);

    // common part of all ctors
    void Init();

private:
    // total range of the scrollbar in logical units
    int m_range;

    // the size of the thumb in logical units (from 0 to m_range) and its
    // position (from 0 to m_range - m_thumbSize)
    int m_thumbSize,
        m_thumbPos;

    // the page size, i.e. the number of lines by which to scroll when page
    // up/down action is performed
    int m_pageSize;

    DECLARE_DYNAMIC_CLASS(wxScrollBar)
};

#endif // _WX_UNIV_SCROLBAR_H_

