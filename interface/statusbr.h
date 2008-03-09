/////////////////////////////////////////////////////////////////////////////
// Name:        statusbr.h
// Purpose:     documentation for wxStatusBar class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxStatusBar
    @wxheader{statusbr.h}

    A status bar is a narrow window that can be placed along the bottom of a frame
    to give
    small amounts of status information. It can contain one or more fields, one or
    more of which can
    be variable length according to the size of the window.

    wxWindow

    wxEvtHandler

    wxObject

    @beginStyleTable
    @style{wxST_SIZEGRIP}:
           On Windows 95, displays a gripper at right-hand side of the status
           bar.
    @endStyleTable

    @library{wxcore}
    @category{miscwnd}

    @seealso
    wxFrame, @ref overview_samplestatbar "Status bar sample"
*/
class wxStatusBar : public wxWindow
{
public:
    //@{
    /**
        Constructor, creating the window.
        
        @param parent
            The window parent, usually a frame.
        @param id
            The window identifier. It may take a value of -1 to indicate a default
        value.
        @param style
            The window style. See wxStatusBar.
        @param name
            The name of the window. This parameter is used to associate a name with the
        item,
            allowing the application user to set Motif resource values for
            individual windows.
        
        @see Create()
    */
    wxStatusBar();
    wxStatusBar(wxWindow* parent, wxWindowID id = wxID_ANY,
                long style = wxST_SIZEGRIP,
                const wxString& name = "statusBar");
    //@}

    /**
        Destructor.
    */
    ~wxStatusBar();

    /**
        Creates the window, for two-step construction.
        See wxStatusBar() for details.
    */
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY,
                long style = wxST_SIZEGRIP,
                const wxString& name = "statusBar");

    /**
        Returns the size and position of a field's internal bounding rectangle.
        
        @param i
            The field in question.
        @param rect
            The rectangle values are placed in this variable.
        
        @returns @true if the field index is valid, @false otherwise.
        
        @see wxRect
    */
    virtual bool GetFieldRect(int i, wxRect& rect);

    /**
        Returns the number of fields in the status bar.
    */
    int GetFieldsCount();

    /**
        Returns the string associated with a status bar field.
        
        @param i
            The number of the status field to retrieve, starting from zero.
        
        @returns The status field string if the field is valid, otherwise the
                 empty string.
        
        @see SetStatusText()
    */
    virtual wxString GetStatusText(int i = 0);

    /**
        Sets the field text to the top of the stack, and pops the stack of saved
        strings.
        
        @see PushStatusText()
    */
    void PopStatusText(int field = 0);

    /**
        Saves the current field text in a per field stack, and sets the field text
        to the string passed as argument.
    */
    void PushStatusText(const wxString& string, int field = 0);

    /**
        Sets the number of fields, and optionally the field widths.
        
        @param number
            The number of fields.
        @param widths
            An array of n integers interpreted in the same way as
            in SetStatusWidths
    */
    virtual void SetFieldsCount(int number = 1, int* widths = NULL);

    /**
        Sets the minimal possible height for the status bar. The real height may be
        bigger than the height specified here depending on the size of the font used by
        the status bar.
    */
    void SetMinHeight(int height);

    /**
        Sets the styles of the fields in the status line which can make fields appear
        flat
        or raised instead of the standard sunken 3D border.
        
        @param n
            The number of fields in the status bar. Must be equal to the
            number passed to SetFieldsCount the last
            time it was called.
        @param styles
            Contains an array of n integers with the styles for each field. There
            are three possible styles:
        
        
        
        
        
        
        
            wxSB_NORMAL
        
        
        
        
            (default) The field appears sunken with a standard 3D border.
        
        
        
        
        
            wxSB_FLAT
        
        
        
        
            No border is painted around the field so that it appears flat.
        
        
        
        
        
            wxSB_RAISED
        
        
        
        
            A raised 3D border is painted around the field.
    */
    virtual void SetStatusStyles(int n, int* styles);

    /**
        Sets the text for one field.
        
        @param text
            The text to be set. Use an empty string ("") to clear the field.
        @param i
            The field to set, starting from zero.
        
        @see GetStatusText(), wxFrame::SetStatusText
    */
    virtual void SetStatusText(const wxString& text, int i = 0);

    /**
        Sets the widths of the fields in the status line. There are two types of
        fields: fixed widths one and variable width fields. For the fixed width fields
        you should specify their (constant) width in pixels. For the variable width
        fields, specify a negative number which indicates how the field should expand:
        the space left for all variable width fields is divided between them according
        to the absolute value of this number. A variable width field with width of -2
        gets twice as much of it as a field with width -1 and so on.
        For example, to create one fixed width field of width 100 in the right part of
        the status bar and two more fields which get 66% and 33% of the remaining
        space correspondingly, you should use an array containing -2, -1 and 100.
        
        @param n
            The number of fields in the status bar. Must be equal to the
            number passed to SetFieldsCount the last
            time it was called.
        @param widths
            Contains an array of n integers, each of which is
            either an absolute status field width in pixels if positive or indicates a
            variable width field if negative.
        
        @remarks The widths of the variable fields are calculated from the total
                 width of all fields, minus the sum of widths of the
                 non-variable fields, divided by the number of variable
                 fields.
        
        @see SetFieldsCount(), wxFrame::SetStatusWidths
    */
    virtual void SetStatusWidths(int n, int* widths);
};
