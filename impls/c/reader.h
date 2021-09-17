#ifndef READER_H
#define READER_H

#include "deps/cvector/cvector.h"
// create a type that defines Cvectorof char
CVector(char) cvector_char_t;
CVector_iterator(cvector_char_t) cvector_char_iterator_t;

typedef enum {
  LEFT_PAREN,
  RIGHT_PAREN,
  COMMENT,
  LEFT_SQ_BRACKET,
  RIGHT_SQ_BRACKET,
  SYMBOL,
  NUMBER,
  LEFT_CURLY,
  RIGHT_CURLY,
  APOSTROPHE, // '
  TILDA, // ~
  CARRAT, // ^
  AT, // @
  DOUBLE_QUOTE,
  SEMI_COLON,
  OTHER,
} TOKEN_TYPE_E;


typedef struct {
  TOKEN_TYPE_E token_type;
  char* value;
  size_t value_length;
} Token_t;

CVector(Token_t) cvector_token_t;
CVector_iterator(cvector_token_t) cvector_token_iterator_t;

typedef struct {
  cvector_token_t cvector_tokens;
  cvector_token_iterator_t cvector_tokens_iterator;
} Reader;

typedef struct {
  // don't know but will figure out soon
} mal_t;

mal_t read_str(char* expr);

#endif
