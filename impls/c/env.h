#ifndef ENV_H
#define ENV_H

#include "reader.h"
#include "deps/cdict.h/cdict.h"
#include "node.h"

typedef struct Env {
  cdict_node_t current;
  struct Env* outer;
} Env;

// constructor
Env env__new();

// setter and getters
void env__set(Env* env, Node k, Node v);
Env* env__find(Env* env, Node key);
Node env__get(Env* env, Node key);


#endif
