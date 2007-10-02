///////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/renderer.cpp
// Purpose:     implementation of wxRendererNative for Windows
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.07.2003
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/window.h"
    #include "wx/dc.h"
    #include "wx/settings.h"
#endif //WX_PRECOMP

#include "wx/scopeguard.h"
#include "wx/splitter.h"
#include "wx/renderer.h"
#include "wx/msw/private.h"
#include "wx/msw/uxtheme.h"

// tmschema.h is in Win32 Platform SDK and might not be available with earlier
// compilers
#ifndef CP_DROPDOWNBUTTON
    #define BP_PUSHBUTTON      1
    #define BP_CHECKBOX        3
    #define CBS_UNCHECKEDNORMAL 1
    #define CBS_CHECKEDNORMAL   (CBS_UNCHECKEDNORMAL + 4)
    #define CBS_MIXEDNORMAL     (CBS_CHECKEDNORMAL + 4)

    #define PBS_NORMAL          1
    #define PBS_HOT             2
    #define PBS_PRESSED         3
    #define PBS_DISABLED        4
    #define PBS_DEFAULTED       5

    #define CP_DROPDOWNBUTTON  1

    #define CBXS_NORMAL        1
    #define CBXS_HOT           2
    #define CBXS_PRESSED       3
    #define CBXS_DISABLED      4

    #define TVP_GLYPH           2

    #define GLPS_CLOSED         1
    #define GLPS_OPENED         2

    #define HP_HEADERITEM       1

    #define HIS_NORMAL          1
    #define HIS_HOT             2
    #define HIS_PRESSED         3

    #define TMT_HEIGHT          2417

    #define HP_HEADERSORTARROW  4
    #define HSAS_SORTEDUP       1
    #define HSAS_SORTEDDOWN     2
#endif

#if defined(__WXWINCE__)
    #ifndef DFCS_FLAT
        #define DFCS_FLAT 0
    #endif
    #ifndef DFCS_MONO
        #define DFCS_MONO 0
    #endif
#endif

#ifndef DFCS_HOT
    #define DFCS_HOT 0x1000
#endif

// ----------------------------------------------------------------------------
// wxRendererMSW: wxRendererNative implementation for "old" Win32 systems
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxRendererMSW : public wxDelegateRendererNative
{
public:
    wxRendererMSW() { }

    static wxRendererNative& Get();

    virtual void DrawComboBoxDropButton(wxWindow *win,
                                        wxDC& dc,
                                        const wxRect& rect,
                                        int flags = 0);

    virtual void DrawCheckBox(wxWindow *win,
                              wxDC& dc,
                              const wxRect& rect,
                              int flags = 0);

    virtual void DrawPushButton(wxWindow *win,
                                wxDC& dc,
                                const wxRect& rect,
                                int flags = 0);

    virtual void DrawFocusRect(wxWindow* win,
                               wxDC& dc,
                               const wxRect& rect,
                               int flags = 0);

    virtual int GetHeaderButtonHeight(wxWindow *win);

private:
    DECLARE_NO_COPY_CLASS(wxRendererMSW)
};

// ----------------------------------------------------------------------------
// wxRendererXP: wxRendererNative implementation for Windows XP and later
// ----------------------------------------------------------------------------

#if wxUSE_UXTHEME

class WXDLLEXPORT wxRendererXP : public wxDelegateRendererNative
{
public:
    wxRendererXP() : wxDelegateRendererNative(wxRendererMSW::Get()) { }

    static wxRendererNative& Get();

    virtual int DrawHeaderButton(wxWindow *win,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int flags = 0,
                                  wxHeaderSortIconType sortArrow = wxHDR_SORT_ICON_NONE,
                                  wxHeaderButtonParams* params = NULL);

    virtual void DrawTreeItemButton(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0);
    virtual void DrawSplitterBorder(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0);
    virtual void DrawSplitterSash(wxWindow *win,
                                  wxDC& dc,
                                  const wxSize& size,
                                  wxCoord position,
                                  wxOrientation orient,
                                  int flags = 0);
    virtual void DrawComboBoxDropButton(wxWindow *win,
                                        wxDC& dc,
                                        const wxRect& rect,
                                        int flags = 0);
    virtual void DrawCheckBox(wxWindow *win,
                              wxDC& dc,
                              const wxRect& rect,
                              int flags = 0);

    virtual void DrawPushButton(wxWindow *win,
                                wxDC& dc,
                                const wxRect& rect,
                                int flags = 0);

    virtual void DrawItemSelectionRect(wxWindow *win,
                                       wxDC& dc,
                                       const wxRect& rect,
                                       int flags = 0 );


    virtual wxSplitterRenderParams GetSplitterParams(const wxWindow *win);
private:
    DECLARE_NO_COPY_CLASS(wxRendererXP)
};

#endif // wxUSE_UXTHEME

// ============================================================================
// wxRendererNative and wxRendererMSW implementation
// ============================================================================

/* static */
wxRendererNative& wxRendererNative::GetDefault()
{
#if wxUSE_UXTHEME
    wxUxThemeEngine *themeEngine = wxUxThemeEngine::Get();
    if ( themeEngine && themeEngine->IsAppThemed() )
        return wxRendererXP::Get();
#endif // wxUSE_UXTHEME

    return wxRendererMSW::Get();
}

/* static */
wxRendererNative& wxRendererMSW::Get()
{
    static wxRendererMSW s_rendererMSW;

    return s_rendererMSW;
}

void
wxRendererMSW::DrawComboBoxDropButton(wxWindow * WXUNUSED(win),
                                      wxDC& dc,
                                      const wxRect& rect,
                                      int flags)
{
    RECT r;
    wxCopyRectToRECT(rect, r);

    int style = DFCS_SCROLLCOMBOBOX;
    if ( flags & wxCONTROL_DISABLED )
        style |= DFCS_INACTIVE;
    if ( flags & wxCONTROL_PRESSED )
        style |= DFCS_PUSHED | DFCS_FLAT;

    ::DrawFrameControl(GetHdcOf(dc), &r, DFC_SCROLL, style);
}

void
wxRendererMSW::DrawCheckBox(wxWindow * WXUNUSED(win),
                            wxDC& dc,
                            const wxRect& rect,
                            int flags)
{
    RECT r;
    wxCopyRectToRECT(rect, r);

    int style = DFCS_BUTTONCHECK;
    if ( flags & wxCONTROL_CHECKED )
        style |= DFCS_CHECKED;
    if ( flags & wxCONTROL_DISABLED )
        style |= DFCS_INACTIVE;
    if ( flags & wxCONTROL_FLAT )
        style |= DFCS_MONO;
    if ( flags & wxCONTROL_PRESSED )
        style |= DFCS_PUSHED;
    if ( flags & wxCONTROL_CURRENT )
        style |= DFCS_HOT;

    ::DrawFrameControl(GetHdcOf(dc), &r, DFC_BUTTON, style);
}

void
wxRendererMSW::DrawPushButton(wxWindow * WXUNUSED(win),
                              wxDC& dc,
                              const wxRect& rectOrig,
                              int flags)
{
    wxRect rect(rectOrig);

    int style = DFCS_BUTTONPUSH;
    if ( flags & wxCONTROL_DISABLED )
        style |= DFCS_INACTIVE;
    if ( flags & wxCONTROL_PRESSED )
        style |= DFCS_PUSHED | DFCS_FLAT;
    if ( flags & wxCONTROL_ISDEFAULT )
    {
        // DrawFrameControl() doesn't seem to support default buttons so we
        // have to draw the border ourselves
        wxDCPenChanger pen(dc, *wxBLACK_PEN);
        wxDCBrushChanger brush(dc, *wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(rect);
        rect.Deflate(1);
    }

    RECT rc;
    wxCopyRectToRECT(rect, rc);

    ::DrawFrameControl(GetHdcOf(dc), &rc, DFC_BUTTON, style);
}

void wxRendererMSW::DrawFocusRect(wxWindow * WXUNUSED(win),
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int WXUNUSED(flags))
{
    RECT rc;
    wxCopyRectToRECT(rect, rc);

    ::DrawFocusRect(GetHdcOf(dc), &rc);
}

int wxRendererMSW::GetHeaderButtonHeight(wxWindow * WXUNUSED(win))
{
    // some "reasonable" value returned in case of error, it doesn't really
    // correspond to anything but it's better than returning 0
    static const int DEFAULT_HEIGHT = 20;


    // create a temporary header window just to get its geometry
    HWND hwndHeader = ::CreateWindow(WC_HEADER, NULL, 0,
                                     0, 0, 0, 0, NULL, NULL, NULL, NULL);
    if ( !hwndHeader )
        return DEFAULT_HEIGHT;

    wxON_BLOCK_EXIT1( ::DestroyWindow, hwndHeader );

    // initialize the struct filled with the values by Header_Layout()
    RECT parentRect = { 0, 0, 100, 100 };
    WINDOWPOS wp = { 0, 0, 0, 0, 0, 0, 0 };
    HDLAYOUT hdl = { &parentRect, &wp };

    return Header_Layout(hwndHeader, &hdl) ? wp.cy : DEFAULT_HEIGHT;
}

// ============================================================================
// wxRendererXP implementation
// ============================================================================

#if wxUSE_UXTHEME

/* static */
wxRendererNative& wxRendererXP::Get()
{
    static wxRendererXP s_rendererXP;

    return s_rendererXP;
}

// NOTE: There is no guarantee that the button drawn fills the entire rect (XP
// default theme, for example), so the caller should have cleared button's
// background before this call. This is quite likely a wxMSW-specific thing.
void
wxRendererXP::DrawComboBoxDropButton(wxWindow * win,
                                      wxDC& dc,
                                      const wxRect& rect,
                                      int flags)
{
    wxUxThemeHandle hTheme(win, L"COMBOBOX");
    if ( !hTheme )
    {
        m_rendererNative.DrawComboBoxDropButton(win, dc, rect, flags);
        return;
    }

    RECT r;
    wxCopyRectToRECT(rect, r);

    int state;
    if ( flags & wxCONTROL_PRESSED )
        state = CBXS_PRESSED;
    else if ( flags & wxCONTROL_CURRENT )
        state = CBXS_HOT;
    else if ( flags & wxCONTROL_DISABLED )
        state = CBXS_DISABLED;
    else
        state = CBXS_NORMAL;

    wxUxThemeEngine::Get()->DrawThemeBackground
                            (
                                hTheme,
                                GetHdcOf(dc),
                                CP_DROPDOWNBUTTON,
                                state,
                                &r,
                                NULL
                            );

}

int
wxRendererXP::DrawHeaderButton(wxWindow *win,
                               wxDC& dc,
                               const wxRect& rect,
                               int flags,
                               wxHeaderSortIconType sortArrow,
                               wxHeaderButtonParams* params)
{
    wxUxThemeHandle hTheme(win, L"HEADER");
    if ( !hTheme )
    {
        return m_rendererNative.DrawHeaderButton(win, dc, rect, flags, sortArrow, params);
    }

    RECT r;
    wxCopyRectToRECT(rect, r);

    int state;
    if ( flags & wxCONTROL_PRESSED )
        state = HIS_PRESSED;
    else if ( flags & wxCONTROL_CURRENT )
        state = HIS_HOT;
    else
        state = HIS_NORMAL;
    wxUxThemeEngine::Get()->DrawThemeBackground
                            (
                                hTheme,
                                GetHdcOf(dc),
                                HP_HEADERITEM,
                                state,
                                &r,
                                NULL
                            );

    // NOTE: Using the theme to draw HP_HEADERSORTARROW doesn't do anything.
    // Why?  If this can be fixed then draw the sort arrows using the theme
    // and then clear those flags before calling DrawHeaderButtonContents.

    // Add any extras that are specified in flags and params
    return DrawHeaderButtonContents(win, dc, rect, flags, sortArrow, params);
}


void
wxRendererXP::DrawTreeItemButton(wxWindow *win,
                                 wxDC& dc,
                                 const wxRect& rect,
                                 int flags)
{
    wxUxThemeHandle hTheme(win, L"TREEVIEW");
    if ( !hTheme )
    {
        m_rendererNative.DrawTreeItemButton(win, dc, rect, flags);
        return;
    }

    RECT r;
    wxCopyRectToRECT(rect, r);

    int state = flags & wxCONTROL_EXPANDED ? GLPS_OPENED : GLPS_CLOSED;
    wxUxThemeEngine::Get()->DrawThemeBackground
                            (
                                hTheme,
                                GetHdcOf(dc),
                                TVP_GLYPH,
                                state,
                                &r,
                                NULL
                            );
}

void
wxRendererXP::DrawCheckBox(wxWindow *win,
                           wxDC& dc,
                           const wxRect& rect,
                           int flags)
{
    wxUxThemeHandle hTheme(win, L"BUTTON");
    if ( !hTheme )
    {
        m_rendererNative.DrawCheckBox(win, dc, rect, flags);
        return;
    }

    RECT r;
    wxCopyRectToRECT(rect, r);

    int state;
    if ( flags & wxCONTROL_CHECKED )
        state = CBS_CHECKEDNORMAL;
    else if ( flags & wxCONTROL_UNDETERMINED )
        state = CBS_MIXEDNORMAL;
    else
        state = CBS_UNCHECKEDNORMAL;

    // CBS_XXX is followed by CBX_XXXHOT, then CBS_XXXPRESSED and DISABLED
    enum
    {
        CBS_HOT_OFFSET = 1,
        CBS_PRESSED_OFFSET = 2,
        CBS_DISABLED_OFFSET = 3
    };

    if ( flags & wxCONTROL_DISABLED )
        state += CBS_DISABLED_OFFSET;
    else if ( flags & wxCONTROL_PRESSED )
        state += CBS_PRESSED_OFFSET;
    else if ( flags & wxCONTROL_CURRENT )
        state += CBS_HOT_OFFSET;

    wxUxThemeEngine::Get()->DrawThemeBackground
                            (
                                hTheme,
                                GetHdcOf(dc),
                                BP_CHECKBOX,
                                state,
                                &r,
                                NULL
                            );
}

void
wxRendererXP::DrawPushButton(wxWindow * win,
                             wxDC& dc,
                             const wxRect& rect,
                             int flags)
{
    wxUxThemeHandle hTheme(win, L"BUTTON");
    if ( !hTheme )
    {
        m_rendererNative.DrawPushButton(win, dc, rect, flags);
        return;
    }

    RECT r;
    wxCopyRectToRECT(rect, r);

    int state;
    if ( flags & wxCONTROL_PRESSED )
        state = PBS_PRESSED;
    else if ( flags & wxCONTROL_CURRENT )
        state = PBS_HOT;
    else if ( flags & wxCONTROL_DISABLED )
        state = PBS_DISABLED;
    else if ( flags & wxCONTROL_ISDEFAULT )
        state = PBS_DEFAULTED;
    else
        state = PBS_NORMAL;

    wxUxThemeEngine::Get()->DrawThemeBackground
                            (
                                hTheme,
                                GetHdcOf(dc),
                                BP_PUSHBUTTON,
                                state,
                                &r,
                                NULL
                            );

}

void
wxRendererXP::DrawItemSelectionRect(wxWindow * WXUNUSED(win),
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags)
{
    wxBrush brush;
    if ( flags & wxCONTROL_SELECTED )
    {
        if ( flags & wxCONTROL_FOCUSED )
        {
            brush = wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        }
        else // !focused
        {
            brush = wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
        }
    }
    else // !selected
    {
        brush = *wxTRANSPARENT_BRUSH;
    }

    dc.SetBrush(brush);

    // unlike for wxRendererGeneric, on windows we _never_ want to draw
    // the outline of the rectangle:
    dc.SetPen(*wxTRANSPARENT_PEN);

    dc.DrawRectangle( rect );
}



// ----------------------------------------------------------------------------
// splitter drawing
// ----------------------------------------------------------------------------

// the width of the sash: this is the same as used by Explorer...
static const wxCoord SASH_WIDTH = 4;

wxSplitterRenderParams
wxRendererXP::GetSplitterParams(const wxWindow * win)
{
    if ( win->HasFlag(wxSP_NO_XP_THEME) )
        return m_rendererNative.GetSplitterParams(win);
    else
        return wxSplitterRenderParams(SASH_WIDTH, 0, false);
}

void
wxRendererXP::DrawSplitterBorder(wxWindow * win,
                                 wxDC& dc,
                                 const wxRect& rect,
                                 int flags)
{
    if ( win->HasFlag(wxSP_NO_XP_THEME) )
    {
        m_rendererNative.DrawSplitterBorder(win, dc, rect, flags);
    }
}

void
wxRendererXP::DrawSplitterSash(wxWindow *win,
                               wxDC& dc,
                               const wxSize& size,
                               wxCoord position,
                               wxOrientation orient,
                               int flags)
{
    if ( !win->HasFlag(wxSP_NO_XP_THEME) )
    {
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE)));
        if ( orient == wxVERTICAL )
        {
            dc.DrawRectangle(position, 0, SASH_WIDTH, size.y);
        }
        else // wxHORIZONTAL
        {
            dc.DrawRectangle(0, position, size.x, SASH_WIDTH);
        }

        return;
    }

    m_rendererNative.DrawSplitterSash(win, dc, size, position, orient, flags);
}

#endif // wxUSE_UXTHEME
