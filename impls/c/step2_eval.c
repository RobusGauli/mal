#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "deps/cdict.h/cdict.h"
#include "reader.h"

typedef char *string;
typedef void *func;

CDict(string, func) cdict_string_func_t;

#define NODE__INT(node) ((node).nodeval.nodeint.val)

Node make_node_int(int val) {
  return (Node) {
    .nodetype=NODE__INT,
      .nodeval = {
        .nodeint = {
          .val=val
        }
      }
  };
}

Node env__sum(Node a, Node b) {
  return make_node_int(NODE__INT(a) + NODE__INT(b));
}

Node env__minus(Node a, Node b) {
  return make_node_int(NODE__INT(a) - NODE__INT(b));
}

Node env__mul(Node a, Node b) {
  return make_node_int(NODE__INT(a) * NODE__INT(b));
}

NodeInt env__div(NodeInt a, NodeInt b) {
  return make_node_int(NODE__INT(a) / NODE__INT(b));
}

Node READ(char *expr) { return read_str(expr); }

Node node_symbol_value__new(void *pointer) {
  Node node = {.nodetype = NODE__SYMBOL_VALUE,
               .nodeval = {.nodesymbolvalue = {.funcpointer = pointer}}};
  return node;
}
Node eval_node_symbol(Node node, cdict_string_func_t *cdict_string_func) {
  // get the symbol value;
  char *mem = node.nodeval.nodesymbol.mem;
  size_t len = node.nodeval.nodesymbol.len;

  char buffer[len + 1];
  memcpy(buffer, mem, len);
  buffer[len] = 0;
  void *pointer = NULL;
  bool ok = cdict__get(cdict_string_func, "+", &pointer);
  return node_symbol_value__new(env__sum);
}

Node EVAL(Node node, cdict_string_func_t *cdict_string_func);

Node eval_ast(Node node, cdict_string_func_t *cdict_string_func) {
  switch (node.nodetype) {
  case NODE__VECTOR: {

    cvector_nodes_t *new_cvector_nodes = malloc(sizeof(cvector_nodes_t));
    cvector__init(new_cvector_nodes);
    cvector_nodes_t *old_cvector_nodes = node.nodeval.nodevector.mem;

    cvector_iterator_nodes_t iterator;
    cvector_iterator__init(&iterator, old_cvector_nodes);

    for (;;) {
      if (cvector_iterator__done(&iterator))
        break;
      cvector__add(
          new_cvector_nodes,
          EVAL(
            cvector_iterator__next(&iterator),
            cdict_string_func
            )
          );
    }
    return node_vector__new(new_cvector_nodes);
  }

  case NODE__SYMBOL:
    // if that is the case look up the symbol
    return eval_node_symbol(node, cdict_string_func);

  default:
    return node;
  }
}

Node EVAL(Node node, cdict_string_func_t *cdict_string_func) {
  Node result = eval_ast(node, cdict_string_func);
  if (node.nodetype != NODE__VECTOR) {
    return eval_ast(node, cdict_string_func);
  }

  cvector_nodes_t *cvector_nodes = node.nodeval.nodevector.mem;
  if (!cvector__size(cvector_nodes)) {
    return node;
  }

  Node res =
      eval_ast(node, cdict_string_func); // this should resolve the symbol
  // take the first item of the evaluated list
  // this is result of the list
  cvector_nodes_t *_inner_nodes = res.nodeval.nodevector.mem;
  Node first_node = cvector__first(_inner_nodes);
  void *funcpointer = first_node.nodeval.nodesymbolvalue.funcpointer;
  Node (*func)(Node, Node) = funcpointer;
  Node r =
      func(cvector__index(_inner_nodes, 1), cvector__index(_inner_nodes, 2));
  return r;
}

char *PRINT(Node node) {
  debug(node);
  return "";
}

void setup_environ(cdict_string_func_t *cdict_string_func) {
  // just care about the things
  cdict__add(cdict_string_func, "+", env__sum);
  cdict__add(cdict_string_func, "-", env__minus);
  cdict__add(cdict_string_func, "*", env__mul);
  cdict__add(cdict_string_func, "/", env__div);
}

int main(void) {

  for (;;) {
    char *input = readline("user> ");

    if (input == NULL) {
      exit(1);
    }

    // create environment
    cdict_string_func_t cdict_string_func;
    cdict__init(&cdict_string_func);
    // setup environment
    setup_environ(&cdict_string_func);

    Node node = READ(input);
    Node evaluated_node = EVAL(node, &cdict_string_func);
    char *result = PRINT(evaluated_node);
    printf("%s\n", result);
  }
}
