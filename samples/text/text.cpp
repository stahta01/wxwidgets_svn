/////////////////////////////////////////////////////////////////////////////
// Name:        controls.cpp
// Purpose:     TextCtrl wxWindows sample
// Author:      Robert Roebling
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling, Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "controls.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if wxUSE_CLIPBOARD
    #include "wx/dataobj.h"
    #include "wx/clipbrd.h"
#endif

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

// We test for wxUSE_DRAG_AND_DROP also, because data objects may not be
// implemented for compilers that can't cope with the OLE parts in
// wxUSE_DRAG_AND_DROP.
#if !wxUSE_DRAG_AND_DROP
    #undef wxUSE_CLIPBOARD
    #define wxUSE_CLIPBOARD 0
#endif

//----------------------------------------------------------------------
// class definitions
//----------------------------------------------------------------------

class MyApp: public wxApp
{
public:
    bool OnInit();
};

// a text ctrl which allows to call different wxTextCtrl functions
// interactively by pressing function keys in it
class MyTextCtrl : public wxTextCtrl
{
public:
    MyTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value,
               const wxPoint &pos, const wxSize &size, int style = 0)
        : wxTextCtrl(parent, id, value, pos, size, style) { }

    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnChar(wxKeyEvent& event);

private:
    static inline wxChar GetChar(bool on, wxChar c) { return on ? c : _T('-'); }
    void LogEvent(const wxChar *name, wxKeyEvent& event) const;

    DECLARE_EVENT_TABLE()
};

class MyPanel: public wxPanel
{
public:
    MyPanel(wxFrame *frame, int x, int y, int w, int h);

#if wxUSE_CLIPBOARD
    void DoPasteFromClipboard();
    void DoCopyToClipboard();
#endif // wxUSE_CLIPBOARD

    void DoMoveToEndOfText();
    void DoMoveToEndOfEntry();

    void OnSize( wxSizeEvent &event );

    MyTextCtrl    *m_text;
    MyTextCtrl    *m_password;
    MyTextCtrl    *m_enter;
    MyTextCtrl    *m_tab;
    MyTextCtrl    *m_readonly;

    MyTextCtrl    *m_multitext;
    MyTextCtrl    *m_horizontal;

    wxTextCtrl    *m_log;

private:
    DECLARE_EVENT_TABLE()
};

class MyFrame: public wxFrame
{
public:
    MyFrame(wxFrame *frame, const char *title, int x, int y, int w, int h);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
#if wxUSE_TOOLTIPS
    void OnSetTooltipDelay(wxCommandEvent& event);
    void OnToggleTooltips(wxCommandEvent& event);
#endif // wxUSE_TOOLTIPS

#if wxUSE_CLIPBOARD
    void OnPasteFromClipboard( wxCommandEvent &event )
        { m_panel->DoPasteFromClipboard(); }
    void OnCopyToClipboard( wxCommandEvent &event )
        { m_panel->DoCopyToClipboard(); }
#endif // wxUSE_CLIPBOARD

    void OnMoveToEndOfText( wxCommandEvent &event )
        { m_panel->DoMoveToEndOfText(); }
    void OnMoveToEndOfEntry( wxCommandEvent &event )
        { m_panel->DoMoveToEndOfEntry(); }

    void OnFileLoad(wxCommandEvent& event);

    void OnIdle( wxIdleEvent& event );

private:
    MyPanel *m_panel;

    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------
// main()
//----------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

//----------------------------------------------------------------------
// MyApp
//----------------------------------------------------------------------

enum
{
    TEXT_QUIT = 100,
    TEXT_ABOUT,
    TEXT_LOAD,

    // clipboard menu
    TEXT_CLIPBOARD_COPY = 200,
    TEXT_CLIPBOARD_PASTE,

    // tooltip menu
    TEXT_TOOLTIPS_SETDELAY = 300,
    TEXT_TOOLTIPS_ENABLE,

    // move menu
    TEXT_MOVE_ENDTEXT = 400,
    TEXT_MOVE_ENDENTRY
};

bool MyApp::OnInit()
{
    // Create the main frame window
    MyFrame *frame = new MyFrame((wxFrame *) NULL,
            "Text wxWindows sample", 50, 50, 640, 420);
    frame->SetSizeHints( 500, 400 );

    wxMenu *file_menu = new wxMenu;
    file_menu->Append(TEXT_LOAD, "&Load file\tCtrl-O",
                      "Load the sample file into text control");
    file_menu->AppendSeparator();
    file_menu->Append(TEXT_ABOUT, "&About\tAlt-A");
    file_menu->AppendSeparator();
    file_menu->Append(TEXT_QUIT, "E&xit\tAlt-X", "Quit controls sample");

    wxMenuBar *menu_bar = new wxMenuBar( wxMB_DOCKABLE );
    menu_bar->Append(file_menu, "&File");

#if wxUSE_TOOLTIPS
    wxMenu *tooltip_menu = new wxMenu;
    tooltip_menu->Append(TEXT_TOOLTIPS_SETDELAY, "Set &delay\tCtrl-D");
    tooltip_menu->AppendSeparator();
    tooltip_menu->Append(TEXT_TOOLTIPS_ENABLE, "&Toggle tooltips\tCtrl-T",
            "enable/disable tooltips", TRUE);
    tooltip_menu->Check(TEXT_TOOLTIPS_ENABLE, TRUE);
    menu_bar->Append(tooltip_menu, "&Tooltips");
#endif // wxUSE_TOOLTIPS

#if wxUSE_CLIPBOARD
    wxMenu *menuClipboard = new wxMenu;
    menuClipboard->Append(TEXT_CLIPBOARD_COPY, "&Copy\tCtrl-C",
                          "Copy the first line to the clipboard");
    menuClipboard->Append(TEXT_CLIPBOARD_PASTE, "&Paste\tCtrl-V",
                          "Paste from clipboard to the text control");
    menu_bar->Append(menuClipboard, "&Clipboard");
#endif // wxUSE_CLIPBOARD

    wxMenu *menuMove = new wxMenu;
    menuMove->Append(TEXT_MOVE_ENDTEXT, "To the end of &text");
    menuMove->Append(TEXT_MOVE_ENDENTRY, "To the end of &entry");
    menu_bar->Append(menuMove, "&Move");

    frame->SetMenuBar(menu_bar);

    frame->Show(TRUE);

    SetTopWindow(frame);

    // report success
    return TRUE;
}

//----------------------------------------------------------------------
// MyTextCtrl
//----------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyTextCtrl, wxTextCtrl)
    EVT_KEY_DOWN(MyTextCtrl::OnKeyDown)
    EVT_KEY_UP(MyTextCtrl::OnKeyUp)
    EVT_CHAR(MyTextCtrl::OnChar)
END_EVENT_TABLE()

void MyTextCtrl::LogEvent(const wxChar *name, wxKeyEvent& event) const
{
    wxString key;
    long keycode = event.KeyCode();
    {
        switch ( keycode )
        {
            case WXK_BACK: key = "BACK"; break;
            case WXK_TAB: key = "TAB"; break;
            case WXK_RETURN: key = "RETURN"; break;
            case WXK_ESCAPE: key = "ESCAPE"; break;
            case WXK_SPACE: key = "SPACE"; break;
            case WXK_DELETE: key = "DELETE"; break;
            case WXK_START: key = "START"; break;
            case WXK_LBUTTON: key = "LBUTTON"; break;
            case WXK_RBUTTON: key = "RBUTTON"; break;
            case WXK_CANCEL: key = "CANCEL"; break;
            case WXK_MBUTTON: key = "MBUTTON"; break;
            case WXK_CLEAR: key = "CLEAR"; break;
            case WXK_SHIFT: key = "SHIFT"; break;
            case WXK_ALT: key = "ALT"; break;
            case WXK_CONTROL: key = "CONTROL"; break;
            case WXK_MENU: key = "MENU"; break;
            case WXK_PAUSE: key = "PAUSE"; break;
            case WXK_CAPITAL: key = "CAPITAL"; break;
            case WXK_PRIOR: key = "PRIOR"; break;
            case WXK_NEXT: key = "NEXT"; break;
            case WXK_END: key = "END"; break;
            case WXK_HOME: key = "HOME"; break;
            case WXK_LEFT: key = "LEFT"; break;
            case WXK_UP: key = "UP"; break;
            case WXK_RIGHT: key = "RIGHT"; break;
            case WXK_DOWN: key = "DOWN"; break;
            case WXK_SELECT: key = "SELECT"; break;
            case WXK_PRINT: key = "PRINT"; break;
            case WXK_EXECUTE: key = "EXECUTE"; break;
            case WXK_SNAPSHOT: key = "SNAPSHOT"; break;
            case WXK_INSERT: key = "INSERT"; break;
            case WXK_HELP: key = "HELP"; break;
            case WXK_NUMPAD0: key = "NUMPAD0"; break;
            case WXK_NUMPAD1: key = "NUMPAD1"; break;
            case WXK_NUMPAD2: key = "NUMPAD2"; break;
            case WXK_NUMPAD3: key = "NUMPAD3"; break;
            case WXK_NUMPAD4: key = "NUMPAD4"; break;
            case WXK_NUMPAD5: key = "NUMPAD5"; break;
            case WXK_NUMPAD6: key = "NUMPAD6"; break;
            case WXK_NUMPAD7: key = "NUMPAD7"; break;
            case WXK_NUMPAD8: key = "NUMPAD8"; break;
            case WXK_NUMPAD9: key = "NUMPAD9"; break;
            case WXK_MULTIPLY: key = "MULTIPLY"; break;
            case WXK_ADD: key = "ADD"; break;
            case WXK_SEPARATOR: key = "SEPARATOR"; break;
            case WXK_SUBTRACT: key = "SUBTRACT"; break;
            case WXK_DECIMAL: key = "DECIMAL"; break;
            case WXK_DIVIDE: key = "DIVIDE"; break;
            case WXK_F1: key = "F1"; break;
            case WXK_F2: key = "F2"; break;
            case WXK_F3: key = "F3"; break;
            case WXK_F4: key = "F4"; break;
            case WXK_F5: key = "F5"; break;
            case WXK_F6: key = "F6"; break;
            case WXK_F7: key = "F7"; break;
            case WXK_F8: key = "F8"; break;
            case WXK_F9: key = "F9"; break;
            case WXK_F10: key = "F10"; break;
            case WXK_F11: key = "F11"; break;
            case WXK_F12: key = "F12"; break;
            case WXK_F13: key = "F13"; break;
            case WXK_F14: key = "F14"; break;
            case WXK_F15: key = "F15"; break;
            case WXK_F16: key = "F16"; break;
            case WXK_F17: key = "F17"; break;
            case WXK_F18: key = "F18"; break;
            case WXK_F19: key = "F19"; break;
            case WXK_F20: key = "F20"; break;
            case WXK_F21: key = "F21"; break;
            case WXK_F22: key = "F22"; break;
            case WXK_F23: key = "F23"; break;
            case WXK_F24: key = "F24"; break;
            case WXK_NUMLOCK: key = "NUMLOCK"; break;
            case WXK_SCROLL: key = "SCROLL"; break;
            case WXK_PAGEUP: key = "PAGEUP"; break;
            case WXK_PAGEDOWN: key = "PAGEDOWN"; break;
            case WXK_NUMPAD_SPACE: key = "NUMPAD_SPACE"; break;
            case WXK_NUMPAD_TAB: key = "NUMPAD_TAB"; break;
            case WXK_NUMPAD_ENTER: key = "NUMPAD_ENTER"; break;
            case WXK_NUMPAD_F1: key = "NUMPAD_F1"; break;
            case WXK_NUMPAD_F2: key = "NUMPAD_F2"; break;
            case WXK_NUMPAD_F3: key = "NUMPAD_F3"; break;
            case WXK_NUMPAD_F4: key = "NUMPAD_F4"; break;
            case WXK_NUMPAD_HOME: key = "NUMPAD_HOME"; break;
            case WXK_NUMPAD_LEFT: key = "NUMPAD_LEFT"; break;
            case WXK_NUMPAD_UP: key = "NUMPAD_UP"; break;
            case WXK_NUMPAD_RIGHT: key = "NUMPAD_RIGHT"; break;
            case WXK_NUMPAD_DOWN: key = "NUMPAD_DOWN"; break;
            case WXK_NUMPAD_PRIOR: key = "NUMPAD_PRIOR"; break;
            case WXK_NUMPAD_PAGEUP: key = "NUMPAD_PAGEUP"; break;
            case WXK_NUMPAD_PAGEDOWN: key = "NUMPAD_PAGEDOWN"; break;
            case WXK_NUMPAD_END: key = "NUMPAD_END"; break;
            case WXK_NUMPAD_BEGIN: key = "NUMPAD_BEGIN"; break;
            case WXK_NUMPAD_INSERT: key = "NUMPAD_INSERT"; break;
            case WXK_NUMPAD_DELETE: key = "NUMPAD_DELETE"; break;
            case WXK_NUMPAD_EQUAL: key = "NUMPAD_EQUAL"; break;
            case WXK_NUMPAD_MULTIPLY: key = "NUMPAD_MULTIPLY"; break;
            case WXK_NUMPAD_ADD: key = "NUMPAD_ADD"; break;
            case WXK_NUMPAD_SEPARATOR: key = "NUMPAD_SEPARATOR"; break;
            case WXK_NUMPAD_SUBTRACT: key = "NUMPAD_SUBTRACT"; break;
            case WXK_NUMPAD_DECIMAL: key = "NUMPAD_DECIMAL"; break;

	    default:
            {
               if ( wxIsprint((int)keycode) )
                   key.Printf( _T("'%c'") , (char)keycode);
               else
		   key.Printf( _T("unknown (%ld)"), keycode);
	    }
        }
    }

    wxLogMessage( _T("%s event: %s (flags = %c%c%c%c)"),
                  name,
                  key.c_str(),
                  GetChar( event.ControlDown(), _T('C') ),
                  GetChar( event.AltDown(), _T('A') ),
                  GetChar( event.ShiftDown(), _T('S') ),
                  GetChar( event.MetaDown(), _T('M') ) );

}

void MyTextCtrl::OnChar(wxKeyEvent& event)
{
    LogEvent( _T("Char"), event);

/*  How are we supposed to test wxTE_PROCESS_TAB with this code?

    if ( event.KeyCode() == WXK_TAB )
    {
        WriteText("\t");
    }
    else
    {
        event.Skip();
    }
*/
    event.Skip();
}

void MyTextCtrl::OnKeyUp(wxKeyEvent& event)
{
    LogEvent( _("Key up"), event);

    event.Skip();
}

void MyTextCtrl::OnKeyDown(wxKeyEvent& event)
{
    switch ( event.KeyCode() )
    {
        case WXK_F1:
            // show current position and text length
            {
                long line, column, pos = GetInsertionPoint();
                PositionToXY(pos, &column, &line);

                wxLogMessage( _T("Current position: %ld\n"
                        "Current line, column: (%ld, %ld)\n"
                        "Number of lines: %ld\n"
                        "Current line length: %ld\n"
                        "Total text length: %ld"),
                        pos,
                        line, column,
                        GetNumberOfLines(),
                        GetLineLength(line),
                        GetLastPosition());
            }
            break;

        case WXK_F2:
            // go to the end
            SetInsertionPointEnd();
            break;

        case WXK_F3:
            // go to position 10
            SetInsertionPoint(10);
            break;
    }

    LogEvent( _("Key down"), event);

    event.Skip();
}

//----------------------------------------------------------------------
// MyPanel
//----------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
    EVT_SIZE(MyPanel::OnSize)
END_EVENT_TABLE()

MyPanel::MyPanel( wxFrame *frame, int x, int y, int w, int h )
       : wxPanel( frame, -1, wxPoint(x, y), wxSize(w, h) )
{
    m_log = new wxTextCtrl( this, -1, "This is the log window.\n", wxPoint(5,260), wxSize(630,100), wxTE_MULTILINE );

    wxLog *old_log = wxLog::SetActiveTarget( new wxLogTextCtrl( m_log ) );

    delete old_log;

    // single line text controls

    m_text = new MyTextCtrl( this, -1, "Single line.",
      wxPoint(10,10), wxSize(140,-1), 0);
    (*m_text) << " Appended.";
    m_text->SetInsertionPoint(0);
    m_text->WriteText( "Prepended. " );

    m_password = new MyTextCtrl( this, -1, "",
      wxPoint(10,50), wxSize(140,-1), wxTE_PASSWORD );

    m_readonly = new MyTextCtrl( this, -1, "Read only",
      wxPoint(10,90), wxSize(140,-1), wxTE_READONLY );

    // multi line text controls

    m_horizontal = new MyTextCtrl( this, -1, "Multiline text control with a horizontal scrollbar.",
      wxPoint(10,170), wxSize(140,70), wxTE_MULTILINE | wxHSCROLL );
    m_horizontal->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxNORMAL,
                                 FALSE, "", wxFONTENCODING_KOI8));

    m_multitext = new MyTextCtrl( this, -1, "Multi line.",
      wxPoint(180,10), wxSize(240,70), wxTE_MULTILINE );
    (*m_multitext) << " Appended.";
    m_multitext->SetInsertionPoint(0);
    m_multitext->WriteText( "Prepended. " );

#if wxUSE_TOOLTIPS
    m_multitext->SetToolTip("Press F1 here.");
#endif

    m_tab = new MyTextCtrl( this, -1, "Multiline, allow <TAB> processing.",
      wxPoint(180,90), wxSize(240,70), wxTE_MULTILINE |  wxTE_PROCESS_TAB );

    m_enter = new MyTextCtrl( this, -1, "Multiline, allow <ENTER> processing.",
      wxPoint(180,170), wxSize(240,70), wxTE_MULTILINE);
}

void MyPanel::OnSize( wxSizeEvent &event )
{
    wxSize client_area( GetClientSize() );
    m_log->SetSize( 0, 260, client_area.x, client_area.y - 260 );
    event.Skip();
}

#if wxUSE_CLIPBOARD
void MyPanel::DoPasteFromClipboard()
{
    // On X11, we want to get the data from the primary selection instead
    // of the normal clipboard (which isn't normal under X11 at all). This
    // call has no effect under MSW.
    wxTheClipboard->UsePrimarySelection();

    if (!wxTheClipboard->Open())
    {
        *m_log << "Error opening the clipboard.\n";
        return;
    }
    else
    {
        *m_log << "Successfully opened the clipboard.\n";
    }

    wxTextDataObject data;

    if (wxTheClipboard->IsSupported( data.GetFormat() ))
    {
        *m_log << "Clipboard supports requested format.\n";

        if (wxTheClipboard->GetData( &data ))
        {
            *m_log << "Successfully retrieved data from the clipboard.\n";
            *m_multitext << data.GetText() << "\n";
        }
        else
        {
            *m_log << "Error getting data from the clipboard.\n";
        }
    }
    else
    {
        *m_log << "Clipboard doesn't support requested format.\n";
    }

    wxTheClipboard->Close();

    *m_log << "Closed the clipboard.\n";
}

void MyPanel::DoCopyToClipboard()
{
    wxString text( m_multitext->GetLineText(0) );

    if (text.IsEmpty())
    {
        *m_log << "No text to copy.\n";

        return;
    }

    if (!wxTheClipboard->Open())
    {
        *m_log << "Error opening the clipboard.\n";

        return;
    }
    else
    {
        *m_log << "Successfully opened the clipboard.\n";
    }

    wxTextDataObject *data = new wxTextDataObject( text );

    if (!wxTheClipboard->SetData( data ))
    {
        *m_log << "Error while copying to the clipboard.\n";
    }
    else
    {
        *m_log << "Successfully copied data to the clipboard.\n";
    }

    wxTheClipboard->Close();

    *m_log << "Closed the clipboard.\n";
}

#endif // wxUSE_CLIPBOARD

void MyPanel::DoMoveToEndOfText()
{
    m_multitext->SetInsertionPointEnd();
    m_multitext->SetFocus();
}

void MyPanel::DoMoveToEndOfEntry()
{
    m_text->SetInsertionPointEnd();
    m_text->SetFocus();
}

//----------------------------------------------------------------------
// MyFrame
//----------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(TEXT_QUIT,   MyFrame::OnQuit)
    EVT_MENU(TEXT_ABOUT,  MyFrame::OnAbout)
    EVT_MENU(TEXT_LOAD,   MyFrame::OnFileLoad)

#if wxUSE_TOOLTIPS
    EVT_MENU(TEXT_TOOLTIPS_SETDELAY,  MyFrame::OnSetTooltipDelay)
    EVT_MENU(TEXT_TOOLTIPS_ENABLE,    MyFrame::OnToggleTooltips)
#endif // wxUSE_TOOLTIPS

#if wxUSE_CLIPBOARD
    EVT_MENU(TEXT_CLIPBOARD_PASTE,    MyFrame::OnPasteFromClipboard)
    EVT_MENU(TEXT_CLIPBOARD_COPY,     MyFrame::OnCopyToClipboard)
#endif // wxUSE_CLIPBOARD

    EVT_MENU(TEXT_MOVE_ENDTEXT,       MyFrame::OnMoveToEndOfText)
    EVT_MENU(TEXT_MOVE_ENDENTRY,      MyFrame::OnMoveToEndOfEntry)

    EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame, const char *title, int x, int y, int w, int h)
       : wxFrame(frame, -1, title, wxPoint(x, y), wxSize(w, h) )
{
    CreateStatusBar(2);

    m_panel = new MyPanel( this, 10, 10, 300, 100 );
}

void MyFrame::OnQuit (wxCommandEvent& WXUNUSED(event) )
{
    Close(TRUE);
}

void MyFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
    wxBeginBusyCursor();

    wxMessageDialog dialog(this,
        "This is a text control sample. It demonstrates the many different\n"
        "text control styles, the use of the clipboard, setting and handling\n"
        "tooltips and intercepting key and char events.\n"
        "\n"
        "Copyright (c) 1999, Robert Roebling, Julian Smart, Vadim Zeitlin",
        "About Text Controls",
        wxOK | wxICON_INFORMATION);

    dialog.ShowModal();

    wxEndBusyCursor();
}

#if wxUSE_TOOLTIPS
void MyFrame::OnSetTooltipDelay(wxCommandEvent& event)
{
    static long s_delay = 5000;

    wxString delay;
    delay.Printf( _T("%ld"), s_delay);

    delay = wxGetTextFromUser("Enter delay (in milliseconds)",
                              "Set tooltip delay",
                              delay,
                              this);
    if ( !delay )
        return; // cancelled

    wxSscanf(delay, _T("%ld"), &s_delay);

    wxToolTip::SetDelay(s_delay);

    wxLogStatus(this, _T("Tooltip delay set to %ld milliseconds"), s_delay);
}

void MyFrame::OnToggleTooltips(wxCommandEvent& event)
{
    static bool s_enabled = TRUE;

    s_enabled = !s_enabled;

    wxToolTip::Enable(s_enabled);

    wxLogStatus(this, _T("Tooltips %sabled"), s_enabled ? _T("en") : _T("dis") );
}
#endif // tooltips

void MyFrame::OnFileLoad(wxCommandEvent& event)
{
    if ( m_panel->m_multitext->LoadFile("text.rc") )
        wxLogStatus(this, _T("Successfully loaded file"));
    else
        wxLogStatus(this, _T("Couldn't load the file"));
}

void MyFrame::OnIdle( wxIdleEvent& event )
{
    // track the window which has the focus in the status bar
    static wxWindow *s_windowFocus = (wxWindow *)NULL;
    wxWindow *focus = wxWindow::FindFocus();
    if ( focus && (focus != s_windowFocus) )
    {
        s_windowFocus = focus;

        wxString msg;
        msg.Printf(
#ifdef __WXMSW__
                _T("Focus: wxWindow = %p, HWND = %08x"),
#else
                _T("Focus: wxWindow = %p"),
#endif
                s_windowFocus
#ifdef __WXMSW__
                , s_windowFocus->GetHWND()
#endif
                  );

        SetStatusText(msg);
    }
    event.Skip();
}
