#include <stdlib.h>
#include <string.h>

#include "deps/cdict.h/cdict.h"
#include "env.h"
#include "mal.h"

mal_t* core_add(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);

  int a = cvector__index(args, 0) -> value;
  int b = cvector__index(args, 1) -> value;

  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (uint64_t)(a+b);
  return result;
}

mal_t* core_mul(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);
  int a = cvector__index(args, 0) -> value;
  int b  = cvector__index(args, 1) -> value;
  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (uint64_t)(a*b);
  return result;
}

mal_t* core_sub(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);
  int a = cvector__index(args, 0) -> value;
  int b  = cvector__index(args, 1) -> value;
  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (uint64_t)(a-b);
  return result;
}

mal_t* core_div(mal_t* arg) {
  mals_t* args = (mals_t*)(arg -> value);
  int a = cvector__index(args, 0) -> value;
  int b  = cvector__index(args, 1) -> value;
  mal_t* result = malloc(sizeof(mal_t));
  result -> type = mal_number;
  result -> value = (uint64_t)(a/b);
  return result;
}

bool env_comp(char** self, char** other) {
  return strcmp(*self, *other) == 0;
}

cdict__u64 env_hash(char** self, cdict__u64 (*hash) (void*, size_t)) {
  return hash(*self, strlen(*self));
}

Env root_env() {
  cdict_env_t *cdict_env = malloc(sizeof(cdict_env_t));
  cdict__init(cdict_env);
  cdict__set_comparator(cdict_env, env_comp);
  cdict__set_hash(cdict_env, env_hash);

  // initialize from the core
  mal_t* add = malloc(sizeof(mal_t));
  add -> type = mal_func;
  add -> value = (uint64_t)core_add;

  // initialize from the core
  mal_t* mul = malloc(sizeof(mal_t));
  mul -> type = mal_func;
  mul-> value = (uint64_t)core_mul;

  mal_t* sub = malloc(sizeof(mal_t));
  sub -> type = mal_func;
  sub-> value = (uint64_t)core_sub;

  mal_t* div  = malloc(sizeof(mal_t));
  div  -> type = mal_func;
  div -> value = (uint64_t)core_div;



  cdict__add(cdict_env, "+", add);
  cdict__add(cdict_env, "*", mul);
  cdict__add(cdict_env, "/", div);
  cdict__add(cdict_env, "-", sub);

  return (Env){.current = cdict_env, .prev = NULL};

}

Env env() {
  return(Env){.current=NULL, .prev=NULL};
}

mal_t* env_get(Env env, mal_t* symbol) {
  mal_t* result = NULL;
  bool ok = cdict__get(env.current, (char*)symbol -> value, &result);
  if (!ok) {
    return NULL;
  }
  return result;
}
