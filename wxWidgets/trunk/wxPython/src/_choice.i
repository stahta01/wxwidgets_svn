/////////////////////////////////////////////////////////////////////////////
// Name:        _choice.i
// Purpose:     SWIG interface defs for wxChoice
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

MAKE_CONST_WXSTRING(ChoiceNameStr);


//---------------------------------------------------------------------------
%newgroup;

class wxChoice : public wxControlWithItems
{
public:
    %pythonAppend wxChoice         "self._setOORInfo(self)"
    %pythonAppend wxChoice()       ""

    wxChoice(wxWindow *parent, wxWindowID id,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             int choices=0, wxString* choices_array=NULL,
             long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxPyChoiceNameStr);
    %name(PreChoice)wxChoice();

    bool Create(wxWindow *parent, wxWindowID id,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             int choices=0, wxString* choices_array=NULL,
             long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxPyChoiceNameStr);

#ifndef __WXX11__
    int GetColumns();
    void SetColumns(const int n = 1);
#endif
    void SetSelection(const int n);
    void SetStringSelection(const wxString& string);
    void SetString(int n, const wxString& s);

    %pragma(python) addtoclass = "
    Select = SetSelection
    "
};

//---------------------------------------------------------------------------
