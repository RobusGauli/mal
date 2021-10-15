#include <assert.h>
#include <stdio.h>

#include "env.h"
#include "eval.h"
#include "mal.h"

typedef mal_t *(*mal_func_t)(mal_t *);

mal_t *EVAL(mal_t *mal, Env env) {
  switch (mal->type) {
  case mal_string: {
    return mal;
  }

  case mal_number: {
    return mal;
  }
  case mal_list: {
    mals_t *mals = (mals_t *)mal->value;
    if (!cvector__size(mals)) {
      return mal;
    }

    mal_t *first = cvector__index(mals, 0);
    assert(first->type == mal_symbol);
    mal_t *resolved = EVAL(first, env);

    if (mal_is_error(resolved)) {
      return resolved;
    }

    // check the list of arguments
    mals_t* evaluated_mals = malloc(sizeof(mal_t*));
    mals_iterator_t mals_iterator;
    cvector_iterator__init(&mals_iterator, mals);

    /*// skip the first element*/
    cvector_iterator__next(&mals_iterator);

    for(;;) {
      if (cvector_iterator__done(&mals_iterator)) break;
      mal_t* result = EVAL(cvector_iterator__next(&mals_iterator), env);
      if (mal_is_error(result)) {
        return result;
      }
      cvector__add(evaluated_mals, result);
    }

    mal_func_t func = (mal_func_t)(resolved -> value);
    mal_t* argument =  malloc(sizeof(mal_t));
    argument -> type = mal_func;
    argument -> value = (uint64_t)evaluated_mals;
    return func(argument);
  }

  case mal_symbol: {
    mal_t *mal_func = env_get(env, mal);
    if (mal_func == NULL) {
      mal_t *error = malloc(sizeof(mal_t));
      error->type = mal_error;
      error->value = (uint64_t)("could not resolve the symbol");
      return error;
    }

    return mal_func;
  }

  case mal_error: {
    return mal;
  }

  default:
    assert(0 && "unreachable");
  }
}
