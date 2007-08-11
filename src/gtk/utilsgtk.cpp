/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/utilsgtk.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/utils.h"

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/intl.h"
    #include "wx/log.h"
#endif

#include "wx/apptrait.h"

#include "wx/process.h"
#include "wx/sysopt.h"

#include "wx/unix/execute.h"

#include "wx/gtk/private/timer.h"
#include "wx/evtloop.h"

#ifdef __WXDEBUG__
    #include "wx/gtk/assertdlg_gtk.h"
    #if wxUSE_STACKWALKER
        #include "wx/stackwalk.h"
    #endif // wxUSE_STACKWALKER
#endif // __WXDEBUG__

#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>   // for WNOHANG
#include <unistd.h>

#include "glib.h"
#include "gdk/gdk.h"
#include "gtk/gtk.h"
#include "gdk/gdkx.h"

#ifdef HAVE_X11_XKBLIB_H
    /* under HP-UX and Solaris 2.6, at least, XKBlib.h defines structures with
     * field named "explicit" - which is, of course, an error for a C++
     * compiler. To be on the safe side, just redefine it everywhere. */
    #define explicit __wx_explicit

    #include "X11/XKBlib.h"

    #undef explicit
#endif // HAVE_X11_XKBLIB_H


#if wxUSE_DETECT_SM
    #include "X11/Xlib.h"
    #include "X11/SM/SMlib.h"
#endif

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern GtkWidget *wxGetRootWindow();

//----------------------------------------------------------------------------
// misc.
//----------------------------------------------------------------------------
#ifndef __EMX__
// on OS/2, we use the wxBell from wxBase library

void wxBell()
{
    gdk_beep();
}
#endif

/* Don't synthesize KeyUp events holding down a key and producing
   KeyDown events with autorepeat. */
#ifdef HAVE_X11_XKBLIB_H
bool wxSetDetectableAutoRepeat( bool flag )
{
    Bool result;
    XkbSetDetectableAutoRepeat( GDK_DISPLAY(), flag, &result );
    return result;       /* true if keyboard hardware supports this mode */
}
#else
bool wxSetDetectableAutoRepeat( bool WXUNUSED(flag) )
{
    return false;
}
#endif

// Escapes string so that it is valid Pango markup XML string:
wxString wxEscapeStringForPangoMarkup(const wxString& str)
{
    size_t len = str.length();
    wxString out;
    out.Alloc(len);
    for (size_t i = 0; i < len; i++)
    {
        wxChar c = str[i];
        switch (c)
        {
            case _T('&'):
                out << _T("&amp;");
                break;
            case _T('<'):
                out << _T("&lt;");
                break;
            case _T('>'):
                out << _T("&gt;");
                break;
            case _T('\''):
                out << _T("&apos;");
                break;
            case _T('"'):
                out << _T("&quot;");
                break;
            default:
                out << c;
                break;
        }
    }
    return out;
}


// ----------------------------------------------------------------------------
// display characterstics
// ----------------------------------------------------------------------------

void *wxGetDisplay()
{
    return GDK_DISPLAY();
}

void wxDisplaySize( int *width, int *height )
{
    if (width) *width = gdk_screen_width();
    if (height) *height = gdk_screen_height();
}

void wxDisplaySizeMM( int *width, int *height )
{
    if (width) *width = gdk_screen_width_mm();
    if (height) *height = gdk_screen_height_mm();
}

void wxClientDisplayRect(int *x, int *y, int *width, int *height)
{
    // This is supposed to return desktop dimensions minus any window
    // manager panels, menus, taskbars, etc.  If there is a way to do that
    // for this platform please fix this function, otherwise it defaults
    // to the entire desktop.
    if (x) *x = 0;
    if (y) *y = 0;
    wxDisplaySize(width, height);
}

void wxGetMousePosition( int* x, int* y )
{
    gdk_window_get_pointer( (GdkWindow*) NULL, x, y, (GdkModifierType*) NULL );
}

bool wxColourDisplay()
{
    return true;
}

int wxDisplayDepth()
{
    return gdk_drawable_get_visual( wxGetRootWindow()->window )->depth;
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

#if !wxUSE_UNICODE

WXDLLIMPEXP_CORE wxCharBuffer
wxConvertToGTK(const wxString& s, wxFontEncoding enc)
{
    wxWCharBuffer wbuf;
    if ( enc == wxFONTENCODING_SYSTEM || enc == wxFONTENCODING_DEFAULT )
    {
        wbuf = wxConvUI->cMB2WC(s.c_str());
    }
    else // another encoding, use generic conversion class
    {
        wbuf = wxCSConv(enc).cMB2WC(s.c_str());
    }

    if ( !wbuf && !s.empty() )
    {
        // conversion failed, but we still want to show something to the user
        // even if it's going to be wrong it is better than nothing
        //
        // we choose ISO8859-1 here arbitrarily, it's just the most common
        // encoding probably and, also importantly here, conversion from it
        // never fails as it's done internally by wxCSConv
        wbuf = wxCSConv(wxFONTENCODING_ISO8859_1).cMB2WC(s.c_str());
    }

    return wxConvUTF8.cWC2MB(wbuf);
}

WXDLLIMPEXP_CORE wxCharBuffer
wxConvertFromGTK(const wxString& s, wxFontEncoding enc)
{
    // this conversion should never fail as GTK+ always uses UTF-8 internally
    // so there are no complications here
    const wxWCharBuffer wbuf(wxConvUTF8.cMB2WC(s.c_str()));
    if ( enc == wxFONTENCODING_SYSTEM )
        return wxConvUI->cWC2MB(wbuf);

    return wxCSConv(enc).cWC2MB(wbuf);
}

#endif // !wxUSE_UNICODE

// Returns false if version is certainly greater or equal than major.minor.micro
// Returns true if version is lower than major.minor.micro OR it cannot be
// determined and one should not rely on the availability of pango version
// major.minor.micro, nor the non-availability
const gchar *wx_pango_version_check (int major, int minor, int micro)
{
#ifdef PANGO_VERSION_MAJOR
    if (!gtk_check_version (2,11,0))
    {
        // GTK+ 2.11 requires Pango >= 1.15.3 and pango_version_check
        // was added in Pango 1.15.2 thus we know for sure the pango lib we're
        // using has the pango_version_check function:
        return pango_version_check (major, minor, micro);
    }

    return "can't check";
#else // !PANGO_VERSION_MAJOR
    return "too old headers";
#endif
}


// ----------------------------------------------------------------------------
// subprocess routines
// ----------------------------------------------------------------------------

extern "C" {
static
void GTK_EndProcessDetector(gpointer data, gint source,
                            GdkInputCondition WXUNUSED(condition) )
{
   wxEndProcessData *proc_data = (wxEndProcessData *)data;

   // has the process really terminated? unfortunately GDK (or GLib) seem to
   // generate G_IO_HUP notification even when it simply tries to read from a
   // closed fd and hasn't terminated at all
   int pid = (proc_data->pid > 0) ? proc_data->pid : -(proc_data->pid);
   int status = 0;
   int rc = waitpid(pid, &status, WNOHANG);

   if ( rc == 0 )
   {
       // no, it didn't exit yet, continue waiting
       return;
   }

   // set exit code to -1 if something bad happened
   proc_data->exitcode = rc != -1 && WIFEXITED(status) ? WEXITSTATUS(status)
                                                      : -1;

   // child exited, end waiting
   close(source);

   // don't call us again!
   gdk_input_remove(proc_data->tag);

   wxHandleProcessTermination(proc_data);
}
}

int wxAddProcessCallback(wxEndProcessData *proc_data, int fd)
{
    int tag = gdk_input_add(fd,
                            GDK_INPUT_READ,
                            GTK_EndProcessDetector,
                            (gpointer)proc_data);

    return tag;
}



// ----------------------------------------------------------------------------
// wxPlatformInfo-related
// ----------------------------------------------------------------------------

wxPortId wxGUIAppTraits::GetToolkitVersion(int *verMaj, int *verMin) const
{
    if ( verMaj )
        *verMaj = gtk_major_version;
    if ( verMin )
        *verMin = gtk_minor_version;

    return wxPORT_GTK;
}

#if wxUSE_TIMER

wxTimerImpl *wxGUIAppTraits::CreateTimerImpl(wxTimer *timer)
{
    return new wxGTKTimerImpl(timer);
}

#endif // wxUSE_TIMER

#if wxUSE_DETECT_SM
static wxString GetSM()
{
    class Dpy
    {
    public:
        Dpy() { m_dpy = XOpenDisplay(NULL); }
        ~Dpy() { if ( m_dpy ) XCloseDisplay(m_dpy); }

        operator Display *() const { return m_dpy; }
    private:
        Display *m_dpy;
    } dpy;

    if ( !dpy )
        return wxEmptyString;

    char smerr[256];
    char *client_id;
    SmcConn smc_conn = SmcOpenConnection(NULL, NULL,
                                         999, 999,
                                         0 /* mask */, NULL /* callbacks */,
                                         NULL, &client_id,
                                         WXSIZEOF(smerr), smerr);

    if ( !smc_conn )
    {
        wxLogWarning(_("Failed to connect to session manager: %s"), smerr);
        return wxEmptyString;
    }

    char *vendor = SmcVendor(smc_conn);
    wxString ret = wxString::FromAscii( vendor );
    free(vendor);

    SmcCloseConnection(smc_conn, 0, NULL);
    free(client_id);

    return ret;
}
#endif // wxUSE_DETECT_SM


//-----------------------------------------------------------------------------
// wxGUIAppTraits
//-----------------------------------------------------------------------------

wxEventLoopBase *wxGUIAppTraits::CreateEventLoop()
{
    return new wxEventLoop();
}


#if wxUSE_INTL
void wxGUIAppTraits::SetLocale()
{
    gtk_set_locale();
    wxUpdateLocaleIsUtf8();
}
#endif

#ifdef __WXDEBUG__

#if wxUSE_STACKWALKER

// private helper class
class StackDump : public wxStackWalker
{
public:
    StackDump(GtkAssertDialog *dlg) { m_dlg=dlg; }

protected:
    virtual void OnStackFrame(const wxStackFrame& frame)
    {
        wxString fncname = frame.GetName();
        wxString fncargs = fncname;

        size_t n = fncname.find(wxT('('));
        if (n != wxString::npos)
        {
            // remove arguments from function name
            fncname.erase(n);

            // remove function name and brackets from arguments
            fncargs = fncargs.substr(n+1, fncargs.length()-n-2);
        }
        else
            fncargs = wxEmptyString;

        // append this stack frame's info in the dialog
        if (!frame.GetFileName().empty() || !fncname.empty())
            gtk_assert_dialog_append_stack_frame(m_dlg,
                                                fncname.mb_str(),
                                                fncargs.mb_str(),
                                                frame.GetFileName().mb_str(),
                                                frame.GetLine());
    }

private:
    GtkAssertDialog *m_dlg;
};

// the callback functions must be extern "C" to comply with GTK+ declarations
extern "C"
{
    void get_stackframe_callback(StackDump *dump)
    {
        // skip over frames up to including wxOnAssert()
        dump->ProcessFrames(3);
    }
}

#endif      // wxUSE_STACKWALKER

bool wxGUIAppTraits::ShowAssertDialog(const wxString& msg)
{
    // under GTK2 we prefer to use a dialog widget written using directly GTK+;
    // in fact we cannot use a dialog written using wxWidgets: it would need
    // the wxWidgets idle processing to work correctly!
    GtkWidget *dialog = gtk_assert_dialog_new();
    gtk_assert_dialog_set_message(GTK_ASSERT_DIALOG(dialog), msg.mb_str());

#if wxUSE_STACKWALKER
    // don't show more than maxLines or we could get a dialog too tall to be
    // shown on screen: 20 should be ok everywhere as even with 15 pixel high
    // characters it is still only 300 pixels...
    static const int maxLines = 20;

    // save current stack frame...
    StackDump dump(GTK_ASSERT_DIALOG(dialog));
    dump.SaveStack(maxLines);

    // ...but process it only if the user needs it
    gtk_assert_dialog_set_backtrace_callback(GTK_ASSERT_DIALOG(dialog),
                                             (GtkAssertDialogStackFrameCallback)get_stackframe_callback,
                                             &dump);
#endif      // wxUSE_STACKWALKER

    gint result = gtk_dialog_run(GTK_DIALOG (dialog));
    bool returnCode = false;
    switch (result)
    {
    case GTK_ASSERT_DIALOG_STOP:
        wxTrap();
        break;
    case GTK_ASSERT_DIALOG_CONTINUE:
        // nothing to do
        break;
    case GTK_ASSERT_DIALOG_CONTINUE_SUPPRESSING:
        // no more asserts
        returnCode = true;
        break;

    default:
        wxFAIL_MSG( _T("unexpected return code from GtkAssertDialog") );
    }

    gtk_widget_destroy(dialog);
    return returnCode;
}

#endif  // __WXDEBUG__

wxString wxGUIAppTraits::GetDesktopEnvironment() const
{
    wxString de = wxSystemOptions::GetOption(_T("gtk.desktop"));
#if wxUSE_DETECT_SM
    if ( de.empty() )
    {
        static const wxString s_SM = GetSM();

        if (s_SM == wxT("GnomeSM"))
            de = wxT("GNOME");
        else if (s_SM == wxT("KDE"))
            de = wxT("KDE");
    }
#endif // wxUSE_DETECT_SM

    return de;
}

#ifdef __WXGTK26__

// see the hack below in wxCmdLineParser::GetUsageString().
// TODO: replace this hack with a g_option_group_get_entries()
//       call as soon as such function exists
struct _GOptionGroup
{
  gchar           *name;
  gchar           *description;
  gchar           *help_description;

  GDestroyNotify   destroy_notify;
  gpointer         user_data;

  GTranslateFunc   translate_func;
  GDestroyNotify   translate_notify;
  gpointer     translate_data;

  GOptionEntry    *entries;
  gint             n_entries;

  GOptionParseFunc pre_parse_func;
  GOptionParseFunc post_parse_func;
  GOptionErrorFunc error_func;
};

wxString wxGetNameFromGtkOptionEntry(const GOptionEntry *opt)
{
    wxString ret;

    if (opt->short_name)
        ret << _T("-") << opt->short_name;
    if (opt->long_name)
    {
        if (!ret.empty())
            ret << _T(", ");
        ret << _T("--") << opt->long_name;

        if (opt->arg_description)
            ret << _T("=") << opt->arg_description;
    }

    return _T("  ") + ret;
}

#endif // __WXGTK26__

wxString
wxGUIAppTraits::GetStandardCmdLineOptions(wxArrayString& names,
                                          wxArrayString& desc) const
{
    wxString usage;

#ifdef __WXGTK26__
    // check whether GTK version is greater than 2.6 but also lower than 2.12
    // because, as we use the undocumented _GOptionGroup struct, we don't want
    // to run this code with future versions which might change it (2.11 is the
    // latest one at the time of this writing)
    if (!gtk_check_version(2,6,0) &&
        gtk_check_version(2,12,0))
    {
        usage << _("The following standard GTK+ options are also supported:\n");

        // passing true here means that the function can open the default
        // display while parsing (not really used here anyhow)
        GOptionGroup *gtkOpts = gtk_get_option_group(true);

        // WARNING: here we access the internals of GOptionGroup:
        GOptionEntry *entries = ((_GOptionGroup*)gtkOpts)->entries;
        unsigned int n_entries = ((_GOptionGroup*)gtkOpts)->n_entries;
        wxArrayString namesOptions, descOptions;

        for ( size_t n = 0; n < n_entries; n++ )
        {
            if ( entries[n].flags & G_OPTION_FLAG_HIDDEN )
                continue;       // skip

            names.push_back(wxGetNameFromGtkOptionEntry(&entries[n]));

            const gchar * const entryDesc = entries[n].description;
            desc.push_back(entryDesc ? wxString(entryDesc) : _T(""));
        }

        g_option_group_free (gtkOpts);
    }
#else
    wxUnusedVar(names);
    wxUnusedVar(desc);
#endif // __WXGTK26__

    return usage;
}

