/////////////////////////////////////////////////////////////////////////////
// Name:        samples/menu.cpp
// Purpose:     wxMenu/wxMenuBar sample
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.11.99
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/app.h>
    #include <wx/frame.h>
    #include <wx/menu.h>
    #include <wx/msgdlg.h>
    #include <wx/log.h>
    #include <wx/textctrl.h>
#endif

#if !wxUSE_MENUS
    // nice try...
    #error "menu sample requires wxUSE_MENUS=1"
#endif // wxUSE_MENUS

#include "copy.xpm"

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new application
class MyApp: public wxApp
{
public:
    bool OnInit();
};

// Define a new frame
class MyFrame: public wxFrame
{
public:
    MyFrame();

    virtual ~MyFrame();

    void LogMenuEvent(const wxCommandEvent& event);

protected:
    void OnQuit(wxCommandEvent& event);
    void OnClearLog(wxCommandEvent& event);

    void OnAbout(wxCommandEvent& event);

    void OnDummy(wxCommandEvent& event);

    void OnAppendMenuItem(wxCommandEvent& event);
    void OnAppendSubMenu(wxCommandEvent& event);
    void OnDeleteMenuItem(wxCommandEvent& event);
    void OnInsertMenuItem(wxCommandEvent& event);
    void OnCheckMenuItem(wxCommandEvent& event);
    void OnEnableMenuItem(wxCommandEvent& event);
    void OnGetLabelMenuItem(wxCommandEvent& event);
    void OnSetLabelMenuItem(wxCommandEvent& event);
    void OnGetMenuItemInfo(wxCommandEvent& event);

    void OnAppendMenu(wxCommandEvent& event);
    void OnInsertMenu(wxCommandEvent& event);
    void OnDeleteMenu(wxCommandEvent& event);
    void OnToggleMenu(wxCommandEvent& event);
    void OnEnableMenu(wxCommandEvent& event);
    void OnGetLabelMenu(wxCommandEvent& event);
    void OnSetLabelMenu(wxCommandEvent& event);

    void OnRightUp(wxMouseEvent& event);

    void OnUpdateCheckMenuItemUI(wxUpdateUIEvent& event);

    void OnSize(wxSizeEvent& event);

private:
    wxMenu *CreateDummyMenu(wxString *title);

    wxMenuItem *GetLastMenuItem() const;

    // the menu previously detached from the menubar (may be NULL)
    wxMenu *m_menu;

    // the count of dummy menus already created
    size_t m_countDummy;

    // the control used for logging
    wxTextCtrl *m_textctrl;

    // the previous log target
    wxLog *m_logOld;

    DECLARE_EVENT_TABLE()
};

// A small helper class which intercepts all menu events and logs them
class MyEvtHandler : public wxEvtHandler
{
public:
    MyEvtHandler(MyFrame *frame) { m_frame = frame; }

    void OnMenuEvent(wxCommandEvent& event)
    {
        m_frame->LogMenuEvent(event);

        event.Skip();
    }

private:
    MyFrame *m_frame;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

enum
{
    Menu_File_Quit = 100,
    Menu_File_ClearLog,

    Menu_MenuBar_Toggle = 200,
    Menu_MenuBar_Append,
    Menu_MenuBar_Insert,
    Menu_MenuBar_Delete,
    Menu_MenuBar_Enable,
    Menu_MenuBar_GetLabel,
    Menu_MenuBar_SetLabel,

    Menu_Menu_Append = 300,
    Menu_Menu_AppendSub,
    Menu_Menu_Insert,
    Menu_Menu_Delete,
    Menu_Menu_Enable,
    Menu_Menu_Check,
    Menu_Menu_GetLabel,
    Menu_Menu_SetLabel,
    Menu_Menu_GetInfo,

    Menu_Dummy_First = 400,
    Menu_Dummy_Second,
    Menu_Dummy_Third,
    Menu_Dummy_Fourth,
    Menu_Dummy_Last,

    Menu_Help_About = 1000,

    Menu_Popup_ToBeDeleted = 2000,
    Menu_Popup_ToBeGreyed,
    Menu_Popup_ToBeChecked,
    Menu_Popup_Submenu,

    Menu_Max
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Menu_File_Quit,     MyFrame::OnQuit)
    EVT_MENU(Menu_File_ClearLog, MyFrame::OnClearLog)

    EVT_MENU(Menu_Help_About, MyFrame::OnAbout)

    EVT_MENU(Menu_MenuBar_Toggle,   MyFrame::OnToggleMenu)
    EVT_MENU(Menu_MenuBar_Append,   MyFrame::OnAppendMenu)
    EVT_MENU(Menu_MenuBar_Insert,   MyFrame::OnInsertMenu)
    EVT_MENU(Menu_MenuBar_Delete,   MyFrame::OnDeleteMenu)
    EVT_MENU(Menu_MenuBar_Enable,   MyFrame::OnEnableMenu)
    EVT_MENU(Menu_MenuBar_GetLabel, MyFrame::OnGetLabelMenu)
    EVT_MENU(Menu_MenuBar_SetLabel, MyFrame::OnSetLabelMenu)

    EVT_MENU(Menu_Menu_Append,    MyFrame::OnAppendMenuItem)
    EVT_MENU(Menu_Menu_AppendSub, MyFrame::OnAppendSubMenu)
    EVT_MENU(Menu_Menu_Insert,    MyFrame::OnInsertMenuItem)
    EVT_MENU(Menu_Menu_Delete,    MyFrame::OnDeleteMenuItem)
    EVT_MENU(Menu_Menu_Enable,    MyFrame::OnEnableMenuItem)
    EVT_MENU(Menu_Menu_Check,     MyFrame::OnCheckMenuItem)
    EVT_MENU(Menu_Menu_GetLabel,  MyFrame::OnGetLabelMenuItem)
    EVT_MENU(Menu_Menu_SetLabel,  MyFrame::OnSetLabelMenuItem)
    EVT_MENU(Menu_Menu_GetInfo,   MyFrame::OnGetMenuItemInfo)

    EVT_MENU_RANGE(Menu_Dummy_First, Menu_Dummy_Last, MyFrame::OnDummy)

    EVT_UPDATE_UI(Menu_Menu_Check, MyFrame::OnUpdateCheckMenuItemUI)

    EVT_RIGHT_UP(MyFrame::OnRightUp)

    EVT_SIZE(MyFrame::OnSize)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyEvtHandler, wxEvtHandler)
    EVT_MENU(-1, MyEvtHandler::OnMenuEvent)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

// The `main program' equivalent, creating the windows and returning the
// main frame
bool MyApp::OnInit()
{
    // Create the main frame window
    MyFrame* frame = new MyFrame;

    frame->Show(TRUE);

#if wxUSE_STATUSBAR
    frame->SetStatusText("Welcome to wxWindows menu sample");
#endif // wxUSE_STATUSBAR

    SetTopWindow(frame);

    return TRUE;
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

// Define my frame constructor
MyFrame::MyFrame()
       : wxFrame((wxFrame *)NULL, -1, "wxWindows menu sample",
                 wxDefaultPosition, wxSize(300, 200))
{
    m_textctrl = NULL;
    m_menu = NULL;
    m_countDummy = 0;
    m_logOld = NULL;

#if wxUSE_STATUSBAR
    CreateStatusBar();
#endif // wxUSE_STATUSBAR

    // create the menubar
    wxMenu *fileMenu = new wxMenu;

    wxMenuItem *item = new wxMenuItem(fileMenu, Menu_File_ClearLog,
                                      "Clear &log\tCtrl-L");
    item->SetBitmap(copy_xpm);
    fileMenu->Append(item);
    fileMenu->AppendSeparator();
    fileMenu->Append(Menu_File_Quit, "E&xit\tAlt-X", "Quit menu sample");

    wxMenu *menubarMenu = new wxMenu;
    menubarMenu->Append(Menu_MenuBar_Append, "&Append menu\tCtrl-A",
                        "Append a menu to the menubar");
    menubarMenu->Append(Menu_MenuBar_Insert, "&Insert menu\tCtrl-I",
                        "Insert a menu into the menubar");
    menubarMenu->Append(Menu_MenuBar_Delete, "&Delete menu\tCtrl-D",
                        "Delete the last menu from the menubar");
    menubarMenu->Append(Menu_MenuBar_Toggle, "&Toggle menu\tCtrl-T",
                        "Toggle the first menu in the menubar", TRUE);
    menubarMenu->AppendSeparator();
    menubarMenu->Append(Menu_MenuBar_Enable, "&Enable menu\tCtrl-E",
                        "Enable or disable the last menu", TRUE);
    menubarMenu->AppendSeparator();
    menubarMenu->Append(Menu_MenuBar_GetLabel, "&Get menu label\tCtrl-G",
                        "Get the label of the last menu");
    menubarMenu->Append(Menu_MenuBar_SetLabel, "&Set menu label\tCtrl-S",
                        "Change the label of the last menu");

    wxMenu *menuMenu = new wxMenu;
    menuMenu->Append(Menu_Menu_Append, "&Append menu item\tAlt-A",
                     "Append a menu item to the last menu");
    menuMenu->Append(Menu_Menu_AppendSub, "&Append sub menu\tAlt-S",
                     "Append a sub menu to the last menu");
    menuMenu->Append(Menu_Menu_Insert, "&Insert menu item\tAlt-I",
                     "Insert a menu item in head of the last menu");
    menuMenu->Append(Menu_Menu_Delete, "&Delete menu item\tAlt-D",
                     "Delete the last menu item from the last menu");
    menuMenu->AppendSeparator();
    menuMenu->Append(Menu_Menu_Enable, "&Enable menu item\tAlt-E",
                     "Enable or disable the last menu item", TRUE);
    menuMenu->Append(Menu_Menu_Check, "&Check menu item\tAlt-C",
                     "Check or uncheck the last menu item", TRUE);
    menuMenu->AppendSeparator();
    menuMenu->Append(Menu_Menu_GetLabel, "&Get menu item label\tAlt-G",
                     "Get the label of the last menu item");
    menuMenu->Append(Menu_Menu_SetLabel, "&Set menu item label\tAlt-S",
                     "Change the label of the last menu item");
    menuMenu->AppendSeparator();
    menuMenu->Append(Menu_Menu_GetInfo, "Get menu item in&fo\tAlt-F",
                     "Show the state of the last menu item");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Menu_Help_About, "&About\tF1", "About menu sample");

    wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(menubarMenu, "Menu&bar");
    menuBar->Append(menuMenu, "&Menu");
    menuBar->Append(helpMenu, "&Help");

    // these items should be initially checked
    menuBar->Check(Menu_MenuBar_Toggle, TRUE);
    menuBar->Check(Menu_MenuBar_Enable, TRUE);
    menuBar->Check(Menu_Menu_Enable, TRUE);
    menuBar->Check(Menu_Menu_Check, FALSE);

    // associate the menu bar with the frame
    SetMenuBar(menuBar);

    // intercept all menu events and log them in this custom event handler
    PushEventHandler(new MyEvtHandler(this));

    // create the log text window
    m_textctrl = new wxTextCtrl(this, -1, _T(""),
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE);
    m_textctrl->SetEditable(FALSE);
    m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_textctrl));

    wxLogMessage(_T("Brief explanations: the commands or the \"Menu\" menu\n")
                 _T("append/insert/delete items to/from the last menu.\n")
                 _T("The commands from \"Menubar\" menu work with the\n")
                 _T("menubar itself.\n")
                 _T("Right click the band below to test popup menus.\n"));
}

MyFrame::~MyFrame()
{
    delete m_menu;

    // delete the event handler installed in ctor
    PopEventHandler(TRUE);

    // restore old logger
    delete wxLog::SetActiveTarget(m_logOld);
}

wxMenu *MyFrame::CreateDummyMenu(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->Append(Menu_Dummy_First, "&First item\tCtrl-F1");
    menu->AppendSeparator();
    menu->Append(Menu_Dummy_Second, "&Second item\tCtrl-F2", "", TRUE);

    if ( title )
    {
        title->Printf(wxT("Dummy menu &%d"), ++m_countDummy);
    }

    return menu;
}

wxMenuItem *MyFrame::GetLastMenuItem() const
{
    wxMenuBar *menubar = GetMenuBar();
    wxMenu *menu = menubar->GetMenu(menubar->GetMenuCount() - 1);

    wxMenuItemList::Node *node = menu->GetMenuItems().GetLast();
    if ( !node )
    {
        wxLogWarning(wxT("No last item in the last menu!"));

        return NULL;
    }
    else
    {
        return node->GetData();
    }
}

void MyFrame::LogMenuEvent(const wxCommandEvent& event)
{
    int id = event.GetId();
    if ( !GetMenuBar()->FindItem(id) )
        return;

    wxString msg = wxString::Format(wxT("Menu command %d"), id);
    if ( GetMenuBar()->FindItem(id)->IsCheckable() )
    {
        msg += wxString::Format(wxT(" (the item is currently %schecked)"),
                                event.IsChecked() ? "" : "not ");
    }

    wxLogMessage(msg);
}

// ----------------------------------------------------------------------------
// menu callbacks
// ----------------------------------------------------------------------------

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
    m_textctrl->Clear();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    (void)wxMessageBox("wxWindows menu sample\n� 1999-2001 Vadim Zeitlin",
                       "About wxWindows menu sample",
                       wxICON_INFORMATION);
}

void MyFrame::OnDeleteMenu(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *mbar = GetMenuBar();

    size_t count = mbar->GetMenuCount();
    if ( count == 2 )
    {
        // don't let delete the first 2 menus
        wxLogError(wxT("Can't delete any more menus"));
    }
    else
    {
        delete mbar->Remove(count - 1);
    }
}

void MyFrame::OnInsertMenu(wxCommandEvent& WXUNUSED(event))
{
    wxString title;
    wxMenu *menu = CreateDummyMenu(&title);
    GetMenuBar()->Insert(0, menu, title);
}

void MyFrame::OnAppendMenu(wxCommandEvent& WXUNUSED(event))
{
    wxString title;
    wxMenu *menu = CreateDummyMenu(&title);
    GetMenuBar()->Append(menu, title);
}

void MyFrame::OnToggleMenu(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *mbar = GetMenuBar();
    if ( !m_menu )
    {
        // hide the menu
        m_menu = mbar->Remove(0);
    }
    else
    {
        // restore it
        mbar->Insert(0, m_menu, "&File");
        m_menu = NULL;
    }
}

void MyFrame::OnEnableMenu(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();
    size_t count = mbar->GetMenuCount();

    mbar->EnableTop(count - 1, event.IsChecked());
}

void MyFrame::OnGetLabelMenu(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *mbar = GetMenuBar();
    size_t count = mbar->GetMenuCount();

    wxLogMessage(wxT("The label of the last menu item is '%s'"),
                 mbar->GetLabelTop(count - 1).c_str());
}

void MyFrame::OnSetLabelMenu(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *mbar = GetMenuBar();
    size_t count = mbar->GetMenuCount();

    mbar->SetLabelTop(count - 1, "Dummy label &0");
}

void MyFrame::OnDummy(wxCommandEvent& event)
{
    wxLogMessage(wxT("Dummy item #%d"), event.GetId() - Menu_Dummy_First + 1);
}

void MyFrame::OnAppendMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *menubar = GetMenuBar();
    wxMenu *menu = menubar->GetMenu(menubar->GetMenuCount() - 1);

    menu->AppendSeparator();
    menu->Append(Menu_Dummy_Third, "&Third dummy item\tCtrl-F3",
                 "Checkable item", TRUE);
}

void MyFrame::OnAppendSubMenu(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *menubar = GetMenuBar();

    wxMenu *menu = menubar->GetMenu(menubar->GetMenuCount() - 1);

    menu->Append(Menu_Dummy_Last, "&Dummy sub menu",
                 CreateDummyMenu(NULL), "Dummy sub menu help");
}

void MyFrame::OnDeleteMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *menubar = GetMenuBar();
    wxMenu *menu = menubar->GetMenu(menubar->GetMenuCount() - 1);

    size_t count = menu->GetMenuItemCount();
    if ( !count )
    {
        wxLogWarning(wxT("No items to delete!"));
    }
    else
    {
        menu->Destroy(menu->GetMenuItems().Item(count - 1)->GetData());
    }
}

void MyFrame::OnInsertMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuBar *menubar = GetMenuBar();
    wxMenu *menu = menubar->GetMenu(menubar->GetMenuCount() - 1);

    menu->Insert(0, wxMenuItem::New(menu, Menu_Dummy_Fourth,
                                    "Fourth dummy item\tCtrl-F4"));
    menu->Insert(1, wxMenuItem::New(menu, wxID_SEPARATOR, ""));
}

void MyFrame::OnEnableMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuItem *item = GetLastMenuItem();

    if ( item )
    {
        item->Enable(!item->IsEnabled());
    }
}

void MyFrame::OnCheckMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuItem *item = GetLastMenuItem();

    item->Toggle();
}

void MyFrame::OnUpdateCheckMenuItemUI(wxUpdateUIEvent& event)
{
    wxLogNull nolog;

    wxMenuItem *item = GetLastMenuItem();

    event.Enable(item && item->IsCheckable());
}

void MyFrame::OnGetLabelMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuItem *item = GetLastMenuItem();

    if ( item )
    {
        wxLogMessage(wxT("The label of the last menu item is '%s'"),
                     item->GetLabel().c_str());
    }
}

void MyFrame::OnSetLabelMenuItem(wxCommandEvent& WXUNUSED(event))
{
    wxMenuItem *item = GetLastMenuItem();

    if ( item )
    {
        item->SetText("Dummy menu item text");
    }
}

void MyFrame::OnGetMenuItemInfo(wxCommandEvent& WXUNUSED(event))
{
    wxMenuItem *item = GetLastMenuItem();

    if ( item )
    {
        wxString msg;
        msg << "The item is " << (item->IsEnabled() ? "enabled"
                                                    : "disabled")
            << '\n';

        if ( item->IsCheckable() )
        {
            msg << "It is checkable and " << (item->IsChecked() ? "" : "un")
                << "checked\n";
        }

#if wxUSE_ACCEL
        wxAcceleratorEntry *accel = item->GetAccel();
        if ( accel )
        {
            msg << "Its accelerator is ";

            int flags = accel->GetFlags();
            if ( flags & wxACCEL_ALT )
                msg << wxT("Alt-");
            if ( flags & wxACCEL_CTRL )
                msg << wxT("Ctrl-");
            if ( flags & wxACCEL_SHIFT )
                msg << wxT("Shift-");

            int code = accel->GetKeyCode();
            switch ( code )
            {
                case WXK_F1:
                case WXK_F2:
                case WXK_F3:
                case WXK_F4:
                case WXK_F5:
                case WXK_F6:
                case WXK_F7:
                case WXK_F8:
                case WXK_F9:
                case WXK_F10:
                case WXK_F11:
                case WXK_F12:
                    msg << wxT('F') << code - WXK_F1 + 1;
                    break;

                // if there are any other keys wxGetAccelFromString() may return,
                // we should process them here

                default:
                    if ( wxIsalnum(code) )
                    {
                        msg << (wxChar)code;

                        break;
                    }

                    wxFAIL_MSG( wxT("unknown keyboard accel") );
            }

            delete accel;
        }
        else
        {
            msg << "It doesn't have an accelerator";
        }
#endif // wxUSE_ACCEL

        wxLogMessage(msg);
    }
}

void MyFrame::OnRightUp(wxMouseEvent &event)
{
    wxMenu menu("Test popup");

    menu.Append(Menu_Help_About, "&About");
    menu.Append(Menu_Popup_Submenu, "&Submenu", CreateDummyMenu(NULL));
    menu.Append(Menu_Popup_ToBeDeleted, "To be &deleted");
    menu.Append(Menu_Popup_ToBeChecked, "To be &checked", "", TRUE);
    menu.Append(Menu_Popup_ToBeGreyed, "To be &greyed");
    menu.AppendSeparator();
    menu.Append(Menu_File_Quit, "E&xit");

    menu.Delete(Menu_Popup_ToBeDeleted);
    menu.Check(Menu_Popup_ToBeChecked, TRUE);
    menu.Enable(Menu_Popup_ToBeGreyed, FALSE);

    PopupMenu(&menu, event.GetX(), event.GetY());

    // test for destroying items in popup menus
#if 0 // doesn't work in wxGTK!
    menu.Destroy(Menu_Popup_Submenu);

    PopupMenu( &menu, event.GetX(), event.GetY() );
#endif // 0
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    if ( !m_textctrl )
        return;

    // leave a band below for popup menu testing
    wxSize size = GetClientSize();
    m_textctrl->SetSize(0, 0, size.x, (3*size.y)/4);

    // this is really ugly but we have to do it as we can't just call
    // event.Skip() because wxFrameBase would make the text control fill the
    // entire frame then
#ifdef __WXUNIVERSAL__
    PositionMenuBar();
#endif // __WXUNIVERSAL__
}

