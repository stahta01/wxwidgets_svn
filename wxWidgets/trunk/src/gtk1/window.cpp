/////////////////////////////////////////////////////////////////////////////
// Name:        window.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
    #pragma implementation "window.h"
#endif

#include "wx/defs.h"
#include "wx/window.h"
#include "wx/dc.h"
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

#include "wx/menu.h"
#include "wx/statusbr.h"
#include "wx/intl.h"
#include "wx/settings.h"
#include "wx/log.h"

#include <math.h>

#include "gdk/gdk.h"
#include "gtk/gtk.h"
#include "gdk/gdkprivate.h"
#include "gdk/gdkkeysyms.h"
#include "wx/gtk/win_gtk.h"

#include "gdk/gdkx.h"

//-----------------------------------------------------------------------------
// documentation on internals
//-----------------------------------------------------------------------------

/*
   I have been asked several times about writing some documentation about
   the GTK port of wxWindows, especially its internal structures. Obviously,
   you cannot understand wxGTK without knowing a little about the GTK, but
   some more information about what the wxWindow, which is the base class
   for all other window classes, does seems required as well.

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
   it has to handle the client area widget (of the type GtkMyFixed, defined in
   win_gtk.c), but there could be any number of widgets, handled by a class
   The common rule for all windows is only, that the widget that interacts with
   the rest of GTK must be referenced in m_widget and all other widgets must be
   children of this widget on the GTK level. The top-most widget, which also
   represents the client area, must be in the m_wxwindow field and must be of
   the type GtkMyFixed.

   As I said, the window classes that display a GTK native widget only have
   one widget, so in the case of e.g. the wxButton class m_widget holds a
   pointer to a GtkButton widget. But windows with client areas (for drawing
   and children) have a m_widget field that is a pointer to a GtkScrolled-
   Window and a m_wxwindow field that is pointer to a GtkMyFixed and this
   one is (in the GTK sense) a child of the GtkScrolledWindow.

   If the m_wxwindow field is set, then all input to this widget is inter-
   cepted and sent to the wxWindows class. If not, all input to the widget
   that gets pointed to by m_widget gets intercepted and sent to the class.

*/

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern wxList     wxPendingDelete;
extern bool       g_blockEventsOnDrag;
extern bool       g_blockEventsOnScroll;
extern wxCursor   g_globalCursor;
static wxWindow  *g_captureWindow = (wxWindow*) NULL;
static wxWindow  *g_focusWindow = (wxWindow*) NULL;

/* hack: we need something to pass to gtk_menu_popup, so we store the time of
   the last click here */
static guint32 gs_timeLastClick = 0;

//-----------------------------------------------------------------------------
// debug
//-----------------------------------------------------------------------------

#ifdef __WXDEBUG__

static gint gtk_debug_focus_in_callback( GtkWidget *WXUNUSED(widget),
                                         GdkEvent *WXUNUSED(event),
                                         const wxChar *name )
{
/*
    static bool s_done = FALSE;
    if ( !s_done )
    {
        wxLog::AddTraceMask("focus");
        s_done = TRUE;
    }
    wxLogTrace(_T("FOCUS NOW AT: %s"), name);
*/

    return FALSE;
}

void debug_focus_in( GtkWidget* widget, const wxChar* name, const wxChar *window )
{
    wxString tmp = name;
    tmp += _T(" FROM ");
    tmp += window;

    wxChar *s = new wxChar[tmp.Length()+1];

    wxStrcpy( s, tmp );

    gtk_signal_connect( GTK_OBJECT(widget), "focus_in_event",
      GTK_SIGNAL_FUNC(gtk_debug_focus_in_callback), (gpointer)s );
}

#endif // Debug

//-----------------------------------------------------------------------------
// missing gdk functions
//-----------------------------------------------------------------------------

void
gdk_window_warp_pointer (GdkWindow      *window,
                         gint            x,
                         gint            y)
{
  GdkWindowPrivate *priv;

  if (!window)
    window = (GdkWindow*) &gdk_root_parent;

  priv = (GdkWindowPrivate*) window;

  if (!priv->destroyed)
  {
      XWarpPointer (priv->xdisplay,
                    None,              /* not source window -> move from anywhere */
                    priv->xwindow,  /* dest window */
                    0, 0, 0, 0,        /* not source window -> move from anywhere */
                    x, y );
  }
}

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// key event conversion routines
//-----------------------------------------------------------------------------

#if (GTK_MINOR_VERSION == 0)
/* these functions are copied verbatim from GTK 1.2 */
static void
gdkx_XConvertCase (KeySym symbol,
                   KeySym *lower,
                   KeySym *upper)
{
  register KeySym sym = symbol;

  g_return_if_fail (lower != NULL);
  g_return_if_fail (upper != NULL);

  *lower = sym;
  *upper = sym;

  switch (sym >> 8)
    {
#if        defined (GDK_A) && defined (GDK_Ooblique)
    case 0: /* Latin 1 */
      if ((sym >= GDK_A) && (sym <= GDK_Z))
        *lower += (GDK_a - GDK_A);
      else if ((sym >= GDK_a) && (sym <= GDK_z))
        *upper -= (GDK_a - GDK_A);
      else if ((sym >= GDK_Agrave) && (sym <= GDK_Odiaeresis))
        *lower += (GDK_agrave - GDK_Agrave);
      else if ((sym >= GDK_agrave) && (sym <= GDK_odiaeresis))
        *upper -= (GDK_agrave - GDK_Agrave);
      else if ((sym >= GDK_Ooblique) && (sym <= GDK_Thorn))
        *lower += (GDK_oslash - GDK_Ooblique);
      else if ((sym >= GDK_oslash) && (sym <= GDK_thorn))
        *upper -= (GDK_oslash - GDK_Ooblique);
      break;
#endif        /* LATIN1 */

#if        defined (GDK_Aogonek) && defined (GDK_tcedilla)
    case 1: /* Latin 2 */
      /* Assume the KeySym is a legal value (ignore discontinuities) */
      if (sym == GDK_Aogonek)
        *lower = GDK_aogonek;
      else if (sym >= GDK_Lstroke && sym <= GDK_Sacute)
        *lower += (GDK_lstroke - GDK_Lstroke);
      else if (sym >= GDK_Scaron && sym <= GDK_Zacute)
        *lower += (GDK_scaron - GDK_Scaron);
      else if (sym >= GDK_Zcaron && sym <= GDK_Zabovedot)
        *lower += (GDK_zcaron - GDK_Zcaron);
      else if (sym == GDK_aogonek)
        *upper = GDK_Aogonek;
      else if (sym >= GDK_lstroke && sym <= GDK_sacute)
        *upper -= (GDK_lstroke - GDK_Lstroke);
      else if (sym >= GDK_scaron && sym <= GDK_zacute)
        *upper -= (GDK_scaron - GDK_Scaron);
      else if (sym >= GDK_zcaron && sym <= GDK_zabovedot)
        *upper -= (GDK_zcaron - GDK_Zcaron);
      else if (sym >= GDK_Racute && sym <= GDK_Tcedilla)
        *lower += (GDK_racute - GDK_Racute);
      else if (sym >= GDK_racute && sym <= GDK_tcedilla)
        *upper -= (GDK_racute - GDK_Racute);
      break;
#endif        /* LATIN2 */

#if        defined (GDK_Hstroke) && defined (GDK_Cabovedot)
    case 2: /* Latin 3 */
      /* Assume the KeySym is a legal value (ignore discontinuities) */
      if (sym >= GDK_Hstroke && sym <= GDK_Hcircumflex)
        *lower += (GDK_hstroke - GDK_Hstroke);
      else if (sym >= GDK_Gbreve && sym <= GDK_Jcircumflex)
        *lower += (GDK_gbreve - GDK_Gbreve);
      else if (sym >= GDK_hstroke && sym <= GDK_hcircumflex)
        *upper -= (GDK_hstroke - GDK_Hstroke);
      else if (sym >= GDK_gbreve && sym <= GDK_jcircumflex)
        *upper -= (GDK_gbreve - GDK_Gbreve);
      else if (sym >= GDK_Cabovedot && sym <= GDK_Scircumflex)
        *lower += (GDK_cabovedot - GDK_Cabovedot);
      else if (sym >= GDK_cabovedot && sym <= GDK_scircumflex)
        *upper -= (GDK_cabovedot - GDK_Cabovedot);
      break;
#endif        /* LATIN3 */

#if        defined (GDK_Rcedilla) && defined (GDK_Amacron)
    case 3: /* Latin 4 */
      /* Assume the KeySym is a legal value (ignore discontinuities) */
      if (sym >= GDK_Rcedilla && sym <= GDK_Tslash)
        *lower += (GDK_rcedilla - GDK_Rcedilla);
      else if (sym >= GDK_rcedilla && sym <= GDK_tslash)
        *upper -= (GDK_rcedilla - GDK_Rcedilla);
      else if (sym == GDK_ENG)
        *lower = GDK_eng;
      else if (sym == GDK_eng)
        *upper = GDK_ENG;
      else if (sym >= GDK_Amacron && sym <= GDK_Umacron)
        *lower += (GDK_amacron - GDK_Amacron);
      else if (sym >= GDK_amacron && sym <= GDK_umacron)
        *upper -= (GDK_amacron - GDK_Amacron);
      break;
#endif        /* LATIN4 */

#if        defined (GDK_Serbian_DJE) && defined (GDK_Cyrillic_yu)
    case 6: /* Cyrillic */
      /* Assume the KeySym is a legal value (ignore discontinuities) */
      if (sym >= GDK_Serbian_DJE && sym <= GDK_Serbian_DZE)
        *lower -= (GDK_Serbian_DJE - GDK_Serbian_dje);
      else if (sym >= GDK_Serbian_dje && sym <= GDK_Serbian_dze)
        *upper += (GDK_Serbian_DJE - GDK_Serbian_dje);
      else if (sym >= GDK_Cyrillic_YU && sym <= GDK_Cyrillic_HARDSIGN)
        *lower -= (GDK_Cyrillic_YU - GDK_Cyrillic_yu);
      else if (sym >= GDK_Cyrillic_yu && sym <= GDK_Cyrillic_hardsign)
        *upper += (GDK_Cyrillic_YU - GDK_Cyrillic_yu);
      break;
#endif        /* CYRILLIC */

#if        defined (GDK_Greek_ALPHAaccent) && defined (GDK_Greek_finalsmallsigma)
    case 7: /* Greek */
      /* Assume the KeySym is a legal value (ignore discontinuities) */
      if (sym >= GDK_Greek_ALPHAaccent && sym <= GDK_Greek_OMEGAaccent)
        *lower += (GDK_Greek_alphaaccent - GDK_Greek_ALPHAaccent);
      else if (sym >= GDK_Greek_alphaaccent && sym <= GDK_Greek_omegaaccent &&
               sym != GDK_Greek_iotaaccentdieresis &&
               sym != GDK_Greek_upsilonaccentdieresis)
        *upper -= (GDK_Greek_alphaaccent - GDK_Greek_ALPHAaccent);
      else if (sym >= GDK_Greek_ALPHA && sym <= GDK_Greek_OMEGA)
        *lower += (GDK_Greek_alpha - GDK_Greek_ALPHA);
      else if (sym >= GDK_Greek_alpha && sym <= GDK_Greek_omega &&
               sym != GDK_Greek_finalsmallsigma)
        *upper -= (GDK_Greek_alpha - GDK_Greek_ALPHA);
      break;
#endif        /* GREEK */
    }
}

static guint
gdk_keyval_to_upper (guint          keyval)
{
  if (keyval)
    {
      KeySym lower_val = 0;
      KeySym upper_val = 0;

      gdkx_XConvertCase (keyval, &lower_val, &upper_val);
      return upper_val;
    }
  return 0;
}
#endif

static long map_to_unmodified_wx_keysym( KeySym keysym )
{
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
            if (keysym <= 0xFF)
            {
                guint upper = gdk_keyval_to_upper( keysym );
                keysym = (upper != 0 ? upper : keysym ); /* to be MSW compatible */
                key_code = keysym;
            }
        }
    }

    return (key_code);
}

static long map_to_wx_keysym( KeySym keysym )
{
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
            if (keysym <= 0xFF)
            {
                key_code = keysym;
            }
        }
    }

    return (key_code);
}

//-----------------------------------------------------------------------------
// local code (see below)
//-----------------------------------------------------------------------------

#if (GTK_MINOR_VERSION > 0)

static void draw_frame( GtkWidget *widget, wxWindow *win )
{
    if (!win->m_hasVMT)
        return;

    int dw = 0;
    int dh = 0;

    if (win->HasScrolling())
    {
        GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(widget);
        GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(widget)->klass );

/*
            GtkWidget *hscrollbar = scroll_window->hscrollbar;
            GtkWidget *vscrollbar = scroll_window->vscrollbar;

            we use this instead:  range.slider_width = 11 + 2*2pts edge
*/

        if (scroll_window->vscrollbar_visible)
        {
            dw += 15;   /* dw += vscrollbar->allocation.width; */
            dw += scroll_class->scrollbar_spacing;
        }

        if (scroll_window->hscrollbar_visible)
        {
            dh += 15;   /* dh += hscrollbar->allocation.height; */
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
                         win->m_width-dw, win->m_height-dh );
        return;
    }

    if (win->HasFlag(wxSUNKEN_BORDER))
    {
        gtk_draw_shadow( widget->style,
                         widget->window,
                         GTK_STATE_NORMAL,
                         GTK_SHADOW_IN,
                         dx, dy,
                         win->m_width-dw, win->m_height-dh );
        return;
    }
}

//-----------------------------------------------------------------------------
// "expose_event" of m_widget
//-----------------------------------------------------------------------------

static void gtk_window_own_expose_callback( GtkWidget *widget, GdkEventExpose *gdk_event, wxWindow *win )
{
    if (gdk_event->count > 0) return;
    draw_frame( widget, win );
}

//-----------------------------------------------------------------------------
// "draw" of m_wxwindow
//-----------------------------------------------------------------------------

static void gtk_window_own_draw_callback( GtkWidget *widget, GdkRectangle *WXUNUSED(rect), wxWindow *win )
{
    draw_frame( widget, win );
}

#endif // GTK_MINOR_VERSION > 0

//-----------------------------------------------------------------------------
// "expose_event" of m_wxwindow
//-----------------------------------------------------------------------------

static void gtk_window_expose_callback( GtkWidget *WXUNUSED(widget), GdkEventExpose *gdk_event, wxWindow *win )
{
    if (!win->m_hasVMT)
        return;

    if (gdk_event->window != win->m_wxwindow->window)
        return;

    win->GetUpdateRegion().Union( gdk_event->area.x,
                                  gdk_event->area.y,
                                  gdk_event->area.width,
                                  gdk_event->area.height );

    if (gdk_event->count > 0)
        return;

    wxPaintEvent event( win->GetId() );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );

/*
    wxPrintf( "OnExpose from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( " %d %d %d %d\n", (int)gdk_event->area.x,
                                (int)gdk_event->area.y,
                                (int)gdk_event->area.width,
                                (int)gdk_event->area.height );
*/

    win->GetUpdateRegion().Clear();
}

//-----------------------------------------------------------------------------
// "draw" of m_wxwindow
//-----------------------------------------------------------------------------

static void gtk_window_draw_callback( GtkWidget *WXUNUSED(widget), GdkRectangle *rect, wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT)
        return;

    win->GetUpdateRegion().Union( rect->x, rect->y,
                                  rect->width, rect->height );

    wxPaintEvent event( win->GetId() );
    event.SetEventObject( win );

/*
    wxPrintf( "OnDraw from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
    wxPrintf( " %d %d %d %d\n", (int)rect->x,
                                (int)rect->y,
                                (int)rect->width,
                                (int)rect->height );
*/

    win->GetEventHandler()->ProcessEvent( event );

    win->GetUpdateRegion().Clear();
}

//-----------------------------------------------------------------------------
// "key_press_event" from any window
//-----------------------------------------------------------------------------

static gint gtk_window_key_press_callback( GtkWidget *widget, GdkEventKey *gdk_event, wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

/*
    printf( "KeyDown-ScanCode is: %d.\n", gdk_event->keyval );
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
    int x = 0;
    int y = 0;
    GdkModifierType state;
    if (gdk_event->window) gdk_window_get_pointer(gdk_event->window, &x, &y, &state);

    long key_code = map_to_unmodified_wx_keysym( gdk_event->keyval );

    /* sending unknown key events doesn't really make sense */
    if (key_code == 0) return FALSE;

    bool ret = FALSE;

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

    key_code = map_to_wx_keysym( gdk_event->keyval );

#if wxUSE_ACCEL
    if (!ret)
    {
        wxWindow *ancestor = win;
        while (ancestor)
        {
            int command = ancestor->GetAcceleratorTable()->GetCommand( event );
            if (command != -1)
            {
                wxCommandEvent command_event( wxEVT_COMMAND_MENU_SELECTED, command );
                ret = ancestor->GetEventHandler()->ProcessEvent( command_event );
                break;
            }
            ancestor = ancestor->GetParent();
        }
    }
#endif // wxUSE_ACCEL
    /* wxMSW doesn't send char events with Alt pressed */
    /* Only send wxEVT_CHAR event if not processed yet. Thus, ALT-x
       will only be sent if it is not a menu accelerator. */
    if ((key_code != 0) && ! ret )
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
        ret = (ret || win->GetEventHandler()->ProcessEvent( event2 ));
    }


    /* win is a control: tab can be propagated up */
    if ( (!ret) &&
         ((gdk_event->keyval == GDK_Tab) || (gdk_event->keyval == GDK_ISO_Left_Tab)) &&
         (win->HasFlag(wxTE_PROCESS_TAB) == 0))
    {
        wxNavigationKeyEvent new_event;
        /* GDK reports GDK_ISO_Left_Tab for SHIFT-TAB */
        new_event.SetDirection( (gdk_event->keyval == GDK_Tab) );
        /* CTRL-TAB changes the (parent) window, i.e. switch notebook page */
        new_event.SetWindowChange( (gdk_event->state & GDK_CONTROL_MASK) );
        new_event.SetCurrentFocus( win );
        ret = win->GetEventHandler()->ProcessEvent( new_event );
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
    /* pressing F10 will activate the menu bar of the top frame */
    if ( (!ret) &&
         (gdk_event->keyval == GDK_F10) )
    {
        wxWindow *ancestor = win;
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
                        // doesn't work correctly
            // wxMenu *firstMenu = (wxMenu*) node->Data();
                        // gtk_menu_item_select( GTK_MENU_ITEM(firstMenu->m_owner) );
                        // ret = TRUE;
                        break;
                    }
                }
            }
            ancestor = ancestor->GetParent();
        }
    }
#endif

/*
    Damn, I forgot why this didn't work, but it didn't work.

    // win is a panel: up can be propagated to the panel
    if ((!ret) && (win->m_wxwindow) && (win->m_parent) && (win->m_parent->AcceptsFocus()) &&
        (gdk_event->keyval == GDK_Up))
    {
        win->m_parent->SetFocus();
        ret = TRUE;
    }

    // win is a panel: left/right can be propagated to the panel
    if ((!ret) && (win->m_wxwindow) &&
        ((gdk_event->keyval == GDK_Right) || (gdk_event->keyval == GDK_Left) ||
         (gdk_event->keyval == GDK_Up) || (gdk_event->keyval == GDK_Down)))
    {
        wxNavigationKeyEvent new_event;
        new_event.SetDirection( (gdk_event->keyval == GDK_Right) || (gdk_event->keyval == GDK_Down) );
        new_event.SetCurrentFocus( win );
        ret = win->GetEventHandler()->ProcessEvent( new_event );
    }
*/

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

static gint gtk_window_key_release_callback( GtkWidget *widget, GdkEventKey *gdk_event, wxWindow *win )
{
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

    long key_code = map_to_unmodified_wx_keysym( gdk_event->keyval );

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

//-----------------------------------------------------------------------------
// "button_press_event"
//-----------------------------------------------------------------------------

static gint gtk_window_button_press_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

/*
    wxPrintf( _T("1) OnButtonPress from ") );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( _T(".\n") );
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
            wxPrintf( _T("GrabFocus from ") );
            if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
                wxPrintf( win->GetClassInfo()->GetClassName() );
            wxPrintf( _T(".\n") );
*/

        }
    }

/*
    wxPrintf( _T("2) OnButtonPress from ") );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        wxPrintf( win->GetClassInfo()->GetClassName() );
    wxPrintf( _T(".\n") );
*/

    wxEventType event_type = wxEVT_LEFT_DOWN;

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

    wxMouseEvent event( event_type );
    event.SetTimestamp( gdk_event->time );
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
    event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
    event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
    event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);

    event.m_x = (long)gdk_event->x;
    event.m_y = (long)gdk_event->y;

    // Some control don't have their own X window and thus cannot get
    // any events.

    if (!g_captureWindow)
    {
        wxNode *node = win->GetChildren().First();
        while (node)
        {
            wxWindow *child = (wxWindow*)node->Data();

            if (child->m_isStaticBox)
            {
                // wxStaticBox is transparent in the box itself
                int x = event.m_x;
                int y = event.m_y;
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
                    (child->m_x <= event.m_x) &&
                    (child->m_y <= event.m_y) &&
                    (child->m_x+child->m_width  >= event.m_x) &&
                    (child->m_y+child->m_height >= event.m_y))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }
            }
            node = node->Next();
        }
    }

    event.SetEventObject( win );

    gs_timeLastClick = gdk_event->time;

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

static gint gtk_window_button_release_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxWindow *win )
{
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
    event.m_x = (long)gdk_event->x;
    event.m_y = (long)gdk_event->y;

    // Some control don't have their own X window and thus cannot get
    // any events.

    if (!g_captureWindow)
    {
        wxNode *node = win->GetChildren().First();
        while (node)
        {
            wxWindow *child = (wxWindow*)node->Data();

            if (child->m_isStaticBox)
            {
                // wxStaticBox is transparent in the box itself
                int x = event.m_x;
                int y = event.m_y;
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
                    (child->m_x <= event.m_x) &&
                    (child->m_y <= event.m_y) &&
                    (child->m_x+child->m_width  >= event.m_x) &&
                    (child->m_y+child->m_height >= event.m_y))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }
            }
            node = node->Next();
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

//-----------------------------------------------------------------------------
// "motion_notify_event"
//-----------------------------------------------------------------------------

static gint gtk_window_motion_notify_callback( GtkWidget *widget, GdkEventMotion *gdk_event, wxWindow *win )
{
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
       gdk_event->state = state;
    }

/*
    printf( "OnMotion from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
      printf( win->GetClassInfo()->GetClassName() );
    printf( ".\n" );
*/

    wxMouseEvent event( wxEVT_MOTION );
    event.SetTimestamp( gdk_event->time );
    event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
    event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
    event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
    event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
    event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
    event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
    event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);

    event.m_x = (long)gdk_event->x;
    event.m_y = (long)gdk_event->y;

    // Some control don't have their own X window and thus cannot get
    // any events.

    if (!g_captureWindow)
    {
        wxNode *node = win->GetChildren().First();
        while (node)
        {
            wxWindow *child = (wxWindow*)node->Data();

            if (child->m_isStaticBox)
            {
                // wxStaticBox is transparent in the box itself
                int x = event.m_x;
                int y = event.m_y;
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
                    (child->m_x <= event.m_x) &&
                    (child->m_y <= event.m_y) &&
                    (child->m_x+child->m_width  >= event.m_x) &&
                    (child->m_y+child->m_height >= event.m_y))
                {
                    win = child;
                    event.m_x -= child->m_x;
                    event.m_y -= child->m_y;
                    break;
                }
            }
            node = node->Next();
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

static gint gtk_window_focus_in_callback( GtkWidget *widget, GdkEvent *WXUNUSED(event), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    g_focusWindow = win;

    if (win->m_wxwindow)
    {
        if (GTK_WIDGET_CAN_FOCUS(win->m_wxwindow))
        {
            GTK_WIDGET_SET_FLAGS (win->m_wxwindow, GTK_HAS_FOCUS);
/*
            printf( "SetFocus flag from " );
            if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
                printf( win->GetClassInfo()->GetClassName() );
            printf( ".\n" );
*/
        }
    }


/*
    printf( "OnSetFocus from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
    printf( "   " );
    printf( WXSTRINGCAST win->GetLabel() );
    printf( ".\n" );
*/

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

static gint gtk_window_focus_out_callback( GtkWidget *widget, GdkEvent *WXUNUSED(event), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    if (win->m_wxwindow)
    {
      if (GTK_WIDGET_CAN_FOCUS(win->m_wxwindow))
          GTK_WIDGET_UNSET_FLAGS (win->m_wxwindow, GTK_HAS_FOCUS);
    }

/*
    printf( "OnKillFocus from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
    printf( ".\n" );
*/

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

static gint gtk_window_enter_callback( GtkWidget *widget, GdkEventCrossing *gdk_event, wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

    wxMouseEvent event( wxEVT_ENTER_WINDOW );
#if (GTK_MINOR_VERSION > 0)
    event.SetTimestamp( gdk_event->time );
#endif
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

    event.m_x = (long)x;
    event.m_y = (long)y;

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

static gint gtk_window_leave_callback( GtkWidget *widget, GdkEventCrossing *gdk_event, wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return FALSE;

    if (!win->IsOwnGtkWindow( gdk_event->window )) return FALSE;

    wxMouseEvent event( wxEVT_LEAVE_WINDOW );
#if (GTK_MINOR_VERSION > 0)
    event.SetTimestamp( gdk_event->time );
#endif
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

    event.m_x = (long)x;
    event.m_y = (long)y;

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

static void gtk_window_vscroll_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return;

    if (!win->m_hasVMT) return;

    float diff = win->m_vAdjust->value - win->m_oldVerticalPos;
    if (fabs(diff) < 0.2) return;
    win->m_oldVerticalPos = win->m_vAdjust->value;

    wxEventType command = wxEVT_NULL;

    float line_step = win->m_vAdjust->step_increment;
    float page_step = win->m_vAdjust->page_increment;

    if (win->IsScrolling())
    {
        command = wxEVT_SCROLLWIN_THUMBTRACK;
    }
    else
    {
        if (fabs(win->m_vAdjust->value-win->m_vAdjust->lower) < 0.2) command = wxEVT_SCROLLWIN_BOTTOM;
        else if (fabs(win->m_vAdjust->value-win->m_vAdjust->upper) < 0.2) command = wxEVT_SCROLLWIN_TOP;
        else if (fabs(diff-line_step) < 0.2) command = wxEVT_SCROLLWIN_LINEDOWN;
        else if (fabs(diff+line_step) < 0.2) command = wxEVT_SCROLLWIN_LINEUP;
        else if (fabs(diff-page_step) < 0.2) command = wxEVT_SCROLLWIN_PAGEDOWN;
        else if (fabs(diff+page_step) < 0.2) command = wxEVT_SCROLLWIN_PAGEUP;
        else command = wxEVT_SCROLLWIN_THUMBTRACK;
    }

    int value = (int)(win->m_vAdjust->value+0.5);

    wxScrollWinEvent event( command, value, wxVERTICAL );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "value_changed" from m_hAdjust
//-----------------------------------------------------------------------------

static void gtk_window_hscroll_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return;
    if (!win->m_hasVMT) return;

    float diff = win->m_hAdjust->value - win->m_oldHorizontalPos;
    if (fabs(diff) < 0.2) return;
    win->m_oldHorizontalPos = win->m_hAdjust->value;

    wxEventType command = wxEVT_NULL;

    float line_step = win->m_hAdjust->step_increment;
    float page_step = win->m_hAdjust->page_increment;

    if (win->IsScrolling())
    {
        command = wxEVT_SCROLLWIN_THUMBTRACK;
    }
    else
    {
        if (fabs(win->m_hAdjust->value-win->m_hAdjust->lower) < 0.2) command = wxEVT_SCROLLWIN_BOTTOM;
        else if (fabs(win->m_hAdjust->value-win->m_hAdjust->upper) < 0.2) command = wxEVT_SCROLLWIN_TOP;
        else if (fabs(diff-line_step) < 0.2) command = wxEVT_SCROLLWIN_LINEDOWN;
        else if (fabs(diff+line_step) < 0.2) command = wxEVT_SCROLLWIN_LINEUP;
        else if (fabs(diff-page_step) < 0.2) command = wxEVT_SCROLLWIN_PAGEDOWN;
        else if (fabs(diff+page_step) < 0.2) command = wxEVT_SCROLLWIN_PAGEUP;
        else command = wxEVT_SCROLLWIN_THUMBTRACK;
    }

    int value = (int)(win->m_hAdjust->value+0.5);

    wxScrollWinEvent event( command, value, wxHORIZONTAL );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "changed" from m_vAdjust
//-----------------------------------------------------------------------------

static void gtk_window_vscroll_change_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return;
    if (!win->m_hasVMT) return;

    wxEventType command = wxEVT_SCROLLWIN_THUMBTRACK;
    int value = (int)(win->m_vAdjust->value+0.5);

    wxScrollWinEvent event( command, value, wxVERTICAL );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "changed" from m_hAdjust
//-----------------------------------------------------------------------------

static void gtk_window_hscroll_change_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return;
    if (!win->m_hasVMT) return;

    wxEventType command = wxEVT_SCROLLWIN_THUMBTRACK;
    int value = (int)(win->m_hAdjust->value+0.5);

    wxScrollWinEvent event( command, value, wxHORIZONTAL );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "button_press_event" from scrollbar
//-----------------------------------------------------------------------------

static gint gtk_scrollbar_button_press_callback( GtkRange *WXUNUSED(widget),
                                                 GdkEventButton *WXUNUSED(gdk_event),
                                                 wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

//  don't test here as we can release the mouse while being over
//  a different window then the slider
//
//    if (gdk_event->window != widget->slider) return FALSE;

    win->SetScrolling( TRUE );

    return FALSE;
}

//-----------------------------------------------------------------------------
// "button_release_event" from scrollbar
//-----------------------------------------------------------------------------

static gint gtk_scrollbar_button_release_callback( GtkRange *widget,
                                                   GdkEventButton *WXUNUSED(gdk_event),
                                                   wxWindow *win )
{

//  don't test here as we can release the mouse while being over
//  a different window then the slider
//
//    if (gdk_event->window != widget->slider) return FALSE;

    GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(win->m_widget);

    if (widget == GTK_RANGE(scrolledWindow->vscrollbar))
        gtk_signal_emit_by_name( GTK_OBJECT(win->m_hAdjust), "value_changed" );
    else
        gtk_signal_emit_by_name( GTK_OBJECT(win->m_vAdjust), "value_changed" );

    win->SetScrolling( FALSE );

    return FALSE;
}

// ----------------------------------------------------------------------------
// this wxWindowBase function is implemented here (in platform-specific file)
// because it is static and so couldn't be made virtual
// ----------------------------------------------------------------------------

wxWindow *wxWindowBase::FindFocus()
{
    return g_focusWindow;
}

//-----------------------------------------------------------------------------
// "realize" from m_widget
//-----------------------------------------------------------------------------

/* we cannot set colours, fonts and cursors before the widget has
   been realized, so we do this directly after realization */

static gint
gtk_window_realized_callback( GtkWidget * WXUNUSED(widget), wxWindow *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (win->m_delayedFont)
        win->SetFont( win->GetFont() );

    if (win->m_delayedBackgroundColour)
        win->SetBackgroundColour( win->GetBackgroundColour() );

    if (win->m_delayedForegroundColour)
        win->SetForegroundColour( win->GetForegroundColour() );

    win->SetCursor( win->GetCursor() );

    wxWindowCreateEvent event( win );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );

    return FALSE;
}

//-----------------------------------------------------------------------------
// InsertChild for wxWindow.
//-----------------------------------------------------------------------------

/* Callback for wxWindow. This very strange beast has to be used because
 * C++ has no virtual methods in a constructor. We have to emulate a
 * virtual function here as wxNotebook requires a different way to insert
 * a child in it. I had opted for creating a wxNotebookPage window class
 * which would have made this superfluous (such in the MDI window system),
 * but no-one was listening to me... */

static void wxInsertChildInWindow( wxWindow* parent, wxWindow* child )
{
    gtk_myfixed_put( GTK_MYFIXED(parent->m_wxwindow),
                     GTK_WIDGET(child->m_widget),
                     child->m_x,
                     child->m_y,
                     child->m_width,
                     child->m_height );

    if (parent->HasFlag(wxTAB_TRAVERSAL))
    {
        /* we now allow a window to get the focus as long as it
           doesn't have any children. */
        GTK_WIDGET_UNSET_FLAGS( parent->m_wxwindow, GTK_CAN_FOCUS );
    }
}

//-----------------------------------------------------------------------------
// global functions
//-----------------------------------------------------------------------------

wxWindow* wxGetActiveWindow()
{
    return g_focusWindow;
}

//-----------------------------------------------------------------------------
// wxWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowBase)

void wxWindow::Init()
{
    // common init
    InitBase();

    // GTK specific
    m_widget = (GtkWidget *) NULL;
    m_wxwindow = (GtkWidget *) NULL;

    // position/size
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;

    m_sizeSet = FALSE;
    m_hasVMT = FALSE;
    m_needParent = TRUE;
    m_isBeingDeleted = FALSE;

    m_hasScrolling = FALSE;
    m_isScrolling = FALSE;

    m_hAdjust = (GtkAdjustment*) NULL;
    m_vAdjust = (GtkAdjustment*) NULL;
    m_oldHorizontalPos = 0.0;
    m_oldVerticalPos = 0.0;

    m_resizing = FALSE;
    m_scrollGC = (GdkGC*) NULL;
    m_widgetStyle = (GtkStyle*) NULL;

    m_insertCallback = (wxInsertChildFunction) NULL;

    m_isStaticBox = FALSE;
    m_acceptsFocus = FALSE;
}

wxWindow::wxWindow()
{
    Init();
}

wxWindow::wxWindow( wxWindow *parent, wxWindowID id,
                    const wxPoint &pos, const wxSize &size,
                    long style, const wxString &name  )
{
    Init();

    Create( parent, id, pos, size, style, name );
}

bool wxWindow::Create( wxWindow *parent, wxWindowID id,
                       const wxPoint &pos, const wxSize &size,
                       long style, const wxString &name  )
{
    PreCreation( parent, id, pos, size, style, name );

    m_insertCallback = wxInsertChildInWindow;

    m_widget = gtk_scrolled_window_new( (GtkAdjustment *) NULL, (GtkAdjustment *) NULL );
    GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );

#ifdef __WXDEBUG__
    debug_focus_in( m_widget, _T("wxWindow::m_widget"), name );
#endif

    GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(m_widget);

#ifdef __WXDEBUG__
    debug_focus_in( scrolledWindow->hscrollbar, _T("wxWindow::hsrcollbar"), name );
    debug_focus_in( scrolledWindow->vscrollbar, _T("wxWindow::vsrcollbar"), name );
#endif

    GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );
    scroll_class->scrollbar_spacing = 0;

    gtk_scrolled_window_set_policy( scrolledWindow, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

    m_hAdjust = gtk_range_get_adjustment( GTK_RANGE(scrolledWindow->hscrollbar) );
    m_vAdjust = gtk_range_get_adjustment( GTK_RANGE(scrolledWindow->vscrollbar) );

    m_wxwindow = gtk_myfixed_new();

#ifdef __WXDEBUG__
    debug_focus_in( m_wxwindow, _T("wxWindow::m_wxwindow"), name );
#endif

    gtk_container_add( GTK_CONTAINER(m_widget), m_wxwindow );

#if (GTK_MINOR_VERSION > 0)
    GtkMyFixed *myfixed = GTK_MYFIXED(m_wxwindow);

    if (HasFlag(wxRAISED_BORDER))
    {
        gtk_myfixed_set_shadow_type( myfixed, GTK_SHADOW_OUT );
    }
    else if (HasFlag(wxSUNKEN_BORDER))
    {
        gtk_myfixed_set_shadow_type( myfixed, GTK_SHADOW_IN );
    }
    else
    {
        gtk_myfixed_set_shadow_type( myfixed, GTK_SHADOW_NONE );
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

    if (HasFlag(wxTAB_TRAVERSAL))
    {
        /* we now allow a window to get the focus as long as it
           doesn't have any children. */
        GTK_WIDGET_SET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );
        m_acceptsFocus = FALSE;
    }
    else
    {
        GTK_WIDGET_SET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );
        m_acceptsFocus = TRUE;
    }

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

    gtk_signal_connect( GTK_OBJECT(m_hAdjust), "changed",
          (GtkSignalFunc) gtk_window_hscroll_change_callback, (gpointer) this );
    gtk_signal_connect(GTK_OBJECT(m_vAdjust), "changed",
          (GtkSignalFunc) gtk_window_vscroll_change_callback, (gpointer) this );

    gtk_widget_show( m_wxwindow );

    if (m_parent)
        m_parent->DoAddChild( this );

    PostCreation();

    Show( TRUE );

    return TRUE;
}

wxWindow::~wxWindow()
{
    m_isBeingDeleted = TRUE;
    m_hasVMT = FALSE;

    if (m_widget)
        Show( FALSE );

    DestroyChildren();

    if (m_parent)
        m_parent->RemoveChild( this );

    if (m_widgetStyle)
    {
        gtk_style_unref( m_widgetStyle );
        m_widgetStyle = (GtkStyle*) NULL;
    }

    if (m_scrollGC)
    {
        gdk_gc_unref( m_scrollGC );
        m_scrollGC = (GdkGC*) NULL;
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

void wxWindow::PreCreation( wxWindow *parent,
                            wxWindowID id,
                            const wxPoint &pos,
                            const wxSize &size,
                            long style,
                            const wxString &name )
{
    wxASSERT_MSG( !m_needParent || parent, _T("Need complete parent.") );

    if ( !CreateBase(parent, id, pos, size, style, name) )
    {
        wxFAIL_MSG(_T("window creation failed"));
    }

    m_width = WidthDefault(size.x);
    m_height = HeightDefault(size.y);

    m_x = (int)pos.x;
    m_y = (int)pos.y;

    if (!parent)  /* some reasonable defaults */
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
}

void wxWindow::PostCreation()
{
    wxASSERT_MSG( (m_widget != NULL), _T("invalid window") );

    if (m_wxwindow)
    {
        /* these get reported to wxWindows -> wxPaintEvent */
        gtk_signal_connect( GTK_OBJECT(m_wxwindow), "expose_event",
          GTK_SIGNAL_FUNC(gtk_window_expose_callback), (gpointer)this );

        gtk_signal_connect( GTK_OBJECT(m_wxwindow), "draw",
          GTK_SIGNAL_FUNC(gtk_window_draw_callback), (gpointer)this );

#if (GTK_MINOR_VERSION > 0)
        /* these are called when the "sunken" or "raised" borders are drawn */
        gtk_signal_connect( GTK_OBJECT(m_widget), "expose_event",
          GTK_SIGNAL_FUNC(gtk_window_own_expose_callback), (gpointer)this );

        gtk_signal_connect( GTK_OBJECT(m_widget), "draw",
          GTK_SIGNAL_FUNC(gtk_window_own_draw_callback), (gpointer)this );
#endif
    }

    GtkWidget *connect_widget = GetConnectWidget();

    ConnectWidget( connect_widget );

   /*  we cannot set colours, fonts and cursors before the widget has
       been realized, so we do this directly after realization */
    gtk_signal_connect( GTK_OBJECT(connect_widget), "realize",
                            GTK_SIGNAL_FUNC(gtk_window_realized_callback), (gpointer) this );

    m_hasVMT = TRUE;
}

void wxWindow::ConnectWidget( GtkWidget *widget )
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

    gtk_signal_connect( GTK_OBJECT(widget), "focus_in_event",
      GTK_SIGNAL_FUNC(gtk_window_focus_in_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "focus_out_event",
      GTK_SIGNAL_FUNC(gtk_window_focus_out_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "enter_notify_event",
      GTK_SIGNAL_FUNC(gtk_window_enter_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(widget), "leave_notify_event",
      GTK_SIGNAL_FUNC(gtk_window_leave_callback), (gpointer)this );
}

bool wxWindow::Destroy()
{
    wxASSERT_MSG( (m_widget != NULL), _T("invalid window") );

    m_hasVMT = FALSE;

    return wxWindowBase::Destroy();
}

void wxWindow::DoSetSize( int x, int y, int width, int height, int sizeFlags )
{
    wxASSERT_MSG( (m_widget != NULL), _T("invalid window") );
    wxASSERT_MSG( (m_parent != NULL), _T("wxWindow::SetSize requires parent.\n") );

    if (m_resizing) return; /* I don't like recursions */
    m_resizing = TRUE;

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
        if ((sizeFlags & wxSIZE_ALLOW_MINUS_ONE) == 0)
        {
            if (x != -1) m_x = x;
            if (y != -1) m_y = y;
            if (width != -1) m_width = width;
            if (height != -1) m_height = height;
        }
        else
        {
            m_x = x;
            m_y = y;
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

        /* this is the result of hours of debugging: the following code
           means that if we have a m_wxwindow and we set the size of
           m_widget, m_widget (which is a GtkScrolledWindow) does NOT
           automatically propagate its size down to its m_wxwindow,
           which is its client area. therefore, we have to tell the
           client area directly that it has to resize itself.
           this will lead to that m_widget (GtkScrolledWindow) will
           calculate how much size it needs for scrollbars etc and
           it will then call XXX_size_allocate of its child, which
           is m_wxwindow. m_wxwindow in turn will do the same with its
           children and so on. problems can arise if this happens
           before all the children have been realized as some widgets
           stupidy need to be realized during XXX_size_allocate (e.g.
           GtkNotebook) and they will segv if called otherwise. this
           emergency is tested in gtk_myfixed_size_allocate. Normally
           this shouldn't be needed and only gtk_widget_queue_resize()
           should be enough to provoke a resize at the next appropriate
           moment, but this seems to fail, e.g. when a wxNotebook contains
           a wxSplitterWindow: the splitter window's children won't
           show up properly resized then. */

        gtk_myfixed_set_size( GTK_MYFIXED(m_parent->m_wxwindow),
                              m_widget,
                              m_x-border,
                              m_y-border,
                              m_width+2*border,
                              m_height+border+bottom_border );
    }

    m_sizeSet = TRUE;

    wxSizeEvent event( wxSize(m_width,m_height), GetId() );
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent( event );

    m_resizing = FALSE;
}

void wxWindow::OnInternalIdle()
{
    wxCursor cursor = m_cursor;
    if (g_globalCursor.Ok()) cursor = g_globalCursor;

    if (cursor.Ok() && m_currentGdkCursor != cursor)
    {
        if (m_wxwindow)
        {
            GdkWindow *window = m_wxwindow->window;
            if (window)
                gdk_window_set_cursor( window, cursor.GetCursor() );

            if (!g_globalCursor.Ok())
                cursor = *wxSTANDARD_CURSOR;

            window = m_widget->window;
            if (window)
                gdk_window_set_cursor( window, cursor.GetCursor() );
        }
        else
        {
            GdkWindow *window = m_widget->window;
            if (window)
               gdk_window_set_cursor( window, cursor.GetCursor() );
        }

        m_currentGdkCursor = cursor;
    }

    UpdateWindowUI();
}

void wxWindow::DoGetSize( int *width, int *height ) const
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (width) (*width) = m_width;
    if (height) (*height) = m_height;
}

void wxWindow::DoSetClientSize( int width, int height )
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_wxwindow)
    {
        SetSize( width, height );
    }
    else
    {
        int dw = 0;
        int dh = 0;

        GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);
        GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );

#if (GTK_MINOR_VERSION == 0)
        GtkWidget *viewport = scroll_window->viewport;
        GtkStyleClass *viewport_class = viewport->style->klass;

        if (HasFlag(wxRAISED_BORDER) || HasFlag(wxSUNKEN_BORDER))
        {
            dw += 2 * viewport_class->xthickness;
            dh += 2 * viewport_class->ythickness;
        }
#else
        if (HasFlag(wxRAISED_BORDER) || HasFlag(wxSUNKEN_BORDER))
        {
            /* when using GTK 1.2 we set the border size to 2 */
            dw += 2 * 2;
            dh += 2 * 2;
        }
#endif

        if (HasScrolling())
        {
/*
            GtkWidget *hscrollbar = scroll_window->hscrollbar;
            GtkWidget *vscrollbar = scroll_window->vscrollbar;

            we use this instead:  range.slider_width = 11 + 2*2pts edge
*/

            if (scroll_window->vscrollbar_visible)
            {
                dw += 15;   /* dw += vscrollbar->allocation.width; */
                dw += scroll_class->scrollbar_spacing;
            }

            if (scroll_window->hscrollbar_visible)
            {
                dh += 15;   /* dh += hscrollbar->allocation.height; */
                dh += scroll_class->scrollbar_spacing;
            }
       }

       SetSize( width+dw, height+dh );
    }
}

void wxWindow::DoGetClientSize( int *width, int *height ) const
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_wxwindow)
    {
        if (width) (*width) = m_width;
        if (height) (*height) = m_height;
    }
    else
    {
        int dw = 0;
        int dh = 0;

#if (GTK_MINOR_VERSION == 0)
        if (HasFlag(wxRAISED_BORDER) || HasFlag(wxSUNKEN_BORDER))
        {
            if (HasScrolling())
            {
                GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);
                GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );

                GtkWidget *viewport = scroll_window->viewport;
                GtkStyleClass *viewport_class = viewport->style->klass;

                dw += 2 * viewport_class->xthickness;
                dh += 2 * viewport_class->ythickness;
            }
        }
#else
        if (HasFlag(wxRAISED_BORDER) || HasFlag(wxSUNKEN_BORDER))
        {
            /* when using GTK 1.2 we set the border size to 2 */
            dw += 2 * 2;
            dh += 2 * 2;
        }
#endif
        if (HasScrolling())
        {
/*
            GtkWidget *hscrollbar = scroll_window->hscrollbar;
            GtkWidget *vscrollbar = scroll_window->vscrollbar;

            we use this instead:  range.slider_width = 11 + 2*2pts edge
*/

            GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);
            GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );

            if (scroll_window->vscrollbar_visible)
            {
                dw += 15;   /* dw += vscrollbar->allocation.width; */
                dw += scroll_class->scrollbar_spacing;
            }

            if (scroll_window->hscrollbar_visible)
            {
                dh += 15;   /* dh += hscrollbar->allocation.height; */
                dh += scroll_class->scrollbar_spacing;
            }
        }

        if (width) (*width) = m_width - dw;
        if (height) (*height) = m_height - dh;
    }
}

void wxWindow::DoGetPosition( int *x, int *y ) const
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (x) (*x) = m_x;
    if (y) (*y) = m_y;
}

void wxWindow::DoClientToScreen( int *x, int *y ) const
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_widget->window) return;

    GdkWindow *source = (GdkWindow *) NULL;
    if (m_wxwindow)
        source = m_wxwindow->window;
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

void wxWindow::DoScreenToClient( int *x, int *y ) const
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_widget->window) return;

    GdkWindow *source = (GdkWindow *) NULL;
    if (m_wxwindow)
        source = m_wxwindow->window;
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

bool wxWindow::Show( bool show )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, _T("invalid window") );

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

bool wxWindow::Enable( bool enable )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, _T("invalid window") );

    if (!wxWindowBase::Enable(enable))
    {
        // nothing to do
        return FALSE;
    }

    gtk_widget_set_sensitive( m_widget, enable );
    if ( m_wxwindow )
        gtk_widget_set_sensitive( m_wxwindow, enable );

    return TRUE;
}

int wxWindow::GetCharHeight() const
{
    wxCHECK_MSG( (m_widget != NULL), 12, _T("invalid window") );

    wxCHECK_MSG( m_font.Ok(), 12, _T("invalid font") );

    GdkFont *font = m_font.GetInternalFont( 1.0 );

    return font->ascent + font->descent;
}

int wxWindow::GetCharWidth() const
{
    wxCHECK_MSG( (m_widget != NULL), 8, _T("invalid window") );

    wxCHECK_MSG( m_font.Ok(), 8, _T("invalid font") );

    GdkFont *font = m_font.GetInternalFont( 1.0 );

    return gdk_string_width( font, "H" );
}

void wxWindow::GetTextExtent( const wxString& string,
                              int *x,
                              int *y,
                              int *descent,
                              int *externalLeading,
                              const wxFont *theFont ) const
{
    wxFont fontToUse = m_font;
    if (theFont) fontToUse = *theFont;

    wxCHECK_RET( fontToUse.Ok(), _T("invalid font") );

    GdkFont *font = fontToUse.GetInternalFont( 1.0 );
    if (x) (*x) = gdk_string_width( font, string.mbc_str() );
    if (y) (*y) = font->ascent + font->descent;
    if (descent) (*descent) = font->descent;
    if (externalLeading) (*externalLeading) = 0;  // ??
}

void wxWindow::SetFocus()
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    GtkWidget *connect_widget = GetConnectWidget();
    if (connect_widget)
    {
        if (GTK_WIDGET_CAN_FOCUS(connect_widget) /*&& !GTK_WIDGET_HAS_FOCUS (connect_widget)*/ )
        {
            gtk_widget_grab_focus (connect_widget);
        }
        else if (GTK_IS_CONTAINER(connect_widget))
        {
            gtk_container_focus( GTK_CONTAINER(connect_widget), GTK_DIR_TAB_FORWARD );
        }
        else
        {
        }
    }
}

bool wxWindow::AcceptsFocus() const
{
    return m_acceptsFocus && wxWindowBase::AcceptsFocus();
}

bool wxWindow::Reparent( wxWindow *newParent )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, _T("invalid window") );

    wxWindow *oldParent = m_parent;

    if ( !wxWindowBase::Reparent(newParent) )
        return FALSE;

    if (oldParent)
    {
        gtk_container_remove( GTK_CONTAINER(oldParent->m_wxwindow), m_widget );
    }

    if (newParent)
    {
        /* insert GTK representation */
        (*(newParent->m_insertCallback))(newParent, this);
    }

    return TRUE;
}

void wxWindow::DoAddChild(wxWindow *child)
{
    wxASSERT_MSG( (m_widget != NULL), _T("invalid window") );

    wxASSERT_MSG( (child != NULL), _T("invalid child window") );

    wxASSERT_MSG( (m_insertCallback != NULL), _T("invalid child insertion function") );

    /* add to list */
    AddChild( child );

    /* insert GTK representation */
    (*m_insertCallback)(this, child);
}

void wxWindow::Raise()
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_widget->window) return;

    gdk_window_raise( m_widget->window );
}

void wxWindow::Lower()
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_widget->window) return;

    gdk_window_lower( m_widget->window );
}

bool wxWindow::SetCursor( const wxCursor &cursor )
{
    wxCHECK_MSG( (m_widget != NULL), FALSE, _T("invalid window") );

    if (!wxWindowBase::SetCursor(cursor))
    {
        // don't leave if the GTK widget has just
        // been realized
        if (!m_delayedCursor) return FALSE;
    }

    GtkWidget *connect_widget = GetConnectWidget();
    if (!connect_widget->window)
    {
        // indicate that a new style has been set
        // but it couldn't get applied as the
        // widget hasn't been realized yet.
        m_delayedCursor = TRUE;

        // pretend we have done something
        return TRUE;
    }

//    gdk_window_set_cursor( connect_widget->window, GetCursor().GetCursor() );

    // cursor was set
    return TRUE;
}

void wxWindow::WarpPointer( int x, int y )
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    GtkWidget *connect_widget = GetConnectWidget();
    if (connect_widget->window)
    {
        /* we provide this function ourselves as it is
           missing in GDK */
        gdk_window_warp_pointer( connect_widget->window, x, y );
    }
}

void wxWindow::Refresh( bool eraseBackground, const wxRect *rect )
{
    wxCHECK_RET( (m_widget != NULL), _T("invalid window") );

    if (!m_widget->window) return;

    if (eraseBackground && m_wxwindow && m_wxwindow->window)
    {
        if (rect)
        {
            gdk_window_clear_area( m_wxwindow->window,
                                   rect->x, rect->y,
                                   rect->width, rect->height );
        }
        else
        {
            gdk_window_clear( m_wxwindow->window );
        }
    }

    if (!rect)
    {
        if (m_wxwindow)
            gtk_widget_draw( m_wxwindow, (GdkRectangle*) NULL );
        else
            gtk_widget_draw( m_widget, (GdkRectangle*) NULL );
    }
    else
    {
        GdkRectangle gdk_rect;
        gdk_rect.x = rect->x;
        gdk_rect.y = rect->y;
        gdk_rect.width = rect->width;
        gdk_rect.height = rect->height;

        if (m_wxwindow)
            gtk_widget_draw( m_wxwindow, &gdk_rect );
        else
            gtk_widget_draw( m_widget, &gdk_rect );
    }
}

void wxWindow::Clear()
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    if (!m_widget->window) return;

    if (m_wxwindow && m_wxwindow->window)
    {
        gdk_window_clear( m_wxwindow->window );
    }
}

#if wxUSE_TOOLTIPS
void wxWindow::DoSetToolTip( wxToolTip *tip )
{
    wxWindowBase::DoSetToolTip(tip);

    if (m_tooltip)
        m_tooltip->Apply( this );
}

void wxWindow::ApplyToolTip( GtkTooltips *tips, const wxChar *tip )
{
    gtk_tooltips_set_tip( tips, GetConnectWidget(), wxConvCurrent->cWX2MB(tip), (gchar*) NULL );
}
#endif // wxUSE_TOOLTIPS

bool wxWindow::SetBackgroundColour( const wxColour &colour )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, _T("invalid window") );

    if (!wxWindowBase::SetBackgroundColour(colour))
    {
        // don't leave if the GTK widget has just
        // been realized
        if (!m_delayedBackgroundColour) return FALSE;
    }

    GtkWidget *connect_widget = GetConnectWidget();
    if (!connect_widget->window)
    {
        // indicate that a new style has been set
        // but it couldn't get applied as the
        // widget hasn't been realized yet.
        m_delayedBackgroundColour = TRUE;

        // pretend we have done something
        return TRUE;
    }

    if (m_wxwindow && m_wxwindow->window)
    {
        /* wxMSW doesn't clear the window here. I don't do that either to
          provide compatibility. call Clear() to do the job. */

        m_backgroundColour.CalcPixel( gdk_window_get_colormap( m_wxwindow->window ) );
        gdk_window_set_background( m_wxwindow->window, m_backgroundColour.GetColor() );
    }

    wxColour sysbg = wxSystemSettings::GetSystemColour( wxSYS_COLOUR_BTNFACE );
    if (sysbg == m_backgroundColour)
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

bool wxWindow::SetForegroundColour( const wxColour &colour )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, _T("invalid window") );

    if (!wxWindowBase::SetForegroundColour(colour))
    {
        // don't leave if the GTK widget has just
        // been realized
        if (!m_delayedForegroundColour) return FALSE;
    }

    GtkWidget *connect_widget = GetConnectWidget();
    if (!connect_widget->window)
    {
        // indicate that a new style has been set
        // but it couldn't get applied as the
        // widget hasn't been realized yet.
        m_delayedForegroundColour = TRUE;

        // pretend we have done something
        return TRUE;
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

GtkStyle *wxWindow::GetWidgetStyle()
{
    if (m_widgetStyle) gtk_style_unref( m_widgetStyle );

    m_widgetStyle = gtk_style_copy( gtk_widget_get_style( m_widget ) );

    return m_widgetStyle;
}

void wxWindow::SetWidgetStyle()
{
    GtkStyle *style = GetWidgetStyle();

    gdk_font_unref( style->font );
    style->font = gdk_font_ref( m_font.GetInternalFont( 1.0 ) );

    if (m_foregroundColour.Ok())
    {
        m_foregroundColour.CalcPixel( gdk_window_get_colormap( m_widget->window ) );
        style->fg[GTK_STATE_NORMAL] = *m_foregroundColour.GetColor();
        style->fg[GTK_STATE_PRELIGHT] = *m_foregroundColour.GetColor();
        style->fg[GTK_STATE_ACTIVE] = *m_foregroundColour.GetColor();
    }

    if (m_backgroundColour.Ok())
    {
        m_backgroundColour.CalcPixel( gdk_window_get_colormap( m_widget->window ) );
        style->bg[GTK_STATE_NORMAL] = *m_backgroundColour.GetColor();
        style->base[GTK_STATE_NORMAL] = *m_backgroundColour.GetColor();
        style->bg[GTK_STATE_PRELIGHT] = *m_backgroundColour.GetColor();
        style->base[GTK_STATE_PRELIGHT] = *m_backgroundColour.GetColor();
        style->bg[GTK_STATE_ACTIVE] = *m_backgroundColour.GetColor();
        style->base[GTK_STATE_ACTIVE] = *m_backgroundColour.GetColor();
        style->bg[GTK_STATE_INSENSITIVE] = *m_backgroundColour.GetColor();
        style->base[GTK_STATE_INSENSITIVE] = *m_backgroundColour.GetColor();
    }
}

void wxWindow::ApplyWidgetStyle()
{
}

static void SetInvokingWindow( wxMenu *menu, wxWindow *win )
{
    menu->SetInvokingWindow( win );
    wxNode *node = menu->GetItems().First();
    while (node)
    {
        wxMenuItem *menuitem = (wxMenuItem*)node->Data();
        if (menuitem->IsSubMenu())
        {
            SetInvokingWindow( menuitem->GetSubMenu(), win );
        }
        node = node->Next();
    }
}

static gint gs_pop_x = 0;
static gint gs_pop_y = 0;

static void pop_pos_callback( GtkMenu * WXUNUSED(menu),
                              gint *x, gint *y,
                              wxWindow *win )
{
    win->ClientToScreen( &gs_pop_x, &gs_pop_y );
    *x = gs_pop_x;
    *y = gs_pop_y;
}

bool wxWindow::PopupMenu( wxMenu *menu, int x, int y )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, _T("invalid window") );

    wxCHECK_MSG( menu != NULL, FALSE, _T("invalid popup-menu") );

    SetInvokingWindow( menu, this );

    menu->UpdateUI();

    gs_pop_x = x;
    gs_pop_y = y;

    gtk_menu_popup(
                  GTK_MENU(menu->m_menu),
                  (GtkWidget *) NULL,          // parent menu shell
                  (GtkWidget *) NULL,          // parent menu item
                  (GtkMenuPositionFunc) pop_pos_callback,
                  (gpointer) this,             // client data
                  0,                           // button used to activate it
                  0 //gs_timeLastClick         // the time of activation
                );
    return TRUE;
}

#if wxUSE_DRAG_AND_DROP

void wxWindow::SetDropTarget( wxDropTarget *dropTarget )
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    GtkWidget *dnd_widget = GetConnectWidget();

    if (m_dropTarget) m_dropTarget->UnregisterWidget( dnd_widget );

    if (m_dropTarget) delete m_dropTarget;
    m_dropTarget = dropTarget;

    if (m_dropTarget) m_dropTarget->RegisterWidget( dnd_widget );
}

#endif // wxUSE_DRAG_AND_DROP

GtkWidget* wxWindow::GetConnectWidget()
{
    GtkWidget *connect_widget = m_widget;
    if (m_wxwindow) connect_widget = m_wxwindow;

    return connect_widget;
}

bool wxWindow::IsOwnGtkWindow( GdkWindow *window )
{
    if (m_wxwindow) return (window == m_wxwindow->window);
    return (window == m_widget->window);
}

bool wxWindow::SetFont( const wxFont &font )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, _T(        "invalid window") );

    if (!wxWindowBase::SetFont(font))
    {
        // don't leave if the GTK widget has just
        // been realized
        if (!m_delayedFont) return FALSE;
    }

    GtkWidget *connect_widget = GetConnectWidget();
    if (!connect_widget->window)
    {
        // indicate that a new style has been set
        // but it couldn't get applied as the
        // widget hasn't been realized yet.
        m_delayedFont = TRUE;

        // pretend we have done something
        return TRUE;
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

void wxWindow::CaptureMouse()
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    wxCHECK_RET( g_captureWindow == NULL, _T("CaptureMouse called twice") );

    GtkWidget *connect_widget = GetConnectWidget();
    if (!connect_widget->window) return;

    gdk_pointer_grab( connect_widget->window, FALSE,
                      (GdkEventMask)
                         (GDK_BUTTON_PRESS_MASK |
                          GDK_BUTTON_RELEASE_MASK |
                          GDK_POINTER_MOTION_MASK),
                      (GdkWindow *) NULL,
                      m_cursor.GetCursor(),
                      GDK_CURRENT_TIME );
    g_captureWindow = this;
}

void wxWindow::ReleaseMouse()
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    wxCHECK_RET( g_captureWindow, _T("ReleaseMouse called twice") );

    GtkWidget *connect_widget = GetConnectWidget();
    if (!connect_widget->window) return;

    gdk_pointer_ungrab ( GDK_CURRENT_TIME );
    g_captureWindow = (wxWindow*) NULL;
}

bool wxWindow::IsRetained() const
{
    return FALSE;
}

void wxWindow::SetScrollbar( int orient, int pos, int thumbVisible,
      int range, bool refresh )
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    wxCHECK_RET( m_wxwindow != NULL, _T("window needs client area for scrolling") );

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

void wxWindow::SetScrollPos( int orient, int pos, bool WXUNUSED(refresh) )
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    wxCHECK_RET( m_wxwindow != NULL, _T("window needs client area for scrolling") );

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

    if (!m_isScrolling)  /* prevent recursion */
    {
        if (m_wxwindow->window)
        {
            if (orient == wxHORIZONTAL)
                gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "value_changed" );
            else
                gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "value_changed" );
        }
    }
}

int wxWindow::GetScrollThumb( int orient ) const
{
    wxCHECK_MSG( m_widget != NULL, 0, _T("invalid window") );

    wxCHECK_MSG( m_wxwindow != NULL, 0, _T("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
        return (int)(m_hAdjust->page_size+0.5);
    else
        return (int)(m_vAdjust->page_size+0.5);
}

int wxWindow::GetScrollPos( int orient ) const
{
    wxCHECK_MSG( m_widget != NULL, 0, _T("invalid window") );

    wxCHECK_MSG( m_wxwindow != NULL, 0, _T("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
        return (int)(m_hAdjust->value+0.5);
    else
        return (int)(m_vAdjust->value+0.5);
}

int wxWindow::GetScrollRange( int orient ) const
{
    wxCHECK_MSG( m_widget != NULL, 0, _T("invalid window") );

    wxCHECK_MSG( m_wxwindow != NULL, 0, _T("window needs client area for scrolling") );

    if (orient == wxHORIZONTAL)
        return (int)(m_hAdjust->upper+0.5);
    else
        return (int)(m_vAdjust->upper+0.5);
}

void wxWindow::ScrollWindow( int dx, int dy, const wxRect* WXUNUSED(rect) )
{
    wxCHECK_RET( m_widget != NULL, _T("invalid window") );

    wxCHECK_RET( m_wxwindow != NULL, _T("window needs client area for scrolling") );

    if (!m_scrollGC)
    {
        m_scrollGC = gdk_gc_new( m_wxwindow->window );
        gdk_gc_set_exposures( m_scrollGC, TRUE );
    }

    wxNode *node = m_children.First();
    while (node)
    {
        wxWindow *child = (wxWindow*) node->Data();
        int sx = 0;
        int sy = 0;
        child->GetSize( &sx, &sy );
        child->SetSize( child->m_x + dx, child->m_y + dy, sx, sy, wxSIZE_ALLOW_MINUS_ONE );
        node = node->Next();
    }

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

        gdk_window_copy_area( m_wxwindow->window, m_scrollGC, d_x, d_y,
            m_wxwindow->window, s_x, s_y, w, h );

        wxRect rect;
        if (dx < 0) rect.x = cw+dx; else rect.x = 0;
        if (dy < 0) rect.y = ch+dy; else rect.y = 0;
        if (dy != 0) rect.width = cw; else rect.width = abs(dx);
        if (dx != 0) rect.height = ch; else rect.height = abs(dy);

        Refresh( TRUE, &rect );
    }
}

void wxWindow::SetScrolling(bool scroll)
{
    m_isScrolling = g_blockEventsOnScroll = scroll;
}
