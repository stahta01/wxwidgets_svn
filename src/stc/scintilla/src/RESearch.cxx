// Scintilla source code edit control
/** @file RESearch.cxx
 ** Regular expression search library.
 **/

/*
 * regex - Regular expression pattern matching and replacement
 *
 * By:  Ozan S. Yigit (oz)
 *      Dept. of Computer Science
 *      York University
 *
 * Original code available from http://www.cs.yorku.ca/~oz/
 * Translation to C++ by Neil Hodgson neilh@scintilla.org
 * Removed all use of register.
 * Converted to modern function prototypes.
 * Put all global/static variables into an object so this code can be
 * used from multiple threads, etc.
 * Some extensions by Philippe Lhoste PhiLho(a)GMX.net
 *
 * These routines are the PUBLIC DOMAIN equivalents of regex
 * routines as found in 4.nBSD UN*X, with minor extensions.
 *
 * These routines are derived from various implementations found
 * in software tools books, and Conroy's grep. They are NOT derived
 * from licensed/restricted software.
 * For more interesting/academic/complicated implementations,
 * see Henry Spencer's regexp routines, or GNU Emacs pattern
 * matching module.
 *
 * Modification history removed.
 *
 * Interfaces:
 *  RESearch::Compile:      compile a regular expression into a NFA.
 *
 *          const char *RESearch::Compile(const char *pat, int length,
 *                                        bool caseSensitive, bool posix)
 *
 * Returns a short error string if they fail.
 *
 *  RESearch::Execute:      execute the NFA to match a pattern.
 *
 *          int RESearch::Execute(characterIndexer &ci, int lp, int endp)
 *
 *  RESearch::Substitute:   substitute the matched portions in a new string.
 *
 *          int RESearch::Substitute(CharacterIndexer &ci, char *src, char *dst)
 *
 *  re_fail:                failure routine for RESearch::Execute. (no longer used)
 *
 *          void re_fail(char *msg, char op)
 *
 * Regular Expressions:
 *
 *      [1]     char    matches itself, unless it is a special
 *                      character (metachar): . \ [ ] * + ^ $
 *                      and ( ) if posix option.
 *
 *      [2]     .       matches any character.
 *
 *      [3]     \       matches the character following it, except:
 *                      - \a, \b, \f, \n, \r, \t, \v match the corresponding C
 *                      escape char, respectively BEL, BS, FF, LF, CR, TAB and VT;
 *                      Note that \r and \n are never matched because Scintilla
 *                      regex searches are made line per line
 *                      (stripped of end-of-line chars).
 *                      - if not in posix mode, when followed by a
 *                      left or right round bracket (see [7]);
 *                      - when followed by a digit 1 to 9 (see [8]);
 *                      - when followed by a left or right angle bracket
 *                      (see [9]);
 *                      - when followed by d, D, s, S, w or W (see [10]);
 *                      - when followed by x and two hexa digits (see [11].
 *                      Backslash is used as an escape character for all
 *                      other meta-characters, and itself.
 *
 *      [4]     [set]   matches one of the characters in the set.
 *                      If the first character in the set is "^",
 *                      it matches the characters NOT in the set, i.e.
 *                      complements the set. A shorthand S-E (start dash end)
 *                      is used to specify a set of characters S up to
 *                      E, inclusive. S and E must be characters, otherwise
 *                      the dash is taken literally (eg. in expression [\d-a]).
 *                      The special characters "]" and "-" have no special
 *                      meaning if they appear as the first chars in the set.
 *                      To include both, put - first: [-]A-Z]
 *                      (or just backslash them).
 *                      examples:        match:
 *
 *                              [-]|]    matches these 3 chars,
 *
 *                              []-|]    matches from ] to | chars
 *
 *                              [a-z]    any lowercase alpha
 *
 *                              [^-]]    any char except - and ]
 *
 *                              [^A-Z]   any char except uppercase
 *                                       alpha
 *
 *                              [a-zA-Z] any alpha
 *
 *      [5]     *       any regular expression form [1] to [4]
 *                      (except [7], [8] and [9] forms of [3]),
 *                      followed by closure char (*)
 *                      matches zero or more matches of that form.
 *
 *      [6]     +       same as [5], except it matches one or more.
 *                      Both [5] and [6] are greedy (they match as much as possible).
 *
 *      [7]             a regular expression in the form [1] to [12], enclosed
 *                      as \(form\) (or (form) with posix flag) matches what
 *                      form matches. The enclosure creates a set of tags,
 *                      used for [8] and for pattern substitution.
 *                      The tagged forms are numbered starting from 1.
 *
 *      [8]             a \ followed by a digit 1 to 9 matches whatever a
 *                      previously tagged regular expression ([7]) matched.
 *
 *      [9]     \<      a regular expression starting with a \< construct
 *              \>      and/or ending with a \> construct, restricts the
 *                      pattern matching to the beginning of a word, and/or
 *                      the end of a word. A word is defined to be a character
 *                      string beginning and/or ending with the characters
 *                      A-Z a-z 0-9 and _. Scintilla extends this definition
 *                      by user setting. The word must also be preceded and/or
 *                      followed by any character outside those mentioned.
 *
 *      [10]    \l      a backslash followed by d, D, s, S, w or W,
 *                      becomes a character class (both inside and
 *                      outside sets []).
 *                        d: decimal digits
 *                        D: any char except decimal digits
 *                        s: whitespace (space, \t \n \r \f \v)
 *                        S: any char except whitespace (see above)
 *                        w: alphanumeric & underscore (changed by user setting)
 *                        W: any char except alphanumeric & underscore (see above)
 *
 *      [11]    \xHH    a backslash followed by x and two hexa digits,
 *                      becomes the character whose Ascii code is equal
 *                      to these digits. If not followed by two digits,
 *                      it is 'x' char itself.
 *
 *      [12]            a composite regular expression xy where x and y
 *                      are in the form [1] to [11] matches the longest
 *                      match of x followed by a match for y.
 *
 *      [13]    ^       a regular expression starting with a ^ character
 *              $       and/or ending with a $ character, restricts the
 *                      pattern matching to the beginning of the line,
 *                      or the end of line. [anchors] Elsewhere in the
 *                      pattern, ^ and $ are treated as ordinary characters.
 *
 *
 * Acknowledgements:
 *
 *  HCR's Hugh Redelmeier has been most helpful in various
 *  stages of development. He convinced me to include BOW
 *  and EOW constructs, originally invented by Rob Pike at
 *  the University of Toronto.
 *
 * References:
 *              Software tools                  Kernighan & Plauger
 *              Software tools in Pascal        Kernighan & Plauger
 *              Grep [rsx-11 C dist]            David Conroy
 *              ed - text editor                Un*x Programmer's Manual
 *              Advanced editing on Un*x        B. W. Kernighan
 *              RegExp routines                 Henry Spencer
 *
 * Notes:
 *
 *  This implementation uses a bit-set representation for character
 *  classes for speed and compactness. Each character is represented
 *  by one bit in a 256-bit block. Thus, CCL always takes a
 *	constant 32 bytes in the internal nfa, and RESearch::Execute does a single
 *  bit comparison to locate the character in the set.
 *
 * Examples:
 *
 *  pattern:    foo*.*
 *  compile:    CHR f CHR o CLO CHR o END CLO ANY END END
 *  matches:    fo foo fooo foobar fobar foxx ...
 *
 *  pattern:    fo[ob]a[rz]
 *  compile:    CHR f CHR o CCL bitset CHR a CCL bitset END
 *  matches:    fobar fooar fobaz fooaz
 *
 *  pattern:    foo\\+
 *  compile:    CHR f CHR o CHR o CHR \ CLO CHR \ END END
 *  matches:    foo\ foo\\ foo\\\  ...
 *
 *  pattern:    \(foo\)[1-3]\1  (same as foo[1-3]foo)
 *  compile:    BOT 1 CHR f CHR o CHR o EOT 1 CCL bitset REF 1 END
 *  matches:    foo1foo foo2foo foo3foo
 *
 *  pattern:    \(fo.*\)-\1
 *  compile:    BOT 1 CHR f CHR o CLO ANY END EOT 1 CHR - REF 1 END
 *  matches:    foo-foo fo-fo fob-fob foobar-foobar ...
 */

#include "CharClassify.h"
#include "RESearch.h"

// Shut up annoying Visual C++ warnings:
#ifdef _MSC_VER
#pragma warning(disable: 4514)
#endif

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

#define OKP     1
#define NOP     0

#define CHR     1
#define ANY     2
#define CCL     3
#define BOL     4
#define EOL     5
#define BOT     6
#define EOT     7
#define BOW     8
#define EOW     9
#define REF     10
#define CLO     11

#define END     0

/*
 * The following defines are not meant to be changeable.
 * They are for readability only.
 */
#define BLKIND  0370
#define BITIND  07

const char bitarr[] = { 1, 2, 4, 8, 16, 32, 64, '\200' };

#define badpat(x)	(*nfa = END, x)

/*
 * Character classification table for word boundary operators BOW
 * and EOW is passed in by the creator of this object (Scintilla
 * Document). The Document default state is that word chars are:
 * 0-9, a-z, A-Z and _
 */

RESearch::RESearch(CharClassify *charClassTable) {
	charClass = charClassTable;
	Init();
}

RESearch::~RESearch() {
	Clear();
}

void RESearch::Init() {
	sta = NOP;                  /* status of lastpat */
	bol = 0;
	for (int i = 0; i < MAXTAG; i++)
		pat[i] = 0;
	for (int j = 0; j < BITBLK; j++)
		bittab[j] = 0;
}

void RESearch::Clear() {
	for (int i = 0; i < MAXTAG; i++) {
		delete []pat[i];
		pat[i] = 0;
		bopat[i] = NOTFOUND;
		eopat[i] = NOTFOUND;
	}
}

bool RESearch::GrabMatches(CharacterIndexer &ci) {
	bool success = true;
	for (unsigned int i = 0; i < MAXTAG; i++) {
		if ((bopat[i] != NOTFOUND) && (eopat[i] != NOTFOUND)) {
			unsigned int len = eopat[i] - bopat[i];
			pat[i] = new char[len + 1];
			if (pat[i]) {
				for (unsigned int j = 0; j < len; j++)
					pat[i][j] = ci.CharAt(bopat[i] + j);
				pat[i][len] = '\0';
			} else {
				success = false;
			}
		}
	}
	return success;
}

void RESearch::ChSet(unsigned char c) {
	bittab[((c) & BLKIND) >> 3] |= bitarr[(c) & BITIND];
}

void RESearch::ChSetWithCase(unsigned char c, bool caseSensitive) {
	if (caseSensitive) {
		ChSet(c);
	} else {
		if ((c >= 'a') && (c <= 'z')) {
			ChSet(c);
			ChSet(static_cast<unsigned char>(c - 'a' + 'A'));
		} else if ((c >= 'A') && (c <= 'Z')) {
			ChSet(c);
			ChSet(static_cast<unsigned char>(c - 'A' + 'a'));
		} else {
			ChSet(c);
		}
	}
}

const unsigned char escapeValue(unsigned char ch) {
	switch (ch) {
	case 'a':	return '\a';
	case 'b':	return '\b';
	case 'f':	return '\f';
	case 'n':	return '\n';
	case 'r':	return '\r';
	case 't':	return '\t';
	case 'v':	return '\v';
	}
	return 0;
}

static int GetHexaChar(unsigned char hd1, unsigned char hd2) {
	int hexValue = 0;
	if (hd1 >= '0' && hd1 <= '9') {
		hexValue += 16 * (hd1 - '0');
	} else if (hd1 >= 'A' && hd1 <= 'F') {
		hexValue += 16 * (hd1 - 'A' + 10);
	} else if (hd1 >= 'a' && hd1 <= 'f') {
		hexValue += 16 * (hd1 - 'a' + 10);
	} else
		return -1;
	if (hd2 >= '0' && hd2 <= '9') {
		hexValue += hd2 - '0';
	} else if (hd2 >= 'A' && hd2 <= 'F') {
		hexValue += hd2 - 'A' + 10;
	} else if (hd2 >= 'a' && hd2 <= 'f') {
		hexValue += hd2 - 'a' + 10;
	} else
		return -1;
	return hexValue;
}

/**
 * Called when the parser finds a backslash not followed
 * by a valid expression (like \( in non-Posix mode).
 * @param pat: pointer on the char after the backslash.
 * @param incr: (out) number of chars to skip after expression evaluation.
 * @return the char if it resolves to a simple char,
 * or -1 for a char class. In this case, bittab is changed.
 */
int RESearch::GetBackslashExpression(
		const char *pat,
		int &incr) {
	// Since error reporting is primitive and messages are not used anyway,
	// I choose to interpret unexpected syntax in a logical way instead
	// of reporting errors. Otherwise, we can stick on, eg., PCRE behavior.
	incr = 0;	// Most of the time, will skip the char "naturally".
	int c;
	int result = -1;
	unsigned char bsc = *pat;
	if (!bsc) {
		// Avoid overrun
		result = '\\';	// \ at end of pattern, take it literally
		return result;
	}

	switch (bsc) {
	case 'a':
	case 'b':
	case 'n':
	case 'f':
	case 'r':
	case 't':
	case 'v':
		result = escapeValue(bsc);
		break;
	case 'x': {
			unsigned char hd1 = *(pat + 1);
			unsigned char hd2 = *(pat + 2);
			int hexValue = GetHexaChar(hd1, hd2);
			if (hexValue >= 0) {
				result = hexValue;
				incr = 2;	// Must skip the digits
			} else {
				result = 'x';	// \x without 2 digits: see it as 'x'
			}
		}
		break;
	case 'd':
		for (c = '0'; c <= '9'; c++) {
			ChSet(static_cast<unsigned char>(c));
		}
		break;
	case 'D':
		for (c = 0; c < MAXCHR; c++) {
			if (c < '0' || c > '9') {
				ChSet(static_cast<unsigned char>(c));
			}
		}
		break;
	case 's':
		ChSet(' ');
		ChSet('\t');
		ChSet('\n');
		ChSet('\r');
		ChSet('\f');
		ChSet('\v');
		break;
	case 'S':
		for (c = 0; c < MAXCHR; c++) {
			if (c != ' ' && !(c >= 0x09 && c <= 0x0D)) {
				ChSet(static_cast<unsigned char>(c));
			}
		}
	case 'w':
		for (c = 0; c < MAXCHR; c++) {
			if (iswordc(static_cast<unsigned char>(c))) {
				ChSet(static_cast<unsigned char>(c));
			}
		}
		break;
	case 'W':
		for (c = 0; c < MAXCHR; c++) {
			if (!iswordc(static_cast<unsigned char>(c))) {
				ChSet(static_cast<unsigned char>(c));
			}
		}
		break;
	default:
		result = bsc;
	}
	return result;
}

const char *RESearch::Compile(const char *pat, int length, bool caseSensitive, bool posix) {
	char *mp=nfa;          /* nfa pointer       */
	char *lp;              /* saved pointer     */
	char *sp=nfa;          /* another one       */
	char *mpMax = mp + MAXNFA - BITBLK - 10;

	int tagi = 0;          /* tag stack index   */
	int tagc = 1;          /* actual tag count  */

	int n;
	char mask;             /* xor mask -CCL/NCL */
	int c1, c2, prevChar;

	if (!pat || !length)
		if (sta)
			return 0;
		else
			return badpat("No previous regular expression");
	sta = NOP;

	const char *p=pat;     /* pattern pointer   */
	for (int i=0; i<length; i++, p++) {
		if (mp > mpMax)
			return badpat("Pattern too long");
		lp = mp;
		switch (*p) {

		case '.':               /* match any char  */
			*mp++ = ANY;
			break;

		case '^':               /* match beginning */
			if (p == pat)
				*mp++ = BOL;
			else {
				*mp++ = CHR;
				*mp++ = *p;
			}
			break;

		case '$':               /* match endofline */
			if (!*(p+1))
				*mp++ = EOL;
			else {
				*mp++ = CHR;
				*mp++ = *p;
			}
			break;

		case '[':               /* match char class */
			*mp++ = CCL;
			prevChar = 0;

			i++;
			if (*++p == '^') {
				mask = '\377';
				i++;
				p++;
			} else
				mask = 0;

			if (*p == '-') {	/* real dash */
				i++;
				prevChar = *p;
				ChSet(*p++);
			}
			if (*p == ']') {	/* real brace */
				i++;
				prevChar = *p;
				ChSet(*p++);
			}
			while (*p && *p != ']') {
				if (*p == '-') {
					if (prevChar < 0) {
						// Previous def. was a char class like \d, take dash literally
						prevChar = *p;
						ChSet(*p);
					} else if (*(p+1)) {
						if (*(p+1) != ']') {
							c1 = prevChar + 1;
							i++;
							c2 = *++p;
							if (c2 == '\\') {
								if (!*(p+1))	// End of RE
									return badpat("Missing ]");
								else {
									i++;
									p++;
									int incr;
									c2 = GetBackslashExpression(p, incr);
									i += incr;
									p += incr;
									if (c2 >= 0) {
										// Convention: \c (c is any char) is case sensitive, whatever the option
										ChSet(static_cast<unsigned char>(c2));
										prevChar = c2;
									} else {
										// bittab is already changed
										prevChar = -1;
									}
								}
							}
							if (prevChar < 0) {
								// Char after dash is char class like \d, take dash literally
								prevChar = '-';
								ChSet('-');
							} else {
								// Put all chars between c1 and c2 included in the char set
								while (c1 <= c2) {
									ChSetWithCase(static_cast<unsigned char>(c1++), caseSensitive);
								}
							}
						} else {
							// Dash before the ], take it literally
							prevChar = *p;
							ChSet(*p);
						}
					} else {
						return badpat("Missing ]");
					}
				} else if (*p == '\\' && *(p+1)) {
					i++;
					p++;
					int incr;
					int c = GetBackslashExpression(p, incr);
					i += incr;
					p += incr;
					if (c >= 0) {
						// Convention: \c (c is any char) is case sensitive, whatever the option
						ChSet(static_cast<unsigned char>(c));
						prevChar = c;
					} else {
						// bittab is already changed
						prevChar = -1;
					}
				} else {
					prevChar = *p;
					ChSetWithCase(*p, caseSensitive);
				}
				i++;
				p++;
			}
			if (!*p)
				return badpat("Missing ]");

			for (n = 0; n < BITBLK; bittab[n++] = 0)
				*mp++ = static_cast<char>(mask ^ bittab[n]);

			break;

		case '*':               /* match 0 or more... */
		case '+':               /* match 1 or more... */
			if (p == pat)
				return badpat("Empty closure");
			lp = sp;		/* previous opcode */
			if (*lp == CLO)		/* equivalence... */
				break;
			switch (*lp) {

			case BOL:
			case BOT:
			case EOT:
			case BOW:
			case EOW:
			case REF:
				return badpat("Illegal closure");
			default:
				break;
			}

			if (*p == '+')
				for (sp = mp; lp < sp; lp++)
					*mp++ = *lp;

			*mp++ = END;
			*mp++ = END;
			sp = mp;
			while (--mp > lp)
				*mp = mp[-1];
			*mp = CLO;
			mp = sp;
			break;

		case '\\':              /* tags, backrefs... */
			i++;
			switch (*++p) {
			case '<':
				*mp++ = BOW;
				break;
			case '>':
				if (*sp == BOW)
					return badpat("Null pattern inside \\<\\>");
				*mp++ = EOW;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				n = *p-'0';
				if (tagi > 0 && tagstk[tagi] == n)
					return badpat("Cyclical reference");
				if (tagc > n) {
					*mp++ = static_cast<char>(REF);
					*mp++ = static_cast<char>(n);
				} else
					return badpat("Undetermined reference");
				break;
			default:
				if (!posix && *p == '(') {
					if (tagc < MAXTAG) {
						tagstk[++tagi] = tagc;
						*mp++ = BOT;
						*mp++ = static_cast<char>(tagc++);
					} else
						return badpat("Too many \\(\\) pairs");
				} else if (!posix && *p == ')') {
					if (*sp == BOT)
						return badpat("Null pattern inside \\(\\)");
					if (tagi > 0) {
						*mp++ = static_cast<char>(EOT);
						*mp++ = static_cast<char>(tagstk[tagi--]);
					} else
						return badpat("Unmatched \\)");
				} else {
					int incr;
					int c = GetBackslashExpression(p, incr);
					i += incr;
					p += incr;
					if (c >= 0) {
						*mp++ = CHR;
						*mp++ = static_cast<unsigned char>(c);
					} else {
						*mp++ = CCL;
						mask = 0;
						for (n = 0; n < BITBLK; bittab[n++] = 0)
							*mp++ = static_cast<char>(mask ^ bittab[n]);
					}
				}
			}
			break;

		default :               /* an ordinary char */
			if (posix && *p == '(') {
				if (tagc < MAXTAG) {
					tagstk[++tagi] = tagc;
					*mp++ = BOT;
					*mp++ = static_cast<char>(tagc++);
				} else
					return badpat("Too many () pairs");
			} else if (posix && *p == ')') {
				if (*sp == BOT)
					return badpat("Null pattern inside ()");
				if (tagi > 0) {
					*mp++ = static_cast<char>(EOT);
					*mp++ = static_cast<char>(tagstk[tagi--]);
				} else
					return badpat("Unmatched )");
			} else {
				unsigned char c = *p;
				if (!c)	// End of RE
					c = '\\';	// We take it as raw backslash
				if (caseSensitive || !iswordc(c)) {
					*mp++ = CHR;
					*mp++ = c;
				} else {
					*mp++ = CCL;
					mask = 0;
					ChSetWithCase(c, false);
					for (n = 0; n < BITBLK; bittab[n++] = 0)
						*mp++ = static_cast<char>(mask ^ bittab[n]);
				}
			}
			break;
		}
		sp = lp;
	}
	if (tagi > 0)
		return badpat((posix ? "Unmatched (" : "Unmatched \\("));
	*mp = END;
	sta = OKP;
	return 0;
}

/*
 * RESearch::Execute:
 *   execute nfa to find a match.
 *
 *  special cases: (nfa[0])
 *      BOL
 *          Match only once, starting from the
 *          beginning.
 *      CHR
 *          First locate the character without
 *          calling PMatch, and if found, call
 *          PMatch for the remaining string.
 *      END
 *          RESearch::Compile failed, poor luser did not
 *          check for it. Fail fast.
 *
 *  If a match is found, bopat[0] and eopat[0] are set
 *  to the beginning and the end of the matched fragment,
 *  respectively.
 *
 */
int RESearch::Execute(CharacterIndexer &ci, int lp, int endp) {
	unsigned char c;
	int ep = NOTFOUND;
	char *ap = nfa;

	bol = lp;
	failure = 0;

	Clear();

	switch (*ap) {

	case BOL:			/* anchored: match from BOL only */
		ep = PMatch(ci, lp, endp, ap);
		break;
	case EOL:			/* just searching for end of line normal path doesn't work */
		if (*(ap+1) == END) {
			lp = endp;
			ep = lp;
			break;
		} else {
			return 0;
		}
	case CHR:			/* ordinary char: locate it fast */
		c = *(ap+1);
		while ((lp < endp) && (ci.CharAt(lp) != c))
			lp++;
		if (lp >= endp)	/* if EOS, fail, else fall thru. */
			return 0;
	default:			/* regular matching all the way. */
		while (lp < endp) {
			ep = PMatch(ci, lp, endp, ap);
			if (ep != NOTFOUND)
				break;
			lp++;
		}
		break;
	case END:			/* munged automaton. fail always */
		return 0;
	}
	if (ep == NOTFOUND)
		return 0;

	bopat[0] = lp;
	eopat[0] = ep;
	return 1;
}

/*
 * PMatch: internal routine for the hard part
 *
 *  This code is partly snarfed from an early grep written by
 *  David Conroy. The backref and tag stuff, and various other
 *  innovations are by oz.
 *
 *  special case optimizations: (nfa[n], nfa[n+1])
 *      CLO ANY
 *          We KNOW .* will match everything upto the
 *          end of line. Thus, directly go to the end of
 *          line, without recursive PMatch calls. As in
 *          the other closure cases, the remaining pattern
 *          must be matched by moving backwards on the
 *          string recursively, to find a match for xy
 *          (x is ".*" and y is the remaining pattern)
 *          where the match satisfies the LONGEST match for
 *          x followed by a match for y.
 *      CLO CHR
 *          We can again scan the string forward for the
 *          single char and at the point of failure, we
 *          execute the remaining nfa recursively, same as
 *          above.
 *
 *  At the end of a successful match, bopat[n] and eopat[n]
 *  are set to the beginning and end of subpatterns matched
 *  by tagged expressions (n = 1 to 9).
 */

extern void re_fail(char *,char);

#define isinset(x,y)	((x)[((y)&BLKIND)>>3] & bitarr[(y)&BITIND])

/*
 * skip values for CLO XXX to skip past the closure
 */

#define ANYSKIP 2 	/* [CLO] ANY END          */
#define CHRSKIP 3	/* [CLO] CHR chr END      */
#define CCLSKIP 34	/* [CLO] CCL 32 bytes END */

int RESearch::PMatch(CharacterIndexer &ci, int lp, int endp, char *ap) {
	int op, c, n;
	int e;		/* extra pointer for CLO  */
	int bp;		/* beginning of subpat... */
	int ep;		/* ending of subpat...    */
	int are;	/* to save the line ptr.  */

	while ((op = *ap++) != END)
		switch (op) {

		case CHR:
			if (ci.CharAt(lp++) != *ap++)
				return NOTFOUND;
			break;
		case ANY:
			if (lp++ >= endp)
				return NOTFOUND;
			break;
		case CCL:
			c = ci.CharAt(lp++);
			if (!isinset(ap,c))
				return NOTFOUND;
			ap += BITBLK;
			break;
		case BOL:
			if (lp != bol)
				return NOTFOUND;
			break;
		case EOL:
			if (lp < endp)
				return NOTFOUND;
			break;
		case BOT:
			bopat[*ap++] = lp;
			break;
		case EOT:
			eopat[*ap++] = lp;
			break;
 		case BOW:
			if (lp!=bol && iswordc(ci.CharAt(lp-1)) || !iswordc(ci.CharAt(lp)))
				return NOTFOUND;
			break;
		case EOW:
			if (lp==bol || !iswordc(ci.CharAt(lp-1)) || iswordc(ci.CharAt(lp)))
				return NOTFOUND;
			break;
		case REF:
			n = *ap++;
			bp = bopat[n];
			ep = eopat[n];
			while (bp < ep)
				if (ci.CharAt(bp++) != ci.CharAt(lp++))
					return NOTFOUND;
			break;
		case CLO:
			are = lp;
			switch (*ap) {

			case ANY:
				while (lp < endp)
					lp++;
				n = ANYSKIP;
				break;
			case CHR:
				c = *(ap+1);
				while ((lp < endp) && (c == ci.CharAt(lp)))
					lp++;
				n = CHRSKIP;
				break;
			case CCL:
				while ((lp < endp) && isinset(ap+1,ci.CharAt(lp)))
					lp++;
				n = CCLSKIP;
				break;
			default:
				failure = true;
				//re_fail("closure: bad nfa.", *ap);
				return NOTFOUND;
			}

			ap += n;

			while (lp >= are) {
				if ((e = PMatch(ci, lp, endp, ap)) != NOTFOUND)
					return e;
				--lp;
			}
			return NOTFOUND;
		default:
			//re_fail("RESearch::Execute: bad nfa.", static_cast<char>(op));
			return NOTFOUND;
		}
	return lp;
}

/*
 * RESearch::Substitute:
 *  substitute the matched portions of the src in dst.
 *
 *  &    substitute the entire matched pattern.
 *
 *  \digit  substitute a subpattern, with the given tag number.
 *      Tags are numbered from 1 to 9. If the particular
 *      tagged subpattern does not exist, null is substituted.
 */
int RESearch::Substitute(CharacterIndexer &ci, char *src, char *dst) {
	unsigned char c;
	int  pin;
	int bp;
	int ep;

	if (!*src || !bopat[0])
		return 0;

	while ((c = *src++) != 0) {
		switch (c) {

		case '&':
			pin = 0;
			break;

		case '\\':
			c = *src++;
			if (c >= '0' && c <= '9') {
				pin = c - '0';
				break;
			}

		default:
			*dst++ = c;
			continue;
		}

		if ((bp = bopat[pin]) != 0 && (ep = eopat[pin]) != 0) {
			while (ci.CharAt(bp) && bp < ep)
				*dst++ = ci.CharAt(bp++);
			if (bp < ep)
				return 0;
		}
	}
	*dst = '\0';
	return 1;
}

