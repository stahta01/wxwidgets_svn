///////////////////////////////////////////////////////////////////////////////
// Name:        textctrl.h
// Purpose:     wxTextCtrlBase class - the interface of wxTextCtrl
// Author:      Vadim Zeitlin
// Modified by:
// Created:     13.07.99
// RCS-ID:      $Id$
// Copyright:   (c) wxWindows team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TEXTCTRL_H_BASE_
#define _WX_TEXTCTRL_H_BASE_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma interface "textctrlbase.h"
#endif

#include "wx/defs.h"

#if wxUSE_TEXTCTRL

#include "wx/control.h"         // the base class

// 16-bit Borland 4.0 doesn't seem to allow multiple inheritance with wxWindow
// and streambuf: it complains about deriving a huge class from the huge class
// streambuf. !! Also, can't use streambuf if making or using a DLL :-(

#if (defined(__BORLANDC__)) || defined(__MWERKS__) || defined(_WINDLL) || defined(WXUSINGDLL) || defined(WXMAKINGDLL)
    #define NO_TEXT_WINDOW_STREAM
#endif

// the streambuf which is used in the declaration of wxTextCtrlBase below is not compatible
// with the standard-conforming implementation found in newer egcs versions
// (that is, the libstdc++ v3 that is shipped with it)
#if defined(__GNUC__)&&( (__GNUC__>2) ||( (__GNUC__==2)&&(__GNUC_MINOR__>97) ) )
    #define NO_TEXT_WINDOW_STREAM
#endif

#ifndef NO_TEXT_WINDOW_STREAM
    #if wxUSE_STD_IOSTREAM
        #include "wx/ioswrap.h"    // for iostream classes if we need them
    #else // !wxUSE_STD_IOSTREAM
        // can't compile this feature in if we don't use streams at all
        #define NO_TEXT_WINDOW_STREAM
    #endif // wxUSE_STD_IOSTREAM/!wxUSE_STD_IOSTREAM
#endif

class WXDLLEXPORT wxTextCtrl;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(extern const wxChar*) wxTextCtrlNameStr;
WXDLLEXPORT_DATA(extern const wxChar*) wxEmptyString;

// ----------------------------------------------------------------------------
// wxTextCtrl style flags
// ----------------------------------------------------------------------------

// the flag bits 0x0001, 2, 4 and 8 are free but should be used only for the
// things which don't make sense for a text control used by wxTextEntryDialog
// because they would otherwise conflict with wxOK, wxCANCEL, wxCENTRE
#define wxTE_READONLY       0x0010
#define wxTE_MULTILINE      0x0020
#define wxTE_PROCESS_TAB    0x0040

// this style means to use RICHEDIT control and does something only under wxMSW
// and Win32 and is silently ignored under all other platforms
#define wxTE_RICH           0x0080
#define wxTE_NO_VSCROLL     0x0100
#define wxTE_AUTO_SCROLL    0x0200
#define wxTE_PROCESS_ENTER  0x0400
#define wxTE_PASSWORD       0x0800

// automatically detect the URLs and generate the events when mouse is
// moved/clicked over an URL
//
// this is for Win32 richedit controls only so far
#define wxTE_AUTO_URL       0x1000

// ----------------------------------------------------------------------------
// wxTextAttr: a structure containing the visual attributes of a text
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTextAttr
{
public:
    // ctors
    wxTextAttr() { }
    wxTextAttr(const wxColour& colText,
               const wxColour& colBack = wxNullColour,
               const wxFont& font = wxNullFont)
        : m_colText(colText), m_colBack(colBack), m_font(font) { }

    // setters
    void SetTextColour(const wxColour& colText) { m_colText = colText; }
    void SetBackgroundColour(const wxColour& colBack) { m_colBack = colBack; }
    void SetFont(const wxFont& font) { m_font = font; }

    // accessors
    bool HasTextColour() const { return m_colText.Ok(); }
    bool HasBackgroundColour() const { return m_colBack.Ok(); }
    bool HasFont() const { return m_font.Ok(); }

    // setters
    const wxColour& GetTextColour() const { return m_colText; }
    const wxColour& GetBackgroundColour() const { return m_colBack; }
    const wxFont& GetFont() const { return m_font; }

    // returns false if we have any attributes set, true otherwise
    bool IsDefault() const
    {
        return !HasTextColour() && !HasBackgroundColour() && !HasFont();
    }

private:
    wxColour m_colText,
             m_colBack;
    wxFont   m_font;
};

// ----------------------------------------------------------------------------
// wxTextCtrl: a single or multiple line text zone where user can enter and
// edit text
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTextCtrlBase : public wxControl
#ifndef NO_TEXT_WINDOW_STREAM
                                 , public streambuf
#endif

{
public:
    // creation
    // --------

    wxTextCtrlBase();
    ~wxTextCtrlBase();

    // accessors
    // ---------

    virtual wxString GetValue() const = 0;
    virtual void SetValue(const wxString& value) = 0;

    virtual int GetLineLength(long lineNo) const = 0;
    virtual wxString GetLineText(long lineNo) const = 0;
    virtual int GetNumberOfLines() const = 0;

    virtual bool IsModified() const = 0;
    virtual bool IsEditable() const = 0;

    // If the return values from and to are the same, there is no selection.
    virtual void GetSelection(long* from, long* to) const = 0;

    // operations
    // ----------

    // editing
    virtual void Clear() = 0;
    virtual void Replace(long from, long to, const wxString& value) = 0;
    virtual void Remove(long from, long to) = 0;

    // load/save the controls contents from/to the file
    virtual bool LoadFile(const wxString& file);
    virtual bool SaveFile(const wxString& file = wxEmptyString);

    // clears the dirty flag
    virtual void DiscardEdits() = 0;

    // writing text inserts it at the current position, appending always
    // inserts it at the end
    virtual void WriteText(const wxString& text) = 0;
    virtual void AppendText(const wxString& text) = 0;

    // text control under some platforms supports the text styles: these
    // methods allow to apply the given text style to the given selection or to
    // set/get the style which will be used for all appended text
    virtual bool SetStyle(long start, long end, const wxTextAttr& style);
    virtual bool SetDefaultStyle(const wxTextAttr& style);
    virtual const wxTextAttr& GetDefaultStyle() const;

    // translate between the position (which is just an index in the text ctrl
    // considering all its contents as a single strings) and (x, y) coordinates
    // which represent column and line.
    virtual long XYToPosition(long x, long y) const = 0;
    virtual bool PositionToXY(long pos, long *x, long *y) const = 0;

    virtual void ShowPosition(long pos) = 0;

    // Clipboard operations
    virtual void Copy() = 0;
    virtual void Cut() = 0;
    virtual void Paste() = 0;

    virtual bool CanCopy() const;
    virtual bool CanCut() const;
    virtual bool CanPaste() const;

    // Undo/redo
    virtual void Undo() = 0;
    virtual void Redo() = 0;

    virtual bool CanUndo() const = 0;
    virtual bool CanRedo() const = 0;

    // Insertion point
    virtual void SetInsertionPoint(long pos) = 0;
    virtual void SetInsertionPointEnd() = 0;
    virtual long GetInsertionPoint() const = 0;
    virtual long GetLastPosition() const = 0;

    virtual void SetSelection(long from, long to) = 0;
    virtual void SelectAll();
    virtual void SetEditable(bool editable) = 0;

    // streambuf methods
#ifndef NO_TEXT_WINDOW_STREAM
    int overflow(int i);
    int sync();
    int underflow();
#endif // NO_TEXT_WINDOW_STREAM

    // stream-like insertion operators: these are always available, whether we
    // were, or not, compiled with streambuf support
    wxTextCtrl& operator<<(const wxString& s);
    wxTextCtrl& operator<<(int i);
    wxTextCtrl& operator<<(long i);
    wxTextCtrl& operator<<(float f);
    wxTextCtrl& operator<<(double d);
    wxTextCtrl& operator<<(const wxChar c);

    // obsolete functions
#if WXWIN_COMPATIBILITY
    bool Modified() const { return IsModified(); }
#endif

protected:
    // the name of the last file loaded with LoadFile() which will be used by
    // SaveFile() by default
    wxString m_filename;

    // the text style which will be used for any new text added to the control
    wxTextAttr m_defaultStyle;

private:
#ifndef NO_TEXT_WINDOW_STREAM
#if !wxUSE_IOSTREAMH
    char *m_streambuf;
#endif
#endif
};

// ----------------------------------------------------------------------------
// include the platform-dependent class definition
// ----------------------------------------------------------------------------

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/textctrl.h"
#elif defined(__WXMSW__)
    #include "wx/msw/textctrl.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/textctrl.h"
#elif defined(__WXGTK__)
    #include "wx/gtk/textctrl.h"
#elif defined(__WXQT__)
    #include "wx/qt/textctrl.h"
#elif defined(__WXMAC__)
    #include "wx/mac/textctrl.h"
#elif defined(__WXPM__)
    #include "wx/os2/textctrl.h"
#elif defined(__WXSTUBS__)
    #include "wx/stubs/textctrl.h"
#endif

// ----------------------------------------------------------------------------
// wxTextCtrl events
// ----------------------------------------------------------------------------

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_UPDATED, 7)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_ENTER, 8)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_URL, 13)
END_DECLARE_EVENT_TYPES()

class WXDLLEXPORT wxTextUrlEvent : public wxCommandEvent
{
public:
    wxTextUrlEvent(int id, const wxMouseEvent& evtMouse,
                   long start, long end)
        : wxCommandEvent(wxEVT_COMMAND_TEXT_URL, id),
          m_evtMouse(evtMouse)
        { m_start = start; m_end = end; }

    // get the mouse event which happend over the URL
    const wxMouseEvent& GetMouseEvent() const { return m_evtMouse; }

    // get the start of the URL
    long GetURLStart() const { return m_start; }

    // get the end of the URL
    long GetURLEnd() const { return m_end; }

protected:
    // the corresponding mouse event
    wxMouseEvent m_evtMouse;

    // the start and end indices of the URL in the text control
    long m_start,
         m_end;

private:
    DECLARE_DYNAMIC_CLASS(wxTextUrlEvent)

public:
    // for wxWin RTTI only, don't use
    wxTextUrlEvent() { }
};

typedef void (wxEvtHandler::*wxTextUrlEventFunction)(wxTextUrlEvent&);

#define EVT_TEXT(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_UPDATED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL ),
#define EVT_TEXT_ENTER(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_ENTER, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL ),
#define EVT_TEXT_URL(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_URL, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxTextUrlEventFunction) & fn, (wxObject *) NULL ),

#endif // wxUSE_TEXTCTRL

#endif
    // _WX_TEXTCTRL_H_BASE_
