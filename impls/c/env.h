#ifndef ENV_H
#define ENV_H

#include "mal.h"
#include "deps/cdict.h/cdict.h"

typedef char* string;

typedef mal_t* mal_ptr_t;

CDict(string, mal_ptr_t) cdict_env_t;

typedef struct Env {
  cdict_env_t* current;
  cdict_env_t* prev;
} Env;

Env root_env();
Env env();

mal_t* env_get(Env env, mal_t* symbol);

#endif
