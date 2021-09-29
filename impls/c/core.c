/* Core file that contains the initial repl environment's functions */
#include <assert.h>

#include "core.h"
#include "env.h"
#include "eval.h"
#include "node.h"
#include "str.h"

void abort() { exit(1); }

#define __unimplemented__                                                      \
  do {                                                                         \
    fprintf(stderr, "function: %s not implemented at line %d in file '%s'\n",  \
            __FUNCTION__, __LINE__, __FILE__);                                 \
    abort();                                                                   \
  } while (0)

Core core__new() {
  cdict_node_t cdict_node;
  cdict__init(&cdict_node);

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("+", 1),
             core__node_from_funcptr(core__sum));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("-", 1),
             core__node_from_funcptr(core__minus));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("*", 1),
             core__node_from_funcptr(core__mul));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("/", 1),
             core__node_from_funcptr(core__div));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("list", 4),
             core__node_from_funcptr(core__list));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("list?", 5),
             core__node_from_funcptr(core__is_list));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("empty?", 6),
             core__node_from_funcptr(core__is_empty));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("count", 5),
             core__node_from_funcptr(core__count));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("=", 1),
             core__node_from_funcptr(core__is_eq));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("if", 2),
             core__node_from_funcptr(core__if));

  cdict__add(&cdict_node, NODE_SYMBOL__NEW_(">", 1),
             core__node_from_funcptr(core__more));
  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("<", 1),
             core__node_from_funcptr(core__less));
  cdict__add(&cdict_node, NODE_SYMBOL__NEW_(">=", 2),
             core__node_from_funcptr(core__more_equals));
  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("<=", 2),
             core__node_from_funcptr(core__less_equals));
  cdict__add(&cdict_node, NODE_SYMBOL__NEW_("fn*", 3),
             core__node_from_funcptr(core__function_closure));
  return (Core){.ns = {.cdict_node = cdict_node}};
}

cdict_iterator_node_t core__ns_new_iterator(Core *core) {
  cdict_iterator_node_t iterator;
  cdict_iterator__init(&iterator, &(core->ns.cdict_node));
  return iterator;
}

Node core__sum(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 3) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's length must be 2");
    str__done(&str);
    return node_error__new(str);
  }

  Node a = EVAL(cvector__index(cvector_nodes, 1), env);
  Node b = EVAL(cvector__index(cvector_nodes, 2), env);
  return (Node){
      .nodetype = NODE__INT,
      .nodeval = {.nodeint = {.val = (NODE__INT_VAL_(a) + NODE__INT_VAL_(b))}}};
}

Node core__minus(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 3) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's length must be 2");
    str__done(&str);
    return node_error__new(str);
  }

  Node a = EVAL(cvector__index(cvector_nodes, 1), env);
  Node b = EVAL(cvector__index(cvector_nodes, 2), env);

  return (Node){
      .nodetype = NODE__INT,
      .nodeval = {.nodeint = {.val = (NODE__INT_VAL_(a) - NODE__INT_VAL_(b))}}};
}

Node core__mul(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 3) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's length must be 2");
    str__done(&str);
    return node_error__new(str);
  }

  Node a = EVAL(cvector__index(cvector_nodes, 1), env);
  Node b = EVAL(cvector__index(cvector_nodes, 2), env);

  return (Node){
      .nodetype = NODE__INT,
      .nodeval = {.nodeint = {.val = (NODE__INT_VAL_(a) * NODE__INT_VAL_(b))}}};
}

Node core__div(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 3) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's length must be 2");
    str__done(&str);
    return node_error__new(str);
  }

  Node a = EVAL(cvector__index(cvector_nodes, 1), env);
  Node b = EVAL(cvector__index(cvector_nodes, 2), env);

  return (Node){
      .nodetype = NODE__INT,
      .nodeval = {.nodeint = {.val = (NODE__INT_VAL_(a) / NODE__INT_VAL_(b))}}};
}

Node core__node_from_funcptr(void *funcpointer) {
  return (Node){.nodetype = NODE__SYMBOL_VALUE,
                .nodeval = {.nodesymbolvalue = {.funcpointer = funcpointer}}};
}

Node core__list(cvector_nodes_t *cvector_nodes, Env *env) {
  cvector_nodes_t *list = malloc(sizeof(cvector_nodes_t));
  cvector__init(list);
  cvector_iterator_nodes_t iterator;
  cvector_iterator__init(&iterator, cvector_nodes);

  cvector_iterator__next(&iterator); // skip first
  for (;;) {
    if (cvector_iterator__done(&iterator))
      break;

    Node node = EVAL(cvector_iterator__next(&iterator), env);
    if (NODE__IS_ERR_(node)) {
      cvector__free(list);
      return node;
    }
    cvector__add(list, node);
  }
  return (Node){.nodetype = NODE__LIST,
                .nodeval = {.nodelist = {
                                .mem = list,
                            }}};
}

Node core__is_list(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 2) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's for list? length must be 1");
    str__done(&str);
    return node_error__new(str);
  }

  Node node = EVAL(cvector__index(cvector_nodes, 1), env);
  if (NODE__IS_ERR_(node)) {
    return node;
  }

  if (node.nodetype == NODE__LIST) {
    return node_true__new();
  }

  return node_false__new();
}

Node core__is_empty(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 2) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's for 'empty?' length must be 1");
    str__done(&str);
    return node_error__new(str);
  }

  Node node = EVAL(cvector__index(cvector_nodes, 1), env);
  if (NODE__IS_ERR_(node)) {
    return node;
  }

  if (node.nodetype != NODE__LIST) {
    Str str = str__new();
    str__nappend(&str, "TypeError: argument for 'empty?' be of type 'list'");
    str__done(&str);
    return node_error__new(str);
  }

  cvector_nodes_t *nodes = NODE__LIST_MEM_(node);
  size_t size = cvector__size(nodes);
  if (size) {
    return node_false__new();
  }

  return node_true__new();
}

Node core__count(cvector_nodes_t *cvector_nodes, Env *env) {

  if (cvector__size(cvector_nodes) != 2) {
    Str str = str__new();
    str__nappend(&str, "ValueError: argument's for 'count' length must be 1");
    str__done(&str);
    return node_error__new(str);
  }

  Node node = EVAL(cvector__index(cvector_nodes, 1), env);

  if (NODE__IS_ERR_(node)) {
    return node;
  }

  if (node.nodetype == NODE__NIL) {
    return nodeint__new(0);
  }

  if (node.nodetype != NODE__LIST) {
    Str str = str__new();
    str__nappend(&str, "TypeError: argument for 'count' be of type 'list'");
    str__done(&str);
    return node_error__new(str);
  }

  cvector_nodes_t *nodes = NODE__LIST_MEM_(node);
  size_t size = cvector__size(nodes);
  return nodeint__new(size);
}

Node _core_eq(Env *env, Node first, Node second) {
  switch (first.nodetype) {

  case NODE__NIL:
    return node_true__new();

  case NODE__INT:
    if (NODE__INT_VAL_(first) == NODE__INT_VAL_(second)) {
      return node_true__new();
    }
    return node_false__new();

  case NODE__TRUE:
    return node_true__new();

  case NODE__FALSE:
    return node_true__new();

  case NODE__STRING:

    if (str__eq(&NODE__STRING_STR_(first), &NODE__STRING_STR_(second))) {
      return node_true__new();
    }

    return node_false__new();

  case NODE__COMMENT:
    __unimplemented__;

  case NODE__SYMBOL:
    __unimplemented__;

  case NODE__SYMBOL_VALUE:
    if (first.nodeval.nodesymbolvalue.funcpointer ==
        second.nodeval.nodesymbolvalue.funcpointer) {
      return node_true__new();
    }
    return node_false__new();

  case NODE__VECTOR:
    __unimplemented__;

  case NODE__EMPTY:
    __unimplemented__;

  case NODE__EOF:
    __unimplemented__;

  case NODE__ERR:
    __unimplemented__;

  case NODE__LIST: {
    cvector_nodes_t *cvector_first = NODE__LIST_MEM_(first);
    cvector_nodes_t *cvector_second = NODE__LIST_MEM_(second);
    size_t first_node_size = cvector__size(cvector_first);
    size_t second_node_size = cvector__size(cvector_second);

    if (first_node_size != second_node_size) {
      return node_false__new();
    }

    cvector_iterator_nodes_t first_iterator;
    cvector_iterator_nodes_t second_iterator;

    cvector_iterator__init(&first_iterator, cvector_first);
    cvector_iterator__init(&second_iterator, cvector_second);

    for (;;) {
      if (cvector_iterator__done(&first_iterator) ||
          cvector_iterator__done(&second_iterator))
        break;
      Node first_next = cvector_iterator__next(&first_iterator);
      Node second_next = cvector_iterator__next(&second_iterator);

      Node res = _core_eq(env, first_next, second_next);

      if (res.nodetype == NODE__TRUE) {
        continue;
      }

      return node_false__new();
    }

    return node_true__new();
  }

  default:
    __unimplemented__;
  }
}

Node core__is_eq(cvector_nodes_t *cvector_nodes, Env *env) {

  size_t arg_count = cvector__size(cvector_nodes) - 1;

  if (cvector__size(cvector_nodes) != 3) {
    Str str = str__new();
    str__nappend(&str, "TypeError: '=' fn takes 2 arguments but ");
    str__intappend(&str, arg_count);
    str__nappend(&str, " were given");
    str__done(&str);
    return node_error__new(str);
  }

  Node first = EVAL(cvector__index(cvector_nodes, 1), env);

  if (NODE__IS_ERR_(first)) {
    return first;
  }

  Node second = EVAL(cvector__index(cvector_nodes, 2), env);

  if (NODE__IS_ERR_(second)) {
    return second;
  }

  if (first.nodetype != second.nodetype) {
    return node_false__new();
  }

  return _core_eq(env, first, second);
}

Node core__if(cvector_nodes_t *cvector_nodes, Env *env) {
  cvector_iterator_nodes_t iterator;
  cvector_iterator__init(&iterator, cvector_nodes);
  // skip the first elem
  assert(!cvector_iterator__done(&iterator));
  cvector_iterator__next(&iterator);

  if (cvector_iterator__done(&iterator))
    return node_nil__new();

  // grab the predicate
  Node predicate = cvector_iterator__next(&iterator);

  // evaluate
  Node predicate_res = EVAL(predicate, env);

  // check for the err
  if (NODE__IS_ERR_(predicate_res))
    return predicate_res;

  // return nil if no evaluation node is available
  if (cvector_iterator__done(&iterator))
    return node_nil__new();

  // evaluate the second argument if result is other than false / nil
  if (!(NODE__IS_FALSE_(predicate_res) || NODE__IS_NIL_(predicate_res))){
    Node truthy_node = cvector_iterator__next(&iterator);
    // eval
    return EVAL(truthy_node, env);
  }

  // if second argument is false or nil
  // skip the truthy arg
  cvector_iterator__next(&iterator);

  // check to see if we have falsy arg
  if (cvector_iterator__done(&iterator))
    return node_nil__new();

  // eval and return
  Node falsy_node = cvector_iterator__next(&iterator);
  // check to see if we have more elements and if that is the case throw out an
  // error
  if (!cvector_iterator__done(&iterator)) {
    Str str = str__new();
    str__nappend(&str, "'if' can't have more than 2 evaluation paths.");
    str__done(&str);
    return node_error__new(str);
  }

  // just so that you know
  return EVAL(falsy_node, env);
}

bool core__arg_len_mustbe(cvector_nodes_t* cvector_nodes, size_t len) {
  return (cvector__size(cvector_nodes) - 1) == len;
}

Node core__more_equals(cvector_nodes_t* cvector_nodes, Env* env) {
  // just to make sure how many parameters this function is going to take is kind of unknown
  // just so that we know what we are doing
  bool ok = core__arg_len_mustbe(cvector_nodes, 2);

  if (!ok) {
    Str str = str__new();
    str__nappend(&str, "TypeError: '>=' takes exactly 2 arguments (");
    str__intappend(&str, cvector__size(cvector_nodes) - 1);
    str__nappend(&str, " given)");
    str__done(&str);
    return node_error__new(str);
  }

  Node first = EVAL(cvector__index(cvector_nodes, 1), env);
  if (NODE__IS_ERR_(first)) return first;

  Node second = EVAL(cvector__index(cvector_nodes, 2), env);
  if (NODE__IS_ERR_(second)) return second;

  if (NODE__TYPE_(second) != NODE__INT || NODE__TYPE_(first) != NODE__INT) {
    Str str = str__new();
    str__nappend(&str, "TypeError: argument to '>=' must of type int");
    str__nappend(&str, " (got '");
    str__nappend(&str, maltypename_fromnode(NODE__TYPE_(first)));
    str__nappend(&str, "')");
    str__done(&str);
    return node_error__new(str);
  }

  if (NODE__INT_VAL_(first) >= NODE__INT_VAL_(second)) {
    return node_true__new();
  }

  return node_false__new();
}

Node core__less_equals(cvector_nodes_t* cvector_nodes, Env* env) {
  bool ok = core__arg_len_mustbe(cvector_nodes, 2);

  if (!ok) {
    Str str = str__new();
    str__nappend(&str, "TypeError: '<=' takes exactly 2 arguments (");
    str__intappend(&str, cvector__size(cvector_nodes) - 1);
    str__nappend(&str, " given)");
    str__done(&str);
    return node_error__new(str);
  }

  Node first = EVAL(cvector__index(cvector_nodes, 1), env);
  if (NODE__IS_ERR_(first)) return first;

  Node second = EVAL(cvector__index(cvector_nodes, 2), env);
  if (NODE__IS_ERR_(second)) return second;

  if (NODE__TYPE_(second) != NODE__INT || NODE__TYPE_(first) != NODE__INT) {
    Str str = str__new();
    str__nappend(&str, "TypeError: argument to '<=' must of type int");
    str__nappend(&str, " (got '");
    str__nappend(&str, maltypename_fromnode(NODE__TYPE_(first)));
    str__nappend(&str, "')");
    str__done(&str);
    return node_error__new(str);
  }

  if (NODE__INT_VAL_(first) <= NODE__INT_VAL_(second)) {
    return node_true__new();
  }

  return node_false__new();
}

Node core__more(cvector_nodes_t* cvector_nodes, Env* env) {
  bool ok = core__arg_len_mustbe(cvector_nodes, 2);

  if (!ok) {
    Str str = str__new();
    str__nappend(&str, "TypeError: '>' takes exactly 2 arguments (");
    str__intappend(&str, cvector__size(cvector_nodes) - 1);
    str__nappend(&str, " given)");
    str__done(&str);
    return node_error__new(str);
  }

  Node first = EVAL(cvector__index(cvector_nodes, 1), env);
  if (NODE__IS_ERR_(first)) return first;

  Node second = EVAL(cvector__index(cvector_nodes, 2), env);
  if (NODE__IS_ERR_(second)) return second;

  if (NODE__TYPE_(second) != NODE__INT || NODE__TYPE_(first) != NODE__INT) {
    Str str = str__new();
    str__nappend(&str, "TypeError: argument to '>' must of type int");
    str__nappend(&str, " (got '");
    str__nappend(&str, maltypename_fromnode(NODE__TYPE_(first)));
    str__nappend(&str, "')");
    str__done(&str);
    return node_error__new(str);
  }

  if (NODE__INT_VAL_(first) > NODE__INT_VAL_(second)) {
    return node_true__new();
  }

  return node_false__new();

}

Node core__less(cvector_nodes_t* cvector_nodes, Env* env) {
   bool ok = core__arg_len_mustbe(cvector_nodes, 2);

  if (!ok) {
    Str str = str__new();
    str__nappend(&str, "TypeError: '<' takes exactly 2 arguments (");
    str__intappend(&str, cvector__size(cvector_nodes) - 1);
    str__nappend(&str, " given)");
    str__done(&str);
    return node_error__new(str);
  }
  Node first = EVAL(cvector__index(cvector_nodes, 1), env);
  if (NODE__IS_ERR_(first)) return first;

  Node second = EVAL(cvector__index(cvector_nodes, 2), env);
  if (NODE__IS_ERR_(second)) return second;

  if (NODE__TYPE_(second) != NODE__INT || NODE__TYPE_(first) != NODE__INT) {
    Str str = str__new();
    str__nappend(&str, "TypeError: argument to '<' must of type int");
    str__nappend(&str, " (got '");
    str__nappend(&str, maltypename_fromnode(NODE__TYPE_(first)));
    str__nappend(&str, "')");
    str__done(&str);
    return node_error__new(str);
  }

  if (NODE__INT_VAL_(first) < NODE__INT_VAL_(second)) {
    return node_true__new();
  }

  return node_false__new();
}

Node core__function_closure(cvector_nodes_t* cvector_nodes, Env* env){
  /*if (!core__arg_len_mustbe(cvector_nodes, 2)) {*/
    /*Str str = str__new();*/
    /*str__nappend(&str, "ValueError: function closure must have binds and body");*/
    /*str__done(&str);*/
    /*return node_error__new(str);*/
  /*}*/
  return node_function_closure__new(env, cvector_nodes);
}

void core__setup_initial(Env *env, Core *core) {
  cdict_iterator_node_t iterator = core__ns_new_iterator(core);
  for (;;) {
    if (cdict_iterator__done(&iterator))
      break;
    Node value;
    Node key = cdict_iterator__next_keyval(&iterator, &value);
    env__set(env, key, value);
  }
}
