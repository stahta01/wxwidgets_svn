///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/menu.h
// Purpose:     wxMenu and wxMenuBar classes for wxUniversal
// Author:      Vadim Zeitlin
// Modified by:
// Created:     05.05.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_MENU_H_
#define _WX_UNIV_MENU_H_

#ifdef __GNUG__
    #pragma interface "univmenu.h"
#endif

#if wxUSE_ACCEL
    #include "wx/accel.h"
#endif // wxUSE_ACCEL

#include "wx/dynarray.h"

// fwd declarations
class WXDLLEXPORT wxMenuInfo;
WX_DECLARE_OBJARRAY(wxMenuInfo, wxMenuInfoArray);

class wxPopupMenuWindow;

class WXDLLEXPORT wxRenderer;

// ----------------------------------------------------------------------------
// wxMenu helper classes, used in implementation only
// ----------------------------------------------------------------------------

// used by wxRenderer
class WXDLLEXPORT wxMenuGeometryInfo
{
public:
    // get the total size of the menu
    virtual wxSize GetSize() const = 0;

    virtual ~wxMenuGeometryInfo();
};

// ----------------------------------------------------------------------------
// wxMenu
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxMenu : public wxMenuBase
{
public:
    // ctors and dtor
    wxMenu(const wxString& title, long style = 0)
        : wxMenuBase(title, style) { Init(); }

    wxMenu(long style = 0) : wxMenuBase(style) { Init(); }

    virtual ~wxMenu();

    // called by wxMenuItem when an item of this menu changes
    void RefreshItem(wxMenuItem *item);

    // show this menu at the given position (in screen coords)
    void Popup(const wxPoint& pos, const wxSize& size);

    // dismiss the menu
    void Dismiss();

    // override the base class methods to connect/disconnect event handlers
    virtual void Attach(wxMenuBarBase *menubar);
    virtual void Detach();

    // implementation only from here

    // process the key event, return TRUE if done
    bool ProcessKeyDown(int key);

#if wxUSE_ACCEL
    // find the item for the given accel and generate an event if found
    bool ProcessAccelEvent(const wxKeyEvent& event);
#endif // wxUSE_ACCEL

protected:
    // implement base class virtuals
    virtual bool DoAppend(wxMenuItem *item);
    virtual bool DoInsert(size_t pos, wxMenuItem *item);
    virtual wxMenuItem *DoRemove(wxMenuItem *item);

    // called by wxPopupMenuWindow when the window is hidden
    void OnDismiss(bool dismissParent);

    // return true if the menu is currently shown on screen
    bool IsShown() const;

    // get the menu geometry info
    const wxMenuGeometryInfo& GetGeometryInfo() const;

    // forget old menu geometry info
    void InvalidateGeometryInfo();

    // return either the menubar or the invoking window, normally never NULL
    wxWindow *GetRootWindow() const
        { return m_menuBar ? (wxWindow *)m_menuBar : m_invokingWindow; }

    // get the renderer we use for drawing: either the one of the menu bar or
    // the one of the window if we're a popup menu
    wxRenderer *GetRenderer() const;

#if wxUSE_ACCEL
    // add/remove accel for the given menu item
    void AddAccelFor(const wxMenuItem *item);
    void RemoveAccelFor(const wxMenuItem *item);
#endif // wxUSE_ACCEL

private:
    // common part of all ctors
    void Init();

    // the exact menu geometry is defined by a struct derived from this one
    // which is opaque and defined by the renderer
    wxMenuGeometryInfo *m_geometry;

    // the menu shown on screen or NULL if not currently shown
    wxPopupMenuWindow *m_popupMenu;

#if wxUSE_ACCEL
    // the accel table for this menu
    wxAcceleratorTable m_accelTable;
#endif // wxUSE_ACCEL

    // it calls out OnDismiss()
    friend wxPopupMenuWindow;

    DECLARE_DYNAMIC_CLASS(wxMenu)
};

// ----------------------------------------------------------------------------
// wxMenuBar
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxMenuBar : public wxMenuBarBase
{
public:
    // ctors and dtor
    wxMenuBar(long WXUNUSED(style) = 0) { Init(); }
    virtual ~wxMenuBar();

    // implement base class virtuals
    virtual bool Append( wxMenu *menu, const wxString &title );
    virtual bool Insert(size_t pos, wxMenu *menu, const wxString& title);
    virtual wxMenu *Replace(size_t pos, wxMenu *menu, const wxString& title);
    virtual wxMenu *Remove(size_t pos);

    virtual void EnableTop(size_t pos, bool enable);
    virtual bool IsEnabledTop(size_t pos) const;

    virtual void SetLabelTop(size_t pos, const wxString& label);
    virtual wxString GetLabelTop(size_t pos) const;

    virtual void Attach(wxFrame *frame);
    virtual void Detach();

    // get the next item for the givan accel letter (used by wxFrame), return
    // -1 if none
    //
    // if unique is not NULL, filled with TRUE if there is only one item with
    // this accel, FALSE if two or more
    int FindNextItemForAccel(int idxStart,
                             int keycode,
                             bool *unique = NULL) const;

    // called by wxFrame to set focus to or open the given menu
    void SelectMenu(size_t pos);
    void PopupMenu(size_t pos);

#if wxUSE_ACCEL
    // find the item for the given accel and generate an event if found
    bool ProcessAccelEvent(const wxKeyEvent& event);
#endif // wxUSE_ACCEL

    // called by wxMenu when it is dismissed
    void OnDismissMenu(bool dismissMenuBar = FALSE);

protected:
    // common part of all ctors
    void Init();

    // event handlers
    void OnLeftDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKillFocus(wxFocusEvent& event);

    // process the mouse move event, return TRUE if we did, FALSE to continue
    // processing as usual
    //
    // the coordinates are client coordinates of menubar, convert if necessary
    bool ProcessMouseEvent(const wxPoint& pt);

    // called when the menu bar loses mouse capture - it is not hidden unlike
    // menus, but it doesn't have modal status any longer
    void OnDismiss();

    // draw the menubar
    virtual void DoDraw(wxControlRenderer *renderer);

    // menubar geometry
    virtual wxSize DoGetBestClientSize() const;

    // has the menubar been created already?
    bool IsCreated() const { return m_frameLast != NULL; }

    // "fast" version of GetMenuCount()
    size_t GetCount() const { return m_menuInfos.GetCount(); }

    // get the (total) width of the specified menu
    wxCoord GetItemWidth(size_t pos) const;

    // get the rect of the item
    wxRect GetItemRect(size_t pos) const;

    // get the menu from the given point or -1 if none
    int GetMenuFromPoint(const wxPoint& pos) const;

    // refresh the given item
    void RefreshItem(size_t pos);

    // refresh all items after this one (including it)
    void RefreshAllItemsAfter(size_t pos);

    // hide the currently shown menu and show this one
    void DoSelectMenu(size_t pos);

    // popup the currently selected menu
    void PopupMenu();

    // hide the currently selected menu
    void DismissMenu();

    // do we show a menu currently?
    bool IsShowingMenu() const { return m_menuShown != 0; }

    // we don't want to have focus except while selecting from menu
    void GiveAwayFocus();

    // the array containing extra menu info we need
    wxMenuInfoArray m_menuInfos;

    // the current item (only used when menubar has focus)
    int m_current;

private:
    // the last frame to which we were attached, NULL initially
    wxFrame *m_frameLast;

    // the currently shown menu or NULL
    wxMenu *m_menuShown;

    // should be showing the menu? this is subtly different from m_menuShown !=
    // NULL as the menu which should be shown may be disabled in which case we
    // don't show it - but will do as soon as the focus shifts to another menu
    bool m_shouldShowMenu;

    // it calls out ProcessMouseEvent()
    friend wxPopupMenuWindow;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxMenuBar)
};

#endif // _WX_UNIV_MENU_H_
