#ifndef MUMPS_LEXER_H_
#define MUMPS_LEXER_H_

#include "Token.h"
#include "Routine.h"

#include <string>

namespace mumps {

class Lexer {
public:
	Lexer(Routine &m): mSource(m), line(0), col(0), pos(0), start(m.source.begin()), 
		cur(start), end(m.source.end()) {}
	Lexer(Lexer &) = delete;
	Lexer& operator=(Lexer& ) = delete;
	bool lexToken(Token &result);
private:
	bool lexIdentifierInternal(Token &result, std::string::iterator curPtr, tok::TokenType type);
	bool lexLabel(Token &result, std::string::iterator curPtr);
	bool lexIdentifier(Token &result, std::string::iterator curPtr, tok::TokenType type);
	bool lexNumber(Token &result, std::string::iterator curPtr);
	bool lexEof(Token &result) { result.setType(tok::eof); return true; }
	bool lexComment(Token &result, std::string::iterator curPtr);
	bool lexStringLiteral(Token &result, std::string::iterator curPtr);
	bool isAtLineStart() { return col == 0; }
	bool isEof() { return cur == end; }
	std::string getSubStr(unsigned start, unsigned end) { return mSource.source.substr(start, end); }
	bool isIdentifierSymbol(char c);
	bool isNumericSymbol(char c);
	void skipSpaces();
	void nextChar() { pos++; cur++; col++; }
private:
	Routine &mSource;
	// number of lines of code
	unsigned line;
	// character position at the line 
	unsigned col;
	// character position in the string
	unsigned pos; 
	std::string::iterator start;
	std::string::iterator cur;
	std::string::iterator end;
};

} // mumps

#endif // MUMPS_LEXER_H_
