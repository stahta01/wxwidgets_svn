/////////////////////////////////////////////////////////////////////////////
// Name:        scrolbar.h
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKSCROLLBARH__
#define __GTKSCROLLBARH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxScrollBar;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char *wxScrollBarNameStr;

//-----------------------------------------------------------------------------
// wxScrollBar
//-----------------------------------------------------------------------------

class wxScrollBar: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxScrollBar)

  public:
  
    wxScrollBar(void) { m_adjust = NULL; m_oldPos = 0.0; };
    wxScrollBar(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxSB_HORIZONTAL,
           const wxString& name = wxScrollBarNameStr );
    ~wxScrollBar(void);
    bool Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxSB_HORIZONTAL,
           const wxString& name = wxScrollBarNameStr);
    int GetPosition(void) const;
    int GetThumbSize() const;
    int GetPageSize() const;
    int GetRange() const;
    virtual void SetPosition( int viewStart );
    virtual void SetScrollbar( int position, int thumbSize, int range, int pageSize,
      bool refresh = TRUE );

    // Backward compatibility
    int GetValue(void) const;
    void SetValue( int viewStart );
    void GetValues( int *viewStart, int *viewLength, int *objectLength, int *pageLength) const;
    int GetViewLength() const;
    int GetObjectLength() const;
    void SetPageSize( int pageLength );
    void SetObjectLength( int objectLength );
    void SetViewLength( int viewLength );

  public:
  
    GtkAdjustment  *m_adjust;
    float           m_oldPos;
};

#endif
    // __GTKSCROLLBARH__
