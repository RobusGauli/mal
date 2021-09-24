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

typedef struct {
  void *funcpointer;
} NodeSymbolValue;

typedef struct {
  StringView stringview;
} NodeSymbol;

typedef struct {
  StringView stringview;
} NodeComment;

typedef struct {
  Str str;
} NodeString;

typedef struct {
  void *mem;
} NodeVector;

typedef struct {
  Str string;
} NodeError;

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

CVector(Node) cvector_nodes_t;
CVector_iterator(cvector_nodes_t) cvector_iterator_nodes_t;

Node node_vector__new(cvector_nodes_t *cvector_nodes);
Node node_nil__new(void);
Node node_symbol__new(char *mem, size_t len);
Node node_comment__new(char *mem, size_t len);
Node node_int__new(char*mem, size_t len);
Node node_eof__new(void);
Node node_string__new(char* mem, size_t len);
Node node_error__new(Str string);
Node node_vector__new(cvector_nodes_t* cvector_nodes);

#endif
