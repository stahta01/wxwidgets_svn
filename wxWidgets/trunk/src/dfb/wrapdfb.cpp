/////////////////////////////////////////////////////////////////////////////
// Name:        src/dfb/wrapdfb.cpp
// Purpose:     wx wrappers for DirectFB interfaces
// Author:      Vaclav Slavik
// Created:     2006-09-04
// RCS-ID:      $Id$
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/dfb/wrapdfb.h"

//-----------------------------------------------------------------------------
// wxDfbCheckReturn
//-----------------------------------------------------------------------------

bool wxDfbCheckReturn(DFBResult code)
{
    switch ( code )
    {
        case DFB_OK:
            return true;

        // these are programming errors, assert:
        #define DFB_ASSERT(code)                                        \
            case code:                                                  \
                wxFAIL_MSG( _T("DirectFB error: ") _T(#code) );         \
                return false                                            \

        DFB_ASSERT(DFB_DEAD);
        DFB_ASSERT(DFB_UNSUPPORTED);
        DFB_ASSERT(DFB_UNIMPLEMENTED);
        DFB_ASSERT(DFB_INVARG);
        DFB_ASSERT(DFB_NOIMPL);
        DFB_ASSERT(DFB_MISSINGFONT);
        DFB_ASSERT(DFB_THIZNULL);
        DFB_ASSERT(DFB_INVAREA);
        DFB_ASSERT(DFB_DESTROYED);
        DFB_ASSERT(DFB_NOSUCHMETHOD);
        DFB_ASSERT(DFB_NOSUCHINSTANCE);
        DFB_ASSERT(DFB_VERSIONMISMATCH);

        #undef  DFB_ASSERT

        // these are not errors, but valid return codes:
        case DFB_INTERRUPTED:
        case DFB_BUFFEREMPTY:
            return true;

        default:
            // FIXME: should handle the errors individually
            wxLogError(_("DirectFB error %d occured."), (int)code);
            return false;
    }
}

//-----------------------------------------------------------------------------
// wxDfbPtrBase
//-----------------------------------------------------------------------------

/* static */
void wxDfbPtrBase::DoAddRef(wxDfbWrapperBase *ptr)
{
    ptr->AddRef();
}

void wxDfbPtrBase::DoRelease(wxDfbWrapperBase *ptr)
{
    ptr->Release();
}

//-----------------------------------------------------------------------------
// wxIDirectFB
//-----------------------------------------------------------------------------

wxIDirectFBPtr wxIDirectFB::ms_ptr;

/* static */
void wxIDirectFB::CreateDirectFB()
{
    IDirectFB *dfb;
    if ( wxDfbCheckReturn(DirectFBCreate(&dfb)) )
        ms_ptr = new wxIDirectFB(dfb);
}

/* static */
void wxIDirectFB::CleanUp()
{
    ms_ptr.Reset();
}
