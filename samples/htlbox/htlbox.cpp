/////////////////////////////////////////////////////////////////////////////
// Name:        htmllbox.cpp
// Purpose:     HtmlLbox wxWidgets sample
// Author:      Vadim Zeitlin
// Modified by:
// Created:     31.05.03
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
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

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/log.h"
    #include "wx/textdlg.h"
    #include "wx/sizer.h"

    #include "wx/menu.h"
    #include "wx/msgdlg.h"
    #include "wx/textctrl.h"

    #include "wx/dc.h"
    #include "wx/icon.h"
#endif

#include "wx/colordlg.h"
#include "wx/numdlg.h"

#include "wx/htmllbox.h"

// you can also have a file containing HTML strings for testing, enable this if
// you want to use it
//#define USE_HTML_FILE
#ifdef USE_HTML_FILE
    #include "wx/textfile.h"
#endif

#include "../sample.xpm"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// to use wxHtmlListBox you must derive a new class from it as you must
// implement pure virtual OnGetItem()
class MyHtmlListBox : public wxHtmlListBox
{
public:
    MyHtmlListBox() { }
    MyHtmlListBox(wxWindow *parent, bool multi = false);

    void SetChangeSelFg(bool change) { m_change = change; }
    void UpdateFirstItem();

protected:
    // override this method to return data to be shown in the listbox (this is
    // mandatory)
    virtual wxString OnGetItem(size_t n) const wxOVERRIDE;

    // change the appearance by overriding these functions (this is optional)
    virtual void OnDrawSeparator(wxDC& dc, wxRect& rect, size_t n) const wxOVERRIDE;
    virtual wxColour GetSelectedTextColour(const wxColour& colFg) const wxOVERRIDE;

    // flag telling us whether we should use fg colour even for the selected
    // item
    bool m_change;

    // flag which we toggle to update the first items text in OnGetItem()
    bool m_firstItemUpdated;

public:

    // flag which we toggle when the user clicks on the link in 2nd item
    // to change 2nd item's text
    bool m_linkClicked;

#ifdef USE_HTML_FILE
    wxTextFile m_file;
#endif

    wxDECLARE_NO_COPY_CLASS(MyHtmlListBox);
    DECLARE_DYNAMIC_CLASS(MyHtmlListBox)
};


class MyFrame : public wxFrame
{
public:
    MyFrame();
    virtual ~MyFrame();

    // event handlers
    void OnSimpleOrCustomBox(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSetMargins(wxCommandEvent& event);
    void OnDrawSeparator(wxCommandEvent&) { m_hlbox->RefreshAll(); }
    void OnToggleMulti(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);
    void OnUpdateItem(wxCommandEvent& event);
    void OnGetItemRect(wxCommandEvent& event);

    void OnSetBgCol(wxCommandEvent& event);
    void OnSetSelBgCol(wxCommandEvent& event);
    void OnSetSelFgCol(wxCommandEvent& event);

    void OnClear(wxCommandEvent& event);

    void OnUpdateUISelectAll(wxUpdateUIEvent& event);

    void OnLboxSelect(wxCommandEvent& event);
    void OnLboxDClick(wxCommandEvent& event)
    {
        wxLogMessage(wxT("Listbox item %d double clicked."), event.GetInt());
    }

    void OnHtmlLinkClicked(wxHtmlLinkEvent& event);
    void OnHtmlCellHover(wxHtmlCellEvent &event);
    void OnHtmlCellClicked(wxHtmlCellEvent &event);

    wxSimpleHtmlListBox *GetSimpleBox()
        { return wxDynamicCast(m_hlbox, wxSimpleHtmlListBox); }
    MyHtmlListBox *GetMyBox()
        { return wxDynamicCast(m_hlbox, MyHtmlListBox); }

    void CreateBox();

private:
    wxHtmlListBox *m_hlbox;

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE { (new MyFrame())->Show(); return true; }
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    HtmlLbox_CustomBox = 1,
    HtmlLbox_SimpleBox,
    HtmlLbox_Quit,

    HtmlLbox_SetMargins,
    HtmlLbox_DrawSeparator,
    HtmlLbox_ToggleMulti,
    HtmlLbox_SelectAll,
    HtmlLbox_UpdateItem,
    HtmlLbox_GetItemRect,

    HtmlLbox_SetBgCol,
    HtmlLbox_SetSelBgCol,
    HtmlLbox_SetSelFgCol,

    HtmlLbox_Clear,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    HtmlLbox_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(HtmlLbox_CustomBox,  MyFrame::OnSimpleOrCustomBox)
    EVT_MENU(HtmlLbox_SimpleBox,  MyFrame::OnSimpleOrCustomBox)
    EVT_MENU(HtmlLbox_Quit,  MyFrame::OnQuit)

    EVT_MENU(HtmlLbox_SetMargins, MyFrame::OnSetMargins)
    EVT_MENU(HtmlLbox_DrawSeparator, MyFrame::OnDrawSeparator)
    EVT_MENU(HtmlLbox_ToggleMulti, MyFrame::OnToggleMulti)
    EVT_MENU(HtmlLbox_SelectAll, MyFrame::OnSelectAll)
    EVT_MENU(HtmlLbox_UpdateItem, MyFrame::OnUpdateItem)
    EVT_MENU(HtmlLbox_GetItemRect, MyFrame::OnGetItemRect)

    EVT_MENU(HtmlLbox_About, MyFrame::OnAbout)

    EVT_MENU(HtmlLbox_SetBgCol, MyFrame::OnSetBgCol)
    EVT_MENU(HtmlLbox_SetSelBgCol, MyFrame::OnSetSelBgCol)
    EVT_MENU(HtmlLbox_SetSelFgCol, MyFrame::OnSetSelFgCol)

    EVT_MENU(HtmlLbox_Clear, MyFrame::OnClear)

    EVT_UPDATE_UI(HtmlLbox_SelectAll, MyFrame::OnUpdateUISelectAll)


    EVT_LISTBOX(wxID_ANY, MyFrame::OnLboxSelect)
    EVT_LISTBOX_DCLICK(wxID_ANY, MyFrame::OnLboxDClick)


    // the HTML listbox's events
    EVT_HTML_LINK_CLICKED(wxID_ANY, MyFrame::OnHtmlLinkClicked)
    EVT_HTML_CELL_HOVER(wxID_ANY, MyFrame::OnHtmlCellHover)
    EVT_HTML_CELL_CLICKED(wxID_ANY, MyFrame::OnHtmlCellClicked)

END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

// ============================================================================
// MyFrame
// ============================================================================

// ----------------------------------------------------------------------------
// MyFrame ctor/dtor
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame()
       : wxFrame(NULL, wxID_ANY, wxT("HtmlLbox wxWidgets Sample"),
                 wxDefaultPosition, wxSize(500, 500))
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->AppendRadioItem(HtmlLbox_CustomBox, wxT("Use custom box"),
                              wxT("Use a wxHtmlListBox virtual class control"));
    menuFile->AppendRadioItem(HtmlLbox_SimpleBox, wxT("Use simple box"),
                              wxT("Use a wxSimpleHtmlListBox control"));
    menuFile->AppendSeparator();
    menuFile->Append(HtmlLbox_Quit, wxT("E&xit\tAlt-X"), wxT("Quit this program"));

    // create our specific menu
    wxMenu *menuHLbox = new wxMenu;
    menuHLbox->Append(HtmlLbox_SetMargins,
                      wxT("Set &margins...\tCtrl-G"),
                      wxT("Change the margins around the items"));
    menuHLbox->AppendCheckItem(HtmlLbox_DrawSeparator,
                               wxT("&Draw separators\tCtrl-D"),
                               wxT("Toggle drawing separators between cells"));
    menuHLbox->AppendSeparator();
    menuHLbox->AppendCheckItem(HtmlLbox_ToggleMulti,
                               wxT("&Multiple selection\tCtrl-M"),
                               wxT("Toggle multiple selection on/off"));
    menuHLbox->AppendSeparator();
    menuHLbox->Append(HtmlLbox_SelectAll, wxT("Select &all items\tCtrl-A"));
    menuHLbox->Append(HtmlLbox_UpdateItem, wxT("Update &first item\tCtrl-U"));
    menuHLbox->Append(HtmlLbox_GetItemRect, wxT("Show &rectangle of item #10\tCtrl-R"));
    menuHLbox->AppendSeparator();
    menuHLbox->Append(HtmlLbox_SetBgCol, wxT("Set &background...\tCtrl-B"));
    menuHLbox->Append(HtmlLbox_SetSelBgCol,
                      wxT("Set &selection background...\tCtrl-S"));
    menuHLbox->AppendCheckItem(HtmlLbox_SetSelFgCol,
                               wxT("Keep &foreground in selection\tCtrl-F"));

    menuHLbox->AppendSeparator();
    menuHLbox->Append(HtmlLbox_Clear, wxT("&Clear\tCtrl-L"));

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(HtmlLbox_About, wxT("&About\tF1"), wxT("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuHLbox, wxT("&Listbox"));
    menuBar->Append(helpMenu, wxT("&Help"));

    menuBar->Check(HtmlLbox_DrawSeparator, true);

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to wxWidgets!"));
#endif // wxUSE_STATUSBAR

    // create the child controls
    CreateBox();
    wxTextCtrl *text = new wxTextCtrl(this, wxID_ANY, wxT(""),
                                      wxDefaultPosition, wxDefaultSize,
                                      wxTE_MULTILINE);
    delete wxLog::SetActiveTarget(new wxLogTextCtrl(text));

    // and lay them out
    wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_hlbox, 2, wxGROW);
    sizer->Add(text, 3, wxGROW);

    SetSizer(sizer);
}

MyFrame::~MyFrame()
{
    delete wxLog::SetActiveTarget(NULL);
}

void MyFrame::CreateBox()
{
    bool multi = GetMenuBar()->IsChecked(HtmlLbox_ToggleMulti);

    if ( GetMenuBar()->IsChecked(HtmlLbox_CustomBox) )
    {
        m_hlbox = new MyHtmlListBox(this, multi);
    }
    else // simple listbox
    {
        m_hlbox = new wxSimpleHtmlListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                          0, NULL, multi ? wxLB_MULTIPLE : 0);

        // unlike wxHtmlListBox which is abstract, wxSimpleHtmlListBox is a
        // concrete control and doesn't support virtual mode, this we need
        // to add all of its items from the beginning
        wxArrayString arr;
        for (size_t n = 0; n < 1000; n++ )
        {
            wxColour clr((unsigned char)(abs((int)n - 192) % 256),
                         (unsigned char)(abs((int)n - 256) % 256),
                         (unsigned char)(abs((int)n - 128) % 256));
            int level = n % 6 + 1;

            wxString label = wxString::Format(wxT("<h%d><font color=%s>")
                    wxT("Item</font> <b>%lu</b>")
                    wxT("</h%d>"),
                    level,
                    clr.GetAsString(wxC2S_HTML_SYNTAX).c_str(),
                    (unsigned long)n, level);
            arr.Add(label);
        }

        GetSimpleBox()->Append(arr);
    }
}


// ----------------------------------------------------------------------------
// menu event handlers
// ----------------------------------------------------------------------------

void MyFrame::OnSimpleOrCustomBox(wxCommandEvent& WXUNUSED(event))
{
    wxWindow *old = m_hlbox;

    // we need to recreate the listbox
    CreateBox();
    GetSizer()->Replace(old, m_hlbox);
    delete old;

    GetSizer()->Layout();
    Refresh();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("This sample shows wxHtmlListBox class.\n")
                 wxT("\n")
                 wxT("(c) 2003 Vadim Zeitlin"),
                 wxT("About HtmlLbox"),
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MyFrame::OnSetMargins(wxCommandEvent& WXUNUSED(event))
{
    long margin = wxGetNumberFromUser
                  (
                    wxT("Enter the margins to use for the listbox items."),
                    wxT("Margin: "),
                    wxT("HtmlLbox: Set the margins"),
                    0, 0, 20,
                    this
                  );

    if ( margin != -1 )
    {
        m_hlbox->SetMargins(margin, margin);
        m_hlbox->RefreshAll();
    }
}

void MyFrame::OnToggleMulti(wxCommandEvent& WXUNUSED(event))
{
    wxWindow *old = m_hlbox;

    // we need to recreate the listbox
    CreateBox();
    GetSizer()->Replace(old, m_hlbox);
    delete old;

    GetSizer()->Layout();
}

void MyFrame::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
    m_hlbox->SelectAll();
}

void MyFrame::OnUpdateUISelectAll(wxUpdateUIEvent& event)
{
    event.Enable( m_hlbox && m_hlbox->HasMultipleSelection() );
}

void MyFrame::OnUpdateItem(wxCommandEvent& WXUNUSED(event))
{
    if (GetMyBox())
        GetMyBox()->UpdateFirstItem();
}

void MyFrame::OnGetItemRect(wxCommandEvent& WXUNUSED(event))
{
    static const int ITEM = 10;
    const wxRect r = m_hlbox->GetItemRect(ITEM);
    wxLogMessage("Rect of item %d: (%d, %d)-(%d, %d)",
                 ITEM, r.x, r.y, r.x + r.width, r.y + r.height);
}

void MyFrame::OnSetBgCol(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, m_hlbox->GetBackgroundColour());
    if ( col.IsOk() )
    {
        m_hlbox->SetBackgroundColour(col);
        m_hlbox->Refresh();

#if wxUSE_STATUSBAR
        SetStatusText(wxT("Background colour changed."));
#endif // wxUSE_STATUSBAR
    }
}

void MyFrame::OnSetSelBgCol(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, m_hlbox->GetSelectionBackground());
    if ( col.IsOk() )
    {
        m_hlbox->SetSelectionBackground(col);
        m_hlbox->Refresh();

#if wxUSE_STATUSBAR
        SetStatusText(wxT("Selection background colour changed."));
#endif // wxUSE_STATUSBAR
    }
}

void MyFrame::OnSetSelFgCol(wxCommandEvent& event)
{
    if (GetMyBox())
    {
        GetMyBox()->SetChangeSelFg(!event.IsChecked());
        GetMyBox()->Refresh();
    }
}

void MyFrame::OnClear(wxCommandEvent& WXUNUSED(event))
{
    m_hlbox->Clear();
}

void MyFrame::OnHtmlLinkClicked(wxHtmlLinkEvent &event)
{
    wxLogMessage(wxT("The url '%s' has been clicked!"), event.GetLinkInfo().GetHref().c_str());

    if (GetMyBox())
    {
        GetMyBox()->m_linkClicked = true;
        GetMyBox()->RefreshRow(1);
    }
}

void MyFrame::OnHtmlCellHover(wxHtmlCellEvent &event)
{
    wxLogMessage(wxT("Mouse moved over cell %p at %d;%d"),
                 event.GetCell(), event.GetPoint().x, event.GetPoint().y);
}

void MyFrame::OnHtmlCellClicked(wxHtmlCellEvent &event)
{
    wxLogMessage(wxT("Click over cell %p at %d;%d"),
                 event.GetCell(), event.GetPoint().x, event.GetPoint().y);

    // if we don't skip the event, OnHtmlLinkClicked won't be called!
    event.Skip();
}

// ----------------------------------------------------------------------------
// listbox event handlers
// ----------------------------------------------------------------------------

void MyFrame::OnLboxSelect(wxCommandEvent& event)
{
    wxLogMessage(wxT("Listbox selection is now %d."), event.GetInt());

    if ( m_hlbox->HasMultipleSelection() )
    {
        wxString s;

        bool first = true;
        unsigned long cookie;
        for ( int item = m_hlbox->GetFirstSelected(cookie);
              item != wxNOT_FOUND;
              item = m_hlbox->GetNextSelected(cookie) )
        {
            if ( first )
                first = false;
            else
                s << wxT(", ");

            s << item;
        }

        if ( !s.empty() )
        {
            wxLogMessage(wxT("Selected items: %s"), s.c_str());
        }
    }

#if wxUSE_STATUSBAR
    SetStatusText(wxString::Format(
                    wxT("# items selected = %lu"),
                    (unsigned long)m_hlbox->GetSelectedCount()
                  ));
#endif // wxUSE_STATUSBAR
}

// ============================================================================
// MyHtmlListBox
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(MyHtmlListBox, wxHtmlListBox)

MyHtmlListBox::MyHtmlListBox(wxWindow *parent, bool multi)
             : wxHtmlListBox(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             multi ? wxLB_MULTIPLE : 0)
{
    m_change = true;
    m_firstItemUpdated = false;
    m_linkClicked = false;


    SetMargins(5, 5);

#ifdef USE_HTML_FILE
    if ( !m_file.Open(wxT("results")) )
    {
        wxLogError(wxT("Failed to open results file"));
    }
    else
    {
        SetItemCount(m_file.GetLineCount());
    }
#else
    SetItemCount(1000);
#endif

    SetSelection(3);
}

void MyHtmlListBox::OnDrawSeparator(wxDC& dc, wxRect& rect, size_t) const
{
    if ( ((MyFrame *)GetParent())->
            GetMenuBar()->IsChecked(HtmlLbox_DrawSeparator) )
    {
        dc.SetPen(*wxBLACK_DASHED_PEN);
        dc.DrawLine(rect.x, rect.y, rect.GetRight(), rect.y);
        dc.DrawLine(rect.x, rect.GetBottom(), rect.GetRight(), rect.GetBottom());
    }
}

wxString MyHtmlListBox::OnGetItem(size_t n) const
{
    if ( !n && m_firstItemUpdated )
    {
        return wxT("<h1><b>Just updated</b></h1>");
    }

#ifdef USE_HTML_FILE
    wxString s;
    if ( m_file.IsOpened() )
        s = m_file[n];

    return s;
#else
    int level = n % 6 + 1;

    wxColour clr((unsigned char)(abs((int)n - 192) % 256),
                 (unsigned char)(abs((int)n - 256) % 256),
                 (unsigned char)(abs((int)n - 128) % 256));

    wxString label = wxString::Format(wxT("<h%d><font color=%s>")
                                      wxT("Item</font> <b>%lu</b>")
                                      wxT("</h%d>"),
                                      level,
                                      clr.GetAsString(wxC2S_HTML_SYNTAX).c_str(),
                                      (unsigned long)n, level);
    if ( n == 1 )
    {
        if ( !m_linkClicked )
            label += wxT("<a href='1'>Click here...</a>");
        else
            label += wxT("<font color='#9999ff'>Clicked here...</font>");
    }

    return label;
#endif
}

wxColour MyHtmlListBox::GetSelectedTextColour(const wxColour& colFg) const
{
    return m_change ? wxHtmlListBox::GetSelectedTextColour(colFg) : colFg;
}

void MyHtmlListBox::UpdateFirstItem()
{
    m_firstItemUpdated = !m_firstItemUpdated;

    RefreshRow(0);
}
