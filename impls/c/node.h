#ifndef NODE_H
#define NODE_H

#include "deps/cvector/cvector.h"
#include "stringview.h"
#include "str.h"

typedef enum {
  NODE__NIL,
  NODE__INT,
  NODE__STRING,
  NODE__COMMENT,
  NODE__SYMBOL,
  NODE__SYMBOL_VALUE,
  NODE__VECTOR,
  NODE__EMPTY,
  NODE__EOF,
  NODE__ERR,
} NodeType;

typedef struct {
  int val;
} NodeInt;

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

typedef struct Node {
  NodeType nodetype;
  union {
    NodeInt nodeint;
    NodeSymbol nodesymbol;
    NodeSymbolValue nodesymbolvalue;
    NodeVector nodevector;
    NodeString nodestring;
    NodeComment nodecomment;
    NodeError nodeerror;
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
Node node_eof__new(void);
Node node_string__new(char* mem, size_t len);
Node node_error__new(Str string);
Node node_vector__new(cvector_nodes_t* cvector_nodes);

// helper macro
#define NODE__IS_ERR_(node) ((node).nodetype == NODE__ERR)
#define NODE_SYMBOL__NEW_(_mem, _len)                                          \
  ({                                                                           \
    (Node){.nodetype = NODE__SYMBOL,                                           \
           .nodeval = {                                                        \
               .nodesymbol = {.stringview = {.mem = (_mem), .len = (_len)},    \
                              .nodesymboltype = NODESYMBOL__NORM_FORM}}};      \
  })


#endif
