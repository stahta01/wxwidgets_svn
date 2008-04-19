/////////////////////////////////////////////////////////////////////////////
// Name:        dcprint.h
// Purpose:     interface of wxPrinterDC
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxPrinterDC
    @wxheader{dcprint.h}

    A printer device context is specific to MSW and Mac, and allows access to
    any printer with a Windows or Macintosh driver. See wxDC for further
    information on device contexts, and wxDC::GetSize() for advice on achieving
    the correct scaling for the page.

    @library{wxcore}
    @category{printing}

    @see @ref overview_printing, wxDC
*/
class wxPrinterDC : public wxDC
{
public:
    /**
        Constructor. Pass a wxPrintData object with information necessary for
        setting up a suitable printer device context. This is the recommended
        way to construct a wxPrinterDC. Make sure you specify a reference to a
        wxPrintData object, not a pointer - you may not even get a warning if
        you pass a pointer instead.
    */
    wxPrinterDC(const wxPrintData& printData);
    /**
        Constructor. With empty strings for the first three arguments, the
        default printer dialog is displayed. @a device indicates the type of
        printer and @a output is an optional file for printing to. The
        @a driver parameter is currently unused.  Use the wxDC::Ok() member to
        test whether the constructor was successful in creating a usable device
        context.

        @deprecated This constructor is deprecated and retained only for
                    backward compatibility.
    */
    wxPrinterDC(const wxString& driver, const wxString& device,
                const wxString& output, const bool interactive = true,
                int orientation = wxPORTRAIT);

    /**
        Return the rectangle in device coordinates that corresponds to the full
        paper area, including the nonprinting regions of the paper. The point
        (0,0) in device coordinates is the top left corner of the page
        rectangle, which is the printable area on MSW and Mac. The coordinates
        of the top left corner of the paper rectangle will therefore have small
        negative values, while the bottom right coordinates will be somewhat
        larger than the values returned by wxDC::GetSize().
    */
    wxRect wxPrinterDC::GetPaperRect();
};

