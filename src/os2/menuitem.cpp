///////////////////////////////////////////////////////////////////////////////
// Name:        menuitem.cpp
// Purpose:     wxMenuItem implementation
// Author:      David Webster
// Modified by:
// Created:     10/10/98
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// headers & declarations
// ============================================================================

#ifdef __GNUG__
    #pragma implementation "menuitem.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

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

#if wxUSE_ACCEL
    #include "wx/accel.h"
#endif // wxUSE_ACCEL

#include "wx/os2/private.h"

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

// ----------------------------------------------------------------------------
// static function for translating menu labels
// ----------------------------------------------------------------------------

static wxString TextToLabel(const wxString& rTitle)
{
    wxString Title;
    const wxChar *pc;
    for (pc = rTitle.c_str(); *pc != wxT('\0'); pc++ )
    {
        if (*pc == wxT('&') )
        {
            if (*(pc+1) == wxT('&'))
            {
                pc++;
                Title << wxT('&');
            }
            else
                Title << wxT('~');
        }
        else
        {
            if ( *pc == wxT('~') )
            {
                // tildes must be doubled to prevent them from being
                // interpreted as accelerator character prefix by PM ???
                Title << *pc;
            }
            Title << *pc;
        }
    }
    return Title;
}

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// dynamic classes implementation
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxMenuItem, wxObject)

// ----------------------------------------------------------------------------
// wxMenuItem
// ----------------------------------------------------------------------------

// ctor & dtor
// -----------

wxMenuItem::wxMenuItem(
  wxMenu*                           pParentMenu
, int                               nId
, const wxString&                   rText
, const wxString&                   rStrHelp
, wxItemKind                        kind
, wxMenu*                           pSubMenu
)
: wxMenuItemBase(pParentMenu, nId, rText, rStrHelp, kind, pSubMenu)
#if wxUSE_OWNER_DRAWN
,  wxOwnerDrawn( TextToLabel(rText)
                ,bCheckable
               )
#endif // owner drawn
{
    wxASSERT_MSG(pParentMenu != NULL, wxT("a menu item should have a parent"));

#if  wxUSE_OWNER_DRAWN

    //
    // Set default menu colors
    //
    #define SYS_COLOR(c) (wxSystemSettings::GetColour(wxSYS_COLOUR_##c))

    SetTextColour(SYS_COLOR(MENUTEXT));
    SetBackgroundColour(SYS_COLOR(MENU));

    //
    // We don't want normal items be owner-drawn
    //
    ResetOwnerDrawn();

    #undef  SYS_COLOR
#endif // wxUSE_OWNER_DRAWN

    m_text        = TextToLabel(rText);

    memset(&m_vMenuData, '\0', sizeof(m_vMenuData));
    m_vMenuData.id= nId;
} // end of wxMenuItem::wxMenuItem

wxMenuItem::~wxMenuItem()
{
} // end of wxMenuItem::~wxMenuItem

//
// Misc
// ----

//
// Return the id for calling Win32 API functions
//
int wxMenuItem::GetRealId() const
{
    return m_subMenu ? (int)m_subMenu->GetHMenu() : GetId();
} // end of wxMenuItem::GetRealId

//
// Get item state
// --------------
bool wxMenuItem::IsChecked() const
{
    USHORT                          uFlag = SHORT1FROMMR(::WinSendMsg( GetHMenuOf(m_parentMenu)
                                                                      ,MM_QUERYITEMATTR
                                                                      ,MPFROM2SHORT(GetId(), TRUE)
                                                                      ,MPFROMSHORT(MIA_CHECKED)
                                                                     ));

    return (uFlag & MIA_CHECKED);
} // end of wxMenuItem::IsChecked

wxString wxMenuItemBase::GetLabelFromText(
  const wxString&                   rText
)
{
    wxString label;
    for ( const wxChar *pc = rText.c_str(); *pc; pc++ )
    {
        if ( *pc == wxT('~') || *pc == wxT('&') )
        {
            // '~' is the escape character for GTK+ and '&' is the one for
            // wxWindows - skip both of them
            continue;
        }

        label += *pc;
    }
    return label;
}

// change item state
// -----------------

void wxMenuItem::Enable(
  bool                              bEnable
)
{
    bool                            bOk;

    if (m_isEnabled == bEnable)
        return;
    if (bEnable)
        bOk = (bool)::WinSendMsg( GetHMenuOf(m_parentMenu)
                                 ,MM_SETITEMATTR
                                 ,MPFROM2SHORT(GetRealId(), TRUE)
                                 ,MPFROM2SHORT(MIA_DISABLED, FALSE)
                                );
    else
        bOk = (bool)::WinSendMsg( GetHMenuOf(m_parentMenu)
                                 ,MM_SETITEMATTR
                                 ,MPFROM2SHORT(GetRealId(), TRUE)
                                 ,MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED)
                                );
    if (!bOk)
    {
        wxLogLastError("EnableMenuItem");
    }
    wxMenuItemBase::Enable(bEnable);
} // end of wxMenuItem::Enable

void wxMenuItem::Check(
  bool                              bCheck
)
{
    bool                            bOk;

    wxCHECK_RET( IsCheckable(), wxT("only checkable items may be checked") );
    if (m_isChecked == bCheck)
        return;
    if (bCheck)
        bOk = (bool)::WinSendMsg( GetHMenuOf(m_parentMenu)
                                 ,MM_SETITEMATTR
                                 ,MPFROM2SHORT(GetRealId(), TRUE)
                                 ,MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED)
                                );
    else
        bOk = (bool)::WinSendMsg( GetHMenuOf(m_parentMenu)
                                 ,MM_SETITEMATTR
                                 ,MPFROM2SHORT(GetRealId(), TRUE)
                                 ,MPFROM2SHORT(MIA_CHECKED, FALSE)
                                );
    if (!bOk)
    {
        wxLogLastError("CheckMenuItem");
    }
    wxMenuItemBase::Check(bCheck);
} // end of wxMenuItem::Check

void wxMenuItem::SetText(
  const wxString&                   rText
)
{
    //
    // Don't do anything if label didn't change
    //

    wxString Text = TextToLabel(rText);
    if (m_text == Text)
        return;

    wxMenuItemBase::SetText(Text);
    OWNER_DRAWN_ONLY(wxOwnerDrawn::SetName(Text));

    HWND                            hMenu = GetHMenuOf(m_parentMenu);

    wxCHECK_RET(hMenu, wxT("menuitem without menu"));

#if wxUSE_ACCEL
    m_parentMenu->UpdateAccel(this);
#endif // wxUSE_ACCEL

    USHORT                          uId = GetRealId();
    MENUITEM                        vItem;
    USHORT                          uFlagsOld;

    if (!::WinSendMsg( hMenu
                      ,MM_QUERYITEM
                      ,MPFROM2SHORT(uId, TRUE)
                      ,(MPARAM)&vItem
                     ))
    {
        wxLogLastError("GetMenuState");
    }
    else
    {
        uFlagsOld = vItem.afStyle;
        if (IsSubMenu())
        {
            uFlagsOld |= MIS_SUBMENU;
        }

        BYTE*                       pData;

#if wxUSE_OWNER_DRAWN
        if (IsOwnerDrawn())
        {
            uFlagsOld |= MIS_OWNERDRAW;
            pData = (BYTE*)this;
        }
        else
#endif  //owner drawn
        {
            uFlagsOld |= MIS_TEXT;
            pData = (BYTE*)Text.c_str();
        }

        //
        // Set the style
        //
        if (!::WinSendMsg( hMenu
                          ,MM_SETITEM
                          ,MPFROM2SHORT(uId, TRUE)
                          ,(MPARAM)&vItem
                         ))
        {
            wxLogLastError(wxT("ModifyMenu"));
        }

        //
        // Set the text
        //
        if (::WinSendMsg( hMenu
                         ,MM_SETITEMTEXT
                         ,MPFROMSHORT(uId)
                         ,(MPARAM)pData
                        ))
        {
            wxLogLastError(wxT("ModifyMenu"));
        }
    }
} // end of wxMenuItem::SetText

void wxMenuItem::SetCheckable(
  bool                              bCheckable
)
{
    wxMenuItemBase::SetCheckable(bCheckable);
    OWNER_DRAWN_ONLY(wxOwnerDrawn::SetCheckable(bCheckable));
} // end of wxMenuItem::SetCheckable

// ----------------------------------------------------------------------------
// wxMenuItemBase
// ----------------------------------------------------------------------------

wxMenuItem* wxMenuItemBase::New(
  wxMenu*                           pParentMenu
, int                               nId
, const wxString&                   rName
, const wxString&                   rHelp
, wxItemKind                        kind
, wxMenu*                           pSubMenu
)
{
    return new wxMenuItem( pParentMenu
                          ,nId
                          ,rName
                          ,rHelp
                          ,kind
                          ,pSubMenu
                         );
} // end of wxMenuItemBase::New

