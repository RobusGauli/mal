#include <assert.h>

#include "deps/cdict.h/cdict.h"
#include "env.h"
#include "eval.h"
#include "node.h"
#include "reader.h"
#include "core.h"


Env env__new(void) {
  Env env;
  env.outer = NULL;
  cdict__init(&(env.current));
  cdict__set_comparator(&(env.current), node_comparator);
  cdict__set_hash(&(env.current), node_hasher);

  return env;
}

void env__set(Env *env, Node k, Node v) { cdict__add(&(env->current), k, v); }

Env *env__find(Env *env, Node key) {
  if (env == NULL) {
    return NULL;
  }
  Node val;
  bool ok = cdict__get(&(env->current), (key), (&val));
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

void env__setup_initial(Env* env, Core* core) {
  cdict_iterator_node_t iterator = core__ns_new_iterator(core);
  for (;;) {
    if (cdict_iterator__done(&iterator)) break;
    Node value;
    Node key = cdict_iterator__next_keyval(&iterator, &value);
    env__set(env, key, value);
  }
}
