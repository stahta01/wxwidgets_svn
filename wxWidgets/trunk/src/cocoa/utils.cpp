/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/utils.cpp
// Purpose:     Various utilities
// Author:      AUTHOR
// Modified by:
// Created:     2003/??/??
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/utils.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif // WX_PRECOMP

#include "wx/apptrait.h"
#include "wx/display.h"

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void wxDisplaySize(int *width, int *height)
{
    // TODO
    if(width)
        *width = 1024;
    if(height)
        *height = 768;
}

void wxDisplaySizeMM(int*,int*)
{
    // TODO
}

void wxClientDisplayRect(int *x,int *y,int *width,int *height)
{
    // TODO
    if(x)
        *x = 0;
    if(y)
        *y = 0;
    if(width)
        *width=1024;
    if(height)
        *height=768;
}

wxPortId wxGUIAppTraits::GetToolkitVersion(int *verMaj, int *verMin) const
{
    // We suppose that toolkit version is the same as OS version under Mac
    wxGetOsVersion(verMaj, verMin);

    return wxPORT_COCOA;
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

// Return true if we have a colour display
bool wxColourDisplay()
{
    // TODO
    return true;
}

void wxGetMousePosition( int* x, int* y )
{
    // TODO
};

// Returns depth of screen
int wxDisplayDepth()
{
    // TODO
    return 0;
}

// Emit a beeeeeep
void wxBell()
{
    // TODO
}

#if 0
// DFE: These aren't even implemented by wxGTK, and no wxWidgets code calls
// them.  If someone needs them, then they'll get a link error

// Consume all events until no more left
void wxFlushEvents()
{
}

// Check whether this window wants to process messages, e.g. Stop button
// in long calculations.
bool wxCheckForInterrupt(wxWindow *wnd)
{
    // TODO
    return false;
}

#endif

