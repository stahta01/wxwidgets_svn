/////////////////////////////////////////////////////////////////////////////
// Name:        gdiobj.h
// Purpose:     interface of wxGDIObject
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxGDIObject
    @wxheader{gdiobj.h}

    This class allows platforms to implement functionality to optimise GDI objects,
    such
    as wxPen, wxBrush and wxFont. On Windows, the underling GDI objects are a
    scarce resource
    and are cleaned up when a usage count goes to zero. On some platforms this
    class may not have any special functionality.

    Since the functionality of this class is platform-specific, it is not
    documented here in detail.

    @library{wxcore}
    @category{FIXME}

    @see wxPen, wxBrush, wxFont
*/
class wxGDIObject : public wxObject
{
public:
    /**
        Default constructor.
    */
    wxGDIObject();
};

