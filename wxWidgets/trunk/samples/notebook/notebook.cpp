/////////////////////////////////////////////////////////////////////////////
// Name:        samples/notebook/notebook.cpp
// Purpose:     a sample demonstrating notebook usage
// Author:      Julian Smart
// Modified by: Dimitri Schoolwerth
// Created:     26/10/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2002 wxWidgets team
// License:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/imaglist.h"
#include "wx/artprov.h"
#include "notebook.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    // Create the main window
    MyFrame *frame = new MyFrame( wxT("Notebook sample") );

    // Problem with generic wxNotebook implementation whereby it doesn't size
    // properly unless you set the size again
#if defined(__WXMOTIF__)
    int width, height;
    frame->GetSize(& width, & height);
    frame->SetSize(wxDefaultCoord, wxDefaultCoord, width, height);
#endif

    frame->Show();

    return true;
}

wxPanel *CreateUserCreatedPage(wxBookCtrl *parent)
{
    wxPanel *panel = new wxPanel(parent);

    (void) new wxButton( panel, wxID_ANY, wxT("Button"),
        wxPoint(10, 10), wxDefaultSize );

    return panel;
}

wxPanel *CreateRadioButtonsPage(wxBookCtrl *parent)
{
    wxPanel *panel = new wxPanel(parent);

    wxString animals[] = { wxT("Fox"), wxT("Hare"), wxT("Rabbit"),
        wxT("Sabre-toothed tiger"), wxT("T Rex") };

    wxRadioBox *radiobox1 = new wxRadioBox(panel, wxID_ANY, wxT("Choose one"),
        wxDefaultPosition, wxDefaultSize, 5, animals, 2, wxRA_SPECIFY_ROWS);

    wxString computers[] = { wxT("Amiga"), wxT("Commodore 64"), wxT("PET"),
        wxT("Another") };

    wxRadioBox *radiobox2 = new wxRadioBox(panel, wxID_ANY,
        wxT("Choose your favourite"), wxDefaultPosition, wxDefaultSize,
        4, computers, 0, wxRA_SPECIFY_COLS);

    wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
    sizerPanel->Add(radiobox1, 2, wxEXPAND);
    sizerPanel->Add(radiobox2, 1, wxEXPAND);
    panel->SetSizer(sizerPanel);

    return panel;
}

wxPanel *CreateVetoPage(wxBookCtrl *parent)
{
    wxPanel *panel = new wxPanel(parent);

    (void) new wxStaticText( panel, wxID_ANY,
        wxT("This page intentionally left blank"), wxPoint(10, 10) );

    return panel;
}

wxPanel *CreateBigButtonPage(wxBookCtrl *parent)
{
    wxPanel *panel = new wxPanel(parent);

    wxButton *buttonBig = new wxButton(panel, wxID_ANY, wxT("Maximized button"));

    wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
    sizerPanel->Add(buttonBig, 1, wxEXPAND);
    panel->SetSizer(sizerPanel);

    return panel;
}


wxPanel *CreateInsertPage(wxBookCtrl *parent)
{
    wxPanel *panel = new wxPanel(parent);

    panel->SetBackgroundColour( wxColour( wxT("MAROON") ) );
    (void) new wxStaticText( panel, wxID_ANY,
        wxT("This page has been inserted, not added."), wxPoint(10, 10) );

    return panel;
}

int GetIconIndex(wxBookCtrl* bookCtrl)
{
    if (bookCtrl && bookCtrl->GetImageList())
    {
       int nImages = bookCtrl->GetImageList()->GetImageCount();
       if (nImages > 0)
       {
           return bookCtrl->GetPageCount() % nImages;
       }
    }

    return -1;
}

void CreateInitialPages(wxBookCtrl *parent)
{
    // Create and add some panels to the notebook

    wxPanel *panel = CreateRadioButtonsPage(parent);
    parent->AddPage( panel, RADIOBUTTONS_PAGE_NAME, false, GetIconIndex(parent) );

    panel = CreateVetoPage(parent);
    parent->AddPage( panel, VETO_PAGE_NAME, false, GetIconIndex(parent) );

    panel = CreateBigButtonPage(parent);
    parent->AddPage( panel, MAXIMIZED_BUTTON_PAGE_NAME, false, GetIconIndex(parent) );

    panel = CreateInsertPage(parent);
    parent->InsertPage( 0, panel, I_WAS_INSERTED_PAGE_NAME, false, GetIconIndex(parent) );

    parent->SetSelection(1);
}

wxPanel *CreatePage(wxBookCtrl *parent, const wxString&pageName)
{
    if
    (
        pageName.Contains(INSERTED_PAGE_NAME)
        || pageName.Contains(ADDED_PAGE_NAME)
    )
    {
        return CreateUserCreatedPage(parent);
    }

    if (pageName == I_WAS_INSERTED_PAGE_NAME)
    {
        return CreateInsertPage(parent);
    }

    if (pageName == VETO_PAGE_NAME)
    {
        return CreateVetoPage(parent);
    }

    if (pageName == RADIOBUTTONS_PAGE_NAME)
    {
        return CreateRadioButtonsPage(parent);
    }


    if (pageName == MAXIMIZED_BUTTON_PAGE_NAME)
    {
        return CreateBigButtonPage(parent);
    }

    wxFAIL;

    return (wxPanel *) NULL;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
                 long style)
    : wxFrame((wxWindow *) NULL, wxID_ANY, title, pos, size, style)
{
    m_type = ID_BOOK_NOTEBOOK;
    m_orient = ID_ORIENT_DEFAULT;
    m_chkShowImages = true;
    m_multi = false;

    // menu of the sample

    wxMenu *menuType = new wxMenu;
#if wxUSE_NOTEBOOK
    menuType->AppendRadioItem(ID_BOOK_NOTEBOOK,   wxT("&Notebook\tCtrl-1"));
#endif
#if wxUSE_LISTBOOK
    menuType->AppendRadioItem(ID_BOOK_LISTBOOK,   wxT("&Listbook\tCtrl-2"));
#endif
#if wxUSE_CHOICEBOOK
    menuType->AppendRadioItem(ID_BOOK_CHOICEBOOK, wxT("&Choicebook\tCtrl-3"));
#endif

    wxMenu *menuOrient = new wxMenu;
    menuOrient->AppendRadioItem(ID_ORIENT_DEFAULT, wxT("&Default\tCtrl-4"));
    menuOrient->AppendRadioItem(ID_ORIENT_TOP,     wxT("&Top\tCtrl-5"));
    menuOrient->AppendRadioItem(ID_ORIENT_BOTTOM,  wxT("&Bottom\tCtrl-6"));
    menuOrient->AppendRadioItem(ID_ORIENT_LEFT,    wxT("&Left\tCtrl-7"));
    menuOrient->AppendRadioItem(ID_ORIENT_RIGHT,   wxT("&Right\tCtrl-8"));

    wxMenu *menuDo = new wxMenu;
    menuDo->Append(ID_ADD_PAGE, wxT("&Add page"));
    menuDo->Append(ID_INSERT_PAGE, wxT("&Insert page"));
    menuDo->Append(ID_DELETE_CUR_PAGE, wxT("&Delete current page"));
    menuDo->Append(ID_DELETE_LAST_PAGE, wxT("D&elete last page"));
    menuDo->Append(ID_NEXT_PAGE, wxT("&Next page"));

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_ANY, wxT("&Type"), menuType, wxT("Type of control"));
    menuFile->Append(wxID_ANY, wxT("&Orientation"), menuOrient, wxT("Orientation of control"));
    menuFile->AppendCheckItem(ID_SHOW_IMAGES, wxT("&Show images"));
    menuFile->AppendCheckItem(ID_MULTI, wxT("&Multiple lines"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("E&xit"), wxT("Quits the application"));
    menuFile->Check(ID_SHOW_IMAGES, m_chkShowImages);
    menuFile->Check(ID_MULTI, m_multi);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuDo, wxT("&Operations"));
    SetMenuBar(menuBar);

    // books creation

    m_panel      = (wxPanel *)      NULL;
    m_notebook   = (wxNotebook *)   NULL;
    m_choicebook = (wxChoicebook *) NULL;
    m_listbook   = (wxListbook *)   NULL;

    // create a dummy image list with a few icons
    wxSize imageSize(32, 32);

    m_imageList
        = new wxImageList( imageSize.GetWidth(), imageSize.GetHeight() );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, imageSize)
        );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, imageSize)
        );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_WARNING, wxART_OTHER, imageSize)
        );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_ERROR, wxART_OTHER, imageSize)
        );

    m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);

#if USE_LOG
    m_text = new wxTextCtrl(m_panel, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    m_logTargetOld = wxLog::SetActiveTarget( new wxLogTextCtrl(m_text) );
#endif // USE_LOG

    // Set sizers
    m_sizerFrame = new wxBoxSizer(wxVERTICAL);

#if USE_LOG
    m_sizerFrame->Add(m_text, 1, wxEXPAND);
#endif // USE_LOG

    RecreateBooks();

    m_panel->SetSizer(m_sizerFrame);

    m_sizerFrame->Fit(this);
    m_sizerFrame->SetSizeHints(this);

    Centre(wxBOTH);
}

MyFrame::~MyFrame()
{
#if USE_LOG
    delete wxLog::SetActiveTarget(m_logTargetOld);
#endif // USE_LOG

    if (m_imageList)
    {
        delete m_imageList;
        m_imageList = (wxImageList *) NULL;
    }
}

int MyFrame::SelectFlag(int id, int nb, int lb, int chb)
{
    switch (id)
    {
        case ID_NOTEBOOK:   return nb;
        case ID_LISTBOOK:   return lb;
        case ID_CHOICEBOOK: return chb;
    }
    return 0;
}

#define RECREATE( wxBookType , idBook, oldBook , newBook )                         \
{                                                                                  \
    int flags;                                                                     \
                                                                                   \
    switch ( m_orient )                                                            \
    {                                                                              \
        case ID_ORIENT_TOP:                                                        \
            flags = SelectFlag(idBook, wxNB_TOP, wxLB_TOP, wxCHB_TOP);             \
            break;                                                                 \
                                                                                   \
        case ID_ORIENT_BOTTOM:                                                     \
            flags = SelectFlag(idBook, wxNB_BOTTOM, wxLB_BOTTOM, wxCHB_BOTTOM);    \
            break;                                                                 \
                                                                                   \
        case ID_ORIENT_LEFT:                                                       \
            flags = SelectFlag(idBook, wxNB_LEFT, wxLB_LEFT, wxCHB_LEFT);          \
            break;                                                                 \
                                                                                   \
        case ID_ORIENT_RIGHT:                                                      \
            flags = SelectFlag(idBook, wxNB_RIGHT, wxLB_RIGHT, wxCHB_RIGHT);       \
            break;                                                                 \
                                                                                   \
        default:                                                                   \
            flags = SelectFlag(idBook, wxNB_DEFAULT, wxLB_DEFAULT, wxCHB_DEFAULT); \
    }                                                                              \
                                                                                   \
    if ( m_multi && ( idBook == ID_NOTEBOOK ) )                                    \
        flags |= wxNB_MULTILINE;                                                   \
                                                                                   \
    wxBookType *oldBook = newBook;                                                 \
                                                                                   \
    newBook = new wxBookType(m_panel, idBook,                                      \
                             wxDefaultPosition, wxDefaultSize,                     \
                             flags);                                               \
                                                                                   \
    if ( m_chkShowImages )                                                         \
    {                                                                              \
        newBook->SetImageList(m_imageList);                                        \
    }                                                                              \
                                                                                   \
    if (oldBook)                                                                   \
    {                                                                              \
        int sel = oldBook->GetSelection();                                         \
                                                                                   \
        int count = oldBook->GetPageCount();                                       \
        for (int n = 0; n < count; n++)                                            \
        {                                                                          \
            wxString str = oldBook->GetPageText(n);                                \
                                                                                   \
            wxWindow *page = CreatePage(newBook, str);                             \
            newBook->AddPage(page, str, false, GetIconIndex(newBook) );            \
        }                                                                          \
                                                                                   \
        m_sizerFrame->Detach(oldBook);                                             \
                                                                                   \
        delete oldBook;                                                            \
                                                                                   \
        if (sel != wxNOT_FOUND)                                                    \
        {                                                                          \
            newBook->SetSelection(sel);                                            \
        }                                                                          \
                                                                                   \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        wxPanel *panel = CreateRadioButtonsPage(newBook);                          \
        newBook->AddPage( panel, RADIOBUTTONS_PAGE_NAME, false, GetIconIndex(newBook) ); \
                                                                                   \
        panel = CreateVetoPage(newBook);                                           \
        newBook->AddPage( panel, VETO_PAGE_NAME, false, GetIconIndex(newBook) );   \
                                                                                   \
        panel = CreateBigButtonPage(newBook);                                      \
        newBook->AddPage( panel, MAXIMIZED_BUTTON_PAGE_NAME, false, GetIconIndex(newBook) ); \
                                                                                   \
        panel = CreateInsertPage(newBook);                                         \
        newBook->InsertPage( 0, panel, I_WAS_INSERTED_PAGE_NAME, false, GetIconIndex(newBook) ); \
                                                                                   \
        newBook->SetSelection(1);                                                  \
    }                                                                              \
                                                                                   \
    m_sizerFrame->Insert(0, newBook, 5, wxEXPAND | wxALL, 4);                      \
                                                                                   \
    m_sizerFrame->Hide(newBook);                                                   \
}

void MyFrame::RecreateBooks()
{
    RECREATE( wxNotebook   , ID_NOTEBOOK   , notebook   , m_notebook );
    RECREATE( wxListbook   , ID_LISTBOOK   , listbook   , m_listbook );
    RECREATE( wxChoicebook , ID_CHOICEBOOK , choicebook , m_choicebook );

    ShowCurrentBook();
}

wxBookCtrl *MyFrame::GetCurrentBook()
{
    switch (m_type)
    {
        case ID_BOOK_NOTEBOOK:   return m_notebook;
        case ID_BOOK_LISTBOOK:   return m_listbook;
        case ID_BOOK_CHOICEBOOK: return m_choicebook;
    }
    return NULL;
}

void MyFrame::ShowCurrentBook()
{
    switch(m_type)
    {
        case ID_BOOK_NOTEBOOK:   if(m_notebook)   m_sizerFrame->Show(m_notebook);   break;
        case ID_BOOK_LISTBOOK:   if(m_listbook)   m_sizerFrame->Show(m_listbook);   break;
        case ID_BOOK_CHOICEBOOK: if(m_choicebook) m_sizerFrame->Show(m_choicebook); break;
    }

    m_sizerFrame->Layout();
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // File menu
    EVT_MENU_RANGE(ID_BOOK_NOTEBOOK,ID_BOOK_MAX,MyFrame::OnType)
    EVT_MENU_RANGE(ID_ORIENT_DEFAULT,ID_ORIENT_MAX,MyFrame::OnOrient)
    EVT_MENU(ID_SHOW_IMAGES, MyFrame::OnShowImages)
    EVT_MENU(ID_MULTI, MyFrame::OnMulti)
    EVT_MENU(wxID_EXIT,MyFrame::OnExit)

    // Operations menu
    EVT_MENU(ID_ADD_PAGE, MyFrame::OnAddPage)
    EVT_MENU(ID_INSERT_PAGE, MyFrame::OnInsertPage)
    EVT_MENU(ID_DELETE_CUR_PAGE, MyFrame::OnDeleteCurPage)
    EVT_MENU(ID_DELETE_LAST_PAGE, MyFrame::OnDeleteLastPage)
    EVT_MENU(ID_NEXT_PAGE, MyFrame::OnNextPage)

    // Book controls
    EVT_NOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK, MyFrame::OnNotebook)
    EVT_NOTEBOOK_PAGE_CHANGING(ID_NOTEBOOK, MyFrame::OnNotebook)
    EVT_LISTBOOK_PAGE_CHANGED(ID_LISTBOOK, MyFrame::OnListbook)
    EVT_LISTBOOK_PAGE_CHANGING(ID_LISTBOOK, MyFrame::OnListbook)
    EVT_CHOICEBOOK_PAGE_CHANGED(ID_CHOICEBOOK, MyFrame::OnChoicebook)
    EVT_CHOICEBOOK_PAGE_CHANGING(ID_CHOICEBOOK, MyFrame::OnChoicebook)

    // Update title in idle time
    EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

void MyFrame::OnType(wxCommandEvent& event)
{
    wxBookCtrl *currBook = GetCurrentBook();

    m_type = event.GetId();

    if (currBook)
        m_sizerFrame->Hide(currBook);

    ShowCurrentBook();
}

void MyFrame::OnOrient(wxCommandEvent& event)
{
    m_orient = event.GetId();
    RecreateBooks();
    m_sizerFrame->Layout();
}

void MyFrame::OnShowImages(wxCommandEvent& event)
{
    m_chkShowImages = event.IsChecked();
    RecreateBooks();
    m_sizerFrame->Layout();
}

void MyFrame::OnMulti(wxCommandEvent& event)
{
    m_multi = event.IsChecked();
    RecreateBooks();
    m_sizerFrame->Layout();
    wxLogMessage(_T("Multiline setting works only in wxNotebook."));
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MyFrame::OnAddPage(wxCommandEvent& WXUNUSED(event))
{
    static unsigned s_pageAdded = 0;

    wxBookCtrl *currBook = GetCurrentBook();

    if ( currBook )
    {
        wxPanel *panel = new wxPanel( currBook, wxID_ANY );
        (void) new wxButton( panel, wxID_ANY, wxT("First button"),
            wxPoint(10, 10), wxDefaultSize );
        (void) new wxButton( panel, wxID_ANY, wxT("Second button"),
            wxPoint(50, 100), wxDefaultSize );

        currBook->AddPage(panel, wxString::Format(ADDED_PAGE_NAME wxT("%u"),
            ++s_pageAdded), true, GetIconIndex(currBook) );
    }
}

void MyFrame::OnInsertPage(wxCommandEvent& WXUNUSED(event))
{
    static unsigned s_pageIns = 0;

    wxBookCtrl *currBook = GetCurrentBook();

    if ( currBook )
    {
        wxPanel *panel = CreateUserCreatedPage( currBook );

        currBook->InsertPage( 0, panel,
            wxString::Format(INSERTED_PAGE_NAME wxT("%u"), ++s_pageIns), false,
            GetIconIndex(currBook) );

        currBook->SetSelection(0);
    }
}

void MyFrame::OnDeleteCurPage(wxCommandEvent& WXUNUSED(event))
{
    wxBookCtrl *currBook = GetCurrentBook();

    if ( currBook )
    {
        int sel = currBook->GetSelection();

        if (sel != wxNOT_FOUND)
        {
            currBook->DeletePage(sel);
        }
    }
}

void MyFrame::OnDeleteLastPage(wxCommandEvent& WXUNUSED(event))
{
    wxBookCtrl *currBook = GetCurrentBook();

    if ( currBook )
    {
        int page = currBook->GetPageCount();

        if ( page != 0 )
        {
            currBook->DeletePage(page - 1);
        }
    }
}

void MyFrame::OnNextPage(wxCommandEvent& WXUNUSED(event))
{
    wxBookCtrl *currBook = GetCurrentBook();

    if ( currBook )
    {
        currBook->AdvanceSelection();
    }
}

void MyFrame::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
    static int s_nPages = wxNOT_FOUND;
    static int s_nSel = wxNOT_FOUND;
    static wxBookCtrl *s_currBook = NULL;

    wxBookCtrl *currBook = GetCurrentBook();

    int nPages = currBook ? currBook->GetPageCount() : 0;
    int nSel = currBook ? currBook->GetSelection() : wxNOT_FOUND;

    if ( nPages != s_nPages || nSel != s_nSel || s_currBook != currBook )
    {
        s_nPages = nPages;
        s_nSel = nSel;
        s_currBook = currBook;

        wxString selection;
        if ( nSel == wxNOT_FOUND )
            selection << wxT("not found");
        else
            selection << nSel;

        wxString title;
        title.Printf(wxT("Notebook and friends (%d pages, selection: %s)"), nPages, selection.c_str());

        SetTitle(title);
    }
}

#define BOOKEVENT(OnBook,wxBookEvent,bookStr,wxEVT_PAGE_CHANGED,wxEVT_PAGE_CHANGING,s_num) \
void MyFrame::OnBook(wxBookEvent& event)                                                   \
{                                                                                          \
    wxString str = wxT("Unknown ");                                                        \
    str << wxT(bookStr);                                                                   \
    str << wxT(" event");                                                                  \
                                                                                           \
    wxEventType eventType = event.GetEventType();                                          \
                                                                                           \
    if (eventType == wxEVT_PAGE_CHANGED)                                                   \
    {                                                                                      \
        str = wxT("Changed");                                                              \
    }                                                                                      \
    else if (eventType == wxEVT_PAGE_CHANGING)                                             \
    {                                                                                      \
        int idx = event.GetOldSelection();                                                 \
        wxBookCtrl *book = (wxBookCtrl *)event.GetEventObject();                           \
        if ( idx != wxNOT_FOUND && book && book->GetPageText(idx) == VETO_PAGE_NAME )      \
        {                                                                                  \
            if                                                                             \
            (                                                                              \
                wxMessageBox(                                                              \
                wxT("Are you sure you want to leave this page?\n")                         \
                wxT("(This demonstrates veto-ing)"),                                       \
                          wxT("Notebook sample"),                                          \
                          wxICON_QUESTION | wxYES_NO, this) != wxYES )                     \
            {                                                                              \
                event.Veto();                                                              \
                                                                                           \
                return;                                                                    \
            }                                                                              \
                                                                                           \
        }                                                                                  \
                                                                                           \
        str = wxT("Changing");                                                             \
    }                                                                                      \
                                                                                           \
    static int s_num = 0;                                                                  \
                                                                                           \
    wxString logMsg;                                                                       \
    logMsg.Printf(wxT("%s event #%d: %s (%d) Sel %d, OldSel %d"),                          \
                  wxT(bookStr),s_num++, str.c_str(), eventType,                            \
                  event.GetSelection(), event.GetOldSelection());                          \
                                                                                           \
    wxLogMessage(logMsg.c_str());                                                          \
                                                                                           \
    m_text->SetInsertionPointEnd();                                                        \
                                                                                           \
    event.Skip();                                                                          \
}

BOOKEVENT(OnNotebook,wxNotebookEvent,"wxNotebook",wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,s_numNotebookEvents)
BOOKEVENT(OnChoicebook,wxChoicebookEvent,"wxChoicebook",wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED,wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING,s_numChoicebookEvents)
BOOKEVENT(OnListbook,wxListbookEvent,"wxListbook",wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED,wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING,s_numListbookEvents)
