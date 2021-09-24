#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "debug.h"


void debug_symbol(Node node) {
  size_t len = node.nodeval.nodesymbol.stringview.len;
  char *mem = node.nodeval.nodesymbol.stringview.mem;
  char buffer[len + 1];
  memcpy(buffer, mem, len);
  buffer[len] = 0;
  printf("%s", buffer);
}

void debug_vector(Node node) {
  cvector_nodes_t *cvector_nodes = node.nodeval.nodevector.mem;

  cvector_iterator_nodes_t cvector_iterator_nodes;
  cvector_iterator__init(&cvector_iterator_nodes, cvector_nodes);

  printf("(");

  for (;;) {
    if (cvector_iterator__done(&cvector_iterator_nodes))
      break;

    if (cvector_iterator_nodes.cvector_iterator__current_index_m != 0) {
      printf(" ");
    }

    Node node = cvector_iterator__next(&cvector_iterator_nodes);
    debug(node);
  }

  printf(")");
}

void debug_comment(Node node) {
  size_t len = node.nodeval.nodecomment.stringview.len;
  char *mem = node.nodeval.nodecomment.stringview.mem;
  char buffer[len + 1];
  memcpy(buffer, mem, len);
  buffer[len] = 0;
  printf("%s", buffer);
}

void debug_string(Node node) {
  printf("\"%s\"", str__ascstr(&node.nodeval.nodestring.str));
}

void debug_eof(Node node) { printf("EOF ,"); }

void debug_int(Node node) { printf("%d", node.nodeval.nodeint.val); }

void debug_error(Node node) {
  const char* msg = str__ascstr(&node.nodeval.nodeerror.string);
  printf("%s\n", msg);
}

void debug(Node node) {
  switch (node.nodetype) {
  case NODE__NIL:
    printf("()");
    break;
  case NODE__VECTOR: {
    debug_vector(node);
    break;
  }

  case NODE__SYMBOL: {
    debug_symbol(node);
    break;
  }

  case NODE__COMMENT: {
    debug_comment(node);
    break;
  }

  case NODE__INT: {
    debug_int(node);
    break;
  }
  case NODE__STRING: {
    debug_string(node);

    break;
  }
  case NODE__EOF: {
    debug_eof(node);
    break;
  }

  case NODE__EMPTY: {
    break;
  }

 case NODE__ERR:
            debug_error(node);
            break;

  default: { printf("unreachable"); }
  }
}
