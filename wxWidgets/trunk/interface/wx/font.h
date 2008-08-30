/////////////////////////////////////////////////////////////////////////////
// Name:        font.h
// Purpose:     interface of wxFont
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxFont

    A font is an object which determines the appearance of text. Fonts are
    used for drawing text to a device context, and setting the appearance of
    a window's text.

    This class uses @ref overview_trefcount "reference counting and copy-on-write"
    internally so that assignments between two instances of this class are very
    cheap. You can therefore use actual objects instead of pointers without
    efficiency problems. If an instance of this class is changed it will create
    its own data internally so that other instances, which previously shared the
    data using the reference counting, are not affected.

    You can retrieve the current system font settings with wxSystemSettings.

    wxSystemSettings

    @library{wxcore}
    @category{gdi}

    @stdobjects
    ::wxNullFont, ::wxNORMAL_FONT, ::wxSMALL_FONT, ::wxITALIC_FONT, ::wxSWISS_FONT

    @see @ref overview_wxfontoverview, wxDC::SetFont, wxDC::DrawText,
    wxDC::GetTextExtent, wxFontDialog, wxSystemSettings
*/
class wxFont : public wxGDIObject
{
public:
    //@{
    /**
        Creates a font object.
    */
    wxFont();

    /**
        Creates a font object with the specified font.
        
        @param font
           font object.     
    */
    wxFont(const wxFont& font);

    /**
        Creates a font object with the specified attributes.

        @param pointSize
            Size in points.
        @param family
            Font family, a generic way of referring to fonts without specifying actual
        facename. One of:
        <TABLE>
            <TR><TD>wxFONTFAMILY_DEFAULT</TD><TD>Chooses a default font.</TD></TR>
            <TR><TD>wxFONTFAMILY_DECORATIVE</TD><TD>A decorative font.</TD></TR>
            <TR><TD>wxFONTFAMILY_ROMAN</TD><TD>A formal, serif font.</TD></TR>
            <TR><TD>wxFONTFAMILY_SCRIPT</TD><TD>A handwriting font.</TD></TR>
            <TR><TD>wxFONTFAMILY_SWISS</TD><TD>A sans-serif font.</TD></TR>
            <TR><TD>wxFONTFAMILY_MODERN</TD><TD>A fixed pitch font.</TD></TR>
            <TR><TD>wxFONTFAMILY_TELETYPE</TD><TD>A teletype font.</TD></TR>
        </TABLE>
        @param style
            One of wxFONTSTYLE_NORMAL, wxFONTSTYLE_SLANT and wxFONTSTYLE_ITALIC.
        @param weight
            Font weight, sometimes also referred to as font boldness. One of:
        <TABLE>
            <TR><TD>wxFONTWEIGHT_NORMAL</TD><TD>Normal font.</TD></TR>
            <TR><TD>wxFONTWEIGHT_LIGHT</TD><TD>Light font.</TD></TR>
            <TR><TD>wxFONTWEIGHT_BOLD</TD><TD>Bold font.</TD></TR>
        </TABLE>    
        @param underline
            The value can be @true or @false. At present this has an effect on Windows
        and Motif 2.x only.
        @param faceName
            An optional string specifying the actual typeface to be used. If it is an
        empty string,
            a default typeface will be chosen based on the family.
        @param encoding
            An encoding which may be one of
        <TABLE>
            <TR><TD>wxFONTENCODING_SYSTEM</TD><TD>Default system encoding.</TD></TR>
            <TR><TD>wxFONTENCODING_DEFAULT</TD><TD>
                Default application encoding: this
                is the encoding set by calls to
                SetDefaultEncoding and which may be set to,
                say, KOI8 to create all fonts by default with KOI8 encoding. Initially, the
                default application encoding is the same as default system encoding.</TD></TR>
            <TR><TD>wxFONTENCODING_ISO8859_1...15</TD><TD>ISO8859 encodings.</TD></TR>
            <TR><TD>wxFONTENCODING_KOI8</TD><TD>The standard Russian encoding for Internet.</TD></TR>
            <TR><TD>wxFONTENCODING_CP1250...1252</TD><TD>Windows encodings similar to ISO8859 (but not identical).</TD></TR>
        </TABLE> 

            If the specified encoding isn't available, no font is created
            (see also font encoding overview).

        @remarks If the desired font does not exist, the closest match will be
                 chosen. Under Windows, only scalable TrueType fonts are
                 used.
    */
    wxFont(int pointSize, wxFontFamily family, int style,
           wxFontWeight weight,
           const bool underline = false,
           const wxString& faceName = "",
           wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    /**
        Creates a font object with the specified attributes.

        @param pixelSize
            Size in pixels: this is directly supported only under MSW
            currently where this constructor can be used directly, under other
        platforms a
            font with the closest size to the given one is found using binary search and
            the static New method must be used.
        @param family
            Font family, a generic way of referring to fonts without specifying actual
        facename. One of:
        <TABLE>
            <TR><TD>wxFONTFAMILY_DEFAULT</TD><TD>Chooses a default font.</TD></TR>
            <TR><TD>wxFONTFAMILY_DECORATIVE</TD><TD>A decorative font.</TD></TR>
            <TR><TD>wxFONTFAMILY_ROMAN</TD><TD>A formal, serif font.</TD></TR>
            <TR><TD>wxFONTFAMILY_SCRIPT</TD><TD>A handwriting font.</TD></TR>
            <TR><TD>wxFONTFAMILY_SWISS</TD><TD>A sans-serif font.</TD></TR>
            <TR><TD>wxFONTFAMILY_MODERN</TD><TD>A fixed pitch font.</TD></TR>
            <TR><TD>wxFONTFAMILY_TELETYPE</TD><TD>A teletype font.</TD></TR>
        </TABLE>
        @param style
            One of wxFONTSTYLE_NORMAL, wxFONTSTYLE_SLANT and wxFONTSTYLE_ITALIC.
        @param weight
            Font weight, sometimes also referred to as font boldness. One of:
        <TABLE>
            <TR><TD>wxFONTWEIGHT_NORMAL</TD><TD>Normal font.</TD></TR>
            <TR><TD>wxFONTWEIGHT_LIGHT</TD><TD>Light font.</TD></TR>
            <TR><TD>wxFONTWEIGHT_BOLD</TD><TD>Bold font.</TD></TR>
        </TABLE>    
        @param underline
            The value can be @true or @false. At present this has an effect on Windows
        and Motif 2.x only.
        @param faceName
            An optional string specifying the actual typeface to be used. If it is an
        empty string,
            a default typeface will be chosen based on the family.
        @param encoding
            An encoding which may be one of
        <TABLE>
            <TR><TD>wxFONTENCODING_SYSTEM</TD><TD>Default system encoding.</TD></TR>
            <TR><TD>wxFONTENCODING_DEFAULT</TD><TD>
                Default application encoding: this
                is the encoding set by calls to
                SetDefaultEncoding and which may be set to,
                say, KOI8 to create all fonts by default with KOI8 encoding. Initially, the
                default application encoding is the same as default system encoding.</TD></TR>
            <TR><TD>wxFONTENCODING_ISO8859_1...15</TD><TD>ISO8859 encodings.</TD></TR>
            <TR><TD>wxFONTENCODING_KOI8</TD><TD>The standard Russian encoding for Internet.</TD></TR>
            <TR><TD>wxFONTENCODING_CP1250...1252</TD><TD>Windows encodings similar to ISO8859 (but not identical).</TD></TR>
        </TABLE> 

            If the specified encoding isn't available, no font is created
            (see also font encoding overview).

        @remarks If the desired font does not exist, the closest match will be
                 chosen. Under Windows, only scalable TrueType fonts are
                 used.
    */
    wxFont(const wxSize& pixelSize, wxFontFamily family,
           int style, wxFontWeight weight,
           const bool underline = false,
           const wxString& faceName = "",
           wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    //@}

    /**
        Destructor.
        See @ref overview_refcountdestruct "reference-counted object destruction" for
        more info.

        @remarks Although all remaining fonts are deleted when the application
                 exits, the application should try to clean up all fonts
                 itself. This is because wxWidgets cannot know if a
                 pointer to the font object is stored in an application
                 data structure, and there is a risk of double deletion.
    */
    ~wxFont();

    /**
        Returns the current application's default encoding.

        @see @ref overview_wxfontencodingoverview, SetDefaultEncoding()
    */
    static wxFontEncoding GetDefaultEncoding();

    /**
        Returns the typeface name associated with the font, or the empty string if
        there is no
        typeface information.

        @see SetFaceName()
    */
    wxString GetFaceName() const;

    /**
        Gets the font family. See SetFamily() for a list of valid
        family identifiers.

        @see SetFamily()
    */
    wxFontFamily GetFamily() const;

    /**
        Returns the platform-dependent string completely describing this font.
        Returned string is always non-empty.
        Note that the returned string is not meant to be shown or edited by the user: a
        typical
        use of this function is for serializing in string-form a wxFont object.

        @see SetNativeFontInfo(),GetNativeFontInfoUserDesc()
    */
    wxString GetNativeFontInfoDesc() const;

    /**
        Returns a user-friendly string for this font object. Returned string is always
        non-empty.
        Some examples of the formats of returned strings (which are platform-dependent)
        are in SetNativeFontInfoUserDesc().

        @see GetNativeFontInfoDesc()
    */
    wxString GetNativeFontInfoUserDesc();

    /**
        Gets the point size.

        @see SetPointSize()
    */
    int GetPointSize() const;

    /**
        Gets the font style. See wxFont() for a list of valid
        styles.

        @see SetStyle()
    */
    int GetStyle() const;

    /**
        Returns @true if the font is underlined, @false otherwise.

        @see SetUnderlined()
    */
    bool GetUnderlined() const;

    /**
        Gets the font weight. See wxFont() for a list of valid
        weight identifiers.

        @see SetWeight()
    */
    wxFontWeight GetWeight() const;

    /**
        Returns @true if the font is a fixed width (or monospaced) font,
        @false if it is a proportional one or font is invalid.
    */
    bool IsFixedWidth() const;

    /**
        Returns @true if this object is a valid font, @false otherwise.
    */
    bool IsOk() const;

    //@{
    /**
        These functions take the same parameters as @ref ctor() wxFont
        constructor and return a new font object allocated on the heap.
        Using @c New() is currently the only way to directly create a font with
        the given size in pixels on platforms other than wxMSW.
    */
    static wxFont* New(int pointSize, wxFontFamily family, int style,
                       wxFontWeight weight,
                       const bool underline = false,
                       const wxString& faceName = "",
                       wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    static wxFont* New(int pointSize, wxFontFamily family,
                       int flags = wxFONTFLAG_DEFAULT,
                       const wxString& faceName = "",
                       wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    static wxFont* New(const wxSize& pixelSize,
                       wxFontFamily family,
                       int style,
                       wxFontWeight weight,
                       const bool underline = false,
                       const wxString& faceName = "",
                       wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    static wxFont* New(const wxSize& pixelSize,
                       wxFontFamily family,
                       int flags = wxFONTFLAG_DEFAULT,
                       const wxString& faceName = "",
                       wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    //@}

    /**
        Sets the default font encoding.

        @see @ref overview_wxfontencodingoverview, GetDefaultEncoding()
    */
    static void SetDefaultEncoding(wxFontEncoding encoding);

    /**
        Sets the facename for the font.
        Returns @true if the given face name exists; @false otherwise.

        @param faceName
            A valid facename, which should be on the end-user's system.

        @remarks To avoid portability problems, don't rely on a specific face,
                 but specify the font family instead or as well. A
                 suitable font will be found on the end-user's system.
                 If both the family and the facename are specified,
                 wxWidgets will first search for the specific face, and
                 then for a font belonging to the same family.

        @see GetFaceName(), SetFamily()
    */
    bool SetFaceName(const wxString& faceName);

    /**
        Sets the font family.

        @param family
            One of:
        <TABLE>
            <TR><TD>wxFONTFAMILY_DEFAULT</TD><TD>Chooses a default font.</TD></TR>
            <TR><TD>wxFONTFAMILY_DECORATIVE</TD><TD>A decorative font.</TD></TR>
            <TR><TD>wxFONTFAMILY_ROMAN</TD><TD>A formal, serif font.</TD></TR>
            <TR><TD>wxFONTFAMILY_SCRIPT</TD><TD>A handwriting font.</TD></TR>
            <TR><TD>wxFONTFAMILY_SWISS</TD><TD>A sans-serif font.</TD></TR>
            <TR><TD>wxFONTFAMILY_MODERN</TD><TD>A fixed pitch font.</TD></TR>
            <TR><TD>wxFONTFAMILY_TELETYPE</TD><TD>A teletype font.</TD></TR>

        @see GetFamily(), SetFaceName()
    */
    void SetFamily(wxFontFamily family);

    /**
        Creates the font corresponding to the given native font description string and
        returns @true if
        the creation was successful.
        which must have been previously returned by
        GetNativeFontInfoDesc(). If the string is
        invalid, font is unchanged. This function is typically used for de-serializing
        a wxFont
        object previously saved in a string-form.

        @see SetNativeFontInfoUserDesc()
    */
    bool SetNativeFontInfo(const wxString& info);

    /**
        Creates the font corresponding to the given native font description string and
        returns @true if
        the creation was successful.
        Unlike SetNativeFontInfo(), this function accepts
        strings which are user-friendly.
        Examples of accepted string formats are:

        Generic syntax

        Example

        on @b wxGTK2: @c [FACE-NAME] [bold] [oblique|italic] [POINTSIZE]

        Monospace bold 10

        on @b wxMSW: @c [light|bold] [italic] [FACE-NAME] [POINTSIZE] [ENCODING]

        Tahoma 10 WINDOWS-1252

        on @b wxMac: FIXME

        FIXME

        For more detailed information about the allowed syntaxes you can look at the
        documentation of the native API used for font-rendering (e.g. pango_font_description_from_string).

        @see SetNativeFontInfo()
    */
    bool SetNativeFontInfoUserDesc(const wxString& info);

    /**
        Sets the point size.

        @param pointSize
            Size in points.

        @see GetPointSize()
    */
    void SetPointSize(int pointSize);

    /**
        Sets the font style.

        @param style
            One of wxFONTSTYLE_NORMAL, wxFONTSTYLE_SLANT and wxFONTSTYLE_ITALIC.

        @see GetStyle()
    */
    void SetStyle(int style);

    /**
        Sets underlining.

        @param underlined
            @true to underline, @false otherwise.

        @see GetUnderlined()
    */
    void SetUnderlined(const bool underlined);

    /**
        Sets the font weight.

        @param weight
            One of:
        <TABLE>
            <TR><TD>wxFONTWEIGHT_NORMAL</TD><TD>Normal font.</TD></TR>
            <TR><TD>wxFONTWEIGHT_LIGHT</TD><TD>Light font.</TD></TR>
            <TR><TD>wxFONTWEIGHT_BOLD</TD><TD>Bold font.</TD></TR>
        </TABLE>

        @see GetWeight()
    */
    void SetWeight(wxFontWeight weight);

    /**
        Inequality operator.
        See @ref overview_refcountequality "reference-counted object comparison" for
        more info.
    */
    bool operator !=(const wxFont& font);

    /**
        Assignment operator, using @ref overview_trefcount "reference counting".
    */
    wxFont operator =(const wxFont& font);

    /**
        Equality operator.
        See @ref overview_refcountequality "reference-counted object comparison" for
        more info.
    */
    bool operator ==(const wxFont& font);
};


/**
    An empty wxFont.
*/
wxFont wxNullFont;

/**
    FIXME
*/
wxFont wxNORMAL_FONT;

/**
    FIXME
*/
wxFont wxSMALL_FONT;

/**
    FIXME
*/
wxFont wxITALIC_FONT;

/**
    FIXME
*/
wxFont wxSWISS_FONT;


/**
    @class wxFontList

    A font list is a list containing all fonts which have been created. There
    is only one instance of this class: @b wxTheFontList.  Use this object to search
    for a previously created font of the desired type and create it if not already
    found.
    In some windowing systems, the font may be a scarce resource, so it is best to
    reuse old resources if possible.  When an application finishes, all fonts will
    be
    deleted and their resources freed, eliminating the possibility of 'memory
    leaks'.

    @library{wxcore}
    @category{gdi}

    @see wxFont
*/
class wxFontList : public wxList
{
public:
    /**
        Constructor. The application should not construct its own font list:
        use the object pointer @b wxTheFontList.
    */
    wxFontList();

    /**
        Finds a font of the given specification, or creates one and adds it to the
        list. See the @ref wxFont::ctor "wxFont constructor" for
        details of the arguments.
    */
    wxFont* FindOrCreateFont(int point_size, int family, int style,
                             int weight,
                             bool underline = false,
                             const wxString& facename = NULL,
                             wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
};



// ============================================================================
// Global functions/macros
// ============================================================================

/** @ingroup group_funcmacro_misc */
//@{

/**
    Converts string to a wxFont best represented by the given string. Returns
    @true on success.

    @see wxToString(const wxFont&)

    @header{wx/font.h}
*/
bool wxFromString(const wxString& string, wxFont* font);

/**
    Converts the given wxFont into a string.

    @see wxFromString(const wxString&, wxFont*)

    @header{wx/font.h}
*/
wxString wxToString(const wxFont& font);

//@}

