/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/settings.cpp
// Purpose:
// Author:      Robert Roebling
// Modified by: Mart Raudsepp (GetMetric)
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/settings.h"

#ifndef WX_PRECOMP
    #include "wx/cmndata.h"
    #include "wx/toplevel.h"
#endif

#include "wx/fontutil.h"

#include <gtk/gtkversion.h>
#if GTK_CHECK_VERSION(2, 9, 0)
    // gtk_object_sink
    #undef GTK_DISABLE_DEPRECATED
#endif
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include <X11/Xatom.h>

// ----------------------------------------------------------------------------
// wxSystemObjects
// ----------------------------------------------------------------------------

struct wxSystemObjects
{
    wxColour m_colBtnFace,
             m_colBtnShadow,
             m_colBtnHighlight,
             m_colHighlight,
             m_colHighlightText,
             m_colListBox,
             m_colWindow,
             m_colWindowText,
             m_colBtnText,
             m_colMenuItemHighlight,
             m_colTooltip,
             m_colTooltipText;

    wxFont m_fontSystem;
};

static wxSystemObjects gs_objects;

void wxClearGtkSystemObjects()
{
    gs_objects.m_colBtnFace = wxColour();
    gs_objects.m_colBtnShadow = wxColour();
    gs_objects.m_colBtnHighlight = wxColour();
    gs_objects.m_colHighlightText = wxColour();
    gs_objects.m_colListBox = wxColour();
    gs_objects.m_colWindow = wxColour();
    gs_objects.m_colWindowText = wxColour();
    gs_objects.m_colBtnText = wxColour();
    gs_objects.m_colMenuItemHighlight = wxColour();
    gs_objects.m_colTooltip = wxColour();
    gs_objects.m_colTooltipText = wxColour();
    gs_objects.m_fontSystem = wxNullFont;
}

// ----------------------------------------------------------------------------
// wxSystemSettings implementation
// ----------------------------------------------------------------------------

// kind of widget to use in GetColourFromGTKWidget
enum wxGtkWidgetType
{
    wxGTK_BUTTON,
    wxGTK_LIST,
    wxGTK_MENUITEM,
    wxGTK_TEXTCTRL
};

// the colour we need
enum wxGtkColourType
{
    wxGTK_FG,
    wxGTK_BG,
    wxGTK_BASE
};

// wxSystemSettings::GetColour() helper: get the colours from a GTK+
// widget style, return true if we did get them
static bool GetColourFromGTKWidget(GdkColor& gdkColor,
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

        case wxGTK_TEXTCTRL:
            widget = gtk_text_view_new();
            break;

        case wxGTK_LIST:
            widget = gtk_tree_view_new_with_model(
                (GtkTreeModel*)gtk_list_store_new(1, G_TYPE_INT));
            break;

        case wxGTK_MENUITEM:
            widget = gtk_menu_item_new();
    }

    GtkStyle *def = gtk_rc_get_style( widget );
    if ( !def )
        def = gtk_widget_get_default_style();

    const bool ok = def != NULL;
    if (ok)
    {
        switch ( colour )
        {
            default:
                wxFAIL_MSG( _T("unexpected GTK colour type") );
                // fall through

            case wxGTK_FG:
                gdkColor = def->fg[state];
                break;

            case wxGTK_BG:
                gdkColor = def->bg[state];
                break;

            case wxGTK_BASE:
                gdkColor = def->base[state];
                break;
        }
    }

    gtk_object_sink((GtkObject*)widget);

    return ok;
}

static void GetTooltipColors()
{
    GtkWidget* widget = gtk_window_new(GTK_WINDOW_POPUP);
    const char* name = "gtk-tooltip";
    if (gtk_check_version(2, 11, 0))
        name = "gtk-tooltips";
    gtk_widget_set_name(widget, name);
    gtk_widget_ensure_style(widget);

    GdkColor c = widget->style->bg[GTK_STATE_NORMAL];
    gs_objects.m_colTooltip = wxColor(c);
    c = widget->style->fg[GTK_STATE_NORMAL];
    gs_objects.m_colTooltipText = wxColor(c);

    gtk_widget_destroy(widget);
}

wxColour wxSystemSettingsNative::GetColour( wxSystemColour index )
{
    wxColor color;
    GdkColor gdkColor;
    switch (index)
    {
        case wxSYS_COLOUR_SCROLLBAR:
        case wxSYS_COLOUR_BACKGROUND:
        case wxSYS_COLOUR_INACTIVECAPTION:
        case wxSYS_COLOUR_MENU:
        case wxSYS_COLOUR_WINDOWFRAME:
        case wxSYS_COLOUR_ACTIVEBORDER:
        case wxSYS_COLOUR_INACTIVEBORDER:
        case wxSYS_COLOUR_BTNFACE:
        case wxSYS_COLOUR_MENUBAR:
        case wxSYS_COLOUR_3DLIGHT:
            if (!gs_objects.m_colBtnFace.Ok())
            {
                gdkColor.red =
                gdkColor.green = 0;
                gdkColor.blue = 0x9c40;
                GetColourFromGTKWidget(gdkColor);
                gs_objects.m_colBtnFace = wxColor(gdkColor);
            }
            color = gs_objects.m_colBtnFace;
            break;

        case wxSYS_COLOUR_WINDOW:
            if (!gs_objects.m_colWindow.Ok())
            {
                gdkColor.red =
                gdkColor.green =
                gdkColor.blue = 0xFFFF;
                GetColourFromGTKWidget(gdkColor, wxGTK_TEXTCTRL, GTK_STATE_NORMAL, wxGTK_BASE);
                gs_objects.m_colWindow = wxColor(gdkColor);
            }
            color = gs_objects.m_colWindow;
            break;

        case wxSYS_COLOUR_3DDKSHADOW:
            color = *wxBLACK;
            break;

        case wxSYS_COLOUR_GRAYTEXT:
        case wxSYS_COLOUR_BTNSHADOW:
        //case wxSYS_COLOUR_3DSHADOW:
            if (!gs_objects.m_colBtnShadow.Ok())
            {
                wxColour faceColour(GetColour(wxSYS_COLOUR_3DFACE));
                gs_objects.m_colBtnShadow =
                   wxColour((unsigned char) (faceColour.Red() * 2 / 3),
                            (unsigned char) (faceColour.Green() * 2 / 3),
                            (unsigned char) (faceColour.Blue() * 2 / 3));
            }
            color = gs_objects.m_colBtnShadow;
            break;

        case wxSYS_COLOUR_3DHIGHLIGHT:
        //case wxSYS_COLOUR_BTNHIGHLIGHT:
            color = *wxWHITE;
            break;

        case wxSYS_COLOUR_HIGHLIGHT:
            if (!gs_objects.m_colHighlight.Ok())
            {
                gdkColor.red =
                gdkColor.green = 0;
                gdkColor.blue = 0x9c40;
                GetColourFromGTKWidget(
                    gdkColor, wxGTK_BUTTON, GTK_STATE_SELECTED);
                gs_objects.m_colHighlight = wxColour(gdkColor);
            }
            color = gs_objects.m_colHighlight;
            break;

        case wxSYS_COLOUR_LISTBOX:
            if (!gs_objects.m_colListBox.Ok())
            {
                if ( GetColourFromGTKWidget(gdkColor,
                                            wxGTK_LIST,
                                            GTK_STATE_NORMAL,
                                            wxGTK_BASE) )
                {
                    gs_objects.m_colListBox = wxColour(gdkColor);
                }
                else
                {
                    gs_objects.m_colListBox = *wxWHITE;
                }
            }
            color = gs_objects.m_colListBox;
            break;

        case wxSYS_COLOUR_MENUTEXT:
        case wxSYS_COLOUR_WINDOWTEXT:
        case wxSYS_COLOUR_CAPTIONTEXT:
        case wxSYS_COLOUR_INACTIVECAPTIONTEXT:
        case wxSYS_COLOUR_BTNTEXT:
            if (!gs_objects.m_colBtnText.Ok())
            {
                gdkColor.red =
                gdkColor.green =
                gdkColor.blue = 0;
                GetColourFromGTKWidget(
                    gdkColor, wxGTK_BUTTON, GTK_STATE_NORMAL, wxGTK_FG);
                gs_objects.m_colBtnText = wxColour(gdkColor);
            }
            color = gs_objects.m_colBtnText;
            break;

        case wxSYS_COLOUR_INFOBK:
            if (!gs_objects.m_colTooltip.Ok()) {
                GetTooltipColors();
            }
            color = gs_objects.m_colTooltip;
            break;

        case wxSYS_COLOUR_INFOTEXT:
            if (!gs_objects.m_colTooltipText.Ok()) {
                GetTooltipColors();
            }
            color = gs_objects.m_colTooltipText;
            break;

        case wxSYS_COLOUR_HIGHLIGHTTEXT:
            if (!gs_objects.m_colHighlightText.Ok())
            {
                wxColour hclr = GetColour(wxSYS_COLOUR_HIGHLIGHT);
                if (hclr.Red() > 200 && hclr.Green() > 200 && hclr.Blue() > 200)
                    gs_objects.m_colHighlightText = *wxBLACK;
                else
                    gs_objects.m_colHighlightText = *wxWHITE;
            }
            color = gs_objects.m_colHighlightText;
            break;

        case wxSYS_COLOUR_APPWORKSPACE:
            color = *wxWHITE;    // ?
            break;

        case wxSYS_COLOUR_ACTIVECAPTION:
        case wxSYS_COLOUR_MENUHILIGHT:
            if (!gs_objects.m_colMenuItemHighlight.Ok())
            {
                gdkColor.red =
                gdkColor.green =
                gdkColor.blue = 0;
                GetColourFromGTKWidget(
                    gdkColor, wxGTK_MENUITEM, GTK_STATE_SELECTED, wxGTK_BG);
                gs_objects.m_colMenuItemHighlight = wxColour(gdkColor);
            }
            color = gs_objects.m_colMenuItemHighlight;
            break;

        case wxSYS_COLOUR_HOTLIGHT:
        case wxSYS_COLOUR_GRADIENTACTIVECAPTION:
        case wxSYS_COLOUR_GRADIENTINACTIVECAPTION:
            // TODO
            color = *wxBLACK;
            break;

        case wxSYS_COLOUR_MAX:
        default:
            wxFAIL_MSG( _T("unknown system colour index") );
            color = *wxWHITE;
            break;
    }

    return color;
}

wxFont wxSystemSettingsNative::GetFont( wxSystemFont index )
{
    wxFont font;
    switch (index)
    {
        case wxSYS_OEM_FIXED_FONT:
        case wxSYS_ANSI_FIXED_FONT:
        case wxSYS_SYSTEM_FIXED_FONT:
            font = *wxNORMAL_FONT;
            break;

        case wxSYS_ANSI_VAR_FONT:
        case wxSYS_SYSTEM_FONT:
        case wxSYS_DEVICE_DEFAULT_FONT:
        case wxSYS_DEFAULT_GUI_FONT:
            if (!gs_objects.m_fontSystem.Ok())
            {
                GtkWidget *widget = gtk_button_new();
                GtkStyle *def = gtk_rc_get_style( widget );
                if ( !def || !def->font_desc )
                    def = gtk_widget_get_default_style();
                if ( def && def->font_desc )
                {
                    wxNativeFontInfo info;
                    info.description =
                        pango_font_description_copy(def->font_desc);
                    gs_objects.m_fontSystem = wxFont(info);
                }
                else
                {
                    GtkSettings *settings = gtk_settings_get_default();
                    gchar *font_name = NULL;
                    g_object_get ( settings,
                                   "gtk-font-name",
                                   &font_name,
                                   NULL);
                    if (!font_name)
                        gs_objects.m_fontSystem = wxFont( 12, wxSWISS, wxNORMAL, wxNORMAL );
                    else
                        gs_objects.m_fontSystem = wxFont(wxString::FromAscii(font_name));
                    g_free (font_name);
                }
                gtk_object_sink((GtkObject*)widget);
            }
            font = gs_objects.m_fontSystem;
            break;

        default:
            break;
    }
    return font;
}

static bool GetFrameExtents(GdkWindow* window, int* left, int* right, int* top, int* bottom)
{
    bool success = false;
    Atom property = 0;
    if (gdk_x11_screen_supports_net_wm_hint(
            gdk_drawable_get_screen(window),
            gdk_atom_intern("_NET_FRAME_EXTENTS", false)))
    {
        success = true;
        property = gdk_x11_get_xatom_by_name_for_display(
            gdk_drawable_get_display(window),
            "_NET_FRAME_EXTENTS");
    }

    if (success)
    {
        Atom type;
        int format;
        gulong nitems, bytes_after;
        guchar* data;
        success = XGetWindowProperty(
            gdk_x11_drawable_get_xdisplay(window),
            gdk_x11_drawable_get_xid(window),
            property,
            0, 4,
            false,
            XA_CARDINAL,
            &type, &format, &nitems, &bytes_after, &data
            ) == Success;
        if (success)
        {
            success = data && nitems == 4;
            if (success)
            {
                long* p = (long*)data;
                if (left)   *left   = int(p[0]);
                if (right)  *right  = int(p[1]);
                if (top)    *top    = int(p[2]);
                if (bottom) *bottom = int(p[3]);
            }
            if (data)
                XFree(data);
        }
    }
    return success;
}

// helper: return the GtkSettings either for the screen the current window is
// on or for the default screen if window is NULL
static GtkSettings *GetSettingsForWindowScreen(GdkWindow *window)
{
    return window ? gtk_settings_get_for_screen(gdk_drawable_get_screen(window))
                  : gtk_settings_get_default();
}

int wxSystemSettingsNative::GetMetric( wxSystemMetric index, wxWindow* win )
{
    GdkWindow *window = NULL;
    if(win && GTK_WIDGET_REALIZED(win->GetHandle()))
        window = win->GetHandle()->window;

    switch (index)
    {
        case wxSYS_BORDER_X:
        case wxSYS_BORDER_Y:
        case wxSYS_EDGE_X:
        case wxSYS_EDGE_Y:
        case wxSYS_FRAMESIZE_X:
        case wxSYS_FRAMESIZE_Y:
            // If a window is specified/realized, and it is a toplevel window, we can query from wm.
            // The returned border thickness is outside the client area in that case.
            if (window)
            {
                wxTopLevelWindow *tlw = wxDynamicCast(win, wxTopLevelWindow);
                if (!tlw)
                    return -1; // not a tlw, not sure how to approach
                else
                {
                    // Get the frame extents from the windowmanager.
                    // In most cases the top extent is the titlebar, so we use the bottom extent
                    // for the heights.
                    int right, bottom;
                    if (GetFrameExtents(window, NULL, &right, NULL, &bottom))
                    {
                        switch (index)
                        {
                            case wxSYS_BORDER_X:
                            case wxSYS_EDGE_X:
                            case wxSYS_FRAMESIZE_X:
                                return right; // width of right extent
                            default:
                                return bottom; // height of bottom extent
                        }
                    }
                }
            }

            return -1; // no window specified

        case wxSYS_CURSOR_X:
        case wxSYS_CURSOR_Y:
                return gdk_display_get_default_cursor_size(
                            window ? gdk_drawable_get_display(window)
                                   : gdk_display_get_default());

        case wxSYS_DCLICK_X:
        case wxSYS_DCLICK_Y:
            gint dclick_distance;
            g_object_get(GetSettingsForWindowScreen(window),
                            "gtk-double-click-distance", &dclick_distance, NULL);

            return dclick_distance * 2;

        case wxSYS_DCLICK_MSEC:
            gint dclick;
            g_object_get(GetSettingsForWindowScreen(window),
                            "gtk-double-click-time", &dclick, NULL);
            return dclick;

        case wxSYS_DRAG_X:
        case wxSYS_DRAG_Y:
            gint drag_threshold;
            g_object_get(GetSettingsForWindowScreen(window),
                            "gtk-dnd-drag-threshold", &drag_threshold, NULL);

            // The correct thing here would be to double the value
            // since that is what the API wants. But the values
            // are much bigger under GNOME than under Windows and
            // just seem to much in many cases to be useful.
            // drag_threshold *= 2;

            return drag_threshold;

        case wxSYS_ICON_X:
        case wxSYS_ICON_Y:
            return 32;

        case wxSYS_SCREEN_X:
            if (window)
                return gdk_screen_get_width(gdk_drawable_get_screen(window));
            else
                return gdk_screen_width();

        case wxSYS_SCREEN_Y:
            if (window)
                return gdk_screen_get_height(gdk_drawable_get_screen(window));
            else
                return gdk_screen_height();

        case wxSYS_HSCROLL_Y:
        case wxSYS_VSCROLL_X:
            return 15;

        case wxSYS_CAPTION_Y:
            if (!window)
                // No realized window specified, and no implementation for that case yet.
                return -1;

            wxASSERT_MSG( wxDynamicCast(win, wxTopLevelWindow),
                          wxT("Asking for caption height of a non toplevel window") );

            // Get the height of the top windowmanager border.
            // This is the titlebar in most cases. The titlebar might be elsewhere, and
            // we could check which is the thickest wm border to decide on which side the
            // titlebar is, but this might lead to interesting behaviours in used code.
            // Reconsider when we have a way to report to the user on which side it is.
            {
                int top;
                if (GetFrameExtents(window, NULL, NULL, &top, NULL))
                {
                    return top; // top frame extent
                }
            }

            // Try a default approach without a window pointer, if possible
            // ...

            return -1;

        case wxSYS_PENWINDOWS_PRESENT:
            // No MS Windows for Pen computing extension available in X11 based gtk+.
            return 0;

        default:
            return -1;   // metric is unknown
    }
}

bool wxSystemSettingsNative::HasFeature(wxSystemFeature index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME:
            return false;

        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
            return true;

        default:
            return false;
    }
}
