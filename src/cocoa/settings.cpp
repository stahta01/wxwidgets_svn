/////////////////////////////////////////////////////////////////////////////
// Name:        settings.cpp
// Purpose:     wxSettings
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "settings.h"
#endif

#include "wx/settings.h"
#include "wx/gdicmn.h"
#include "wx/utils.h"

// ----------------------------------------------------------------------------
// wxSystemSettingsNative
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// colours
// ----------------------------------------------------------------------------

wxColour wxSystemSettingsNative::GetColour(wxSystemColour index)
{
    return wxColour();
#if 0
    int major,minor;
    wxGetOsVersion( &major, &minor );
    
    switch( index )
    {
        case wxSYS_COLOUR_SCROLLBAR :
        case wxSYS_COLOUR_BACKGROUND:
        case wxSYS_COLOUR_ACTIVECAPTION:
        case wxSYS_COLOUR_INACTIVECAPTION:
        case wxSYS_COLOUR_MENU:
        case wxSYS_COLOUR_WINDOW:
        case wxSYS_COLOUR_WINDOWFRAME:
        case wxSYS_COLOUR_ACTIVEBORDER:
        case wxSYS_COLOUR_INACTIVEBORDER:
        case wxSYS_COLOUR_BTNFACE:
        case wxSYS_COLOUR_MENUBAR:
            return wxColor( 0xDD , 0xDD , 0xDD ) ;
            break ;

        case wxSYS_COLOUR_LISTBOX :
        {
            if (major >= 10)
                return *wxWHITE ;
            else
                return wxColor( 0xEE , 0xEE , 0xEE ) ;
            break ;
        }
        case wxSYS_COLOUR_BTNSHADOW:
            return wxColor( 0x44 , 0x44 , 0x44 ) ;
            break ;

        case wxSYS_COLOUR_BTNTEXT:
        case wxSYS_COLOUR_MENUTEXT:
        case wxSYS_COLOUR_WINDOWTEXT:
        case wxSYS_COLOUR_CAPTIONTEXT:
        case wxSYS_COLOUR_INFOTEXT:
        case wxSYS_COLOUR_INACTIVECAPTIONTEXT:
            return *wxBLACK;
            break ;
        case wxSYS_COLOUR_HIGHLIGHT:
            {
                RGBColor hilite ;
                LMGetHiliteRGB(&hilite) ;
                return wxColor( hilite.red >> 8 , hilite.green >> 8  , hilite.blue >> 8  ) ;
            }
            break ;
        case wxSYS_COLOUR_BTNHIGHLIGHT:
        case wxSYS_COLOUR_GRAYTEXT:
            return wxColor( 0xCC , 0xCC , 0xCC ) ;
            break ;
    
        case wxSYS_COLOUR_3DDKSHADOW:
            return wxColor( 0x44 , 0x44 , 0x44 ) ;
            break ;
        case wxSYS_COLOUR_3DLIGHT:
            return wxColor( 0xCC , 0xCC , 0xCC ) ;
            break ;
        case wxSYS_COLOUR_HIGHLIGHTTEXT :
            {
                RGBColor hilite ;
                LMGetHiliteRGB(&hilite) ;
                if ( ( hilite.red + hilite.green + hilite.blue ) == 0 )
                        return *wxWHITE ;
                else
                        return *wxBLACK ;
            }
            break ;
        case wxSYS_COLOUR_INFOBK :
        case wxSYS_COLOUR_APPWORKSPACE: 
            return *wxWHITE ;
            break ;

        case wxSYS_COLOUR_HOTLIGHT:
        case wxSYS_COLOUR_GRADIENTACTIVECAPTION:
        case wxSYS_COLOUR_GRADIENTINACTIVECAPTION:
        case wxSYS_COLOUR_MENUHILIGHT:
            // TODO
            return *wxBLACK;
         
        case wxSYS_COLOUR_MAX:
            wxFAIL_MSG( _T("unknown system colour index") );
            break ;
    }
    return *wxWHITE;
#endif
}

// ----------------------------------------------------------------------------
// fonts
// ----------------------------------------------------------------------------

wxFont wxSystemSettingsNative::GetFont(wxSystemFont index)
{
// return a nonworking font object, crash from wxInitializeStockObjects
    return wxFont();
    switch (index)
    {
        case wxSYS_ANSI_VAR_FONT :
        case wxSYS_SYSTEM_FONT :
        case wxSYS_DEVICE_DEFAULT_FONT :
        case wxSYS_DEFAULT_GUI_FONT :
            {
                return *wxSMALL_FONT ;
                    } ;
            break ;
        case wxSYS_OEM_FIXED_FONT :
        case wxSYS_ANSI_FIXED_FONT :
        case wxSYS_SYSTEM_FIXED_FONT :
        default :
            {
                return *wxNORMAL_FONT ;
                    } ;
            break ;
                
    }
    return *wxNORMAL_FONT;
}

// ----------------------------------------------------------------------------
// system metrics/features
// ----------------------------------------------------------------------------

// Get a system metric, e.g. scrollbar size
int wxSystemSettingsNative::GetMetric(wxSystemMetric index)
{
    switch ( index)
    {
        case wxSYS_MOUSE_BUTTONS:
                    return 2; // we emulate a two button mouse (ctrl + click = right button )
                    
        // TODO case wxSYS_BORDER_X:
        // TODO case wxSYS_BORDER_Y:
        // TODO case wxSYS_CURSOR_X:
        // TODO case wxSYS_CURSOR_Y:
        // TODO case wxSYS_DCLICK_X:
        // TODO case wxSYS_DCLICK_Y:
        // TODO case wxSYS_DRAG_X:
        // TODO case wxSYS_DRAG_Y:
        // TODO case wxSYS_EDGE_X:
        // TODO case wxSYS_EDGE_Y:

        case wxSYS_HSCROLL_ARROW_X:
            return 16;
        case wxSYS_HSCROLL_ARROW_Y:
            return 16;
        case wxSYS_HTHUMB_X:
            return 16;
            
        // TODO case wxSYS_ICON_X:
        // TODO case wxSYS_ICON_Y:
        // TODO case wxSYS_ICONSPACING_X:
        // TODO case wxSYS_ICONSPACING_Y:
        // TODO case wxSYS_WINDOWMIN_X:
        // TODO case wxSYS_WINDOWMIN_Y:
        // TODO case wxSYS_SCREEN_X:
        // TODO case wxSYS_SCREEN_Y:
        // TODO case wxSYS_FRAMESIZE_X:
        // TODO case wxSYS_FRAMESIZE_Y:
        // TODO case wxSYS_SMALLICON_X:
        // TODO case wxSYS_SMALLICON_Y:
            
        case wxSYS_HSCROLL_Y:
            return 16;
        case wxSYS_VSCROLL_X:
            return 16;
        case wxSYS_VSCROLL_ARROW_X:
            return 16;
        case wxSYS_VSCROLL_ARROW_Y:
            return 16;
        case wxSYS_VTHUMB_Y:
            return 16;
            
        // TODO case wxSYS_CAPTION_Y:
        // TODO case wxSYS_MENU_Y:
        // TODO case wxSYS_NETWORK_PRESENT:
            
        case wxSYS_PENWINDOWS_PRESENT:
            return 0;
            
        // TODO case wxSYS_SHOW_SOUNDS:

        case wxSYS_SWAP_BUTTONS:
            return 0;
            
        default:
            return -1;  // unsupported metric
    }
    return 0;
}

bool wxSystemSettingsNative::HasFeature(wxSystemFeature index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME: 
        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
            return TRUE;
            
        default:
            return FALSE;
    }
}
