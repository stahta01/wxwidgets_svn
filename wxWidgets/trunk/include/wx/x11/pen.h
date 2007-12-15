/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/pen.h
// Purpose:     wxPen class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PEN_H_
#define _WX_PEN_H_

#include "wx/gdicmn.h"
#include "wx/gdiobj.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_CORE wxPen;
class WXDLLIMPEXP_FWD_CORE wxColour;
class WXDLLIMPEXP_FWD_CORE wxBitmap;

typedef char wxX11Dash;

//-----------------------------------------------------------------------------
// wxPen
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPen: public wxGDIObject
{
public:
    wxPen() { }

    wxPen( const wxColour &colour, int width = 1, int style = wxSOLID );
    wxPen( const wxBitmap &stipple, int width );
    virtual ~wxPen();

    bool operator == ( const wxPen& pen ) const;
    bool operator != (const wxPen& pen) const { return !(*this == pen); }

    void SetColour( const wxColour &colour );
    void SetColour( unsigned char red, unsigned char green, unsigned char blue );
    void SetCap( int capStyle );
    void SetJoin( int joinStyle );
    void SetStyle( int style );
    void SetWidth( int width );
    void SetDashes( int number_of_dashes, const wxDash *dash );
    void SetStipple( wxBitmap *stipple );

    wxColour &GetColour() const;
    int GetCap() const;
    int GetJoin() const;
    int GetStyle() const;
    int GetWidth() const;
    int GetDashes(wxDash **ptr) const;
    int GetDashCount() const;
    wxDash* GetDash() const;
    wxBitmap* GetStipple() const;

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

    DECLARE_DYNAMIC_CLASS(wxPen)
};

#endif // _WX_PEN_H_
