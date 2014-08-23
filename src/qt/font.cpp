/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/font.cpp
// Author:      Peter Most, Javier Torres
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/font.h"
#include "wx/fontutil.h"
#include "wx/qt/utils.h"
#include "wx/qt/converter.h"

#include <QtGui/QFont>

static QFont::StyleHint ConvertFontFamily(wxFontFamily family)
{
    switch (family)
    {
        case wxFONTFAMILY_DEFAULT:
            return QFont::AnyStyle;

        case wxFONTFAMILY_DECORATIVE:
            return QFont::Decorative;

        case wxFONTFAMILY_ROMAN:
            return QFont::Serif;

        case wxFONTFAMILY_SCRIPT:
            return QFont::Decorative;

        case wxFONTFAMILY_SWISS:
            return QFont::SansSerif;

        case wxFONTFAMILY_MODERN:
            return QFont::TypeWriter;

        case wxFONTFAMILY_TELETYPE:
            return QFont::TypeWriter;

        case wxFONTFAMILY_MAX:
            wxFAIL_MSG( "Invalid font family value" );
            break;
    }
    return QFont::AnyStyle;
}

static QFont::Weight ConvertFontWeight(wxFontWeight weight)
{
    switch (weight)
    {
        case wxFONTWEIGHT_NORMAL:
            return QFont::Normal;

        case wxFONTWEIGHT_LIGHT:
            return QFont::Light;

        case wxFONTWEIGHT_BOLD:
            return QFont::Bold;

        case wxFONTWEIGHT_MAX:
            wxFAIL_MSG( "Invalid font weight value" );
            break;
    }
    return QFont::Normal;
}

class wxFontRefData: public wxGDIRefData
{
    public:
        wxFontRefData()
        {
        }

        wxFontRefData(int size, QFont::StyleHint family, bool italic, QFont::Weight weight, bool underlined, QString face)
        {
            if (!face.isEmpty())
                m_qtFont.setFamily(face);
            else
            {
                m_qtFont.setStyleHint(family);
                // force qt to choose a font using the hint:
                m_qtFont.setFamily("");
            }
            m_qtFont.setItalic(italic);
            m_qtFont.setWeight(weight);
            m_qtFont.setUnderline(underlined);
            m_qtFont.setPointSize(size);
        }

        wxFontRefData( const wxFontRefData& data )
        : wxGDIRefData()
        {
            m_qtFont = data.m_qtFont;
        }

        bool operator == (const wxFontRefData& data) const
        {
            return m_qtFont == data.m_qtFont;
        }
        
        QFont m_qtFont;
};

#define M_FONTDATA ((wxFontRefData *)m_refData)->m_qtFont

wxFont::wxFont()
{
    m_refData = new wxFontRefData();
}

wxFont::wxFont(const wxFontInfo& info)
{
    Create(info.GetPointSize(),
           info.GetFamily(),
           info.GetStyle(),
           info.GetWeight(),
           info.IsUnderlined(),
           info.GetFaceName(),
           info.GetEncoding());

    SetStrikethrough(info.IsStrikethrough());

    wxSize pixelSize = info.GetPixelSize();
    if ( pixelSize != wxDefaultSize )
        SetPixelSize(pixelSize);
}

wxFont::wxFont(const wxString& nativeFontInfoString)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );

    m_refData = new wxFontRefData();
}

wxFont::wxFont(const wxNativeFontInfo& info)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );

    m_refData = new wxFontRefData();
}

wxFont::wxFont(int size,
       wxFontFamily family,
       wxFontStyle style,
       wxFontWeight weight,
       bool underlined,
       const wxString& face,
       wxFontEncoding encoding)
{
    Create(size, family, style, weight, underlined, face, encoding);
}

wxFont::wxFont(const wxSize& pixelSize,
       wxFontFamily family,
       wxFontStyle style,
       wxFontWeight weight,
       bool underlined,
       const wxString& face,
       wxFontEncoding encoding)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );

    m_refData = new wxFontRefData();
}

wxFont::wxFont(int size,
       int family,
       int style,
       int weight,
       bool underlined,
       const wxString& face,
       wxFontEncoding encoding)
{
    Create(size, (wxFontFamily)family, (wxFontStyle)style, (wxFontWeight)weight, underlined, face, encoding);
}


bool wxFont::Create(int size, wxFontFamily family, wxFontStyle style,
        wxFontWeight weight, bool underlined, const wxString& face,
        wxFontEncoding encoding )
{
    m_refData = new wxFontRefData(size, ConvertFontFamily(family), style != wxFONTSTYLE_NORMAL,
                                  ConvertFontWeight(weight), underlined, wxQtConvertString(face));

    return true;
}

int wxFont::GetPointSize() const
{
    return M_FONTDATA.pointSize();
}

wxFontFamily wxFont::GetFamily() const
{
    switch (M_FONTDATA.styleHint())
    {
        case QFont::AnyStyle:
            return wxFONTFAMILY_DEFAULT;

        case QFont::Decorative:
            return wxFONTFAMILY_DECORATIVE;

        case QFont::Serif:
            return wxFONTFAMILY_ROMAN;

        case QFont::SansSerif:
            return wxFONTFAMILY_SWISS;

        case QFont::TypeWriter:
            return wxFONTFAMILY_TELETYPE;

        default:
            wxFAIL_MSG( "Invalid font family value" );
            break;
    }
    return wxFontFamily();
}

wxFontStyle wxFont::GetStyle() const
{
    switch (M_FONTDATA.style())
    {
        case QFont::StyleNormal:
            return wxFONTSTYLE_NORMAL;

        case QFont::StyleItalic:
            return wxFONTSTYLE_ITALIC;

        case QFont::StyleOblique:
            return wxFONTSTYLE_SLANT;

        default:
            wxFAIL_MSG( "Invalid font style value" );
            break;
    }
    return wxFontStyle();
}

wxFontWeight wxFont::GetWeight() const
{
    switch ( M_FONTDATA.weight() )
    {
        case QFont::Normal:
            return wxFONTWEIGHT_NORMAL;

        case QFont::Light:
            return wxFONTWEIGHT_LIGHT;

        case QFont::Bold:
            return wxFONTWEIGHT_BOLD;

        default:
            wxFAIL_MSG( "Invalid font weight value" );
            break;
    }
    return wxFontWeight();
}

bool wxFont::GetUnderlined() const
{
    return M_FONTDATA.underline();
}

wxString wxFont::GetFaceName() const
{
    return wxQtConvertString(M_FONTDATA.family());
}

wxFontEncoding wxFont::GetEncoding() const
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );

    return wxFontEncoding();
}

const wxNativeFontInfo *wxFont::GetNativeFontInfo() const
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );

    return NULL;
}


void wxFont::SetPointSize( int pointSize )
{
    AllocExclusive();
    M_FONTDATA.setPointSize(pointSize);
}

bool wxFont::SetFaceName(const wxString& facename)
{
    AllocExclusive();
    M_FONTDATA.setFamily(wxQtConvertString(facename));
    // qt uses a "font matching algoritm" so the font will be allways valid
    return true;
}

void wxFont::SetFamily( wxFontFamily family )
{
    AllocExclusive();
    M_FONTDATA.setStyleHint(ConvertFontFamily(family));
    // reset the face name to force qt to choose a new font
    M_FONTDATA.setFamily("");
}

void wxFont::SetStyle( wxFontStyle style )
{
    AllocExclusive();
    M_FONTDATA.setItalic(style != wxFONTSTYLE_NORMAL);
}

void wxFont::SetWeight( wxFontWeight weight )
{
    AllocExclusive();
    M_FONTDATA.setWeight(ConvertFontWeight(weight));
}

void wxFont::SetUnderlined( bool underlined )
{
    AllocExclusive();
    M_FONTDATA.setUnderline(underlined);
}

void wxFont::SetEncoding(wxFontEncoding encoding)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
}

wxGDIRefData *wxFont::CreateGDIRefData() const
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
    return NULL;
}

wxGDIRefData *wxFont::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxFontRefData(*(wxFontRefData *)data);
}


QFont wxFont::GetHandle() const
{
    return M_FONTDATA;
}

wxFontFamily wxFont::DoGetFamily() const
{
    return wxFONTFAMILY_DEFAULT;
}

