/////////////////////////////////////////////////////////////////////////////
// Name:        univ/menu.cpp
// Purpose:     wxMenuItem, wxMenu and wxMenuBar implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     25.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "univmenuitem.h"
    #pragma implementation "univmenu.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/dynarray.h"
    #include "wx/control.h"      // for FindAccelIndex()
    #include "wx/menu.h"
    #include "wx/settings.h"
    #include "wx/accel.h"
#endif // WX_PRECOMP

#if wxUSE_MENUS

#include "wx/popupwin.h"

#include "wx/univ/renderer.h"

#ifdef __WXMSW__
    #include "wx/msw/private.h"
#endif // __WXMSW__

// ----------------------------------------------------------------------------
// wxMenuInfo contains all extra information about top level menus we need
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxMenuInfo
{
public:
    // ctor
    wxMenuInfo(wxMenuBar *menubar, const wxString& text)
    {
        SetLabel(menubar, text);
    }

    // modifiers

    void SetLabel(wxMenuBar *menubar, const wxString& text)
    {
        // remember the accel char (may be -1 if none)
        m_indexAccel = wxControl::FindAccelIndex(text, &m_label);

        wxSize size;
        wxClientDC dc(menubar);
        dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));
        dc.GetTextExtent(m_label, &size.x, &size.y);

        // adjust for the renderer we use and store the width
        m_width = menubar->GetRenderer()->GetMenuBarItemSize(size).x;
    }

    void SetEnabled(bool enabled = TRUE) { m_isEnabled = TRUE; }

    // accessors

    const wxString& GetLabel() const { return m_label; }
    bool IsEnabled() const { return m_isEnabled; }
    wxCoord GetWidth() const { return m_width; }
    int GetAccelIndex() const { return m_indexAccel; }

private:
    wxString m_label;
    wxCoord m_width;
    int m_indexAccel;
    bool m_isEnabled;
};

#include "wx/arrimpl.cpp"

WX_DEFINE_OBJARRAY(wxMenuInfoArray);

// ----------------------------------------------------------------------------
// wxPopupMenuWindow: a popup window showing a menu
// ----------------------------------------------------------------------------

class wxPopupMenuWindow : public wxPopupTransientWindow
{
public:
    wxPopupMenuWindow(wxWindow *parent, wxMenu *menu);

    // override the base class version to select the first item initially
    virtual void Popup(wxWindow *focus = NULL);

    // override the base class version to dismiss any open submenus
    virtual void Dismiss();

    // notify the menu when the window disappears from screen
    virtual void OnDismiss();

    // called when a submenu is dismissed
    void OnSubmenuDismiss() { m_hasOpenSubMenu = FALSE; }

    // get the currently selected item (may be NULL)
    wxMenuItem *GetCurrentItem() const
    {
        return m_nodeCurrent ? m_nodeCurrent->GetData() : NULL;
    }

    // find the menu item at given position
    wxMenuItemList::Node *GetMenuItemFromPoint(const wxPoint& pt) const;

    // refresh the given item
    void RefreshItem(wxMenuItem *item);

    // process the key event, return TRUE if done
    bool ProcessKeyDown(int key);

    // process mouse move event
    void ProcessMouseMove(const wxPoint& pt);

protected:
    // draw the menu inside this window
    virtual void DoDraw(wxControlRenderer *renderer);

    // event handlers
    void OnLeftUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);

    // reset the current item and node
    void ResetCurrent();

    // set the current node and item withotu refreshing anything
    void SetCurrent(wxMenuItemList::Node *node);

    // change the current item refreshing the old and new items
    void ChangeCurrent(wxMenuItemList::Node *node);

    // activate item, i.e. call either ClickItem() or OpenSubmenu() depending
    // on what it is, return TRUE if something was done (i.e. it's not a
    // separator...)
    bool ActivateItem(wxMenuItem *item);

    // send the event about the item click
    void ClickItem(wxMenuItem *item);

    // show the submenu for this item
    void OpenSubmenu(wxMenuItem *item);

    // can this tiem be opened?
    bool CanOpen(wxMenuItem *item)
    {
        return item && item->IsEnabled() && item->IsSubMenu();
    }

    // dismiss the menu and all parent menus too
    void DismissAndNotify();

    // react to dimissing this menu and also dismiss the parent if
    // dismissParent
    void HandleDismiss(bool dismissParent);

    // do we have an open submenu?
    bool HasOpenSubmenu() const { return m_hasOpenSubMenu; }

    // get previous node after the current one
    wxMenuItemList::Node *GetPrevNode() const;

    // get previous node before the given one, wrapping if it's the first one
    wxMenuItemList::Node *GetPrevNode(wxMenuItemList::Node *node) const;

    // get next node after the current one
    wxMenuItemList::Node *GetNextNode() const;

    // get next node after the given one, wrapping if it's the last one
    wxMenuItemList::Node *GetNextNode(wxMenuItemList::Node *node) const;

private:
    // the menu we show
    wxMenu *m_menu;

    // the menu node corresponding to the current item
    wxMenuItemList::Node *m_nodeCurrent;

    // do we currently have an opened submenu?
    bool m_hasOpenSubMenu;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxMenu, wxEvtHandler)
IMPLEMENT_DYNAMIC_CLASS(wxMenuBar, wxWindow)
IMPLEMENT_DYNAMIC_CLASS(wxMenuItem, wxObject)

BEGIN_EVENT_TABLE(wxPopupMenuWindow, wxPopupTransientWindow)
    EVT_KEY_DOWN(wxPopupMenuWindow::OnKeyDown)

    EVT_LEFT_UP(wxPopupMenuWindow::OnLeftUp)
    EVT_MOTION(wxPopupMenuWindow::OnMouseMove)
    EVT_LEAVE_WINDOW(wxPopupMenuWindow::OnMouseLeave)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxMenuBar, wxMenuBarBase)
    EVT_KEY_DOWN(wxMenuBar::OnKeyDown)

    EVT_LEFT_DOWN(wxMenuBar::OnLeftDown)
    EVT_MOTION(wxMenuBar::OnMouseMove)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxPopupMenuWindow
// ----------------------------------------------------------------------------

wxPopupMenuWindow::wxPopupMenuWindow(wxWindow *parent, wxMenu *menu)
{
    m_menu = menu;
    m_hasOpenSubMenu = FALSE;

    ResetCurrent();

    (void)Create(parent, wxBORDER_RAISED);

    SetCursor(wxCURSOR_ARROW);
}

// ----------------------------------------------------------------------------
// wxPopupMenuWindow current item/node handling
// ----------------------------------------------------------------------------

void wxPopupMenuWindow::ResetCurrent()
{
    SetCurrent(NULL);
}

void wxPopupMenuWindow::SetCurrent(wxMenuItemList::Node *node)
{
    m_nodeCurrent = node;
}

void wxPopupMenuWindow::ChangeCurrent(wxMenuItemList::Node *node)
{
    if ( node != m_nodeCurrent )
    {
        if ( m_nodeCurrent )
        {
            wxMenuItem *item = m_nodeCurrent->GetData();
            wxCHECK_RET( item, _T("no current item?") );

            // if it was the currently opened menu, close it
            if ( item->IsSubMenu() && item->GetSubMenu()->IsShown() )
            {
                item->GetSubMenu()->Dismiss();
                OnSubmenuDismiss();
            }

            RefreshItem(item);
        }

        m_nodeCurrent = node;

        if ( m_nodeCurrent )
            RefreshItem(m_nodeCurrent->GetData());
    }
}

wxMenuItemList::Node *wxPopupMenuWindow::GetPrevNode() const
{
    wxMenuItemList::Node *node = m_nodeCurrent;
    if ( !node )
    {
        // start from the end if no current item
        node = m_menu->GetMenuItems().GetLast();
    }

    return GetPrevNode(node);
}

wxMenuItemList::Node *
wxPopupMenuWindow::GetPrevNode(wxMenuItemList::Node *node) const
{
    if ( node )
    {
        node = node->GetPrevious();
        if ( !node )
        {
            node = m_menu->GetMenuItems().GetLast();
        }
    }
    //else: the menu is empty

    return node;
}

wxMenuItemList::Node *wxPopupMenuWindow::GetNextNode() const
{
    wxMenuItemList::Node *node = m_nodeCurrent;
    if ( !node )
    {
        // start from the beginning if no current item
        node = m_menu->GetMenuItems().GetFirst();
    }

    return GetNextNode(node);
}

wxMenuItemList::Node *
wxPopupMenuWindow::GetNextNode(wxMenuItemList::Node *node) const
{
    if ( node )
    {
        node = node->GetNext();
        if ( !node )
        {
            node = m_menu->GetMenuItems().GetFirst();
        }
    }
    //else: the menu is empty

    return node;
}

// ----------------------------------------------------------------------------
// wxPopupMenuWindow popup/dismiss
// ----------------------------------------------------------------------------

void wxPopupMenuWindow::Popup(wxWindow *focus)
{
    SetCurrent(m_menu->GetMenuItems().GetFirst());

    wxPopupTransientWindow::Popup(focus);

#ifdef __WXMSW__
    // ensure that this window is really on top of everything: without using
    // SetWindowPos() it can be covered by its parent menu which is not
    // really what we want
    wxMenu *menuParent = m_menu->GetParent();
    if ( menuParent )
    {
        wxPopupMenuWindow *win = menuParent->m_popupMenu;

        // if we're shown, the parent menu must be also shown
        wxCHECK_RET( win, _T("parent menu is not shown?") );

        if ( !::SetWindowPos(GetHwndOf(win), GetHwnd(),
                             0, 0, 0, 0,
                             SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW) )
        {
            wxLogLastError(_T("SetWindowPos(HWND_TOP)"));
        }

        Refresh();
    }
#endif // __WXMSW__
}

void wxPopupMenuWindow::Dismiss()
{
    if ( HasOpenSubmenu() )
    {
        wxMenuItem *item = GetCurrentItem();
        wxCHECK_RET( item && item->IsSubMenu(), _T("where is our open submenu?") );

        wxPopupMenuWindow *win = item->GetSubMenu()->m_popupMenu;
        wxCHECK_RET( win, _T("opened submenu is not opened?") );

        win->Dismiss();
        OnSubmenuDismiss();
    }

    wxPopupTransientWindow::Dismiss();
}

void wxPopupMenuWindow::OnDismiss()
{
    // when we are dismissed because the user clicked elsewhere or we lost
    // focus in any other way, hide the parent menu as well
    HandleDismiss(TRUE);
}

void wxPopupMenuWindow::HandleDismiss(bool dismissParent)
{
    ResetCurrent();

    m_menu->OnDismiss(dismissParent);
}

void wxPopupMenuWindow::DismissAndNotify()
{
    Dismiss();
    HandleDismiss(TRUE);
}

// ----------------------------------------------------------------------------
// wxPopupMenuWindow geometry
// ----------------------------------------------------------------------------

wxMenuItemList::Node *
wxPopupMenuWindow::GetMenuItemFromPoint(const wxPoint& pt) const
{
    // we only use the y coord normally, but still check x in case the point is
    // outside the window completely
    if ( wxWindow::HitTest(pt) == wxHT_WINDOW_INSIDE )
    {
        wxCoord y = 0;
        for ( wxMenuItemList::Node *node = m_menu->GetMenuItems().GetFirst();
              node;
              node = node->GetNext() )
        {
            wxMenuItem *item = node->GetData();
            y += item->GetHeight();
            if ( y > pt.y )
            {
                // found
                return node;
            }
        }
    }

    return NULL;
}

// ----------------------------------------------------------------------------
// wxPopupMenuWindow drawing
// ----------------------------------------------------------------------------

void wxPopupMenuWindow::RefreshItem(wxMenuItem *item)
{
    wxCHECK_RET( item, _T("can't refresh NULL item") );

    wxASSERT_MSG( IsShown(), _T("can't refresh menu which is not shown") );

    // FIXME: -1 here because of SetLogicalOrigin(1, 1) in DoDraw()
    RefreshRect(wxRect(0, item->GetPosition() - 1,
                m_menu->GetGeometryInfo().GetSize().x, item->GetHeight()));
}

void wxPopupMenuWindow::DoDraw(wxControlRenderer *renderer)
{
    // no clipping so far - do we need it? I don't think so as the menu is
    // never partially covered as it is always on top of everything

    wxDC& dc = renderer->GetDC();
    dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));

    // FIXME: this should be done in the renderer, however when it is fixed
    //        wxPopupMenuWindow::RefreshItem() should be changed too!
    dc.SetLogicalOrigin(1, 1);

    wxRenderer *rend = renderer->GetRenderer();

    wxCoord y = 0;
    const wxMenuGeometryInfo& gi = m_menu->GetGeometryInfo();
    for ( wxMenuItemList::Node *node = m_menu->GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem *item = node->GetData();

        if ( item->IsSeparator() )
        {
            rend->DrawMenuSeparator(dc, y, gi);
        }
        else // not a separator
        {
            int flags = 0;
            if ( item->IsCheckable() )
            {
                flags |= wxCONTROL_CHECKABLE;

                if ( item->IsChecked() )
                {
                    flags |= wxCONTROL_CHECKED;
                }
            }

            if ( !item->IsEnabled() )
                flags |= wxCONTROL_DISABLED;

            if ( item->IsSubMenu() )
                flags |= wxCONTROL_ISSUBMENU;

            if ( item == GetCurrentItem() )
                flags |= wxCONTROL_SELECTED;

            rend->DrawMenuItem
                  (
                     dc,
                     y,
                     gi,
                     item->GetLabel(),
                     item->GetAccelString(),
                     // strangely enough, for unchecked item we use the
                     // "checked" bitmap because this is the default one - this
                     // explains this strange boolean expression
                     item->GetBitmap(!item->IsCheckable() || item->IsChecked()),
                     flags,
                     item->GetAccelIndex()
                  );
        }

        y += item->GetHeight();
    }
}

// ----------------------------------------------------------------------------
// wxPopupMenuWindow actions
// ----------------------------------------------------------------------------

void wxPopupMenuWindow::ClickItem(wxMenuItem *item)
{
    wxCHECK_RET( item, _T("can't click NULL item") );

    wxASSERT_MSG( !item->IsSeparator() && !item->IsSubMenu(),
                  _T("can't click this item") );

    m_menu->SendEvent(item->GetId(),
                      item->IsCheckable() ? item->IsChecked() : -1);

    // close all menus
    DismissAndNotify();
}

void wxPopupMenuWindow::OpenSubmenu(wxMenuItem *item)
{
    wxCHECK_RET( item, _T("can't open NULL submenu") );

    wxMenu *submenu = item->GetSubMenu();
    wxCHECK_RET( submenu, _T("can only open submenus!") );

    // FIXME: should take into account the border width
    submenu->Popup(ClientToScreen(wxPoint(0, item->GetPosition())),
                   wxSize(m_menu->GetGeometryInfo().GetSize().x, 0));

    m_hasOpenSubMenu = TRUE;
}

bool wxPopupMenuWindow::ActivateItem(wxMenuItem *item)
{
    // don't activate disabled items
    if ( !item || !item->IsEnabled() )
    {
        return FALSE;
    }

    // normal menu items generate commands, submenus can be opened and
    // the separators don't do anything
    if ( item->IsSubMenu() )
    {
        OpenSubmenu(item);
    }
    else if ( !item->IsSeparator() )
    {
        ClickItem(item);
    }
    else // separator, can't activate
    {
        return FALSE;
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// wxPopupMenuWindow input handling
// ----------------------------------------------------------------------------

void wxPopupMenuWindow::OnLeftUp(wxMouseEvent& event)
{
    wxMenuItemList::Node *node = GetMenuItemFromPoint(event.GetPosition());
    if ( node )
    {
        ActivateItem(node->GetData());
    }
}

void wxPopupMenuWindow::OnMouseMove(wxMouseEvent& event)
{
    ProcessMouseMove(event.GetPosition());

    event.Skip();
}

void wxPopupMenuWindow::ProcessMouseMove(const wxPoint& pt)
{
    wxMenuItemList::Node *node = GetMenuItemFromPoint(pt);

    // don't reset current to NULL here, we only do it when the mouse leaves
    // the window (see below)
    if ( node )
    {
        if ( node != m_nodeCurrent )
        {
            ChangeCurrent(node);

            wxMenuItem *item = GetCurrentItem();
            if ( CanOpen(item) )
            {
                OpenSubmenu(item);
            }
        }
        //else: same item, nothing to do
    }
    else // not on an item
    {
        // the last open submenu forwards the mouse move messages to its
        // parent, so if the mouse moves to another item of the parent menu,
        // this menu is closed and this other item is selected - in the similar
        // manner, the top menu forwards the mouse moves to the menubar which
        // allows to select another top level menu by just moving the mouse

        // we need to translate our client coords to the client coords of the
        // window we forward this event to
        wxPoint ptScreen = ClientToScreen(pt);

        // if the mouse is outside this menu, let the parent one to
        // process it
        wxMenu *menuParent = m_menu->GetParent();
        if ( menuParent )
        {
            wxPopupMenuWindow *win = menuParent->m_popupMenu;

            // if we're shown, the parent menu must be also shown
            wxCHECK_RET( win, _T("parent menu is not shown?") );

            win->ProcessMouseMove(win->ScreenToClient(ptScreen));
        }
        else // no parent menu
        {
            wxMenuBar *menubar = m_menu->GetMenuBar();
            if ( menubar )
            {
                if ( menubar->ProcessMouseEvent(
                            menubar->ScreenToClient(ptScreen)) )
                {
                    // menubar has closed this menu and opened another one, probably
                    return;
                }
            }
        }
        //else: top level popup menu, no other processing to do
    }
}

void wxPopupMenuWindow::OnMouseLeave(wxMouseEvent& event)
{
    // due to the artefact of mouse events generation under MSW, we actually
    // may get the mouse leave event after the menu had been already dismissed
    // and calling ChangeCurrent() would then assert, so don't do it
    if ( IsShown() )
    {
        // we shouldn't change the current them if our submenu is opened and
        // mouse moved there, in this case the submenu is responsable for
        // handling it
        bool resetCurrent;
        if ( HasOpenSubmenu() )
        {
            wxMenuItem *item = GetCurrentItem();
            wxCHECK_RET( CanOpen(item), _T("where is our open submenu?") );

            wxPopupMenuWindow *win = item->GetSubMenu()->m_popupMenu;
            wxCHECK_RET( win, _T("submenu is opened but not shown?") );

            // only handle this event if the mouse is not inside the submenu
            wxPoint pt = ClientToScreen(event.GetPosition());
            resetCurrent =
                win->HitTest(win->ScreenToClient(pt)) == wxHT_WINDOW_OUTSIDE;
        }
        else
        {
            // this menu is the last opened
            resetCurrent = TRUE;
        }

        if ( resetCurrent )
        {
            ChangeCurrent(NULL);
        }
    }

    event.Skip();
}

void wxPopupMenuWindow::OnKeyDown(wxKeyEvent& event)
{
    if ( !ProcessKeyDown(event.GetKeyCode()) )
    {
        event.Skip();
    }
}

bool wxPopupMenuWindow::ProcessKeyDown(int key)
{
    wxMenuItem *item = GetCurrentItem();

    // first let the opened submenu to have it (no test for IsEnabled() here,
    // the keys navigate even in a disabled submenu if we had somehow managed
    // to open it inspit of this)
    if ( HasOpenSubmenu() )
    {
        wxCHECK_MSG( CanOpen(item), FALSE,
                     _T("has open submenu but another item selected?") );

        if ( item->GetSubMenu()->ProcessKeyDown(key) )
            return TRUE;
    }

    bool processed = TRUE;

    // handle the up/down arrows, home, end, esc and return here, pass the
    // left/right arrows to the menu bar except when the right arrow can be
    // used to open a submenu
    switch ( key )
    {
        case WXK_LEFT:
            // if we're not a top level menu, close us, else leave this to the
            // menubar
            if ( !m_menu->GetParent() )
            {
                processed = FALSE;
                break;
            }

            // fall through

        case WXK_ESCAPE:
            // close just this menu
            Dismiss();
            HandleDismiss(FALSE);
            break;

        case WXK_RETURN:
            processed = ActivateItem(item);
            break;

        case WXK_HOME:
            ChangeCurrent(m_menu->GetMenuItems().GetFirst());
            break;

        case WXK_END:
            ChangeCurrent(m_menu->GetMenuItems().GetLast());
            break;

        case WXK_UP:
        case WXK_DOWN:
            {
                bool up = key == WXK_UP;

                wxMenuItemList::Node *nodeStart = up ? GetPrevNode()
                                                     : GetNextNode(),
                                     *node = nodeStart;
                while ( node && node->GetData()->IsSeparator() )
                {
                    node = up ? GetPrevNode(node) : GetNextNode(node);

                    if ( node == nodeStart )
                    {
                        // nothing but separators and disabled items in this
                        // menu, break out
                        node = NULL;
                    }
                }

                if ( node )
                {
                    ChangeCurrent(node);
                }
                else
                {
                    processed = FALSE;
                }
            }
            break;

        case WXK_RIGHT:
            // don't try to reopen an already opened menu
            if ( !HasOpenSubmenu() && CanOpen(item) )
            {
                OpenSubmenu(item);
            }
            else
            {
                processed = FALSE;
            }
            break;

        default:
            // look for the menu item starting with this letter
            if ( wxIsalnum(key) )
            {
                // we want to start from the item after this one because
                // if we're already on the item with the given accel we want to
                // go to the next one, not to stay in place
                wxMenuItemList::Node *nodeStart = GetNextNode();

                // do we have more than one item with this accel?
                bool notUnique = FALSE;

                // translate everything to lower case before comparing
                wxChar chAccel = wxTolower(key);

                // loop through all items searching for the item with this
                // accel
                wxMenuItemList::Node *node = nodeStart,
                                     *nodeFound = NULL;
                for ( ;; )
                {
                    item = node->GetData();

                    int idxAccel = item->GetAccelIndex();
                    if ( idxAccel != -1 &&
                         wxTolower(item->GetLabel()[(size_t)idxAccel])
                            == chAccel )
                    {
                        // ok, found an item with this accel
                        if ( !nodeFound )
                        {
                            // store it but continue searching as we need to
                            // know if it's the only item with this accel or if
                            // there are more
                            nodeFound = node;
                        }
                        else // we already had found such item
                        {
                            notUnique = TRUE;

                            // no need to continue further, we won't find
                            // anything we don't already know
                            break;
                        }
                    }

                    // we want to iterate over all items wrapping around if
                    // necessary
                    node = GetNextNode(node);
                    if ( node == nodeStart )
                    {
                        // we've seen all nodes
                        break;
                    }
                }

                if ( nodeFound )
                {
                    item = nodeFound->GetData();

                    if ( notUnique || !item->IsEnabled() )
                    {
                        // just select it but don't activate as the user might
                        // have wanted to activate another item
                        ChangeCurrent(nodeFound);
                    }
                    else // unique item with this accel
                    {
                        // activate the item
                        processed = ActivateItem(item);
                    }

                    // skip "processed = FALSE" below
                    break;
                }
            }

            processed = FALSE;
    }

    return processed;
}

// ----------------------------------------------------------------------------
// wxMenu
// ----------------------------------------------------------------------------

void wxMenu::Init()
{
    m_geometry = NULL;

    m_popupMenu = NULL;
}

wxMenu::~wxMenu()
{
    delete m_geometry;
    delete m_popupMenu;
}

// ----------------------------------------------------------------------------
// wxMenu and wxMenuGeometryInfo
// ----------------------------------------------------------------------------

wxMenuGeometryInfo::~wxMenuGeometryInfo()
{
}

const wxMenuGeometryInfo& wxMenu::GetGeometryInfo() const
{
    if ( !m_geometry )
    {
        if ( m_popupMenu )
        {
            wxConstCast(this, wxMenu)->m_geometry =
                m_popupMenu->GetRenderer()->GetMenuGeometry(m_popupMenu, *this);
        }
        else
        {
            wxFAIL_MSG( _T("can't get geometry without window") );
        }
    }

    return *m_geometry;
}

void wxMenu::InvalidateGeometryInfo()
{
    if ( m_geometry )
    {
        delete m_geometry;
        m_geometry = NULL;
    }
}

// ----------------------------------------------------------------------------
// wxMenu adding/removing items
// ----------------------------------------------------------------------------

bool wxMenu::DoAppend(wxMenuItem *item)
{
    if ( !wxMenuBase::DoAppend(item) )
        return FALSE;

    InvalidateGeometryInfo();

#if wxUSE_ACCEL
    AddAccelFor(item);
#endif // wxUSE_ACCEL

    return TRUE;
}

bool wxMenu::DoInsert(size_t pos, wxMenuItem *item)
{
    if ( !wxMenuBase::DoInsert(pos, item) )
        return FALSE;

    InvalidateGeometryInfo();

#if wxUSE_ACCEL
    AddAccelFor(item);
#endif // wxUSE_ACCEL

    return TRUE;
}

wxMenuItem *wxMenu::DoRemove(wxMenuItem *item)
{
    wxMenuItem *itemOld = wxMenuBase::DoRemove(item);

    if ( itemOld )
    {
        InvalidateGeometryInfo();

#if wxUSE_ACCEL
        RemoveAccelFor(item);
#endif // wxUSE_ACCEL
    }

    return itemOld;
}

// ----------------------------------------------------------------------------
// wxMenu attaching/detaching
// ----------------------------------------------------------------------------

void wxMenu::Attach(wxMenuBarBase *menubar)
{
    wxMenuBase::Attach(menubar);

    wxCHECK_RET( m_menuBar, _T("menubar can't be NULL after attaching") );

    SetNextHandler(m_menuBar->GetEventHandler());
}

void wxMenu::Detach()
{
    wxMenuBase::Detach();
}

// ----------------------------------------------------------------------------
// wxMenu drawing
// ----------------------------------------------------------------------------

wxRenderer *wxMenu::GetRenderer() const
{
    // we're going to crash without renderer!
    wxCHECK_MSG( m_popupMenu, NULL, _T("neither popup nor menubar menu?") );

    return m_popupMenu->GetRenderer();
}

void wxMenu::RefreshItem(wxMenuItem *item)
{
    // the item geometry changed, so our might have changed as well
    InvalidateGeometryInfo();

    if ( IsShown() )
    {
        // this would be a bug in IsShown()
        wxCHECK_RET( m_popupMenu, _T("must have popup window if shown!") );

        // recalc geometry to update the item height and such
        (void)GetGeometryInfo();

        m_popupMenu->RefreshItem(item);
    }
}

// ----------------------------------------------------------------------------
// wxMenu showing and hiding
// ----------------------------------------------------------------------------

bool wxMenu::IsShown() const
{
    return m_popupMenu && m_popupMenu->IsShown();
}

void wxMenu::OnDismiss(bool dismissParent)
{
    if ( m_menuParent )
    {
        // always notify the parent about submenu disappearance
        wxPopupMenuWindow *win = m_menuParent->m_popupMenu;
        if ( win )
        {
            win->OnSubmenuDismiss();
        }
        else
        {
            wxFAIL_MSG( _T("parent menu not shown?") );
        }

        // and if we dismiss everything, propagate to parent
        if ( dismissParent )
        {
            // dismissParent is recursive
            m_menuParent->Dismiss();
            m_menuParent->OnDismiss(TRUE);
        }
    }
    else // no parent menu
    {
        // notify the menu bar if we're a top level menu
        if ( m_menuBar )
        {
            m_menuBar->OnDismissMenu(dismissParent);
        }
        else // popup window
        {
            wxCHECK_RET( m_invokingWindow, _T("what kind of menu is this?") );

            // TODO we need to return from DoPopupMenu()
        }
    }
}

void wxMenu::Popup(const wxPoint& pos, const wxSize& size)
{
    // create the popup window if not done yet
    if ( !m_popupMenu )
    {
#if 0
        // find the parent window
        wxWindow *parent = m_menuParent ? m_menuParent->m_popupMenu : NULL;
        if ( !parent )
        {
            parent = GetRootWindow();
        }

        wxCHECK_RET( parent, _T("no parent menu, no menu bar, no window??") );

        m_popupMenu = new wxPopupMenuWindow(parent, this);
#else
        m_popupMenu = new wxPopupMenuWindow(GetRootWindow(), this);
#endif
    }

    // the geometry might have changed since the last time we were shown, so
    // always resize
    m_popupMenu->SetClientSize(GetGeometryInfo().GetSize());

    // position it as specified
    m_popupMenu->Position(pos, size);

    // the menu can't have the focus itself (it is a Windows limitation), so
    // always keep the focus at the originating window
    wxWindow *focus = GetRootWindow();

    wxASSERT_MSG( focus, _T("no window to keep focus on?") );

    // and show it
    m_popupMenu->Popup(focus);
}

void wxMenu::Dismiss()
{
    wxCHECK_RET( IsShown(), _T("can't dismiss hidden menu") );

    m_popupMenu->Dismiss();
}

// ----------------------------------------------------------------------------
// wxMenu event processing
// ----------------------------------------------------------------------------

bool wxMenu::ProcessKeyDown(int key)
{
    wxCHECK_MSG( m_popupMenu, FALSE,
                 _T("can't process key events if not shown") );

    return m_popupMenu->ProcessKeyDown(key);
}

// ----------------------------------------------------------------------------
// wxMenu accel support
// ----------------------------------------------------------------------------

#if wxUSE_ACCEL

bool wxMenu::ProcessAccelEvent(const wxKeyEvent& event)
{
    // do we have an item for this accel?
    wxMenuItem *item = m_accelTable.GetMenuItem(event);
    if ( item )
    {
        if ( item->IsEnabled() )
        {
            SendEvent(item->GetId(),
                      item->IsCheckable() ? item->IsChecked() : -1);
        }

        return TRUE;
    }

    // try our submenus
    for ( wxMenuItemList::Node *node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        const wxMenuItem *item = node->GetData();
        if ( item->IsSubMenu() && item->IsEnabled() )
        {
            // try its elements
            if ( item->GetSubMenu()->ProcessAccelEvent(event) )
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void wxMenu::AddAccelFor(const wxMenuItem *item)
{
    wxAcceleratorEntry *accel = item->GetAccel();
    if ( accel )
    {
        accel->SetMenuItem((wxMenuItem *)item);

        m_accelTable.Add(*accel);

        delete accel;
    }
}

void wxMenu::RemoveAccelFor(const wxMenuItem *item)
{
    wxAcceleratorEntry *accel = item->GetAccel();
    if ( accel )
    {
        m_accelTable.Remove(*accel);

        delete accel;
    }
}

#endif // wxUSE_ACCEL

// ----------------------------------------------------------------------------
// wxMenuItem construction
// ----------------------------------------------------------------------------

wxMenuItem::wxMenuItem(wxMenu *parentMenu,
                       int id,
                       const wxString& text,
                       const wxString& help,
                       bool isCheckable,
                       wxMenu *subMenu)
{
    m_id = id;
    m_parentMenu = parentMenu;
    m_subMenu = subMenu;

    m_text = text;
    m_help = help;

    m_isCheckable = isCheckable;
    m_isEnabled = TRUE;
    m_isChecked = FALSE;

    m_posY =
    m_height = -1;

    UpdateAccelInfo();
}

wxMenuItem::~wxMenuItem()
{
}

// ----------------------------------------------------------------------------
// wxMenuItemBase methods implemented here
// ----------------------------------------------------------------------------

/* static */
wxMenuItem *wxMenuItemBase::New(wxMenu *parentMenu,
                                int id,
                                const wxString& name,
                                const wxString& help,
                                bool isCheckable,
                                wxMenu *subMenu)
{
    return new wxMenuItem(parentMenu, id, name, help, isCheckable, subMenu);
}

/* static */
wxString wxMenuItemBase::GetLabelFromText(const wxString& text)
{
    return wxStripMenuCodes(text);
}

// ----------------------------------------------------------------------------
// wxMenuItem operations
// ----------------------------------------------------------------------------

void wxMenuItem::NotifyMenu()
{
    m_parentMenu->RefreshItem(this);
}

void wxMenuItem::UpdateAccelInfo()
{
    m_indexAccel = wxControl::FindAccelIndex(m_text);

    // will be empty if the text contains no TABs - ok
    m_strAccel = m_text.AfterFirst(_T('\t'));
}

void wxMenuItem::SetText(const wxString& text)
{
    if ( text != m_text )
    {
        // first call the base class version to change m_text
        wxMenuItemBase::SetText(text);

        UpdateAccelInfo();

        NotifyMenu();
    }
}

void wxMenuItem::SetCheckable(bool checkable)
{
    if ( checkable != m_isCheckable )
    {
        wxMenuItemBase::SetCheckable(checkable);

        NotifyMenu();
    }
}

void wxMenuItem::SetBitmaps(const wxBitmap& bmpChecked,
                            const wxBitmap& bmpUnchecked)
{
    m_bmpChecked = bmpChecked;
    m_bmpUnchecked = bmpUnchecked;

    NotifyMenu();
}

void wxMenuItem::Enable(bool enable)
{
    if ( enable != m_isEnabled )
    {
        wxMenuItemBase::Enable(enable);

        NotifyMenu();
    }
}

void wxMenuItem::Check(bool check)
{
    if ( check != m_isChecked )
    {
        wxMenuItemBase::Check(check);

        NotifyMenu();
    }
}

// ----------------------------------------------------------------------------
// wxMenuBar creation
// ----------------------------------------------------------------------------

void wxMenuBar::Init()
{
    m_frameLast = NULL;

    m_current = -1;

    m_menuShown = NULL;

    m_shouldShowMenu = FALSE;
}

void wxMenuBar::Attach(wxFrame *frame)
{
    // maybe you really wanted to call Detach()?
    wxCHECK_RET( frame, _T("wxMenuBar::Attach(NULL) called") );

    wxMenuBarBase::Attach(frame);

    if ( IsCreated() )
    {
        // reparent if necessary
        if ( m_frameLast != frame )
        {
            Reparent(frame);
        }

        // show it back - was hidden by Detach()
        Show();
    }
    else // not created yet, do it now
    {
        // we have no way to return the error from here anyhow :-(
        (void)Create(frame, -1);

        SetCursor(wxCURSOR_ARROW);

        SetFont(wxSystemSettings::GetSystemFont(wxSYS_SYSTEM_FONT));
    }

    // remember the last frame which had us to avoid unnecessarily reparenting
    // above
    m_frameLast = frame;
}

void wxMenuBar::Detach()
{
    // don't delete the window because we may be reattached later, just hide it
    if ( m_frameLast )
    {
        Hide();
    }

    wxMenuBarBase::Detach();
}

wxMenuBar::~wxMenuBar()
{
}

// ----------------------------------------------------------------------------
// wxMenuBar adding/removing items
// ----------------------------------------------------------------------------

bool wxMenuBar::Append(wxMenu *menu, const wxString& title)
{
    return Insert(GetCount(), menu, title);
}

bool wxMenuBar::Insert(size_t pos, wxMenu *menu, const wxString& title)
{
    if ( !wxMenuBarBase::Insert(pos, menu, title) )
        return FALSE;

    wxMenuInfo *info = new wxMenuInfo(this, title);
    m_menuInfos.Insert(info, pos);

    RefreshAllItemsAfter(pos);

    return TRUE;
}

wxMenu *wxMenuBar::Replace(size_t pos, wxMenu *menu, const wxString& title)
{
    wxMenu *menuOld = wxMenuBarBase::Replace(pos, menu, title);

    if ( menuOld )
    {
        wxMenuInfo& info = m_menuInfos[pos];

        info.SetLabel(this, title);

        // even if the old menu was disabled, the new one is not any more
        info.SetEnabled();

        // even if we change only this one, the new label has different width,
        // so we need to refresh everything beyond this item as well
        RefreshAllItemsAfter(pos);
    }

    return menuOld;
}

wxMenu *wxMenuBar::Remove(size_t pos)
{
    wxMenu *menuOld = wxMenuBarBase::Remove(pos);

    if ( menuOld )
    {
        m_menuInfos.RemoveAt(pos);

        // this doesn't happen too often, so don't try to be too smart - just
        // refresh everything
        Refresh();
    }

    return menuOld;
}

// ----------------------------------------------------------------------------
// wxMenuBar top level menus access
// ----------------------------------------------------------------------------

wxCoord wxMenuBar::GetItemWidth(size_t pos) const
{
    return m_menuInfos[pos].GetWidth();
}

void wxMenuBar::EnableTop(size_t pos, bool enable)
{
    wxCHECK_RET( pos < GetCount(), _T("invalid index in EnableTop") );

    if ( enable != m_menuInfos[pos].IsEnabled() )
    {
        m_menuInfos[pos].SetEnabled(enable);

        RefreshItem(pos);
    }
    //else: nothing to do
}

bool wxMenuBar::IsEnabledTop(size_t pos) const
{
    wxCHECK_MSG( pos < GetCount(), FALSE, _T("invalid index in IsEnabledTop") );

    return m_menuInfos[pos].IsEnabled();
}

void wxMenuBar::SetLabelTop(size_t pos, const wxString& label)
{
    wxCHECK_RET( pos < GetCount(), _T("invalid index in EnableTop") );

    if ( label != m_menuInfos[pos].GetLabel() )
    {
        m_menuInfos[pos].SetLabel(this, label);

        RefreshItem(pos);
    }
    //else: nothing to do
}

wxString wxMenuBar::GetLabelTop(size_t pos) const
{
    wxCHECK_MSG( pos < GetCount(), _T(""), _T("invalid index in GetLabelTop") );

    return m_menuInfos[pos].GetLabel();
}

// ----------------------------------------------------------------------------
// wxMenuBar drawing
// ----------------------------------------------------------------------------

void wxMenuBar::RefreshAllItemsAfter(size_t pos)
{
    wxRect rect = GetItemRect(pos);
    rect.width = GetClientSize().x - rect.x;
    RefreshRect(rect);
}

void wxMenuBar::RefreshItem(size_t pos)
{
    wxCHECK_RET( pos != (size_t)-1,
                 _T("invalid item in wxMenuBar::RefreshItem") );

    RefreshRect(GetItemRect(pos));
}

void wxMenuBar::DoDraw(wxControlRenderer *renderer)
{
    wxDC& dc = renderer->GetDC();
    dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));

    // redraw only the items which must be redrawn

    // we don't have to use GetUpdateClientRect() here because our client rect
    // is the same as total one
    wxRect rectUpdate = GetUpdateRegion().GetBox();

    int flagsMenubar = GetStateFlags();

    wxRect rect;
    rect.y = 0;
    rect.height = GetClientSize().y;

    wxCoord x = 0;
    size_t count = GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        if ( x > rectUpdate.GetRight() )
        {
            // all remaining items are to the right of rectUpdate
            break;
        }

        rect.x = x;
        rect.width = GetItemWidth(n);
        x += rect.width;
        if ( x < rectUpdate.x )
        {
            // this item is still to the left of rectUpdate
            continue;
        }

        int flags = flagsMenubar;
        if ( m_current != -1 && n == (size_t)m_current )
        {
            flags |= wxCONTROL_SELECTED;
        }

        if ( !IsEnabledTop(n) )
        {
            flags |= wxCONTROL_DISABLED;
        }

        GetRenderer()->DrawMenuBarItem
                       (
                            dc,
                            rect,
                            m_menuInfos[n].GetLabel(),
                            flags,
                            m_menuInfos[n].GetAccelIndex()
                       );
    }
}

// ----------------------------------------------------------------------------
// wxMenuBar geometry
// ----------------------------------------------------------------------------

wxRect wxMenuBar::GetItemRect(size_t pos) const
{
    wxASSERT_MSG( pos < GetCount(), _T("invalid menu bar item index") );

    wxRect rect;
    rect.x =
    rect.y = 0;
    rect.height = GetClientSize().y;

    for ( size_t n = 0; n < pos; n++ )
    {
        rect.x += GetItemWidth(n);
    }

    rect.width = GetItemWidth(pos);

    return rect;
}

wxSize wxMenuBar::DoGetBestClientSize() const
{
    wxSize size;
    if ( GetMenuCount() > 0 )
    {
        wxClientDC dc(wxConstCast(this, wxMenuBar));
        dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));
        dc.GetTextExtent(GetLabelTop(0), &size.x, &size.y);

        // adjust for the renderer we use
        size = GetRenderer()->GetMenuBarItemSize(size);
    }
    else // empty menubar
    {
        size.x =
        size.y = 0;
    }

    // the width is arbitrary, of course, for horizontal menubar
    size.x = 100;

    return size;
}

int wxMenuBar::GetMenuFromPoint(const wxPoint& pos) const
{
    if ( pos.x < 0 || pos.y < 0 || pos.y > GetClientSize().y )
        return -1;

    // do find it
    wxCoord x = 0;
    size_t count = GetCount();
    for ( size_t item = 0; item < count; item++ )
    {
        x += GetItemWidth(item);

        if ( x > pos.x )
        {
            return item;
        }
    }

    // to the right of the last menu item
    return -1;
}

// ----------------------------------------------------------------------------
// wxMenuBar menu operations
// ----------------------------------------------------------------------------

void wxMenuBar::SelectMenu(size_t pos)
{
    wxCHECK_RET( pos < GetCount(), _T("invalid menu index in SelectMenu") );

    if ( m_current != -1 )
    {
        // close the previous menu
        if ( IsShowingMenu() )
        {
            // restore m_shouldShowMenu flag after DismissMenu() which resets
            // it to FALSE
            bool old = m_shouldShowMenu;

            DismissMenu();

            m_shouldShowMenu = old;
        }

        RefreshItem((size_t)m_current);
    }

    m_current = pos;

    RefreshItem(pos);
}

void wxMenuBar::PopupMenu(size_t pos)
{
    wxCHECK_RET( pos < GetCount(), _T("invalid menu index in PopupMenu") );

    SelectMenu(pos);
    PopupMenu();
}

// ----------------------------------------------------------------------------
// wxMenuBar input handing
// ----------------------------------------------------------------------------

/*
   Note that wxMenuBar doesn't use wxInputHandler but handles keyboard and
   mouse in the same way under all platforms. This is because it doesn't derive
   from wxControl (which works with input handlers) but directly from wxWindow.

   Also, menu bar input handling is rather simple, so maybe it's not really
   worth making it themeable - at least I've decided against doing it now as it
   would merging the changes back into trunk more difficult. But it still could
   be done later if really needed.
 */

void wxMenuBar::OnLeftDown(wxMouseEvent& event)
{
    if ( HasCapture() )
    {
        OnDismiss();

        event.Skip();
    }
    else // we didn't have mouse capture, capture it now
    {
        m_current = GetMenuFromPoint(event.GetPosition());
        if ( m_current == -1 )
        {
            // unfortunately, we can't prevent wxMSW from giving us the focus,
            // so we can only give it back
            GiveAwayFocus();
        }
        else // on item
        {
            CaptureMouse();

            // show it as selected
            RefreshItem((size_t)m_current);

            // show the menu
            PopupMenu();
        }
    }
}

void wxMenuBar::OnMouseMove(wxMouseEvent& event)
{
    if ( HasCapture() )
    {
        (void)ProcessMouseEvent(event.GetPosition());
    }
    else
    {
        event.Skip();
    }
}

bool wxMenuBar::ProcessMouseEvent(const wxPoint& pt)
{
    // a hack to ignore the extra mouse events MSW sends us: this is similar to
    // wxUSE_MOUSEEVENT_HACK in wxWin itself but it isn't enough for us here as
    // we get the messages from different windows (old and new popup menus for
    // example)
#ifdef __WXMSW__
    static wxPoint s_ptLast;
    if ( pt == s_ptLast )
    {
        return FALSE;
    }

    s_ptLast = pt;
#endif // __WXMSW__

    int currentNew = GetMenuFromPoint(pt);
    if ( (currentNew == -1) || (currentNew == m_current) )
    {
        return FALSE;
    }

    // select the new active item
    SelectMenu(currentNew);

    // show the menu if we know that we should, even if we hadn't been showing
    // it before (this may happen if the previous menu was disabled)
    if ( m_shouldShowMenu )
    {
        // open the new menu if the old one we closed had been opened
        PopupMenu();
    }

    return TRUE;
}

void wxMenuBar::OnKeyDown(wxKeyEvent& event)
{
    // the current item must have been set before
    wxCHECK_RET( m_current != -1, _T("where is current item?") );

    int key = event.GetKeyCode();

    // first let the menu have it
    if ( IsShowingMenu() && m_menuShown->ProcessKeyDown(key) )
    {
        return;
    }

    // cycle through the menu items when left/right arrows are pressed and open
    // the menu when up/down one is
    switch ( key )
    {
        case WXK_ESCAPE:
        case WXK_MENU:
            // remove the selection and give the focus away
            if ( m_current != -1 )
            {
                if ( IsShowingMenu() )
                {
                    DismissMenu();
                }

                OnDismiss();
            }
            break;

        case WXK_LEFT:
        case WXK_RIGHT:
            {
                size_t count = GetCount();
                if ( count == 1 )
                {
                    // the item won't change anyhow
                    break;
                }
                //else: otherwise, it will

                // remember if we were showing a menu - if we did, we should
                // show the new menu after changing the item
                bool wasMenuOpened = IsShowingMenu();
                if ( wasMenuOpened )
                {
                    DismissMenu();
                }

                // cast is safe as we tested for -1 above
                size_t currentNew = (size_t)m_current;

                if ( key == WXK_LEFT )
                {
                    if ( currentNew-- == 0 )
                        currentNew = count - 1;
                }
                else // right
                {
                    if ( ++currentNew == (int)count )
                        currentNew = 0;
                }

                SelectMenu(currentNew);

                if ( wasMenuOpened )
                {
                    PopupMenu();
                }
            }
            break;

        case WXK_DOWN:
        case WXK_UP:
        case WXK_RETURN:
            // open the menu
            PopupMenu();
            break;

        default:
            // letters open the corresponding menu
            {
                bool unique;
                int idxFound = FindNextItemForAccel(m_current, key, &unique);

                if ( idxFound != -1 )
                {
                    if ( IsShowingMenu() )
                    {
                        DismissMenu();
                    }

                    SelectMenu(m_current);

                    // if the item is not unique, just select it but don't
                    // activate as the user might have wanted to activate
                    // another item
                    //
                    // also, don't try to open a disabled menu
                    if ( unique && IsEnabledTop((size_t)idxFound) )
                    {
                        // open the menu
                        PopupMenu();
                    }

                    // skip the "event.Skip()" below
                    break;
                }
            }

            event.Skip();
    }
}

// ----------------------------------------------------------------------------
// wxMenuBar accel handling
// ----------------------------------------------------------------------------

int wxMenuBar::FindNextItemForAccel(int idxStart, int key, bool *unique) const
{
    if ( !wxIsalnum(key) )
    {
        // we only support letters/digits as accels
        return -1;
    }

    // do we have more than one item with this accel?
    if ( unique )
        *unique = TRUE;

    // translate everything to lower case before comparing
    wxChar chAccel = wxTolower(key);

    // the index of the item with this accel
    int idxFound = -1;

    // loop through all items searching for the item with this
    // accel starting at the item after the current one
    int count = GetCount();
    int n = idxStart == -1 ? 0 : idxStart + 1;

    if ( n == count )
    {
        // wrap
        n = 0;
    }

    idxStart = n;
    for ( ;; )
    {
        const wxMenuInfo& info = m_menuInfos[n];

        int idxAccel = info.GetAccelIndex();
        if ( idxAccel != -1 &&
             wxTolower(info.GetLabel()[(size_t)idxAccel])
                == chAccel )
        {
            // ok, found an item with this accel
            if ( idxFound == -1 )
            {
                // store it but continue searching as we need to
                // know if it's the only item with this accel or if
                // there are more
                idxFound = n;
            }
            else // we already had found such item
            {
                if ( unique )
                    *unique = FALSE;

                // no need to continue further, we won't find
                // anything we don't already know
                break;
            }
        }

        // we want to iterate over all items wrapping around if
        // necessary
        if ( ++n == count )
        {
            // wrap
            n = 0;
        }

        if ( n == idxStart )
        {
            // we've seen all items
            break;
        }
    }

    return idxFound;
}

#if wxUSE_ACCEL

bool wxMenuBar::ProcessAccelEvent(const wxKeyEvent& event)
{
    for ( wxMenuList::Node *node = m_menus.GetFirst();
          node;
          node = node->GetNext() )
    {
        if ( node->GetData()->ProcessAccelEvent(event) )
        {
            // menu processed it
            return TRUE;
        }
    }

    // not found
    return FALSE;
}

#endif // wxUSE_ACCEL

// ----------------------------------------------------------------------------
// wxMenuBar menus showing
// ----------------------------------------------------------------------------

void wxMenuBar::PopupMenu()
{
    wxCHECK_RET( m_current != -1, _T("no menu to popup") );

    // forgot to call DismissMenu()?
    wxASSERT_MSG( !m_menuShown, _T("shouldn't show two menu at once!") );

    // in any case, we should show it - even if we won't
    m_shouldShowMenu = TRUE;

    if ( IsEnabledTop(m_current) )
    {
        // remember the menu we show
        m_menuShown = GetMenu(m_current);

        // position it correctly: note that we must use screen coords and that we
        // pass 0 as width to position the menu exactly below the item, not to the
        // right of it
        wxRect rectItem = GetItemRect(m_current);
        m_menuShown->Popup(ClientToScreen(rectItem.GetPosition()),
                           wxSize(0, rectItem.GetHeight()));
    }
    //else: don't show disabled menu
}

void wxMenuBar::DismissMenu()
{
    wxCHECK_RET( m_menuShown, _T("can't dismiss menu if none is shown") );

    m_menuShown->Dismiss();
    OnDismissMenu();
}

void wxMenuBar::OnDismissMenu(bool dismissMenuBar)
{
    m_shouldShowMenu = FALSE;
    m_menuShown = NULL;
    if ( dismissMenuBar )
    {
        OnDismiss();
    }
}

void wxMenuBar::OnDismiss()
{
    ReleaseCapture();

    if ( m_current != -1 )
    {
        RefreshItem((size_t)m_current);

        m_current = -1;
    }

    GiveAwayFocus();
}

void wxMenuBar::GiveAwayFocus()
{
    GetFrame()->SetFocus();
}

// ----------------------------------------------------------------------------
// popup menu support
// ----------------------------------------------------------------------------

bool wxWindow::DoPopupMenu(wxMenu *menu, int x, int y)
{
    // TODO
    wxFAIL_MSG( _T("not implemented") );

    return FALSE;
}

#endif // wxUSE_MENUS

