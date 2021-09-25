#ifndef ENV_H
#define ENV_H

#include "reader.h"
#include "deps/cdict.h/cdict.h"
#include "node.h"

CDict(Node, Node) cdict_node_t;

typedef struct Env {
  cdict_node_t current;
  struct Env* outer;
} Env;

// constructor
Env env__new();
void env__setup_initial(Env* env);

// setter and getters
void env__set(Env* env, Node k, Node v);
Env* env__find(Env* env, Node key);
Node env__get(Env* env, Node key);

Node env__sum(Node a, Node b);
Node env__minus(Node a , Node b);
Node env__mul(Node a, Node b);
Node env_div(Node a, Node b);

bool node_comparator(Node *self, Node *other);

cdict__u64 node_hasher(Node *self, cdict__u64 (*hash)(void *, size_t));

#endif
