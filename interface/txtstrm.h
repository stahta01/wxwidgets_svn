/////////////////////////////////////////////////////////////////////////////
// Name:        txtstrm.h
// Purpose:     documentation for wxTextInputStream class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxTextInputStream
    @wxheader{txtstrm.h}
    
    This class provides functions that read text datas using an input stream.
    So, you can read @e text floats, integers.
    
    The wxTextInputStream correctly reads text files (or streams) in DOS, Macintosh
    and Unix formats and reports a single newline char as a line ending.
    
    Operator  is overloaded and you can use this class like a standard C++ iostream.
    Note, however, that the arguments are the fixed size types wxUint32, wxInt32 etc
    and on a typical 32-bit computer, none of these match to the "long" type
    (wxInt32
    is defined as int on 32-bit architectures) so that you cannot use long. To avoid
    problems (here and elsewhere), make use of wxInt32, wxUint32 and similar types.
    
    If you're scanning through a file using wxTextInputStream, you should check for
    EOF @b before
    reading the next item (word / number), because otherwise the last item may get
    lost. 
    You should however be prepared to receive an empty item (empty string / zero
    number) at the
    end of file, especially on Windows systems. This is unavoidable because most
    (but not all) files end
    with whitespace (i.e. usually a newline).
    
    For example:
    
    @code
    wxFileInputStream input( "mytext.txt" );
      wxTextInputStream text( input );
      wxUint8 i1;
      float f2;
      wxString line;
    
      text  i1;       // read a 8 bit integer.
      text  i1  f2; // read a 8 bit integer followed by float.
      text  line;     // read a text line
    @endcode
    
    @library{wxbase}
    @category{streams}
    
    @seealso
    wxTextInputStream::SetStringSeparators
*/
class wxTextInputStream 
{
public:
    /**
        )
        
        Constructs a text stream associated to the given input stream.
        
        @param stream 
        The underlying input stream.
        
        @param sep 
        The initial string separator characters.
        
        @param conv 
        In Unicode build only: The encoding converter used to convert the bytes in the
          underlying input stream to characters.
    */
    wxTextInputStream(wxInputStream& stream,
                      const wxString& sep=" \t");

    /**
        Destroys the wxTextInputStream object.
    */
    ~wxTextInputStream();

    /**
        Reads a character, returns 0 if there are no more characters in the stream.
    */
    wxChar GetChar();

    /**
        Reads a unsigned 16 bit integer from the stream.
        
        See wxTextInputStream::Read8 for the
        description of the @e base parameter.
    */
    wxUint16 Read16(int base = 10);

    /**
        Reads a signed 16 bit integer from the stream.
        
        See wxTextInputStream::Read8 for the
        description of the @e base parameter.
    */
    wxInt16 Read16S(int base = 10);

    /**
        Reads a 32 bit unsigned integer from the stream.
        
        See wxTextInputStream::Read8 for the
        description of the @e base parameter.
    */
    wxUint32 Read32(int base = 10);

    /**
        Reads a 32 bit signed integer from the stream.
        
        See wxTextInputStream::Read8 for the
        description of the @e base parameter.
    */
    wxInt32 Read32S(int base = 10);

    /**
        Reads a single unsigned byte from the stream, given in base @e base.
        
        The value of @e base must be comprised between 2 and 36, inclusive, or
        be a special value 0 which means that the usual rules of @c C numbers are
        applied: if the number starts with @c 0x it is considered to be in base
        16, if it starts with @c 0 - in base 8 and in base 10 otherwise. Note
        that you may not want to specify the base 0 if you are parsing the numbers
        which may have leading zeroes as they can yield unexpected (to the user not
        familiar with C) results.
    */
    wxUint8 Read8(int base = 10);

    /**
        Reads a single signed byte from the stream.
        
        See wxTextInputStream::Read8 for the
        description of the @e base parameter.
    */
    wxInt8 Read8S(int base = 10);

    /**
        Reads a double (IEEE encoded) from the stream.
    */
    double ReadDouble();

    /**
        Reads a line from the input stream and returns it (without the end of line
        character).
    */
    wxString ReadLine();

    /**
        @b NB: This method is deprecated, use ReadLine() 
        or ReadWord() instead.
        
        Same as ReadLine().
    */
    wxString ReadString();

    /**
        Reads a word (a sequence of characters until the next separator) from the
        input stream.
        
        @sa SetStringSeparators()
    */
    wxString ReadWord();

    /**
        Sets the characters which are used to define the word boundaries in 
        ReadWord().
        
        The default separators are the space and @c TAB characters.
    */
    void SetStringSeparators(const wxString& sep);
};


/**
    @class wxTextOutputStream
    @wxheader{txtstrm.h}
    
    This class provides functions that write text datas using an output stream.
    So, you can write @e text floats, integers.
    
    You can also simulate the C++ cout class:
    
    @code
    wxFFileOutputStream output( stderr );
      wxTextOutputStream cout( output );
    
      cout  "This is a text line"  endl;
      cout  1234;
      cout  1.23456;
    @endcode
    
    The wxTextOutputStream writes text files (or streams) on DOS, Macintosh
    and Unix in their native formats (concerning the line ending).
    
    @library{wxbase}
    @category{streams}
*/
class wxTextOutputStream 
{
public:
    /**
        )
        
        Constructs a text stream object associated to the given output stream.
        
        @param stream 
        The output stream.
        
        @param mode 
        The end-of-line mode. One of wxEOL_NATIVE, wxEOL_DOS, wxEOL_MAC and wxEOL_UNIX.
        
        @param conv 
        In Unicode build only: The object used to convert
        Unicode text into ASCII characters written to the output stream.
    */
    wxTextOutputStream(wxOutputStream& stream,
                       wxEOL mode = wxEOL_NATIVE);

    /**
        Destroys the wxTextOutputStream object.
    */
    ~wxTextOutputStream();

    /**
        Returns the end-of-line mode. One of @b wxEOL_DOS, @b wxEOL_MAC and @b
        wxEOL_UNIX.
    */
    wxEOL GetMode();

    /**
        Writes a character to the stream.
    */
    void PutChar(wxChar c);

    /**
        Set the end-of-line mode. One of @b wxEOL_NATIVE, @b wxEOL_DOS, @b wxEOL_MAC
        and @b wxEOL_UNIX.
    */
    void SetMode(wxEOL mode = wxEOL_NATIVE);

    /**
        Writes the 16 bit integer @e i16 to the stream.
    */
    void Write16(wxUint16 i16);

    /**
        Writes the 32 bit integer @e i32 to the stream.
    */
    void Write32(wxUint32 i32);

    /**
        Writes the single byte @e i8 to the stream.
    */
    void Write8(wxUint8 i8);

    /**
        Writes the double @e f to the stream using the IEEE format.
    */
    virtual void WriteDouble(double f);

    /**
        Writes @e string as a line. Depending on the end-of-line mode the end of
        line ('\n') characters in the string are converted to the correct
        line ending terminator.
    */
    virtual void WriteString(const wxString& string);
};
