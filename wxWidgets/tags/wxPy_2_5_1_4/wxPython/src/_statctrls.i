/////////////////////////////////////////////////////////////////////////////
// Name:        _statctrls.i
// Purpose:     SWIG interface defs for wxStaticBox, wxStaticLine,
//              wxStaticText, wxStaticBitmap
//
// Author:      Robin Dunn
//
// Created:     10-June-1998
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

MAKE_CONST_WXSTRING(StaticBitmapNameStr);
MAKE_CONST_WXSTRING(StaticBoxNameStr);
MAKE_CONST_WXSTRING(StaticTextNameStr);

//---------------------------------------------------------------------------
%newgroup

class wxStaticBox : public wxControl {
public:
    %pythonAppend wxStaticBox         "self._setOORInfo(self)"
    %pythonAppend wxStaticBox()       ""

    wxStaticBox(wxWindow* parent, wxWindowID id, const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPyStaticBoxNameStr);
    %name(PreStaticBox)wxStaticBox();

    bool Create(wxWindow* parent, wxWindowID id, const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPyStaticBoxNameStr);
};


//---------------------------------------------------------------------------
%newgroup


class wxStaticLine : public wxControl {
public:
    %pythonAppend wxStaticLine         "self._setOORInfo(self)"
    %pythonAppend wxStaticLine()       ""

    wxStaticLine( wxWindow *parent, wxWindowID id,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxLI_HORIZONTAL,
                  const wxString& name = wxPyStaticTextNameStr);
    %name(PreStaticLine)wxStaticLine();

    bool Create( wxWindow *parent, wxWindowID id,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxLI_HORIZONTAL,
                  const wxString& name = wxPyStaticTextNameStr);

    // is the line vertical?
    bool IsVertical() const;

    // get the default size for the "lesser" dimension of the static line
    static int GetDefaultSize();

};


//---------------------------------------------------------------------------
%newgroup

class wxStaticText : public wxControl {
public:
    %pythonAppend wxStaticText         "self._setOORInfo(self)"
    %pythonAppend wxStaticText()       ""

    wxStaticText(wxWindow* parent, wxWindowID id, const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxPyStaticTextNameStr);
    %name(PreStaticText)wxStaticText();

    bool Create(wxWindow* parent, wxWindowID id, const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxPyStaticTextNameStr);
};

//---------------------------------------------------------------------------
%newgroup

class wxStaticBitmap : public wxControl {
public:
    %pythonAppend wxStaticBitmap         "self._setOORInfo(self)"
    %pythonAppend wxStaticBitmap()       ""

    wxStaticBitmap(wxWindow* parent, wxWindowID id,
                   const wxBitmap& bitmap,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxString& name = wxPyStaticBitmapNameStr);
    %name(PreStaticBitmap)wxStaticBitmap();

    bool Create(wxWindow* parent, wxWindowID id,
                   const wxBitmap& bitmap,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxString& name = wxPyStaticBitmapNameStr);

    wxBitmap GetBitmap();
    void SetBitmap(const wxBitmap& bitmap);
    void SetIcon(const wxIcon& icon);
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
