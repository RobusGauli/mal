#include <assert.h>
#include <string.h>

#include "eval.h"
#include "reader.h"
#include "token.h"
#include "str.h"
#include "debug.h"


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
    cvector_nodes_t *old_cvector_nodes = node.nodeval.nodevector.mem;
    if (!cvector__size(old_cvector_nodes)) {
      return node_nil__new();
    }

    cvector_nodes_t *new_cvector_nodes = malloc(sizeof(cvector_nodes_t));
    cvector__init(new_cvector_nodes);

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
    return eval_node_symbol(node, cdict_node_func);

  case NODE__ERR:
    return node;
  default:
    return node;
  }
}

Node EVAL1(Node node, cdict_node_func_t *cdict_node_func) {
  Node result = eval_ast(node, cdict_node_func);
  if (node.nodetype != NODE__VECTOR) {
    return eval_ast(node, cdict_node_func);
  }

  cvector_nodes_t *cvector_nodes = node.nodeval.nodevector.mem;
  if (!cvector__size(cvector_nodes)) {
    return node;
  }

  Node res = eval_ast(node, cdict_node_func); // this should resolve the symbol
  // it could be error
  if (res.nodetype == NODE__ERR) {
    return res;
  }
  if (res.nodetype == NODE__EMPTY) {
    return res;
  }
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


Node resolve_symbol(Node node, cdict_node_func_t* cdict_node_func) {
  char *mem = node.nodeval.nodesymbol.stringview.mem;
  size_t len = node.nodeval.nodesymbol.stringview.len;

  void *pointer = NULL;

  bool ok = cdict__get(cdict_node_func, node, &pointer);
  if (!ok) {
    Str str = str__new();
    str__nappend(&str, "Uncaught error: var ");
    str__cappend(&str,'\'');
    str__append(&str, mem, len);
    str__cappend(&str,'\'');
    str__nappend(&str, " not found");
    str__done(&str);
    return node_error__new(str);
  }
  return node_symbol_value__new(pointer);
}

Node eval_vector(Node node, cdict_node_func_t* cdict_node_func) {
  // just go through the each element and resolve the shit
  cvector_nodes_t* cvector_nodes = node.nodeval.nodevector.mem;
  if (!cvector__size(cvector_nodes)) {
    return node_nil__new();
  }

  Node first = cvector__first(cvector_nodes);
  Node second = cvector__index(cvector_nodes, 1);
  Node third = cvector__index(cvector_nodes, 2);
  // try an resolve the symbol
  // first element must be symbol
  if (first.nodetype != NODE__SYMBOL) {
    // it must be symbol
    Str str = str__new();
    Str what = debug(first);
    str__cappend(&str, '\'');
    str__add(&str, &what);
    str__cappend(&str, '\'');
    str__nappend(&str, " is not a symbol; try using a symbol instead");
    str__done(&str);
    return node_error__new(str);
  }


  // resolve symbol
  Node symbolval = resolve_symbol(first, cdict_node_func);
  // once the symbol is resolved
  if (symbolval.nodetype != NODE__SYMBOL_VALUE) {
    return symbolval;
  }

  if (cvector__size(cvector_nodes) != 3) {
    Str str = str__new();
    str__nappend(&str, "cannot be more than 2 arguments");
    str__done(&str);
    return node_error__new(str);
  }

  void *funcpointer = symbolval.nodeval.nodesymbolvalue.funcpointer;
  Node (*func)(Node, Node) = funcpointer;
  Node result_node =
      func(EVAL(cvector__index(cvector_nodes, 1), cdict_node_func), EVAL(cvector__index(cvector_nodes, 2), cdict_node_func));
  return result_node;
}

Node EVAL(Node node, cdict_node_func_t* cdict_node_func) {
  switch(node.nodetype) {
   case NODE__NIL:
     return node;
   case NODE__INT:
     return node;
   case NODE__STRING:
     return node;
   case NODE__COMMENT:
     return node;
   case NODE__SYMBOL:
     return resolve_symbol(node, cdict_node_func);
   case NODE__SYMBOL_VALUE:
     return node;
   case NODE__VECTOR:
     return eval_vector(node, cdict_node_func);
   case NODE__EMPTY:
     return node;
   case NODE__EOF:
     return node;
   case NODE__ERR:
      return node;
   default:
    assert(0 && "unreachable");
  }
}
