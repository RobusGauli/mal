#include <assert.h>
#include "mal.h"
#include "closure.h"
#include "types.h"


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
  case mal_core_func:
    return "mal_core_func";
  case mal_bool_true:
    return "mal_bool_true";
  case mal_bool_false:
      return "mal_bool_false";
  case mal_nil:
      return "mal_nil";
  default:
    assert(0 && "unreachable");
  }
}

bool mal_is_error(mal_t* mal) {
  return mal -> type == mal_error;
}

mal_t* new_mal() {
  mal_t* mal = malloc(sizeof(mal_t* ));
  return mal;
}

mal_t* new_mal_error(string_t* string) {
  mal_t* mal = new_mal();
  mal -> type = mal_error;
  mal -> value = (u64)string;
  return mal;
}

mal_t* new_mal_func(closure_t* closure) {
  mal_t* mal = new_mal();
  mal -> type = mal_func;
  mal -> value = (u64)closure;
  return mal;
}
mal_t* new_mal_false() {
  mal_t* mal = new_mal();
  mal -> type = mal_bool_false;
  return mal;
}

mal_t* new_mal_true()  {
  mal_t* mal = new_mal();
  mal -> type = mal_bool_true;
  return mal;
}
