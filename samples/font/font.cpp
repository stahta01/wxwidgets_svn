/////////////////////////////////////////////////////////////////////////////
// Name:        font.cpp
// Purpose:     wxFont demo
// Author:      Vadim Zeitlin
// Modified by:
// Created:     30.09.99
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all <standard< wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"

    #include "wx/log.h"
#endif

#include "wx/choicdlg.h"
#include "wx/fontdlg.h"
#include "wx/fontenum.h"
#include "wx/fontmap.h"
#include "wx/encconv.h"
#include "wx/splitter.h"
#include "wx/textfile.h"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// MyCanvas is a canvas on which we show the font sample
class MyCanvas: public wxWindow
{
public:
    MyCanvas( wxWindow *parent );
    ~MyCanvas();

    // accessors for the frame
    const wxFont& GetTextFont() const { return m_font; }
    const wxColour& GetColour() const { return m_colour; }
    void SetTextFont(const wxFont& font) { m_font = font; }
    void SetColour(const wxColour& colour) { m_colour = colour; }

    // event handlers
    void OnPaint( wxPaintEvent &event );

private:
    wxColour m_colour;
    wxFont   m_font;

    DECLARE_EVENT_TABLE()
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    // accessors
    MyCanvas *GetCanvas() const { return m_canvas; }

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnIncFont(wxCommandEvent& event) { DoResizeFont(+2); }
    void OnDecFont(wxCommandEvent& event) { DoResizeFont(-2); }

    void OnBold(wxCommandEvent& event);
    void OnItalic(wxCommandEvent& event);
    void OnUnderline(wxCommandEvent& event);

    void OnViewMsg(wxCommandEvent& event);
    void OnSelectFont(wxCommandEvent& event);
    void OnEnumerateFamiliesForEncoding(wxCommandEvent& event);
    void OnEnumerateFamilies(wxCommandEvent& WXUNUSED(event))
        { DoEnumerateFamilies(FALSE); }
    void OnEnumerateFixedFamilies(wxCommandEvent& WXUNUSED(event))
        { DoEnumerateFamilies(TRUE); }
    void OnEnumerateEncodings(wxCommandEvent& event);

    void OnCheckNativeToFromString(wxCommandEvent& event);

protected:
    bool DoEnumerateFamilies(bool fixedWidthOnly,
                             wxFontEncoding encoding = wxFONTENCODING_SYSTEM,
                             bool silent = FALSE);

    void DoResizeFont(int diff);
    void DoChangeFont(const wxFont& font, const wxColour& col = wxNullColour);

    size_t      m_fontSize; // in points

    wxTextCtrl *m_textctrl;
    MyCanvas   *m_canvas;

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Font_Quit = 1,
    Font_About,
    Font_ViewMsg,
    Font_IncSize,
    Font_DecSize,
    Font_Bold,
    Font_Italic,
    Font_Underlined,
    Font_Choose = 100,
    Font_EnumFamiliesForEncoding,
    Font_EnumFamilies,
    Font_EnumFixedFamilies,
    Font_EnumEncodings,
    Font_CheckNativeToFromString,
    Font_Max
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Font_Quit,  MyFrame::OnQuit)
    EVT_MENU(Font_ViewMsg, MyFrame::OnViewMsg)
    EVT_MENU(Font_About, MyFrame::OnAbout)

    EVT_MENU(Font_IncSize, MyFrame::OnIncFont)
    EVT_MENU(Font_DecSize, MyFrame::OnDecFont)
    EVT_MENU(Font_Bold, MyFrame::OnBold)
    EVT_MENU(Font_Italic, MyFrame::OnItalic)
    EVT_MENU(Font_Underlined, MyFrame::OnUnderline)
    EVT_MENU(Font_CheckNativeToFromString, MyFrame::OnCheckNativeToFromString)

    EVT_MENU(Font_Choose, MyFrame::OnSelectFont)
    EVT_MENU(Font_EnumFamiliesForEncoding, MyFrame::OnEnumerateFamiliesForEncoding)
    EVT_MENU(Font_EnumFamilies, MyFrame::OnEnumerateFamilies)
    EVT_MENU(Font_EnumFixedFamilies, MyFrame::OnEnumerateFixedFamilies)
    EVT_MENU(Font_EnumEncodings, MyFrame::OnEnumerateEncodings)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // Create the main application window
    MyFrame *frame = new MyFrame("Font wxWindows demo",
                                 wxPoint(50, 50), wxSize(600, 400));

    // Show it and tell the application that it's our main window
    frame->Show(TRUE);
    SetTopWindow(frame);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size), m_textctrl(NULL)
{
    m_fontSize = 12;

    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    menuFile->Append(Font_ViewMsg, "&View...\tCtrl-V",
                     "View an email message file");
    menuFile->AppendSeparator();
    menuFile->Append(Font_About, "&About...\tCtrl-A", "Show about dialog");
    menuFile->AppendSeparator();
    menuFile->Append(Font_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenu *menuFont = new wxMenu;
    menuFont->Append(Font_IncSize, "&Increase font size by 2 points\tCtrl-I");
    menuFont->Append(Font_DecSize, "&Decrease font size by 2 points\tCtrl-D");
    menuFont->AppendSeparator();
    menuFont->Append(Font_Bold, "&Bold\tCtrl-B", "Toggle bold state", TRUE);
    menuFont->Append(Font_Italic, "&Oblique\tCtrl-O", "Toggle italic state", TRUE);
    menuFont->Append(Font_Underlined, "&Underlined\tCtrl-U",
                     "Toggle underlined state", TRUE);
    menuFont->AppendSeparator();
    menuFont->Append(Font_CheckNativeToFromString,
                     "Check Native Font Info To/From String");

    wxMenu *menuSelect = new wxMenu;
    menuSelect->Append(Font_Choose, "&Select font...\tCtrl-S",
                     "Select a standard font");
    menuSelect->AppendSeparator();
    menuSelect->Append(Font_EnumFamilies, "Enumerate font &families\tCtrl-F");
    menuSelect->Append(Font_EnumFixedFamilies,
                     "Enumerate fi&xed font families\tCtrl-X");
    menuSelect->Append(Font_EnumEncodings,
                     "Enumerate &encodings\tCtrl-E");
    menuSelect->Append(Font_EnumFamiliesForEncoding,
                     "Find font for en&coding...\tCtrl-C",
                     "Find font families for given encoding");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuFont, "F&ont");
    menuBar->Append(menuSelect, "&Select");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    wxSplitterWindow *splitter = new wxSplitterWindow(this);

    m_textctrl = new wxTextCtrl(splitter, -1,
                                "Paste text here to see how it looks\n"
                                "like in the given font",
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE);

    m_canvas = new MyCanvas(splitter);

    splitter->SplitHorizontally(m_textctrl, m_canvas, 100);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar();
    SetStatusText("Welcome to wxWindows font demo!");
}

// --------------------------------------------------------

class MyEncodingEnumerator : public wxFontEnumerator
{
public:
    MyEncodingEnumerator()
        { m_n = 0; }

    const wxString& GetText() const
        { return m_text; }

protected:
    virtual bool OnFontEncoding(const wxString& facename,
                                const wxString& encoding)
    {
        wxString text;
        text.Printf(wxT("Encoding %d: %s (available in facename '%s')\n"),
                    ++m_n, encoding.c_str(), facename.c_str());
        m_text += text;
        return TRUE;
    }

private:
    size_t m_n;
    wxString m_text;
};

void MyFrame::OnEnumerateEncodings(wxCommandEvent& WXUNUSED(event))
{
    MyEncodingEnumerator fontEnumerator;

    fontEnumerator.EnumerateEncodings();

    wxLogMessage(wxT("Enumerating all available encodings:\n%s"),
                 fontEnumerator.GetText().c_str());
}

// -------------------------------------------------------------

class MyFontEnumerator : public wxFontEnumerator
{
public:
    bool GotAny() const
        { return !m_facenames.IsEmpty(); }

    const wxArrayString& GetFacenames() const
        { return m_facenames; }

protected:
    virtual bool OnFacename(const wxString& facename)
    {
        m_facenames.Add(facename);
        return TRUE;
    }

    private:
        wxArrayString m_facenames;
} fontEnumerator;

bool MyFrame::DoEnumerateFamilies(bool fixedWidthOnly,
                                  wxFontEncoding encoding,
                                  bool silent)
{
    MyFontEnumerator fontEnumerator;

    fontEnumerator.EnumerateFacenames(encoding, fixedWidthOnly);

    if ( fontEnumerator.GotAny() )
    {
        int nFacenames = fontEnumerator.GetFacenames().GetCount();
        if ( !silent )
        {
            wxLogStatus(this, wxT("Found %d %sfonts"),
                        nFacenames, fixedWidthOnly ? wxT("fixed width ") : wxT(""));
        }

        wxString facename;
        if ( silent )
        {
            // choose the first
            facename = fontEnumerator.GetFacenames().Item(0);
        }
        else
        {
            // let the user choose
            wxString *facenames = new wxString[nFacenames];
            int n;
            for ( n = 0; n < nFacenames; n++ )
                facenames[n] = fontEnumerator.GetFacenames().Item(n);

            n = wxGetSingleChoiceIndex("Choose a facename", "Font demo",
                                       nFacenames, facenames, this);

            if ( n != -1 )
                facename = facenames[n];

            delete [] facenames;
        }

        if ( !facename.IsEmpty() )
        {
            wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_NORMAL, FALSE, facename, encoding);

            DoChangeFont(font);
        }

        return TRUE;
    }
    else if ( !silent )
    {
        wxLogWarning(wxT("No such fonts found."));
    }

    return FALSE;
}

void MyFrame::OnEnumerateFamiliesForEncoding(wxCommandEvent& WXUNUSED(event))
{
    static wxFontEncoding encodings[] =
    {
        wxFONTENCODING_ISO8859_1,
        wxFONTENCODING_ISO8859_2,
        wxFONTENCODING_ISO8859_5,
        wxFONTENCODING_ISO8859_7,
        wxFONTENCODING_ISO8859_15,
        wxFONTENCODING_KOI8,
        wxFONTENCODING_CP1250,
        wxFONTENCODING_CP1251,
        wxFONTENCODING_CP1252,
    };

    static const wxString encodingNames[] =
    {
        "Western European (ISO-8859-1)",
        "Central European (ISO-8859-2)",
        "Cyrillic (ISO-8859-5)",
        "Greek (ISO-8859-7)",
        "Western European with Euro (ISO-8859-15)",
        "KOI8-R",
        "Windows Central European (CP 1250)",
        "Windows Cyrillic (CP 1251)",
        "Windows Western European (CP 1252)",
    };

    int n = wxGetSingleChoiceIndex("Choose an encoding", "Font demo",
                                   WXSIZEOF(encodingNames),
                                   encodingNames,
                                   this);

    if ( n != -1 )
    {
        DoEnumerateFamilies(FALSE, encodings[n]);
    }
}

void MyFrame::OnCheckNativeToFromString(wxCommandEvent& WXUNUSED(event))
{
    wxString fontInfo = m_canvas->GetTextFont().GetNativeFontInfoDesc();

    if ( fontInfo.IsEmpty() )
    {
        wxLogError(wxT("Native font info string is empty!"));
    }
    else
    {
        wxFont *font = wxFont::New(fontInfo);
        if ( fontInfo != font->GetNativeFontInfoDesc() )
            wxLogError(wxT("wxNativeFontInfo ToString()/FromString() broken!"));
        else
            wxLogMessage(wxT("wxNativeFontInfo works: %s"), fontInfo.c_str());

        delete font;
     }
}

void MyFrame::DoResizeFont(int diff)
{
    wxFont font = m_canvas->GetTextFont();

    font.SetPointSize(font.GetPointSize() + diff);
    DoChangeFont(font);
}

void MyFrame::OnBold(wxCommandEvent& event)
{
    wxFont font = m_canvas->GetTextFont();

    font.SetWeight(event.IsChecked() ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
    DoChangeFont(font);
}

void MyFrame::OnItalic(wxCommandEvent& event)
{
    wxFont font = m_canvas->GetTextFont();

    font.SetStyle(event.IsChecked() ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL);
    DoChangeFont(font);
}

void MyFrame::OnUnderline(wxCommandEvent& event)
{
    wxFont font = m_canvas->GetTextFont();

    font.SetUnderlined(event.IsChecked());
    DoChangeFont(font);
}

void MyFrame::DoChangeFont(const wxFont& font, const wxColour& col)
{
    m_canvas->SetTextFont(font);
    if ( col.Ok() )
        m_canvas->SetColour(col);
    m_canvas->Refresh();

    m_textctrl->SetFont(font);
    if ( col.Ok() )
        m_textctrl->SetForegroundColour(col);
}

void MyFrame::OnSelectFont(wxCommandEvent& WXUNUSED(event))
{
    wxFontData data;
    data.SetInitialFont(m_canvas->GetTextFont());
    data.SetColour(m_canvas->GetColour());

    wxFontDialog dialog(this, &data);
    if ( dialog.ShowModal() == wxID_OK )
    {
        wxFontData retData = dialog.GetFontData();
        wxFont font = retData.GetChosenFont();
        wxColour colour = retData.GetColour();

        DoChangeFont(font, colour);

        // update the state of the bold/italic/underlined menu items
        wxMenuBar *mbar = GetMenuBar();
        if ( mbar )
        {
            mbar->Check(Font_Bold, font.GetWeight() == wxFONTWEIGHT_BOLD);
            mbar->Check(Font_Italic, font.GetStyle() == wxFONTSTYLE_ITALIC);
            mbar->Check(Font_Underlined, font.GetUnderlined());
        }
    }
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnViewMsg(wxCommandEvent& WXUNUSED(event))
{
    // first, choose the file
    static wxString s_dir, s_file;
    wxFileDialog dialog(this, "Open an email message file",
                        s_dir, s_file);
    if ( dialog.ShowModal() != wxID_OK )
        return;

    // save for the next time
    s_dir = dialog.GetDirectory();
    s_file = dialog.GetFilename();

    wxString filename = dialog.GetPath();

    // load it and search for Content-Type header
    wxTextFile file(filename);
    if ( !file.Open() )
        return;

    wxString charset;

    static const char *prefix = "Content-Type: text/plain; charset=";
    const size_t len = strlen(prefix);

    size_t n, count = file.GetLineCount();
    for ( n = 0; n < count; n++ )
    {
        wxString line = file[n];

        if ( !line )
        {
            // if it is an email message, headers are over, no need to parse
            // all the file
            break;
        }

        if ( line.Left(len) == prefix )
        {
            // found!
            const wxChar *pc = line.c_str() + len;
            if ( *pc == '"' )
                pc++;

            while ( *pc && *pc != '"' )
            {
                charset += *pc++;
            }

            break;
        }
    }

    if ( !charset )
    {
        wxLogError(wxT("The file '%s' doesn't contain charset information."),
                   filename.c_str());

        return;
    }

    // ok, now get the corresponding encoding
    wxFontEncoding fontenc = wxTheFontMapper->CharsetToEncoding(charset);
    if ( fontenc == wxFONTENCODING_SYSTEM )
    {
        wxLogError(wxT("Charset '%s' is unsupported."), charset.c_str());
        return;
    }

    m_textctrl->LoadFile(filename);

    if ( fontenc == wxFONTENCODING_UTF8 ||
            !wxTheFontMapper->IsEncodingAvailable(fontenc) )
    {
        // try to find some similar encoding:
        wxFontEncoding encAlt;
        if ( wxTheFontMapper->GetAltForEncoding(fontenc, &encAlt) )
        {
            wxEncodingConverter conv;

            if (conv.Init(fontenc, encAlt))
            {
                fontenc = encAlt;
                m_textctrl -> SetValue(conv.Convert(m_textctrl -> GetValue()));
            }
            else
            {
                wxLogWarning(wxT("Cannot convert from '%s' to '%s'."),
                             wxFontMapper::GetEncodingDescription(fontenc).c_str(),
                             wxFontMapper::GetEncodingDescription(encAlt).c_str());
            }
        }
        else
            wxLogWarning(wxT("No fonts for encoding '%s' on this system."),
                         wxFontMapper::GetEncodingDescription(fontenc).c_str());
    }

    // and now create the correct font
    if ( !DoEnumerateFamilies(FALSE, fontenc, TRUE /* silent */) )
    {
        wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_NORMAL, FALSE /* !underlined */,
                    wxEmptyString /* facename */, fontenc);
        if ( font.Ok() )
        {
            DoChangeFont(font);
        }
        else
        {
            wxLogWarning(wxT("No fonts for encoding '%s' on this system."),
                         wxFontMapper::GetEncodingDescription(fontenc).c_str());
        }
    }
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("wxWindows font demo\n"
                 "(c) 1999 Vadim Zeitlin",
                 "About Font",
                 wxOK | wxICON_INFORMATION, this);
}

// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
    EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

MyCanvas::MyCanvas( wxWindow *parent )
        : wxWindow( parent, -1 ),
          m_colour(*wxRED), m_font(*wxNORMAL_FONT)
{
}

MyCanvas::~MyCanvas()
{
}

void MyCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    // set background
    dc.SetBackground(wxBrush(wxT("white"), wxSOLID));
    dc.Clear();

    // one text line height
    wxCoord hLine = dc.GetCharHeight();

    // the current text origin
    wxCoord x = 5,
            y = 5;

    // output the font name/info
    wxString fontInfo;
    fontInfo.Printf(wxT("Font size is %d points, family: %s, encoding: %s"),
                    m_font.GetPointSize(),
                    m_font.GetFamilyString().c_str(),
                    wxTheFontMapper->
                        GetEncodingDescription(m_font.GetEncoding()).c_str());

    dc.DrawText(fontInfo, x, y);
    y += hLine;
                        
    fontInfo.Printf(wxT("Style: %s, weight: %s, fixed width: %s"),
                    m_font.GetStyleString().c_str(),
                    m_font.GetWeightString().c_str(),
                    m_font.IsFixedWidth() ? _T("yes") : _T("no"));

    dc.DrawText(fontInfo, x, y);
    y += hLine;

    if ( m_font.Ok() )
    {
        wxNativeFontInfo *info = m_font.GetNativeFontInfo();
        if ( info )
        {
            delete info;

            wxString fontDesc = m_font.GetNativeFontInfoUserDesc();
            fontInfo.Printf(wxT("Native font info: %s"), fontDesc.c_str());

            dc.DrawText(fontInfo, x, y);
            y += hLine;
        }
    }

    y += hLine;

    // prepare to draw the font
    dc.SetFont(m_font);
    dc.SetTextForeground(m_colour);

    // the size of one cell (Normally biggest char + small margin)
    long maxCharWidth, maxCharHeight;
    dc.GetTextExtent(wxT("W"), &maxCharWidth, &maxCharHeight);
    int w = maxCharWidth + 5,
        h = maxCharHeight + 4;


    // print all font symbols from 32 to 256 in 7 rows of 32 chars each
    for ( int i = 0; i < 7; i++ )
    {
        for ( int j = 0; j < 32; j++ )
        {
            wxChar c = 32 * (i + 1) + j;

            long charWidth, charHeight;
            dc.GetTextExtent(c, &charWidth, &charHeight);
            dc.DrawText
            (
                c,
                x + w*j + (maxCharWidth - charWidth) / 2 + 1,
                y + h*i + (maxCharHeight - charHeight) / 2
            );
        }
    }

    // draw the lines between them
    dc.SetPen(wxPen(wxColour("blue"), 1, wxSOLID));
    int l;

    // horizontal
    for ( l = 0; l < 8; l++ )
    {
        int yl = y + h*l - 2;
        dc.DrawLine(x - 2, yl, x + 32*w - 1, yl);
    }

    // and vertical
    for ( l = 0; l < 33; l++ )
    {
        int xl = x + w*l - 2;
        dc.DrawLine(xl, y - 2, xl, y + 7*h - 1);
    }
}
