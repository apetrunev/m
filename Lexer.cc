#include "Lexer.h" // Token
#include "Routine.h"

#include <string>
#include <iostream>
#include <cctype>

using namespace mumps;

bool Lexer::isIdentifierSymbol(char c)
{
	if (std::isalnum(c) || c == '%') return true;
	return false;
}

bool Lexer::isNumericSymbol(char c)
{	
	if (std::isdigit(c)) return true;
	return false;
}

bool Lexer::lexIdentifier(Token &result, std::string::iterator curPtr, tok::TokenType type)
{
	unsigned len = 0;
	
	if (isEof(curPtr)) return false;
		
	char c = *curPtr;
	
	if (isIdentifierSymbol(c)) {	
		while (isIdentifierSymbol(c)) {
			len++;
			curPtr++;
			if (isEof(curPtr)) break;
			c = *curPtr;
		}	
		result.setType(type);
		result.setValue(getSubStr(pos, len));
		// return iterator one symbol back
		pos += len;
		col += len;
		cur = curPtr;
		return true;
	}
	return false;
}
    
bool Lexer::lexNumber(Token &result, std::string::iterator curPtr)
{
	unsigned len = 0;
	
	if (isEof(curPtr)) return false;
	
	char c = *curPtr;

	if (isNumericSymbol(c)) {
		while (isNumericSymbol(c)) {
			len++;
			curPtr++;	
			if (isEof(curPtr)) goto do_number;
			c = *curPtr;
		}
		if (c == '.') {
			curPtr++;
			if (isEof(curPtr)) {
				// return period to lexer
				curPtr--;
				goto do_number;
			}
			// floating point number
			if (isNumericSymbol(*curPtr)) {
				// consume period
				len++;
				c = *curPtr;
				while (isNumericSymbol(c)) {
					len++;
					curPtr++;	
					if (isEof(curPtr)) break;
					c = *curPtr;
				}
			} else {
				// return period to lexer
				curPtr--;
			}
		}	
do_number:
		result.setType(tok::number);
		result.setValue(getSubStr(pos, len));
		pos += len;
		col += len;
		cur = curPtr;
		return true;
	}
	return false;
}
 
bool Lexer::lexComment(Token &result, std::string::iterator curPtr)
{
	unsigned len = 0;

	if (isEof(curPtr)) return false;

	char c = *curPtr;	

	if (c == ';') {
		while ((c != '\r') && (c != '\n')) {
			len++;
			curPtr++;
			if (isEof(curPtr)) break;
			c = *curPtr;
		}
		result.setType(tok::comment);
		result.setValue(getSubStr(pos, len));
		pos += len;
		col += len;
		cur = curPtr;
		return true;
	}
	return false;	
}
                
bool Lexer::lexStringLiteral(Token &result, std::string::iterator curPtr)
{
	unsigned len = 0;
		
	if (isEof(curPtr)) return false;

	char c = *curPtr;

	if (c == '"') {
		len++;
		curPtr++;
		do {
again:
			if (isEof(curPtr)) goto do_string;
			c = *curPtr;	
			if (c == '"') {
				// consume double qoute
				len++;
				curPtr++;
				if (isEof(curPtr)) goto do_string;
				char peek = *curPtr;
				// escaped double quote `""'
				if (peek == '"') {
					// consume escaped double quote
					len++;
					curPtr++;
					goto again;
				}
			} else {
				len++;
				curPtr++;
			}
		} while (c != '"');
do_string:
		result.setType(tok::stringliteral);
		result.setValue(getSubStr(pos, len));
		pos += len;
		col += len;
		cur = curPtr;
		return true;
	}
	return false;
}
 
void Lexer::skipSpaces()
{
	if (isEof(cur)) return;

	if (*cur == '\r') {
		cur++;
		pos++;
		if (isEof(cur)) return;
		if (*cur == '\n') {
			cur++;
			pos++;
			line++;
			col = 0;
		}
	} else if (*cur == '\n') {
		cur++;
		pos++;
		line++;
		col = 0;
	}
}

bool Lexer::lexToken(Token &result)
{
	tok::TokenType type;
scan_again:
	result.clear();
	skipSpaces();

	if (isEof())
		return lexEof(result);

	char c = *cur;
	// if first symbol is not a space then we are processing label
	if (isAtLineStart()) {
		if (isIdentifierSymbol(c)) 
			return lexIdentifier(result, cur, tok::label);
	} else {
		if (isNumericSymbol(c)) {
			return lexNumber(result, cur);	
		} else if (isIdentifierSymbol(c)) {
			return lexIdentifier(result, cur, tok::identifier);
		}		
	}

	switch (c) {
	case '+':
		type = tok::plus;
		break;
	case '-':
		type = tok::minus;
		break;
	case '*':
		if (peekChar(cur + 1, c)) {
			if (c == '*') {
				nextChar();
				type = tok::starstar;
				break;
			}
		}
		type = tok::star;
		break;
	case '\'':
		if (peekChar(cur + 1, c)) {
			if (c == '<') {
				nextChar();
				type = tok::s_quoteless;
				break;
			} else if (c == '=') {
				nextChar();
				type = tok::s_quoteequal;
				break;
			} else if (c == '>') {
				nextChar();
				type = tok::s_quotegreater;
				break;
			} else if (c == ']') {
				nextChar();
				type = tok::s_quoter_square;
				break;
			}
		}	
		type = tok::s_quote;
		break;
	case '"':
		return lexStringLiteral(result, cur);
	case '!':
		type = tok::exclam;
		break;
	case '[':
		type = tok::l_square;
		break;
	case ']':
		type = tok::r_square;
		break;
	case '(':
		type = tok::l_parenth;
		break;
	case ')':
		type = tok::r_parenth;
		break;
	case '<':	
		if (peekChar(cur + 1, c)) {
			if (c == '=') {
				nextChar();
				type = tok::lessequal;
				break;
			}
		}
		type = tok::less;
		break;
	case '>':
		if (peekChar(cur + 1, c)) {
			if (c == '=') {
				nextChar();
				type = tok::greaterequal;
				break;
			}
		}
		type = tok::greater;
		break;
	case ';':
		return lexComment(result, cur);
	case ':':
		type = tok::colon;
		break;
	case '.':
		type = tok::period;
		break;
	case ',':
		type = tok::comma;
		break;
	case '@':
		type = tok::at;
		break;
	case '&':
		if (peekChar(cur + 1, c)) {
			if (c == '&') {
				nextChar();
				type = tok::ampamp;
				break;
			}
		}
		type = tok::amp;
		break;
	case '|':
		if (peekChar(cur + 1, c)) {
			if (c == '|') {
				nextChar();
				type = tok::pipepipe;
				break;
			}
		}
		type = tok::pipe;
		break;
	case '#':
		type = tok::hash;
		break;
	case '$':
		if (peekChar(cur + 1, c)) {
			if (c == '$') {
				nextChar();
				type = tok::dollardollar;
				break;
			}
		}
		type = tok::dollar;
		break;
	case '~':
		type = tok::tilde;
		break;
	case '/':
		type = tok::slash;
		break;
	case '\\':
		type = tok::backslash;
		break;
	case '_':
		type = tok::underscore;
		break;
	case '^':
		type = tok::caret;
		break;
	case '=':
		type = tok::equal;
		break;
	case ' ':
		type = tok::space;
		break;
	case '?':
		type = tok::question;
		break;
	case '\t':
		type = tok::tab;
		break;
	case '\r':
	case '\n':
		goto scan_again;
	default:
		nextChar();
		result.setType(tok::undefined);	
		return false;
	}
	nextChar();
	result.setType(type);
	return true;
}
