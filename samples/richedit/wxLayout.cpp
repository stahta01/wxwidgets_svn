/*
 * Program: wxLayout
 *
 * Author: Karsten Ball�der
 *
 * Copyright: (C) 1998, Karsten Ball�der <Ballueder@usa.net>
 *
 */

#ifdef __GNUG__
#pragma implementation "wxLayout.h"
#endif

#include "wx/wxprec.h"
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#include "wxLayout.h"
#include <wx/textfile.h>
#include <wx/image.h>

#if wxUSE_IOSTREAMH
    #include <iostream.h>
#else
    #include <iostream>
#endif

#include   "Micon.xpm"


//-----------------------------------------------------------------------------
// main program
//-----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

//-----------------------------------------------------------------------------
// MyFrame
//-----------------------------------------------------------------------------

enum ids
{
    ID_ADD_SAMPLE = 1, ID_CLEAR, ID_PRINT,
    ID_PRINT_SETUP, ID_PAGE_SETUP, ID_PREVIEW, ID_PRINT_PS,
    ID_PRINT_SETUP_PS, ID_PAGE_SETUP_PS,ID_PREVIEW_PS,
    ID_WRAP, ID_NOWRAP, ID_PASTE, ID_COPY, ID_CUT,
    ID_COPY_PRIMARY, ID_PASTE_PRIMARY,
    ID_FIND,
    ID_WXLAYOUT_DEBUG, ID_QUIT, ID_CLICK, ID_HTML, ID_TEXT,
    ID_TEST, ID_LINEBREAKS_TEST, ID_LONG_TEST, ID_URL_TEST
};


IMPLEMENT_DYNAMIC_CLASS( MyFrame, wxFrame )

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
   EVT_MENU(ID_PRINT, MyFrame::OnPrint)
   EVT_MENU(ID_PREVIEW, MyFrame::OnPrintPreview)
   EVT_MENU(ID_PRINT_SETUP, MyFrame::OnPrintSetup)
   EVT_MENU(ID_PAGE_SETUP, MyFrame::OnPageSetup)
   EVT_MENU(ID_PRINT_PS, MyFrame::OnPrintPS)
   EVT_MENU(ID_PREVIEW_PS, MyFrame::OnPrintPreviewPS)
   EVT_MENU(ID_PRINT_SETUP_PS, MyFrame::OnPrintSetupPS)
   EVT_MENU(ID_PAGE_SETUP_PS, MyFrame::OnPageSetupPS)
   EVT_MENU    (-1,       MyFrame::OnCommand)
   EVT_COMMAND (-1,-1,    MyFrame::OnCommand)
   EVT_CHAR    (  wxLayoutWindow::OnChar  )
END_EVENT_TABLE()


MyFrame::MyFrame(void) :
   wxFrame( (wxFrame *) NULL, -1, _T("wxLayout"),
             wxPoint(880,100), wxSize(256,256) )
{
   CreateStatusBar( 2 );

   SetStatusText( _T("wxLayout by Karsten Ball�der.") );

   wxMenuBar *menu_bar = new wxMenuBar();

   wxMenu *file_menu = new wxMenu;
   file_menu->Append(ID_PRINT, _T("&Print..."), _T("Print"));
   file_menu->Append(ID_PRINT_SETUP, _T("Print &Setup..."),_T("Setup printer properties"));
   file_menu->Append(ID_PAGE_SETUP, _T("Page Set&up..."), _T("Page setup"));
   file_menu->Append(ID_PREVIEW, _T("Print Pre&view"), _T("Preview"));
#ifdef __WXMSW__
   file_menu->AppendSeparator();
   file_menu->Append(ID_PRINT_PS, _T("Print PostScript..."), _T("Print (PostScript)"));
   file_menu->Append(ID_PRINT_SETUP_PS, _T("Print Setup PostScript..."), _T("Setup printer properties (PostScript)"));
   file_menu->Append(ID_PAGE_SETUP_PS, _T("Page Setup PostScript..."), _T("Page setup (PostScript)"));
   file_menu->Append(ID_PREVIEW_PS, _T("Print Preview PostScript"), _T("Preview (PostScript)"));
#endif
   file_menu->AppendSeparator();
   file_menu->Append( ID_TEXT, _T("Export &Text"));
   file_menu->Append( ID_HTML, _T("Export &HTML"));
   file_menu->Append( ID_QUIT, _T("E&xit"));
   menu_bar->Append(file_menu, _T("&File"));

   wxMenu *edit_menu = new wxMenu;
   edit_menu->Append( ID_CLEAR, _T("C&lear"));
   edit_menu->Append( ID_ADD_SAMPLE, _T("&Example"));
   edit_menu->Append( ID_LONG_TEST, _T("Add &many lines"));
   edit_menu->AppendSeparator();
   edit_menu->Append( ID_LINEBREAKS_TEST, _T("Add &several lines"));
   edit_menu->Append( ID_URL_TEST, _T("Insert an &URL"));
   edit_menu->AppendSeparator();
   edit_menu->Append(ID_WRAP, _T("&Wrap mode"), _T("Activate wrapping at pixel 200."));
   edit_menu->Append(ID_NOWRAP, _T("&No-wrap mode"), _T("Deactivate wrapping."));
   edit_menu->AppendSeparator();
   edit_menu->Append(ID_COPY, _T("&Copy"), _T("Copy text to clipboard."));
   edit_menu->Append(ID_CUT, _T("Cu&t"), _T("Cut text to clipboard."));
   edit_menu->Append(ID_PASTE,_T("&Paste"), _T("Paste text from clipboard."));
#ifdef __WXGTK__
   edit_menu->Append(ID_COPY_PRIMARY, _T("C&opy primary"), _T("Copy text to primary selecton."));
   edit_menu->Append(ID_PASTE_PRIMARY, _T("&Paste primary"), _T("Paste text from primary selection."));
#endif
   edit_menu->Append(ID_FIND, _T("&Find"), _T("Find text."));
   menu_bar->Append(edit_menu, _T("&Edit") );

#ifndef __WXMSW__
   menu_bar->Show( TRUE );
#endif // MSW

   SetMenuBar( menu_bar );

   m_lwin = new wxLayoutWindow(this);
   m_lwin->SetStatusBar(GetStatusBar(), 0, 1);
   m_lwin->SetMouseTracking(true);
   m_lwin->SetEditable(true);
   m_lwin->SetWrapMargin(40);
   m_lwin->SetFocus();

   // JACS: under MSW, the window doesn't show the caret initially,
   // and the following line I added doesn't help either.
   // going to another window and then back again fixes it.
   // m_lwin->OnSetFocus(wxFocusEvent());

   Clear();

#if 0
   // create and set the background bitmap (this will result in a lattice)
   static const int sizeBmp = 10;
   wxBitmap *bitmap = new wxBitmap(sizeBmp, sizeBmp);
   wxMemoryDC dcMem;
   dcMem.SelectObject( *bitmap );
   dcMem.SetBackground( *wxWHITE_BRUSH );
   dcMem.Clear();

   dcMem.SetPen( *wxGREEN_PEN );
   dcMem.DrawLine(sizeBmp/2, 0, sizeBmp/2, sizeBmp);
   dcMem.DrawLine(0, sizeBmp/2, sizeBmp, sizeBmp/2);

   dcMem.SelectObject( wxNullBitmap );

   m_lwin->SetBackgroundBitmap(bitmap);
#endif // 0
};

void
MyFrame::AddSampleText(wxLayoutList *llist)
{
   llist->Clear(wxSWISS,16,wxNORMAL,wxNORMAL, false);
   llist->SetFont(-1,-1,-1,-1,-1,_T("blue"));
   llist->Insert(_T("blue"));
   llist->SetFont(-1,-1,-1,-1,-1,_T("black"));
   llist->Insert(_T("The quick brown fox jumps over the lazy dog."));
   llist->LineBreak();

   llist->SetFont(wxROMAN,16,wxNORMAL,wxNORMAL, false);
   llist->Insert(_T("--"));
   llist->LineBreak();

   llist->SetFont(wxROMAN);
   llist->Insert(_T("The quick brown fox jumps over the lazy dog."));
   llist->LineBreak();
   llist->Insert(_T("Hello "));
#if wxICON_IS_BITMAP
   llist->Insert(new wxLayoutObjectIcon(new wxICON(Micon)));
#else
   llist->Insert(new wxLayoutObjectIcon(new wxBitmap (wxICON(Micon))));
#endif
   llist->SetFontWeight(wxBOLD);
   llist->Insert(_T("World! "));
   llist->SetFontWeight(wxNORMAL);
   llist->Insert(_T("The quick brown fox jumps..."));
   llist->LineBreak();
   llist->Insert(_T("over the lazy dog."));
   llist->SetFont(-1,-1,-1,-1,true);
   llist->Insert(_T("underlined"));
   llist->SetFont(-1,-1,-1,-1,false);
   llist->SetFont(wxROMAN);
   llist->Insert(_T("This is "));
   llist->SetFont(-1,-1,-1,wxBOLD);  llist->Insert(_T("BOLD "));  llist->SetFont(-1,-1,-1,wxNORMAL);
   llist->Insert(_T("and "));
   llist->SetFont(-1,-1,wxITALIC);
   llist->Insert(_T("italics "));
   llist->SetFont(-1,-1,wxNORMAL);
   llist->LineBreak();
   llist->Insert(_T("and "));
   llist->SetFont(-1,-1,wxSLANT);
   llist->Insert(_T("slanted"));
   llist->SetFont(-1,-1,wxNORMAL);
   llist->Insert(_T(" text."));
   llist->LineBreak();
   llist->Insert(_T("and "));
   llist->SetFont(-1,-1,-1,-1,-1,_T("blue"));
   llist->Insert(_T("blue"));
   llist->SetFont(-1,-1,-1,-1,-1,_T("black"));
   llist->Insert(_T(" and "));
   llist->SetFont(-1,-1,-1,-1,-1,_T("green"),_T("black"));
   llist->Insert(_T("green on black"));
   llist->SetFont(-1,-1,-1,-1,-1,_T("black"),_T("white"));
   llist->Insert(_T(" text."));
   llist->LineBreak();

   llist->SetFont(-1,-1,wxSLANT);
   llist->Insert(_T("Slanted"));
   llist->SetFont(-1,-1,wxNORMAL);
   llist->Insert(_T(" and normal text and "));
   llist->SetFont(-1,-1,wxSLANT);
   llist->Insert(_T("slanted"));
   llist->SetFont(-1,-1,wxNORMAL);
   llist->Insert(_T(" again."));
   llist->LineBreak();

   // add some more text for testing:
   llist->Insert(_T("And here the source for the test program:"));
   llist->LineBreak();
   llist->SetFont(wxTELETYPE,16);
   llist->Insert(_T("And here the source for the test program:"));
   llist->LineBreak();
   llist->Insert(_T("And here the source for the test program:"));
   llist->LineBreak();
   llist->Insert(_T("And here the source for the test program:"));

   wxFile file( _T("wxLayout.cpp") );
   if ( file.IsOpened() )
   {
      off_t len = file.Length();
      wxChar *data = (wxChar *)malloc(2*len);
      if ( file.Read(data, len) == len )
      {
        wxLayoutImportText(llist, data);
      }
      free(data);
   }

   llist->MoveCursorTo(wxPoint(0,0));
   m_lwin->SetDirty();
   m_lwin->Refresh();
}

void
MyFrame::Clear(void)
{
   wxColour colBg(0, 0, 0);

   m_lwin->Clear(wxROMAN,16,wxNORMAL,wxNORMAL, false, wxRED, &colBg);
}


void MyFrame::OnCommand( wxCommandEvent &event )
{
    switch (event.GetId())
    {
    case ID_QUIT:
        Close( TRUE );
        break;
    case ID_PRINT:
    {
        wxPrinter printer;
        wxLayoutPrintout printout(m_lwin->GetLayoutList(),_("M: Printout"));
        if (! printer.Print(this, &printout, TRUE))
        {
            // Had to remove the split up strings that used to be below, and
            // put them into one long strong. Otherwise MSVC would give an
            // error "C2308: concatenating mismatched wide strings" when
            // building a Unicode version.
            wxMessageBox
            (
                _("There was a problem with printing the message:\nperhaps your current printer is not set up correctly?"),
                _("Printing"), wxOK
            );
        }
        break;
    }

    case ID_NOWRAP:
    case ID_WRAP:
        m_lwin->SetWrapMargin(event.GetId() == ID_NOWRAP ? 0 : 40);
        break;
    case ID_ADD_SAMPLE:
        AddSampleText(m_lwin->GetLayoutList());
        break;
    case ID_CLEAR:
        Clear();
        break;
   case ID_CLICK:
        cerr << "Received click event." << endl;
        break;
   case ID_PASTE:
        m_lwin->Paste(TRUE);
        m_lwin->Refresh(FALSE);
        break;
#ifdef __WXGTK__
    case ID_PASTE_PRIMARY:
        // text only from primary:
        m_lwin->Paste(FALSE, TRUE);
        m_lwin->Refresh(FALSE);
        break;
    case ID_COPY_PRIMARY:
        // copy text-only to primary selection:
        m_lwin->Copy(FALSE,FALSE,TRUE);
        m_lwin->Refresh(FALSE);
        break;
#endif
    case ID_COPY:
        m_lwin->Copy(TRUE,TRUE,FALSE);
        m_lwin->Refresh(FALSE);
        break;
    case ID_CUT:
        m_lwin->Cut();
        m_lwin->Refresh(FALSE);
        break;
#ifdef M_BASEDIR
    case ID_FIND:
        m_lwin->Find("void");
        m_lwin->Refresh(FALSE);
        break;
#endif
    case ID_HTML:
    {
        wxLayoutExportObject *export0;
        wxLayoutExportStatus status(m_lwin->GetLayoutList());

        cout << "<HTML>" << endl;
        while((export0 = wxLayoutExport( &status,
                                         WXLO_EXPORT_AS_HTML)) != NULL)
        {
            if(export0->type == WXLO_EXPORT_HTML)
                cout << *(export0->content.text);
            else
                ; // ignore itcout << "<!--UNKNOWN OBJECT>";

            delete export0;
        }
        break;
    }

    case ID_TEXT:
    {
        wxLayoutExportObject *export0;
        wxLayoutExportStatus status(m_lwin->GetLayoutList());

        while((export0 = wxLayoutExport( &status, WXLO_EXPORT_AS_TEXT)) != NULL)
        {
            if(export0->type == WXLO_EXPORT_TEXT)
                cout << *(export0->content.text);
            else
                cout << "<!--UNKNOWN OBJECT>";

            delete export0;
        }

        break;
    }


    case ID_LONG_TEST:
    {
        wxString line;
        wxLayoutList *llist = m_lwin->GetLayoutList();
        for(int i = 1; i < 300; i++)
        {
            line.Printf(wxT("This is line number %d."), i);
            llist->Insert(line);
            llist->LineBreak();
        }

        llist->MoveCursorTo(wxPoint(0,0));
        m_lwin->SetDirty();
        m_lwin->Refresh();
        break;
    }

    case ID_LINEBREAKS_TEST:
        wxLayoutImportText
        (
            m_lwin->GetLayoutList(),
            wxT("This is a text\nwith embedded line\nbreaks.\n")
        );

        m_lwin->SetDirty();
        m_lwin->Refresh();
        break;

    case ID_URL_TEST:
        // VZ: this doesn't work, of course, but I think it should -
        //     wxLayoutWindow should have a flag m_highlightUrls and do it itself
        //     (instead of doing it manually like M does now)
        m_lwin->GetLayoutList()->Insert(_T("http://www.wxwindows.org/"));
    }
};

void MyFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
#ifdef __WXMSW__
   wxGetApp().SetPrintMode(wxPRINT_WINDOWS);
#else
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);
#endif
   wxPrinter printer;
   wxLayoutPrintout printout( m_lwin->GetLayoutList(), _T("Printout from wxLayout"));
   if (! printer.Print(this, &printout, TRUE))
      wxMessageBox(
         _T("There was a problem printing.\nPerhaps your current printer is not set correctly?"),
         _T("Printing"), wxOK);
}

void MyFrame::OnPrintPS(wxCommandEvent& WXUNUSED(event))
{
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);

#ifdef OS_UNIX
   wxPostScriptPrinter printer;
   wxLayoutPrintout printout( m_lwin->GetLayoutList(),"My printout");
   printer.Print(this, &printout, TRUE);
#endif
}

void MyFrame::OnPrintPreview(wxCommandEvent& WXUNUSED(event))
{
#ifdef __WXMSW__
   wxGetApp().SetPrintMode(wxPRINT_WINDOWS);
#else
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);
#endif
   wxPrintData printData;

   // Pass two printout objects: for preview, and possible printing.
   wxPrintPreview *preview = new wxPrintPreview(new
                                                wxLayoutPrintout(
                                                   m_lwin->GetLayoutList()), new wxLayoutPrintout( m_lwin->GetLayoutList()), & printData);
   if (!preview->Ok())
   {
      delete preview;
      wxMessageBox(_T("There was a problem previewing.\nPerhaps your current printer is not set correctly?"), _T("Previewing"), wxOK);
      return;
   }

   wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _T("Demo Print Preview"), wxPoint(100, 100), wxSize(600, 650));
   frame->Centre(wxBOTH);
   frame->Initialize();
   frame->Show(TRUE);
}

void MyFrame::OnPrintPreviewPS(wxCommandEvent& WXUNUSED(event))
{
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);

   wxPrintData printData;

   // Pass two printout objects: for preview, and possible printing.
   wxPrintPreview *preview = new wxPrintPreview(new wxLayoutPrintout( m_lwin->GetLayoutList()), new wxLayoutPrintout( m_lwin->GetLayoutList()), & printData);
   wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _T("Demo Print Preview"), wxPoint(100, 100), wxSize(600, 650));
   frame->Centre(wxBOTH);
   frame->Initialize();
   frame->Show(TRUE);
}

void MyFrame::OnPrintSetup(wxCommandEvent& WXUNUSED(event))
{
#ifdef OS_WIN
   wxGetApp().SetPrintMode(wxPRINT_WINDOWS);
#else
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);
#endif
   wxPrintDialog printerDialog(this, & m_PrintData);
   printerDialog.ShowModal();
}

void MyFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
#ifdef __WXMSW__
   wxGetApp().SetPrintMode(wxPRINT_WINDOWS);
#else
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);
#endif
   wxPageSetupData data;

#if defined(__WXMSW__) || defined(__WXMAC__)
   wxPageSetupDialog pageSetupDialog(this, & data);
#else
   wxGenericPageSetupDialog pageSetupDialog(this, & data);
#endif
   pageSetupDialog.ShowModal();

   data = pageSetupDialog.GetPageSetupData();
}

void MyFrame::OnPrintSetupPS(wxCommandEvent& WXUNUSED(event))
{
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);

   wxPrintData data;

#if defined(__WXMSW__) || defined(__WXMAC__)
   wxPrintDialog printerDialog(this, & data);
#else
   wxGenericPrintDialog printerDialog(this, & data);
#endif
   printerDialog.ShowModal();
}

void MyFrame::OnPageSetupPS(wxCommandEvent& WXUNUSED(event))
{
   wxGetApp().SetPrintMode(wxPRINT_POSTSCRIPT);

   wxPageSetupData data;
#if defined(__WXMSW__) || defined(__WXMAC__)
   wxPageSetupDialog pageSetupDialog(this, & data);
#else
   wxGenericPageSetupDialog pageSetupDialog(this, & data);
#endif
   pageSetupDialog.ShowModal();
}


//-----------------------------------------------------------------------------
// MyApp
//-----------------------------------------------------------------------------

MyApp::MyApp(void) :
   wxApp( )
{
};

bool MyApp::OnInit(void)
{
   wxFrame *frame = new MyFrame();
   wxInitAllImageHandlers();
   frame->Show( TRUE );
//   wxSetAFMPath("/usr/local/src/wxWindows/misc/afm/");
   return TRUE;
};





