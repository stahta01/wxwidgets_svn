// Scintilla source code edit control
/** @file KeyWords.cxx
 ** Colourise for particular languages.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

/**
 * Creates an array that points into each word in the string and puts \0 terminators
 * after each word.
 */
static char **ArrayFromWordList(char *wordlist, int *len, bool onlyLineEnds = false) {
	int prev = '\n';
	int words = 0;
	// For rapid determination of whether a character is a separator, build
	// a look up table.
	bool wordSeparator[256];
	for (int i=0;i<256; i++) {
		wordSeparator[i] = false;
	}
	wordSeparator['\r'] = true;
	wordSeparator['\n'] = true;
	if (!onlyLineEnds) {
		wordSeparator[' '] = true;
		wordSeparator['\t'] = true;
	}
	for (int j = 0; wordlist[j]; j++) {
		int curr = static_cast<unsigned char>(wordlist[j]);
		if (!wordSeparator[curr] && wordSeparator[prev])
			words++;
		prev = curr;
	}
	char **keywords = new char *[words + 1];
	if (keywords) {
		words = 0;
		prev = '\0';
		size_t slen = strlen(wordlist);
		for (size_t k = 0; k < slen; k++) {
			if (!wordSeparator[static_cast<unsigned char>(wordlist[k])]) {
				if (!prev) {
					keywords[words] = &wordlist[k];
					words++;
				}
			} else {
				wordlist[k] = '\0';
			}
			prev = wordlist[k];
		}
		keywords[words] = &wordlist[slen];
		*len = words;
	} else {
		*len = 0;
	}
	return keywords;
}

void WordList::Clear() {
	if (words) {
		delete []list;
		delete []words;
	}
	words = 0;
	list = 0;
	len = 0;
	sorted = false;
}

void WordList::Set(const char *s) {
	list = new char[strlen(s) + 1];
	strcpy(list, s);
	sorted = false;
	words = ArrayFromWordList(list, &len, onlyLineEnds);
}

extern "C" int cmpString(const void *a1, const void *a2) {
	// Can't work out the correct incantation to use modern casts here
	return strcmp(*(char**)(a1), *(char**)(a2));
}

static void SortWordList(char **words, unsigned int len) {
	qsort(reinterpret_cast<void*>(words), len, sizeof(*words),
	      cmpString);
}

bool WordList::InList(const char *s) {
	if (0 == words)
		return false;
	if (!sorted) {
		sorted = true;
		SortWordList(words, len);
		for (unsigned int k = 0; k < (sizeof(starts) / sizeof(starts[0])); k++)
			starts[k] = -1;
		for (int l = len - 1; l >= 0; l--) {
			unsigned char indexChar = words[l][0];
			starts[indexChar] = l;
		}
	}
	unsigned char firstChar = s[0];
	int j = starts[firstChar];
	if (j >= 0) {
		while ((unsigned char)words[j][0] == firstChar) {
			if (s[1] == words[j][1]) {
				const char *a = words[j] + 1;
				const char *b = s + 1;
				while (*a && *a == *b) {
					a++;
					b++;
				}
				if (!*a && !*b)
					return true;
			}
			j++;
		}
	}
	j = starts['^'];
	if (j >= 0) {
		while (words[j][0] == '^') {
			const char *a = words[j] + 1;
			const char *b = s;
			while (*a && *a == *b) {
				a++;
				b++;
			}
			if (!*a)
				return true;
			j++;
		}
	}
	return false;
}

/** similar to InList, but word s can be a substring of keyword.
 * eg. the keyword define is defined as def~ine. This means the word must start
 * with def to be a keyword, but also defi, defin and define are valid.
 * The marker is ~ in this case.
 */
bool WordList::InListAbbreviated(const char *s, const char marker) {
	if (0 == words)
		return false;
	if (!sorted) {
		sorted = true;
		SortWordList(words, len);
		for (unsigned int k = 0; k < (sizeof(starts) / sizeof(starts[0])); k++)
			starts[k] = -1;
		for (int l = len - 1; l >= 0; l--) {
			unsigned char indexChar = words[l][0];
			starts[indexChar] = l;
		}
	}
	unsigned char firstChar = s[0];
	int j = starts[firstChar];
	if (j >= 0) {
		while (words[j][0] == firstChar) {
			bool isSubword = false;
			int start = 1;
			if (words[j][1] == marker) {
				isSubword = true;
				start++;
			}
			if (s[1] == words[j][start]) {
				const char *a = words[j] + start;
				const char *b = s + 1;
				while (*a && *a == *b) {
					a++;
					if (*a == marker) {
						isSubword = true;
						a++;
					}
					b++;
				}
				if ((!*a || isSubword) && !*b)
					return true;
			}
			j++;
		}
	}
	j = starts['^'];
	if (j >= 0) {
		while (words[j][0] == '^') {
			const char *a = words[j] + 1;
			const char *b = s;
			while (*a && *a == *b) {
				a++;
				b++;
			}
			if (!*a)
				return true;
			j++;
		}
	}
	return false;
}

const LexerModule *LexerModule::base = 0;
int LexerModule::nextLanguage = SCLEX_AUTOMATIC+1;

LexerModule::LexerModule(int language_,
	LexerFunction fnLexer_,
	const char *languageName_,
	LexerFunction fnFolder_,
	const char * const wordListDescriptions_[],
	int styleBits_) :
	language(language_),
	fnLexer(fnLexer_),
	fnFolder(fnFolder_),
	wordListDescriptions(wordListDescriptions_),
	styleBits(styleBits_),
	languageName(languageName_) {
	next = base;
	base = this;
	if (language == SCLEX_AUTOMATIC) {
		language = nextLanguage;
		nextLanguage++;
	}
}

int LexerModule::GetNumWordLists() const {
	if (wordListDescriptions == NULL) {
		return -1;
	} else {
		int numWordLists = 0;

		while (wordListDescriptions[numWordLists]) {
			++numWordLists;
		}

		return numWordLists;
	}
}

const char *LexerModule::GetWordListDescription(int index) const {
	static const char *emptyStr = "";

	PLATFORM_ASSERT(index < GetNumWordLists());
	if (index >= GetNumWordLists()) {
		return emptyStr;
	} else {
		return wordListDescriptions[index];
 	}
}

int LexerModule::GetStyleBitsNeeded() const {
	return styleBits;
}

const LexerModule *LexerModule::Find(int language) {
	const LexerModule *lm = base;
	while (lm) {
		if (lm->language == language) {
			return lm;
		}
		lm = lm->next;
	}
	return 0;
}

const LexerModule *LexerModule::Find(const char *languageName) {
	if (languageName) {
		const LexerModule *lm = base;
		while (lm) {
			if (lm->languageName && 0 == strcmp(lm->languageName, languageName)) {
				return lm;
			}
			lm = lm->next;
		}
	}
	return 0;
}

void LexerModule::Lex(unsigned int startPos, int lengthDoc, int initStyle,
	  WordList *keywordlists[], Accessor &styler) const {
	if (fnLexer)
		fnLexer(startPos, lengthDoc, initStyle, keywordlists, styler);
}

void LexerModule::Fold(unsigned int startPos, int lengthDoc, int initStyle,
	  WordList *keywordlists[], Accessor &styler) const {
	if (fnFolder) {
		int lineCurrent = styler.GetLine(startPos);
		// Move back one line in case deletion wrecked current line fold state
		if (lineCurrent > 0) {
			lineCurrent--;
			int newStartPos = styler.LineStart(lineCurrent);
			lengthDoc += startPos - newStartPos;
			startPos = newStartPos;
			initStyle = 0;
			if (startPos > 0) {
				initStyle = styler.StyleAt(startPos - 1);
			}
		}
		fnFolder(startPos, lengthDoc, initStyle, keywordlists, styler);
	}
}

// Alternative historical name for Scintilla_LinkLexers
int wxForceScintillaLexers(void) {
	return Scintilla_LinkLexers();
}

// To add or remove a lexer, add or remove its file and run LexGen.py.

// Force a reference to all of the Scintilla lexers so that the linker will
// not remove the code of the lexers.
int Scintilla_LinkLexers() {
	static int forcer = 0;

// Shorten the code that declares a lexer and ensures it is linked in by calling a method.
#define LINK_LEXER(lexer) extern LexerModule lexer; forcer += lexer.GetLanguage();

//++Autogenerated -- run src/LexGen.py to regenerate
//**\(\tLINK_LEXER(\*);\n\)
	LINK_LEXER(lmAbaqus);
	LINK_LEXER(lmAda);
	LINK_LEXER(lmAns1);
	LINK_LEXER(lmAPDL);
	LINK_LEXER(lmAsm);
	LINK_LEXER(lmASY);
	LINK_LEXER(lmAU3);
	LINK_LEXER(lmAVE);
	LINK_LEXER(lmBaan);
	LINK_LEXER(lmBash);
	LINK_LEXER(lmBatch);
	LINK_LEXER(lmBlitzBasic);
	LINK_LEXER(lmBullant);
	LINK_LEXER(lmCaml);
	LINK_LEXER(lmClw);
	LINK_LEXER(lmClwNoCase);
	LINK_LEXER(lmCmake);
	LINK_LEXER(lmCOBOL);
	LINK_LEXER(lmConf);
	LINK_LEXER(lmCPP);
	LINK_LEXER(lmCPPNoCase);
	LINK_LEXER(lmCsound);
	LINK_LEXER(lmCss);
	LINK_LEXER(lmD);
	LINK_LEXER(lmDiff);
	LINK_LEXER(lmEiffel);
	LINK_LEXER(lmEiffelkw);
	LINK_LEXER(lmErlang);
	LINK_LEXER(lmErrorList);
	LINK_LEXER(lmESCRIPT);
	LINK_LEXER(lmF77);
	LINK_LEXER(lmFlagShip);
	LINK_LEXER(lmForth);
	LINK_LEXER(lmFortran);
	LINK_LEXER(lmFreeBasic);
	LINK_LEXER(lmGAP);
	LINK_LEXER(lmGui4Cli);
	LINK_LEXER(lmHaskell);
	LINK_LEXER(lmHTML);
	LINK_LEXER(lmInno);
	LINK_LEXER(lmKix);
	LINK_LEXER(lmLatex);
	LINK_LEXER(lmLISP);
	LINK_LEXER(lmLot);
	LINK_LEXER(lmLout);
	LINK_LEXER(lmLua);
	LINK_LEXER(lmMagikSF);
	LINK_LEXER(lmMake);
	LINK_LEXER(lmMarkdown);
	LINK_LEXER(lmMatlab);
	LINK_LEXER(lmMETAPOST);
	LINK_LEXER(lmMMIXAL);
	LINK_LEXER(lmMSSQL);
	LINK_LEXER(lmMySQL);
	LINK_LEXER(lmNimrod);
	LINK_LEXER(lmNncrontab);
	LINK_LEXER(lmNsis);
	LINK_LEXER(lmNull);
	LINK_LEXER(lmOctave);
	LINK_LEXER(lmOpal);
	LINK_LEXER(lmPascal);
	LINK_LEXER(lmPB);
	LINK_LEXER(lmPerl);
	LINK_LEXER(lmPHPSCRIPT);
	LINK_LEXER(lmPLM);
	LINK_LEXER(lmPo);
	LINK_LEXER(lmPOV);
	LINK_LEXER(lmPowerPro);
	LINK_LEXER(lmPowerShell);
	LINK_LEXER(lmProgress);
	LINK_LEXER(lmProps);
	LINK_LEXER(lmPS);
	LINK_LEXER(lmPureBasic);
	LINK_LEXER(lmPython);
	LINK_LEXER(lmR);
	LINK_LEXER(lmREBOL);
	LINK_LEXER(lmRuby);
	LINK_LEXER(lmScriptol);
	LINK_LEXER(lmSmalltalk);
	LINK_LEXER(lmSML);
	LINK_LEXER(lmSorc);
	LINK_LEXER(lmSpecman);
	LINK_LEXER(lmSpice);
	LINK_LEXER(lmSQL);
	LINK_LEXER(lmTACL);
	LINK_LEXER(lmTADS3);
	LINK_LEXER(lmTAL);
	LINK_LEXER(lmTCL);
	LINK_LEXER(lmTeX);
	LINK_LEXER(lmVB);
	LINK_LEXER(lmVBScript);
	LINK_LEXER(lmVerilog);
	LINK_LEXER(lmVHDL);
	LINK_LEXER(lmXML);
	LINK_LEXER(lmYAML);

//--Autogenerated -- end of automatically generated section

	return 1;
}
