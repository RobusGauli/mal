#ifndef READER_H
#define READER_H

#include "deps/cvector/cvector.h"

enum Tokentype {
  TOKEN__COMMENT,
  TOKEN__SYMBOL,
  TOKEN__NUMBER,
  TOKEN__STRING,
  TOKEN__SEMICOLON,
  TOKEN__OTHER,
  TOKEN__TILDA_AT,
  TOKEN__AT,
  TOKEN__LEFT_SQ_BR,
  TOKEN__RIGHT_SQ_BR,
  TOKEN__LEFT_CURLY_BR,
  TOKEN__RIGHT_CURLY_BR,
  TOKEN__LEFT_PAREN,
  TOKEN__RIGHT_PAREN,
  TOKEN__APOSTROPHE,
  TOKEN__WEIRD,
  TOKEN__TILDA,
  TOKEN__CARRAT,
};

typedef struct Token {
  enum Tokentype tokentype;
  char *mem;
  size_t len;
} Token;

static Token token__new(enum Tokentype tokentype) {
  return (Token){.tokentype = tokentype};
}

static Token token__new_with_val(enum Tokentype tokentype, char *mem,
                                 size_t len) {
  return (Token){.tokentype = tokentype, .mem = mem, .len = len};
}

CVector(Token) cvector_tokens_t;

CVector_iterator(cvector_tokens_t) cvector_iterator_tokens_t;

typedef struct {
  cvector_tokens_t cvector_tokens;
  cvector_iterator_tokens_t cvector_iterator_tokens_t;
} Reader;

typedef enum NodeType {
  NODE__INT = 0,
  NODE__STRING, // 1
  NODE__COMMENT, //2
  NODE__SYMBOL, // 3
  NODE__SYMBOL_VALUE, // 4
  NODE__VECTOR, // 5
  NODE__EMPTY, // 6
  NODE__EOF // 7
} NodeType;

typedef struct NodeInt {
  int val;
} NodeInt;

typedef struct NodeSymbolValue {
  void *funcpointer;
} NodeSymbolValue;

typedef struct NodeSymbol {
  char *mem;
  size_t len;
} NodeSymbol;

typedef struct NodeComment {
  char *mem;
  size_t len;
} NodeComment;

CVector(char) cvector_chars_t;

typedef struct NodeString {
  cvector_chars_t cvector_chars;
  size_t len;
} NodeString;

typedef struct NodeVector {
  void *mem;
} NodeVector;

typedef struct Node {
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
#endif
