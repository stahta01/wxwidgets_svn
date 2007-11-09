///////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/win_gtk.cpp
// Purpose:     native GTK+ widget for wxWindow
// Author:      Paul Cornett
// Id:          $Id$
// Copyright:   (c) 2007 Paul Cornett
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/defs.h"
#include "wx/gtk/win_gtk.h"
#include "gtk/gtk.h"

/*
wxPizza is a custom GTK+ widget derived from GtkFixed.  A custom widget
is needed to adapt GTK+ to wxWidgets needs in 3 areas: scrolling, window
borders, and RTL.

For scrolling, the "set_scroll_adjustments" signal is implemented
to make wxPizza appear scrollable to GTK+, allowing it to be put in a
GtkScrolledWindow.  Child widget positions are adjusted for the scrolling
position in size_allocate.  A second same-size GdkWindow is placed behind
widget->window, to allow GDK to use a more efficient scrolling technique.

For borders, space is reserved in realize and size_allocate.
For non-scrolling wxWindows, a second GdkWindow is used for the
border; scrolling wxWindows draw their border on wxPizza's parent
(a GtkScrolledWindow) GdkWindow.  Border widths for sunken and raised
styles are taken from widget->style, so they will match the current theme.

For RTL, child widget positions are mirrored in size_allocate.
*/

static GtkWidgetClass* parent_class;

extern "C" {

struct wxPizzaClass
{
    GtkFixedClass parent;
    void (*set_scroll_adjustments)(GtkWidget*, GtkAdjustment*, GtkAdjustment*);
};

static void size_allocate(GtkWidget* widget, GtkAllocation* alloc)
{
    const bool is_resize =
        widget->allocation.width  != alloc->width ||
        widget->allocation.height != alloc->height;
    const bool is_move =
        widget->allocation.x != alloc->x ||
        widget->allocation.y != alloc->y;

    widget->allocation = *alloc;

    wxPizza* pizza = WX_PIZZA(widget);
    int border_x, border_y;
    pizza->get_border_widths(border_x, border_y);
    int w = alloc->width - 2 * border_x;
    if (w < 0) w = 0;

    if (GTK_WIDGET_REALIZED(widget) && (is_move || is_resize))
    {
        int h = alloc->height - 2 * border_y;
        if (h < 0) h = 0;

        if (pizza->m_is_scrollable)
        {
            // backing window is inside border
            gdk_window_move_resize(pizza->m_backing_window,
                alloc->x + border_x, alloc->y + border_y, w, h);
        }
        else
        {
            // border window (or only window if
            // no-scroll no-border) is full size
            GdkWindow* window = pizza->m_backing_window;
            if (window == NULL)
                window = widget->window;
            gdk_window_move_resize(
                window, alloc->x, alloc->y, alloc->width, alloc->height);
        }
        if (is_resize && pizza->m_backing_window)
        {
            // main window is inside border
            if (pizza->m_is_scrollable)
                gdk_window_resize(widget->window, w, h);
            else
                // need move as well as resize because border can change
                gdk_window_move_resize(widget->window, border_x, border_y, w, h);

            // wxWidgets turns off redraw-on-allocate by default,
            // so border area needs to be invalidated
            if (border_x > 1 || border_y > 1)
            {
                if (pizza->m_is_scrollable)
                    ; // invalidate does not seem to be needed in this case
                else
                    gdk_window_invalidate_rect(pizza->m_backing_window, NULL, false);
            }
        }
    }
    // adjust child positions
    for (const GList* list = pizza->m_fixed.children; list; list = list->next)
    {
        const GtkFixedChild* child = static_cast<GtkFixedChild*>(list->data);
        if (GTK_WIDGET_VISIBLE(child->widget))
        {
            GtkAllocation child_alloc;
            // note that child positions do not take border into
            // account, they need to be relative to widget->window,
            // which has already been adjusted
            child_alloc.x = child->x - pizza->m_scroll_x;
            child_alloc.y = child->y - pizza->m_scroll_y;
            GtkRequisition req;
            gtk_widget_get_child_requisition(child->widget, &req);
            child_alloc.width  = req.width;
            child_alloc.height = req.height;
            if (gtk_widget_get_direction(widget) == GTK_TEXT_DIR_RTL)
            {
                child_alloc.x = w - child_alloc.x - child_alloc.width;
            }
            gtk_widget_size_allocate(child->widget, &child_alloc);
        }
    }
}

static void realize(GtkWidget* widget)
{
    parent_class->realize(widget);

    wxPizza* pizza = WX_PIZZA(widget);
    // second window is created if there is a border, or wxWindow is scrollable
    if (pizza->m_border_style || pizza->m_is_scrollable)
    {
        GdkWindowAttr attr;
        attr.event_mask = pizza->m_is_scrollable ? 0 : GDK_EXPOSURE_MASK;
        attr.x = widget->allocation.x;
        attr.y = widget->allocation.y;
        attr.width = widget->allocation.width;
        attr.height = widget->allocation.height;
        int border_x, border_y;
        pizza->get_border_widths(border_x, border_y);
        int w = widget->allocation.width  - 2 * border_x;
        int h = widget->allocation.height - 2 * border_y;
        if (w < 0) w = 0;
        if (h < 0) h = 0;
        if (pizza->m_is_scrollable)
        {
            attr.x += border_x;
            attr.y += border_y;
            attr.width  = w;
            attr.height = h;
        }
        attr.wclass = GDK_INPUT_OUTPUT;
        attr.visual = gtk_widget_get_visual(widget);
        attr.colormap = gtk_widget_get_colormap(widget);
        attr.window_type = GDK_WINDOW_CHILD;

        pizza->m_backing_window = gdk_window_new(
            gdk_window_get_parent(widget->window),
            &attr,
            GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP);

        gdk_window_set_user_data(pizza->m_backing_window, widget);
        if (pizza->m_is_scrollable)
            gdk_window_reparent(widget->window, pizza->m_backing_window, 0, 0);
        else
            gdk_window_reparent(widget->window, pizza->m_backing_window, border_x, border_y);
        gdk_window_resize(widget->window, w, h);
        
        widget->style = gtk_style_attach (widget->style, pizza->m_backing_window);
        gtk_style_set_background (widget->style, pizza->m_backing_window, GTK_STATE_NORMAL);
    }
}

static void unrealize(GtkWidget* widget)
{
    parent_class->unrealize(widget);

    wxPizza* pizza = WX_PIZZA(widget);
    if (pizza->m_backing_window)
    {
        gdk_window_set_user_data(pizza->m_backing_window, NULL);
        gdk_window_destroy(pizza->m_backing_window);
        pizza->m_backing_window = NULL;
    }
}

static void map(GtkWidget* widget)
{
    parent_class->map(widget);

    wxPizza* pizza = WX_PIZZA(widget);
    if (pizza->m_backing_window)
        gdk_window_show(pizza->m_backing_window);
}

static void unmap(GtkWidget* widget)
{
    parent_class->unmap(widget);

    wxPizza* pizza = WX_PIZZA(widget);
    if (pizza->m_backing_window)
        gdk_window_hide(pizza->m_backing_window);
}

// not used, but needs to exist so gtk_widget_set_scroll_adjustments will work
static void set_scroll_adjustments(GtkWidget*, GtkAdjustment*, GtkAdjustment*)
{
}

// Marshaller needed for set_scroll_adjustments signal,
// generated with GLib-2.4.6 glib-genmarshal
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
static void
g_cclosure_user_marshal_VOID__OBJECT_OBJECT (GClosure     *closure,
                                             GValue       * /*return_value*/,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      /*invocation_hint*/,
                                             gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__OBJECT_OBJECT) (gpointer     data1,
                                                    gpointer     arg_1,
                                                    gpointer     arg_2,
                                                    gpointer     data2);
  register GMarshalFunc_VOID__OBJECT_OBJECT callback;
  register GCClosure *cc = (GCClosure*) closure;
  register gpointer data1, data2;

  g_return_if_fail (n_param_values == 3);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__OBJECT_OBJECT) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_object (param_values + 1),
            g_marshal_value_peek_object (param_values + 2),
            data2);
}

static void class_init(void* g_class, void*)
{
    GtkWidgetClass* widget_class = (GtkWidgetClass*)g_class;
    widget_class->size_allocate = size_allocate;
    widget_class->realize = realize;
    widget_class->unrealize = unrealize;
    widget_class->map = map;
    widget_class->unmap = unmap;
    wxPizzaClass* klass = (wxPizzaClass*)g_class;

    // needed to make widget appear scrollable to GTK+
    klass->set_scroll_adjustments = set_scroll_adjustments;
    widget_class->set_scroll_adjustments_signal =
        g_signal_new(
            "set_scroll_adjustments",
            G_TYPE_FROM_CLASS(g_class),
            G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(wxPizzaClass, set_scroll_adjustments),
            NULL, NULL,
            g_cclosure_user_marshal_VOID__OBJECT_OBJECT,
            G_TYPE_NONE, 2, GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT);

    parent_class = GTK_WIDGET_CLASS(g_type_class_peek_parent(g_class));
}

} // extern "C"

GType wxPizza::type()
{
    static GtkType type;
    if (type == 0)
    {
        const GTypeInfo info = {
            sizeof(wxPizzaClass),
            NULL, NULL,
            class_init,
            NULL, NULL,
            sizeof(wxPizza), 0,
            NULL, NULL
        };
        type = g_type_register_static(
            GTK_TYPE_FIXED, "wxPizza", &info, GTypeFlags(0));
    }
    return type;
}

GtkWidget* wxPizza::New(long windowStyle)
{
    GtkWidget* widget = GTK_WIDGET(g_object_new(type(), NULL));
    wxPizza* pizza = WX_PIZZA(widget);
    pizza->m_backing_window = NULL;
    pizza->m_scroll_x = 0;
    pizza->m_scroll_y = 0;
    pizza->m_is_scrollable = (windowStyle & (wxHSCROLL | wxVSCROLL)) != 0;
    pizza->m_border_style =
        int(windowStyle & (wxBORDER_SIMPLE | wxBORDER_RAISED | wxBORDER_SUNKEN));
    gtk_fixed_set_has_window(GTK_FIXED(widget), true);
    gtk_widget_add_events(widget,
        GDK_EXPOSURE_MASK |
        GDK_SCROLL_MASK |
        GDK_POINTER_MOTION_MASK |
        GDK_POINTER_MOTION_HINT_MASK |
        GDK_BUTTON_MOTION_MASK |
        GDK_BUTTON1_MOTION_MASK |
        GDK_BUTTON2_MOTION_MASK |
        GDK_BUTTON3_MOTION_MASK |
        GDK_BUTTON_PRESS_MASK |
        GDK_BUTTON_RELEASE_MASK |
        GDK_KEY_PRESS_MASK |
        GDK_KEY_RELEASE_MASK |
        GDK_ENTER_NOTIFY_MASK |
        GDK_LEAVE_NOTIFY_MASK |
        GDK_FOCUS_CHANGE_MASK);
    gtk_container_set_resize_mode(GTK_CONTAINER(widget), GTK_RESIZE_QUEUE);
    return widget;
}

// gtk_fixed_move does not check for a change before issuing a queue_resize,
// we need to avoid that to prevent endless sizing loops, so check first
void wxPizza::move(GtkWidget* widget, int x, int y)
{
    GtkFixed* fixed = &m_fixed;
    for (const GList* list = fixed->children; list; list = list->next)
    {
        const GtkFixedChild* child = static_cast<GtkFixedChild*>(list->data);
        if (child->widget == widget)
        {
            if (child->x != x || child->y != y)
                gtk_fixed_move(fixed, widget, x, y);
            break;
        }
    }
}

void wxPizza::scroll(int dx, int dy)
{
    GtkWidget* widget = GTK_WIDGET(this);
    if (gtk_widget_get_direction(widget) == GTK_TEXT_DIR_RTL)
        dx = -dx;
    m_scroll_x -= dx;
    m_scroll_y -= dy;
    if (widget->window)
        gdk_window_scroll(widget->window, dx, dy);
    const GList* list = m_fixed.children;
    if (list)
    {
        const GtkFixedChild* child = static_cast<GtkFixedChild*>(list->data);
        // queueing a resize on any child will update them all
        gtk_widget_queue_resize(child->widget);
    }
}

extern GtkWidget *GetEntryWidget();

void wxPizza::get_border_widths(int& x, int& y)
{
    x = y = 0;
    if (m_border_style & wxBORDER_SIMPLE)
        x = y = 1;
    else if (m_border_style)
    {
        GtkWidget *entry_widget = GetEntryWidget();
        if (entry_widget->style)
        {
            x = entry_widget->style->xthickness;
            y = entry_widget->style->ythickness;
        }
    }
}
