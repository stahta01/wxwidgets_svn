// Scintilla source code edit control
/** @file LexLua.cxx
 ** Lexer for Lua language.
 **
 ** Written by Paul Winwood.
 ** Folder by Alexey Yutkin.
 ** Modified by Marcos E. Wurzius
 **/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"


static inline bool IsAWordChar(const int ch) {
	return (ch < 0x80) && (isalnum(ch) || ch == '.' || ch == '_');
}

inline bool IsAWordStart(const int ch) {
	return (ch < 0x80) && (isalnum(ch) || ch == '_');
}


inline bool isLuaOperator(char ch) {
	if (isalnum(ch))
		return false;
	// '.' left out as it is used to make up numbers
	if (ch == '*' || ch == '/' || ch == '-' || ch == '+' ||
		ch == '(' || ch == ')' || ch == '=' ||
		ch == '{' || ch == '}' || ch == '~' ||
		ch == '[' || ch == ']' || ch == ';' ||
		ch == '<' || ch == '>' || ch == ',' ||
		ch == '.' || ch == '^' || ch == '%' || ch == ':')
		return true;
	return false;
}


static void ColouriseLuaDoc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[],
                            Accessor &styler) {

	WordList &keywords = *keywordlists[0];
	WordList &keywords2 = *keywordlists[1];
	WordList &keywords3 = *keywordlists[2];
	WordList &keywords4 = *keywordlists[3];
	WordList &keywords5 = *keywordlists[4];
	WordList &keywords6 = *keywordlists[5];
	int literalString = 0;
	int literalStringFlag =0;

	// Do not leak onto next line
	if (initStyle == SCE_LUA_STRINGEOL)
		initStyle = SCE_LUA_DEFAULT;

	StyleContext sc(startPos, length, initStyle, styler);
	if(startPos == 0 && sc.ch == '#') sc.SetState(SCE_LUA_COMMENTLINE);
	for (; sc.More(); sc.Forward()) {
	
		// Handle line continuation generically.
		if (sc.ch == '\\') {
			if (sc.Match("\\\n")) {
				sc.Forward();
				sc.Forward();
				continue;
			}
			if (sc.Match("\\\r\n")) {
				sc.Forward();
				sc.Forward();
				sc.Forward();
				continue;
			}
		}

		// Determine if the current state should terminate.
		if (sc.state == SCE_LUA_OPERATOR) {
			sc.SetState(SCE_LUA_DEFAULT);
		} else if (sc.state == SCE_LUA_NUMBER) {
			if (!IsAWordChar(sc.ch)) {
				sc.SetState(SCE_LUA_DEFAULT);
			}
		} else if (sc.state == SCE_LUA_IDENTIFIER) {
			if (!IsAWordChar(sc.ch) || (sc.ch == '.')) {
				char s[100];
				sc.GetCurrent(s, sizeof(s));
				if (keywords.InList(s)) {
					sc.ChangeState(SCE_LUA_WORD);
				} else if (keywords2.InList(s)) {
					sc.ChangeState(SCE_LUA_WORD2);
				} else if (keywords3.InList(s)) {
					sc.ChangeState(SCE_LUA_WORD3);
				} else if (keywords4.InList(s)) {
					sc.ChangeState(SCE_LUA_WORD4);
				} else if (keywords5.InList(s)) {
					sc.ChangeState(SCE_LUA_WORD5);
				} else if (keywords6.InList(s)) {
					sc.ChangeState(SCE_LUA_WORD6);
				}
				sc.SetState(SCE_LUA_DEFAULT);
			}
 

		} else if (sc.state == SCE_LUA_COMMENTLINE ) {
			if (sc.atLineEnd) {
				sc.SetState(SCE_LUA_DEFAULT);
			}
		} else if (sc.state == SCE_LUA_STRING) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(SCE_LUA_DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(SCE_LUA_STRINGEOL);
				sc.ForwardSetState(SCE_LUA_DEFAULT);
			}
			
		} else if (sc.state == SCE_LUA_CHARACTER) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(SCE_LUA_DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(SCE_LUA_STRINGEOL);
				sc.ForwardSetState(SCE_LUA_DEFAULT);
			}
		} else if (sc.state == SCE_LUA_LITERALSTRING) {
			if (sc.chPrev == '[' && sc.ch == '[' && literalStringFlag != 1) 	{
				literalString++;
				literalStringFlag = 1;
			}
			else if (sc.chPrev == ']' && sc.ch == ']' && literalStringFlag != 2 ) {
				if((--literalString == 1))
					sc.ForwardSetState(SCE_LUA_DEFAULT);
				literalStringFlag = 2;
			}
			else literalStringFlag = 0;
		}		
		// Determine if a new state should be entered.
		if (sc.state == SCE_LUA_DEFAULT) {
			if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
					sc.SetState(SCE_LUA_NUMBER);
			} else if (IsAWordStart(sc.ch) || (sc.ch == '@')) {
					sc.SetState(SCE_LUA_IDENTIFIER);
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_LUA_STRING);
			} else if (sc.ch == '\'') {
				sc.SetState(SCE_LUA_CHARACTER);
			} else if (sc.ch == '[' && sc.chNext == '[') {
				sc.SetState(SCE_LUA_LITERALSTRING);
				literalString = 1;
			} else if (sc.ch == '-' && sc.chNext == '-') {
				sc.SetState(SCE_LUA_COMMENTLINE);
			}  else if (isLuaOperator(static_cast<char>(sc.ch))) {
				sc.SetState(SCE_LUA_OPERATOR);
			}
		}
	}
	sc.Complete();
}


static void FoldLuaDoc(unsigned int startPos, int length, int /* initStyle */, WordList *[],
                       Accessor &styler) {
	unsigned int lengthDoc = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int levelCurrent = levelPrev;
	char chNext = styler[startPos];
	bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;
	int styleNext = styler.StyleAt(startPos);
	char s[10];
	for (unsigned int i = startPos; i < lengthDoc; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		int style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (style == SCE_LUA_WORD) {
			if ( ch == 'i' || ch == 'e' || ch == 't' || ch == 'd' || ch == 'f') {
				for (unsigned int j = 0; j < 8; j++) {
					if (!iswordchar(styler[i + j]))
						break;
					s[j] = styler[i + j];
					s[j + 1] = '\0';
				}
				
				if ((strcmp(s, "if") == 0) || (strcmp(s, "do") == 0)
					|| (strcmp(s, "function") == 0))
					levelCurrent++;
				if ((strcmp(s, "end") == 0) || (strcmp(s, "elseif") == 0))
					levelCurrent--;
				
			}
		}
		else if (style == SCE_LUA_OPERATOR)
		{
			if(ch == '{' || ch == '(')
				levelCurrent++;
			else if(ch == '}' || ch == ')')
				levelCurrent--;
		}
			
		if (atEOL) {
			int lev = levelPrev;
			if (visibleChars == 0 && foldCompact)
				lev |= SC_FOLDLEVELWHITEFLAG;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
		if (!isspacechar(ch))
			visibleChars++;
	}
	// Fill in the real level of the next line, keeping the current flags as they will be filled in later

	int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel(lineCurrent, levelPrev | flagsNext);
}

LexerModule lmLua(SCLEX_LUA, ColouriseLuaDoc, "lua", FoldLuaDoc);
