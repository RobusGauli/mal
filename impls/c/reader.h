#ifndef READER_H
#define READER_H

#include "token.h"
#include "mal.h"

// Entry
mal_t* READ(char* input);

// Readers
mal_t *read_atom(tokens_iterator_t *tokens_iterator);
mal_t* read_form(tokens_iterator_t *tokens_iterator);
mal_t* read_list(tokens_iterator_t *tokens_iterator);

// Helpers
mals_t *as_mals(mal_t *mal);
int as_number(mal_t *mal);
char *as_string(mal_t *mal);
char *as_symbol(mal_t *mal);

#endif
