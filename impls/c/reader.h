#ifndef READER_H
#define READER_H

#include "deps/cvector/cvector.h"
#include "token.h"

typedef struct {
  cvector_tokens_t cvector_tokens;
  cvector_iterator_tokens_t cvector_iterator_tokens_t;
} Reader;

typedef enum {
  NODE__INT,
  NODE__STRING,
  NODE__COMMENT,
  NODE__SYMBOL,
  NODE__SYMBOL_VALUE,
  NODE__VECTOR,
  NODE__EMPTY,
  NODE__EOF
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

CVector(char) cvector_chars_t;

typedef struct {
  cvector_chars_t cvector_chars;
  size_t len;
} NodeString;

typedef struct {
  void *mem;
} NodeVector;

typedef struct {
  NodeType nodetype;
  union {
    NodeInt nodeint;
    NodeSymbol nodesymbol;
    NodeSymbolValue nodesymbolvalue;
    NodeVector nodevector;
    NodeString nodestring;
    NodeComment nodecomment;
  } nodeval;
} Node;

CVector(Node) cvector_nodes_t;
CVector_iterator(cvector_nodes_t) cvector_iterator_nodes_t;

Node read_str(char *expr);
Node read_list(cvector_iterator_tokens_t *cvector_iterator);
Node read_form(cvector_iterator_tokens_t *cvector_iterator);
Node node_vector__new(cvector_nodes_t *cvector_nodes);
Node node_symbol__new(char *mem, size_t len);
Node node_comment__new(char *mem, size_t len);
Node node_int__new(char*mem, size_t len);
Node node_eof__new(void);
Node node_string__new(char* mem, size_t len);
Node node_vector__new(cvector_nodes_t* cvector_nodes);
#endif
