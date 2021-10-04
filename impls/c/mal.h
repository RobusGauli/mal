#ifndef MAL__H_
#define MAL__H_

#include <stdint.h>
#include "deps/cvector/cvector.h"
#include "token.h"

typedef enum t_maltype{
  mal_string,
  mal_number,
  mal_list,
  mal_symbol,
  mal_error,
} t_maltype;

typedef struct Mal {
  t_maltype type;
  uint64_t value;
} Mal;

// just another way to make sense
CVector(Mal *) t_cvector_mal;
CVector_iterator(t_cvector_mal) t_cvector_mal_iterator;


Mal *m_read(char *input);
Mal* read_list(token_t* token);
Mal* read_atom(token_t* token);

char *m_print(Mal *mal);
#endif
