/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/icon.h
// Purpose:     wxIcon class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ICON_H_
#define _WX_ICON_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "icon.h"
#endif

#include "wx/bitmap.h"

// Icon
class WXDLLEXPORT wxIcon: public wxBitmap
{
  DECLARE_DYNAMIC_CLASS(wxIcon)

public:
  wxIcon();

  // Copy constructors
  wxIcon(const wxIcon& icon)
      : wxBitmap()
      { Ref(icon); }

  wxIcon(const char **data);
  wxIcon(char **data);
  wxIcon(const char bits[], int width , int height );
  wxIcon(const wxString& name, int flags = wxBITMAP_TYPE_ICON_RESOURCE,
    int desiredWidth = -1, int desiredHeight = -1);
  wxIcon(const wxIconLocation& loc)
  {
      LoadFile(loc.GetFileName(), wxBITMAP_TYPE_ICON);
  }
  ~wxIcon();

  bool LoadFile(const wxString& name, wxBitmapType flags /* = wxBITMAP_TYPE_ICON_RESOURCE */ ,
      int desiredWidth /* = -1 */ , int desiredHeight = -1);
  bool LoadFile(const wxString& name ,wxBitmapType flags = wxBITMAP_TYPE_ICON_RESOURCE )
  	{ return LoadFile( name , flags , -1 , -1 ) ; }

  wxIcon& operator=(const wxIcon& icon)
    { if (this != &icon) Ref(icon); return *this; }
  bool operator==(const wxIcon& icon) const { return m_refData == icon.m_refData; }
  bool operator!=(const wxIcon& icon) const { return !(*this == icon); }

  // create from bitmap (which should have a mask unless it's monochrome):
  // there shouldn't be any implicit bitmap -> icon conversion (i.e. no
  // ctors, assignment operators...), but it's ok to have such function
  void CopyFromBitmap(const wxBitmap& bmp);
};

#endif
    // _WX_ICON_H_
