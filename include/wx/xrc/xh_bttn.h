/////////////////////////////////////////////////////////////////////////////
// Name:        xh_bttn.h
// Purpose:     XML resource handler for buttons
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_BTTN_H_
#define _WX_XH_BTTN_H_

#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_XRC wxButtonXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxButtonXmlHandler)
public:
    wxButtonXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


#endif // _WX_XH_BTTN_H_
