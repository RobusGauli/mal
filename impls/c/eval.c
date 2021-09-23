#include <assert.h>

#include "reader.h"
#include "eval.h"
#include "token.h"

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

Node make_node_symbol(char* mem, size_t len) {
  return (Node) {
    .nodetype = NODE__SYMBOL,
      .nodeval = {
        .nodesymbol = {
          .stringview = stringview__new(mem, len)
        }
      }
  };
}

Node READ(char *expr) { return read_str(expr); }

Node node_symbol_value__new(void *pointer) {
  Node node = {.nodetype = NODE__SYMBOL_VALUE,
               .nodeval = {.nodesymbolvalue = {.funcpointer = pointer}}};
  return node;
}

Node eval_node_symbol(Node node, cdict_node_func_t *cdict_node_func) {
  char *mem = node.nodeval.nodesymbol.stringview.mem;
  size_t len = node.nodeval.nodesymbol.stringview.len;

  void* pointer = NULL;
  char* a = node.nodeval.nodesymbol.stringview.mem;
  size_t size = node.nodeval.nodesymbol.stringview.len;
  bool ok = cdict__get(cdict_node_func, node, &pointer);
  assert(ok);
  return node_symbol_value__new(pointer);
}

Node EVAL(Node node, cdict_node_func_t *cdict_node_func);

Node eval_ast(Node node, cdict_node_func_t *cdict_node_func) {
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
            cdict_node_func
            )
          );
    }
    return node_vector__new(new_cvector_nodes);
  }

  case NODE__SYMBOL:
    // if that is the case look up the symbol
    return eval_node_symbol(node, cdict_node_func);

  default:
    return node;
  }
}

Node EVAL(Node node, cdict_node_func_t *cdict_node_func) {
  Node result = eval_ast(node, cdict_node_func);
  if (node.nodetype != NODE__VECTOR) {
    return eval_ast(node, cdict_node_func);
  }

  cvector_nodes_t *cvector_nodes = node.nodeval.nodevector.mem;
  if (!cvector__size(cvector_nodes)) {
    return node;
  }

  Node res =
      eval_ast(node, cdict_node_func); // this should resolve the symbol
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
