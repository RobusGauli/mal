#ifndef NODE_H
#define NODE_H

#include "deps/cvector/cvector.h"
#include "deps/cdict.h/cdict.h"
#include "stringview.h"
#include "str.h"

typedef enum {
  NODE__NIL,
  NODE__INT,
  NODE__TRUE,
  NODE__FALSE,
  NODE__STRING,
  NODE__COMMENT,
  NODE__SYMBOL,
  NODE__VECTOR,
  NODE__EMPTY,
  NODE__EOF,
  NODE__ERR,
  NODE__LIST,
  NODE__SYMBOL_VALUE,
  NODE__FUNCTION_CLOSURE,
} NodeType;

typedef struct {
  int val;
} NodeInt;

// constructor
#define NODE__INT_(node) ((node).nodeval.nodeint)
#define NODE__INT_VAL_(node) (NODE__INT_(node).val)

typedef struct {
  void *funcpointer;
} NodeSymbolValue;

#define NODE__SYMBOLVALUE_(node) ((node).nodeval.nodesymbolvalue)
#define NODE__SYMBOLVALUE_FUNCPTR_(node) (NODE__SYMBOLVALUE_(node).funcpointer)

typedef enum {
  NODESYMBOL__NORM_FORM,
  NODESYMBOL__SPECIAL_LET_FORM,
  NODESYMBOL__SPECIAL_DEF_FORM,
  NODESYMBOL__SPECIAL_FN_FORM,
} NodeSymbolType;


typedef struct {
  StringView stringview;
  NodeSymbolType nodesymboltype;
} NodeSymbol;

#define NODE__SYMBOL_(node) ((node).nodeval.nodesymbol)

#define NODE__SYMBOL_STRINGVIEW_MEM_(node) ((NODE__SYMBOL_(node)).stringview.mem)
#define NODE__SYMBOL_STRINGVIEW_LEN_(node) ((NODE__SYMBOL_(node)).stringview.len)
#define NODE__SYMBOL_TYPE_(node) ((NODE__SYMBOL_(node)).nodesymboltype)

typedef struct {
  StringView stringview;
} NodeComment;

#define NODE__COMMENT_(node) ((node).nodeval.nodecomment)
#define NODE__COMMENT_STRINGVIEW_MEM_(node) ((NODE__COMMENT_(node)).stringview.mem)
#define NODE_COMMENT_STRINGVIEW_LEN_(node) ((NODE__COMMENT_(node)).strinview.len)

typedef struct {
  Str str;
} NodeString;

#define NODE__STRING_(node) ((node).nodeval.nodestring)
#define NODE__STRING_STR_(node) ((NODE__STRING_(node)).str)

typedef struct {
  void *mem;
} NodeVector;

#define NODE__VECTOR_(node) ((node).nodeval.nodevector)
#define NODE__VECTOR_MEM_(node) ((NODE__VECTOR_(node)).mem)

typedef struct {
  Str string;
} NodeError;

#define NODE__ERROR_(node) ((node).nodeval.nodeerror)
#define NODE__ERROR_STRING_(node) ((NODE__ERROR_(node)).string)

typedef struct {
  void* mem;
} NodeList;


#define NODE__LIST_(node) ((node).nodeval.nodelist)
#define NODE__LIST_MEM_(node) ((NODE__LIST_(node)).mem)


typedef struct {
  void* env;
  void* cvector_nodes; // just pointer to vec
} NodeFunctionClosure;

#define NODE__FUNCTION_CLOSURE_(node) ((node).nodeval.nodefunctionclosure)
#define NODE__FUNCTION_ENV_(node) (NODE__FUNCTION_CLOSURE_(node).env)
#define NODE__FUNCTION_CVECTOR_(node) (NODE__FUNCTION_CLOSURE_(node).cvector_nodes)

typedef struct Node {
  NodeType nodetype;
  union {
    NodeInt nodeint;
    NodeList nodelist;
    NodeError nodeerror;
    NodeSymbol nodesymbol;
    NodeVector nodevector;
    NodeString nodestring;
    NodeComment nodecomment;
    NodeSymbolValue nodesymbolvalue;
    NodeFunctionClosure nodefunctionclosure;
  } nodeval;
} Node;

#define NODE__TYPE_(node) ((node).nodetype)

CVector(Node) cvector_nodes_t;
CVector_iterator(cvector_nodes_t) cvector_iterator_nodes_t;

Node node_vector__new(cvector_nodes_t *cvector_nodes);
Node node_nil__new(void);
Node node_symbol__new(char *mem, size_t len, NodeSymbolType nodesymboltype);

Node node_comment__new(char *mem, size_t len);
Node node_int__new(char*mem, size_t len);
Node node_true__new(void);
Node node_false__new(void);
Node node_eof__new(void);
Node node_string__new(char* mem, size_t len);
Node node_error__new(Str string);
Node node_vector__new(cvector_nodes_t* cvector_nodes);
Node node_function_closure__new(void* env, cvector_nodes_t* cvector_nodes);
Node node_list__new(cvector_nodes_t* cvector_nodes);

const char* maltypename_fromnode(NodeType nodetype);

// might want to delete these
Node node__empty_list(void);

CDict(Node, Node) cdict_node_t;
CDict_iterator(cdict_node_t) cdict_iterator_node_t;

bool node_comparator(Node *self, Node *other);

cdict__u64 node_hasher(Node *self, cdict__u64 (*hash)(void *, size_t));
//
// helper macro
#define NODE__IS_ERR_(node) ((node).nodetype == NODE__ERR)
#define NODE__IS_FALSE_(node) ((node).nodetype == NODE__FALSE)
#define NODE__IS_NIL_(node) ((node).nodetype == NODE__NIL)
#define NODE_SYMBOL__NEW_(_mem, _len)                                          \
  ({                                                                           \
    (Node){.nodetype = NODE__SYMBOL,                                           \
           .nodeval = {                                                        \
               .nodesymbol = {.stringview = {.mem = (_mem), .len = (_len)},    \
                              .nodesymboltype = NODESYMBOL__NORM_FORM}}};      \
  })


// from natives
Node nodeint__new(int val);
#endif
