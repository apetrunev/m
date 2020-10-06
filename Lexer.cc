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
	char c = *curPtr;
	
	if (isIdentifierSymbol(c)) {	
		while (isIdentifierSymbol(c)) {
			c = *(curPtr + 1);
			curPtr++;
			len++;
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
	char c = *curPtr;

	if (isNumericSymbol(c)) {
		while (isNumericSymbol(c)) {
			c = *(curPtr + 1);
			curPtr++;
			len++;	
		}	
		// if after period immediatly come a digit
		if ((c == '.') && isNumericSymbol(*(curPtr + 1))) {
			c = *(curPtr + 1);
			// consume period
			len++;
			while (isNumericSymbol(c)) {
				c = *(curPtr + 1);
				curPtr++;
				len++;	
			}
		}
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
	char c = *curPtr;	

	if (c == ';') {
		while ((c != '\r') && (c != '\n')) {
			c = *(curPtr + 1);
			curPtr++;
			len++;
		}
		result.setType(tok::comment);
		result.setValue(getSubStr(pos, len));
		// return curPtr to \n symbol
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
	char c = *curPtr;
	
	if (c == '"') {
		curPtr++;
		pos++;
		do {
again:
			c = *curPtr++;
			// skip escaped double quote 
			if (c == '"' && *curPtr == '"') {
				curPtr++;
				len += 2;
				goto again;
			}
			len++;
		} while (c != '"');
		result.setType(tok::stringliteral);
		result.setValue(getSubStr(pos, len - 1));
		pos += len;
		col += len;
		cur = curPtr;
		return true;
	}
	return false;
}
 
void Lexer::skipSpaces()
{
	if ((*cur == '\r') && (*(cur + 1) == '\n')) { 
		cur += 2;
		pos += 2;
		line++;
		col = 0;
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
		c = *(cur + 1);
		if (c == '*') {
			nextChar();
			type = tok::starstar;
			break;
		}
		type = tok::star;
		break;
	case '\'':
		c = *(cur + 1);
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
		c = *(cur + 1);
		if (c == '=') {
			nextChar();
			type = tok::lessequal;
			break;
		}
		type = tok::less;
		break;
	case '>':
		c = *(cur + 1);
		if (c == '=') {
			nextChar();
			type = tok::greaterequal;
			break;
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
		c = *(cur + 1);
		if (c == '&') {
			nextChar();
			type = tok::ampamp;
			break;
		}
		type = tok::amp;
		break;
	case '|':
		c = *(cur + 1);
		if (c == '|') {
			nextChar();
			type = tok::pipepipe;
			break;
		}
		type = tok::pipe;
		break;
	case '#':
		type = tok::hash;
		break;
	case '$':
		c = *(cur + 1);
		if (c == '$') {
			nextChar();
			type = tok::dollardollar;
			break;
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
