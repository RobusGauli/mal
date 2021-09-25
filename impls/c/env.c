#include <assert.h>

#include "deps/cdict.h/cdict.h"
#include "env.h"
#include "eval.h"
#include "node.h"
#include "reader.h"

bool node_comparator(Node *self, Node *other) {
  return memcmp(NODE__SYMBOL_STRINGVIEW_MEM_(*self),
                NODE__SYMBOL_STRINGVIEW_MEM_(*other),
                NODE__SYMBOL_STRINGVIEW_LEN_(*self)) == 0;
}

cdict__u64 node_hasher(Node *self, cdict__u64 (*hash)(void *, size_t)) {
  return hash(NODE__SYMBOL_STRINGVIEW_MEM_(*self),
              NODE__SYMBOL_STRINGVIEW_LEN_(*self));
}

Env env__new(void) {
  Env env;
  env.outer = NULL;
  cdict__init(&(env.current));
  cdict__set_comparator(&(env.current), node_comparator);
  cdict__set_hash(&(env.current), node_hasher);

  return env;
}

Node env__sum(Node a, Node b) {
  return (Node){.nodetype = NODE__INT,
                .nodeval = {.nodeint = {.val = (NODE__INT(a) + NODE__INT(b))}}};
}

Node env__minus(Node a, Node b) {
  return (Node){.nodetype = NODE__INT,
                .nodeval = {.nodeint = {.val = (NODE__INT(a) - NODE__INT(b))}}};
}

Node env__mul(Node a, Node b) {
  return (Node){.nodetype = NODE__INT,
                .nodeval = {.nodeint = {.val = (NODE__INT(a) * NODE__INT(b))}}};
}

Node env__div(Node a, Node b) {
  return (Node){.nodetype = NODE__INT,
                .nodeval = {.nodeint = {.val = (NODE__INT(a) / NODE__INT(b))}}};
}

Node node_from_func_pointer(void *funcpointer) {
  return (Node){.nodetype = NODE__SYMBOL_VALUE,
                .nodeval = {.nodesymbolvalue = {.funcpointer = funcpointer}}};
}

void env__setup_initial(Env *env) {

  cdict__add(&(env->current), NODE_SYMBOL__NEW_("+", 1),
             node_from_func_pointer(env__sum));

  cdict__add(&(env->current), NODE_SYMBOL__NEW_("-", 1),
             node_from_func_pointer(env__minus));

  cdict__add(&(env->current), NODE_SYMBOL__NEW_("*", 1),
             node_from_func_pointer(env__mul));

  cdict__add(&(env->current), NODE_SYMBOL__NEW_("/", 1),
             node_from_func_pointer(env__div));
}
void env__set(Env *env, Node k, Node v) { cdict__add(&(env->current), k, v); }

Env *env__find(Env *env, Node key) {
  if (env == NULL) {
    return NULL;
  }
  Node val;
  bool ok = cdict__get(&(env->current), (key), (&val));
  printf("ok: %d\n", ok);
  if (ok) {
    return env;
  } else {
    return env__find((env->outer), key);
  }
}

Node env__get(Env *env, Node key) {
  Node val;

  bool ok = cdict__get(&(env->current), key, &val);
  assert(ok);

  return val;
}
