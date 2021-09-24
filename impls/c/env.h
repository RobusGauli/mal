#ifndef ENV_H
#define ENV_H

#include "reader.h"
#include "deps/cdict.h/cdict.h"
#include "node.h"

typedef void* func;

CDict(Node, func) cdict_node_func_t;

void setup_environ(cdict_node_func_t *cdict_node_func);

Node env__sum(Node a, Node b);
Node env__minus(Node a , Node b);
Node env__mul(Node a, Node b);
Node env_div(Node a, Node b);

bool node_comparator(Node *self, Node *other);

cdict__u64 node_hasher(Node *self, cdict__u64 (*hash)(void *, size_t));

#endif
