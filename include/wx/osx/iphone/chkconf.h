/*
 * Name:        wx/osx/iphone/chkconf.h
 * Purpose:     Compiler-specific configuration checking
 * Author:      Stefan Csomor
 * Modified by:
 * Created:     2008-07-30
 * RCS-ID:      $Id: chkconf.h 54833 2008-07-29 21:03:18Z SC $
 * Copyright:   (c) Stefan Csomor
 * Licence:     wxWindows licence
 */

#ifndef _WX_OSX_IPHONE_CHKCONF_H_
#define _WX_OSX_IPHONE_CHKCONF_H_

/* 
 * text rendering system 
 */

/* we have different options and we turn on all that make sense 
 * under a certain platform
 */

#define wxOSX_USE_CORE_TEXT 0
#define wxOSX_USE_ATSU_TEXT 0

/*
 * turning off capabilities that don't work under iphone yet
 */

#if wxUSE_MDI
#undef wxUSE_MDI
#define wxUSE_MDI 0
#endif

#if wxUSE_MDI_ARCHITECTURE
#undef wxUSE_MDI_ARCHITECTURE
#define wxUSE_MDI_ARCHITECTURE 0
#endif

#if wxUSE_DRAG_AND_DROP
#undef wxUSE_DRAG_AND_DROP
#define wxUSE_DRAG_AND_DROP 0
#endif

#if wxUSE_TASKBARICON
#undef wxUSE_TASKBARICON
#define wxUSE_TASKBARICON 0
#endif

#if wxUSE_TOOLTIPS
#undef wxUSE_TOOLTIPS
#define wxUSE_TOOLTIPS 0
#endif

#if wxUSE_DATAVIEWCTRL
#undef wxUSE_DATAVIEWCTRL
#define wxUSE_DATAVIEWCTRL 0
#endif

#if wxUSE_DRAG_AND_DROP
#undef wxUSE_DRAG_AND_DROP
#define wxUSE_DRAG_AND_DROP 0
#endif

#if wxUSE_TASKBARICON
#undef wxUSE_TASKBARICON
#define wxUSE_TASKBARICON 0
#endif

/*
#if wxUSE_POPUPWIN
#undef wxUSE_POPUPWIN
#define wxUSE_POPUPWIN 0
#endif

#if wxUSE_COMBOBOX
#undef wxUSE_COMBOBOX
#define wxUSE_COMBOBOX 0
#endif


#if wxUSE_MENUS
#undef wxUSE_MENUS
#define wxUSE_MENUS 0
#endif

#if wxUSE_CALENDARCTRL
#undef wxUSE_CALENDARCTRL
#define wxUSE_CALENDARCTRL 0
#endif

#if wxUSE_WXHTML_HELP
#undef wxUSE_WXHTML_HELP
#define wxUSE_WXHTML_HELP 0
#endif

#if wxUSE_DOC_VIEW_ARCHITECTURE
#undef wxUSE_DOC_VIEW_ARCHITECTURE
#define wxUSE_DOC_VIEW_ARCHITECTURE 0
#endif

#if wxUSE_PRINTING_ARCHITECTURE
#undef wxUSE_PRINTING_ARCHITECTURE
#define wxUSE_PRINTING_ARCHITECTURE 0
#endif

*/

#if wxUSE_CLIPBOARD
#undef wxUSE_CLIPBOARD
#define wxUSE_CLIPBOARD 0
#endif // wxUSE_CLIPBOARD

#endif
    /* _WX_OSX_IPHONE_CHKCONF_H_ */

