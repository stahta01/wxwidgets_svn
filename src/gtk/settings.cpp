/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/settings.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "settings.h"
#endif

#include "wx/settings.h"
#include "wx/debug.h"

#include "wx/cmndata.h"

#include <gdk/gdk.h>
#include <gdk/gdkprivate.h>
#include <gtk/gtk.h>

extern GdkFont *GtkGetDefaultGuiFont();


/*
#define wxSYS_COLOUR_SCROLLBAR         0
#define wxSYS_COLOUR_BACKGROUND        1
#define wxSYS_COLOUR_ACTIVECAPTION     2
#define wxSYS_COLOUR_INACTIVECAPTION   3
#define wxSYS_COLOUR_MENU              4
#define wxSYS_COLOUR_WINDOW            5
#define wxSYS_COLOUR_WINDOWFRAME       6
#define wxSYS_COLOUR_MENUTEXT          7
#define wxSYS_COLOUR_WINDOWTEXT        8
#define wxSYS_COLOUR_CAPTIONTEXT       9
#define wxSYS_COLOUR_ACTIVEBORDER      10
#define wxSYS_COLOUR_INACTIVEBORDER    11
#define wxSYS_COLOUR_APPWORKSPACE      12
#define wxSYS_COLOUR_HIGHLIGHT         13
#define wxSYS_COLOUR_HIGHLIGHTTEXT     14
#define wxSYS_COLOUR_BTNFACE           15
#define wxSYS_COLOUR_BTNSHADOW         16
#define wxSYS_COLOUR_GRAYTEXT          17
#define wxSYS_COLOUR_BTNTEXT           18
#define wxSYS_COLOUR_INACTIVECAPTIONTEXT 19
#define wxSYS_COLOUR_BTNHIGHLIGHT      20

#define wxSYS_COLOUR_3DDKSHADOW        21
#define wxSYS_COLOUR_3DLIGHT           22
#define wxSYS_COLOUR_INFOTEXT          23
#define wxSYS_COLOUR_INFOBK            24

#define wxSYS_COLOUR_DESKTOP           wxSYS_COLOUR_BACKGROUND
#define wxSYS_COLOUR_3DFACE            wxSYS_COLOUR_BTNFACE
#define wxSYS_COLOUR_3DSHADOW          wxSYS_COLOUR_BTNSHADOW
#define wxSYS_COLOUR_3DHIGHLIGHT       wxSYS_COLOUR_BTNHIGHLIGHT
#define wxSYS_COLOUR_3DHILIGHT         wxSYS_COLOUR_BTNHIGHLIGHT
#define wxSYS_COLOUR_BTNHILIGHT        wxSYS_COLOUR_BTNHIGHLIGHT
*/

#define SHIFT (8*(sizeof(short int)-sizeof(char)))

//wxColour *g_systemWinColour          = (wxColour *) NULL;
wxColour *g_systemBtnFaceColour       = (wxColour *) NULL;
wxColour *g_systemBtnShadowColour     = (wxColour *) NULL;
wxColour *g_systemBtnHighlightColour  = (wxColour *) NULL;
wxColour *g_systemHighlightColour     = (wxColour *) NULL;
wxColour *g_systemHighlightTextColour = (wxColour *) NULL;
wxColour *g_systemListBoxColour       = (wxColour *) NULL;
wxColour *g_systemBtnTextColour       = (wxColour *) NULL;

wxFont *g_systemFont = (wxFont *) NULL;

void wxSystemSettings::Done()
{
//    delete g_systemWinColour;
    delete g_systemBtnFaceColour;
    delete g_systemBtnShadowColour;
    delete g_systemBtnHighlightColour;
    delete g_systemHighlightColour;
    delete g_systemHighlightTextColour;
    delete g_systemListBoxColour;
    delete g_systemFont;
    delete g_systemBtnTextColour;
}

// kind of widget to use in GetColourFromGTKWidget
enum wxGtkWidgetType
{
    wxGTK_BUTTON,
    wxGTK_LIST
};

// the colour we need
enum wxGtkColourType
{
    wxGTK_FG,
    wxGTK_BG,
    wxGTK_BASE
};

// wxSystemSettings::GetSystemColour() helper: get the colours from a GTK+
// widget style, return true if we did get them, false to use defaults
static bool GetColourFromGTKWidget(int& red, int& green, int& blue,
                                   wxGtkWidgetType type = wxGTK_BUTTON,
                                   GtkStateType state = GTK_STATE_NORMAL,
                                   wxGtkColourType colour = wxGTK_BG)
{
    GtkWidget *widget;
    switch ( type )
    {
        default:
            wxFAIL_MSG( _T("unexpected GTK widget type") );
            // fall through

        case wxGTK_BUTTON:
            widget = gtk_button_new();
            break;

        case wxGTK_LIST:
            widget = gtk_list_new();
    }

    GtkStyle *def = gtk_rc_get_style( widget );
    if ( !def )
        def = gtk_widget_get_default_style();

    bool ok;
    if ( def )
    {
        GdkColor *col;
        switch ( colour )
        {
            default:
                wxFAIL_MSG( _T("unexpected GTK colour type") );
                // fall through

            case wxGTK_FG:
                col = def->fg;
                break;

            case wxGTK_BG:
                col = def->bg;
                break;

            case wxGTK_BASE:
                col = def->base;
                break;
        }

        red = col[state].red;
        green = col[state].green;
        blue = col[state].blue;

        ok = TRUE;
    }
    else
    {
        ok = FALSE;
    }

    gtk_widget_destroy( widget );

    return ok;
}

wxColour wxSystemSettings::GetSystemColour( int index )
{
    switch (index)
    {
        case wxSYS_COLOUR_SCROLLBAR:
        case wxSYS_COLOUR_BACKGROUND:
        case wxSYS_COLOUR_ACTIVECAPTION:
        case wxSYS_COLOUR_INACTIVECAPTION:
        case wxSYS_COLOUR_MENU:
        case wxSYS_COLOUR_WINDOWFRAME:
        case wxSYS_COLOUR_ACTIVEBORDER:
        case wxSYS_COLOUR_INACTIVEBORDER:
        case wxSYS_COLOUR_BTNFACE:
        case wxSYS_COLOUR_3DLIGHT:
            if (!g_systemBtnFaceColour)
            {
                int red, green, blue;
                if ( !GetColourFromGTKWidget(red, green, blue) )
                {
                    red =
                    green = 0;
                    blue = 0x9c40;
                }

                g_systemBtnFaceColour = new wxColour( red   >> SHIFT,
                                                      green >> SHIFT,
                                                      blue  >> SHIFT );
            }
            return *g_systemBtnFaceColour;

        case wxSYS_COLOUR_WINDOW:
            return *wxWHITE;

        case wxSYS_COLOUR_3DDKSHADOW:
            return *wxBLACK;

        case wxSYS_COLOUR_GRAYTEXT:
        case wxSYS_COLOUR_BTNSHADOW:
        //case wxSYS_COLOUR_3DSHADOW:
            if (!g_systemBtnShadowColour)
            {
                wxColour faceColour(GetSystemColour(wxSYS_COLOUR_3DFACE));
                g_systemBtnShadowColour =
                   new wxColour((unsigned char) (faceColour.Red() * 0.666),
                                (unsigned char) (faceColour.Green() * 0.666),
                                (unsigned char) (faceColour.Blue() * 0.666));
            }

            return *g_systemBtnShadowColour;

        case wxSYS_COLOUR_3DHIGHLIGHT:
        //case wxSYS_COLOUR_BTNHIGHLIGHT:
            return * wxWHITE;
/* I think this should normally be white (JACS 8/2000)

   Hmm, I'm quite sure it shouldn't ... (VZ 20.08.01)
            if (!g_systemBtnHighlightColour)
            {
                g_systemBtnHighlightColour =
                    new wxColour( 0xea60 >> SHIFT,
                                  0xea60 >> SHIFT,
                                  0xea60 >> SHIFT );
            }
            return *g_systemBtnHighlightColour;
*/

        case wxSYS_COLOUR_HIGHLIGHT:
            if (!g_systemHighlightColour)
            {
                int red, green, blue;
                if ( !GetColourFromGTKWidget(red, green, blue,
                                             wxGTK_BUTTON,
                                             GTK_STATE_SELECTED) )
                {
                    red =
                    green = 0;
                    blue = 0x9c40;
                }

                g_systemHighlightColour = new wxColour( red   >> SHIFT,
                                                        green >> SHIFT,
                                                        blue  >> SHIFT );
            }
            return *g_systemHighlightColour;

        case wxSYS_COLOUR_LISTBOX:
            if (!g_systemListBoxColour)
            {
                int red, green, blue;
                if ( GetColourFromGTKWidget(red, green, blue,
                                            wxGTK_LIST,
                                            GTK_STATE_NORMAL,
                                            wxGTK_BASE) )
                {
                    g_systemListBoxColour = new wxColour( red   >> SHIFT,
                                                          green >> SHIFT,
                                                          blue  >> SHIFT );
                }
                else
                {
                    g_systemListBoxColour = new wxColour(*wxWHITE);
                }
            }
            return *g_systemListBoxColour;

        case wxSYS_COLOUR_MENUTEXT:
        case wxSYS_COLOUR_WINDOWTEXT:
        case wxSYS_COLOUR_CAPTIONTEXT:
        case wxSYS_COLOUR_INACTIVECAPTIONTEXT:
        case wxSYS_COLOUR_BTNTEXT:
        case wxSYS_COLOUR_INFOTEXT:
            if (!g_systemBtnTextColour)
            {
                int red, green, blue;
                if ( !GetColourFromGTKWidget(red, green, blue,
                                             wxGTK_BUTTON,
                                             GTK_STATE_NORMAL,
                                             wxGTK_FG) )
                {
                    red =
                    green =
                    blue = 0;
                }

                g_systemBtnTextColour = new wxColour( red   >> SHIFT,
                                                      green >> SHIFT,
                                                      blue  >> SHIFT );
            }
            return *g_systemBtnTextColour;

        case wxSYS_COLOUR_HIGHLIGHTTEXT:
            if (!g_systemHighlightTextColour)
            {
                wxColour hclr = GetSystemColour(wxSYS_COLOUR_HIGHLIGHT);
                if (hclr.Red() > 200 && hclr.Green() > 200 && hclr.Blue() > 200)
                    g_systemHighlightTextColour = new wxColour(*wxBLACK);
                else
                    g_systemHighlightTextColour = new wxColour(*wxWHITE);
            }
            return *g_systemHighlightTextColour;

        case wxSYS_COLOUR_INFOBK:
        case wxSYS_COLOUR_APPWORKSPACE:
            return *wxWHITE;    // ?
  }

  return *wxWHITE;
}

wxFont wxSystemSettings::GetSystemFont( int index )
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
            if (!g_systemFont)
            {
#if 0
                GdkFont *gdk_font = GtkGetDefaultGuiFont();
                if (gdk_font)
                {
                    GSList *font_list = ((GdkFontPrivate*)gdk_font)->names;
                    char *name = (char*)font_list->data;
                    wxString font_string( name );
                    wxFontData font_data;
                    g_systemFont = new wxFont( font_string, font_data );
                }
                gtk_widget_destroy( widget );
#endif

                g_systemFont = new wxFont( 12, wxSWISS, wxNORMAL, wxNORMAL );

            }
            return *g_systemFont;
        }
    }

    return wxNullFont;
}

int wxSystemSettings::GetSystemMetric( int index )
{
    switch (index)
    {
        case wxSYS_SCREEN_X:   return gdk_screen_width();
        case wxSYS_SCREEN_Y:   return gdk_screen_height();
        case wxSYS_HSCROLL_Y:  return 15;
        case wxSYS_VSCROLL_X:  return 15;
    }

    wxCHECK_MSG( index, 0, wxT("wxSystemSettings::GetSystemMetric not fully implemented") );

    return 0;
}

bool wxSystemSettings::GetCapability(int index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME: 
            return FALSE; break;
        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
            return TRUE; break;
        default:
            return FALSE;
    }
}
