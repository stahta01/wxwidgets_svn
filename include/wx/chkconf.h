/*
 * Name:        wx/chkconf.h
 * Purpose:     check the config settings for consistency
 * Author:      Vadim Zeitlin
 * Modified by:
 * Created:     09.08.00
 * RCS-ID:      $Id$
 * Copyright:   (c) 2000 Vadim Zeitlin <vadim@wxwindows.org>
 * Licence:     wxWindows license
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

/*
   this global setting determintes what should we do if the setting FOO
   requires BAR and BAR is not set: we can either silently define BAR
   (default, recommended) or give an error and abort (mainly useful for
   developpers only)
 */
#define wxABORT_ON_CONFIG_ERROR

/*
   all these tests are for GUI onlu
 */
#if wxUSE_GUI

/*
   all of the settings tested below must be defined or we'd get an error from
   preprocessor about invalid integer expression
 */

#ifndef wxUSE_ACCEL
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_ACCEL must be defined."
#   else
#       define wxUSE_ACCEL 0
#   endif
#endif /* !defined(wxUSE_ACCEL) */

#ifndef wxUSE_BMPBUTTON
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_BMPBUTTON must be defined."
#   else
#       define wxUSE_BMPBUTTON 0
#   endif
#endif /* !defined(wxUSE_BMPBUTTON) */

#ifndef wxUSE_BUTTON
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_BUTTON must be defined."
#   else
#       define wxUSE_BUTTON 0
#   endif
#endif /* !defined(wxUSE_BUTTON) */

#ifndef wxUSE_CALENDARCTRL
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_CALENDARCTRL must be defined."
#   else
#       define wxUSE_CALENDARCTRL 0
#   endif
#endif /* !defined(wxUSE_CALENDARCTRL) */

#ifndef wxUSE_CARET
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_CARET must be defined."
#   else
#       define wxUSE_CARET 0
#   endif
#endif /* !defined(wxUSE_CARET) */

#ifndef wxUSE_CHECKBOX
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_CHECKBOX must be defined."
#   else
#       define wxUSE_CHECKBOX 0
#   endif
#endif /* !defined(wxUSE_CHECKBOX) */

#ifndef wxUSE_CHECKLISTBOX
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_CHECKLISTBOX must be defined."
#   else
#       define wxUSE_CHECKLISTBOX 0
#   endif
#endif /* !defined(wxUSE_CHECKLISTBOX) */

#ifndef wxUSE_CHOICE
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_CHOICE must be defined."
#   else
#       define wxUSE_CHOICE 0
#   endif
#endif /* !defined(wxUSE_CHOICE) */

#ifndef wxUSE_CLIPBOARD
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_CLIPBOARD must be defined."
#   else
#       define wxUSE_CLIPBOARD 0
#   endif
#endif /* !defined(wxUSE_CLIPBOARD) */

#ifndef wxUSE_COMBOBOX
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_COMBOBOX must be defined."
#   else
#       define wxUSE_COMBOBOX 0
#   endif
#endif /* !defined(wxUSE_COMBOBOX) */

#ifndef wxUSE_DATAOBJ
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_DATAOBJ must be defined."
#   else
#       define wxUSE_DATAOBJ 0
#   endif
#endif /* !defined(wxUSE_DATAOBJ) */

#ifndef wxUSE_FILEDLG
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_FILEDLG must be defined."
#   else
#       define wxUSE_FILEDLG 0
#   endif
#endif /* !defined(wxUSE_FILEDLG) */

#ifndef wxUSE_FONTDLG
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_FONTDLG must be defined."
#   else
#       define wxUSE_FONTDLG 0
#   endif
#endif /* !defined(wxUSE_FONTDLG) */

#ifndef wxUSE_FONTMAP
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_FONTMAP must be defined."
#   else
#       define wxUSE_FONTMAP 0
#   endif
#endif /* !defined(wxUSE_FONTMAP) */

#ifndef wxUSE_GAUGE
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_GAUGE must be defined."
#   else
#       define wxUSE_GAUGE 0
#   endif
#endif /* !defined(wxUSE_GAUGE) */

#ifndef wxUSE_GRID
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_GRID must be defined."
#   else
#       define wxUSE_GRID 0
#   endif
#endif /* !defined(wxUSE_GRID) */

#ifndef wxUSE_IMAGLIST
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_IMAGLIST must be defined."
#   else
#       define wxUSE_IMAGLIST 0
#   endif
#endif /* !defined(wxUSE_IMAGLIST) */

#ifndef wxUSE_LISTBOX
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_LISTBOX must be defined."
#   else
#       define wxUSE_LISTBOX 0
#   endif
#endif /* !defined(wxUSE_LISTBOX) */

#ifndef wxUSE_LISTCTRL
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_LISTCTRL must be defined."
#   else
#       define wxUSE_LISTCTRL 0
#   endif
#endif /* !defined(wxUSE_LISTCTRL) */

#ifndef wxUSE_MSGDLG
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_MSGDLG must be defined."
#   else
#       define wxUSE_MSGDLG 0
#   endif
#endif /* !defined(wxUSE_MSGDLG) */

#ifndef wxUSE_MDI_ARCHITECTURE
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_MDI_ARCHITECTURE must be defined."
#   else
#       define wxUSE_MDI_ARCHITECTURE 0
#   endif
#endif /* !defined(wxUSE_MDI_ARCHITECTURE) */

#ifndef wxUSE_MENUS
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_MENUS must be defined."
#   else
#       define wxUSE_MENUS 0
#   endif
#endif /* !defined(wxUSE_MENUS) */

#ifndef wxUSE_MIMETYPE
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_MIMETYPE must be defined."
#   else
#       define wxUSE_MIMETYPE 0
#   endif
#endif /* !defined(wxUSE_MIMETYPE) */

#ifndef wxUSE_NEW_GRID
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_NEW_GRID must be defined."
#   else
#       define wxUSE_NEW_GRID 0
#   endif
#endif /* !defined(wxUSE_NEW_GRID) */

#ifndef wxUSE_NOTEBOOK
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_NOTEBOOK must be defined."
#   else
#       define wxUSE_NOTEBOOK 0
#   endif
#endif /* !defined(wxUSE_NOTEBOOK) */

#ifndef wxUSE_POPUPWIN
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_POPUPWIN must be defined."
#   else
#       define wxUSE_POPUPWIN 0
#   endif
#endif /* !defined(wxUSE_POPUPWIN) */

#ifndef wxUSE_RADIOBOX
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_RADIOBOX must be defined."
#   else
#       define wxUSE_RADIOBOX 0
#   endif
#endif /* !defined(wxUSE_RADIOBOX) */

#ifndef wxUSE_RADIOBTN
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_RADIOBTN must be defined."
#   else
#       define wxUSE_RADIOBTN 0
#   endif
#endif /* !defined(wxUSE_RADIOBTN) */

#ifndef wxUSE_SASH
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_SASH must be defined."
#   else
#       define wxUSE_SASH 0
#   endif
#endif /* !defined(wxUSE_SASH) */

#ifndef wxUSE_SCROLLBAR
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_SCROLLBAR must be defined."
#   else
#       define wxUSE_SCROLLBAR 0
#   endif
#endif /* !defined(wxUSE_SCROLLBAR) */

#ifndef wxUSE_SLIDER
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_SLIDER must be defined."
#   else
#       define wxUSE_SLIDER 0
#   endif
#endif /* !defined(wxUSE_SLIDER) */

#ifndef wxUSE_SPINBTN
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_SPINBTN must be defined."
#   else
#       define wxUSE_SPINBTN 0
#   endif
#endif /* !defined(wxUSE_SPINBTN) */

#ifndef wxUSE_SPINCTRL
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_SPINCTRL must be defined."
#   else
#       define wxUSE_SPINCTRL 0
#   endif
#endif /* !defined(wxUSE_SPINCTRL) */

#ifndef wxUSE_SPLITTER
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_SPLITTER must be defined."
#   else
#       define wxUSE_SPLITTER 0
#   endif
#endif /* !defined(wxUSE_SPLITTER) */

#ifndef wxUSE_STATBMP
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_STATBMP must be defined."
#   else
#       define wxUSE_STATBMP 0
#   endif
#endif /* !defined(wxUSE_STATBMP) */

#ifndef wxUSE_STATBOX
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_STATBOX must be defined."
#   else
#       define wxUSE_STATBOX 0
#   endif
#endif /* !defined(wxUSE_STATBOX) */

#ifndef wxUSE_STATLINE
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_STATLINE must be defined."
#   else
#       define wxUSE_STATLINE 0
#   endif
#endif /* !defined(wxUSE_STATLINE) */

#ifndef wxUSE_STATUSBAR
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_STATUSBAR must be defined."
#   else
#       define wxUSE_STATUSBAR 0
#   endif
#endif /* !defined(wxUSE_STATUSBAR) */

#ifndef wxUSE_STOPWATCH
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_STOPWATCH must be defined."
#   else
#       define wxUSE_STOPWATCH 0
#   endif
#endif /* !defined(wxUSE_STOPWATCH) */

#ifndef wxUSE_TAB_DIALOG
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_TAB_DIALOG must be defined."
#   else
#       define wxUSE_TAB_DIALOG 0
#   endif
#endif /* !defined(wxUSE_TAB_DIALOG) */

#ifndef wxUSE_TEXTCTRL
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_TEXTCTRL must be defined."
#   else
#       define wxUSE_TEXTCTRL 0
#   endif
#endif /* !defined(wxUSE_TEXTCTRL) */

#ifndef wxUSE_TOOLBAR
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_TOOLBAR must be defined."
#   else
#       define wxUSE_TOOLBAR 0
#   endif
#endif /* !defined(wxUSE_TOOLBAR) */

#ifndef wxUSE_TOOLTIPS
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_TOOLTIPS must be defined."
#   else
#       define wxUSE_TOOLTIPS 0
#   endif
#endif /* !defined(wxUSE_TOOLTIPS) */

#ifndef wxUSE_TREECTRL
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_TREECTRL must be defined."
#   else
#       define wxUSE_TREECTRL 0
#   endif
#endif /* !defined(wxUSE_TREECTRL) */

/*
   check consistency of the settings
 */
#if wxUSE_BUTTON || \
    wxUSE_CALENDARCTRL || \
    wxUSE_CARET || \
    wxUSE_COMBOBOX || \
    wxUSE_BMPBUTTON || \
    wxUSE_CHECKBOX || \
    wxUSE_CHECKLISTBOX || \
    wxUSE_CHOICE || \
    wxUSE_GAUGE || \
    wxUSE_GRID || \
    wxUSE_NEW_GRID || \
    wxUSE_LISTBOX || \
    wxUSE_LISTCTRL || \
    wxUSE_NOTEBOOK || \
    wxUSE_RADIOBOX || \
    wxUSE_RADIOBTN || \
    wxUSE_SCROLLBAR || \
    wxUSE_SLIDER || \
    wxUSE_SPINBTN || \
    wxUSE_SPINCTRL || \
    wxUSE_STATBMP || \
    wxUSE_STATBOX || \
    wxUSE_STATLINE || \
    wxUSE_STATUSBAR || \
    wxUSE_TEXTCTRL || \
    wxUSE_TOOLBAR || \
    wxUSE_TREECTRL
#    if !wxUSE_CONTROLS
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxUSE_CONTROLS unset but some controls used"
#        else
#            undef wxUSE_CONTROLS
#            define wxUSE_CONTROLS 1
#        endif
#    endif
#endif /* controls */

#if wxUSE_CALENDARCTRL
#   if !(wxUSE_SPINBTN && wxUSE_COMBOBOX)
#       ifdef wxABORT_ON_CONFIG_ERROR
#           error "wxCalendarCtrl requires wxSpinButton and wxComboBox"
#       else
#           undef wxUSE_SPINBTN
#           undef wxUSE_COMBOBOX
#           define wxUSE_SPINBTN 1
#           define wxUSE_COMBOBOX 1
#       endif
#   endif
#endif /* wxUSE_CALENDARCTRL */

#if wxUSE_CHECKLISTBOX
#   if !wxUSE_LISTBOX
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxCheckListBox requires wxListBox"
#        else
#            undef wxUSE_LISTBOX
#            define wxUSE_LISTBOX 1
#        endif
#   endif
#endif /* wxUSE_RADIOBTN */

#if wxUSE_COMBOBOX
#   if defined(__WXUNIVERSAL__) && !wxUSE_LISTBOX
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxComboBox requires wxListBox in wxUniversal"
#        else
#            undef wxUSE_LISTBOX
#            define wxUSE_LISTBOX 1
#        endif
#   endif
#endif /* wxUSE_COMBOBOX */

#if !wxUSE_FILEDLG
#   if wxUSE_DOC_VIEW_ARCHITECTURE || wxUSE_HTML
#       ifdef wxABORT_ON_CONFIG_ERROR
#           error "wxFileDialog must be compiled as well"
#       else
#           undef wxUSE_FILEDLG
#           define wxUSE_FILEDLG 1
#       endif
#   endif
#endif /* wxUSE_FILEDLG */

#if !wxUSE_FONTDLG
#   if wxUSE_FONTMAP
#       ifdef wxABORT_ON_CONFIG_ERROR
#           error "wxFontDialog must be compiled as well"
#       else
#           undef wxUSE_FONTDLG
#           define wxUSE_FONTDLG 1
#       endif
#   endif
#endif /* !wxUSE_FONTDLG */

#if !wxUSE_IMAGLIST
#   if wxUSE_TREECTRL || wxUSE_NOTEBOOK || wxUSE_LISTCTRL
#       ifdef wxABORT_ON_CONFIG_ERROR
#           error "wxImageList must be compiled as well"
#       else
#           undef wxUSE_IMAGLIST
#           define wxUSE_IMAGLIST 1
#       endif
#   endif
#endif /* !wxUSE_IMAGLIST */

#if !wxUSE_MSGDLG
#   ifdef wxABORT_ON_CONFIG_ERROR
        /* FIXME: should compile without it, of course, but doesn't */
#       error "wxMessageBox is always needed"
#   else
#       undef wxUSE_MSGDLG
#       define wxUSE_MSGDLG 1
#   endif
#endif

#if wxUSE_RADIOBTN
#   if defined(__WXUNIVERSAL__) && !wxUSE_CHECKBOX
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxUSE_RADIOBTN requires wxUSE_CHECKBOX in wxUniversal"
#        else
#            undef wxUSE_CHECKBOX
#            define wxUSE_CHECKBOX 1
#        endif
#   endif
#endif /* wxUSE_RADIOBTN */

#if wxUSE_RADIOBOX
#   if !wxUSE_RADIOBTN || !wxUSE_STATBOX
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxUSE_RADIOBOX requires wxUSE_RADIOBTN and wxUSE_STATBOX"
#        else
#            undef wxUSE_RADIOBTN
#            undef wxUSE_STATBOX
#            define wxUSE_RADIOBTN 1
#            define wxUSE_STATBOX 1
#        endif
#   endif
#endif /* wxUSE_RADIOBOX */

#if wxUSE_STOPWATCH
#    if !wxUSE_LONGLONG
#        ifdef wxABORT_ON_CONFIG_ERROR
#            error "wxUSE_STOPWATCH requires wxUSE_LONGLONG"
#        else
#            undef wxUSE_LONGLONG
#            define wxUSE_LONGLONG 1
#        endif
#    endif
#endif /* wxUSE_STOPWATCH */

#if wxUSE_TEXTCTRL
#   ifdef __WXUNIVERSAL__
#       if !wxUSE_CARET
#           ifdef wxABORT_ON_CONFIG_ERROR
#               error "wxTextCtrl requires wxCaret in wxUniversal"
#           else
#               undef wxUSE_CARET
#               define wxUSE_CARET 1
#           endif
#       endif /* wxUSE_CARET */

#       if !wxUSE_SCROLLBAR
#           ifdef wxABORT_ON_CONFIG_ERROR
#               error "wxTextCtrl requires wxScrollBar in wxUniversal"
#           else
#               undef wxUSE_SCROLLBAR
#               define wxUSE_SCROLLBAR 1
#           endif
#       endif /* wxUSE_SCROLLBAR */
#   endif /* __WXUNIVERSAL__ */
#endif /* wxUSE_RADIOBTN */

/* I wonder if we shouldn't just remove all occurrences of
   wxUSE_DYNAMIC_CLASSES from the sources? */
#if !defined(wxUSE_DYNAMIC_CLASSES) || !wxUSE_DYNAMIC_CLASSES
#   if wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_DYNAMIC_CLASSES must be defined as 1"
#   else
#       undef wxUSE_DYNAMIC_CLASSES
#       define wxUSE_DYNAMIC_CLASSES
#   endif
#endif /* wxUSE_DYNAMIC_CLASSES */

#if wxUSE_MIMETYPE && !wxUSE_TEXTFILE
#   if wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_MIMETYPE requires wxUSE_TEXTFILE"
#   else
#       undef wxUSE_TEXTFILE
#       define wxUSE_TEXTFILE 1
#   endif
#endif /* wxUSE_MIMETYPE */

#if wxUSE_TEXTFILE && !wxUSE_FILE
#   if wxABORT_ON_CONFIG_ERROR
#       error "wxUSE_TEXTFILE requires wxUSE_FILE"
#   else
#       undef wxUSE_FILE
#       define wxUSE_FILE 1
#   endif
#endif /* wxUSE_TEXTFILE */

#if wxUSE_CLIPBOARD && !wxUSE_DATAOBJ
#   if wxABORT_ON_CONFIG_ERROR
#       error "wxClipboard requires wxDataObject"
#   else
#       undef wxUSE_DATAOBJ
#       define wxUSE_DATAOBJ 1
#   endif
#endif /* wxUSE_CLIPBOARD */

/* wxUniv-specific dependencies */
#if defined(__WXUNIVERSAL__)
#   if (wxUSE_COMBOBOX || wxUSE_MENUS) && !wxUSE_POPUPWIN
#       if wxABORT_ON_CONFIG_ERROR
#           error "wxUSE_POPUPWIN must be defined to use comboboxes/menus"
#       else
#           undef wxUSE_POPUPWIN
#           define wxUSE_POPUPWIN 1
#       endif
#   endif
#endif /* wxUSE_POPUPWIN */

/* wxGTK-specific dependencies */
#if defined(__WXGTK__) && !defined(__WXUNIVERSAL__)
#   if wxUSE_MDI_ARCHITECTURE && !wxUSE_MENUS
#       if wxABORT_ON_CONFIG_ERROR
#           error "MDI requires wxUSE_MENUS in wxGTK"
#       else
#           undef wxUSE_MENUS 
#           define wxUSE_MENUS 1
#       endif
#   endif
#endif /* wxGTK && !wxUniv */

#endif /* wxUSE_GUI */

