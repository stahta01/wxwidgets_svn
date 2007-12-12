/**
* Name:        wx/math.h
* Purpose:     Declarations/definitions of common math functions
* Author:      John Labenski and others
* Modified by:
* Created:     02/02/03
* RCS-ID:      $Id$
* Copyright:   (c) John Labenski
* Licence:     wxWindows licence
*/

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_MATH_H_
#define _WX_MATH_H_

#include "wx/defs.h"

#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795
#endif

/* Scaling factors for various unit conversions: 1 inch = 2.54 cm */
#ifndef METRIC_CONVERSION_CONSTANT
    #define METRIC_CONVERSION_CONSTANT (1./254)
#endif

#ifndef mm2inches
    #define mm2inches (METRIC_CONVERSION_CONSTANT)
#endif

#ifndef inches2mm
    #define inches2mm (1/(mm2inches))
#endif

#ifndef mm2twips
    #define mm2twips (METRIC_CONVERSION_CONSTANT*1440)
#endif

#ifndef twips2mm
    #define twips2mm (1/(mm2twips))
#endif

#ifndef mm2pt
    #define mm2pt (METRIC_CONVERSION_CONSTANT*72)
#endif

#ifndef pt2mm
    #define pt2mm (1/(mm2pt))
#endif


/* unknown __VISAGECC__, __SYMANTECCC__ */

#if defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
    #include <float.h>
    #define wxFinite(x) _finite(x)
#elif defined(__GNUG__)||defined(__GNUWIN32__)||defined(__DJGPP__)|| \
      defined(__SGI_CC__)||defined(__SUNCC__)||defined(__XLC__)|| \
      defined(__HPUX__)||defined(__MWERKS__)
    #define wxFinite(x) finite(x)
#else
    #define wxFinite(x) ((x) == (x))
#endif


#if defined(__VISUALC__)||defined(__BORLAND__)
    #define wxIsNaN(x) _isnan(x)
#elif defined(__GNUG__)||defined(__GNUWIN32__)||defined(__DJGPP__)|| \
      defined(__SGI_CC__)||defined(__SUNCC__)||defined(__XLC__)|| \
      defined(__HPUX__)||defined(__MWERKS__)
    #define wxIsNaN(x) isnan(x)
#else
    #define wxIsNaN(x) ((x) != (x))
#endif

#ifdef __cplusplus

    #ifdef __INTELC__

        inline bool wxIsSameDouble(double x, double y)
        {
            // VZ: this warning, given for operators==() and !=() is not wrong, as ==
            //     shouldn't be used with doubles, but we get too many of them and
            //     removing these operators is probably not a good idea
            //
            //     Maybe we should alway compare doubles up to some "epsilon" precision
            #pragma warning(push)

            // floating-point equality and inequality comparisons are unreliable
            #pragma warning(disable: 1572)

            return x == y;

            #pragma warning(pop)
        }

    #else /* !__INTELC__ */

        inline bool wxIsSameDouble(double x, double y) { return x == y; }

    #endif /* __INTELC__/!__INTELC__ */

    inline bool wxIsNullDouble(double x) { return wxIsSameDouble(x, 0.); }

    inline int wxRound(double x)
    {
        wxASSERT_MSG( x > INT_MIN - 0.5 && x < INT_MAX + 0.5,
                      _T("argument out of supported range") );

        #if defined(HAVE_ROUND)
            return int(round(x));
        #else
            return (int)(x < 0 ? x - 0.5 : x + 0.5);
        #endif
    }
#endif /* __cplusplus */


#if defined(__WXMSW__) && !defined(__WXWINCE__)
    #define wxMulDivInt32( a , b , c ) ::MulDiv( a , b , c )
#elif defined( __WXMAC__ )
    #define wxMulDivInt32( a , b , c ) ( (wxInt32) ( ( (wxInt64)(a) * (wxInt64)(b) ) / (wxInt64)(c) ) )
#else
    #define wxMulDivInt32( a , b , c ) ((wxInt32)((a)*(((wxDouble)b)/((wxDouble)c))))
#endif

#if wxUSE_APPLE_IEEE
#ifdef __cplusplus
    extern "C" {
#endif
    /* functions from common/extended.c */
    WXDLLIMPEXP_BASE wxFloat64 wxConvertFromIeeeExtended(const wxInt8 *bytes);
    WXDLLIMPEXP_BASE void wxConvertToIeeeExtended(wxFloat64 num, wxInt8 *bytes);

    /* use wxConvertFromIeeeExtended() and wxConvertToIeeeExtended() instead */
#if WXWIN_COMPATIBILITY_2_8
    wxDEPRECATED( WXDLLIMPEXP_BASE wxFloat64 ConvertFromIeeeExtended(const wxInt8 *bytes) );
    wxDEPRECATED( WXDLLIMPEXP_BASE void ConvertToIeeeExtended(wxFloat64 num, wxInt8 *bytes) );
#endif // WXWIN_COMPATIBILITY_2_8

#ifdef __cplusplus
    }
#endif
#endif /* wxUSE_APPLE_IEEE */


#endif /* _WX_MATH_H_ */
