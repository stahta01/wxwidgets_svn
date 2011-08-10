///////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/win_gtk.cpp
// Purpose:     native GTK+ widget for wxWindow
// Author:      Paul Cornett
// Id:          $Id$
// Copyright:   (c) 2007 Paul Cornett
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/defs.h"
#include "wx/gtk/private.h"
#include "wx/gtk/private/win_gtk.h"

/*
wxPizza is a custom GTK+ widget derived from GtkFixed.  A custom widget
is needed to adapt GTK+ to wxWidgets needs in 3 areas: scrolling, window
borders, and RTL.

For scrolling, the "set_scroll_adjustments" signal is implemented
to make wxPizza appear scrollable to GTK+, allowing it to be put in a
GtkScrolledWindow.  Child widget positions are adjusted for the scrolling
position in size_allocate.

For borders, space is reserved in realize and size_allocate.  The border is
drawn on wxPizza's parent GdkWindow.

For RTL, child widget positions are mirrored in size_allocate.
*/

static GtkWidgetClass* parent_class;

extern "C" {


struct wxPizzaClass
{
    GtkFixedClass parent;
    void (*set_scroll_adjustments)(GtkWidget*, GtkAdjustment*, GtkAdjustment*);
};

#ifdef __WXGTK30__
enum {
    P_0,
    P_HADJUSTMENT,
    P_VADJUSTMENT
};

G_DEFINE_TYPE_WITH_CODE(wxPizza, wxpizza, GTK_TYPE_FIXED, 
        G_IMPLEMENT_INTERFACE(GTK_TYPE_SCROLLABLE, NULL));
#endif

static void size_allocate(GtkWidget* widget, GtkAllocation* alloc)
{
    GtkAllocation allocation;

    gtk_widget_get_allocation(widget, &allocation);

    const bool is_resize =
        allocation.width  != alloc->width ||
        allocation.height != alloc->height;
    const bool is_move =
        allocation.x != alloc->x ||
        allocation.y != alloc->y;

    wxPizza* pizza = WX_PIZZA(widget);
    int border_x, border_y;
    pizza->get_border_widths(border_x, border_y);
    int w = alloc->width - 2 * border_x;
    if (w < 0) w = 0;

    if (gtk_widget_get_realized(widget) && (is_move || is_resize))
    {
        int h = alloc->height - 2 * border_y;
        if (h < 0) h = 0;

        gdk_window_move_resize(gtk_widget_get_window(widget),
            alloc->x + border_x, alloc->y + border_y, w, h);

        if (is_resize && (border_x || border_y))
        {
            // old and new border areas need to be invalidated,
            // otherwise they will not be erased/redrawn properly
            GdkWindow* parent = gtk_widget_get_parent_window(widget);
            gdk_window_invalidate_rect(parent, &allocation, false);
            gdk_window_invalidate_rect(parent, alloc, false);
        }
    }

    gtk_widget_set_allocation(widget, alloc);

    // adjust child positions
    GList* list = gtk_container_get_children(GTK_CONTAINER(pizza));
    for (const GList* p = list; p; p = p->next)
    {
        GtkWidget* child = GTK_WIDGET(p->data);
        if (gtk_widget_get_visible(child))
        {
            GValue value = { 0 };
            g_value_init(&value, G_TYPE_INT);
            gtk_container_child_get_property(GTK_CONTAINER(pizza), child, "x", &value);
            const int x = g_value_get_int(&value);
            gtk_container_child_get_property(GTK_CONTAINER(pizza), child, "y", &value);
            const int y = g_value_get_int(&value);
            GtkAllocation child_alloc;
            // note that child positions do not take border into
            // account, they need to be relative to widget->window,
            // which has already been adjusted
            child_alloc.x = x - pizza->m_scroll_x;
            child_alloc.y = y - pizza->m_scroll_y;
            GtkRequisition req;
            gtk_widget_get_child_requisition(child, &req);
            child_alloc.width  = req.width;
            child_alloc.height = req.height;
            if (gtk_widget_get_direction(widget) == GTK_TEXT_DIR_RTL)
                child_alloc.x = w - child_alloc.x - child_alloc.width;
            gtk_widget_size_allocate(child, &child_alloc);
        }
    }
    g_list_free(list);
}

static void realize(GtkWidget* widget)
{
    parent_class->realize(widget);

    GtkAllocation allocation;
    GdkWindow*    win;

    gtk_widget_get_allocation(widget, &allocation);
    win = gtk_widget_get_window(widget);

    wxPizza* pizza = WX_PIZZA(widget);
    if (pizza->m_border_style)
    {
        int border_x, border_y;
        pizza->get_border_widths(border_x, border_y);
        int x = allocation.x + border_x;
        int y = allocation.y + border_y;
        int w = allocation.width  - 2 * border_x;
        int h = allocation.height - 2 * border_y;
        if (w < 0) w = 0;
        if (h < 0) h = 0;
        gdk_window_move_resize(win, x, y, w, h);
    }
}

static void show(GtkWidget* widget)
{
    GtkWidget* parent;
    GtkAllocation allocation;

    parent = gtk_widget_get_parent(widget);
    gtk_widget_get_allocation(widget, &allocation);

    if (parent && WX_PIZZA(widget)->m_border_style)
    {
        // invalidate whole allocation so borders will be drawn properly
        const GtkAllocation& a = allocation;
        gtk_widget_queue_draw_area(parent, a.x, a.y, a.width, a.height);
    }

    parent_class->show(widget);
}

static void hide(GtkWidget* widget)
{
    GtkWidget* parent;
    GtkAllocation allocation;

    parent = gtk_widget_get_parent(widget);
    gtk_widget_get_allocation(widget, &allocation);

    if (parent && WX_PIZZA(widget)->m_border_style)
    {
        // invalidate whole allocation so borders will be erased properly
        const GtkAllocation& a = allocation;
        gtk_widget_queue_draw_area(parent, a.x, a.y, a.width, a.height);
    }

    parent_class->hide(widget);
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

static void
wxpizza_set_property(GObject         *object,
               guint            prop_id,
               const GValue    *value,
               GParamSpec      *pspec)
{
  wxPizza *pizza = WX_PIZZA(object);

  switch (prop_id)
    {
    case P_HADJUSTMENT:
      // wxpizza_set_hadjustment (pizza, g_value_get_object (value));
      break;
    case P_VADJUSTMENT:
      // wxpizza_set_vadjustment (pizza, g_value_get_object (value));
      break;
    // case PROP_HSCROLL_POLICY:
    //   viewport->priv->hscroll_policy = g_value_get_enum (value);
    //   gtk_widget_queue_resize (GTK_WIDGET (viewport));
    //   break;
    // case PROP_VSCROLL_POLICY:
    //   viewport->priv->vscroll_policy = g_value_get_enum (value);
    //   gtk_widget_queue_resize (GTK_WIDGET (viewport));
    //   break;
    // case PROP_SHADOW_TYPE:
    //   gtk_viewport_set_shadow_type (viewport, g_value_get_enum (value));
    //   break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
wxpizza_get_property(GObject         *object,
               guint            prop_id,
               GValue          *value,
               GParamSpec      *pspec)
{
  wxPizza *pizza = WX_PIZZA(object);

  switch (prop_id)
    {
    case P_HADJUSTMENT:
      // g_value_set_object (value, priv->hadjustment);
      break;
    case P_VADJUSTMENT:
      // g_value_set_object (value, priv->vadjustment);
      break;
    // case PROP_HSCROLL_POLICY:
    //   g_value_set_enum (value, priv->hscroll_policy);
    //   break;
    // case PROP_VSCROLL_POLICY:
    //   g_value_set_enum (value, priv->vscroll_policy);
    //   break;
    // case PROP_SHADOW_TYPE:
    //   g_value_set_enum (value, priv->shadow_type);
    //   break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

#ifdef __WXGTK30__
static void wxpizza_class_init(wxPizzaClass* g_class)
#else
static void wxpizza_class_init(void* g_class, void*)
#endif
{
    GtkWidgetClass* widget_class = (GtkWidgetClass*)g_class;
    widget_class->size_allocate = size_allocate;
    widget_class->realize = realize;
    widget_class->show = show;
    widget_class->hide = hide;
    wxPizzaClass* klass = (wxPizzaClass*)g_class;

    // needed to make widget appear scrollable to GTK+
    klass->set_scroll_adjustments = set_scroll_adjustments;
#ifdef __WXGTK30__
    GObjectClass *gobject_class = G_OBJECT_CLASS(g_class);

    gobject_class->set_property = wxpizza_set_property;
    gobject_class->get_property = wxpizza_get_property;
    // Override properties
    g_object_class_override_property(gobject_class, P_HADJUSTMENT, "hadjustment");
    g_object_class_override_property(gobject_class, P_VADJUSTMENT, "vadjustment");
#else
    widget_class->set_scroll_adjustments_signal =
        g_signal_new(
            "set_scroll_adjustments",
            G_TYPE_FROM_CLASS(g_class),
            G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(wxPizzaClass, set_scroll_adjustments),
            NULL, NULL,
            g_cclosure_user_marshal_VOID__OBJECT_OBJECT,
            G_TYPE_NONE, 2, GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT);
#endif

    parent_class = GTK_WIDGET_CLASS(g_type_class_peek_parent(g_class));
}

#ifdef __WXGTK30__
static void wxpizza_init(wxPizza* self)
{
    self->m_scroll_x = 0;
    self->m_scroll_y = 0;
    // self->m_is_scrollable = (windowStyle & (wxHSCROLL | wxVSCROLL)) != 0;
    // mask off border styles not useable with wxPizza
    // self->m_border_style = int(windowStyle & BORDER_STYLES);
}
#endif

} // extern "C"

#if defined(__WXGTK20__) && !defined(__WXGTK30__)
GType wxPizza::type()
{
    // static GType type;
    static GtkType type;
    if (type == 0)
    {
        const GTypeInfo info = {
            sizeof(wxPizzaClass),
            NULL, NULL,
            wxpizza_class_init,
            NULL, NULL,
            sizeof(wxPizza), 0,
            NULL, NULL
        };
        type = g_type_register_static(
            GTK_TYPE_FIXED, "wxPizza", &info, GTypeFlags(0));
    }

    return type;
}
#endif

GtkWidget* wxPizza::New(long windowStyle)
{
#ifdef __WXGTK30__
    GtkWidget* widget = GTK_WIDGET(g_object_new(wxpizza_get_type(), NULL));
#else
    GtkWidget* widget = GTK_WIDGET(g_object_new(type(), NULL));
#endif
    wxPizza* pizza = WX_PIZZA(widget);
    pizza->m_scroll_x = 0;
    pizza->m_scroll_y = 0;
    pizza->m_is_scrollable = (windowStyle & (wxHSCROLL | wxVSCROLL)) != 0;
    // mask off border styles not useable with wxPizza
    pizza->m_border_style = int(windowStyle & BORDER_STYLES);
    gtk_widget_set_has_window(widget, true);
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
    return widget;
}

// gtk_fixed_move does not check for a change before issuing a queue_resize,
// we need to avoid that to prevent endless sizing loops, so check first
void wxPizza::move(GtkWidget* widget, int x, int y)
{
    if (gtk_widget_get_parent(widget))
    {
        GValue value = { 0 };
        g_value_init(&value, G_TYPE_INT);
        GtkContainer* container = GTK_CONTAINER(this);
        gtk_container_child_get_property(container, widget, "x", &value);
        const int child_x = g_value_get_int(&value);
        gtk_container_child_get_property(container, widget, "y", &value);
        const int child_y = g_value_get_int(&value);
        if (child_x != x || child_y != y)
            gtk_fixed_move(&m_fixed, widget, x, y);
    }
}

void wxPizza::put(GtkWidget* widget, int x, int y)
{
    gtk_fixed_put(&m_fixed, widget, x, y);
}

struct AdjustData {
    GdkWindow* window;
    int dx, dy;
};

// Adjust allocations for all widgets using the GdkWindow which was just scrolled
extern "C" {
static void scroll_adjust(GtkWidget* widget, void* data)
{
    const AdjustData* p = static_cast<AdjustData*>(data);
    GtkAllocation allocation;
    GdkWindow*    win;

    gtk_widget_get_allocation(widget, &allocation);
    win = gtk_widget_get_window(widget);
    allocation.x += p->dx;
    allocation.y += p->dy;

    if (win == p->window)
    {
        // GtkFrame requires a queue_resize, otherwise parts of
        // the frame newly exposed by the scroll are not drawn.
        // To be safe, do it for all widgets.
        gtk_widget_queue_resize_no_redraw(widget);
        if (GTK_IS_CONTAINER(widget))
            gtk_container_forall(GTK_CONTAINER(widget), scroll_adjust, data);
    }
}
}

void wxPizza::scroll(int dx, int dy)
{
    GtkWidget* widget = GTK_WIDGET(this);
    GdkWindow*    win;

    win = gtk_widget_get_window(widget);

    if (gtk_widget_get_direction(widget) == GTK_TEXT_DIR_RTL)
        dx = -dx;
    m_scroll_x -= dx;
    m_scroll_y -= dy;
    if (win)
    {
        gdk_window_scroll(win, dx, dy);
        // Adjust child allocations. Doing a queue_resize on the children is not
        // enough, sometimes they redraw in the wrong place during fast scrolling.
        AdjustData data = { win, dx, dy };
        gtk_container_forall(GTK_CONTAINER(widget), scroll_adjust, &data);
    }
}

void wxPizza::get_border_widths(int& x, int& y)
{
    x = y = 0;
    if (m_border_style == 0)
        return;

#ifndef __WXUNIVERSAL__
    if (m_border_style & wxBORDER_SIMPLE)
        x = y = 1;
    else if (m_is_scrollable /* || (m_border_style & wxBORDER_THEME) */)
    {
        GtkWidget *style_widget = wxGTKPrivate::GetTreeWidget();
        GtkStyle  *style = gtk_widget_get_style(style_widget);

        if (style)
        {
            x = style->xthickness;
            y = style->ythickness;
        }
    }
    else
    {
        GtkWidget *style_widget = wxGTKPrivate::GetEntryWidget();
        GtkStyle  *style = gtk_widget_get_style(style_widget);

        if (style)
        {
            x = style->xthickness;
            y = style->ythickness;
        }
    }
#endif
}
