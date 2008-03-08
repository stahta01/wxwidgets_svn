/////////////////////////////////////////////////////////////////////////////
// Name:        statline.h
// Purpose:     documentation for wxStaticLine class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxStaticLine
    @wxheader{statline.h}
    
    A static line is just a line which may be used in a dialog to separate the
    groups of controls. The line may be only vertical or horizontal.
    
    @beginStyleTable
    @style{wxLI_HORIZONTAL}:
           Creates a horizontal line.
    @style{wxLI_VERTICAL}:
           Creates a vertical line.
    @endStyleTable
    
    @library{wxcore}
    @category{FIXME}
    
    @seealso
    wxStaticBox
*/
class wxStaticLine : public wxControl
{
public:
    //@{
    /**
        Constructor, creating and showing a static line.
        
        @param parent 
        Parent window. Must not be @NULL.
        
        @param id 
        Window identifier. The value wxID_ANY indicates a default value.
        
        @param pos 
        Window position. If wxDefaultPosition is specified then a default position
        is chosen.
        
        @param size 
        Size. Note that either the height or the width (depending on
        whether the line if horizontal or vertical) is ignored.
        
        @param style 
        Window style (either wxLI_HORIZONTAL or wxLI_VERTICAL).
        
        @param name 
        Window name.
        
        @sa Create()
    */
    wxStaticLine();
        wxStaticLine(wxWindow* parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxLI_HORIZONTAL,
                     const wxString& name = "staticLine");
    //@}

    /**
        Creates the static line for two-step construction. See wxStaticLine()
        for further details.
    */
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = "staticLine");

    /**
        This static function returns the size which will be given to the smaller
        dimension of the static line, i.e. its height for a horizontal line or its
        width for a vertical one.
    */
    int GetDefaultSize();

    /**
        Returns @true if the line is vertical, @false if horizontal.
    */
    bool IsVertical();
};
