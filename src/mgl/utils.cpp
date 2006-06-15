/////////////////////////////////////////////////////////////////////////////
// Name:        src/mgl/utils.cpp
// Purpose:
// Author:      Vaclav Slavik
// Id:          $Id$
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/utils.h"

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/intl.h"
    #include "wx/log.h"
#endif

#include "wx/apptrait.h"
#include "wx/process.h"

#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <mgraph.hpp>

#ifdef __UNIX__
#include "wx/unix/execute.h"
#endif

#include "wx/mgl/private.h"

//----------------------------------------------------------------------------
// misc.
//----------------------------------------------------------------------------

void wxBell()
{
    // FIXME_MGL
}

bool wxGetKeyState(wxKeyCode key)
{
    wxASSERT_MSG(key != WXK_LBUTTON && key != WXK_RBUTTON && key !=
        WXK_MBUTTON, wxT("can't use wxGetKeyState() for mouse buttons"));

    // TODO

    return false;
}

// ----------------------------------------------------------------------------
// display characterstics
// ----------------------------------------------------------------------------

void wxDisplaySize(int *width, int *height)
{
   wxASSERT_MSG( g_displayDC, wxT("You must call wxApp::SetDisplayMode before using this function") );
    if (width) *width = g_displayDC->sizex()+1;
    if (height) *height = g_displayDC->sizey()+1;
}

void wxDisplaySizeMM(int *width, int *height)
{
    wxASSERT_MSG( g_displayDC, wxT("You must call wxApp::SetDisplayMode before using this function") );

    int xDPI, yDPI;
    MGL_getDotsPerInch(&xDPI, &yDPI);

    if ( width )
        *width = (int)((g_displayDC->sizex()+1) * 25.4 / xDPI);
    if ( height )
        *height = (int)((g_displayDC->sizey()+1) * 25.4 / yDPI);
}

void wxClientDisplayRect(int *x, int *y, int *width, int *height)
{
    if ( x ) *x = 0;
    if ( y ) *y = 0;
    wxDisplaySize(width, height);
    // FIXME_MGL - windowed version needs different handling
}

bool wxColourDisplay()
{
    wxASSERT_MSG( g_displayDC, wxT("You must call wxApp::SetDisplayMode before using this function") );

    return (wxDisplayDepth() > 1);
}

int wxDisplayDepth()
{
    wxASSERT_MSG( g_displayDC, wxT("You must call wxApp::SetDisplayMode before using this function") );

    return g_displayDC->getBitsPerPixel();
}

#if wxUSE_GUI

wxToolkitInfo& wxGUIAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    info.shortName = _T("mgluniv");
    info.name = _T("wxMGL");
    info.versionMajor = MGL_RELEASE_MAJOR;
    info.versionMinor = MGL_RELEASE_MINOR;
    info.os = wxGTK;
#if defined(__UNIX__)
    info.os = wxMGL_UNIX;
#elif defined(__OS2__)
    info.os = wxMGL_OS2;
#elif defined(__WIN32__)
    info.os = wxMGL_WIN32;
#elif defined(__DOS__)
    info.os = wxMGL_DOS;
#else
    #error Platform not supported by wxMGL!
#endif
    return info;
}

#endif

#if 0
wxToolkitInfo& wxConsoleAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    info.shortName = _T("mglbase");
    info.versionMajor = MGL_RELEASE_MAJOR;
    info.versionMinor = MGL_RELEASE_MINOR;
    info.name = _T("wxBase");
    info.os = wxGTK;
#if defined(__UNIX__)
    info.os = wxMGL_UNIX;
#elif defined(__OS2__)
    info.os = wxMGL_OS2;
#elif defined(__WIN32__)
    info.os = wxMGL_WIN32;
#elif defined(__DOS__)
    info.os = wxMGL_DOS;
#else
    #error Platform not supported by wxMGL!
#endif
    return info;
}
#endif

void wxGetMousePosition(int* x, int* y)
{
    MS_getPos(x, y);
}

wxPoint wxGetMousePosition()
{
    wxPoint pt;
    wxGetMousePosition(&pt.x, &pt.y);
    return pt;
}

wxMouseState wxGetMouseState()
{
    wxMouseState ms;
    int x, y;

    wxGetMousePosition(&x, &y);

    ms.SetX(x);
    ms.SetY(y);

    extern unsigned long g_buttonState;
    ms.SetLeftDown(g_buttonState & EVT_LEFTBUT);
    ms.SetMiddleDown(g_buttonState & EVT_MIDDLEBUT);
    ms.SetRightDown(g_buttonState & EVT_RIGHTBUT);

    ms.SetControlDown(EVT_isKeyDown(KB_leftCtrl) || EVT_isKeyDown(KB_rightCtrl));
    ms.SetShiftDown(EVT_isKeyDown(KB_leftShift) || EVT_isKeyDown(KB_rightShift));
    ms.SetAltDown(EVT_isKeyDown(KB_leftAlt));
    ms.SetMetaDown(EVT_isKeyDown(KB_rightAlt));

    return ms;
}


#ifdef __UNIX__

int wxAddProcessCallback(wxEndProcessData *proc_data, int fd)
{
    wxFAIL_MSG(wxT("wxAddProcessCallback not implemented in wxMGL!"));
    return 0;
#if 0 // FIXME_MGL -do we need it at all?
    int tag = gdk_input_add(fd,
                            GDK_INPUT_READ,
                            GTK_EndProcessDetector,
                            (gpointer)proc_data);

    return tag;
#endif
}

#endif
