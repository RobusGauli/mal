#include "token.h"

Token token__new(TokenType tokentype) {
  return (Token){.tokentype = tokentype};
}

Token token__new_with_val(TokenType tokentype, StringView stringview) {
  return (Token){.tokentype = tokentype, .stringview = stringview};
}

StringView stringview__new(char *mem, size_t len) {
  return (StringView){.mem = mem, .len = len};
}
