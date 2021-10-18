#ifndef MAL__H_
#define MAL__H_

#include <stdint.h>

#include "deps/cvector/cvector.h"
#include "token.h"
#include "str.h"
#include "types.h"

typedef enum mal_kind_e{
  mal_string,
  mal_number,
  mal_list,
  mal_symbol,
  mal_error,
  mal_func,
  mal_core_func,
  mal_bool_true,
  mal_bool_false,
  mal_nil,
} mal_kind_e;

typedef struct mal_t {
  mal_kind_e type;
  u64 value;
} mal_t;

// vectors
CVector(mal_t *) mals_t;
CVector_iterator(mals_t) mals_iterator_t;

// Iterator helpers
mals_iterator_t mals_iterator(mals_t* mals);

// check weather the mal type is error
bool mal_is_error(mal_t* mal);

// Helper to debug
char* mal_kind_name(mal_kind_e kind);

// constructors
mal_t* new_mal();
mal_t* new_mal_error(string_t* string);
#endif
