/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/font.cpp
// Purpose:     wxFont class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/font.h"

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/utils.h"
    #include "wx/intl.h"
    #include "wx/gdicmn.h"
    #include "wx/log.h"
#endif

#include "wx/fontutil.h"
#include "wx/graphics.h"

#include "wx/mac/uma.h"

#ifndef __DARWIN__
#include <ATSUnicode.h>
#endif

#include <map>

IMPLEMENT_DYNAMIC_CLASS(wxFont, wxGDIObject)


class WXDLLEXPORT wxFontRefData: public wxGDIRefData
{
public:
    wxFontRefData()
    {
        Init(10, wxDEFAULT, wxNORMAL, wxNORMAL,
             false, wxT("applicationfont"), wxFONTENCODING_DEFAULT);
    }

    wxFontRefData(const wxFontRefData& data)
    {
        Init(data.m_pointSize, data.m_family, data.m_style, data.m_weight,
             data.m_underlined, data.m_faceName, data.m_encoding);
    }

    wxFontRefData(int size,
                  int family,
                  int style,
                  int weight,
                  bool underlined,
                  const wxString& faceName,
                  wxFontEncoding encoding)
    {
        Init(size, family, style, weight, underlined, faceName, encoding);
    }

#if wxMAC_USE_CORE_TEXT
    wxFontRefData( wxUint32 coreTextFontType );
    wxFontRefData( CTFontRef font );
    wxFontRefData( CTFontDescriptorRef fontdescriptor, int size );
#endif

    virtual ~wxFontRefData();

    void SetNoAntiAliasing( bool no = true ) { m_noAA = no; }

    bool GetNoAntiAliasing() const { return m_noAA; }

    void SetPointSize( int size )
    {
        m_pointSize = size;
        MacInvalidateNativeFont();
    }

    int GetPointSize() const { return m_pointSize; }

    void SetFamily( int family )
    {
        m_family = family;
        MacInvalidateNativeFont();
    }


    int GetFamily() const { return m_family; }

    void SetStyle( int style )
    {
        m_style = style;
        MacInvalidateNativeFont();
    }


    int GetStyle() const { return m_style; }

    void SetWeight( int weight )
    {
        m_weight = weight;
        MacInvalidateNativeFont();
    }


    int GetWeight() const { return m_weight; }

    void SetUnderlined( bool u )
    {
        m_underlined = u;
        MacInvalidateNativeFont();
    }

    bool GetUnderlined() const { return m_underlined; }

    void SetFaceName( const wxString& facename )
    {
        m_faceName = facename;
        MacInvalidateNativeFont();
    }

    const wxString& GetFaceName() const { return m_faceName; }

    void SetEncoding( wxFontEncoding encoding )
    {
        m_encoding = encoding;
        MacInvalidateNativeFont();
    }

    wxFontEncoding GetEncoding() const { return m_encoding; }

    void MacInvalidateNativeFont();

    void MacFindFont();

protected:
    // common part of all ctors
    void Init(int size,
              int family,
              int style,
              int weight,
              bool underlined,
              const wxString& faceName,
              wxFontEncoding encoding);

#if wxMAC_USE_CORE_TEXT
    void Init( CTFontRef font );
#endif
    // font characterstics
    int             m_pointSize;
    int             m_family;
    int             m_style;
    int             m_weight;
    bool            m_underlined;
    wxString        m_faceName;
    wxFontEncoding  m_encoding;
    bool            m_noAA;      // No anti-aliasing

public:
#if wxMAC_USE_ATSU_TEXT
    FMFontFamily    m_macFontFamily;
    FMFontSize      m_macFontSize;
    FMFontStyle     m_macFontStyle;

    // ATSU Font Information

    // this is split into an ATSU font id that may
    // contain some styles (special bold fonts etc) and
    // these are the additional qd styles that are not
    // included in the ATSU font id
    ATSUFontID      m_macATSUFontID;
    FMFontStyle     m_macATSUAdditionalQDStyles ;

    // for true themeing support we must store the correct font
    // information here, as this speeds up and optimizes rendering
    ThemeFontID     m_macThemeFontID ;
#endif
#if wxMAC_USE_CORE_TEXT
    wxCFRef<CTFontRef> m_ctFont;
    wxCFRef<CTFontDescriptorRef> m_ctFontDescriptor;
#endif
#if wxMAC_USE_CORE_TEXT || wxMAC_USE_ATSU_TEXT
    ATSUStyle       m_macATSUStyle ;
#endif
    wxNativeFontInfo  m_info;
};

#define M_FONTDATA ((wxFontRefData*)m_refData)


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxFontRefData
// ----------------------------------------------------------------------------

void wxFontRefData::Init(int pointSize,
    int family,
    int style,
    int weight,
    bool underlined,
    const wxString& faceName,
    wxFontEncoding encoding)
{
    m_style = style;
    m_pointSize = pointSize;
    m_family = family;
    m_style = style;
    m_weight = weight;
    m_underlined = underlined;
    m_faceName = faceName;
    m_encoding = encoding;
    m_noAA = false;
#if wxMAC_USE_ATSU_TEXT
    m_macFontFamily = 0 ;
    m_macFontSize = 0;
    m_macFontStyle = 0;
    m_macATSUFontID = 0;
    m_macATSUAdditionalQDStyles = 0 ;
    m_macThemeFontID = kThemeCurrentPortFont ;
#endif
#if wxMAC_USE_CORE_TEXT || wxMAC_USE_ATSU_TEXT
    m_macATSUStyle = NULL ;
#endif
}

wxFontRefData::~wxFontRefData()
{
#if wxMAC_USE_CORE_TEXT || wxMAC_USE_ATSU_TEXT
    if ( m_macATSUStyle )
    {
        ::ATSUDisposeStyle((ATSUStyle)m_macATSUStyle);
        m_macATSUStyle = NULL ;
    }
#endif
}

void wxFontRefData::MacInvalidateNativeFont()
{
#if wxMAC_USE_CORE_TEXT
    m_ctFont.reset();
    m_ctFontDescriptor.reset();
#endif
#if wxMAC_USE_CORE_TEXT || wxMAC_USE_ATSU_TEXT
    if ( m_macATSUStyle )
    {
        ::ATSUDisposeStyle((ATSUStyle)m_macATSUStyle);
        m_macATSUStyle = NULL ;
    }
#endif
}

#if wxMAC_USE_CORE_TEXT

/* from Core Text Manual Common Operations */

static CTFontDescriptorRef wxMacCreateCTFontDescriptor(CFStringRef iFamilyName, CTFontSymbolicTraits iTraits )
{
    CTFontDescriptorRef descriptor = NULL;
    CFMutableDictionaryRef attributes;

    assert(iFamilyName != NULL);
    // Create a mutable dictionary to hold our attributes.
    attributes = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                           &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    check(attributes != NULL);

    if (attributes != NULL) {
        // Add a family name to our attributes.
        CFDictionaryAddValue(attributes, kCTFontFamilyNameAttribute, iFamilyName);


        if ( iTraits ) {
            CFMutableDictionaryRef traits;
            CFNumberRef symTraits;

            // Create the traits dictionary.
            symTraits = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type,
                                       &iTraits);
            check(symTraits != NULL);

            if (symTraits != NULL) {
                // Create a dictionary to hold our traits values.
                traits = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                   &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                check(traits != NULL);

                if (traits != NULL) {
                    // Add the symbolic traits value to the traits dictionary.
                    CFDictionaryAddValue(traits, kCTFontSymbolicTrait, symTraits);

                    // Add the traits attribute to our attributes.
                    CFDictionaryAddValue(attributes, kCTFontTraitsAttribute, traits);
                    CFRelease(traits);
                }
                CFRelease(symTraits);
            }
        }
        // Create the font descriptor with our attributes
        descriptor = CTFontDescriptorCreateWithAttributes(attributes);
        check(descriptor != NULL);

        CFRelease(attributes);
    }
    // Return our font descriptor.
    return descriptor ;
}

wxFontRefData::wxFontRefData( wxUint32 coreTextFontType )
{
    CTFontRef font = CTFontCreateUIFontForLanguage( coreTextFontType, 0.0, NULL ) ;
    if ( CTFontGetSize(m_ctFont) == 0 )
    {
        CFRelease(font);
        font = CTFontCreateUIFontForLanguage( coreTextFontType, 12.0, NULL );
    }
    Init( font );
}

wxFontRefData::wxFontRefData( CTFontRef font )
{
    Init( font );
}

wxFontRefData::wxFontRefData( CTFontDescriptorRef fontdescriptor, int size )
{
    if ( size == 0 )
    {
        wxCFRef< CFNumberRef > value( (CFNumberRef) CTFontDescriptorCopyAttribute( fontdescriptor, kCTFontSizeAttribute ) );

        float fsize;
        if ( CFNumberGetValue( value , kCFNumberFloatType , &fsize ) )
        {
            size = (int) fsize + 0.5 ;
        }
    }
    Init( CTFontCreateWithFontDescriptor(fontdescriptor, size,NULL)  );
}

void wxFontRefData::Init( CTFontRef font )
{
    Init(10, wxDEFAULT, wxNORMAL, wxNORMAL,
         false, wxT("applicationfont"), wxFONTENCODING_DEFAULT);

    m_ctFont.reset( font );
}

#endif

void wxFontRefData::MacFindFont()
{

#if wxMAC_USE_CORE_TEXT
    if (  UMAGetSystemVersion() >= 0x1050 )
    {
        if ( m_faceName.empty() && m_family == wxDEFAULT )
        {
            m_ctFont.reset(CTFontCreateUIFontForLanguage( kCTFontSystemFontType, 0.0, NULL ));
        }

        if ( m_ctFont )
        {
            wxCFStringRef name( CTFontCopyFamilyName( m_ctFont ) );
            m_faceName = name.AsString();
            m_pointSize = CTFontGetSize(m_ctFont) ;
            CTFontSymbolicTraits traits = CTFontGetSymbolicTraits( m_ctFont );
            if ( traits & kCTFontItalicTrait )
                m_style = wxITALIC;
            if (  traits & kCTFontBoldTrait )
                m_weight = wxBOLD ;
            if ( !m_ctFontDescriptor.get() )
                m_ctFontDescriptor.reset( CTFontCopyFontDescriptor( m_ctFont ) );
        }
        else
        {
            if ( m_faceName.empty() )
            {
                switch ( m_family )
                {
                    case wxSCRIPT :
                    case wxROMAN :
                    case wxDECORATIVE :
                        m_faceName = wxT("Times");
                        break ;

                    case wxSWISS :
                        m_faceName =  wxT("Lucida Grande");
                        break ;

                    case wxMODERN :
                    case wxTELETYPE:
                        m_faceName =  wxT("Monaco");
                        break ;

                    default:
                        m_faceName =  wxT("Times");
                        break ;
                }
            }

            
            CTFontSymbolicTraits traits = 0;

            if (m_weight == wxBOLD)
                traits |= kCTFontBoldTrait;
            if (m_style == wxITALIC || m_style == wxSLANT)
                traits |= kCTFontItalicTrait;
     
// use font descriptor caching
#if 1
            wxString lookupname = wxString::Format( "%s_%ld", m_faceName.c_str(), traits );
            
            static std::map< std::wstring , wxCFRef< CTFontDescriptorRef > > fontdescriptorcache ;
            
            m_ctFontDescriptor = fontdescriptorcache[ std::wstring(lookupname.wc_str()) ];
            if ( !m_ctFontDescriptor )
            {
                wxCFStringRef cf( m_faceName, wxLocale::GetSystemEncoding() );
                m_ctFontDescriptor.reset( wxMacCreateCTFontDescriptor( cf, traits ) );
                fontdescriptorcache[ std::wstring(lookupname.wc_str()) ] = m_ctFontDescriptor;
            }
#else
            wxCFStringRef cf( m_faceName, wxLocale::GetSystemEncoding() );
            m_ctFontDescriptor.reset( wxMacCreateCTFontDescriptor( cf, traits ) );
#endif
            
// use font caching
#if 1
            wxString lookupnameWithSize = wxString::Format( "%s_%ld_%ld", m_faceName.c_str(), traits, m_pointSize );
            
            static std::map< std::wstring , wxCFRef< CTFontRef > > fontcache ;
            m_ctFont = fontcache[ std::wstring(lookupnameWithSize.wc_str()) ];
            if ( !m_ctFont )
            {
                m_ctFont.reset( CTFontCreateWithFontDescriptor( m_ctFontDescriptor, m_pointSize, NULL ) );
                fontcache[ std::wstring(lookupnameWithSize.wc_str()) ] = m_ctFont;
            }
#else
            m_ctFont.reset( CTFontCreateWithFontDescriptor( m_ctFontDescriptor, m_pointSize, NULL ) );
#endif
        }
#if wxMAC_USE_ATSU_TEXT == 0
        OSStatus status = noErr;
        CTFontDescriptorRef desc = m_ctFontDescriptor ;
        ATSFontRef atsfont = CTFontGetPlatformFont( m_ctFont, &desc );
        FMFont fmfont = FMGetFontFromATSFontRef( atsfont );
        ATSUAttributeTag atsuTags[] =
        {
            kATSUFontTag ,
            kATSUSizeTag ,
            kATSUVerticalCharacterTag,
            kATSUQDBoldfaceTag ,
            kATSUQDItalicTag ,
            kATSUQDUnderlineTag ,
        };
        ByteCount atsuSizes[sizeof(atsuTags) / sizeof(ATSUAttributeTag)] =
        {
            sizeof( ATSUFontID ) ,
            sizeof( Fixed ) ,
            sizeof( ATSUVerticalCharacterType),
            sizeof( Boolean ) ,
            sizeof( Boolean ) ,
            sizeof( Boolean ) ,
        };
        Boolean kTrue = true ;
        Boolean kFalse = false ;

        Fixed atsuSize = IntToFixed( m_pointSize );
        ATSUVerticalCharacterType kHorizontal = kATSUStronglyHorizontal;
        ATSUAttributeValuePtr    atsuValues[sizeof(atsuTags) / sizeof(ATSUAttributeTag)] =
        {
            &fmfont ,
            &atsuSize ,
            &kHorizontal,
            (m_weight == wxBOLD) ? &kTrue : &kFalse ,
            (m_style == wxITALIC || m_style == wxSLANT) ? &kTrue : &kFalse ,
            (m_underlined) ? &kTrue : &kFalse ,
        };

        if ( m_macATSUStyle )
        {
            ::ATSUDisposeStyle((ATSUStyle)m_macATSUStyle);
            m_macATSUStyle = NULL ;
        }
        status = ::ATSUCreateStyle((ATSUStyle *)&m_macATSUStyle);
        wxASSERT_MSG( status == noErr , wxT("couldn't create ATSU style") );
        status = ::ATSUSetAttributes(
                                     (ATSUStyle)m_macATSUStyle,
                                     sizeof(atsuTags) / sizeof(ATSUAttributeTag) ,
                                     atsuTags, atsuSizes, atsuValues);
#endif
    }
#endif
#if wxMAC_USE_ATSU_TEXT
    {
        OSStatus status = noErr;
        Str255 qdFontName ;
        if ( m_macThemeFontID != kThemeCurrentPortFont )
        {
            Style style ;
            GetThemeFont( m_macThemeFontID, GetApplicationScript(), qdFontName, &m_macFontSize, &style );
            if ( m_macFontSize == 0 )
                m_macFontSize = 12;
            m_macFontStyle = style ;
            m_faceName = wxMacMakeStringFromPascal( qdFontName );
            if ( m_macFontStyle & bold )
                m_weight = wxBOLD ;
            else
                m_weight = wxNORMAL ;
            if ( m_macFontStyle & italic )
                m_style = wxITALIC ;
            if ( m_macFontStyle & underline )
                m_underlined = true ;
            m_pointSize = m_macFontSize ;
            m_macFontFamily = FMGetFontFamilyFromName( qdFontName );
        }
        else
        {
            if ( m_faceName.empty() )
            {
                if ( m_family == wxDEFAULT )
                {
                    m_macFontFamily = GetAppFont();
                    FMGetFontFamilyName(m_macFontFamily,qdFontName);
                    m_faceName = wxMacMakeStringFromPascal( qdFontName );
                }
                else
                {
                    switch ( m_family )
                    {
                        case wxSCRIPT :
                        case wxROMAN :
                        case wxDECORATIVE :
                            m_faceName = wxT("Times");
                            break ;

                        case wxSWISS :
                            m_faceName =  wxT("Lucida Grande");
                            break ;

                        case wxMODERN :
                        case wxTELETYPE:
                            m_faceName =  wxT("Monaco");
                            break ;

                        default:
                            m_faceName =  wxT("Times");
                            break ;
                    }
                    wxMacStringToPascal( m_faceName , qdFontName );
                    m_macFontFamily = FMGetFontFamilyFromName( qdFontName );
                    if ( m_macFontFamily == kInvalidFontFamily )
                    {
                        wxLogDebug( wxT("ATSFontFamilyFindFromName failed for %s"), m_faceName.c_str() );
                        m_macFontFamily = GetAppFont();
                    }
                }
            }
            else
            {
                if ( m_faceName == wxT("systemfont") )
                    m_macFontFamily = GetSysFont();
                else if ( m_faceName == wxT("applicationfont") )
                    m_macFontFamily = GetAppFont();
                else
                {
                    wxCFStringRef cf( m_faceName, wxLocale::GetSystemEncoding() );
                    ATSFontFamilyRef atsfamily = ATSFontFamilyFindFromName( cf , kATSOptionFlagsDefault );
                    if ( atsfamily == (ATSFontFamilyRef) -1 )
                    {
                        wxLogDebug( wxT("ATSFontFamilyFindFromName failed for ") + m_faceName );
                        m_macFontFamily = GetAppFont();
                    }
                    else
                        m_macFontFamily = FMGetFontFamilyFromATSFontFamilyRef( atsfamily );
                }
            }

            m_macFontStyle = 0;
            if (m_weight == wxBOLD)
                m_macFontStyle |= bold;
            if (m_style == wxITALIC || m_style == wxSLANT)
                m_macFontStyle |= italic;
            if (m_underlined)
                m_macFontStyle |= underline;
            m_macFontSize = m_pointSize ;
        }

        // we try to get as much styles as possible into ATSU


        // ATSUFontID and FMFont are equivalent
        FMFontStyle intrinsicStyle = 0 ;
        status = FMGetFontFromFontFamilyInstance( m_macFontFamily , m_macFontStyle , &m_macATSUFontID , &intrinsicStyle);
        wxASSERT_MSG( status == noErr , wxT("couldn't get an ATSUFont from font family") );
        m_macATSUAdditionalQDStyles = m_macFontStyle & (~intrinsicStyle );

        if ( m_macATSUStyle )
        {
            ::ATSUDisposeStyle((ATSUStyle)m_macATSUStyle);
            m_macATSUStyle = NULL ;
        }

        status = ::ATSUCreateStyle((ATSUStyle *)&m_macATSUStyle);
        wxASSERT_MSG( status == noErr , wxT("couldn't create ATSU style") );

        ATSUAttributeTag atsuTags[] =
        {
            kATSUFontTag ,
            kATSUSizeTag ,
            kATSUVerticalCharacterTag,
            kATSUQDBoldfaceTag ,
            kATSUQDItalicTag ,
            kATSUQDUnderlineTag ,
            kATSUQDCondensedTag ,
            kATSUQDExtendedTag ,
        };
        ByteCount atsuSizes[sizeof(atsuTags) / sizeof(ATSUAttributeTag)] =
        {
            sizeof( ATSUFontID ) ,
            sizeof( Fixed ) ,
            sizeof( ATSUVerticalCharacterType),
            sizeof( Boolean ) ,
            sizeof( Boolean ) ,
            sizeof( Boolean ) ,
            sizeof( Boolean ) ,
            sizeof( Boolean ) ,
        };

        Boolean kTrue = true ;
        Boolean kFalse = false ;

        Fixed atsuSize = IntToFixed( m_macFontSize );
        ATSUVerticalCharacterType kHorizontal = kATSUStronglyHorizontal;
        ATSUAttributeValuePtr    atsuValues[sizeof(atsuTags) / sizeof(ATSUAttributeTag)] =
        {
            &m_macATSUFontID ,
            &atsuSize ,
            &kHorizontal,
            (m_macATSUAdditionalQDStyles & bold) ? &kTrue : &kFalse ,
            (m_macATSUAdditionalQDStyles & italic) ? &kTrue : &kFalse ,
            (m_macATSUAdditionalQDStyles & underline) ? &kTrue : &kFalse ,
            (m_macATSUAdditionalQDStyles & condense) ? &kTrue : &kFalse ,
            (m_macATSUAdditionalQDStyles & extend) ? &kTrue : &kFalse ,
        };

        status = ::ATSUSetAttributes(
                                     (ATSUStyle)m_macATSUStyle,
                                     sizeof(atsuTags) / sizeof(ATSUAttributeTag) ,
                                     atsuTags, atsuSizes, atsuValues);

        wxASSERT_MSG( status == noErr , wxT("couldn't modify ATSU style") );
        return;
    }
#endif
}

// ----------------------------------------------------------------------------
// wxFont
// ----------------------------------------------------------------------------

bool wxFont::Create(const wxNativeFontInfo& info)
{
    return Create(
        info.pointSize, info.family, info.style, info.weight,
        info.underlined, info.faceName, info.encoding );
}

wxFont::wxFont(const wxString& fontdesc)
{
    wxNativeFontInfo info;
    if ( info.FromString(fontdesc) )
        (void)Create(info);
}

bool wxFont::Create(int pointSize,
    int family,
    int style,
    int weight,
    bool underlined,
    const wxString& faceName,
    wxFontEncoding encoding)
{
    UnRef();

    m_refData = new wxFontRefData(
        pointSize, family, style, weight,
        underlined, faceName, encoding);

    RealizeResource();

    return true;
}

#if wxMAC_USE_CORE_TEXT

bool wxFont::MacCreateFromUIFont(wxUint32 ctFontType )
{
    UnRef();

    m_refData = new wxFontRefData(ctFontType);
    RealizeResource();

    return true;
}

bool wxFont::MacCreateFromCTFontDescriptor( const void * ctFontDescriptor , int size )
{
    UnRef();

    m_refData = new wxFontRefData((CTFontDescriptorRef)ctFontDescriptor, size);;
    RealizeResource();

    return true;
}


#endif

bool wxFont::MacCreateFromThemeFont(wxUint16 themeFontID)
{
#if wxMAC_USE_CORE_TEXT
    if ( UMAGetSystemVersion() >= 0x1050)
    {
        return MacCreateFromUIFont(HIThemeGetUIFontType(themeFontID));
    }
#endif
#if wxMAC_USE_ATSU_TEXT
    {
        UnRef();

        m_refData = new wxFontRefData(
                                      12, wxDEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL,
                                      false, wxEmptyString, wxFONTENCODING_DEFAULT );

        M_FONTDATA->m_macThemeFontID = themeFontID ;
        RealizeResource();
        return true;
    }
#endif
    return false;
}

wxFont::~wxFont()
{
}

bool wxFont::RealizeResource()
{
    M_FONTDATA->MacFindFont();

    return true;
}

void wxFont::SetEncoding(wxFontEncoding encoding)
{
    Unshare();

    M_FONTDATA->SetEncoding( encoding );

    RealizeResource();
}

void wxFont::Unshare()
{
    // Don't change shared data
    if (!m_refData)
    {
        m_refData = new wxFontRefData();
    }
    else
    {
        wxFontRefData* ref = new wxFontRefData(*(wxFontRefData*)m_refData);
        UnRef();
        m_refData = ref;
    }
}

wxGDIRefData *wxFont::CreateGDIRefData() const
{
    return new wxFontRefData;
}

wxGDIRefData *wxFont::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxFontRefData(*wx_static_cast(const wxFontRefData *, data));
}

void wxFont::SetPointSize(int pointSize)
{
    if ( M_FONTDATA->GetPointSize() == pointSize )
        return;

    Unshare();

    M_FONTDATA->SetPointSize( pointSize );

    RealizeResource();
}

void wxFont::SetFamily(int family)
{
    Unshare();

    M_FONTDATA->SetFamily( family );

    RealizeResource();
}

void wxFont::SetStyle(int style)
{
    Unshare();

    M_FONTDATA->SetStyle( style );

    RealizeResource();
}

void wxFont::SetWeight(int weight)
{
    Unshare();

    M_FONTDATA->SetWeight( weight );

    RealizeResource();
}

bool wxFont::SetFaceName(const wxString& faceName)
{
    Unshare();

    M_FONTDATA->SetFaceName( faceName );

    RealizeResource();

    return wxFontBase::SetFaceName(faceName);
}

void wxFont::SetUnderlined(bool underlined)
{
    Unshare();

    M_FONTDATA->SetUnderlined( underlined );

    RealizeResource();
}

void wxFont::SetNoAntiAliasing( bool no )
{
    Unshare();

    M_FONTDATA->SetNoAntiAliasing( no );

    RealizeResource();
}

// ----------------------------------------------------------------------------
// accessors
// ----------------------------------------------------------------------------

// TODO: insert checks everywhere for M_FONTDATA == NULL!

int wxFont::GetPointSize() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->GetPointSize();
}

wxSize wxFont::GetPixelSize() const
{
#if wxUSE_GRAPHICS_CONTEXT
    // TODO: consider caching the value
    wxGraphicsContext* dc = wxGraphicsContext::CreateFromNative((CGContextRef) NULL);
    dc->SetFont(*(wxFont *)this,*wxBLACK);
    wxDouble width, height = 0;
    dc->GetTextExtent( wxT("g"), &width, &height, NULL, NULL);
    delete dc;
    return wxSize((int)width, (int)height);
#else
    return wxFontBase::GetPixelSize();
#endif
}

int wxFont::GetFamily() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->GetFamily();
}

int wxFont::GetStyle() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->GetStyle() ;
}

int wxFont::GetWeight() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->GetWeight();
}

bool wxFont::GetUnderlined() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , false, wxT("invalid font") );

    return M_FONTDATA->GetUnderlined();
}

wxString wxFont::GetFaceName() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , wxEmptyString , wxT("invalid font") );

    return M_FONTDATA->GetFaceName() ;
}

wxFontEncoding wxFont::GetEncoding() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , wxFONTENCODING_DEFAULT , wxT("invalid font") );

    return M_FONTDATA->GetEncoding() ;
}

bool wxFont::GetNoAntiAliasing() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , false, wxT("invalid font") );

    return M_FONTDATA->GetNoAntiAliasing();
}

#if wxMAC_USE_ATSU_TEXT

short wxFont::MacGetFontNum() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->m_macFontFamily;
}

short wxFont::MacGetFontSize() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->m_macFontSize;
}

wxByte wxFont::MacGetFontStyle() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->m_macFontStyle;
}

wxUint32 wxFont::MacGetATSUFontID() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->m_macATSUFontID;
}

wxUint32 wxFont::MacGetATSUAdditionalQDStyles() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->m_macATSUAdditionalQDStyles;
}

wxUint16 wxFont::MacGetThemeFontID() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return M_FONTDATA->m_macThemeFontID;
}
#endif

#if wxMAC_USE_CORE_TEXT || wxMAC_USE_ATSU_TEXT
void * wxFont::MacGetATSUStyle() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , NULL, wxT("invalid font") );

    return M_FONTDATA->m_macATSUStyle;
}
#endif

#if wxMAC_USE_CORE_TEXT

const void * wxFont::MacGetCTFont() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return (CTFontRef)(M_FONTDATA->m_ctFont);
}

const void * wxFont::MacGetCTFontDescriptor() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , 0, wxT("invalid font") );

    return (CTFontDescriptorRef)(M_FONTDATA->m_ctFontDescriptor);
}

#endif

const wxNativeFontInfo * wxFont::GetNativeFontInfo() const
{
    wxCHECK_MSG( M_FONTDATA != NULL , NULL, wxT("invalid font") );
    wxCHECK_MSG( Ok(), NULL, wxT("invalid font") );

    M_FONTDATA->m_info.InitFromFont(*this);

    return &(M_FONTDATA->m_info);
}
