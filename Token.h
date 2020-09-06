#ifndef MUMPS_TOKEN_H_
#define MUMPS_TOKEN_H_

#include "TokenType.h"

#include <string>

namespace mumps {

class Token final {
private:
	tok::TokenType type;
	std::string value;
public:
	Token(): type(tok::undefined), value(std::string()) {}
	~Token() {}
	Token(Token &) = default;
	Token& operator=(Token &) = default;
	tok::TokenType getType() const { return type; }
	void setType(tok::TokenType t) { type = t; }
	std::string getValue() const { return value; }
	void setValue(std::string v) { value = v; }
	bool is(tok::TokenType t) { return type == t; }
	bool isNot(tok::TokenType t) { return type != t; }	
	void clear() { type = tok::undefined; value.clear(); }
	const char* getTypeName() const;
	std::string getValue() { return value; }
};

} // namespace mumps
#endif // MUMPS_TOKEN_H
