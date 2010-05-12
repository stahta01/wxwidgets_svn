/////////////////////////////////////////////////////////////////////////////
// Name:        wx/docmdi.h
// Purpose:     Frame classes for MDI document/view applications
// Author:      Julian Smart
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Julian Smart
//              (c) 2010 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DOCMDI_H_
#define _WX_DOCMDI_H_

#include "wx/defs.h"

#if wxUSE_MDI_ARCHITECTURE

#include "wx/docview.h"
#include "wx/mdi.h"

#ifdef __VISUALC6__
    // "non dll-interface class 'wxDocXXXFrameAny<>' used as base interface for
    // dll-interface class 'wxDocMDIXXXFrame'" -- this is bogus as the template
    // will be DLL-exported but only once it is used as base class here!
    #pragma warning (push)
    #pragma warning (disable:4275)
#endif

// Define MDI versions of the doc-view frame classes. Note that we need to
// define them as classes for wxRTTI, otherwise we could simply define them as
// typedefs.

// ----------------------------------------------------------------------------
// An MDI document parent frame
// ----------------------------------------------------------------------------

typedef
  wxDocParentFrameAny<wxMDIParentFrame> wxDocMDIParentFrameBase;

class WXDLLIMPEXP_CORE wxDocMDIParentFrame : public wxDocMDIParentFrameBase
{
public:
    wxDocMDIParentFrame() : wxDocMDIParentFrameBase() { }

    wxDocMDIParentFrame(wxDocManager *manager,
                        wxFrame *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE,
                        const wxString& name = wxFrameNameStr)
        : wxDocMDIParentFrameBase(manager,
                                  parent, id, title, pos, size, style, name)
    {
    }

private:
    DECLARE_CLASS(wxDocMDIParentFrame)
    wxDECLARE_NO_COPY_CLASS(wxDocMDIParentFrame);
};

// ----------------------------------------------------------------------------
// An MDI document child frame
// ----------------------------------------------------------------------------

typedef
  wxDocChildFrameAny<wxMDIChildFrame, wxMDIParentFrame> wxDocMDIChildFrameBase;

class WXDLLIMPEXP_CORE wxDocMDIChildFrame : public wxDocMDIChildFrameBase
{
public:
    wxDocMDIChildFrame() { }

    wxDocMDIChildFrame(wxDocument *doc,
                       wxView *view,
                       wxMDIParentFrame *parent,
                       wxWindowID id,
                       const wxString& title,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxDEFAULT_FRAME_STYLE,
                       const wxString& name = wxFrameNameStr)
        : wxDocMDIChildFrameBase(doc, view,
                                 parent, id, title, pos, size, style, name)
    {
    }

private:
    DECLARE_CLASS(wxDocMDIChildFrame)
    wxDECLARE_NO_COPY_CLASS(wxDocMDIChildFrame);
};

#ifdef __VISUALC6__
    #pragma warning (pop)
#endif

#endif // wxUSE_MDI_ARCHITECTURE

#endif // _WX_DOCMDI_H_
