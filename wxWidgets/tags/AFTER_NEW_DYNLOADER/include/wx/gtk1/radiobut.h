/////////////////////////////////////////////////////////////////////////////
// Name:        radiobut.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKRADIOBUTTONH__
#define __GTKRADIOBUTTONH__

#ifdef __GNUG__
#pragma interface
#endif

//-----------------------------------------------------------------------------
// wxRadioButton
//-----------------------------------------------------------------------------

class wxRadioButton: public wxControl
{
public:
    inline wxRadioButton() {}
    inline wxRadioButton( wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxRadioButtonNameStr )
    {
        Create( parent, id, label, pos, size, style, validator, name );
    }
    bool Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxRadioButtonNameStr );
    virtual void SetLabel(const wxString& label);
    virtual void SetValue(bool val);
    virtual bool GetValue() const;
    bool Enable( bool enable );
    
  // implementation
    
    GSList *m_radioButtonGroup;
    void ApplyWidgetStyle();
    bool IsOwnGtkWindow( GdkWindow *window );
    void OnInternalIdle();
    
    bool m_blockEvent;
    
protected:
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxRadioButton)
};

#endif // __GTKRADIOBUTTONH__
