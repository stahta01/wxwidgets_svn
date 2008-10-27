/////////////////////////////////////////////////////////////////////////////
// Name:        dataobj.h
// Purpose:     interface of wx*DataObject
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxCustomDataObject

    wxCustomDataObject is a specialization of wxDataObjectSimple for some
    application-specific data in arbitrary (either custom or one of the
    standard ones). The only restriction is that it is supposed that this data
    can be copied bitwise (i.e. with @c memcpy()), so it would be a bad idea to
    make it contain a C++ object (though C struct is fine).

    By default, wxCustomDataObject stores the data inside in a buffer. To put
    the data into the buffer you may use either SetData() or TakeData()
    depending on whether you want the object to make a copy of data or not.

    This class may be used as is, but if you don't want store the data inside
    the object but provide it on demand instead, you should override GetSize(),
    GetData() and SetData() (or may be only the first two or only the last one
    if you only allow reading/writing the data).

    @library{wxcore}
    @category{dnd}

    @see wxDataObject
*/
class wxCustomDataObject : public wxDataObjectSimple
{
public:
    /**
        The constructor accepts a @a format argument which specifies the
        (single) format supported by this object. If it isn't set here,
        wxDataObjectSimple::SetFormat() should be used.
    */
    wxCustomDataObject(const wxDataFormat& format = wxFormatInvalid);

    /**
        The destructor will free the data held by the object. Notice that
        although it calls the virtual Free() function, the base class version
        will always be called (C++ doesn't allow calling virtual functions from
        constructors or destructors), so if you override Free(), you should
        override the destructor in your class as well (which would probably
        just call the derived class' version of Free()).
    */
    virtual ~wxCustomDataObject();

    /**
        This function is called to allocate @a size bytes of memory from
        SetData(). The default version just uses the operator new.
    */
    virtual void* Alloc(size_t size);

    /**
        This function is called when the data is freed, you may override it to
        anything you want (or may be nothing at all). The default version calls
        operator delete[] on the data.
    */
    virtual void Free();

    /**
        Returns a pointer to the data.
    */
    virtual void* GetData() const;

    /**
        Returns the data size in bytes.
    */
    virtual size_t GetSize() const;

    /**
        Set the data. The data object will make an internal copy.

        @beginWxPythonOnly
        This method expects a string in wxPython. You can pass nearly any
        object by pickling it first.
        @endWxPythonOnly
    */
    virtual void SetData(size_t size, const void data);

    /**
        Like SetData(), but doesn't copy the data - instead the object takes
        ownership of the pointer.

        @beginWxPythonOnly
        This method expects a string in wxPython. You can pass nearly any
        object by pickling it first.
        @endWxPythonOnly
    */
    void TakeData(size_t size, void* data);
};



/**
    @class wxDataObjectComposite

    wxDataObjectComposite is the simplest wxDataObject derivation which may be
    used to support multiple formats. It contains several wxDataObjectSimple
    objects and supports any format supported by at least one of them. Only one
    of these data objects is @e preferred (the first one if not explicitly
    changed by using the second parameter of Add()) and its format determines
    the preferred format of the composite data object as well.

    See wxDataObject documentation for the reasons why you might prefer to use
    wxDataObject directly instead of wxDataObjectComposite for efficiency
    reasons.

    @library{wxcore}
    @category{dnd}

    @see @ref overview_dnd, wxDataObject, wxDataObjectSimple, wxFileDataObject,
         wxTextDataObject, wxBitmapDataObject
*/
class wxDataObjectComposite : public wxDataObject
{
public:
    /**
        The default constructor.
    */
    wxDataObjectComposite();

    /**
        Adds the @a dataObject to the list of supported objects and it becomes
        the preferred object if @a preferred is @true.
    */
    void Add(wxDataObjectSimple* dataObject, bool preferred = false);

    /**
        Report the format passed to the SetData() method.  This should be the
        format of the data object within the composite that recieved data from
        the clipboard or the DnD operation.  You can use this method to find
        out what kind of data object was recieved.
    */
    wxDataFormat GetReceivedFormat() const;
};



/**
    @class wxDataObjectSimple

    This is the simplest possible implementation of the wxDataObject class. The
    data object of (a class derived from) this class only supports one format,
    so the number of virtual functions to be implemented is reduced.

    Notice that this is still an abstract base class and cannot be used
    directly, it must be derived. The objects supporting rendering the data
    must override GetDataSize() and GetDataHere() while the objects which may
    be set must override SetData(). Of course, the objects supporting both
    operations must override all three methods.

    @beginWxPythonOnly
    If you wish to create a derived wxDataObjectSimple class in wxPython you
    should derive the class from wxPyDataObjectSimple in order to get
    Python-aware capabilities for the various virtual methods.
    @endWxPythonOnly

    @beginWxPerlOnly
    In wxPerl, you need to derive your data object class from
    Wx::PlDataObjectSimple.
    @endWxPerlOnly

    @library{wxcore}
    @category{dnd}

    @see @ref overview_dnd, @ref page_samples_dnd, wxFileDataObject,
         wxTextDataObject, wxBitmapDataObject
*/
class wxDataObjectSimple : public wxDataObject
{
public:
    /**
        Constructor accepts the supported format (none by default) which may
        also be set later with SetFormat().
    */
    wxDataObjectSimple(const wxDataFormat& format = wxFormatInvalid);

    /**
        Copy the data to the buffer, return @true on success. Must be
        implemented in the derived class if the object supports rendering its
        data.

        @beginWxPythonOnly
        When implementing this method in wxPython, no additional parameters are
        required and the data should be returned from the method as a string.
        @endWxPythonOnly
    */
    virtual bool GetDataHere(void buf) const;

    /**
        Gets the size of our data. Must be implemented in the derived class if
        the object supports rendering its data.
    */
    virtual size_t GetDataSize() const;

    /**
        Returns the (one and only one) format supported by this object. It is
        assumed that the format is supported in both directions.
    */
    const wxDataFormat& GetFormat() const;

    /**
        Copy the data from the buffer, return @true on success. Must be
        implemented in the derived class if the object supports setting its
        data.

        @beginWxPythonOnly
        When implementing this method in wxPython, the data comes as a single
        string parameter rather than the two shown here.
        @endWxPythonOnly
    */
    virtual bool SetData(size_t len, const void buf);

    /**
        Sets the supported format.
    */
    void SetFormat(const wxDataFormat& format);
};



/**
    @class wxBitmapDataObject

    wxBitmapDataObject is a specialization of wxDataObject for bitmap data. It
    can be used without change to paste data into the wxClipboard or a
    wxDropSource. A user may wish to derive a new class from this class for
    providing a bitmap on-demand in order to minimize memory consumption when
    offering data in several formats, such as a bitmap and GIF.

    This class may be used as is, but GetBitmap() may be overridden to increase
    efficiency.

    @beginWxPythonOnly
    If you wish to create a derived wxBitmapDataObject class in wxPython you
    should derive the class from wxPyBitmapDataObject in order to get
    Python-aware capabilities for the various virtual methods.
    @endWxPythonOnly

    @library{wxcore}
    @category{dnd}

    @see @ref overview_dnd, wxDataObject, wxDataObjectSimple, wxFileDataObject,
         wxTextDataObject, wxDataObject
*/
class wxBitmapDataObject : public wxDataObjectSimple
{
public:
    /**
        Constructor, optionally passing a bitmap (otherwise use SetBitmap()
        later).
    */
    wxBitmapDataObject(const wxBitmap& bitmap = wxNullBitmap);

    /**
        Returns the bitmap associated with the data object. You may wish to
        override this method when offering data on-demand, but this is not
        required by wxWidgets' internals. Use this method to get data in bitmap
        form from the wxClipboard.
    */
    virtual wxBitmap GetBitmap() const;

    /**
        Sets the bitmap associated with the data object. This method is called
        when the data object receives data. Usually there will be no reason to
        override this function.
    */
    virtual void SetBitmap(const wxBitmap& bitmap);
};



/**
    @class wxURLDataObject

    wxURLDataObject is a wxDataObject containing an URL and can be used e.g.
    when you need to put an URL on or retrieve it from the clipboard:

    @code
    wxTheClipboard->SetData(new wxURLDataObject(url));
    @endcode

    @note This class is derived from wxDataObjectComposite on Windows rather
          than wxTextDataObject on all other platforms.

    @library{wxcore}
    @category{dnd}

    @see @ref overview_dnd, wxDataObject
*/
class wxURLDataObject: public wxTextDataObject
{
public:
    /**
        Constructor, may be used to initialize the URL. If @a url is empty,
        SetURL() can be used later.
    */
    wxURLDataObject(const wxString& url = wxEmptyString);

    /**
        Returns the URL stored by this object, as a string.
    */
    wxString GetURL() const;

    /**
        Sets the URL stored by this object.
    */
    void SetURL(const wxString& url);
};


/**
    @class wxTextDataObject

    wxTextDataObject is a specialization of wxDataObject for text data. It can
    be used without change to paste data into the wxClipboard or a
    wxDropSource. A user may wish to derive a new class from this class for
    providing text on-demand in order to minimize memory consumption when
    offering data in several formats, such as plain text and RTF because by
    default the text is stored in a string in this class, but it might as well
    be generated when requested. For this, GetTextLength() and GetText() will
    have to be overridden.

    Note that if you already have the text inside a string, you will not
    achieve any efficiency gain by overriding these functions because copying
    wxStrings is already a very efficient operation (data is not actually
    copied because wxStrings are reference counted).

    @beginWxPythonOnly
    If you wish to create a derived wxTextDataObject class in wxPython you
    should derive the class from wxPyTextDataObject in order to get
    Python-aware capabilities for the various virtual methods.
    @endWxPythonOnly

    @library{wxcore}
    @category{dnd}

    @see @ref overview_dnd, wxDataObject, wxDataObjectSimple, wxFileDataObject,
         wxBitmapDataObject
*/
class wxTextDataObject : public wxDataObjectSimple
{
public:
    /**
        Constructor, may be used to initialise the text (otherwise SetText()
        should be used later).
    */
    wxTextDataObject(const wxString& text = wxEmptyString);

    /**
        Returns the text associated with the data object. You may wish to
        override this method when offering data on-demand, but this is not
        required by wxWidgets' internals. Use this method to get data in text
        form from the wxClipboard.
    */
    virtual wxString GetText() const;

    /**
        Returns the data size. By default, returns the size of the text data
        set in the constructor or using SetText(). This can be overridden to
        provide text size data on-demand. It is recommended to return the text
        length plus 1 for a trailing zero, but this is not strictly required.
    */
    virtual size_t GetTextLength() const;

    /**
        Sets the text associated with the data object. This method is called
        when the data object receives the data and, by default, copies the text
        into the member variable. If you want to process the text on the fly
        you may wish to override this function.
    */
    virtual void SetText(const wxString& strText);
};



/**
    @class wxFileDataObject

    wxFileDataObject is a specialization of wxDataObject for file names. The
    program works with it just as if it were a list of absolute file names, but
    internally it uses the same format as Explorer and other compatible
    programs under Windows or GNOME/KDE filemanager under Unix which makes it
    possible to receive files from them using this class.

    @warning Under all non-Windows platforms this class is currently
             "input-only", i.e. you can receive the files from another
             application, but copying (or dragging) file(s) from a wxWidgets
             application is not currently supported. PS: GTK2 should work as
             well.

    @library{wxcore}
    @category{dnd}

    @see wxDataObject, wxDataObjectSimple, wxTextDataObject,
         wxBitmapDataObject, wxDataObject
*/
class wxFileDataObject : public wxDataObjectSimple
{
public:
    /**
        Constructor.
    */
    wxFileDataObject();

    /**
        Adds a file to the file list represented by this data object (Windows
        only).
    */
    void AddFile(const wxString& file);

    /**
        Returns the array of file names.
    */
    const wxArrayString& GetFilenames() const;
};



/**
    @class wxDataFormat

    A wxDataFormat is an encapsulation of a platform-specific format handle
    which is used by the system for the clipboard and drag and drop operations.
    The applications are usually only interested in, for example, pasting data
    from the clipboard only if the data is in a format the program understands
    and a data format is something which uniquely identifies this format.

    On the system level, a data format is usually just a number (@c CLIPFORMAT
    under Windows or @c Atom under X11, for example) and the standard formats
    are, indeed, just numbers which can be implicitly converted to wxDataFormat.
    The standard formats are:

    @beginDefList
    @itemdef{wxDF_INVALID,
             An invalid format - used as default argument for functions taking
             a wxDataFormat argument sometimes.}
    @itemdef{wxDF_TEXT,
             Text format (wxString).}
    @itemdef{wxDF_BITMAP,
             A bitmap (wxBitmap).}
    @itemdef{wxDF_METAFILE,
             A metafile (wxMetafile, Windows only).}
    @itemdef{wxDF_FILENAME,
             A list of filenames.}
    @itemdef{wxDF_HTML,
             An HTML string. This is only valid when passed to
             wxSetClipboardData when compiled with Visual C++ in non-Unicode
             mode.}
    @endDefList

    As mentioned above, these standard formats may be passed to any function
    taking wxDataFormat argument because wxDataFormat has an implicit
    conversion from them (or, to be precise from the type
    @c wxDataFormat::NativeFormat which is the type used by the underlying
    platform for data formats).

    Aside the standard formats, the application may also use custom formats
    which are identified by their names (strings) and not numeric identifiers.
    Although internally custom format must be created (or @e registered) first,
    you shouldn't care about it because it is done automatically the first time
    the wxDataFormat object corresponding to a given format name is created.
    The only implication of this is that you should avoid having global
    wxDataFormat objects with non-default constructor because their
    constructors are executed before the program has time to perform all
    necessary initialisations and so an attempt to do clipboard format
    registration at this time will usually lead to a crash!

    @library{wxbase}
    @category{dnd}

    @see @ref overview_dnd, @ref page_samples_dnd, wxDataObject
*/
class wxDataFormat
{
public:
    /**
        Constructs a data format object for one of the standard data formats or
        an empty data object (use SetType() or SetId() later in this case).
    */
    wxDataFormat(wxDataFormatId format = wxDF_INVALID);

    /**
        Constructs a data format object for a custom format identified by its
        name @a format.
    */
    wxDataFormat(const wxChar format);

    /**
        Returns the name of a custom format (this function will fail for a
        standard format).
    */
    wxString GetId() const;

    /**
        Returns the platform-specific number identifying the format.
    */
    wxDataFormatId GetType() const;

    /**
        Sets the format to be the custom format identified by the given name.
    */
    void SetId(const wxChar format);

    /**
        Sets the format to the given value, which should be one of wxDF_XXX
        constants.
    */
    void SetType(wxDataFormatId type);

    /**
        Returns @true if the formats are different.
    */
    bool operator !=(const wxDataFormat& format) const;

    /**
        Returns @true if the formats are equal.
    */
    bool operator ==(const wxDataFormat& format) const;
};



/**
    @class wxDataObject

    A wxDataObject represents data that can be copied to or from the clipboard,
    or dragged and dropped. The important thing about wxDataObject is that this
    is a 'smart' piece of data unlike 'dumb' data containers such as memory
    buffers or files. Being 'smart' here means that the data object itself
    should know what data formats it supports and how to render itself in each
    of its supported formats.

    A supported format, incidentally, is exactly the format in which the data
    can be requested from a data object or from which the data object may be
    set. In the general case, an object may support different formats on
    'input' and 'output', i.e. it may be able to render itself in a given
    format but not be created from data on this format or vice versa.
    wxDataObject defines an enumeration type which distinguishes between them:

    @code
    enum Direction
    {
        Get  = 0x01,    // format is supported by GetDataHere()
        Set  = 0x02     // format is supported by SetData()
    };
    @endcode

    See wxDataFormat documentation for more about formats.

    Not surprisingly, being 'smart' comes at a price of added complexity. This
    is reasonable for the situations when you really need to support multiple
    formats, but may be annoying if you only want to do something simple like
    cut and paste text.

    To provide a solution for both cases, wxWidgets has two predefined classes
    which derive from wxDataObject: wxDataObjectSimple and
    wxDataObjectComposite. wxDataObjectSimple is the simplest wxDataObject
    possible and only holds data in a single format (such as HTML or text) and
    wxDataObjectComposite is the simplest way to implement a wxDataObject that
    does support multiple formats because it achieves this by simply holding
    several wxDataObjectSimple objects.

    So, you have several solutions when you need a wxDataObject class (and you
    need one as soon as you want to transfer data via the clipboard or drag and
    drop):

    -# Use one of the built-in classes.
        - You may use wxTextDataObject, wxBitmapDataObject or wxFileDataObject
          in the simplest cases when you only need to support one format and
          your data is either text, bitmap or list of files.
    -# Use wxDataObjectSimple
        - Deriving from wxDataObjectSimple is the simplest solution for custom
          data - you will only support one format and so probably won't be able
          to communicate with other programs, but data transfer will work in
          your program (or between different copies of it).
    -# Use wxDataObjectComposite
        - This is a simple but powerful solution which allows you to support
          any number of formats (either standard or custom if you combine it
          with the previous solution).
    -# Use wxDataObject Directly
        - This is the solution for maximal flexibility and efficiency, but it
          is also the most difficult to implement.

    Please note that the easiest way to use drag and drop and the clipboard
    with multiple formats is by using wxDataObjectComposite, but it is not the
    most efficient one as each wxDataObjectSimple would contain the whole data
    in its respective formats. Now imagine that you want to paste 200 pages of
    text in your proprietary format, as well as Word, RTF, HTML, Unicode and
    plain text to the clipboard and even today's computers are in trouble. For
    this case, you will have to derive from wxDataObject directly and make it
    enumerate its formats and provide the data in the requested format on
    demand.

    Note that neither the GTK+ data transfer mechanisms for clipboard and drag
    and drop, nor OLE data transfer, copy any data until another application
    actually requests the data. This is in contrast to the 'feel' offered to
    the user of a program who would normally think that the data resides in the
    clipboard after having pressed 'Copy' - in reality it is only declared to
    be available.

    There are several predefined data object classes derived from
    wxDataObjectSimple: wxFileDataObject, wxTextDataObject, wxBitmapDataObject
    and wxURLDataObject which can be used without change.

    You may also derive your own data object classes from wxCustomDataObject
    for user-defined types. The format of user-defined data is given as a
    mime-type string literal, such as "application/word" or "image/png". These
    strings are used as they are under Unix (so far only GTK+) to identify a
    format and are translated into their Windows equivalent under Win32 (using
    the OLE IDataObject for data exchange to and from the clipboard and for
    drag and drop). Note that the format string translation under Windows is
    not yet finished.

    Each class derived directly from wxDataObject must override and implement
    all of its functions which are pure virtual in the base class. The data
    objects which only render their data or only set it (i.e. work in only one
    direction), should return 0 from GetFormatCount().

    @beginWxPythonOnly
    At this time this class is not directly usable from wxPython. Derive a
    class from wxPyDataObjectSimple() instead.
    @endWxPythonOnly

    @beginWxPerlOnly
    This class is not currently usable from wxPerl; you may use
    Wx::PlDataObjectSimple instead.
    @endWxPerlOnly

    @library{wxcore}
    @category{dnd}

    @see @ref overview_dnd, @ref page_samples_dnd, wxFileDataObject,
         wxTextDataObject, wxBitmapDataObject, wxCustomDataObject,
         wxDropTarget, wxDropSource, wxTextDropTarget, wxFileDropTarget
*/
class wxDataObject
{
public:
    /**
        Constructor.
    */
    wxDataObject();

    /**
        Destructor.
    */
    virtual ~wxDataObject();

    /**
        Copy all supported formats in the given direction to the array pointed
        to by @a formats. There is enough space for GetFormatCount(dir) formats
        in it.
    */
    virtual void GetAllFormats(wxDataFormat* formats,
                                 Direction dir = Get) const;

    /**
        The method will write the data of the format @a format in the buffer
        @a buf and return @true on success, @false on failure.
    */
    virtual bool GetDataHere(const wxDataFormat& format, void* buf) const = 0;

    /**
        Returns the data size of the given format @a format.
    */
    virtual size_t GetDataSize(const wxDataFormat& format) const = 0;

    /**
        Returns the number of available formats for rendering or setting the
        data.
    */
    virtual size_t GetFormatCount(Direction dir = Get) const = 0;

    /**
        Returns the preferred format for either rendering the data (if @a dir
        is @c Get, its default value) or for setting it. Usually this will be
        the native format of the wxDataObject.
    */
    virtual wxDataFormat GetPreferredFormat(Direction dir = Get) const = 0;

    /**
        Set the data in the format @a format of the length @a len provided in
        the buffer @a buf.

        @return @true on success, @false on failure.
    */
    virtual bool SetData(const wxDataFormat& format, size_t len,
                         const void* buf);
};

