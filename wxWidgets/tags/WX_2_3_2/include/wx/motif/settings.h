/////////////////////////////////////////////////////////////////////////////
// Name:        settings.h
// Purpose:     wxSystemSettings class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SETTINGS_H_
#define _WX_SETTINGS_H_

#ifdef __GNUG__
#pragma interface "settings.h"
#endif

#include "wx/setup.h"
#include "wx/defs.h"
#include "wx/object.h"
#include "wx/colour.h"
#include "wx/font.h"


class WXDLLEXPORT wxSystemSettings: public wxObject
{
public:
    inline wxSystemSettings() {}

    // Get a system colour
    static wxColour    GetSystemColour(int index);

    // Get a system font
    static wxFont      GetSystemFont(int index);

    // Get a system metric, e.g. scrollbar size
    static int         GetSystemMetric(int index);

    // Return true if the port has certain feature
    static bool        GetCapability(int index);

};

#endif
    // _WX_SETTINGS_H_
