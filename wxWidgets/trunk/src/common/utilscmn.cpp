/////////////////////////////////////////////////////////////////////////////
// Name:        utilscmn.cpp
// Purpose:     Miscellaneous utility functions and classes
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "utils.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/string.h"
    #include "wx/utils.h"
    #include "wx/intl.h"
    #include "wx/log.h"

    #if wxUSE_GUI
        #include "wx/app.h"
        #include "wx/window.h"
        #include "wx/frame.h"
        #include "wx/menu.h"
        #include "wx/msgdlg.h"
        #include "wx/textdlg.h"
        #include "wx/textctrl.h"    // for wxTE_PASSWORD
        #if wxUSE_ACCEL
            #include "wx/menuitem.h"
            #include "wx/accel.h"
        #endif // wxUSE_ACCEL
    #endif // wxUSE_GUI
#endif // WX_PRECOMP

#ifndef __WIN16__
#include "wx/process.h"
#include "wx/txtstrm.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__WATCOMC__)
    #if !(defined(_MSC_VER) && (_MSC_VER > 800))
        #include <errno.h>
    #endif
#endif

#if wxUSE_GUI
    #include "wx/colordlg.h"
    #include "wx/fontdlg.h"
    #include "wx/notebook.h"
    #include "wx/frame.h"
    #include "wx/statusbr.h"
#endif // wxUSE_GUI

#include <time.h>

#ifndef __MWERKS__
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

#ifdef __SALFORDC__
    #include <clib.h>
#endif

#ifdef __WXMSW__
    #include "wx/msw/private.h"
#endif

#if 1 // def __WXBASE__

// ----------------------------------------------------------------------------
// common data
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_2
    const wxChar *wxInternalErrorStr = wxT("wxWindows Internal Error");
    const wxChar *wxFatalErrorStr = wxT("wxWindows Fatal Error");
#endif // WXWIN_COMPATIBILITY_2_2

// ============================================================================
// implementation
// ============================================================================

#if WXWIN_COMPATIBILITY_2_4

wxChar *
copystring (const wxChar *s)
{
  if (s == NULL) s = wxT("");
  size_t len = wxStrlen (s) + 1;

  wxChar *news = new wxChar[len];
  memcpy (news, s, len * sizeof(wxChar));    // Should be the fastest

  return news;
}

#endif // WXWIN_COMPATIBILITY_2_4

// Id generation
static long wxCurrentId = 100;

long
wxNewId (void)
{
  return wxCurrentId++;
}

long
wxGetCurrentId(void) { return wxCurrentId; }

void
wxRegisterId (long id)
{
  if (id >= wxCurrentId)
    wxCurrentId = id + 1;
}

// ----------------------------------------------------------------------------
// String <-> Number conversions (deprecated)
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_4

WXDLLEXPORT_DATA(const wxChar *) wxFloatToStringStr = wxT("%.2f");
WXDLLEXPORT_DATA(const wxChar *) wxDoubleToStringStr = wxT("%.2f");

void
StringToFloat (const wxChar *s, float *number)
{
  if (s && *s && number)
    *number = (float) wxStrtod (s, (wxChar **) NULL);
}

void
StringToDouble (const wxChar *s, double *number)
{
  if (s && *s && number)
    *number = wxStrtod (s, (wxChar **) NULL);
}

wxChar *
FloatToString (float number, const wxChar *fmt)
{
  static wxChar buf[256];

  wxSprintf (buf, fmt, number);
  return buf;
}

wxChar *
DoubleToString (double number, const wxChar *fmt)
{
  static wxChar buf[256];

  wxSprintf (buf, fmt, number);
  return buf;
}

void
StringToInt (const wxChar *s, int *number)
{
  if (s && *s && number)
    *number = (int) wxStrtol (s, (wxChar **) NULL, 10);
}

void
StringToLong (const wxChar *s, long *number)
{
  if (s && *s && number)
    *number = wxStrtol (s, (wxChar **) NULL, 10);
}

wxChar *
IntToString (int number)
{
  static wxChar buf[20];

  wxSprintf (buf, wxT("%d"), number);
  return buf;
}

wxChar *
LongToString (long number)
{
  static wxChar buf[20];

  wxSprintf (buf, wxT("%ld"), number);
  return buf;
}

#endif // WXWIN_COMPATIBILITY_2_4

// Array used in DecToHex conversion routine.
static wxChar hexArray[] = wxT("0123456789ABCDEF");

// Convert 2-digit hex number to decimal
int wxHexToDec(const wxString& buf)
{
  int firstDigit, secondDigit;

  if (buf.GetChar(0) >= wxT('A'))
    firstDigit = buf.GetChar(0) - wxT('A') + 10;
  else
    firstDigit = buf.GetChar(0) - wxT('0');

  if (buf.GetChar(1) >= wxT('A'))
    secondDigit = buf.GetChar(1) - wxT('A') + 10;
  else
    secondDigit = buf.GetChar(1) - wxT('0');

  return (firstDigit & 0xF) * 16 + (secondDigit & 0xF );
}

// Convert decimal integer to 2-character hex string
void wxDecToHex(int dec, wxChar *buf)
{
  int firstDigit = (int)(dec/16.0);
  int secondDigit = (int)(dec - (firstDigit*16.0));
  buf[0] = hexArray[firstDigit];
  buf[1] = hexArray[secondDigit];
  buf[2] = 0;
}

// Convert decimal integer to 2-character hex string
wxString wxDecToHex(int dec)
{
    wxChar buf[3];
    wxDecToHex(dec, buf);
    return wxString(buf);
}

// ----------------------------------------------------------------------------
// misc functions
// ----------------------------------------------------------------------------

// Return the current date/time
wxString wxNow()
{
    time_t now = time((time_t *) NULL);
    char *date = ctime(&now);
    date[24] = '\0';
    return wxString::FromAscii(date);
}

const wxChar *wxGetInstallPrefix()
{
    wxString prefix;

    if ( wxGetEnv(wxT("WXPREFIX"), &prefix) )
        return prefix.c_str();

#ifdef wxINSTALL_PREFIX
    return wxT(wxINSTALL_PREFIX);
#else
    return wxT("");
#endif
}

wxString wxGetDataDir()
{
    wxString format = wxGetInstallPrefix();
    format <<  wxFILE_SEP_PATH
           << wxT("share") << wxFILE_SEP_PATH
           << wxT("wx") << wxFILE_SEP_PATH
           << wxT("%i.%i");
    wxString dir;
    dir.Printf(format.c_str(), wxMAJOR_VERSION, wxMINOR_VERSION);
    return dir;
}


// ----------------------------------------------------------------------------
// network and user id functions
// ----------------------------------------------------------------------------

// Get Full RFC822 style email address
bool wxGetEmailAddress(wxChar *address, int maxSize)
{
    wxString email = wxGetEmailAddress();
    if ( !email )
        return FALSE;

    wxStrncpy(address, email, maxSize - 1);
    address[maxSize - 1] = wxT('\0');

    return TRUE;
}

wxString wxGetEmailAddress()
{
    wxString email;

    wxString host = wxGetFullHostName();
    if ( !!host )
    {
        wxString user = wxGetUserId();
        if ( !!user )
        {
            email << user << wxT('@') << host;
        }
    }

    return email;
}

wxString wxGetUserId()
{
    static const int maxLoginLen = 256; // FIXME arbitrary number

    wxString buf;
    bool ok = wxGetUserId(buf.GetWriteBuf(maxLoginLen), maxLoginLen);
    buf.UngetWriteBuf();

    if ( !ok )
        buf.Empty();

    return buf;
}

wxString wxGetUserName()
{
    static const int maxUserNameLen = 1024; // FIXME arbitrary number

    wxString buf;
    bool ok = wxGetUserName(buf.GetWriteBuf(maxUserNameLen), maxUserNameLen);
    buf.UngetWriteBuf();

    if ( !ok )
        buf.Empty();

    return buf;
}

wxString wxGetHostName()
{
    static const size_t hostnameSize = 257;

    wxString buf;
    bool ok = wxGetHostName(buf.GetWriteBuf(hostnameSize), hostnameSize);

    buf.UngetWriteBuf();

    if ( !ok )
        buf.Empty();

    return buf;
}

wxString wxGetFullHostName()
{
    static const size_t hostnameSize = 257;

    wxString buf;
    bool ok = wxGetFullHostName(buf.GetWriteBuf(hostnameSize), hostnameSize);

    buf.UngetWriteBuf();

    if ( !ok )
        buf.Empty();

    return buf;
}

wxString wxGetHomeDir()
{
    wxString home;
    wxGetHomeDir(&home);

    return home;
}

#if 0

wxString wxGetCurrentDir()
{
    wxString dir;
    size_t len = 1024;
    bool ok;
    do
    {
        ok = getcwd(dir.GetWriteBuf(len + 1), len) != NULL;
        dir.UngetWriteBuf();

        if ( !ok )
        {
            if ( errno != ERANGE )
            {
                wxLogSysError(_T("Failed to get current directory"));

                return wxEmptyString;
            }
            else
            {
                // buffer was too small, retry with a larger one
                len *= 2;
            }
        }
        //else: ok
    } while ( !ok );

    return dir;
}

#endif // 0

// ----------------------------------------------------------------------------
// wxExecute
// ----------------------------------------------------------------------------

// wxDoExecuteWithCapture() helper: reads an entire stream into one array
//
// returns TRUE if ok, FALSE if error
#if wxUSE_STREAMS
static bool ReadAll(wxInputStream *is, wxArrayString& output)
{
    wxCHECK_MSG( is, FALSE, _T("NULL stream in wxExecute()?") );

    // the stream could be already at EOF or in wxSTREAM_BROKEN_PIPE state
    is->Reset();

    wxTextInputStream tis(*is);

    bool cont = TRUE;
    while ( cont )
    {
        wxString line = tis.ReadLine();
        if ( is->Eof() )
            break;

        if ( !*is )
        {
            cont = FALSE;
        }
        else
        {
            output.Add(line);
        }
    }

    return cont;
}
#endif // wxUSE_STREAMS

// this is a private function because it hasn't a clean interface: the first
// array is passed by reference, the second by pointer - instead we have 2
// public versions of wxExecute() below
static long wxDoExecuteWithCapture(const wxString& command,
                                   wxArrayString& output,
                                   wxArrayString* error)
{
#ifdef __WIN16__
    wxFAIL_MSG("Sorry, this version of wxExecute not implemented on WIN16.");

    return 0;
#else // !Win16
    // create a wxProcess which will capture the output
    wxProcess *process = new wxProcess;
    process->Redirect();

    long rc = wxExecute(command, wxEXEC_SYNC, process);

#if wxUSE_STREAMS
    if ( rc != -1 )
    {
        if ( !ReadAll(process->GetInputStream(), output) )
            rc = -1;

        if ( error )
        {
            if ( !ReadAll(process->GetErrorStream(), *error) )
                rc = -1;
        }

    }
#endif // wxUSE_STREAMS

    delete process;

    return rc;
#endif // IO redirection supported
}

long wxExecute(const wxString& command, wxArrayString& output)
{
    return wxDoExecuteWithCapture(command, output, NULL);
}

long wxExecute(const wxString& command,
               wxArrayString& output,
               wxArrayString& error)
{
    return wxDoExecuteWithCapture(command, output, &error);
}

// ----------------------------------------------------------------------------
// wxApp::Yield() wrappers for backwards compatibility
// ----------------------------------------------------------------------------

bool wxYield()
{
    return wxTheApp && wxTheApp->Yield();
}

bool wxYieldIfNeeded()
{
    return wxTheApp && wxTheApp->Yield(TRUE);
}

#endif // __WXBASE__

// ============================================================================
// GUI-only functions from now on
// ============================================================================

#if wxUSE_GUI

#if wxUSE_MENUS

// ----------------------------------------------------------------------------
// Menu accelerators related functions
// ----------------------------------------------------------------------------

wxChar *wxStripMenuCodes(const wxChar *in, wxChar *out)
{
    wxString s = wxMenuItem::GetLabelFromText(in);
    if ( out )
    {
        // go smash their buffer if it's not big enough - I love char * params
        memcpy(out, s.c_str(), s.length() * sizeof(wxChar));
    }
    else
    {
        out = copystring(s);
    }

    return out;
}

wxString wxStripMenuCodes(const wxString& in)
{
    wxString out;

    size_t len = in.length();
    out.reserve(len);

    for ( size_t n = 0; n < len; n++ )
    {
        wxChar ch = in[n];
        if ( ch == _T('&') )
        {
            // skip it, it is used to introduce the accel char (or to quote
            // itself in which case it should still be skipped): note that it
            // can't be the last character of the string
            if ( ++n == len )
            {
                wxLogDebug(_T("Invalid menu string '%s'"), in.c_str());
            }
            else
            {
                // use the next char instead
                ch = in[n];
            }
        }
        else if ( ch == _T('\t') )
        {
            // everything after TAB is accel string, exit the loop
            break;
        }

        out += ch;
    }

    return out;
}

#endif // wxUSE_MENUS

// ----------------------------------------------------------------------------
// Window search functions
// ----------------------------------------------------------------------------

/*
 * If parent is non-NULL, look through children for a label or title
 * matching the specified string. If NULL, look through all top-level windows.
 *
 */

wxWindow *
wxFindWindowByLabel (const wxString& title, wxWindow * parent)
{
    return wxWindow::FindWindowByLabel( title, parent );
}


/*
 * If parent is non-NULL, look through children for a name
 * matching the specified string. If NULL, look through all top-level windows.
 *
 */

wxWindow *
wxFindWindowByName (const wxString& name, wxWindow * parent)
{
    return wxWindow::FindWindowByName( name, parent );
}

// Returns menu item id or -1 if none.
int
wxFindMenuItemId (wxFrame * frame, const wxString& menuString, const wxString& itemString)
{
#if wxUSE_MENUS
  wxMenuBar *menuBar = frame->GetMenuBar ();
  if ( menuBar )
      return menuBar->FindMenuItem (menuString, itemString);
#endif // wxUSE_MENUS

  return -1;
}

// Try to find the deepest child that contains 'pt'.
// We go backwards, to try to allow for controls that are spacially
// within other controls, but are still siblings (e.g. buttons within
// static boxes). Static boxes are likely to be created _before_ controls
// that sit inside them.
wxWindow* wxFindWindowAtPoint(wxWindow* win, const wxPoint& pt)
{
    if (!win->IsShown())
        return NULL;

    // Hack for wxNotebook case: at least in wxGTK, all pages
    // claim to be shown, so we must only deal with the selected one.
#if wxUSE_NOTEBOOK
    if (win->IsKindOf(CLASSINFO(wxNotebook)))
    {
      wxNotebook* nb = (wxNotebook*) win;
      int sel = nb->GetSelection();
      if (sel >= 0)
      {
        wxWindow* child = nb->GetPage(sel);
        wxWindow* foundWin = wxFindWindowAtPoint(child, pt);
        if (foundWin)
           return foundWin;
      }
    }
#endif

    wxWindowList::Node  *node = win->GetChildren().GetLast();
    while (node)
    {
        wxWindow* child = node->GetData();
        wxWindow* foundWin = wxFindWindowAtPoint(child, pt);
        if (foundWin)
          return foundWin;
        node = node->GetPrevious();
    }

    wxPoint pos = win->GetPosition();
    wxSize sz = win->GetSize();
    if (win->GetParent())
    {
        pos = win->GetParent()->ClientToScreen(pos);
    }

    wxRect rect(pos, sz);
    if (rect.Inside(pt))
        return win;
    else
        return NULL;
}

wxWindow* wxGenericFindWindowAtPoint(const wxPoint& pt)
{
    // Go backwards through the list since windows
    // on top are likely to have been appended most
    // recently.
    wxWindowList::Node  *node = wxTopLevelWindows.GetLast();
    while (node)
    {
        wxWindow* win = node->GetData();
        wxWindow* found = wxFindWindowAtPoint(win, pt);
        if (found)
            return found;
        node = node->GetPrevious();
    }
    return NULL;
}

// ----------------------------------------------------------------------------
// GUI helpers
// ----------------------------------------------------------------------------

/*
 * N.B. these convenience functions must be separate from msgdlgg.cpp, textdlgg.cpp
 * since otherwise the generic code may be pulled in unnecessarily.
 */

#if wxUSE_MSGDLG

int wxMessageBox(const wxString& message, const wxString& caption, long style,
                 wxWindow *parent, int WXUNUSED(x), int WXUNUSED(y) )
{
    wxMessageDialog dialog(parent, message, caption, style);

    int ans = dialog.ShowModal();
    switch ( ans )
    {
        case wxID_OK:
            return wxOK;
        case wxID_YES:
            return wxYES;
        case wxID_NO:
            return wxNO;
        case wxID_CANCEL:
            return wxCANCEL;
    }

    wxFAIL_MSG( _T("unexpected return code from wxMessageDialog") );

    return wxCANCEL;
}

#endif // wxUSE_MSGDLG

#if wxUSE_TEXTDLG

wxString wxGetTextFromUser(const wxString& message, const wxString& caption,
                        const wxString& defaultValue, wxWindow *parent,
                        int x, int y, bool WXUNUSED(centre) )
{
    wxString str;
    wxTextEntryDialog dialog(parent, message, caption, defaultValue, wxOK|wxCANCEL, wxPoint(x, y));
    if (dialog.ShowModal() == wxID_OK)
    {
        str = dialog.GetValue();
    }

    return str;
}

wxString wxGetPasswordFromUser(const wxString& message,
                               const wxString& caption,
                               const wxString& defaultValue,
                               wxWindow *parent)
{
    wxString str;
    wxTextEntryDialog dialog(parent, message, caption, defaultValue,
                             wxOK | wxCANCEL | wxTE_PASSWORD);
    if ( dialog.ShowModal() == wxID_OK )
    {
        str = dialog.GetValue();
    }

    return str;
}

#endif // wxUSE_TEXTDLG

#if wxUSE_COLOURDLG

wxColour wxGetColourFromUser(wxWindow *parent, const wxColour& colInit)
{
    wxColourData data;
    data.SetChooseFull(TRUE);
    if ( colInit.Ok() )
    {
        data.SetColour((wxColour &)colInit); // const_cast
    }

    wxColour colRet;
    wxColourDialog dialog(parent, &data);
    if ( dialog.ShowModal() == wxID_OK )
    {
        colRet = dialog.GetColourData().GetColour();
    }
    //else: leave it invalid

    return colRet;
}

#endif // wxUSE_COLOURDLG

#if wxUSE_FONTDLG

wxFont wxGetFontFromUser(wxWindow *parent, const wxFont& fontInit)
{
    wxFontData data;
    if ( fontInit.Ok() )
    {
        data.SetInitialFont(fontInit);
    }

    wxFont fontRet;
    wxFontDialog dialog(parent, data);
    if ( dialog.ShowModal() == wxID_OK )
    {
        fontRet = dialog.GetFontData().GetChosenFont();
    }
    //else: leave it invalid

    return fontRet;
}

#endif // wxUSE_FONTDLG
// ----------------------------------------------------------------------------
// missing C RTL functions (FIXME shouldn't be here at all)
// ----------------------------------------------------------------------------

#if defined( __MWERKS__ ) && !defined(__MACH__)
char *strdup(const char *s)
{
        return strcpy( (char*) malloc( strlen( s ) + 1 ) , s ) ;
}
int isascii( int c )
{
        return ( c >= 0 && c < 128 ) ;
}
#endif // __MWERKS__

// ----------------------------------------------------------------------------
// wxSafeYield and supporting functions
// ----------------------------------------------------------------------------

void wxEnableTopLevelWindows(bool enable)
{
    wxWindowList::Node *node;
    for ( node = wxTopLevelWindows.GetFirst(); node; node = node->GetNext() )
        node->GetData()->Enable(enable);
}

wxWindowDisabler::wxWindowDisabler(wxWindow *winToSkip)
{
    // remember the top level windows which were already disabled, so that we
    // don't reenable them later
    m_winDisabled = NULL;

    wxWindowList::Node *node;
    for ( node = wxTopLevelWindows.GetFirst(); node; node = node->GetNext() )
    {
        wxWindow *winTop = node->GetData();
        if ( winTop == winToSkip )
            continue;

        // we don't need to disable the hidden or already disabled windows
        if ( winTop->IsEnabled() && winTop->IsShown() )
        {
            winTop->Disable();
        }
        else
        {
            if ( !m_winDisabled )
            {
                m_winDisabled = new wxWindowList;
            }

            m_winDisabled->Append(winTop);
        }
    }
}

wxWindowDisabler::~wxWindowDisabler()
{
    wxWindowList::Node *node;
    for ( node = wxTopLevelWindows.GetFirst(); node; node = node->GetNext() )
    {
        wxWindow *winTop = node->GetData();
        if ( !m_winDisabled || !m_winDisabled->Find(winTop) )
        {
            winTop->Enable();
        }
        //else: had been already disabled, don't reenable
    }

    delete m_winDisabled;
}

// Yield to other apps/messages and disable user input to all windows except
// the given one
bool wxSafeYield(wxWindow *win, bool onlyIfNeeded)
{
    wxWindowDisabler wd(win);

    bool rc;
    if (onlyIfNeeded)
        rc = wxYieldIfNeeded();
    else
        rc = wxYield();

    return rc;
}

// Don't synthesize KeyUp events holding down a key and producing KeyDown
// events with autorepeat. On by default and always on in wxMSW. wxGTK version
// in utilsgtk.cpp.
#ifndef __WXGTK__
bool wxSetDetectableAutoRepeat( bool WXUNUSED(flag) )
{
    return TRUE;    // detectable auto-repeat is the only mode MSW supports
}
#endif // !wxGTK

#endif // wxUSE_GUI

