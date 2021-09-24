#ifndef READER_H
#define READER_H

#include "deps/cvector/cvector.h"
#include "token.h"
#include "node.h"

typedef struct {
  cvector_tokens_t cvector_tokens;
  cvector_iterator_tokens_t cvector_iterator_tokens_t;
} Reader;

Node READ(char *expr);
Node read_str(char *expr);
Node read_list(cvector_iterator_tokens_t *cvector_iterator);
Node read_form(cvector_iterator_tokens_t *cvector_iterator);
#endif
