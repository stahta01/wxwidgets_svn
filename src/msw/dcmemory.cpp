/////////////////////////////////////////////////////////////////////////////
// Name:        dcmemory.cpp
// Purpose:     wxMemoryDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#pragma implementation "dcmemory.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#endif

#include "wx/dcmemory.h"

#include <windows.h>

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxMemoryDC, wxDC)
#endif

/*
 * Memory DC
 *
 */

wxMemoryDC::wxMemoryDC(void)
{
  m_hDC = (WXHDC) ::CreateCompatibleDC(NULL);
  m_ok = (m_hDC != 0);
  m_bOwnsDC = TRUE;

  SetBrush(*wxWHITE_BRUSH);
  SetPen(*wxBLACK_PEN);
}

wxMemoryDC::wxMemoryDC(wxDC *old_dc)
{
  old_dc->BeginDrawing();

  m_hDC = (WXHDC) ::CreateCompatibleDC((HDC) old_dc->GetHDC());
  m_ok = (m_hDC != 0);

  old_dc->EndDrawing();

  SetBrush(*wxWHITE_BRUSH);
  SetPen(*wxBLACK_PEN);
}

wxMemoryDC::~wxMemoryDC(void)
{
}

void wxMemoryDC::SelectObject(const wxBitmap& bitmap)
{
  // Select old bitmap out of the device context
  if (m_oldBitmap)
  {
    ::SelectObject((HDC) m_hDC, (HBITMAP) m_oldBitmap);
    if (m_selectedBitmap.Ok())
    {
      m_selectedBitmap.SetSelectedInto(NULL);
      m_selectedBitmap = wxNullBitmap;
    }
  }

  // Do own check for whether the bitmap is already selected into
  // a device context
  if (bitmap.GetSelectedInto() && (bitmap.GetSelectedInto() != this))
  {
    wxFatalError("Error in wxMemoryDC::SelectObject\nBitmap is selected in another wxMemoryDC.\nDelete the first wxMemoryDC or use SelectObject(NULL)");
    return;
  }
  
  m_selectedBitmap = bitmap;

  if (!m_selectedBitmap.Ok())
    return;

  m_selectedBitmap.SetSelectedInto(this);
#if WXDEBUG > 1
  wxDebugMsg("wxMemoryDC::SelectObject: Selecting HBITMAP %X\n", m_selectedBitmap.GetHBITMAP());
#endif
  HBITMAP bm = (HBITMAP) ::SelectObject((HDC) m_hDC, (HBITMAP) m_selectedBitmap.GetHBITMAP());

  if (bm == ERROR)
  {
    wxFatalError("Error in wxMemoryDC::SelectObject\nBitmap may not be loaded, or may be selected in another wxMemoryDC.\nDelete the first wxMemoryDC to deselect bitmap.");
  }
  else if (!m_oldBitmap)
    m_oldBitmap = (WXHBITMAP) bm;
}

void wxMemoryDC::GetSize(int *width, int *height) const
{
  if (!m_selectedBitmap.Ok())
  {
    *width = 0; *height = 0;
    return;
  }
  *width = m_selectedBitmap.GetWidth();
  *height = m_selectedBitmap.GetHeight();
}

