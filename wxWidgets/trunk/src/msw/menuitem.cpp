///////////////////////////////////////////////////////////////////////////////
// Name:        menuitem.cpp
// Purpose:     wxMenuItem implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     11.11.97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "menuitem.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/font.h"
    #include "wx/bitmap.h"
    #include "wx/settings.h"
    #include "wx/font.h"
    #include "wx/window.h"
    #include "wx/accel.h"
    #include "wx/menu.h"
    #include "wx/string.h"
#endif

#include "wx/menuitem.h"
#include "wx/log.h"

#include "wx/msw/private.h"

// ---------------------------------------------------------------------------
// macro
// ---------------------------------------------------------------------------

// hide the ugly cast
#define GetHMenuOf(menu)    ((HMENU)menu->GetHMenu())

// conditional compilation
#if wxUSE_OWNER_DRAWN
    #define OWNER_DRAWN_ONLY( code ) if ( IsOwnerDrawn() ) code
#else // !wxUSE_OWNER_DRAWN
    #define OWNER_DRAWN_ONLY( code )
#endif // wxUSE_OWNER_DRAWN/!wxUSE_OWNER_DRAWN

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// dynamic classes implementation
// ----------------------------------------------------------------------------

#if     !defined(USE_SHARED_LIBRARY) || !USE_SHARED_LIBRARY
    #if wxUSE_OWNER_DRAWN
        IMPLEMENT_DYNAMIC_CLASS2(wxMenuItem, wxMenuItemBase, wxOwnerDrawn)
    #else   //!USE_OWNER_DRAWN
        IMPLEMENT_DYNAMIC_CLASS(wxMenuItem, wxMenuItemBase)
    #endif  //USE_OWNER_DRAWN
#endif  //USE_SHARED_LIBRARY

// ----------------------------------------------------------------------------
// wxMenuItem
// ----------------------------------------------------------------------------

// ctor & dtor
// -----------

wxMenuItem::wxMenuItem(wxMenu *pParentMenu,
                       int id,
                       const wxString& text,
                       const wxString& strHelp,
                       bool bCheckable,
                       wxMenu *pSubMenu) :
#if wxUSE_OWNER_DRAWN
                        wxOwnerDrawn(text, bCheckable)
#endif // owner drawn
{
    wxASSERT_MSG( pParentMenu != NULL, wxT("a menu item should have a parent") );

#if  wxUSE_OWNER_DRAWN
    // set default menu colors
    #define SYS_COLOR(c) (wxSystemSettings::GetSystemColour(wxSYS_COLOUR_##c))

    SetTextColour(SYS_COLOR(MENUTEXT));
    SetBackgroundColour(SYS_COLOR(MENU));

    // we don't want normal items be owner-drawn
    ResetOwnerDrawn();

    #undef  SYS_COLOR
#endif // wxUSE_OWNER_DRAWN

    m_parentMenu  = pParentMenu;
    m_subMenu     = pSubMenu;
    m_isEnabled   = TRUE;
    m_isChecked   = FALSE;
    m_id          = id;
    m_text        = text;
    m_isCheckable = bCheckable;
    m_help        = strHelp;
}

wxMenuItem::~wxMenuItem()
{
}

// misc
// ----

// return the id for calling Win32 API functions
int wxMenuItem::GetRealId() const
{
    return m_subMenu ? (int)m_subMenu->GetHMenu() : GetId();
}

// delete the sub menu
// -------------------
void wxMenuItem::DeleteSubMenu()
{
    delete m_subMenu;
    m_subMenu = NULL;
}

// change item state
// -----------------

void wxMenuItem::Enable(bool bDoEnable)
{
    if ( m_isEnabled != bDoEnable ) {
        long rc = EnableMenuItem(GetHMenuOf(m_parentMenu),
                                 GetRealId(),
                                 MF_BYCOMMAND |
                                 (bDoEnable ? MF_ENABLED : MF_GRAYED));

        if ( rc == -1 ) {
            wxLogLastError("EnableMenuItem");
        }

        wxMenuItemBase::Enable(m_isEnabled);
    }
}

void wxMenuItem::Check(bool bDoCheck)
{
    wxCHECK_RET( m_isCheckable, wxT("only checkable items may be checked") );

    if ( m_isChecked != bDoCheck ) {
        long rc = CheckMenuItem(GetHMenuOf(m_parentMenu),
                                GetId(),
                                MF_BYCOMMAND |
                                (bDoCheck ? MF_CHECKED : MF_UNCHECKED));

        if ( rc == -1 ) {
            wxLogLastError("CheckMenuItem");
        }

        wxMenuItemBase::Check(m_isChecked);
    }
}

void wxMenuItem::SetText(const wxString& text)
{
    // don't do anything if label didn't change
    if ( m_text == text )
        return;

    wxMenuItemBase::SetText(text);
    OWNER_DRAWN_ONLY( wxOwnerDrawn::SetName(text) );

    HMENU hMenu = GetHMenuOf(m_parentMenu);

    UINT id = GetRealId();
    UINT flagsOld = ::GetMenuState(hMenu, id, MF_BYCOMMAND);
    if ( flagsOld == 0xFFFFFFFF )
    {
        wxLogLastError("GetMenuState");
    }
    else
    {
        if ( IsSubMenu() )
        {
            // high byte contains the number of items in a submenu for submenus
            flagsOld &= 0xFF;
            flagsOld |= MF_POPUP;
        }

        LPCTSTR data;

#if wxUSE_OWNER_DRAWN
        if ( IsOwnerDrawn() )
        {
            flagsOld |= MF_OWNERDRAW;
            data = (LPCTSTR)this;
        }
        else
#endif  //owner drawn
        {
            flagsOld |= MF_STRING;
            data = (char*) text.c_str();
        }

        if ( ::ModifyMenu(hMenu, id,
                          MF_BYCOMMAND | flagsOld,
                          id, data) == (int)0xFFFFFFFF )
        {
            wxLogLastError(wxT("ModifyMenu"));
        }
    }
}

void wxMenuItem::SetCheckable(bool checkable)
{
    wxMenuItemBase::SetCheckable(checkable);
    OWNER_DRAWN_ONLY( wxOwnerDrawn::SetCheckable(checkable) );
}

// ----------------------------------------------------------------------------
// wxMenuItemBase
// ----------------------------------------------------------------------------

wxMenuItem *wxMenuItemBase::New(wxMenu *parentMenu,
                                int id,
                                const wxString& name,
                                const wxString& help,
                                bool isCheckable,
                                wxMenu *subMenu)
{
    return new wxMenuItem(parentMenu, id, name, help, isCheckable, subMenu);
}
