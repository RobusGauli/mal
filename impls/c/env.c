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

Env env__new_with_binds(cvector_nodes_t* symbols, cvector_nodes_t* exprs) {
  Env env = env__new();
  // create a iterator out of symbols
  cvector_iterator_nodes_t s_iterator;
  cvector_iterator__init(&s_iterator, symbols);

  cvector_iterator_nodes_t e_iterator;
  cvector_iterator__init(&e_iterator, exprs);

  for(;;) {
    if (cvector_iterator__done(&s_iterator) || cvector_iterator__done(&e_iterator)) break;
    Node symbol = cvector_iterator__next(&s_iterator);
    Node expr = cvector_iterator__next(&e_iterator);
    env__set(&env, symbol, expr);
  }

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
