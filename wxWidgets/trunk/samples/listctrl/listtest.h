/////////////////////////////////////////////////////////////////////////////
// Name:        listctrl.h
// Purpose:     wxListCtrl sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Define a new application type
class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

class MyListCtrl: public wxListCtrl
{
public:
    MyListCtrl(wxWindow *parent, const wxWindowID id, const wxPoint& pos,
            const wxSize& size, long style):
        wxListCtrl(parent, id, pos, size, style)
        {
        }

    // add one item to the listctrl in report mode
    void InsertItemInReportView(int i);

    void OnColClick(wxListEvent& event);
    void OnBeginDrag(wxListEvent& event);
    void OnBeginRDrag(wxListEvent& event);
    void OnBeginLabelEdit(wxListEvent& event);
    void OnEndLabelEdit(wxListEvent& event);
    void OnDeleteItem(wxListEvent& event);
    void OnDeleteAllItems(wxListEvent& event);
    void OnGetInfo(wxListEvent& event);
    void OnSetInfo(wxListEvent& event);
    void OnSelected(wxListEvent& event);
    void OnDeselected(wxListEvent& event);
    void OnListKeyDown(wxListEvent& event);
    void OnActivated(wxListEvent& event);

    void OnChar(wxKeyEvent& event);

private:
    void LogEvent(const wxListEvent& event, const wxChar *eventName);

    virtual wxString OnGetItemText(long item, long column) const;
    virtual int OnGetItemImage(long item) const;

    DECLARE_EVENT_TABLE()
};

// Define a new frame type
class MyFrame: public wxFrame
{
public:
    MyListCtrl *m_listCtrl;
    wxTextCtrl *m_logWindow;

    MyFrame(const wxChar *title, int x, int y, int w, int h);
    ~MyFrame();

public:
    void OnSize(wxSizeEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnListView(wxCommandEvent& event);
    void OnReportView(wxCommandEvent& event);
    void OnIconView(wxCommandEvent& event);
    void OnIconTextView(wxCommandEvent& event);
    void OnSmallIconView(wxCommandEvent& event);
    void OnSmallIconTextView(wxCommandEvent& event);
    void OnVirtualView(wxCommandEvent& event);

    void OnToggleFirstSel(wxCommandEvent& event);
    void OnDeselectAll(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);
    void OnAdd(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnDeleteAll(wxCommandEvent& event);
    void OnSort(wxCommandEvent& event);
    void OnSetFgColour(wxCommandEvent& event);
    void OnSetBgColour(wxCommandEvent& event);
    void OnToggleMultiSel(wxCommandEvent& event);
    void OnShowColInfo(wxCommandEvent& event);
    void OnShowSelInfo(wxCommandEvent& event);

    void OnUpdateShowColInfo(wxUpdateUIEvent& event);

    wxImageList *m_imageListNormal;
    wxImageList *m_imageListSmall;

private:
    // recreate the list control with the new flags
    void RecreateList(long flags, bool withText = TRUE);

    // fill the control with items depending on the view
    void InitWithListItems();
    void InitWithReportItems();
    void InitWithIconItems(bool withText, bool sameIcon = FALSE);
    void InitWithVirtualItems();

    wxLog *m_logOld;

    DECLARE_EVENT_TABLE()
};


// IDs for the menu commands
enum
{
    LIST_ABOUT,
    LIST_QUIT,

    LIST_LIST_VIEW,
    LIST_ICON_VIEW,
    LIST_ICON_TEXT_VIEW,
    LIST_SMALL_ICON_VIEW,
    LIST_SMALL_ICON_TEXT_VIEW,
    LIST_REPORT_VIEW,
    LIST_VIRTUAL_VIEW,

    LIST_DESELECT_ALL,
    LIST_SELECT_ALL,
    LIST_DELETE_ALL,
    LIST_DELETE,
    LIST_ADD,
    LIST_SORT,
    LIST_SET_FG_COL,
    LIST_SET_BG_COL,
    LIST_TOGGLE_MULTI_SEL,
    LIST_TOGGLE_FIRST,
    LIST_SHOW_COL_INFO,
    LIST_SHOW_SEL_INFO,

    LIST_CTRL                   = 1000
};

