#ifndef _WX_HELP_H_BASE_
#define _WX_HELP_H_BASE_

#ifdef __WXMSW__
#include "wx/msw/helpwin.h"
#elif defined(__WXGTK__)
#include "wx/generic/helphtml.h"
#else
#include "wx/generic/helpxlp.h"
#endif

#ifdef __WXMSW__
#define wxHelpController wxWinHelpController
#define sm_classwxHelpController sm_classwxWinHelpController
#elif defined(__WXGTK__)
#define wxHelpController wxHTMLHelpController
#define sm_classwxHelpController sm_classwxHTMLHelpController
#else
#define wxHelpController wxXLPHelpController
#define sm_classwxHelpController sm_classwxXLPHelpController
#endif

#endif
    // _WX_HELP_H_BASE_
