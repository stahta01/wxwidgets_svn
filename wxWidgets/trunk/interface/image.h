/////////////////////////////////////////////////////////////////////////////
// Name:        image.h
// Purpose:     interface of wxImageHandler
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxImageHandler
    @wxheader{image.h}

    This is the base class for implementing image file loading/saving, and image
    creation from data.
    It is used within wxImage and is not normally seen by the application.

    If you wish to extend the capabilities of wxImage, derive a class from
    wxImageHandler
    and add the handler using wxImage::AddHandler in your
    application initialisation.

    @library{wxcore}
    @category{FIXME}

    @see wxImage, wxInitAllImageHandlers()
*/
class wxImageHandler : public wxObject
{
public:
    /**
        Default constructor. In your own default constructor, initialise the members
        m_name, m_extension and m_type.
    */
    wxImageHandler();

    /**
        Destroys the wxImageHandler object.
    */
    ~wxImageHandler();

    /**
        Gets the file extension associated with this handler.
    */
    const wxString GetExtension() const;

    /**
        If the image file contains more than one image and the image handler is capable
        of retrieving these individually, this function will return the number of
        available images.

        @param stream
            Opened input stream for reading image data. Currently, the stream must
        support seeking.

        @returns Number of available images. For most image handlers, this is 1
                 (exceptions are TIFF and ICO formats).
    */
    int GetImageCount(wxInputStream& stream);

    /**
        Gets the MIME type associated with this handler.
    */
    const wxString GetMimeType() const;

    /**
        Gets the name of this handler.
    */
    const wxString GetName() const;

    /**
        Gets the image type associated with this handler.
    */
    long GetType() const;

    /**
        Loads a image from a stream, putting the resulting data into @e image. If the
        image file contains
        more than one image and the image handler is capable of retrieving these
        individually, @e index
        indicates which image to read from the stream.

        @param image
            The image object which is to be affected by this operation.
        @param stream
            Opened input stream for reading image data.
        @param verbose
            If set to @true, errors reported by the image handler will produce
        wxLogMessages.
        @param index
            The index of the image in the file (starting from zero).

        @returns @true if the operation succeeded, @false otherwise.

        @see wxImage::LoadFile, wxImage::SaveFile, SaveFile()
    */
    bool LoadFile(wxImage* image, wxInputStream& stream,
                  bool verbose = true, int index = 0);

    /**
        Saves a image in the output stream.

        @param image
            The image object which is to be affected by this operation.
        @param stream
            Opened output stream for writing the data.

        @returns @true if the operation succeeded, @false otherwise.

        @see wxImage::LoadFile, wxImage::SaveFile, LoadFile()
    */
    bool SaveFile(wxImage* image, wxOutputStream& stream);

    /**
        Sets the handler extension.

        @param extension
            Handler extension.
    */
    void SetExtension(const wxString& extension);

    /**
        Sets the handler MIME type.

        @param mimename
            Handler MIME type.
    */
    void SetMimeType(const wxString& mimetype);

    /**
        Sets the handler name.

        @param name
            Handler name.
    */
    void SetName(const wxString& name);
};



/**
    @class wxImage
    @wxheader{image.h}

    This class encapsulates a platform-independent image. An image can be created
    from data, or using wxBitmap::ConvertToImage. An image
    can be loaded from a file in a variety of formats, and is extensible to new
    formats
    via image format handlers. Functions are available to set and get image bits, so
    it can be used for basic image manipulation.

    A wxImage cannot (currently) be drawn directly to a wxDC. Instead,
    a platform-specific wxBitmap object must be created from it using
    the wxBitmap::wxBitmap(wxImage,int depth) constructor.
    This bitmap can then
    be drawn in a device context, using wxDC::DrawBitmap.

    One colour value of the image may be used as a mask colour which will lead to
    the automatic creation of a wxMask object associated to the bitmap object.

    @library{wxcore}
    @category{gdi}

    @stdobjects
    ::wxNullImage

    @see wxBitmap, wxInitAllImageHandlers(), wxPixelData
*/
class wxImage : public wxObject
{
public:
    //@{
    /**
        Creates an image from data in memory. If static_data is false
        then the wxImage will take ownership of the data and free it
        afterwards. For this, it has to be allocated with @e malloc.
    
        @param width
            Specifies the width of the image.
        @param height
            Specifies the height of the image.
        @param data
            A pointer to RGB data
        @param alpha
            A pointer to alpha-channel data
        @param static_data
            Indicates if the data should be free'd after use
        
    */
    wxImage(int width, int height, unsigned char* data, unsigned char* alpha, bool static_data = false );
    wxImage(int width, int height, unsigned char* data,  bool static_data = false);
    //@}
    
    /**
         Creates an image with the given size and clears it if requested.
         Does not create an alpha channel.
         
        @param width
            Specifies the width of the image.
        @param height
            Specifies the height of the image.
        @clear
            Clear the image with zeros.
    */
    wxImage(int width, int height, bool clear = true);
    
    /**
         Creates an empty wxImage object. Does not create
         an alpha channel.
    */
    wxImage();
    
    /**
        Creates an image from XPM data.
        
        @param xpmData
            A pointer to XPM image data.
    */
    wxImage(const char* const* xpmData);
    //@{
    /**
        @param name
            Name of the file from which to load the image.
        @param stream
            Opened input stream from which to load the image. Currently,
            the stream must support seeking.
        @param type
            May be one of the following:
            @li wxBITMAP_TYPE_BMP: Load a Windows bitmap file.
            @li wxBITMAP_TYPE_GIF: Load a GIF bitmap file.
            @li wxBITMAP_TYPE_JPEG: Load a JPEG bitmap file.
            @li wxBITMAP_TYPE_PNG: Load a PNG bitmap file.
            @li wxBITMAP_TYPE_PCX: Load a PCX bitmap file.
            @li wxBITMAP_TYPE_PNM: Load a PNM bitmap file.
            @li wxBITMAP_TYPE_TIF: Load a TIFF bitmap file.
            @li wxBITMAP_TYPE_TGA: Load a TGA bitmap file.
            @li wxBITMAP_TYPE_XPM: Load a XPM bitmap file.
            @li wxBITMAP_TYPE_ICO: Load a Windows icon file (ICO).
            @li wxBITMAP_TYPE_CUR: Load a Windows cursor file (CUR).
            @li wxBITMAP_TYPE_ANI: Load a Windows animated cursor file (ANI).
            @li wxBITMAP_TYPE_ANY: Will try to autodetect the format.
            
        @param mimetype
            MIME type string (for example 'image/jpeg')
        @param index
            Index of the image to load in the case that the image file contains
            multiple images. This is only used by GIF, ICO and TIFF handlers.
            The default value (-1) means "choose the default image" and is
            interpreted as the first image (index=0) by the GIF and TIFF handler
            and as the largest and most colourful one by the ICO handler.

        @remarks Depending on how wxWidgets has been configured, not all formats
                 may be available.

        @see LoadFile()
    */
    wxImage(const wxString& name, long type = wxBITMAP_TYPE_ANY, int index = -1);
    wxImage(const wxString& name, const wxString& mimetype, int index = -1);
    wxImage(wxInputStream& stream, long type = wxBITMAP_TYPE_ANY, int index = -1);
    wxImage(wxInputStream& stream, const wxString& mimetype, int index = -1);
    //@}
    

    /**
        Destructor.
        See @ref overview_refcountdestruct "reference-counted object destruction" for
        more info.
    */
    ~wxImage();

    //@{
    /**
        returns @true if the current image handlers can read this file
    */
    static void AddHandler(wxImageHandler* handler);
    See also bool CanRead(const wxString& filename);
    //@}

    /**
        Blurs the image in both horizontal and vertical directions by the specified
        pixel
        @e blurRadius. This should not be used when using a single mask colour
        for transparency.

        @see @ref horzblur() BlurHorizontal, @ref vertblur() BlurVertical
    */
    wxImage Blur(int blurRadius);

    /**
        Blurs the image in the horizontal direction only. This should not be used
        when using a single mask colour for transparency.

        @see Blur(), @ref vertblur() BlurVertical
    */
    wxImage BlurHorizontal(int blurRadius);

    /**
        Blurs the image in the vertical direction only. This should not be used
        when using a single mask colour for transparency.

        @see Blur(), @ref horzblur() BlurHorizontal
    */
    wxImage BlurVertical(int blurRadius);

    /**
        Deletes all image handlers.
        This function is called by wxWidgets on exit.
    */
    static void CleanUpHandlers();

    /**
        Computes the histogram of the image. @a histogram is a reference to
        wxImageHistogram object. wxImageHistogram is a specialization of
        wxHashMap "template" and is defined as follows:

        @returns Returns number of colours in the histogram.
    */
    unsigned long ComputeHistogram(wxImageHistogram& histogram) const;

    /**
        If the image has alpha channel, this method converts it to mask. All pixels
        with alpha value less than @a threshold are replaced with mask colour
        and the alpha channel is removed. Mask colour is chosen automatically using
        FindFirstUnusedColour().
        If the image image doesn't have alpha channel,
        ConvertAlphaToMask does nothing.

        @returns @false if FindFirstUnusedColour returns @false, @true otherwise.
    */
    bool ConvertAlphaToMask(unsigned char threshold = 128);

    /**
        Deprecated, use equivalent @ref wxBitmap::ctor "wxBitmap constructor"
        (which takes wxImage and depth as its arguments) instead.
    */
    wxBitmap ConvertToBitmap() const;

    /**
        Returns a greyscale version of the image. The returned image uses the luminance
        component of the original to calculate the greyscale. Defaults to using
        ITU-T BT.601 when converting to YUV, where every pixel equals
        (R * @e lr) + (G * @e lg) + (B * @e lb).
    */
    wxImage ConvertToGreyscale(double lr = 0.299, double lg = 0.587,
                               double lb = 0.114) const;

    /**
        Returns monochromatic version of the image. The returned image has white
        colour where the original has @e (r,g,b) colour and black colour
        everywhere else.
    */
    wxImage ConvertToMono(unsigned char r, unsigned char g,
                          unsigned char b) const;

    /**
        Returns an identical copy of the image.
    */
    wxImage Copy() const;

    /**
        Creates a fresh image.  If @a clear is @true, the new image will be initialized
        to black.
        Otherwise, the image data will be uninitialized.

        @param width
            The width of the image in pixels.
        @param height
            The height of the image in pixels.

        @returns @true if the call succeeded, @false otherwise.
    */
    bool Create(int width, int height, bool clear = true);

    /**
        Destroys the image data.
    */
    void Destroy();

    /**
        @param r,g,b
            Pointers to variables to save the colour.
        @param startR,startG,startB
            Initial values of the colour. Returned colour
            will have RGB values equal to or greater than these.

        @returns Returns @false if there is no unused colour left, @true on success.
    */
    bool FindFirstUnusedColour(unsigned char* r, unsigned char* g,
                               unsigned char* b,
                               unsigned char startR = 1,
                               unsigned char startG = 0,
                               unsigned char startB = 0);

    //@{
    /**
        Finds the handler associated with the given MIME type.

        @param name
            The handler name.
        @param extension
            The file extension, such as "bmp".
        @param imageType
            The image type, such as wxBITMAP_TYPE_BMP.
        @param mimetype
            MIME type.

        @returns A pointer to the handler if found, @NULL otherwise.

        @see wxImageHandler
    */
    static wxImageHandler* FindHandler(const wxString& name);
    static wxImageHandler* FindHandler(const wxString& extension,
                                       long imageType);
    static wxImageHandler* FindHandler(long imageType);
    static wxImageHandler* FindHandlerMime(const wxString& mimetype);
    //@}

    //@{
    /**
        Returns pointer to the array storing the alpha values for this image. This
        pointer is @NULL for the images without the alpha channel. If the image
        does have it, this pointer may be used to directly manipulate the alpha values
        which are stored as the @ref getdata() RGB ones.
    */
    unsigned char GetAlpha(int x, int y) const;
    const unsigned char * GetAlpha() const;
    //@}

    /**
        Returns the blue intensity at the given coordinate.
    */
    unsigned char GetBlue(int x, int y) const;

    /**
        Returns the image data as an array. This is most often used when doing
        direct image manipulation. The return value points to an array of
        characters in RGBRGBRGB... format in the top-to-bottom, left-to-right
        order, that is the first RGB triplet corresponds to the pixel first pixel of
        the first row, the second one --- to the second pixel of the first row and so
        on until the end of the first row, with second row following after it and so
        on.
        You should not delete the returned pointer nor pass it to
        SetData().
    */
    unsigned char* GetData() const;

    /**
        Returns the green intensity at the given coordinate.
    */
    unsigned char GetGreen(int x, int y) const;

    /**
        Returns the static list of image format handlers.

        @see wxImageHandler
    */
    static wxList GetHandlers();

    /**
        Gets the height of the image in pixels.
    */
    int GetHeight() const;

    //@{
    /**
        If the image file contains more than one image and the image handler is capable
        of retrieving these individually, this function will return the number of
        available images.

        @param name
            Name of the file to query.
        @param stream
            Opened input stream with image data. Currently, the stream must
            support seeking.
        @param type
            May be one of the following:
            @li wxBITMAP_TYPE_BMP: Load a Windows bitmap file.
            @li wxBITMAP_TYPE_GIF: Load a GIF bitmap file.
            @li wxBITMAP_TYPE_JPEG: Load a JPEG bitmap file.
            @li wxBITMAP_TYPE_PNG: Load a PNG bitmap file.
            @li wxBITMAP_TYPE_PCX: Load a PCX bitmap file.
            @li wxBITMAP_TYPE_PNM: Load a PNM bitmap file.
            @li wxBITMAP_TYPE_TIF: Load a TIFF bitmap file.
            @li wxBITMAP_TYPE_TGA: Load a TGA bitmap file.
            @li wxBITMAP_TYPE_XPM: Load a XPM bitmap file.
            @li wxBITMAP_TYPE_ICO: Load a Windows icon file (ICO).
            @li wxBITMAP_TYPE_CUR: Load a Windows cursor file (CUR).
            @li wxBITMAP_TYPE_ANI: Load a Windows animated cursor file (ANI).
            @li wxBITMAP_TYPE_ANY: Will try to autodetect the format.

        @returns Number of available images. For most image handlers, this is 1
                 (exceptions are TIFF and ICO formats).
    */
    static int GetImageCount(const wxString& filename,
                             long type = wxBITMAP_TYPE_ANY);
    static int GetImageCount(wxInputStream& stream,
                             long type = wxBITMAP_TYPE_ANY);
    //@}

    /**
        Iterates all registered wxImageHandler objects, and returns a string containing
        file extension masks
        suitable for passing to file open/save dialog boxes.

        @returns The format of the returned string is
                 "(*.ext1;*.ext2)|*.ext1;*.ext2".

        @see wxImageHandler
    */
    static wxString GetImageExtWildcard();

    /**
        Gets the blue value of the mask colour.
    */
    unsigned char GetMaskBlue() const;

    /**
        Gets the green value of the mask colour.
    */
    unsigned char GetMaskGreen() const;

    /**
        Gets the red value of the mask colour.
    */
    unsigned char GetMaskRed() const;

    /**
        Gets a user-defined option. The function is case-insensitive to @e name.
        For example, when saving as a JPEG file, the option @b quality is
        used, which is a number between 0 and 100 (0 is terrible, 100 is very good).

        @see SetOption(), GetOptionInt(), HasOption()
    */
    wxString GetOption(const wxString& name) const;

    /**
        Gets a user-defined option as an integer. The function is case-insensitive
        to @e name. If the given option is not present, the function returns 0. 
        Use HasOption() is 0 is a possibly valid value for the option.
        Options for wxPNGHandler
        @li wxIMAGE_OPTION_PNG_FORMAT: Format for saving a PNG file.
        @li wxIMAGE_OPTION_PNG_BITDEPTH: Bit depth for every channel (R/G/B/A).
        
        Supported values for wxIMAGE_OPTION_PNG_FORMAT:
        @li wxPNG_TYPE_COLOUR: Stores RGB image.
        @li wxPNG_TYPE_GREY: Stores grey image, converts from RGB.
        @li wxPNG_TYPE_GREY_RED: Stores grey image, uses red value as grey.

        @see SetOption(), GetOption()
    */
    int GetOptionInt(const wxString& name) const;

    /**
        Get the current mask colour or find a suitable unused colour that could be
        used as a mask colour. Returns @true if the image currently has a mask.
    */
    bool GetOrFindMaskColour(unsigned char r, unsigned char g,
                             unsigned char b) const;

    /**
        Returns the palette associated with the image. Currently the palette is only
        used when converting to wxBitmap under Windows. Some of the wxImage handlers
        have been modified to set the palette if one exists in the image file (usually
        256 or less colour images in GIF or PNG format).
    */
    const wxPalette GetPalette() const;

    /**
        Returns the red intensity at the given coordinate.
    */
    unsigned char GetRed(int x, int y) const;

    /**
        Returns a sub image of the current one as long as the rect belongs entirely to
        the image.
    */
    wxImage GetSubImage(const wxRect& rect) const;

    /**
        Gets the width of the image in pixels.

        @see GetHeight()
    */
    int GetWidth() const;

    /**
        Constructor for HSVValue, an object that contains values for hue, saturation
        and value which
        represent the value of a color. It is used by HSVtoRGB()
        and RGBtoHSV(), which
        converts between HSV color space and RGB color space.
    */
    HSVValue(double h = 0.0, double s = 0.0, double v = 0.0);

    /**
        Converts a color in HSV color space to RGB color space.
    */
#define wxImage::RGBValue HSVtoRGB(const HSVValue& hsv)     /* implementation is private */

    /**
        Returns @true if this image has alpha channel, @false otherwise.

        @see GetAlpha(), SetAlpha()
    */
    bool HasAlpha() const;

    /**
        Returns @true if there is a mask active, @false otherwise.
    */
    bool HasMask() const;

    /**
        Returns @true if the given option is present. The function is case-insensitive
        to @e name.

        @see SetOption(), GetOption(), GetOptionInt()
    */
    bool HasOption(const wxString& name) const;

    /**
        Initializes the image alpha channel data. It is an error to call it
        if the image already has alpha data. If it doesn't, alpha data will be
        by default initialized to all pixels being fully opaque. But if the image has a
        a mask colour, all mask pixels will be completely transparent.
    */
    void InitAlpha();

    /**
        Internal use only. Adds standard image format handlers. It only install BMP
        for the time being, which is used by wxBitmap.
        This function is called by wxWidgets on startup, and shouldn't be called by
        the user.

        @see wxImageHandler, wxInitAllImageHandlers(), wxQuantize
    */
    static void InitStandardHandlers();

    /**
        Adds a handler at the start of the static list of format handlers.

        @param handler
            A new image format handler object. There is usually only one instance
            of a given handler class in an application session.

        @see wxImageHandler
    */
    static void InsertHandler(wxImageHandler* handler);

    /**
        Returns @true if image data is present.
    */
    bool IsOk() const;

    /**
        Returns @true if the given pixel is transparent, i.e. either has the mask
        colour if this image has a mask or if this image has alpha channel and alpha
        value of this pixel is strictly less than @e threshold.
    */
    bool IsTransparent(int x, int y, unsigned char threshold = 128) const;

    //@{
    /**
        Loads an image from an input stream.

        @param name
            Name of the file from which to load the image.
        @param stream
            Opened input stream from which to load the image. Currently, the
            stream must support seeking.
        @param type
            May be one of the following:
            @li wxBITMAP_TYPE_BMP: Load a Windows bitmap file.
            @li wxBITMAP_TYPE_GIF: Load a GIF bitmap file.
            @li wxBITMAP_TYPE_JPEG: Load a JPEG bitmap file.
            @li wxBITMAP_TYPE_PNG: Load a PNG bitmap file.
            @li wxBITMAP_TYPE_PCX: Load a PCX bitmap file.
            @li wxBITMAP_TYPE_PNM: Load a PNM bitmap file.
            @li wxBITMAP_TYPE_TIF: Load a TIFF bitmap file.
            @li wxBITMAP_TYPE_TGA: Load a TGA bitmap file.
            @li wxBITMAP_TYPE_XPM: Load a XPM bitmap file.
            @li wxBITMAP_TYPE_ICO: Load a Windows icon file (ICO).
            @li wxBITMAP_TYPE_CUR: Load a Windows cursor file (CUR).
            @li wxBITMAP_TYPE_ANI: Load a Windows animated cursor file (ANI).
            @li wxBITMAP_TYPE_ANY: Will try to autodetect the format.
        @param mimetype
            MIME type string (for example 'image/jpeg')
        @param index
            Index of the image to load in the case that the image file contains
            multiple images. This is only used by GIF, ICO and TIFF handlers.
            The default value (-1) means "choose the default image" and is
            interpreted as the first image (index=0) by the GIF and TIFF handler
            and as the largest and most colourful one by the ICO handler.

        @returns @true if the operation succeeded, @false otherwise. If the
                 optional index parameter is out of range, @false is
                 returned and a call to wxLogError() takes place.

        @remarks Depending on how wxWidgets has been configured, not all formats
                 may be available.

        @see SaveFile()
    */
    bool LoadFile(const wxString& name,
                  long type = wxBITMAP_TYPE_ANY,
                  int index = -1);
    bool LoadFile(const wxString& name, const wxString& mimetype,
                  int index = -1);
    bool LoadFile(wxInputStream& stream, long type,
                  int index = -1);
    bool LoadFile(wxInputStream& stream,
                  const wxString& mimetype,
                  int index = -1);
    //@}

    /**
        Returns a mirrored copy of the image. The parameter @e horizontally
        indicates the orientation.
    */
    wxImage Mirror(bool horizontally = true) const;

    /**
        Copy the data of the given @a image to the specified position in this image.
    */
    void Paste(const wxImage& image, int x, int y);

    /**
        Constructor for RGBValue, an object that contains values for red, green and
        blue which
        represent the value of a color. It is used by HSVtoRGB()
        and RGBtoHSV(), which
        converts between HSV color space and RGB color space.
    */
    RGBValue(unsigned char r = 0, unsigned char g = 0,
             unsigned char b = 0);

    /**
        Converts a color in RGB color space to HSV color space.
    */
#define wxImage::HSVValue RGBtoHSV(const RGBValue& rgb)     /* implementation is private */

    /**
        Finds the handler with the given name, and removes it. The handler
        is not deleted.

        @param name
            The handler name.

        @returns @true if the handler was found and removed, @false otherwise.

        @see wxImageHandler
    */
    static bool RemoveHandler(const wxString& name);

    /**
        Replaces the colour specified by @e r1,g1,b1 by the colour @e r2,g2,b2.
    */
    void Replace(unsigned char r1, unsigned char g1,
                 unsigned char b1, unsigned char r2,
                 unsigned char g2, unsigned char b2);

    /**
        Changes the size of the image in-place by scaling it: after a call to this
        function,
        the image will have the given width and height.
        For a description of the @a quality parameter, see the Scale() function.
        Returns the (modified) image itself.

        @see Scale()
    */
    wxImage Rescale(int width, int height,
                    int quality = wxIMAGE_QUALITY_NORMAL);

    /**
        Changes the size of the image in-place without scaling it by adding either a
        border
        with the given colour or cropping as necessary. The image is pasted into a new
        image with the given @a size and background colour at the position @e pos
        relative to the upper left of the new image. If @a red = green = blue = -1
        then use either the  current mask colour if set or find, use, and set a
        suitable mask colour for any newly exposed areas.
        Returns the (modified) image itself.

        @see Size()
    */
    wxImage Resize(const wxSize& size, const wxPoint pos,
                   int red = -1, int green = -1,
                   int blue = -1);

    /**
        Rotates the image about the given point, by @a angle radians. Passing @true
        to @a interpolating results in better image quality, but is slower. If the
        image has a mask, then the mask colour is used for the uncovered pixels in the
        rotated image background. Else, black (rgb 0, 0, 0) will be used.
        Returns the rotated image, leaving this image intact.
    */
    wxImage Rotate(double angle, const wxPoint& rotationCentre,
                   bool interpolating = true,
                   wxPoint* offsetAfterRotation = NULL);

    /**
        Returns a copy of the image rotated 90 degrees in the direction
        indicated by @e clockwise.
    */
    wxImage Rotate90(bool clockwise = true) const;

    /**
        Rotates the hue of each pixel in the image by @e angle, which is a double in
        the range of -1.0 to +1.0, where -1.0 corresponds to -360 degrees and +1.0
        corresponds
        to +360 degrees.
    */
    void RotateHue(double angle);

    //@{
    /**
        Saves an image in the given stream.

        @param name
            Name of the file to save the image to.
        @param stream
            Opened output stream to save the image to.
        @param type
            Currently these types can be used:
            @li wxBITMAP_TYPE_BMP: Save a BMP image file.
            @li wxBITMAP_TYPE_JPEG: Save a JPEG image file.
            @li wxBITMAP_TYPE_PNG: Save a PNG image file.
            @li wxBITMAP_TYPE_PCX: Save a PCX image file (tries to save as 8-bit if possible,
                falls back to 24-bit otherwise).
            @li wxBITMAP_TYPE_PNM: Save a PNM image file (as raw RGB always).
            @li wxBITMAP_TYPE_TIFF: Save a TIFF image file.
            @li wxBITMAP_TYPE_XPM: Save a XPM image file.
            @li wxBITMAP_TYPE_ICO: Save a Windows icon file (ICO) (the size may
                be up to 255 wide by 127 high. A single image is saved in 8 colors
                at the size supplied).
            @li wxBITMAP_TYPE_CUR: Save a Windows cursor file (CUR).
        @param mimetype
            MIME type.

        @returns @true if the operation succeeded, @false otherwise.

        @remarks Depending on how wxWidgets has been configured, not all formats
                 may be available.

        @see LoadFile()
    */
    bool SaveFile(const wxString& name, int type) const;
    const bool SaveFile(const wxString& name,
                        const wxString& mimetype) const;
    const bool SaveFile(const wxString& name) const;
    const bool SaveFile(wxOutputStream& stream, int type) const;
    const bool SaveFile(wxOutputStream& stream,
                        const wxString& mimetype) const;
    //@}

    /**
        Returns a scaled version of the image. This is also useful for
        scaling bitmaps in general as the only other way to scale bitmaps
        is to blit a wxMemoryDC into another wxMemoryDC.
        It should be noted that although using wxIMAGE_QUALITY_HIGH produces much nicer
        looking results it is a slower method.  Downsampling will use the box averaging
        method
        which seems to operate very fast.  If you are upsampling larger images using
        this method you will most likely notice that it is a bit slower and in extreme
        cases
        it will be quite substantially slower as the bicubic algorithm has to process a
        lot of
        data.
        It should also be noted that the high quality scaling may not work as expected
        when using a single mask colour for transparency, as the scaling will blur the
        image and will therefore remove the mask partially. Using the alpha channel
        will work.
        Example:

        @param quality
            Determines what method to use for resampling the image. 
            
            Can be one of the following:
            @li wxIMAGE_QUALITY_NORMAL: Uses the normal default scaling method of
                pixel replication
            @li wxIMAGE_QUALITY_HIGH: Uses bicubic and box averaging resampling
                methods for upsampling and downsampling respectively

        @see Rescale()
    */
    wxImage Scale(int width, int height,
                  int quality = wxIMAGE_QUALITY_NORMAL) const;

    //@{
    /**
        Sets the alpha value for the given pixel. This function should only be called
        if the image has alpha channel data, use HasAlpha() to
        check for this.
    */
    void SetAlpha(unsigned char* alpha = NULL,
                  bool static_data = false);
    void SetAlpha(int x, int y, unsigned char alpha);
    //@}

    /**
        Sets the image data without performing checks. The data given must have
        the size (width*height*3) or results will be unexpected. Don't use this
        method if you aren't sure you know what you are doing.
        The data must have been allocated with @c malloc(), @b NOT with
        @c operator new.
        After this call the pointer to the data is owned by the wxImage object,
        that will be responsible for deleting it.
        Do not pass to this function a pointer obtained through
        GetData().
    */
    void SetData(unsigned char* data);

    /**
        Specifies whether there is a mask or not. The area of the mask is determined by
        the current mask colour.
    */
    void SetMask(bool hasMask = true);

    /**
        Sets the mask colour for this image (and tells the image to use the mask).
    */
    void SetMaskColour(unsigned char red, unsigned char green,
                       unsigned char blue);

    /**
        @param mask
            The mask image to extract mask shape from. Must have same dimensions as the
        image.
        @param mr,mg,mb
            RGB value of pixels in mask that will be used to create the mask.

        @returns Returns @false if mask does not have same dimensions as the image
                 or if there is no unused colour left. Returns @true if
                 the mask was successfully applied.
    */
    bool SetMaskFromImage(const wxImage& mask, unsigned char mr,
                          unsigned char mg,
                          unsigned char mb);

    //@{
    /**
        Sets a user-defined option. The function is case-insensitive to @e name.
        For example, when saving as a JPEG file, the option @b quality is
        used, which is a number between 0 and 100 (0 is terrible, 100 is very good).

        @see GetOption(), GetOptionInt(), HasOption()
    */
    void SetOption(const wxString& name, const wxString& value);
    void SetOption(const wxString& name, int value);
    //@}

    /**
        Associates a palette with the image. The palette may be used when converting
        wxImage to wxBitmap (MSW only at present) or in file save operations (none as
        yet).
    */
    void SetPalette(const wxPalette& palette);

    /**
        Sets the colour of the pixels within the given rectangle. This routine performs
        bounds-checks for the coordinate so it can be considered a safe way to
        manipulate the
        data.
    */
    void SetRGB(wxRect& rect, unsigned char red,
                unsigned char green,
                unsigned char blue);

    /**
        Returns a resized version of this image without scaling it by adding either a
        border
        with the given colour or cropping as necessary. The image is pasted into a new
        image with the given @a size and background colour at the position @e pos
        relative to the upper left of the new image. If @a red = green = blue = -1
        then the areas of the larger image not covered by this image are made
        transparent by filling them with the image mask colour (which will be allocated
        automatically if it isn't currently set). Otherwise, the areas will be filled
        with the colour with the specified RGB components.

        @see Resize()
    */
    wxImage Size(const wxSize& size, const wxPoint pos, int red = -1,
                 int green = -1, int blue = -1) const;

    /**
        Assignment operator, using @ref overview_trefcount "reference counting".

        @param image
            Image to assign.

        @returns Returns 'this' object.
    */
    wxImage operator =(const wxImage& image);
};

/**
    An empty wxImage.
*/
wxImage wxNullImage;


// ============================================================================
// Global functions/macros
// ============================================================================

/** @ingroup group_funcmacro_appinitterm */
//@{

/**
    Initializes all available image handlers. For a list of available handlers,
    see wxImage.

    @see wxImage, wxImageHandler

    @header{wx/image.h}
*/
void wxInitAllImageHandlers();

//@}

