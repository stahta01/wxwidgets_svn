/////////////////////////////////////////////////////////////////////////////
// Name:        accel.h
// Purpose:     wxAcceleratorTable class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ACCEL_H_
#define _WX_ACCEL_H_

#ifdef __GNUG__
#pragma interface "accel.h"
#endif

#include "wx/string.h"

class WXDLLEXPORT wxAcceleratorTable: public wxObject
{
DECLARE_DYNAMIC_CLASS(wxAcceleratorTable)
public:
    wxAcceleratorTable();
    wxAcceleratorTable(const wxString& resource); // Load from .rc resource
    wxAcceleratorTable(int n, wxAcceleratorEntry entries[]); // Load from array

    // Copy constructors
    inline wxAcceleratorTable(const wxAcceleratorTable& accel) { Ref(accel); }
    inline wxAcceleratorTable(const wxAcceleratorTable* accel) { if (accel) Ref(*accel); }

    ~wxAcceleratorTable();

    inline wxAcceleratorTable& operator = (const wxAcceleratorTable& accel) { if (*this == accel) return (*this); Ref(accel); return *this; }
    inline bool operator == (const wxAcceleratorTable& accel) { return m_refData == accel.m_refData; }
    inline bool operator != (const wxAcceleratorTable& accel) { return m_refData != accel.m_refData; }

    bool Ok() const;
};

WXDLLEXPORT_DATA(extern wxAcceleratorTable) wxNullAcceleratorTable;

#endif
    // _WX_ACCEL_H_
