/////////////////////////////////////////////////////////////////////////////
// Name:        listctrl.cpp
// Purpose:     wxListCtrl sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifndef __WXMSW__
#include "mondrian.xpm"
#endif

#include "wx/listctrl.h"
#include "listtest.h"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(BUSY_ON, MyFrame::BusyOn)
    EVT_MENU(BUSY_OFF, MyFrame::BusyOff)
    EVT_MENU(LIST_QUIT, MyFrame::OnQuit)
    EVT_MENU(LIST_ABOUT, MyFrame::OnAbout)
    EVT_MENU(LIST_LIST_VIEW, MyFrame::OnListView)
    EVT_MENU(LIST_REPORT_VIEW, MyFrame::OnReportView)
    EVT_MENU(LIST_ICON_VIEW, MyFrame::OnIconView)
    EVT_MENU(LIST_ICON_TEXT_VIEW, MyFrame::OnIconTextView)
    EVT_MENU(LIST_SMALL_ICON_VIEW, MyFrame::OnSmallIconView)
    EVT_MENU(LIST_SMALL_ICON_TEXT_VIEW, MyFrame::OnSmallIconTextView)
    EVT_MENU(LIST_DESELECT_ALL, MyFrame::OnDeselectAll)
    EVT_MENU(LIST_SELECT_ALL, MyFrame::OnSelectAll)
    EVT_MENU(LIST_DELETE_ALL, MyFrame::OnDeleteAll)
    EVT_MENU(LIST_SORT, MyFrame::OnSort)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyListCtrl, wxListCtrl)
    EVT_LIST_BEGIN_DRAG(LIST_CTRL, MyListCtrl::OnBeginDrag)
    EVT_LIST_BEGIN_RDRAG(LIST_CTRL, MyListCtrl::OnBeginRDrag)
    EVT_LIST_BEGIN_LABEL_EDIT(LIST_CTRL, MyListCtrl::OnBeginLabelEdit)
    EVT_LIST_END_LABEL_EDIT(LIST_CTRL, MyListCtrl::OnEndLabelEdit)
    EVT_LIST_DELETE_ITEM(LIST_CTRL, MyListCtrl::OnDeleteItem)
    EVT_LIST_GET_INFO(LIST_CTRL, MyListCtrl::OnGetInfo)
    EVT_LIST_SET_INFO(LIST_CTRL, MyListCtrl::OnSetInfo)
    EVT_LIST_ITEM_SELECTED(LIST_CTRL, MyListCtrl::OnSelected)
    EVT_LIST_ITEM_DESELECTED(LIST_CTRL, MyListCtrl::OnDeselected)
    EVT_LIST_KEY_DOWN(LIST_CTRL, MyListCtrl::OnListKeyDown)
    EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, MyListCtrl::OnActivated)
    EVT_LIST_COL_CLICK(LIST_CTRL, MyListCtrl::OnColClick)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

int wxCALLBACK MyCompareFunction(long item1, long item2, long sortData)
{
    // inverse the order
    return item1 < item2;
}

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit(void)
{
  // Create the main frame window
  MyFrame *frame = new MyFrame((wxFrame *) NULL, "wxListCtrl Test", 50, 50, 450, 340);

  // This reduces flicker effects - even better would be to define OnEraseBackground
  // to do nothing. When the list control's scrollbars are show or hidden, the
  // frame is sent a background erase event.
  frame->SetBackgroundColour( *wxWHITE );

  // Give it an icon
  frame->SetIcon( wxICON(mondrian) );

  // Make an image list containing large icons
  m_imageListNormal = new wxImageList(32, 32, TRUE);
  m_imageListSmall = new wxImageList(16, 16, TRUE);

#ifdef __WXMSW__
  m_imageListNormal->Add( wxIcon("icon1", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon2", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon3", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon4", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon5", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon6", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon7", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon8", wxBITMAP_TYPE_ICO_RESOURCE) );
  m_imageListNormal->Add( wxIcon("icon9", wxBITMAP_TYPE_ICO_RESOURCE) );

  m_imageListSmall->Add( wxIcon("iconsmall", wxBITMAP_TYPE_ICO_RESOURCE) );

#else

  #include "bitmaps/toolbrai.xpm"
  m_imageListNormal->Add( wxIcon( toolbrai_xpm ) );
  #include "bitmaps/toolchar.xpm"
  m_imageListNormal->Add( wxIcon( toolchar_xpm ) );
  #include "bitmaps/tooldata.xpm"
  m_imageListNormal->Add( wxIcon( tooldata_xpm ) );
  #include "bitmaps/toolnote.xpm"
  m_imageListNormal->Add( wxIcon( toolnote_xpm ) );
  #include "bitmaps/tooltodo.xpm"
  m_imageListNormal->Add( wxIcon( tooltodo_xpm ) );
  #include "bitmaps/toolchec.xpm"
  m_imageListNormal->Add( wxIcon( toolchec_xpm ) );
  #include "bitmaps/toolgame.xpm"
  m_imageListNormal->Add( wxIcon( toolgame_xpm ) );
  #include "bitmaps/tooltime.xpm"
  m_imageListNormal->Add( wxIcon( tooltime_xpm ) );
  #include "bitmaps/toolword.xpm"
  m_imageListNormal->Add( wxIcon( toolword_xpm ) );

  #include "bitmaps/small1.xpm"
  m_imageListSmall->Add( wxIcon( small1_xpm) );

#endif

  // Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append(LIST_LIST_VIEW,         "&List view");
  file_menu->Append(LIST_REPORT_VIEW,         "&Report view");
  file_menu->Append(LIST_ICON_VIEW,         "&Icon view");
  file_menu->Append(LIST_ICON_TEXT_VIEW,     "Icon view with &text");
  file_menu->Append(LIST_SMALL_ICON_VIEW,     "&Small icon view");
  file_menu->Append(LIST_SMALL_ICON_TEXT_VIEW,     "Small icon &view with text");
  file_menu->Append(LIST_DESELECT_ALL, "&Deselect All");
  file_menu->Append(LIST_SELECT_ALL, "S&elect All");
  file_menu->AppendSeparator();
  file_menu->Append(LIST_SORT, "&Sort\tCtrl-S");
  file_menu->AppendSeparator();
  file_menu->Append(LIST_DELETE_ALL, "Delete &all items");
  file_menu->AppendSeparator();
  file_menu->Append(BUSY_ON,         "&Busy cursor on");
  file_menu->Append(BUSY_OFF,         "&Busy cursor off");
  file_menu->AppendSeparator();
  file_menu->Append(LIST_ABOUT, "&About");
  file_menu->Append(LIST_QUIT, "E&xit");
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, "&File");
  frame->SetMenuBar(menu_bar);

  // Make a panel with a message
  frame->m_listCtrl = new MyListCtrl(frame, LIST_CTRL, wxPoint(0, 0), wxSize(400, 200),
          wxLC_LIST|wxSUNKEN_BORDER|wxLC_EDIT_LABELS );
//          wxLC_LIST|wxLC_USER_TEXT|wxSUNKEN_BORDER); // wxLC_USER_TEXT requires app to supply all text on demand
  frame->m_logWindow = new wxTextCtrl(frame, -1, "", wxPoint(0, 0), wxSize(400, 200), wxTE_MULTILINE|wxSUNKEN_BORDER);

  wxLayoutConstraints *c = new wxLayoutConstraints;
  c->top.SameAs            (frame, wxTop);
  c->left.SameAs        (frame, wxLeft);
  c->right.SameAs        (frame, wxRight);
  c->height.PercentOf    (frame, wxHeight, 66);
  frame->m_listCtrl->SetConstraints(c);

  c = new wxLayoutConstraints;
  c->top.Below            (frame->m_listCtrl);
  c->left.SameAs        (frame, wxLeft);
  c->right.SameAs        (frame, wxRight);
  c->bottom.SameAs        (frame, wxBottom);
  frame->m_logWindow->SetConstraints(c);
  frame->SetAutoLayout(TRUE);

  for ( int i=0; i < 30; i++)
    {
        wxChar buf[20];
        wxSprintf(buf, _T("Item %d"), i);
        frame->m_listCtrl->InsertItem(i, buf);
    }

  frame->CreateStatusBar(3);
  frame->SetStatusText("", 0);

  // Show the frame
  frame->Show(TRUE);

  SetTopWindow(frame);

  return TRUE;
}

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, char *title, int x, int y, int w, int h):
  wxFrame(frame, -1, title, wxPoint(x, y), wxSize(w, h))
{
    m_listCtrl = (MyListCtrl *) NULL;
    m_logWindow = (wxTextCtrl *) NULL;
}

MyFrame::~MyFrame(void)
{
    delete wxGetApp().m_imageListNormal;
    delete wxGetApp().m_imageListSmall;
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(TRUE);
}

void MyFrame::BusyOn(wxCommandEvent& WXUNUSED(event))
{
   wxBeginBusyCursor();
}

void MyFrame::BusyOff(wxCommandEvent& WXUNUSED(event))
{
   wxEndBusyCursor();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageDialog dialog(this, "List test sample\nJulian Smart (c) 1997",
            "About list test", wxOK|wxCANCEL);

    dialog.ShowModal();
}

void MyFrame::OnDeselectAll(wxCommandEvent& WXUNUSED(event))
{
    int n = m_listCtrl->GetItemCount();
    for (int i = 0; i < n; i++)
        m_listCtrl->SetItemState(i,0,wxLIST_STATE_SELECTED);
}

void MyFrame::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
    int n = m_listCtrl->GetItemCount();
    for (int i = 0; i < n; i++)
        m_listCtrl->SetItemState(i,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void MyFrame::OnListView(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->DeleteAllItems();
    m_logWindow->Clear();
    m_listCtrl->SetSingleStyle(wxLC_LIST);
    m_listCtrl->SetImageList((wxImageList *) NULL, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList((wxImageList *) NULL, wxIMAGE_LIST_SMALL);

    for ( int i=0; i < 30; i++)
    {
        wxChar buf[20];
        wxSprintf(buf, _T("Item %d"), i);
        m_listCtrl->InsertItem(i, buf);
    }
}

void MyFrame::OnReportView(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->DeleteAllItems();
    m_listCtrl->DeleteAllColumns();
    m_logWindow->Clear();

    m_listCtrl->SetSingleStyle(wxLC_REPORT);
    m_listCtrl->SetImageList((wxImageList *) NULL, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList(wxGetApp().m_imageListSmall, wxIMAGE_LIST_SMALL);

    m_listCtrl->InsertColumn(0, "Column 1"); // , wxLIST_FORMAT_LEFT, 140);
    m_listCtrl->InsertColumn(1, "Column 2"); // , wxLIST_FORMAT_LEFT, 140);
    m_listCtrl->InsertColumn(2, "One More Column (2)"); // , wxLIST_FORMAT_LEFT, 140);

    for ( int i = 0; i < 300; i++ )
    {
        wxChar buf[50];
        wxSprintf(buf, _T("This is item %d"), i);
        long tmp = m_listCtrl->InsertItem(i, buf, 0);
        m_listCtrl->SetItemData(tmp, i);

        wxSprintf(buf, _T("Col 1, item %d"), i);
        tmp = m_listCtrl->SetItem(i, 1, buf);

        wxSprintf(buf, _T("Item %d in column 2"), i);
        tmp = m_listCtrl->SetItem(i, 2, buf);
    }
    
#ifndef __WXMSW__
    // we leave all mask fields to 0 and only change the colour
    wxListItem first;
    first.m_itemId = 0;
    first.m_colour = wxBLUE;
    m_listCtrl->SetItem( first );
    
    first.m_itemId = 2;
    first.m_colour = wxLIGHT_GREY;
    m_listCtrl->SetItem( first );
    first.m_itemId = 3;
    first.m_colour = wxLIGHT_GREY;
    m_listCtrl->SetItem( first );
#endif

    m_listCtrl->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    m_listCtrl->SetColumnWidth( 1, wxLIST_AUTOSIZE );
    m_listCtrl->SetColumnWidth( 2, wxLIST_AUTOSIZE );
}

void MyFrame::OnIconView(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->DeleteAllItems();
    m_logWindow->Clear();
    m_listCtrl->SetSingleStyle(wxLC_ICON);
    m_listCtrl->SetImageList(wxGetApp().m_imageListNormal, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList(wxGetApp().m_imageListSmall, wxIMAGE_LIST_SMALL);

    for ( int i=0; i < 9; i++)
    {
        m_listCtrl->InsertItem(i, i);
    }
}

void MyFrame::OnIconTextView(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->DeleteAllItems();
    m_logWindow->Clear();
    m_listCtrl->SetSingleStyle(wxLC_ICON);
    m_listCtrl->SetImageList(wxGetApp().m_imageListNormal, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList(wxGetApp().m_imageListSmall, wxIMAGE_LIST_SMALL);

    for ( int i=0; i < 9; i++)
    {
        wxChar buf[20];
        wxSprintf(buf, _T("Label %d"), i);
        m_listCtrl->InsertItem(i, buf, i);
    }
}

void MyFrame::OnSmallIconView(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->DeleteAllItems();
    m_logWindow->Clear();
    m_listCtrl->SetSingleStyle(wxLC_SMALL_ICON);
    m_listCtrl->SetImageList(wxGetApp().m_imageListNormal, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList(wxGetApp().m_imageListSmall, wxIMAGE_LIST_SMALL);

    for ( int i=0; i < 9; i++)
    {
        m_listCtrl->InsertItem(i, 0);
    }
}

void MyFrame::OnSmallIconTextView(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->DeleteAllItems();
    m_logWindow->Clear();
    m_listCtrl->SetSingleStyle(wxLC_SMALL_ICON);
    m_listCtrl->SetImageList(wxGetApp().m_imageListNormal, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList(wxGetApp().m_imageListSmall, wxIMAGE_LIST_SMALL);

    for ( int i=0; i < 9; i++)
    {
        m_listCtrl->InsertItem(i, "Label", 0);
    }
}

void MyFrame::OnSort(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->SortItems(MyCompareFunction, 0);
}

void MyFrame::OnDeleteAll(wxCommandEvent& WXUNUSED(event))
{
    (void)wxGetElapsedTime(TRUE);

    int nItems = m_listCtrl->GetItemCount();
    m_listCtrl->DeleteAllItems();

    wxLogMessage("Deleting %d items took %ld ms",
                 nItems, wxGetElapsedTime());
}

// MyListCtrl

void MyListCtrl::OnColClick(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    wxString msg;
    msg.Printf( "OnColumnClick at %d.\n", event.GetColumn() );
    text->WriteText(msg);
}

void MyListCtrl::OnBeginDrag(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    wxString msg;
    msg.Printf( "OnBeginDrag at %d,%d.\n", event.m_pointDrag.x, event.m_pointDrag.y );
    text->WriteText(msg);
}

void MyListCtrl::OnBeginRDrag(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    wxString msg;
    msg.Printf( "OnBeginRDrag at %d,%d.\n", event.m_pointDrag.x, event.m_pointDrag.y );
    text->WriteText(msg);
}

void MyListCtrl::OnBeginLabelEdit(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnBeginLabelEdit: ");
    text->WriteText(event.m_item.m_text);
    text->WriteText("\n");
}

void MyListCtrl::OnEndLabelEdit(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnEndLabelEdit: ");
    text->WriteText(event.m_item.m_text);
    text->WriteText("\n");
}

void MyListCtrl::OnDeleteItem(wxListEvent& WXUNUSED(event))
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

        text->WriteText("OnDeleteItem\n");
}

void MyListCtrl::OnGetInfo(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnGetInfo\n");

    (*text) << "OnGetInfo (" << event.m_item.m_itemId << ", " << event.m_item.m_col << ")";
    if ( event.m_item.m_mask & wxLIST_MASK_STATE )
        (*text) << " wxLIST_MASK_STATE";
    if ( event.m_item.m_mask & wxLIST_MASK_TEXT )
        (*text) << " wxLIST_MASK_TEXT";
    if ( event.m_item.m_mask & wxLIST_MASK_IMAGE )
        (*text) << " wxLIST_MASK_IMAGE";
    if ( event.m_item.m_mask & wxLIST_MASK_DATA )
        (*text) << " wxLIST_MASK_DATA";
    if ( event.m_item.m_mask & wxLIST_SET_ITEM )
        (*text) << " wxLIST_SET_ITEM";
    if ( event.m_item.m_mask & wxLIST_MASK_WIDTH )
        (*text) << " wxLIST_MASK_WIDTH";
    if ( event.m_item.m_mask & wxLIST_MASK_FORMAT )
        (*text) << " wxLIST_MASK_WIDTH";

    if ( event.m_item.m_mask & wxLIST_MASK_TEXT )
    {
        event.m_item.m_text = "My callback text";
    }
    (*text) << "\n";
}

void MyListCtrl::OnSetInfo(wxListEvent& WXUNUSED(event))
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnSetInfo\n");
}

void MyListCtrl::OnSelected(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    if ( GetWindowStyle() & wxLC_REPORT )
    {
        wxListItem info;
        info.m_itemId = event.m_itemIndex;
        info.m_col = 1;
        info.m_mask = wxLIST_MASK_TEXT;
        if ( GetItem(info) )
        {
            *text << "Value of the 2nd field of the selected item: "
                  << info.m_text << '\n';
        }
        else
        {
            wxFAIL_MSG("wxListCtrl::GetItem() failed");
        }
    }

    text->WriteText("OnSelected\n");
}

void MyListCtrl::OnDeselected(wxListEvent& WXUNUSED(event))
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnDeselected\n");
}

void MyListCtrl::OnActivated(wxListEvent& WXUNUSED(event))
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnActivated\n");
}

void MyListCtrl::OnListKeyDown(wxListEvent& event)
{
    if ( !wxGetApp().GetTopWindow() )
        return;

    wxTextCtrl *text = ((MyFrame *)wxGetApp().GetTopWindow())->m_logWindow;
    if ( !text )
        return;

    text->WriteText("OnListKeyDown\n");
}


