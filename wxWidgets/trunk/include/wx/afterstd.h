///////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/afterstd.h
// Purpose:     #include after STL headers
// Author:      Vadim Zeitlin
// Modified by:
// Created:     07/07/03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    See the comments in beforestd.h.
 */

#if defined(__WXMSW__) && defined(__MINGW32__)
    #include "wx/msw/winundef.h"
#endif
#ifdef _MSC_VER
    // MSVC 5 does not have this
    #if _MSC_VER > 1100
        #pragma warning(pop)
    #else
        // 'expression' : signed/unsigned mismatch
        #pragma warning(default:4018)

        // 'conversion' : conversion from 'type1' to 'type2',
        // possible loss of data
        #pragma warning(default:4244)

        // C++ language change: to explicitly specialize class template
        // 'identifier' use the following syntax
        #pragma warning(default:4663)
    #endif
#endif

