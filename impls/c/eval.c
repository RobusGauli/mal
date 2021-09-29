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

Node resolve_symbol(Node node, Env *env) {

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

Node eval_vector(Node node, Env *env) {

  cvector_nodes_t *cvector_nodes = NODE__VECTOR_MEM_(node);

  if (!cvector__size(cvector_nodes)) {
    return node__empty_list();
  }

  Node first = cvector__first(cvector_nodes);

  switch (first.nodetype) {
  // user> (+ (let* name 333 h (+ name 30)) 5) // answer is 338
  case NODE__SYMBOL: {
    NodeSymbolType symbol_type = NODE__SYMBOL_TYPE_(first);

    if (symbol_type == NODESYMBOL__SPECIAL_FN_FORM) {

      if (cvector__size(cvector_nodes) != 3) {
        Str str = str__new();
        str__nappend(&str, "ValueError: takes 2 positional arguments but ");
        str__intappend(&str, cvector__size(cvector_nodes) - 1);
        str__nappend(&str, " were given");
        str__done(&str);
        return node_error__new(str);
      }

      /*Node second = cvector__index(cvector_nodes, 1);*/
      /*Node third = cvector__index(cvector_nodes, 2);*/
      return node_function_closure__new(env, cvector_nodes);
    }

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

      printf("the node type is: %d\n", node.nodetype);
      void *funcpointer = NODE__SYMBOLVALUE_FUNCPTR_(node);
      Node (*func)(cvector_nodes_t * cvector_node, Env * env) = funcpointer;
      Node result_node = func(cvector_nodes, env);
      return result_node;
    }

  case NODE__VECTOR: {
    Node result = EVAL(first, env);
    cvector_nodes_t *ns = NODE__FUNCTION_CVECTOR_(result);
    if (NODE__TYPE_(result) == NODE__FUNCTION_CLOSURE) {
      Node params = cvector__index(ns, 1);
      assert(params.nodetype == NODE__VECTOR);

      Node body = cvector__index(ns, 2);


      cvector_nodes_t* cvector_params = NODE__VECTOR_MEM_(params);

      cvector_nodes_t cvector_exprs;
      cvector__init(&cvector_exprs);
      cvector_iterator_nodes_t cvector_iterator;
      cvector_iterator__init(&cvector_iterator, cvector_nodes);
      /*// skip the first one*/
      cvector_iterator__next(&cvector_iterator);
      for(;;) {
        if (cvector_iterator__done(&cvector_iterator)) break;

        Node arg = EVAL(cvector_iterator__next(&cvector_iterator), env);
        if (NODE__IS_ERR_(arg)) {
          return arg;
        }
        cvector__add(&cvector_exprs, arg);
      }

      // extract the params
      /*cvector_nodes_t* cvector_closure = NODE__FUNCTION_CVECTOR_(result);*/
      /*Node params = cvector__index(cvector_closure, 1);*/

      /*cvector_nodes_t* cvector_params = NODE__VECTOR_MEM_(params);*/

      /*// create*/
      /*printf("the size is: %ld\n", cvector__size(cvector_params));*/
      Env closured_env = env__new_with_binds(cvector_params, &cvector_exprs);
      closured_env.outer = env;

      /*// evaluate the body*/
      /*Node body = cvector__index(cvector_closure, 2);*/
      Node func_result = EVAL(body, &closured_env);
      return func_result;
    }
    return  result;
  }

  case NODE__FUNCTION_CLOSURE: {

                                 printf("here is the result: %d\n", node.nodetype);
    return node;
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
}}

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
