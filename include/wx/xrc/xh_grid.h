/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xh_grid.h
// Purpose:     XML resource handler for the grid control
// Author:      Agron Selimaj
// Created:     2005/08/11
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Agron Selimaj, Freepour Controls Inc.
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_GRD_H_
#define _WX_XH_GRD_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "xh_grid.cpp"
#endif

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_GRID

class WXDLLIMPEXP_XRC wxGridXmlHandler : public wxXmlResourceHandler
{
public:
    wxGridXmlHandler();

    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    DECLARE_DYNAMIC_CLASS(wxGridXmlHandler)
};

#endif // wxUSE_XRC && wxUSE_GRID

#endif // _WX_XH_GRD_H_
