/////////////////////////////////////////////////////////////////////////////
// Name:        icon.cpp
// Purpose:     wxIcon class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "icon.h"
#endif

#include "wx/icon.h"

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxIcon, wxBitmap)
#endif

/*
 * Icons
 */

wxIcon::wxIcon()
{
}

wxIcon::wxIcon(const char bits[], int width, int height) :
    wxBitmap(bits,width,height )
{
    
}

wxIcon::wxIcon( const char **bits ) :
    wxBitmap(bits  )
{
}

wxIcon::wxIcon( char **bits ) :
    wxBitmap(bits )
{
}

wxIcon::wxIcon(const wxString& icon_file, long flags,
    int desiredWidth, int desiredHeight)

{
    LoadFile(icon_file, flags, desiredWidth, desiredHeight);
}

wxIcon::~wxIcon()
{
}

bool wxIcon::LoadFile(const wxString& filename, long type,
    int desiredWidth, int desiredHeight)
{
  UnRef();

  m_refData = new wxBitmapRefData;

  wxBitmapHandler *handler = FindHandler((wxBitmapType)type);

  if ( handler )
	return handler->LoadFile(this, filename, type, desiredWidth, desiredHeight);
  else
	return FALSE;
}

IMPLEMENT_DYNAMIC_CLASS(wxICONResourceHandler, wxBitmapHandler)

bool  wxICONResourceHandler::LoadFile(wxBitmap *bitmap, const wxString& name, long flags,
          int desiredWidth, int desiredHeight)
{
	short theId = -1 ;
    if ( name == "wxICON_INFO" )
    {
        theId = kNoteIcon ;
    }
    else if ( name == "wxICON_QUESTION" )
    {
        theId = kCautionIcon ;
    }
    else if ( name == "wxICON_WARNING" )
    {
         theId = kCautionIcon ;
   }
    else if ( name == "wxICON_ERROR" )
    {
        theId = kStopIcon ;
    }
    else
    {
    	Str255 theName ;
    	OSType theType ;

    #if TARGET_CARBON
    	c2pstrcpy( (StringPtr) theName , name ) ;
    #else
    	strcpy( (char *) theName , name ) ;
    	c2pstr( (char *) theName ) ;
    #endif
    	
    	Handle resHandle = GetNamedResource( 'cicn' , theName ) ;
    	if ( resHandle != 0L )
    	{
    		GetResInfo( resHandle , &theId , &theType , theName ) ;
    		ReleaseResource( resHandle ) ;
    	}
    }
	if ( theId != -1 )
	{
		CIconHandle theIcon = (CIconHandle ) GetCIcon( theId ) ;
		if ( theIcon )
		{
			M_BITMAPHANDLERDATA->m_hIcon = theIcon ;
			M_BITMAPHANDLERDATA->m_width =  32 ;
			M_BITMAPHANDLERDATA->m_height = 32 ;
			
			M_BITMAPHANDLERDATA->m_depth = 8 ;
			M_BITMAPHANDLERDATA->m_ok = true ;
			M_BITMAPHANDLERDATA->m_numColors = 256 ;
			M_BITMAPHANDLERDATA->m_bitmapType = kMacBitmapTypeIcon ;
			return TRUE ;
		}
	}
	return FALSE ;
}
