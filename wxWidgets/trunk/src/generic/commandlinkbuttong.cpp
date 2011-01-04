/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/commandlinkbuttong.cpp
// Purpose:     wxGenericCommandLinkButton
// Author:      Rickard Westerlund
// Created:     2010-06-23
// RCS-ID:      $Id$
// Copyright:   (c) 2010 wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_COMMANDLINKBUTTON

#include "wx/commandlinkbutton.h"
#include "wx/artprov.h"

wxIMPLEMENT_DYNAMIC_CLASS_XTI(wxCommandLinkButton, wxButton, "wx/commandlinkbutton.h")

// ----------------------------------------------------------------------------
// Generic command link button
// ----------------------------------------------------------------------------

bool wxGenericCommandLinkButton::Create(wxWindow *parent,
                                        wxWindowID id,
                                        const wxString& mainLabel,
                                        const wxString& note,
                                        const wxPoint& pos,
                                        const wxSize& size,
                                        long style,
                                        const wxValidator& validator,
                                        const wxString& name)
{
    if ( !wxButton::Create(parent,
                           id,
                           mainLabel + '\n' + note,
                           pos,
                           size,
                           style,
                           validator,
                           name) )
        return false;

    if ( !HasNativeBitmap() )
        SetDefaultBitmap();

    return true;

}

void wxGenericCommandLinkButton::SetDefaultBitmap()
{
    SetBitmap(wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_BUTTON));
}

#endif // wxUSE_COMMANDLINKBUTTON
