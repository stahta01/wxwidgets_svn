/////////////////////////////////////////////////////////////////////////////
// Name:        xh_bmp.h
// Purpose:     XML resource handler for wxBitmap and wxIcon
// Author:      Vaclav Slavik
// Created:     2000/09/00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_BMP_H_
#define _WX_XH_BMP_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "xh_bmp.h"
#endif

#include "wx/xrc/xmlres.h"


class WXXMLDLLEXPORT wxBitmapXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxBitmapXmlHandler)
public:
    wxBitmapXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

class WXXMLDLLEXPORT wxIconXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxIconXmlHandler)
public:
    wxIconXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


#endif // _WX_XH_BMP_H_
