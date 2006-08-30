/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/colour.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_COLOUR_H_
#define _WX_GTK_COLOUR_H_

//-----------------------------------------------------------------------------
// wxColour
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxColour : public wxColourBase
{
public:
    // constructors
    // ------------

    // default
    wxColour() {}
    DEFINE_STD_WXCOLOUR_CONSTRUCTORS
    wxColour(const GdkColor& gdkColor);

    ~wxColour();

    bool Ok() const { return m_refData != NULL; }

    bool operator == ( const wxColour& col ) const;
    bool operator != ( const wxColour& col ) const { return !(*this == col); }

    unsigned char Red() const;
    unsigned char Green() const;
    unsigned char Blue() const;

    // Implementation part
    void CalcPixel( GdkColormap *cmap );
    int GetPixel() const;
#ifdef __WXGTK24__
    const GdkColor *GetColor() const;
#else
    // GDK functions from old gtk2 versions take non-const color parameters,
    // too many uses to deal with individually
    GdkColor *GetColor() const;
#endif

protected:
    virtual bool FromString(const wxChar *str);
    virtual void InitWith( unsigned char red, unsigned char green, unsigned char blue );

private:
    DECLARE_DYNAMIC_CLASS(wxColour)
};

#endif // _WX_GTK_COLOUR_H_
