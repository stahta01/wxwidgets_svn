/////////////////////////////////////////////////////////////////////////////
// Name:        icon.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "icon.h"
#endif

#include "wx/icon.h"

//-----------------------------------------------------------------------------
// wxIcon
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxIcon,wxBitmap)

wxIcon::wxIcon( char **bits, int WXUNUSED(width), int WXUNUSED(height) ) : 
  wxBitmap( bits ) 
{
}
    
wxIcon::wxIcon() :  wxBitmap() 
{  
}

wxIcon::wxIcon(const wxIcon& icon) : wxBitmap()
{ 
  Ref(icon); 
}

wxIcon::wxIcon(const wxIcon* icon) : wxBitmap()
{ 
  if (icon) Ref(*icon); 
}

wxIcon& wxIcon::operator = (const wxIcon& icon) 
{ 
  if (*this == icon) return (*this); 
  Ref(icon); 
  return *this; 
}

