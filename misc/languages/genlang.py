#!/usr/bin/env python

#
# This script must be ran from misc/languages subdirectory.
#
# It generates wxLanguage enum (to be cut&pasted to include/wx/intl.h)
# and conversion tables (ditto to src/common/intl.cpp)
#

import string

def ReadTable():
    table = []
    f = open('langtabl.txt')
    for i in f.readlines():
        ispl = i.split()
        table.append((ispl[0], ispl[1], ispl[2], ispl[3], ispl[4], string.join(ispl[5:])))
    f.close()
    return table


def GenEnum(table):
   f = open('_wxlang.h', 'wt')
   f.write("""

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

""");
   knownLangs = []
   for i in table:
       if i[0] not in knownLangs:
          f.write('    %s,\n' % i[0])
          knownLangs.append(i[0])
   f.write("""
    /// For custom, user-defined languages.
    wxLANGUAGE_USER_DEFINED
};

// --- --- --- generated code ends here --- --- ---

""")
   f.close()


def GenTable(table):
   all_langs = []
   all_sublangs = []

   lngtable = ''
   ifdefs = ''

   for i in table:
       ican = '"%s"' % i[1]
       if ican == '"-"': ican = '""'
       ilang = i[2]
       if ilang == '-': ilang = '0'
       isublang = i[3]
       if isublang == '-': isublang = '0'
       if (i[4] == "LTR") :
           ilayout = "wxLayout_LeftToRight"
       elif (i[4] == "RTL"):
           ilayout = "wxLayout_RightToLeft"
       else:
           print "ERROR: Invalid value for the layout direction";
       lngtable += '   LNG(%-38s %-7s, %-15s, %-34s, %s, %s)\n' % \
                     ((i[0]+','), ican, ilang, isublang, ilayout, i[5])
       if ilang not in all_langs: all_langs.append(ilang)
       if isublang not in all_sublangs: all_sublangs.append(isublang)

   for s in all_langs:
       if s != '0':
           ifdefs += '#ifndef %s\n#define %s (0)\n#endif\n' % (s, s)
   for s in all_sublangs:
       if s != '0' and s != 'SUBLANG_DEFAULT':
           ifdefs += '#ifndef %s\n#define %s SUBLANG_DEFAULT\n#endif\n' % (s, s)

   f = open('_wxlang.cpp', 'wt')
   f.write("""
// --- --- --- generated code begins here --- --- ---

// This table is generated by misc/languages/genlang.py
// When making changes, please put them into misc/languages/langtabl.txt

#if !defined(__WIN32__) || defined(__WXMICROWIN__)

#define SETWINLANG(info,lang,sublang)

#else

#define SETWINLANG(info,lang,sublang) \\
    info.WinLang = lang, info.WinSublang = sublang;

%s

#endif // __WIN32__

#define LNG(wxlang, canonical, winlang, winsublang, layout, desc) \\
    info.Language = wxlang;                               \\
    info.CanonicalName = wxT(canonical);                  \\
    info.LayoutDirection = layout;                        \\
    info.Description = wxT(desc);                         \\
    SETWINLANG(info, winlang, winsublang)                 \\
    AddLanguage(info);

void wxLocale::InitLanguagesDB()
{
   wxLanguageInfo info;
   wxStringTokenizer tkn;

%s
}
#undef LNG

// --- --- --- generated code ends here --- --- ---

""" % (ifdefs, lngtable))
   f.close()





table = ReadTable()
GenEnum(table)          # the enum is used also (thanks to doxygen) in the docs
GenTable(table)
