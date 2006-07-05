/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/colour.h
// Purpose:     wxColour class
// Author:      David Elliott
// Modified by:
// Created:     2003/06/17
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_COLOUR_H__
#define __WX_COCOA_COLOUR_H__

#include "wx/object.h"
#include "wx/string.h"

// ========================================================================
// wxColour
// ========================================================================

class WXDLLEXPORT wxColour : public wxColourBase
{
public:
    // constructors
    // ------------

    // default
    wxColour() { Init(); }
    DEFINE_STD_WXCOLOUR_CONSTRUCTORS

    // initialization using existing NSColor
    wxColour( WX_NSColor aColor );


    // copy ctors and assignment operators
    wxColour( const wxColour& col );
    wxColour& operator = ( const wxColour& col );

    virtual ~wxColour();

    // accessors
    bool Ok() const { return m_cocoaNSColor; }
    WX_NSColor GetNSColor() { return m_cocoaNSColor; }

    unsigned char Red() const { return m_red; }
    unsigned char Green() const { return m_green; }
    unsigned char Blue() const { return m_blue; }

    // comparison
    bool operator == (const wxColour& colour) const
    {
        // TODO: Really compare the NSColor
        return (m_cocoaNSColor == colour.m_cocoaNSColor
            || (m_red == colour.m_red
            && m_green == colour.m_green
            && m_blue == colour.m_blue));
    }
    bool operator != (const wxColour& colour) const
    {   return !(*this == colour); }

    // Set() functions
    void Set( WX_NSColor aColor );
    
    // reroute the inherited ones
    void Set(unsigned char red, unsigned char green, unsigned char blue)
    { wxColourBase::Set(red,green,blue); }
    
    // implemented in colourcmn.cpp
    bool Set(const wxChar *str)
    { return wxColourBase::Set(str); }
    
    bool Set(const wxString &str)
    { return wxColourBase::Set(str); }
    
    void Set(unsigned long colRGB)
    { wxColourBase::Set(colRGB); }
    
protected:
    // puts the object in an invalid, uninitialized state
    void Init();

    virtual void InitWith( unsigned char red, unsigned char green, unsigned char blue );

private:
    WX_NSColor m_cocoaNSColor;
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;

    DECLARE_DYNAMIC_CLASS(wxColour)
};

#endif // __WX_COCOA_COLOUR_H__
