/////////////////////////////////////////////////////////////////////////////
// Name:        proplist.cpp
// Purpose:     Property sheet test implementation
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "proplist_sample.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#if !wxUSE_PROPSHEET
#error Please set wxUSE_PROPSHEET to 1 in include/wx/msw/setup.h and recompile.
#endif

#include "proplist.h"

IMPLEMENT_APP(MyApp)

wxPropertyValidatorRegistry myListValidatorRegistry;
wxPropertyValidatorRegistry myFormValidatorRegistry;

MyApp::MyApp(void)
{
    m_childWindow = NULL;
    m_mainFrame = NULL;
}

bool MyApp::OnInit(void)
{
  RegisterValidators();

  // Create the main frame window
  m_mainFrame = new MyFrame(NULL, "wxPropertySheet Demo", wxPoint(0, 0), wxSize(300, 400), wxDEFAULT_FRAME_STYLE);

  // Make a menubar
  wxMenu *file_menu = new wxMenu;
  file_menu->Append(PROPERTY_TEST_DIALOG_LIST, "Test property list &dialog...");
  file_menu->Append(PROPERTY_TEST_FRAME_LIST, "Test property list &frame...");
  file_menu->AppendSeparator();
  file_menu->Append(PROPERTY_TEST_DIALOG_FORM, "Test property form d&ialog...");
  file_menu->Append(PROPERTY_TEST_FRAME_FORM, "Test property form f&rame...");
  file_menu->AppendSeparator();
  file_menu->Append(PROPERTY_QUIT, "E&xit");

  wxMenu *help_menu = new wxMenu;
  help_menu->Append(PROPERTY_ABOUT, "&About");

  wxMenuBar *menu_bar = new wxMenuBar;

  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");

  // Associate the menu bar with the frame
  m_mainFrame->SetMenuBar(menu_bar);

  m_mainFrame->Centre(wxBOTH);
  m_mainFrame->Show(TRUE);

  SetTopWindow(m_mainFrame);
    
  return TRUE;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_CLOSE(MyFrame::OnCloseWindow)
    EVT_MENU(PROPERTY_QUIT, MyFrame::OnQuit)
    EVT_MENU(PROPERTY_ABOUT, MyFrame::OnAbout)
    EVT_MENU(PROPERTY_TEST_DIALOG_LIST, MyFrame::OnDialogList)
    EVT_MENU(PROPERTY_TEST_FRAME_LIST, MyFrame::OnFrameList)
    EVT_MENU(PROPERTY_TEST_DIALOG_FORM, MyFrame::OnDialogForm)
    EVT_MENU(PROPERTY_TEST_FRAME_FORM, MyFrame::OnFrameForm)
END_EVENT_TABLE()

// Define my frame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, long type):
  wxFrame(frame, -1, title, pos, size, type)
{
}

// Define the behaviour for the frame closing
// - must delete all frames except for the main one.
void MyFrame::OnCloseWindow(wxCloseEvent& event)
{
    if (wxGetApp().m_childWindow)
    {
        wxGetApp().m_childWindow->Close(TRUE);
    }

    Destroy();
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
    Close(TRUE);
}

void MyFrame::OnDialogList(wxCommandEvent& event)
{
    wxGetApp().PropertyListTest(TRUE);
}

void MyFrame::OnFrameList(wxCommandEvent& event)
{
    wxGetApp().PropertyListTest(FALSE);
}

void MyFrame::OnDialogForm(wxCommandEvent& event)
{
    wxGetApp().PropertyFormTest(TRUE);
}

void MyFrame::OnFrameForm(wxCommandEvent& event)
{
    wxGetApp().PropertyFormTest(FALSE);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    (void)wxMessageBox("Property Classes Demo\nAuthor: Julian Smart", "About Property Classes Test");
}

void MyApp::RegisterValidators(void)
{
  myListValidatorRegistry.RegisterValidator((wxString)"real", new wxRealListValidator);
  myListValidatorRegistry.RegisterValidator((wxString)"string", new wxStringListValidator);
  myListValidatorRegistry.RegisterValidator((wxString)"integer", new wxIntegerListValidator);
  myListValidatorRegistry.RegisterValidator((wxString)"bool", new wxBoolListValidator);
  myListValidatorRegistry.RegisterValidator((wxString)"stringlist", new wxListOfStringsListValidator);

  myFormValidatorRegistry.RegisterValidator((wxString)"real", new wxRealFormValidator);
  myFormValidatorRegistry.RegisterValidator((wxString)"string", new wxStringFormValidator);
  myFormValidatorRegistry.RegisterValidator((wxString)"integer", new wxIntegerFormValidator);
  myFormValidatorRegistry.RegisterValidator((wxString)"bool", new wxBoolFormValidator);
}

void MyApp::PropertyListTest(bool useDialog)
{
    if (m_childWindow)
        return;

    wxPropertySheet *sheet = new wxPropertySheet;

    sheet->AddProperty(new wxProperty("fred", 1.0, "real"));
    sheet->AddProperty(new wxProperty("tough choice", (bool)TRUE, "bool"));
    sheet->AddProperty(new wxProperty("ian", (long)45, "integer", new wxIntegerListValidator(-50, 50)));
    sheet->AddProperty(new wxProperty("bill", 25.0, "real", new wxRealListValidator(0.0, 100.0)));
    sheet->AddProperty(new wxProperty("julian", "one", "string"));
    sheet->AddProperty(new wxProperty("bitmap", "none", "string", new wxFilenameListValidator("Select a bitmap file", "*.bmp")));
    wxStringList *strings = new wxStringList(wxT("one"), wxT("two"), wxT("three"), NULL);
    sheet->AddProperty(new wxProperty("constrained", "one", "string", new wxStringListValidator(strings)));

    wxStringList *strings2 = new wxStringList(wxT("earth"), wxT("fire"), wxT("wind"), wxT("water"), NULL);
    sheet->AddProperty(new wxProperty("string list", strings2, "stringlist"));

    wxPropertyListView *view = new wxPropertyListView
        (
            NULL,
            wxPROP_BUTTON_OK | wxPROP_BUTTON_CANCEL | wxPROP_BUTTON_CHECK_CROSS
            |wxPROP_DYNAMIC_VALUE_FIELD|wxPROP_PULLDOWN|wxPROP_SHOWVALUES
        );

    wxDialog *propDialog = NULL;
    wxPropertyListFrame *propFrame = NULL;
    if (useDialog)
    {
        propDialog = new PropListDialog(view, NULL, "Property Sheet Test",
        wxPoint(-1, -1), wxSize(400, 500), wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODELESS);
        m_childWindow = propDialog;
    }
    else
    {
        propFrame = new PropListFrame(view, NULL, "Property Sheet Test", wxPoint(-1, -1), wxSize(400, 500));
        m_childWindow = propFrame;
    }

    view->AddRegistry(&myListValidatorRegistry);

    if (useDialog)
    {
        view->ShowView(sheet, (wxPanel *)propDialog);
        propDialog->Centre(wxBOTH);
        propDialog->Show(TRUE);
    }
    else
    {
        propFrame->Initialize();
        view->ShowView(sheet, propFrame->GetPropertyPanel());

        propFrame->Centre(wxBOTH);
        propFrame->Show(TRUE);
    }
}

void MyApp::PropertyFormTest(bool useDialog)
{
    if (m_childWindow)
        return;

    wxPropertySheet *sheet = new wxPropertySheet;

    sheet->AddProperty(new wxProperty("fred", 25.0, "real", new wxRealFormValidator(0.0, 100.0)));
    sheet->AddProperty(new wxProperty("tough choice", (bool)TRUE, "bool"));
    sheet->AddProperty(new wxProperty("ian", (long)45, "integer", new wxIntegerFormValidator(-50, 50)));
    sheet->AddProperty(new wxProperty("julian", "one", "string"));
    wxStringList *strings = new wxStringList(wxT("one"), wxT("two"), wxT("three"), NULL);
    sheet->AddProperty(new wxProperty("constrained", "one", "string", new wxStringFormValidator(strings)));

    wxPropertyFormView *view = new wxPropertyFormView(NULL);

    wxDialog *propDialog = NULL;
    wxPropertyFormFrame *propFrame = NULL;

    if (useDialog)
    {
        propDialog = new PropFormDialog(view, NULL, "Property Form Test",
        wxPoint(-1, -1), wxSize(380, 250), wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL);
        m_childWindow = propDialog;
    }
    else
    {
        propFrame = new PropFormFrame(view, NULL, "Property Form Test",
            wxPoint(-1, -1), wxSize(380, 250));
        propFrame->Initialize();
        m_childWindow = propFrame;
    }

    // BCC32 does not like ?:
    wxWindow *panel ;
    if ( propDialog )
    {
        panel = propDialog;
    }
    else
    {
        panel = propFrame->GetPropertyPanel() ;
    }

    wxLayoutConstraints* c;

#if 0
    if (!propDialog)
    {
        c = new wxLayoutConstraints;
        c->left.SameAs(m_childWindow, wxLeft, 4);
        c->right.SameAs(m_childWindow, wxRight, 4);
        c->top.SameAs(m_childWindow, wxTop, 4);
        c->bottom.SameAs(m_childWindow, wxBottom, 40);

        panel->SetConstraints(c);
    }
#endif
 
    // Add items to the panel
    wxButton *okButton = new wxButton(panel, wxID_OK, "OK", wxPoint(-1, -1),
        wxSize(80, 26), 0, wxDefaultValidator, "ok");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel",  wxPoint(-1, -1),
        wxSize(80, 26), 0, wxDefaultValidator, "cancel");
    wxButton *updateButton = new wxButton(panel, wxID_PROP_UPDATE, "Update",  wxPoint(-1, -1),
        wxSize(80, 26), 0, wxDefaultValidator, "update");
    wxButton *revertButton = new wxButton(panel, wxID_PROP_REVERT, "Revert",  wxPoint(-1, -1),
        wxSize(80, 26), 0, wxDefaultValidator, "revert");

    c = new wxLayoutConstraints;
    c->right.SameAs(panel, wxRight, 4);
    c->bottom.SameAs(panel, wxBottom, 4);
    c->height.AsIs();
    c->width.AsIs();
    revertButton->SetConstraints(c);

    c = new wxLayoutConstraints;
    c->right.SameAs(revertButton, wxLeft, 4);
    c->bottom.SameAs(panel, wxBottom, 4);
    c->height.AsIs();
    c->width.AsIs();
    updateButton->SetConstraints(c);

    c = new wxLayoutConstraints;
    c->right.SameAs(updateButton, wxLeft, 4);
    c->bottom.SameAs(panel, wxBottom, 4);
    c->height.AsIs();
    c->width.AsIs();
    cancelButton->SetConstraints(c);

    c = new wxLayoutConstraints;
    c->right.SameAs(cancelButton, wxLeft, 4);
    c->bottom.SameAs(panel, wxBottom, 4);
    c->height.AsIs();
    c->width.AsIs();
    okButton->SetConstraints(c);

    // The name of this text item matches the "fred" property
    wxTextCtrl *text = new wxTextCtrl(panel, -1, "Fred", wxPoint(-1, -1), wxSize(
    200, -1), 0, wxDefaultValidator, "fred");

    c = new wxLayoutConstraints;
    c->left.SameAs(panel, wxLeft, 4);
    c->top.SameAs(panel, wxTop, 4);
    c->height.AsIs();
    c->width.AsIs();
    text->SetConstraints(c);

    wxCheckBox *checkBox = new wxCheckBox(panel, -1, "Yes or no", wxPoint(-1, -1),
        wxSize(-1, -1), 0, wxDefaultValidator, "tough choice");

    c = new wxLayoutConstraints;
    c->left.SameAs(text, wxRight, 20);
    c->top.SameAs(panel, wxTop, 4);
    c->height.AsIs();
    c->width.AsIs();
    checkBox->SetConstraints(c);

  wxSlider *slider = new wxSlider(panel, -1, -50, 50, 150, wxPoint(-1, -1),
    wxSize(200,10), 0, wxDefaultValidator, "ian");

    c = new wxLayoutConstraints;
    c->left.SameAs(panel, wxLeft, 4);
    c->top.SameAs(text, wxBottom, 10);
    c->height.AsIs();
    c->width.AsIs();
    slider->SetConstraints(c);

    wxListBox *listBox = new wxListBox(panel, -1, wxPoint(-1, -1),
        wxSize(200, 100), 0, NULL, 0, wxDefaultValidator, "constrained");

    c = new wxLayoutConstraints;
    c->left.SameAs(panel, wxLeft, 4);
    c->top.SameAs(slider, wxBottom, 10);
    c->height.AsIs();
    c->width.AsIs();
    listBox->SetConstraints(c);

    view->AddRegistry(&myFormValidatorRegistry);

    panel->SetAutoLayout(TRUE);

    view->ShowView(sheet, panel);
    view->AssociateNames();
    view->TransferToDialog();

    if (useDialog) {
        propDialog->Layout();
        propDialog->Centre(wxBOTH);
        propDialog->Show(TRUE);
    }
    else
    {
        // panel->Layout();
        propFrame->Centre(wxBOTH);
        propFrame->Show(TRUE);
    }
}

BEGIN_EVENT_TABLE(PropListFrame, wxPropertyListFrame)
    EVT_CLOSE(PropListFrame::OnCloseWindow)
END_EVENT_TABLE()

void PropListFrame::OnCloseWindow(wxCloseEvent& event)
{
    wxGetApp().m_childWindow = NULL;

    wxPropertyListFrame::OnCloseWindow(event);
}

BEGIN_EVENT_TABLE(PropListDialog, wxPropertyListDialog)
    EVT_CLOSE(PropListDialog::OnCloseWindow)
END_EVENT_TABLE()

void PropListDialog::OnCloseWindow(wxCloseEvent& event)
{
    wxGetApp().m_childWindow = NULL;

    wxPropertyListDialog::OnCloseWindow(event);
}

BEGIN_EVENT_TABLE(PropFormFrame, wxPropertyFormFrame)
    EVT_CLOSE(PropFormFrame::OnCloseWindow)
    EVT_SIZE(PropFormFrame::OnSize)
END_EVENT_TABLE()

void PropFormFrame::OnCloseWindow(wxCloseEvent& event)
{
    wxGetApp().m_childWindow = NULL;

    wxPropertyFormFrame::OnCloseWindow(event);
}

void PropFormFrame::OnSize(wxSizeEvent& event)
{
    wxPropertyFormFrame::OnSize(event);
    GetPropertyPanel()->Layout();
}

BEGIN_EVENT_TABLE(PropFormDialog, wxPropertyFormDialog)
    EVT_CLOSE(PropFormDialog::OnCloseWindow)
END_EVENT_TABLE()

void PropFormDialog::OnCloseWindow(wxCloseEvent& event)
{
    wxGetApp().m_childWindow = NULL;

    wxPropertyFormDialog::OnCloseWindow(event);
}

