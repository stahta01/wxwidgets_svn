/////////////////////////////////////////////////////////////////////////////
// Name:        settings.h
// Author:      Vaclav Slavik, Robert Roebling
// Id:          $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "settings.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/settings.h"
#include "wx/colour.h"
#include "wx/font.h"
#include "wx/module.h"

// ----------------------------------------------------------------------------
// global data
// ----------------------------------------------------------------------------

static wxFont *gs_fontDefault = NULL;

class wxSystemSettingsModule : public wxModule
{
public:
    virtual bool OnInit() { return TRUE; }
    virtual void OnExit()
    {
        delete gs_fontDefault;
        gs_fontDefault = NULL;
    }

private:
    DECLARE_DYNAMIC_CLASS(wxSystemSettingsModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxSystemSettingsModule, wxModule)



wxColour wxSystemSettings::GetSystemColour(int WXUNUSED(index))
{
    // FIXME_MGL
    return wxColour(0,0,0);
}

wxFont wxSystemSettings::GetSystemFont(int index)
{
    switch (index)
    {
        case wxSYS_OEM_FIXED_FONT:
        case wxSYS_ANSI_FIXED_FONT:
        case wxSYS_SYSTEM_FIXED_FONT:
        {
            return *wxNORMAL_FONT;
        }
        case wxSYS_ANSI_VAR_FONT:
        case wxSYS_SYSTEM_FONT:
        case wxSYS_DEVICE_DEFAULT_FONT:
        case wxSYS_DEFAULT_GUI_FONT:
        {
            if ( !gs_fontDefault )
                gs_fontDefault = new wxFont(10, wxSWISS, wxNORMAL, wxNORMAL, FALSE, "Arial");
            return *gs_fontDefault;
        }
        default:
            return wxNullFont;
    }
}

int wxSystemSettings::GetSystemMetric(int WXUNUSED(index))
{
    int val;
    
    switch (index)
    {
        case wxSYS_SCREEN_X:
            wxDisplaySize(&val, NULL);
            return val;
        case wxSYS_SCREEN_Y:
            wxDisplaySize(NULL, &val);
            return val;
        case wxSYS_VSCROLL_X:
        case wxSYS_HSCROLL_Y:
            return 15; 
            break;
        default:
            wxCHECK_MSG(index, 0, wxT("wxSystemSettings::GetSystemMetric not fully implemented"));
            return 0;
    }
}

bool wxSystemSettings::GetCapability(int index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME: 
            return FALSE; break;
        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
            return FALSE; break;
        default:
            return FALSE;
    }
}
