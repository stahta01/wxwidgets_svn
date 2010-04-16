/////////////////////////////////////////////////////////////////////////////
// Name:        wx/intl.h
// Purpose:     Internationalization and localisation for wxWidgets
// Author:      Vadim Zeitlin
// Modified by: Michael N. Filippov <michael@idisys.iae.nsk.su>
//              (2003/09/30 - plural forms support)
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// WARNING: Parts of this file are generated. See misc/languages/README for
// details.

#ifndef _WX_INTL_H_
#define _WX_INTL_H_

#include "wx/defs.h"
#include "wx/string.h"

// Make wxLayoutDirection enum available without need for wxUSE_INTL so wxWindow, wxApp
// and other classes are not distrubed by wxUSE_INTL

enum wxLayoutDirection
{
    wxLayout_Default,
    wxLayout_LeftToRight,
    wxLayout_RightToLeft
};

#if wxUSE_INTL

#include "wx/fontenc.h"

// ============================================================================
// global decls
// ============================================================================

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// gettext() style macros (notice that xgettext should be invoked with
// --keyword="_" --keyword="wxPLURAL:1,2" options
// to extract the strings from the sources)
#ifndef WXINTL_NO_GETTEXT_MACRO
    #define _(s)                     wxGetTranslation((s))
    #define wxPLURAL(sing, plur, n)  wxGetTranslation((sing), (plur), n)
#endif

// another one which just marks the strings for extraction, but doesn't
// perform the translation (use -kwxTRANSLATE with xgettext!)
#define wxTRANSLATE(str) str

// ----------------------------------------------------------------------------
// forward decls
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_BASE wxLocale;
class WXDLLIMPEXP_FWD_BASE wxLanguageInfoArray;
class wxMsgCatalog;

// ============================================================================
// locale support
// ============================================================================

// ----------------------------------------------------------------------------
// wxLanguage: defines all supported languages
// ----------------------------------------------------------------------------

// --- --- --- generated code begins here --- --- ---

/**
    The languages supported by wxLocale.

    This enum is generated by misc/languages/genlang.py
    When making changes, please put them into misc/languages/langtabl.txt
*/
enum wxLanguage
{
    /// User's default/preffered language as got from OS.
    wxLANGUAGE_DEFAULT,

    /// Unknown language, returned if wxLocale::GetSystemLanguage fails.
    wxLANGUAGE_UNKNOWN,

    wxLANGUAGE_ABKHAZIAN,
    wxLANGUAGE_AFAR,
    wxLANGUAGE_AFRIKAANS,
    wxLANGUAGE_ALBANIAN,
    wxLANGUAGE_AMHARIC,
    wxLANGUAGE_ARABIC,
    wxLANGUAGE_ARABIC_ALGERIA,
    wxLANGUAGE_ARABIC_BAHRAIN,
    wxLANGUAGE_ARABIC_EGYPT,
    wxLANGUAGE_ARABIC_IRAQ,
    wxLANGUAGE_ARABIC_JORDAN,
    wxLANGUAGE_ARABIC_KUWAIT,
    wxLANGUAGE_ARABIC_LEBANON,
    wxLANGUAGE_ARABIC_LIBYA,
    wxLANGUAGE_ARABIC_MOROCCO,
    wxLANGUAGE_ARABIC_OMAN,
    wxLANGUAGE_ARABIC_QATAR,
    wxLANGUAGE_ARABIC_SAUDI_ARABIA,
    wxLANGUAGE_ARABIC_SUDAN,
    wxLANGUAGE_ARABIC_SYRIA,
    wxLANGUAGE_ARABIC_TUNISIA,
    wxLANGUAGE_ARABIC_UAE,
    wxLANGUAGE_ARABIC_YEMEN,
    wxLANGUAGE_ARMENIAN,
    wxLANGUAGE_ASSAMESE,
    wxLANGUAGE_ASTURIAN,
    wxLANGUAGE_AYMARA,
    wxLANGUAGE_AZERI,
    wxLANGUAGE_AZERI_CYRILLIC,
    wxLANGUAGE_AZERI_LATIN,
    wxLANGUAGE_BASHKIR,
    wxLANGUAGE_BASQUE,
    wxLANGUAGE_BELARUSIAN,
    wxLANGUAGE_BENGALI,
    wxLANGUAGE_BHUTANI,
    wxLANGUAGE_BIHARI,
    wxLANGUAGE_BISLAMA,
    wxLANGUAGE_BRETON,
    wxLANGUAGE_BULGARIAN,
    wxLANGUAGE_BURMESE,
    wxLANGUAGE_CAMBODIAN,
    wxLANGUAGE_CATALAN,
    wxLANGUAGE_CHINESE,
    wxLANGUAGE_CHINESE_SIMPLIFIED,
    wxLANGUAGE_CHINESE_TRADITIONAL,
    wxLANGUAGE_CHINESE_HONGKONG,
    wxLANGUAGE_CHINESE_MACAU,
    wxLANGUAGE_CHINESE_SINGAPORE,
    wxLANGUAGE_CHINESE_TAIWAN,
    wxLANGUAGE_CORSICAN,
    wxLANGUAGE_CROATIAN,
    wxLANGUAGE_CZECH,
    wxLANGUAGE_DANISH,
    wxLANGUAGE_DUTCH,
    wxLANGUAGE_DUTCH_BELGIAN,
    wxLANGUAGE_ENGLISH,
    wxLANGUAGE_ENGLISH_UK,
    wxLANGUAGE_ENGLISH_US,
    wxLANGUAGE_ENGLISH_AUSTRALIA,
    wxLANGUAGE_ENGLISH_BELIZE,
    wxLANGUAGE_ENGLISH_BOTSWANA,
    wxLANGUAGE_ENGLISH_CANADA,
    wxLANGUAGE_ENGLISH_CARIBBEAN,
    wxLANGUAGE_ENGLISH_DENMARK,
    wxLANGUAGE_ENGLISH_EIRE,
    wxLANGUAGE_ENGLISH_JAMAICA,
    wxLANGUAGE_ENGLISH_NEW_ZEALAND,
    wxLANGUAGE_ENGLISH_PHILIPPINES,
    wxLANGUAGE_ENGLISH_SOUTH_AFRICA,
    wxLANGUAGE_ENGLISH_TRINIDAD,
    wxLANGUAGE_ENGLISH_ZIMBABWE,
    wxLANGUAGE_ESPERANTO,
    wxLANGUAGE_ESTONIAN,
    wxLANGUAGE_FAEROESE,
    wxLANGUAGE_FARSI,
    wxLANGUAGE_FIJI,
    wxLANGUAGE_FINNISH,
    wxLANGUAGE_FRENCH,
    wxLANGUAGE_FRENCH_BELGIAN,
    wxLANGUAGE_FRENCH_CANADIAN,
    wxLANGUAGE_FRENCH_LUXEMBOURG,
    wxLANGUAGE_FRENCH_MONACO,
    wxLANGUAGE_FRENCH_SWISS,
    wxLANGUAGE_FRISIAN,
    wxLANGUAGE_GALICIAN,
    wxLANGUAGE_GEORGIAN,
    wxLANGUAGE_GERMAN,
    wxLANGUAGE_GERMAN_AUSTRIAN,
    wxLANGUAGE_GERMAN_BELGIUM,
    wxLANGUAGE_GERMAN_LIECHTENSTEIN,
    wxLANGUAGE_GERMAN_LUXEMBOURG,
    wxLANGUAGE_GERMAN_SWISS,
    wxLANGUAGE_GREEK,
    wxLANGUAGE_GREENLANDIC,
    wxLANGUAGE_GUARANI,
    wxLANGUAGE_GUJARATI,
    wxLANGUAGE_HAUSA,
    wxLANGUAGE_HEBREW,
    wxLANGUAGE_HINDI,
    wxLANGUAGE_HUNGARIAN,
    wxLANGUAGE_ICELANDIC,
    wxLANGUAGE_INDONESIAN,
    wxLANGUAGE_INTERLINGUA,
    wxLANGUAGE_INTERLINGUE,
    wxLANGUAGE_INUKTITUT,
    wxLANGUAGE_INUPIAK,
    wxLANGUAGE_IRISH,
    wxLANGUAGE_ITALIAN,
    wxLANGUAGE_ITALIAN_SWISS,
    wxLANGUAGE_JAPANESE,
    wxLANGUAGE_JAVANESE,
    wxLANGUAGE_KANNADA,
    wxLANGUAGE_KASHMIRI,
    wxLANGUAGE_KASHMIRI_INDIA,
    wxLANGUAGE_KAZAKH,
    wxLANGUAGE_KERNEWEK,
    wxLANGUAGE_KINYARWANDA,
    wxLANGUAGE_KIRGHIZ,
    wxLANGUAGE_KIRUNDI,
    wxLANGUAGE_KONKANI,
    wxLANGUAGE_KOREAN,
    wxLANGUAGE_KURDISH,
    wxLANGUAGE_LAOTHIAN,
    wxLANGUAGE_LATIN,
    wxLANGUAGE_LATVIAN,
    wxLANGUAGE_LINGALA,
    wxLANGUAGE_LITHUANIAN,
    wxLANGUAGE_MACEDONIAN,
    wxLANGUAGE_MALAGASY,
    wxLANGUAGE_MALAY,
    wxLANGUAGE_MALAYALAM,
    wxLANGUAGE_MALAY_BRUNEI_DARUSSALAM,
    wxLANGUAGE_MALAY_MALAYSIA,
    wxLANGUAGE_MALTESE,
    wxLANGUAGE_MANIPURI,
    wxLANGUAGE_MAORI,
    wxLANGUAGE_MARATHI,
    wxLANGUAGE_MOLDAVIAN,
    wxLANGUAGE_MONGOLIAN,
    wxLANGUAGE_NAURU,
    wxLANGUAGE_NEPALI,
    wxLANGUAGE_NEPALI_INDIA,
    wxLANGUAGE_NORWEGIAN_BOKMAL,
    wxLANGUAGE_NORWEGIAN_NYNORSK,
    wxLANGUAGE_OCCITAN,
    wxLANGUAGE_ORIYA,
    wxLANGUAGE_OROMO,
    wxLANGUAGE_PASHTO,
    wxLANGUAGE_POLISH,
    wxLANGUAGE_PORTUGUESE,
    wxLANGUAGE_PORTUGUESE_BRAZILIAN,
    wxLANGUAGE_PUNJABI,
    wxLANGUAGE_QUECHUA,
    wxLANGUAGE_RHAETO_ROMANCE,
    wxLANGUAGE_ROMANIAN,
    wxLANGUAGE_RUSSIAN,
    wxLANGUAGE_RUSSIAN_UKRAINE,
    wxLANGUAGE_SAMI,
    wxLANGUAGE_SAMOAN,
    wxLANGUAGE_SANGHO,
    wxLANGUAGE_SANSKRIT,
    wxLANGUAGE_SCOTS_GAELIC,
    wxLANGUAGE_SERBIAN,
    wxLANGUAGE_SERBIAN_CYRILLIC,
    wxLANGUAGE_SERBIAN_LATIN,
    wxLANGUAGE_SERBO_CROATIAN,
    wxLANGUAGE_SESOTHO,
    wxLANGUAGE_SETSWANA,
    wxLANGUAGE_SHONA,
    wxLANGUAGE_SINDHI,
    wxLANGUAGE_SINHALESE,
    wxLANGUAGE_SISWATI,
    wxLANGUAGE_SLOVAK,
    wxLANGUAGE_SLOVENIAN,
    wxLANGUAGE_SOMALI,
    wxLANGUAGE_SPANISH,
    wxLANGUAGE_SPANISH_ARGENTINA,
    wxLANGUAGE_SPANISH_BOLIVIA,
    wxLANGUAGE_SPANISH_CHILE,
    wxLANGUAGE_SPANISH_COLOMBIA,
    wxLANGUAGE_SPANISH_COSTA_RICA,
    wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC,
    wxLANGUAGE_SPANISH_ECUADOR,
    wxLANGUAGE_SPANISH_EL_SALVADOR,
    wxLANGUAGE_SPANISH_GUATEMALA,
    wxLANGUAGE_SPANISH_HONDURAS,
    wxLANGUAGE_SPANISH_MEXICAN,
    wxLANGUAGE_SPANISH_MODERN,
    wxLANGUAGE_SPANISH_NICARAGUA,
    wxLANGUAGE_SPANISH_PANAMA,
    wxLANGUAGE_SPANISH_PARAGUAY,
    wxLANGUAGE_SPANISH_PERU,
    wxLANGUAGE_SPANISH_PUERTO_RICO,
    wxLANGUAGE_SPANISH_URUGUAY,
    wxLANGUAGE_SPANISH_US,
    wxLANGUAGE_SPANISH_VENEZUELA,
    wxLANGUAGE_SUNDANESE,
    wxLANGUAGE_SWAHILI,
    wxLANGUAGE_SWEDISH,
    wxLANGUAGE_SWEDISH_FINLAND,
    wxLANGUAGE_TAGALOG,
    wxLANGUAGE_TAJIK,
    wxLANGUAGE_TAMIL,
    wxLANGUAGE_TATAR,
    wxLANGUAGE_TELUGU,
    wxLANGUAGE_THAI,
    wxLANGUAGE_TIBETAN,
    wxLANGUAGE_TIGRINYA,
    wxLANGUAGE_TONGA,
    wxLANGUAGE_TSONGA,
    wxLANGUAGE_TURKISH,
    wxLANGUAGE_TURKMEN,
    wxLANGUAGE_TWI,
    wxLANGUAGE_UIGHUR,
    wxLANGUAGE_UKRAINIAN,
    wxLANGUAGE_URDU,
    wxLANGUAGE_URDU_INDIA,
    wxLANGUAGE_URDU_PAKISTAN,
    wxLANGUAGE_UZBEK,
    wxLANGUAGE_UZBEK_CYRILLIC,
    wxLANGUAGE_UZBEK_LATIN,
    wxLANGUAGE_VALENCIAN,
    wxLANGUAGE_VIETNAMESE,
    wxLANGUAGE_VOLAPUK,
    wxLANGUAGE_WELSH,
    wxLANGUAGE_WOLOF,
    wxLANGUAGE_XHOSA,
    wxLANGUAGE_YIDDISH,
    wxLANGUAGE_YORUBA,
    wxLANGUAGE_ZHUANG,
    wxLANGUAGE_ZULU,

    /// For custom, user-defined languages.
    wxLANGUAGE_USER_DEFINED
};

// --- --- --- generated code ends here --- --- ---

// ----------------------------------------------------------------------------
// wxLanguageInfo: encapsulates wxLanguage to OS native lang.desc.
//                 translation information
// ----------------------------------------------------------------------------

struct WXDLLIMPEXP_BASE wxLanguageInfo
{
    int Language;                   // wxLanguage id
    wxString CanonicalName;         // Canonical name, e.g. fr_FR
#ifdef __WXMSW__
    wxUint32 WinLang,               // Win32 language identifiers
             WinSublang;
#endif // __WXMSW__
    wxString Description;           // human-readable name of the language
    wxLayoutDirection LayoutDirection;

#ifdef __WXMSW__
    // return the LCID corresponding to this language
    wxUint32 GetLCID() const;
#endif // __WXMSW__

    // return the locale name corresponding to this language usable with
    // setlocale() on the current system
    wxString GetLocaleName() const;
};

// for Unix systems GetLocaleName() is trivial so implement it inline here, for
// MSW it's implemented in intl.cpp
#ifndef __WXMSW__
inline wxString wxLanguageInfo::GetLocaleName() const { return CanonicalName; }
#endif // !__WXMSW__

// ----------------------------------------------------------------------------
// wxLocaleCategory: the category of locale settings
// ----------------------------------------------------------------------------

enum wxLocaleCategory
{
    // (any) numbers
    wxLOCALE_CAT_NUMBER,

    // date/time
    wxLOCALE_CAT_DATE,

    // monetary value
    wxLOCALE_CAT_MONEY,

    // default category for wxLocaleInfo values which only apply to a single
    // category (e.g. wxLOCALE_SHORT_DATE_FMT)
    wxLOCALE_CAT_DEFAULT,

    wxLOCALE_CAT_MAX
};

// ----------------------------------------------------------------------------
// wxLocaleInfo: the items understood by wxLocale::GetInfo()
// ----------------------------------------------------------------------------

enum wxLocaleInfo
{
    // the thousands separator (for wxLOCALE_CAT_NUMBER or MONEY)
    wxLOCALE_THOUSANDS_SEP,

    // the character used as decimal point (for wxLOCALE_CAT_NUMBER or MONEY)
    wxLOCALE_DECIMAL_POINT,

    // the stftime()-formats used for short/long date and time representations
    // (under some platforms short and long date formats are the same)
    //
    // NB: these elements should appear in this order, code in GetInfo() relies
    //     on it
    wxLOCALE_SHORT_DATE_FMT,
    wxLOCALE_LONG_DATE_FMT,
    wxLOCALE_DATE_TIME_FMT,
    wxLOCALE_TIME_FMT

};

// ----------------------------------------------------------------------------
// wxLocale: encapsulates all language dependent settings, including current
//           message catalogs, date, time and currency formats (TODO) &c
// ----------------------------------------------------------------------------

enum wxLocaleInitFlags
{
    wxLOCALE_DONT_LOAD_DEFAULT = 0x0000,     // don't load wxwin.mo
    wxLOCALE_LOAD_DEFAULT      = 0x0001      // load wxwin.mo?
#if WXWIN_COMPATIBILITY_2_8
   ,wxLOCALE_CONV_ENCODING     = 0x0002      // no longer used, simply remove
                                             // it from the existing code
#endif
};

class WXDLLIMPEXP_BASE wxLocale
{
public:
    // ctor & dtor
    // -----------

        // call Init() if you use this ctor
    wxLocale() { DoCommonInit(); }

        // the ctor has a side effect of changing current locale
    wxLocale(const wxString& name,                               // name (for messages)
             const wxString& shortName = wxEmptyString,      // dir prefix (for msg files)
             const wxString& locale = wxEmptyString,     // locale (for setlocale)
             bool bLoadDefault = true                            // preload wxstd.mo?
#if WXWIN_COMPATIBILITY_2_8
             ,bool bConvertEncoding = true                      // convert Win<->Unix if necessary?
#endif
             )
        {
            DoCommonInit();

            Init(name, shortName, locale, bLoadDefault, bConvertEncoding);
        }

    wxLocale(int language, // wxLanguage id or custom language
             int flags = wxLOCALE_LOAD_DEFAULT)
        {
            DoCommonInit();

            Init(language, flags);
        }

        // the same as a function (returns true on success)
    bool Init(const wxString& name,
              const wxString& shortName = wxEmptyString,
              const wxString& locale = wxEmptyString,
              bool bLoadDefault = true
#if WXWIN_COMPATIBILITY_2_8
              ,bool bConvertEncoding = true
#endif
              );

        // same as second ctor (returns true on success)
    bool Init(int language = wxLANGUAGE_DEFAULT,
              int flags = wxLOCALE_LOAD_DEFAULT);

        // restores old locale
    virtual ~wxLocale();

    // Try to get user's (or OS's) preferred language setting.
    // Return wxLANGUAGE_UNKNOWN if language-guessing algorithm failed
    static int GetSystemLanguage();

    // get the encoding used by default for text on this system, returns
    // wxFONTENCODING_SYSTEM if it couldn't be determined
    static wxFontEncoding GetSystemEncoding();

    // get the string describing the system encoding, return empty string if
    // couldn't be determined
    static wxString GetSystemEncodingName();

    // get the values of the given locale-dependent datum: the current locale
    // is used, the US default value is returned if everything else fails
    static wxString GetInfo(wxLocaleInfo index,
                            wxLocaleCategory cat = wxLOCALE_CAT_DEFAULT);

    // return true if the locale was set successfully
    bool IsOk() const { return m_pszOldLocale != NULL; }

    // returns locale name
    const wxString& GetLocale() const { return m_strLocale; }

    // return current locale wxLanguage value
    int GetLanguage() const { return m_language; }

    // return locale name to be passed to setlocale()
    wxString GetSysName() const;

    // return 'canonical' name, i.e. in the form of xx[_YY], where xx is
    // language code according to ISO 639 and YY is country name
    // as specified by ISO 3166.
    wxString GetCanonicalName() const { return m_strShort; }

    // add a prefix to the catalog lookup path: the message catalog files will be
    // looked up under prefix/<lang>/LC_MESSAGES, prefix/LC_MESSAGES and prefix
    // (in this order).
    //
    // This only applies to subsequent invocations of AddCatalog()!
    static void AddCatalogLookupPathPrefix(const wxString& prefix);

    // add a catalog: it's searched for in standard places (current directory
    // first, system one after), but the you may prepend additional directories to
    // the search path with AddCatalogLookupPathPrefix().
    //
    // The loaded catalog will be used for message lookup by GetString().
    //
    // Returns 'true' if it was successfully loaded
    bool AddCatalog(const wxString& domain);
    bool AddCatalog(const wxString& domain,
                    wxLanguage msgIdLanguage, const wxString& msgIdCharset);

    // check if the given locale is provided by OS and C run time
    static bool IsAvailable(int lang);

    // check if the given catalog is loaded
    bool IsLoaded(const wxString& domain) const;

    // Retrieve the language info struct for the given language
    //
    // Returns NULL if no info found, pointer must *not* be deleted by caller
    static const wxLanguageInfo *GetLanguageInfo(int lang);

    // Returns language name in English or empty string if the language
    // is not in database
    static wxString GetLanguageName(int lang);

    // Find the language for the given locale string which may be either a
    // canonical ISO 2 letter language code ("xx"), a language code followed by
    // the country code ("xx_XX") or a Windows full language name ("Xxxxx...")
    //
    // Returns NULL if no info found, pointer must *not* be deleted by caller
    static const wxLanguageInfo *FindLanguageInfo(const wxString& locale);

    // Add custom language to the list of known languages.
    // Notes: 1) wxLanguageInfo contains platform-specific data
    //        2) must be called before Init to have effect
    static void AddLanguage(const wxLanguageInfo& info);

    // retrieve the translation for a string in all loaded domains unless
    // the szDomain parameter is specified (and then only this domain is
    // searched)
    // n - additional parameter for PluralFormsParser
    //
    // return original string if translation is not available
    // (in this case an error message is generated the first time
    //  a string is not found; use wxLogNull to suppress it)
    //
    // domains are searched in the last to first order, i.e. catalogs
    // added later override those added before.
    virtual const wxString& GetString(const wxString& origString,
                                      const wxString& domain = wxEmptyString) const;
    // plural form version of the same:
    virtual const wxString& GetString(const wxString& origString,
                                      const wxString& origString2,
                                      size_t n,
                                      const wxString& domain = wxEmptyString) const;

    // this is hack to work around a problem with wxGetTranslation() which
    // returns const wxString& and not wxString, so when it returns untranslated
    // string, it needs to have a copy of it somewhere
    static const wxString& GetUntranslatedString(const wxString& str);

    // Returns the current short name for the locale
    const wxString& GetName() const { return m_strShort; }

    // return the contents of .po file header
    wxString GetHeaderValue(const wxString& header,
                            const wxString& domain = wxEmptyString) const;

    // These two methods are for internal use only. First one creates
    // ms_languagesDB if it doesn't already exist, second one destroys
    // it.
    static void CreateLanguagesDB();
    static void DestroyLanguagesDB();

private:
    // find catalog by name in a linked list, return NULL if !found
    wxMsgCatalog *FindCatalog(const wxString& domain) const;

    // copy default table of languages from global static array to
    // m_langugagesInfo, called by InitLanguagesDB
    static void InitLanguagesDB();

    // initialize the member fields to default values
    void DoCommonInit();

    wxString       m_strLocale,       // this locale name
                   m_strShort;        // short name for the locale
    int            m_language;        // this locale wxLanguage value

    const char  *m_pszOldLocale;      // previous locale from setlocale()
    wxLocale      *m_pOldLocale;      // previous wxLocale

    wxMsgCatalog  *m_pMsgCat;         // pointer to linked list of catalogs

    bool           m_initialized;

    static wxLanguageInfoArray *ms_languagesDB;

    wxDECLARE_NO_COPY_CLASS(wxLocale);
};

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

// get the current locale object (note that it may be NULL!)
extern WXDLLIMPEXP_BASE wxLocale* wxGetLocale();

// get the translation of the string in the current locale
inline const wxString& wxGetTranslation(const wxString& str,
                                        const wxString& domain = wxEmptyString)
{
    wxLocale *pLoc = wxGetLocale();
    if (pLoc)
        return pLoc->GetString(str, domain);
    else
        // NB: this function returns reference to a string, so we have to keep
        //     a copy of it somewhere
        return wxLocale::GetUntranslatedString(str);
}
inline const wxString& wxGetTranslation(const wxString& str1,
                                        const wxString& str2,
                                        size_t n,
                                        const wxString& domain = wxEmptyString)
{
    wxLocale *pLoc = wxGetLocale();
    if (pLoc)
        return pLoc->GetString(str1, str2, n, domain);
    else
        // NB: this function returns reference to a string, so we have to keep
        //     a copy of it somewhere
        return n == 1
               ? wxLocale::GetUntranslatedString(str1)
               : wxLocale::GetUntranslatedString(str2);
}

#else // !wxUSE_INTL

// the macros should still be defined - otherwise compilation would fail

#if !defined(WXINTL_NO_GETTEXT_MACRO)
    #if !defined(_)
        #define _(s)                 (s)
    #endif
    #define wxPLURAL(sing, plur, n)  ((n) == 1 ? (sing) : (plur))
#endif

#define wxTRANSLATE(str) str

// NB: we use a template here in order to avoid using
//     wxLocale::GetUntranslatedString() above, which would be required if
//     we returned const wxString&; this way, the compiler should be able to
//     optimize wxGetTranslation() away

template<typename TString>
inline TString wxGetTranslation(TString str)
    { return str; }

template<typename TString, typename TDomain>
inline TString wxGetTranslation(TString str, TDomain WXUNUSED(domain))
    { return str; }

template<typename TString, typename TDomain>
inline TString wxGetTranslation(TString str1, TString str2, size_t n)
    { return n == 1 ? str1 : str2; }

template<typename TString, typename TDomain>
inline TString wxGetTranslation(TString str1, TString str2, size_t n,
                                TDomain WXUNUSED(domain))
    { return n == 1 ? str1 : str2; }

#endif // wxUSE_INTL/!wxUSE_INTL

// define this one just in case it occurs somewhere (instead of preferred
// wxTRANSLATE) too
#if !defined(WXINTL_NO_GETTEXT_MACRO)
    #if !defined(gettext_noop)
        #define gettext_noop(str) (str)
    #endif
    #if !defined(N_)
        #define N_(s)             (s)
    #endif
#endif

#endif // _WX_INTL_H_
