/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.cpp
// Purpose:     wxCursor class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "cursor.h"
#endif

#include "wx/cursor.h"
#include "wx/icon.h"
#include "wx/mac/private.h"

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxCursor, wxBitmap)
#endif

const short kwxCursorHandId = 9 ;
const short kwxCursorSizeWEId = 10 ;
const short kwxCursorSizeNSId = 11 ;
#if !TARGET_CARBON
Cursor*		MacArrowCursorPtr = &qd.arrow ;
CursHandle	MacArrowCursor = &MacArrowCursorPtr ;
#else
bool		MacArrowInstalled = false ;
Cursor 		MacArrow ;
Cursor*		MacArrowCursorPtr = &MacArrow ;
CursHandle	MacArrowCursor = &MacArrowCursorPtr ;
#endif
CursHandle	gMacCurrentCursor = NULL ;

wxCursorRefData::wxCursorRefData()
{
#if TARGET_CARBON
	if ( !MacArrowInstalled )
	{
		MacArrowCursorPtr = GetQDGlobalsArrow( &MacArrow ) ;
		MacArrowInstalled = true ;
	}
#endif
    m_width = 32; 
    m_height = 32;
    m_hCursor = NULL ;
}

wxCursorRefData::~wxCursorRefData()
{
//	if ( m_hCursor && ( m_hCursor != MacArrowCursor ) )
//		::ReleaseResource( (Handle) m_hCursor ) ;
}

// Cursors
wxCursor::wxCursor()
{
}

wxCursor::wxCursor(const char WXUNUSED(bits)[], int WXUNUSED(width), int WXUNUSED(height),
    int WXUNUSED(hotSpotX), int WXUNUSED(hotSpotY), const char WXUNUSED(maskBits)[])
{
}

wxCursor::wxCursor(const wxString& cursor_file, long flags, int hotSpotX, int hotSpotY)
{
    m_refData = new wxCursorRefData;

    // TODO: create cursor from a file
}

// Cursors by stock number
wxCursor::wxCursor(int cursor_type)
{
  m_refData = new wxCursorRefData;
  
  
  switch (cursor_type)
  {
    case wxCURSOR_WAIT:
      M_CURSORDATA->m_hCursor = ::GetCursor(watchCursor);
      break;
    case wxCURSOR_IBEAM:
      M_CURSORDATA->m_hCursor = ::GetCursor(iBeamCursor);
      break;
    case wxCURSOR_CROSS:
      M_CURSORDATA->m_hCursor = ::GetCursor(crossCursor);
      break;
    case wxCURSOR_SIZENWSE:
      {
        wxStAppResource resload ;
        M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorSizeWEId);
      }
      break;
    case wxCURSOR_SIZENESW:
      {
        wxStAppResource resload ;
        M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorSizeWEId);
      }
      break;
    case wxCURSOR_SIZEWE:
      {
        wxStAppResource resload ;
        M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorSizeWEId);
      }
      break;
    case wxCURSOR_SIZENS:
      {
        wxStAppResource resload ;
        M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorSizeNSId);
      }
      break;
    case wxCURSOR_CHAR:
        {
          M_CURSORDATA->m_hCursor = MacArrowCursor;
          break;
        }
    case wxCURSOR_HAND:
        {
          wxStAppResource resload ;
          M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorHandId);
        }
        break;
    case wxCURSOR_BULLSEYE:
        {
          wxStAppResource resload ;
          M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorHandId);
        }
        break;
    case wxCURSOR_PENCIL:
        {
          wxStAppResource resload ;
          M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorHandId);
        }
        break;
    case wxCURSOR_MAGNIFIER:
        {
          wxStAppResource resload ;
          M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorHandId);
        }
        break;
    case wxCURSOR_NO_ENTRY:
        {
          wxStAppResource resload ;
          M_CURSORDATA->m_hCursor = ::GetCursor(kwxCursorHandId);
        }
        break;
    case wxCURSOR_LEFT_BUTTON:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_RIGHT_BUTTON:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_MIDDLE_BUTTON:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_SIZING:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_WATCH:
    {
      M_CURSORDATA->m_hCursor = ::GetCursor(watchCursor);
      break;
    }
    case wxCURSOR_SPRAYCAN:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_PAINT_BRUSH:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_POINT_LEFT:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_POINT_RIGHT:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_QUESTION_ARROW:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    case wxCURSOR_BLANK:
    {
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
    }
    default:
    case wxCURSOR_ARROW:
      M_CURSORDATA->m_hCursor = MacArrowCursor;
      break;
	}
}

void wxCursor::MacInstall() const 
{
	if ( m_refData && M_CURSORDATA->m_hCursor )
	{
		::SetCursor(  *((CursHandle)M_CURSORDATA->m_hCursor) ) ;
		gMacCurrentCursor = (CursHandle)M_CURSORDATA->m_hCursor ;
	}
	else
	{
		::SetCursor( *MacArrowCursor ) ;
		gMacCurrentCursor = NULL ;
	}
}

wxCursor::~wxCursor()
{
}

// Global cursor setting
void wxSetCursor(const wxCursor& cursor)
{
	cursor.MacInstall() ;
}


