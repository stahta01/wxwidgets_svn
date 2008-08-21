/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/utils_osx.cpp
// Purpose:     Various utilities
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: utils.cpp 54886 2008-07-31 13:02:53Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#include "wx/wxprec.h"

#include "wx/utils.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/app.h"
    #if wxUSE_GUI
        #include "wx/toplevel.h"
        #include "wx/font.h"
    #endif
#endif

#include "wx/apptrait.h"

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// #include "MoreFilesX.h"

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
    #include <AudioToolbox/AudioServices.h>
#endif

#include "wx/osx/private.h"

#ifdef wxOSX_USE_COCOA
// to get the themeing APIs
#include <Carbon/Carbon.h>
#endif

#if wxUSE_GUI
    #include "wx/osx/private/timer.h"
#endif // wxUSE_GUI

#include "wx/evtloop.h"

#if defined(__MWERKS__) && wxUSE_UNICODE
#if __MWERKS__ < 0x4100
    #include <wtime.h>
#endif
#endif

//
// TODO BEGIN move to utils_osx.cpp
//

#if wxUSE_BASE

extern bool WXDLLEXPORT wxIsDebuggerRunning()
{
    // TODO : try to find out ...
    return false;
}

#if wxOSX_USE_COCOA_OR_CARBON

// have a fast version for mac code that returns the version as a return value

long UMAGetSystemVersion() 
{ 
    static SInt32 sUMASystemVersion = 0 ;
    if ( sUMASystemVersion == 0 )
    {
        verify_noerr(Gestalt(gestaltSystemVersion, &sUMASystemVersion));
    }
    return sUMASystemVersion ; 
}

// our OS version is the same in non GUI and GUI cases
wxOperatingSystemId wxGetOsVersion(int *majorVsn, int *minorVsn)
{
    SInt32 theSystem;
    Gestalt(gestaltSystemVersion, &theSystem);

    if ( majorVsn != NULL )
        *majorVsn = (theSystem >> 8);

    if ( minorVsn != NULL )
        *minorVsn = (theSystem & 0xFF);

    return wxOS_MAC_OSX_DARWIN;
}

#include <sys/utsname.h>

wxString wxGetOsDescription()
{
    struct utsname name;
    uname(&name);
    return wxString::Format(_T("Mac OS X (%s %s %s)"),
            wxString::FromAscii(name.sysname).c_str(),
            wxString::FromAscii(name.release).c_str(),
            wxString::FromAscii(name.machine).c_str());
}

#endif // wxOSX_USE_COCOA_OR_CARBON


//---------------------------------------------------------------------------
// wxMac Specific utility functions
//---------------------------------------------------------------------------

void wxMacStringToPascal( const wxString&from , StringPtr to )
{
    wxCharBuffer buf = from.mb_str( wxConvLocal );
    int len = strlen(buf);

    if ( len > 255 )
        len = 255;
    to[0] = len;
    memcpy( (char*) &to[1] , buf , len );
}

wxString wxMacMakeStringFromPascal( ConstStringPtr from )
{
    return wxString( (char*) &from[1] , wxConvLocal , from[0] );
}

#endif // wxUSE_BASE

#if wxUSE_GUI

// Check whether this window wants to process messages, e.g. Stop button
// in long calculations.
bool wxCheckForInterrupt(wxWindow *WXUNUSED(wnd))
{
    // TODO
    return false;
}

// Return true if we have a colour display
bool wxColourDisplay()
{
    return true;
}

#if wxOSX_USE_COCOA_OR_CARBON
// Returns depth of screen
int wxDisplayDepth()
{
    int theDepth = (int) CGDisplayBitsPerPixel(CGMainDisplayID());
    return theDepth;
}

// Get size of display
void wxDisplaySize(int *width, int *height)
{
    // TODO adapt for multi-displays
    CGRect bounds = CGDisplayBounds(CGMainDisplayID());
    if ( width )
        *width = (int)bounds.size.width ;
    if ( height )
        *height = (int)bounds.size.height;
}
#endif

void wxDisplaySizeMM(int *width, int *height)
{
    wxDisplaySize(width, height);
    // on mac 72 is fixed (at least now;-)
    double cvPt2Mm = 25.4 / 72;

    if (width != NULL)
        *width = int( *width * cvPt2Mm );

    if (height != NULL)
        *height = int( *height * cvPt2Mm );
}


wxPortId wxGUIAppTraits::GetToolkitVersion(int *verMaj, int *verMin) const
{
    // We suppose that toolkit version is the same as OS version under Mac
    wxGetOsVersion(verMaj, verMin);

    return wxPORT_OSX;
}

wxEventLoopBase* wxGUIAppTraits::CreateEventLoop()
{
    return new wxEventLoop;
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

/*
    Return the generic RGB color space. This is a 'get' function and the caller should
    not release the returned value unless the caller retains it first. Usually callers
    of this routine will immediately use the returned colorspace with CoreGraphics
    so they typically do not need to retain it themselves.

    This function creates the generic RGB color space once and hangs onto it so it can
    return it whenever this function is called.
*/

CGColorSpaceRef wxMacGetGenericRGBColorSpace()
{
    static wxCFRef<CGColorSpaceRef> genericRGBColorSpace;

    if (genericRGBColorSpace == NULL)
    {
#if wxOSX_USE_IPHONE
        genericRGBColorSpace.reset( CGColorSpaceCreateDeviceRGB() );
#else
        genericRGBColorSpace.reset( CGColorSpaceCreateWithName( kCGColorSpaceGenericRGB ) );
#endif
    }

    return genericRGBColorSpace;
}

#if wxOSX_USE_COCOA_OR_CARBON

CGColorRef wxMacCreateCGColorFromHITheme( ThemeBrush brush )
{
    CGColorRef color ;
    HIThemeBrushCreateCGColor( brush, &color );
    return color;
}

#endif // wxOSX_USE_COCOA_OR_CARBON

IMPLEMENT_ABSTRACT_CLASS( wxWidgetImpl , wxObject )

wxWidgetImpl::wxWidgetImpl( wxWindowMac* peer , bool isRootControl )
{
    Init();
    m_isRootControl = isRootControl;
    m_wxPeer = peer;
}

wxWidgetImpl::wxWidgetImpl()
{
    Init();
}

wxWidgetImpl::~wxWidgetImpl()
{
}

void wxWidgetImpl::Init()
{
    m_isRootControl = false;
    m_wxPeer = NULL;
    m_needsFocusRect = false;
}

void wxWidgetImpl::Destroy()
{
}

void wxWidgetImpl::SetNeedsFocusRect( bool needs )
{
    m_needsFocusRect = needs;
}

bool wxWidgetImpl::NeedsFocusRect() const
{
    return m_needsFocusRect;
}

#endif // wxUSE_GUI
