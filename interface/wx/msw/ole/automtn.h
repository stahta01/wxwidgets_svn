/////////////////////////////////////////////////////////////////////////////
// Name:        msw/ole/automtn.h
// Purpose:     interface of wxAutomationObject
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    Automation object creation flags.

    These flags can be used with wxAutomationObject::GetInstance().

    @since 2.9.2
*/
enum wxAutomationInstanceFlags
{
    /**
        Only use the existing instance, never create a new one.

        This flag can be used to forbid the creation of a new instance if none
        is currently running.
     */
    wxAutomationInstance_UseExistingOnly = 0,

    /**
        Create a new instance if there are no existing ones.

        This flag corresponds to the default behaviour of
        wxAutomationObject::GetInstance() and means that if getting an existing
        instance failed, we should call wxAutomationObject::CreateInstance() to
        create a new one.
     */
    wxAutomationInstance_CreateIfNeeded = 1,

    /**
        Do not show an error message if no existing instance is currently
        running.

        All other errors will still be reported as usual.
     */
    wxAutomationInstance_SilentIfNone = 2
};

/**
    @class wxVariantDataCurrency

    This class represents a thin wrapper for Microsoft Windows CURRENCY type.

    It is used for converting between wxVariant and OLE VARIANT
    with type set to VT_CURRENCY. When wxVariant stores
    wxVariantDataCurrency, it returns "currency" as its type.

    An example of setting and getting CURRENCY value to and from wxVariant:
    @code
    CURRENCY cy;
    wxVariant variant;

    // set wxVariant to currency type
    if ( SUCCEEDED(VarCyFromR8(123.45, &cy)) )  // set cy to 123.45
    {
        variant.SetData(new wxVariantDataCurrency(cy));

        // or instead of the line above you could write:
        // wxVariantDataCurrency wxCy;
        // wxCy.SetValue(cy);
        // variant.SetData(wxCy.Clone());
    }

    // get CURRENCY value from wxVariant
    if ( variant.GetType() == "currency" )
    {
        wxVariantDataCurrency*
            wxCy = wxDynamicCastVariantData(variant.GetData(), wxVariantDataCurrency);
        cy = wxCy->GetValue();
    }
    @endcode

    @onlyfor{wxmsw}
    @since 2.9.5

    @library{wxcore}
    @category{data}

    @see wxAutomationObject, wxVariant, wxVariantData, wxVariantDataErrorCode

    @header{wx/msw/ole/oleutils.h}
*/
class wxVariantDataCurrency : public wxVariantData
{
public:
    /**
        Default constructor initializes the object to 0.0.
    */
    wxVariantDataCurrency();

    /**
        Constructor from CURRENCY.
    */
    wxVariantDataCurrency(CURRENCY value);

    /**
        Returns the stored CURRENCY value.
    */
    CURRENCY GetValue() const;

    /**
        Sets the stored value to @a value.
    */
    void SetValue(CURRENCY value);

    /**
        Returns true if @a data is of wxVariantDataCurency type
        and contains the same CURRENCY value.
    */
    virtual bool Eq(wxVariantData& data) const;

    /**
        Fills the provided string with the textual representation of this
        object.

        The implementation of this method using @c VarBstrFromCy() Windows API
        function with LOCALE_USER_DEFAULT.
    */
    virtual bool Write(wxString& str) const;

    /**
        Returns a copy of itself.
    */
    wxVariantData* Clone() const;

    /**
        Returns "currency".
    */
    virtual wxString GetType() const;

    /**
        Converts the value of this object to wxAny.
    */
    virtual bool GetAsAny(wxAny* any) const;
};


/**
    @class wxVariantDataErrorCode

    This class represents a thin wrapper for Microsoft Windows SCODE type
    (which is the same as HRESULT).

    It is used for converting between a wxVariant and OLE VARIANT with type set
    to VT_ERROR. When wxVariant stores wxVariantDataErrorCode, it returns
    "errorcode" as its type. This class can be used for returning error codes
    of automation calls or exchanging values with other applications: e.g.
    Microsoft Excel returns VARIANTs with VT_ERROR type for cell values with
    errors (one of XlCVError constants, displayed as e.g. "#DIV/0!" or "#REF!"
    there) etc. See wxVariantDataCurrency for an example of how to  exchange
    values between wxVariant and a native type not directly supported by it.

    @onlyfor{wxmsw}
    @since 2.9.5

    @library{wxcore}
    @category{data}

    @see wxAutomationObject, wxVariant, wxVariantData, wxVariantDataCurrency

    @header{wx/msw/ole/oleutils.h}
*/
class wxVariantDataErrorCode : public wxVariantData
{
public:
    /**
        Constructor initializes the object to @a value or S_OK if no value was
        passed.
    */
    wxVariantDataErrorCode(SCODE value = S_OK);

    /**
        Returns the stored SCODE value.
    */
    SCODE GetValue() const;

    /**
        Set the stored value to @a value.
    */
    void SetValue(SCODE value);

    /**
        Returns true if @a data is of wxVariantDataErrorCode type
        and contains the same SCODE value.
    */
    virtual bool Eq(wxVariantData& data) const;

    /**
        Fills the provided string with the textual representation of this
        object.

        The error code is just a number, so it's output as such.
    */
    virtual bool Write(wxString& str) const;

    /**
        Returns a copy of itself.
    */
    wxVariantData* Clone() const;

    /**
        Returns "errorcode".
    */
    virtual wxString GetType() const { return wxS("errorcode"); }

    /**
        Converts the value of this object to wxAny.
    */
    virtual bool GetAsAny(wxAny* any) const;
};


/**
    @class wxAutomationObject

    The @b wxAutomationObject class represents an OLE automation object containing
    a single data member,
    an IDispatch pointer. It contains a number of functions that make it easy to
    perform
    automation operations, and set and get properties. The class makes heavy use of
    the wxVariant class.

    The usage of these classes is quite close to OLE automation usage in Visual
    Basic. The API is
    high-level, and the application can specify multiple properties in a single
    string. The following example
    gets the current Excel instance, and if it exists, makes the active cell bold.

    @code
    wxAutomationObject excelObject;
      if (excelObject.GetInstance("Excel.Application"))
          excelObject.PutProperty("ActiveCell.Font.Bold", @true);
    @endcode

    Note that this class obviously works under Windows only.

    @onlyfor{wxmsw}

    @library{wxcore}
    @category{data}

    @see wxVariant, wxVariantDataCurrency, wxVariantDataErrorCode
*/
class wxAutomationObject : public wxObject
{
public:
    /**
        Constructor, taking an optional IDispatch pointer which will be released when
        the
        object is deleted.
    */
    wxAutomationObject(WXIDISPATCH* dispatchPtr = NULL);

    /**
        Destructor. If the internal IDispatch pointer is non-null, it will be released.
    */
    ~wxAutomationObject();

    //@{
    /**
        Calls an automation method for this object. The first form takes a method name,
        number of
        arguments, and an array of variants. The second form takes a method name and
        zero to six
        constant references to variants. Since the variant class has constructors for
        the basic
        data types, and C++ provides temporary objects automatically, both of the
        following lines
        are syntactically valid:

        Note that @a method can contain dot-separated property names, to save the
        application
        needing to call GetProperty several times using several temporary objects. For
        example:
    */
    wxVariant CallMethod(const wxString& method, int noArgs,
                         wxVariant args[]) const;
    const wxVariant  CallMethod(const wxString& method, ... ) const;
    //@}

    /**
        Creates a new object based on the ProgID, returning @true if the object was
        successfully created,
        or @false if not.
    */
    bool CreateInstance(const wxString& progId) const;

    /**
        Checks if the object is in a valid state.

        Returns @true if the object was successfully initialized or @false if
        it has no valid IDispatch pointer.

        @see GetDispatchPtr()
     */
    bool IsOk() const;

    /**
        Gets the IDispatch pointer.

        Notice that the return value of this function is an untyped pointer but
        it can be safely cast to @c IDispatch.
    */
    void* GetDispatchPtr() const;

    /**
        Retrieves the current object associated with the specified ProgID, and
        attaches the IDispatch pointer to this object.

        If attaching to an existing object failed and @a flags includes
        wxAutomationInstance_CreateIfNeeded flag, a new object will be created.
        Otherwise this function will normally log an error message which may be
        undesirable if the object may or may not exist. The
        wxAutomationInstance_SilentIfNone flag can be used to prevent the error
        from being logged in this case.

        Returns @true if a pointer was successfully retrieved, @false
        otherwise.

        Note that this cannot cope with two instances of a given OLE object being
        active simultaneously,
        such as two copies of Excel running. Which object is referenced cannot
        currently be specified.

        @param progId COM ProgID, e.g. "Excel.Application"
        @param flags The creation flags (this parameters was added in wxWidgets
            2.9.2)
    */
    bool GetInstance(const wxString& progId,
                     int flags = wxAutomationInstance_CreateIfNeeded) const;

    /**
        Retrieves a property from this object, assumed to be a dispatch pointer, and
        initialises @a obj with it.
        To avoid having to deal with IDispatch pointers directly, use this function in
        preference
        to GetProperty() when retrieving objects
        from other objects.
        Note that an IDispatch pointer is stored as a void* pointer in wxVariant
        objects.

        @see GetProperty()
    */
    bool GetObject(wxAutomationObject& obj, const wxString& property,
                   int noArgs = 0,
                   wxVariant args[] = NULL) const;

    //@{
    /**
        Gets a property value from this object. The first form takes a property name,
        number of
        arguments, and an array of variants. The second form takes a property name and
        zero to six
        constant references to variants. Since the variant class has constructors for
        the basic
        data types, and C++ provides temporary objects automatically, both of the
        following lines
        are syntactically valid:

        Note that @a property can contain dot-separated property names, to save the
        application
        needing to call GetProperty several times using several temporary objects.
    */
    wxVariant GetProperty(const wxString& property, int noArgs,
                          wxVariant args[]) const;
    const wxVariant  GetProperty(const wxString& property, ... ) const;
    //@}

    /**
        This function is a low-level implementation that allows access to the IDispatch
        Invoke function.
        It is not meant to be called directly by the application, but is used by other
        convenience functions.

        @param member
            The member function or property name.
        @param action
            Bitlist: may contain DISPATCH_PROPERTYPUT, DISPATCH_PROPERTYPUTREF,
            DISPATCH_METHOD.
        @param retValue
            Return value (ignored if there is no return value)
        @param noArgs
            Number of arguments in args or ptrArgs.
        @param args
            If non-null, contains an array of variants.
        @param ptrArgs
            If non-null, contains an array of constant pointers to variants.

        @return @true if the operation was successful, @false otherwise.

        @remarks Two types of argument array are provided, so that when possible
                 pointers are used for efficiency.
    */
    bool Invoke(const wxString& member, int action,
                wxVariant& retValue, int noArgs,
                wxVariant args[],
                const wxVariant* ptrArgs[] = 0) const;

    //@{
    /**
        Puts a property value into this object. The first form takes a property name,
        number of
        arguments, and an array of variants. The second form takes a property name and
        zero to six
        constant references to variants. Since the variant class has constructors for
        the basic
        data types, and C++ provides temporary objects automatically, both of the
        following lines
        are syntactically valid:

        Note that @a property can contain dot-separated property names, to save the
        application
        needing to call GetProperty several times using several temporary objects.
    */
    bool PutProperty(const wxString& property, int noArgs,
                     wxVariant args[]);
    const bool PutProperty(const wxString& property, ... );
    //@}

    /**
        Sets the IDispatch pointer. This function does not check if there is already an
        IDispatch pointer.
        You may need to cast from IDispatch* to WXIDISPATCH* when calling this function.
    */
    void SetDispatchPtr(WXIDISPATCH* dispatchPtr);
};

