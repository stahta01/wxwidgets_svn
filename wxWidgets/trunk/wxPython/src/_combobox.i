/////////////////////////////////////////////////////////////////////////////
// Name:        _combobox.i
// Purpose:     SWIG interface defs for wxComboBox
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

MAKE_CONST_WXSTRING(ComboBoxNameStr);

//---------------------------------------------------------------------------
%newgroup;


#ifdef __WXMSW__
class wxComboBox : public wxChoice
#else
class wxComboBox : public wxControl, public wxItemContainer
#endif
{
public:
    %pythonAppend wxComboBox         "self._setOORInfo(self)"
    %pythonAppend wxComboBox()       ""

    wxComboBox(wxWindow* parent, wxWindowID id,
               const wxString& value = wxPyEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               //int choices=0, wxString* choices_array=NULL,
               const wxArrayString& choices = wxPyEmptyStringArray,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxPyComboBoxNameStr);
    %name(PreComboBox)wxComboBox();

    bool Create(wxWindow* parent, wxWindowID id,
                const wxString& value = wxPyEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                //int choices=0, wxString* choices_array=NULL,
                const wxArrayString& choices = wxPyEmptyStringArray,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxPyComboBoxNameStr);


    virtual wxString GetValue() const;
    virtual void SetValue(const wxString& value);

    virtual void Copy();
    virtual void Cut();
    virtual void Paste();
    
    virtual void SetInsertionPoint(long pos);
    virtual long GetInsertionPoint() const;
    virtual long GetLastPosition() const;
    virtual void Replace(long from, long to, const wxString& value);
    void SetSelection(int n);
    %name(SetMark) virtual void SetSelection(long from, long to);
    virtual void SetEditable(bool editable);

    virtual void SetInsertionPointEnd();
    virtual void Remove(long from, long to);
};

//---------------------------------------------------------------------------
