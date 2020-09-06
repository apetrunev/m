#ifndef MUMPS_TOKEN_TYPE_H_
#define MUMPS_TOKEN_TYPE_H_

namespace mumps {
namespace tok {

enum TokenType {
#define TOK(X) X,
#define PUNCTUATOR(X,Y) TOK(X)
#include "TokenType.def"
NUM_TOKENS
};
#undef PUNCTUATOR
#undef TOK
} // namespace tok
} // namespace mumps

#endif // MUMPS_TOKEN_TYPE_H_
