
/**
    @class wxPixelData
    @wxheader{rawbmp.h}

    A class template with ready to use implementations for getting
    direct and efficient access to wxBitmap's internal data and
    wxImage's internal data through a standard interface. It is
    possible to extend this class (interface) to other types of
    image content.
    
    Implemented on Windows, GTK+ and OS X:
    wxNativePixelData: Class to access to wxBitmap's internal data without alpha channel (RGB).
    wxAlphaPixelData: Class to access to wxBitmap's internal data with alpha channel (RGBA).
    
    Implemented everywhere:
    wxImagePixelData: Class to access to wxImage's internal data with alpha channel (RGBA).
    
    @code
    wxBitmap bmp;
    wxNativePixelData data(bmp);
    if ( !data )
    {
        ... raw access to bitmap data unavailable, do something else ...
        return;
    }

    if ( data.GetWidth() < 20 || data.GetHeight() < 20 )
    {
        ... complain: the bitmap it too small ...
        return;
    }

    wxNativePixelData::Iterator p(data);

    // we draw a (10, 10)-(20, 20) rect manually using the given r, g, b
    p.Offset(data, 10, 10);

    for ( int y = 0; y < 10; ++y )
    {
        wxNativePixelData::Iterator rowStart = p;

        for ( int x = 0; x < 10; ++x, ++p )
        {
            p.Red() = r;
            p.Green() = g;
            p.Blue() = b;
        }

        p = rowStart;
        p.OffsetY(data, 1);
    }
    @endcode

    @library{wxcore}
    @category{gdi}
    
    @see wxBitmap, wxImage
*/
template <class Image, class PixelFormat = wxPixelFormatFor<Image> >
class wxPixelData :
    public wxPixelDataOut<Image>::template wxPixelDataIn<PixelFormat>
{
public:
    /** 
        the type of the class we're working with
    */
    typedef Image ImageType;
    
    /**
        Create pixel data object representing the entire image
    */
    wxPixelData(Image& image);
    
    
    /**
        Create pixel data object representing the area
        of the image defined by @e rect.
    */
    wxPixelData(Image& i, const wxRect& rect);
    
    /**
        Create pixel data object representing the area
        of the image defined by @e pt and @e sz.
    */
    wxPixelData(Image& i, const wxPoint& pt, const wxSize& sz)
    
    /**
        Return true of if we could get access to bitmap data
        successfully
    */
    operator bool() const:

    /** 
       Return the iterator pointing to the origin of the image
    */
    Iterator GetPixels() const;

    /**
        Returns origin of the rectangular region we represent
    */
    wxPoint GetOrigin() const;

    /** 
         Return width of the region we represent
    */
    int GetWidth() const;

    /** 
         Return height of the region we represent
    */
    int GetHeight() const;

    /*
        Return area which this class represents in the image
    */
    wxSize GetSize() const;

    /**
       Return the distance between two rows
    */
    int GetRowStride() const;


    /**
        Iterator
    */
        class Iterator
        {
        public:
            /**
               go back to (0, 0)
            */
            void Reset(const PixelData& data);
            
            /**
               Initializes the iterator to point to the origin of the given
               pixel data
            */
            Iterator(PixelData& data);

            /**
               Initializes the iterator to point to the origin of the given
               Bitmap
            */
            Iterator(wxBitmap& bmp, PixelData& data);

            /**
               Default constructor
            */
            Iterator();

            /**
               Return true if this iterator is valid
            */
            bool IsOk() const;

            /**
               Advance the iterator to the next pixel, prefix version
            */
            Iterator& operator++();

            /**
               Postfix (hence less efficient -- don't use it unless you
               absolutely must) version
            */
            Iterator operator++(int);

            /**
               Move x pixels to the right and y down
               note that the rows don't wrap!
            */
            void Offset(const PixelData& data, int x, int y);

            /**
               Move x pixels to the right (again, no row wrapping)
            */
            void OffsetX(const PixelData&data, int x);

            /**
               Move y rows to the bottom
            */
            void OffsetY(const PixelData& data, int y);

            /**
               Go to the given position
            */
            void MoveTo(const PixelData& data, int x, int y);

    //@{
            /**
               Data access: gives access to invidividual colour components.
            */
            ChannelType& Red();
            ChannelType& Green();
            ChannelType& Blue();
            ChannelType& Alpha();
    //@}
        };
};

