/////////////////////////////////////////////////////////////////////////////
// Name:        validate.h
// Purpose:     wxWindows validation sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma interface
#endif

// Define a new application type
class MyApp : public wxApp
{
public:
    bool OnInit();
};

// Define a new frame type
class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxChar *title, int x, int y, int w, int h);

    void OnQuit(wxCommandEvent& event);
    void OnTestDialog(wxCommandEvent& event);
    void OnSilent(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

class MyDialog : public wxDialog
{
public:
    MyDialog(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
            const long style = wxDEFAULT_DIALOG_STYLE);
};

class MyData
{
public:
    wxString m_string;

    MyData() { m_string = _T("My string"); }
};

#define VALIDATE_DIALOG_ID      200

#define VALIDATE_TEST_DIALOG    2
#define VALIDATE_SILENT         3
#define VALIDATE_TEXT           101

