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

    if (cvector_iterator_nodes.cvector_iterator__current_index_m != 0) {
      printf(" ");
    }

    Node node = cvector_iterator__next(&cvector_iterator_nodes);
    debug(node);
  }

  printf(")");
}

void debug_comment(Node node) {
  size_t len = node.nodeval.nodecomment.len;
  char* mem = node.nodeval.nodecomment.mem;
  char buffer[len+1];
  memcpy(buffer, mem, len);
  buffer[len] = 0;
  printf("%s", buffer);
}

CVector(char) cvector_chars_t;
void debug_string(Node node) {
  size_t len = node.nodeval.nodestring.len;
  char* mem = node.nodeval.nodestring.mem;
  cvector_chars_t cvector_chars;
  cvector__init(&cvector_chars);
  for(size_t i = 0; i < len; ++i) {
    if (mem[i] == '\\') {
      // that means we need to avoid it
      continue;
    }
    cvector__add(&cvector_chars, mem[i]);
  }
  cvector__add(&cvector_chars, 0);
  printf("\"%s\"", cvector__wrapped_buffer(&cvector_chars));
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
    debug_comment(node);
    break;
  }

  case NODE__INT: {

    printf("this is int");
    break;
  }
  case NODE__STRING: {
     debug_string(node);
       break;
 }
  default: {

      printf("unreachable");
           }
  }
}
