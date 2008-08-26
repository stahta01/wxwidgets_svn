///////////////////////////////////////////////////////////////////////////////
// Name:        wx/iconbndl.h
// Purpose:     wxIconBundle
// Author:      Mattia barbon
// Modified by:
// Created:     23.03.02
// RCS-ID:      $Id$
// Copyright:   (c) Mattia Barbon
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ICONBNDL_H_
#define _WX_ICONBNDL_H_

#include "wx/gdiobj.h"
// for wxSize
#include "wx/gdicmn.h"
#include "wx/icon.h"

WX_DECLARE_EXPORTED_OBJARRAY(wxIcon, wxIconArray);

// this class can't load bitmaps of type wxBITMAP_TYPE_ICO_RESOURCE,
// if you need them, you have to load them manually and call
// wxIconCollection::AddIcon
class WXDLLIMPEXP_CORE wxIconBundle : public wxGDIObject
{
public:
    // default constructor
    wxIconBundle();

    // initializes the bundle with the icon(s) found in the file
    wxIconBundle(const wxString& file, wxBitmapType type);

    // initializes the bundle with a single icon
    wxIconBundle(const wxIcon& icon);

    // default copy ctor and assignment operator are OK

    // adds all the icons contained in the file to the collection,
    // if the collection already contains icons with the same
    // width and height, they are replaced
    void AddIcon(const wxString& file, wxBitmapType type);

    // adds the icon to the collection, if the collection already
    // contains an icon with the same width and height, it is
    // replaced
    void AddIcon(const wxIcon& icon);

    // returns the icon with the given size; if no such icon exists,
    // returns the icon with size wxSYS_ICON_[XY]; if no such icon exists,
    // returns the first icon in the bundle
    wxIcon GetIcon(const wxSize& size) const;

    // equivalent to GetIcon(wxSize(size, size))
    wxIcon GetIcon(wxCoord size = wxDefaultCoord) const
        { return GetIcon(wxSize(size, size)); }

    // returns the icon exactly of the specified size or wxNullIcon if no icon
    // of exactly given size are available
    wxIcon GetIconOfExactSize(const wxSize& size) const;
    wxIcon GetIconOfExactSize(wxCoord size) const
        { return GetIconOfExactSize(wxSize(size, size)); }

    // enumerate all icons in the bundle: don't use these functions if ti can
    // be avoided, using GetIcon() directly is better

    // return the number of available icons
    size_t GetIconCount() const;

    // return the icon at index (must be < GetIconCount())
    wxIcon GetIconByIndex(size_t n) const;

    // check if we have any icons at all
    bool IsEmpty() const { return GetIconCount() == 0; }

#if WXWIN_COMPATIBILITY_2_8
    wxDEPRECATED( void AddIcon(const wxString& file, long type)
        {
            AddIcon(file, (wxBitmapType)type);
        }
    )

    wxDEPRECATED( wxIconBundle(const wxString& file, long type)
        {
            AddIcon(file, (wxBitmapType)type);
        }
    )
#endif // WXWIN_COMPATIBILITY_2_8

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

private:
    // delete all icons
    void DeleteIcons();

    DECLARE_DYNAMIC_CLASS(wxIconBundle)
};

#endif // _WX_ICONBNDL_H_
