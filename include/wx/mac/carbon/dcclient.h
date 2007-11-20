/////////////////////////////////////////////////////////////////////////////
// Name:        dcclient.h
// Purpose:     wxClientDC, wxPaintDC and wxWindowDC classes
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCCLIENT_H_
#define _WX_DCCLIENT_H_

#include "wx/dc.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_CORE wxPaintDC;
class WXDLLIMPEXP_FWD_CORE wxWindow;

class WXDLLEXPORT wxWindowDC: public wxDC
{
  DECLARE_DYNAMIC_CLASS(wxWindowDC)

 public:
  wxWindowDC(void);

  // Create a DC corresponding to a canvas
  wxWindowDC(wxWindow *win);

  virtual ~wxWindowDC(void);
  wxWindow *GetWindow() const { return m_window; }
  protected :
    virtual void DoGetSize( int *width, int *height ) const;
    virtual wxBitmap DoGetAsBitmap(const wxRect *subrect) const; 
    wxWindow     *m_window;

	bool		m_release;
	int			m_width;
	int			m_height;

};


class WXDLLEXPORT wxClientDC: public wxWindowDC
{
  DECLARE_DYNAMIC_CLASS(wxClientDC)

 public:
  wxClientDC(void);

  // Create a DC corresponding to a canvas
  wxClientDC(wxWindow *win);

  virtual ~wxClientDC(void);

protected:
};

class WXDLLEXPORT wxPaintDC: public wxWindowDC
{
  DECLARE_DYNAMIC_CLASS(wxPaintDC)

 public:
  wxPaintDC(void);

  // Create a DC corresponding to a canvas
  wxPaintDC(wxWindow *win);

  virtual ~wxPaintDC(void);

protected:
};

#endif
    // _WX_DCCLIENT_H_
