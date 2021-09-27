#ifndef CORE_H
#define CORE_H

#include "node.h"
#include "env.h"

typedef struct {
  cdict_node_t cdict_node;
} NS;

typedef struct {
  NS ns;
} Core;

// core functions
Node core__sum(cvector_nodes_t* cvector_nodes, Env* env);
Node core__minus(cvector_nodes_t* cvector_nodes, Env* env);
Node core__mul(cvector_nodes_t* cvector_nodes, Env* env);
Node core__list(cvector_nodes_t* cvector_nodes, Env* env);
Node core__is_list(cvector_nodes_t* cvector_nodes, Env* env);
Node core__is_empty(cvector_nodes_t* cvector_nodes, Env* env);
Node core__count(cvector_nodes_t* cvector_nodes, Env* env);
Node core__div(cvector_nodes_t* cvector_nodes, Env* env);
Node core__is_eq(cvector_nodes_t* cvector_nodes, Env*env);

void core__setup_initial(Env* env, Core* core);

// constructor
Core core__new();
// construct for iterator
cdict_iterator_node_t core__ns_new_iterator(Core* core);
Node core__node_from_funcptr(void *funcpointer);
#endif
