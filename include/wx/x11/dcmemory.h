/////////////////////////////////////////////////////////////////////////////
// Name:        dcmemory.h
// Purpose:     wxMemoryDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCMEMORY_H_
#define _WX_DCMEMORY_H_

#include "wx/dcclient.h"

class WXDLLIMPEXP_CORE wxMemoryDC : public wxWindowDC
{
public:
    wxMemoryDC();
    wxMemoryDC( wxDC *dc ); // Create compatible DC
    ~wxMemoryDC();
    virtual void SelectObject( const wxBitmap& bitmap );

    // implementation
    wxBitmap  m_selected;

protected:
    void DoGetSize( int *width, int *height ) const;

    DECLARE_DYNAMIC_CLASS(wxMemoryDC)
};

#endif
// _WX_DCMEMORY_H_
