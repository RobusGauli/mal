#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "deps/cdict.h/cdict.h"
#include "env.h"
#include "mal.h"
#include "str.h"
#include "types.h"
#include "print.h"

mal_t *core_add(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);

  int a = cvector__index(args, 0)->value;
  int b = cvector__index(args, 1)->value;

  mal_t *result = malloc(sizeof(mal_t));
  result->type = mal_number;
  result->value = (u64)(a + b);
  return result;
}

mal_t *core_mul(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  int a = cvector__index(args, 0)->value;
  int b = cvector__index(args, 1)->value;
  mal_t *result = malloc(sizeof(mal_t));
  result->type = mal_number;
  result->value = (u64)(a * b);
  return result;
}

mal_t *core_sub(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  int a = cvector__index(args, 0)->value;
  int b = cvector__index(args, 1)->value;
  mal_t *result = malloc(sizeof(mal_t));
  result->type = mal_number;
  result->value = (u64)(a - b);
  return result;
}

mal_t *core_div(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  int a = cvector__index(args, 0)->value;
  int b = cvector__index(args, 1)->value;
  mal_t *result = malloc(sizeof(mal_t));
  result->type = mal_number;
  result->value = (u64)(a / b);
  return result;
}

mal_t *core_list(mal_t *arg) { return arg; }

// list?
mal_t *core_is_list(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);

  assert(cvector__size(args) == 1);
  mal_t *value = cvector__index(args, 0);

  mal_t *result = new_mal();

  if (value->type == mal_list) {
    result->type = mal_bool_true;
  } else {
    result->type = mal_bool_false;
  }
  return result;
}

mal_t *core_is_list_empty(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  assert(cvector__size(args) == 1);

  mal_t *value = cvector__index(args, 0);
  assert(value->type == mal_list);

  mals_t *list = (mals_t *)value->value;

  mal_t *result = new_mal();

  if (cvector__size(list)) {
    result->type = mal_bool_false;
    ;
  } else {
    result->type = mal_bool_true;
  }

  return result;
}

mal_t *core_list_count(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  assert(cvector__size(args) == 1);

  mal_t *value = cvector__index(args, 0);

  if (value->type == mal_nil) {
    mal_t *result = new_mal();
    result->type = mal_number;
    result->value = 0;
    return result;
  }

  assert(value->type == mal_list);

  mals_t *list = (mals_t *)(value->value);

  mal_t *result = new_mal();

  result->value = cvector__size(list);
  result->type = mal_number;
  return result;
}

mal_t *core_greater_than(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  mal_t *a_mal = cvector__index(args, 0);
  mal_t *b_mal = cvector__index(args, 1);
  assert(a_mal->type == mal_number);
  assert(b_mal->type == mal_number);
  mal_t *result = new_mal();

  if (a_mal->value > b_mal->value) {
    result->type = mal_bool_true;
    return result;
  }

  result->type = mal_bool_false;
  return result;
}

mal_t* core_less_than(mal_t* arg) {

  mals_t *args = (mals_t*)(arg -> value);
  assert(cvector__size(args) == 2);
  mal_t* a_mal  = cvector__index(args, 0);
  mal_t* b_mal = cvector__index(args, 1);

  assert(a_mal -> type == mal_number);
  assert(b_mal -> type == mal_number);

  if (a_mal -> value < b_mal -> value) {
    return new_mal_true();
  }
  return new_mal_false();
}
mal_t* core_less_than_equal(mal_t* arg){

  mals_t *args = (mals_t*)(arg -> value);
  assert(cvector__size(args) == 2);
  mal_t* a_mal  = cvector__index(args, 0);
  mal_t* b_mal = cvector__index(args, 1);

  assert(a_mal -> type == mal_number);
  assert(b_mal -> type == mal_number);

  if (a_mal -> value <= b_mal -> value) {
    return new_mal_true();
  }
  return new_mal_false();
}

mal_t* core_greater_than_equal(mal_t* arg){

  mals_t *args = (mals_t*)(arg -> value);
  assert(cvector__size(args) == 2);
  mal_t* a_mal  = cvector__index(args, 0);
  mal_t* b_mal = cvector__index(args, 1);

  assert(a_mal -> type == mal_number);
  assert(b_mal -> type == mal_number);

  if (a_mal -> value >= b_mal -> value) {
    return new_mal_true();
  }
  return new_mal_false();
}

mal_t *_core_is_equal(mal_t *first, mal_t *second) {
  switch (first->type) {
  case mal_bool_false: {
    return new_mal_true();
  }

  case mal_bool_true: {
    return new_mal_true();
  }

  case mal_number: {
    u64 first_value = first->value;
    u64 second_value = second->value;
    if (first_value == second_value) {
      return new_mal_true();
    }
    return new_mal_false();
  }

  case mal_list: {
    mals_t *first_list = (mals_t *)(first->value);
    mals_t *second_list = (mals_t *)(second->value);
    if (cvector__size(first_list) != cvector__size(second_list)) {
      return new_mal_false();
    }
    // check for the individual element in the list
    mals_iterator_t first_list_iterator = mals_iterator(first_list);
    mals_iterator_t second_list_iterator = mals_iterator(second_list);

    for (;;) {
      if (cvector_iterator__done(&first_list_iterator) ||
          cvector_iterator__done(&second_list_iterator)) {
        break;
      }

      mal_t *result =
          _core_is_equal(cvector_iterator__next(&first_list_iterator),
                         cvector_iterator__next(&second_list_iterator));
      if (result->type == mal_bool_false) {
        return result;
      }
    }
    return new_mal_true();
  }

  case mal_nil: {
    return new_mal_true();
  }

  case mal_func: {
    if (first->value == second->value) {
      return new_mal_true();
    }
    return new_mal_false();
  }
  case mal_core_func: {
    if (first->value == second->value) {
      return new_mal_true();
    }
    return new_mal_false();
  }

  case mal_string: {
    string_t *first_string = (string_t *)(first->value);
    string_t *second_string = (string_t *)(second->value);
    if (str_is_equal(first_string, second_string)) {
      return new_mal_true();
    }
    return new_mal_false();
  }

  default:
    assert(0 && "unreachable");
  }
}

mal_t *core_is_equal(mal_t *arg) {
  mals_t *args = (mals_t *)(arg->value);
  // assertion
  assert(cvector__size(args) == 2);
  // get the first
  mal_t *first = cvector__index(args, 0);
  mal_t *second = cvector__index(args, 1);

  // nightly basis
  mal_kind_e first_mal_kind = first->type;
  mal_kind_e second_mal_kind = second->type;

  if (first_mal_kind != second_mal_kind) {
    return new_mal_false();
  }

  // equal check
  return _core_is_equal(first, second);
}

mal_t* core_pr_str(mal_t* arg) {
  string_t* result = new_str();

  mal_t* mal_result = new_mal();
  mals_t* args = (mals_t*) (arg -> value);
  mals_iterator_t iterator = mals_iterator(args);

  for(;;) {
    if (cvector_iterator__done(&iterator)) break;
    mal_t* next = cvector_iterator__next(&iterator);
    char* next_result = pr_str(next, true);
    str_append_cstr(result, next_result);
    if (!cvector_iterator__done(&iterator)) {
      str_append_space(result);
    }
    free(next_result);
  }

  mal_result -> type = mal_string;
  mal_result -> value = (u64)(result);
  return mal_result;
}

mal_t* core_str(mal_t* arg) {
  string_t* result = new_str();

  mal_t* mal_result = new_mal();
  mals_t* args = (mals_t*) (arg -> value);
  mals_iterator_t iterator = mals_iterator(args);

  for(;;) {
    if (cvector_iterator__done(&iterator)) break;

    mal_t* next = cvector_iterator__next(&iterator);

    char* next_result = pr_str(next, false);
    str_append_cstr(result, next_result);
    free(next_result);
  }

  mal_result -> type = mal_string;
  mal_result -> value = (u64)(result);
  return mal_result;

}

mal_t* core_prn(mal_t* arg) {
  string_t* result = new_str();

  mal_t* mal_result = new_mal();
  mals_t* args = (mals_t*)(arg -> value);
  mals_iterator_t iterator = mals_iterator(args);

  for(;;) {
    if (cvector_iterator__done(&iterator)) break;

    mal_t* next = cvector_iterator__next(&iterator);

    char* next_result = pr_str(next, true);
    str_append_cstr(result, next_result);

    if (!cvector_iterator__done(&iterator)) {
      str_append_space(result);
    }

    free(next_result);
  }
  char* cstr_result = str_ascstr(result);
  printf("%s\n", cstr_result);
  str_free(result);
  return new_mal_nil();
}

mal_t* core_println(mal_t* arg) {
  string_t* result = new_str();

  mal_t* mal_result = new_mal();
  mals_t* args = (mals_t*)(arg -> value);
  mals_iterator_t iterator = mals_iterator(args);

  for(;;) {
    if (cvector_iterator__done(&iterator)) break;

    mal_t* next = cvector_iterator__next(&iterator);

    char* next_result = pr_str(next, false);
    str_append_cstr(result, next_result);

    if (!cvector_iterator__done(&iterator)) {
      str_append_space(result);
    }

    free(next_result);
  }
  char* cstr_result = str_ascstr(result);
  printf("%s\n", cstr_result);
  str_free(result);
  return new_mal_nil();
}

bool env_comp(mal_t **self, mal_t **other) {
  assert((*self)->type == mal_symbol);

  mal_t *m_self = *self;
  mal_t *m_other = *other;
  char *self_string = (char *)((m_self)->value);
  char *other_string = (char *)((m_other)->value);

  return strcmp(self_string, other_string) == 0;
}

cdict__u64 env_hash(mal_t **self, cdict__u64 (*hash)(void *, size_t)) {
  mal_t *m_self = *self;
  char *string = (char *)((m_self)->value);
  return hash(string, strlen(string));
}

Env *new_env(Env *prev) {
  Env *env = malloc(sizeof(Env));
  env->prev = prev;
  cdict__init(&(env->data));
  cdict__set_comparator(&(env->data), env_comp);
  cdict__set_hash(&(env->data), env_hash);

  return env;
}

mals_t* _core_create_mals_from_iter(mals_iterator_t* iterator) {
  mals_t* mals = malloc(sizeof(mals_t));
  cvector__init(mals);
  for(;;) {
    if (cvector_iterator__done(iterator)) {
      break;
    }
    cvector__add(mals, cvector_iterator__next(iterator));
  }
  return mals;
}

Env *new_env_with_binds(Env *prev, mal_t *binds, mal_t *exprs) {
  Env *env = new_env(prev);
  assert(binds->type == mal_list);
  assert(exprs->type == mal_list);
  mals_t *binds_list = (mals_t *)(binds->value);
  mals_t *exprs_list = (mals_t *)(exprs->value);


  mals_iterator_t binds_iter = mals_iterator(binds_list);
  mals_iterator_t exprs_iter = mals_iterator(exprs_list);

  for (;;) {
    if (cvector_iterator__done(&binds_iter))
      break;

    mal_t *key = cvector_iterator__next(&binds_iter);
    // check the symbol
    assert(key -> type == mal_symbol);
    char* key_cstr = (char*)key -> value;
    if (strncmp(key_cstr, "&", 1) == 0) {
      if (!cvector_iterator__done(&binds_iter)) {
        mal_t* to_bound_key = cvector_iterator__next(&binds_iter);
        mals_t* to_bound_values = _core_create_mals_from_iter(&exprs_iter);
        mal_t* to_bound_value = new_mal();
        to_bound_value -> type = mal_list;
        to_bound_value -> value = (u64)(to_bound_values);
        env_set(env, to_bound_key, to_bound_value);
      }
      break;
    } else {
      mal_t* value = cvector_iterator__next(&exprs_iter);
      env_set(env, key, value);
    }
  }
  return env;
}

Env *env_find(Env *env, mal_t *symbol) {

  if (env == NULL) {
    return NULL;
  }

  bool ok = cdict__contains(&(env->data), symbol);

  if (ok) {
    return env;
  }
  return env_find(env->prev, symbol);
}

mal_t *env_get(Env *env, mal_t *symbol) {
  Env *found = env_find(env, symbol);

  if (found == NULL) {
    string_t *result = new_str_from_cstr("could not resolve symbol: '");
    str_append_cstr(result, (char *)symbol->value);
    str_append_char(result, '\'');
    mal_t *err = new_mal_error(result);
    return err;
  }

  mal_t *buffer = NULL;
  bool ok = cdict__get(&(found->data), symbol, &buffer);
  return buffer;
}

void env_set(Env *env, mal_t *symbol, mal_t *value) {
  cdict__add(&(env->data), symbol, value);
}
