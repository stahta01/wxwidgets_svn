/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/accel.h
// Purpose:     wxAcceleratorTable class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id$
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ACCEL_H_
#define _WX_ACCEL_H_

// ----------------------------------------------------------------------------
// the accel table has all accelerators for a given window or menu
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxAcceleratorTable : public wxObject
{
public:
    // default ctor
    wxAcceleratorTable();

    // load from .rc resource (Windows specific)
    wxAcceleratorTable(const wxString& resource);

    // initialize from array
    wxAcceleratorTable(int n, const wxAcceleratorEntry entries[]);

    virtual ~wxAcceleratorTable();

#if WXWIN_COMPATIBILITY_2_4
    bool operator==(const wxAcceleratorTable& accel) const
        { return m_refData == accel.m_refData; }
    bool operator!=(const wxAcceleratorTable& accel) const
        { return !(*this == accel); }
#endif

    bool Ok() const { return IsOk(); }
    bool IsOk() const;
    void SetHACCEL(WXHACCEL hAccel);
    WXHACCEL GetHACCEL() const;

    // translate the accelerator, return true if done
    bool Translate(wxWindow *window, WXMSG *msg) const;

private:
    DECLARE_DYNAMIC_CLASS(wxAcceleratorTable)
};

#endif
    // _WX_ACCEL_H_
