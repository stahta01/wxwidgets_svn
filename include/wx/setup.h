
#ifndef _WX_SETUP_H_BASE_
#define _WX_SETUP_H_BASE_

// compatibility code, to be removed asap:

#if !defined(__WXMSW__) && !defined(__WXGTK__) && !defined(__WXMOTIF__) && !defined(__WXQT__) && !defined(__WXSTUBS__) && !defined(__WXMAC__)
#    error No __WXxxx__ define set! Please define one of __WXGTK__,__WXMSW__,__WXMOTIF__,__WXMAC__,__WXQT__,__WXSTUBS__
#endif

#if defined(__WXMSW__)
#include "wx/msw/setup.h"
#elif defined(__WXMAC__)
#include "wx/mac/setup.h"
#elif defined(__WXQT__)
#include "wx/qt/setup.h"
#elif defined(__WXMOTIF__)
#include "wx/motif/setup.h"
#elif defined(__WXSTUBS__)
#include "wx/stubs/setup.h"
#elif defined(__WXGTK__)
#include "wx/../../install/unix/setup/setup.h"
#endif

#endif
	// _WX_SETUP_H_BASE_
