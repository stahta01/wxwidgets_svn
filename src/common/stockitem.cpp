///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/stockitem.cpp
// Purpose:     Stock buttons, menu and toolbar items labels
// Author:      Vaclav Slavik
// Modified by:
// Created:     2004-08-15
// RCS-ID:      $Id$
// Copyright:   (c) Vaclav Slavik, 2004
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/stockitem.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/utils.h" // for wxStripMenuCodes()
#endif

bool wxIsStockID(wxWindowID id)
{
    switch (id)
    {
        case wxID_ABOUT:
        case wxID_ADD:
        case wxID_APPLY:
        case wxID_BACKWARD:
        case wxID_BOLD:
        case wxID_BOTTOM:
        case wxID_CANCEL:
        case wxID_CDROM:
        case wxID_CLEAR:
        case wxID_CLOSE:
        case wxID_CONVERT:
        case wxID_COPY:
        case wxID_CUT:
        case wxID_DELETE:
        case wxID_DOWN:
        case wxID_EDIT:
        case wxID_EXECUTE:
        case wxID_EXIT:
        case wxID_FILE:
        case wxID_FIND:
        case wxID_FIRST:
        case wxID_FLOPPY:
        case wxID_FORWARD:
        case wxID_HARDDISK:
        case wxID_HELP:
        case wxID_HOME:
        case wxID_INDENT:
        case wxID_INDEX:
        case wxID_INFO:
        case wxID_ITALIC:
        case wxID_JUMP_TO:
        case wxID_JUSTIFY_CENTER:
        case wxID_JUSTIFY_FILL:
        case wxID_JUSTIFY_LEFT:
        case wxID_JUSTIFY_RIGHT:
        case wxID_LAST:
        case wxID_NETWORK:
        case wxID_NEW:
        case wxID_NO:
        case wxID_OK:
        case wxID_OPEN:
        case wxID_PASTE:
        case wxID_PREFERENCES:
        case wxID_PREVIEW:
        case wxID_PRINT:
        case wxID_PROPERTIES:
        case wxID_REDO:
        case wxID_REFRESH:
        case wxID_REMOVE:
        case wxID_REPLACE:
        case wxID_REVERT_TO_SAVED:
        case wxID_SAVE:
        case wxID_SAVEAS:
        case wxID_SELECTALL:
        case wxID_SELECT_COLOR:
        case wxID_SELECT_FONT:
        case wxID_SORT_ASCENDING:
        case wxID_SORT_DESCENDING:
        case wxID_SPELL_CHECK:
        case wxID_STOP:
        case wxID_STRIKETHROUGH:
        case wxID_TOP:
        case wxID_UNDELETE:
        case wxID_UNDERLINE:
        case wxID_UNDO:
        case wxID_UNINDENT:
        case wxID_UP:
        case wxID_YES:
        case wxID_ZOOM_100:
        case wxID_ZOOM_FIT:
        case wxID_ZOOM_IN:
        case wxID_ZOOM_OUT:
            return true;

        default:
            return false;
    }
}

wxString wxGetStockLabel(wxWindowID id, long flags)
{
    wxString stockLabel;

    #define STOCKITEM(stockid, label) \
        case stockid:                 \
            stockLabel = label;       \
            break;

    switch (id)
    {
        STOCKITEM(wxID_ABOUT,               _("&About..."))
        STOCKITEM(wxID_ADD,                 _("Add"))
        STOCKITEM(wxID_APPLY,               _("&Apply"))
        STOCKITEM(wxID_BACKWARD,            _("&Back"))
        STOCKITEM(wxID_BOLD,                _("&Bold"))
        STOCKITEM(wxID_BOTTOM,              _("&Bottom"))
        STOCKITEM(wxID_CANCEL,              _("&Cancel"))
        STOCKITEM(wxID_CDROM,               _("&CD-Rom"))
        STOCKITEM(wxID_CLEAR,               _("&Clear"))
        STOCKITEM(wxID_CLOSE,               _("&Close"))
        STOCKITEM(wxID_CONVERT,             _("&Convert"))
        STOCKITEM(wxID_COPY,                _("&Copy"))
        STOCKITEM(wxID_CUT,                 _("Cu&t"))
        STOCKITEM(wxID_DELETE,              _("&Delete"))
        STOCKITEM(wxID_DOWN,                _("&Down"))
        STOCKITEM(wxID_EDIT,                _("&Edit"))
        STOCKITEM(wxID_EXECUTE,             _("&Execute"))
        STOCKITEM(wxID_EXIT,                _("&Quit"))
        STOCKITEM(wxID_FILE,                _("&File"))
        STOCKITEM(wxID_FIND,                _("&Find"))
        STOCKITEM(wxID_FIRST,               _("&First"))
        STOCKITEM(wxID_FLOPPY,              _("&Floppy"))
        STOCKITEM(wxID_FORWARD,             _("&Forward"))
        STOCKITEM(wxID_HARDDISK,            _("&Harddisk"))
        STOCKITEM(wxID_HELP,                _("&Help"))
        STOCKITEM(wxID_HOME,                _("&Home"))
        STOCKITEM(wxID_INDENT,              _("Indent"))
        STOCKITEM(wxID_INDEX,               _("&Index"))
        STOCKITEM(wxID_INFO,                _("&Info"))
        STOCKITEM(wxID_ITALIC,              _("&Italic"))
        STOCKITEM(wxID_JUMP_TO,             _("&Jump to"))
        STOCKITEM(wxID_JUSTIFY_CENTER,      _("Centered"))
        STOCKITEM(wxID_JUSTIFY_FILL,        _("Justified"))
        STOCKITEM(wxID_JUSTIFY_LEFT,        _("Align Left"))
        STOCKITEM(wxID_JUSTIFY_RIGHT,       _("Align Right"))
        STOCKITEM(wxID_LAST,                _("&Last"))
        STOCKITEM(wxID_NETWORK,             _("&Network"))
        STOCKITEM(wxID_NEW,                 _("&New"))
        STOCKITEM(wxID_NO,                  _("&No"))
        STOCKITEM(wxID_OK,                  _("&OK"))
        STOCKITEM(wxID_OPEN,                _("&Open..."))
        STOCKITEM(wxID_PASTE,               _("&Paste"))
        STOCKITEM(wxID_PREFERENCES,         _("&Preferences"))
        STOCKITEM(wxID_PREVIEW,             _("Print previe&w"))
        STOCKITEM(wxID_PRINT,               _("&Print..."))
        STOCKITEM(wxID_PROPERTIES,          _("&Properties"))
        STOCKITEM(wxID_REDO,                _("&Redo"))
        STOCKITEM(wxID_REFRESH,             _("Refresh"))
        STOCKITEM(wxID_REMOVE,              _("Remove"))
        STOCKITEM(wxID_REPLACE,             _("Rep&lace"))
        STOCKITEM(wxID_REVERT_TO_SAVED,     _("Revert to Saved"))
        STOCKITEM(wxID_SAVE,                _("&Save"))
        STOCKITEM(wxID_SAVEAS,              _("Save &As..."))
        STOCKITEM(wxID_SELECTALL,           _("Select &All"))
        STOCKITEM(wxID_SELECT_COLOR,        _("&Color"))
        STOCKITEM(wxID_SELECT_FONT,         _("&Font"))
        STOCKITEM(wxID_SORT_ASCENDING,      _("&Ascending"))
        STOCKITEM(wxID_SORT_DESCENDING,     _("&Descending"))
        STOCKITEM(wxID_SPELL_CHECK,         _("&Spell Check"))
        STOCKITEM(wxID_STOP,                _("&Stop"))
        STOCKITEM(wxID_STRIKETHROUGH,       _("&Strikethrough"))
        STOCKITEM(wxID_TOP,                 _("&Top"))
        STOCKITEM(wxID_UNDELETE,            _("Undelete"))
        STOCKITEM(wxID_UNDERLINE,           _("&Underline"))
        STOCKITEM(wxID_UNDO,                _("&Undo"))
        STOCKITEM(wxID_UNINDENT,            _("&Unindent"))
        STOCKITEM(wxID_UP,                  _("&Up"))
        STOCKITEM(wxID_YES,                 _("&Yes"))
        STOCKITEM(wxID_ZOOM_100,            _("&Actual Size"))
        STOCKITEM(wxID_ZOOM_FIT,            _("Zoom to &Fit"))
        STOCKITEM(wxID_ZOOM_IN,             _("Zoom &In"))
        STOCKITEM(wxID_ZOOM_OUT,            _("Zoom &Out"))

        default:
            wxFAIL_MSG( wxT("invalid stock item ID") );
            break;
    };

    #undef STOCKITEM

    if ( flags & wxSTOCK_WITHOUT_ELLIPSIS )
    {
        wxString baseLabel;
        if ( stockLabel.EndsWith("...", &baseLabel) )
            stockLabel = baseLabel;

        // accelerators only make sense for the menu items which should have
        // ellipsis too while wxSTOCK_WITHOUT_ELLIPSIS is mostly useful for
        // buttons which shouldn't have accelerators in their labels
        wxASSERT_MSG( !(flags & wxSTOCK_WITH_ACCELERATOR),
                        "labels without ellipsis shouldn't use accelerators" );
    }

#ifdef __WXMSW__
    // special case: the "Cancel" button shouldn't have a mnemonic under MSW
    // for consistency with the native dialogs (which don't use any mnemonic
    // for it because it is already bound to Esc implicitly)
    if ( id == wxID_CANCEL )
        flags &= ~wxSTOCK_WITH_MNEMONIC;
#endif // __WXMSW__

    if ( !(flags & wxSTOCK_WITH_MNEMONIC) )
    {
        stockLabel = wxStripMenuCodes(stockLabel);
    }

#if wxUSE_ACCEL
    if ( !stockLabel.empty() && (flags & wxSTOCK_WITH_ACCELERATOR) )
    {
        wxAcceleratorEntry accel = wxGetStockAccelerator(id);
        if (accel.IsOk())
            stockLabel << wxT('\t') << accel.ToString();
    }
#endif // wxUSE_ACCEL

    return stockLabel;
}

wxString wxGetStockHelpString(wxWindowID id, wxStockHelpStringClient client)
{
    wxString stockHelp;

    #define STOCKITEM(stockid, ctx, helpstr)             \
        case stockid:                                    \
            if (client==ctx) stockHelp = helpstr;        \
            break;

    switch (id)
    {
        // NB: these help string should be not too specific as they could be used
        //     in completely different programs!
        STOCKITEM(wxID_ABOUT,    wxSTOCK_MENU, _("Show about dialog"))
        STOCKITEM(wxID_COPY,     wxSTOCK_MENU, _("Copy selection"))
        STOCKITEM(wxID_CUT,      wxSTOCK_MENU, _("Cut selection"))
        STOCKITEM(wxID_DELETE,   wxSTOCK_MENU, _("Delete selection"))
        STOCKITEM(wxID_REPLACE,  wxSTOCK_MENU, _("Replace selection"))
        STOCKITEM(wxID_PASTE,    wxSTOCK_MENU, _("Paste selection"))
        STOCKITEM(wxID_EXIT,     wxSTOCK_MENU, _("Quit this program"))
        STOCKITEM(wxID_REDO,     wxSTOCK_MENU, _("Redo last action"))
        STOCKITEM(wxID_UNDO,     wxSTOCK_MENU, _("Undo last action"))
        STOCKITEM(wxID_CLOSE,    wxSTOCK_MENU, _("Close current document"))
        STOCKITEM(wxID_SAVE,     wxSTOCK_MENU, _("Save current document"))
        STOCKITEM(wxID_SAVEAS,   wxSTOCK_MENU, _("Save current document with a different filename"))

        default:
            // there's no stock help string for this ID / client
            return wxEmptyString;
    }

    #undef STOCKITEM

    return stockHelp;
}

#if wxUSE_ACCEL

wxAcceleratorEntry wxGetStockAccelerator(wxWindowID id)
{
    wxAcceleratorEntry ret;

    #define STOCKITEM(stockid, flags, keycode)      \
        case stockid:                               \
            ret.Set(flags, keycode, stockid);       \
            break;

    switch (id)
    {
        STOCKITEM(wxID_COPY,                wxACCEL_CMD,'C')
        STOCKITEM(wxID_CUT,                 wxACCEL_CMD,'X')
        STOCKITEM(wxID_FIND,                wxACCEL_CMD,'F')
        STOCKITEM(wxID_HELP,                wxACCEL_CMD,'H')
        STOCKITEM(wxID_NEW,                 wxACCEL_CMD,'N')
        STOCKITEM(wxID_OPEN,                wxACCEL_CMD,'O')
        STOCKITEM(wxID_PASTE,               wxACCEL_CMD,'V')
        STOCKITEM(wxID_REDO,                wxACCEL_CMD | wxACCEL_SHIFT,'Z')
        STOCKITEM(wxID_REPLACE,             wxACCEL_CMD,'R')
        STOCKITEM(wxID_SAVE,                wxACCEL_CMD,'S')
        STOCKITEM(wxID_UNDO,                wxACCEL_CMD,'Z')

        default:
            // set the wxAcceleratorEntry to return into an invalid state:
            // there's no stock accelerator for that.
            ret.Set(0, 0, id);
            break;
    };

    #undef STOCKITEM

    // always use wxAcceleratorEntry::IsOk on returned value !
    return ret;
}

#endif // wxUSE_ACCEL

bool wxIsStockLabel(wxWindowID id, const wxString& label)
{
    if (label.empty())
        return true;

    wxString stock = wxGetStockLabel(id);

    if (label == stock)
        return true;

    stock.Replace(wxT("&"), wxEmptyString);
    if (label == stock)
        return true;

    return false;
}
