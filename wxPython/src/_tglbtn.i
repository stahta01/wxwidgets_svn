/////////////////////////////////////////////////////////////////////////////
// Name:        _tglbtn.i
// Purpose:     SWIG interface defs for wxToggleButton
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

MAKE_CONST_WXSTRING2(ToggleButtonNameStr, _T("wxToggleButton"));

//---------------------------------------------------------------------------
%newgroup


%{
#if defined(__WXMAC__) || defined(__WXX11__)
// implement dummy classes and such for wxMac

#define wxEVT_COMMAND_TOGGLEBUTTON_CLICKED 0
    
class wxToggleButton : public wxControl
{
public:
    wxToggleButton(wxWindow *, wxWindowID, const wxString&,
                   const wxPoint&, const wxSize&, long,
                   const wxValidator&, const wxString&)
        { wxPyRaiseNotImplemented(); }
    
    wxToggleButton()
        { wxPyRaiseNotImplemented(); }
};
#endif
%}



%constant wxEventType wxEVT_COMMAND_TOGGLEBUTTON_CLICKED;

%pythoncode {
    EVT_TOGGLEBUTTON = wx.PyEventBinder( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, 1)
}


class wxToggleButton : public wxControl
{
public:
    %addtofunc wxToggleButton         "self._setOORInfo(self)"
    %addtofunc wxToggleButton()       ""

    wxToggleButton(wxWindow *parent,
                   wxWindowID id,
                   const wxString& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxPyToggleButtonNameStr);
    %name(PreToggleButton)wxToggleButton();

#if defined(__WXMSW__) || defined(__WXGTK__)
    bool Create(wxWindow *parent,
                   wxWindowID id,
                   const wxString& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxPyToggleButtonNameStr);

    void SetValue(bool value);
    bool GetValue() const ;
    void SetLabel(const wxString& label);
#endif
};

//---------------------------------------------------------------------------
