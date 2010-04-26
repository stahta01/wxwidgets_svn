// Scintilla source code edit control
/** @file Scintilla.h
 ** Interface to the edit control.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

// Most of this file is automatically generated from the Scintilla.iface interface definition
// file which contains any comments about the definitions. HFacer.py does the generation.

#ifndef SCINTILLA_H
#define SCINTILLA_H

#if defined(LCCWIN) && LCCWIN
typedef BOOL bool;
#endif

#if PLAT_WIN
// Return false on failure:
bool Scintilla_RegisterClasses(void *hInstance);
bool Scintilla_ReleaseResources();
#endif
int Scintilla_LinkLexers();

// Here should be placed typedefs for uptr_t, an unsigned integer type large enough to
// hold a pointer and sptr_t, a signed integer large enough to hold a pointer.
// May need to be changed for 64 bit platforms.
#if defined(_MSC_VER) && _MSC_VER >= 1300
#include <BaseTsd.h>
#endif
#ifdef MAXULONG_PTR
typedef ULONG_PTR uptr_t;
typedef LONG_PTR sptr_t;
#else
typedef unsigned long uptr_t;
typedef long sptr_t;
#endif

typedef sptr_t (*SciFnDirect)(sptr_t ptr, unsigned int iMessage, uptr_t wParam, sptr_t lParam);

//++Autogenerated -- start of section automatically generated from Scintilla.iface
#define INVALID_POSITION -1
#define SCI_START 2000
#define SCI_OPTIONAL_START 3000
#define SCI_LEXER_START 4000
#define SCI_ADDTEXT 2001
#define SCI_ADDSTYLEDTEXT 2002
#define SCI_INSERTTEXT 2003
#define SCI_CLEARALL 2004
#define SCI_CLEARDOCUMENTSTYLE 2005
#define SCI_GETLENGTH 2006
#define SCI_GETCHARAT 2007
#define SCI_GETCURRENTPOS 2008
#define SCI_GETANCHOR 2009
#define SCI_GETSTYLEAT 2010
#define SCI_REDO 2011
#define SCI_SETUNDOCOLLECTION 2012
#define SCI_SELECTALL 2013
#define SCI_SETSAVEPOINT 2014
#define SCI_GETSTYLEDTEXT 2015
#define SCI_CANREDO 2016
#define SCI_MARKERLINEFROMHANDLE 2017
#define SCI_MARKERDELETEHANDLE 2018
#define SCI_GETUNDOCOLLECTION 2019
#define SCWS_INVISIBLE 0
#define SCWS_VISIBLEALWAYS 1
#define SCWS_VISIBLEAFTERINDENT 2
#define SCI_GETVIEWWS 2020
#define SCI_SETVIEWWS 2021
#define SCI_POSITIONFROMPOINT 2022
#define SCI_POSITIONFROMPOINTCLOSE 2023
#define SCI_GOTOLINE 2024
#define SCI_GOTOPOS 2025
#define SCI_SETANCHOR 2026
#define SCI_GETCURLINE 2027
#define SCI_GETENDSTYLED 2028
#define SC_EOL_CRLF 0
#define SC_EOL_CR 1
#define SC_EOL_LF 2
#define SCI_CONVERTEOLS 2029
#define SCI_GETEOLMODE 2030
#define SCI_SETEOLMODE 2031
#define SCI_STARTSTYLING 2032
#define SCI_SETSTYLING 2033
#define SCI_GETBUFFEREDDRAW 2034
#define SCI_SETBUFFEREDDRAW 2035
#define SCI_SETTABWIDTH 2036
#define SCI_GETTABWIDTH 2121
#define SC_CP_UTF8 65001
#define SC_CP_DBCS 1
#define SCI_SETCODEPAGE 2037
#define SCI_SETUSEPALETTE 2039
#define MARKER_MAX 31
#define SC_MARK_CIRCLE 0
#define SC_MARK_ROUNDRECT 1
#define SC_MARK_ARROW 2
#define SC_MARK_SMALLRECT 3
#define SC_MARK_SHORTARROW 4
#define SC_MARK_EMPTY 5
#define SC_MARK_ARROWDOWN 6
#define SC_MARK_MINUS 7
#define SC_MARK_PLUS 8
#define SC_MARK_VLINE 9
#define SC_MARK_LCORNER 10
#define SC_MARK_TCORNER 11
#define SC_MARK_BOXPLUS 12
#define SC_MARK_BOXPLUSCONNECTED 13
#define SC_MARK_BOXMINUS 14
#define SC_MARK_BOXMINUSCONNECTED 15
#define SC_MARK_LCORNERCURVE 16
#define SC_MARK_TCORNERCURVE 17
#define SC_MARK_CIRCLEPLUS 18
#define SC_MARK_CIRCLEPLUSCONNECTED 19
#define SC_MARK_CIRCLEMINUS 20
#define SC_MARK_CIRCLEMINUSCONNECTED 21
#define SC_MARK_BACKGROUND 22
#define SC_MARK_DOTDOTDOT 23
#define SC_MARK_ARROWS 24
#define SC_MARK_PIXMAP 25
#define SC_MARK_FULLRECT 26
#define SC_MARK_CHARACTER 10000
#define SC_MARKNUM_FOLDEREND 25
#define SC_MARKNUM_FOLDEROPENMID 26
#define SC_MARKNUM_FOLDERMIDTAIL 27
#define SC_MARKNUM_FOLDERTAIL 28
#define SC_MARKNUM_FOLDERSUB 29
#define SC_MARKNUM_FOLDER 30
#define SC_MARKNUM_FOLDEROPEN 31
#define SC_MASK_FOLDERS 0xFE000000
#define SCI_MARKERDEFINE 2040
#define SCI_MARKERSETFORE 2041
#define SCI_MARKERSETBACK 2042
#define SCI_MARKERADD 2043
#define SCI_MARKERDELETE 2044
#define SCI_MARKERDELETEALL 2045
#define SCI_MARKERGET 2046
#define SCI_MARKERNEXT 2047
#define SCI_MARKERPREVIOUS 2048
#define SCI_MARKERDEFINEPIXMAP 2049
#define SCI_MARKERADDSET 2466
#define SCI_MARKERSETALPHA 2476
#define SC_MARGIN_SYMBOL 0
#define SC_MARGIN_NUMBER 1
#define SC_MARGIN_BACK 2
#define SC_MARGIN_FORE 3
#define SCI_SETMARGINTYPEN 2240
#define SCI_GETMARGINTYPEN 2241
#define SCI_SETMARGINWIDTHN 2242
#define SCI_GETMARGINWIDTHN 2243
#define SCI_SETMARGINMASKN 2244
#define SCI_GETMARGINMASKN 2245
#define SCI_SETMARGINSENSITIVEN 2246
#define SCI_GETMARGINSENSITIVEN 2247
#define STYLE_DEFAULT 32
#define STYLE_LINENUMBER 33
#define STYLE_BRACELIGHT 34
#define STYLE_BRACEBAD 35
#define STYLE_CONTROLCHAR 36
#define STYLE_INDENTGUIDE 37
#define STYLE_CALLTIP 38
#define STYLE_LASTPREDEFINED 39
#define STYLE_MAX 127
#define SC_CHARSET_ANSI 0
#define SC_CHARSET_DEFAULT 1
#define SC_CHARSET_BALTIC 186
#define SC_CHARSET_CHINESEBIG5 136
#define SC_CHARSET_EASTEUROPE 238
#define SC_CHARSET_GB2312 134
#define SC_CHARSET_GREEK 161
#define SC_CHARSET_HANGUL 129
#define SC_CHARSET_MAC 77
#define SC_CHARSET_OEM 255
#define SC_CHARSET_RUSSIAN 204
#define SC_CHARSET_CYRILLIC 1251
#define SC_CHARSET_SHIFTJIS 128
#define SC_CHARSET_SYMBOL 2
#define SC_CHARSET_TURKISH 162
#define SC_CHARSET_JOHAB 130
#define SC_CHARSET_HEBREW 177
#define SC_CHARSET_ARABIC 178
#define SC_CHARSET_VIETNAMESE 163
#define SC_CHARSET_THAI 222
#define SC_CHARSET_8859_15 1000
#define SCI_STYLECLEARALL 2050
#define SCI_STYLESETFORE 2051
#define SCI_STYLESETBACK 2052
#define SCI_STYLESETBOLD 2053
#define SCI_STYLESETITALIC 2054
#define SCI_STYLESETSIZE 2055
#define SCI_STYLESETFONT 2056
#define SCI_STYLESETEOLFILLED 2057
#define SCI_STYLERESETDEFAULT 2058
#define SCI_STYLESETUNDERLINE 2059
#define SC_CASE_MIXED 0
#define SC_CASE_UPPER 1
#define SC_CASE_LOWER 2
#define SCI_STYLESETCASE 2060
#define SCI_STYLESETCHARACTERSET 2066
#define SCI_STYLESETHOTSPOT 2409
#define SCI_SETSELFORE 2067
#define SCI_SETSELBACK 2068
#define SCI_GETSELALPHA 2477
#define SCI_SETSELALPHA 2478
#define SCI_SETCARETFORE 2069
#define SCI_ASSIGNCMDKEY 2070
#define SCI_CLEARCMDKEY 2071
#define SCI_CLEARALLCMDKEYS 2072
#define SCI_SETSTYLINGEX 2073
#define SCI_STYLESETVISIBLE 2074
#define SCI_GETCARETPERIOD 2075
#define SCI_SETCARETPERIOD 2076
#define SCI_SETWORDCHARS 2077
#define SCI_BEGINUNDOACTION 2078
#define SCI_ENDUNDOACTION 2079
#define INDIC_MAX 7
#define INDIC_PLAIN 0
#define INDIC_SQUIGGLE 1
#define INDIC_TT 2
#define INDIC_DIAGONAL 3
#define INDIC_STRIKE 4
#define INDIC_HIDDEN 5
#define INDIC_BOX 6
#define INDIC_ROUNDBOX 7
#define INDIC0_MASK 0x20
#define INDIC1_MASK 0x40
#define INDIC2_MASK 0x80
#define INDICS_MASK 0xE0
#define SCI_INDICSETSTYLE 2080
#define SCI_INDICGETSTYLE 2081
#define SCI_INDICSETFORE 2082
#define SCI_INDICGETFORE 2083
#define SCI_SETWHITESPACEFORE 2084
#define SCI_SETWHITESPACEBACK 2085
#define SCI_SETSTYLEBITS 2090
#define SCI_GETSTYLEBITS 2091
#define SCI_SETLINESTATE 2092
#define SCI_GETLINESTATE 2093
#define SCI_GETMAXLINESTATE 2094
#define SCI_GETCARETLINEVISIBLE 2095
#define SCI_SETCARETLINEVISIBLE 2096
#define SCI_GETCARETLINEBACK 2097
#define SCI_SETCARETLINEBACK 2098
#define SCI_STYLESETCHANGEABLE 2099
#define SCI_AUTOCSHOW 2100
#define SCI_AUTOCCANCEL 2101
#define SCI_AUTOCACTIVE 2102
#define SCI_AUTOCPOSSTART 2103
#define SCI_AUTOCCOMPLETE 2104
#define SCI_AUTOCSTOPS 2105
#define SCI_AUTOCSETSEPARATOR 2106
#define SCI_AUTOCGETSEPARATOR 2107
#define SCI_AUTOCSELECT 2108
#define SCI_AUTOCSETCANCELATSTART 2110
#define SCI_AUTOCGETCANCELATSTART 2111
#define SCI_AUTOCSETFILLUPS 2112
#define SCI_AUTOCSETCHOOSESINGLE 2113
#define SCI_AUTOCGETCHOOSESINGLE 2114
#define SCI_AUTOCSETIGNORECASE 2115
#define SCI_AUTOCGETIGNORECASE 2116
#define SCI_USERLISTSHOW 2117
#define SCI_AUTOCSETAUTOHIDE 2118
#define SCI_AUTOCGETAUTOHIDE 2119
#define SCI_AUTOCSETDROPRESTOFWORD 2270
#define SCI_AUTOCGETDROPRESTOFWORD 2271
#define SCI_REGISTERIMAGE 2405
#define SCI_CLEARREGISTEREDIMAGES 2408
#define SCI_AUTOCGETTYPESEPARATOR 2285
#define SCI_AUTOCSETTYPESEPARATOR 2286
#define SCI_AUTOCSETMAXWIDTH 2208
#define SCI_AUTOCGETMAXWIDTH 2209
#define SCI_AUTOCSETMAXHEIGHT 2210
#define SCI_AUTOCGETMAXHEIGHT 2211
#define SCI_SETINDENT 2122
#define SCI_GETINDENT 2123
#define SCI_SETUSETABS 2124
#define SCI_GETUSETABS 2125
#define SCI_SETLINEINDENTATION 2126
#define SCI_GETLINEINDENTATION 2127
#define SCI_GETLINEINDENTPOSITION 2128
#define SCI_GETCOLUMN 2129
#define SCI_SETHSCROLLBAR 2130
#define SCI_GETHSCROLLBAR 2131
#define SCI_SETINDENTATIONGUIDES 2132
#define SCI_GETINDENTATIONGUIDES 2133
#define SCI_SETHIGHLIGHTGUIDE 2134
#define SCI_GETHIGHLIGHTGUIDE 2135
#define SCI_GETLINEENDPOSITION 2136
#define SCI_GETCODEPAGE 2137
#define SCI_GETCARETFORE 2138
#define SCI_GETUSEPALETTE 2139
#define SCI_GETREADONLY 2140
#define SCI_SETCURRENTPOS 2141
#define SCI_SETSELECTIONSTART 2142
#define SCI_GETSELECTIONSTART 2143
#define SCI_SETSELECTIONEND 2144
#define SCI_GETSELECTIONEND 2145
#define SCI_SETPRINTMAGNIFICATION 2146
#define SCI_GETPRINTMAGNIFICATION 2147
#define SC_PRINT_NORMAL 0
#define SC_PRINT_INVERTLIGHT 1
#define SC_PRINT_BLACKONWHITE 2
#define SC_PRINT_COLOURONWHITE 3
#define SC_PRINT_COLOURONWHITEDEFAULTBG 4
#define SCI_SETPRINTCOLOURMODE 2148
#define SCI_GETPRINTCOLOURMODE 2149
#define SCFIND_WHOLEWORD 2
#define SCFIND_MATCHCASE 4
#define SCFIND_WORDSTART 0x00100000
#define SCFIND_REGEXP 0x00200000
#define SCFIND_POSIX 0x00400000
#define SCI_FINDTEXT 2150
#define SCI_FORMATRANGE 2151
#define SCI_GETFIRSTVISIBLELINE 2152
#define SCI_GETLINE 2153
#define SCI_GETLINECOUNT 2154
#define SCI_SETMARGINLEFT 2155
#define SCI_GETMARGINLEFT 2156
#define SCI_SETMARGINRIGHT 2157
#define SCI_GETMARGINRIGHT 2158
#define SCI_GETMODIFY 2159
#define SCI_SETSEL 2160
#define SCI_GETSELTEXT 2161
#define SCI_GETTEXTRANGE 2162
#define SCI_HIDESELECTION 2163
#define SCI_POINTXFROMPOSITION 2164
#define SCI_POINTYFROMPOSITION 2165
#define SCI_LINEFROMPOSITION 2166
#define SCI_POSITIONFROMLINE 2167
#define SCI_LINESCROLL 2168
#define SCI_SCROLLCARET 2169
#define SCI_REPLACESEL 2170
#define SCI_SETREADONLY 2171
#define SCI_NULL 2172
#define SCI_CANPASTE 2173
#define SCI_CANUNDO 2174
#define SCI_EMPTYUNDOBUFFER 2175
#define SCI_UNDO 2176
#define SCI_CUT 2177
#define SCI_COPY 2178
#define SCI_PASTE 2179
#define SCI_CLEAR 2180
#define SCI_SETTEXT 2181
#define SCI_GETTEXT 2182
#define SCI_GETTEXTLENGTH 2183
#define SCI_GETDIRECTFUNCTION 2184
#define SCI_GETDIRECTPOINTER 2185
#define SCI_SETOVERTYPE 2186
#define SCI_GETOVERTYPE 2187
#define SCI_SETCARETWIDTH 2188
#define SCI_GETCARETWIDTH 2189
#define SCI_SETTARGETSTART 2190
#define SCI_GETTARGETSTART 2191
#define SCI_SETTARGETEND 2192
#define SCI_GETTARGETEND 2193
#define SCI_REPLACETARGET 2194
#define SCI_REPLACETARGETRE 2195
#define SCI_SEARCHINTARGET 2197
#define SCI_SETSEARCHFLAGS 2198
#define SCI_GETSEARCHFLAGS 2199
#define SCI_CALLTIPSHOW 2200
#define SCI_CALLTIPCANCEL 2201
#define SCI_CALLTIPACTIVE 2202
#define SCI_CALLTIPPOSSTART 2203
#define SCI_CALLTIPSETHLT 2204
#define SCI_CALLTIPSETBACK 2205
#define SCI_CALLTIPSETFORE 2206
#define SCI_CALLTIPSETFOREHLT 2207
#define SCI_CALLTIPUSESTYLE 2212
#define SCI_VISIBLEFROMDOCLINE 2220
#define SCI_DOCLINEFROMVISIBLE 2221
#define SCI_WRAPCOUNT 2235
#define SC_FOLDLEVELBASE 0x400
#define SC_FOLDLEVELWHITEFLAG 0x1000
#define SC_FOLDLEVELHEADERFLAG 0x2000
#define SC_FOLDLEVELBOXHEADERFLAG 0x4000
#define SC_FOLDLEVELBOXFOOTERFLAG 0x8000
#define SC_FOLDLEVELCONTRACTED 0x10000
#define SC_FOLDLEVELUNINDENT 0x20000
#define SC_FOLDLEVELNUMBERMASK 0x0FFF
#define SCI_SETFOLDLEVEL 2222
#define SCI_GETFOLDLEVEL 2223
#define SCI_GETLASTCHILD 2224
#define SCI_GETFOLDPARENT 2225
#define SCI_SHOWLINES 2226
#define SCI_HIDELINES 2227
#define SCI_GETLINEVISIBLE 2228
#define SCI_SETFOLDEXPANDED 2229
#define SCI_GETFOLDEXPANDED 2230
#define SCI_TOGGLEFOLD 2231
#define SCI_ENSUREVISIBLE 2232
#define SC_FOLDFLAG_LINEBEFORE_EXPANDED 0x0002
#define SC_FOLDFLAG_LINEBEFORE_CONTRACTED 0x0004
#define SC_FOLDFLAG_LINEAFTER_EXPANDED 0x0008
#define SC_FOLDFLAG_LINEAFTER_CONTRACTED 0x0010
#define SC_FOLDFLAG_LEVELNUMBERS 0x0040
#define SC_FOLDFLAG_BOX 0x0001
#define SCI_SETFOLDFLAGS 2233
#define SCI_ENSUREVISIBLEENFORCEPOLICY 2234
#define SCI_SETTABINDENTS 2260
#define SCI_GETTABINDENTS 2261
#define SCI_SETBACKSPACEUNINDENTS 2262
#define SCI_GETBACKSPACEUNINDENTS 2263
#define SC_TIME_FOREVER 10000000
#define SCI_SETMOUSEDWELLTIME 2264
#define SCI_GETMOUSEDWELLTIME 2265
#define SCI_WORDSTARTPOSITION 2266
#define SCI_WORDENDPOSITION 2267
#define SC_WRAP_NONE 0
#define SC_WRAP_WORD 1
#define SC_WRAP_CHAR 2
#define SCI_SETWRAPMODE 2268
#define SCI_GETWRAPMODE 2269
#define SC_WRAPVISUALFLAG_NONE 0x0000
#define SC_WRAPVISUALFLAG_END 0x0001
#define SC_WRAPVISUALFLAG_START 0x0002
#define SCI_SETWRAPVISUALFLAGS 2460
#define SCI_GETWRAPVISUALFLAGS 2461
#define SC_WRAPVISUALFLAGLOC_DEFAULT 0x0000
#define SC_WRAPVISUALFLAGLOC_END_BY_TEXT 0x0001
#define SC_WRAPVISUALFLAGLOC_START_BY_TEXT 0x0002
#define SCI_SETWRAPVISUALFLAGSLOCATION 2462
#define SCI_GETWRAPVISUALFLAGSLOCATION 2463
#define SCI_SETWRAPSTARTINDENT 2464
#define SCI_GETWRAPSTARTINDENT 2465
#define SC_CACHE_NONE 0
#define SC_CACHE_CARET 1
#define SC_CACHE_PAGE 2
#define SC_CACHE_DOCUMENT 3
#define SCI_SETLAYOUTCACHE 2272
#define SCI_GETLAYOUTCACHE 2273
#define SCI_SETSCROLLWIDTH 2274
#define SCI_GETSCROLLWIDTH 2275
#define SCI_TEXTWIDTH 2276
#define SCI_SETENDATLASTLINE 2277
#define SCI_GETENDATLASTLINE 2278
#define SCI_TEXTHEIGHT 2279
#define SCI_SETVSCROLLBAR 2280
#define SCI_GETVSCROLLBAR 2281
#define SCI_APPENDTEXT 2282
#define SCI_GETTWOPHASEDRAW 2283
#define SCI_SETTWOPHASEDRAW 2284
#define SCI_TARGETFROMSELECTION 2287
#define SCI_LINESJOIN 2288
#define SCI_LINESSPLIT 2289
#define SCI_SETFOLDMARGINCOLOUR 2290
#define SCI_SETFOLDMARGINHICOLOUR 2291
#define SCI_LINEDOWN 2300
#define SCI_LINEDOWNEXTEND 2301
#define SCI_LINEUP 2302
#define SCI_LINEUPEXTEND 2303
#define SCI_CHARLEFT 2304
#define SCI_CHARLEFTEXTEND 2305
#define SCI_CHARRIGHT 2306
#define SCI_CHARRIGHTEXTEND 2307
#define SCI_WORDLEFT 2308
#define SCI_WORDLEFTEXTEND 2309
#define SCI_WORDRIGHT 2310
#define SCI_WORDRIGHTEXTEND 2311
#define SCI_HOME 2312
#define SCI_HOMEEXTEND 2313
#define SCI_LINEEND 2314
#define SCI_LINEENDEXTEND 2315
#define SCI_DOCUMENTSTART 2316
#define SCI_DOCUMENTSTARTEXTEND 2317
#define SCI_DOCUMENTEND 2318
#define SCI_DOCUMENTENDEXTEND 2319
#define SCI_PAGEUP 2320
#define SCI_PAGEUPEXTEND 2321
#define SCI_PAGEDOWN 2322
#define SCI_PAGEDOWNEXTEND 2323
#define SCI_EDITTOGGLEOVERTYPE 2324
#define SCI_CANCEL 2325
#define SCI_DELETEBACK 2326
#define SCI_TAB 2327
#define SCI_BACKTAB 2328
#define SCI_NEWLINE 2329
#define SCI_FORMFEED 2330
#define SCI_VCHOME 2331
#define SCI_VCHOMEEXTEND 2332
#define SCI_ZOOMIN 2333
#define SCI_ZOOMOUT 2334
#define SCI_DELWORDLEFT 2335
#define SCI_DELWORDRIGHT 2336
#define SCI_LINECUT 2337
#define SCI_LINEDELETE 2338
#define SCI_LINETRANSPOSE 2339
#define SCI_LINEDUPLICATE 2404
#define SCI_LOWERCASE 2340
#define SCI_UPPERCASE 2341
#define SCI_LINESCROLLDOWN 2342
#define SCI_LINESCROLLUP 2343
#define SCI_DELETEBACKNOTLINE 2344
#define SCI_HOMEDISPLAY 2345
#define SCI_HOMEDISPLAYEXTEND 2346
#define SCI_LINEENDDISPLAY 2347
#define SCI_LINEENDDISPLAYEXTEND 2348
#define SCI_HOMEWRAP 2349
#define SCI_HOMEWRAPEXTEND 2450
#define SCI_LINEENDWRAP 2451
#define SCI_LINEENDWRAPEXTEND 2452
#define SCI_VCHOMEWRAP 2453
#define SCI_VCHOMEWRAPEXTEND 2454
#define SCI_LINECOPY 2455
#define SCI_MOVECARETINSIDEVIEW 2401
#define SCI_LINELENGTH 2350
#define SCI_BRACEHIGHLIGHT 2351
#define SCI_BRACEBADLIGHT 2352
#define SCI_BRACEMATCH 2353
#define SCI_GETVIEWEOL 2355
#define SCI_SETVIEWEOL 2356
#define SCI_GETDOCPOINTER 2357
#define SCI_SETDOCPOINTER 2358
#define SCI_SETMODEVENTMASK 2359
#define EDGE_NONE 0
#define EDGE_LINE 1
#define EDGE_BACKGROUND 2
#define SCI_GETEDGECOLUMN 2360
#define SCI_SETEDGECOLUMN 2361
#define SCI_GETEDGEMODE 2362
#define SCI_SETEDGEMODE 2363
#define SCI_GETEDGECOLOUR 2364
#define SCI_SETEDGECOLOUR 2365
#define SCI_SEARCHANCHOR 2366
#define SCI_SEARCHNEXT 2367
#define SCI_SEARCHPREV 2368
#define SCI_LINESONSCREEN 2370
#define SCI_USEPOPUP 2371
#define SCI_SELECTIONISRECTANGLE 2372
#define SCI_SETZOOM 2373
#define SCI_GETZOOM 2374
#define SCI_CREATEDOCUMENT 2375
#define SCI_ADDREFDOCUMENT 2376
#define SCI_RELEASEDOCUMENT 2377
#define SCI_GETMODEVENTMASK 2378
#define SCI_SETFOCUS 2380
#define SCI_GETFOCUS 2381
#define SCI_SETSTATUS 2382
#define SCI_GETSTATUS 2383
#define SCI_SETMOUSEDOWNCAPTURES 2384
#define SCI_GETMOUSEDOWNCAPTURES 2385
#define SC_CURSORNORMAL -1
#define SC_CURSORWAIT 4
#define SCI_SETCURSOR 2386
#define SCI_GETCURSOR 2387
#define SCI_SETCONTROLCHARSYMBOL 2388
#define SCI_GETCONTROLCHARSYMBOL 2389
#define SCI_WORDPARTLEFT 2390
#define SCI_WORDPARTLEFTEXTEND 2391
#define SCI_WORDPARTRIGHT 2392
#define SCI_WORDPARTRIGHTEXTEND 2393
#define VISIBLE_SLOP 0x01
#define VISIBLE_STRICT 0x04
#define SCI_SETVISIBLEPOLICY 2394
#define SCI_DELLINELEFT 2395
#define SCI_DELLINERIGHT 2396
#define SCI_SETXOFFSET 2397
#define SCI_GETXOFFSET 2398
#define SCI_CHOOSECARETX 2399
#define SCI_GRABFOCUS 2400
#define CARET_SLOP 0x01
#define CARET_STRICT 0x04
#define CARET_JUMPS 0x10
#define CARET_EVEN 0x08
#define SCI_SETXCARETPOLICY 2402
#define SCI_SETYCARETPOLICY 2403
#define SCI_SETPRINTWRAPMODE 2406
#define SCI_GETPRINTWRAPMODE 2407
#define SCI_SETHOTSPOTACTIVEFORE 2410
#define SCI_SETHOTSPOTACTIVEBACK 2411
#define SCI_SETHOTSPOTACTIVEUNDERLINE 2412
#define SCI_SETHOTSPOTSINGLELINE 2421
#define SCI_PARADOWN 2413
#define SCI_PARADOWNEXTEND 2414
#define SCI_PARAUP 2415
#define SCI_PARAUPEXTEND 2416
#define SCI_POSITIONBEFORE 2417
#define SCI_POSITIONAFTER 2418
#define SCI_COPYRANGE 2419
#define SCI_COPYTEXT 2420
#define SC_SEL_STREAM 0
#define SC_SEL_RECTANGLE 1
#define SC_SEL_LINES 2
#define SCI_SETSELECTIONMODE 2422
#define SCI_GETSELECTIONMODE 2423
#define SCI_GETLINESELSTARTPOSITION 2424
#define SCI_GETLINESELENDPOSITION 2425
#define SCI_LINEDOWNRECTEXTEND 2426
#define SCI_LINEUPRECTEXTEND 2427
#define SCI_CHARLEFTRECTEXTEND 2428
#define SCI_CHARRIGHTRECTEXTEND 2429
#define SCI_HOMERECTEXTEND 2430
#define SCI_VCHOMERECTEXTEND 2431
#define SCI_LINEENDRECTEXTEND 2432
#define SCI_PAGEUPRECTEXTEND 2433
#define SCI_PAGEDOWNRECTEXTEND 2434
#define SCI_STUTTEREDPAGEUP 2435
#define SCI_STUTTEREDPAGEUPEXTEND 2436
#define SCI_STUTTEREDPAGEDOWN 2437
#define SCI_STUTTEREDPAGEDOWNEXTEND 2438
#define SCI_WORDLEFTEND 2439
#define SCI_WORDLEFTENDEXTEND 2440
#define SCI_WORDRIGHTEND 2441
#define SCI_WORDRIGHTENDEXTEND 2442
#define SCI_SETWHITESPACECHARS 2443
#define SCI_SETCHARSDEFAULT 2444
#define SCI_AUTOCGETCURRENT 2445
#define SCI_ALLOCATE 2446
#define SCI_TARGETASUTF8 2447
#define SCI_SETLENGTHFORENCODE 2448
#define SCI_ENCODEDFROMUTF8 2449
#define SCI_FINDCOLUMN 2456
#define SCI_GETCARETSTICKY 2457
#define SCI_SETCARETSTICKY 2458
#define SCI_TOGGLECARETSTICKY 2459
#define SCI_SETPASTECONVERTENDINGS 2467
#define SCI_GETPASTECONVERTENDINGS 2468
#define SCI_SELECTIONDUPLICATE 2469
#define SC_ALPHA_TRANSPARENT 0
#define SC_ALPHA_OPAQUE 255
#define SC_ALPHA_NOALPHA 256
#define SCI_SETCARETLINEBACKALPHA 2470
#define SCI_GETCARETLINEBACKALPHA 2471
#define SCI_STARTRECORD 3001
#define SCI_STOPRECORD 3002
#define SCI_SETLEXER 4001
#define SCI_GETLEXER 4002
#define SCI_COLOURISE 4003
#define SCI_SETPROPERTY 4004
#define KEYWORDSET_MAX 8
#define SCI_SETKEYWORDS 4005
#define SCI_SETLEXERLANGUAGE 4006
#define SCI_LOADLEXERLIBRARY 4007
#define SCI_GETPROPERTY 4008
#define SCI_GETPROPERTYEXPANDED 4009
#define SCI_GETPROPERTYINT 4010
#define SCI_GETSTYLEBITSNEEDED 4011
#define SC_MOD_INSERTTEXT 0x1
#define SC_MOD_DELETETEXT 0x2
#define SC_MOD_CHANGESTYLE 0x4
#define SC_MOD_CHANGEFOLD 0x8
#define SC_PERFORMED_USER 0x10
#define SC_PERFORMED_UNDO 0x20
#define SC_PERFORMED_REDO 0x40
#define SC_MULTISTEPUNDOREDO 0x80
#define SC_LASTSTEPINUNDOREDO 0x100
#define SC_MOD_CHANGEMARKER 0x200
#define SC_MOD_BEFOREINSERT 0x400
#define SC_MOD_BEFOREDELETE 0x800
#define SC_MULTILINEUNDOREDO 0x1000
#define SC_MODEVENTMASKALL 0x1FFF
#define SCEN_CHANGE 768
#define SCEN_SETFOCUS 512
#define SCEN_KILLFOCUS 256
#define SCK_DOWN 300
#define SCK_UP 301
#define SCK_LEFT 302
#define SCK_RIGHT 303
#define SCK_HOME 304
#define SCK_END 305
#define SCK_PRIOR 306
#define SCK_NEXT 307
#define SCK_DELETE 308
#define SCK_INSERT 309
#define SCK_ESCAPE 7
#define SCK_BACK 8
#define SCK_TAB 9
#define SCK_RETURN 13
#define SCK_ADD 310
#define SCK_SUBTRACT 311
#define SCK_DIVIDE 312
#define SCMOD_NORM 0
#define SCMOD_SHIFT 1
#define SCMOD_CTRL 2
#define SCMOD_ALT 4
#define SCN_STYLENEEDED 2000
#define SCN_CHARADDED 2001
#define SCN_SAVEPOINTREACHED 2002
#define SCN_SAVEPOINTLEFT 2003
#define SCN_MODIFYATTEMPTRO 2004
#define SCN_KEY 2005
#define SCN_DOUBLECLICK 2006
#define SCN_UPDATEUI 2007
#define SCN_MODIFIED 2008
#define SCN_MACRORECORD 2009
#define SCN_MARGINCLICK 2010
#define SCN_NEEDSHOWN 2011
#define SCN_PAINTED 2013
#define SCN_USERLISTSELECTION 2014
#define SCN_URIDROPPED 2015
#define SCN_DWELLSTART 2016
#define SCN_DWELLEND 2017
#define SCN_ZOOM 2018
#define SCN_HOTSPOTCLICK 2019
#define SCN_HOTSPOTDOUBLECLICK 2020
#define SCN_CALLTIPCLICK 2021
#define SCN_AUTOCSELECTION 2022
//--Autogenerated -- end of section automatically generated from Scintilla.iface

// These structures are defined to be exactly the same shape as the Win32
// CHARRANGE, TEXTRANGE, FINDTEXTEX, FORMATRANGE, and NMHDR structs.
// So older code that treats Scintilla as a RichEdit will work.

struct CharacterRange {
	long cpMin;
	long cpMax;
};

struct TextRange {
	struct CharacterRange chrg;
	char *lpstrText;
};

struct TextToFind {
	struct CharacterRange chrg;
	char *lpstrText;
	struct CharacterRange chrgText;
};

#ifdef PLATFORM_H

// This structure is used in printing and requires some of the graphics types
// from Platform.h.  Not needed by most client code.

struct RangeToFormat {
	SurfaceID hdc;
	SurfaceID hdcTarget;
	PRectangle rc;
	PRectangle rcPage;
	CharacterRange chrg;
};

#endif

struct NotifyHeader {
	// Compatible with Windows NMHDR.
	// hwndFrom is really an environment specific window handle or pointer
	// but most clients of Scintilla.h do not have this type visible.
	void *hwndFrom;
	uptr_t idFrom;
	unsigned int code;
};

struct SCNotification {
	struct NotifyHeader nmhdr;
	int position;	// SCN_STYLENEEDED, SCN_MODIFIED, SCN_DWELLSTART, SCN_DWELLEND
	int ch;		// SCN_CHARADDED, SCN_KEY
	int modifiers;	// SCN_KEY
	int modificationType;	// SCN_MODIFIED
	const char *text;	// SCN_MODIFIED, SCN_USERLISTSELECTION, SCN_AUTOCSELECTION
	int length;		// SCN_MODIFIED
	int linesAdded;	// SCN_MODIFIED
	int message;	// SCN_MACRORECORD
	uptr_t wParam;	// SCN_MACRORECORD
	sptr_t lParam;	// SCN_MACRORECORD
	int line;		// SCN_MODIFIED
	int foldLevelNow;	// SCN_MODIFIED
	int foldLevelPrev;	// SCN_MODIFIED
	int margin;		// SCN_MARGINCLICK
	int listType;	// SCN_USERLISTSELECTION
	int x;			// SCN_DWELLSTART, SCN_DWELLEND
	int y;		// SCN_DWELLSTART, SCN_DWELLEND
};

// Deprecation section listing all API features that are deprecated and will
// will be removed completely in a future version.
// To enable these features define INCLUDE_DEPRECATED_FEATURES

#ifdef INCLUDE_DEPRECATED_FEATURES

#define SCI_SETCARETPOLICY 2369
#define CARET_CENTER 0x02
#define CARET_XEVEN 0x08
#define CARET_XJUMPS 0x10

#define SCN_POSCHANGED 2012
#define SCN_CHECKBRACE 2007

#endif

#endif
