#ifndef EVAL_H
#define EVAL_H

#include "reader.h"
#include "env.h"
#include "node.h"

#define NODE__INT(node) ((node).nodeval.nodeint.val)

Node EVAL(Node node, Env* env);
Node env__div(Node a, Node b);
Node env__minus(Node a, Node b);
Node env__mul(Node a, Node b);
Node env__sum(Node a, Node b);
Node eval_node_symbol(Node node, Env env);
Node make_node_int(int val);
Node make_node_symbol(char* mem, size_t len);
Node node_symbol_value__new(Node node);

#endif
