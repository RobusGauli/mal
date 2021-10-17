#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "mal.h"
#include "print.h"
#include "reader.h"
#include "str.h"

char *PRINT(mal_t *mal) {
  switch (mal->type) {
  case mal_string: {
    char *value = cstr_cpy(as_string(mal));
    return value;
  }

  case mal_number: {
    char *value = num_to_string(as_number(mal));
    return value;
  }

  case mal_list: {
    string_t string;
    cvector__init(&string);
    cvector__add(&string, '(');
    mals_t *mals = as_mals(mal);
    mals_iterator_t mals_iter = mals_iterator(mals);

    while (!cvector_iterator__done(&mals_iter)) {
      if (cvector_iterator__current_index_(&mals_iter) > 0) {
        cvector__add(&string, ' ');
      }
      mal_t *mal = cvector_iterator__next(&mals_iter);
      char *mal_printable_string = PRINT(mal);
      str_append_cstr(&string, mal_printable_string);
      free(mal_printable_string);
    }
    cvector__add(&string, ')');
    cvector__add(&string, '\0');
    return cvector__wrapped_buffer(&string);
  }

  case mal_symbol:
    return cstr_cpy(as_string(mal));

  case mal_error: {
    string_t *string = (string_t *)mal->value;
    return str_ascstr(string);
  }

  case mal_func: {
    return "#<function>";
  }

  default:
    fprintf(stderr, "[ERROR]: could not print the mal_type: %s\n",
            mal_kind_name(mal->type));
    assert(0 && "unreachable");
  }
}
