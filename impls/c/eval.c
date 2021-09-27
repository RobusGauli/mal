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

Node resolve_symbol(Node node, Env* env) {

  Str v = debug(node);
  Env *found_env = env__find(env, node);
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

Node eval_vector(Node node, Env* env) {
  cvector_nodes_t *cvector_nodes = NODE__VECTOR_MEM_(node);
  if (!cvector__size(cvector_nodes)) {
    return node__empty_list();
  }

  Node first = cvector__first(cvector_nodes);

  switch (first.nodetype) {
  // user> (+ (let* name 333 h (+ name 30)) 5) // answer is 338
  case NODE__SYMBOL: {
    NodeSymbolType symbol_type = NODE__SYMBOL_TYPE_(first);

    if (symbol_type == NODESYMBOL__SPECIAL_LET_FORM) {
      if (cvector__size(cvector_nodes) != 3) {
        Str str = str__new();

        str__nappend(&str, "ValueError: takes 2 positional arguments but ");
        str__intappend(&str, cvector__size(cvector_nodes) - 1);
        str__nappend(&str, " were given");
        str__done(&str);
        return node_error__new(str);
      }

      Node second = cvector__index(cvector_nodes, 1);
      if (NODE__TYPE_(second) != NODE__VECTOR) {
        Str str = str__new();
        str__nappend(&str, "ValueError: expected first argument to be grouped "
                           "s-expression for let* binding");
        str__done(&str);
        return node_error__new(str);
      }

      Env new_env = env__new();
      new_env.outer = env;

      cvector_nodes_t *cvector_nodes_second = second.nodeval.nodevector.mem;
      cvector_iterator_nodes_t iterator;
      cvector_iterator__init(&iterator, cvector_nodes_second);

      for (;;) {
        if (cvector_iterator__done(&iterator))
          break;
        Node key = cvector_iterator__next(&iterator);

        // error out if we don't have complete key/val pair
        if (cvector_iterator__done(&iterator)) {
          Str str = str__new();
          str__nappend(&str, "ValueError: value pair expected for key '");
          Str debug_key = debug(key);
          str__add(&str, &debug_key);
          str__free(&debug_key);
          str__nappend(&str, "' in let* binding(s)");
          str__done(&str);
          return node_error__new(str);
        }

        Node val = cvector_iterator__next(&iterator);
        env__set(&new_env, key, EVAL(val, &new_env));
      }

      Node third = cvector__index(cvector_nodes, 2);
      return EVAL(third, &new_env);
    }

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
      if (NODE__IS_ERR_(evaluated_third_elem)) {
        return evaluated_third_elem;
      }
      env__set(env, second_elem, evaluated_third_elem);
      return evaluated_third_elem;
    }

    if (symbol_type == NODESYMBOL__NORM_FORM) {
      Node node = resolve_symbol(first, env);

      if (NODE__IS_ERR_(node)) {
        return node;
      }

      void *funcpointer = NODE__SYMBOLVALUE_FUNCPTR_(node);
      Node (*func)(cvector_nodes_t* cvector_node, Env* env) = funcpointer;
      Node result_node = func(cvector_nodes, env);
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

Node EVAL(Node node, Env *env) {
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
  case NODE__TRUE:
    return node;
  case NODE__FALSE:
    return node;
  default:
    assert(0 && "unreachable");
  }
}
