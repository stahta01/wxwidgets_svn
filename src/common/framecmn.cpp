/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/framecmn.cpp
// Purpose:     common (for all platforms) wxFrame functions
// Author:      Julian Smart, Vadim Zeitlin
// Created:     01/02/97
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling and Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/frame.h"

#ifndef WX_PRECOMP
    #include "wx/menu.h"
    #include "wx/menuitem.h"
    #include "wx/dcclient.h"
    #include "wx/toolbar.h"
    #include "wx/statusbr.h"
#endif // WX_PRECOMP

// ----------------------------------------------------------------------------
// event table
// ----------------------------------------------------------------------------

#if wxUSE_MENUS && wxUSE_STATUSBAR

BEGIN_EVENT_TABLE(wxFrameBase, wxTopLevelWindow)
    EVT_MENU_OPEN(wxFrameBase::OnMenuOpen)
    EVT_MENU_CLOSE(wxFrameBase::OnMenuClose)

    EVT_MENU_HIGHLIGHT_ALL(wxFrameBase::OnMenuHighlight)
END_EVENT_TABLE()

#endif // wxUSE_MENUS && wxUSE_STATUSBAR

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// construction/destruction
// ----------------------------------------------------------------------------

wxFrameBase::wxFrameBase()
{
#if wxUSE_MENUS
    m_frameMenuBar = NULL;
#endif // wxUSE_MENUS

#if wxUSE_TOOLBAR
    m_frameToolBar = NULL;
#endif // wxUSE_TOOLBAR

#if wxUSE_STATUSBAR
    m_frameStatusBar = NULL;
#endif // wxUSE_STATUSBAR

    m_statusBarPane = 0;
}

wxFrameBase::~wxFrameBase()
{
    // this destructor is required for Darwin
}

wxFrame *wxFrameBase::New(wxWindow *parent,
                          wxWindowID id,
                          const wxString& title,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
    return new wxFrame(parent, id, title, pos, size, style, name);
}

void wxFrameBase::DeleteAllBars()
{
#if wxUSE_MENUS
    if ( m_frameMenuBar )
    {
        delete m_frameMenuBar;
        m_frameMenuBar = NULL;
    }
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    if ( m_frameStatusBar )
    {
        delete m_frameStatusBar;
        m_frameStatusBar = NULL;
    }
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
    if ( m_frameToolBar )
    {
        delete m_frameToolBar;
        m_frameToolBar = NULL;
    }
#endif // wxUSE_TOOLBAR
}

bool wxFrameBase::IsOneOfBars(const wxWindow *win) const
{
#if wxUSE_MENUS
    if ( win == GetMenuBar() )
        return true;
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    if ( win == GetStatusBar() )
        return true;
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
    if ( win == GetToolBar() )
        return true;
#endif // wxUSE_TOOLBAR

    wxUnusedVar(win);

    return false;
}

// ----------------------------------------------------------------------------
// wxFrame size management: we exclude the areas taken by menu/status/toolbars
// from the client area, so the client area is what's really available for the
// frame contents
// ----------------------------------------------------------------------------

// get the origin of the client area in the client coordinates
wxPoint wxFrameBase::GetClientAreaOrigin() const
{
    wxPoint pt = wxTopLevelWindow::GetClientAreaOrigin();

#if wxUSE_TOOLBAR && !defined(__WXUNIVERSAL__)
    wxToolBar *toolbar = GetToolBar();
    if ( toolbar && toolbar->IsShown() )
    {
        int w, h;
        toolbar->GetSize(&w, &h);

        if ( toolbar->GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            pt.x += w;
        }
        else
        {
            pt.y += h;
        }
    }
#endif // wxUSE_TOOLBAR

    return pt;
}

// ----------------------------------------------------------------------------
// misc
// ----------------------------------------------------------------------------

#if wxUSE_MENUS

bool wxFrameBase::ProcessCommand(int id)
{
    wxMenuBar *bar = GetMenuBar();
    if ( !bar )
        return false;

    wxMenuItem *item = bar->FindItem(id);
    if ( !item )
        return false;

    return ProcessCommand(item);
}

bool wxFrameBase::ProcessCommand(wxMenuItem *item)
{
    wxCommandEvent commandEvent(wxEVT_COMMAND_MENU_SELECTED, item->GetId());
    commandEvent.SetEventObject(this);

    if (!item->IsEnabled())
        return true;

    if ((item->GetKind() == wxITEM_RADIO) && item->IsChecked() )
        return true;

    if (item->IsCheckable())
    {
        item->Toggle();

        // use the new value
        commandEvent.SetInt(item->IsChecked());
    }

    return HandleWindowEvent(commandEvent);
}

#endif // wxUSE_MENUS

// Do the UI update processing for this window. This is
// provided for the application to call if it wants to
// force a UI update, particularly for the menus and toolbar.
void wxFrameBase::UpdateWindowUI(long flags)
{
    wxWindowBase::UpdateWindowUI(flags);

#if wxUSE_TOOLBAR
    if (GetToolBar())
        GetToolBar()->UpdateWindowUI(flags);
#endif

#if wxUSE_MENUS
    if (GetMenuBar())
    {
        // If coming from an idle event, we only want to update the menus if
        // we're in the wxUSE_IDLEMENUUPDATES configuration, otherwise they
        // will be update when the menu is opened later
#if !wxUSE_IDLEMENUUPDATES
        if ( !(flags & wxUPDATE_UI_FROMIDLE) )
#endif // wxUSE_IDLEMENUUPDATES
            DoMenuUpdates();
    }
#endif // wxUSE_MENUS
}

// ----------------------------------------------------------------------------
// event handlers for status bar updates from menus
// ----------------------------------------------------------------------------

#if wxUSE_MENUS && wxUSE_STATUSBAR

void wxFrameBase::OnMenuHighlight(wxMenuEvent& event)
{
#if wxUSE_STATUSBAR
    (void)ShowMenuHelp(event.GetMenuId());
#endif // wxUSE_STATUSBAR
}

void wxFrameBase::OnMenuOpen(wxMenuEvent& event)
{
#if wxUSE_IDLEMENUUPDATES
    wxUnusedVar(event);
#else // !wxUSE_IDLEMENUUPDATES
    // as we didn't update the menus from idle time, do it now
    DoMenuUpdates(event.GetMenu());
#endif // wxUSE_IDLEMENUUPDATES/!wxUSE_IDLEMENUUPDATES
}

void wxFrameBase::OnMenuClose(wxMenuEvent& WXUNUSED(event))
{
    DoGiveHelp(wxEmptyString, false);
}

#endif // wxUSE_MENUS && wxUSE_STATUSBAR

// Implement internal behaviour (menu updating on some platforms)
void wxFrameBase::OnInternalIdle()
{
    wxTopLevelWindow::OnInternalIdle();

#if wxUSE_MENUS && wxUSE_IDLEMENUUPDATES
    if (wxUpdateUIEvent::CanUpdate(this))
        DoMenuUpdates();
#endif
}

// ----------------------------------------------------------------------------
// status bar stuff
// ----------------------------------------------------------------------------

#if wxUSE_STATUSBAR

wxStatusBar* wxFrameBase::CreateStatusBar(int number,
                                          long style,
                                          wxWindowID id,
                                          const wxString& name)
{
    // the main status bar can only be created once (or else it should be
    // deleted before calling CreateStatusBar() again)
    wxCHECK_MSG( !m_frameStatusBar, NULL,
                 wxT("recreating status bar in wxFrame") );

    SetStatusBar(OnCreateStatusBar(number, style, id, name));

    return m_frameStatusBar;
}

wxStatusBar *wxFrameBase::OnCreateStatusBar(int number,
                                            long style,
                                            wxWindowID id,
                                            const wxString& name)
{
    wxStatusBar *statusBar = new wxStatusBar(this, id, style, name);

    statusBar->SetFieldsCount(number);

    return statusBar;
}

void wxFrameBase::SetStatusText(const wxString& text, int number)
{
    wxCHECK_RET( m_frameStatusBar != NULL, wxT("no statusbar to set text for") );

    m_frameStatusBar->SetStatusText(text, number);
}

void wxFrameBase::SetStatusWidths(int n, const int widths_field[] )
{
    wxCHECK_RET( m_frameStatusBar != NULL, wxT("no statusbar to set widths for") );

    m_frameStatusBar->SetStatusWidths(n, widths_field);

    PositionStatusBar();
}

void wxFrameBase::PushStatusText(const wxString& text, int number)
{
    wxCHECK_RET( m_frameStatusBar != NULL, wxT("no statusbar to set text for") );

    m_frameStatusBar->PushStatusText(text, number);
}

void wxFrameBase::PopStatusText(int number)
{
    wxCHECK_RET( m_frameStatusBar != NULL, wxT("no statusbar to set text for") );

    m_frameStatusBar->PopStatusText(number);
}

bool wxFrameBase::ShowMenuHelp(int menuId)
{
#if wxUSE_MENUS
    // if no help string found, we will clear the status bar text
    wxString helpString;
    if ( menuId != wxID_SEPARATOR && menuId != -3 /* wxID_TITLE */ )
    {
        const wxMenuItem * const item = FindItemInMenuBar(menuId);
        if ( item && !item->IsSeparator() )
            helpString = item->GetHelp();

        // notice that it's ok if we don't find the item because it might
        // belong to the popup menu, so don't assert here
    }

    DoGiveHelp(helpString, true);

    return !helpString.empty();
#else // !wxUSE_MENUS
    return false;
#endif // wxUSE_MENUS/!wxUSE_MENUS
}

void wxFrameBase::SetStatusBar(wxStatusBar *statBar)
{
    bool hadBar = m_frameStatusBar != NULL;
    m_frameStatusBar = statBar;

    if ( (m_frameStatusBar != NULL) != hadBar )
    {
        PositionStatusBar();

        DoLayout();
    }
}

#endif // wxUSE_STATUSBAR

#if wxUSE_MENUS || wxUSE_TOOLBAR
void wxFrameBase::DoGiveHelp(const wxString& help, bool show)
{
#if wxUSE_STATUSBAR
    if ( m_statusBarPane < 0 )
    {
        // status bar messages disabled
        return;
    }

    wxStatusBar *statbar = GetStatusBar();
    if ( !statbar )
        return;

    wxString text;
    if ( show )
    {
        // remember the old status bar text if this is the first time we're
        // called since the menu has been opened as we're going to overwrite it
        // in our DoGiveHelp() and we want to restore it when the menu is
        // closed
        //
        // note that it would be logical to do this in OnMenuOpen() but under
        // MSW we get an EVT_MENU_HIGHLIGHT before EVT_MENU_OPEN, strangely
        // enough, and so this doesn't work and instead we use the ugly trick
        // with using special m_oldStatusText value as "menu opened" (but it is
        // arguably better than adding yet another member variable to wxFrame
        // on all platforms)
        if ( m_oldStatusText.empty() )
        {
            m_oldStatusText = statbar->GetStatusText(m_statusBarPane);
            if ( m_oldStatusText.empty() )
            {
                // use special value to prevent us from doing this the next time
                m_oldStatusText += wxT('\0');
            }
        }

        text = help;
    }
    else // hide help, restore the original text
    {
        text = m_oldStatusText;
        m_oldStatusText.clear();
    }

    statbar->SetStatusText(text, m_statusBarPane);
#else
    wxUnusedVar(help);
    wxUnusedVar(show);
#endif // wxUSE_STATUSBAR
}
#endif // wxUSE_MENUS || wxUSE_TOOLBAR


// ----------------------------------------------------------------------------
// toolbar stuff
// ----------------------------------------------------------------------------

#if wxUSE_TOOLBAR

wxToolBar* wxFrameBase::CreateToolBar(long style,
                                      wxWindowID id,
                                      const wxString& name)
{
    // the main toolbar can't be recreated (unless it was explicitly deleted
    // before)
    wxCHECK_MSG( !m_frameToolBar, NULL,
                 wxT("recreating toolbar in wxFrame") );

    if ( style == -1 )
    {
        // use default style
        //
        // NB: we don't specify the default value in the method declaration
        //     because
        //      a) this allows us to have different defaults for different
        //         platforms (even if we don't have them right now)
        //      b) we don't need to include wx/toolbar.h in the header then
        style = wxBORDER_NONE | wxTB_HORIZONTAL | wxTB_FLAT;
    }

    SetToolBar(OnCreateToolBar(style, id, name));

    return m_frameToolBar;
}

wxToolBar* wxFrameBase::OnCreateToolBar(long style,
                                        wxWindowID id,
                                        const wxString& name)
{
#if defined(__WXWINCE__) && defined(__POCKETPC__)
    return new wxToolMenuBar(this, id,
                         wxDefaultPosition, wxDefaultSize,
                         style, name);
#else
    return new wxToolBar(this, id,
                         wxDefaultPosition, wxDefaultSize,
                         style, name);
#endif
}

void wxFrameBase::SetToolBar(wxToolBar *toolbar)
{
    bool hadBar = m_frameToolBar != NULL;
    m_frameToolBar = toolbar;

    if ( (m_frameToolBar != NULL) != hadBar )
    {
        PositionToolBar();

        DoLayout();
    }
}

#endif // wxUSE_TOOLBAR

// ----------------------------------------------------------------------------
// menus
// ----------------------------------------------------------------------------

#if wxUSE_MENUS

// update all menus
void wxFrameBase::DoMenuUpdates(wxMenu* menu)
{
    if (menu)
    {
        wxEvtHandler* source = GetEventHandler();
        menu->UpdateUI(source);
    }
    else
    {
        wxMenuBar* bar = GetMenuBar();
        if (bar != NULL)
            bar->UpdateMenus();
    }
}

void wxFrameBase::DetachMenuBar()
{
    if ( m_frameMenuBar )
    {
        m_frameMenuBar->Detach();
        m_frameMenuBar = NULL;
    }
}

void wxFrameBase::AttachMenuBar(wxMenuBar *menubar)
{
    if ( menubar )
    {
        menubar->Attach((wxFrame *)this);
        m_frameMenuBar = menubar;
    }
}

void wxFrameBase::SetMenuBar(wxMenuBar *menubar)
{
    if ( menubar == GetMenuBar() )
    {
        // nothing to do
        return;
    }

    DetachMenuBar();

    this->AttachMenuBar(menubar);
}

wxMenuItem *wxFrameBase::FindItemInMenuBar(int menuId) const
{
    const wxMenuBar * const menuBar = GetMenuBar();

    return menuBar ? menuBar->FindItem(menuId) : NULL;
}

#endif // wxUSE_MENUS
