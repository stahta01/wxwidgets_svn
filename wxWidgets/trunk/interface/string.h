/////////////////////////////////////////////////////////////////////////////
// Name:        string.h
// Purpose:     interface of wxStringBuffer
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxStringBuffer
    @wxheader{string.h}

    This tiny class allows to conveniently access the wxString
    internal buffer as a writable pointer without any risk of forgetting to restore
    the string to the usable state later.

    For example, assuming you have a low-level OS function called
    @c GetMeaningOfLifeAsString(char *) returning the value in the provided
    buffer (which must be writable, of course) you might call it like this:

    @code
    wxString theAnswer;
        GetMeaningOfLifeAsString(wxStringBuffer(theAnswer, 1024));
        if ( theAnswer != "42" )
        {
            wxLogError("Something is very wrong!");
        }
    @endcode

    Note that the exact usage of this depends on whether on not wxUSE_STL is
    enabled.  If
    wxUSE_STL is enabled, wxStringBuffer creates a separate empty character buffer,
    and
    if wxUSE_STL is disabled, it uses GetWriteBuf() from wxString, keeping the same
    buffer
    wxString uses intact.  In other words, relying on wxStringBuffer containing the
    old
    wxString data is probably not a good idea if you want to build your program in
    both
    with and without wxUSE_STL.

    @library{wxbase}
    @category{FIXME}
*/
class wxStringBuffer
{
public:
    /**
        Constructs a writable string buffer object associated with the given string
        and containing enough space for at least @a len characters. Basically, this
        is equivalent to calling wxString::GetWriteBuf and
        saving the result.
    */
    wxStringBuffer(const wxString& str, size_t len);

    /**
        Restores the string passed to the constructor to the usable state by calling
        wxString::UngetWriteBuf on it.
    */
    ~wxStringBuffer();

    /**
        Returns the writable pointer to a buffer of the size at least equal to the
        length specified in the constructor.
    */
    wxStringCharType* operator wxStringCharType *();
};



/**
    @class wxString
    @wxheader{string.h}

    wxString is a class representing a character string. It uses 
    reference counting and copy-on-write internally and is not
    thread-safe. Please see the 
    @ref overview_string "wxString overview" and the 
    @ref overview_unicode "Unicode overview" for more information
    about it.
    
    Since wxWidgets 3.0 wxString internally uses UCS-2 (basically 2-byte per
    character wchar_t) under Windows and UTF-8 under Unix, Linux and
    OS X to store its content. Much work has been done to make
    existing code using ANSI string literals work as before.

    wxString implements most of the methods of the
    std::string class. These standard functions are not documented in
    this manual, please see the STL documentation. The behaviour of
    all these functions is identical to the behaviour described there.

    You may notice that wxString sometimes has many functions which do
    the same thing like, for example, wxString::Length, wxString::Len and @c length()
    which all return the string length. In all cases of such duplication the @c std::string
    compatible method (@c length() in this case, always the lowercase version) should be
    used as it will ensure smoother transition to @c std::string when wxWidgets
    starts using it instead of wxString.

        Anything may be concatenated (appended to) with a string. However, you can't
        append something to a C string (including literal constants), so to do this it
        should be converted to a wxString first.
        
        @li @ref operatorout() "operator "
        @li operator+=()
        @li operator+()
        @li Append()
        @li Prepend()

        A string may be constructed either from a C string, (some number of copies of)
        a single character or a wide (UNICODE) string. For all constructors (except the
        default which creates an empty string) there is also a corresponding assignment
        operator.
        
        @li wxString()
        @li operator=()
        @li ~wxString()

        The MakeXXX() variants modify the string in place, while the other functions
        return a new string which contains the original text converted to the upper or
        lower case and leave the original string unchanged.
        
        @li MakeUpper()
        @li Upper()
        @li MakeLower()
        @li Lower()


        Many functions in this section take a character index in the string. As with C
        strings and/or arrays, the indices start from 0, so the first character of a
        string is string[0]. Attempt to access a character beyond the end of the
        string (which may be even 0 if the string is empty) will provoke an assert
        failure in @ref overview_debugging "debug build", but no checks are
        done in release builds.
        This section also contains both implicit and explicit conversions to C style
        strings. Although implicit conversion is quite convenient, it is advised to use
        explicit c_str() method for the sake of clarity. 
        
        @li GetChar()
        @li GetWritableChar()
        @li SetChar()
        @li Last()
        @li operator[]()
        @li c_str()
        @li mb_str()
        @li wc_str()
        @li fn_str()

        The default comparison function Cmp() is case-sensitive and
        so is the default version of IsSameAs(). For case
        insensitive comparisons you should use CmpNoCase() or
        give a second parameter to IsSameAs. This last function is may be more
        convenient if only equality of the strings matters because it returns a boolean
        @true value if the strings are the same and not 0 (which is usually @false
        in C)as Cmp() does.
        Matches() is a poor man's regular expression matcher: it only understands 
        '*' and '?' metacharacters in the sense of DOS command line interpreter.
        StartsWith() is helpful when parsing a line of text which should start
        with some predefined prefix and is more efficient than doing direct string
        comparison as you would also have to precalculate the length of the prefix then.
        
        @li Cmp()
        @li CmpNoCase()
        @li IsSameAs()
        @li Matches()
        @li StartsWith()
        @li EndsWith()

        The string provides functions for conversion to signed and unsigned integer and
        floating point numbers. All three functions take a pointer to the variable to
        put the numeric value in and return @true if the @b entire string could be
        converted to a number.
        
        @li ToLong()
        @li ToLongLong()
        @li ToULong()
        @li ToULongLong()
        @li ToDouble()

        These are "advanced" functions and they will be needed quite rarely.
        Alloc() and Shrink() are only interesting for optimization purposes.
        wxStringBuffer and wxStringBufferLength classes may be very useful
        when working with some external API which requires the caller to provide
        a writable buffer.
        
        @li Alloc()
        @li Shrink()
        @li wxStringBuffer
        @li wxStringBufferLength

        Misc. other string functions.
        
        @li Trim()
        @li Truncate()
        @li Pad()

        These functions return the string length and check whether the string
        is empty or empty it.
        
        @li Len()
        @li IsEmpty()
        @li operator!()
        @li Empty()
        @li Clear()


        These functions allow to extract substring from this string. All of them don't
        modify the original string and return a new string containing the extracted
        substring.
        
        @li Mid()
        @li operator()()
        @li Left()
        @li Right()
        @li BeforeFirst()
        @li BeforeLast()
        @li AfterFirst()
        @li AfterLast()
        @li StartsWith()
        @li EndsWith()

        These functions replace the standard @e strchr() and @e strstr()
        functions.
        
        @li Find()
        @li Replace()

        Both formatted versions (Printf/() and stream-like insertion operators
        exist (for basic types only). Additionally, the Format() function allows
        to use simply append formatted value to a string:

        @li Format()
        @li FormatV()
        @li Printf()
        @li PrintfV()
        @li operator>>()

        These functions are deprecated, please consider using new wxWidgets 2.0
        functions instead of them (or, even better, std::string compatible variants).
        
        Contains(), First(), Freq(), IsAscii(), IsNull(),
        IsNumber(), IsWord(), Last(), Length(), LowerCase(), Remove(), Strip(),
        SubString(), UpperCase()

    @library{wxbase}
    @category{data}

    @stdobjects
    ::Objects:, ::wxEmptyString,

    @see @ref overview_string "wxString overview", @ref overview_unicode
    "Unicode overview"
*/
class wxString
{
public:
    /**
        An 'invalid' value for string index
    */
    static const size_t npos;

    //@{
    /** 
        standard types
    */
    typedef wxUniChar value_type;
    typedef wxUniChar char_type;
    typedef wxUniCharRef reference;
    typedef wxChar* pointer;
    typedef const wxChar* const_pointer;
    typedef size_t size_type;
    typedef wxUniChar const_reference;
    //@}

    /**
       Default constructor
    */
    wxString();
    
    /**
       Creates a string from another string. Just increases the ref 
       count by 1.
    */
    wxString(const wxString& stringSrc);
    

    /**
       Constructs a string from the string literal @c psz using
       the current locale encoding to convert it to Unicode.
    */
    wxString(const char *psz);

    /**
       Constructs a string from the string literal @c psz using
       @c conv to convert it Unicode.
    */
    wxString(const char *psz, const wxMBConv& conv);

    /**
       Constructs a string from the first @ nLength character of the string literal @c psz using
       the current locale encoding to convert it to Unicode.
    */
    wxString(const char *psz, size_t nLength);

    /**
       Constructs a string from the first @ nLength character of the string literal @c psz using
       @c conv to convert it Unicode.
    */
    wxString(const char *psz, const wxMBConv& conv, size_t nLength);

    /**
       Constructs a string from the string literal @c pwz.
    */
    wxString(const wchar_t *pwz);

    /**
       Constructs a string from the first @ nLength characters of the string literal @c pwz.
    */
    wxString(const wchar_t *pwz, size_t nLength);

    /**
       Constructs a string from @c buf using the using
       the current locale encoding to convert it to Unicode.
    */
    wxString(const wxCharBuffer& buf);
    
    /**
       Constructs a string from @c buf.
    */
    wxString(const wxWCharBuffer& buf);

    /**
       Constructs a string from @str using the using
       the current locale encoding to convert it to Unicode.
    */
    wxString(const std::string& str);
    
    /**
       Constructs a string from @str.
    */
    wxString(const std::wstring& str);
    

    /**
        String destructor. Note that this is not virtual, so wxString must not be
        inherited from.
    */
    ~wxString();

    /**
        Gets all the characters after the first occurrence of @e ch.
        Returns the empty string if @a ch is not found.
    */
    wxString AfterFirst(wxUniChar ch) const;

    /**
        Gets all the characters after the last occurrence of @e ch.
        Returns the whole string if @a ch is not found.
    */
    wxString AfterLast(wxUniChar ch) const;

    /**
        Preallocate enough space for wxString to store @a nLen characters.

        Please note that this method does the same thing as the standard
        reserve() one and shouldn't be used in new code.

        This function may be used to increase speed when the string is
        constructed by repeated concatenation as in

        @code
            // delete all vowels from the string
            wxString DeleteAllVowels(const wxString& original)
            {
                wxString result;

                size_t len = original.length();

                result.Alloc(len);

                for ( size_t n = 0; n < len; n++ )
                {
                    if ( strchr("aeuio", tolower(original[n])) == NULL )
                        result += original[n];
                }

                return result;
            }
        @endcode

        because it will avoid the need to reallocate string memory many times
        (in case of long strings). Note that it does not set the maximal length
        of a string -- it will still expand if more than @a nLen characters are
        stored in it. Also, it does not truncate the existing string (use
        Truncate() for this) even if its current length is greater than @a nLen.

        @return @true if memory was successfully allocated, @false otherwise.
    */
    bool Alloc(size_t nLen);

    //@{
    /**
       Appends the string or string literal or character.
    */
    wxString& Append(const char* psz, size_t nLen);
    wxString& Append(const wchar_t* pwz, size_t nLen)
    wxString &Append(const wxString &s);
    wxString &Append(wxUniChar ch, size_t count = 1u);
    //@}

    /**
        Gets all characters before the first occurrence of @e ch.
        Returns the whole string if @a ch is not found.
    */
    wxString BeforeFirst(wxUniChar ch) const;

    /**
        Gets all characters before the last occurrence of @e ch.
        Returns the empty string if @a ch is not found.
    */
    wxString BeforeLast(wxUniChar ch) const;


    /**
        Empties the string and frees memory occupied by it.
        See also: Empty()
    */
    void Clear();

    /**
        Returns a deep copy of the string.

        That is, the returned string is guaranteed to not share data with this
        string when using reference-counted wxString implementation.

        This method is primarily useful for passing strings between threads
        (because wxString is not thread-safe). Unlike creating a copy using
        @c wxString(c_str()), Clone() handles embedded NULs correctly.

        @since 2.9.0
     */
    wxString Clone() const;

    /**
        Case-sensitive comparison.
        Returns a positive value if the string is greater than the argument,
        zero if it is equal to it or a negative value if it is less than the
        argument (same semantics as the standard @e strcmp() function).
        
        See also CmpNoCase(), IsSameAs().
    */
    int Cmp(const wxString& s) const;

    /**
        Case-insensitive comparison.
        Returns a positive value if the string is greater than the argument,
        zero if it is equal to it or a negative value if it is less than the
        argument (same semantics as the standard @e strcmp() function).
        
        See also Cmp(), IsSameAs().
    */
    int CmpNoCase(const wxString& s) const;


    //@{
    /**
        Comparison operators
    */
    bool operator ==(const wxString& x, const wxString& y);
    bool operator ==(const wxString& x, wxUniChar ch);
    bool operator !=(const wxString& x, const wxString& y);
    bool operator !=(const wxString& x, wxUniChar ch);
    bool operator(const wxString& x, const wxString& y);
    bool operator(const wxString& x, wxUniChar ch);
    bool operator =(const wxString& x, const wxString& y);
    bool operator =(const wxString& x, wxUniChar ch);
    bool operator(const wxString& x, const wxString& y);
    bool operator(const wxString& x, wxUniChar ch);
    bool operator =(const wxString& x, const wxString& y);
    bool operator =(const wxString& x, wxUniChar ch);
    //@}


    /**
        Returns @true if target appears anywhere in wxString; else @false.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    bool Contains(const wxString& str) const;


    /**
        Makes the string empty, but doesn't free memory occupied by the string.
        See also: Clear().
    */
    void Empty();

    /**
        This function can be used to test if the string ends with the specified
        @e suffix. If it does, the function will return @true and put the
        beginning of the string before the suffix into @a rest string if it is not
        @NULL. Otherwise, the function returns @false and doesn't
        modify the @e rest.
    */
    bool EndsWith(const wxString& suffix, wxString rest = NULL) const;

    //@{
    /**
        Searches for the given string. Returns the starting index, or
        @c wxNOT_FOUND if not found.
    */
    int Find(wxUniChar ch, bool fromEnd = false) const;
    int Find(const wxString& sub) const;
    //@}

    //@{
    /**
        Same as Find().
        This is a wxWidgets 1.xx compatibility function; 
        you should not use it in new code.
    */
    int First(wxUniChar ch) const;
    int First(const wxString& str) const;
    //@}

    /**
        This static function returns the string containing the result of calling
        Printf() with the passed parameters on it.

        @see FormatV(), Printf()
    */
    static wxString Format(const wxChar format, ...);

    /**
        This static function returns the string containing the result of calling
        PrintfV() with the passed parameters on it.

        @see Format(), PrintfV()
    */
    static wxString FormatV(const wxChar format, va_list argptr);

    /**
        Returns the number of occurrences of @a ch in the string.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    int Freq(wxUniChar ch) const;

    //@{
    /**
        Converts given buffer of binary data from 8-bit string to wxString. In
        Unicode build, the string is interpreted as being in ISO-8859-1
        encoding. The version without @a len parameter takes NUL-terminated
        data.

        This is a convenience method useful when storing binary data in
        wxString. It should be used @em only for that purpose and only in
        conjunction with To8BitData(). Use mb_str() for conversion of character
        data to known encoding.

        @since 2.8.4

        @see wxString::To8BitData()
    */
    static wxString From8BitData(const char* buf, size_t len);
    static wxString From8BitData(const char* buf);
    //@}

    //@{
    /**
        Converts the string or character from an ASCII, 7-bit form
        to the native wxString representation. 
    */
    static wxString FromAscii(const char* s);
    static wxString FromAscii(const unsigned char* s);
    static wxString FromAscii(const char* s, size_t len);
    static wxString FromAscii(const unsigned char* s, size_t len);
    static wxString FromAscii(char c);
    //@}

    //@{
    /**
        Converts C string encoded in UTF-8 to wxString.
        Note that this method assumes that @a s is a valid UTF-8 sequence and
        doesn't do any validation in release builds, it's validity is only checked in
        debug builds.
    */
    static wxString FromUTF8(const char* s);
    static wxString FromUTF8(const char* s, size_t len);
    //@}

    /**
        Returns the character at position @a n (read-only).
    */
    wxUniChar GetChar(size_t n) const;

    /**
        wxWidgets compatibility conversion. Same as c_str().
    */
    const wxCStrData* GetData() const;

    /**
        Returns a reference to the character at position @e n.
    */
    wxUniCharRef GetWritableChar(size_t n);

    /**
        Returns a writable buffer of at least @a len bytes.
        It returns a pointer to a new memory block, and the
        existing data will not be copied.
        Call UngetWriteBuf() as soon as possible to put the
        string back into a reasonable state.
        This method is deprecated, please use wxStringBuffer or
        wxStringBufferLength instead.
    */
    wxStringCharType* GetWriteBuf(size_t len);

    /**
        Returns @true if the string contains only ASCII characters.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    bool IsAscii() const;

    /**
        Returns @true if the string is empty.
    */
    bool IsEmpty() const;

    /**
        Returns @true if the string is empty (same as wxString::IsEmpty).
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    bool IsNull() const;

    /**
        Returns @true if the string is an integer (with possible sign).
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    bool IsNumber() const;

    //@{
    /**
        Test whether the string is equal to the single character @e c. The test is
        case-sensitive if @a caseSensitive is @true (default) or not if it is @c
        @false.
        Returns @true if the string is equal to the character, @false otherwise.
        See also Cmp(), CmpNoCase()
    */
    bool IsSameAs(const wxString &s, bool caseSensitive = true) const;
    bool IsSameAs(wxUniChar ch, bool caseSensitive = true) const;
    //@}

    /**
        Returns @true if the string is a word.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    bool IsWord() const;

    //@{
    /**
        Returns a reference to the last character (writable).
        This is a wxWidgets 1.xx compatibility function; 
        you should not use it in new code.
    */
    wxUniCharRef Last();
    const wxUniChar Last();
    //@}

    /**
        Returns the first @a count characters of the string.
    */
    wxString Left(size_t count) const;

    /**
        Returns the length of the string.
    */
    size_t Len() const;

    /**
        Returns the length of the string (same as Len).
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    size_t Length() const;

    /**
        Returns this string converted to the lower case.
    */
    wxString Lower() const;

    /**
        Same as MakeLower.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    void LowerCase();

    /**
        Converts all characters to lower case and returns the result.
    */
    wxString& MakeLower();

    /**
        Converts all characters to upper case and returns the result.
    */
    wxString& MakeUpper();

    /**
        Returns @true if the string contents matches a mask containing '*' and '?'.
    */
    bool Matches(const wxString& mask) const;

    /**
        Returns a substring starting at @e first, with length @e count, or the rest of
        the string if @a count is the default value.
    */
    wxString Mid(size_t first, size_t count = wxSTRING_MAXLEN) const;


    /**
        Adds @a count copies of @a pad to the beginning, or to the end of the
        string (the default).  Removes spaces from the left or from the right (default).
    */
    wxString& Pad(size_t count, wxUniChar pad = ' ',
                 bool fromRight = true);

    /**
        Prepends @a str to this string, returning a reference to this string.
    */
    wxString& Prepend(const wxString& str);

    /**
        Similar to the standard function @e sprintf(). Returns the number of
        characters written, or an integer less than zero on error.
        Note that if @c wxUSE_PRINTF_POS_PARAMS is set to 1, then this function supports
        Unix98-style positional parameters:

        @note This function will use a safe version of @e vsprintf() (usually called
        @e vsnprintf()) whenever available to always allocate the buffer of correct
        size. Unfortunately, this function is not available on all platforms and the
        dangerous @e vsprintf() will be used then which may lead to buffer overflows.
    */
    int Printf(const wxChar* pszFormat, ...);

    /**
        Similar to vprintf. Returns the number of characters written, or an integer
        less than zero
        on error.
    */
    int PrintfV(const wxChar* pszFormat, va_list argPtr);

    //@{
    /**
        Removes @a len characters from the string, starting at @e pos.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    wxString Remove(size_t pos);
    wxString Remove(size_t pos, size_t len);
    //@}

    /**
        Removes the last character.
    */
    wxString RemoveLast();

    /**
        Replace first (or all) occurrences of substring with another one.
        @e replaceAll: global replace (default), or only the first occurrence.
        Returns the number of replacements made.
    */
    size_t Replace(const wxString& strOld, const wxString& strNew,
                   bool replaceAll = true);

    /**
        Returns the last @a count characters.
    */
    wxString Right(size_t count) const;

    /**
        Sets the character at position @e n.
    */
    void SetChar(size_t n, wxUniChar ch);

    /**
        Minimizes the string's memory. This can be useful after a call to
        Alloc() if too much memory were preallocated.
    */
    void Shrink();

    /**
        This function can be used to test if the string starts with the specified
        @e prefix. If it does, the function will return @true and put the rest
        of the string (i.e. after the prefix) into @a rest string if it is not
        @NULL. Otherwise, the function returns @false and doesn't modify the
        @e rest.
    */
    bool StartsWith(const wxString& prefix, wxString rest = NULL) const;

    /**
        Strip characters at the front and/or end. The same as Trim except that it
        doesn't change this string.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    wxString Strip(stripType s = trailing) const;

    /**
        Returns the part of the string between the indices @a from and @e to
        inclusive.
        This is a wxWidgets 1.xx compatibility function, use Mid()
        instead (but note that parameters have different meaning).
    */
    wxString SubString(size_t from, size_t to) const;

    //@{
    /**
        Converts the string to an 8-bit string in ISO-8859-1 encoding in the
        form of a wxCharBuffer (Unicode builds only).

        This is a convenience method useful when storing binary data in
        wxString. It should be used @em only for this purpose. It is only valid
        to call this method on strings created using From8BitData().

        @since 2.8.4

        @see wxString::From8BitData()
    */
    const char* To8BitData() const;
    const const wxCharBuffer To8BitData() const;
    //@}

    //@{
    /**
        Converts the string to an ASCII, 7-bit string in the form of
        a wxCharBuffer (Unicode builds only) or a C string (ANSI builds).
        Note that this conversion only works if the string contains only ASCII
        characters. The @ref mbstr() mb_str method provides more
        powerful means of converting wxString to C string.
    */
    const char* ToAscii() const;
    const const wxCharBuffer ToAscii() const;
    //@}

    /**
        Attempts to convert the string to a floating point number. Returns @true on
        success (the number is stored in the location pointed to by @e val) or @false
        if the string does not represent such number (the value of @a val is not
        modified in this case).

        @see ToLong(), ToULong()
    */
    bool ToDouble(double val) const;

    /**
        Attempts to convert the string to a signed integer in base @e base. Returns
        @true on success in which case the number is stored in the location
        pointed to by @a val or @false if the string does not represent a
        valid number in the given base (the value of @a val is not modified
        in this case).
        The value of @a base must be comprised between 2 and 36, inclusive, or
        be a special value 0 which means that the usual rules of @c C numbers are
        applied: if the number starts with @c 0x it is considered to be in base
        16, if it starts with @c 0 - in base 8 and in base 10 otherwise. Note
        that you may not want to specify the base 0 if you are parsing the numbers
        which may have leading zeroes as they can yield unexpected (to the user not
        familiar with C) results.

        @see ToDouble(), ToULong()
    */
    bool ToLong(long val, int base = 10) const;

    /**
        This is exactly the same as ToLong() but works with 64
        bit integer numbers.
        Notice that currently it doesn't work (always returns @false) if parsing of 64
        bit numbers is not supported by the underlying C run-time library. Compilers
        with C99 support and Microsoft Visual C++ version 7 and higher do support this.

        @see ToLong(), ToULongLong()
    */
    bool ToLongLong(wxLongLong_t val, int base = 10) const;

    /**
        Attempts to convert the string to an unsigned integer in base @e base.
        Returns @true on success in which case the number is stored in the
        location pointed to by @a val or @false if the string does not
        represent a valid number in the given base (the value of @a val is not
        modified in this case). Please notice that this function
        behaves in the same way as the standard @c strtoul() and so it simply
        converts negative numbers to unsigned representation instead of rejecting them
        (e.g. -1 is returned as @c ULONG_MAX).
        See ToLong() for the more detailed
        description of the @a base parameter.

        @see ToDouble(), ToLong()
    */
    bool ToULong(unsigned long val, int base = 10) const;

    /**
        This is exactly the same as ToULong() but works with 64
        bit integer numbers.
        Please see ToLongLong() for additional remarks.
    */
    bool ToULongLong(wxULongLong_t val, int base = 10) const;

    //@{
    /**
        Same as utf8_str().
    */
    const char* ToUTF8() const;
    const const wxCharBuffer ToUF8() const;
    //@}

    /**
        Removes white-space (space, tabs, form feed, newline and carriage return) from
        the left or from the right end of the string (right is default).
    */
    wxString& Trim(bool fromRight = true);

    /**
        Truncate the string to the given length.
    */
    wxString& Truncate(size_t len);

    //@{
    /**
        Puts the string back into a reasonable state (in which it can be used
        normally), after
        GetWriteBuf() was called.
        The version of the function without the @a len parameter will calculate the
        new string length itself assuming that the string is terminated by the first
        @c NUL character in it while the second one will use the specified length
        and thus is the only version which should be used with the strings with
        embedded @c NULs (it is also slightly more efficient as @c strlen()
        doesn't have to be called).
        This method is deprecated, please use
        wxStringBuffer or
        wxStringBufferLength instead.
    */
    void UngetWriteBuf();
    void UngetWriteBuf(size_t len);
    //@}

    /**
        Returns this string converted to upper case.
    */
    wxString Upper() const;

    /**
        The same as MakeUpper.
        This is a wxWidgets 1.xx compatibility function; you should not use it in new
        code.
    */
    void UpperCase();

    /**
        Returns a pointer to the string data (@c const char* when using UTF-8
        internally, @c const wchar_t* when using UCS-2 internally).
        
        Note that the returned value is not convertible to @c char* or
        @c wchar_t*, use char_str() or wchar_str() if you need to pass
        string value to a function expecting non-const pointer.
    */
    const wxCStrData c_str() const;

    /**
        Returns an object with string data that is implicitly convertible to
        @c char* pointer. Note that any change to the returned buffer is lost and so
        this function is only usable for passing strings to legacy libraries that
        don't have const-correct API. Use wxStringBuffer if you want to modify
        the string.

        @see c_str()
    */
    wxWritableCharBuffer char_str(const wxMBConv& conv = wxConvLibc) const;

    //@{
    /**
        Returns string representation suitable for passing to OS' functions
        for file handling. 
    */
    const wchar_t* fn_str() const;
    const char* fn_str() const;
    const wxCharBuffer fn_str() const;
    //@}

    //@{
    /**
        Returns multibyte (C string) representation of the string.
        In Unicode build, converts using @e conv's wxMBConv::cWC2MB
        method and returns wxCharBuffer. In ANSI build, this function
        is same as c_str().
        The macro wxWX2MBbuf is defined as the correct return type (without const).

        @see wxMBConv, c_str(), wc_str(), fn_str(), char_str()
    */
    const char* mb_str(const wxMBConv& conv = wxConvLibc) const;
    const const wxCharBuffer mb_str(const wxMBConv& conv = wxConvLibc) const;
    //@}

    /**
        Extraction from a stream.
    */
    friend istream operator(istream& is, wxString& str);

    //@{
    /**
        These functions work as C++ stream insertion operators: they insert the given
        value into the string. Precision or format cannot be set using them, you can
        use Printf() for this.
    */
    wxString operator(const wxString& str);
    wxString operator(wxUniChar ch);
    wxString operator(int i);
    wxString operator(float f);
    wxString operator(double d);
    //@}

    /**
        Same as Mid (substring extraction).
    */
    wxString operator ()(size_t start, size_t len);

    //@{
    /**
        Concatenation: these operators return a new string equal to the
        concatenation of the operands.
    */
    wxString operator +(const wxString& x, const wxString& y);
    wxString operator +(const wxString& x, wxUniChar y);
    //@}

    //@{
    /**
        Concatenation in place: the argument is appended to the string.
    */
    void operator +=(const wxString& str);
    void operator +=(wxUniChar c);
    //@}

    //@{
    /**
        Assignment: the effect of each operation is the same as for the corresponding
        constructor (see @ref construct() "wxString constructors").
    */
    wxString operator =(const wxString& str);
    wxString operator =(wxUniChar c);
    //@}

    //@{
    /**
        Element extraction.
    */
    wxUniChar operator [](size_t i) const;
    wxUniCharRef operator [](size_t i);
    //@}

    /**
        Empty string is @false, so !string will only return @true if the
        string is empty.
        
        See also IsEmpty().
    */
    bool operator!() const;


    //@{
    /**
        Converts the strings contents to UTF-8 and returns it either as a
        temporary wxCharBuffer object or as a pointer to the internal
        string contents in UTF-8 build.
    */
    const char* utf8_str() const;
    const wxCharBuffer utf8_str() const;
    //@}

    //@{
    /**
        Converts the strings contents to the wide character represention
        and returns it as a temporary wxWCharBuffer object or returns a
        pointer to the internal string contents in wide character mode.
        
        The macro wxWX2WCbuf is defined as the correct return
        type (without const).

        @see wxMBConv, c_str(), mb_str(), fn_str(), wchar_str()
    */
    const wchar_t* wc_str() const;
    const wxWCharBuffer wc_str() const;
    //@}

    /**
        Returns an object with string data that is implicitly convertible to
        @c char* pointer. Note that changes to the returned buffer may or may
        not be lost (depending on the build) and so this function is only usable for
        passing strings to legacy libraries that don't have const-correct API. Use
        wxStringBuffer if you want to modify the string.

        @see mb_str(), wc_str(), fn_str(), c_str(), char_str()
    */
    wxWritableWCharBuffer wchar_str() const;

    /**
        The supported STL functions are listed here. Please see any 
        STL reference for their documentation.
    */
    //@{
        const_iterator begin() const;
        iterator begin();
        const_iterator end() const;
        iterator end();

        const_reverse_iterator rbegin() const;
        reverse_iterator rbegin();
        const_reverse_iterator rend() const;
        reverse_iterator rend();

        size_t length() const;
        size_type size() const;
        size_type max_size() const;
        size_type capacity() const;
        void reserve(size_t sz);

        void resize(size_t nSize, wxUniChar ch = wxT('\0'))

        wxString& append(const wxString& str, size_t pos, size_t n);
        wxString& append(const wxString& str);
        wxString& append(const char *sz, size_t n);
        wxString& append(const wchar_t *sz, size_t n);
        wxString& append(size_t n, wxUniChar ch);
        wxString& append(const_iterator first, const_iterator last);

        wxString& assign(const wxString& str, size_t pos, size_t n);
        wxString& assign(const wxString& str);
        wxString& assign(const char *sz, size_t n);
        wxString& assign(const wchar_t *sz, size_t n);
        wxString& assign(size_t n, wxUniChar ch);
        wxString& assign(const_iterator first, const_iterator last);

        void clear();
        
        int compare(const wxString& str) const;
        int compare(size_t nStart, size_t nLen, const wxString& str) const;
        int compare(size_t nStart, size_t nLen,
              const wxString& str, size_t nStart2, size_t nLen2) const;
        int compare(size_t nStart, size_t nLen,
              const char* sz, size_t nCount = npos) const;
        int compare(size_t nStart, size_t nLen,
              const wchar_t* sz, size_t nCount = npos) const;

        bool empty() const;

        wxString& erase(size_type pos = 0, size_type n = npos);
        iterator erase(iterator first, iterator last);
        iterator erase(iterator first);

        size_t find(const wxString& str, size_t nStart = 0) const;
        size_t find(const char* sz, size_t nStart = 0, size_t n = npos) const;
        size_t find(const wchar_t* sz, size_t nStart = 0, size_t n = npos) const;
        size_t find(wxUniChar ch, size_t nStart = 0) const;

        wxString& insert(size_t nPos, const wxString& str);
        wxString& insert(size_t nPos, const wxString& str, size_t nStart, size_t n);
        wxString& insert(size_t nPos, const char *sz, size_t n);
        wxString& insert(size_t nPos, const wchar_t *sz, size_t n);
        wxString& insert(size_t nPos, size_t n, wxUniChar ch);
        iterator insert(iterator it, wxUniChar ch);
        void insert(iterator it, const_iterator first, const_iterator last);
        void insert(iterator it, size_type n, wxUniChar ch);

        wxString& replace(size_t nStart, size_t nLen, const wxString& str);
        wxString& replace(size_t nStart, size_t nLen, size_t nCount, wxUniChar ch);
        wxString& replace(size_t nStart, size_t nLen,
                    const wxString& str, size_t nStart2, size_t nLen2);
        wxString& replace(size_t nStart, size_t nLen,
                    const char* sz, size_t nCount);
        wxString& replace(size_t nStart, size_t nLen,
                    const wchar_t* sz, size_t nCount);
        wxString& replace(size_t nStart, size_t nLen,
                    const wxString& s, size_t nCount);
        wxString& replace(iterator first, iterator last, const wxString& s);
        wxString& replace(iterator first, iterator last, const char* s, size_type n);
        wxString& replace(iterator first, iterator last, const wchar_t* s, size_type n);
        wxString& replace(iterator first, iterator last, size_type n, wxUniChar ch);
        wxString& replace(iterator first, iterator last,
                    const_iterator first1, const_iterator last1);
        wxString& replace(iterator first, iterator last,
                    const char *first1, const char *last1);
        wxString& replace(iterator first, iterator last,
                    const wchar_t *first1, const wchar_t *last1)

        size_t rfind(const wxString& str, size_t nStart = npos) const;
        size_t rfind(const char* sz, size_t nStart = npos, size_t n = npos) const;
        size_t rfind(const wchar_t* sz, size_t nStart = npos, size_t n = npos) const;
        size_t rfind(wxUniChar ch, size_t nStart = npos) const;

        wxString substr(size_t nStart = 0, size_t nLen = npos) const;
    
        void swap(wxString& str)
  
    //@}

};


/**
    FIXME
*/
wxString Objects:
;

/**
    FIXME
*/
wxString wxEmptyString;




/**
    @class wxStringBufferLength
    @wxheader{string.h}

    This tiny class allows to conveniently access the wxString
    internal buffer as a writable pointer without any risk of forgetting to restore
    the string to the usable state later, and allows the user to set the internal
    length of the string.

    For example, assuming you have a low-level OS function called
    @c int GetMeaningOfLifeAsString(char *) copying the value in the provided
    buffer (which must be writable, of course), and returning the actual length
    of the string, you might call it like this:

    @code
    wxString theAnswer;
        wxStringBuffer theAnswerBuffer(theAnswer, 1024);
        int nLength = GetMeaningOfLifeAsString(theAnswerBuffer);
        theAnswerBuffer.SetLength(nLength);
        if ( theAnswer != "42" )
        {
            wxLogError("Something is very wrong!");
        }
    @endcode

    Note that the exact usage of this depends on whether on not wxUSE_STL is
    enabled.  If
    wxUSE_STL is enabled, wxStringBuffer creates a separate empty character buffer,
    and
    if wxUSE_STL is disabled, it uses GetWriteBuf() from wxString, keeping the same
    buffer
    wxString uses intact.  In other words, relying on wxStringBuffer containing the
    old
    wxString data is probably not a good idea if you want to build your program in
    both
    with and without wxUSE_STL.

    Note that SetLength @c must be called before wxStringBufferLength destructs.

    @library{wxbase}
    @category{FIXME}
*/
class wxStringBufferLength
{
public:
    /**
        Constructs a writable string buffer object associated with the given string
        and containing enough space for at least @a len characters. Basically, this
        is equivalent to calling wxString::GetWriteBuf and
        saving the result.
    */
    wxStringBufferLength(const wxString& str, size_t len);

    /**
        Restores the string passed to the constructor to the usable state by calling
        wxString::UngetWriteBuf on it.
    */
    ~wxStringBufferLength();

    /**
        Sets the internal length of the string referred to by wxStringBufferLength to
        @a nLength characters.
        Must be called before wxStringBufferLength destructs.
    */
    void SetLength(size_t nLength);

    /**
        Returns the writable pointer to a buffer of the size at least equal to the
        length specified in the constructor.
    */
    wxChar* operator wxChar *();
};

