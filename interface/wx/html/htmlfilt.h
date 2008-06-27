/////////////////////////////////////////////////////////////////////////////
// Name:        html/htmlfilt.h
// Purpose:     interface of wxHtmlFilter
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxHtmlFilter

    This class is the parent class of input filters for wxHtmlWindow.
    It allows you to read and display files of different file formats.

    @library{wxhtml}
    @category{FIXME}

    @see Overview()
*/
class wxHtmlFilter : public wxObject
{
public:
    /**
        Constructor.
    */
    wxHtmlFilter();

    /**
        Returns @true if this filter is capable of reading file @e file.
        Example:
    */
    bool CanRead(const wxFSFile& file);

    /**
        Reads the file and returns string with HTML document.
        Example:
    */
    wxString ReadFile(const wxFSFile& file);
};

