/////////////////////////////////////////////////////////////////////////////
// Name:        _slider.i
// Purpose:     SWIG interface defs for wxSlider
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

%{
#include <wx/slider.h>

    DECLARE_DEF_STRING(SliderNameStr);
%}

//---------------------------------------------------------------------------
%newgroup

class wxSlider : public wxControl {
public:
    %addtofunc wxSlider         "self._setOORInfo(self)"
    %addtofunc wxSlider()       ""

    wxSlider(wxWindow* parent, wxWindowID id,
             int value, int minValue, int maxValue,
             const wxPoint& point = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxSL_HORIZONTAL,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxPySliderNameStr);
    %name(PreSlider)wxSlider();

    bool Create(wxWindow* parent, wxWindowID id,
             int value, int minValue, int maxValue,
             const wxPoint& point = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxSL_HORIZONTAL,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxPySliderNameStr);

    // get/set the current slider value (should be in range)
    virtual int GetValue() const;
    virtual void SetValue(int value);

    // retrieve/change the range
    virtual void SetRange(int minValue, int maxValue);
    virtual int GetMin() const;
    virtual int GetMax() const;
    
    void SetMin( int minValue );
    void SetMax( int maxValue );

    // the line/page size is the increment by which the slider moves when
    // cursor arrow key/page up or down are pressed (clicking the mouse is like
    // pressing PageUp/Down) and are by default set to 1 and 1/10 of the range
    virtual void SetLineSize(int lineSize);
    virtual void SetPageSize(int pageSize);
    virtual int GetLineSize() const;
    virtual int GetPageSize() const;

    // these methods get/set the length of the slider pointer in pixels
    virtual void SetThumbLength(int lenPixels);
    virtual int GetThumbLength() const;

    virtual void SetTickFreq(int n, int pos);
    virtual int GetTickFreq() const;
    virtual void ClearTicks();
    virtual void SetTick(int tickPos);

    virtual void ClearSel();
    virtual int GetSelEnd() const;
    virtual int GetSelStart() const;
    virtual void SetSelection(int min, int max);

};

//---------------------------------------------------------------------------
