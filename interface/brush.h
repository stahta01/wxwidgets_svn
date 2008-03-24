/////////////////////////////////////////////////////////////////////////////
// Name:        brush.h
// Purpose:     interface of wxBrush
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    The possible brush styles.
*/
enum wxBrushStyle
{
    wxBRUSHSTYLE_SOLID = wxSOLID,
        /**< Solid. */

    wxBRUSHSTYLE_TRANSPARENT = wxTRANSPARENT,
        /**< Transparent (no fill). */

    wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE = wxSTIPPLE_MASK_OPAQUE,
        /**< @todo WHAT's THIS?? */

    wxBRUSHSTYLE_STIPPLE_MASK = wxSTIPPLE_MASK,
        /**< @todo WHAT's THIS?? */

    wxBRUSHSTYLE_STIPPLE = wxSTIPPLE,
        /**< Uses a bitmap as a stipple. */

    wxBRUSHSTYLE_BDIAGONAL_HATCH = wxBDIAGONAL_HATCH,
        /**< Backward diagonal hatch. */

    wxBRUSHSTYLE_CROSSDIAG_HATCH = wxCROSSDIAG_HATCH,
        /**< Cross-diagonal hatch. */

    wxBRUSHSTYLE_FDIAGONAL_HATCH = wxFDIAGONAL_HATCH,
        /**< Forward diagonal hatch. */

    wxBRUSHSTYLE_CROSS_HATCH = wxCROSS_HATCH,
        /**< Cross hatch. */

    wxBRUSHSTYLE_HORIZONTAL_HATCH = wxHORIZONTAL_HATCH,
        /**< Horizontal hatch. */

    wxBRUSHSTYLE_VERTICAL_HATCH = wxVERTICAL_HATCH,
        /**< Vertical hatch. */

    wxBRUSHSTYLE_FIRST_HATCH = wxFIRST_HATCH,
    wxBRUSHSTYLE_LAST_HATCH = wxLAST_HATCH,
    wxBRUSHSTYLE_MAX
};



/**
    @class wxBrush
    @wxheader{brush.h}

    A brush is a drawing tool for filling in areas. It is used for painting
    the background of rectangles, ellipses, etc. It has a colour and a style.

    On a monochrome display, wxWidgets shows all brushes as white unless the
    colour is really black.

    Do not initialize objects on the stack before the program commences, since
    other required structures may not have been set up yet. Instead, define
    global pointers to objects and create them in wxApp::OnInit or when required.

    An application may wish to create brushes with different characteristics
    dynamically, and there is the consequent danger that a large number of
    duplicate brushes will be created. Therefore an application may wish to
    get a pointer to a brush by using the global list of brushes ::wxTheBrushList,
    and calling the member function wxBrushList::FindOrCreateBrush().

    This class uses reference counting and copy-on-write internally so that
    assignments between two instances of this class are very cheap.
    You can therefore use actual objects instead of pointers without efficiency problems.
    If an instance of this class is changed it will create its own data internally
    so that other instances, which previously shared the data using the reference
    counting, are not affected.

    @library{wxcore}
    @category{gdi}

    @stdobjects
    ::wxNullBrush, ::wxBLUE_BRUSH, ::wxGREEN_BRUSH, ::wxWHITE_BRUSH,
    ::wxBLACK_BRUSH, ::wxGREY_BRUSH, ::wxMEDIUM_GREY_BRUSH, ::wxLIGHT_GREY_BRUSH,
    ::wxTRANSPARENT_BRUSH, ::wxCYAN_BRUSH, ::wxRED_BRUSH

    @see wxBrushList, wxDC, wxDC::SetBrush
*/
class wxBrush : public wxGDIObject
{
public:
    /**
        Default constructor.
        The brush will be uninitialised, and wxBrush:IsOk() will return @false.
    */
    wxBrush();

    /**
        Constructs a brush from a colour object and @a style.

        @param colour
            Colour object.
        @param style
            One of the ::wxBrushStyle enumeration values.
    */
    wxBrush(const wxColour& colour, wxBrushStyle style = wxSOLID);

    /**
        Constructs a brush from a colour name and @a style.

        @param colourName
            Colour name. The name will be looked up in the colour database.
        @param style
            One of the ::wxBrushStyle enumeration values.

        @see wxColourDatabase
    */
    wxBrush(const wxString& colourName, wxBrushStyle style);

    /**
        Constructs a stippled brush using a bitmap.
        The brush style will be set to wxBRUSHSTYLE_STIPPLE.
    */
    wxBrush(const wxBitmap& stippleBitmap);

    /**
        Copy constructor, uses @ref overview_refcount "reference counting".
    */
    wxBrush(const wxBrush& brush);

    /**
        Destructor.

        See @ref overview_refcount_destruct for more info.

        @remarks Although all remaining brushes are deleted when the application
                 exits, the application should try to clean up all brushes itself.
                 This is because wxWidgets cannot know if a pointer to the brush
                 object is stored in an application data structure, and there is
                 a risk of double deletion.
    */
    virtual ~wxBrush();

    /**
        Returns a reference to the brush colour.

        @see SetColour()
    */
    wxColour& GetColour() const;

    /**
        Gets a pointer to the stipple bitmap. If the brush does not have a wxBRUSHSTYLE_STIPPLE
        style, this bitmap may be non-@NULL but uninitialised (i.e. wxBitmap:IsOk() returns @false).

        @see SetStipple()
    */
    wxBitmap* GetStipple() const;

    /**
        Returns the brush style, one of the ::wxBrushStyle values.

        @see SetStyle(), SetColour(), SetStipple()
    */
    wxBrushStyle GetStyle() const;

    /**
        Returns @true if the style of the brush is any of hatched fills.

        @see GetStyle()
    */
    virtual bool IsHatch() const;

    /**
        Returns @true if the brush is initialised. It will return @false if the default
        constructor has been used (for example, the brush is a member of a class, or
        @NULL has been assigned to it).
    */
    bool IsOk() const;

    //@{
    /**
        Sets the brush colour using red, green and blue values.

        @see GetColour()
    */
    void SetColour(wxColour& colour);
    void SetColour(const wxString& colourName);
    void SetColour(unsigned char red, unsigned char green,
                   unsigned char blue);
    //@}

    /**
        Sets the stipple bitmap.

        @param bitmap
            The bitmap to use for stippling.

        @remarks The style will be set to wxBRUSHSTYLE_STIPPLE, unless the bitmap
                 has a mask associated to it, in which case the style will be set
                 to wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE.

        @see wxBitmap
    */
    void SetStipple(const wxBitmap& bitmap);

    /**
        Sets the brush style.

        @param style
            One of the ::wxBrushStyle values.

        @see GetStyle()
    */
    void SetStyle(wxBrushStyle style);

    /**
        Inequality operator.
        See @ref overview_refcount_equality for more info.
    */
    bool operator !=(const wxBrush& brush);

    /**
        Assignment operator, using @ref overview_refcount "reference counting".
    */
    wxBrush operator =(const wxBrush& brush);

    /**
        Equality operator.
        See @ref overview_refcount_equality for more info.
    */
    bool operator ==(const wxBrush& brush);
};

/**
    An empty brush.
*/
wxBrush wxNullBrush;

/**
    Blue brush.
*/
wxBrush* wxBLUE_BRUSH;

/**
    Green brush.
*/
wxBrush* wxGREEN_BRUSH;

/**
    White brush.
*/
wxBrush* wxWHITE_BRUSH;

/**
    Black brush.
*/
wxBrush* wxBLACK_BRUSH;

/**
    Grey brush.
*/
wxBrush* wxGREY_BRUSH;

/**
    Medium grey brush.
*/
wxBrush* wxMEDIUM_GREY_BRUSH;

/**
    Light grey brush.
*/
wxBrush* wxLIGHT_GREY_BRUSH;

/**
    Transparent brush.
*/
wxBrush* wxTRANSPARENT_BRUSH;

/**
    Cyan brush.
*/
wxBrush* wxCYAN_BRUSH;

/**
    Red brush.
*/
wxBrush* wxRED_BRUSH;



/**
    @class wxBrushList
    @wxheader{gdicmn.h}

    A brush list is a list containing all brushes which have been created.

    @library{wxcore}
    @category{gdi}

    @see wxBrush
*/
class wxBrushList : public wxList
{
public:
    /**
        Constructor. The application should not construct its own brush list:
        use the object pointer ::wxTheBrushList.
    */
    wxBrushList();

    /**
        Finds a brush with the specified attributes and returns it, else creates a new
        brush, adds it to the brush list, and returns it.

        @param colour
            Colour object.
        @param style
            Brush style. See ::wxBrushStyle for a list of styles.
    */
    wxBrush* FindOrCreateBrush(const wxColour& colour,
                               wxBrushStyle style = wxBRUSHSTYLE_SOLID);
};

/**
    The global wxBrushList instance.
*/
wxBrushList* wxTheBrushList;
