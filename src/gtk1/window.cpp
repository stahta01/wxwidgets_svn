/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/window.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
    #pragma implementation "window.h"
#endif

#ifdef __VMS
#define XWarpPointer XWARPPOINTER
#endif

#include "wx/defs.h"
#include "wx/window.h"
#include "wx/dcclient.h"
#include "wx/frame.h"
#include "wx/app.h"
#include "wx/layout.h"
#include "wx/utils.h"
#include "wx/dialog.h"
#include "wx/msgdlg.h"

#if wxUSE_DRAG_AND_DROP
    #include "wx/dnd.h"
#endif

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#if wxUSE_CARET
    #include "wx/caret.h"
#endif // wxUSE_CARET

#if wxUSE_TEXTCTRL
#include "wx/textctrl.h"
#endif

#include "wx/menu.h"
#include "wx/statusbr.h"
#include "wx/intl.h"
#include "wx/settings.h"
#include "wx/log.h"

#ifdef __WXDEBUG__
    #include "wx/thread.h"
#endif

#include <math.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkprivate.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>

#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>

#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// documentation on internals
//-----------------------------------------------------------------------------

/*
   I have been asked several times about writing some documentation about
   the GTK port of wxWindows, especially its internal structures. Obviously,
   you cannot understand wxGTK without knowing a little about the GTK, but
   some more information about what the wxWindow, which is the base class
   for all other window classes, does seems required as well.

   I)

   What does wxWindow do? It contains the common interface for the following
   jobs of its descendants:

   1) Define the rudimentary behaviour common to all window classes, such as
   resizing, intercepting user input (so as to make it possible to use these
   events for special purposes in a derived class), window names etc.

   2) Provide the possibility to contain and manage children, if the derived
   class is allowed to contain children, which holds true for those window
   classes which do not display a native GTK widget. To name them, these
   classes are wxPanel, wxScrolledWindow, wxDialog, wxFrame. The MDI frame-
   work classes are a special case and are handled a bit differently from
   the rest. The same holds true for the wxNotebook class.

   3) Provide the possibility to draw into a client area of a window. This,
   too, only holds true for classes that do not display a native GTK widget
   as above.

   4) Provide the entire mechanism for scrolling widgets. This actual inter-
   face for this is usually in wxScrolledWindow, but the GTK implementation
   is in this class.

   5) A multitude of helper or extra methods for special purposes, such as
   Drag'n'Drop, managing validators etc.

   6) Display a border (sunken, raised, simple or none).

   Normally one might expect, that one wxWindows window would always correspond
   to one GTK widget. Under GTK, there is no such allround widget that has all
   the functionality. Moreover, the GTK defines a client area as a different
   widget from the actual widget you are handling. Last but not least some
   special classes (e.g. wxFrame) handle different categories of widgets and
   still have the possibility to draw something in the client area.
   It was therefore required to write a special purpose GTK widget, that would
   represent a client area in the sense of wxWindows capable to do the jobs
   2), 3) and 4). I have written this class and it resides in win_gtk.c of
   this directory.

   All windows must have a widget, with which they interact with other under-
   lying GTK widgets. It is this widget, e.g. that has to be resized etc and
   thw wxWindow class has a member variable called m_widget which holds a
   pointer to this widget. When the window class represents a GTK native widget,
   this is (in most cases) the only GTK widget the class manages. E.g. the
   wxStatitText class handles only a GtkLabel widget a pointer to which you
   can find in m_widget (defined in wxWindow)

   When the class has a client area for drawing into and for containing children
   it has to handle the client area widget (of the type GtkPizza, defined in
   win_gtk.c), but there could be any number of widgets, handled by a class
   The common rule for all windows is only, that the widget that interacts with
   the rest of GTK must be referenced in m_widget and all other widgets must be
   children of this widget on the GTK level. The top-most widget, which also
   represents the client area, must be in the m_wxwindow field and must be of
   the type GtkPizza.

   As I said, the window classes that display a GTK native widget only have
   one widget, so in the case of e.g. the wxButton class m_widget holds a
   pointer to a GtkButton widget. But windows with client areas (for drawing
   and children) have a m_widget field that is a pointer to a GtkScrolled-
   Window and a m_wxwindow field that is pointer to a GtkPizza and this
   one is (in the GTK sense) a child of the GtkScrolledWindow.

   If the m_wxwindow field is set, then all input to this widget is inter-
   cepted and sent to the wxWindows class. If not, all input to the widget
   that gets pointed to by m_widget gets intercepted and sent to the class.

   II)

   The design of scrolling in wxWindows is markedly different from that offered
   by the GTK itself and therefore we cannot simply take it as it is. In GTK,
   clicking on a scrollbar belonging to scrolled window will inevitably move
   the window. In wxWindows, the scrollbar will only emit an event, send this
   to (normally) a wxScrolledWindow and that class will call ScrollWindow()
   which actually moves the window and its subchildren. Note that GtkPizza
   memorizes how much it has been scrolled but that wxWindows forgets this
   so that the two coordinates systems have to be kept in synch. This is done
   in various places using the pizza->xoffset and pizza->yoffset values.

   III)

   Singularily the most broken code in GTK is the code that is supposes to
   inform subwindows (child windows) about new positions. Very often, duplicate
   events are sent without changes in size or position, equally often no
   events are sent at all (All this is due to a bug in the GtkContainer code
   which got fixed in GTK 1.2.6). For that reason, wxGTK completely ignores
   GTK's own system and it simply waits for size events for toplevel windows
   and then iterates down the respective size events to all window. This has
   the disadvantage, that windows might get size events before the GTK widget
   actually has the reported size. This doesn't normally pose any problem, but
   the OpenGl drawing routines rely on correct behaviour. Therefore, I have
   added the m_nativeSizeEvents flag, which is true only for the OpenGL canvas,
   i.e. the wxGLCanvas will emit a size event, when (and not before) the X11
   window that is used for OpenGl output really has that size (as reported by
   GTK).

   IV)

   If someone at some point of time feels the immense desire to have a look at,
   change or attempt to optimse the Refresh() logic, this person will need an
   intimate understanding of what a "draw" and what an "expose" events are and
   what there are used for, in particular when used in connection with GTK's
   own windowless widgets. Beware.

   V)

   Cursors, too, have been a constant source of pleasure. The main difficulty
   is that a GdkWindow inherits a cursor if the programmer sets a new cursor
   for the parent. To prevent this from doing too much harm, I use idle time
   to set the cursor over and over again, starting from the toplevel windows
   and ending with the youngest generation (speaking of parent and child windows).
   Also don't forget that cursors (like much else) are connected to GdkWindows,
   not GtkWidgets and that the "window" field of a GtkWidget might very well
   point to the GdkWindow of the parent widget (-> "window less widget") and
   that the two obviously have very different meanings.

*/

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern wxList     wxPendingDelete;
extern bool       g_blockEventsOnDrag;
extern bool       g_blockEventsOnScroll;
extern wxCursor   g_globalCursor;

// mouse capture state: the window which has it and if the mouse is currently
// inside it
static wxWindowGTK  *g_captureWindow = (wxWindowGTK*) NULL;
static bool g_captureWindowHasMouse = FALSE;

/* extern */ wxWindowGTK  *g_focusWindow = (wxWindowGTK*) NULL;

// the last window which had the focus - this is normally never NULL (except
// if we never had focus at all) as even when g_focusWindow is NULL it still
// keeps its previous value
static wxWindowGTK *g_focusWindowLast = (wxWindowGTK *)NULL;

// the frame that is currently active (i.e. its child has focus). It is
// used to generate wxActivateEvents
static wxWindowGTK *g_activeFrame = (wxWindowGTK *)NULL;
static bool g_activeFrameLostFocus = FALSE;

// if we detect that the app has got/lost the focus, we set this variable to
// either TRUE or FALSE and an activate event will be sent during the next
// OnIdle() call and it is reset to -1: this value means that we shouldn't
// send any activate events at all
static int        g_sendActivateEvent = -1;

/* hack: we need something to pass to gtk_menu_popup, so we store the time of
   the last click here */
static guint32 gs_timeLastClick = 0;

extern bool g_mainThreadLocked;

//-----------------------------------------------------------------------------
// debug
//-----------------------------------------------------------------------------

#ifndef __WXGTK20__
#define DISABLE_STYLE_IF_BROKEN_THEME 1
#endif

#ifdef __WXDEBUG__

#if wxUSE_THREADS
#   define DEBUG_MAIN_THREAD if (wxThread::IsMain() && g_mainThreadLocked) printf("gui reentrance");
#else
#   define DEBUG_MAIN_THREAD
#endif

static gint gtk_debug_focus_in_callback( GtkWidget *WXUNUSED(widget),
                                         GdkEvent *WXUNUSED(event),
                                         const wxChar *WXUNUSED(name) )
{
/*
    static bool s_done = FALSE;
    if ( !s_done )
    {
        wxLog::AddTraceMask("focus");
        s_done = TRUE;
    }
    wxLogTrace(wxT("FOCUS NOW AT: %s"), name);
*/

    return FALSE;
}

void debug_focus_in( GtkWidget* widget, const wxChar* name, const wxChar *window )
{
    // suppress warnings about gtk_debug_focus_in_callback being unused with
    // this "if ( 0 )"
    if ( 0 )
    {
        wxString tmp = name;
        tmp += wxT(" FROM ");
        tmp += window;

        wxChar *s = new wxChar[tmp.Length()+1];

        wxStrcpy( s, tmp );

        gtk_signal_connect( GTK_OBJECT(widget), "focus_in_event",
          GTK_SIGNAL_FUNC(gtk_debug_focus_in_callback), (gpointer)s );
    }
}

#else
#define DEBUG_MAIN_THREAD
#endif // Debug

//-----------------------------------------------------------------------------
// missing gdk functions
//-----------------------------------------------------------------------------

void
gdk_window_warp_pointer (GdkWindow      *window,
                         gint            x,
                         gint            y)
{
#ifndef __WXGTK20__
  GdkWindowPrivate *priv;
#endif

  if (!window)
    window = GDK_ROOT_PARENT();

#ifdef __WXGTK20__
  if (!GDK_WINDOW_DESTROYED(window))
  {
      XWarpPointer (GDK_WINDOW_XDISPLAY(window),
                    None,              /* not source window -> move from anywhere */
                    GDK_WINDOW_XID(window),  /* dest window */
                    0, 0, 0, 0,        /* not source window -> move from anywhere */
                    x, y );
  }
#else
  priv = (GdkWindowPrivate*) window;

  if (!priv->destroyed)
  {
      XWarpPointer (priv->xdisplay,
                    None,              /* not source window -> move from anywhere */
                    priv->xwindow,  /* dest window */
                    0, 0, 0, 0,        /* not source window -> move from anywhere */
                    x, y );
  }
#endif
}

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// local code (see below)
//-----------------------------------------------------------------------------

// returns the child of win which currently has focus or NULL if not found
//
// Note: can't be static, needed by textctrl.cpp.
wxWindow *wxFindFocusedChild(wxWindowGTK *win)
{
    wxWindow *winFocus = wxWindowGTK::FindFocus();
    if ( !winFocus )
        return (wxWindow *)NULL;

    if ( winFocus == win )
        return (wxWindow *)win;

    for ( wxWindowList::Node *node = win->GetChildren().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *child = wxFindFocusedChild(node->GetData());
        if ( child )
            return child;
    }

    return (wxWindow *)NULL;
}

// Returns toplevel grandparent of given window:
static wxWindowGTK* wxGetTopLevelParent(wxWindowGTK *win)
{
    wxWindowGTK *p = win;
    while (p && !p->IsTopLevel())
         p = p->GetParent();
    return p;
}


static void draw_frame( GtkWidget *widget, wxWindowGTK *win )
{
    // wxUniversal widgets draw the borders and scrollbars themselves
#ifndef __WXUNIVERSAL__
    if (!win->m_hasVMT)
        return;

    int dw = 0;
    int dh = 0;

    if (win->m_hasScrolling)
    {
            GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(widget);

            GtkRequisition vscroll_req;
            vscroll_req.width = 2;
            vscroll_req.height = 2;
            (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(scroll_window->vscrollbar) )->size_request )
                (scroll_window->vscrollbar, &vscroll_req );

            GtkRequisition hscroll_req;
            hscroll_req.width = 2;
            hscroll_req.height = 2;
            (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(scroll_window->hscrollbar) )->size_request )
                (scroll_window->hscrollbar, &hscroll_req );

            GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT_GET_CLASS(widget) );

            if (scroll_window->vscrollbar_visible)
            {
                dw += vscroll_req.width;
                dw += scroll_class->scrollbar_spacing;
            }

            if (scroll_window->hscrollbar_visible)
            {
                dh += hscroll_req.height;
                dh += scroll_class->scrollbar_spacing;
            }
    }

    int dx = 0;
    int dy = 0;
    if (GTK_WIDGET_NO_WINDOW (widget))
    {
        dx += widget->allocation.x;
        dy += widget->allocation.y;
    }

    if (win->HasFlag(wxRAISED_BORDER))
    {
        gtk_draw_shadow( widget->style,
                         widget->window,
                         GTK_STATE_NORMAL,
                         GTK_SHADOW_OUT,
                         dx, dy,
                         widget->allocation.width-dw, widget->allocation.height-dh );
        return;
    }

    if (win->HasFlag(wxSUNKEN_BORDER))
    {
        gtk_draw_shadow( widget->style,
                         widget->window,
                         GTK_STATE_NORMAL,
                         GTK_SHADOW_IN,
                         dx, dy,
                         widget->allocation.width-dw, widget->allocation.height-dh );
        return;
    }

    if (win->HasFlag(wxSIMPLE_BORDER))
    {
        GdkGC *gc;
        gc = gdk_gc_new( widget->window );
        gdk_gc_set_foreground( gc, &widget->style->black );
        gdk_draw_rectangle( widget->window, gc, FALSE,
                         dx, dy,
                         widget->allocation.width-dw-1, widget->allocation.height-dh-1 );
        gdk_gc_unref( gc );
        return;
    }
#endif // __WXUNIVERSAL__
}

//-----------------------------------------------------------------------------
// "expose_event" of m_widget
//-----------------------------------------------------------------------------

gint gtk_window_own_expose_callback( GtkWidget *widget, GdkEventExpose *gdk_event, wxWindowGTK *win )
{
    if (gdk_event->count > 0) return FALSE;

    draw_frame( widget, win );

    return TRUE;
}

//-----------------------------------------------------------------------------
// "draw" of m_widget
//-----------------------------------------------------------------------------

static void gtk_window_own_draw_callback( GtkWidget *widget, GdkRectangle *WXUNUSED(rect), wxWindowGTK *win )
{
    draw_frame( widget, win );
}

//-----------------------------------------------------------------------------
// key code mapping routines
//-----------------------------------------------------------------------------

static long map_to_unmodified_wx_keysym( GdkEventKey *event )
{
    KeySym keysym = event->keyval;
    guint key_code = 0;

    switch (keysym)
    {
        case GDK_Shift_L:
        case GDK_Shift_R:       key_code = WXK_SHIFT;       break;
        case GDK_Control_L:
        case GDK_Control_R:     key_code = WXK_CONTROL;     break;
        case GDK_Meta_L:
        case GDK_Meta_R:
        case GDK_Alt_L:
        case GDK_Alt_R:
        case GDK_Super_L:
        case GDK_Super_R:       key_code = WXK_ALT;         break;
        case GDK_Menu:          key_code = WXK_MENU;        break;
        case GDK_Help:          key_code = WXK_HELP;        break;
        case GDK_BackSpace:     key_code = WXK_BACK;        break;
        case GDK_ISO_Left_Tab:
        case GDK_Tab:           key_code = WXK_TAB;         break;
        case GDK_Linefeed:      key_code = WXK_RETURN;      break;
        case GDK_Clear:         key_code = WXK_CLEAR;       break;
        case GDK_Return:        key_code = WXK_RETURN;      break;
        case GDK_Pause:         key_code = WXK_PAUSE;       break;
        case GDK_Scroll_Lock:   key_code = WXK_SCROLL;      break;
        case GDK_Escape:        key_code = WXK_ESCAPE;      break;
        case GDK_Delete:        key_code = WXK_DELETE;      break;
        case GDK_Home:          key_code = WXK_HOME;        break;
        case GDK_Left:          key_code = WXK_LEFT;        break;
        case GDK_Up:            key_code = WXK_UP;          break;
        case GDK_Right:         key_code = WXK_RIGHT;       break;
        case GDK_Down:          key_code = WXK_DOWN;        break;
        case GDK_Prior:         key_code = WXK_PRIOR;       break;
//      case GDK_Page_Up:       key_code = WXK_PAGEUP;      break;
        case GDK_Next:          key_code = WXK_NEXT;        break;
//      case GDK_Page_Down:     key_code = WXK_PAGEDOWN;    break;
        case GDK_End:           key_code = WXK_END;         break;
        case GDK_Begin:         key_code = WXK_HOME;        break;
        case GDK_Select:        key_code = WXK_SELECT;      break;
        case GDK_Print:         key_code = WXK_PRINT;       break;
        case GDK_Execute:       key_code = WXK_EXECUTE;     break;
        case GDK_Insert:        key_code = WXK_INSERT;      break;
        case GDK_Num_Lock:      key_code = WXK_NUMLOCK;     break;

        case GDK_KP_0:         key_code = WXK_NUMPAD0;      break;
        case GDK_KP_1:         key_code = WXK_NUMPAD1;      break;
        case GDK_KP_2:         key_code = WXK_NUMPAD2;      break;
        case GDK_KP_3:         key_code = WXK_NUMPAD3;      break;
        case GDK_KP_4:         key_code = WXK_NUMPAD4;      break;
        case GDK_KP_5:         key_code = WXK_NUMPAD5;      break;
        case GDK_KP_6:         key_code = WXK_NUMPAD6;      break;
        case GDK_KP_7:         key_code = WXK_NUMPAD7;      break;
        case GDK_KP_8:         key_code = WXK_NUMPAD8;      break;
        case GDK_KP_9:         key_code = WXK_NUMPAD9;      break;
        case GDK_KP_Space:     key_code = WXK_NUMPAD_SPACE; break;
        case GDK_KP_Tab:       key_code = WXK_NUMPAD_TAB;   break;
        case GDK_KP_Enter:     key_code = WXK_NUMPAD_ENTER; break;
        case GDK_KP_F1:        key_code = WXK_NUMPAD_F1;    break;
        case GDK_KP_F2:        key_code = WXK_NUMPAD_F2;    break;
        case GDK_KP_F3:        key_code = WXK_NUMPAD_F3;    break;
        case GDK_KP_F4:        key_code = WXK_NUMPAD_F4;    break;
        case GDK_KP_Home:      key_code = WXK_NUMPAD_HOME;  break;
        case GDK_KP_Left:      key_code = WXK_NUMPAD_LEFT;  break;
        case GDK_KP_Up:        key_code = WXK_NUMPAD_UP;    break;
        case GDK_KP_Right:     key_code = WXK_NUMPAD_RIGHT; break;
        case GDK_KP_Down:      key_code = WXK_NUMPAD_DOWN;  break;
        case GDK_KP_Prior:     key_code = WXK_NUMPAD_PRIOR; break;
//      case GDK_KP_Page_Up:   key_code = WXK_NUMPAD_PAGEUP;   break;
        case GDK_KP_Next:      key_code = WXK_NUMPAD_NEXT;  break;
//      case GDK_KP_Page_Down: key_code = WXK_NUMPAD_PAGEDOWN; break;
        case GDK_KP_End:       key_code = WXK_NUMPAD_END;   break;
        case GDK_KP_Begin:     key_code = WXK_NUMPAD_BEGIN; break;
        case GDK_KP_Insert:    key_code = WXK_NUMPAD_INSERT; break;
        case GDK_KP_Delete:    key_code = WXK_NUMPAD_DELETE; break;
        case GDK_KP_Equal:     key_code = WXK_NUMPAD_EQUAL;  break;
        case GDK_KP_Multiply:  key_code = WXK_NUMPAD_MULTIPLY; break;
        case GDK_KP_Add:       key_code = WXK_NUMPAD_ADD;    break;
        case GDK_KP_Separator: key_code = WXK_NUMPAD_SEPARATOR; break;
        case GDK_KP_Subtract:  key_code = WXK_NUMPAD_SUBTRACT;  break;
        case GDK_KP_Decimal:   key_code = WXK_NUMPAD_DECIMAL;   break;
        case GDK_KP_Divide:    key_code = WXK_NUMPAD_DIVIDE;    break;

        case GDK_F1:            key_code = WXK_F1;          break;
        case GDK_F2:            key_code = WXK_F2;          break;
        case GDK_F3:            key_code = WXK_F3;          break;
        case GDK_F4:            key_code = WXK_F4;          break;
        case GDK_F5:            key_code = WXK_F5;          break;
        case GDK_F6:            key_code = WXK_F6;          break;
        case GDK_F7:            key_code = WXK_F7;          break;
        case GDK_F8:            key_code = WXK_F8;          break;
        case GDK_F9:            key_code = WXK_F9;          break;
        case GDK_F10:           key_code = WXK_F10;         break;
        case GDK_F11:           key_code = WXK_F11;         break;
        case GDK_F12:           key_code = WXK_F12;         break;
        default:
        {
        if (event->length == 1)
        {
            key_code = toupper( (unsigned char)*event->string );
        }
        else if ((keysym & 0xFF) == keysym)
            {
                guint upper = gdk_keyval_to_upper( (guint)keysym );
                keysym = (upper != 0 ? upper : keysym ); /* to be MSW compatible */
                key_code = (guint)keysym;
            }
        }
    }

    return (key_code);
}

static long map_to_wx_keysym( GdkEventKey *event )
{
    KeySym keysym = event->keyval;
    guint key_code = 0;

    switch (keysym)
    {
        case GDK_Menu:          key_code = WXK_MENU;        break;
        case GDK_Help:          key_code = WXK_HELP;        break;
        case GDK_BackSpace:     key_code = WXK_BACK;        break;
        case GDK_ISO_Left_Tab:
        case GDK_Tab:           key_code = WXK_TAB;         break;
        case GDK_Linefeed:      key_code = WXK_RETURN;      break;
        case GDK_Clear:         key_code = WXK_CLEAR;       break;
        case GDK_Return:        key_code = WXK_RETURN;      break;
        case GDK_Pause:         key_code = WXK_PAUSE;       break;
        case GDK_Scroll_Lock:   key_code = WXK_SCROLL;      break;
        case GDK_Escape:        key_code = WXK_ESCAPE;      break;
        case GDK_Delete:        key_code = WXK_DELETE;      break;
        case GDK_Home:          key_code = WXK_HOME;        break;
        case GDK_Left:          key_code = WXK_LEFT;        break;
        case GDK_Up:            key_code = WXK_UP;          break;
        case GDK_Right:         key_code = WXK_RIGHT;       break;
        case GDK_Down:          key_code = WXK_DOWN;        break;
        case GDK_Prior:         key_code = WXK_PRIOR;       break;
//      case GDK_Page_Up:       key_code = WXK_PAGEUP;      break;
        case GDK_Next:          key_code = WXK_NEXT;        break;
//      case GDK_Page_Down:     key_code = WXK_PAGEDOWN;    break;
        case GDK_End:           key_code = WXK_END;         break;
        case GDK_Begin:         key_code = WXK_HOME;        break;
        case GDK_Select:        key_code = WXK_SELECT;      break;
        case GDK_Print:         key_code = WXK_PRINT;       break;
        case GDK_Execute:       key_code = WXK_EXECUTE;     break;
        case GDK_Insert:        key_code = WXK_INSERT;      break;
        case GDK_Num_Lock:      key_code = WXK_NUMLOCK;     break;

        case GDK_KP_0:         key_code = '0';      break;
        case GDK_KP_1:         key_code = '1';      break;
        case GDK_KP_2:         key_code = '2';      break;
        case GDK_KP_3:         key_code = '3';      break;
        case GDK_KP_4:         key_code = '4';      break;
        case GDK_KP_5:         key_code = '5';      break;
        case GDK_KP_6:         key_code = '6';      break;
        case GDK_KP_7:         key_code = '7';      break;
        case GDK_KP_8:         key_code = '8';      break;
        case GDK_KP_9:         key_code = '9';      break;
        case GDK_KP_Space:     key_code = ' ';      break;
        case GDK_KP_Tab:       key_code = WXK_TAB;    break;        /* or '\t' ??? */
        case GDK_KP_Enter:     key_code = WXK_RETURN; break;        /* or '\r' ??? */
        case GDK_KP_F1:        key_code = WXK_NUMPAD_F1;    break;
        case GDK_KP_F2:        key_code = WXK_NUMPAD_F2;    break;
        case GDK_KP_F3:        key_code = WXK_NUMPAD_F3;    break;
        case GDK_KP_F4:        key_code = WXK_NUMPAD_F4;    break;
        case GDK_KP_Home:      key_code = WXK_HOME;  break;
        case GDK_KP_Left:      key_code = WXK_LEFT;  break;
        case GDK_KP_Up:        key_code = WXK_UP;    break;
        case GDK_KP_Right:     key_code = WXK_RIGHT; break;
        case GDK_KP_Down:      key_code = WXK_DOWN;  break;
        case GDK_KP_Prior:     key_code = WXK_PRIOR; break;
//      case GDK_KP_Page_Up:   key_code = WXK_PAGEUP; break;
        case GDK_KP_Next:      key_code = WXK_NEXT;  break;
//      case GDK_KP_Page_Down: key_code = WXK_PAGEDOWN; break;
        case GDK_KP_End:       key_code = WXK_END;    break;
        case GDK_KP_Begin:     key_code = WXK_HOME;   break;
        case GDK_KP_Insert:    key_code = WXK_INSERT; break;
        case GDK_KP_Delete:    key_code = WXK_DELETE; break;
        case GDK_KP_Equal:     key_code = '=';   break;
        case GDK_KP_Multiply:  key_code = '*';   break;
        case GDK_KP_Add:       key_code = '+';   break;
        case GDK_KP_Separator: key_code = ',';   break;
        case GDK_KP_Subtract:  key_code = '-';   break;
        case GDK_KP_Decimal:   key_code = '.';   break;
        case GDK_KP_Divide:    key_code = '/';   break;

        case GDK_F1:            key_code = WXK_F1;          break;
        case GDK_F2:            key_code = WXK_F2;          break;
        case GDK_F3:            key_code = WXK_F3;          break;
        case GDK_F4:            key_code = WXK_F4;          break;
        case GDK_F5:            key_code = WXK_F5;          break;
        case GDK_F6:            key_code = WXK_F6;          break;
        case GDK_F7:            key_code = WXK_F7;          break;
        case GDK_F8:            key_code = WXK_F8;          break;
        case GDK_F9:            key_code = WXK_F9;          break;
        case GDK_F10:           key_code = WXK_F10;         break;
        case GDK_F11:           key_code = WXK_F11;         break;
        case GDK_F12:           key_code = WXK_F12;         break;
        default:
        {
        if (event->length == 1)
        {
            key_code = (unsigned char)*event->string;
        }
        else if ((keysym & 0xFF) == keysym)
            {
                key_code = (guint)keysym;
            }
        }
    }

    return (key_code);
}

//-----------------------------------------------------------------------------
// "size_request" of m_widget
//-----------------------------------------------------------------------------

static void gtk_window_size_request_callback( GtkWidget *widget, GtkRequisition *requisition, wxWindow *win )
{
    int w,h;
    win->GetSize( &w, &h );
    if (w < 2) w = 2;
    if (h < 2) h = 2;

    requisition->height = h;
    requisition->width = w;
}

//-----------------------------------------------------------------------------
// "expose_event" of m_wxwindow
//-----------------------------------------------------------------------------

static int gtk_window_expose_callback( GtkWidget *widget,
                                       GdkEventExpose *gdk_event,
                                       wxWindow *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

/*
    if (win->GetName() == wxT("panel"))
    {
        wxPrintf( wxT("OnExpose from ") );
        if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
            wxPrintf( win->GetClassInfo()->GetClassName() );
        wxPrintf( wxT(" %d %d %d %d\n"), (int)gdk_event->area.x,
                                         (int)gdk_event->area.y,
                                         (int)gdk_event->area.width,
                                         (int)gdk_event->area.height );
    }
*/

    GtkPizza *pizza = GTK_PIZZA (widget);

    if (win->GetThemeEnabled())
    {
        wxWindow *parent = win->GetParent();
        while (parent && !parent->IsTopLevel())
            parent = parent->GetParent();
        if (!parent)
            parent = win;

        gtk_paint_flat_box (parent->m_widget->style, pizza->bin_window, GTK_STATE_NORMAL,
            GTK_SHADOW_NONE, &gdk_event->area, parent->m_widget, "base", 0, 0, -1, -1);
    }

    win->GetUpdateRegion().Union( gdk_event->area.x,
                                  gdk_event->area.y,
                                  gdk_event->area.width,
                                  gdk_event->area.height );

    if (gdk_event->count == 0)
    {
        win->m_clipPaintRegion = TRUE;

        wxWindowDC dc(win);
        dc.SetClippingRegion(win->GetUpdateRegion());
        wxEraseEvent eevent( win->GetId(), &dc );
        eevent.SetEventObject( win );
#if 1
        (void)win->GetEventHandler()->ProcessEvent(eevent);
#else // 0
        if (!win->GetEventHandler()->ProcessEvent(eevent))
        {
            wxClientDC dc( win );
            dc.SetBrush( wxBrush( win->GetBackgroundColour(), wxSOLID ) );
            dc.SetPen( *wxTRANSPARENT_PEN );

            wxRegionIterator upd( win->GetUpdateRegion() );
            while (upd)
            {
                dc.DrawRectangle( upd.GetX(), upd.GetY(), upd.GetWidth(), upd.GetHeight() );
                upd ++;
            }
        }
#endif // 1/0

        wxNcPaintEvent eventNc( win->GetId() );
        eventNc.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( eventNc );

        wxPaintEvent event( win->GetId() );
        event.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( event );

        win->GetUpdateRegion().Clear();

        win->m_clipPaintRegion = FALSE;
    }

    /* The following code will result in all window-less widgets
       being redrawn if the wxWindows class is given a chance to
       paint *anything* because it will then be allowed to paint
       over the window-less widgets */
    GList *children = pizza->children;
    while (children)
    {
        GtkPizzaChild *child = (GtkPizzaChild*) children->data;
        children = children->next;

        GdkEventExpose child_event = *gdk_event;

        if (GTK_WIDGET_NO_WINDOW (child->widget) &&
            GTK_WIDGET_DRAWABLE (child->widget) /* &&
            gtk_widget_intersect (child->widget, &gdk_event->area, &child_event.area)*/ )
        {
            child_event.area.x = child->widget->allocation.x;
            child_event.area.y = child->widget->allocation.y;
            child_event.area.width = child->widget->allocation.width;
            child_event.area.height = child->widget->allocation.height;
            gtk_widget_event (child->widget, (GdkEvent*) &child_event);
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// "event" of m_wxwindow
//-----------------------------------------------------------------------------

/* GTK thinks it is clever and filters out a certain amount of "unneeded"
   expose events. We need them, of course, so we override the main event
   procedure in GtkWidget by giving our own handler for all system events.
   There, we look for expose events ourselves whereas all other events are
   handled normally. */

gint gtk_window_event_event_callback( GtkWidget *widget,
                                      GdkEventExpose *event,
                                      wxWindow *win )
{
    if (event->type == GDK_EXPOSE)
    {
        gint ret = gtk_window_expose_callback( widget, event, win );
        return ret;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "draw" of m_wxwindow
//-----------------------------------------------------------------------------

/* This callback is a complete replacement of the gtk_pizza_draw() function,
   which disabled. */

static void gtk_window_draw_callback( GtkWidget *widget,
                                      GdkRectangle *rect,
                                      wxWindow *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if ((win->HasFlag(wxNO_FULL_REPAINT_ON_RESIZE)) &&
        (win->GetChildren().GetCount() == 0))
    {
        return;
    }

/*
    if (win->GetName() == wxT("panel"))
    {
        wxPrintf( wxT("OnDraw from ") );
        if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
            wxPrintf( win->GetClassInfo()->GetClassName() );
        wxPrintf( wxT(" %d %d %d %d\n"), (int)rect->x,
                                         (int)rect->y,
                                         (int)rect->width,
                                         (int)rect->height );
    }
*/

    GtkPizza *pizza = GTK_PIZZA (widget);

    if (win->GetThemeEnabled())
    {
        wxWindow *parent = win->GetParent();
        while (parent && !parent->IsTopLevel())
            parent = parent->GetParent();
        if (!parent)
            parent = win;

        gtk_paint_flat_box (parent->m_widget->style, pizza->bin_window, GTK_STATE_NORMAL,
            GTK_SHADOW_NONE, rect, parent->m_widget, "base", 0, 0, -1, -1);
    }


    if (!(GTK_WIDGET_APP_PAINTABLE (widget)) &&
         (pizza->clear_on_draw))
    {
        gdk_window_clear_area( pizza->bin_window,
                               rect->x, rect->y, rect->width, rect->height);
    }

    win->GetUpdateRegion().Union( rect->x, rect->y, rect->width, rect->height );

    win->m_clipPaintRegion = TRUE;

    wxWindowDC dc(win);
    dc.SetClippingRegion(win->GetUpdateRegion());
    wxEraseEvent eevent( win->GetId(), &dc );
    eevent.SetEventObject( win );

#if 1
    (void)win->GetEventHandler()->ProcessEvent(eevent);
#else
    if (!win->GetEventHandler()->ProcessEvent(eevent))
    {
        if (!win->GetEventHandler()->ProcessEvent(eevent))
        {
            wxClientDC dc( win );
            dc.SetBrush( wxBrush( win->GetBackgroundColour(), wxSOLID ) );
            dc.SetPen( *wxTRANSPARENT_PEN );

            wxRegionIterator upd( win->GetUpdateRegion() );
            while (upd)
            {
                dc.DrawRectangle( upd.GetX(), upd.GetY(), upd.GetWidth(), upd.GetHeight() );
                upd ++;
            }
        }
    }
#endif

    wxNcPaintEvent eventNc( win->GetId() );
    eventNc.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( eventNc );

    wxPaintEvent event( win->GetId() );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );

    win->GetUpdateRegion().Clear();

    win->m_clipPaintRegion = FALSE;


    GList *children = pizza->children;
    while (children)
    {
            GtkPizzaChild *child = (GtkPizzaChild*) children->data;
            children = children->next;

            GdkRectangle child_area;
            if (gtk_widget_intersect (child->widget, rect, &child_area))
            {
                gtk_widget_draw (child->widget, &child_area /* (GdkRectangle*) NULL*/ );
            }
    }
}

//-----------------------------------------------------------------------------
// "key_press_event" from any window
//-----------------------------------------------------------------------------

static gint gtk_window_key_press_callback( GtkWidget *widget,
                                           GdkEventKey *gdk_event,
                                           wxWindow *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;


/*
    wxString tmp;
    tmp += (char)gdk_event->keyval;
    printf( "KeyDown-Code is: %s.\n", tmp.c_str() );
    printf( "KeyDown-ScanCode is: %d.\n", gdk_event->keyval );
*/

    int x = 0;
    int y = 0;
    GdkModifierType state;
    if (gdk_event->window) gdk_window_get_pointer(gdk_event->window, &x, &y, &state);

    bool ret = FALSE;

    long key_code = map_to_unmodified_wx_keysym( gdk_event );
    /* sending unknown key events doesn't really make sense */
    if (key_code == 0) return FALSE;

    wxKeyEvent event( wxEVT_KEY_DOWN );
    event.SetTimestamp( gdk_event->time );
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
    event.m_keyCode = key_code;
    event.m_scanCode = gdk_event->keyval;
    event.m_x = x;
    event.m_y = y;
    event.SetEventObject( win );
    ret = win->GetEventHandler()->ProcessEvent( event );

#if wxUSE_ACCEL
    if (!ret)
    {
        wxWindowGTK *ancestor = win;
        while (ancestor)
        {
            int command = ancestor->GetAcceleratorTable()->GetCommand( event );
            if (command != -1)
            {
                wxCommandEvent command_event( wxEVT_COMMAND_MENU_SELECTED, command );
                ret = ancestor->GetEventHandler()->ProcessEvent( command_event );
                break;
            }
            if (ancestor->IsTopLevel())
                break;
            ancestor = ancestor->GetParent();
        }
    }
#endif // wxUSE_ACCEL

    /* wxMSW doesn't send char events with Alt pressed */
    /* Only send wxEVT_CHAR event if not processed yet. Thus, ALT-x
       will only be sent if it is not in an accelerator table. */
    key_code = map_to_wx_keysym( gdk_event );

    if ( (!ret) &&
         (key_code != 0))
    {
        wxKeyEvent event2( wxEVT_CHAR );
        event2.SetTimestamp( gdk_event->time );
        event2.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
        event2.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
        event2.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
        event2.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
        event2.m_keyCode = key_code;
        event2.m_scanCode = gdk_event->keyval;
        event2.m_x = x;
        event2.m_y = y;
        event2.SetEventObject( win );
        ret = win->GetEventHandler()->ProcessEvent( event2 );
    }

    /* win is a control: tab can be propagated up */
    if ( (!ret) &&
         ((gdk_event->keyval == GDK_Tab) || (gdk_event->keyval == GDK_ISO_Left_Tab)) &&
// VZ: testing for wxTE_PROCESS_TAB shouldn't be done here the control may
//     have this style, yet choose not to process this particular TAB in which
//     case TAB must still work as a navigational character
#if 0
         (!win->HasFlag(wxTE_PROCESS_TAB)) &&
#endif // 0
         (win->GetParent()) &&
         (win->GetParent()->HasFlag( wxTAB_TRAVERSAL)) )
    {
        wxNavigationKeyEvent new_event;
        new_event.SetEventObject( win->GetParent() );
        /* GDK reports GDK_ISO_Left_Tab for SHIFT-TAB */
        new_event.SetDirection( (gdk_event->keyval == GDK_Tab) );
        /* CTRL-TAB changes the (parent) window, i.e. switch notebook page */
        new_event.SetWindowChange( (gdk_event->state & GDK_CONTROL_MASK) );
        new_event.SetCurrentFocus( win );
        ret = win->GetParent()->GetEventHandler()->ProcessEvent( new_event );
    }

    /* generate wxID_CANCEL if <esc> has been pressed (typically in dialogs) */
    if ( (!ret) &&
         (gdk_event->keyval == GDK_Escape) )
    {
        wxCommandEvent new_event(wxEVT_COMMAND_BUTTON_CLICKED,wxID_CANCEL);
        new_event.SetEventObject( win );
        ret = win->GetEventHandler()->ProcessEvent( new_event );
    }

#if (GTK_MINOR_VERSION > 0)
    /* Pressing F10 will activate the menu bar of the top frame. */
    /* Doesn't work. */
/*
    if ( (!ret) &&
         (gdk_event->keyval == GDK_F10) )
    {
        wxWindowGTK *ancestor = win;
        while (ancestor)
        {
            if (wxIsKindOf(ancestor,wxFrame))
            {
                wxFrame *frame = (wxFrame*) ancestor;
                wxMenuBar *menubar = frame->GetMenuBar();
                if (menubar)
                {
                    wxNode *node = menubar->GetMenus().First();
                    if (node)
                    {
                        wxMenu *firstMenu = (wxMenu*) node->Data();
                        gtk_menu_item_select( GTK_MENU_ITEM(firstMenu->m_owner) );
                        ret = TRUE;
                        break;
                    }
                }
            }
            ancestor = ancestor->GetParent();
        }
    }
*/
#endif

    if (ret)
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "key_press_event" );
        return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "key_release_event" from any window
//-----------------------------------------------------------------------------

static gint gtk_window_key_release_callback( GtkWidget *widget, GdkEventKey *gdk_event, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

/*
    printf( "KeyUp-ScanCode is: %d.\n", gdk_event->keyval );
    if (gdk_event->state & GDK_SHIFT_MASK)
      printf( "ShiftDown.\n" );
    else
      printf( "ShiftUp.\n" );
    if (gdk_event->state & GDK_CONTROL_MASK)
      printf( "ControlDown.\n" );
    else
      printf( "ControlUp.\n" );
    printf( "\n" );
*/

    long key_code = map_to_unmodified_wx_keysym( gdk_event );

    /* sending unknown key events doesn't really make sense */
    if (key_code == 0) return FALSE;

    int x = 0;
    int y = 0;
    GdkModifierType state;
    if (gdk_event->window) gdk_window_get_pointer(gdk_event->window, &x, &y, &state);

    wxKeyEvent event( wxEVT_KEY_UP );
    event.SetTimestamp( gdk_event->time );
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
    event.m_keyCode = key_code;
    event.m_scanCode = gdk_event->keyval;
    event.m_x = x;
    event.m_y = y;
    event.SetEventObject( win );

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "key_release_event" );
        return TRUE;
    }

    return FALSE;
}

// ----------------------------------------------------------------------------
// mouse event processing helper
// ----------------------------------------------------------------------------

static void AdjustEventButtonState(wxMouseEvent& event)
{
    // GDK reports the old state of the button for a button press event, but
    // for compatibility with MSW and common sense we want m_leftDown be TRUE
    // for a LEFT_DOWN event, not FALSE, so we will invert
    // left/right/middleDown for the corresponding click events

    if ((event.GetEventType() == wxEVT_LEFT_DOWN) ||
        (event.GetEventType() == wxEVT_LEFT_DCLICK) ||
        (event.GetEventType() == wxEVT_LEFT_UP))
    {
        event.m_leftDown = !event.m_leftDown;
        return;
    }

    if ((event.GetEventType() == wxEVT_MIDDLE_DOWN) ||
        (event.GetEventType() == wxEVT_MIDDLE_DCLICK) ||
        (event.GetEventType() == wxEVT_MIDDLE_UP))
    {
        event.m_middleDown = !event.m_middleDown;
        return;
    }

    if ((event.GetEventType() == wxEVT_RIGHT_DOWN) ||
        (event.GetEventType() == wxEVT_RIGHT_DCLICK) ||
        (event.GetEventType() == wxEVT_RIGHT_UP))
    {
        event.m_rightDown = !event.m_rightDown;
        return;
    }
}

//-----------------------------------------------------------------------------
// "button_press_event"
//-----------------------------------------------------------------------------

static gint gtk_window_button_press_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

/*
    wxPrintf( wxT("1) OnButtonPress from ") );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( wxT(".\n") );
*/
    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

    if (win->m_wxwindow)
    {
        if (GTK_WIDGET_CAN_FOCUS(win->m_wxwindow) && !GTK_WIDGET_HAS_FOCUS (win->m_wxwindow) )
        {
            gtk_widget_grab_focus (win->m_wxwindow);

/*
            wxPrintf( wxT("GrabFocus from ") );
            if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
                wxPrintf( win->GetClassInfo()->GetClassName() );
            wxPrintf( wxT(".\n") );
*/

        }
    }

    wxEventType event_type = wxEVT_NULL;

    if (gdk_event->button == 1)
    {
        switch (gdk_event->type)
        {
            case GDK_BUTTON_PRESS: event_type = wxEVT_LEFT_DOWN; break;
            case GDK_2BUTTON_PRESS: event_type = wxEVT_LEFT_DCLICK; break;
            default:  break;
        }
    }
    else if (gdk_event->button == 2)
    {
        switch (gdk_event->type)
        {
            case GDK_BUTTON_PRESS: event_type = wxEVT_MIDDLE_DOWN; break;
            case GDK_2BUTTON_PRESS: event_type = wxEVT_MIDDLE_DCLICK; break;
            default:  break;
        }
    }
    else if (gdk_event->button == 3)
    {
        switch (gdk_event->type)
        {
            case GDK_BUTTON_PRESS: event_type = wxEVT_RIGHT_DOWN; break;
            case GDK_2BUTTON_PRESS: event_type = wxEVT_RIGHT_DCLICK; break;
            default:  break;
        }
    }

    if ( event_type == wxEVT_NULL )
    {
        // unknown mouse button or click type
        return FALSE;
    }

    wxMouseEvent event( event_type );
    event.SetTimestamp( gdk_event->time );
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
    event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
    event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
    event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);

    event.m_x = (wxCoord)gdk_event->x;
    event.m_y = (wxCoord)gdk_event->y;

    AdjustEventButtonState(event);

    // Some control don't have their own X window and thus cannot get
    // any events.

    if (!g_captureWindow)
    {
        wxCoord x = event.m_x;
        wxCoord y = event.m_y;
        if (win->m_wxwindow)
        {
            GtkPizza *pizza = GTK_PIZZA(win->m_wxwindow);
            x += pizza->xoffset;
            y += pizza->yoffset;
        }

        wxNode *node = win->GetChildren().First();
        while (node)
        {
            wxWindowGTK *child = (wxWindowGTK*)node->Data();

            node = node->Next();
            if (!child->IsShown())
                continue;

            if (child->m_isStaticBox)
            {
                // wxStaticBox is transparent in the box itself
                int xx1 = child->m_x;
                int yy1 = child->m_y;
                int xx2 = child->m_x + child->m_width;
                int yy2 = child->m_x + child->m_height;

                // left
                if (((x >= xx1) && (x <= xx1+10) && (y >= yy1) && (y <= yy2)) ||
                // right
                    ((x >= xx2-10) && (x <= xx2) && (y >= yy1) && (y <= yy2)) ||
                // top
                    ((x >= xx1) && (x <= xx2) && (y >= yy1) && (y <= yy1+10)) ||
                // bottom
                    ((x >= xx1) && (x <= xx2) && (y >= yy2-1) && (y <= yy2)))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }

            }
            else
            {
                if ((child->m_wxwindow == (GtkWidget*) NULL) &&
                    (child->m_x <= x) &&
                    (child->m_y <= y) &&
                    (child->m_x+child->m_width  >= x) &&
                    (child->m_y+child->m_height >= y))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }
            }
        }
    }

    event.SetEventObject( win );

    gs_timeLastClick = gdk_event->time;

/*
    wxPrintf( wxT("2) OnButtonPress from ") );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( wxT(".\n") );
*/

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "button_press_event" );
        return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "button_release_event"
//-----------------------------------------------------------------------------

static gint gtk_window_button_release_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;
    if (g_blockEventsOnScroll) return FALSE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

/*
    printf( "OnButtonRelease from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
    printf( ".\n" );
*/

    wxEventType event_type = wxEVT_NULL;

    switch (gdk_event->button)
    {
        case 1: event_type = wxEVT_LEFT_UP; break;
        case 2: event_type = wxEVT_MIDDLE_UP; break;
        case 3: event_type = wxEVT_RIGHT_UP; break;
        default: return FALSE;
    }

    wxMouseEvent event( event_type );
    event.SetTimestamp( gdk_event->time );
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
    event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
    event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
    event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);
    event.m_x = (wxCoord)gdk_event->x;
    event.m_y = (wxCoord)gdk_event->y;

    AdjustEventButtonState(event);

    // Some control don't have their own X window and thus cannot get
    // any events.

    if (!g_captureWindow)
    {
        wxCoord x = event.m_x;
        wxCoord y = event.m_y;
        if (win->m_wxwindow)
        {
            GtkPizza *pizza = GTK_PIZZA(win->m_wxwindow);
            x += pizza->xoffset;
            y += pizza->yoffset;
        }

        wxNode *node = win->GetChildren().First();
        while (node)
        {
            wxWindowGTK *child = (wxWindowGTK*)node->Data();

            node = node->Next();
            if (!child->IsShown())
                continue;

            if (child->m_isStaticBox)
            {
                // wxStaticBox is transparent in the box itself
                int xx1 = child->m_x;
                int yy1 = child->m_y;
                int xx2 = child->m_x + child->m_width;
                int yy2 = child->m_x + child->m_height;

                // left
                if (((x >= xx1) && (x <= xx1+10) && (y >= yy1) && (y <= yy2)) ||
                // right
                    ((x >= xx2-10) && (x <= xx2) && (y >= yy1) && (y <= yy2)) ||
                // top
                    ((x >= xx1) && (x <= xx2) && (y >= yy1) && (y <= yy1+10)) ||
                // bottom
                    ((x >= xx1) && (x <= xx2) && (y >= yy2-1) && (y <= yy2)))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }

            }
            else
            {
                if ((child->m_wxwindow == (GtkWidget*) NULL) &&
                    (child->m_x <= x) &&
                    (child->m_y <= y) &&
                    (child->m_x+child->m_width  >= x) &&
                    (child->m_y+child->m_height >= y))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }
            }
        }
    }

    event.SetEventObject( win );

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "button_release_event" );
        return TRUE;
    }

    return FALSE;
}

// ============================================================================
// the mouse events
// ============================================================================

// init wxMouseEvent with the info from gdk_event
#define InitMouseEvent(event, gdk_event) \
    event.SetTimestamp( gdk_event->time ); \
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK); \
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK); \
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK); \
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK); \
    event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK); \
    event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK); \
    event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK); \
\
    event.m_x = (wxCoord)gdk_event->x; \
    event.m_y = (wxCoord)gdk_event->y \

//-----------------------------------------------------------------------------
// "motion_notify_event"
//-----------------------------------------------------------------------------

static gint gtk_window_motion_notify_callback( GtkWidget *widget,
                                               GdkEventMotion *gdk_event,
                                               wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;
    if (g_blockEventsOnScroll) return FALSE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

    if (gdk_event->is_hint)
    {
        int x = 0;
        int y = 0;
        GdkModifierType state;
        gdk_window_get_pointer(gdk_event->window, &x, &y, &state);
        gdk_event->x = x;
        gdk_event->y = y;
    }

/*
    printf( "OnMotion from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
      printf( win->GetClassInfo()->GetClassName() );
    printf( ".\n" );
*/

    wxMouseEvent event( wxEVT_MOTION );
    InitMouseEvent(event, gdk_event);

    if ( g_captureWindow )
    {
        // synthetize a mouse enter or leave event if needed
        GdkWindow *winUnderMouse = gdk_window_at_pointer(NULL, NULL);
        bool hasMouse = winUnderMouse == gdk_event->window;
        if ( hasMouse != g_captureWindowHasMouse )
        {
            // the mouse changed window
            g_captureWindowHasMouse = hasMouse;

            wxMouseEvent event(g_captureWindowHasMouse ? wxEVT_ENTER_WINDOW
                                                       : wxEVT_LEAVE_WINDOW);
            InitMouseEvent(event, gdk_event);
            event.SetEventObject(win);
            win->GetEventHandler()->ProcessEvent(event);
        }
    }
    else // no capture
    {
        // Some control don't have their own X window and thus cannot get
        // any events.

        wxCoord x = event.m_x;
        wxCoord y = event.m_y;
        if (win->m_wxwindow)
        {
            GtkPizza *pizza = GTK_PIZZA(win->m_wxwindow);
            x += pizza->xoffset;
            y += pizza->yoffset;
        }

        wxNode *node = win->GetChildren().First();
        while (node)
        {
            wxWindowGTK *child = (wxWindowGTK*)node->Data();

            node = node->Next();
            if (!child->IsShown())
                continue;

            if (child->m_isStaticBox)
            {
                // wxStaticBox is transparent in the box itself
                int xx1 = child->m_x;
                int yy1 = child->m_y;
                int xx2 = child->m_x + child->m_width;
                int yy2 = child->m_x + child->m_height;

                // left
                if (((x >= xx1) && (x <= xx1+10) && (y >= yy1) && (y <= yy2)) ||
                // right
                    ((x >= xx2-10) && (x <= xx2) && (y >= yy1) && (y <= yy2)) ||
                // top
                    ((x >= xx1) && (x <= xx2) && (y >= yy1) && (y <= yy1+10)) ||
                // bottom
                    ((x >= xx1) && (x <= xx2) && (y >= yy2-1) && (y <= yy2)))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }

            }
            else
            {
                if ((child->m_wxwindow == (GtkWidget*) NULL) &&
                    (child->m_x <= x) &&
                    (child->m_y <= y) &&
                    (child->m_x+child->m_width  >= x) &&
                    (child->m_y+child->m_height >= y))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }
            }
        }
    }

    event.SetEventObject( win );

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "motion_notify_event" );
        return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "focus_in_event"
//-----------------------------------------------------------------------------

static gint gtk_window_focus_in_callback( GtkWidget *widget,
                                          GdkEvent *WXUNUSED(event),
                                          wxWindow *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    switch ( g_sendActivateEvent )
    {
        case -1:
            // we've got focus from outside, synthetize wxActivateEvent
            g_sendActivateEvent = 1;
            break;

        case 0:
            // another our window just lost focus, it was already ours before
            // - don't send any wxActivateEvent
            g_sendActivateEvent = -1;
            break;
    }

    g_focusWindowLast =
    g_focusWindow = win;

#if 0
    wxPrintf( "OnSetFocus from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( ".\n" );
#endif

    // notify the parent keeping track of focus for the kbd navigation
    // purposes that we got it
    wxChildFocusEvent eventFocus(win);
    (void)win->GetEventHandler()->ProcessEvent(eventFocus);

#ifdef HAVE_XIM
    if (win->m_ic)
        gdk_im_begin(win->m_ic, win->m_wxwindow->window);
#endif

#if wxUSE_CARET
    // caret needs to be informed about focus change
    wxCaret *caret = win->GetCaret();
    if ( caret )
    {
        caret->OnSetFocus();
    }
#endif // wxUSE_CARET

    
    wxWindowGTK *active = wxGetTopLevelParent(win);
    if ( active != g_activeFrame )
    {
        if ( g_activeFrame )
        {
            wxActivateEvent event(wxEVT_ACTIVATE, FALSE, g_activeFrame->GetId());
            event.SetEventObject(g_activeFrame);
            g_activeFrame->GetEventHandler()->ProcessEvent(event);
        }

        g_activeFrame = active;
        wxActivateEvent event(wxEVT_ACTIVATE, TRUE, g_activeFrame->GetId());
        event.SetEventObject(g_activeFrame);
        g_activeFrame->GetEventHandler()->ProcessEvent(event);
    }
    g_activeFrameLostFocus = FALSE;


    wxFocusEvent event( wxEVT_SET_FOCUS, win->GetId() );
    event.SetEventObject( win );

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
       gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "focus_in_event" );
       return TRUE;
    }


    return FALSE;
}

//-----------------------------------------------------------------------------
// "focus_out_event"
//-----------------------------------------------------------------------------

static GtkWidget *gs_widgetLastFocus = NULL;

static gint gtk_window_focus_out_callback( GtkWidget *widget, GdkEvent *WXUNUSED(event), wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    wxASSERT_MSG( wxGetTopLevelParent(win) == g_activeFrame, wxT("unfocusing window that haven't gained focus properly") )
    g_activeFrameLostFocus = TRUE;

    // VZ: this is really weird but GTK+ seems to call us from inside
    //     gtk_widget_grab_focus(), i.e. it first sends "focus_out" signal to
    //     this widget and then "focus_in". This is totally unexpected and
    //     completely breaks wxUniv code so ignore this dummy event (we can't
    //     be losing focus if we're about to acquire it!)
    if ( widget == gs_widgetLastFocus )
    {
        gs_widgetLastFocus = NULL;

        return FALSE;
    }

    // if the focus goes out of our app alltogether, OnIdle() will send
    // wxActivateEvent, otherwise gtk_window_focus_in_callback() will reset
    // g_sendActivateEvent to -1
    g_sendActivateEvent = 0;

    wxWindowGTK *winFocus = wxFindFocusedChild(win);
    if ( winFocus )
        win = winFocus;

    g_focusWindow = (wxWindowGTK *)NULL;

#if 0
    wxPrintf( "OnKillFocus from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( ".\n" );
#endif

#ifdef HAVE_XIM
    if (win->m_ic)
        gdk_im_end();
#endif

#if wxUSE_CARET
    // caret needs to be informed about focus change
    wxCaret *caret = win->GetCaret();
    if ( caret )
    {
        caret->OnKillFocus();
    }
#endif // wxUSE_CARET

    wxFocusEvent event( wxEVT_KILL_FOCUS, win->GetId() );
    event.SetEventObject( win );

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "focus_out_event" );
        return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "enter_notify_event"
//-----------------------------------------------------------------------------

static gint gtk_window_enter_callback( GtkWidget *widget, GdkEventCrossing *gdk_event, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

    wxMouseEvent event( wxEVT_ENTER_WINDOW );
    event.SetTimestamp( gdk_event->time );
    event.SetEventObject( win );

    int x = 0;
    int y = 0;
    GdkModifierType state = (GdkModifierType)0;

    gdk_window_get_pointer( widget->window, &x, &y, &state );

    InitMouseEvent(event, gdk_event);

    event.m_x = x;
    event.m_y = y;

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
       gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "enter_notify_event" );
       return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "leave_notify_event"
//-----------------------------------------------------------------------------

static gint gtk_window_leave_callback( GtkWidget *widget, GdkEventCrossing *gdk_event, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

    wxMouseEvent event( wxEVT_LEAVE_WINDOW );
    event.SetTimestamp( gdk_event->time );
    event.SetEventObject( win );

    int x = 0;
    int y = 0;
    GdkModifierType state = (GdkModifierType)0;

    gdk_window_get_pointer( widget->window, &x, &y, &state );

    event.m_shiftDown = (state & GDK_SHIFT_MASK);
    event.m_controlDown = (state & GDK_CONTROL_MASK);
    event.m_altDown = (state & GDK_MOD1_MASK);
    event.m_metaDown = (state & GDK_MOD2_MASK);
    event.m_leftDown = (state & GDK_BUTTON1_MASK);
    event.m_middleDown = (state & GDK_BUTTON2_MASK);
    event.m_rightDown = (state & GDK_BUTTON3_MASK);

    event.m_x = x;
    event.m_y = y;

    if (win->GetEventHandler()->ProcessEvent( event ))
    {
        gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "leave_notify_event" );
        return TRUE;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// "value_changed" from m_vAdjust
//-----------------------------------------------------------------------------

static void gtk_window_vscroll_callback( GtkAdjustment *adjust, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return;

    if (!win->m_hasVMT) return;

    float diff = adjust->value - win->m_oldVerticalPos;
    if (fabs(diff) < 0.2) return;

    win->m_oldVerticalPos = adjust->value;

    GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(win->m_widget);
    GtkRange *range = GTK_RANGE( scrolledWindow->vscrollbar );

    wxEventType command = wxEVT_SCROLLWIN_THUMBTRACK;
    if      (range->scroll_type == GTK_SCROLL_STEP_BACKWARD) command = wxEVT_SCROLLWIN_LINEUP;
    else if (range->scroll_type == GTK_SCROLL_STEP_FORWARD)  command = wxEVT_SCROLLWIN_LINEDOWN;
    else if (range->scroll_type == GTK_SCROLL_PAGE_BACKWARD) command = wxEVT_SCROLLWIN_PAGEUP;
    else if (range->scroll_type == GTK_SCROLL_PAGE_FORWARD)  command = wxEVT_SCROLLWIN_PAGEDOWN;

    int value = (int)(adjust->value+0.5);

    wxScrollWinEvent event( command, value, wxVERTICAL );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "value_changed" from m_hAdjust
//-----------------------------------------------------------------------------

static void gtk_window_hscroll_callback( GtkAdjustment *adjust, wxWindowGTK *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return;
    if (!win->m_hasVMT) return;

    float diff = adjust->value - win->m_oldHorizontalPos;
    if (fabs(diff) < 0.2) return;

    win->m_oldHorizontalPos = adjust->value;

    GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(win->m_widget);
    GtkRange *range = GTK_RANGE( scrolledWindow->hscrollbar );

    wxEventType command = wxEVT_SCROLLWIN_THUMBTRACK;
    if      (range->scroll_type == GTK_SCROLL_STEP_BACKWARD) command = wxEVT_SCROLLWIN_LINEUP;
    else if (range->scroll_type == GTK_SCROLL_STEP_FORWARD)  command = wxEVT_SCROLLWIN_LINEDOWN;
    else if (range->scroll_type == GTK_SCROLL_PAGE_BACKWARD) command = wxEVT_SCROLLWIN_PAGEUP;
    else if (range->scroll_type == GTK_SCROLL_PAGE_FORWARD)  command = wxEVT_SCROLLWIN_PAGEDOWN;

    int value = (int)(adjust->value+0.5);

    wxScrollWinEvent event( command, value, wxHORIZONTAL );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "button_press_event" from scrollbar
//-----------------------------------------------------------------------------

static gint gtk_scrollbar_button_press_callback( GtkRange *widget,
                                                 GdkEventButton *gdk_event,
                                                 wxWindowGTK *win)
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();


    g_blockEventsOnScroll = TRUE;
    win->m_isScrolling = (gdk_event->window == widget->slider);

    return FALSE;
}

//-----------------------------------------------------------------------------
// "button_release_event" from scrollbar
//-----------------------------------------------------------------------------

static gint gtk_scrollbar_button_release_callback( GtkRange *widget,
                                                   GdkEventButton *WXUNUSED(gdk_event),
                                                   wxWindowGTK *win)
{
    DEBUG_MAIN_THREAD

//  don't test here as we can release the mouse while being over
//  a different window than the slider
//
//    if (gdk_event->window != widget->slider) return FALSE;

    g_blockEventsOnScroll = FALSE;

    if (win->m_isScrolling)
    {
        wxEventType command = wxEVT_SCROLLWIN_THUMBRELEASE;
        int value = -1;
        int dir = -1;

        GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(win->m_widget);
        if (widget == GTK_RANGE(scrolledWindow->hscrollbar))
        {
            value = (int)(win->m_hAdjust->value+0.5);
            dir = wxHORIZONTAL;
        }
        if (widget == GTK_RANGE(scrolledWindow->vscrollbar))
        {
            value = (int)(win->m_vAdjust->value+0.5);
            dir = wxVERTICAL;
        }

        wxScrollWinEvent event( command, value, dir );
        event.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( event );
    }

    win->m_isScrolling = FALSE;

    return FALSE;
}

// ----------------------------------------------------------------------------
// this wxWindowBase function is implemented here (in platform-specific file)
// because it is static and so couldn't be made virtual
// ----------------------------------------------------------------------------

wxWindow *wxWindowBase::FindFocus()
{
    // the cast is necessary when we compile in wxUniversal mode
    return (wxWindow *)g_focusWindow;
}

//-----------------------------------------------------------------------------
// "realize" from m_widget
//-----------------------------------------------------------------------------

/* We cannot set colours and fonts before the widget has
   been realized, so we do this directly after realization. */

static gint
gtk_window_realized_callback( GtkWidget *WXUNUSED(m_widget), wxWindow *win )
{
    DEBUG_MAIN_THREAD

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (win->m_delayedBackgroundColour)
        win->SetBackgroundColour( win->GetBackgroundColour() );

    if (win->m_delayedForegroundColour)
        win->SetForegroundColour( win->GetForegroundColour() );

    wxWindowCreateEvent event( win );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );

    return FALSE;
}

//-----------------------------------------------------------------------------
// "size_allocate"
//-----------------------------------------------------------------------------

static
void gtk_window_size_callback( GtkWidget *WXUNUSED(widget),
                               GtkAllocation *WXUNUSED(alloc),
                               wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasScrolling) return;

    int client_width = 0;
    int client_height = 0;
    win->GetClientSize( &client_width, &client_height );
    if ((client_width == win->m_oldClientWidth) && (client_height == win->m_oldClientHeight))
        return;

    win->m_oldClientWidth = client_width;
    win->m_oldClientHeight = client_height;

    if (!win->m_nativeSizeEvent)
    {
        wxSizeEvent event( win->GetSize(), win->GetId() );
        event.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( event );
    }
}


#ifdef HAVE_XIM
    #define WXUNUSED_UNLESS_XIM(param)  param
#else
    #define WXUNUSED_UNLESS_XIM(param)  WXUNUSED(param)
#endif

/* Resize XIM window */

static
void gtk_wxwindow_size_callback( GtkWidget* WXUNUSED_UNLESS_XIM(widget),
                                 GtkAllocation* WXUNUSED_UNLESS_XIM(alloc),
                                 wxWindowGTK* WXUNUSED_UNLESS_XIM(win) )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

#ifdef HAVE_XIM
    if (!win->m_ic)
        return;

    if  (gdk_ic_get_style (win->m_ic) & GDK_IM_PREEDIT_POSITION)
    {
        gint width, height;

        gdk_window_get_size (widget->window, &width, &height);
        win->m_icattr->preedit_area.width = width;
        win->m_icattr->preedit_area.height = height;
        gdk_ic_set_attr (win->m_ic, win->m_icattr, GDK_IC_PREEDIT_AREA);
    }
#endif // HAVE_XIM
}

//-----------------------------------------------------------------------------
// "realize" from m_wxwindow
//-----------------------------------------------------------------------------

/* Initialize XIM support */

static gint
gtk_wxwindow_realized_callback( GtkWidget * WXUNUSED_UNLESS_XIM(widget),
                                wxWindowGTK * WXUNUSED_UNLESS_XIM(win) )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

#ifdef HAVE_XIM
    if (win->m_ic) return FALSE;
    if (!widget) return FALSE;
    if (!gdk_im_ready()) return FALSE;

    win->m_icattr = gdk_ic_attr_new();
    if (!win->m_icattr) return FALSE;

    gint width, height;
    GdkEventMask mask;
    GdkColormap *colormap;
    GdkICAttr *attr = win->m_icattr;
    unsigned attrmask = GDK_IC_ALL_REQ;
    GdkIMStyle style;
    GdkIMStyle supported_style = (GdkIMStyle)
                                  (GDK_IM_PREEDIT_NONE |
                                   GDK_IM_PREEDIT_NOTHING |
                                   GDK_IM_PREEDIT_POSITION |
                                   GDK_IM_STATUS_NONE |
                                   GDK_IM_STATUS_NOTHING);

    if (widget->style && widget->style->font->type != GDK_FONT_FONTSET)
        supported_style = (GdkIMStyle)(supported_style & ~GDK_IM_PREEDIT_POSITION);

    attr->style = style = gdk_im_decide_style (supported_style);
    attr->client_window = widget->window;

    if ((colormap = gtk_widget_get_colormap (widget)) !=
            gtk_widget_get_default_colormap ())
    {
        attrmask |= GDK_IC_PREEDIT_COLORMAP;
        attr->preedit_colormap = colormap;
    }

    attrmask |= GDK_IC_PREEDIT_FOREGROUND;
    attrmask |= GDK_IC_PREEDIT_BACKGROUND;
    attr->preedit_foreground = widget->style->fg[GTK_STATE_NORMAL];
    attr->preedit_background = widget->style->base[GTK_STATE_NORMAL];

    switch (style & GDK_IM_PREEDIT_MASK)
    {
        case GDK_IM_PREEDIT_POSITION:
            if (widget->style && widget->style->font->type != GDK_FONT_FONTSET)
            {
                g_warning ("over-the-spot style requires fontset");
                break;
            }

            gdk_window_get_size (widget->window, &width, &height);

            attrmask |= GDK_IC_PREEDIT_POSITION_REQ;
            attr->spot_location.x = 0;
            attr->spot_location.y = height;
            attr->preedit_area.x = 0;
            attr->preedit_area.y = 0;
            attr->preedit_area.width = width;
            attr->preedit_area.height = height;
            attr->preedit_fontset = widget->style->font;

            break;
    }

      win->m_ic = gdk_ic_new (attr, (GdkICAttributesType)attrmask);

      if (win->m_ic == NULL)
          g_warning ("Can't create input context.");
      else
      {
          mask = gdk_window_get_events (widget->window);
          mask = (GdkEventMask)(mask | gdk_ic_get_events (win->m_ic));
          gdk_window_set_events (widget->window, mask);

          if (GTK_WIDGET_HAS_FOCUS(widget))
              gdk_im_begin (win->m_ic, widget->window);
      }
#endif // HAVE_XIM

    return FALSE;
}

//-----------------------------------------------------------------------------
// InsertChild for wxWindowGTK.
//-----------------------------------------------------------------------------

/* Callback for wxWindowGTK. This very strange beast has to be used because
 * C++ has no virtual methods in a constructor. We have to emulate a
 * virtual function here as wxNotebook requires a different way to insert
 * a child in it. I had opted for creating a wxNotebookPage window class
 * which would have made this superfluous (such in the MDI window system),
 * but no-one was listening to me... */

static void wxInsertChildInWindow( wxWindowGTK* parent, wxWindowGTK* child )
{
    /* the window might have been scrolled already, do we
       have to adapt the position */
    GtkPizza *pizza = GTK_PIZZA(parent->m_wxwindow);
    child->m_x += pizza->xoffset;
    child->m_y += pizza->yoffset;

    gtk_pizza_put( GTK_PIZZA(parent->m_wxwindow),
                     GTK_WIDGET(child->m_widget),
                     child->m_x,
                     child->m_y,
                     child->m_width,
                     child->m_height );
}

//-----------------------------------------------------------------------------
// global functions
//-----------------------------------------------------------------------------

wxWindow *wxGetActiveWindow()
{
    // the cast is necessary when we compile in wxUniversal mode
    return (wxWindow *)g_focusWindow;
}

//-----------------------------------------------------------------------------
// wxWindowGTK
//-----------------------------------------------------------------------------

// in wxUniv/MSW this class is abstract because it doesn't have DoPopupMenu()
// method
#ifdef __WXUNIVERSAL__
    IMPLEMENT_ABSTRACT_CLASS(wxWindowGTK, wxWindowBase)
#else // __WXGTK__
    IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowBase)
#endif // __WXUNIVERSAL__/__WXGTK__

void wxWindowGTK::Init()
{
    // common init
    InitBase();

    // GTK specific
    m_widget = (GtkWidget *) NULL;
    m_wxwindow = (GtkWidget *) NULL;
    m_focusWidget = (GtkWidget *) NULL;

    // position/size
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;

    m_sizeSet = FALSE;
    m_hasVMT = FALSE;
    m_needParent = TRUE;
    m_isBeingDeleted = FALSE;

    m_noExpose = FALSE;
    m_nativeSizeEvent = FALSE;
    
    m_hasScrolling = FALSE;
    m_isScrolling = FALSE;

    m_hAdjust = (GtkAdjustment*) NULL;
    m_vAdjust = (GtkAdjustment*) NULL;
    m_oldHorizontalPos = 0.0;
    m_oldVerticalPos = 0.0;

    m_resizing = FALSE;
    m_widgetStyle = (GtkStyle*) NULL;

    m_insertCallback = (wxInsertChildFunction) NULL;

    m_isStaticBox = FALSE;
    m_isRadioButton = FALSE;
    m_isFrame = FALSE;
    m_acceptsFocus = FALSE;

    m_clipPaintRegion = FALSE;

    m_cursor = *wxSTANDARD_CURSOR;

    m_delayedForegroundColour = FALSE;
    m_delayedBackgroundColour = FALSE;

#ifdef HAVE_XIM
    m_ic = (GdkIC*) NULL;
    m_icattr = (GdkICAttr*) NULL;
#endif
}

wxWindowGTK::wxWindowGTK()
{
    Init();
}

wxWindowGTK::wxWindowGTK( wxWindow *parent,
                          wxWindowID id,
                          const wxPoint &pos,
                          const wxSize &size,
                          long style,
                          const wxString &name  )
{
    Init();

    Create( parent, id, pos, size, style, name );
}

bool wxWindowGTK::Create( wxWindow *parent,
                          wxWindowID id,
                          const wxPoint &pos,
                          const wxSize &size,
                          long style,
                          const wxString &name  )
{
    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, wxDefaultValidator, name ))
    {
        wxFAIL_MSG( wxT("wxWindowGTK creation failed") );
        return FALSE;
    }

    m_insertCallback = wxInsertChildInWindow;

    m_widget = gtk_scrolled_window_new( (GtkAdjustment *) NULL, (GtkAdjustment *) NULL );
    GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );

    GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(m_widget);

    GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT_GET_CLASS(m_widget) );
    scroll_class->scrollbar_spacing = 0;

    gtk_scrolled_window_set_policy( scrolledWindow, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

    m_hAdjust = gtk_range_get_adjustment( GTK_RANGE(scrolledWindow->hscrollbar) );
    m_vAdjust = gtk_range_get_adjustment( GTK_RANGE(scrolledWindow->vscrollbar) );

    m_wxwindow = gtk_pizza_new();

    gtk_container_add( GTK_CONTAINER(m_widget), m_wxwindow );

#ifndef __WXUNIVERSAL__
#if (GTK_MINOR_VERSION > 0)
    GtkPizza *pizza = GTK_PIZZA(m_wxwindow);

    if (HasFlag(wxRAISED_BORDER))
    {
        gtk_pizza_set_shadow_type( pizza, GTK_MYSHADOW_OUT );
    }
    else if (HasFlag(wxSUNKEN_BORDER))
    {
        gtk_pizza_set_shadow_type( pizza, GTK_MYSHADOW_IN );
    }
    else if (HasFlag(wxSIMPLE_BORDER))
    {
        gtk_pizza_set_shadow_type( pizza, GTK_MYSHADOW_THIN );
    }
    else
    {
        gtk_pizza_set_shadow_type( pizza, GTK_MYSHADOW_NONE );
    }
#else // GTK_MINOR_VERSION == 0
    GtkViewport *viewport = GTK_VIEWPORT(scrolledWindow->viewport);

    if (HasFlag(wxRAISED_BORDER))
    {
        gtk_viewport_set_shadow_type( viewport, GTK_SHADOW_OUT );
    }
    else if (HasFlag(wxSUNKEN_BORDER))
    {
        gtk_viewport_set_shadow_type( viewport, GTK_SHADOW_IN );
    }
    else
    {
        gtk_viewport_set_shadow_type( viewport, GTK_SHADOW_NONE );
    }
#endif // GTK_MINOR_VERSION
#endif // __WXUNIVERSAL__

    GTK_WIDGET_SET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );
    m_acceptsFocus = TRUE;

#if (GTK_MINOR_VERSION == 0)
    // shut the viewport up
    gtk_viewport_set_hadjustment( viewport, (GtkAdjustment*) gtk_adjustment_new( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) );
    gtk_viewport_set_vadjustment( viewport, (GtkAdjustment*) gtk_adjustment_new( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) );
#endif // GTK_MINOR_VERSION == 0

    // I _really_ don't want scrollbars in the beginning
    m_vAdjust->lower = 0.0;
    m_vAdjust->upper = 1.0;
    m_vAdjust->value = 0.0;
    m_vAdjust->step_increment = 1.0;
    m_vAdjust->page_increment = 1.0;
    m_vAdjust->page_size = 5.0;
    gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "changed" );
    m_hAdjust->lower = 0.0;
    m_hAdjust->upper = 1.0;
    m_hAdjust->value = 0.0;
    m_hAdjust->step_increment = 1.0;
    m_hAdjust->page_increment = 1.0;
    m_hAdjust->page_size = 5.0;
    gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "changed" );

    // these handlers block mouse events to any window during scrolling such as
    // motion events and prevent GTK and wxWindows from fighting over where the
    // slider should be

    gtk_signal_connect( GTK_OBJECT(scrolledWindow->vscrollbar), "button_press_event",
          (GtkSignalFunc)gtk_scrollbar_button_press_callback, (gpointer) this );

    gtk_signal_connect( GTK_OBJECT(scrolledWindow->hscrollbar), "button_press_event",
          (GtkSignalFunc)gtk_scrollbar_button_press_callback, (gpointer) this );

    gtk_signal_connect( GTK_OBJECT(scrolledWindow->vscrollbar), "button_release_event",
          (GtkSignalFunc)gtk_scrollbar_button_release_callback, (gpointer) this );

    gtk_signal_connect( GTK_OBJECT(scrolledWindow->hscrollbar), "button_release_event",
          (GtkSignalFunc)gtk_scrollbar_button_release_callback, (gpointer) this );

    // these handlers get notified when screen updates are required either when
    // scrolling or when the window size (and therefore scrollbar configuration)
    // has changed

    gtk_signal_connect( GTK_OBJECT(m_hAdjust), "value_changed",
          (GtkSignalFunc) gtk_window_hscroll_callback, (gpointer) this );
    gtk_signal_connect( GTK_OBJECT(m_vAdjust), "value_changed",
          (GtkSignalFunc) gtk_window_vscroll_callback, (gpointer) this );

    gtk_widget_show( m_wxwindow );

    if (m_parent)
        m_parent->DoAddChild( this );
        
    m_focusWidget = m_wxwindow;

    PostCreation();

    Show( TRUE );

    return TRUE;
}

wxWindowGTK::~wxWindowGTK()
{
    if (g_focusWindow == this)
        g_focusWindow = NULL;

    if (g_activeFrame == this)
        g_activeFrame = NULL;

    m_isBeingDeleted = TRUE;
    m_hasVMT = FALSE;

    if (m_widget)
        Show( FALSE );

    DestroyChildren();

    if (m_parent)
        m_parent->RemoveChild( this );

#ifdef HAVE_XIM
    if (m_ic)
        gdk_ic_destroy (m_ic);
    if (m_icattr)
        gdk_ic_attr_destroy (m_icattr);
#endif

    if (m_widgetStyle)
    {
#if DISABLE_STYLE_IF_BROKEN_THEME
        // don't delete if it's a pixmap theme style
        if (!m_widgetStyle->engine_data)
            gtk_style_unref( m_widgetStyle );
#endif
        m_widgetStyle = (GtkStyle*) NULL;
    }

    if (m_wxwindow)
    {
        gtk_widget_destroy( m_wxwindow );
        m_wxwindow = (GtkWidget*) NULL;
    }

    if (m_widget)
    {
        gtk_widget_destroy( m_widget );
        m_widget = (GtkWidget*) NULL;
    }
}

bool wxWindowGTK::PreCreation( wxWindowGTK *parent, const wxPoint &pos,  const wxSize &size )
{
    wxCHECK_MSG( !m_needParent || parent, FALSE, wxT("Need complete parent.") );

    /* this turns -1 into 20 so that a minimal window is
       visible even although -1,-1 has been given as the
       size of the window. the same trick is used in other
       ports and should make debugging easier */
    m_width = WidthDefault(size.x);
    m_height = HeightDefault(size.y);

    m_x = (int)pos.x;
    m_y = (int)pos.y;

    /* some reasonable defaults */
    if (!parent)
    {
        if (m_x == -1)
        {
            m_x = (gdk_screen_width () - m_width) / 2;
            if (m_x < 10) m_x = 10;
        }
        if (m_y == -1)
        {
            m_y = (gdk_screen_height () - m_height) / 2;
            if (m_y < 10) m_y = 10;
        }
    }

    return TRUE;
}

void wxWindowGTK::PostCreation()
{
    wxASSERT_MSG( (m_widget != NULL), wxT("invalid window") );
    
    if (m_wxwindow)
    {
        if (!m_noExpose)
        {
            // these get reported to wxWindows -> wxPaintEvent

            gtk_pizza_set_external( GTK_PIZZA(m_wxwindow), TRUE );

            gtk_signal_connect( GTK_OBJECT(m_wxwindow), "expose_event",
                GTK_SIGNAL_FUNC(gtk_window_expose_callback), (gpointer)this );

            gtk_signal_connect( GTK_OBJECT(m_wxwindow), "draw",
                GTK_SIGNAL_FUNC(gtk_window_draw_callback), (gpointer)this );

            if (HasFlag(wxNO_FULL_REPAINT_ON_RESIZE))
            {
                gtk_signal_connect( GTK_OBJECT(m_wxwindow), "event",
                    GTK_SIGNAL_FUNC(gtk_window_event_event_callback), (gpointer)this );
            }
        }

        // these are called when the "sunken" or "raised" borders are drawn */
        gtk_signal_connect( GTK_OBJECT(m_widget), "expose_event",
          GTK_SIGNAL_FUNC(gtk_window_own_expose_callback), (gpointer)this );

        gtk_signal_connect( GTK_OBJECT(m_widget), "draw",
          GTK_SIGNAL_FUNC(gtk_window_own_draw_callback), (gpointer)this );
    }

    // focus handling

    if (m_focusWidget == NULL)
        m_focusWidget = m_widget;

#if 0
    if (GetClassInfo() && GetClassInfo()->GetClassName())
        wxPrintf( GetClassInfo()->GetClassName() );
    wxPrintf( ".\n" );
#endif

    gtk_signal_connect( GTK_OBJECT(m_focusWidget), "focus_in_event",
        GTK_SIGNAL_FUNC(gtk_window_focus_in_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_focusWidget), "focus_out_event",
         GTK_SIGNAL_FUNC(gtk_window_focus_out_callback), (gpointer)this );

    // connect to the various key and mouse handlers

    GtkWidget *connect_widget = GetConnectWidget();

    ConnectWidget( connect_widget );

    /* We cannot set colours, fonts and cursors before the widget has
       been realized, so we do this directly after realization */
    gtk_signal_connect( GTK_OBJECT(connect_widget), "realize",
                            GTK_SIGNAL_FUNC(gtk_window_realized_callback), (gpointer) this );

    if (m_wxwindow)
    {
        // Catch native resize events
        gtk_signal_connect( GTK_OBJECT(m_wxwindow), "size_allocate",
                            GTK_SIGNAL_FUNC(gtk_window_size_callback), (gpointer)this );

        // Initialize XIM support
        gtk_signal_connect( GTK_OBJECT(m_wxwindow), "realize",
                            GTK_SIGNAL_FUNC(gtk_wxwindow_realized_callback), (gpointer) this );

        // And resize XIM window
        gtk_signal_connect( GTK_OBJECT(m_wxwindow), "size_allocate",
                            GTK_SIGNAL_FUNC(gtk_wxwindow_size_callback), (gpointer)this );
    }

    if (!GTK_IS_COMBO(m_widget))
    {
        // This is needed if we want to add our windows into native
        // GTK control, such as the toolbar. With this callback, the
        // toolbar gets to know the correct size (the one set by the
        // programmer). Sadly, it misbehaves for wxComboBox. FIXME
        // when moving to GTK 2.0.
        gtk_signal_connect( GTK_OBJECT(m_widget), "size_request",
                            GTK_SIGNAL_FUNC(gtk_window_size_request_callback), (gpointer) this );
    }

    m_hasVMT = TRUE;
}

void wxWindowGTK::ConnectWidget( GtkWidget *widget )
{
    gtk_signal_connect( GTK_OBJECT(widget), "key_press_event",
      GTK_SIGNAL_FUNC(gtk_window_key_press_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "key_release_event",
      GTK_SIGNAL_FUNC(gtk_window_key_release_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "button_press_event",
      GTK_SIGNAL_FUNC(gtk_window_button_press_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "button_release_event",
      GTK_SIGNAL_FUNC(gtk_window_button_release_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "motion_notify_event",
      GTK_SIGNAL_FUNC(gtk_window_motion_notify_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "enter_notify_event",
      GTK_SIGNAL_FUNC(gtk_window_enter_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "leave_notify_event",
      GTK_SIGNAL_FUNC(gtk_window_leave_callback), (gpointer)this );
}

bool wxWindowGTK::Destroy()
{
    wxASSERT_MSG( (m_widget != NULL), wxT("invalid window") );

    m_hasVMT = FALSE;

    return wxWindowBase::Destroy();
}

void wxWindowGTK::DoMoveWindow(int x, int y, int width, int height)
{
    gtk_pizza_set_size( GTK_PIZZA(m_parent->m_wxwindow), m_widget, x, y, width, height );
}

void wxWindowGTK::DoSetSize( int x, int y, int width, int height, int sizeFlags )
{
    wxASSERT_MSG( (m_widget != NULL), wxT("invalid window") );
    wxASSERT_MSG( (m_parent != NULL), wxT("wxWindowGTK::SetSize requires parent.\n") );

/*
    printf( "DoSetSize: name %s, x,y,w,h: %d,%d,%d,%d \n", GetName().c_str(), x,y,width,height );
*/

    if (m_resizing) return; /* I don't like recursions */
    m_resizing = TRUE;

    int currentX, currentY;
    GetPosition(&currentX, &currentY);
    if (x == -1)
        x = currentX;
    if (y == -1)
        y = currentY;
    AdjustForParentClientOrigin(x, y, sizeFlags);

    if (m_parent->m_wxwindow == NULL) /* i.e. wxNotebook */
    {
        /* don't set the size for children of wxNotebook, just take the values. */
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
    }
    else
    {
        GtkPizza *pizza = GTK_PIZZA(m_parent->m_wxwindow);

        if ((sizeFlags & wxSIZE_ALLOW_MINUS_ONE) == 0)
        {
            if (x != -1) m_x = x + pizza->xoffset;
            if (y != -1) m_y = y + pizza->yoffset;
            if (width != -1) m_width = width;
            if (height != -1) m_height = height;
        }
        else
        {
            m_x = x + pizza->xoffset;
            m_y = y + pizza->yoffset;
            m_width = width;
            m_height = height;
        }

        if ((sizeFlags & wxSIZE_AUTO_WIDTH) == wxSIZE_AUTO_WIDTH)
        {
             if (width == -1) m_width = 80;
        }

        if ((sizeFlags & wxSIZE_AUTO_HEIGHT) == wxSIZE_AUTO_HEIGHT)
        {
             if (height == -1) m_height = 26;
        }

        if ((m_minWidth != -1) && (m_width < m_minWidth)) m_width = m_minWidth;
        if ((m_minHeight != -1) && (m_height < m_minHeight)) m_height = m_minHeight;
        if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_maxWidth;
        if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_maxHeight;

        int border = 0;
        int bottom_border = 0;

        if (GTK_WIDGET_CAN_DEFAULT(m_widget))
        {
            /* the default button has a border around it */
            border = 6;
            bottom_border = 5;
        }

        DoMoveWindow( m_x-border,
                      m_y-border,
                      m_width+2*border,
                      m_height+border+bottom_border );
    }

    if (m_hasScrolling)
    {
        /* Sometimes the client area changes size without the
           whole windows's size changing, but if the whole
           windows's size doesn't change, no wxSizeEvent will
           normally be sent. Here we add an extra test if
           the client test has been changed and this will
           be used then. */
        GetClientSize( &m_oldClientWidth, &m_oldClientHeight );
    }

/*
    wxPrintf( "OnSize sent from " );
    if (GetClassInfo() && GetClassInfo()->GetClassName())
        wxPrintf( GetClassInfo()->GetClassName() );
    wxPrintf( " %d %d %d %d\n", (int)m_x, (int)m_y, (int)m_width, (int)m_height );
*/

    if (!m_nativeSizeEvent)
    {
        wxSizeEvent event( wxSize(m_width,m_height), GetId() );
        event.SetEventObject( this );
        GetEventHandler()->ProcessEvent( event );
    }

    m_resizing = FALSE;
}

void wxWindowGTK::OnInternalIdle()
{
    if ( g_sendActivateEvent != -1 )
    {
        bool activate = g_sendActivateEvent != 0;

        // do it only once
        g_sendActivateEvent = -1;

        wxTheApp->SetActive(activate, (wxWindow *)g_focusWindowLast);
    }

    if ( g_activeFrameLostFocus )
    {
        if ( g_activeFrame )
        {
            wxActivateEvent event(wxEVT_ACTIVATE, FALSE, g_activeFrame->GetId());
            event.SetEventObject(g_activeFrame);
            g_activeFrame->GetEventHandler()->ProcessEvent(event);
            g_activeFrame = NULL;
        }
        g_activeFrameLostFocus = FALSE;
    }

    wxCursor cursor = m_cursor;
    if (g_globalCursor.Ok()) cursor = g_globalCursor;

    if (cursor.Ok())
    {
        /* I now set the cursor anew in every OnInternalIdle call
           as setting the cursor in a parent window also effects the
           windows above so that checking for the current cursor is
           not possible. */

        if (m_wxwindow)
        {
            GdkWindow *window = GTK_PIZZA(m_wxwindow)->bin_window;
            if (window)
                gdk_window_set_cursor( window, cursor.GetCursor() );

            if (!g_globalCursor.Ok())
                cursor = *wxSTANDARD_CURSOR;

            window = m_widget->window;
            if ((window) && !(GTK_WIDGET_NO_WINDOW(m_widget)))
                gdk_window_set_cursor( window, cursor.GetCursor() );

        }
        else
        {

            GdkWindow *window = m_widget->window;
            if ((window) && !(GTK_WIDGET_NO_WINDOW(m_widget)))
               gdk_window_set_cursor( window, cursor.GetCursor() );

        }
    }

    UpdateWindowUI();
}

void wxWindowGTK::DoGetSize( int *width, int *height ) const
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (width) (*width) = m_width;
    if (height) (*height) = m_height;
}

void wxWindowGTK::DoSetClientSize( int width, int height )
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (!m_wxwindow)
    {
        SetSize( width, height );
    }
    else
    {
        int dw = 0;
        int dh = 0;

#ifndef __WXUNIVERSAL__
        if (HasFlag(wxRAISED_BORDER) || HasFlag(wxSUNKEN_BORDER))
        {
            /* when using GTK 1.2 we set the shadow border size to 2 */
            dw += 2 * 2;
            dh += 2 * 2;
        }
        if (HasFlag(wxSIMPLE_BORDER))
        {
            /* when using GTK 1.2 we set the simple border size to 1 */
            dw += 1 * 2;
            dh += 1 * 2;
        }
#endif // __WXUNIVERSAL__

        if (m_hasScrolling)
        {
            GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);

            GtkRequisition vscroll_req;
            vscroll_req.width = 2;
            vscroll_req.height = 2;
            (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(scroll_window->vscrollbar) )->size_request )
                (scroll_window->vscrollbar, &vscroll_req );

            GtkRequisition hscroll_req;
            hscroll_req.width = 2;
            hscroll_req.height = 2;
            (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(scroll_window->hscrollbar) )->size_request )
                (scroll_window->hscrollbar, &hscroll_req );

            GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT_GET_CLASS(m_widget) );

            if (scroll_window->vscrollbar_visible)
            {
                dw += vscroll_req.width;
                dw += scroll_class->scrollbar_spacing;
            }

            if (scroll_window->hscrollbar_visible)
            {
                dh += hscroll_req.height;
                dh += scroll_class->scrollbar_spacing;
            }
        }

       SetSize( width+dw, height+dh );
    }
}

void wxWindowGTK::DoGetClientSize( int *width, int *height ) const
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (!m_wxwindow)
    {
        if (width) (*width) = m_width;
        if (height) (*height) = m_height;
    }
    else
    {
        int dw = 0;
        int dh = 0;

#ifndef __WXUNIVERSAL__
        if (HasFlag(wxRAISED_BORDER) || HasFlag(wxSUNKEN_BORDER))
        {
            /* when using GTK 1.2 we set the shadow border size to 2 */
            dw += 2 * 2;
            dh += 2 * 2;
        }
        if (HasFlag(wxSIMPLE_BORDER))
        {
            /* when using GTK 1.2 we set the simple border size to 1 */
            dw += 1 * 2;
            dh += 1 * 2;
        }
#endif // __WXUNIVERSAL__

        if (m_hasScrolling)
        {
            GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);

            GtkRequisition vscroll_req;
            vscroll_req.width = 2;
            vscroll_req.height = 2;
            (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(scroll_window->vscrollbar) )->size_request )
                (scroll_window->vscrollbar, &vscroll_req );

            GtkRequisition hscroll_req;
            hscroll_req.width = 2;
            hscroll_req.height = 2;
            (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(scroll_window->hscrollbar) )->size_request )
                (scroll_window->hscrollbar, &hscroll_req );

            GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT_GET_CLASS(m_widget) );

            if (scroll_window->vscrollbar_visible)
            {
                dw += vscroll_req.width;
                dw += scroll_class->scrollbar_spacing;
            }

            if (scroll_window->hscrollbar_visible)
            {
                dh += hscroll_req.height;
                dh += scroll_class->scrollbar_spacing;
            }
        }

        if (width) (*width) = m_width - dw;
        if (height) (*height) = m_height - dh;
    }

/*
    printf( "GetClientSize, name %s ", GetName().c_str() );
    if (width) printf( " width = %d", (*width) );
    if (height) printf( " height = %d", (*height) );
    printf( "\n" );
*/
}

void wxWindowGTK::DoGetPosition( int *x, int *y ) const
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    int dx = 0;
    int dy = 0;
    if (m_parent && m_parent->m_wxwindow)
    {
        GtkPizza *pizza = GTK_PIZZA(m_parent->m_wxwindow);
        dx = pizza->xoffset;
        dy = pizza->yoffset;
    }
    
    if (x) (*x) = m_x - dx;
    if (y) (*y) = m_y - dy;
}

void wxWindowGTK::DoClientToScreen( int *x, int *y ) const
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (!m_widget->window) return;

    GdkWindow *source = (GdkWindow *) NULL;
    if (m_wxwindow)
        source = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        source = m_widget->window;

    int org_x = 0;
    int org_y = 0;
    gdk_window_get_origin( source, &org_x, &org_y );

    if (!m_wxwindow)
    {
        if (GTK_WIDGET_NO_WINDOW (m_widget))
        {
            org_x += m_widget->allocation.x;
            org_y += m_widget->allocation.y;
        }
    }

    if (x) *x += org_x;
    if (y) *y += org_y;
}

void wxWindowGTK::DoScreenToClient( int *x, int *y ) const
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (!m_widget->window) return;

    GdkWindow *source = (GdkWindow *) NULL;
    if (m_wxwindow)
        source = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        source = m_widget->window;

    int org_x = 0;
    int org_y = 0;
    gdk_window_get_origin( source, &org_x, &org_y );

    if (!m_wxwindow)
    {
        if (GTK_WIDGET_NO_WINDOW (m_widget))
        {
            org_x += m_widget->allocation.x;
            org_y += m_widget->allocation.y;
        }
    }

    if (x) *x -= org_x;
    if (y) *y -= org_y;
}

bool wxWindowGTK::Show( bool show )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, wxT("invalid window") );

    if (!wxWindowBase::Show(show))
    {
        // nothing to do
        return FALSE;
    }

    if (show)
        gtk_widget_show( m_widget );
    else
        gtk_widget_hide( m_widget );

    return TRUE;
}

static void wxWindowNotifyEnable(wxWindowGTK* win, bool enable)
{
    win->OnParentEnable(enable);

    // Recurse, so that children have the opportunity to Do The Right Thing
    // and reset colours that have been messed up by a parent's (really ancestor's)
    // Enable call
    for ( wxWindowList::Node *node = win->GetChildren().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *child = node->GetData();
        if (!child->IsKindOf(CLASSINFO(wxDialog)) && !child->IsKindOf(CLASSINFO(wxFrame)))
            wxWindowNotifyEnable(child, enable);
    }
}

bool wxWindowGTK::Enable( bool enable )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, wxT("invalid window") );

    if (!wxWindowBase::Enable(enable))
    {
        // nothing to do
        return FALSE;
    }

    gtk_widget_set_sensitive( m_widget, enable );
    if ( m_wxwindow )
        gtk_widget_set_sensitive( m_wxwindow, enable );

    wxWindowNotifyEnable(this, enable);

    return TRUE;
}

int wxWindowGTK::GetCharHeight() const
{
    wxCHECK_MSG( (m_widget != NULL), 12, wxT("invalid window") );

    wxCHECK_MSG( m_font.Ok(), 12, wxT("invalid font") );

    GdkFont *font = m_font.GetInternalFont( 1.0 );

    return font->ascent + font->descent;
}

int wxWindowGTK::GetCharWidth() const
{
    wxCHECK_MSG( (m_widget != NULL), 8, wxT("invalid window") );

    wxCHECK_MSG( m_font.Ok(), 8, wxT("invalid font") );

    GdkFont *font = m_font.GetInternalFont( 1.0 );

    return gdk_string_width( font, "H" );
}

void wxWindowGTK::GetTextExtent( const wxString& string,
                              int *x,
                              int *y,
                              int *descent,
                              int *externalLeading,
                              const wxFont *theFont ) const
{
    wxFont fontToUse = m_font;
    if (theFont) fontToUse = *theFont;

    wxCHECK_RET( fontToUse.Ok(), wxT("invalid font") );

    GdkFont *font = fontToUse.GetInternalFont( 1.0 );
    if (x) (*x) = gdk_string_width( font, string.mbc_str() );
    if (y) (*y) = font->ascent + font->descent;
    if (descent) (*descent) = font->descent;
    if (externalLeading) (*externalLeading) = 0;  // ??
}

void wxWindowGTK::SetFocus()
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (m_wxwindow)
    {
        if (!GTK_WIDGET_HAS_FOCUS (m_wxwindow))
        {
            // see comment in gtk_window_focus_out_callback()
            gs_widgetLastFocus = m_wxwindow;
            gtk_widget_grab_focus (m_wxwindow);
        }
    }
    else if (m_widget)
    {
        if (GTK_WIDGET_CAN_FOCUS(m_widget) && !GTK_WIDGET_HAS_FOCUS (m_widget) )
        {
            gtk_widget_grab_focus (m_widget);
        }
        else if (GTK_IS_CONTAINER(m_widget))
        {
            gtk_container_focus( GTK_CONTAINER(m_widget), GTK_DIR_TAB_FORWARD );
        }
        else
        {
           // ?
        }
    }
}

bool wxWindowGTK::AcceptsFocus() const
{
    return m_acceptsFocus && wxWindowBase::AcceptsFocus();
}

bool wxWindowGTK::Reparent( wxWindowBase *newParentBase )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, wxT("invalid window") );

    wxWindowGTK *oldParent = m_parent,
             *newParent = (wxWindowGTK *)newParentBase;

    wxASSERT( GTK_IS_WIDGET(m_widget) );

    if ( !wxWindowBase::Reparent(newParent) )
        return FALSE;

    wxASSERT( GTK_IS_WIDGET(m_widget) );

    /* prevent GTK from deleting the widget arbitrarily */
    gtk_widget_ref( m_widget );

    if (oldParent)
    {
        gtk_container_remove( GTK_CONTAINER(m_widget->parent), m_widget );
    }

    wxASSERT( GTK_IS_WIDGET(m_widget) );

    if (newParent)
    {
        /* insert GTK representation */
        (*(newParent->m_insertCallback))(newParent, this);
    }

    /* reverse: prevent GTK from deleting the widget arbitrarily */
    gtk_widget_unref( m_widget );

    return TRUE;
}

void wxWindowGTK::DoAddChild(wxWindowGTK *child)
{
    wxASSERT_MSG( (m_widget != NULL), wxT("invalid window") );

    wxASSERT_MSG( (child != NULL), wxT("invalid child window") );

    wxASSERT_MSG( (m_insertCallback != NULL), wxT("invalid child insertion function") );

    /* add to list */
    AddChild( child );

    /* insert GTK representation */
    (*m_insertCallback)(this, child);
}

void wxWindowGTK::Raise()
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (!m_widget->window) return;

    gdk_window_raise( m_widget->window );
}

void wxWindowGTK::Lower()
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    if (!m_widget->window) return;

    gdk_window_lower( m_widget->window );
}

bool wxWindowGTK::SetCursor( const wxCursor &cursor )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, wxT("invalid window") );

    if (cursor == m_cursor)
       return FALSE;

    if (g_isIdle)
        wxapp_install_idle_handler();

    if (cursor == wxNullCursor)
       return wxWindowBase::SetCursor( *wxSTANDARD_CURSOR );
    else
       return wxWindowBase::SetCursor( cursor );
}

void wxWindowGTK::WarpPointer( int x, int y )
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid window") );

    /* we provide this function ourselves as it is
       missing in GDK (top of this file)  */

    GdkWindow *window = (GdkWindow*) NULL;
    if (m_wxwindow)
        window = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        window = GetConnectWidget()->window;

    if (window)
        gdk_window_warp_pointer( window, x, y );
}

void wxWindowGTK::Refresh( bool eraseBackground, const wxRect *rect )
{
    if (!m_widget) return;
    if (!m_widget->window) return;

    // temporarily hide the caret to avoid nasty interactions between caret
    // drawing and the window contents redraw
#if 0 // def wxUSE_CARET -- doesn't seem to help :-(
    wxCaretSuspend cs((wxWindow *)this);
#endif // wxUSE_CARET

    if (eraseBackground && m_wxwindow && m_wxwindow->window)
    {
        if (rect)
        {
            gdk_window_clear_area( GTK_PIZZA(m_wxwindow)->bin_window,
                                   rect->x, rect->y,
                                   rect->width, rect->height );
        }
        else
        {
            gdk_window_clear( GTK_PIZZA(m_wxwindow)->bin_window );
        }
    }

    /* there is no GTK equivalent of "draw only, don't clear" so we
       invent our own in the GtkPizza widget */

    if (!rect)
    {
        if (m_wxwindow)
        {

/*
            GtkPizza *pizza = GTK_PIZZA(m_wxwindow);
            gboolean old_clear = pizza->clear_on_draw;
            gtk_pizza_set_clear( pizza, FALSE );
            gtk_widget_draw( m_wxwindow, (GdkRectangle*) NULL );
            gtk_pizza_set_clear( pizza, old_clear );
*/
            GdkEventExpose gdk_event;
            gdk_event.type = GDK_EXPOSE;
            gdk_event.window = GTK_PIZZA(m_wxwindow)->bin_window;
            gdk_event.count = 0;
            gdk_event.area.x = 0;
            gdk_event.area.y = 0;
            gdk_event.area.width = m_wxwindow->allocation.width;
            gdk_event.area.height = m_wxwindow->allocation.height;
            gtk_window_expose_callback( m_wxwindow, &gdk_event, (wxWindow *)this );
        }
        else
        {
            gtk_widget_draw( m_widget, (GdkRectangle*) NULL );
        }
    }
    else
    {

        if (m_wxwindow)
        {
/*
            GtkPizza *pizza = GTK_PIZZA(m_wxwindow);
            gboolean old_clear = pizza->clear_on_draw;
            gtk_pizza_set_clear( pizza, FALSE );

            GdkRectangle gdk_rect;
            gdk_rect.x = rect->x;
            gdk_rect.y = rect->y;
            gdk_rect.width = rect->width;
            gdk_rect.height = rect->height;
            gtk_widget_draw( m_wxwindow, &gdk_rect );
            gtk_window_draw_callback( m_wxwindow, &gdk_rect, this );

            gtk_pizza_set_clear( pizza, old_clear );
*/
            GdkEventExpose gdk_event;
            gdk_event.type = GDK_EXPOSE;
            gdk_event.window = GTK_PIZZA(m_wxwindow)->bin_window;
            gdk_event.count = 0;
            gdk_event.area.x = rect->x;
            gdk_event.area.y = rect->y;
            gdk_event.area.width = rect->width;
            gdk_event.area.height = rect->height;
            gtk_window_expose_callback( m_wxwindow, &gdk_event, (wxWindow *)this );
        }
        else
        {
            GdkRectangle gdk_rect;
            gdk_rect.x = rect->x;
            gdk_rect.y = rect->y;
            gdk_rect.width = rect->width;
            gdk_rect.height = rect->height;
            gtk_widget_draw( m_widget, &gdk_rect );
        }
    }
}

void wxWindowGTK::Clear()
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    if (!m_widget->window) return;

    if (m_wxwindow && m_wxwindow->window)
    {
//        gdk_window_clear( m_wxwindow->window );
    }
}

#if wxUSE_TOOLTIPS
void wxWindowGTK::DoSetToolTip( wxToolTip *tip )
{
    wxWindowBase::DoSetToolTip(tip);

    if (m_tooltip)
        m_tooltip->Apply( (wxWindow *)this );
}

void wxWindowGTK::ApplyToolTip( GtkTooltips *tips, const wxChar *tip )
{
    gtk_tooltips_set_tip( tips, GetConnectWidget(), wxConvCurrent->cWX2MB(tip), (gchar*) NULL );
}
#endif // wxUSE_TOOLTIPS

bool wxWindowGTK::SetBackgroundColour( const wxColour &colour )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, wxT("invalid window") );

    if (!wxWindowBase::SetBackgroundColour(colour))
    {
        // don't leave if the GTK widget has just
        // been realized
        if (!m_delayedBackgroundColour) return FALSE;
    }

    GdkWindow *window = (GdkWindow*) NULL;
    if (m_wxwindow)
        window = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        window = GetConnectWidget()->window;

    if (!window)
    {
        // indicate that a new style has been set
        // but it couldn't get applied as the
        // widget hasn't been realized yet.
        m_delayedBackgroundColour = TRUE;
    }

    if ((m_wxwindow) &&
        (m_wxwindow->window) &&
        (m_backgroundColour != wxSystemSettings::GetSystemColour(wxSYS_COLOUR_BTNFACE)))
    {
        /* wxMSW doesn't clear the window here. I don't do that either to
          provide compatibility. call Clear() to do the job. */

        m_backgroundColour.CalcPixel( gdk_window_get_colormap( window ) );
        gdk_window_set_background( window, m_backgroundColour.GetColor() );
    }

    ApplyWidgetStyle();

    return TRUE;
}

bool wxWindowGTK::SetForegroundColour( const wxColour &colour )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, wxT("invalid window") );

    if (!wxWindowBase::SetForegroundColour(colour))
    {
        // don't leave if the GTK widget has just
        // been realized
        if (!m_delayedForegroundColour) return FALSE;
    }

    GdkWindow *window = (GdkWindow*) NULL;
    if (m_wxwindow)
        window = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        window = GetConnectWidget()->window;

    if (!window)
    {
        // indicate that a new style has been set
        // but it couldn't get applied as the
        // widget hasn't been realized yet.
        m_delayedForegroundColour = TRUE;
    }

    ApplyWidgetStyle();

    return TRUE;
}

GtkStyle *wxWindowGTK::GetWidgetStyle()
{
    if (m_widgetStyle)
    {
        GtkStyle *remake = gtk_style_copy( m_widgetStyle );
#ifdef __WXGTK20__
        /* FIXME: is this necessary? */
        _G_TYPE_IGC(remake, GtkObjectClass) = _G_TYPE_IGC(m_widgetStyle, GtkObjectClass);
#else
        remake->klass = m_widgetStyle->klass;
#endif

        gtk_style_unref( m_widgetStyle );
        m_widgetStyle = remake;
    }
    else
    {
        GtkStyle *def = gtk_rc_get_style( m_widget );

        if (!def)
            def = gtk_widget_get_default_style();

        m_widgetStyle = gtk_style_copy( def );
#ifdef __WXGTK20__
        /* FIXME: is this necessary? */
        _G_TYPE_IGC(m_widgetStyle, GtkObjectClass) = _G_TYPE_IGC(def, GtkObjectClass);
#else
        m_widgetStyle->klass = def->klass;
#endif
    }

    return m_widgetStyle;
}

void wxWindowGTK::SetWidgetStyle()
{
#if DISABLE_STYLE_IF_BROKEN_THEM
    if (m_widget->style->engine_data)
    {
        static bool s_warningPrinted = FALSE;
        if (!s_warningPrinted)
        {
            printf( "wxWindows warning: Widget styles disabled due to buggy GTK theme.\n" );
            s_warningPrinted = TRUE;
        }
        m_widgetStyle = m_widget->style;
        return;
    }
#endif

    GtkStyle *style = GetWidgetStyle();

    if (m_font != wxSystemSettings::GetSystemFont( wxSYS_DEFAULT_GUI_FONT ))
    {
        gdk_font_unref( style->font );
        style->font = gdk_font_ref( m_font.GetInternalFont( 1.0 ) );
    }

    if (m_foregroundColour.Ok())
    {
        m_foregroundColour.CalcPixel( gtk_widget_get_colormap( m_widget ) );
        if (m_foregroundColour != wxSystemSettings::GetSystemColour(wxSYS_COLOUR_BTNTEXT))
        {
            style->fg[GTK_STATE_NORMAL] = *m_foregroundColour.GetColor();
            style->fg[GTK_STATE_PRELIGHT] = *m_foregroundColour.GetColor();
            style->fg[GTK_STATE_ACTIVE] = *m_foregroundColour.GetColor();
        }
        else
        {
            // Try to restore the gtk default style.  This is still a little
            // oversimplified for what is probably really needed here for controls
            // other than buttons, but is better than not being able to (re)set a
            // control's foreground colour to *wxBLACK -- RL
            GtkStyle *def = gtk_rc_get_style( m_widget );

            if (!def)
                def = gtk_widget_get_default_style();

            style->fg[GTK_STATE_NORMAL] = def->fg[GTK_STATE_NORMAL];
            style->fg[GTK_STATE_PRELIGHT] = def->fg[GTK_STATE_PRELIGHT];
            style->fg[GTK_STATE_ACTIVE] = def->fg[GTK_STATE_ACTIVE];
        }
    }

    if (m_backgroundColour.Ok())
    {
        m_backgroundColour.CalcPixel( gtk_widget_get_colormap( m_widget ) );
        if (m_backgroundColour != wxSystemSettings::GetSystemColour(wxSYS_COLOUR_BTNFACE))
        {
            style->bg[GTK_STATE_NORMAL] = *m_backgroundColour.GetColor();
            style->base[GTK_STATE_NORMAL] = *m_backgroundColour.GetColor();
            style->bg[GTK_STATE_PRELIGHT] = *m_backgroundColour.GetColor();
            style->base[GTK_STATE_PRELIGHT] = *m_backgroundColour.GetColor();
            style->bg[GTK_STATE_ACTIVE] = *m_backgroundColour.GetColor();
            style->base[GTK_STATE_ACTIVE] = *m_backgroundColour.GetColor();
            style->bg[GTK_STATE_INSENSITIVE] = *m_backgroundColour.GetColor();
            style->base[GTK_STATE_INSENSITIVE] = *m_backgroundColour.GetColor();
        }
        else
        {
            // Try to restore the gtk default style.  This is still a little
            // oversimplified for what is probably really needed here for controls
            // other than buttons, but is better than not being able to (re)set a
            // control's background colour to default grey and means resetting a
            // button to wxSYS_COLOUR_BTNFACE will restore its usual highlighting
            // behavior -- RL
            GtkStyle *def = gtk_rc_get_style( m_widget );

            if (!def)
                def = gtk_widget_get_default_style();

            style->bg[GTK_STATE_NORMAL] = def->bg[GTK_STATE_NORMAL];
            style->base[GTK_STATE_NORMAL] = def->base[GTK_STATE_NORMAL];
            style->bg[GTK_STATE_PRELIGHT] = def->bg[GTK_STATE_PRELIGHT];
            style->base[GTK_STATE_PRELIGHT] = def->base[GTK_STATE_PRELIGHT];
            style->bg[GTK_STATE_ACTIVE] = def->bg[GTK_STATE_ACTIVE];
            style->base[GTK_STATE_ACTIVE] = def->base[GTK_STATE_ACTIVE];
            style->bg[GTK_STATE_INSENSITIVE] = def->bg[GTK_STATE_INSENSITIVE];
            style->base[GTK_STATE_INSENSITIVE] = def->base[GTK_STATE_INSENSITIVE];
        }
    }
}

void wxWindowGTK::ApplyWidgetStyle()
{
}

//-----------------------------------------------------------------------------
// Pop-up menu stuff
//-----------------------------------------------------------------------------

#if wxUSE_MENUS_NATIVE

static void gtk_pop_hide_callback( GtkWidget *WXUNUSED(widget), bool* is_waiting  )
{
    *is_waiting = FALSE;
}

static void SetInvokingWindow( wxMenu *menu, wxWindowGTK *win )
{
    menu->SetInvokingWindow( win );
    wxMenuItemList::Node *node = menu->GetMenuItems().GetFirst();
    while (node)
    {
        wxMenuItem *menuitem = node->GetData();
        if (menuitem->IsSubMenu())
        {
            SetInvokingWindow( menuitem->GetSubMenu(), win );
        }

        node = node->GetNext();
    }
}

static gint gs_pop_x = 0;
static gint gs_pop_y = 0;

static void pop_pos_callback( GtkMenu * WXUNUSED(menu),
                              gint *x, gint *y,
                              wxWindowGTK *win )
{
    win->ClientToScreen( &gs_pop_x, &gs_pop_y );
    *x = gs_pop_x;
    *y = gs_pop_y;
}

bool wxWindowGTK::DoPopupMenu( wxMenu *menu, int x, int y )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, wxT("invalid window") );

    wxCHECK_MSG( menu != NULL, FALSE, wxT("invalid popup-menu") );

    SetInvokingWindow( menu, this );

    menu->UpdateUI();

    gs_pop_x = x;
    gs_pop_y = y;

    bool is_waiting = TRUE;

    gtk_signal_connect( GTK_OBJECT(menu->m_menu), "hide",
      GTK_SIGNAL_FUNC(gtk_pop_hide_callback), (gpointer)&is_waiting );

    gtk_menu_popup(
                  GTK_MENU(menu->m_menu),
                  (GtkWidget *) NULL,          // parent menu shell
                  (GtkWidget *) NULL,          // parent menu item
                  (GtkMenuPositionFunc) pop_pos_callback,
                  (gpointer) this,             // client data
                  0,                           // button used to activate it
                  gs_timeLastClick             // the time of activation
                );

    while (is_waiting)
    {
        while (gtk_events_pending())
            gtk_main_iteration();
    }

    return TRUE;
}

#endif // wxUSE_MENUS_NATIVE

#if wxUSE_DRAG_AND_DROP

void wxWindowGTK::SetDropTarget( wxDropTarget *dropTarget )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    GtkWidget *dnd_widget = GetConnectWidget();

    if (m_dropTarget) m_dropTarget->UnregisterWidget( dnd_widget );

    if (m_dropTarget) delete m_dropTarget;
    m_dropTarget = dropTarget;

    if (m_dropTarget) m_dropTarget->RegisterWidget( dnd_widget );
}

#endif // wxUSE_DRAG_AND_DROP

GtkWidget* wxWindowGTK::GetConnectWidget()
{
    GtkWidget *connect_widget = m_widget;
    if (m_wxwindow) connect_widget = m_wxwindow;

    return connect_widget;
}

bool wxWindowGTK::IsOwnGtkWindow( GdkWindow *window )
{
    if (m_wxwindow)
        return (window == GTK_PIZZA(m_wxwindow)->bin_window);

    return (window == m_widget->window);
}

bool wxWindowGTK::SetFont( const wxFont &font )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, wxT("invalid window") );

    if (!wxWindowBase::SetFont(font))
    {
        return FALSE;
    }

    wxColour sysbg = wxSystemSettings::GetSystemColour( wxSYS_COLOUR_BTNFACE );
    if ( sysbg == m_backgroundColour )
    {
        m_backgroundColour = wxNullColour;
        ApplyWidgetStyle();
        m_backgroundColour = sysbg;
    }
    else
    {
        ApplyWidgetStyle();
    }

    return TRUE;
}

void wxWindowGTK::CaptureMouse()
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    GdkWindow *window = (GdkWindow*) NULL;
    if (m_wxwindow)
        window = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        window = GetConnectWidget()->window;

    wxCHECK_RET( window, _T("CaptureMouse() failed") );

    wxCursor* cursor = & m_cursor;
    if (!cursor->Ok())
        cursor = wxSTANDARD_CURSOR;

    gdk_pointer_grab( window, FALSE,
                      (GdkEventMask)
                         (GDK_BUTTON_PRESS_MASK |
                          GDK_BUTTON_RELEASE_MASK |
                          GDK_POINTER_MOTION_HINT_MASK |
                          GDK_POINTER_MOTION_MASK),
                      (GdkWindow *) NULL,
                      cursor->GetCursor(),
                      (guint32)GDK_CURRENT_TIME );
    g_captureWindow = this;
    g_captureWindowHasMouse = TRUE;
}

void wxWindowGTK::ReleaseMouse()
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    wxCHECK_RET( g_captureWindow, wxT("can't release mouse - not captured") );

    GdkWindow *window = (GdkWindow*) NULL;
    if (m_wxwindow)
        window = GTK_PIZZA(m_wxwindow)->bin_window;
    else
        window = GetConnectWidget()->window;

    if (!window)
        return;

    gdk_pointer_ungrab ( (guint32)GDK_CURRENT_TIME );
    g_captureWindow = (wxWindowGTK*) NULL;
}

/* static */
wxWindow *wxWindowBase::GetCapture()
{
    return (wxWindow *)g_captureWindow;
}

bool wxWindowGTK::IsRetained() const
{
    return FALSE;
}

void wxWindowGTK::SetScrollbar( int orient, int pos, int thumbVisible,
      int range, bool refresh )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    wxCHECK_RET( m_wxwindow != NULL, wxT("window needs client area for scrolling") );

    m_hasScrolling = TRUE;

    if (orient == wxHORIZONTAL)
    {
        float fpos = (float)pos;
        float frange = (float)range;
        float fthumb = (float)thumbVisible;
        if (fpos > frange-fthumb) fpos = frange-fthumb;
        if (fpos < 0.0) fpos = 0.0;

        if ((fabs(frange-m_hAdjust->upper) < 0.2) &&
            (fabs(fthumb-m_hAdjust->page_size) < 0.2))
        {
            SetScrollPos( orient, pos, refresh );
            return;
        }

        m_oldHorizontalPos = fpos;

        m_hAdjust->lower = 0.0;
        m_hAdjust->upper = frange;
        m_hAdjust->value = fpos;
        m_hAdjust->step_increment = 1.0;
        m_hAdjust->page_increment = (float)(wxMax(fthumb,0));
        m_hAdjust->page_size = fthumb;
    }
    else
    {
        float fpos = (float)pos;
        float frange = (float)range;
        float fthumb = (float)thumbVisible;
        if (fpos > frange-fthumb) fpos = frange-fthumb;
        if (fpos < 0.0) fpos = 0.0;

        if ((fabs(frange-m_vAdjust->upper) < 0.2) &&
            (fabs(fthumb-m_vAdjust->page_size) < 0.2))
        {
            SetScrollPos( orient, pos, refresh );
            return;
        }

        m_oldVerticalPos = fpos;

        m_vAdjust->lower = 0.0;
        m_vAdjust->upper = frange;
        m_vAdjust->value = fpos;
        m_vAdjust->step_increment = 1.0;
        m_vAdjust->page_increment = (float)(wxMax(fthumb,0));
        m_vAdjust->page_size = fthumb;
    }

    if (orient == wxHORIZONTAL)
        gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "changed" );
    else
        gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "changed" );
}

void wxWindowGTK::SetScrollPos( int orient, int pos, bool WXUNUSED(refresh) )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    wxCHECK_RET( m_wxwindow != NULL, wxT("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
    {
        float fpos = (float)pos;
        if (fpos > m_hAdjust->upper - m_hAdjust->page_size) fpos = m_hAdjust->upper - m_hAdjust->page_size;
        if (fpos < 0.0) fpos = 0.0;
        m_oldHorizontalPos = fpos;

        if (fabs(fpos-m_hAdjust->value) < 0.2) return;
        m_hAdjust->value = fpos;
    }
    else
    {
        float fpos = (float)pos;
        if (fpos > m_vAdjust->upper - m_vAdjust->page_size) fpos = m_vAdjust->upper - m_vAdjust->page_size;
        if (fpos < 0.0) fpos = 0.0;
        m_oldVerticalPos = fpos;

        if (fabs(fpos-m_vAdjust->value) < 0.2) return;
        m_vAdjust->value = fpos;
    }

    if (m_wxwindow->window)
    {
        if (orient == wxHORIZONTAL)
        {
            gtk_signal_disconnect_by_func( GTK_OBJECT(m_hAdjust),
                (GtkSignalFunc) gtk_window_hscroll_callback, (gpointer) this );

            gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "value_changed" );

            gtk_signal_connect( GTK_OBJECT(m_hAdjust), "value_changed",
                (GtkSignalFunc) gtk_window_hscroll_callback, (gpointer) this );
        }
        else
        {
            gtk_signal_disconnect_by_func( GTK_OBJECT(m_vAdjust),
                (GtkSignalFunc) gtk_window_vscroll_callback, (gpointer) this );

            gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "value_changed" );

            gtk_signal_connect( GTK_OBJECT(m_vAdjust), "value_changed",
                (GtkSignalFunc) gtk_window_vscroll_callback, (gpointer) this );
        }
    }
}

int wxWindowGTK::GetScrollThumb( int orient ) const
{
    wxCHECK_MSG( m_widget != NULL, 0, wxT("invalid window") );

    wxCHECK_MSG( m_wxwindow != NULL, 0, wxT("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
        return (int)(m_hAdjust->page_size+0.5);
    else
        return (int)(m_vAdjust->page_size+0.5);
}

int wxWindowGTK::GetScrollPos( int orient ) const
{
    wxCHECK_MSG( m_widget != NULL, 0, wxT("invalid window") );

    wxCHECK_MSG( m_wxwindow != NULL, 0, wxT("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
        return (int)(m_hAdjust->value+0.5);
    else
        return (int)(m_vAdjust->value+0.5);
}

int wxWindowGTK::GetScrollRange( int orient ) const
{
    wxCHECK_MSG( m_widget != NULL, 0, wxT("invalid window") );

    wxCHECK_MSG( m_wxwindow != NULL, 0, wxT("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
        return (int)(m_hAdjust->upper+0.5);
    else
        return (int)(m_vAdjust->upper+0.5);
}

void wxWindowGTK::ScrollWindow( int dx, int dy, const wxRect* WXUNUSED(rect) )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid window") );

    wxCHECK_RET( m_wxwindow != NULL, wxT("window needs client area for scrolling") );

    if ((dx == 0) && (dy == 0)) return;

    m_clipPaintRegion = TRUE;
    gtk_pizza_scroll( GTK_PIZZA(m_wxwindow), -dx, -dy );
    m_clipPaintRegion = FALSE;

/*
    if (m_children.GetCount() > 0)
    {
        gtk_pizza_scroll( GTK_PIZZA(m_wxwindow), -dx, -dy );
    }
    else
    {
        GtkPizza *pizza = GTK_PIZZA(m_wxwindow);

        pizza->xoffset -= dx;
        pizza->yoffset -= dy;

        GdkGC *m_scrollGC = gdk_gc_new( pizza->bin_window );
        gdk_gc_set_exposures( m_scrollGC, TRUE );

        int cw = 0;
        int ch = 0;
        GetClientSize( &cw, &ch );
        int w = cw - abs(dx);
        int h = ch - abs(dy);

        if ((h < 0) || (w < 0))
        {
            Refresh();
        }
        else
        {
            int s_x = 0;
            int s_y = 0;
            if (dx < 0) s_x = -dx;
            if (dy < 0) s_y = -dy;
            int d_x = 0;
            int d_y = 0;
            if (dx > 0) d_x = dx;
            if (dy > 0) d_y = dy;

            gdk_window_copy_area( pizza->bin_window, m_scrollGC, d_x, d_y,
                pizza->bin_window, s_x, s_y, w, h );

            wxRect rect;
            if (dx < 0) rect.x = cw+dx; else rect.x = 0;
            if (dy < 0) rect.y = ch+dy; else rect.y = 0;
            if (dy != 0) rect.width = cw; else rect.width = abs(dx);
            if (dx != 0) rect.height = ch; else rect.height = abs(dy);

            Refresh( TRUE, &rect );
        }

        gdk_gc_unref( m_scrollGC );
    }
*/
}

// Find the wxWindow at the current mouse position, also returning the mouse
// position.
wxWindow* wxFindWindowAtPointer(wxPoint& pt)
{
    pt = wxGetMousePosition();
    wxWindow* found = wxFindWindowAtPoint(pt);
    return found;
}

// Get the current mouse position.
wxPoint wxGetMousePosition()
{
  /* This crashes when used within wxHelpContext,
     so we have to use the X-specific implementation below.
    gint x, y;
    GdkModifierType *mask;
    (void) gdk_window_get_pointer(NULL, &x, &y, mask);

    return wxPoint(x, y);
  */

    int x, y;
    GdkWindow* windowAtPtr = gdk_window_at_pointer(& x, & y);
    if (!windowAtPtr)
      return wxPoint(-999, -999);

    Display *display = GDK_WINDOW_XDISPLAY(windowAtPtr);
    Window rootWindow = RootWindowOfScreen (DefaultScreenOfDisplay(display));
    Window rootReturn, childReturn;
    int rootX, rootY, winX, winY;
    unsigned int maskReturn;

    XQueryPointer (display,
           rootWindow,
           &rootReturn,
                   &childReturn,
                   &rootX, &rootY, &winX, &winY, &maskReturn);
    return wxPoint(rootX, rootY);

}

