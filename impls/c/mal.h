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

// vectors
CVector(mal_t *) mals_t;
CVector_iterator(mals_t) mals_iterator_t;

// Iterator helpers
mals_iterator_t mals_iterator(mals_t* mals);

#endif
