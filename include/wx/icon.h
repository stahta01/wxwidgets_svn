#ifndef _WX_ICON_H_BASE_
#define _WX_ICON_H_BASE_

#if defined(__WXMSW__)
#include "wx/msw/icon.h"
#elif defined(__WXMOTIF__)
#include "wx/motif/icon.h"
#elif defined(__WXGTK__)
#include "wx/gtk/icon.h"
#elif defined(__WXQT__)
#include "wx/qt/icon.h"
#elif defined(__WXMAC__)
#include "wx/mac/icon.h"
#elif defined(__WXSTUBS__)
#include "wx/stubs/icon.h"
#endif

#endif
    // _WX_ICON_H_BASE_
