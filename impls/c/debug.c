#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "str.h"

Str debug__symbol(Node node) {
  Str result = str__new();
  str__append(&result, NODE__SYMBOL_STRINGVIEW_MEM_(node),
              NODE__SYMBOL_STRINGVIEW_LEN_(node));
  str__done(&result);
  return result;
}

Str debug__vector(Node node) {
  Str result = str__new();

  cvector_nodes_t *cvector_nodes = NODE__VECTOR_MEM_(node);

  cvector_iterator_nodes_t cvector_iterator_nodes;
  cvector_iterator__init(&cvector_iterator_nodes, cvector_nodes);

  str__cappend(&result, '(');

  for (;;) {
    if (cvector_iterator__done(&cvector_iterator_nodes))
      break;

    if (cvector_iterator_nodes.cvector_iterator__current_index_m != 0) {
      str__cappend(&result, ' ');
    }

    Node node = cvector_iterator__next(&cvector_iterator_nodes);
    Str node_result = debug(node);
    str__add(&result, &node_result);
  }

  str__cappend(&result, ')');
  str__done(&result);
  return result;
}

Str debug__comment(Node node) {
  Str result = str__new();
  str__append(&result, NODE__SYMBOL_STRINGVIEW_MEM_(node),
              NODE__SYMBOL_STRINGVIEW_LEN_(node));
  str__done(&result);
  return result;
}

Str debug__string(Node node) { return str__dclone(&(NODE__STRING_STR_(node))); }

size_t digits_count(int val) {
  // domain error if the value is 0
  if (val == 0) {
    return 1;
  }
  return (int)(log10((double)(val)) + 1);
}

Str debug__int(Node node) {
  // convert it to string
  int val = NODE__INT_VAL_(node);
  Str result = str__new();
  if (val & 1 << 31) { // check if it is neg
    val *= -1;
    str__cappend(&result, '-');
  }

  size_t count = digits_count(val);

  while (count >= 1) {
    int num = (int)(val / pow(10, count - 1));
    str__cappend(&result, '0' + num);
    val = (val - ((num) * (pow(10, count - 1))));
    count--;
  }
  // null terminator
  str__done(&result);

  return result;
}

Str debug__error(Node node) {
  return str__dclone(&(NODE__ERROR_STRING_(node)));
}

Str debug__eof(Node node) {
  Str result = str__new();
  str__nappend(&result, "EOF");
  str__done(&result);
  return result;
}

Str debug__nil(Node node) {
  Str result = str__new();
  str__nappend(&result, "()");
  str__done(&result);
  return result;
}

Str debug__empty(Node node) {
  Str result = str__new();
  str__done(&result);
  return result;
}

Str debug__symbolvalue(Node node) {
  Str result = str__new();
  str__nappend(&result, "special function");
  str__done(&result);
  return result;
}

Str debug(Node node) {
  switch (node.nodetype) {
  case NODE__NIL:
    return debug__nil(node);

  case NODE__VECTOR: {
    return debug__vector(node);
  }

  case NODE__SYMBOL_VALUE:
    return debug__symbolvalue(node);

  case NODE__SYMBOL: {
    return debug__symbol(node);
  }

  case NODE__COMMENT: {
    return debug__comment(node);
  }

  case NODE__INT: {
    return debug__int(node);
    break;
  }
  case NODE__STRING: {
    return debug__string(node);
  }
  case NODE__EOF: {
    return debug__eof(node);
  }

  case NODE__EMPTY: {
    return debug__empty(node);
    break;
  }

  case NODE__ERR:
    return debug__error(node);

  default: { printf("unreachable"); }
  }
}
