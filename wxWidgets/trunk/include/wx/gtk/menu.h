/////////////////////////////////////////////////////////////////////////////
// Name:        menu.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKMENUH__
#define __GTKMENUH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/window.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxMenuBar;
class wxMenuItem;
class wxMenu;

//-----------------------------------------------------------------------------
// const
//-----------------------------------------------------------------------------

#define   ID_SEPARATOR    (-1)

//-----------------------------------------------------------------------------
// wxMenuBar
//-----------------------------------------------------------------------------

class wxMenuBar: public wxWindow
{
DECLARE_DYNAMIC_CLASS(wxMenuBar)

public:
  wxMenuBar();
  void Append( wxMenu *menu, const wxString &title );

  int FindMenuItem( const wxString &menuString, const wxString &itemString ) const;
  wxMenuItem* FindMenuItemById( int id ) const;
  
  void Check( int id, bool check );
  bool Checked( int id ) const;
  void Enable( int id, bool enable );
  bool Enabled( int id ) const;
  inline bool IsEnabled(int Id) const { return Enabled(Id); };
  inline bool IsChecked(int Id) const { return Checked(Id); };

  int     GetMenuCount() const { return m_menus.Number(); }
  wxMenu *GetMenu(int n) const { return (wxMenu *)m_menus.Nth(n)->Data(); }

  wxList       m_menus;
  GtkWidget   *m_menubar;
};

//-----------------------------------------------------------------------------
// wxMenu
//-----------------------------------------------------------------------------

class wxMenuItem: public wxObject
{
DECLARE_DYNAMIC_CLASS(wxMenuItem)

public:
  wxMenuItem();

  // accessors
    // id
  void SetId(int id) { m_id = id; }
  int  GetId() const { return m_id; }
  bool IsSeparator() const { return m_id == ID_SEPARATOR; }

    // the item's text
  void SetText(const wxString& str);
  const wxString& GetText() const { return m_text; }

    // what kind of menu item we are
  void SetCheckable(bool checkable) { m_isCheckMenu = checkable; }
  bool IsCheckable() const { return m_isCheckMenu; }
  void SetSubMenu(wxMenu *menu) { m_subMenu = menu; }
  wxMenu *GetSubMenu() const { return m_subMenu; }
  bool IsSubMenu() const { return m_subMenu != NULL; }

    // state
  void Enable( bool enable = TRUE ); 
  bool IsEnabled() const { return m_isEnabled; }
  void Check( bool check = TRUE );
  bool IsChecked() const;

    // help string (displayed in the status bar by default)
  void SetHelp(const wxString& str) { m_helpStr = str; }
  const wxString& GetHelp() const { return m_helpStr; }

  // implementation
  void SetMenuItem(GtkWidget *menuItem) { m_menuItem = menuItem; }
  GtkWidget *GetMenuItem() const { return m_menuItem; }

private:
  int           m_id;
  wxString      m_text;
  bool          m_isCheckMenu;
  bool          m_isChecked;
  bool          m_isEnabled;
  wxMenu       *m_subMenu;
  wxString      m_helpStr;

  GtkWidget    *m_menuItem;  // GtkMenuItem
};

class wxMenu: public wxEvtHandler
{
DECLARE_DYNAMIC_CLASS(wxMenu)

public:
  // construction
  wxMenu( const wxString& title = wxEmptyString, const wxFunction func = (wxFunction) NULL );

  // operations
    // title
  void SetTitle(const wxString& label);
  const wxString GetTitle() const;
    // menu creation
  void AppendSeparator();
  void Append(int id, const wxString &item,
              const wxString &helpStr = "", bool checkable = FALSE);
  void Append(int id, const wxString &item,
              wxMenu *subMenu, const wxString &helpStr = "" );
  void Break() {};

    // find item by name/id
  int FindItem( const wxString itemString ) const;
  wxMenuItem *FindItem( int id ) const;
  wxMenuItem *FindItemForId( int id ) const { return FindItem( id ); }

    // get/set item's state
  void Enable( int id, bool enable );
  bool IsEnabled( int id ) const;
  void Check( int id, bool check );
  bool IsChecked( int id ) const;

  void SetLabel( int id, const wxString &label );
  wxString GetLabel(int id) const;

  // helpstring
  virtual void SetHelpString(int id, const wxString& helpString);
  virtual wxString GetHelpString(int id) const ;
  
  // accessors
  wxList& GetItems() { return m_items; }

  inline void Callback(const wxFunction func) { m_callback = func; }
  
  inline void SetEventHandler(wxEvtHandler *handler) { m_eventHandler = handler; }
  inline wxEvtHandler *GetEventHandler() { return m_eventHandler; }
  
public:
  int FindMenuIdByMenuItem( GtkWidget *menuItem ) const;
  void SetInvokingWindow( wxWindow *win );
  wxWindow *GetInvokingWindow();

  wxString       m_title;
  wxList         m_items;
  wxWindow      *m_invokingWindow;
  wxFunction     m_callback;
  wxEvtHandler  *m_eventHandler;

  GtkWidget     *m_menu;  // GtkMenu
};

#endif // __GTKMENUH__
