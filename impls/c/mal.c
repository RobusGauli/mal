#include <assert.h>
#include "mal.h"


mals_iterator_t mals_iterator(mals_t *mals) {
  mals_iterator_t mals_iterator;
  cvector_iterator__init(&mals_iterator, mals);
  return mals_iterator;
}

char *mal_kind_name(mal_kind_e kind) {
  switch (kind) {
  case mal_string:
    return "mal_string";
  case mal_number:
    return "mal_number";
  case mal_list:
    return "mal_list";
  case mal_symbol:
    return "mal_symbol";
  case mal_error:
    return "mal_error";
  case mal_func:
    return "mal_func";
  default:
    assert(0 && "unreachable");
  }
}

bool mal_is_error(mal_t* mal) {
  return mal -> type == mal_error;
}
