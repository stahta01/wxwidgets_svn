/////////////////////////////////////////////////////////////////////////////
// Name:        listctrl.h
// Purpose:     wxListCtrl sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Define a new application type
class MyApp: public wxApp
{ public:
    bool OnInit(void);

    wxImageList *m_imageListNormal;
    wxImageList *m_imageListSmall;
};

class MyListCtrl: public wxListCtrl
{
public:
   MyListCtrl(wxWindow *parent, const wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style):
        wxListCtrl(parent, id, pos, size, style)
   {
   }

	void OnBeginDrag(wxListEvent& event);
	void OnBeginRDrag(wxListEvent& event);
	void OnBeginLabelEdit(wxListEvent& event);
	void OnEndLabelEdit(wxListEvent& event);
	void OnDeleteItem(wxListEvent& event);
	void OnGetInfo(wxListEvent& event);
	void OnSetInfo(wxListEvent& event);
	void OnSelected(wxListEvent& event);
	void OnDeselected(wxListEvent& event);
	void OnKeyDown(wxListEvent& event);

   DECLARE_EVENT_TABLE()
};

// Define a new frame type
class MyFrame: public wxFrame
{ public:
    MyListCtrl *m_listCtrl;
    wxTextCtrl *m_logWindow;

    MyFrame(wxFrame *frame, char *title, int x, int y, int w, int h);
    ~MyFrame(void);
    
 public:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnListView(wxCommandEvent& event);
    void OnReportView(wxCommandEvent& event);
    void OnIconView(wxCommandEvent& event);
    void OnIconTextView(wxCommandEvent& event);
    void OnSmallIconView(wxCommandEvent& event);
    void OnSmallIconTextView(wxCommandEvent& event);
	bool OnClose(void) { return TRUE; }

   DECLARE_EVENT_TABLE()
};


// ID for the menu quit command
#define LIST_QUIT 	                1
#define LIST_LIST_VIEW              2
#define LIST_ICON_VIEW              3
#define LIST_ICON_TEXT_VIEW         4
#define LIST_SMALL_ICON_VIEW        5
#define LIST_SMALL_ICON_TEXT_VIEW   6
#define LIST_REPORT_VIEW            7
#define LIST_ABOUT                  102

#define LIST_CTRL                   1000


