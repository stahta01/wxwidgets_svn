/////////////////////////////////////////////////////////////////////////////
// Name:        tbtest.h
// Purpose:     wxTaskBarIcon sample
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

class MyTaskBarIcon: public wxTaskBarIcon
{
public:
    MyTaskBarIcon() {};

    void OnLButtonDClick(wxEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    void OnMenuSetNewIcon(wxCommandEvent&);

    virtual wxMenu *CreatePopupMenu();

DECLARE_EVENT_TABLE()
};


// Define a new application
class MyApp: public wxApp
{
public:
    bool OnInit(void);
};

class MyDialog: public wxDialog
{
public:
    MyDialog(wxWindow* parent, const wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, const long windowStyle = wxDEFAULT_DIALOG_STYLE);
    ~MyDialog();

    void OnOK(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void Init(void);

protected:
    MyTaskBarIcon   *m_taskBarIcon;

DECLARE_EVENT_TABLE()
};


