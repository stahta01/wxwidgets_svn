///////////////////////////////////////////////////////////////////////////////
// Name:        menuitem.h
// Purpose:     wxMenuItem class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     11.11.97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef   _MENUITEM_H
#define   _MENUITEM_H

#ifdef __GNUG__
    #pragma interface "menuitem.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if wxUSE_OWNER_DRAWN
    #include  "wx/ownerdrw.h"   // base class
#endif

// ----------------------------------------------------------------------------
// wxMenuItem: an item in the menu, optionally implements owner-drawn behaviour
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxMenuItem : public wxMenuItemBase
#if wxUSE_OWNER_DRAWN
                             , public wxOwnerDrawn
#endif
{
public:
    // ctor & dtor
    wxMenuItem(wxMenu *parentMenu = (wxMenu *)NULL,
               int id = wxID_SEPARATOR,
               const wxString& name = wxEmptyString,
               const wxString& help = wxEmptyString,
               wxItemKind kind = wxItem_Normal,
               wxMenu *subMenu = (wxMenu *)NULL);
    virtual ~wxMenuItem();

    // override base class virtuals
    virtual void SetText(const wxString& strName);
    virtual void SetCheckable(bool checkable);

    virtual void Enable(bool bDoEnable = TRUE);
    virtual void Check(bool bDoCheck = TRUE);
    virtual bool IsChecked() const;

    // unfortunately needed to resolve ambiguity between
    // wxMenuItemBase::IsCheckable() and wxOwnerDrawn::IsCheckable()
    bool IsCheckable() const { return wxMenuItemBase::IsCheckable(); }

    // the id for a popup menu is really its menu handle (as required by
    // ::AppendMenu() API), so this function will return either the id or the
    // menu handle depending on what we're
    int GetRealId() const;

    // mark item as belonging to the given radio group
    void SetRadioGroup(int start, int end)
    {
        m_startRadioGroup = start;
        m_endRadioGroup = end;
    }

private:
    // the positions of the first and last items of the radio group this item
    // belongs to or -1
    int m_startRadioGroup,
        m_endRadioGroup;

    DECLARE_DYNAMIC_CLASS(wxMenuItem)
};

#endif  //_MENUITEM_H
