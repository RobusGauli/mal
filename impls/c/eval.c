#include <assert.h>
#include <string.h>

#include "eval.h"
#include "reader.h"
#include "token.h"
#include "str.h"

Node READ(char *expr) { return read_str(expr); }

Node eval_node_symbol(Node node, cdict_node_func_t *cdict_node_func) {
  char *mem = node.nodeval.nodesymbol.stringview.mem;
  size_t len = node.nodeval.nodesymbol.stringview.len;

  void *pointer = NULL;

  bool ok = cdict__get(cdict_node_func, node, &pointer);
  if (!ok) {
    Str str = str__new();
    str__nappend(&str, "Uncaught error: var ");
    str__append(&str, mem, len);
    str__nappend(&str, " not found");
    return node_error__new(str);
  }
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
     Node node = EVAL(cvector_iterator__next(&iterator), cdict_node_func);
     if (node.nodetype == NODE__ERR) {
       return node;
     }
      cvector__add(new_cvector_nodes, node);
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

  Node res = eval_ast(node, cdict_node_func); // this should resolve the symbol
  // take the first item of the evaluated list
  // this is result of the list
  cvector_nodes_t *_inner_nodes = res.nodeval.nodevector.mem;
  Node first_node = cvector__first(_inner_nodes);
  void *funcpointer = first_node.nodeval.nodesymbolvalue.funcpointer;
  Node (*func)(Node, Node) = funcpointer;
  Node result_node =
      func(cvector__index(_inner_nodes, 1), cvector__index(_inner_nodes, 2));
  return result_node;
}
