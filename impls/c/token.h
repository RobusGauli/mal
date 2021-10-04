#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include "deps/cvector/cvector.h"

typedef enum {
  token_kind_number,
  token_kind_string,
  token_kind_symbol,
  token_kind_special_chr,
  token_kind_eol,
  token_kind_unreachable
} token_kind_e;

typedef struct {
  token_kind_e token_kind;
  char *buffer;
  size_t len;
} token_t;

CVector(token_t*) tokens_t;
CVector_iterator(tokens_t) tokens_iterator_t;


// constructor
tokens_t tokens(char* input);

// constructor for iterator
tokens_iterator_t tokens_iterator(tokens_t* tokens);


token_t* token_next(char** input);

const char* token_kind_name(token_t* token);

char* token_as_cstr(token_t* token);
#endif
