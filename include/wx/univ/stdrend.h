///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/stdrend.h
// Purpose:     wxStdRenderer class declaration
// Author:      Vadim Zeitlin
// Created:     2006-09-18
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_STDREND_H_
#define _WX_UNIV_STDREND_H_

#include "wx/univ/renderer.h"

class WXDLLEXPORT wxColourScheme;

// ----------------------------------------------------------------------------
// wxStdRenderer: implements as much of wxRenderer API as possible generically
// ----------------------------------------------------------------------------

class wxStdRenderer : public wxRenderer
{
public:
    // the renderer will use the given scheme, whose lifetime must be at least
    // as long as of this object itself, to choose the colours for drawing
    wxStdRenderer(const wxColourScheme *scheme);

    virtual void DrawBackground(wxDC& dc,
                                const wxColour& col,
                                const wxRect& rect,
                                int flags = 0,
                                wxWindow *window = NULL);
    virtual void DrawButtonSurface(wxDC& dc,
                                   const wxColour& col,
                                   const wxRect& rect,
                                   int flags);


    virtual void DrawFocusRect(wxDC& dc, const wxRect& rect);
    virtual void DrawLabel(wxDC& dc,
                           const wxString& label,
                           const wxRect& rect,
                           int flags = 0,
                           int alignment = wxALIGN_LEFT | wxALIGN_TOP,
                           int indexAccel = -1,
                           wxRect *rectBounds = NULL);
    virtual void DrawButtonLabel(wxDC& dc,
                                 const wxString& label,
                                 const wxBitmap& image,
                                 const wxRect& rect,
                                 int flags = 0,
                                 int alignment = wxALIGN_LEFT | wxALIGN_TOP,
                                 int indexAccel = -1,
                                 wxRect *rectBounds = NULL);


    virtual void DrawBorder(wxDC& dc,
                            wxBorder border,
                            const wxRect& rect,
                            int flags = 0,
                            wxRect *rectIn = NULL);
    virtual void DrawTextBorder(wxDC& dc,
                                wxBorder border,
                                const wxRect& rect,
                                int flags = 0,
                                wxRect *rectIn = NULL);

    virtual void DrawHorizontalLine(wxDC& dc,
                                    wxCoord y, wxCoord x1, wxCoord x2);
    virtual void DrawVerticalLine(wxDC& dc,
                                  wxCoord x, wxCoord y1, wxCoord y2);
    virtual void DrawFrame(wxDC& dc,
                           const wxString& label,
                           const wxRect& rect,
                           int flags = 0,
                           int alignment = wxALIGN_LEFT,
                           int indexAccel = -1);


    virtual void DrawItem(wxDC& dc,
                          const wxString& label,
                          const wxRect& rect,
                          int flags = 0);

    virtual void DrawCheckButton(wxDC& dc,
                                 const wxString& label,
                                 const wxBitmap& bitmap,
                                 const wxRect& rect,
                                 int flags = 0,
                                 wxAlignment align = wxALIGN_LEFT,
                                 int indexAccel = -1);
    virtual void DrawRadioButton(wxDC& dc,
                                 const wxString& label,
                                 const wxBitmap& bitmap,
                                 const wxRect& rect,
                                 int flags = 0,
                                 wxAlignment align = wxALIGN_LEFT,
                                 int indexAccel = -1);

#if wxUSE_TEXTCTRL
    virtual void DrawTextLine(wxDC& dc,
                              const wxString& text,
                              const wxRect& rect,
                              int selStart = -1,
                              int selEnd = -1,
                              int flags = 0);

    virtual void DrawLineWrapMark(wxDC& dc, const wxRect& rect);
#endif // wxUSE_TEXTCTRL

    virtual wxRect GetBorderDimensions(wxBorder border) const;

    virtual bool AreScrollbarsInsideBorder() const;

#if wxUSE_SCROLLBAR
    virtual wxRect GetScrollbarRect(const wxScrollBar *scrollbar,
                                    wxScrollBar::Element elem,
                                    int thumbPos = -1) const;

    virtual wxCoord GetScrollbarSize(const wxScrollBar *scrollbar);

    virtual wxHitTest HitTestScrollbar(const wxScrollBar *scrollbar,
                                       const wxPoint& pt) const;

    virtual wxCoord ScrollbarToPixel(const wxScrollBar *scrollbar,
                                     int thumbPos = -1);
    virtual int PixelToScrollbar(const wxScrollBar *scrollbar, wxCoord coord);
#endif // wxUSE_SCROLLBAR

protected:
    // various constants
    enum IndicatorType
    {
        IndicatorType_Check,
        IndicatorType_Radio,
        IndicatorType_Menu,
        IndicatorType_Max
    };

    enum IndicatorState
    {
        IndicatorState_Normal,
        IndicatorState_Pressed, // this one is for check/radioboxes
        IndicatorState_Disabled,
        IndicatorState_MaxCtrl,

        // the rest of the states are valid for menu items only
        IndicatorState_Selected = IndicatorState_Pressed,
        IndicatorState_SelectedDisabled = IndicatorState_MaxCtrl,
        IndicatorState_MaxMenu
    };

    enum IndicatorStatus
    {
        IndicatorStatus_Checked,
        IndicatorStatus_Unchecked,
        IndicatorStatus_Undetermined,
        IndicatorStatus_Max
    };

    // translate the appropriate bits in flags to the above enum elements
    static void GetIndicatorsFromFlags(int flags,
                                       IndicatorState& state,
                                       IndicatorStatus& status);

    // fill the rectangle with a brush of given colour (must be valid)
    void DrawSolidRect(wxDC& dc, const wxColour& col, const wxRect& rect);


    // all the functions in this section adjust the rect parameter to
    // correspond to the interiour of the drawn area

        // draw complete rectangle
    void DrawRect(wxDC& dc, wxRect *rect, const wxPen& pen);

        // draw the rectange using the first pen for the left and top sides
        // and the second one for the bottom and right ones
    void DrawShadedRect(wxDC& dc, wxRect *rect,
                        const wxPen& pen1, const wxPen& pen2);

        // border drawing routines, may be overridden in the derived class
    virtual void DrawRaisedBorder(wxDC& dc, wxRect *rect);
    virtual void DrawSunkenBorder(wxDC& dc, wxRect *rect);
    virtual void DrawAntiSunkenBorder(wxDC& dc, wxRect *rect);


    // draw the frame with non-empty label inside the given rectText
    virtual void DrawFrameWithLabel(wxDC& dc,
                                    const wxString& label,
                                    const wxRect& rectFrame,
                                    const wxRect& rectText,
                                    int flags,
                                    int alignment,
                                    int indexAccel);

    // draw the (static box) frame without the part corresponding to rectLabel
    void DrawFrameWithoutLabel(wxDC& dc,
                               const wxRect& rectFrame,
                               const wxRect& rectLabel);


    // common routine for drawing check and radio buttons
    void DrawCheckOrRadioButton(wxDC& dc,
                                const wxString& label,
                                const wxBitmap& bitmap,
                                const wxRect& rect,
                                int flags,
                                wxAlignment align,
                                int indexAccel);

    // return the check/radio bitmap for the given flags
    virtual wxBitmap GetRadioBitmap(int flags) = 0;
    virtual wxBitmap GetCheckBitmap(int flags) = 0;


    // return the starting and ending positions, in pixels, of the thumb of a
    // scrollbar with the given logical position, thumb size and range and the
    // given physical length
    static void GetScrollBarThumbSize(wxCoord length,
                                      int thumbPos,
                                      int thumbSize,
                                      int range,
                                      wxCoord *thumbStart,
                                      wxCoord *thumbEnd);

    // GDI objects we often use
    wxPen m_penBlack,
          m_penDarkGrey,
          m_penLightGrey,
          m_penHighlight;

    // the colours we use, they never change currently so we don't have to ever
    // update m_penXXX objects above
    const wxColourScheme * const m_scheme;

    DECLARE_NO_COPY_CLASS(wxStdRenderer)
};

#endif // _WX_UNIV_STDREND_H_

