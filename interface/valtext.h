/////////////////////////////////////////////////////////////////////////////
// Name:        valtext.h
// Purpose:     documentation for wxTextValidator class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxTextValidator
    @wxheader{valtext.h}
    
    wxTextValidator validates text controls, providing a variety of filtering
    behaviours.
    
    For more information, please see @ref overview_validatoroverview "Validator
    overview".
    
    @library{wxcore}
    @category{validator}
    
    @seealso
    @ref overview_validatoroverview "Validator overview", wxValidator,
    wxGenericValidator
*/
class wxTextValidator : public wxValidator
{
public:
    //@{
    /**
        Constructor, taking a style and optional pointer to a wxString variable.
        
        @param style 
        A bitlist of flags, which can be:
        
        
        wxFILTER_NONE
        
        
        No filtering takes place.
        
        wxFILTER_ASCII
        
        
        Non-ASCII characters are filtered out.
        
        wxFILTER_ALPHA
        
        
        Non-alpha characters are filtered out.
        
        wxFILTER_ALPHANUMERIC
        
        
        Non-alphanumeric characters are filtered out.
        
        wxFILTER_NUMERIC
        
        
        Non-numeric characters are filtered out.
        
        wxFILTER_INCLUDE_LIST
        
        
        Use an include list. The validator
        checks if the user input is on the list, complaining if not. See 
        SetIncludes().
        
        wxFILTER_EXCLUDE_LIST
        
        
        Use an exclude list. The validator
        checks if the user input is on the list, complaining if it is. See 
        SetExcludes().
        
        wxFILTER_INCLUDE_CHAR_LIST
        
        
        Use an include list. The validator
        checks if each input character is in the list (one character per list element),
        complaining if not.
        See SetIncludes().
        
        wxFILTER_EXCLUDE_CHAR_LIST
        
        
        Use an include list. The validator
        checks if each input character is in the list (one character per list element),
        complaining if it is.
        See SetExcludes().
        
        @param valPtr 
        A pointer to a wxString variable that contains the value. This variable
        should have a lifetime equal to or longer than the validator lifetime (which is
        usually
        determined by the lifetime of the window).
    */
    wxTextValidator(const wxTextValidator& validator);
        wxTextValidator(long style = wxFILTER_NONE,
                        wxString* valPtr = @NULL);
    //@}

    /**
        Clones the text validator using the copy constructor.
    */
    virtual wxValidator* Clone();

    /**
        Returns a reference to the exclude list (the list of invalid values).
    */
    wxArrayString GetExcludes();

    /**
        Returns a reference to the include list (the list of valid values).
    */
    wxArrayString GetIncludes();

    /**
        Returns the validator style.
    */
    long GetStyle();

    /**
        Receives character input from the window and filters it according to the
        current validator style.
    */
    void OnChar(wxKeyEvent& event);

    /**
        Sets the exclude list (invalid values for the user input).
    */
    void SetExcludes(const wxArrayString& stringList);

    /**
        Sets the include list (valid values for the user input).
    */
    void SetIncludes(const wxArrayString& stringList);

    /**
        Sets the validator style.
    */
    void SetStyle(long style);

    /**
        Transfers the value in the text control to the string.
    */
    virtual bool TransferFromWindow();

    /**
        Transfers the string value to the text control.
    */
    virtual bool TransferToWindow();

    /**
        Validates the window contents against the include or exclude lists, depending
        on the validator style.
    */
    virtual bool Validate(wxWindow* parent);
};
