## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by ./distrib/build_renamers.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx._gdi

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxGDIObject = wx._gdi.GDIObject
wxGDIObjectPtr = wx._gdi.GDIObjectPtr
wxColour = wx._gdi.Colour
wxColourPtr = wx._gdi.ColourPtr
wxNamedColour = wx._gdi.NamedColour
wxColourRGB = wx._gdi.ColourRGB
wxPalette = wx._gdi.Palette
wxPalettePtr = wx._gdi.PalettePtr
wxPen = wx._gdi.Pen
wxPenPtr = wx._gdi.PenPtr
wxBrush = wx._gdi.Brush
wxBrushPtr = wx._gdi.BrushPtr
wxBitmap = wx._gdi.Bitmap
wxBitmapPtr = wx._gdi.BitmapPtr
wxEmptyBitmap = wx._gdi.EmptyBitmap
wxBitmapFromIcon = wx._gdi.BitmapFromIcon
wxBitmapFromImage = wx._gdi.BitmapFromImage
wxBitmapFromXPMData = wx._gdi.BitmapFromXPMData
wxBitmapFromBits = wx._gdi.BitmapFromBits
wxMask = wx._gdi.Mask
wxMaskPtr = wx._gdi.MaskPtr
wxIcon = wx._gdi.Icon
wxIconPtr = wx._gdi.IconPtr
wxEmptyIcon = wx._gdi.EmptyIcon
wxIconFromLocation = wx._gdi.IconFromLocation
wxIconFromBitmap = wx._gdi.IconFromBitmap
wxIconFromXPMData = wx._gdi.IconFromXPMData
wxIconLocation = wx._gdi.IconLocation
wxIconLocationPtr = wx._gdi.IconLocationPtr
wxIconBundle = wx._gdi.IconBundle
wxIconBundlePtr = wx._gdi.IconBundlePtr
wxIconBundleFromFile = wx._gdi.IconBundleFromFile
wxIconBundleFromIcon = wx._gdi.IconBundleFromIcon
wxCursor = wx._gdi.Cursor
wxCursorPtr = wx._gdi.CursorPtr
wxStockCursor = wx._gdi.StockCursor
wxCursorFromImage = wx._gdi.CursorFromImage
wxOutRegion = wx._gdi.OutRegion
wxPartRegion = wx._gdi.PartRegion
wxInRegion = wx._gdi.InRegion
wxRegion = wx._gdi.Region
wxRegionPtr = wx._gdi.RegionPtr
wxRegionFromBitmap = wx._gdi.RegionFromBitmap
wxRegionFromPoints = wx._gdi.RegionFromPoints
wxRegionIterator = wx._gdi.RegionIterator
wxRegionIteratorPtr = wx._gdi.RegionIteratorPtr
wxFONTFAMILY_DEFAULT = wx._gdi.FONTFAMILY_DEFAULT
wxFONTFAMILY_DECORATIVE = wx._gdi.FONTFAMILY_DECORATIVE
wxFONTFAMILY_ROMAN = wx._gdi.FONTFAMILY_ROMAN
wxFONTFAMILY_SCRIPT = wx._gdi.FONTFAMILY_SCRIPT
wxFONTFAMILY_SWISS = wx._gdi.FONTFAMILY_SWISS
wxFONTFAMILY_MODERN = wx._gdi.FONTFAMILY_MODERN
wxFONTFAMILY_TELETYPE = wx._gdi.FONTFAMILY_TELETYPE
wxFONTFAMILY_MAX = wx._gdi.FONTFAMILY_MAX
wxFONTFAMILY_UNKNOWN = wx._gdi.FONTFAMILY_UNKNOWN
wxFONTSTYLE_NORMAL = wx._gdi.FONTSTYLE_NORMAL
wxFONTSTYLE_ITALIC = wx._gdi.FONTSTYLE_ITALIC
wxFONTSTYLE_SLANT = wx._gdi.FONTSTYLE_SLANT
wxFONTSTYLE_MAX = wx._gdi.FONTSTYLE_MAX
wxFONTWEIGHT_NORMAL = wx._gdi.FONTWEIGHT_NORMAL
wxFONTWEIGHT_LIGHT = wx._gdi.FONTWEIGHT_LIGHT
wxFONTWEIGHT_BOLD = wx._gdi.FONTWEIGHT_BOLD
wxFONTWEIGHT_MAX = wx._gdi.FONTWEIGHT_MAX
wxFONTFLAG_DEFAULT = wx._gdi.FONTFLAG_DEFAULT
wxFONTFLAG_ITALIC = wx._gdi.FONTFLAG_ITALIC
wxFONTFLAG_SLANT = wx._gdi.FONTFLAG_SLANT
wxFONTFLAG_LIGHT = wx._gdi.FONTFLAG_LIGHT
wxFONTFLAG_BOLD = wx._gdi.FONTFLAG_BOLD
wxFONTFLAG_ANTIALIASED = wx._gdi.FONTFLAG_ANTIALIASED
wxFONTFLAG_NOT_ANTIALIASED = wx._gdi.FONTFLAG_NOT_ANTIALIASED
wxFONTFLAG_UNDERLINED = wx._gdi.FONTFLAG_UNDERLINED
wxFONTFLAG_STRIKETHROUGH = wx._gdi.FONTFLAG_STRIKETHROUGH
wxFONTFLAG_MASK = wx._gdi.FONTFLAG_MASK
wxFONTENCODING_SYSTEM = wx._gdi.FONTENCODING_SYSTEM
wxFONTENCODING_DEFAULT = wx._gdi.FONTENCODING_DEFAULT
wxFONTENCODING_ISO8859_1 = wx._gdi.FONTENCODING_ISO8859_1
wxFONTENCODING_ISO8859_2 = wx._gdi.FONTENCODING_ISO8859_2
wxFONTENCODING_ISO8859_3 = wx._gdi.FONTENCODING_ISO8859_3
wxFONTENCODING_ISO8859_4 = wx._gdi.FONTENCODING_ISO8859_4
wxFONTENCODING_ISO8859_5 = wx._gdi.FONTENCODING_ISO8859_5
wxFONTENCODING_ISO8859_6 = wx._gdi.FONTENCODING_ISO8859_6
wxFONTENCODING_ISO8859_7 = wx._gdi.FONTENCODING_ISO8859_7
wxFONTENCODING_ISO8859_8 = wx._gdi.FONTENCODING_ISO8859_8
wxFONTENCODING_ISO8859_9 = wx._gdi.FONTENCODING_ISO8859_9
wxFONTENCODING_ISO8859_10 = wx._gdi.FONTENCODING_ISO8859_10
wxFONTENCODING_ISO8859_11 = wx._gdi.FONTENCODING_ISO8859_11
wxFONTENCODING_ISO8859_12 = wx._gdi.FONTENCODING_ISO8859_12
wxFONTENCODING_ISO8859_13 = wx._gdi.FONTENCODING_ISO8859_13
wxFONTENCODING_ISO8859_14 = wx._gdi.FONTENCODING_ISO8859_14
wxFONTENCODING_ISO8859_15 = wx._gdi.FONTENCODING_ISO8859_15
wxFONTENCODING_ISO8859_MAX = wx._gdi.FONTENCODING_ISO8859_MAX
wxFONTENCODING_KOI8 = wx._gdi.FONTENCODING_KOI8
wxFONTENCODING_KOI8_U = wx._gdi.FONTENCODING_KOI8_U
wxFONTENCODING_ALTERNATIVE = wx._gdi.FONTENCODING_ALTERNATIVE
wxFONTENCODING_BULGARIAN = wx._gdi.FONTENCODING_BULGARIAN
wxFONTENCODING_CP437 = wx._gdi.FONTENCODING_CP437
wxFONTENCODING_CP850 = wx._gdi.FONTENCODING_CP850
wxFONTENCODING_CP852 = wx._gdi.FONTENCODING_CP852
wxFONTENCODING_CP855 = wx._gdi.FONTENCODING_CP855
wxFONTENCODING_CP866 = wx._gdi.FONTENCODING_CP866
wxFONTENCODING_CP874 = wx._gdi.FONTENCODING_CP874
wxFONTENCODING_CP932 = wx._gdi.FONTENCODING_CP932
wxFONTENCODING_CP936 = wx._gdi.FONTENCODING_CP936
wxFONTENCODING_CP949 = wx._gdi.FONTENCODING_CP949
wxFONTENCODING_CP950 = wx._gdi.FONTENCODING_CP950
wxFONTENCODING_CP1250 = wx._gdi.FONTENCODING_CP1250
wxFONTENCODING_CP1251 = wx._gdi.FONTENCODING_CP1251
wxFONTENCODING_CP1252 = wx._gdi.FONTENCODING_CP1252
wxFONTENCODING_CP1253 = wx._gdi.FONTENCODING_CP1253
wxFONTENCODING_CP1254 = wx._gdi.FONTENCODING_CP1254
wxFONTENCODING_CP1255 = wx._gdi.FONTENCODING_CP1255
wxFONTENCODING_CP1256 = wx._gdi.FONTENCODING_CP1256
wxFONTENCODING_CP1257 = wx._gdi.FONTENCODING_CP1257
wxFONTENCODING_CP12_MAX = wx._gdi.FONTENCODING_CP12_MAX
wxFONTENCODING_UTF7 = wx._gdi.FONTENCODING_UTF7
wxFONTENCODING_UTF8 = wx._gdi.FONTENCODING_UTF8
wxFONTENCODING_EUC_JP = wx._gdi.FONTENCODING_EUC_JP
wxFONTENCODING_UTF16BE = wx._gdi.FONTENCODING_UTF16BE
wxFONTENCODING_UTF16LE = wx._gdi.FONTENCODING_UTF16LE
wxFONTENCODING_UTF32BE = wx._gdi.FONTENCODING_UTF32BE
wxFONTENCODING_UTF32LE = wx._gdi.FONTENCODING_UTF32LE
wxFONTENCODING_MACROMAN = wx._gdi.FONTENCODING_MACROMAN
wxFONTENCODING_MACJAPANESE = wx._gdi.FONTENCODING_MACJAPANESE
wxFONTENCODING_MACCHINESETRAD = wx._gdi.FONTENCODING_MACCHINESETRAD
wxFONTENCODING_MACKOREAN = wx._gdi.FONTENCODING_MACKOREAN
wxFONTENCODING_MACARABIC = wx._gdi.FONTENCODING_MACARABIC
wxFONTENCODING_MACHEBREW = wx._gdi.FONTENCODING_MACHEBREW
wxFONTENCODING_MACGREEK = wx._gdi.FONTENCODING_MACGREEK
wxFONTENCODING_MACCYRILLIC = wx._gdi.FONTENCODING_MACCYRILLIC
wxFONTENCODING_MACDEVANAGARI = wx._gdi.FONTENCODING_MACDEVANAGARI
wxFONTENCODING_MACGURMUKHI = wx._gdi.FONTENCODING_MACGURMUKHI
wxFONTENCODING_MACGUJARATI = wx._gdi.FONTENCODING_MACGUJARATI
wxFONTENCODING_MACORIYA = wx._gdi.FONTENCODING_MACORIYA
wxFONTENCODING_MACBENGALI = wx._gdi.FONTENCODING_MACBENGALI
wxFONTENCODING_MACTAMIL = wx._gdi.FONTENCODING_MACTAMIL
wxFONTENCODING_MACTELUGU = wx._gdi.FONTENCODING_MACTELUGU
wxFONTENCODING_MACKANNADA = wx._gdi.FONTENCODING_MACKANNADA
wxFONTENCODING_MACMALAJALAM = wx._gdi.FONTENCODING_MACMALAJALAM
wxFONTENCODING_MACSINHALESE = wx._gdi.FONTENCODING_MACSINHALESE
wxFONTENCODING_MACBURMESE = wx._gdi.FONTENCODING_MACBURMESE
wxFONTENCODING_MACKHMER = wx._gdi.FONTENCODING_MACKHMER
wxFONTENCODING_MACTHAI = wx._gdi.FONTENCODING_MACTHAI
wxFONTENCODING_MACLAOTIAN = wx._gdi.FONTENCODING_MACLAOTIAN
wxFONTENCODING_MACGEORGIAN = wx._gdi.FONTENCODING_MACGEORGIAN
wxFONTENCODING_MACARMENIAN = wx._gdi.FONTENCODING_MACARMENIAN
wxFONTENCODING_MACCHINESESIMP = wx._gdi.FONTENCODING_MACCHINESESIMP
wxFONTENCODING_MACTIBETAN = wx._gdi.FONTENCODING_MACTIBETAN
wxFONTENCODING_MACMONGOLIAN = wx._gdi.FONTENCODING_MACMONGOLIAN
wxFONTENCODING_MACETHIOPIC = wx._gdi.FONTENCODING_MACETHIOPIC
wxFONTENCODING_MACCENTRALEUR = wx._gdi.FONTENCODING_MACCENTRALEUR
wxFONTENCODING_MACVIATNAMESE = wx._gdi.FONTENCODING_MACVIATNAMESE
wxFONTENCODING_MACARABICEXT = wx._gdi.FONTENCODING_MACARABICEXT
wxFONTENCODING_MACSYMBOL = wx._gdi.FONTENCODING_MACSYMBOL
wxFONTENCODING_MACDINGBATS = wx._gdi.FONTENCODING_MACDINGBATS
wxFONTENCODING_MACTURKISH = wx._gdi.FONTENCODING_MACTURKISH
wxFONTENCODING_MACCROATIAN = wx._gdi.FONTENCODING_MACCROATIAN
wxFONTENCODING_MACICELANDIC = wx._gdi.FONTENCODING_MACICELANDIC
wxFONTENCODING_MACROMANIAN = wx._gdi.FONTENCODING_MACROMANIAN
wxFONTENCODING_MACCELTIC = wx._gdi.FONTENCODING_MACCELTIC
wxFONTENCODING_MACGAELIC = wx._gdi.FONTENCODING_MACGAELIC
wxFONTENCODING_MACKEYBOARD = wx._gdi.FONTENCODING_MACKEYBOARD
wxFONTENCODING_MACMIN = wx._gdi.FONTENCODING_MACMIN
wxFONTENCODING_MACMAX = wx._gdi.FONTENCODING_MACMAX
wxFONTENCODING_MAX = wx._gdi.FONTENCODING_MAX
wxFONTENCODING_UTF16 = wx._gdi.FONTENCODING_UTF16
wxFONTENCODING_UTF32 = wx._gdi.FONTENCODING_UTF32
wxFONTENCODING_UNICODE = wx._gdi.FONTENCODING_UNICODE
wxFONTENCODING_GB2312 = wx._gdi.FONTENCODING_GB2312
wxFONTENCODING_BIG5 = wx._gdi.FONTENCODING_BIG5
wxFONTENCODING_SHIFT_JIS = wx._gdi.FONTENCODING_SHIFT_JIS
wxNativeFontInfo = wx._gdi.NativeFontInfo
wxNativeFontInfoPtr = wx._gdi.NativeFontInfoPtr
wxNativeEncodingInfo = wx._gdi.NativeEncodingInfo
wxNativeEncodingInfoPtr = wx._gdi.NativeEncodingInfoPtr
wxGetNativeFontEncoding = wx._gdi.GetNativeFontEncoding
wxTestFontEncoding = wx._gdi.TestFontEncoding
wxFontMapper = wx._gdi.FontMapper
wxFontMapperPtr = wx._gdi.FontMapperPtr
wxFontMapper_Get = wx._gdi.FontMapper_Get
wxFontMapper_Set = wx._gdi.FontMapper_Set
wxFontMapper_GetSupportedEncodingsCount = wx._gdi.FontMapper_GetSupportedEncodingsCount
wxFontMapper_GetEncoding = wx._gdi.FontMapper_GetEncoding
wxFontMapper_GetEncodingName = wx._gdi.FontMapper_GetEncodingName
wxFontMapper_GetEncodingDescription = wx._gdi.FontMapper_GetEncodingDescription
wxFontMapper_GetEncodingFromName = wx._gdi.FontMapper_GetEncodingFromName
wxFontMapper_GetDefaultConfigPath = wx._gdi.FontMapper_GetDefaultConfigPath
wxFont = wx._gdi.Font
wxFontPtr = wx._gdi.FontPtr
wxFontFromNativeInfo = wx._gdi.FontFromNativeInfo
wxFontFromNativeInfoString = wx._gdi.FontFromNativeInfoString
wxFont2 = wx._gdi.Font2
wxFont_GetDefaultEncoding = wx._gdi.Font_GetDefaultEncoding
wxFont_SetDefaultEncoding = wx._gdi.Font_SetDefaultEncoding
wxFontEnumerator = wx._gdi.FontEnumerator
wxFontEnumeratorPtr = wx._gdi.FontEnumeratorPtr
wxLANGUAGE_DEFAULT = wx._gdi.LANGUAGE_DEFAULT
wxLANGUAGE_UNKNOWN = wx._gdi.LANGUAGE_UNKNOWN
wxLANGUAGE_ABKHAZIAN = wx._gdi.LANGUAGE_ABKHAZIAN
wxLANGUAGE_AFAR = wx._gdi.LANGUAGE_AFAR
wxLANGUAGE_AFRIKAANS = wx._gdi.LANGUAGE_AFRIKAANS
wxLANGUAGE_ALBANIAN = wx._gdi.LANGUAGE_ALBANIAN
wxLANGUAGE_AMHARIC = wx._gdi.LANGUAGE_AMHARIC
wxLANGUAGE_ARABIC = wx._gdi.LANGUAGE_ARABIC
wxLANGUAGE_ARABIC_ALGERIA = wx._gdi.LANGUAGE_ARABIC_ALGERIA
wxLANGUAGE_ARABIC_BAHRAIN = wx._gdi.LANGUAGE_ARABIC_BAHRAIN
wxLANGUAGE_ARABIC_EGYPT = wx._gdi.LANGUAGE_ARABIC_EGYPT
wxLANGUAGE_ARABIC_IRAQ = wx._gdi.LANGUAGE_ARABIC_IRAQ
wxLANGUAGE_ARABIC_JORDAN = wx._gdi.LANGUAGE_ARABIC_JORDAN
wxLANGUAGE_ARABIC_KUWAIT = wx._gdi.LANGUAGE_ARABIC_KUWAIT
wxLANGUAGE_ARABIC_LEBANON = wx._gdi.LANGUAGE_ARABIC_LEBANON
wxLANGUAGE_ARABIC_LIBYA = wx._gdi.LANGUAGE_ARABIC_LIBYA
wxLANGUAGE_ARABIC_MOROCCO = wx._gdi.LANGUAGE_ARABIC_MOROCCO
wxLANGUAGE_ARABIC_OMAN = wx._gdi.LANGUAGE_ARABIC_OMAN
wxLANGUAGE_ARABIC_QATAR = wx._gdi.LANGUAGE_ARABIC_QATAR
wxLANGUAGE_ARABIC_SAUDI_ARABIA = wx._gdi.LANGUAGE_ARABIC_SAUDI_ARABIA
wxLANGUAGE_ARABIC_SUDAN = wx._gdi.LANGUAGE_ARABIC_SUDAN
wxLANGUAGE_ARABIC_SYRIA = wx._gdi.LANGUAGE_ARABIC_SYRIA
wxLANGUAGE_ARABIC_TUNISIA = wx._gdi.LANGUAGE_ARABIC_TUNISIA
wxLANGUAGE_ARABIC_UAE = wx._gdi.LANGUAGE_ARABIC_UAE
wxLANGUAGE_ARABIC_YEMEN = wx._gdi.LANGUAGE_ARABIC_YEMEN
wxLANGUAGE_ARMENIAN = wx._gdi.LANGUAGE_ARMENIAN
wxLANGUAGE_ASSAMESE = wx._gdi.LANGUAGE_ASSAMESE
wxLANGUAGE_AYMARA = wx._gdi.LANGUAGE_AYMARA
wxLANGUAGE_AZERI = wx._gdi.LANGUAGE_AZERI
wxLANGUAGE_AZERI_CYRILLIC = wx._gdi.LANGUAGE_AZERI_CYRILLIC
wxLANGUAGE_AZERI_LATIN = wx._gdi.LANGUAGE_AZERI_LATIN
wxLANGUAGE_BASHKIR = wx._gdi.LANGUAGE_BASHKIR
wxLANGUAGE_BASQUE = wx._gdi.LANGUAGE_BASQUE
wxLANGUAGE_BELARUSIAN = wx._gdi.LANGUAGE_BELARUSIAN
wxLANGUAGE_BENGALI = wx._gdi.LANGUAGE_BENGALI
wxLANGUAGE_BHUTANI = wx._gdi.LANGUAGE_BHUTANI
wxLANGUAGE_BIHARI = wx._gdi.LANGUAGE_BIHARI
wxLANGUAGE_BISLAMA = wx._gdi.LANGUAGE_BISLAMA
wxLANGUAGE_BRETON = wx._gdi.LANGUAGE_BRETON
wxLANGUAGE_BULGARIAN = wx._gdi.LANGUAGE_BULGARIAN
wxLANGUAGE_BURMESE = wx._gdi.LANGUAGE_BURMESE
wxLANGUAGE_CAMBODIAN = wx._gdi.LANGUAGE_CAMBODIAN
wxLANGUAGE_CATALAN = wx._gdi.LANGUAGE_CATALAN
wxLANGUAGE_CHINESE = wx._gdi.LANGUAGE_CHINESE
wxLANGUAGE_CHINESE_SIMPLIFIED = wx._gdi.LANGUAGE_CHINESE_SIMPLIFIED
wxLANGUAGE_CHINESE_TRADITIONAL = wx._gdi.LANGUAGE_CHINESE_TRADITIONAL
wxLANGUAGE_CHINESE_HONGKONG = wx._gdi.LANGUAGE_CHINESE_HONGKONG
wxLANGUAGE_CHINESE_MACAU = wx._gdi.LANGUAGE_CHINESE_MACAU
wxLANGUAGE_CHINESE_SINGAPORE = wx._gdi.LANGUAGE_CHINESE_SINGAPORE
wxLANGUAGE_CHINESE_TAIWAN = wx._gdi.LANGUAGE_CHINESE_TAIWAN
wxLANGUAGE_CORSICAN = wx._gdi.LANGUAGE_CORSICAN
wxLANGUAGE_CROATIAN = wx._gdi.LANGUAGE_CROATIAN
wxLANGUAGE_CZECH = wx._gdi.LANGUAGE_CZECH
wxLANGUAGE_DANISH = wx._gdi.LANGUAGE_DANISH
wxLANGUAGE_DUTCH = wx._gdi.LANGUAGE_DUTCH
wxLANGUAGE_DUTCH_BELGIAN = wx._gdi.LANGUAGE_DUTCH_BELGIAN
wxLANGUAGE_ENGLISH = wx._gdi.LANGUAGE_ENGLISH
wxLANGUAGE_ENGLISH_UK = wx._gdi.LANGUAGE_ENGLISH_UK
wxLANGUAGE_ENGLISH_US = wx._gdi.LANGUAGE_ENGLISH_US
wxLANGUAGE_ENGLISH_AUSTRALIA = wx._gdi.LANGUAGE_ENGLISH_AUSTRALIA
wxLANGUAGE_ENGLISH_BELIZE = wx._gdi.LANGUAGE_ENGLISH_BELIZE
wxLANGUAGE_ENGLISH_BOTSWANA = wx._gdi.LANGUAGE_ENGLISH_BOTSWANA
wxLANGUAGE_ENGLISH_CANADA = wx._gdi.LANGUAGE_ENGLISH_CANADA
wxLANGUAGE_ENGLISH_CARIBBEAN = wx._gdi.LANGUAGE_ENGLISH_CARIBBEAN
wxLANGUAGE_ENGLISH_DENMARK = wx._gdi.LANGUAGE_ENGLISH_DENMARK
wxLANGUAGE_ENGLISH_EIRE = wx._gdi.LANGUAGE_ENGLISH_EIRE
wxLANGUAGE_ENGLISH_JAMAICA = wx._gdi.LANGUAGE_ENGLISH_JAMAICA
wxLANGUAGE_ENGLISH_NEW_ZEALAND = wx._gdi.LANGUAGE_ENGLISH_NEW_ZEALAND
wxLANGUAGE_ENGLISH_PHILIPPINES = wx._gdi.LANGUAGE_ENGLISH_PHILIPPINES
wxLANGUAGE_ENGLISH_SOUTH_AFRICA = wx._gdi.LANGUAGE_ENGLISH_SOUTH_AFRICA
wxLANGUAGE_ENGLISH_TRINIDAD = wx._gdi.LANGUAGE_ENGLISH_TRINIDAD
wxLANGUAGE_ENGLISH_ZIMBABWE = wx._gdi.LANGUAGE_ENGLISH_ZIMBABWE
wxLANGUAGE_ESPERANTO = wx._gdi.LANGUAGE_ESPERANTO
wxLANGUAGE_ESTONIAN = wx._gdi.LANGUAGE_ESTONIAN
wxLANGUAGE_FAEROESE = wx._gdi.LANGUAGE_FAEROESE
wxLANGUAGE_FARSI = wx._gdi.LANGUAGE_FARSI
wxLANGUAGE_FIJI = wx._gdi.LANGUAGE_FIJI
wxLANGUAGE_FINNISH = wx._gdi.LANGUAGE_FINNISH
wxLANGUAGE_FRENCH = wx._gdi.LANGUAGE_FRENCH
wxLANGUAGE_FRENCH_BELGIAN = wx._gdi.LANGUAGE_FRENCH_BELGIAN
wxLANGUAGE_FRENCH_CANADIAN = wx._gdi.LANGUAGE_FRENCH_CANADIAN
wxLANGUAGE_FRENCH_LUXEMBOURG = wx._gdi.LANGUAGE_FRENCH_LUXEMBOURG
wxLANGUAGE_FRENCH_MONACO = wx._gdi.LANGUAGE_FRENCH_MONACO
wxLANGUAGE_FRENCH_SWISS = wx._gdi.LANGUAGE_FRENCH_SWISS
wxLANGUAGE_FRISIAN = wx._gdi.LANGUAGE_FRISIAN
wxLANGUAGE_GALICIAN = wx._gdi.LANGUAGE_GALICIAN
wxLANGUAGE_GEORGIAN = wx._gdi.LANGUAGE_GEORGIAN
wxLANGUAGE_GERMAN = wx._gdi.LANGUAGE_GERMAN
wxLANGUAGE_GERMAN_AUSTRIAN = wx._gdi.LANGUAGE_GERMAN_AUSTRIAN
wxLANGUAGE_GERMAN_BELGIUM = wx._gdi.LANGUAGE_GERMAN_BELGIUM
wxLANGUAGE_GERMAN_LIECHTENSTEIN = wx._gdi.LANGUAGE_GERMAN_LIECHTENSTEIN
wxLANGUAGE_GERMAN_LUXEMBOURG = wx._gdi.LANGUAGE_GERMAN_LUXEMBOURG
wxLANGUAGE_GERMAN_SWISS = wx._gdi.LANGUAGE_GERMAN_SWISS
wxLANGUAGE_GREEK = wx._gdi.LANGUAGE_GREEK
wxLANGUAGE_GREENLANDIC = wx._gdi.LANGUAGE_GREENLANDIC
wxLANGUAGE_GUARANI = wx._gdi.LANGUAGE_GUARANI
wxLANGUAGE_GUJARATI = wx._gdi.LANGUAGE_GUJARATI
wxLANGUAGE_HAUSA = wx._gdi.LANGUAGE_HAUSA
wxLANGUAGE_HEBREW = wx._gdi.LANGUAGE_HEBREW
wxLANGUAGE_HINDI = wx._gdi.LANGUAGE_HINDI
wxLANGUAGE_HUNGARIAN = wx._gdi.LANGUAGE_HUNGARIAN
wxLANGUAGE_ICELANDIC = wx._gdi.LANGUAGE_ICELANDIC
wxLANGUAGE_INDONESIAN = wx._gdi.LANGUAGE_INDONESIAN
wxLANGUAGE_INTERLINGUA = wx._gdi.LANGUAGE_INTERLINGUA
wxLANGUAGE_INTERLINGUE = wx._gdi.LANGUAGE_INTERLINGUE
wxLANGUAGE_INUKTITUT = wx._gdi.LANGUAGE_INUKTITUT
wxLANGUAGE_INUPIAK = wx._gdi.LANGUAGE_INUPIAK
wxLANGUAGE_IRISH = wx._gdi.LANGUAGE_IRISH
wxLANGUAGE_ITALIAN = wx._gdi.LANGUAGE_ITALIAN
wxLANGUAGE_ITALIAN_SWISS = wx._gdi.LANGUAGE_ITALIAN_SWISS
wxLANGUAGE_JAPANESE = wx._gdi.LANGUAGE_JAPANESE
wxLANGUAGE_JAVANESE = wx._gdi.LANGUAGE_JAVANESE
wxLANGUAGE_KANNADA = wx._gdi.LANGUAGE_KANNADA
wxLANGUAGE_KASHMIRI = wx._gdi.LANGUAGE_KASHMIRI
wxLANGUAGE_KASHMIRI_INDIA = wx._gdi.LANGUAGE_KASHMIRI_INDIA
wxLANGUAGE_KAZAKH = wx._gdi.LANGUAGE_KAZAKH
wxLANGUAGE_KERNEWEK = wx._gdi.LANGUAGE_KERNEWEK
wxLANGUAGE_KINYARWANDA = wx._gdi.LANGUAGE_KINYARWANDA
wxLANGUAGE_KIRGHIZ = wx._gdi.LANGUAGE_KIRGHIZ
wxLANGUAGE_KIRUNDI = wx._gdi.LANGUAGE_KIRUNDI
wxLANGUAGE_KONKANI = wx._gdi.LANGUAGE_KONKANI
wxLANGUAGE_KOREAN = wx._gdi.LANGUAGE_KOREAN
wxLANGUAGE_KURDISH = wx._gdi.LANGUAGE_KURDISH
wxLANGUAGE_LAOTHIAN = wx._gdi.LANGUAGE_LAOTHIAN
wxLANGUAGE_LATIN = wx._gdi.LANGUAGE_LATIN
wxLANGUAGE_LATVIAN = wx._gdi.LANGUAGE_LATVIAN
wxLANGUAGE_LINGALA = wx._gdi.LANGUAGE_LINGALA
wxLANGUAGE_LITHUANIAN = wx._gdi.LANGUAGE_LITHUANIAN
wxLANGUAGE_MACEDONIAN = wx._gdi.LANGUAGE_MACEDONIAN
wxLANGUAGE_MALAGASY = wx._gdi.LANGUAGE_MALAGASY
wxLANGUAGE_MALAY = wx._gdi.LANGUAGE_MALAY
wxLANGUAGE_MALAYALAM = wx._gdi.LANGUAGE_MALAYALAM
wxLANGUAGE_MALAY_BRUNEI_DARUSSALAM = wx._gdi.LANGUAGE_MALAY_BRUNEI_DARUSSALAM
wxLANGUAGE_MALAY_MALAYSIA = wx._gdi.LANGUAGE_MALAY_MALAYSIA
wxLANGUAGE_MALTESE = wx._gdi.LANGUAGE_MALTESE
wxLANGUAGE_MANIPURI = wx._gdi.LANGUAGE_MANIPURI
wxLANGUAGE_MAORI = wx._gdi.LANGUAGE_MAORI
wxLANGUAGE_MARATHI = wx._gdi.LANGUAGE_MARATHI
wxLANGUAGE_MOLDAVIAN = wx._gdi.LANGUAGE_MOLDAVIAN
wxLANGUAGE_MONGOLIAN = wx._gdi.LANGUAGE_MONGOLIAN
wxLANGUAGE_NAURU = wx._gdi.LANGUAGE_NAURU
wxLANGUAGE_NEPALI = wx._gdi.LANGUAGE_NEPALI
wxLANGUAGE_NEPALI_INDIA = wx._gdi.LANGUAGE_NEPALI_INDIA
wxLANGUAGE_NORWEGIAN_BOKMAL = wx._gdi.LANGUAGE_NORWEGIAN_BOKMAL
wxLANGUAGE_NORWEGIAN_NYNORSK = wx._gdi.LANGUAGE_NORWEGIAN_NYNORSK
wxLANGUAGE_OCCITAN = wx._gdi.LANGUAGE_OCCITAN
wxLANGUAGE_ORIYA = wx._gdi.LANGUAGE_ORIYA
wxLANGUAGE_OROMO = wx._gdi.LANGUAGE_OROMO
wxLANGUAGE_PASHTO = wx._gdi.LANGUAGE_PASHTO
wxLANGUAGE_POLISH = wx._gdi.LANGUAGE_POLISH
wxLANGUAGE_PORTUGUESE = wx._gdi.LANGUAGE_PORTUGUESE
wxLANGUAGE_PORTUGUESE_BRAZILIAN = wx._gdi.LANGUAGE_PORTUGUESE_BRAZILIAN
wxLANGUAGE_PUNJABI = wx._gdi.LANGUAGE_PUNJABI
wxLANGUAGE_QUECHUA = wx._gdi.LANGUAGE_QUECHUA
wxLANGUAGE_RHAETO_ROMANCE = wx._gdi.LANGUAGE_RHAETO_ROMANCE
wxLANGUAGE_ROMANIAN = wx._gdi.LANGUAGE_ROMANIAN
wxLANGUAGE_RUSSIAN = wx._gdi.LANGUAGE_RUSSIAN
wxLANGUAGE_RUSSIAN_UKRAINE = wx._gdi.LANGUAGE_RUSSIAN_UKRAINE
wxLANGUAGE_SAMOAN = wx._gdi.LANGUAGE_SAMOAN
wxLANGUAGE_SANGHO = wx._gdi.LANGUAGE_SANGHO
wxLANGUAGE_SANSKRIT = wx._gdi.LANGUAGE_SANSKRIT
wxLANGUAGE_SCOTS_GAELIC = wx._gdi.LANGUAGE_SCOTS_GAELIC
wxLANGUAGE_SERBIAN = wx._gdi.LANGUAGE_SERBIAN
wxLANGUAGE_SERBIAN_CYRILLIC = wx._gdi.LANGUAGE_SERBIAN_CYRILLIC
wxLANGUAGE_SERBIAN_LATIN = wx._gdi.LANGUAGE_SERBIAN_LATIN
wxLANGUAGE_SERBO_CROATIAN = wx._gdi.LANGUAGE_SERBO_CROATIAN
wxLANGUAGE_SESOTHO = wx._gdi.LANGUAGE_SESOTHO
wxLANGUAGE_SETSWANA = wx._gdi.LANGUAGE_SETSWANA
wxLANGUAGE_SHONA = wx._gdi.LANGUAGE_SHONA
wxLANGUAGE_SINDHI = wx._gdi.LANGUAGE_SINDHI
wxLANGUAGE_SINHALESE = wx._gdi.LANGUAGE_SINHALESE
wxLANGUAGE_SISWATI = wx._gdi.LANGUAGE_SISWATI
wxLANGUAGE_SLOVAK = wx._gdi.LANGUAGE_SLOVAK
wxLANGUAGE_SLOVENIAN = wx._gdi.LANGUAGE_SLOVENIAN
wxLANGUAGE_SOMALI = wx._gdi.LANGUAGE_SOMALI
wxLANGUAGE_SPANISH = wx._gdi.LANGUAGE_SPANISH
wxLANGUAGE_SPANISH_ARGENTINA = wx._gdi.LANGUAGE_SPANISH_ARGENTINA
wxLANGUAGE_SPANISH_BOLIVIA = wx._gdi.LANGUAGE_SPANISH_BOLIVIA
wxLANGUAGE_SPANISH_CHILE = wx._gdi.LANGUAGE_SPANISH_CHILE
wxLANGUAGE_SPANISH_COLOMBIA = wx._gdi.LANGUAGE_SPANISH_COLOMBIA
wxLANGUAGE_SPANISH_COSTA_RICA = wx._gdi.LANGUAGE_SPANISH_COSTA_RICA
wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC = wx._gdi.LANGUAGE_SPANISH_DOMINICAN_REPUBLIC
wxLANGUAGE_SPANISH_ECUADOR = wx._gdi.LANGUAGE_SPANISH_ECUADOR
wxLANGUAGE_SPANISH_EL_SALVADOR = wx._gdi.LANGUAGE_SPANISH_EL_SALVADOR
wxLANGUAGE_SPANISH_GUATEMALA = wx._gdi.LANGUAGE_SPANISH_GUATEMALA
wxLANGUAGE_SPANISH_HONDURAS = wx._gdi.LANGUAGE_SPANISH_HONDURAS
wxLANGUAGE_SPANISH_MEXICAN = wx._gdi.LANGUAGE_SPANISH_MEXICAN
wxLANGUAGE_SPANISH_MODERN = wx._gdi.LANGUAGE_SPANISH_MODERN
wxLANGUAGE_SPANISH_NICARAGUA = wx._gdi.LANGUAGE_SPANISH_NICARAGUA
wxLANGUAGE_SPANISH_PANAMA = wx._gdi.LANGUAGE_SPANISH_PANAMA
wxLANGUAGE_SPANISH_PARAGUAY = wx._gdi.LANGUAGE_SPANISH_PARAGUAY
wxLANGUAGE_SPANISH_PERU = wx._gdi.LANGUAGE_SPANISH_PERU
wxLANGUAGE_SPANISH_PUERTO_RICO = wx._gdi.LANGUAGE_SPANISH_PUERTO_RICO
wxLANGUAGE_SPANISH_URUGUAY = wx._gdi.LANGUAGE_SPANISH_URUGUAY
wxLANGUAGE_SPANISH_US = wx._gdi.LANGUAGE_SPANISH_US
wxLANGUAGE_SPANISH_VENEZUELA = wx._gdi.LANGUAGE_SPANISH_VENEZUELA
wxLANGUAGE_SUNDANESE = wx._gdi.LANGUAGE_SUNDANESE
wxLANGUAGE_SWAHILI = wx._gdi.LANGUAGE_SWAHILI
wxLANGUAGE_SWEDISH = wx._gdi.LANGUAGE_SWEDISH
wxLANGUAGE_SWEDISH_FINLAND = wx._gdi.LANGUAGE_SWEDISH_FINLAND
wxLANGUAGE_TAGALOG = wx._gdi.LANGUAGE_TAGALOG
wxLANGUAGE_TAJIK = wx._gdi.LANGUAGE_TAJIK
wxLANGUAGE_TAMIL = wx._gdi.LANGUAGE_TAMIL
wxLANGUAGE_TATAR = wx._gdi.LANGUAGE_TATAR
wxLANGUAGE_TELUGU = wx._gdi.LANGUAGE_TELUGU
wxLANGUAGE_THAI = wx._gdi.LANGUAGE_THAI
wxLANGUAGE_TIBETAN = wx._gdi.LANGUAGE_TIBETAN
wxLANGUAGE_TIGRINYA = wx._gdi.LANGUAGE_TIGRINYA
wxLANGUAGE_TONGA = wx._gdi.LANGUAGE_TONGA
wxLANGUAGE_TSONGA = wx._gdi.LANGUAGE_TSONGA
wxLANGUAGE_TURKISH = wx._gdi.LANGUAGE_TURKISH
wxLANGUAGE_TURKMEN = wx._gdi.LANGUAGE_TURKMEN
wxLANGUAGE_TWI = wx._gdi.LANGUAGE_TWI
wxLANGUAGE_UIGHUR = wx._gdi.LANGUAGE_UIGHUR
wxLANGUAGE_UKRAINIAN = wx._gdi.LANGUAGE_UKRAINIAN
wxLANGUAGE_URDU = wx._gdi.LANGUAGE_URDU
wxLANGUAGE_URDU_INDIA = wx._gdi.LANGUAGE_URDU_INDIA
wxLANGUAGE_URDU_PAKISTAN = wx._gdi.LANGUAGE_URDU_PAKISTAN
wxLANGUAGE_UZBEK = wx._gdi.LANGUAGE_UZBEK
wxLANGUAGE_UZBEK_CYRILLIC = wx._gdi.LANGUAGE_UZBEK_CYRILLIC
wxLANGUAGE_UZBEK_LATIN = wx._gdi.LANGUAGE_UZBEK_LATIN
wxLANGUAGE_VIETNAMESE = wx._gdi.LANGUAGE_VIETNAMESE
wxLANGUAGE_VOLAPUK = wx._gdi.LANGUAGE_VOLAPUK
wxLANGUAGE_WELSH = wx._gdi.LANGUAGE_WELSH
wxLANGUAGE_WOLOF = wx._gdi.LANGUAGE_WOLOF
wxLANGUAGE_XHOSA = wx._gdi.LANGUAGE_XHOSA
wxLANGUAGE_YIDDISH = wx._gdi.LANGUAGE_YIDDISH
wxLANGUAGE_YORUBA = wx._gdi.LANGUAGE_YORUBA
wxLANGUAGE_ZHUANG = wx._gdi.LANGUAGE_ZHUANG
wxLANGUAGE_ZULU = wx._gdi.LANGUAGE_ZULU
wxLANGUAGE_USER_DEFINED = wx._gdi.LANGUAGE_USER_DEFINED
wxLanguageInfo = wx._gdi.LanguageInfo
wxLanguageInfoPtr = wx._gdi.LanguageInfoPtr
wxLOCALE_CAT_NUMBER = wx._gdi.LOCALE_CAT_NUMBER
wxLOCALE_CAT_DATE = wx._gdi.LOCALE_CAT_DATE
wxLOCALE_CAT_MONEY = wx._gdi.LOCALE_CAT_MONEY
wxLOCALE_CAT_MAX = wx._gdi.LOCALE_CAT_MAX
wxLOCALE_THOUSANDS_SEP = wx._gdi.LOCALE_THOUSANDS_SEP
wxLOCALE_DECIMAL_POINT = wx._gdi.LOCALE_DECIMAL_POINT
wxLOCALE_LOAD_DEFAULT = wx._gdi.LOCALE_LOAD_DEFAULT
wxLOCALE_CONV_ENCODING = wx._gdi.LOCALE_CONV_ENCODING
wxLocale = wx._gdi.Locale
wxLocalePtr = wx._gdi.LocalePtr
wxLocale_GetSystemLanguage = wx._gdi.Locale_GetSystemLanguage
wxLocale_GetSystemEncoding = wx._gdi.Locale_GetSystemEncoding
wxLocale_GetSystemEncodingName = wx._gdi.Locale_GetSystemEncodingName
wxLocale_AddCatalogLookupPathPrefix = wx._gdi.Locale_AddCatalogLookupPathPrefix
wxLocale_GetLanguageInfo = wx._gdi.Locale_GetLanguageInfo
wxLocale_GetLanguageName = wx._gdi.Locale_GetLanguageName
wxLocale_FindLanguageInfo = wx._gdi.Locale_FindLanguageInfo
wxLocale_AddLanguage = wx._gdi.Locale_AddLanguage
wxGetLocale = wx._gdi.GetLocale
wxGetTranslation = wx._gdi.GetTranslation
wxGetTranslation = wx._gdi.GetTranslation
wxCONVERT_STRICT = wx._gdi.CONVERT_STRICT
wxCONVERT_SUBSTITUTE = wx._gdi.CONVERT_SUBSTITUTE
wxPLATFORM_CURRENT = wx._gdi.PLATFORM_CURRENT
wxPLATFORM_UNIX = wx._gdi.PLATFORM_UNIX
wxPLATFORM_WINDOWS = wx._gdi.PLATFORM_WINDOWS
wxPLATFORM_OS2 = wx._gdi.PLATFORM_OS2
wxPLATFORM_MAC = wx._gdi.PLATFORM_MAC
wxEncodingConverter = wx._gdi.EncodingConverter
wxEncodingConverterPtr = wx._gdi.EncodingConverterPtr
wxEncodingConverter_GetPlatformEquivalents = wx._gdi.EncodingConverter_GetPlatformEquivalents
wxEncodingConverter_GetAllEquivalents = wx._gdi.EncodingConverter_GetAllEquivalents
wxEncodingConverter_CanConvert = wx._gdi.EncodingConverter_CanConvert
wxDC = wx._gdi.DC
wxDCPtr = wx._gdi.DCPtr
wxMemoryDC = wx._gdi.MemoryDC
wxMemoryDCPtr = wx._gdi.MemoryDCPtr
wxMemoryDCFromDC = wx._gdi.MemoryDCFromDC
wxBufferedDC = wx._gdi.BufferedDC
wxBufferedDCPtr = wx._gdi.BufferedDCPtr
wxBufferedDCInternalBuffer = wx._gdi.BufferedDCInternalBuffer
wxBufferedPaintDC = wx._gdi.BufferedPaintDC
wxBufferedPaintDCPtr = wx._gdi.BufferedPaintDCPtr
wxScreenDC = wx._gdi.ScreenDC
wxScreenDCPtr = wx._gdi.ScreenDCPtr
wxClientDC = wx._gdi.ClientDC
wxClientDCPtr = wx._gdi.ClientDCPtr
wxPaintDC = wx._gdi.PaintDC
wxPaintDCPtr = wx._gdi.PaintDCPtr
wxWindowDC = wx._gdi.WindowDC
wxWindowDCPtr = wx._gdi.WindowDCPtr
wxMirrorDC = wx._gdi.MirrorDC
wxMirrorDCPtr = wx._gdi.MirrorDCPtr
wxPostScriptDC = wx._gdi.PostScriptDC
wxPostScriptDCPtr = wx._gdi.PostScriptDCPtr
wxPostScriptDC_SetResolution = wx._gdi.PostScriptDC_SetResolution
wxPostScriptDC_GetResolution = wx._gdi.PostScriptDC_GetResolution
wxMetaFile = wx._gdi.MetaFile
wxMetaFilePtr = wx._gdi.MetaFilePtr
wxMetaFileDC = wx._gdi.MetaFileDC
wxMetaFileDCPtr = wx._gdi.MetaFileDCPtr
wxPrinterDC = wx._gdi.PrinterDC
wxPrinterDCPtr = wx._gdi.PrinterDCPtr
wxIMAGELIST_DRAW_NORMAL = wx._gdi.IMAGELIST_DRAW_NORMAL
wxIMAGELIST_DRAW_TRANSPARENT = wx._gdi.IMAGELIST_DRAW_TRANSPARENT
wxIMAGELIST_DRAW_SELECTED = wx._gdi.IMAGELIST_DRAW_SELECTED
wxIMAGELIST_DRAW_FOCUSED = wx._gdi.IMAGELIST_DRAW_FOCUSED
wxIMAGE_LIST_NORMAL = wx._gdi.IMAGE_LIST_NORMAL
wxIMAGE_LIST_SMALL = wx._gdi.IMAGE_LIST_SMALL
wxIMAGE_LIST_STATE = wx._gdi.IMAGE_LIST_STATE
wxImageList = wx._gdi.ImageList
wxImageListPtr = wx._gdi.ImageListPtr
wxNORMAL_FONT = wx._gdi.NORMAL_FONT
wxSMALL_FONT = wx._gdi.SMALL_FONT
wxITALIC_FONT = wx._gdi.ITALIC_FONT
wxSWISS_FONT = wx._gdi.SWISS_FONT
wxRED_PEN = wx._gdi.RED_PEN
wxCYAN_PEN = wx._gdi.CYAN_PEN
wxGREEN_PEN = wx._gdi.GREEN_PEN
wxBLACK_PEN = wx._gdi.BLACK_PEN
wxWHITE_PEN = wx._gdi.WHITE_PEN
wxTRANSPARENT_PEN = wx._gdi.TRANSPARENT_PEN
wxBLACK_DASHED_PEN = wx._gdi.BLACK_DASHED_PEN
wxGREY_PEN = wx._gdi.GREY_PEN
wxMEDIUM_GREY_PEN = wx._gdi.MEDIUM_GREY_PEN
wxLIGHT_GREY_PEN = wx._gdi.LIGHT_GREY_PEN
wxBLUE_BRUSH = wx._gdi.BLUE_BRUSH
wxGREEN_BRUSH = wx._gdi.GREEN_BRUSH
wxWHITE_BRUSH = wx._gdi.WHITE_BRUSH
wxBLACK_BRUSH = wx._gdi.BLACK_BRUSH
wxTRANSPARENT_BRUSH = wx._gdi.TRANSPARENT_BRUSH
wxCYAN_BRUSH = wx._gdi.CYAN_BRUSH
wxRED_BRUSH = wx._gdi.RED_BRUSH
wxGREY_BRUSH = wx._gdi.GREY_BRUSH
wxMEDIUM_GREY_BRUSH = wx._gdi.MEDIUM_GREY_BRUSH
wxLIGHT_GREY_BRUSH = wx._gdi.LIGHT_GREY_BRUSH
wxBLACK = wx._gdi.BLACK
wxWHITE = wx._gdi.WHITE
wxRED = wx._gdi.RED
wxBLUE = wx._gdi.BLUE
wxGREEN = wx._gdi.GREEN
wxCYAN = wx._gdi.CYAN
wxLIGHT_GREY = wx._gdi.LIGHT_GREY
wxSTANDARD_CURSOR = wx._gdi.STANDARD_CURSOR
wxHOURGLASS_CURSOR = wx._gdi.HOURGLASS_CURSOR
wxCROSS_CURSOR = wx._gdi.CROSS_CURSOR
wxNullBitmap = wx._gdi.NullBitmap
wxNullIcon = wx._gdi.NullIcon
wxNullCursor = wx._gdi.NullCursor
wxNullPen = wx._gdi.NullPen
wxNullBrush = wx._gdi.NullBrush
wxNullPalette = wx._gdi.NullPalette
wxNullFont = wx._gdi.NullFont
wxNullColour = wx._gdi.NullColour
wxPenList = wx._gdi.PenList
wxPenListPtr = wx._gdi.PenListPtr
wxBrushList = wx._gdi.BrushList
wxBrushListPtr = wx._gdi.BrushListPtr
wxColourDatabase = wx._gdi.ColourDatabase
wxColourDatabasePtr = wx._gdi.ColourDatabasePtr
wxFontList = wx._gdi.FontList
wxFontListPtr = wx._gdi.FontListPtr
wxTheFontList = wx._gdi.TheFontList
wxThePenList = wx._gdi.ThePenList
wxTheBrushList = wx._gdi.TheBrushList
wxTheColourDatabase = wx._gdi.TheColourDatabase
wxEffects = wx._gdi.Effects
wxEffectsPtr = wx._gdi.EffectsPtr
wxMaskColour = wx._gdi.MaskColour


