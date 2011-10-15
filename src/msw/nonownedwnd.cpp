///////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/nonownedwnd.cpp
// Purpose:     wxNonOwnedWindow implementation for MSW.
// Author:      Vadim Zeitlin
// Created:     2011-10-09 (extracted from src/msw/toplevel.cpp)
// RCS-ID:      $Id: $
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// This class can't be implemented and hence is not used under Win CE.
#ifndef __WXWINCE__

#ifndef WX_PRECOMP
    #include "wx/frame.h"       // Only for wxFRAME_SHAPED.
    #include "wx/region.h"
    #include "wx/msw/private.h"
#endif // WX_PRECOMP

#include "wx/nonownedwnd.h"

// ============================================================================
// wxNonOwnedWindow implementation
// ============================================================================

bool wxNonOwnedWindow::SetShape(const wxRegion& region)
{
    wxCHECK_MSG( HasFlag(wxFRAME_SHAPED), false,
                 wxT("Shaped windows must be created with the wxFRAME_SHAPED style."));

    // The empty region signifies that the shape should be removed from the
    // window.
    if ( region.IsEmpty() )
    {
        if (::SetWindowRgn(GetHwnd(), NULL, TRUE) == 0)
        {
            wxLogLastError(wxT("SetWindowRgn"));
            return false;
        }
        return true;
    }

    // Windows takes ownership of the region, so
    // we'll have to make a copy of the region to give to it.
    DWORD noBytes = ::GetRegionData(GetHrgnOf(region), 0, NULL);
    RGNDATA *rgnData = (RGNDATA*) new char[noBytes];
    ::GetRegionData(GetHrgnOf(region), noBytes, rgnData);
    HRGN hrgn = ::ExtCreateRegion(NULL, noBytes, rgnData);
    delete[] (char*) rgnData;

    // SetWindowRgn expects the region to be in coordinates
    // relative to the window, not the client area.  Figure
    // out the offset, if any.
    RECT rect;
    DWORD dwStyle =   ::GetWindowLong(GetHwnd(), GWL_STYLE);
    DWORD dwExStyle = ::GetWindowLong(GetHwnd(), GWL_EXSTYLE);
    ::GetClientRect(GetHwnd(), &rect);
    ::AdjustWindowRectEx(&rect, dwStyle, ::GetMenu(GetHwnd()) != NULL, dwExStyle);
    ::OffsetRgn(hrgn, -rect.left, -rect.top);

    // Now call the shape API with the new region.
    if (::SetWindowRgn(GetHwnd(), hrgn, TRUE) == 0)
    {
        wxLogLastError(wxT("SetWindowRgn"));
        return false;
    }
    return true;
}

#endif // !__WXWINCE__
