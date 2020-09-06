#include "Token.h"

#include <map>

namespace mumps {

namespace tok { 
static std::map<tok::TokenType, const char*> tokenNames = {
#define TOK(X) { X, #X },
#define PUNCTUATOR(X,Y) TOK(X)
#include "TokenType.def"
{ NUM_TOKENS, nullptr }
};
} // namespace tok

const char* Token::getTypeName() const
{
	return tok::tokenNames[type];
}
                                                           
} // namespace mumps
