///////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/renderer.cpp
// Purpose:     implementation of wxRendererNative for wxGTK
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

#include "wx/renderer.h"

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/dcclient.h"
    #include "wx/settings.h"
    #include "wx/module.h"
#endif

#include "wx/gtk/dc.h"

#include <gtk/gtk.h>

// ----------------------------------------------------------------------------
// wxRendererGTK: our wxRendererNative implementation
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxRendererGTK : public wxDelegateRendererNative
{
public:
    // draw the header control button (used by wxListCtrl)
    virtual int  DrawHeaderButton(wxWindow *win,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int flags = 0,
                                  wxHeaderSortIconType sortArrow = wxHDR_SORT_ICON_NONE,
                                  wxHeaderButtonParams* params = NULL);

    // draw the expanded/collapsed icon for a tree control item
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

    virtual void DrawDropArrow(wxWindow *win,
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
                                       int flags = 0);

    virtual void DrawFocusRect(wxWindow* win, wxDC& dc, const wxRect& rect, int flags = 0);

    virtual wxSplitterRenderParams GetSplitterParams(const wxWindow *win);

    class Module;
    friend class Module;

private:
    // used by DrawPushButton and DrawDropArrow
    static GtkWidget *GetButtonWidget();

    // used by DrawTreeItemButton()
    static GtkWidget *GetTreeWidget();

    // used by DrawCheckBox()
    static GtkWidget *GetCheckButtonWidget();

    // Used by DrawHeaderButton
    static GtkWidget *GetHeaderButtonWidget();

    static GtkWidget* GetSplitterWidget();

    // container for created widgets
    static GtkContainer* GetContainer();
    static GtkWidget* ms_container;
};

// Module for destroying created widgets
class wxRendererGTK::Module: public wxModule
{
public:
    virtual bool OnInit()
    {
        return true;
    }
    virtual void OnExit()
    {
        if (wxRendererGTK::ms_container)
        {
            GtkWidget* parent =
                gtk_widget_get_parent(wxRendererGTK::ms_container);
            gtk_widget_destroy(parent);
        }
    }
    DECLARE_DYNAMIC_CLASS(wxRendererGTK::Module)
};
IMPLEMENT_DYNAMIC_CLASS(wxRendererGTK::Module, wxModule)

// ============================================================================
// implementation
// ============================================================================

GtkWidget* wxRendererGTK::ms_container;

/* static */
wxRendererNative& wxRendererNative::GetDefault()
{
    static wxRendererGTK s_rendererGTK;

    return s_rendererGTK;
}

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

GtkContainer* wxRendererGTK::GetContainer()
{
    if (ms_container == NULL)
    {
        GtkWidget* window = gtk_window_new(GTK_WINDOW_POPUP);
        ms_container = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(window), ms_container);
    }
    return GTK_CONTAINER(ms_container);
}

GtkWidget *
wxRendererGTK::GetButtonWidget()
{
    static GtkWidget *s_button = NULL;

    if ( !s_button )
    {
        s_button = gtk_button_new();
        gtk_container_add(GetContainer(), s_button);
        gtk_widget_realize( s_button );
    }

    return s_button;
}

GtkWidget *
wxRendererGTK::GetCheckButtonWidget()
{
    static GtkWidget *s_button = NULL;

    if ( !s_button )
    {
        s_button = gtk_check_button_new();
        gtk_container_add(GetContainer(), s_button);
        gtk_widget_realize( s_button );
    }

    return s_button;
}

GtkWidget *
wxRendererGTK::GetTreeWidget()
{
    static GtkWidget *s_tree = NULL;

    if ( !s_tree )
    {
        s_tree = gtk_tree_view_new();
        gtk_container_add(GetContainer(), s_tree);
        gtk_widget_realize( s_tree );
    }

    return s_tree;
}

// used elsewhere
GtkWidget *GetEntryWidget()
{
    static GtkWidget *s_entry = NULL;
    static GtkWidget *s_window = NULL;

    if ( !s_entry )
    {
        s_window = gtk_window_new( GTK_WINDOW_POPUP );
        gtk_widget_realize( s_window );
        s_entry = gtk_entry_new();
        gtk_container_add( GTK_CONTAINER(s_window), s_entry );
        gtk_widget_realize( s_entry );
    }

    return s_entry;
}

// This one just gets the button used by the column header.  Although it's
// still a gtk_button the themes will typically differentiate and draw them
// differently if the button is in a treeview.
GtkWidget *
wxRendererGTK::GetHeaderButtonWidget()
{
    static GtkWidget *s_button = NULL;

    if ( !s_button )
    {
        // Get the dummy tree widget, give it a column, and then use the
        // widget in the column header for the rendering code.
        GtkWidget* treewidget = GetTreeWidget();
        GtkTreeViewColumn*  column = gtk_tree_view_column_new();
        gtk_tree_view_append_column(GTK_TREE_VIEW(treewidget), column);
        s_button = column->button;
    }

    return s_button;
}

GtkWidget* wxRendererGTK::GetSplitterWidget()
{
    static GtkWidget* widget;
    if (widget == NULL)
    {
        widget = gtk_vpaned_new();
        gtk_container_add(GetContainer(), widget);
        gtk_widget_realize(widget);
    }
    return widget;
}

// ----------------------------------------------------------------------------
// list/tree controls drawing
// ----------------------------------------------------------------------------

int
wxRendererGTK::DrawHeaderButton(wxWindow *win,
                                wxDC& dc,
                                const wxRect& rect,
                                int flags,
                                wxHeaderSortIconType sortArrow,
                                wxHeaderButtonParams* params)
{

    GtkWidget *button = GetHeaderButtonWidget();

    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    int x_diff = 0;
    if (win->GetLayoutDirection() == wxLayout_RightToLeft)
        x_diff = rect.width;

    GtkStateType state = GTK_STATE_NORMAL;
    if (flags & wxCONTROL_DISABLED)
        state = GTK_STATE_INSENSITIVE;
    else
    {
        if (flags & wxCONTROL_CURRENT)
            state = GTK_STATE_PRELIGHT;
    }

    gtk_paint_box
    (
        button->style,
        gdk_window,
        state,
        GTK_SHADOW_OUT,
        NULL,
        button,
        "button",
        dc.LogicalToDeviceX(rect.x) - x_diff, rect.y, rect.width, rect.height
    );

    return DrawHeaderButtonContents(win, dc, rect, flags, sortArrow, params);
}

// draw a ">" or "v" button
void
wxRendererGTK::DrawTreeItemButton(wxWindow* win,
                                  wxDC& dc, const wxRect& rect, int flags)
{
    GtkWidget *tree = GetTreeWidget();

    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    GtkStateType state;
    if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;

    int x_diff = 0;
    if (win->GetLayoutDirection() == wxLayout_RightToLeft)
        x_diff = rect.width;

    // VZ: I don't know how to get the size of the expander so as to centre it
    //     in the given rectangle, +2/3 below is just what looks good here...
    gtk_paint_expander
    (
        tree->style,
        gdk_window,
        state,
        NULL,
        tree,
        "treeview",
        dc.LogicalToDeviceX(rect.x) + 6 - x_diff,
        dc.LogicalToDeviceY(rect.y) + 3,
        flags & wxCONTROL_EXPANDED ? GTK_EXPANDER_EXPANDED
                                   : GTK_EXPANDER_COLLAPSED
    );
}


// ----------------------------------------------------------------------------
// splitter sash drawing
// ----------------------------------------------------------------------------

static int GetGtkSplitterFullSize(GtkWidget* widget)
{
    gint handle_size;
    gtk_widget_style_get(widget, "handle_size", &handle_size, NULL);

    return handle_size;
}

wxSplitterRenderParams
wxRendererGTK::GetSplitterParams(const wxWindow *WXUNUSED(win))
{
    // we don't draw any border, hence 0 for the second field
    return wxSplitterRenderParams
           (
               GetGtkSplitterFullSize(GetSplitterWidget()),
               0,
               true     // hot sensitive
           );
}

void
wxRendererGTK::DrawSplitterBorder(wxWindow * WXUNUSED(win),
                                  wxDC& WXUNUSED(dc),
                                  const wxRect& WXUNUSED(rect),
                                  int WXUNUSED(flags))
{
    // nothing to do
}

void
wxRendererGTK::DrawSplitterSash(wxWindow *win,
                                wxDC& dc,
                                const wxSize& size,
                                wxCoord position,
                                wxOrientation orient,
                                int flags)
{
    if ( !win->m_wxwindow->window )
    {
        // window not realized yet
        return;
    }

    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    wxCoord full_size = GetGtkSplitterFullSize(GetSplitterWidget());

    // are we drawing vertical or horizontal splitter?
    const bool isVert = orient == wxVERTICAL;

    GdkRectangle rect;

    if ( isVert )
    {
        rect.x = position;
        rect.y = 0;
        rect.width = full_size;
        rect.height = size.y;
    }
    else // horz
    {
        rect.x = 0;
        rect.y = position;
        rect.height = full_size;
        rect.width = size.x;
    }

    int x_diff = 0;
    if (win->GetLayoutDirection() == wxLayout_RightToLeft)
        x_diff = rect.width;

    gtk_paint_handle
    (
        win->m_wxwindow->style,
        gdk_window,
        flags & wxCONTROL_CURRENT ? GTK_STATE_PRELIGHT : GTK_STATE_NORMAL,
        GTK_SHADOW_NONE,
        NULL /* no clipping */,
        win->m_wxwindow,
        "paned",
        dc.LogicalToDeviceX(rect.x) - x_diff,
        dc.LogicalToDeviceY(rect.y),
        rect.width,
        rect.height,
        isVert ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL
    );
}

void
wxRendererGTK::DrawDropArrow(wxWindow *WXUNUSED(win),
                             wxDC& dc,
                             const wxRect& rect,
                             int flags)
{
    GtkWidget *button = GetButtonWidget();

    // If we give GTK_PIZZA(win->m_wxwindow)->bin_window as
    // a window for gtk_paint_xxx function, then it won't
    // work for wxMemoryDC. So that is why we assume wxDC
    // is wxWindowDC (wxClientDC, wxMemoryDC and wxPaintDC
    // are derived from it) and use its m_window.
    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    // draw arrow so that there is even space horizontally
    // on both sides
    int arrowX = rect.width/4 + 1;
    int arrowWidth = rect.width - (arrowX*2);

    // scale arrow's height accoording to the width
    int arrowHeight = rect.width/3;
    int arrowY = (rect.height-arrowHeight)/2 +
                 ((rect.height-arrowHeight) & 1);

    GtkStateType state;

    if ( flags & wxCONTROL_PRESSED )
        state = GTK_STATE_ACTIVE;
    else if ( flags & wxCONTROL_DISABLED )
        state = GTK_STATE_INSENSITIVE;
    else if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;

    // draw arrow on button
    gtk_paint_arrow
    (
        button->style,
        gdk_window,
        state,
        flags & wxCONTROL_PRESSED ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
        NULL,
        button,
        "arrow",
        GTK_ARROW_DOWN,
        FALSE,
        rect.x + arrowX,
        rect.y + arrowY,
        arrowWidth,
        arrowHeight
    );
}

void
wxRendererGTK::DrawComboBoxDropButton(wxWindow *win,
                                      wxDC& dc,
                                      const wxRect& rect,
                                      int flags)
{
    DrawPushButton(win,dc,rect,flags);
    DrawDropArrow(win,dc,rect);
}

void
wxRendererGTK::DrawCheckBox(wxWindow *WXUNUSED(win),
                            wxDC& dc,
                            const wxRect& rect,
                            int flags )
{
    GtkWidget *button = GetCheckButtonWidget();

    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    GtkStateType state;

    if ( flags & wxCONTROL_PRESSED )
        state = GTK_STATE_ACTIVE;
    else if ( flags & wxCONTROL_DISABLED )
        state = GTK_STATE_INSENSITIVE;
    else if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;

    gtk_paint_check
    (
        button->style,
        gdk_window,
        state,
        flags & wxCONTROL_CHECKED ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
        NULL,
        button,
        "cellcheck",
        dc.LogicalToDeviceX(rect.x)+2,
        dc.LogicalToDeviceY(rect.y)+3,
        13, 13
    );
}

void
wxRendererGTK::DrawPushButton(wxWindow *WXUNUSED(win),
                              wxDC& dc,
                              const wxRect& rect,
                              int flags)
{
    GtkWidget *button = GetButtonWidget();

    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    // draw button
    GtkStateType state;

    if ( flags & wxCONTROL_PRESSED )
        state = GTK_STATE_ACTIVE;
    else if ( flags & wxCONTROL_DISABLED )
        state = GTK_STATE_INSENSITIVE;
    else if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;

    gtk_paint_box
    (
        button->style,
        gdk_window,
        state,
        flags & wxCONTROL_PRESSED ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
        NULL,
        button,
        "button",
        rect.x, rect.y, rect.width, rect.height
    );
}

void
wxRendererGTK::DrawItemSelectionRect(wxWindow *win,
                                     wxDC& dc,
                                     const wxRect& rect,
                                     int flags )
{
    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    int x_diff = 0;
    if (win->GetLayoutDirection() == wxLayout_RightToLeft)
        x_diff = rect.width;

    GtkStateType state;
    if (flags & wxCONTROL_SELECTED)
    {
        // the wxCONTROL_FOCUSED state is deduced
        // directly from the m_wxwindow by GTK+
        state = GTK_STATE_SELECTED;

        gtk_paint_flat_box( win->m_widget->style,
                        gdk_window,
                        state,
                        GTK_SHADOW_NONE,
                        NULL,
                        win->m_wxwindow,
                        "cell_even",
                        dc.LogicalToDeviceX(rect.x) - x_diff,
                        dc.LogicalToDeviceY(rect.y),
                        rect.width,
                        rect.height );
    }
    else // !wxCONTROL_SELECTED
    {
        state = GTK_STATE_NORMAL;
    }

    if ((flags & wxCONTROL_CURRENT) && (flags & wxCONTROL_FOCUSED))
    {
        gtk_paint_focus( win->m_widget->style,
                         gdk_window,
                         state,
                         NULL,
                         win->m_wxwindow,
                         "treeview",
                         dc.LogicalToDeviceX(rect.x),
                         dc.LogicalToDeviceY(rect.y),
                         rect.width,
                         rect.height );
    }
}

void wxRendererGTK::DrawFocusRect(wxWindow* win, wxDC& dc, const wxRect& rect, int flags)
{
    GdkWindow* gdk_window = NULL;
#if wxUSE_NEW_DC
    wxDCImpl *impl = dc.GetImpl();
    wxGTKDCImpl *gtk_impl = wxDynamicCast( impl, wxGTKDCImpl );
    if (gtk_impl)
        gdk_window = gtk_impl->GetGDKWindow();
#else
    gdk_window = dc.GetGDKWindow();
#endif
    wxASSERT_MSG( gdk_window,
                  wxT("cannot use wxRendererNative on wxDC of this type") );

    GtkStateType state;
    if (flags & wxCONTROL_SELECTED)
        state = GTK_STATE_SELECTED;
    else
        state = GTK_STATE_NORMAL;

    gtk_paint_focus( win->m_widget->style,
                     gdk_window,
                     state,
                     NULL,
                     win->m_wxwindow,
                     NULL,
                     dc.LogicalToDeviceX(rect.x),
                     dc.LogicalToDeviceY(rect.y),
                     rect.width,
                     rect.height );
}
