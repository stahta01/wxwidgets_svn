#ifndef _WX_COLOUR_H_BASE_
#define _WX_COLOUR_H_BASE_

#if defined(__WXMSW__)
#include "wx/msw/colour.h"
#elif defined(__WXMOTIF__)
#include "wx/motif/colour.h"
#elif defined(__WXGTK__)
#include "wx/gtk/colour.h"
#elif defined(__WXQT__)
#include "wx/qt/colour.h"
#elif defined(__WXMAC__)
#include "wx/mac/colour.h"
#elif defined(__WXSTUBS__)
#include "wx/stubs/colour.h"
#endif

#endif
    // _WX_COLOUR_H_BASE_
