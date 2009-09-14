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
#define wxHAS_OPENGL_ES

#define wxOSX_USE_QUICKTIME 0
#define wxOSX_USE_AUDIOTOOLBOX 1

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

#define wxUSE_BUTTON 1

#if wxUSE_CARET
#undef wxUSE_CARET
#define wxUSE_CARET 0
#endif

#if wxUSE_CHOICE
#undef wxUSE_CHOICE
#define wxUSE_CHOICE 0
#endif

#if wxUSE_COMBOBOX
#undef wxUSE_COMBOBOX
#define wxUSE_COMBOBOX 0
#endif

#ifndef __WXUNIVERSAL__
#undef wxUSE_SCROLLBAR 
#define wxUSE_SCROLLBAR 0
#endif

#undef wxUSE_STATUSBAR
#undef wxUSE_NATIVE_STATUSBAR
#undef wxUSE_TEXTCTRL
#undef wxUSE_ABOUTDLG
#undef wxUSE_STATTEXT
#undef wxUSE_STATLINE
#undef wxUSE_COLLPANE
#undef wxUSE_STATBMP
#undef wxUSE_STATBOX
#undef wxUSE_RADIOBTN
#undef wxUSE_RADIOBOX
#undef wxUSE_TOGGLEBTN


#define wxUSE_STATUSBAR 0
#define wxUSE_NATIVE_STATUSBAR 0
#define wxUSE_TEXTCTRL 0
#define wxUSE_ABOUTDLG 0
#define wxUSE_STATTEXT 0
#define wxUSE_STATLINE 0
#define wxUSE_COLLPANE 0
#define wxUSE_STATBMP 0
#define wxUSE_STATBOX 0
#define wxUSE_RADIOBTN 0
#define wxUSE_RADIOBOX 0
#define wxUSE_TOGGLEBTN 0

#undef wxUSE_TOOLBAR
#undef wxUSE_HTML

#define wxUSE_TOOLBAR 0
#define wxUSE_HTML 0

#undef wxUSE_RICHTEXT
#define wxUSE_RICHTEXT 0

#undef wxUSE_ANIMATIONCTRL
#undef wxUSE_CALENDARCTRL
#undef wxUSE_COMBOCTRL
#undef wxUSE_ODCOMBOBOX
#undef wxUSE_BITMAPCOMBOBOX
#undef wxUSE_BMPBUTTON
#undef wxUSE_CHECKLISTBOX
#undef wxUSE_GAUGE
#undef wxUSE_GRID
#undef wxUSE_LISTBOX
#undef wxUSE_LISTCTRL
#undef wxUSE_NOTEBOOK
#undef wxUSE_SLIDER
#undef wxUSE_SPINBTN
#undef wxUSE_SPINCTRL
#undef wxUSE_TREECTRL
#undef wxUSE_DATEPICKCTRL
#undef wxUSE_DATAVIEWCTRL
#undef wxUSE_EDITABLELISTBOX
#undef wxUSE_FILEPICKERCTRL
#undef wxUSE_DIRPICKERCTRL
#undef wxUSE_FILECTRL
#undef wxUSE_COLOURPICKERCTRL
#undef wxUSE_FONTPICKERCTRL
#undef wxUSE_DEBUGREPORT
#undef wxUSE_HYPERLINKCTRL
#undef wxUSE_STC
#undef wxUSE_AUI
#undef wxUSE_BUSYINFO
#undef wxUSE_SEARCHCTRL

#define wxUSE_ANIMATIONCTRL 0
#define wxUSE_CALENDARCTRL 0
#define wxUSE_COMBOCTRL 0
#define wxUSE_ODCOMBOBOX 0
#define wxUSE_BITMAPCOMBOBOX 0
#define wxUSE_BMPBUTTON 0
#define wxUSE_CHECKLISTBOX 0
#define wxUSE_GAUGE 0
#define wxUSE_GRID 0
#define wxUSE_LISTBOX 0
#define wxUSE_LISTCTRL 0
#define wxUSE_NOTEBOOK 0
#define wxUSE_SLIDER 0
#define wxUSE_SPINBTN 0
#define wxUSE_SPINCTRL 0
#define wxUSE_TREECTRL 0
#define wxUSE_DATEPICKCTRL 0
#define wxUSE_DATAVIEWCTRL 0
#define wxUSE_EDITABLELISTBOX 0
#define wxUSE_FILEPICKERCTRL 0
#define wxUSE_DIRPICKERCTRL 0
#define wxUSE_FILECTRL 0
#define wxUSE_COLOURPICKERCTRL 0
#define wxUSE_FONTPICKERCTRL 0
#define wxUSE_DEBUGREPORT 0
#define wxUSE_HYPERLINKCTRL 0
#define wxUSE_STC 0
#define wxUSE_AUI 0
#define wxUSE_BUSYINFO 0
#define wxUSE_SEARCHCTRL 0

#undef wxUSE_LOGWINDOW
#undef wxUSE_LOG_DIALOG
#undef wxUSE_LISTBOOK
#undef wxUSE_CHOICEBOOK
#undef wxUSE_TREEBOOK
#undef wxUSE_TOOLBOOK
#undef wxUSE_CHOICEDLG
#undef wxUSE_HELP
#undef wxUSE_PROGRESSDLG
#undef  wxUSE_FONTDLG
#undef wxUSE_FILEDLG
#undef wxUSE_CHOICEDLG
#undef wxUSE_NUMBERDLG
#undef wxUSE_TEXTDLG
#undef wxUSE_DIRDLG
#undef wxUSE_STARTUP_TIPS
#undef wxUSE_WIZARDDLG
#undef wxUSE_TOOLBAR_NATIVE
#undef wxUSE_FINDREPLDLG
#undef wxUSE_TASKBARICON
#undef wxUSE_REARRANGECTRL

#define wxUSE_LOGWINDOW 0
#define wxUSE_LOG_DIALOG 0
#define wxUSE_LISTBOOK 0
#define wxUSE_CHOICEBOOK 0
#define wxUSE_TREEBOOK 0
#define wxUSE_TOOLBOOK 0
#define wxUSE_CHOICEDLG 0
#define wxUSE_HELP 0
#define wxUSE_PROGRESSDLG 0
#define  wxUSE_FONTDLG 0
#define wxUSE_FILEDLG 0
#define wxUSE_CHOICEDLG 0
#define wxUSE_NUMBERDLG 0
#define wxUSE_TEXTDLG 0
#define wxUSE_DIRDLG 0
#define wxUSE_STARTUP_TIPS 0
#define wxUSE_WIZARDDLG 0
#define wxUSE_TOOLBAR_NATIVE 0
#define wxUSE_FINDREPLDLG 0
#define wxUSE_TASKBARICON 0
#define wxUSE_REARRANGECTRL 0

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

#if wxUSE_MENUS
#undef wxUSE_MENUS
#define wxUSE_MENUS 0
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



#if wxUSE_CALENDARCTRL
#undef wxUSE_CALENDARCTRL
#define wxUSE_CALENDARCTRL 0
#endif

*/

#if wxUSE_CLIPBOARD
#undef wxUSE_CLIPBOARD
#define wxUSE_CLIPBOARD 0
#endif // wxUSE_CLIPBOARD

/*
#if wxUSE_GLCANVAS
#undef wxUSE_GLCANVAS
#define wxUSE_GLCANVAS 0
#endif // wxUSE_GLCANVAS
*/

#if wxUSE_COLOURDLG
#undef wxUSE_COLOURDLG
#define wxUSE_COLOURDLG 0
#endif // wxUSE_COLOURDLG

#endif
    /* _WX_OSX_IPHONE_CHKCONF_H_ */

