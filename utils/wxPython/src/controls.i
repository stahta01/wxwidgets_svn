/////////////////////////////////////////////////////////////////////////////
// Name:        controls.i
// Purpose:     Control (widget) classes for wxPython
//
// Author:      Robin Dunn
//
// Created:     6/10/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%module controls

%{
#include "helpers.h"
#include <wx/slider.h>
#include <wx/spinbutt.h>
#include <wx/dynarray.h>
#include <wx/statline.h>

#ifdef __WXMSW__
#if wxUSE_OWNER_DRAWN
#include <wx/checklst.h>
#endif
#endif

#ifdef __WXGTK__
#include <wx/checklst.h>
#endif

%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i
%import windows.i
%import gdi.i
%import events.i

%pragma(python) code = "import wx"

//----------------------------------------------------------------------

%{
wxValidator wxPyDefaultValidator;       // Non-const default because of SWIG
%}

//----------------------------------------------------------------------

class wxControl : public wxWindow {
public:
#ifdef __WXMSW__
    void Command(wxCommandEvent& event);
#endif
    wxString GetLabel();
    void SetLabel(const wxString& label);
};

//----------------------------------------------------------------------

class wxButton : public wxControl {
public:
    wxButton(wxWindow* parent, wxWindowID id, const wxString& label,
             const wxPoint& pos = wxPyDefaultPosition,
             const wxSize& size = wxPyDefaultSize,
             long style = 0,
             const wxValidator& validator = wxPyDefaultValidator,
             char* name = "button");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void SetDefault();
};

//----------------------------------------------------------------------

class wxBitmapButton : public wxButton {
public:
    wxBitmapButton(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap,
                   const wxPoint& pos = wxPyDefaultPosition,
                   const wxSize& size = wxPyDefaultSize,
                   long style = wxBU_AUTODRAW,
                   const wxValidator& validator = wxPyDefaultValidator,
                   char* name = "button");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    wxBitmap& GetBitmapLabel();
    wxBitmap& GetBitmapDisabled();
    wxBitmap& GetBitmapFocus();
    wxBitmap& GetBitmapSelected();
    void SetBitmapDisabled(const wxBitmap& bitmap);
    void SetBitmapFocus(const wxBitmap& bitmap);
    void SetBitmapSelected(const wxBitmap& bitmap);
    void SetBitmapLabel(const wxBitmap& bitmap);

};

//----------------------------------------------------------------------

class wxCheckBox : public wxControl {
public:
    wxCheckBox(wxWindow* parent, wxWindowID id, const wxString& label,
               const wxPoint& pos = wxPyDefaultPosition,
               const wxSize& size = wxPyDefaultSize,
               long style = 0,
               const wxValidator& val = wxPyDefaultValidator,
               char* name = "checkBox");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    bool GetValue();
    void SetValue(const bool state);
};

//----------------------------------------------------------------------

class wxChoice : public wxControl {
public:
    wxChoice(wxWindow *parent, wxWindowID id,
             const wxPoint& pos = wxPyDefaultPosition,
             const wxSize& size = wxPyDefaultSize,
             int LCOUNT=0, wxString* LIST=NULL,
             long style = 0,
             const wxValidator& validator = wxPyDefaultValidator,
             char* name = "choice");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void Append(const wxString& item);
    void Clear();
    int FindString(const wxString& string);
    int GetColumns();
    int GetSelection();
    wxString GetString(const int n);
    wxString GetStringSelection();
    int Number();
    void SetColumns(const int n = 1);
    void SetSelection(const int n);
    void SetStringSelection(const wxString& string);
};

//----------------------------------------------------------------------

class wxComboBox : public wxChoice {
public:
    wxComboBox(wxWindow* parent, wxWindowID id, char* value = "",
               const wxPoint& pos = wxPyDefaultPosition,
               const wxSize& size = wxPyDefaultSize,
               int LCOUNT=0, wxString* LIST=NULL,
               long style = 0,
               const wxValidator& validator = wxPyDefaultValidator,
               char* name = "comboBox");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void Append(const wxString& item);
    // TODO: void Append(const wxString& item, char* clientData);
    void Clear();
    void Copy();
    void Cut();
    void Delete(int n);
    // NotMember??:    void Deselect(int n);
    int FindString(const wxString& string);
    // TODO: char* GetClientData(const int n);
    long GetInsertionPoint();
    long GetLastPosition();
    int GetSelection();
    wxString GetString(int n);
    wxString GetStringSelection();
    wxString GetValue();
    int Number();
    void Paste();
    void Replace(long from, long to, const wxString& text);
    void Remove(long from, long to);
    // TODO:    void SetClientData(const int n, char* data);
    void SetInsertionPoint(long pos);
    void SetInsertionPointEnd();
    //void SetSelection(int n, int select = TRUE);  **** Just use the one in wxChoice
    %name(SetMark)void SetSelection(long from, long to);
    void SetValue(const wxString& text);
};

//----------------------------------------------------------------------

class wxGauge : public wxControl {
public:
    wxGauge(wxWindow* parent, wxWindowID id, int range,
            const wxPoint& pos = wxPyDefaultPosition,
            const wxSize& size = wxPyDefaultSize,
            long style = wxGA_HORIZONTAL,
            const wxValidator& validator = wxPyDefaultValidator,
            char* name = "gauge");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    int GetBezelFace();
    int GetRange();
    int GetShadowWidth();
    int GetValue();
    void SetBezelFace(int width);
    void SetRange(int range);
    void SetShadowWidth(int width);
    void SetValue(int pos);
};

//----------------------------------------------------------------------

class wxStaticBox : public wxControl {
public:
    wxStaticBox(wxWindow* parent, wxWindowID id, const wxString& label,
                const wxPoint& pos = wxPyDefaultPosition,
                const wxSize& size = wxPyDefaultSize,
                long style = 0,
                char* name = "staticBox");
};


//----------------------------------------------------------------------


class wxStaticLine : public wxControl {
public:
    wxStaticLine( wxWindow *parent, wxWindowID id,
                  const wxPoint &pos = wxPyDefaultPosition,
                  const wxSize &size = wxPyDefaultSize,
                  long style = wxLI_HORIZONTAL,
                  const char* name = "staticLine" );
};


//----------------------------------------------------------------------

class wxStaticText : public wxControl {
public:
    wxStaticText(wxWindow* parent, wxWindowID id, const wxString& label,
                 const wxPoint& pos = wxPyDefaultPosition,
                 const wxSize& size = wxPyDefaultSize,
                 long style = 0,
                 char* name = "staticText");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    wxString GetLabel();
    void SetLabel(const wxString& label);
};

//----------------------------------------------------------------------

class wxListBox : public wxControl {
public:
    wxListBox(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxPyDefaultPosition,
              const wxSize& size = wxPyDefaultSize,
              int LCOUNT, wxString* LIST = NULL,
              long style = 0,
              const wxValidator& validator = wxPyDefaultValidator,
              char* name = "listBox");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void Append(const wxString& item);
    // TODO:    void Append(const wxString& item, char* clientData);
    void Clear();
    void Delete(int n);
    void Deselect(int n);
    int FindString(const wxString& string);
    // TODO:    char* GetClientData(const int n);
    int GetSelection();

    // int GetSelections(int **selections);
    %addmethods {
      PyObject* GetSelections() {
          wxArrayInt lst;
          self->GetSelections(lst);
          PyObject *tup = PyTuple_New(lst.GetCount());
          for(int i=0; i<lst.GetCount(); i++) {
              PyTuple_SetItem(tup, i, PyInt_FromLong(lst[i]));
          }
          return tup;
      }
    }

    wxString GetString(int n);
    wxString GetStringSelection();
    int Number();
    bool Selected(const int n);
    void Set(int LCOUNT, wxString* LIST);
    // TODO:    void SetClientData(const int n, char* data);
    void SetFirstItem(int n);
    %name(SetFirstItemStr)void SetFirstItem(const wxString& string);
    void SetSelection(int n, bool select = TRUE);
    void SetString(int n, const wxString& string);
    void SetStringSelection(const wxString& string, bool select = TRUE);
};


//----------------------------------------------------------------------

class wxCheckListBox : public wxListBox {
public:
    wxCheckListBox(wxWindow *parent, wxWindowID id,
                   const wxPoint& pos = wxPyDefaultPosition,
                   const wxSize& size = wxPyDefaultSize,
                   int LCOUNT = 0,
                   wxString* LIST = NULL,
                   long style = 0,
                   const wxValidator& validator = wxPyDefaultValidator,
                   char* name = "listBox");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    bool  IsChecked(int uiIndex);
    void  Check(int uiIndex, bool bCheck = TRUE);

    int GetItemHeight();
};

//----------------------------------------------------------------------

class wxTextCtrl : public wxControl {
public:
    wxTextCtrl(wxWindow* parent, wxWindowID id, char* value = "",
               const wxPoint& pos = wxPyDefaultPosition,
               const wxSize& size = wxPyDefaultSize,
               long style = 0,
               const wxValidator& validator = wxPyDefaultValidator,
               char* name = "text");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void Clear();
    void Copy();
    void Cut();
    void DiscardEdits();
    long GetInsertionPoint();
    long GetLastPosition();
    int GetLineLength(long lineNo);
    wxString GetLineText(long lineNo);
    int GetNumberOfLines();
    wxString GetValue();
    bool IsModified();
    bool LoadFile(const wxString& filename);
    void Paste();
    void PositionToXY(long pos, long *OUTPUT, long *OUTPUT);
    void Remove(long from, long to);
    void Replace(long from, long to, const wxString& value);
    bool SaveFile(const wxString& filename);
    void SetEditable(bool editable);
    void SetInsertionPoint(long pos);
    void SetInsertionPointEnd();
    void SetSelection(long from, long to);
    void SetValue(const wxString& value);
    void ShowPosition(long pos);
    void WriteText(const wxString& text);
    void AppendText(const wxString& text);
    long XYToPosition(long x, long y);

    bool CanCopy();
    bool CanCut();
    bool CanPaste();
    bool CanRedo();
    bool CanUndo();
    void GetSelection(long* OUTPUT, long* OUTPUT);
    bool IsEditable();
};

//----------------------------------------------------------------------

class wxScrollBar : public wxControl {
public:
    wxScrollBar(wxWindow* parent, wxWindowID id = -1,
                const wxPoint& pos = wxPyDefaultPosition,
                const wxSize& size = wxPyDefaultSize,
                long style = wxSB_HORIZONTAL,
                const wxValidator& validator = wxPyDefaultValidator,
                char* name = "scrollBar");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    int GetRange();
    int GetPageSize();
    int GetThumbPosition();
    int GetThumbSize();
    void SetThumbPosition(int viewStart);
    void SetScrollbar(int position, int thumbSize,
                      int range,    int pageSize,
                      bool refresh = TRUE);
};

//----------------------------------------------------------------------

class wxSpinButton : public wxControl {
public:
    wxSpinButton(wxWindow* parent, wxWindowID id = -1,
                 const wxPoint& pos = wxPyDefaultPosition,
                 const wxSize& size = wxPyDefaultSize,
                 long style = wxSP_HORIZONTAL,
                 char* name = "spinButton");

    int GetMax();
    int GetMin();
    int GetValue();
    void SetRange(int min, int max);
    void SetValue(int value);
};

//----------------------------------------------------------------------

class wxStaticBitmap : public wxControl {
public:
    wxStaticBitmap(wxWindow* parent, wxWindowID id,
                   const wxBitmap& bitmap,
                   const wxPoint& pos = wxPyDefaultPosition,
                   const wxSize& size = wxPyDefaultSize,
                   long style = 0,
                   char* name = "staticBitmap");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    const wxBitmap& GetBitmap();
    void SetBitmap(const wxBitmap& bitmap);
    void SetIcon(const wxIcon& icon);
};

//----------------------------------------------------------------------

class wxRadioBox : public wxControl {
public:
    wxRadioBox(wxWindow* parent, wxWindowID id,
               const wxString& label,
               const wxPoint& point = wxPyDefaultPosition,
               const wxSize& size = wxPyDefaultSize,
               int LCOUNT = 0, wxString* LIST = NULL,
               int majorDimension = 0,
               long style = wxRA_HORIZONTAL,
               const wxValidator& validator = wxPyDefaultValidator,
               char* name = "radioBox");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void Enable(bool enable);
    %name(EnableItem)void Enable(int n, bool enable);
    int FindString(const wxString& string);

//***    wxString GetLabel();
//***    void SetLabel(const wxString& label);

    %name(GetItemLabel)wxString GetLabel(int n);
    int GetSelection();
    wxString GetString(int n);
    wxString GetStringSelection();
    int Number();
    %name(SetItemLabel)void SetLabel(int n, const wxString& label);
    void SetSelection(int n);
    void SetStringSelection(const wxString& string);
    void Show(bool show);
    %name(ShowItem)void Show(int item, bool show);
};

//----------------------------------------------------------------------

class wxRadioButton : public wxControl {
public:
    wxRadioButton(wxWindow* parent, wxWindowID id,
                  const wxString& label,
                  const wxPoint& pos = wxPyDefaultPosition,
                  const wxSize& size = wxPyDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxPyDefaultValidator,
                  char* name = "radioButton");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    bool GetValue();
    void SetValue(bool value);
};

//----------------------------------------------------------------------

class wxSlider : public wxControl {
public:
    wxSlider(wxWindow* parent, wxWindowID id,
             int value, int minValue, int maxValue,
             const wxPoint& point = wxPyDefaultPosition,
             const wxSize& size = wxPyDefaultSize,
             long style = wxSL_HORIZONTAL,
             const wxValidator& validator = wxPyDefaultValidator,
             char* name = "slider");

    %pragma(python) addtomethod = "__init__:wx._StdWindowCallbacks(self)"

    void ClearSel();
    void ClearTicks();
    int GetLineSize();
    int GetMax();
    int GetMin();
    int GetPageSize();
    int GetSelEnd();
    int GetSelStart();
    int GetThumbLength();
    int GetTickFreq();
    int GetValue();
    void SetRange(int minValue, int maxValue);
    void SetTickFreq(int n, int pos);
    void SetLineSize(int lineSize);
    void SetPageSize(int pageSize);
    void SetSelection(int startPos, int endPos);
    void SetThumbLength(int len);
    void SetTick(int tickPos);
    void SetValue(int value);
};


//----------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
//
// $Log$
// Revision 1.16  1999/07/31 07:54:35  RD
// wxPython 2.1b1:
//
// 	Added the missing wxWindow.GetUpdateRegion() method.
//
// 	Made a new change in SWIG (update your patches everybody) that
// 	provides a fix for global shadow objects that get an exception in
// 	their __del__ when their extension module has already been deleted.
// 	It was only a 1 line change in .../SWIG/Modules/pycpp.cxx at about
// 	line 496 if you want to do it by hand.
//
// 	It is now possible to run through MainLoop more than once in any one
// 	process.  The cleanup that used to happen as MainLoop completed (and
// 	prevented it from running again) has been delayed until the wxc module
// 	is being unloaded by Python.
//
// 	wxWindow.PopupMenu() now takes a wxPoint instead of  x,y.  Added
// 	wxWindow.PopupMenuXY to be consistent with some other methods.
//
// 	Added wxGrid.SetEditInPlace and wxGrid.GetEditInPlace.
//
// 	You can now provide your own app.MainLoop method.  See
// 	wxPython/demo/demoMainLoop.py for an example and some explaination.
//
// 	Got the in-place-edit for the wxTreeCtrl fixed and added some demo
// 	code to show how to use it.
//
// 	Put the wxIcon constructor back in for GTK as it now has one that
// 	matches MSW's.
//
// 	Added wxGrid.GetCells
//
// 	Added wxSystemSettings static methods as functions with names like
// 	wxSystemSettings_GetSystemColour.
//
// 	Removed wxPyMenu since using menu callbacks have been depreciated in
// 	wxWindows.  Use wxMenu and events instead.
//
// 	Added alternate wxBitmap constructor (for MSW only) as
// 	      wxBitmapFromData(data, type, width, height, depth = 1)
//
// 	Added a helper function named wxPyTypeCast that can convert shadow
// 	objects of one type into shadow objects of another type.  (Like doing
// 	a down-cast.)  See the implementation in wx.py for some docs.
//
// Revision 1.15  1999/06/22 17:45:18  RD
//
// wxPython 2.1b1:  Very minor changes needed for wxGTK
//
// Revision 1.14  1999/06/22 07:03:02  RD
//
// wxPython 2.1b1 for wxMSW  (wxGTK coming soon)
// Lots of changes, see the README.txt for details...
//
// Revision 1.13  1999/04/30 21:13:43  RD
//
// wxPython 2.0b9, first phase (win32)
// Added gobs of stuff, see wxPython/README.txt for details
//
// Revision 1.12  1999/04/30 03:29:18  RD
//
// wxPython 2.0b9, first phase (win32)
// Added gobs of stuff, see wxPython/README.txt for details
//
// Revision 1.11.4.1  1999/03/27 23:29:14  RD
//
// wxPython 2.0b8
//     Python thread support
//     various minor additions
//     various minor fixes
//
// Revision 1.11  1999/02/25 07:08:30  RD
//
// wxPython version 2.0b5
//
// Revision 1.10  1998/12/17 17:52:19  RD
//
// wxPython 0.5.2
// Minor fixes and SWIG code generation for RR's changes.  MSW and GTK
// versions are much closer now!
//
// Revision 1.9  1998/12/17 14:07:29  RR
//
//   Removed minor differences between wxMSW and wxGTK
//
// Revision 1.8  1998/12/15 20:41:15  RD
// Changed the import semantics from "from wxPython import *" to "from
// wxPython.wx import *"  This is for people who are worried about
// namespace pollution, they can use "from wxPython import wx" and then
// prefix all the wxPython identifiers with "wx."
//
// Added wxTaskbarIcon for wxMSW.
//
// Made the events work for wxGrid.
//
// Added wxConfig.
//
// Added wxMiniFrame for wxGTK, (untested.)
//
// Changed many of the args and return values that were pointers to gdi
// objects to references to reflect changes in the wxWindows API.
//
// Other assorted fixes and additions.
//
// Revision 1.7  1998/11/16 00:00:53  RD
// Generic treectrl for wxPython/GTK compiles...
//
// Revision 1.6  1998/11/15 23:03:43  RD
// Removing some ifdef's for wxGTK
//
// Revision 1.5  1998/10/07 07:34:32  RD
// Version 0.4.1 for wxGTK
//
// Revision 1.4  1998/10/02 06:40:35  RD
//
// Version 0.4 of wxPython for MSW.
//
// Revision 1.3  1998/08/18 19:48:14  RD
// more wxGTK compatibility things.
//
// It builds now but there are serious runtime problems...
//
// Revision 1.2  1998/08/15 07:36:28  RD
// - Moved the header in the .i files out of the code that gets put into
// the .cpp files.  It caused CVS conflicts because of the RCS ID being
// different each time.
//
// - A few minor fixes.
//
// Revision 1.1  1998/08/09 08:25:49  RD
// Initial version
//
//

