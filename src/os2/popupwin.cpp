///////////////////////////////////////////////////////////////////////////////
// Name:        os2/popupwin.cpp
// Purpose:     implements wxPopupWindow for OS2
// Author:      Dave Webster
// Modified by:
// Created:     13.05.02
// RCS-ID:      $Id$
// Copyright:   (c) 2002 Dave Webster <dwebster@bhmi.com>
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "popup.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"


#ifndef WX_PRECOMP
#endif //WX_PRECOMP

#include "wx/popupwin.h"

// ============================================================================
// implementation
// ============================================================================

bool wxPopupWindow::Create(
  wxWindow*                         pParent
, int                               nFlags
)
{
    return wxPopupWindowBase::Create(pParent) &&
                    wxWindow::Create( pParent
                                     ,-1
                                     ,wxDefaultPosition
                                     ,wxDefaultSize
                                     ,nFlags | wxPOPUP_WINDOW
                                    );
} // end of wxPopupWindow::Create

void wxPopupWindow::DoGetPosition(
  int*                              pnX
, int*                              pnY
) const
{
    //
    // The position of a "top level" window such as this should be in
    // screen coordinates, not in the client ones which MSW gives us
    // (because we are a child window)
    //
    wxPopupWindowBase::DoGetPosition(pnX, pnY);
    GetParent()->ClientToScreen(pnX, pnY);
} // end of wxPopupWindow::DoGetPosition

WXDWORD wxPopupWindow::OS2GetStyle(
  long                              lFlags
, WXDWORD*                          dwExstyle
) const
{
    WXDWORD                         dwStyle = wxWindow::OS2GetStyle( lFlags & wxBORDER_MASK
                                                                    ,dwExstyle
                                                                   );

    return dwStyle;
} // end of wxPopupWindow::OS2GetStyle

