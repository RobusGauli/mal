#ifndef TOKEN_H
#define TOKEN_H

#include "deps/cvector/cvector.h"

typedef enum {
  TOKEN__COMMENT,
  TOKEN__SYMBOL,
  TOKEN__INT,
  TOKEN__STRING,
  TOKEN__SEMICOLON,
  TOKEN__OTHER,
  TOKEN__TILDA_AT,
  TOKEN__AT,
  TOKEN__LEFT_SQ_BR,
  TOKEN__RIGHT_SQ_BR,
  TOKEN__LEFT_CURLY_BR,
  TOKEN__RIGHT_CURLY_BR,
  TOKEN__LEFT_PAREN,
  TOKEN__RIGHT_PAREN,
  TOKEN__APOSTROPHE,
  TOKEN__WEIRD,
  TOKEN__TILDA,
  TOKEN__CARRAT,
} TokenType;

typedef struct {
  char* mem;
  size_t len;
} StringView;

typedef struct {
  StringView stringview;
  TokenType tokentype;
} Token;


// constructor
Token token__new(TokenType tokentype);
Token token__new_with_val(TokenType tokentype, StringView stringview);

CVector(Token) cvector_tokens_t;
CVector_iterator(cvector_tokens_t) cvector_iterator_tokens_t;

// string view functions
StringView stringview__new(char* mem, size_t len);

#endif
