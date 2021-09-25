#include <assert.h>
#include <string.h>

#include "debug.h"
#include "eval.h"
#include "node.h"
#include "reader.h"
#include "str.h"
#include "token.h"

Node eval_node_symbol(Node node, Env env) {

  Env *found_env = env__find(&env, node);

  if (found_env == NULL) {
    Str str = str__new();
    str__nappend(&str, "Uncaught error: var ");
    str__append(&str, NODE__SYMBOL_STRINGVIEW_MEM_(node),
                NODE__SYMBOL_STRINGVIEW_LEN_(node));
    str__nappend(&str, " not found");
    return node_error__new(str);
  }

  Node n = env__get(found_env, node);

  return node_symbol_value__new(n);
}

Node resolve_symbol(Node node, Env env) {

  Str v = debug(node);
  Env *found_env = env__find(&env, node);
  if (found_env == NULL) {
    Str str = str__new();
    str__nappend(&str, "Uncaught error: var ");
    str__cappend(&str, '\'');
    str__append(&str, NODE__SYMBOL_STRINGVIEW_MEM_(node),
                NODE__SYMBOL_STRINGVIEW_LEN_(node));
    str__cappend(&str, '\'');
    str__nappend(&str, " not found");
    str__done(&str);
    return node_error__new(str);
  }

  Node found_node = env__get(found_env, node);

  return found_node;
}

Node eval_vector(Node node, Env env) {
  cvector_nodes_t *cvector_nodes = NODE__VECTOR_MEM_(node);
  if (!cvector__size(cvector_nodes)) {
    return node_nil__new();
  }

  Node first = cvector__first(cvector_nodes);

  switch (first.nodetype) {
  // user> (+ (let* name 333 h (+ name 30)) 5) // answer is 338
  case NODE__SYMBOL: {
    NodeSymbolType symbol_type = NODE__SYMBOL_TYPE_(first);

    if (symbol_type == NODESYMBOL__SPECIAL_DEF_FORM) {
      Node second_elem = cvector__index(cvector_nodes, 1);
      // bubble up error node if this is not of type symbol
      if (NODE__TYPE_(second_elem) != NODE__SYMBOL ||
          NODE__SYMBOL_TYPE_(second_elem) != NODESYMBOL__NORM_FORM) {
        Str str = str__new();
        str__nappend(&str, "KeyError: ");
        Str node_stringified = debug(second_elem);
        str__add(&str, &node_stringified);
        str__nappend(&str, " cannot be used as a key for def! env bindings");
        str__done(&str);
        str__free(&node_stringified);
        return node_error__new(str);
      }

      Node third_elem = cvector__index(cvector_nodes, 2);
      // if that is not the case, set this as a key
      Node evaluated_third_elem = EVAL(third_elem, env);
      env__set(&env, second_elem, evaluated_third_elem);
      return evaluated_third_elem;
    }

    if (symbol_type == NODESYMBOL__NORM_FORM) {
      Node node = resolve_symbol(first, env);

      if (NODE__IS_ERR_(node)) {
        return node;
      }

      if (cvector__size(cvector_nodes) != 3) {
        Str str = str__new();
        str__nappend(&str, "cannot be more than 2 arguments");
        str__done(&str);
        return node_error__new(str);
      }

      void *funcpointer = NODE__SYMBOLVALUE_FUNCPTR_(node);
      Node (*func)(Node, Node) = funcpointer;
      Node result_node = func(EVAL(cvector__index(cvector_nodes, 1), env),
                              EVAL(cvector__index(cvector_nodes, 2), env));
      return result_node;
    }
  }

  default: {

    Str str = str__new();
    Str what = debug(first);
    str__cappend(&str, '\'');
    str__add(&str, &what);
    str__cappend(&str, '\'');
    str__nappend(&str, " is not a symbol; try using a symbol instead");
    str__done(&str);
    return node_error__new(str);
  }
  }
}

Node EVAL(Node node, Env env) {
  switch (node.nodetype) {
  case NODE__NIL:
    return node;
  case NODE__INT:
    return node;
  case NODE__STRING:
    return node;
  case NODE__COMMENT:
    return node;
  case NODE__SYMBOL:
    // yup resolve this shit
    return resolve_symbol(node, env);
  case NODE__SYMBOL_VALUE:
    return node;
  case NODE__VECTOR:
    return eval_vector(node, env);
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
