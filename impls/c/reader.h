#ifndef READER_H
#define READER_H

#include "deps/cvector/cvector.h"
// create a type that defines Cvectorof char
CVector(char) cvector_char_t;
CVector_iterator(cvector_char_t) cvector_char_iterator_t;

typedef enum {
  COMMENT,
  SYMBOL,
  NUMBER,
  SEMI_COLON,
  OTHER,
  SPECIAL_TWO_CHARS__TILDA_AT,
  SPECIAL_SINGLE_CHAR__AT,
  SPECIAL_SINGLE_CHAR__LEFT_SQ_BR,
  SPECIAL_SINGLE_CHAR__RIGHT_SQ_BR,
  SPECIAL_SINGLE_CHAR__LEFT_CURLY_BR,
  SPECIAL_SINGLE_CHAR__RIGHT_CURLY_BR,
  SPECIAL_SINGLE_CHAR__LEFT_PAREN,
  SPECIAL_SINGLE_CHAR__RIGHT_PAREN,
  SPECIAL_SINGLE_CHAR__APOSTROPHE,
  SPECIAL_SINGLE_CHAR__WEIRD,
  SPECIAL_SINGLE_CHAR__TILDA,
  SPECIAL_SINGLE_CHAR__CARRAT,
} Token_type_e;

typedef struct {
  Token_type_e token_type;
  char *value;
  size_t value_length;
} Token_t;

Token_t token__new(Token_type_e token_type);

Token_t token__new_with_val(Token_type_e token_type, char *value,
                            size_t value_length);

char *token__stringify(Token_type_e token_type);

CVector(Token_t) cvector_token_t;

CVector_iterator(cvector_token_t) cvector_token_iterator_t;

typedef struct {

  cvector_token_t cvector_tokens;
  cvector_token_iterator_t cvector_tokens_iterator;
} Reader;

typedef struct {
  // don't know but will figure out soon
} mal_t;

mal_t read_str(char *expr);

#endif
