/////////////////////////////////////////////////////////////////////////////
// Name:        private.h
// Purpose:     Private declarations: as this header is only included by
//              wxWindows itself, it may contain identifiers which don't start
//              with "wx".
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRIVATE_H_
#define _WX_PRIVATE_H_

#include <windows.h>

// undefine conflicting symbols which were defined in windows.h
#include "wx/msw/winundef.h"

class WXDLLEXPORT wxFont;
class WXDLLEXPORT wxWindow;

// ---------------------------------------------------------------------------
// private constants
// ---------------------------------------------------------------------------

// Conversion
static const double METRIC_CONVERSION_CONSTANT = 0.0393700787;

// Scaling factors for various unit conversions
static const double mm2inches = (METRIC_CONVERSION_CONSTANT);
static const double inches2mm = (1/METRIC_CONVERSION_CONSTANT);

static const double mm2twips = (METRIC_CONVERSION_CONSTANT*1440);
static const double twips2mm = (1/(METRIC_CONVERSION_CONSTANT*1440));

static const double mm2pt = (METRIC_CONVERSION_CONSTANT*72);
static const double pt2mm = (1/(METRIC_CONVERSION_CONSTANT*72));

// ---------------------------------------------------------------------------
// standard icons from the resources
// ---------------------------------------------------------------------------

#if wxUSE_GUI

WXDLLEXPORT_DATA(extern HICON) wxSTD_FRAME_ICON;
WXDLLEXPORT_DATA(extern HICON) wxSTD_MDIPARENTFRAME_ICON;
WXDLLEXPORT_DATA(extern HICON) wxSTD_MDICHILDFRAME_ICON;
WXDLLEXPORT_DATA(extern HICON) wxDEFAULT_FRAME_ICON;
WXDLLEXPORT_DATA(extern HICON) wxDEFAULT_MDIPARENTFRAME_ICON;
WXDLLEXPORT_DATA(extern HICON) wxDEFAULT_MDICHILDFRAME_ICON;
WXDLLEXPORT_DATA(extern HFONT) wxSTATUS_LINE_FONT;

#endif // wxUSE_GUI

// ---------------------------------------------------------------------------
// define things missing from some compilers' headers
// ---------------------------------------------------------------------------

#if defined(__GNUWIN32__) && !defined(wxUSE_NORLANDER_HEADERS)
#ifndef ZeroMemory
    inline void ZeroMemory(void *buf, size_t len) { memset(buf, 0, len); }
#endif
#endif // old mingw32

// this defines a CASTWNDPROC macro which casts a pointer to the type of a
// window proc
#if defined(__GNUWIN32__) && !defined(wxUSE_NORLANDER_HEADERS)
#  define CASTWNDPROC (long unsigned)
#else
#  ifdef __BORLANDC__

#  ifdef __WIN32__
       typedef int (pascal * WndProcCast) ();
#      define CASTWNDPROC (WndProcCast)
// #    define CASTWNDPROC
#  else
       typedef int (pascal * WndProcCast) ();
#      define CASTWNDPROC (WndProcCast)
#  endif

#  else
#    if defined (__WIN32__) && defined(STRICT)
       typedef long (_stdcall * WndProcCast) (HWND, unsigned int, unsigned int, long);
#      define CASTWNDPROC (WndProcCast)
#    elif defined(__WIN16__)
#    ifdef __BORLANDC__
       typedef int (pascal * WndProcCast) ();
#      define CASTWNDPROC (WndProcCast)
#    else
#      if defined(__VISUALC__) && defined(STRICT)
#        define CASTWNDPROC (WNDPROC)
#      else
         typedef int (PASCAL * WndProcCast) ();
#        define CASTWNDPROC (WndProcCast)
#      endif
#    endif
#    else
#      define CASTWNDPROC
#    endif
#  endif
#endif

// ---------------------------------------------------------------------------
// some stuff for old Windows versions (FIXME: what does it do here??)
// ---------------------------------------------------------------------------

#if !defined(APIENTRY)  // NT defines APIENTRY, 3.x not
    #define APIENTRY FAR PASCAL
#endif

#ifdef __WIN32__
    #define _EXPORT
#else
    #define _EXPORT _export
#endif

#ifndef __WIN32__
    typedef signed short int SHORT;
#endif

#if !defined(__WIN32__)  // 3.x uses FARPROC for dialogs
#ifndef STRICT
    #define DLGPROC FARPROC
#endif
#endif

#if wxUSE_PENWIN
    WXDLLEXPORT void wxRegisterPenWin();
    WXDLLEXPORT void wxCleanUpPenWin();
    WXDLLEXPORT void wxEnablePenAppHooks (bool hook);
#endif // wxUSE_PENWIN

#if wxUSE_ITSY_BITSY
    #define IBS_HORZCAPTION    0x4000L
    #define IBS_VERTCAPTION    0x8000L

    UINT    WINAPI ibGetCaptionSize( HWND hWnd  ) ;
    UINT    WINAPI ibSetCaptionSize( HWND hWnd, UINT nSize ) ;
    LRESULT WINAPI ibDefWindowProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;
    VOID    WINAPI ibAdjustWindowRect( HWND hWnd, LPRECT lprc ) ;
#endif // wxUSE_ITSY_BITSY

#if wxUSE_CTL3D
    #include "wx/msw/ctl3d/ctl3d.h"
#endif // wxUSE_CTL3D

/*
 * Decide what window classes we're going to use
 * for this combination of CTl3D/FAFA settings
 */

#define STATIC_CLASS     wxT("STATIC")
#define STATIC_FLAGS     (SS_LEFT|WS_CHILD|WS_VISIBLE)
#define CHECK_CLASS      wxT("BUTTON")
#define CHECK_FLAGS      (BS_AUTOCHECKBOX|WS_TABSTOP|WS_CHILD)
#define CHECK_IS_FAFA   FALSE
#define RADIO_CLASS      wxT("BUTTON")
#define RADIO_FLAGS      (BS_AUTORADIOBUTTON|WS_CHILD|WS_VISIBLE)
#define RADIO_SIZE       20
#define RADIO_IS_FAFA   FALSE
#define PURE_WINDOWS
#define GROUP_CLASS      wxT("BUTTON")
#define GROUP_FLAGS      (BS_GROUPBOX|WS_CHILD|WS_VISIBLE)

/*
#define BITCHECK_FLAGS   (FB_BITMAP|FC_BUTTONDRAW|FC_DEFAULT|WS_VISIBLE)
#define BITRADIO_FLAGS   (FC_BUTTONDRAW|FB_BITMAP|FC_RADIO|WS_CHILD|WS_VISIBLE)
*/

// ---------------------------------------------------------------------------
// misc macros
// ---------------------------------------------------------------------------

#define MEANING_CHARACTER '0'
#define DEFAULT_ITEM_WIDTH  100
#define DEFAULT_ITEM_HEIGHT 80

// Scale font to get edit control height
#define EDIT_HEIGHT_FROM_CHAR_HEIGHT(cy)    (3*(cy)/2)

// Generic subclass proc, for panel item moving/sizing and intercept
// EDIT control VK_RETURN messages
extern LONG APIENTRY _EXPORT
  wxSubclassedGenericControlProc(WXHWND hWnd, WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

// ---------------------------------------------------------------------------
// constants which might miss from some compilers' headers
// ---------------------------------------------------------------------------

#if !defined(__WIN32__) && !defined(WS_EX_CLIENTEDGE)
    #define WS_EX_CLIENTEDGE 0
#endif

#if defined(__WIN32__) && !defined(WS_EX_CLIENTEDGE)
    #define WS_EX_CLIENTEDGE 0x00000200L
#endif

#ifndef ENDSESSION_LOGOFF
    #define ENDSESSION_LOGOFF    0x80000000
#endif

// ---------------------------------------------------------------------------
// useful macros and functions
// ---------------------------------------------------------------------------

// a wrapper macro for ZeroMemory()
#ifdef __WIN32__
#define wxZeroMemory(obj)   ::ZeroMemory(&obj, sizeof(obj))
#else
#define wxZeroMemory(obj)   memset((void*) & obj, 0, sizeof(obj))
#endif

#include <wx/gdicmn.h>

// make conversion from wxColour and COLORREF a bit less painful
inline COLORREF wxColourToRGB(const wxColour& c)
{
    return RGB(c.Red(), c.Green(), c.Blue());
}

inline void wxRGBToColour(wxColour& c, COLORREF rgb)
{
    c.Set(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
}

// translations between HIMETRIC units (which OLE likes) and pixels (which are
// liked by all the others) - implemented in msw/utilsexc.cpp
extern void HIMETRICToPixel(LONG *x, LONG *y);
extern void PixelToHIMETRIC(LONG *x, LONG *y);

// ---------------------------------------------------------------------------
// small helper classes
// ---------------------------------------------------------------------------

// create an instance of this class and use it as the HDC for screen, will
// automatically release the DC going out of scope
class ScreenHDC
{
public:
    ScreenHDC() { m_hdc = GetDC(NULL);    }
   ~ScreenHDC() { ReleaseDC(NULL, m_hdc); }
    operator HDC() const { return m_hdc;  }

private:
    HDC m_hdc;
};

// ---------------------------------------------------------------------------
// macros to make casting between WXFOO and FOO a bit easier: the GetFoo()
// returns Foo cast to the Windows type for oruselves, while GetFooOf() takes
// an argument which should be a pointer or reference to the object of the
// corresponding class (this depends on the macro)
// ---------------------------------------------------------------------------

#define GetHwnd()               ((HWND)GetHWND())
#define GetHwndOf(win)          ((HWND)((win)->GetHWND()))
// old name
#define GetWinHwnd              GetHwndOf

#define GetHdc()                ((HDC)GetHDC())
#define GetHdcOf(dc)            ((HDC)(dc).GetHDC())

#define GetHbitmap()            ((HBITMAP)GetHBITMAP())
#define GetHbitmapOf(bmp)       ((HBITMAP)(bmp).GetHBITMAP())

#define GetHicon()              ((HICON)GetHICON())
#define GetHiconOf(icon)        ((HICON)(icon).GetHICON())

#define GetHaccel()             ((HACCEL)GetHACCEL())
#define GetHaccelOf(table)      ((HACCEL)((table).GetHACCEL()))

#define GetHmenu()              ((HMENU)GetHMenu())
#define GetHmenuOf(menu)        ((HMENU)menu->GetHMenu())

// ---------------------------------------------------------------------------
// global data
// ---------------------------------------------------------------------------

#if 0 // where is this??
// The MakeProcInstance version of the function wxSubclassedGenericControlProc
WXDLLEXPORT_DATA(extern FARPROC) wxGenericControlSubClassProc;
#endif // 0

WXDLLEXPORT_DATA(extern wxChar*) wxBuffer;

WXDLLEXPORT_DATA(extern HINSTANCE) wxhInstance;

// ---------------------------------------------------------------------------
// global functions
// ---------------------------------------------------------------------------

extern "C"
{
    WXDLLEXPORT HINSTANCE wxGetInstance();
}

WXDLLEXPORT void wxSetInstance(HINSTANCE hInst);

#if wxUSE_GUI

WXDLLEXPORT wxWindow* wxFindWinFromHandle(WXHWND hWnd);

WXDLLEXPORT void wxGetCharSize(WXHWND wnd, int *x, int *y, const wxFont *the_font);
WXDLLEXPORT void wxFillLogFont(LOGFONT *logFont, const wxFont *font);
WXDLLEXPORT wxFont wxCreateFontFromLogFont(const LOGFONT *logFont);

WXDLLEXPORT void wxSliderEvent(WXHWND control, WXWORD wParam, WXWORD pos);
WXDLLEXPORT void wxScrollBarEvent(WXHWND hbar, WXWORD wParam, WXWORD pos);

// Find maximum size of window/rectangle
WXDLLEXPORT extern void wxFindMaxSize(WXHWND hwnd, RECT *rect);

WXDLLEXPORT wxWindow* wxFindControlFromHandle(WXHWND hWnd);
WXDLLEXPORT void wxAddControlHandle(WXHWND hWnd, wxWindow *item);

// Safely get the window text (i.e. without using fixed size buffer)
WXDLLEXPORT extern wxString wxGetWindowText(WXHWND hWnd);

// get the window class name
WXDLLEXPORT extern wxString wxGetWindowClass(WXHWND hWnd);

// get the window id (should be unsigned, hence this is not wxWindowID which
// is, for mainly historical reasons, signed)
WXDLLEXPORT extern WXWORD wxGetWindowId(WXHWND hWnd);

// Does this window style specify any border?
inline bool wxStyleHasBorder(long style)
{
  return (style & (wxSIMPLE_BORDER | wxRAISED_BORDER |
                   wxSUNKEN_BORDER | wxDOUBLE_BORDER)) != 0;
}

#endif // wxUSE_GUI

#endif
    // _WX_PRIVATE_H_
