///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/theme.h
// Purpose:     wxTheme class manages all configurable aspects of the
//              application including the look (wxRenderer), feel
//              (wxInputHandler) and the colours (wxColourScheme)
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_THEME_H_
#define _WX_UNIX_THEME_H_

#ifdef __GNUG__
    #pragma interface "theme.h"
#endif

// ----------------------------------------------------------------------------
// wxTheme
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxRenderer;
class WXDLLEXPORT wxInputHandler { };
class WXDLLEXPORT wxColourScheme { };

class WXDLLEXPORT wxTheme
{
public:
    // static methods
    // --------------

    // create the default theme
    static bool CreateDefault();

    // create the theme by name (will return NULL if not found)
    static wxTheme *Create(const wxString& name);

    // change the current scheme
    static wxTheme *Set(wxTheme *theme);

    // get the current theme (never NULL)
    static wxTheme *Get() { return ms_theme; }

    // the theme methods
    // -----------------
    virtual wxRenderer *GetRenderer() = 0;
    virtual wxInputHandler *GetInputHandler() = 0;
    virtual wxColourScheme *GetColourScheme() = 0;

    // implementation only from now on
    // -------------------------------

    virtual ~wxTheme();

    // dynamic creation helpers
    typedef wxTheme *(*Constructor)();

    struct wxThemeInfo
    {
        // theme name and (user readable) description
        wxString name, desc;

        // the function to create a theme object
        Constructor ctor;

        // next node in the linked list or NULL
        wxThemeInfo *next;

        // constructor for the struct itself
        wxThemeInfo(Constructor ctor, const wxChar *name, const wxChar *desc);
    };

private:
    // the list of descriptions of all known themes
    static wxThemeInfo *ms_allThemes;

    // the current theme
    static wxTheme *ms_theme;

    friend wxThemeInfo;
};

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// to declare a new theme, this macro must be used in the class declaration
#define WX_DECLARE_THEME() static wxThemeInfo ms_info

// and this one must be inserted in the source file
#define WX_IMPLEMENT_THEME(classname, themename, themedesc)                 \
    wxTheme *wxCtorFor##themename() { return new classname; }               \
    wxTheme::wxThemeInfo classname::ms_info(wxCtorFor##themename,           \
                                            #themename, themedesc)

#endif // _WX_UNIX_THEME_H_

