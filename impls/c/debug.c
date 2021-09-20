#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void debug_symbol(Node node) {
  size_t len = node.nodeval.nodesymbol.len;
  char *mem = node.nodeval.nodesymbol.mem;
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
    Node node = cvector_iterator__next(&cvector_iterator_nodes);
    debug(node);
  }

  printf(")");
}

void debug(Node node) {
  switch (node.nodetype) {
  case NODE__VECTOR: {

    debug_vector(node);
    break;
  }

  case NODE__SYMBOL: {
    debug_symbol(node);
    break;
  }

  case NODE__COMMENT: {
    printf("thi is comment");
    break;
  }

  case NODE__INT: {

    printf("this is int");
    break;
  }
  default:
    printf("unreachable");
  }
}
