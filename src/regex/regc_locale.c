/* 
 * regc_locale.c --
 *
 *	This file contains the Unicode locale specific regexp routines.
 *	This file is #included by regcomp.c.
 *
 * Copyright (c) 1998 by Scriptics Corporation.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id$
 */

/* ASCII character-name table */

static struct cname {
    char *name;
    char code;
} cnames[] = {
    {"NUL",		'\0'},
    {"SOH",		'\001'},
    {"STX",		'\002'},
    {"ETX",		'\003'},
    {"EOT",		'\004'},
    {"ENQ",		'\005'},
    {"ACK",		'\006'},
    {"BEL",		'\007'},
    {"alert",		'\007'},
    {"BS",		'\010'},
    {"backspace",	'\b'},
    {"HT",		'\011'},
    {"tab",		'\t'},
    {"LF",		'\012'},
    {"newline",		'\n'},
    {"VT",		'\013'},
    {"vertical-tab",	'\v'},
    {"FF",		'\014'},
    {"form-feed",	'\f'},
    {"CR",		'\015'},
    {"carriage-return",	'\r'},
    {"SO",		'\016'},
    {"SI",		'\017'},
    {"DLE",		'\020'},
    {"DC1",		'\021'},
    {"DC2",		'\022'},
    {"DC3",		'\023'},
    {"DC4",		'\024'},
    {"NAK",		'\025'},
    {"SYN",		'\026'},
    {"ETB",		'\027'},
    {"CAN",		'\030'},
    {"EM",		'\031'},
    {"SUB",		'\032'},
    {"ESC",		'\033'},
    {"IS4",		'\034'},
    {"FS",		'\034'},
    {"IS3",		'\035'},
    {"GS",		'\035'},
    {"IS2",		'\036'},
    {"RS",		'\036'},
    {"IS1",		'\037'},
    {"US",		'\037'},
    {"space",		' '},
    {"exclamation-mark",'!'},
    {"quotation-mark",	'"'},
    {"number-sign",	'#'},
    {"dollar-sign",	'$'},
    {"percent-sign",	'%'},
    {"ampersand",	'&'},
    {"apostrophe",	'\''},
    {"left-parenthesis",'('},
    {"right-parenthesis", ')'},
    {"asterisk",	'*'},
    {"plus-sign",	'+'},
    {"comma",		','},
    {"hyphen",		'-'},
    {"hyphen-minus",	'-'},
    {"period",		'.'},
    {"full-stop",	'.'},
    {"slash",		'/'},
    {"solidus",		'/'},
    {"zero",		'0'},
    {"one",		'1'},
    {"two",		'2'},
    {"three",		'3'},
    {"four",		'4'},
    {"five",		'5'},
    {"six",		'6'},
    {"seven",		'7'},
    {"eight",		'8'},
    {"nine",		'9'},
    {"colon",		':'},
    {"semicolon",	';'},
    {"less-than-sign",	'<'},
    {"equals-sign",	'='},
    {"greater-than-sign", '>'},
    {"question-mark",	'?'},
    {"commercial-at",	'@'},
    {"left-square-bracket", '['},
    {"backslash",	'\\'},
    {"reverse-solidus",	'\\'},
    {"right-square-bracket", ']'},
    {"circumflex",	'^'},
    {"circumflex-accent", '^'},
    {"underscore",	'_'},
    {"low-line",	'_'},
    {"grave-accent",	'`'},
    {"left-brace",	'{'},
    {"left-curly-bracket", '{'},
    {"vertical-line",	'|'},
    {"right-brace",	'}'},
    {"right-curly-bracket", '}'},
    {"tilde",		'~'},
    {"DEL",		'\177'},
    {NULL,		0}
};

/* Unicode character-class tables */

typedef struct crange {
    chr start;
    chr end;
} crange;

/*
 *	Declarations of Unicode character ranges.  This code
 *	is automatically generated by the tools/uniClass.tcl script
 *	and used in generic/regc_locale.c.  Do not modify by hand.
 */

/* Unicode: alphabetic characters */

static crange alphaRangeTable[] = {
    {0x0041, 0x005a}, {0x0061, 0x007a}, {0x00c0, 0x00d6}, {0x00d8, 0x00f6}, 
    {0x00f8, 0x021f}, {0x0222, 0x0233}, {0x0250, 0x02ad}, {0x02b0, 0x02b8}, 
    {0x02bb, 0x02c1}, {0x02e0, 0x02e4}, {0x0388, 0x038a}, {0x038e, 0x03a1}, 
    {0x03a3, 0x03ce}, {0x03d0, 0x03d7}, {0x03da, 0x03f5}, {0x0400, 0x0481}, 
    {0x048c, 0x04c4}, {0x04d0, 0x04f5}, {0x0531, 0x0556}, {0x0561, 0x0587}, 
    {0x05d0, 0x05ea}, {0x05f0, 0x05f2}, {0x0621, 0x063a}, {0x0640, 0x064a}, 
    {0x0671, 0x06d3}, {0x06fa, 0x06fc}, {0x0712, 0x072c}, {0x0780, 0x07a5}, 
    {0x0905, 0x0939}, {0x0958, 0x0961}, {0x0985, 0x098c}, {0x0993, 0x09a8}, 
    {0x09aa, 0x09b0}, {0x09b6, 0x09b9}, {0x09df, 0x09e1}, {0x0a05, 0x0a0a}, 
    {0x0a13, 0x0a28}, {0x0a2a, 0x0a30}, {0x0a59, 0x0a5c}, {0x0a72, 0x0a74}, 
    {0x0a85, 0x0a8b}, {0x0a8f, 0x0a91}, {0x0a93, 0x0aa8}, {0x0aaa, 0x0ab0}, 
    {0x0ab5, 0x0ab9}, {0x0b05, 0x0b0c}, {0x0b13, 0x0b28}, {0x0b2a, 0x0b30}, 
    {0x0b36, 0x0b39}, {0x0b5f, 0x0b61}, {0x0b85, 0x0b8a}, {0x0b8e, 0x0b90}, 
    {0x0b92, 0x0b95}, {0x0ba8, 0x0baa}, {0x0bae, 0x0bb5}, {0x0bb7, 0x0bb9}, 
    {0x0c05, 0x0c0c}, {0x0c0e, 0x0c10}, {0x0c12, 0x0c28}, {0x0c2a, 0x0c33}, 
    {0x0c35, 0x0c39}, {0x0c85, 0x0c8c}, {0x0c8e, 0x0c90}, {0x0c92, 0x0ca8}, 
    {0x0caa, 0x0cb3}, {0x0cb5, 0x0cb9}, {0x0d05, 0x0d0c}, {0x0d0e, 0x0d10}, 
    {0x0d12, 0x0d28}, {0x0d2a, 0x0d39}, {0x0d85, 0x0d96}, {0x0d9a, 0x0db1}, 
    {0x0db3, 0x0dbb}, {0x0dc0, 0x0dc6}, {0x0e01, 0x0e30}, {0x0e40, 0x0e46}, 
    {0x0e94, 0x0e97}, {0x0e99, 0x0e9f}, {0x0ea1, 0x0ea3}, {0x0ead, 0x0eb0}, 
    {0x0ec0, 0x0ec4}, {0x0f40, 0x0f47}, {0x0f49, 0x0f6a}, {0x0f88, 0x0f8b}, 
    {0x1000, 0x1021}, {0x1023, 0x1027}, {0x1050, 0x1055}, {0x10a0, 0x10c5}, 
    {0x10d0, 0x10f6}, {0x1100, 0x1159}, {0x115f, 0x11a2}, {0x11a8, 0x11f9}, 
    {0x1200, 0x1206}, {0x1208, 0x1246}, {0x124a, 0x124d}, {0x1250, 0x1256}, 
    {0x125a, 0x125d}, {0x1260, 0x1286}, {0x128a, 0x128d}, {0x1290, 0x12ae}, 
    {0x12b2, 0x12b5}, {0x12b8, 0x12be}, {0x12c2, 0x12c5}, {0x12c8, 0x12ce}, 
    {0x12d0, 0x12d6}, {0x12d8, 0x12ee}, {0x12f0, 0x130e}, {0x1312, 0x1315}, 
    {0x1318, 0x131e}, {0x1320, 0x1346}, {0x1348, 0x135a}, {0x13a0, 0x13f4}, 
    {0x1401, 0x166c}, {0x166f, 0x1676}, {0x1681, 0x169a}, {0x16a0, 0x16ea}, 
    {0x1780, 0x17b3}, {0x1820, 0x1877}, {0x1880, 0x18a8}, {0x1e00, 0x1e9b}, 
    {0x1ea0, 0x1ef9}, {0x1f00, 0x1f15}, {0x1f18, 0x1f1d}, {0x1f20, 0x1f45}, 
    {0x1f48, 0x1f4d}, {0x1f50, 0x1f57}, {0x1f5f, 0x1f7d}, {0x1f80, 0x1fb4}, 
    {0x1fb6, 0x1fbc}, {0x1fc2, 0x1fc4}, {0x1fc6, 0x1fcc}, {0x1fd0, 0x1fd3}, 
    {0x1fd6, 0x1fdb}, {0x1fe0, 0x1fec}, {0x1ff2, 0x1ff4}, {0x1ff6, 0x1ffc}, 
    {0x210a, 0x2113}, {0x2119, 0x211d}, {0x212a, 0x212d}, {0x212f, 0x2131}, 
    {0x2133, 0x2139}, {0x3031, 0x3035}, {0x3041, 0x3094}, {0x30a1, 0x30fa}, 
    {0x30fc, 0x30fe}, {0x3105, 0x312c}, {0x3131, 0x318e}, {0x31a0, 0x31b7}, 
    {0x3400, 0x4db5}, {0x4e00, 0x9fa5}, {0xa000, 0xa48c}, {0xac00, 0xd7a3}, 
    {0xf900, 0xfa2d}, {0xfb00, 0xfb06}, {0xfb13, 0xfb17}, {0xfb1f, 0xfb28}, 
    {0xfb2a, 0xfb36}, {0xfb38, 0xfb3c}, {0xfb46, 0xfbb1}, {0xfbd3, 0xfd3d}, 
    {0xfd50, 0xfd8f}, {0xfd92, 0xfdc7}, {0xfdf0, 0xfdfb}, {0xfe70, 0xfe72}, 
    {0xfe76, 0xfefc}, {0xff21, 0xff3a}, {0xff41, 0xff5a}, {0xff66, 0xffbe}, 
    {0xffc2, 0xffc7}, {0xffca, 0xffcf}, {0xffd2, 0xffd7}, {0xffda, 0xffdc}
};

#define NUM_ALPHA_RANGE (sizeof(alphaRangeTable)/sizeof(crange))

static chr alphaCharTable[] = {
    0x00aa, 0x00b5, 0x00ba, 0x02d0, 0x02d1, 0x02ee, 0x037a, 0x0386, 0x038c, 
    0x04c7, 0x04c8, 0x04cb, 0x04cc, 0x04f8, 0x04f9, 0x0559, 0x06d5, 0x06e5, 
    0x06e6, 0x0710, 0x093d, 0x0950, 0x098f, 0x0990, 0x09b2, 0x09dc, 0x09dd, 
    0x09f0, 0x09f1, 0x0a0f, 0x0a10, 0x0a32, 0x0a33, 0x0a35, 0x0a36, 0x0a38, 
    0x0a39, 0x0a5e, 0x0a8d, 0x0ab2, 0x0ab3, 0x0abd, 0x0ad0, 0x0ae0, 0x0b0f, 
    0x0b10, 0x0b32, 0x0b33, 0x0b3d, 0x0b5c, 0x0b5d, 0x0b99, 0x0b9a, 0x0b9c, 
    0x0b9e, 0x0b9f, 0x0ba3, 0x0ba4, 0x0c60, 0x0c61, 0x0cde, 0x0ce0, 0x0ce1, 
    0x0d60, 0x0d61, 0x0dbd, 0x0e32, 0x0e33, 0x0e81, 0x0e82, 0x0e84, 0x0e87, 
    0x0e88, 0x0e8a, 0x0e8d, 0x0ea5, 0x0ea7, 0x0eaa, 0x0eab, 0x0eb2, 0x0eb3, 
    0x0ebd, 0x0ec6, 0x0edc, 0x0edd, 0x0f00, 0x1029, 0x102a, 0x1248, 0x1258, 
    0x1288, 0x12b0, 0x12c0, 0x1310, 0x1f59, 0x1f5b, 0x1f5d, 0x1fbe, 0x207f, 
    0x2102, 0x2107, 0x2115, 0x2124, 0x2126, 0x2128, 0x3005, 0x3006, 0x309d, 
    0x309e, 0xfb1d, 0xfb3e, 0xfb40, 0xfb41, 0xfb43, 0xfb44, 0xfe74, 0xfffe
};

#define NUM_ALPHA_CHAR (sizeof(alphaCharTable)/sizeof(chr))

/* Unicode: decimal digit characters */

static crange digitRangeTable[] = {
    {0x0030, 0x0039}, {0x0660, 0x0669}, {0x06f0, 0x06f9}, {0x0966, 0x096f}, 
    {0x09e6, 0x09ef}, {0x0a66, 0x0a6f}, {0x0ae6, 0x0aef}, {0x0b66, 0x0b6f}, 
    {0x0be7, 0x0bef}, {0x0c66, 0x0c6f}, {0x0ce6, 0x0cef}, {0x0d66, 0x0d6f}, 
    {0x0e50, 0x0e59}, {0x0ed0, 0x0ed9}, {0x0f20, 0x0f29}, {0x1040, 0x1049}, 
    {0x1369, 0x1371}, {0x17e0, 0x17e9}, {0x1810, 0x1819}, {0xff10, 0xff19}
};

#define NUM_DIGIT_RANGE (sizeof(digitRangeTable)/sizeof(crange))

/* no singletons of digit characters */

/* Unicode: punctuation characters */

static crange punctRangeTable[] = {
    {0x0021, 0x0023}, {0x0025, 0x002a}, {0x002c, 0x002f}, {0x005b, 0x005d}, 
    {0x055a, 0x055f}, {0x066a, 0x066d}, {0x0700, 0x070d}, {0x0f04, 0x0f12}, 
    {0x0f3a, 0x0f3d}, {0x104a, 0x104f}, {0x1361, 0x1368}, {0x16eb, 0x16ed}, 
    {0x17d4, 0x17da}, {0x1800, 0x180a}, {0x2010, 0x2027}, {0x2030, 0x2043}, 
    {0x2048, 0x204d}, {0x3001, 0x3003}, {0x3008, 0x3011}, {0x3014, 0x301f}, 
    {0xfe30, 0xfe44}, {0xfe49, 0xfe52}, {0xfe54, 0xfe61}, {0xff01, 0xff03}, 
    {0xff05, 0xff0a}, {0xff0c, 0xff0f}, {0xff3b, 0xff3d}, {0xff61, 0xff65}
};

#define NUM_PUNCT_RANGE (sizeof(punctRangeTable)/sizeof(crange))

static chr punctCharTable[] = {
    0x003a, 0x003b, 0x003f, 0x0040, 0x005f, 0x007b, 0x007d, 0x00a1, 0x00ab, 
    0x00ad, 0x00b7, 0x00bb, 0x00bf, 0x037e, 0x0387, 0x0589, 0x058a, 0x05be, 
    0x05c0, 0x05c3, 0x05f3, 0x05f4, 0x060c, 0x061b, 0x061f, 0x06d4, 0x0964, 
    0x0965, 0x0970, 0x0df4, 0x0e4f, 0x0e5a, 0x0e5b, 0x0f85, 0x10fb, 0x166d, 
    0x166e, 0x169b, 0x169c, 0x17dc, 0x2045, 0x2046, 0x207d, 0x207e, 0x208d, 
    0x208e, 0x2329, 0x232a, 0x3030, 0x30fb, 0xfd3e, 0xfd3f, 0xfe63, 0xfe68, 
    0xfe6a, 0xfe6b, 0xff1a, 0xff1b, 0xff1f, 0xff20, 0xff3f, 0xff5b, 0xff5d
};

#define NUM_PUNCT_CHAR (sizeof(punctCharTable)/sizeof(chr))

/* Unicode: white space characters */

static crange spaceRangeTable[] = {
    {0x0009, 0x000d}, {0x2000, 0x200b}
};

#define NUM_SPACE_RANGE (sizeof(spaceRangeTable)/sizeof(crange))

static chr spaceCharTable[] = {
    0x0020, 0x00a0, 0x1680, 0x2028, 0x2029, 0x202f, 0x3000
};

#define NUM_SPACE_CHAR (sizeof(spaceCharTable)/sizeof(chr))

/* Unicode: lowercase characters */

static crange lowerRangeTable[] = {
    {0x0061, 0x007a}, {0x00df, 0x00f6}, {0x00f8, 0x00ff}, {0x017e, 0x0180}, 
    {0x0199, 0x019b}, {0x01bd, 0x01bf}, {0x0250, 0x02ad}, {0x03ac, 0x03ce}, 
    {0x03d5, 0x03d7}, {0x03ef, 0x03f3}, {0x0430, 0x045f}, {0x0561, 0x0587}, 
    {0x1e95, 0x1e9b}, {0x1f00, 0x1f07}, {0x1f10, 0x1f15}, {0x1f20, 0x1f27}, 
    {0x1f30, 0x1f37}, {0x1f40, 0x1f45}, {0x1f50, 0x1f57}, {0x1f60, 0x1f67}, 
    {0x1f70, 0x1f7d}, {0x1f80, 0x1f87}, {0x1f90, 0x1f97}, {0x1fa0, 0x1fa7}, 
    {0x1fb0, 0x1fb4}, {0x1fc2, 0x1fc4}, {0x1fd0, 0x1fd3}, {0x1fe0, 0x1fe7}, 
    {0x1ff2, 0x1ff4}, {0xfb00, 0xfb06}, {0xfb13, 0xfb17}, {0xff41, 0xff5a}
};

#define NUM_LOWER_RANGE (sizeof(lowerRangeTable)/sizeof(crange))

static chr lowerCharTable[] = {
    0x00aa, 0x00b5, 0x00ba, 0x0101, 0x0103, 0x0105, 0x0107, 0x0109, 0x010b, 
    0x010d, 0x010f, 0x0111, 0x0113, 0x0115, 0x0117, 0x0119, 0x011b, 0x011d, 
    0x011f, 0x0121, 0x0123, 0x0125, 0x0127, 0x0129, 0x012b, 0x012d, 0x012f, 
    0x0131, 0x0133, 0x0135, 0x0137, 0x0138, 0x013a, 0x013c, 0x013e, 0x0140, 
    0x0142, 0x0144, 0x0146, 0x0148, 0x0149, 0x014b, 0x014d, 0x014f, 0x0151, 
    0x0153, 0x0155, 0x0157, 0x0159, 0x015b, 0x015d, 0x015f, 0x0161, 0x0163, 
    0x0165, 0x0167, 0x0169, 0x016b, 0x016d, 0x016f, 0x0171, 0x0173, 0x0175, 
    0x0177, 0x017a, 0x017c, 0x0183, 0x0185, 0x0188, 0x018c, 0x018d, 0x0192, 
    0x0195, 0x019e, 0x01a1, 0x01a3, 0x01a5, 0x01a8, 0x01aa, 0x01ab, 0x01ad, 
    0x01b0, 0x01b4, 0x01b6, 0x01b9, 0x01ba, 0x01c6, 0x01c9, 0x01cc, 0x01ce, 
    0x01d0, 0x01d2, 0x01d4, 0x01d6, 0x01d8, 0x01da, 0x01dc, 0x01dd, 0x01df, 
    0x01e1, 0x01e3, 0x01e5, 0x01e7, 0x01e9, 0x01eb, 0x01ed, 0x01ef, 0x01f0, 
    0x01f3, 0x01f5, 0x01f9, 0x01fb, 0x01fd, 0x01ff, 0x0201, 0x0203, 0x0205, 
    0x0207, 0x0209, 0x020b, 0x020d, 0x020f, 0x0211, 0x0213, 0x0215, 0x0217, 
    0x0219, 0x021b, 0x021d, 0x021f, 0x0223, 0x0225, 0x0227, 0x0229, 0x022b, 
    0x022d, 0x022f, 0x0231, 0x0233, 0x0390, 0x03d0, 0x03d1, 0x03db, 0x03dd, 
    0x03df, 0x03e1, 0x03e3, 0x03e5, 0x03e7, 0x03e9, 0x03eb, 0x03ed, 0x03f5, 
    0x0461, 0x0463, 0x0465, 0x0467, 0x0469, 0x046b, 0x046d, 0x046f, 0x0471, 
    0x0473, 0x0475, 0x0477, 0x0479, 0x047b, 0x047d, 0x047f, 0x0481, 0x048d, 
    0x048f, 0x0491, 0x0493, 0x0495, 0x0497, 0x0499, 0x049b, 0x049d, 0x049f, 
    0x04a1, 0x04a3, 0x04a5, 0x04a7, 0x04a9, 0x04ab, 0x04ad, 0x04af, 0x04b1, 
    0x04b3, 0x04b5, 0x04b7, 0x04b9, 0x04bb, 0x04bd, 0x04bf, 0x04c2, 0x04c4, 
    0x04c8, 0x04cc, 0x04d1, 0x04d3, 0x04d5, 0x04d7, 0x04d9, 0x04db, 0x04dd, 
    0x04df, 0x04e1, 0x04e3, 0x04e5, 0x04e7, 0x04e9, 0x04eb, 0x04ed, 0x04ef, 
    0x04f1, 0x04f3, 0x04f5, 0x04f9, 0x1e01, 0x1e03, 0x1e05, 0x1e07, 0x1e09, 
    0x1e0b, 0x1e0d, 0x1e0f, 0x1e11, 0x1e13, 0x1e15, 0x1e17, 0x1e19, 0x1e1b, 
    0x1e1d, 0x1e1f, 0x1e21, 0x1e23, 0x1e25, 0x1e27, 0x1e29, 0x1e2b, 0x1e2d, 
    0x1e2f, 0x1e31, 0x1e33, 0x1e35, 0x1e37, 0x1e39, 0x1e3b, 0x1e3d, 0x1e3f, 
    0x1e41, 0x1e43, 0x1e45, 0x1e47, 0x1e49, 0x1e4b, 0x1e4d, 0x1e4f, 0x1e51, 
    0x1e53, 0x1e55, 0x1e57, 0x1e59, 0x1e5b, 0x1e5d, 0x1e5f, 0x1e61, 0x1e63, 
    0x1e65, 0x1e67, 0x1e69, 0x1e6b, 0x1e6d, 0x1e6f, 0x1e71, 0x1e73, 0x1e75, 
    0x1e77, 0x1e79, 0x1e7b, 0x1e7d, 0x1e7f, 0x1e81, 0x1e83, 0x1e85, 0x1e87, 
    0x1e89, 0x1e8b, 0x1e8d, 0x1e8f, 0x1e91, 0x1e93, 0x1ea1, 0x1ea3, 0x1ea5, 
    0x1ea7, 0x1ea9, 0x1eab, 0x1ead, 0x1eaf, 0x1eb1, 0x1eb3, 0x1eb5, 0x1eb7, 
    0x1eb9, 0x1ebb, 0x1ebd, 0x1ebf, 0x1ec1, 0x1ec3, 0x1ec5, 0x1ec7, 0x1ec9, 
    0x1ecb, 0x1ecd, 0x1ecf, 0x1ed1, 0x1ed3, 0x1ed5, 0x1ed7, 0x1ed9, 0x1edb, 
    0x1edd, 0x1edf, 0x1ee1, 0x1ee3, 0x1ee5, 0x1ee7, 0x1ee9, 0x1eeb, 0x1eed, 
    0x1eef, 0x1ef1, 0x1ef3, 0x1ef5, 0x1ef7, 0x1ef9, 0x1fb6, 0x1fb7, 0x1fbe, 
    0x1fc6, 0x1fc7, 0x1fd6, 0x1fd7, 0x1ff6, 0x1ff7, 0x207f, 0x210a, 0x210e, 
    0x210f, 0x2113, 0x212f, 0x2134, 0x2139
};

#define NUM_LOWER_CHAR (sizeof(lowerCharTable)/sizeof(chr))

/* Unicode: uppercase characters */

static crange upperRangeTable[] = {
    {0x0041, 0x005a}, {0x00c0, 0x00d6}, {0x00d8, 0x00de}, {0x0189, 0x018b}, 
    {0x018e, 0x0191}, {0x0196, 0x0198}, {0x01b1, 0x01b3}, {0x01f6, 0x01f8}, 
    {0x0388, 0x038a}, {0x0391, 0x03a1}, {0x03a3, 0x03ab}, {0x03d2, 0x03d4}, 
    {0x0400, 0x042f}, {0x0531, 0x0556}, {0x10a0, 0x10c5}, {0x1f08, 0x1f0f}, 
    {0x1f18, 0x1f1d}, {0x1f28, 0x1f2f}, {0x1f38, 0x1f3f}, {0x1f48, 0x1f4d}, 
    {0x1f68, 0x1f6f}, {0x1fb8, 0x1fbb}, {0x1fc8, 0x1fcb}, {0x1fd8, 0x1fdb}, 
    {0x1fe8, 0x1fec}, {0x1ff8, 0x1ffb}, {0x210b, 0x210d}, {0x2110, 0x2112}, 
    {0x2119, 0x211d}, {0x212a, 0x212d}, {0xff21, 0xff3a}
};

#define NUM_UPPER_RANGE (sizeof(upperRangeTable)/sizeof(crange))

static chr upperCharTable[] = {
    0x0100, 0x0102, 0x0104, 0x0106, 0x0108, 0x010a, 0x010c, 0x010e, 0x0110, 
    0x0112, 0x0114, 0x0116, 0x0118, 0x011a, 0x011c, 0x011e, 0x0120, 0x0122, 
    0x0124, 0x0126, 0x0128, 0x012a, 0x012c, 0x012e, 0x0130, 0x0132, 0x0134, 
    0x0136, 0x0139, 0x013b, 0x013d, 0x013f, 0x0141, 0x0143, 0x0145, 0x0147, 
    0x014a, 0x014c, 0x014e, 0x0150, 0x0152, 0x0154, 0x0156, 0x0158, 0x015a, 
    0x015c, 0x015e, 0x0160, 0x0162, 0x0164, 0x0166, 0x0168, 0x016a, 0x016c, 
    0x016e, 0x0170, 0x0172, 0x0174, 0x0176, 0x0178, 0x0179, 0x017b, 0x017d, 
    0x0181, 0x0182, 0x0184, 0x0186, 0x0187, 0x0193, 0x0194, 0x019c, 0x019d, 
    0x019f, 0x01a0, 0x01a2, 0x01a4, 0x01a6, 0x01a7, 0x01a9, 0x01ac, 0x01ae, 
    0x01af, 0x01b5, 0x01b7, 0x01b8, 0x01bc, 0x01c4, 0x01c7, 0x01ca, 0x01cd, 
    0x01cf, 0x01d1, 0x01d3, 0x01d5, 0x01d7, 0x01d9, 0x01db, 0x01de, 0x01e0, 
    0x01e2, 0x01e4, 0x01e6, 0x01e8, 0x01ea, 0x01ec, 0x01ee, 0x01f1, 0x01f4, 
    0x01fa, 0x01fc, 0x01fe, 0x0200, 0x0202, 0x0204, 0x0206, 0x0208, 0x020a, 
    0x020c, 0x020e, 0x0210, 0x0212, 0x0214, 0x0216, 0x0218, 0x021a, 0x021c, 
    0x021e, 0x0222, 0x0224, 0x0226, 0x0228, 0x022a, 0x022c, 0x022e, 0x0230, 
    0x0232, 0x0386, 0x038c, 0x038e, 0x038f, 0x03da, 0x03dc, 0x03de, 0x03e0, 
    0x03e2, 0x03e4, 0x03e6, 0x03e8, 0x03ea, 0x03ec, 0x03ee, 0x03f4, 0x0460, 
    0x0462, 0x0464, 0x0466, 0x0468, 0x046a, 0x046c, 0x046e, 0x0470, 0x0472, 
    0x0474, 0x0476, 0x0478, 0x047a, 0x047c, 0x047e, 0x0480, 0x048c, 0x048e, 
    0x0490, 0x0492, 0x0494, 0x0496, 0x0498, 0x049a, 0x049c, 0x049e, 0x04a0, 
    0x04a2, 0x04a4, 0x04a6, 0x04a8, 0x04aa, 0x04ac, 0x04ae, 0x04b0, 0x04b2, 
    0x04b4, 0x04b6, 0x04b8, 0x04ba, 0x04bc, 0x04be, 0x04c0, 0x04c1, 0x04c3, 
    0x04c7, 0x04cb, 0x04d0, 0x04d2, 0x04d4, 0x04d6, 0x04d8, 0x04da, 0x04dc, 
    0x04de, 0x04e0, 0x04e2, 0x04e4, 0x04e6, 0x04e8, 0x04ea, 0x04ec, 0x04ee, 
    0x04f0, 0x04f2, 0x04f4, 0x04f8, 0x1e00, 0x1e02, 0x1e04, 0x1e06, 0x1e08, 
    0x1e0a, 0x1e0c, 0x1e0e, 0x1e10, 0x1e12, 0x1e14, 0x1e16, 0x1e18, 0x1e1a, 
    0x1e1c, 0x1e1e, 0x1e20, 0x1e22, 0x1e24, 0x1e26, 0x1e28, 0x1e2a, 0x1e2c, 
    0x1e2e, 0x1e30, 0x1e32, 0x1e34, 0x1e36, 0x1e38, 0x1e3a, 0x1e3c, 0x1e3e, 
    0x1e40, 0x1e42, 0x1e44, 0x1e46, 0x1e48, 0x1e4a, 0x1e4c, 0x1e4e, 0x1e50, 
    0x1e52, 0x1e54, 0x1e56, 0x1e58, 0x1e5a, 0x1e5c, 0x1e5e, 0x1e60, 0x1e62, 
    0x1e64, 0x1e66, 0x1e68, 0x1e6a, 0x1e6c, 0x1e6e, 0x1e70, 0x1e72, 0x1e74, 
    0x1e76, 0x1e78, 0x1e7a, 0x1e7c, 0x1e7e, 0x1e80, 0x1e82, 0x1e84, 0x1e86, 
    0x1e88, 0x1e8a, 0x1e8c, 0x1e8e, 0x1e90, 0x1e92, 0x1e94, 0x1ea0, 0x1ea2, 
    0x1ea4, 0x1ea6, 0x1ea8, 0x1eaa, 0x1eac, 0x1eae, 0x1eb0, 0x1eb2, 0x1eb4, 
    0x1eb6, 0x1eb8, 0x1eba, 0x1ebc, 0x1ebe, 0x1ec0, 0x1ec2, 0x1ec4, 0x1ec6, 
    0x1ec8, 0x1eca, 0x1ecc, 0x1ece, 0x1ed0, 0x1ed2, 0x1ed4, 0x1ed6, 0x1ed8, 
    0x1eda, 0x1edc, 0x1ede, 0x1ee0, 0x1ee2, 0x1ee4, 0x1ee6, 0x1ee8, 0x1eea, 
    0x1eec, 0x1eee, 0x1ef0, 0x1ef2, 0x1ef4, 0x1ef6, 0x1ef8, 0x1f59, 0x1f5b, 
    0x1f5d, 0x1f5f, 0x2102, 0x2107, 0x2115, 0x2124, 0x2126, 0x2128, 0x2130, 
    0x2131, 0x2133
};

#define NUM_UPPER_CHAR (sizeof(upperCharTable)/sizeof(chr))

/* Unicode: unicode print characters excluding space */

static crange graphRangeTable[] = {
    {0x0021, 0x007e}, {0x00a0, 0x011f}, {0x0121, 0x021f}, {0x0222, 0x0233}, 
    {0x0250, 0x02ad}, {0x02b0, 0x02ee}, {0x0300, 0x031f}, {0x0321, 0x034e}, 
    {0x0360, 0x0362}, {0x0384, 0x038a}, {0x038e, 0x03a1}, {0x03a3, 0x03ce}, 
    {0x03d0, 0x03d7}, {0x03da, 0x03f5}, {0x0400, 0x041f}, {0x0421, 0x0486}, 
    {0x048c, 0x04c4}, {0x04d0, 0x04f5}, {0x0531, 0x0556}, {0x0559, 0x055f}, 
    {0x0561, 0x0587}, {0x0591, 0x05a1}, {0x05a3, 0x05b9}, {0x05bb, 0x05c4}, 
    {0x05d0, 0x05ea}, {0x05f0, 0x05f4}, {0x0621, 0x063a}, {0x0640, 0x0655}, 
    {0x0660, 0x066d}, {0x0670, 0x06ed}, {0x06f0, 0x06fe}, {0x0700, 0x070d}, 
    {0x0710, 0x071f}, {0x0721, 0x072c}, {0x0730, 0x074a}, {0x0780, 0x07b0}, 
    {0x0901, 0x0903}, {0x0905, 0x091f}, {0x0921, 0x0939}, {0x093c, 0x094d}, 
    {0x0950, 0x0954}, {0x0958, 0x0970}, {0x0981, 0x0983}, {0x0985, 0x098c}, 
    {0x0993, 0x09a8}, {0x09aa, 0x09b0}, {0x09b6, 0x09b9}, {0x09be, 0x09c4}, 
    {0x09cb, 0x09cd}, {0x09df, 0x09e3}, {0x09e6, 0x09fa}, {0x0a05, 0x0a0a}, 
    {0x0a13, 0x0a1f}, {0x0a21, 0x0a28}, {0x0a2a, 0x0a30}, {0x0a3e, 0x0a42}, 
    {0x0a4b, 0x0a4d}, {0x0a59, 0x0a5c}, {0x0a66, 0x0a74}, {0x0a81, 0x0a83}, 
    {0x0a85, 0x0a8b}, {0x0a8f, 0x0a91}, {0x0a93, 0x0aa8}, {0x0aaa, 0x0ab0}, 
    {0x0ab5, 0x0ab9}, {0x0abc, 0x0ac5}, {0x0ac7, 0x0ac9}, {0x0acb, 0x0acd}, 
    {0x0ae6, 0x0aef}, {0x0b01, 0x0b03}, {0x0b05, 0x0b0c}, {0x0b13, 0x0b1f}, 
    {0x0b21, 0x0b28}, {0x0b2a, 0x0b30}, {0x0b36, 0x0b39}, {0x0b3c, 0x0b43}, 
    {0x0b4b, 0x0b4d}, {0x0b5f, 0x0b61}, {0x0b66, 0x0b70}, {0x0b85, 0x0b8a}, 
    {0x0b8e, 0x0b90}, {0x0b92, 0x0b95}, {0x0ba8, 0x0baa}, {0x0bae, 0x0bb5}, 
    {0x0bb7, 0x0bb9}, {0x0bbe, 0x0bc2}, {0x0bc6, 0x0bc8}, {0x0bca, 0x0bcd}, 
    {0x0be7, 0x0bf2}, {0x0c01, 0x0c03}, {0x0c05, 0x0c0c}, {0x0c0e, 0x0c10}, 
    {0x0c12, 0x0c1f}, {0x0c21, 0x0c28}, {0x0c2a, 0x0c33}, {0x0c35, 0x0c39}, 
    {0x0c3e, 0x0c44}, {0x0c46, 0x0c48}, {0x0c4a, 0x0c4d}, {0x0c66, 0x0c6f}, 
    {0x0c85, 0x0c8c}, {0x0c8e, 0x0c90}, {0x0c92, 0x0ca8}, {0x0caa, 0x0cb3}, 
    {0x0cb5, 0x0cb9}, {0x0cbe, 0x0cc4}, {0x0cc6, 0x0cc8}, {0x0cca, 0x0ccd}, 
    {0x0ce6, 0x0cef}, {0x0d05, 0x0d0c}, {0x0d0e, 0x0d10}, {0x0d12, 0x0d1f}, 
    {0x0d21, 0x0d28}, {0x0d2a, 0x0d39}, {0x0d3e, 0x0d43}, {0x0d46, 0x0d48}, 
    {0x0d4a, 0x0d4d}, {0x0d66, 0x0d6f}, {0x0d85, 0x0d96}, {0x0d9a, 0x0db1}, 
    {0x0db3, 0x0dbb}, {0x0dc0, 0x0dc6}, {0x0dcf, 0x0dd4}, {0x0dd8, 0x0ddf}, 
    {0x0df2, 0x0df4}, {0x0e01, 0x0e1f}, {0x0e21, 0x0e3a}, {0x0e3f, 0x0e5b}, 
    {0x0e94, 0x0e97}, {0x0e99, 0x0e9f}, {0x0ea1, 0x0ea3}, {0x0ead, 0x0eb9}, 
    {0x0ebb, 0x0ebd}, {0x0ec0, 0x0ec4}, {0x0ec8, 0x0ecd}, {0x0ed0, 0x0ed9}, 
    {0x0f00, 0x0f1f}, {0x0f21, 0x0f47}, {0x0f49, 0x0f6a}, {0x0f71, 0x0f8b}, 
    {0x0f90, 0x0f97}, {0x0f99, 0x0fbc}, {0x0fbe, 0x0fcc}, {0x1000, 0x101f}, 
    {0x1023, 0x1027}, {0x102c, 0x1032}, {0x1036, 0x1039}, {0x1040, 0x1059}, 
    {0x10a0, 0x10c5}, {0x10d0, 0x10f6}, {0x1100, 0x111f}, {0x1121, 0x1159}, 
    {0x115f, 0x11a2}, {0x11a8, 0x11f9}, {0x1200, 0x1206}, {0x1208, 0x121f}, 
    {0x1221, 0x1246}, {0x124a, 0x124d}, {0x1250, 0x1256}, {0x125a, 0x125d}, 
    {0x1260, 0x1286}, {0x128a, 0x128d}, {0x1290, 0x12ae}, {0x12b2, 0x12b5}, 
    {0x12b8, 0x12be}, {0x12c2, 0x12c5}, {0x12c8, 0x12ce}, {0x12d0, 0x12d6}, 
    {0x12d8, 0x12ee}, {0x12f0, 0x130e}, {0x1312, 0x1315}, {0x1318, 0x131e}, 
    {0x1321, 0x1346}, {0x1348, 0x135a}, {0x1361, 0x137c}, {0x13a0, 0x13f4}, 
    {0x1401, 0x141f}, {0x1421, 0x151f}, {0x1521, 0x161f}, {0x1621, 0x1676}, 
    {0x1680, 0x169c}, {0x16a0, 0x16f0}, {0x1780, 0x17dc}, {0x17e0, 0x17e9}, 
    {0x1800, 0x180a}, {0x1810, 0x1819}, {0x1821, 0x1877}, {0x1880, 0x18a9}, 
    {0x1e00, 0x1e1f}, {0x1e21, 0x1e9b}, {0x1ea0, 0x1ef9}, {0x1f00, 0x1f15}, 
    {0x1f18, 0x1f1d}, {0x1f21, 0x1f45}, {0x1f48, 0x1f4d}, {0x1f50, 0x1f57}, 
    {0x1f5f, 0x1f7d}, {0x1f80, 0x1fb4}, {0x1fb6, 0x1fc4}, {0x1fc6, 0x1fd3}, 
    {0x1fd6, 0x1fdb}, {0x1fdd, 0x1fef}, {0x1ff2, 0x1ff4}, {0x1ff6, 0x1ffe}, 
    {0x2000, 0x200b}, {0x2010, 0x201f}, {0x2021, 0x2029}, {0x202f, 0x2046}, 
    {0x2048, 0x204d}, {0x2074, 0x208e}, {0x20a0, 0x20af}, {0x20d0, 0x20e3}, 
    {0x2100, 0x211f}, {0x2121, 0x213a}, {0x2153, 0x2183}, {0x2190, 0x21f3}, 
    {0x2200, 0x221f}, {0x2221, 0x22f1}, {0x2300, 0x231f}, {0x2321, 0x237b}, 
    {0x237d, 0x239a}, {0x2400, 0x241f}, {0x2421, 0x2426}, {0x2440, 0x244a}, 
    {0x2460, 0x24ea}, {0x2500, 0x251f}, {0x2521, 0x2595}, {0x25a0, 0x25f7}, 
    {0x2600, 0x2613}, {0x2619, 0x261f}, {0x2621, 0x2671}, {0x2701, 0x2704}, 
    {0x2706, 0x2709}, {0x270c, 0x271f}, {0x2721, 0x2727}, {0x2729, 0x274b}, 
    {0x274f, 0x2752}, {0x2758, 0x275e}, {0x2761, 0x2767}, {0x2776, 0x2794}, 
    {0x2798, 0x27af}, {0x27b1, 0x27be}, {0x2800, 0x281f}, {0x2821, 0x28ff}, 
    {0x2e80, 0x2e99}, {0x2e9b, 0x2ef3}, {0x2f00, 0x2f1f}, {0x2f21, 0x2fd5}, 
    {0x2ff0, 0x2ffb}, {0x3000, 0x301f}, {0x3021, 0x303a}, {0x3041, 0x3094}, 
    {0x3099, 0x309e}, {0x30a1, 0x30fe}, {0x3105, 0x311f}, {0x3121, 0x312c}, 
    {0x3131, 0x318e}, {0x3190, 0x31b7}, {0x3200, 0x321c}, {0x3221, 0x3243}, 
    {0x3260, 0x327b}, {0x327f, 0x32b0}, {0x32c0, 0x32cb}, {0x32d0, 0x32fe}, 
    {0x3300, 0x331f}, {0x3321, 0x3376}, {0x337b, 0x33dd}, {0x33e0, 0x33fe}, 
    {0x3400, 0x341f}, {0x3421, 0x351f}, {0x3521, 0x361f}, {0x3621, 0x371f}, 
    {0x3721, 0x381f}, {0x3821, 0x391f}, {0x3921, 0x3a1f}, {0x3a21, 0x3b1f}, 
    {0x3b21, 0x3c1f}, {0x3c21, 0x3d1f}, {0x3d21, 0x3e1f}, {0x3e21, 0x3f1f}, 
    {0x3f21, 0x401f}, {0x4021, 0x411f}, {0x4121, 0x421f}, {0x4221, 0x431f}, 
    {0x4321, 0x441f}, {0x4421, 0x451f}, {0x4521, 0x461f}, {0x4621, 0x471f}, 
    {0x4721, 0x481f}, {0x4821, 0x491f}, {0x4921, 0x4a1f}, {0x4a21, 0x4b1f}, 
    {0x4b21, 0x4c1f}, {0x4c21, 0x4d1f}, {0x4d21, 0x4db5}, {0x4e00, 0x4e1f}, 
    {0x4e21, 0x4f1f}, {0x4f21, 0x501f}, {0x5021, 0x511f}, {0x5121, 0x521f}, 
    {0x5221, 0x531f}, {0x5321, 0x541f}, {0x5421, 0x551f}, {0x5521, 0x561f}, 
    {0x5621, 0x571f}, {0x5721, 0x581f}, {0x5821, 0x591f}, {0x5921, 0x5a1f}, 
    {0x5a21, 0x5b1f}, {0x5b21, 0x5c1f}, {0x5c21, 0x5d1f}, {0x5d21, 0x5e1f}, 
    {0x5e21, 0x5f1f}, {0x5f21, 0x601f}, {0x6021, 0x611f}, {0x6121, 0x621f}, 
    {0x6221, 0x631f}, {0x6321, 0x641f}, {0x6421, 0x651f}, {0x6521, 0x661f}, 
    {0x6621, 0x671f}, {0x6721, 0x681f}, {0x6821, 0x691f}, {0x6921, 0x6a1f}, 
    {0x6a21, 0x6b1f}, {0x6b21, 0x6c1f}, {0x6c21, 0x6d1f}, {0x6d21, 0x6e1f}, 
    {0x6e21, 0x6f1f}, {0x6f21, 0x701f}, {0x7021, 0x711f}, {0x7121, 0x721f}, 
    {0x7221, 0x731f}, {0x7321, 0x741f}, {0x7421, 0x751f}, {0x7521, 0x761f}, 
    {0x7621, 0x771f}, {0x7721, 0x781f}, {0x7821, 0x791f}, {0x7921, 0x7a1f}, 
    {0x7a21, 0x7b1f}, {0x7b21, 0x7c1f}, {0x7c21, 0x7d1f}, {0x7d21, 0x7e1f}, 
    {0x7e21, 0x7f1f}, {0x7f21, 0x801f}, {0x8021, 0x811f}, {0x8121, 0x821f}, 
    {0x8221, 0x831f}, {0x8321, 0x841f}, {0x8421, 0x851f}, {0x8521, 0x861f}, 
    {0x8621, 0x871f}, {0x8721, 0x881f}, {0x8821, 0x891f}, {0x8921, 0x8a1f}, 
    {0x8a21, 0x8b1f}, {0x8b21, 0x8c1f}, {0x8c21, 0x8d1f}, {0x8d21, 0x8e1f}, 
    {0x8e21, 0x8f1f}, {0x8f21, 0x901f}, {0x9021, 0x911f}, {0x9121, 0x921f}, 
    {0x9221, 0x931f}, {0x9321, 0x941f}, {0x9421, 0x951f}, {0x9521, 0x961f}, 
    {0x9621, 0x971f}, {0x9721, 0x981f}, {0x9821, 0x991f}, {0x9921, 0x9a1f}, 
    {0x9a21, 0x9b1f}, {0x9b21, 0x9c1f}, {0x9c21, 0x9d1f}, {0x9d21, 0x9e1f}, 
    {0x9e21, 0x9f1f}, {0x9f21, 0x9fa5}, {0xa000, 0xa01f}, {0xa021, 0xa11f}, 
    {0xa121, 0xa21f}, {0xa221, 0xa31f}, {0xa321, 0xa41f}, {0xa421, 0xa48c}, 
    {0xa490, 0xa4a1}, {0xa4a4, 0xa4b3}, {0xa4b5, 0xa4c0}, {0xa4c2, 0xa4c4}, 
    {0xac00, 0xac1f}, {0xac21, 0xad1f}, {0xad21, 0xae1f}, {0xae21, 0xaf1f}, 
    {0xaf21, 0xb01f}, {0xb021, 0xb11f}, {0xb121, 0xb21f}, {0xb221, 0xb31f}, 
    {0xb321, 0xb41f}, {0xb421, 0xb51f}, {0xb521, 0xb61f}, {0xb621, 0xb71f}, 
    {0xb721, 0xb81f}, {0xb821, 0xb91f}, {0xb921, 0xba1f}, {0xba21, 0xbb1f}, 
    {0xbb21, 0xbc1f}, {0xbc21, 0xbd1f}, {0xbd21, 0xbe1f}, {0xbe21, 0xbf1f}, 
    {0xbf21, 0xc01f}, {0xc021, 0xc11f}, {0xc121, 0xc21f}, {0xc221, 0xc31f}, 
    {0xc321, 0xc41f}, {0xc421, 0xc51f}, {0xc521, 0xc61f}, {0xc621, 0xc71f}, 
    {0xc721, 0xc81f}, {0xc821, 0xc91f}, {0xc921, 0xca1f}, {0xca21, 0xcb1f}, 
    {0xcb21, 0xcc1f}, {0xcc21, 0xcd1f}, {0xcd21, 0xce1f}, {0xce21, 0xcf1f}, 
    {0xcf21, 0xd01f}, {0xd021, 0xd11f}, {0xd121, 0xd21f}, {0xd221, 0xd31f}, 
    {0xd321, 0xd41f}, {0xd421, 0xd51f}, {0xd521, 0xd61f}, {0xd621, 0xd71f}, 
    {0xd721, 0xd7a3}, {0xf900, 0xf91f}, {0xf921, 0xfa1f}, {0xfa21, 0xfa2d}, 
    {0xfb00, 0xfb06}, {0xfb13, 0xfb17}, {0xfb1d, 0xfb1f}, {0xfb21, 0xfb36}, 
    {0xfb38, 0xfb3c}, {0xfb46, 0xfbb1}, {0xfbd3, 0xfc1f}, {0xfc21, 0xfd1f}, 
    {0xfd21, 0xfd3f}, {0xfd50, 0xfd8f}, {0xfd92, 0xfdc7}, {0xfdf0, 0xfdfb}, 
    {0xfe21, 0xfe23}, {0xfe30, 0xfe44}, {0xfe49, 0xfe52}, {0xfe54, 0xfe66}, 
    {0xfe68, 0xfe6b}, {0xfe70, 0xfe72}, {0xfe76, 0xfefc}, {0xff01, 0xff1f}, 
    {0xff21, 0xff5e}, {0xff61, 0xffbe}, {0xffc2, 0xffc7}, {0xffca, 0xffcf}, 
    {0xffd2, 0xffd7}, {0xffda, 0xffdc}, {0xffe0, 0xffe6}, {0xffe8, 0xffee}, 
    {0xfffc, 0xffff}
};

#define NUM_GRAPH_RANGE (sizeof(graphRangeTable)/sizeof(crange))

static chr graphCharTable[] = {
    0x0374, 0x0375, 0x037a, 0x037e, 0x038c, 0x0488, 0x0489, 0x04c7, 0x04c8, 
    0x04cb, 0x04cc, 0x04f8, 0x04f9, 0x0589, 0x058a, 0x060c, 0x061b, 0x061f, 
    0x098f, 0x0990, 0x09b2, 0x09bc, 0x09c7, 0x09c8, 0x09d7, 0x09dc, 0x09dd, 
    0x0a02, 0x0a0f, 0x0a10, 0x0a32, 0x0a33, 0x0a35, 0x0a36, 0x0a38, 0x0a39, 
    0x0a3c, 0x0a47, 0x0a48, 0x0a5e, 0x0a8d, 0x0ab2, 0x0ab3, 0x0ad0, 0x0ae0, 
    0x0b0f, 0x0b10, 0x0b32, 0x0b33, 0x0b47, 0x0b48, 0x0b56, 0x0b57, 0x0b5c, 
    0x0b5d, 0x0b82, 0x0b83, 0x0b99, 0x0b9a, 0x0b9c, 0x0b9e, 0x0b9f, 0x0ba3, 
    0x0ba4, 0x0bd7, 0x0c55, 0x0c56, 0x0c60, 0x0c61, 0x0c82, 0x0c83, 0x0cd5, 
    0x0cd6, 0x0cde, 0x0ce0, 0x0ce1, 0x0d02, 0x0d03, 0x0d57, 0x0d60, 0x0d61, 
    0x0d82, 0x0d83, 0x0dbd, 0x0dca, 0x0dd6, 0x0e81, 0x0e82, 0x0e84, 0x0e87, 
    0x0e88, 0x0e8a, 0x0e8d, 0x0ea5, 0x0ea7, 0x0eaa, 0x0eab, 0x0ec6, 0x0edc, 
    0x0edd, 0x0fcf, 0x1021, 0x1029, 0x102a, 0x10fb, 0x1248, 0x1258, 0x1288, 
    0x12b0, 0x12c0, 0x1310, 0x1f59, 0x1f5b, 0x1f5d, 0x2070, 0x274d, 0x2756, 
    0x303e, 0x303f, 0xa4c6, 0xfb3e, 0xfb40, 0xfb41, 0xfb43, 0xfb44, 0xfe74
};

#define NUM_GRAPH_CHAR (sizeof(graphCharTable)/sizeof(chr))

/*
 *	End of auto-generated Unicode character ranges declarations.
 */

#define	CH	NOCELT

/*
 - nmcces - how many distinct MCCEs are there?
 ^ static int nmcces(struct vars *);
 */
static int
nmcces(v)
    struct vars *v;			/* context */
{
    /*
     * No multi-character collating elements defined at the moment.
     */
    return 0;
}

/*
 - nleaders - how many chrs can be first chrs of MCCEs?
 ^ static int nleaders(struct vars *);
 */
static int
nleaders(v)
    struct vars *v;			/* context */
{
    return 0;
}

/*
 - allmcces - return a cvec with all the MCCEs of the locale
 ^ static struct cvec *allmcces(struct vars *, struct cvec *);
 */
static struct cvec *
allmcces(v, cv)
    struct vars *v;			/* context */
    struct cvec *cv;			/* this is supposed to have enough room */
{
    return clearcvec(cv);
}

/*
 - element - map collating-element name to celt
 ^ static celt element(struct vars *, chr *, chr *);
 */
static celt
element(v, startp, endp)
    struct vars *v;			/* context */
    chr *startp;			/* points to start of name */
    chr *endp;				/* points just past end of name */
{
    struct cname *cn;
    size_t len;
    Tcl_DString ds;
    CONST char *np;

    /* generic:  one-chr names stand for themselves */
    assert(startp < endp);
    len = endp - startp;
    if (len == 1) {
	return *startp;
    }

    NOTE(REG_ULOCALE);

    /* search table */
    Tcl_DStringInit(&ds);
    np = Tcl_UniCharToUtfDString(startp, (int)len, &ds);
    for (cn=cnames; cn->name!=NULL; cn++) {
	if (strlen(cn->name)==len && strncmp(cn->name, np, len)==0) {
	    break;			/* NOTE BREAK OUT */
	}
    }
    Tcl_DStringFree(&ds);
    if (cn->name != NULL) {
	return CHR(cn->code);
    }

    /* couldn't find it */
    ERR(REG_ECOLLATE);
    return 0;
}

/*
 - range - supply cvec for a range, including legality check
 ^ static struct cvec *range(struct vars *, celt, celt, int);
 */
static struct cvec *
range(v, a, b, cases)
    struct vars *v;			/* context */
    celt a;				/* range start */
    celt b;				/* range end, might equal a */
    int cases;				/* case-independent? */
{
    int nchrs;
    struct cvec *cv;
    celt c, lc, uc, tc;

    if (a != b && !before(a, b)) {
	ERR(REG_ERANGE);
	return NULL;
    }

    if (!cases) {			/* easy version */
	cv = getcvec(v, 0, 1, 0);
	NOERRN();
	addrange(cv, a, b);
	return cv;
    }

    /*
     * When case-independent, it's hard to decide when cvec ranges are
     * usable, so for now at least, we won't try.  We allocate enough
     * space for two case variants plus a little extra for the two
     * title case variants.
     */

    nchrs = (b - a + 1)*2 + 4;

    cv = getcvec(v, nchrs, 0, 0);
    NOERRN();

    for (c=a; c<=b; c++) {
	addchr(cv, c);
	lc = Tcl_UniCharToLower((chr)c);
	uc = Tcl_UniCharToUpper((chr)c);
	tc = Tcl_UniCharToTitle((chr)c);
	if (c != lc) {
	    addchr(cv, lc);
	}
	if (c != uc) {
	    addchr(cv, uc);
	}
	if (c != tc && tc != uc) {
	    addchr(cv, tc);
	}
    }

    return cv;
}

/*
 - before - is celt x before celt y, for purposes of range legality?
 ^ static int before(celt, celt);
 */
static int				/* predicate */
before(x, y)
    celt x, y;				/* collating elements */
{
    /* trivial because no MCCEs */
    if (x < y) {
	return 1;
    }
    return 0;
}

/*
 - eclass - supply cvec for an equivalence class
 * Must include case counterparts on request.
 ^ static struct cvec *eclass(struct vars *, celt, int);
 */
static struct cvec *
eclass(v, c, cases)
    struct vars *v;			/* context */
    celt c;				/* Collating element representing
					 * the equivalence class. */
    int cases;				/* all cases? */
{
    struct cvec *cv;

    /* crude fake equivalence class for testing */
    if ((v->cflags&REG_FAKE) && c == 'x') {
	cv = getcvec(v, 4, 0, 0);
	addchr(cv, (chr)'x');
	addchr(cv, (chr)'y');
	if (cases) {
	    addchr(cv, (chr)'X');
	    addchr(cv, (chr)'Y');
	}
	return cv;
    }

    /* otherwise, none */
    if (cases) {
	return allcases(v, c);
    }
    cv = getcvec(v, 1, 0, 0);
    assert(cv != NULL);
    addchr(cv, (chr)c);
    return cv;
}

/*
 - cclass - supply cvec for a character class
 * Must include case counterparts on request.
 ^ static struct cvec *cclass(struct vars *, chr *, chr *, int);
 */
static struct cvec *
cclass(v, startp, endp, cases)
    struct vars *v;			/* context */
    chr *startp;			/* where the name starts */
    chr *endp;				/* just past the end of the name */
    int cases;				/* case-independent? */
{
    size_t len;
    struct cvec *cv = NULL;
    Tcl_DString ds;
    CONST char *np;
    char **namePtr;
    int i, index;

    /*
     * The following arrays define the valid character class names.
     */

    static char *classNames[] = {
	"alnum", "alpha", "ascii", "blank", "cntrl", "digit", "graph",
	"lower", "print", "punct", "space", "upper", "xdigit", NULL
    };

    enum classes {
	CC_ALNUM, CC_ALPHA, CC_ASCII, CC_BLANK, CC_CNTRL, CC_DIGIT, CC_GRAPH,
	CC_LOWER, CC_PRINT, CC_PUNCT, CC_SPACE, CC_UPPER, CC_XDIGIT
    };
    

    /*
     * Extract the class name
     */

    len = endp - startp;
    Tcl_DStringInit(&ds);
    np = Tcl_UniCharToUtfDString(startp, (int)len, &ds);

    /*
     * Remap lower and upper to alpha if the match is case insensitive.
     */

    if (cases && len == 5 && (strncmp("lower", np, 5) == 0
	    || strncmp("upper", np, 5) == 0)) {
	np = "alpha";
    }

    /*
     * Map the name to the corresponding enumerated value.
     */

    index = -1;
    for (namePtr=classNames,i=0 ; *namePtr!=NULL ; namePtr++,i++) {
	if ((strlen(*namePtr) == len) && (strncmp(*namePtr, np, len) == 0)) {
	    index = i;
	    break;
	}
    }
    Tcl_DStringInit(&ds);
    if (index == -1) {
	ERR(REG_ECTYPE);
	return NULL;
    }
    
    /*
     * Now compute the character class contents.
     */

    switch((enum classes) index) {
    case CC_PRINT:
    case CC_ALNUM:
	cv = getcvec(v, NUM_ALPHA_CHAR, NUM_DIGIT_RANGE + NUM_ALPHA_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_ALPHA_CHAR ; i++) {
		addchr(cv, alphaCharTable[i]);
	    }
	    for (i=0 ; i<NUM_ALPHA_RANGE ; i++) {
		addrange(cv, alphaRangeTable[i].start,
			alphaRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_DIGIT_RANGE ; i++) {
		addrange(cv, digitRangeTable[i].start,
			digitRangeTable[i].end);
	    }
	}
	break;
    case CC_ALPHA:
	cv = getcvec(v, NUM_ALPHA_CHAR, NUM_ALPHA_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_ALPHA_RANGE ; i++) {
		addrange(cv, alphaRangeTable[i].start,
			alphaRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_ALPHA_CHAR ; i++) {
		addchr(cv, alphaCharTable[i]);
	    }
	}
	break;
    case CC_ASCII:
	cv = getcvec(v, 0, 1, 0);
	if (cv) {
	    addrange(cv, 0, 0x7f);
	}
	break;
    case CC_BLANK:
	cv = getcvec(v, 2, 0, 0);
	addchr(cv, '\t');
	addchr(cv, ' ');
	break;
    case CC_CNTRL:
	cv = getcvec(v, 0, 2, 0);
	addrange(cv, 0x0, 0x1f);
	addrange(cv, 0x7f, 0x9f);
	break;
    case CC_DIGIT:
	cv = getcvec(v, 0, NUM_DIGIT_RANGE, 0);
	if (cv) {	
	    for (i=0 ; i<NUM_DIGIT_RANGE ; i++) {
		addrange(cv, digitRangeTable[i].start,
			digitRangeTable[i].end);
	    }
	}
	break;
    case CC_PUNCT:
	cv = getcvec(v, NUM_PUNCT_CHAR, NUM_PUNCT_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_PUNCT_RANGE ; i++) {
		addrange(cv, punctRangeTable[i].start,
			punctRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_PUNCT_CHAR ; i++) {
		addchr(cv, punctCharTable[i]);
	    }
	}
	break;
    case CC_XDIGIT:
	/*
	 * This is a 3 instead of (NUM_DIGIT_RANGE+2) because I've no
	 * idea how to define the digits 'a' through 'f' in
	 * non-western locales.  The concept is quite possibly non
	 * portable, or only used in contextx where the characters
	 * used would be the western ones anyway!  Whatever is
	 * actually the case, the number of ranges is fixed (until
	 * someone comes up with a better arrangement!)
	 */
	cv = getcvec(v, 0, 3, 0);
	if (cv) {	
	    addrange(cv, '0', '9');
	    addrange(cv, 'a', 'f');
	    addrange(cv, 'A', 'F');
	}
	break;
    case CC_SPACE:
	cv = getcvec(v, NUM_SPACE_CHAR, NUM_SPACE_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_SPACE_RANGE ; i++) {
		addrange(cv, spaceRangeTable[i].start,
			spaceRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_SPACE_CHAR ; i++) {
		addchr(cv, spaceCharTable[i]);
	    }
	}
	break;
    case CC_LOWER:
	cv  = getcvec(v, NUM_LOWER_CHAR, NUM_LOWER_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_LOWER_RANGE ; i++) {
		addrange(cv, lowerRangeTable[i].start,
			lowerRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_LOWER_CHAR ; i++) {
		addchr(cv, lowerCharTable[i]);
	    }
	}
	break;
    case CC_UPPER:
	cv  = getcvec(v, NUM_UPPER_CHAR, NUM_UPPER_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_UPPER_RANGE ; i++) {
		addrange(cv, upperRangeTable[i].start,
			upperRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_UPPER_CHAR ; i++) {
		addchr(cv, upperCharTable[i]);
	    }
	}
	break;
    case CC_GRAPH:
	cv  = getcvec(v, NUM_GRAPH_CHAR, NUM_GRAPH_RANGE, 0);
	if (cv) {
	    for (i=0 ; i<NUM_GRAPH_RANGE ; i++) {
		addrange(cv, graphRangeTable[i].start,
			graphRangeTable[i].end);
	    }
	    for (i=0 ; i<NUM_GRAPH_CHAR ; i++) {
		addchr(cv, graphCharTable[i]);
	    }
	}
	break;
    }
    if (cv == NULL) {
	ERR(REG_ESPACE);
    }
    return cv;
}

/*
 - allcases - supply cvec for all case counterparts of a chr (including itself)
 * This is a shortcut, preferably an efficient one, for simple characters;
 * messy cases are done via range().
 ^ static struct cvec *allcases(struct vars *, pchr);
 */
static struct cvec *
allcases(v, pc)
    struct vars *v;			/* context */
    pchr pc;				/* character to get case equivs of */
{
    struct cvec *cv;
    chr c = (chr)pc;
    chr lc, uc, tc;

    lc = Tcl_UniCharToLower((chr)c);
    uc = Tcl_UniCharToUpper((chr)c);
    tc = Tcl_UniCharToTitle((chr)c);

    if (tc != uc) {
	cv = getcvec(v, 3, 0, 0);
	addchr(cv, tc);
    } else {
	cv = getcvec(v, 2, 0, 0);
    }
    addchr(cv, lc);
    if (lc != uc) {
	addchr(cv, uc);
    }
    return cv;
}

/*
 - cmp - chr-substring compare
 * Backrefs need this.  It should preferably be efficient.
 * Note that it does not need to report anything except equal/unequal.
 * Note also that the length is exact, and the comparison should not
 * stop at embedded NULs!
 ^ static int cmp(CONST chr *, CONST chr *, size_t);
 */
static int				/* 0 for equal, nonzero for unequal */
cmp(x, y, len)
    CONST chr *x, *y;			/* strings to compare */
    size_t len;				/* exact length of comparison */
{
    return memcmp(VS(x), VS(y), len*sizeof(chr));
}

/*
 - casecmp - case-independent chr-substring compare
 * REG_ICASE backrefs need this.  It should preferably be efficient.
 * Note that it does not need to report anything except equal/unequal.
 * Note also that the length is exact, and the comparison should not
 * stop at embedded NULs!
 ^ static int casecmp(CONST chr *, CONST chr *, size_t);
 */
static int				/* 0 for equal, nonzero for unequal */
casecmp(x, y, len)
    CONST chr *x, *y;			/* strings to compare */
    size_t len;				/* exact length of comparison */
{
    for (; len > 0; len--, x++, y++) {
	if ((*x!=*y) && (Tcl_UniCharToLower(*x) != Tcl_UniCharToLower(*y))) {
	    return 1;
	}
    }
    return 0;
}
