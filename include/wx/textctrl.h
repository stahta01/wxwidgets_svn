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

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "textctrlbase.h"
#endif

#include "wx/defs.h"

#if wxUSE_TEXTCTRL

#include "wx/control.h"         // the base class
#include "wx/dynarray.h"

// 16-bit Borland 4.0 doesn't seem to allow multiple inheritance with wxWindow
// and streambuf: it complains about deriving a huge class from the huge class
// streambuf. !! Also, can't use streambuf if making or using a DLL :-(

#if (defined(__BORLANDC__)) || defined(__MWERKS__) || \
    defined(WXUSINGDLL) || defined(WXMAKINGDLL)
    #define NO_TEXT_WINDOW_STREAM
#endif

#ifndef NO_TEXT_WINDOW_STREAM
    #if wxUSE_STD_IOSTREAM
        #include "wx/ioswrap.h"    // derivation: we need the full decls.
    #else // !wxUSE_STD_IOSTREAM
        // can't compile this feature in if we don't use streams at all
        #define NO_TEXT_WINDOW_STREAM
    #endif // wxUSE_STD_IOSTREAM/!wxUSE_STD_IOSTREAM
#endif

class WXDLLEXPORT wxTextCtrl;
class WXDLLEXPORT wxTextCtrlBase;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

WXDLLEXPORT_DATA(extern const wxChar*) wxTextCtrlNameStr;

// ----------------------------------------------------------------------------
// wxTextCtrl style flags
// ----------------------------------------------------------------------------

// the flag bits 0x0001, and 0x0004 are free but should be used only for the
// things which don't make sense for a text control used by wxTextEntryDialog
// because they would otherwise conflict with wxOK, wxCANCEL, wxCENTRE

#define wxTE_NO_VSCROLL     0x0002
#define wxTE_AUTO_SCROLL    0x0008

#define wxTE_READONLY       0x0010
#define wxTE_MULTILINE      0x0020
#define wxTE_PROCESS_TAB    0x0040

// alignment flags
#define wxTE_LEFT           0x0000                    // 0x0000
#define wxTE_CENTER         wxALIGN_CENTER_HORIZONTAL // 0x0100
#define wxTE_RIGHT          wxALIGN_RIGHT             // 0x0200
#define wxTE_CENTRE         wxTE_CENTER

// this style means to use RICHEDIT control and does something only under wxMSW
// and Win32 and is silently ignored under all other platforms
#define wxTE_RICH           0x0080

#define wxTE_PROCESS_ENTER  0x0400
#define wxTE_PASSWORD       0x0800

// automatically detect the URLs and generate the events when mouse is
// moved/clicked over an URL
//
// this is for Win32 richedit controls only so far
#define wxTE_AUTO_URL       0x1000

// by default, the Windows text control doesn't show the selection when it
// doesn't have focus - use this style to force it to always show it
#define wxTE_NOHIDESEL      0x2000

// use wxHSCROLL to not wrap text at all, wxTE_LINEWRAP to wrap it at any
// position and wxTE_WORDWRAP to wrap at words boundary
#define wxTE_DONTWRAP       wxHSCROLL
#define wxTE_LINEWRAP       0x4000
#define wxTE_WORDWRAP       0x0000  // it's just == !wxHSCROLL

// force using RichEdit version 2.0 or 3.0 instead of 1.0 (default) for
// wxTE_RICH controls - can be used together with or instead of wxTE_RICH
#define wxTE_RICH2          0x8000

// ----------------------------------------------------------------------------
// Types for wxTextAttr
// ----------------------------------------------------------------------------

// Alignment

enum wxTextAttrAlignment
{
    wxTEXT_ALIGNMENT_DEFAULT,
    wxTEXT_ALIGNMENT_LEFT,
    wxTEXT_ALIGNMENT_CENTRE,
    wxTEXT_ALIGNMENT_CENTER = wxTEXT_ALIGNMENT_CENTRE,
    wxTEXT_ALIGNMENT_RIGHT,
    wxTEXT_ALIGNMENT_JUSTIFIED
};

// Flags to indicate which attributes are being applied

#define wxTEXT_ATTR_TEXT_COLOUR             0x0001
#define wxTEXT_ATTR_BACKGROUND_COLOUR       0x0002
#define wxTEXT_ATTR_FONT_FACE               0x0004
#define wxTEXT_ATTR_FONT_SIZE               0x0008
#define wxTEXT_ATTR_FONT_WEIGHT             0x0010
#define wxTEXT_ATTR_FONT_ITALIC             0x0020
#define wxTEXT_ATTR_FONT_UNDERLINE          0x0040
#define wxTEXT_ATTR_FONT \
  wxTEXT_ATTR_FONT_FACE | wxTEXT_ATTR_FONT_SIZE | wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_ITALIC | wxTEXT_ATTR_FONT_UNDERLINE
#define wxTEXT_ATTR_ALIGNMENT               0x0080
#define wxTEXT_ATTR_LEFT_INDENT             0x0100
#define wxTEXT_ATTR_RIGHT_INDENT            0x0200
#define wxTEXT_ATTR_TABS                    0x0400

// ----------------------------------------------------------------------------
// wxTextAttr: a structure containing the visual attributes of a text
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTextAttr
{
public:
    // ctors
    wxTextAttr() { Init(); }
    wxTextAttr(const wxColour& colText,
               const wxColour& colBack = wxNullColour,
               const wxFont& font = wxNullFont,
               wxTextAttrAlignment alignment = wxTEXT_ALIGNMENT_DEFAULT);

    // operations
    void Init();

    // operators
    void operator= (const wxTextAttr& attr);

    // setters
    void SetTextColour(const wxColour& colText) { m_colText = colText; m_flags |= wxTEXT_ATTR_TEXT_COLOUR; }
    void SetBackgroundColour(const wxColour& colBack) { m_colBack = colBack; m_flags |= wxTEXT_ATTR_BACKGROUND_COLOUR; }
    void SetFont(const wxFont& font, long flags = wxTEXT_ATTR_FONT) { m_font = font; m_flags |= flags; }
    void SetAlignment(wxTextAttrAlignment alignment) { m_textAlignment = alignment; m_flags |= wxTEXT_ATTR_ALIGNMENT; }
    void SetTabs(const wxArrayInt& tabs) { m_tabs = tabs; m_flags |= wxTEXT_ATTR_TABS; }
    void SetLeftIndent(int indent) { m_leftIndent = indent; m_flags |= wxTEXT_ATTR_LEFT_INDENT; }
    void SetRightIndent(int indent) { m_rightIndent = indent; m_flags |= wxTEXT_ATTR_RIGHT_INDENT; }
    void SetFlags(long flags) { m_flags = flags; }

    // accessors
    bool HasTextColour() const { return m_colText.Ok() && HasFlag(wxTEXT_ATTR_TEXT_COLOUR) ; }
    bool HasBackgroundColour() const { return m_colBack.Ok() && HasFlag(wxTEXT_ATTR_BACKGROUND_COLOUR) ; }
    bool HasFont() const { return m_font.Ok() && HasFlag(wxTEXT_ATTR_FONT) ; }
    bool HasAlignment() const { return (m_textAlignment != wxTEXT_ALIGNMENT_DEFAULT) || ((m_flags & wxTEXT_ATTR_ALIGNMENT) != 0) ; }
    bool HasTabs() const { return (m_flags & wxTEXT_ATTR_TABS) != 0 ; }
    bool HasLeftIndent() const { return (m_flags & wxTEXT_ATTR_LEFT_INDENT) != 0 ; }
    bool HasRightIndent() const { return (m_flags & wxTEXT_ATTR_RIGHT_INDENT) != 0 ; }
    bool HasFlag(long flag) const { return (m_flags & flag) != 0; }

    const wxColour& GetTextColour() const { return m_colText; }
    const wxColour& GetBackgroundColour() const { return m_colBack; }
    const wxFont& GetFont() const { return m_font; }
    wxTextAttrAlignment GetAlignment() const { return m_textAlignment; }
    const wxArrayInt& GetTabs() const { return m_tabs; }
    long GetLeftIndent() const { return m_leftIndent; }
    long GetRightIndent() const { return m_rightIndent; }
    long GetFlags() const { return m_flags; }

    // returns false if we have any attributes set, true otherwise
    bool IsDefault() const
    {
        return !HasTextColour() && !HasBackgroundColour() && !HasFont() && !HasAlignment() &&
               !HasTabs() && !HasLeftIndent() && !HasRightIndent() ;
    }

    // return the attribute having the valid font and colours: it uses the
    // attributes set in attr and falls back first to attrDefault and then to
    // the text control font/colours for those attributes which are not set
    static wxTextAttr Combine(const wxTextAttr& attr,
                              const wxTextAttr& attrDef,
                              const wxTextCtrlBase *text);

private:
    long                m_flags;
    wxColour            m_colText,
                        m_colBack;
    wxFont              m_font;
    wxTextAttrAlignment m_textAlignment;
    wxArrayInt          m_tabs; // array of int: tab stops in 1/10 mm
    int                 m_leftIndent; // left indent in 1/10 mm
    int                 m_rightIndent; // right indent in 1/10 mm
};

// ----------------------------------------------------------------------------
// wxTextCtrl: a single or multiple line text zone where user can enter and
// edit text
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTextCtrlBase : public wxControl
#ifndef NO_TEXT_WINDOW_STREAM
                                 , public wxSTD streambuf
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

    virtual wxString GetRange(long from, long to) const;

    virtual int GetLineLength(long lineNo) const = 0;
    virtual wxString GetLineText(long lineNo) const = 0;
    virtual int GetNumberOfLines() const = 0;

    virtual bool IsModified() const = 0;
    virtual bool IsEditable() const = 0;

    // more readable flag testing methods
    bool IsSingleLine() const { return !(GetWindowStyle() & wxTE_MULTILINE); }
    bool IsMultiLine() const { return !IsSingleLine(); }

    // If the return values from and to are the same, there is no selection.
    virtual void GetSelection(long* from, long* to) const = 0;

    virtual wxString GetStringSelection() const;

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

    // set the max number of characters which may be entered in a single line
    // text control
    virtual void SetMaxLength(unsigned long WXUNUSED(len)) { }

    // writing text inserts it at the current position, appending always
    // inserts it at the end
    virtual void WriteText(const wxString& text) = 0;
    virtual void AppendText(const wxString& text) = 0;

    // insert the character which would have resulted from this key event,
    // return TRUE if anything has been inserted
    virtual bool EmulateKeyPress(const wxKeyEvent& event);

    // text control under some platforms supports the text styles: these
    // methods allow to apply the given text style to the given selection or to
    // set/get the style which will be used for all appended text
    virtual bool SetStyle(long start, long end, const wxTextAttr& style);
    virtual bool GetStyle(long position, wxTextAttr& style);
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

    // override streambuf method
#ifndef NO_TEXT_WINDOW_STREAM
    int overflow(int i);
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
};

// ----------------------------------------------------------------------------
// include the platform-dependent class definition
// ----------------------------------------------------------------------------

#if defined(__WXX11__)
    #include "wx/x11/textctrl.h"
#elif defined(__WXUNIVERSAL__)
    #include "wx/univ/textctrl.h"
#elif defined(__WXMSW__)
    #include "wx/msw/textctrl.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/textctrl.h"
#elif defined(__WXGTK__)
    #include "wx/gtk/textctrl.h"
#elif defined(__WXMAC__)
    #include "wx/mac/textctrl.h"
#elif defined(__WXCOCOA__)
    #include "wx/cocoa/textctrl.h"
#elif defined(__WXPM__)
    #include "wx/os2/textctrl.h"
#endif

// ----------------------------------------------------------------------------
// wxTextCtrl events
// ----------------------------------------------------------------------------

#if !WXWIN_COMPATIBILITY_EVENT_TYPES

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_UPDATED, 7)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_ENTER, 8)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_URL, 13)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_TEXT_MAXLEN, 14)
END_DECLARE_EVENT_TYPES()

#endif // !WXWIN_COMPATIBILITY_EVENT_TYPES

class WXDLLEXPORT wxTextUrlEvent : public wxCommandEvent
{
public:
    wxTextUrlEvent(int winid, const wxMouseEvent& evtMouse,
                   long start, long end)
        : wxCommandEvent(wxEVT_COMMAND_TEXT_URL, winid)
        , m_evtMouse(evtMouse), m_start(start), m_end(end)
        { }

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
    wxTextUrlEvent() : m_evtMouse(), m_start(0), m_end(0) { }
};

typedef void (wxEvtHandler::*wxTextUrlEventFunction)(wxTextUrlEvent&);

#define EVT_TEXT(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_UPDATED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL ),
#define EVT_TEXT_ENTER(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_ENTER, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL ),
#define EVT_TEXT_URL(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_URL, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxTextUrlEventFunction) & fn, (wxObject *) NULL ),
#define EVT_TEXT_MAXLEN(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_TEXT_MAXLEN, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL ),

#ifndef NO_TEXT_WINDOW_STREAM

// ----------------------------------------------------------------------------
// wxStreamToTextRedirector: this class redirects all data sent to the given
// C++ stream to the wxTextCtrl given to its ctor during its lifetime.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxStreamToTextRedirector
{
public:
    wxStreamToTextRedirector(wxTextCtrl *text, wxSTD ostream *ostr = NULL)
        : m_ostr(ostr ? *ostr : wxSTD cout)
    {
        m_sbufOld = m_ostr.rdbuf();
        m_ostr.rdbuf(text);
    }

    ~wxStreamToTextRedirector()
    {
        m_ostr.rdbuf(m_sbufOld);
    }

private:
    // the stream we're redirecting
    wxSTD ostream&   m_ostr;

    // the old streambuf (before we changed it)
    wxSTD streambuf *m_sbufOld;
};

#endif // !NO_TEXT_WINDOW_STREAM

#endif // wxUSE_TEXTCTRL

#endif
    // _WX_TEXTCTRL_H_BASE_
