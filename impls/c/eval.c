#include <assert.h>
#include <stdio.h>

#include "types.h"
#include "env.h"
#include "eval.h"
#include "mal.h"
#include "closure.h"

typedef mal_t *(*mal_func_t)(mal_t *);

bool is_let_form(mal_t *mal) {
  return mal->type == mal_symbol && strncmp((char *)mal->value, "let*", 4) == 0;
}
bool is_def_form(mal_t *mal) {
  return mal->type == mal_symbol && strncmp((char *)mal->value, "def!", 4) == 0;
}
bool is_func_form(mal_t* mal) {
  return mal -> type == mal_symbol && strncmp((char*) mal -> value, "fn*", 3) == 0;
}

bool is_mal_func(mal_t* mal) {
  return mal -> type == mal_func;
}

mal_t* eval_mal_func(mals_t* mals, Env* env) {
  printf("evaluating the function\n");
  fflush(stdout);
  return NULL;
}

mal_t* eval_def_form(mals_t* mals, Env* env) {

  // take second argument
  mal_t* key = cvector__index(mals, 1);
  assert(key -> type == mal_symbol);

  mal_t* value = cvector__index(mals, 2);
  // evaluated
  mal_t* evaluated_value = EVAL(value, env);

  // set the key/value in env
  env_set(env, key, evaluated_value);
  return evaluated_value;
}

mal_t* eval_let_form(mals_t* mals, Env* env) {
  // create a new environment
  // set key/value pair from the list
  Env* child_env = new_env(env);

  mal_t* list = cvector__index(mals, 1);
  assert(list -> type == mal_list);

  // assert we need to have even number of elements there
  mals_t* pairs = (mals_t*)list -> value;
  assert((cvector__size(pairs) & 1) == 0);

  // check to see
  mals_iterator_t iterator = mals_iterator(pairs);
  for(;;) {
    if (cvector_iterator__done(&iterator)) break;

    mal_t* key = cvector_iterator__next(&iterator);
    assert(key -> type == mal_symbol);
    mal_t* val = cvector_iterator__next(&iterator);
    mal_t* evaluated_val = EVAL(val, child_env);
    env_set(child_env, key, evaluated_val);
  }

  // we need to pass in new environment for evaluation of this value
  mal_t* value = cvector__index(mals, 2);
  mal_t* evaluated_result  = EVAL(value, child_env);
  // now delete the environment
  return evaluated_result;
}

mal_t* eval_func_form(mals_t* args, Env* env ) {
  // binds
  mal_t* binds = cvector__index(args, 1);
  assert(binds -> type == mal_list);
  // body
  mal_t* body = cvector__index(args, 2);
  // closure
  closure_t* closure = malloc(sizeof(closure_t));
  closure -> binds = binds;
  closure -> env = env;
  closure -> body = body;

  mal_t* result = new_mal();
  result -> type = mal_func;
  result -> value = (u64)(closure);
  return result;
}

mal_t *EVAL(mal_t *mal, Env *env) {
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

    if (is_let_form(first)) {
      return eval_let_form(mals, env);
    }

    if (is_def_form(first)) {
      return eval_def_form(mals, env);
    }

    if (is_func_form(first)) {
      return eval_func_form(mals, env);
    }

    /*assert(first->type == mal_symbol);*/
    mal_t *resolved = EVAL(first, env);

    if (mal_is_error(resolved)) {
      return resolved;
    }

    if (resolved -> type == mal_func) {
      // function closure
      closure_t* closure = (closure_t*)resolved -> value;

      Env* closed_env = closure -> env;

      mals_t *evaluated_mals = malloc(sizeof(mals_t *));
      mals_iterator_t mals_iterator;
      cvector_iterator__init(&mals_iterator, mals);

      // skip the first one
      cvector_iterator__next(&mals_iterator);

      for (;;) {
        if (cvector_iterator__done(&mals_iterator))
          break;
        mal_t *result = EVAL(cvector_iterator__next(&mals_iterator), env);
        if (mal_is_error(result)) {
          return result;
        }
        cvector__add(evaluated_mals, result);
      }

      mal_t* evaluated_exprs = new_mal();
      evaluated_exprs -> type = mal_list;
      evaluated_exprs -> value = (u64)(evaluated_mals);


      Env* new_env = new_env_with_binds(closed_env, closure -> binds, evaluated_exprs);
      return EVAL(closure -> body, new_env);
    }

    if (resolved -> type == mal_core_func) {
      mals_t *evaluated_mals = malloc(sizeof(mals_t *));
      mals_iterator_t mals_iterator;
      cvector_iterator__init(&mals_iterator, mals);

      cvector_iterator__next(&mals_iterator);

      for (;;) {
        if (cvector_iterator__done(&mals_iterator))
          break;
        mal_t *result = EVAL(cvector_iterator__next(&mals_iterator), env);
        if (mal_is_error(result)) {
          return result;
        }
        cvector__add(evaluated_mals, result);
      }

      mal_func_t func = (mal_func_t)(resolved->value);
      mal_t *argument = malloc(sizeof(mal_t));
      argument->type = mal_list;
      argument->value = (u64)evaluated_mals;

      return func(argument);
    }

    return resolved;
  }

  case mal_symbol: {
    mal_t *mal_func = env_get(env, mal);
    return mal_func;
  }

  case mal_error: {
    return mal;
  }

  default:
    assert(0 && "unreachable");
  }
}
