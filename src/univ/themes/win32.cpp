///////////////////////////////////////////////////////////////////////////////
// Name:        univ/themes/win32.cpp
// Purpose:     wxUniversal theme implementing Win32-like LNF
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/timer.h"
    #include "wx/intl.h"
    #include "wx/dc.h"
    #include "wx/window.h"

    #include "wx/dcmemory.h"

    #include "wx/button.h"
    #include "wx/listbox.h"
    #include "wx/checklst.h"
    #include "wx/combobox.h"
    #include "wx/scrolbar.h"
    #include "wx/slider.h"
    #include "wx/textctrl.h"
#endif // WX_PRECOMP

#include "wx/notebook.h"
#include "wx/spinbutt.h"
#include "wx/settings.h"
#include "wx/menu.h"

#include "wx/univ/scrtimer.h"

#include "wx/univ/renderer.h"
#include "wx/univ/inphand.h"
#include "wx/univ/colschem.h"
#include "wx/univ/theme.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const int BORDER_THICKNESS = 2;

// the offset between the label and focus rect around it
static const int FOCUS_RECT_OFFSET_X = 1;
static const int FOCUS_RECT_OFFSET_Y = 1;

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
    IndicatorState_Selected = IndicatorState_Pressed, // for menus
    IndicatorState_Disabled,
    IndicatorState_SelectedDisabled,    // only for the menus
    IndicatorState_Max
};

enum IndicatorStatus
{
    IndicatorStatus_Checked,
    IndicatorStatus_Unchecked,
    IndicatorStatus_Max
};

// wxWin32Renderer: draw the GUI elements in Win32 style
// ----------------------------------------------------------------------------

class wxWin32Renderer : public wxRenderer
{
public:
    // constants
    enum wxArrowDirection
    {
        Arrow_Left,
        Arrow_Right,
        Arrow_Up,
        Arrow_Down,
        Arrow_Max
    };

    enum wxArrowStyle
    {
        Arrow_Normal,
        Arrow_Disabled,
        Arrow_Pressed,
        Arrow_Inversed,
        Arrow_InversedDisabled,
        Arrow_StateMax
    };

    // ctor
    wxWin32Renderer(const wxColourScheme *scheme);

    // implement the base class pure virtuals
    virtual void DrawBackground(wxDC& dc,
                                const wxColour& col,
                                const wxRect& rect,
                                int flags = 0);
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
                            wxRect *rectIn = (wxRect *)NULL);
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
    virtual void DrawTextBorder(wxDC& dc,
                                wxBorder border,
                                const wxRect& rect,
                                int flags = 0,
                                wxRect *rectIn = (wxRect *)NULL);
    virtual void DrawButtonBorder(wxDC& dc,
                                  const wxRect& rect,
                                  int flags = 0,
                                  wxRect *rectIn = (wxRect *)NULL);
    virtual void DrawArrow(wxDC& dc,
                           wxDirection dir,
                           const wxRect& rect,
                           int flags = 0);
    virtual void DrawScrollbarArrow(wxDC& dc,
                                    wxDirection dir,
                                    const wxRect& rect,
                                    int flags = 0)
        { DrawArrow(dc, dir, rect, flags); }
    virtual void DrawScrollbarThumb(wxDC& dc,
                                    wxOrientation orient,
                                    const wxRect& rect,
                                    int flags = 0);
    virtual void DrawScrollbarShaft(wxDC& dc,
                                    wxOrientation orient,
                                    const wxRect& rect,
                                    int flags = 0);
    virtual void DrawScrollCorner(wxDC& dc,
                                  const wxRect& rect);
    virtual void DrawItem(wxDC& dc,
                          const wxString& label,
                          const wxRect& rect,
                          int flags = 0);
    virtual void DrawCheckItem(wxDC& dc,
                               const wxString& label,
                               const wxBitmap& bitmap,
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
    virtual void DrawTextLine(wxDC& dc,
                              const wxString& text,
                              const wxRect& rect,
                              int selStart = -1,
                              int selEnd = -1,
                              int flags = 0);
    virtual void DrawLineWrapMark(wxDC& dc, const wxRect& rect);
    virtual void DrawTab(wxDC& dc,
                         const wxRect& rect,
                         wxDirection dir,
                         const wxString& label,
                         const wxBitmap& bitmap = wxNullBitmap,
                         int flags = 0,
                         int indexAccel = -1);

    virtual void DrawSliderShaft(wxDC& dc,
                                 const wxRect& rect,
                                 wxOrientation orient,
                                 int flags = 0,
                                 wxRect *rectShaft = NULL);
    virtual void DrawSliderThumb(wxDC& dc,
                                 const wxRect& rect,
                                 wxOrientation orient,
                                 int flags = 0);
    virtual void DrawSliderTicks(wxDC& dc,
                                 const wxRect& rect,
                                 const wxSize& sizeThumb,
                                 wxOrientation orient,
                                 int start,
                                 int end,
                                 int step = 1,
                                 int flags = 0);
#if wxUSE_MENUS

    virtual void DrawMenuBarItem(wxDC& dc,
                                 const wxRect& rect,
                                 const wxString& label,
                                 int flags = 0,
                                 int indexAccel = -1);
    virtual void DrawMenuItem(wxDC& dc,
                              wxCoord y,
                              const wxMenuGeometryInfo& geometryInfo,
                              const wxString& label,
                              const wxString& accel,
                              const wxBitmap& bitmap = wxNullBitmap,
                              int flags = 0,
                              int indexAccel = -1);
    virtual void DrawMenuSeparator(wxDC& dc,
                                   wxCoord y,
                                   const wxMenuGeometryInfo& geomInfo);
#endif
    virtual void GetComboBitmaps(wxBitmap *bmpNormal,
                                 wxBitmap *bmpPressed,
                                 wxBitmap *bmpDisabled);

    virtual void AdjustSize(wxSize *size, const wxWindow *window);
    virtual wxRect GetBorderDimensions(wxBorder border) const;
    virtual bool AreScrollbarsInsideBorder() const;

    virtual wxSize GetScrollbarArrowSize() const
        { return m_sizeScrollbarArrow; }
    virtual wxRect GetScrollbarRect(const wxScrollBar *scrollbar,
                                    wxScrollBar::Element elem,
                                    int thumbPos = -1) const;
    virtual wxCoord GetScrollbarSize(const wxScrollBar *scrollbar);
    virtual wxHitTest HitTestScrollbar(const wxScrollBar *scrollbar,
                                       const wxPoint& pt) const;
    virtual wxCoord ScrollbarToPixel(const wxScrollBar *scrollbar,
                                     int thumbPos = -1);
    virtual int PixelToScrollbar(const wxScrollBar *scrollbar, wxCoord coord);
    virtual wxCoord GetListboxItemHeight(wxCoord fontHeight)
        { return fontHeight + 2; }
    virtual wxSize GetCheckBitmapSize() const
        { return wxSize(13, 13); }
    virtual wxSize GetRadioBitmapSize() const
        { return wxSize(12, 12); }
    virtual wxCoord GetCheckItemMargin() const
        { return 0; }

    virtual wxRect GetTextTotalArea(const wxTextCtrl *text,
                                    const wxRect& rect);
    virtual wxRect GetTextClientArea(const wxTextCtrl *text,
                                     const wxRect& rect,
                                     wxCoord *extraSpaceBeyond);

    virtual wxSize GetTabIndent() const { return wxSize(2, 2); }
    virtual wxSize GetTabPadding() const { return wxSize(6, 5); }

    virtual wxCoord GetSliderDim() const { return 20; }
    virtual wxCoord GetSliderTickLen() const { return 4; }
    virtual wxRect GetSliderShaftRect(const wxRect& rect,
                                      wxOrientation orient) const;
    virtual wxSize GetSliderThumbSize(const wxRect& rect,
                                      wxOrientation orient) const;
    virtual wxSize GetProgressBarStep() const { return wxSize(16, 32); }

#if wxUSE_MENUS
    virtual wxSize GetMenuBarItemSize(const wxSize& sizeText) const;
    virtual wxMenuGeometryInfo *GetMenuGeometry(wxWindow *win,
                                                const wxMenu& menu) const;
#endif
protected:
    // helper of DrawLabel() and DrawCheckOrRadioButton()
    void DoDrawLabel(wxDC& dc,
                     const wxString& label,
                     const wxRect& rect,
                     int flags = 0,
                     int alignment = wxALIGN_LEFT | wxALIGN_TOP,
                     int indexAccel = -1,
                     wxRect *rectBounds = NULL,
                     const wxPoint& focusOffset
                        = wxPoint(FOCUS_RECT_OFFSET_X, FOCUS_RECT_OFFSET_Y));

    // common part of DrawLabel() and DrawItem()
    void DrawFocusRect(wxDC& dc, const wxRect& rect);

    // DrawLabel() and DrawButtonLabel() helper
    void DrawLabelShadow(wxDC& dc,
                         const wxString& label,
                         const wxRect& rect,
                         int alignment,
                         int indexAccel);

    // DrawButtonBorder() helper
    void DoDrawBackground(wxDC& dc,
                          const wxColour& col,
                          const wxRect& rect);

    // DrawBorder() helpers: all of them shift and clip the DC after drawing
    // the border

    // just draw a rectangle with the given pen
    void DrawRect(wxDC& dc, wxRect *rect, const wxPen& pen);

    // draw the lower left part of rectangle
    void DrawHalfRect(wxDC& dc, wxRect *rect, const wxPen& pen);

    // draw the rectange using the first brush for the left and top sides and
    // the second one for the bottom and right ones
    void DrawShadedRect(wxDC& dc, wxRect *rect,
                        const wxPen& pen1, const wxPen& pen2);

    // draw the normal 3D border
    void DrawRaisedBorder(wxDC& dc, wxRect *rect);

    // draw the sunken 3D border
    void DrawSunkenBorder(wxDC& dc, wxRect *rect);

    // draw the border used for scrollbar arrows
    void DrawArrowBorder(wxDC& dc, wxRect *rect, bool isPressed = FALSE);

    // public DrawArrow()s helper
    void DrawArrow(wxDC& dc, const wxRect& rect,
                   wxArrowDirection arrowDir, wxArrowStyle arrowStyle);

    // DrawArrowButton is used by DrawScrollbar and DrawComboButton
    void DrawArrowButton(wxDC& dc, const wxRect& rect,
                         wxArrowDirection arrowDir,
                         wxArrowStyle arrowStyle);

    // DrawCheckButton/DrawRadioButton helper
    void DrawCheckOrRadioButton(wxDC& dc,
                                const wxString& label,
                                const wxBitmap& bitmap,
                                const wxRect& rect,
                                int flags,
                                wxAlignment align,
                                int indexAccel,
                                wxCoord focusOffsetY);

    // draw a normal or transposed line (useful for using the same code fo both
    // horizontal and vertical widgets)
    void DrawLine(wxDC& dc,
                  wxCoord x1, wxCoord y1,
                  wxCoord x2, wxCoord y2,
                  bool transpose = FALSE)
    {
        if ( transpose )
            dc.DrawLine(y1, x1, y2, x2);
        else
            dc.DrawLine(x1, y1, x2, y2);
    }

    // get the standard check/radio button bitmap
    wxBitmap GetIndicator(IndicatorType indType, int flags);
    wxBitmap GetCheckBitmap(int flags)
        { return GetIndicator(IndicatorType_Check, flags); }
    wxBitmap GetRadioBitmap(int flags)
        { return GetIndicator(IndicatorType_Radio, flags); }

private:
    const wxColourScheme *m_scheme;

    // the sizing parameters (TODO make them changeable)
    wxSize m_sizeScrollbarArrow;

    // GDI objects we use for drawing
    wxColour m_colDarkGrey,
             m_colHighlight;

    wxPen m_penBlack,
          m_penDarkGrey,
          m_penLightGrey,
          m_penHighlight;

    // first row is for the normal state, second - for the disabled
    wxBitmap m_bmpArrows[Arrow_StateMax][Arrow_Max];
};

// ----------------------------------------------------------------------------
// wxWin32InputHandler and derived classes: process the keyboard and mouse
// messages according to Windows standards
// ----------------------------------------------------------------------------

class wxWin32InputHandler : public wxInputHandler
{
public:
    wxWin32InputHandler(wxWin32Renderer *renderer);

    virtual bool HandleKey(wxControl *control,
                           const wxKeyEvent& event,
                           bool pressed);
    virtual bool HandleMouse(wxControl *control,
                             const wxMouseEvent& event);

protected:
    wxWin32Renderer *m_renderer;
};

class wxWin32ScrollBarInputHandler : public wxStdScrollBarInputHandler
{
public:
    wxWin32ScrollBarInputHandler(wxWin32Renderer *renderer,
                                 wxInputHandler *handler);

    virtual bool HandleMouse(wxControl *control, const wxMouseEvent& event);
    virtual bool HandleMouseMove(wxControl *control, const wxMouseEvent& event);

    virtual bool OnScrollTimer(wxScrollBar *scrollbar,
                               const wxControlAction& action);

protected:
    virtual bool IsAllowedButton(int button) { return button == 1; }

    virtual void Highlight(wxScrollBar *scrollbar, bool doIt)
    {
        // we don't highlight anything
    }

    // the first and last event which caused the thumb to move
    wxMouseEvent m_eventStartDrag,
                 m_eventLastDrag;

    // have we paused the scrolling because the mouse moved?
    bool m_scrollPaused;

    // we remember the interval of the timer to be able to restart it
    int m_interval;
};

class wxWin32CheckboxInputHandler : public wxStdCheckboxInputHandler
{
public:
    wxWin32CheckboxInputHandler(wxInputHandler *handler)
        : wxStdCheckboxInputHandler(handler) { }

    virtual bool HandleKey(wxControl *control,
                           const wxKeyEvent& event,
                           bool pressed);
};

class wxWin32TextCtrlInputHandler : public wxStdTextCtrlInputHandler
{
public:
    wxWin32TextCtrlInputHandler(wxInputHandler *handler)
        : wxStdTextCtrlInputHandler(handler) { }

    virtual bool HandleKey(wxControl *control,
                           const wxKeyEvent& event,
                           bool pressed);
};

// ----------------------------------------------------------------------------
// wxWin32ColourScheme: uses (default) Win32 colours
// ----------------------------------------------------------------------------

class wxWin32ColourScheme : public wxColourScheme
{
public:
    virtual wxColour Get(StdColour col) const;
    virtual wxColour GetBackground(wxWindow *win) const;
};

// ----------------------------------------------------------------------------
// wxWin32Theme
// ----------------------------------------------------------------------------

WX_DEFINE_ARRAY(wxInputHandler *, wxArrayHandlers);

class wxWin32Theme : public wxTheme
{
public:
    wxWin32Theme();
    virtual ~wxWin32Theme();

    virtual wxRenderer *GetRenderer() { return m_renderer; }
    virtual wxInputHandler *GetInputHandler(const wxString& control);
    virtual wxColourScheme *GetColourScheme();

private:
    // get the default input handler
    wxInputHandler *GetDefaultInputHandler();

    wxWin32Renderer *m_renderer;

    // the names of the already created handlers and the handlers themselves
    // (these arrays are synchronized)
    wxSortedArrayString m_handlerNames;
    wxArrayHandlers m_handlers;

    wxWin32InputHandler *m_handlerDefault;

    wxWin32ColourScheme *m_scheme;

    WX_DECLARE_THEME(win32)
};

// ----------------------------------------------------------------------------
// standard bitmaps
// ----------------------------------------------------------------------------

// menu bitmaps

static const char *checked_menu_xpm[] = {
/* columns rows colors chars-per-pixel */
"9 9 2 1",
"w c None",
"b c black",
/* pixels */
"wwwwwwwww",
"wwwwwwwbw",
"wwwwwwbbw",
"wbwwwbbbw",
"wbbwbbbww",
"wbbbbbwww",
"wwbbbwwww",
"wwwbwwwww",
"wwwwwwwww"
};

static const char *selected_checked_menu_xpm[] = {
/* columns rows colors chars-per-pixel */
"9 9 2 1",
"w c None",
"b c white",
/* pixels */
"wwwwwwwww",
"wwwwwwwbw",
"wwwwwwbbw",
"wbwwwbbbw",
"wbbwbbbww",
"wbbbbbwww",
"wwbbbwwww",
"wwwbwwwww",
"wwwwwwwww"
};

static const char *disabled_checked_menu_xpm[] = {
/* columns rows colors chars-per-pixel */
"9 9 3 1",
"w c None",
"b c #7f7f7f",
"W c #e0e0e0",
/* pixels */
"wwwwwwwww",
"wwwwwwwbw",
"wwwwwwbbW",
"wbwwwbbbW",
"wbbwbbbWW",
"wbbbbbWWw",
"wwbbbWWww",
"wwwbWWwww",
"wwwwWwwww"
};

static const char *selected_disabled_checked_menu_xpm[] = {
/* columns rows colors chars-per-pixel */
"9 9 2 1",
"w c None",
"b c #7f7f7f",
/* pixels */
"wwwwwwwww",
"wwwwwwwbw",
"wwwwwwbbw",
"wbwwwbbbw",
"wbbwbbbww",
"wbbbbbwww",
"wwbbbwwww",
"wwwbwwwww",
"wwwwwwwww"
};

// checkbox and radiobox bitmaps below

static const char *checked_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 5 1",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"ddddddddddddh",
"dbbbbbbbbbbgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwbwgh",
"dbwwwwwwbbwgh",
"dbwbwwwbbbwgh",
"dbwbbwbbbwwgh",
"dbwbbbbbwwwgh",
"dbwwbbbwwwwgh",
"dbwwwbwwwwwgh",
"dbwwwwwwwwwgh",
"dgggggggggggh",
"hhhhhhhhhhhhh"
};

static const char *pressed_checked_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 4 1",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"ddddddddddddh",
"dbbbbbbbbbbgh",
"dbggggggggggh",
"dbgggggggbggh",
"dbggggggbbggh",
"dbgbgggbbbggh",
"dbgbbgbbbgggh",
"dbgbbbbbggggh",
"dbggbbbgggggh",
"dbgggbggggggh",
"dbggggggggggh",
"dgggggggggggh",
"hhhhhhhhhhhhh"
};

static const char *pressed_disabled_checked_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 4 1",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"ddddddddddddh",
"dbbbbbbbbbbgh",
"dbggggggggggh",
"dbgggggggdggh",
"dbggggggddggh",
"dbgdgggdddggh",
"dbgddgdddgggh",
"dbgdddddggggh",
"dbggdddgggggh",
"dbgggdggggggh",
"dbggggggggggh",
"dgggggggggggh",
"hhhhhhhhhhhhh"
};

static const char *checked_item_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 3 1",
"w c white",
"b c black",
"d c #808080",
/* pixels */
"wwwwwwwwwwwww",
"wdddddddddddw",
"wdwwwwwwwwwdw",
"wdwwwwwwwbwdw",
"wdwwwwwwbbwdw",
"wdwbwwwbbbwdw",
"wdwbbwbbbwwdw",
"wdwbbbbbwwwdw",
"wdwwbbbwwwwdw",
"wdwwwbwwwwwdw",
"wdwwwwwwwwwdw",
"wdddddddddddw",
"wwwwwwwwwwwww"
};

static const char *unchecked_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 5 1",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"ddddddddddddh",
"dbbbbbbbbbbgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dbwwwwwwwwwgh",
"dgggggggggggh",
"hhhhhhhhhhhhh"
};

static const char *pressed_unchecked_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 4 1",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"ddddddddddddh",
"dbbbbbbbbbbgh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"dbggggggggggh",
"hhhhhhhhhhhhh"
};

static const char *unchecked_item_xpm[] = {
/* columns rows colors chars-per-pixel */
"13 13 2 1",
"w c white",
"d c #808080",
/* pixels */
"wwwwwwwwwwwww",
"wdddddddddddw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdwwwwwwwwwdw",
"wdddddddddddw",
"wwwwwwwwwwwww"
};

static const char *checked_radio_xpm[] = {
/* columns rows colors chars-per-pixel */
"12 12 6 1",
"  c None",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"    dddd    ",
"  ddbbbbdd  ",
" dbbwwwwbbh ",
" dbwwwwwwgh ",
"dbwwwbbwwwgh",
"dbwwbbbbwwgh",
"dbwwbbbbwwgh",
"dbwwwbbwwwgh",
" dbwwwwwwgh ",
" dggwwwwggh ",
"  hhgggghh  ",
"    hhhh    "
};

static const char *pressed_checked_radio_xpm[] = {
/* columns rows colors chars-per-pixel */
"12 12 6 1",
"  c None",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"    dddd    ",
"  ddbbbbdd  ",
" dbbggggbbh ",
" dbgggggggh ",
"dbgggbbggggh",
"dbggbbbbgggh",
"dbggbbbbgggh",
"dbgggbbggggh",
" dbgggggggh ",
" dggggggggh ",
"  hhgggghh  ",
"    hhhh    "
};

static const char *pressed_disabled_checked_radio_xpm[] = {
/* columns rows colors chars-per-pixel */
"12 12 6 1",
"  c None",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"    dddd    ",
"  ddbbbbdd  ",
" dbbggggbbh ",
" dbgggggggh ",
"dbgggddggggh",
"dbggddddgggh",
"dbggddddgggh",
"dbgggddggggh",
" dbgggggggh ",
" dggggggggh ",
"  hhgggghh  ",
"    hhhh    ",
};

static const char *unchecked_radio_xpm[] = {
/* columns rows colors chars-per-pixel */
"12 12 6 1",
"  c None",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"    dddd    ",
"  ddbbbbdd  ",
" dbbwwwwbbh ",
" dbwwwwwwgh ",
"dbwwwwwwwwgh",
"dbwwwwwwwwgh",
"dbwwwwwwwwgh",
"dbwwwwwwwwgh",
" dbwwwwwwgh ",
" dggwwwwggh ",
"  hhgggghh  ",
"    hhhh    "
};

static const char *pressed_unchecked_radio_xpm[] = {
/* columns rows colors chars-per-pixel */
"12 12 6 1",
"  c None",
"w c white",
"b c black",
"d c #7f7f7f",
"g c #c0c0c0",
"h c #e0e0e0",
/* pixels */
"    dddd    ",
"  ddbbbbdd  ",
" dbbggggbbh ",
" dbgggggggh ",
"dbgggggggggh",
"dbgggggggggh",
"dbgggggggggh",
"dbgggggggggh",
" dbgggggggh ",
" dggggggggh ",
"  hhgggghh  ",
"    hhhh    "
};

static const char **
    bmpIndicators[IndicatorType_Max][IndicatorState_Max][IndicatorStatus_Max] =
{
    // checkboxes first
    {
        // normal state
        { checked_xpm, unchecked_xpm },

        // pressed state
        { pressed_checked_xpm, pressed_unchecked_xpm },

        // disabled state
        { pressed_disabled_checked_xpm, pressed_unchecked_xpm },
    },

    // radio
    {
        // normal state
        { checked_radio_xpm, unchecked_radio_xpm },

        // pressed state
        { pressed_checked_radio_xpm, pressed_unchecked_radio_xpm },

        // disabled state
        { pressed_disabled_checked_radio_xpm, pressed_unchecked_radio_xpm },
    },

    // menu
    {
        // normal state
        { checked_menu_xpm, NULL },

        // selected state
        { selected_checked_menu_xpm, NULL },

        // disabled state
        { disabled_checked_menu_xpm, NULL },

        // disabled selected state
        { selected_disabled_checked_menu_xpm, NULL },
    }
};

// ============================================================================
// implementation
// ============================================================================

WX_IMPLEMENT_THEME(wxWin32Theme, win32, wxTRANSLATE("Win32 theme"));

// ----------------------------------------------------------------------------
// wxWin32Theme
// ----------------------------------------------------------------------------

wxWin32Theme::wxWin32Theme()
{
    m_scheme = new wxWin32ColourScheme;
    m_renderer = new wxWin32Renderer(m_scheme);
    m_handlerDefault = NULL;
}

wxWin32Theme::~wxWin32Theme()
{
    size_t count = m_handlers.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        if ( m_handlers[n] != m_handlerDefault )
            delete m_handlers[n];
    }

    delete m_handlerDefault;

    delete m_renderer;
    delete m_scheme;
}

wxInputHandler *wxWin32Theme::GetDefaultInputHandler()
{
    if ( !m_handlerDefault )
    {
        m_handlerDefault = new wxWin32InputHandler(m_renderer);
    }

    return m_handlerDefault;
}

wxInputHandler *wxWin32Theme::GetInputHandler(const wxString& control)
{
    wxInputHandler *handler;
    int n = m_handlerNames.Index(control);
    if ( n == wxNOT_FOUND )
    {
        // create a new handler
        if ( control == wxINP_HANDLER_SCROLLBAR )
            handler = new wxWin32ScrollBarInputHandler(m_renderer,
                                                       GetDefaultInputHandler());
#if wxUSE_BUTTON
        else if ( control == wxINP_HANDLER_BUTTON )
            handler = new wxStdButtonInputHandler(GetDefaultInputHandler());
#endif // wxUSE_BUTTON
#if wxUSE_CHECKBOX
        else if ( control == wxINP_HANDLER_CHECKBOX )
            handler = new wxWin32CheckboxInputHandler(GetDefaultInputHandler());
#endif // wxUSE_CHECKBOX
#if wxUSE_COMBOBOX
        else if ( control == wxINP_HANDLER_COMBOBOX )
            handler = new wxStdComboBoxInputHandler(GetDefaultInputHandler());
#endif // wxUSE_COMBOBOX
#if wxUSE_LISTBOX
        else if ( control == wxINP_HANDLER_LISTBOX )
            handler = new wxStdListboxInputHandler(GetDefaultInputHandler());
#endif // wxUSE_LISTBOX
#if wxUSE_CHECKLISTBOX
        else if ( control == wxINP_HANDLER_CHECKLISTBOX )
            handler = new wxStdCheckListboxInputHandler(GetDefaultInputHandler());
#endif // wxUSE_CHECKLISTBOX
#if wxUSE_TEXTCTRL
        else if ( control == wxINP_HANDLER_TEXTCTRL )
            handler = new wxWin32TextCtrlInputHandler(GetDefaultInputHandler());
#endif // wxUSE_TEXTCTRL
#if wxUSE_SLIDER
        else if ( control == wxINP_HANDLER_SLIDER )
            handler = new wxStdSliderButtonInputHandler(GetDefaultInputHandler());
#endif // wxUSE_SLIDER
#if wxUSE_SPINBTN
        else if ( control == wxINP_HANDLER_SPINBTN )
            handler = new wxStdSpinButtonInputHandler(GetDefaultInputHandler());
#endif // wxUSE_SPINBTN
#if wxUSE_NOTEBOOK
        else if ( control == wxINP_HANDLER_NOTEBOOK )
            handler = new wxStdNotebookInputHandler(GetDefaultInputHandler());
#endif // wxUSE_NOTEBOOK
        else
            handler = GetDefaultInputHandler();

        n = m_handlerNames.Add(control);
        m_handlers.Insert(handler, n);
    }
    else // we already have it
    {
        handler = m_handlers[n];
    }

    return handler;
}

wxColourScheme *wxWin32Theme::GetColourScheme()
{
    return m_scheme;
}

// ============================================================================
// wxWin32ColourScheme
// ============================================================================

wxColour wxWin32ColourScheme::GetBackground(wxWindow *win) const
{
    wxColour col;
    if ( win->UseBgCol() )
    {
        // use the user specified colour
        col = win->GetBackgroundColour();
    }

    if ( win->IsContainerWindow() )
    {
        wxTextCtrl *text = wxDynamicCast(win, wxTextCtrl);
        if ( text )
        {
            if ( !text->IsEnabled() ) // not IsEditable()
                col = Get(CONTROL);
            //else: execute code below
        }

        if ( !col.Ok() )
        {
            // doesn't depend on the state
            col = Get(WINDOW);
        }
    }
    else
    {
        int flags = win->GetStateFlags();

        // the colour set by the user should be used for the normal state
        // and for the states for which we don't have any specific colours
        if ( !col.Ok() || (flags != 0) )
        {
            if ( wxDynamicCast(win, wxScrollBar) )
                col = Get(flags & wxCONTROL_PRESSED ? SCROLLBAR_PRESSED
                                                    : SCROLLBAR);
            else
                col = Get(CONTROL);
        }
    }

    return col;
}

wxColour wxWin32ColourScheme::Get(wxWin32ColourScheme::StdColour col) const
{
    switch ( col )
    {
        case WINDOW:            return *wxWHITE;

        case CONTROL_PRESSED:
        case CONTROL_CURRENT:
        case CONTROL:           return wxColour(0xc0c0c0);

        case CONTROL_TEXT:      return *wxBLACK;

        case SCROLLBAR:         return wxColour(0xe0e0e0);
        case SCROLLBAR_PRESSED: return *wxBLACK;

        case HIGHLIGHT:         return wxColour(0x800000);
        case HIGHLIGHT_TEXT:    return wxColour(0xffffff);

        case SHADOW_DARK:       return *wxBLACK;

        case CONTROL_TEXT_DISABLED:
        case SHADOW_HIGHLIGHT:  return wxColour(0xe0e0e0);

        case SHADOW_IN:         return wxColour(0xc0c0c0);

        case CONTROL_TEXT_DISABLED_SHADOW:
        case SHADOW_OUT:        return wxColour(0x7f7f7f);

        case MAX:
        default:
            wxFAIL_MSG(_T("invalid standard colour"));
            return *wxBLACK;
    }
}

// ============================================================================
// wxWin32Renderer
// ============================================================================

// ----------------------------------------------------------------------------
// construction
// ----------------------------------------------------------------------------

wxWin32Renderer::wxWin32Renderer(const wxColourScheme *scheme)
{
    // init data
    m_scheme = scheme;
    m_sizeScrollbarArrow = wxSize(16, 16);

    // init colours and pens
    m_penBlack = wxPen(wxSCHEME_COLOUR(scheme, SHADOW_DARK), 0, wxSOLID);

    m_colDarkGrey = wxSCHEME_COLOUR(scheme, SHADOW_OUT);
    m_penDarkGrey = wxPen(m_colDarkGrey, 0, wxSOLID);

    m_penLightGrey = wxPen(wxSCHEME_COLOUR(scheme, SHADOW_IN), 0, wxSOLID);

    m_colHighlight = wxSCHEME_COLOUR(scheme, SHADOW_HIGHLIGHT);
    m_penHighlight = wxPen(m_colHighlight, 0, wxSOLID);

    // init the arrow bitmaps
    static const size_t ARROW_WIDTH = 7;
    static const size_t ARROW_LENGTH = 4;

    wxMask *mask;
    wxMemoryDC dcNormal,
               dcDisabled,
               dcInverse;
    for ( size_t n = 0; n < Arrow_Max; n++ )
    {
        bool isVertical = n > Arrow_Right;
        int w, h;
        if ( isVertical )
        {
            w = ARROW_WIDTH;
            h = ARROW_LENGTH;
        }
        else
        {
            h = ARROW_WIDTH;
            w = ARROW_LENGTH;
        }

        // disabled arrow is larger because of the shadow
        m_bmpArrows[Arrow_Normal][n].Create(w, h);
        m_bmpArrows[Arrow_Disabled][n].Create(w + 1, h + 1);

        dcNormal.SelectObject(m_bmpArrows[Arrow_Normal][n]);
        dcDisabled.SelectObject(m_bmpArrows[Arrow_Disabled][n]);

        dcNormal.SetBackground(*wxWHITE_BRUSH);
        dcDisabled.SetBackground(*wxWHITE_BRUSH);
        dcNormal.Clear();
        dcDisabled.Clear();

        dcNormal.SetPen(m_penBlack);
        dcDisabled.SetPen(m_penDarkGrey);

        // calculate the position of the point of the arrow
        wxCoord x1, y1;
        if ( isVertical )
        {
            x1 = (ARROW_WIDTH - 1)/2;
            y1 = n == Arrow_Up ? 0 : ARROW_LENGTH - 1;
        }
        else // horizontal
        {
            x1 = n == Arrow_Left ? 0 : ARROW_LENGTH - 1;
            y1 = (ARROW_WIDTH - 1)/2;
        }

        wxCoord x2 = x1,
                y2 = y1;

        if ( isVertical )
            x2++;
        else
            y2++;

        for ( size_t i = 0; i < ARROW_LENGTH; i++ )
        {
            dcNormal.DrawLine(x1, y1, x2, y2);
            dcDisabled.DrawLine(x1, y1, x2, y2);

            if ( isVertical )
            {
                x1--;
                x2++;

                if ( n == Arrow_Up )
                {
                    y1++;
                    y2++;
                }
                else // down arrow
                {
                    y1--;
                    y2--;
                }
            }
            else // left or right arrow
            {
                y1--;
                y2++;

                if ( n == Arrow_Left )
                {
                    x1++;
                    x2++;
                }
                else
                {
                    x1--;
                    x2--;
                }
            }
        }

        // draw the shadow for the disabled one
        dcDisabled.SetPen(m_penHighlight);
        switch ( n )
        {
            case Arrow_Left:
                y1 += 2;
                dcDisabled.DrawLine(x1, y1, x2, y2);
                break;

            case Arrow_Right:
                x1 = ARROW_LENGTH - 1;
                y1 = (ARROW_WIDTH - 1)/2 + 1;
                x2 = 0;
                y2 = ARROW_WIDTH;
                dcDisabled.DrawLine(x1, y1, x2, y2);
                dcDisabled.DrawLine(++x1, y1, x2, ++y2);
                break;

            case Arrow_Up:
                x1 += 2;
                dcDisabled.DrawLine(x1, y1, x2, y2);
                break;

            case Arrow_Down:
                x1 = ARROW_WIDTH - 1;
                y1 = 1;
                x2 = (ARROW_WIDTH - 1)/2;
                y2 = ARROW_LENGTH;
                dcDisabled.DrawLine(x1, y1, x2, y2);
                dcDisabled.DrawLine(++x1, y1, x2, ++y2);
                break;

        }

        // create the inversed bitmap but only for the right arrow as we only
        // use it for the menus
        if ( n == Arrow_Right )
        {
            m_bmpArrows[Arrow_Inversed][n].Create(w, h);
            dcInverse.SelectObject(m_bmpArrows[Arrow_Inversed][n]);
            dcInverse.Clear();
            dcInverse.Blit(0, 0, w, h,
                          &dcNormal, 0, 0,
                          wxXOR);
            dcInverse.SelectObject(wxNullBitmap);

            mask = new wxMask(m_bmpArrows[Arrow_Inversed][n], *wxBLACK);
            m_bmpArrows[Arrow_Inversed][n].SetMask(mask);

            m_bmpArrows[Arrow_InversedDisabled][n].Create(w, h);
            dcInverse.SelectObject(m_bmpArrows[Arrow_InversedDisabled][n]);
            dcInverse.Clear();
            dcInverse.Blit(0, 0, w, h,
                          &dcDisabled, 0, 0,
                          wxXOR);
            dcInverse.SelectObject(wxNullBitmap);

            mask = new wxMask(m_bmpArrows[Arrow_InversedDisabled][n], *wxBLACK);
            m_bmpArrows[Arrow_InversedDisabled][n].SetMask(mask);
        }

        dcNormal.SelectObject(wxNullBitmap);
        dcDisabled.SelectObject(wxNullBitmap);

        mask = new wxMask(m_bmpArrows[Arrow_Normal][n], *wxWHITE);
        m_bmpArrows[Arrow_Normal][n].SetMask(mask);
        mask = new wxMask(m_bmpArrows[Arrow_Disabled][n], *wxWHITE);
        m_bmpArrows[Arrow_Disabled][n].SetMask(mask);

        m_bmpArrows[Arrow_Pressed][n] = m_bmpArrows[Arrow_Normal][n];
    }
}

// ----------------------------------------------------------------------------
// border stuff
// ----------------------------------------------------------------------------

/*
   The raised border in Win32 looks like this:

   IIIIIIIIIIIIIIIIIIIIIIB
   I                    GB
   I                    GB  I = white       (HILIGHT)
   I                    GB  H = light grey  (LIGHT)
   I                    GB  G = dark grey   (SHADOI)
   I                    GB  B = black       (DKSHADOI)
   I                    GB  I = hIghlight (COLOR_3DHILIGHT)
   I                    GB
   IGGGGGGGGGGGGGGGGGGGGGB
   BBBBBBBBBBBBBBBBBBBBBBB

   The sunken border looks like this:

   GGGGGGGGGGGGGGGGGGGGGGI
   GBBBBBBBBBBBBBBBBBBBBHI
   GB                   HI
   GB                   HI
   GB                   HI
   GB                   HI
   GB                   HI
   GB                   HI
   GHHHHHHHHHHHHHHHHHHHHHI
   IIIIIIIIIIIIIIIIIIIIIII

   The static border (used for the controls which don't get focus) is like
   this:

   GGGGGGGGGGGGGGGGGGGGGGW
   G                     W
   G                     W
   G                     W
   G                     W
   G                     W
   G                     W
   G                     W
   WWWWWWWWWWWWWWWWWWWWWWW

   The most complicated is the double border:

   HHHHHHHHHHHHHHHHHHHHHHB
   HWWWWWWWWWWWWWWWWWWWWGB
   HWHHHHHHHHHHHHHHHHHHHGB
   HWH                 HGB
   HWH                 HGB
   HWH                 HGB
   HWH                 HGB
   HWHHHHHHHHHHHHHHHHHHHGB
   HGGGGGGGGGGGGGGGGGGGGGB
   BBBBBBBBBBBBBBBBBBBBBBB

   And the simple border is, well, simple:

   BBBBBBBBBBBBBBBBBBBBBBB
   B                     B
   B                     B
   B                     B
   B                     B
   B                     B
   B                     B
   B                     B
   B                     B
   BBBBBBBBBBBBBBBBBBBBBBB
*/

void wxWin32Renderer::DrawRect(wxDC& dc, wxRect *rect, const wxPen& pen)
{
    // draw
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(*rect);

    // adjust the rect
    rect->Inflate(-1);
}

void wxWin32Renderer::DrawHalfRect(wxDC& dc, wxRect *rect, const wxPen& pen)
{
    // draw the bottom and right sides
    dc.SetPen(pen);
    dc.DrawLine(rect->GetLeft(), rect->GetBottom(),
                rect->GetRight() + 1, rect->GetBottom());
    dc.DrawLine(rect->GetRight(), rect->GetTop(),
                rect->GetRight(), rect->GetBottom());

    // adjust the rect
    rect->width--;
    rect->height--;
}

void wxWin32Renderer::DrawShadedRect(wxDC& dc, wxRect *rect,
                                     const wxPen& pen1, const wxPen& pen2)
{
    // draw the rectangle
    dc.SetPen(pen1);
    dc.DrawLine(rect->GetLeft(), rect->GetTop(),
                rect->GetLeft(), rect->GetBottom());
    dc.DrawLine(rect->GetLeft() + 1, rect->GetTop(),
                rect->GetRight(), rect->GetTop());
    dc.SetPen(pen2);
    dc.DrawLine(rect->GetRight(), rect->GetTop(),
                rect->GetRight(), rect->GetBottom());
    dc.DrawLine(rect->GetLeft(), rect->GetBottom(),
                rect->GetRight() + 1, rect->GetBottom());

    // adjust the rect
    rect->Inflate(-1);
}

void wxWin32Renderer::DrawRaisedBorder(wxDC& dc, wxRect *rect)
{
    DrawShadedRect(dc, rect, m_penHighlight, m_penBlack);
    DrawShadedRect(dc, rect, m_penLightGrey, m_penDarkGrey);
}

void wxWin32Renderer::DrawSunkenBorder(wxDC& dc, wxRect *rect)
{
    DrawShadedRect(dc, rect, m_penDarkGrey, m_penHighlight);
    DrawShadedRect(dc, rect, m_penBlack, m_penLightGrey);
}

void wxWin32Renderer::DrawArrowBorder(wxDC& dc, wxRect *rect, bool isPressed)
{
    if ( isPressed )
    {
        DrawRect(dc, rect, m_penDarkGrey);

        // the arrow is usually drawn inside border of width 2 and is offset by
        // another pixel in both directions when it's pressed - as the border
        // in this case is more narrow as well, we have to adjust rect like
        // this:
        rect->Inflate(-1);
        rect->x++;
        rect->y++;
    }
    else
    {
        DrawShadedRect(dc, rect, m_penLightGrey, m_penBlack);
        DrawShadedRect(dc, rect, m_penHighlight, m_penDarkGrey);
    }
}

void wxWin32Renderer::DrawBorder(wxDC& dc,
                                 wxBorder border,
                                 const wxRect& rectTotal,
                                 int WXUNUSED(flags),
                                 wxRect *rectIn)
{
    int i;

    wxRect rect = rectTotal;

    switch ( border )
    {
        case wxBORDER_SUNKEN:
            for ( i = 0; i < BORDER_THICKNESS / 2; i++ )
            {
                DrawSunkenBorder(dc, &rect);
            }
            break;

        case wxBORDER_STATIC:
            DrawShadedRect(dc, &rect, m_penDarkGrey, m_penHighlight);
            break;

        case wxBORDER_RAISED:
            for ( i = 0; i < BORDER_THICKNESS / 2; i++ )
            {
                DrawRaisedBorder(dc, &rect);
            }
            break;

        case wxBORDER_DOUBLE:
            DrawArrowBorder(dc, &rect);
            DrawRect(dc, &rect, m_penLightGrey);
            break;

        case wxBORDER_SIMPLE:
            for ( i = 0; i < BORDER_THICKNESS / 2; i++ )
            {
                DrawRect(dc, &rect, m_penBlack);
            }
            break;

        default:
            wxFAIL_MSG(_T("unknown border type"));
            // fall through

        case wxBORDER_DEFAULT:
        case wxBORDER_NONE:
            break;
    }

    if ( rectIn )
        *rectIn = rect;
}

wxRect wxWin32Renderer::GetBorderDimensions(wxBorder border) const
{
    wxCoord width;
    switch ( border )
    {
        case wxBORDER_RAISED:
        case wxBORDER_SUNKEN:
            width = BORDER_THICKNESS;
            break;

        case wxBORDER_SIMPLE:
        case wxBORDER_STATIC:
            width = 1;
            break;

        case wxBORDER_DOUBLE:
            width = 3;
            break;

        default:
            wxFAIL_MSG(_T("unknown border type"));
            // fall through

        case wxBORDER_DEFAULT:
        case wxBORDER_NONE:
            width = 0;
            break;
    }

    wxRect rect;
    rect.x =
    rect.y =
    rect.width =
    rect.height = width;

    return rect;
}

bool wxWin32Renderer::AreScrollbarsInsideBorder() const
{
    return TRUE;
}

// ----------------------------------------------------------------------------
// borders
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawTextBorder(wxDC& dc,
                                     wxBorder border,
                                     const wxRect& rect,
                                     int flags,
                                     wxRect *rectIn)
{
    // text controls are not special under windows
    DrawBorder(dc, border, rect, flags, rectIn);
}

void wxWin32Renderer::DrawButtonBorder(wxDC& dc,
                                       const wxRect& rectTotal,
                                       int flags,
                                       wxRect *rectIn)
{
    wxRect rect = rectTotal;

    if ( flags & wxCONTROL_PRESSED )
    {
        // button pressed: draw a double border around it
        DrawRect(dc, &rect, m_penBlack);
        DrawRect(dc, &rect, m_penDarkGrey);
    }
    else
    {
        // button not pressed

        if ( flags & (wxCONTROL_FOCUSED | wxCONTROL_ISDEFAULT) )
        {
            // button either default or focused (or both): add an extra border around it
            DrawRect(dc, &rect, m_penBlack);
        }

        // now draw a normal button
        DrawShadedRect(dc, &rect, m_penHighlight, m_penBlack);
        DrawHalfRect(dc, &rect, m_penDarkGrey);
    }

    if ( rectIn )
    {
        *rectIn = rect;
    }
}

// ----------------------------------------------------------------------------
// lines and frame
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawHorizontalLine(wxDC& dc,
                                         wxCoord y, wxCoord x1, wxCoord x2)
{
    dc.SetPen(m_penDarkGrey);
    dc.DrawLine(x1, y, x2 + 1, y);
    dc.SetPen(m_penHighlight);
    y++;
    dc.DrawLine(x1, y, x2 + 1, y);
}

void wxWin32Renderer::DrawVerticalLine(wxDC& dc,
                                       wxCoord x, wxCoord y1, wxCoord y2)
{
    dc.SetPen(m_penDarkGrey);
    dc.DrawLine(x, y1, x, y2 + 1);
    dc.SetPen(m_penHighlight);
    x++;
    dc.DrawLine(x, y1, x, y2 + 1);
}

void wxWin32Renderer::DrawFrame(wxDC& dc,
                                const wxString& label,
                                const wxRect& rect,
                                int flags,
                                int alignment,
                                int indexAccel)
{
    wxCoord height = 0; // of the label
    wxRect rectFrame = rect;
    if ( !label.empty() )
    {
        // the text should touch the top border of the rect, so the frame
        // itself should be lower
        dc.GetTextExtent(label, NULL, &height);
        rectFrame.y += height / 2;
        rectFrame.height -= height / 2;

        // we have to draw each part of the frame individually as we can't
        // erase the background beyond the label as it might contain some
        // pixmap already, so drawing everything and then overwriting part of
        // the frame with label doesn't work

        // TODO: the +5 and space insertion should be customizable

        wxRect rectText;
        rectText.x = rectFrame.x + 5;
        rectText.y = rect.y;
        rectText.width = rectFrame.width - 7; // +2 border width
        rectText.height = height;

        wxString label2;
        label2 << _T(' ') << label << _T(' ');
        if ( indexAccel != -1 )
        {
            // adjust it as we prepended a space
            indexAccel++;
        }

        wxRect rectLabel;
        DrawLabel(dc, label2, rectText, flags, alignment, indexAccel, &rectLabel);

        StandardDrawFrame(dc, rectFrame, rectLabel);
    }
    else
    {
        // just draw the complete frame
        DrawShadedRect(dc, &rectFrame, m_penDarkGrey, m_penHighlight);
        DrawShadedRect(dc, &rectFrame, m_penHighlight, m_penDarkGrey);
    }
}

// ----------------------------------------------------------------------------
// label
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawFocusRect(wxDC& dc, const wxRect& rect)
{
    // VZ: this doesn't work under Windows, the dotted pen has dots of 3
    //     pixels each while we really need dots here... PS_ALTERNATE might
    //     work, but it is for NT 5 only
#if 0
    DrawRect(dc, &rect, wxPen(*wxBLACK, 0, wxDOT));
#else
    // draw the pixels manually: note that to behave in the same manner as
    // DrawRect(), we must exclude the bottom and right borders from the
    // rectangle
    wxCoord x1 = rect.GetLeft(),
            y1 = rect.GetTop(),
            x2 = rect.GetRight(),
            y2 = rect.GetBottom();

    dc.SetPen(wxPen(*wxBLACK, 0, wxSOLID));

    // this seems to be closer than what Windows does than wxINVERT although
    // I'm still not sure if it's correct
    dc.SetLogicalFunction(wxAND_REVERSE);

    wxCoord z;
    for ( z = x1 + 1; z < x2; z += 2 )
        dc.DrawPoint(z, rect.GetTop());

    wxCoord shift = z == x2 ? 0 : 1;
    for ( z = y1 + shift; z < y2; z += 2 )
        dc.DrawPoint(x2, z);

    shift = z == y2 ? 0 : 1;
    for ( z = x2 - shift; z > x1; z -= 2 )
        dc.DrawPoint(z, y2);

    shift = z == x1 ? 0 : 1;
    for ( z = y2 - shift; z > y1; z -= 2 )
        dc.DrawPoint(x1, z);

    dc.SetLogicalFunction(wxCOPY);
#endif // 0/1
}

void wxWin32Renderer::DrawLabelShadow(wxDC& dc,
                                      const wxString& label,
                                      const wxRect& rect,
                                      int alignment,
                                      int indexAccel)
{
    // draw shadow of the text
    dc.SetTextForeground(m_colHighlight);
    wxRect rectShadow = rect;
    rectShadow.x++;
    rectShadow.y++;
    dc.DrawLabel(label, rectShadow, alignment, indexAccel);

    // make the text grey
    dc.SetTextForeground(m_colDarkGrey);
}

void wxWin32Renderer::DrawLabel(wxDC& dc,
                                const wxString& label,
                                const wxRect& rect,
                                int flags,
                                int alignment,
                                int indexAccel,
                                wxRect *rectBounds)
{
    DoDrawLabel(dc, label, rect, flags, alignment, indexAccel, rectBounds);
}

void wxWin32Renderer::DoDrawLabel(wxDC& dc,
                                  const wxString& label,
                                  const wxRect& rect,
                                  int flags,
                                  int alignment,
                                  int indexAccel,
                                  wxRect *rectBounds,
                                  const wxPoint& focusOffset)
{
    // the underscores are not drawn for focused controls in wxMSW
    if ( flags & wxCONTROL_FOCUSED )
    {
        indexAccel = -1;
    }

    if ( flags & wxCONTROL_DISABLED )
    {
        // the combination of wxCONTROL_SELECTED and wxCONTROL_DISABLED
        // currently only can happen for a menu item and it seems that Windows
        // doesn't draw the shadow in this case, so we don't do it neither
        if ( flags & wxCONTROL_SELECTED )
        {
            // just make the label text greyed out
            dc.SetTextForeground(m_colDarkGrey);
        }
        else // draw normal disabled label
        {
            DrawLabelShadow(dc, label, rect, alignment, indexAccel);
        }
    }

    wxRect rectLabel;
    dc.DrawLabel(label, wxNullBitmap, rect, alignment, indexAccel, &rectLabel);

    if ( flags & wxCONTROL_DISABLED )
    {
        // restore the fg colour
        dc.SetTextForeground(*wxBLACK);
    }

    if ( flags & wxCONTROL_FOCUSED )
    {
        if ( focusOffset.x || focusOffset.y )
        {
            rectLabel.Inflate(focusOffset.x, focusOffset.y);
        }

        DrawFocusRect(dc, rectLabel);
    }

    if ( rectBounds )
        *rectBounds = rectLabel;
}

void wxWin32Renderer::DrawButtonLabel(wxDC& dc,
                                      const wxString& label,
                                      const wxBitmap& image,
                                      const wxRect& rect,
                                      int flags,
                                      int alignment,
                                      int indexAccel,
                                      wxRect *rectBounds)
{
    // the underscores are not drawn for focused controls in wxMSW
    if ( flags & wxCONTROL_PRESSED )
    {
        indexAccel = -1;
    }

    wxRect rectLabel = rect;
    if ( !label.empty() )
    {
        // shift the label if a button is pressed
        if ( flags & wxCONTROL_PRESSED )
        {
            rectLabel.x++;
            rectLabel.y++;
        }

        if ( flags & wxCONTROL_DISABLED )
        {
            DrawLabelShadow(dc, label, rectLabel, alignment, indexAccel);
        }

        // leave enough space for the focus rectangle
        if ( flags & wxCONTROL_FOCUSED )
        {
            rectLabel.Inflate(-2);
        }
    }

    dc.DrawLabel(label, image, rectLabel, alignment, indexAccel, rectBounds);

    if ( !label.empty() && (flags & wxCONTROL_FOCUSED) )
    {
        if ( flags & wxCONTROL_PRESSED )
        {
            // the focus rectangle is never pressed, so undo the shift done
            // above
            rectLabel.x--;
            rectLabel.y--;
            rectLabel.width--;
            rectLabel.height--;
        }

        DrawFocusRect(dc, rectLabel);
    }
}

// ----------------------------------------------------------------------------
// (check)listbox items
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawItem(wxDC& dc,
                               const wxString& label,
                               const wxRect& rect,
                               int flags)
{
    wxDCTextColourChanger colChanger(dc);

    if ( flags & wxCONTROL_SELECTED )
    {
        colChanger.Set(wxSCHEME_COLOUR(m_scheme, HIGHLIGHT_TEXT));

        wxColour colBg = wxSCHEME_COLOUR(m_scheme, HIGHLIGHT);
        dc.SetBrush(wxBrush(colBg, wxSOLID));
        dc.SetPen(wxPen(colBg, 0, wxSOLID));
        dc.DrawRectangle(rect);
    }

    wxRect rectText = rect;
    rectText.x += 2;
    rectText.width -= 2;
    dc.DrawLabel(label, wxNullBitmap, rectText);

    if ( flags & wxCONTROL_FOCUSED )
    {
        DrawFocusRect(dc, rect);
    }
}

void wxWin32Renderer::DrawCheckItem(wxDC& dc,
                                    const wxString& label,
                                    const wxBitmap& bitmap,
                                    const wxRect& rect,
                                    int flags)
{
    wxBitmap bmp;
    if ( bitmap.Ok() )
    {
        bmp = bitmap;
    }
    else // use default bitmap
    {
        bmp = wxBitmap(flags & wxCONTROL_CHECKED ? checked_item_xpm
                                                 : unchecked_item_xpm);
    }

    dc.DrawBitmap(bmp, rect.x, rect.y + (rect.height - bmp.GetHeight()) / 2 - 1,
                  TRUE /* use mask */);

    wxRect rectLabel = rect;
    int bmpWidth = bmp.GetWidth();
    rectLabel.x += bmpWidth;
    rectLabel.width -= bmpWidth;

    DrawItem(dc, label, rectLabel, flags);
}

// ----------------------------------------------------------------------------
// check/radio buttons
// ----------------------------------------------------------------------------

wxBitmap wxWin32Renderer::GetIndicator(IndicatorType indType, int flags)
{
    IndicatorState indState;
    if ( flags & wxCONTROL_SELECTED )
        indState = flags & wxCONTROL_DISABLED ? IndicatorState_SelectedDisabled
                                              : IndicatorState_Selected;
    else if ( flags & wxCONTROL_DISABLED )
        indState = IndicatorState_Disabled;
    else if ( flags & wxCONTROL_PRESSED )
        indState = IndicatorState_Pressed;
    else
        indState = IndicatorState_Normal;

    IndicatorStatus indStatus = flags & wxCONTROL_CHECKED
                                    ? IndicatorStatus_Checked
                                    : IndicatorStatus_Unchecked;

    const char **xpm = bmpIndicators[indType][indState][indStatus];
    return xpm ? wxBitmap(xpm) : wxNullBitmap;
}

void wxWin32Renderer::DrawCheckOrRadioButton(wxDC& dc,
                                             const wxString& label,
                                             const wxBitmap& bitmap,
                                             const wxRect& rect,
                                             int flags,
                                             wxAlignment align,
                                             int indexAccel,
                                             wxCoord focusOffsetY)
{
    // calculate the position of the bitmap and of the label
    wxCoord heightBmp = bitmap.GetHeight();
    wxCoord xBmp,
            yBmp = rect.y + (rect.height - heightBmp) / 2;

    wxRect rectLabel;
    dc.GetMultiLineTextExtent(label, NULL, &rectLabel.height);
    rectLabel.y = rect.y + (rect.height - rectLabel.height) / 2;

    // align label vertically with the bitmap - looks nicer like this
    rectLabel.y -= (rectLabel.height - heightBmp) % 2;

    // calc horz position
    if ( align == wxALIGN_RIGHT )
    {
        xBmp = rect.GetRight() - bitmap.GetWidth();
        rectLabel.x = rect.x + 3;
        rectLabel.SetRight(xBmp);
    }
    else // normal (checkbox to the left of the text) case
    {
        xBmp = rect.x;
        rectLabel.x = xBmp + bitmap.GetWidth() + 5;
        rectLabel.SetRight(rect.GetRight());
    }

    dc.DrawBitmap(bitmap, xBmp, yBmp, TRUE /* use mask */);

    DoDrawLabel(
                dc, label, rectLabel,
                flags,
                wxALIGN_LEFT | wxALIGN_TOP,
                indexAccel,
                NULL,         // we don't need bounding rect
                // use custom vert focus rect offset
                wxPoint(FOCUS_RECT_OFFSET_X, focusOffsetY)
               );
}

void wxWin32Renderer::DrawRadioButton(wxDC& dc,
                                      const wxString& label,
                                      const wxBitmap& bitmap,
                                      const wxRect& rect,
                                      int flags,
                                      wxAlignment align,
                                      int indexAccel)
{
    DrawCheckOrRadioButton(dc, label,
                           bitmap.Ok() ? bitmap : GetRadioBitmap(flags),
                           rect, flags, align, indexAccel,
                           FOCUS_RECT_OFFSET_Y); // default focus rect offset
}

void wxWin32Renderer::DrawCheckButton(wxDC& dc,
                                      const wxString& label,
                                      const wxBitmap& bitmap,
                                      const wxRect& rect,
                                      int flags,
                                      wxAlignment align,
                                      int indexAccel)
{
    DrawCheckOrRadioButton(dc, label,
                           bitmap.Ok() ? bitmap : GetCheckBitmap(flags),
                           rect, flags, align, indexAccel,
                           0); // no focus rect offset for checkboxes
}

// ----------------------------------------------------------------------------
// text control
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawTextLine(wxDC& dc,
                                   const wxString& text,
                                   const wxRect& rect,
                                   int selStart,
                                   int selEnd,
                                   int flags)
{
    // nothing special to do here
    StandardDrawTextLine(dc, text, rect, selStart, selEnd, flags);
}

void wxWin32Renderer::DrawLineWrapMark(wxDC& dc, const wxRect& rect)
{
    // we don't draw them
}

// ----------------------------------------------------------------------------
// notebook
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawTab(wxDC& dc,
                              const wxRect& rectOrig,
                              wxDirection dir,
                              const wxString& label,
                              const wxBitmap& bitmap,
                              int flags,
                              int indexAccel)
{
    wxRect rect = rectOrig;

    // the current tab is drawn indented (to the top for default case) and
    // bigger than the other ones
    const wxSize indent = GetTabIndent();
    if ( flags & wxCONTROL_SELECTED )
    {
        switch ( dir )
        {
            default:
                wxFAIL_MSG(_T("invaild notebook tab orientation"));
                // fall through

            case wxTOP:
                rect.Inflate(indent.x, 0);
                rect.y -= indent.y;
                rect.height += indent.y;
                break;

            case wxBOTTOM:
                rect.Inflate(indent.x, 0);
                rect.height += indent.y;
                break;

            case wxLEFT:
            case wxRIGHT:
                wxFAIL_MSG(_T("TODO"));
                break;
        }
    }

    // draw the text, image and the focus around them (if necessary)
    wxRect rectLabel = rect;
    rectLabel.Deflate(1, 1);
    DrawButtonLabel(dc, label, bitmap, rectLabel,
                    flags, wxALIGN_CENTRE, indexAccel);

    // now draw the tab border itself (maybe use DrawRoundedRectangle()?)
    static const wxCoord CUTOFF = 2; // radius of the rounded corner
    wxCoord x = rect.x,
            y = rect.y,
            x2 = rect.GetRight(),
            y2 = rect.GetBottom();

    // FIXME: all this code will break if the tab indent or the border width,
    //        it is tied to the fact that both of them are equal to 2
    switch ( dir )
    {
        default:
        case wxTOP:
            dc.SetPen(m_penHighlight);
            dc.DrawLine(x, y2, x, y + CUTOFF);
            dc.DrawLine(x, y + CUTOFF, x + CUTOFF, y);
            dc.DrawLine(x + CUTOFF, y, x2 - CUTOFF + 1, y);

            dc.SetPen(m_penBlack);
            dc.DrawLine(x2, y2, x2, y + CUTOFF);
            dc.DrawLine(x2, y + CUTOFF, x2 - CUTOFF, y);

            dc.SetPen(m_penDarkGrey);
            dc.DrawLine(x2 - 1, y2, x2 - 1, y + CUTOFF - 1);

            if ( flags & wxCONTROL_SELECTED )
            {
                dc.SetPen(m_penLightGrey);

                // overwrite the part of the border below this tab
                dc.DrawLine(x + 1, y2 + 1, x2 - 1, y2 + 1);

                // and the shadow of the tab to the left of us
                dc.DrawLine(x + 1, y + CUTOFF + 1, x + 1, y2 + 1);
            }
            break;

        case wxBOTTOM:
            dc.SetPen(m_penHighlight);
            // we need to continue one pixel further to overwrite the corner of
            // the border for the selected tab
            dc.DrawLine(x, y - (flags & wxCONTROL_SELECTED ? 1 : 0),
                        x, y2 - CUTOFF);
            dc.DrawLine(x, y2 - CUTOFF, x + CUTOFF, y2);

            dc.SetPen(m_penBlack);
            dc.DrawLine(x + CUTOFF, y2, x2 - CUTOFF + 1, y2);
            dc.DrawLine(x2, y, x2, y2 - CUTOFF);
            dc.DrawLine(x2, y2 - CUTOFF, x2 - CUTOFF, y2);

            dc.SetPen(m_penDarkGrey);
            dc.DrawLine(x + CUTOFF, y2 - 1, x2 - CUTOFF + 1, y2 - 1);
            dc.DrawLine(x2 - 1, y, x2 - 1, y2 - CUTOFF + 1);

            if ( flags & wxCONTROL_SELECTED )
            {
                dc.SetPen(m_penLightGrey);

                // overwrite the part of the (double!) border above this tab
                dc.DrawLine(x + 1, y - 1, x2 - 1, y - 1);
                dc.DrawLine(x + 1, y - 2, x2 - 1, y - 2);

                // and the shadow of the tab to the left of us
                dc.DrawLine(x + 1, y2 - CUTOFF, x + 1, y - 1);
            }
            break;

        case wxLEFT:
        case wxRIGHT:
            wxFAIL_MSG(_T("TODO"));
    }
}

// ----------------------------------------------------------------------------
// slider
// ----------------------------------------------------------------------------

wxSize wxWin32Renderer::GetSliderThumbSize(const wxRect& rect,
                                           wxOrientation orient) const
{
    wxSize size;

    wxRect rectShaft = GetSliderShaftRect(rect, orient);
    if ( orient == wxHORIZONTAL )
    {
        size.y = rect.height - 6;
        size.x = wxMin(size.y / 2, rectShaft.width);
    }
    else // vertical
    {
        size.x = rect.width - 6;
        size.y = wxMin(size.x / 2, rectShaft.height);
    }

    return size;
}

wxRect wxWin32Renderer::GetSliderShaftRect(const wxRect& rectOrig,
                                           wxOrientation orient) const
{
    static const wxCoord SLIDER_MARGIN = 6;

    wxRect rect = rectOrig;

    if ( orient == wxHORIZONTAL )
    {
        // make the rect of minimal width and centre it
        rect.height = 2*BORDER_THICKNESS;
        rect.y = rectOrig.y + (rectOrig.height - rect.height) / 2;
        if ( rect.y < 0 )
            rect.y = 0;

        // leave margins on the sides
        rect.Deflate(SLIDER_MARGIN, 0);
    }
    else // vertical
    {
        // same as above but in other direction
        rect.width = 2*BORDER_THICKNESS;
        rect.x = rectOrig.x + (rectOrig.width - rect.width) / 2;
        if ( rect.x < 0 )
            rect.x = 0;

        rect.Deflate(0, SLIDER_MARGIN);
    }

    return rect;
}

void wxWin32Renderer::DrawSliderShaft(wxDC& dc,
                                      const wxRect& rectOrig,
                                      wxOrientation orient,
                                      int flags,
                                      wxRect *rectShaft)
{
    if ( flags & wxCONTROL_FOCUSED )
    {
        DrawFocusRect(dc, rectOrig);
    }

    wxRect rect = GetSliderShaftRect(rectOrig, orient);

    if ( rectShaft )
        *rectShaft = rect;

    DrawSunkenBorder(dc, &rect);
}

void wxWin32Renderer::DrawSliderThumb(wxDC& dc,
                                      const wxRect& rect,
                                      wxOrientation orient,
                                      int flags)
{
    /*
       we are drawing a shape of this form

       HHHHHHB <--- y
       H    DB
       H    DB
       H    DB   where H is hightlight colour
       H    DB         D    dark grey
       H    DB         B    black
       H    DB
       H    DB <--- y3
        H  DB
         HDB
          B    <--- y2

       ^  ^  ^
       |  |  |
       x x3  x2

       The interior of this shape is filled with the hatched brush if the thumb
       is pressed.
    */

    DrawBackground(dc, wxNullColour, rect, flags);

    bool transpose = orient == wxVERTICAL;

    wxCoord x, y, x2, y2;
    if ( transpose )
    {
        x = rect.y;
        y = rect.x;
        x2 = rect.GetBottom();
        y2 = rect.GetRight();
    }
    else
    {
        x = rect.x;
        y = rect.y;
        x2 = rect.GetRight();
        y2 = rect.GetBottom();
    }

    // the size of the pointed part of the thumb
    wxCoord sizeArrow = (transpose ? rect.height : rect.width) / 2;

    wxCoord x3 = x + sizeArrow,
            y3 = y2 - sizeArrow;

    dc.SetPen(m_penHighlight);
    DrawLine(dc, x, y, x2, y, transpose);
    DrawLine(dc, x, y + 1, x, y2 - sizeArrow, transpose);
    DrawLine(dc, x, y3, x3, y2, transpose);

    dc.SetPen(m_penBlack);
    DrawLine(dc, x3, y2, x2, y3, transpose);
    DrawLine(dc, x2, y3, x2, y - 1, transpose);

    dc.SetPen(m_penDarkGrey);
    DrawLine(dc, x3, y2 - 1, x2 - 1, y3, transpose);
    DrawLine(dc, x2 - 1, y3, x2 - 1, y, transpose);

    if ( flags & wxCONTROL_PRESSED )
    {
        // TODO: MSW fills the entire area inside, not just the rect
        wxRect rectInt = rect;
        if ( transpose )
            rectInt.SetRight(y3);
        else
            rectInt.SetBottom(y3);
        rectInt.Deflate(2);

        static const char *stipple_xpm[] = {
            /* columns rows colors chars-per-pixel */
            "2 2 2 1",
            "  c None",
            "w c white",
            /* pixels */
            "w ",
            " w",
        };
        dc.SetBrush(wxBrush(stipple_xpm));

        dc.SetTextForeground(wxSCHEME_COLOUR(m_scheme, SHADOW_HIGHLIGHT));
        dc.SetTextBackground(wxSCHEME_COLOUR(m_scheme, CONTROL));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(rectInt);
    }
}

void wxWin32Renderer::DrawSliderTicks(wxDC& dc,
                                      const wxRect& rect,
                                      const wxSize& sizeThumb,
                                      wxOrientation orient,
                                      int start,
                                      int end,
                                      int step,
                                      int flags)
{
    if ( end == start )
    {
        // empty slider?
        return;
    }

    // the variable names correspond to horizontal case, but they can be used
    // for both orientations
    wxCoord x1, x2, y1, y2, len, widthThumb;
    if ( orient == wxHORIZONTAL )
    {
        x1 = rect.GetLeft();
        x2 = rect.GetRight();

        // draw from bottom to top to leave one pixel space between the ticks
        // and the slider as Windows do
        y1 = rect.GetBottom();
        y2 = rect.GetTop();

        len = rect.width;

        widthThumb = sizeThumb.x;
    }
    else // vertical
    {
        x1 = rect.GetTop();
        x2 = rect.GetBottom();

        y1 = rect.GetRight();
        y2 = rect.GetLeft();

        len = rect.height;

        widthThumb = sizeThumb.y;
    }

    // the first tick should be positioned in such way that a thumb drawn in
    // the first position points down directly to it
    x1 += widthThumb / 2;
    x2 -= widthThumb / 2;

    // this also means that we have slightly less space for the ticks in
    // between the first and the last
    len -= widthThumb;

    dc.SetPen(m_penBlack);

    int range = end - start;
    for ( int n = 0; n < range; n += step )
    {
        wxCoord x = x1 + (len*n) / range;

        DrawLine(dc, x, y1, x, y2, orient == wxVERTICAL);
    }

    // always draw the line at the end position
    DrawLine(dc, x2, y1, x2, y2, orient == wxVERTICAL);
}

// ----------------------------------------------------------------------------
// menu and menubar
// ----------------------------------------------------------------------------

#if wxUSE_MENUS

// wxWin32MenuGeometryInfo: the wxMenuGeometryInfo used by wxWin32Renderer
class WXDLLEXPORT wxWin32MenuGeometryInfo : public wxMenuGeometryInfo
{
public:
    virtual wxSize GetSize() const { return m_size; }

    wxCoord GetLabelOffset() const { return m_ofsLabel; }
    wxCoord GetAccelOffset() const { return m_ofsAccel; }

    wxCoord GetItemHeight() const { return m_heightItem; }

private:
    // the total size of the menu
    wxSize m_size;

    // the offset of the start of the menu item label
    wxCoord m_ofsLabel;

    // the offset of the start of the accel label
    wxCoord m_ofsAccel;

    // the height of a normal (not separator) item
    wxCoord m_heightItem;

    friend wxMenuGeometryInfo *wxWin32Renderer::
                GetMenuGeometry(wxWindow *, const wxMenu&) const;
};

#endif // wxUSE_MENUS

// FIXME: all constants are hardcoded but shouldn't be
static const wxCoord MENU_LEFT_MARGIN = 9;
static const wxCoord MENU_RIGHT_MARGIN = 18;
static const wxCoord MENU_VERT_MARGIN = 3;

// the margin around bitmap/check marks (on each side)
static const wxCoord MENU_BMP_MARGIN = 2;

// the margin between the labels and accel strings
static const wxCoord MENU_ACCEL_MARGIN = 8;

// the separator height in pixels: in fact, strangely enough, the real height
// is 2 but Windows adds one extra pixel in the bottom margin, so take it into
// account here
static const wxCoord MENU_SEPARATOR_HEIGHT = 3;

// the size of the standard checkmark bitmap
static const wxCoord MENU_CHECK_SIZE = 9;

// we can't implement these methods without wxMenuGeometryInfo implementation
// which we don't have if !wxUSE_MENUS
#if wxUSE_MENUS

void wxWin32Renderer::DrawMenuBarItem(wxDC& dc,
                                      const wxRect& rectOrig,
                                      const wxString& label,
                                      int flags,
                                      int indexAccel)
{
    wxRect rect = rectOrig;
    rect.height--;

    wxDCTextColourChanger colChanger(dc);

    if ( flags & wxCONTROL_SELECTED )
    {
        colChanger.Set(wxSCHEME_COLOUR(m_scheme, HIGHLIGHT_TEXT));

        wxColour colBg = wxSCHEME_COLOUR(m_scheme, HIGHLIGHT);
        dc.SetBrush(wxBrush(colBg, wxSOLID));
        dc.SetPen(wxPen(colBg, 0, wxSOLID));
        dc.DrawRectangle(rect);
    }

    // don't draw the focus rect around menu bar items
    DrawLabel(dc, label, rect, flags & ~wxCONTROL_FOCUSED,
              wxALIGN_CENTRE, indexAccel);
}

void wxWin32Renderer::DrawMenuItem(wxDC& dc,
                                   wxCoord y,
                                   const wxMenuGeometryInfo& gi,
                                   const wxString& label,
                                   const wxString& accel,
                                   const wxBitmap& bitmap,
                                   int flags,
                                   int indexAccel)
{
    const wxWin32MenuGeometryInfo& geometryInfo =
        (const wxWin32MenuGeometryInfo&)gi;

    wxRect rect;
    rect.x = 0;
    rect.y = y;
    rect.width = geometryInfo.GetSize().x;
    rect.height = geometryInfo.GetItemHeight();

    // draw the selected item specially
    wxDCTextColourChanger colChanger(dc);
    if ( flags & wxCONTROL_SELECTED )
    {
        colChanger.Set(wxSCHEME_COLOUR(m_scheme, HIGHLIGHT_TEXT));

        wxColour colBg = wxSCHEME_COLOUR(m_scheme, HIGHLIGHT);
        dc.SetBrush(wxBrush(colBg, wxSOLID));
        dc.SetPen(wxPen(colBg, 0, wxSOLID));
        dc.DrawRectangle(rect);
    }

    // draw the bitmap: use the bitmap provided or the standard checkmark for
    // the checkable items
    wxBitmap bmp = bitmap;
    if ( !bmp.Ok() && (flags & wxCONTROL_CHECKED) )
    {
        bmp = GetIndicator(IndicatorType_Menu, flags);
    }

    if ( bmp.Ok() )
    {
        rect.SetRight(geometryInfo.GetLabelOffset());
        wxControlRenderer::DrawBitmap(dc, bmp, rect);
    }

    // draw the label
    rect.x = geometryInfo.GetLabelOffset();
    rect.SetRight(geometryInfo.GetAccelOffset());

    DrawLabel(dc, label, rect, flags, wxALIGN_CENTRE_VERTICAL, indexAccel);

    // draw the accel string
    rect.x = geometryInfo.GetAccelOffset();
    rect.SetRight(geometryInfo.GetSize().x);

    // NB: no accel index here
    DrawLabel(dc, accel, rect, flags, wxALIGN_CENTRE_VERTICAL);

    // draw the submenu indicator
    if ( flags & wxCONTROL_ISSUBMENU )
    {
        rect.x = geometryInfo.GetSize().x - MENU_RIGHT_MARGIN;
        rect.width = MENU_RIGHT_MARGIN;

        wxArrowStyle arrowStyle;
        if ( flags & wxCONTROL_DISABLED )
            arrowStyle = flags & wxCONTROL_SELECTED ? Arrow_InversedDisabled
                                                    : Arrow_Disabled;
        else if ( flags & wxCONTROL_SELECTED )
            arrowStyle = Arrow_Inversed;
        else
            arrowStyle = Arrow_Normal;

        DrawArrow(dc, rect, Arrow_Right, arrowStyle);
    }
}

void wxWin32Renderer::DrawMenuSeparator(wxDC& dc,
                                        wxCoord y,
                                        const wxMenuGeometryInfo& geomInfo)
{
    DrawHorizontalLine(dc, y + MENU_VERT_MARGIN, 0, geomInfo.GetSize().x);
}

wxSize wxWin32Renderer::GetMenuBarItemSize(const wxSize& sizeText) const
{
    wxSize size = sizeText;

    // FIXME: menubar height is configurable under Windows
    size.x += 12;
    size.y += 6;

    return size;
}

wxMenuGeometryInfo *wxWin32Renderer::GetMenuGeometry(wxWindow *win,
                                                     const wxMenu& menu) const
{
    // prepare the dc: for now we draw all the items with the system font
    wxClientDC dc(win);
    dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));

    // the height of a normal item
    wxCoord heightText = dc.GetCharHeight();

    // the total height
    wxCoord height = 0;

    // the max length of label and accel strings: the menu width is the sum of
    // them, even if they're for different items (as the accels should be
    // aligned)
    //
    // the max length of the bitmap is never 0 as Windows always leaves enough
    // space for a check mark indicator
    wxCoord widthLabelMax = 0,
            widthAccelMax = 0,
            widthBmpMax = MENU_LEFT_MARGIN;

    for ( wxMenuItemList::Node *node = menu.GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        // height of this item
        wxCoord h;

        wxMenuItem *item = node->GetData();
        if ( item->IsSeparator() )
        {
            h = MENU_SEPARATOR_HEIGHT;
        }
        else // not separator
        {
            h = heightText;

            wxCoord widthLabel;
            dc.GetTextExtent(item->GetLabel(), &widthLabel, NULL);
            if ( widthLabel > widthLabelMax )
            {
                widthLabelMax = widthLabel;
            }

            wxCoord widthAccel;
            dc.GetTextExtent(item->GetAccelString(), &widthAccel, NULL);
            if ( widthAccel > widthAccelMax )
            {
                widthAccelMax = widthAccel;
            }

            const wxBitmap& bmp = item->GetBitmap();
            if ( bmp.Ok() )
            {
                wxCoord widthBmp = bmp.GetWidth();
                if ( widthBmp > widthBmpMax )
                    widthBmpMax = widthBmp;
            }
            //else if ( item->IsCheckable() ): no need to check for this as
            // MENU_LEFT_MARGIN is big enough to show the check mark
        }

        h += 2*MENU_VERT_MARGIN;

        // remember the item position and height
        item->SetGeometry(height, h);

        height += h;
    }

    // bundle the metrics into a struct and return it
    wxWin32MenuGeometryInfo *gi = new wxWin32MenuGeometryInfo;

    gi->m_ofsLabel = widthBmpMax + 2*MENU_BMP_MARGIN;
    gi->m_ofsAccel = gi->m_ofsLabel + widthLabelMax;
    if ( widthAccelMax > 0 )
    {
        // if we actually have any accesl, add a margin
        gi->m_ofsAccel += MENU_ACCEL_MARGIN;
    }

    gi->m_heightItem = heightText + 2*MENU_VERT_MARGIN;

    gi->m_size.x = gi->m_ofsAccel + widthAccelMax + MENU_RIGHT_MARGIN;
    gi->m_size.y = height;

    return gi;
}

#else // !wxUSE_MENUS

/*
void wxWin32Renderer::DrawMenuBarItem(wxDC& WXUNUSED(dc),
                                      const wxRect& WXUNUSED(rectOrig),
                                      const wxString& WXUNUSED(label),
                                      int WXUNUSED(flags),
                                      int WXUNUSED(indexAccel))
{
}

void wxWin32Renderer::DrawMenuItem(wxDC& WXUNUSED(dc),
                                   wxCoord WXUNUSED(y),
                                   const wxMenuGeometryInfo& WXUNUSED(gi),
                                   const wxString& WXUNUSED(label),
                                   const wxString& WXUNUSED(accel),
                                   const wxBitmap& WXUNUSED(bitmap),
                                   int WXUNUSED(flags),
                                   int WXUNUSED(indexAccel))
{
}

void wxWin32Renderer::DrawMenuSeparator(wxDC& WXUNUSED(dc),
                                        wxCoord WXUNUSED(y),
                                        const wxMenuGeometryInfo& WXUNUSED(gi))
{
}

wxSize wxWin32Renderer::GetMenuBarItemSize(const wxSize& size) const
{
    return size;
}

wxMenuGeometryInfo *
wxWin32Renderer::GetMenuGeometry(wxWindow *WXUNUSED(win),
                                 const wxMenu& WXUNUSED(menu)) const
{
    return NULL;
}
*/

#endif // wxUSE_MENUS/!wxUSE_MENUS

// ----------------------------------------------------------------------------
// combobox
// ----------------------------------------------------------------------------

void wxWin32Renderer::GetComboBitmaps(wxBitmap *bmpNormal,
                                      wxBitmap *bmpPressed,
                                      wxBitmap *bmpDisabled)
{
    static const wxCoord widthCombo = 16;
    static const wxCoord heightCombo = 17;

    wxMemoryDC dcMem;

    if ( bmpNormal )
    {
        bmpNormal->Create(widthCombo, heightCombo);
        dcMem.SelectObject(*bmpNormal);
        DrawArrowButton(dcMem, wxRect(0, 0, widthCombo, heightCombo),
                        Arrow_Down, Arrow_Normal);
    }

    if ( bmpPressed )
    {
        bmpPressed->Create(widthCombo, heightCombo);
        dcMem.SelectObject(*bmpPressed);
        DrawArrowButton(dcMem, wxRect(0, 0, widthCombo, heightCombo),
                        Arrow_Down, Arrow_Pressed);
    }

    if ( bmpDisabled )
    {
        bmpDisabled->Create(widthCombo, heightCombo);
        dcMem.SelectObject(*bmpDisabled);
        DrawArrowButton(dcMem, wxRect(0, 0, widthCombo, heightCombo),
                        Arrow_Down, Arrow_Disabled);
    }
}

// ----------------------------------------------------------------------------
// background
// ----------------------------------------------------------------------------

void wxWin32Renderer::DoDrawBackground(wxDC& dc,
                                       const wxColour& col,
                                       const wxRect& rect)
{
    wxBrush brush(col, wxSOLID);
    dc.SetBrush(brush);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(rect);
}

void wxWin32Renderer::DrawBackground(wxDC& dc,
                                     const wxColour& col,
                                     const wxRect& rect,
                                     int flags)
{
    // just fill it with the given or default bg colour
    wxColour colBg = col.Ok() ? col : wxSCHEME_COLOUR(m_scheme, CONTROL);
    DoDrawBackground(dc, colBg, rect);
}

// ----------------------------------------------------------------------------
// scrollbar
// ----------------------------------------------------------------------------

void wxWin32Renderer::DrawArrow(wxDC& dc,
                                wxDirection dir,
                                const wxRect& rect,
                                int flags)
{
    // get the bitmap for this arrow
    wxArrowDirection arrowDir;
    switch ( dir )
    {
        case wxLEFT:    arrowDir = Arrow_Left; break;
        case wxRIGHT:   arrowDir = Arrow_Right; break;
        case wxUP:      arrowDir = Arrow_Up; break;
        case wxDOWN:    arrowDir = Arrow_Down; break;

        default:
            wxFAIL_MSG(_T("unknown arrow direction"));
            return;
    }

    wxArrowStyle arrowStyle;
    if ( flags & wxCONTROL_PRESSED )
    {
        // can't be pressed and disabled
        arrowStyle = Arrow_Pressed;
    }
    else
    {
        arrowStyle = flags & wxCONTROL_DISABLED ? Arrow_Disabled : Arrow_Normal;
    }

    DrawArrowButton(dc, rect, arrowDir, arrowStyle);
}

void wxWin32Renderer::DrawArrow(wxDC& dc,
                                const wxRect& rect,
                                wxArrowDirection arrowDir,
                                wxArrowStyle arrowStyle)
{
    const wxBitmap& bmp = m_bmpArrows[arrowStyle][arrowDir];

    // under Windows the arrows always have the same size so just centre it in
    // the provided rectangle
    wxCoord x = rect.x + (rect.width - bmp.GetWidth()) / 2,
            y = rect.y + (rect.height - bmp.GetHeight()) / 2;

    // Windows does it like this...
    if ( arrowDir == Arrow_Left )
        x--;

    // draw it
    dc.DrawBitmap(bmp, x, y, TRUE /* use mask */);
}

void wxWin32Renderer::DrawArrowButton(wxDC& dc,
                                      const wxRect& rectAll,
                                      wxArrowDirection arrowDir,
                                      wxArrowStyle arrowStyle)
{
    wxRect rect = rectAll;
    DoDrawBackground(dc, wxSCHEME_COLOUR(m_scheme, CONTROL), rect);
    DrawArrowBorder(dc, &rect, arrowStyle == Arrow_Pressed);
    DrawArrow(dc, rect, arrowDir, arrowStyle);
}

void wxWin32Renderer::DrawScrollbarThumb(wxDC& dc,
                                         wxOrientation orient,
                                         const wxRect& rect,
                                         int flags)
{
    // we don't use the flags, the thumb never changes appearance
    wxRect rectThumb = rect;
    DrawArrowBorder(dc, &rectThumb);
    DrawBackground(dc, wxNullColour, rectThumb);
}

void wxWin32Renderer::DrawScrollbarShaft(wxDC& dc,
                                         wxOrientation orient,
                                         const wxRect& rectBar,
                                         int flags)
{
    wxColourScheme::StdColour col = flags & wxCONTROL_PRESSED
                                    ? wxColourScheme::SCROLLBAR_PRESSED
                                    : wxColourScheme::SCROLLBAR;
    DoDrawBackground(dc, m_scheme->Get(col), rectBar);
}

void wxWin32Renderer::DrawScrollCorner(wxDC& dc, const wxRect& rect)
{
    DoDrawBackground(dc, wxSCHEME_COLOUR(m_scheme, CONTROL), rect);
}

wxRect wxWin32Renderer::GetScrollbarRect(const wxScrollBar *scrollbar,
                                         wxScrollBar::Element elem,
                                         int thumbPos) const
{
    return StandardGetScrollbarRect(scrollbar, elem,
                                    thumbPos, m_sizeScrollbarArrow);
}

wxCoord wxWin32Renderer::GetScrollbarSize(const wxScrollBar *scrollbar)
{
    return StandardScrollBarSize(scrollbar, m_sizeScrollbarArrow);
}

wxHitTest wxWin32Renderer::HitTestScrollbar(const wxScrollBar *scrollbar,
                                            const wxPoint& pt) const
{
    return StandardHitTestScrollbar(scrollbar, pt, m_sizeScrollbarArrow);
}

wxCoord wxWin32Renderer::ScrollbarToPixel(const wxScrollBar *scrollbar,
                                          int thumbPos)
{
    return StandardScrollbarToPixel(scrollbar, thumbPos, m_sizeScrollbarArrow);
}

int wxWin32Renderer::PixelToScrollbar(const wxScrollBar *scrollbar,
                                      wxCoord coord)
{
    return StandardPixelToScrollbar(scrollbar, coord, m_sizeScrollbarArrow);
}

// ----------------------------------------------------------------------------
// text control geometry
// ----------------------------------------------------------------------------

static inline int GetTextBorderWidth()
{
    return 1;
}

wxRect wxWin32Renderer::GetTextTotalArea(const wxTextCtrl *text,
                                         const wxRect& rect)
{
    wxRect rectTotal = rect;

    wxCoord widthBorder = GetTextBorderWidth();
    rectTotal.Inflate(widthBorder);

    // this is what Windows does
    rectTotal.height++;

    return rectTotal;
}

wxRect wxWin32Renderer::GetTextClientArea(const wxTextCtrl *text,
                                          const wxRect& rect,
                                          wxCoord *extraSpaceBeyond)
{
    wxRect rectText = rect;

    // undo GetTextTotalArea()
    if ( rectText.height > 0 )
        rectText.height--;

    wxCoord widthBorder = GetTextBorderWidth();
    rectText.Inflate(-widthBorder);

    if ( extraSpaceBeyond )
        *extraSpaceBeyond = 0;

    return rectText;
}

// ----------------------------------------------------------------------------
// size adjustments
// ----------------------------------------------------------------------------

void wxWin32Renderer::AdjustSize(wxSize *size, const wxWindow *window)
{
#if wxUSE_SCROLLBAR
    if ( wxDynamicCast(window, wxScrollBar) )
    {
        // we only set the width of vert scrollbars and height of the
        // horizontal ones
        if ( window->GetWindowStyle() & wxSB_HORIZONTAL )
            size->y = m_sizeScrollbarArrow.y;
        else
            size->x = m_sizeScrollbarArrow.x;

        // skip border width adjustments, they don't make sense for us
        return;
    }
#endif // wxUSE_SCROLLBAR/!wxUSE_SCROLLBAR

#if wxUSE_BUTTON
    if ( wxDynamicCast(window, wxButton) )
    {
        // TODO
        size->x += 3*window->GetCharWidth();
#if 0 // do allow creating small buttons if wanted
        wxSize sizeDef = wxButton::GetDefaultSize();
        if ( size->x < sizeDef.x )
            size->x = sizeDef.x;
#endif // 0

        wxCoord heightBtn = (11*(window->GetCharHeight() + 8))/10;
        if ( size->y < heightBtn - 8 )
            size->y = heightBtn;
        else
            size->y += 9;

        // no border width adjustments for buttons
        return;
    }
#endif // wxUSE_BUTTON

    // take into account the border width
    wxRect rectBorder = GetBorderDimensions(window->GetBorder());
    size->x += rectBorder.x + rectBorder.width;
    size->y += rectBorder.y + rectBorder.height;
}

// ============================================================================
// wxInputHandler
// ============================================================================

// ----------------------------------------------------------------------------
// wxWin32InputHandler
// ----------------------------------------------------------------------------

wxWin32InputHandler::wxWin32InputHandler(wxWin32Renderer *renderer)
{
    m_renderer = renderer;
}

bool wxWin32InputHandler::HandleKey(wxControl *control,
                                    const wxKeyEvent& event,
                                    bool pressed)
{
    return FALSE;
}

bool wxWin32InputHandler::HandleMouse(wxControl *control,
                                      const wxMouseEvent& event)
{
    return FALSE;
}

// ----------------------------------------------------------------------------
// wxWin32ScrollBarInputHandler
// ----------------------------------------------------------------------------

wxWin32ScrollBarInputHandler::
wxWin32ScrollBarInputHandler(wxWin32Renderer *renderer,
                             wxInputHandler *handler)
        : wxStdScrollBarInputHandler(renderer, handler)
{
    m_scrollPaused = FALSE;
    m_interval = 0;
}

bool wxWin32ScrollBarInputHandler::OnScrollTimer(wxScrollBar *scrollbar,
                                                 const wxControlAction& action)
{
    // stop if went beyond the position of the original click (this can only
    // happen when we scroll by pages)
    bool stop = FALSE;
    if ( action == wxACTION_SCROLL_PAGE_DOWN )
    {
        stop = m_renderer->HitTestScrollbar(scrollbar, m_ptStartScrolling)
                != wxHT_SCROLLBAR_BAR_2;
    }
    else if ( action == wxACTION_SCROLL_PAGE_UP )
    {
        stop = m_renderer->HitTestScrollbar(scrollbar, m_ptStartScrolling)
                != wxHT_SCROLLBAR_BAR_1;
    }

    if ( stop )
    {
        StopScrolling(scrollbar);

        scrollbar->Refresh();

        return FALSE;
    }

    return wxStdScrollBarInputHandler::OnScrollTimer(scrollbar, action);
}

bool wxWin32ScrollBarInputHandler::HandleMouse(wxControl *control,
                                               const wxMouseEvent& event)
{
    // remember the current state
    bool wasDraggingThumb = m_htLast == wxHT_SCROLLBAR_THUMB;

    // do process the message
    bool rc = wxStdScrollBarInputHandler::HandleMouse(control, event);

    // analyse the changes
    if ( !wasDraggingThumb && (m_htLast == wxHT_SCROLLBAR_THUMB) )
    {
        // we just started dragging the thumb, remember its initial position to
        // be able to restore it if the drag is cancelled later
        m_eventStartDrag = event;
    }

    return rc;
}

bool wxWin32ScrollBarInputHandler::HandleMouseMove(wxControl *control,
                                                   const wxMouseEvent& event)
{
    // we don't highlight scrollbar elements, so there is no need to process
    // mouse move events normally - only do it while mouse is captured (i.e.
    // when we're dragging the thumb or pressing on something)
    if ( !m_winCapture )
        return FALSE;

    if ( event.Entering() )
    {
        // we're not interested in this at all
        return FALSE;
    }

    wxScrollBar *scrollbar = wxStaticCast(control, wxScrollBar);
    wxHitTest ht;
    if ( m_scrollPaused )
    {
        // check if the mouse returned to its original location

        if ( event.Leaving() )
        {
            // it surely didn't
            return FALSE;
        }

        ht = m_renderer->HitTestScrollbar(scrollbar, event.GetPosition());
        if ( ht == m_htLast )
        {
            // yes it did, resume scrolling
            m_scrollPaused = FALSE;
            if ( m_timerScroll )
            {
                // we were scrolling by line/page, restart timer
                m_timerScroll->Start(m_interval);

                Press(scrollbar, TRUE);
            }
            else // we were dragging the thumb
            {
                // restore its last location
                HandleThumbMove(scrollbar, m_eventLastDrag);
            }

            return TRUE;
        }
    }
    else // normal case, scrolling hasn't been paused
    {
        // if we're scrolling the scrollbar because the arrow or the shaft was
        // pressed, check that the mouse stays on the same scrollbar element

        if ( event.Moving() )
        {
            ht = m_renderer->HitTestScrollbar(scrollbar, event.GetPosition());
        }
        else // event.Leaving()
        {
            ht = wxHT_NOWHERE;
        }

        // if we're dragging the thumb and the mouse stays in the scrollbar, it
        // is still ok - we only want to catch the case when the mouse leaves
        // the scrollbar here
        if ( m_htLast == wxHT_SCROLLBAR_THUMB && ht != wxHT_NOWHERE )
        {
            ht = wxHT_SCROLLBAR_THUMB;
        }

        if ( ht != m_htLast )
        {
            // what were we doing? 2 possibilities: either an arrow/shaft was
            // pressed in which case we have a timer and so we just stop it or
            // we were dragging the thumb
            if ( m_timerScroll )
            {
                // pause scrolling
                m_interval = m_timerScroll->GetInterval();
                m_timerScroll->Stop();
                m_scrollPaused = TRUE;

                // unpress the arrow
                Press(scrollbar, FALSE);
            }
            else // we were dragging the thumb
            {
                // remember the current thumb position to be able to restore it
                // if the mouse returns to it later
                m_eventLastDrag = event;

                // and restore the original position (before dragging) of the
                // thumb for now
                HandleThumbMove(scrollbar, m_eventStartDrag);
            }

            return TRUE;
        }
    }

    return wxStdScrollBarInputHandler::HandleMouseMove(control, event);
}

// ----------------------------------------------------------------------------
// wxWin32CheckboxInputHandler
// ----------------------------------------------------------------------------

bool wxWin32CheckboxInputHandler::HandleKey(wxControl *control,
                                            const wxKeyEvent& event,
                                            bool pressed)
{
    if ( pressed )
    {
        wxControlAction action;
        int keycode = event.GetKeyCode();
        switch ( keycode )
        {
            case WXK_SPACE:
                action = wxACTION_CHECKBOX_TOGGLE;
                break;

            case WXK_SUBTRACT:
            case WXK_NUMPAD_SUBTRACT:
                action = wxACTION_CHECKBOX_CHECK;
                break;

            case WXK_ADD:
            case WXK_NUMPAD_ADD:
            case WXK_NUMPAD_EQUAL:
                action = wxACTION_CHECKBOX_CLEAR;
                break;
        }

        if ( !!action )
        {
            control->PerformAction(action);

            return TRUE;
        }
    }

    return FALSE;
}

// ----------------------------------------------------------------------------
// wxWin32TextCtrlInputHandler
// ----------------------------------------------------------------------------

bool wxWin32TextCtrlInputHandler::HandleKey(wxControl *control,
                                            const wxKeyEvent& event,
                                            bool pressed)
{
    // handle only MSW-specific text bindings here, the others are handled in
    // the base class
    if ( pressed )
    {
        int keycode = event.GetKeyCode();

        wxControlAction action;
        if ( keycode == WXK_DELETE && event.ShiftDown() )
        {
            action = wxACTION_TEXT_CUT;
        }
        else if ( keycode == WXK_INSERT )
        {
            if ( event.ControlDown() )
                action = wxACTION_TEXT_COPY;
            else if ( event.ShiftDown() )
                action = wxACTION_TEXT_PASTE;
        }

        if ( action != wxACTION_NONE )
        {
            control->PerformAction(action);

            return TRUE;
        }
    }

    return wxStdTextCtrlInputHandler::HandleKey(control, event, pressed);
}

