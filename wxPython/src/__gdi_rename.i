// A bunch of %rename directives generated by BuildRenamers in config.py
// in order to remove the wx prefix from all global scope names.

#ifndef BUILDING_RENAMERS

%rename(GDIObject)                          wxGDIObject;
%rename(C2S_NAME)                           wxC2S_NAME;
%rename(C2S_CSS_SYNTAX)                     wxC2S_CSS_SYNTAX;
%rename(C2S_HTML_SYNTAX)                    wxC2S_HTML_SYNTAX;
%rename(Colour)                             wxColour;
%rename(Palette)                            wxPalette;
%rename(Pen)                                wxPen;
%rename(Brush)                              wxBrush;
%rename(Bitmap)                             wxBitmap;
%rename(Mask)                               wxMask;
%rename(Icon)                               wxIcon;
%rename(IconLocation)                       wxIconLocation;
%rename(IconBundle)                         wxIconBundle;
%rename(Cursor)                             wxCursor;
%rename(OutRegion)                          wxOutRegion;
%rename(PartRegion)                         wxPartRegion;
%rename(InRegion)                           wxInRegion;
%rename(Region)                             wxRegion;
%rename(RegionIterator)                     wxRegionIterator;
%rename(FONTFAMILY_DEFAULT)                 wxFONTFAMILY_DEFAULT;
%rename(FONTFAMILY_DECORATIVE)              wxFONTFAMILY_DECORATIVE;
%rename(FONTFAMILY_ROMAN)                   wxFONTFAMILY_ROMAN;
%rename(FONTFAMILY_SCRIPT)                  wxFONTFAMILY_SCRIPT;
%rename(FONTFAMILY_SWISS)                   wxFONTFAMILY_SWISS;
%rename(FONTFAMILY_MODERN)                  wxFONTFAMILY_MODERN;
%rename(FONTFAMILY_TELETYPE)                wxFONTFAMILY_TELETYPE;
%rename(FONTFAMILY_MAX)                     wxFONTFAMILY_MAX;
%rename(FONTFAMILY_UNKNOWN)                 wxFONTFAMILY_UNKNOWN;
%rename(FONTSTYLE_NORMAL)                   wxFONTSTYLE_NORMAL;
%rename(FONTSTYLE_ITALIC)                   wxFONTSTYLE_ITALIC;
%rename(FONTSTYLE_SLANT)                    wxFONTSTYLE_SLANT;
%rename(FONTSTYLE_MAX)                      wxFONTSTYLE_MAX;
%rename(FONTWEIGHT_NORMAL)                  wxFONTWEIGHT_NORMAL;
%rename(FONTWEIGHT_LIGHT)                   wxFONTWEIGHT_LIGHT;
%rename(FONTWEIGHT_BOLD)                    wxFONTWEIGHT_BOLD;
%rename(FONTWEIGHT_MAX)                     wxFONTWEIGHT_MAX;
%rename(FONTFLAG_DEFAULT)                   wxFONTFLAG_DEFAULT;
%rename(FONTFLAG_ITALIC)                    wxFONTFLAG_ITALIC;
%rename(FONTFLAG_SLANT)                     wxFONTFLAG_SLANT;
%rename(FONTFLAG_LIGHT)                     wxFONTFLAG_LIGHT;
%rename(FONTFLAG_BOLD)                      wxFONTFLAG_BOLD;
%rename(FONTFLAG_ANTIALIASED)               wxFONTFLAG_ANTIALIASED;
%rename(FONTFLAG_NOT_ANTIALIASED)           wxFONTFLAG_NOT_ANTIALIASED;
%rename(FONTFLAG_UNDERLINED)                wxFONTFLAG_UNDERLINED;
%rename(FONTFLAG_STRIKETHROUGH)             wxFONTFLAG_STRIKETHROUGH;
%rename(FONTFLAG_MASK)                      wxFONTFLAG_MASK;
%rename(FONTENCODING_SYSTEM)                wxFONTENCODING_SYSTEM;
%rename(FONTENCODING_DEFAULT)               wxFONTENCODING_DEFAULT;
%rename(FONTENCODING_ISO8859_1)             wxFONTENCODING_ISO8859_1;
%rename(FONTENCODING_ISO8859_2)             wxFONTENCODING_ISO8859_2;
%rename(FONTENCODING_ISO8859_3)             wxFONTENCODING_ISO8859_3;
%rename(FONTENCODING_ISO8859_4)             wxFONTENCODING_ISO8859_4;
%rename(FONTENCODING_ISO8859_5)             wxFONTENCODING_ISO8859_5;
%rename(FONTENCODING_ISO8859_6)             wxFONTENCODING_ISO8859_6;
%rename(FONTENCODING_ISO8859_7)             wxFONTENCODING_ISO8859_7;
%rename(FONTENCODING_ISO8859_8)             wxFONTENCODING_ISO8859_8;
%rename(FONTENCODING_ISO8859_9)             wxFONTENCODING_ISO8859_9;
%rename(FONTENCODING_ISO8859_10)            wxFONTENCODING_ISO8859_10;
%rename(FONTENCODING_ISO8859_11)            wxFONTENCODING_ISO8859_11;
%rename(FONTENCODING_ISO8859_12)            wxFONTENCODING_ISO8859_12;
%rename(FONTENCODING_ISO8859_13)            wxFONTENCODING_ISO8859_13;
%rename(FONTENCODING_ISO8859_14)            wxFONTENCODING_ISO8859_14;
%rename(FONTENCODING_ISO8859_15)            wxFONTENCODING_ISO8859_15;
%rename(FONTENCODING_ISO8859_MAX)           wxFONTENCODING_ISO8859_MAX;
%rename(FONTENCODING_KOI8)                  wxFONTENCODING_KOI8;
%rename(FONTENCODING_KOI8_U)                wxFONTENCODING_KOI8_U;
%rename(FONTENCODING_ALTERNATIVE)           wxFONTENCODING_ALTERNATIVE;
%rename(FONTENCODING_BULGARIAN)             wxFONTENCODING_BULGARIAN;
%rename(FONTENCODING_CP437)                 wxFONTENCODING_CP437;
%rename(FONTENCODING_CP850)                 wxFONTENCODING_CP850;
%rename(FONTENCODING_CP852)                 wxFONTENCODING_CP852;
%rename(FONTENCODING_CP855)                 wxFONTENCODING_CP855;
%rename(FONTENCODING_CP866)                 wxFONTENCODING_CP866;
%rename(FONTENCODING_CP874)                 wxFONTENCODING_CP874;
%rename(FONTENCODING_CP932)                 wxFONTENCODING_CP932;
%rename(FONTENCODING_CP936)                 wxFONTENCODING_CP936;
%rename(FONTENCODING_CP949)                 wxFONTENCODING_CP949;
%rename(FONTENCODING_CP950)                 wxFONTENCODING_CP950;
%rename(FONTENCODING_CP1250)                wxFONTENCODING_CP1250;
%rename(FONTENCODING_CP1251)                wxFONTENCODING_CP1251;
%rename(FONTENCODING_CP1252)                wxFONTENCODING_CP1252;
%rename(FONTENCODING_CP1253)                wxFONTENCODING_CP1253;
%rename(FONTENCODING_CP1254)                wxFONTENCODING_CP1254;
%rename(FONTENCODING_CP1255)                wxFONTENCODING_CP1255;
%rename(FONTENCODING_CP1256)                wxFONTENCODING_CP1256;
%rename(FONTENCODING_CP1257)                wxFONTENCODING_CP1257;
%rename(FONTENCODING_CP12_MAX)              wxFONTENCODING_CP12_MAX;
%rename(FONTENCODING_UTF7)                  wxFONTENCODING_UTF7;
%rename(FONTENCODING_UTF8)                  wxFONTENCODING_UTF8;
%rename(FONTENCODING_EUC_JP)                wxFONTENCODING_EUC_JP;
%rename(FONTENCODING_UTF16BE)               wxFONTENCODING_UTF16BE;
%rename(FONTENCODING_UTF16LE)               wxFONTENCODING_UTF16LE;
%rename(FONTENCODING_UTF32BE)               wxFONTENCODING_UTF32BE;
%rename(FONTENCODING_UTF32LE)               wxFONTENCODING_UTF32LE;
%rename(FONTENCODING_MACROMAN)              wxFONTENCODING_MACROMAN;
%rename(FONTENCODING_MACJAPANESE)           wxFONTENCODING_MACJAPANESE;
%rename(FONTENCODING_MACCHINESETRAD)        wxFONTENCODING_MACCHINESETRAD;
%rename(FONTENCODING_MACKOREAN)             wxFONTENCODING_MACKOREAN;
%rename(FONTENCODING_MACARABIC)             wxFONTENCODING_MACARABIC;
%rename(FONTENCODING_MACHEBREW)             wxFONTENCODING_MACHEBREW;
%rename(FONTENCODING_MACGREEK)              wxFONTENCODING_MACGREEK;
%rename(FONTENCODING_MACCYRILLIC)           wxFONTENCODING_MACCYRILLIC;
%rename(FONTENCODING_MACDEVANAGARI)         wxFONTENCODING_MACDEVANAGARI;
%rename(FONTENCODING_MACGURMUKHI)           wxFONTENCODING_MACGURMUKHI;
%rename(FONTENCODING_MACGUJARATI)           wxFONTENCODING_MACGUJARATI;
%rename(FONTENCODING_MACORIYA)              wxFONTENCODING_MACORIYA;
%rename(FONTENCODING_MACBENGALI)            wxFONTENCODING_MACBENGALI;
%rename(FONTENCODING_MACTAMIL)              wxFONTENCODING_MACTAMIL;
%rename(FONTENCODING_MACTELUGU)             wxFONTENCODING_MACTELUGU;
%rename(FONTENCODING_MACKANNADA)            wxFONTENCODING_MACKANNADA;
%rename(FONTENCODING_MACMALAJALAM)          wxFONTENCODING_MACMALAJALAM;
%rename(FONTENCODING_MACSINHALESE)          wxFONTENCODING_MACSINHALESE;
%rename(FONTENCODING_MACBURMESE)            wxFONTENCODING_MACBURMESE;
%rename(FONTENCODING_MACKHMER)              wxFONTENCODING_MACKHMER;
%rename(FONTENCODING_MACTHAI)               wxFONTENCODING_MACTHAI;
%rename(FONTENCODING_MACLAOTIAN)            wxFONTENCODING_MACLAOTIAN;
%rename(FONTENCODING_MACGEORGIAN)           wxFONTENCODING_MACGEORGIAN;
%rename(FONTENCODING_MACARMENIAN)           wxFONTENCODING_MACARMENIAN;
%rename(FONTENCODING_MACCHINESESIMP)        wxFONTENCODING_MACCHINESESIMP;
%rename(FONTENCODING_MACTIBETAN)            wxFONTENCODING_MACTIBETAN;
%rename(FONTENCODING_MACMONGOLIAN)          wxFONTENCODING_MACMONGOLIAN;
%rename(FONTENCODING_MACETHIOPIC)           wxFONTENCODING_MACETHIOPIC;
%rename(FONTENCODING_MACCENTRALEUR)         wxFONTENCODING_MACCENTRALEUR;
%rename(FONTENCODING_MACVIATNAMESE)         wxFONTENCODING_MACVIATNAMESE;
%rename(FONTENCODING_MACARABICEXT)          wxFONTENCODING_MACARABICEXT;
%rename(FONTENCODING_MACSYMBOL)             wxFONTENCODING_MACSYMBOL;
%rename(FONTENCODING_MACDINGBATS)           wxFONTENCODING_MACDINGBATS;
%rename(FONTENCODING_MACTURKISH)            wxFONTENCODING_MACTURKISH;
%rename(FONTENCODING_MACCROATIAN)           wxFONTENCODING_MACCROATIAN;
%rename(FONTENCODING_MACICELANDIC)          wxFONTENCODING_MACICELANDIC;
%rename(FONTENCODING_MACROMANIAN)           wxFONTENCODING_MACROMANIAN;
%rename(FONTENCODING_MACCELTIC)             wxFONTENCODING_MACCELTIC;
%rename(FONTENCODING_MACGAELIC)             wxFONTENCODING_MACGAELIC;
%rename(FONTENCODING_MACKEYBOARD)           wxFONTENCODING_MACKEYBOARD;
%rename(FONTENCODING_MACMIN)                wxFONTENCODING_MACMIN;
%rename(FONTENCODING_MACMAX)                wxFONTENCODING_MACMAX;
%rename(FONTENCODING_MAX)                   wxFONTENCODING_MAX;
%rename(FONTENCODING_UTF16)                 wxFONTENCODING_UTF16;
%rename(FONTENCODING_UTF32)                 wxFONTENCODING_UTF32;
%rename(FONTENCODING_UNICODE)               wxFONTENCODING_UNICODE;
%rename(FONTENCODING_GB2312)                wxFONTENCODING_GB2312;
%rename(FONTENCODING_BIG5)                  wxFONTENCODING_BIG5;
%rename(FONTENCODING_SHIFT_JIS)             wxFONTENCODING_SHIFT_JIS;
%rename(NativeFontInfo)                     wxNativeFontInfo;
%rename(NativeEncodingInfo)                 wxNativeEncodingInfo;
%rename(GetNativeFontEncoding)              wxGetNativeFontEncoding;
%rename(TestFontEncoding)                   wxTestFontEncoding;
%rename(FontMapper)                         wxFontMapper;
%rename(Font)                               wxFont;
%rename(LANGUAGE_DEFAULT)                   wxLANGUAGE_DEFAULT;
%rename(LANGUAGE_UNKNOWN)                   wxLANGUAGE_UNKNOWN;
%rename(LANGUAGE_ABKHAZIAN)                 wxLANGUAGE_ABKHAZIAN;
%rename(LANGUAGE_AFAR)                      wxLANGUAGE_AFAR;
%rename(LANGUAGE_AFRIKAANS)                 wxLANGUAGE_AFRIKAANS;
%rename(LANGUAGE_ALBANIAN)                  wxLANGUAGE_ALBANIAN;
%rename(LANGUAGE_AMHARIC)                   wxLANGUAGE_AMHARIC;
%rename(LANGUAGE_ARABIC)                    wxLANGUAGE_ARABIC;
%rename(LANGUAGE_ARABIC_ALGERIA)            wxLANGUAGE_ARABIC_ALGERIA;
%rename(LANGUAGE_ARABIC_BAHRAIN)            wxLANGUAGE_ARABIC_BAHRAIN;
%rename(LANGUAGE_ARABIC_EGYPT)              wxLANGUAGE_ARABIC_EGYPT;
%rename(LANGUAGE_ARABIC_IRAQ)               wxLANGUAGE_ARABIC_IRAQ;
%rename(LANGUAGE_ARABIC_JORDAN)             wxLANGUAGE_ARABIC_JORDAN;
%rename(LANGUAGE_ARABIC_KUWAIT)             wxLANGUAGE_ARABIC_KUWAIT;
%rename(LANGUAGE_ARABIC_LEBANON)            wxLANGUAGE_ARABIC_LEBANON;
%rename(LANGUAGE_ARABIC_LIBYA)              wxLANGUAGE_ARABIC_LIBYA;
%rename(LANGUAGE_ARABIC_MOROCCO)            wxLANGUAGE_ARABIC_MOROCCO;
%rename(LANGUAGE_ARABIC_OMAN)               wxLANGUAGE_ARABIC_OMAN;
%rename(LANGUAGE_ARABIC_QATAR)              wxLANGUAGE_ARABIC_QATAR;
%rename(LANGUAGE_ARABIC_SAUDI_ARABIA)       wxLANGUAGE_ARABIC_SAUDI_ARABIA;
%rename(LANGUAGE_ARABIC_SUDAN)              wxLANGUAGE_ARABIC_SUDAN;
%rename(LANGUAGE_ARABIC_SYRIA)              wxLANGUAGE_ARABIC_SYRIA;
%rename(LANGUAGE_ARABIC_TUNISIA)            wxLANGUAGE_ARABIC_TUNISIA;
%rename(LANGUAGE_ARABIC_UAE)                wxLANGUAGE_ARABIC_UAE;
%rename(LANGUAGE_ARABIC_YEMEN)              wxLANGUAGE_ARABIC_YEMEN;
%rename(LANGUAGE_ARMENIAN)                  wxLANGUAGE_ARMENIAN;
%rename(LANGUAGE_ASSAMESE)                  wxLANGUAGE_ASSAMESE;
%rename(LANGUAGE_AYMARA)                    wxLANGUAGE_AYMARA;
%rename(LANGUAGE_AZERI)                     wxLANGUAGE_AZERI;
%rename(LANGUAGE_AZERI_CYRILLIC)            wxLANGUAGE_AZERI_CYRILLIC;
%rename(LANGUAGE_AZERI_LATIN)               wxLANGUAGE_AZERI_LATIN;
%rename(LANGUAGE_BASHKIR)                   wxLANGUAGE_BASHKIR;
%rename(LANGUAGE_BASQUE)                    wxLANGUAGE_BASQUE;
%rename(LANGUAGE_BELARUSIAN)                wxLANGUAGE_BELARUSIAN;
%rename(LANGUAGE_BENGALI)                   wxLANGUAGE_BENGALI;
%rename(LANGUAGE_BHUTANI)                   wxLANGUAGE_BHUTANI;
%rename(LANGUAGE_BIHARI)                    wxLANGUAGE_BIHARI;
%rename(LANGUAGE_BISLAMA)                   wxLANGUAGE_BISLAMA;
%rename(LANGUAGE_BRETON)                    wxLANGUAGE_BRETON;
%rename(LANGUAGE_BULGARIAN)                 wxLANGUAGE_BULGARIAN;
%rename(LANGUAGE_BURMESE)                   wxLANGUAGE_BURMESE;
%rename(LANGUAGE_CAMBODIAN)                 wxLANGUAGE_CAMBODIAN;
%rename(LANGUAGE_CATALAN)                   wxLANGUAGE_CATALAN;
%rename(LANGUAGE_CHINESE)                   wxLANGUAGE_CHINESE;
%rename(LANGUAGE_CHINESE_SIMPLIFIED)        wxLANGUAGE_CHINESE_SIMPLIFIED;
%rename(LANGUAGE_CHINESE_TRADITIONAL)       wxLANGUAGE_CHINESE_TRADITIONAL;
%rename(LANGUAGE_CHINESE_HONGKONG)          wxLANGUAGE_CHINESE_HONGKONG;
%rename(LANGUAGE_CHINESE_MACAU)             wxLANGUAGE_CHINESE_MACAU;
%rename(LANGUAGE_CHINESE_SINGAPORE)         wxLANGUAGE_CHINESE_SINGAPORE;
%rename(LANGUAGE_CHINESE_TAIWAN)            wxLANGUAGE_CHINESE_TAIWAN;
%rename(LANGUAGE_CORSICAN)                  wxLANGUAGE_CORSICAN;
%rename(LANGUAGE_CROATIAN)                  wxLANGUAGE_CROATIAN;
%rename(LANGUAGE_CZECH)                     wxLANGUAGE_CZECH;
%rename(LANGUAGE_DANISH)                    wxLANGUAGE_DANISH;
%rename(LANGUAGE_DUTCH)                     wxLANGUAGE_DUTCH;
%rename(LANGUAGE_DUTCH_BELGIAN)             wxLANGUAGE_DUTCH_BELGIAN;
%rename(LANGUAGE_ENGLISH)                   wxLANGUAGE_ENGLISH;
%rename(LANGUAGE_ENGLISH_UK)                wxLANGUAGE_ENGLISH_UK;
%rename(LANGUAGE_ENGLISH_US)                wxLANGUAGE_ENGLISH_US;
%rename(LANGUAGE_ENGLISH_AUSTRALIA)         wxLANGUAGE_ENGLISH_AUSTRALIA;
%rename(LANGUAGE_ENGLISH_BELIZE)            wxLANGUAGE_ENGLISH_BELIZE;
%rename(LANGUAGE_ENGLISH_BOTSWANA)          wxLANGUAGE_ENGLISH_BOTSWANA;
%rename(LANGUAGE_ENGLISH_CANADA)            wxLANGUAGE_ENGLISH_CANADA;
%rename(LANGUAGE_ENGLISH_CARIBBEAN)         wxLANGUAGE_ENGLISH_CARIBBEAN;
%rename(LANGUAGE_ENGLISH_DENMARK)           wxLANGUAGE_ENGLISH_DENMARK;
%rename(LANGUAGE_ENGLISH_EIRE)              wxLANGUAGE_ENGLISH_EIRE;
%rename(LANGUAGE_ENGLISH_JAMAICA)           wxLANGUAGE_ENGLISH_JAMAICA;
%rename(LANGUAGE_ENGLISH_NEW_ZEALAND)       wxLANGUAGE_ENGLISH_NEW_ZEALAND;
%rename(LANGUAGE_ENGLISH_PHILIPPINES)       wxLANGUAGE_ENGLISH_PHILIPPINES;
%rename(LANGUAGE_ENGLISH_SOUTH_AFRICA)      wxLANGUAGE_ENGLISH_SOUTH_AFRICA;
%rename(LANGUAGE_ENGLISH_TRINIDAD)          wxLANGUAGE_ENGLISH_TRINIDAD;
%rename(LANGUAGE_ENGLISH_ZIMBABWE)          wxLANGUAGE_ENGLISH_ZIMBABWE;
%rename(LANGUAGE_ESPERANTO)                 wxLANGUAGE_ESPERANTO;
%rename(LANGUAGE_ESTONIAN)                  wxLANGUAGE_ESTONIAN;
%rename(LANGUAGE_FAEROESE)                  wxLANGUAGE_FAEROESE;
%rename(LANGUAGE_FARSI)                     wxLANGUAGE_FARSI;
%rename(LANGUAGE_FIJI)                      wxLANGUAGE_FIJI;
%rename(LANGUAGE_FINNISH)                   wxLANGUAGE_FINNISH;
%rename(LANGUAGE_FRENCH)                    wxLANGUAGE_FRENCH;
%rename(LANGUAGE_FRENCH_BELGIAN)            wxLANGUAGE_FRENCH_BELGIAN;
%rename(LANGUAGE_FRENCH_CANADIAN)           wxLANGUAGE_FRENCH_CANADIAN;
%rename(LANGUAGE_FRENCH_LUXEMBOURG)         wxLANGUAGE_FRENCH_LUXEMBOURG;
%rename(LANGUAGE_FRENCH_MONACO)             wxLANGUAGE_FRENCH_MONACO;
%rename(LANGUAGE_FRENCH_SWISS)              wxLANGUAGE_FRENCH_SWISS;
%rename(LANGUAGE_FRISIAN)                   wxLANGUAGE_FRISIAN;
%rename(LANGUAGE_GALICIAN)                  wxLANGUAGE_GALICIAN;
%rename(LANGUAGE_GEORGIAN)                  wxLANGUAGE_GEORGIAN;
%rename(LANGUAGE_GERMAN)                    wxLANGUAGE_GERMAN;
%rename(LANGUAGE_GERMAN_AUSTRIAN)           wxLANGUAGE_GERMAN_AUSTRIAN;
%rename(LANGUAGE_GERMAN_BELGIUM)            wxLANGUAGE_GERMAN_BELGIUM;
%rename(LANGUAGE_GERMAN_LIECHTENSTEIN)      wxLANGUAGE_GERMAN_LIECHTENSTEIN;
%rename(LANGUAGE_GERMAN_LUXEMBOURG)         wxLANGUAGE_GERMAN_LUXEMBOURG;
%rename(LANGUAGE_GERMAN_SWISS)              wxLANGUAGE_GERMAN_SWISS;
%rename(LANGUAGE_GREEK)                     wxLANGUAGE_GREEK;
%rename(LANGUAGE_GREENLANDIC)               wxLANGUAGE_GREENLANDIC;
%rename(LANGUAGE_GUARANI)                   wxLANGUAGE_GUARANI;
%rename(LANGUAGE_GUJARATI)                  wxLANGUAGE_GUJARATI;
%rename(LANGUAGE_HAUSA)                     wxLANGUAGE_HAUSA;
%rename(LANGUAGE_HEBREW)                    wxLANGUAGE_HEBREW;
%rename(LANGUAGE_HINDI)                     wxLANGUAGE_HINDI;
%rename(LANGUAGE_HUNGARIAN)                 wxLANGUAGE_HUNGARIAN;
%rename(LANGUAGE_ICELANDIC)                 wxLANGUAGE_ICELANDIC;
%rename(LANGUAGE_INDONESIAN)                wxLANGUAGE_INDONESIAN;
%rename(LANGUAGE_INTERLINGUA)               wxLANGUAGE_INTERLINGUA;
%rename(LANGUAGE_INTERLINGUE)               wxLANGUAGE_INTERLINGUE;
%rename(LANGUAGE_INUKTITUT)                 wxLANGUAGE_INUKTITUT;
%rename(LANGUAGE_INUPIAK)                   wxLANGUAGE_INUPIAK;
%rename(LANGUAGE_IRISH)                     wxLANGUAGE_IRISH;
%rename(LANGUAGE_ITALIAN)                   wxLANGUAGE_ITALIAN;
%rename(LANGUAGE_ITALIAN_SWISS)             wxLANGUAGE_ITALIAN_SWISS;
%rename(LANGUAGE_JAPANESE)                  wxLANGUAGE_JAPANESE;
%rename(LANGUAGE_JAVANESE)                  wxLANGUAGE_JAVANESE;
%rename(LANGUAGE_KANNADA)                   wxLANGUAGE_KANNADA;
%rename(LANGUAGE_KASHMIRI)                  wxLANGUAGE_KASHMIRI;
%rename(LANGUAGE_KASHMIRI_INDIA)            wxLANGUAGE_KASHMIRI_INDIA;
%rename(LANGUAGE_KAZAKH)                    wxLANGUAGE_KAZAKH;
%rename(LANGUAGE_KERNEWEK)                  wxLANGUAGE_KERNEWEK;
%rename(LANGUAGE_KINYARWANDA)               wxLANGUAGE_KINYARWANDA;
%rename(LANGUAGE_KIRGHIZ)                   wxLANGUAGE_KIRGHIZ;
%rename(LANGUAGE_KIRUNDI)                   wxLANGUAGE_KIRUNDI;
%rename(LANGUAGE_KONKANI)                   wxLANGUAGE_KONKANI;
%rename(LANGUAGE_KOREAN)                    wxLANGUAGE_KOREAN;
%rename(LANGUAGE_KURDISH)                   wxLANGUAGE_KURDISH;
%rename(LANGUAGE_LAOTHIAN)                  wxLANGUAGE_LAOTHIAN;
%rename(LANGUAGE_LATIN)                     wxLANGUAGE_LATIN;
%rename(LANGUAGE_LATVIAN)                   wxLANGUAGE_LATVIAN;
%rename(LANGUAGE_LINGALA)                   wxLANGUAGE_LINGALA;
%rename(LANGUAGE_LITHUANIAN)                wxLANGUAGE_LITHUANIAN;
%rename(LANGUAGE_MACEDONIAN)                wxLANGUAGE_MACEDONIAN;
%rename(LANGUAGE_MALAGASY)                  wxLANGUAGE_MALAGASY;
%rename(LANGUAGE_MALAY)                     wxLANGUAGE_MALAY;
%rename(LANGUAGE_MALAYALAM)                 wxLANGUAGE_MALAYALAM;
%rename(LANGUAGE_MALAY_BRUNEI_DARUSSALAM)   wxLANGUAGE_MALAY_BRUNEI_DARUSSALAM;
%rename(LANGUAGE_MALAY_MALAYSIA)            wxLANGUAGE_MALAY_MALAYSIA;
%rename(LANGUAGE_MALTESE)                   wxLANGUAGE_MALTESE;
%rename(LANGUAGE_MANIPURI)                  wxLANGUAGE_MANIPURI;
%rename(LANGUAGE_MAORI)                     wxLANGUAGE_MAORI;
%rename(LANGUAGE_MARATHI)                   wxLANGUAGE_MARATHI;
%rename(LANGUAGE_MOLDAVIAN)                 wxLANGUAGE_MOLDAVIAN;
%rename(LANGUAGE_MONGOLIAN)                 wxLANGUAGE_MONGOLIAN;
%rename(LANGUAGE_NAURU)                     wxLANGUAGE_NAURU;
%rename(LANGUAGE_NEPALI)                    wxLANGUAGE_NEPALI;
%rename(LANGUAGE_NEPALI_INDIA)              wxLANGUAGE_NEPALI_INDIA;
%rename(LANGUAGE_NORWEGIAN_BOKMAL)          wxLANGUAGE_NORWEGIAN_BOKMAL;
%rename(LANGUAGE_NORWEGIAN_NYNORSK)         wxLANGUAGE_NORWEGIAN_NYNORSK;
%rename(LANGUAGE_OCCITAN)                   wxLANGUAGE_OCCITAN;
%rename(LANGUAGE_ORIYA)                     wxLANGUAGE_ORIYA;
%rename(LANGUAGE_OROMO)                     wxLANGUAGE_OROMO;
%rename(LANGUAGE_PASHTO)                    wxLANGUAGE_PASHTO;
%rename(LANGUAGE_POLISH)                    wxLANGUAGE_POLISH;
%rename(LANGUAGE_PORTUGUESE)                wxLANGUAGE_PORTUGUESE;
%rename(LANGUAGE_PORTUGUESE_BRAZILIAN)      wxLANGUAGE_PORTUGUESE_BRAZILIAN;
%rename(LANGUAGE_PUNJABI)                   wxLANGUAGE_PUNJABI;
%rename(LANGUAGE_QUECHUA)                   wxLANGUAGE_QUECHUA;
%rename(LANGUAGE_RHAETO_ROMANCE)            wxLANGUAGE_RHAETO_ROMANCE;
%rename(LANGUAGE_ROMANIAN)                  wxLANGUAGE_ROMANIAN;
%rename(LANGUAGE_RUSSIAN)                   wxLANGUAGE_RUSSIAN;
%rename(LANGUAGE_RUSSIAN_UKRAINE)           wxLANGUAGE_RUSSIAN_UKRAINE;
%rename(LANGUAGE_SAMOAN)                    wxLANGUAGE_SAMOAN;
%rename(LANGUAGE_SANGHO)                    wxLANGUAGE_SANGHO;
%rename(LANGUAGE_SANSKRIT)                  wxLANGUAGE_SANSKRIT;
%rename(LANGUAGE_SCOTS_GAELIC)              wxLANGUAGE_SCOTS_GAELIC;
%rename(LANGUAGE_SERBIAN)                   wxLANGUAGE_SERBIAN;
%rename(LANGUAGE_SERBIAN_CYRILLIC)          wxLANGUAGE_SERBIAN_CYRILLIC;
%rename(LANGUAGE_SERBIAN_LATIN)             wxLANGUAGE_SERBIAN_LATIN;
%rename(LANGUAGE_SERBO_CROATIAN)            wxLANGUAGE_SERBO_CROATIAN;
%rename(LANGUAGE_SESOTHO)                   wxLANGUAGE_SESOTHO;
%rename(LANGUAGE_SETSWANA)                  wxLANGUAGE_SETSWANA;
%rename(LANGUAGE_SHONA)                     wxLANGUAGE_SHONA;
%rename(LANGUAGE_SINDHI)                    wxLANGUAGE_SINDHI;
%rename(LANGUAGE_SINHALESE)                 wxLANGUAGE_SINHALESE;
%rename(LANGUAGE_SISWATI)                   wxLANGUAGE_SISWATI;
%rename(LANGUAGE_SLOVAK)                    wxLANGUAGE_SLOVAK;
%rename(LANGUAGE_SLOVENIAN)                 wxLANGUAGE_SLOVENIAN;
%rename(LANGUAGE_SOMALI)                    wxLANGUAGE_SOMALI;
%rename(LANGUAGE_SPANISH)                   wxLANGUAGE_SPANISH;
%rename(LANGUAGE_SPANISH_ARGENTINA)         wxLANGUAGE_SPANISH_ARGENTINA;
%rename(LANGUAGE_SPANISH_BOLIVIA)           wxLANGUAGE_SPANISH_BOLIVIA;
%rename(LANGUAGE_SPANISH_CHILE)             wxLANGUAGE_SPANISH_CHILE;
%rename(LANGUAGE_SPANISH_COLOMBIA)          wxLANGUAGE_SPANISH_COLOMBIA;
%rename(LANGUAGE_SPANISH_COSTA_RICA)        wxLANGUAGE_SPANISH_COSTA_RICA;
%rename(LANGUAGE_SPANISH_DOMINICAN_REPUBLIC)  wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC;
%rename(LANGUAGE_SPANISH_ECUADOR)           wxLANGUAGE_SPANISH_ECUADOR;
%rename(LANGUAGE_SPANISH_EL_SALVADOR)       wxLANGUAGE_SPANISH_EL_SALVADOR;
%rename(LANGUAGE_SPANISH_GUATEMALA)         wxLANGUAGE_SPANISH_GUATEMALA;
%rename(LANGUAGE_SPANISH_HONDURAS)          wxLANGUAGE_SPANISH_HONDURAS;
%rename(LANGUAGE_SPANISH_MEXICAN)           wxLANGUAGE_SPANISH_MEXICAN;
%rename(LANGUAGE_SPANISH_MODERN)            wxLANGUAGE_SPANISH_MODERN;
%rename(LANGUAGE_SPANISH_NICARAGUA)         wxLANGUAGE_SPANISH_NICARAGUA;
%rename(LANGUAGE_SPANISH_PANAMA)            wxLANGUAGE_SPANISH_PANAMA;
%rename(LANGUAGE_SPANISH_PARAGUAY)          wxLANGUAGE_SPANISH_PARAGUAY;
%rename(LANGUAGE_SPANISH_PERU)              wxLANGUAGE_SPANISH_PERU;
%rename(LANGUAGE_SPANISH_PUERTO_RICO)       wxLANGUAGE_SPANISH_PUERTO_RICO;
%rename(LANGUAGE_SPANISH_URUGUAY)           wxLANGUAGE_SPANISH_URUGUAY;
%rename(LANGUAGE_SPANISH_US)                wxLANGUAGE_SPANISH_US;
%rename(LANGUAGE_SPANISH_VENEZUELA)         wxLANGUAGE_SPANISH_VENEZUELA;
%rename(LANGUAGE_SUNDANESE)                 wxLANGUAGE_SUNDANESE;
%rename(LANGUAGE_SWAHILI)                   wxLANGUAGE_SWAHILI;
%rename(LANGUAGE_SWEDISH)                   wxLANGUAGE_SWEDISH;
%rename(LANGUAGE_SWEDISH_FINLAND)           wxLANGUAGE_SWEDISH_FINLAND;
%rename(LANGUAGE_TAGALOG)                   wxLANGUAGE_TAGALOG;
%rename(LANGUAGE_TAJIK)                     wxLANGUAGE_TAJIK;
%rename(LANGUAGE_TAMIL)                     wxLANGUAGE_TAMIL;
%rename(LANGUAGE_TATAR)                     wxLANGUAGE_TATAR;
%rename(LANGUAGE_TELUGU)                    wxLANGUAGE_TELUGU;
%rename(LANGUAGE_THAI)                      wxLANGUAGE_THAI;
%rename(LANGUAGE_TIBETAN)                   wxLANGUAGE_TIBETAN;
%rename(LANGUAGE_TIGRINYA)                  wxLANGUAGE_TIGRINYA;
%rename(LANGUAGE_TONGA)                     wxLANGUAGE_TONGA;
%rename(LANGUAGE_TSONGA)                    wxLANGUAGE_TSONGA;
%rename(LANGUAGE_TURKISH)                   wxLANGUAGE_TURKISH;
%rename(LANGUAGE_TURKMEN)                   wxLANGUAGE_TURKMEN;
%rename(LANGUAGE_TWI)                       wxLANGUAGE_TWI;
%rename(LANGUAGE_UIGHUR)                    wxLANGUAGE_UIGHUR;
%rename(LANGUAGE_UKRAINIAN)                 wxLANGUAGE_UKRAINIAN;
%rename(LANGUAGE_URDU)                      wxLANGUAGE_URDU;
%rename(LANGUAGE_URDU_INDIA)                wxLANGUAGE_URDU_INDIA;
%rename(LANGUAGE_URDU_PAKISTAN)             wxLANGUAGE_URDU_PAKISTAN;
%rename(LANGUAGE_UZBEK)                     wxLANGUAGE_UZBEK;
%rename(LANGUAGE_UZBEK_CYRILLIC)            wxLANGUAGE_UZBEK_CYRILLIC;
%rename(LANGUAGE_UZBEK_LATIN)               wxLANGUAGE_UZBEK_LATIN;
%rename(LANGUAGE_VIETNAMESE)                wxLANGUAGE_VIETNAMESE;
%rename(LANGUAGE_VOLAPUK)                   wxLANGUAGE_VOLAPUK;
%rename(LANGUAGE_WELSH)                     wxLANGUAGE_WELSH;
%rename(LANGUAGE_WOLOF)                     wxLANGUAGE_WOLOF;
%rename(LANGUAGE_XHOSA)                     wxLANGUAGE_XHOSA;
%rename(LANGUAGE_YIDDISH)                   wxLANGUAGE_YIDDISH;
%rename(LANGUAGE_YORUBA)                    wxLANGUAGE_YORUBA;
%rename(LANGUAGE_ZHUANG)                    wxLANGUAGE_ZHUANG;
%rename(LANGUAGE_ZULU)                      wxLANGUAGE_ZULU;
%rename(LANGUAGE_USER_DEFINED)              wxLANGUAGE_USER_DEFINED;
%rename(LanguageInfo)                       wxLanguageInfo;
%rename(LOCALE_CAT_NUMBER)                  wxLOCALE_CAT_NUMBER;
%rename(LOCALE_CAT_DATE)                    wxLOCALE_CAT_DATE;
%rename(LOCALE_CAT_MONEY)                   wxLOCALE_CAT_MONEY;
%rename(LOCALE_CAT_MAX)                     wxLOCALE_CAT_MAX;
%rename(LOCALE_THOUSANDS_SEP)               wxLOCALE_THOUSANDS_SEP;
%rename(LOCALE_DECIMAL_POINT)               wxLOCALE_DECIMAL_POINT;
%rename(LOCALE_LOAD_DEFAULT)                wxLOCALE_LOAD_DEFAULT;
%rename(LOCALE_CONV_ENCODING)               wxLOCALE_CONV_ENCODING;
%rename(Locale)                             wxLocale;
%rename(GetLocale)                          wxGetLocale;
%rename(GetTranslation)                     wxGetTranslation;
%rename(GetTranslation)                     wxGetTranslation;
%rename(CONVERT_STRICT)                     wxCONVERT_STRICT;
%rename(CONVERT_SUBSTITUTE)                 wxCONVERT_SUBSTITUTE;
%rename(PLATFORM_CURRENT)                   wxPLATFORM_CURRENT;
%rename(PLATFORM_UNIX)                      wxPLATFORM_UNIX;
%rename(PLATFORM_WINDOWS)                   wxPLATFORM_WINDOWS;
%rename(PLATFORM_OS2)                       wxPLATFORM_OS2;
%rename(PLATFORM_MAC)                       wxPLATFORM_MAC;
%rename(EncodingConverter)                  wxEncodingConverter;
%rename(DC)                                 wxDC;
%rename(MemoryDC)                           wxMemoryDC;
%rename(BUFFER_VIRTUAL_AREA)                wxBUFFER_VIRTUAL_AREA;
%rename(BUFFER_CLIENT_AREA)                 wxBUFFER_CLIENT_AREA;
%rename(BufferedDC)                         wxBufferedDC;
%rename(BufferedPaintDC)                    wxBufferedPaintDC;
%rename(ScreenDC)                           wxScreenDC;
%rename(ClientDC)                           wxClientDC;
%rename(PaintDC)                            wxPaintDC;
%rename(WindowDC)                           wxWindowDC;
%rename(MirrorDC)                           wxMirrorDC;
%rename(PostScriptDC)                       wxPostScriptDC;
%rename(MetaFile)                           wxMetaFile;
%rename(MetaFileDC)                         wxMetaFileDC;
%rename(PrinterDC)                          wxPrinterDC;
%rename(IMAGELIST_DRAW_NORMAL)              wxIMAGELIST_DRAW_NORMAL;
%rename(IMAGELIST_DRAW_TRANSPARENT)         wxIMAGELIST_DRAW_TRANSPARENT;
%rename(IMAGELIST_DRAW_SELECTED)            wxIMAGELIST_DRAW_SELECTED;
%rename(IMAGELIST_DRAW_FOCUSED)             wxIMAGELIST_DRAW_FOCUSED;
%rename(IMAGE_LIST_NORMAL)                  wxIMAGE_LIST_NORMAL;
%rename(IMAGE_LIST_SMALL)                   wxIMAGE_LIST_SMALL;
%rename(IMAGE_LIST_STATE)                   wxIMAGE_LIST_STATE;
%rename(ImageList)                          wxImageList;
%rename(StockGDI)                           wxStockGDI;
%rename(NullBitmap)                         wxNullBitmap;
%rename(NullIcon)                           wxNullIcon;
%rename(NullCursor)                         wxNullCursor;
%rename(NullPen)                            wxNullPen;
%rename(NullBrush)                          wxNullBrush;
%rename(NullPalette)                        wxNullPalette;
%rename(NullFont)                           wxNullFont;
%rename(NullColour)                         wxNullColour;
%rename(GDIObjListBase)                     wxGDIObjListBase;
%rename(PenList)                            wxPenList;
%rename(BrushList)                          wxBrushList;
%rename(FontList)                           wxFontList;
%rename(ColourDatabase)                     wxColourDatabase;
%rename(Effects)                            wxEffects;
%rename(CONTROL_DISABLED)                   wxCONTROL_DISABLED;
%rename(CONTROL_FOCUSED)                    wxCONTROL_FOCUSED;
%rename(CONTROL_PRESSED)                    wxCONTROL_PRESSED;
%rename(CONTROL_ISDEFAULT)                  wxCONTROL_ISDEFAULT;
%rename(CONTROL_ISSUBMENU)                  wxCONTROL_ISSUBMENU;
%rename(CONTROL_EXPANDED)                   wxCONTROL_EXPANDED;
%rename(CONTROL_CURRENT)                    wxCONTROL_CURRENT;
%rename(CONTROL_SELECTED)                   wxCONTROL_SELECTED;
%rename(CONTROL_CHECKED)                    wxCONTROL_CHECKED;
%rename(CONTROL_CHECKABLE)                  wxCONTROL_CHECKABLE;
%rename(CONTROL_UNDETERMINED)               wxCONTROL_UNDETERMINED;
%rename(CONTROL_FLAGS_MASK)                 wxCONTROL_FLAGS_MASK;
%rename(CONTROL_DIRTY)                      wxCONTROL_DIRTY;
%rename(SplitterRenderParams)               wxSplitterRenderParams;
%rename(RendererVersion)                    wxRendererVersion;
%rename(RendererNative)                     wxRendererNative;

#endif
