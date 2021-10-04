#ifndef MAL__H_
#define MAL__H_

#include <stdint.h>
#include "deps/cvector/cvector.h"
#include "token.h"

typedef enum mal_kind_e{
  mal_string,
  mal_number,
  mal_list,
  mal_symbol,
  mal_error,
} mal_kind_e;

typedef struct mal_t {
  mal_kind_e type;
  uint64_t value;
} mal_t;

// just another way to make sense
CVector(mal_t *) mals_t;
CVector_iterator(mals_t) mals_iterator_t;


mal_t *m_read(char *input);
mal_t* read_form(tokens_iterator_t* tokens_iterator);
mal_t* read_list(tokens_iterator_t* tokens_iterator);
mal_t* read_atom(tokens_iterator_t* tokens_iterator);

char *m_print(mal_t *mal);
#endif
