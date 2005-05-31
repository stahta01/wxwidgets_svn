// Scintilla source code edit control

// File: LexMetapost.cxx - general context conformant metapost coloring scheme
// Author: Hans Hagen - PRAGMA ADE - Hasselt NL - www.pragma-ade.com
// Version: September 28, 2003

// Copyright: 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

// This lexer is derived from the one written for the texwork environment (1999++) which in
// turn is inspired on texedit (1991++) which finds its roots in wdt (1986).

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
#include "StyleContext.h"

// val SCE_METAPOST_DEFAULT = 0
// val SCE_METAPOST_SPECIAL = 1
// val SCE_METAPOST_GROUP = 2
// val SCE_METAPOST_SYMBOL = 3
// val SCE_METAPOST_COMMAND = 4
// val SCE_METAPOST_TEXT = 5

// Definitions in SciTEGlobal.properties:
//
// Metapost Highlighting
//
// # Default
// style.metapost.0=fore:#7F7F00
// # Special
// style.metapost.1=fore:#007F7F
// # Group
// style.metapost.2=fore:#880000
// # Symbol
// style.metapost.3=fore:#7F7F00
// # Command
// style.metapost.4=fore:#008800
// # Text
// style.metapost.5=fore:#000000

// lexer.tex.comment.process=0

// Auxiliary functions:

static inline bool endOfLine(Accessor &styler, unsigned int i) {
	return
      (styler[i] == '\n') || ((styler[i] == '\r') && (styler.SafeGetCharAt(i + 1) != '\n')) ;
}

static inline bool isMETAPOSTcomment(int ch) {
	return
      (ch == '%') ;
}

static inline bool isMETAPOSTone(int ch) {
	return
      (ch == '[') || (ch == ']') || (ch == '(') || (ch == ')') ||
      (ch == ':') || (ch == '=') || (ch == '<') || (ch == '>') ||
      (ch == '{') || (ch == '}') || (ch == '\'') || (ch == '\"') ;
}

static inline bool isMETAPOSTtwo(int ch) {
	return
      (ch == ';') || (ch == '$') || (ch == '@') || (ch == '#');
}

static inline bool isMETAPOSTthree(int ch) {
	return
      (ch == '.') || (ch == '-') || (ch == '+') || (ch == '/') ||
      (ch == '*') || (ch == ',') || (ch == '|') || (ch == '`') ||
      (ch == '!') || (ch == '?') || (ch == '^') || (ch == '&') ||
      (ch == '%') ;
}

static inline bool isMETAPOSTidentifier(int ch) {
	return
      ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) ||
      (ch == '_') ;
}

static inline bool isMETAPOSTnumber(int ch) {
	return
      (ch >= '0') && (ch <= '9') ;
}

static inline bool isMETAPOSTstring(int ch) {
	return
      (ch == '\"') ;
}

static inline bool isMETAPOSTcolon(int ch) {
	return
		(ch == ':') ;
}

static inline bool isMETAPOSTequal(int ch) {
	return
		(ch == '=') ;
}

static int CheckMETAPOSTInterface(
    unsigned int startPos,
    int length,
    Accessor &styler,
	int defaultInterface) {

    char lineBuffer[1024] ;
	unsigned int linePos = 0 ;

	// some day we can make something lexer.metapost.mapping=(none,0)(metapost,1)(mp,1)(metafun,2)...

    if (styler.SafeGetCharAt(0) == '%') {
        for (unsigned int i = 0; i < startPos + length; i++) {
            lineBuffer[linePos++] = styler.SafeGetCharAt(i) ;
            if (endOfLine(styler, i) || (linePos >= sizeof(lineBuffer) - 1)) {
                lineBuffer[linePos] = '\0';
				if (strstr(lineBuffer, "interface=none")) {
                    return 0 ;
				} else if (strstr(lineBuffer, "interface=metapost") || strstr(lineBuffer, "interface=mp")) {
                    return 1 ;
				} else if (strstr(lineBuffer, "interface=metafun")) {
                    return 2 ;
				} else if (styler.SafeGetCharAt(1) == 'D' && strstr(lineBuffer, "%D \\module")) {
					// better would be to limit the search to just one line
					return 2 ;
                } else {
                    return defaultInterface ;
                }
            }
		}
    }

    return defaultInterface ;
}

static void ColouriseMETAPOSTDoc(
    unsigned int startPos,
    int length,
    int,
    WordList *keywordlists[],
    Accessor &styler) {

	styler.StartAt(startPos) ;
	styler.StartSegment(startPos) ;

	bool processComment   = styler.GetPropertyInt("lexer.metapost.comment.process",   0) == 1 ;
    int  defaultInterface = styler.GetPropertyInt("lexer.metapost.interface.default", 1) ;

	int currentInterface = CheckMETAPOSTInterface(startPos,length,styler,defaultInterface) ;

	// 0  no keyword highlighting
	// 1  metapost keyword highlighting
	// 2+ metafun keyword highlighting

	int extraInterface = 0 ;

	if (currentInterface != 0) {
		extraInterface = currentInterface ;
	}

	WordList &keywords  = *keywordlists[0] ;
	WordList &keywords2 = *keywordlists[extraInterface-1] ;

	StyleContext sc(startPos, length, SCE_METAPOST_TEXT, styler) ;

	char key[100] ;

    bool inTeX     = false ;
	bool inComment = false ;
	bool inString  = false ;
	bool inClause  = false ;

	bool going = sc.More() ; // needed because of a fuzzy end of file state

	for (; going; sc.Forward()) {

		if (! sc.More()) { going = false ; } // we need to go one behind the end of text

		if (inClause) {
			sc.SetState(SCE_METAPOST_TEXT) ;
			inClause = false ;
		}

		if (inComment) {
			if (sc.atLineEnd) {
				sc.SetState(SCE_METAPOST_TEXT) ;
				inTeX = false ;
				inComment = false ;
				inClause = false ;
				inString = false ; // not correct but we want to stimulate one-lines
			}
		} else if (inString) {
			if (isMETAPOSTstring(sc.ch)) {
				sc.SetState(SCE_METAPOST_SPECIAL) ;
				sc.ForwardSetState(SCE_METAPOST_TEXT) ;
				inString = false ;
			} else if (sc.atLineEnd) {
				sc.SetState(SCE_METAPOST_TEXT) ;
				inTeX = false ;
				inComment = false ;
				inClause = false ;
				inString = false ; // not correct but we want to stimulate one-lines
			}
		} else {
			if ((! isMETAPOSTidentifier(sc.ch)) && (sc.LengthCurrent() > 0)) {
				if (sc.state == SCE_METAPOST_COMMAND) {
					sc.GetCurrent(key, sizeof(key)) ;
					if ((strcmp(key,"btex") == 0) || (strcmp(key,"verbatimtex") == 0)) {
    					sc.ChangeState(SCE_METAPOST_GROUP) ;
						inTeX = true ;
					} else if (inTeX) {
						if (strcmp(key,"etex") == 0) {
	    					sc.ChangeState(SCE_METAPOST_GROUP) ;
							inTeX = false ;
						} else {
	    					sc.ChangeState(SCE_METAPOST_TEXT) ;
						}
					} else {
						if (keywords && keywords.InList(key)) {
    						sc.ChangeState(SCE_METAPOST_COMMAND) ;
						} else if (keywords2 && keywords2.InList(key)) {
							sc.ChangeState(SCE_METAPOST_EXTRA) ;
						} else {
							sc.ChangeState(SCE_METAPOST_TEXT) ;
						}
					}
				}
			}
			if (isMETAPOSTcomment(sc.ch)) {
				if (! inTeX) {
					sc.SetState(SCE_METAPOST_SYMBOL) ;
					sc.ForwardSetState(SCE_METAPOST_DEFAULT) ;
					inComment = ! processComment ;
				} else {
					sc.SetState(SCE_METAPOST_TEXT) ;
				}
			} else if (isMETAPOSTstring(sc.ch)) {
				if (! inTeX) {
					sc.SetState(SCE_METAPOST_SPECIAL) ;
					if (! isMETAPOSTstring(sc.chNext)) {
						sc.ForwardSetState(SCE_METAPOST_TEXT) ;
					}
					inString = true ;
				} else {
					sc.SetState(SCE_METAPOST_TEXT) ;
				}
			} else if (isMETAPOSTcolon(sc.ch)) {
				if (! inTeX) {
					if (! isMETAPOSTequal(sc.chNext)) {
						sc.SetState(SCE_METAPOST_COMMAND) ;
						inClause = true ;
					} else {
						sc.SetState(SCE_METAPOST_SPECIAL) ;
					}
				} else {
					sc.SetState(SCE_METAPOST_TEXT) ;
				}
			} else if (isMETAPOSTone(sc.ch)) {
				if (! inTeX) {
					sc.SetState(SCE_METAPOST_SPECIAL) ;
				} else {
					sc.SetState(SCE_METAPOST_TEXT) ;
				}
			} else if (isMETAPOSTtwo(sc.ch)) {
				if (! inTeX) {
					sc.SetState(SCE_METAPOST_GROUP) ;
				} else {
					sc.SetState(SCE_METAPOST_TEXT) ;
				}
			} else if (isMETAPOSTthree(sc.ch)) {
				if (! inTeX) {
					sc.SetState(SCE_METAPOST_SYMBOL) ;
				} else {
					sc.SetState(SCE_METAPOST_TEXT) ;
				}
			} else if (isMETAPOSTidentifier(sc.ch)) {
				if (sc.state != SCE_METAPOST_COMMAND) {
					sc.SetState(SCE_METAPOST_TEXT) ;
					sc.ChangeState(SCE_METAPOST_COMMAND) ;
				}
			} else if (isMETAPOSTnumber(sc.ch)) {
				// rather redundant since for the moment we don't handle numbers
				sc.SetState(SCE_METAPOST_TEXT) ;
			} else if (sc.atLineEnd) {
				sc.SetState(SCE_METAPOST_TEXT) ;
				inTeX = false ;
				inComment = false ;
				inClause = false ;
				inString = false ;
			} else {
				sc.SetState(SCE_METAPOST_TEXT) ;
			}
		}

	}

	sc.Complete();

}

// Hooks info the system:

static const char * const metapostWordListDesc[] = {
	"MetaPost",
	"MetaFun",
	0
} ;

LexerModule lmMETAPOST(SCLEX_METAPOST, ColouriseMETAPOSTDoc, "metapost", 0, metapostWordListDesc);
