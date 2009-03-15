/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/fontcmn.cpp
// Purpose:     implementation of wxFontBase methods
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.09.99
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/font.h"

#ifndef WX_PRECOMP
    #include "wx/dc.h"
    #include "wx/intl.h"
    #include "wx/dcscreen.h"
    #include "wx/log.h"
    #include "wx/gdicmn.h"
#endif // WX_PRECOMP

#if defined(__WXMSW__)
    #include  "wx/msw/private.h"  // includes windows.h for LOGFONT
    #include  "wx/msw/winundef.h"
#endif

#include "wx/fontutil.h" // for wxNativeFontInfo
#include "wx/fontmap.h"
#include "wx/fontenum.h"

#include "wx/tokenzr.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

static inline int flags2Style(int flags)
{
    return flags & wxFONTFLAG_ITALIC
                    ? wxFONTSTYLE_ITALIC
                    : flags & wxFONTFLAG_SLANT
                        ? wxFONTSTYLE_SLANT
                        : wxFONTSTYLE_NORMAL;
}

static inline int flags2Weight(int flags)
{
    return flags & wxFONTFLAG_LIGHT
                    ? wxFONTWEIGHT_LIGHT
                    : flags & wxFONTFLAG_BOLD
                        ? wxFONTWEIGHT_BOLD
                        : wxFONTWEIGHT_NORMAL;
}

static inline bool flags2Underlined(int flags)
{
    return (flags & wxFONTFLAG_UNDERLINED) != 0;
}

// ----------------------------------------------------------------------------
// wxFontBase
// ----------------------------------------------------------------------------

wxFontEncoding wxFontBase::ms_encodingDefault = wxFONTENCODING_SYSTEM;

/* static */
void wxFontBase::SetDefaultEncoding(wxFontEncoding encoding)
{
    // GetDefaultEncoding() should return something != wxFONTENCODING_DEFAULT
    // and, besides, using this value here doesn't make any sense
    wxCHECK_RET( encoding != wxFONTENCODING_DEFAULT,
                 _T("can't set default encoding to wxFONTENCODING_DEFAULT") );

    ms_encodingDefault = encoding;
}

wxFontBase::~wxFontBase()
{
    // this destructor is required for Darwin
}

/* static */
wxFont *wxFontBase::New(int size,
                        wxFontFamily family,
                        wxFontStyle style,
                        wxFontWeight weight,
                        bool underlined,
                        const wxString& face,
                        wxFontEncoding encoding)
{
    return new wxFont(size, family, style, weight, underlined, face, encoding);
}

/* static */
wxFont *wxFontBase::New(const wxSize& pixelSize,
                        wxFontFamily family,
                        wxFontStyle style,
                        wxFontWeight weight,
                        bool underlined,
                        const wxString& face,
                        wxFontEncoding encoding)
{
    return new wxFont(pixelSize, family, style, weight, underlined,
                      face, encoding);
}

/* static */
wxFont *wxFontBase::New(int pointSize,
                        wxFontFamily family,
                        int flags,
                        const wxString& face,
                        wxFontEncoding encoding)
{
    return New(pointSize, family, flags2Style(flags), flags2Weight(flags),
               flags2Underlined(flags), face, encoding);
}

/* static */
wxFont *wxFontBase::New(const wxSize& pixelSize,
                        wxFontFamily family,
                        int flags,
                        const wxString& face,
                        wxFontEncoding encoding)
{
    return New(pixelSize, family, flags2Style(flags), flags2Weight(flags),
               flags2Underlined(flags), face, encoding);
}

/* static */
wxFont *wxFontBase::New(const wxNativeFontInfo& info)
{
    return new wxFont(info);
}

/* static */
wxFont *wxFontBase::New(const wxString& strNativeFontDesc)
{
    wxNativeFontInfo fontInfo;
    if ( !fontInfo.FromString(strNativeFontDesc) )
        return new wxFont(*wxNORMAL_FONT);

    return New(fontInfo);
}

bool wxFontBase::IsFixedWidth() const
{
    return GetFamily() == wxFONTFAMILY_TELETYPE;
}

wxSize wxFontBase::GetPixelSize() const
{
    wxScreenDC dc;
    dc.SetFont(*(wxFont *)this);
    return wxSize(dc.GetCharWidth(), dc.GetCharHeight());
}

bool wxFontBase::IsUsingSizeInPixels() const
{
    return false;
}

void wxFontBase::SetPixelSize( const wxSize& pixelSize )
{
    wxCHECK_RET( pixelSize.GetWidth() >= 0 && pixelSize.GetHeight() > 0,
                 "Negative values for the pixel size or zero pixel height are not allowed" );

    wxScreenDC dc;

    // NOTE: this algorithm for adjusting the font size is used by all
    //       implementations of wxFont except under wxMSW and wxGTK where
    //       native support to font creation using pixel-size is provided.
    
    int largestGood = 0;
    int smallestBad = 0;

    bool initialGoodFound = false;
    bool initialBadFound = false;

    // NB: this assignment was separated from the variable definition
    // in order to fix a gcc v3.3.3 compiler crash
    int currentSize = GetPointSize();
    while (currentSize > 0)
    {
        dc.SetFont(*static_cast<wxFont*>(this));

        // if currentSize (in points) results in a font that is smaller
        // than required by pixelSize it is considered a good size
        // NOTE: the pixel size width may be zero
        if (dc.GetCharHeight() <= pixelSize.GetHeight() &&
                (pixelSize.GetWidth() == 0 ||
                 dc.GetCharWidth() <= pixelSize.GetWidth()))
        {
            largestGood = currentSize;
            initialGoodFound = true;
        }
        else
        {
            smallestBad = currentSize;
            initialBadFound = true;
        }
        if (!initialGoodFound)
        {
            currentSize /= 2;
        }
        else if (!initialBadFound)
        {
            currentSize *= 2;
        }
        else
        {
            int distance = smallestBad - largestGood;
            if (distance == 1)
                break;

            currentSize = largestGood + distance / 2;
        }

        SetPointSize(currentSize);
    }

    if (currentSize != largestGood)
        SetPointSize(largestGood);
}

void wxFontBase::DoSetNativeFontInfo(const wxNativeFontInfo& info)
{
#ifdef wxNO_NATIVE_FONTINFO
    SetPointSize(info.pointSize);
    SetFamily(info.family);
    SetStyle(info.style);
    SetWeight(info.weight);
    SetUnderlined(info.underlined);
    SetFaceName(info.faceName);
    SetEncoding(info.encoding);
#else
    (void)info;
#endif
}

wxString wxFontBase::GetNativeFontInfoDesc() const
{
    wxString fontDesc;
    const wxNativeFontInfo *fontInfo = GetNativeFontInfo();
    if ( fontInfo )
    {
        fontDesc = fontInfo->ToString();
        wxASSERT_MSG(!fontDesc.empty(), wxT("This should be a non-empty string!"));
    }
    else
    {
        wxFAIL_MSG(wxT("Derived class should have created the wxNativeFontInfo!"));
    }

    return fontDesc;
}

wxString wxFontBase::GetNativeFontInfoUserDesc() const
{
    wxString fontDesc;
    const wxNativeFontInfo *fontInfo = GetNativeFontInfo();
    if ( fontInfo )
    {
        fontDesc = fontInfo->ToUserString();
        wxASSERT_MSG(!fontDesc.empty(), wxT("This should be a non-empty string!"));
    }
    else
    {
        wxFAIL_MSG(wxT("Derived class should have created the wxNativeFontInfo!"));
    }

    return fontDesc;
}

bool wxFontBase::SetNativeFontInfo(const wxString& info)
{
    wxNativeFontInfo fontInfo;
    if ( !info.empty() && fontInfo.FromString(info) )
    {
        SetNativeFontInfo(fontInfo);
        return true;
    }

    return false;
}

bool wxFontBase::SetNativeFontInfoUserDesc(const wxString& info)
{
    wxNativeFontInfo fontInfo;
    if ( !info.empty() && fontInfo.FromUserString(info) )
    {
        SetNativeFontInfo(fontInfo);
        return true;
    }

    return false;
}

bool wxFontBase::operator==(const wxFont& font) const
{
    // either it is the same font, i.e. they share the same common data or they
    // have different ref datas but still describe the same font
    return IsSameAs(font) ||
           (
            Ok() == font.Ok() &&
            GetPointSize() == font.GetPointSize() &&
            // in wxGTK1 GetPixelSize() calls GetInternalFont() which uses
            // operator==() resulting in infinite recursion so we can't use it
            // in that port
#if !defined(__WXGTK__) || defined(__WXGTK20__)
            GetPixelSize() == font.GetPixelSize() &&
#endif
            GetFamily() == font.GetFamily() &&
            GetStyle() == font.GetStyle() &&
            GetWeight() == font.GetWeight() &&
            GetUnderlined() == font.GetUnderlined() &&
            GetFaceName().IsSameAs(font.GetFaceName(), false) &&
            GetEncoding() == font.GetEncoding()
           );
}

wxString wxFontBase::GetFamilyString() const
{
    wxCHECK_MSG( Ok(), wxT("wxDEFAULT"), wxT("invalid font") );

    switch ( GetFamily() )
    {
        case wxDECORATIVE:   return wxT("wxDECORATIVE");
        case wxROMAN:        return wxT("wxROMAN");
        case wxSCRIPT:       return wxT("wxSCRIPT");
        case wxSWISS:        return wxT("wxSWISS");
        case wxMODERN:       return wxT("wxMODERN");
        case wxTELETYPE:     return wxT("wxTELETYPE");
        default:             return wxT("wxDEFAULT");
    }
}

wxString wxFontBase::GetStyleString() const
{
    wxCHECK_MSG( Ok(), wxT("wxDEFAULT"), wxT("invalid font") );

    switch ( GetStyle() )
    {
        case wxNORMAL:   return wxT("wxNORMAL");
        case wxSLANT:    return wxT("wxSLANT");
        case wxITALIC:   return wxT("wxITALIC");
        default:         return wxT("wxDEFAULT");
    }
}

wxString wxFontBase::GetWeightString() const
{
    wxCHECK_MSG( Ok(), wxT("wxDEFAULT"), wxT("invalid font") );

    switch ( GetWeight() )
    {
        case wxNORMAL:   return wxT("wxNORMAL");
        case wxBOLD:     return wxT("wxBOLD");
        case wxLIGHT:    return wxT("wxLIGHT");
        default:         return wxT("wxDEFAULT");
    }
}

bool wxFontBase::SetFaceName(const wxString& facename)
{
#if wxUSE_FONTENUM
    if (!wxFontEnumerator::IsValidFacename(facename))
    {
        UnRef();        // make Ok() return false
        return false;
    }
#else // !wxUSE_FONTENUM
    wxUnusedVar(facename);
#endif // wxUSE_FONTENUM/!wxUSE_FONTENUM

    return true;
}


// ----------------------------------------------------------------------------
// wxNativeFontInfo
// ----------------------------------------------------------------------------

// Up to now, there are no native implementations of this function:
void wxNativeFontInfo::SetFaceName(const wxArrayString& facenames)
{
#if wxUSE_FONTENUM
    for (size_t i=0; i < facenames.GetCount(); i++)
    {
        if (wxFontEnumerator::IsValidFacename(facenames[i]))
        {
            SetFaceName(facenames[i]);
            return;
        }
    }

    // set the first valid facename we can find on this system
    wxString validfacename = wxFontEnumerator::GetFacenames().Item(0);
    wxLogTrace(wxT("font"), wxT("Falling back to '%s'"), validfacename.c_str());
    SetFaceName(validfacename);
#else // !wxUSE_FONTENUM
    SetFaceName(facenames[0]);
#endif // wxUSE_FONTENUM/!wxUSE_FONTENUM
}


#ifdef wxNO_NATIVE_FONTINFO

// These are the generic forms of FromString()/ToString.
//
// convert to/from the string representation: format is
//      version;pointsize;family;style;weight;underlined;facename;encoding

bool wxNativeFontInfo::FromString(const wxString& s)
{
    long l;

    wxStringTokenizer tokenizer(s, _T(";"));

    wxString token = tokenizer.GetNextToken();
    //
    //  Ignore the version for now
    //

    token = tokenizer.GetNextToken();
    if ( !token.ToLong(&l) )
        return false;
    pointSize = (int)l;

    token = tokenizer.GetNextToken();
    if ( !token.ToLong(&l) )
        return false;
    family = (wxFontFamily)l;

    token = tokenizer.GetNextToken();
    if ( !token.ToLong(&l) )
        return false;
    style = (wxFontStyle)l;

    token = tokenizer.GetNextToken();
    if ( !token.ToLong(&l) )
        return false;
    weight = (wxFontWeight)l;

    token = tokenizer.GetNextToken();
    if ( !token.ToLong(&l) )
        return false;
    underlined = l != 0;

    faceName = tokenizer.GetNextToken();

#ifndef __WXMAC__
    if( !faceName )
        return false;
#endif

    token = tokenizer.GetNextToken();
    if ( !token.ToLong(&l) )
        return false;
    encoding = (wxFontEncoding)l;

    return true;
}

wxString wxNativeFontInfo::ToString() const
{
    wxString s;

    s.Printf(_T("%d;%d;%d;%d;%d;%d;%s;%d"),
             0,                                 // version
             pointSize,
             family,
             (int)style,
             (int)weight,
             underlined,
             faceName.GetData(),
             (int)encoding);

    return s;
}

void wxNativeFontInfo::Init()
{
    pointSize = 0;
    family = wxFONTFAMILY_DEFAULT;
    style = wxFONTSTYLE_NORMAL;
    weight = wxFONTWEIGHT_NORMAL;
    underlined = false;
    faceName.clear();
    encoding = wxFONTENCODING_DEFAULT;
}

int wxNativeFontInfo::GetPointSize() const
{
    return pointSize;
}

wxFontStyle wxNativeFontInfo::GetStyle() const
{
    return style;
}

wxFontWeight wxNativeFontInfo::GetWeight() const
{
    return weight;
}

bool wxNativeFontInfo::GetUnderlined() const
{
    return underlined;
}

wxString wxNativeFontInfo::GetFaceName() const
{
    return faceName;
}

wxFontFamily wxNativeFontInfo::GetFamily() const
{
    return family;
}

wxFontEncoding wxNativeFontInfo::GetEncoding() const
{
    return encoding;
}

void wxNativeFontInfo::SetPointSize(int pointsize)
{
    pointSize = pointsize;
}

void wxNativeFontInfo::SetStyle(wxFontStyle style_)
{
    style = style_;
}

void wxNativeFontInfo::SetWeight(wxFontWeight weight_)
{
    weight = weight_;
}

void wxNativeFontInfo::SetUnderlined(bool underlined_)
{
    underlined = underlined_;
}

bool wxNativeFontInfo::SetFaceName(const wxString& facename_)
{
    faceName = facename_;
    return true;
}

void wxNativeFontInfo::SetFamily(wxFontFamily family_)
{
    family = family_;
}

void wxNativeFontInfo::SetEncoding(wxFontEncoding encoding_)
{
    encoding = encoding_;
}

#endif // generic wxNativeFontInfo implementation

// conversion to/from user-readable string: this is used in the generic
// versions and under MSW as well because there is no standard font description
// format there anyhow (but there is a well-defined standard for X11 fonts used
// by wxGTK and wxMotif)

#if defined(wxNO_NATIVE_FONTINFO) || defined(__WXMSW__) || defined (__WXPM__)

wxString wxNativeFontInfo::ToUserString() const
{
    wxString desc;

    // first put the adjectives, if any - this is English-centric, of course,
    // but what else can we do?
    if ( GetUnderlined() )
    {
        desc << _("underlined");
    }

    switch ( GetWeight() )
    {
        default:
            wxFAIL_MSG( _T("unknown font weight") );
            // fall through

        case wxFONTWEIGHT_NORMAL:
            break;

        case wxFONTWEIGHT_LIGHT:
            desc << _(" light");
            break;

        case wxFONTWEIGHT_BOLD:
            desc << _(" bold");
            break;
    }

    switch ( GetStyle() )
    {
        default:
            wxFAIL_MSG( _T("unknown font style") );
            // fall through

        case wxFONTSTYLE_NORMAL:
            break;

            // we don't distinguish between the two for now anyhow...
        case wxFONTSTYLE_ITALIC:
        case wxFONTSTYLE_SLANT:
            desc << _(" italic");
            break;
    }

    wxString face = GetFaceName();
    if ( !face.empty() )
    {
        desc << _T(' ') << face;
    }

    int size = GetPointSize();
    if ( size != wxNORMAL_FONT->GetPointSize() )
    {
        desc << _T(' ') << size;
    }

#if wxUSE_FONTMAP
    wxFontEncoding enc = GetEncoding();
    if ( enc != wxFONTENCODING_DEFAULT && enc != wxFONTENCODING_SYSTEM )
    {
        desc << _T(' ') << wxFontMapper::GetEncodingName(enc);
    }
#endif // wxUSE_FONTMAP

    return desc.Strip(wxString::both).MakeLower();
}

bool wxNativeFontInfo::FromUserString(const wxString& s)
{
    // reset to the default state
    Init();

    // parse a more or less free form string
    //
    // TODO: we should handle at least the quoted facenames
    wxStringTokenizer tokenizer(s, _T(";, "), wxTOKEN_STRTOK);

    wxString face;
    unsigned long size;
    bool weightfound = false, pointsizefound = false;
#if wxUSE_FONTMAP
    bool encodingfound = false;
#endif

    while ( tokenizer.HasMoreTokens() )
    {
        wxString token = tokenizer.GetNextToken();

        // normalize it
        token.Trim(true).Trim(false).MakeLower();

        // look for the known tokens
        if ( token == _T("underlined") || token == _("underlined") )
        {
            SetUnderlined(true);
        }
        else if ( token == _T("light") || token == _("light") )
        {
            SetWeight(wxFONTWEIGHT_LIGHT);
            weightfound = true;
        }
        else if ( token == _T("bold") || token == _("bold") )
        {
            SetWeight(wxFONTWEIGHT_BOLD);
            weightfound = true;
        }
        else if ( token == _T("italic") || token == _("italic") )
        {
            SetStyle(wxFONTSTYLE_ITALIC);
        }
        else if ( token.ToULong(&size) )
        {
            SetPointSize(size);
            pointsizefound = true;
        }
        else
        {
#if wxUSE_FONTMAP
            // try to interpret this as an encoding
            wxFontEncoding encoding = wxFontMapper::Get()->CharsetToEncoding(token, false);
            if ( encoding != wxFONTENCODING_DEFAULT &&
                 encoding != wxFONTENCODING_SYSTEM )    // returned when the recognition failed
        {
            SetEncoding(encoding);
                encodingfound = true;
        }
            else
        {
#endif // wxUSE_FONTMAP

                // assume it is the face name
            if ( !face.empty() )
            {
                face += _T(' ');
            }

            face += token;

            // skip the code which resets face below
            continue;

#if wxUSE_FONTMAP
        }
#endif // wxUSE_FONTMAP
        }

        // if we had had the facename, we shouldn't continue appending tokens
        // to it (i.e. "foo bold bar" shouldn't result in the facename "foo
        // bar")
        if ( !face.empty() )
        {
            // NB: the check on the facename is implemented in wxFontBase::SetFaceName
            //     and not in wxNativeFontInfo::SetFaceName thus we need to explicitely
            //     call here wxFontEnumerator::IsValidFacename
            if (
#if wxUSE_FONTENUM
                    !wxFontEnumerator::IsValidFacename(face) ||
#endif // wxUSE_FONTENUM
                    !SetFaceName(face) )
            {
                SetFaceName(wxNORMAL_FONT->GetFaceName());
            }

            face.clear();
        }
    }

    // we might not have flushed it inside the loop
    if ( !face.empty() )
    {
        // NB: the check on the facename is implemented in wxFontBase::SetFaceName
        //     and not in wxNativeFontInfo::SetFaceName thus we need to explicitely
        //     call here wxFontEnumerator::IsValidFacename
        if (
#if wxUSE_FONTENUM
                !wxFontEnumerator::IsValidFacename(face) ||
#endif // wxUSE_FONTENUM
                !SetFaceName(face) )
            {
                SetFaceName(wxNORMAL_FONT->GetFaceName());
            }
    }

    // set point size to default value if size was not given
    if ( !pointsizefound )
        SetPointSize(wxNORMAL_FONT->GetPointSize());

    // set font weight to default value if weight was not given
    if ( !weightfound )
        SetWeight(wxFONTWEIGHT_NORMAL);

#if wxUSE_FONTMAP
    // set font encoding to default value if encoding was not given
    if ( !encodingfound )
        SetEncoding(wxFONTENCODING_SYSTEM);
#endif // wxUSE_FONTMAP

    return true;
}

#endif // generic or wxMSW or wxOS2


// wxFont <-> wxString utilities, used by wxConfig
wxString wxToString(const wxFontBase& font)
{
    return font.IsOk() ? font.GetNativeFontInfoDesc()
                       : wxString();
}

bool wxFromString(const wxString& str, wxFontBase *font)
{
    wxCHECK_MSG( font, false, _T("NULL output parameter") );

    if ( str.empty() )
    {
        *font = wxNullFont;
        return true;
    }

    return font->SetNativeFontInfo(str);
}


