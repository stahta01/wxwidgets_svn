/////////////////////////////////////////////////////////////////////////////
// Name:        button.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKBUTTONH__
#define __GTKBUTTONH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxButton;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const wxChar *wxButtonNameStr;

//-----------------------------------------------------------------------------
// wxButton
//-----------------------------------------------------------------------------

class wxButton: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxButton)

  public:

    wxButton();
    inline wxButton(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr)
    {
      Create(parent, id, label, pos, size, style, validator, name);
    }
    ~wxButton();
    bool Create(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr);
    virtual void SetDefault();
    void SetLabel( const wxString &label );
    bool Enable( bool enable );

    static wxSize GetDefaultSize();
    
  // implementation        
  
    void ApplyWidgetStyle();    
};

#endif // __GTKBUTTONH__
