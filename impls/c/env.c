#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "deps/cdict.h/cdict.h"
#include "env.h"
#include "str.h"
#include "mal.h"
#include "types.h"

mal_t* core_add(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);

  int a = cvector__index(args, 0) -> value;
  int b = cvector__index(args, 1) -> value;

  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (u64)(a+b);
  return result;
}

mal_t* core_mul(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);
  int a = cvector__index(args, 0) -> value;
  int b  = cvector__index(args, 1) -> value;
  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (u64)(a*b);
  return result;
}

mal_t* core_sub(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);
  int a = cvector__index(args, 0) -> value;
  int b  = cvector__index(args, 1) -> value;
  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (u64)(a-b);
  return result;
}

mal_t* core_div(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);
  int a = cvector__index(args, 0) -> value;
  int b  = cvector__index(args, 1) -> value;
  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (u64)(a/b);
  return result;
}

bool env_comp(mal_t** self, mal_t** other) {
  // just compariing
  assert((*self) -> type == mal_symbol);

  mal_t* m_self = *self;
  mal_t* m_other = *other;
  char* self_string = (char*)((m_self) -> value);
  char* other_string = (char*)((m_other) -> value);

  return strcmp(self_string, other_string) == 0;
}

cdict__u64 env_hash(mal_t** self, cdict__u64 (*hash) (void*, size_t)) {
  mal_t* m_self = *self;
  char* string = (char*)((m_self) -> value);
  return hash(string, strlen(string));
}


Env* new_env(Env* prev) {
  Env* env = malloc(sizeof(Env));
  env -> prev = prev;
  cdict__init(&(env -> data));
  cdict__set_comparator(&(env->data), env_comp);
  cdict__set_hash(&(env -> data), env_hash);

  return env;
}

Env* new_env_with_binds(Env* prev, mal_t* binds, mal_t* exprs) {
  Env* env = new_env(prev);
  assert(binds -> type == mal_list);
  assert(exprs -> type == mal_list);
  mals_t* binds_list = (mals_t*)(binds -> value);
  mals_t* exprs_list = (mals_t*)(exprs -> value);

  assert(cvector__size(binds_list) == cvector__size(exprs_list));

  mals_iterator_t binds_iter = mals_iterator(binds_list);
  mals_iterator_t exprs_iter = mals_iterator(exprs_list);

  for (;;) {
    if (cvector_iterator__done(&binds_iter)) break;

    mal_t* key = cvector_iterator__next(&binds_iter);
    mal_t* value = cvector_iterator__next(&exprs_iter);
    env_set(env, key, value);
  }

  return env;
}

Env* env_find(Env* env, mal_t* symbol) {

  if (env == NULL) {
    return NULL;
  }

  bool ok = cdict__contains(&(env -> data), symbol);

  if (ok) {
    return env;
  }
  return env_find(env -> prev, symbol);
}

mal_t* env_get(Env* env, mal_t* symbol) {
  Env* found = env_find(env, symbol);

  if (found == NULL) {
    string_t* result = new_str_from_cstr("could not resolve symbol: '");
    str_append_cstr(result, (char*)symbol -> value);
    str_append_char(result, '\'');
    mal_t* err = new_mal_error(result);
    return err;
  }

  mal_t* buffer = NULL;
  bool ok = cdict__get(&(found -> data), symbol, &buffer);
  return buffer;
}

void env_set(Env* env, mal_t* symbol, mal_t* value) {
  cdict__add(&(env -> data), symbol, value);
}
