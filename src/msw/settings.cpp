/////////////////////////////////////////////////////////////////////////////
// Name:        msw/settings.cpp
// Purpose:     wxSystemSettings
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "settings.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <stdio.h>
    #include "wx/defs.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/gdicmn.h"
#endif

#include "wx/settings.h"
#include "wx/window.h"
#include "wx/msw/private.h"
#include "wx/module.h"
#include "wx/fontutil.h"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// the module which is used to clean up wxSystemSettings data (this is a
// singleton class so it can't be done in the dtor)
class wxSystemSettingsModule : public wxModule
{
    friend class wxSystemSettings;
public:
    virtual bool OnInit();
    virtual void OnExit();

private:
    DECLARE_DYNAMIC_CLASS(wxSystemSettingsModule)
};

// ----------------------------------------------------------------------------
// global data
// ----------------------------------------------------------------------------

static wxFont *gs_fontDefault = NULL;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxSystemSettingsModule
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxSystemSettingsModule, wxModule)

bool wxSystemSettingsModule::OnInit()
{
    return TRUE;
}

void wxSystemSettingsModule::OnExit()
{
    delete gs_fontDefault;
    gs_fontDefault = NULL;
}

// ----------------------------------------------------------------------------
// wxSystemSettings
// ----------------------------------------------------------------------------

// TODO: see ::SystemParametersInfo for all sorts of Windows settings.
// Different args are required depending on the id. How does this differ
// from GetSystemMetric, and should it? Perhaps call it GetSystemParameter
// and pass an optional void* arg to get further info.
// Should also have SetSystemParameter.
// Also implement WM_WININICHANGE (NT) / WM_SETTINGCHANGE (Win95)

wxColour wxSystemSettings::GetSystemColour(int index)
{
    switch (index)
    {
        case wxSYS_COLOUR_LISTBOX:
            return *wxWHITE;

        default:
            COLORREF ref = ::GetSysColor(index);
            wxColour col(GetRValue(ref), GetGValue(ref), GetBValue(ref));
            return col;
    }
}

wxFont wxCreateFontFromStockObject(int index)
{
    wxFont font;

    HFONT hFont = (HFONT) ::GetStockObject(index);
    if ( hFont )
    {
        LOGFONT lf;
        if ( ::GetObject(hFont, sizeof(LOGFONT), &lf) != 0 )
        {
            wxNativeFontInfo info;
            info.lf = lf;
            // Under MicroWindows we pass the HFONT as well
            // because it's hard to convert HFONT -> LOGFONT -> HFONT
            // It's OK to delete stock objects, the delete will be ignored.
#ifdef __WXMICROWIN__
            font.Create(info, (WXHFONT) hFont);
#else
            font.Create(info);
#endif
        }
        else
        {
            wxFAIL_MSG( _T("failed to get LOGFONT") );
        }
    }
    else // GetStockObject() failed
    {
        wxFAIL_MSG( _T("stock font not found") );
    }
    return font;
}

wxFont wxSystemSettings::GetSystemFont(int index)
{
    // wxWindow ctor calls GetSystemFont(wxSYS_DEFAULT_GUI_FONT) so we're
    // called fairly often - this is why we cache this particular font
    bool isDefaultRequested = index == wxSYS_DEFAULT_GUI_FONT;
    if ( isDefaultRequested && gs_fontDefault )
    {
        return *gs_fontDefault;
    }

    wxFont font = wxCreateFontFromStockObject(index);

    if ( isDefaultRequested )
    {
        // if we got here it means we hadn't cached it yet - do now
        gs_fontDefault = new wxFont(font);
    }

    return font;
}

// Get a system metric, e.g. scrollbar size
int wxSystemSettings::GetSystemMetric(int index)
{
#ifdef __WXMICROWIN__
    // TODO: probably use wxUniv themes functionality
    return 0;
#else
    switch ( index)
    {
#ifdef __WIN32__
        case wxSYS_MOUSE_BUTTONS:
            return ::GetSystemMetrics(SM_CMOUSEBUTTONS);
#endif

        case wxSYS_BORDER_X:
            return ::GetSystemMetrics(SM_CXBORDER);
        case wxSYS_BORDER_Y:
            return ::GetSystemMetrics(SM_CYBORDER);
        case wxSYS_CURSOR_X:
            return ::GetSystemMetrics(SM_CXCURSOR);
        case wxSYS_CURSOR_Y:
            return ::GetSystemMetrics(SM_CYCURSOR);
        case wxSYS_DCLICK_X:
            return ::GetSystemMetrics(SM_CXDOUBLECLK);
        case wxSYS_DCLICK_Y:
            return ::GetSystemMetrics(SM_CYDOUBLECLK);
#if defined(__WIN32__) && defined(SM_CXDRAG)
        case wxSYS_DRAG_X:
            return ::GetSystemMetrics(SM_CXDRAG);
        case wxSYS_DRAG_Y:
            return ::GetSystemMetrics(SM_CYDRAG);
        case wxSYS_EDGE_X:
            return ::GetSystemMetrics(SM_CXEDGE);
        case wxSYS_EDGE_Y:
            return ::GetSystemMetrics(SM_CYEDGE);
#endif
        case wxSYS_HSCROLL_ARROW_X:
            return ::GetSystemMetrics(SM_CXHSCROLL);
        case wxSYS_HSCROLL_ARROW_Y:
            return ::GetSystemMetrics(SM_CYHSCROLL);
        case wxSYS_HTHUMB_X:
            return ::GetSystemMetrics(SM_CXHTHUMB);
        case wxSYS_ICON_X:
            return ::GetSystemMetrics(SM_CXICON);
        case wxSYS_ICON_Y:
            return ::GetSystemMetrics(SM_CYICON);
        case wxSYS_ICONSPACING_X:
            return ::GetSystemMetrics(SM_CXICONSPACING);
        case wxSYS_ICONSPACING_Y:
            return ::GetSystemMetrics(SM_CYICONSPACING);
        case wxSYS_WINDOWMIN_X:
            return ::GetSystemMetrics(SM_CXMIN);
        case wxSYS_WINDOWMIN_Y:
            return ::GetSystemMetrics(SM_CYMIN);
        case wxSYS_SCREEN_X:
            return ::GetSystemMetrics(SM_CXSCREEN);
        case wxSYS_SCREEN_Y:
            return ::GetSystemMetrics(SM_CYSCREEN);

#if defined(__WIN32__) && defined(SM_CXSIZEFRAME)
        case wxSYS_FRAMESIZE_X:
            return ::GetSystemMetrics(SM_CXSIZEFRAME);
        case wxSYS_FRAMESIZE_Y:
            return ::GetSystemMetrics(SM_CYSIZEFRAME);
        case wxSYS_SMALLICON_X:
            return ::GetSystemMetrics(SM_CXSMICON);
        case wxSYS_SMALLICON_Y:
            return ::GetSystemMetrics(SM_CYSMICON);
#endif
        case wxSYS_HSCROLL_Y:
            return ::GetSystemMetrics(SM_CYHSCROLL);
        case wxSYS_VSCROLL_X:
            return ::GetSystemMetrics(SM_CXVSCROLL);
        case wxSYS_VSCROLL_ARROW_X:
            return ::GetSystemMetrics(SM_CXVSCROLL);
        case wxSYS_VSCROLL_ARROW_Y:
            return ::GetSystemMetrics(SM_CYVSCROLL);
        case wxSYS_VTHUMB_Y:
            return ::GetSystemMetrics(SM_CYVTHUMB);
        case wxSYS_CAPTION_Y:
            return ::GetSystemMetrics(SM_CYCAPTION);
        case wxSYS_MENU_Y:
            return ::GetSystemMetrics(SM_CYMENU);
#if defined(__WIN32__) && defined(SM_NETWORK)
        case wxSYS_NETWORK_PRESENT:
            return ::GetSystemMetrics(SM_NETWORK) & 0x0001;
#endif
        case wxSYS_PENWINDOWS_PRESENT:
            return ::GetSystemMetrics(SM_PENWINDOWS);
#if defined(__WIN32__) && defined(SM_SHOWSOUNDS)
        case wxSYS_SHOW_SOUNDS:
            return ::GetSystemMetrics(SM_SHOWSOUNDS);
#endif
        case wxSYS_SWAP_BUTTONS:
            return ::GetSystemMetrics(SM_SWAPBUTTON);
        default:
            return 0;
    }
#endif
    // __WXMICROWIN__
}

bool wxSystemSettings::GetCapability(int index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME: 
        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
            return TRUE; break;
        default:
            return FALSE;
    }
}
