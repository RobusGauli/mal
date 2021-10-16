#ifndef ENV_H
#define ENV_H

#include "mal.h"
#include "deps/cdict.h/cdict.h"

typedef mal_t* mal_ptr_t;
CDict(mal_ptr_t, mal_ptr_t) cdict_env_t;

typedef struct Env {
  struct Env* prev;
  cdict_env_t data;
} Env;

Env* new_env(Env* prev);

mal_t* env_get(Env* env, mal_t* symbol);
Env* env_find(Env* env, mal_t* symbol);
void env_set(Env* env, mal_t* symbol, mal_t* value);

// core functions
mal_t* core_add(mal_t* arg);
mal_t* core_mul(mal_t* arg);
mal_t* core_sub(mal_t* arg);
mal_t* core_div(mal_t* arg);

#endif
