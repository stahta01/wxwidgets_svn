/////////////////////////////////////////////////////////////////////////////
// Name:        settings.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKSETTINGSH__
#define __GTKSETTINGSH__

#ifdef __GNUG__
#pragma interface "settings.h"
#endif

#include "wx/defs.h"
#include "wx/gdicmn.h"
#include "wx/pen.h"
#include "wx/font.h"

// FIXME_MGL - probably belongs to wxUniversal

class wxSystemSettings: public wxObject
{
public:
    inline wxSystemSettings() {}

    inline static void Init() {}
    static void Done() {}

    // Get a system colour
    static wxColour    GetSystemColour(int index) {}

    // Get a system font
    static wxFont      GetSystemFont(int index) {}

    // Get a system metric, e.g. scrollbar size
    static int         GetSystemMetric(int index) {}
};

#endif
    // __GTKSETTINGSH__
