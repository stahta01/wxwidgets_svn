#ifndef __FONTDLGH_BASE__
#define __FONTDLGH_BASE__

#if defined(__WXMSW__)
#include "wx/msw/fontdlg.h"
#elif defined(__WXMOTIF__)
#include "wx/generic/fontdlgg.h"
#elif defined(__WXGTK__)
#include "wx/generic/fontdlgg.h"
#endif

#endif
    // __FONTDLGH_BASE__
