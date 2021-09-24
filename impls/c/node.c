#include <inttypes.h>
#include "node.h"
#include "eval.h"
#include "str.h"

Node node_vector__new(cvector_nodes_t* cvector_nodes) {
  Node node = {.nodetype=NODE__VECTOR, .nodeval = {.nodevector = {.mem=cvector_nodes}}};
  return node;
}

#define NODE__INT(node) ((node).nodeval.nodeint.val)


Node node_symbol_value__new(void *pointer) {
  Node node = {.nodetype = NODE__SYMBOL_VALUE,
               .nodeval = {.nodesymbolvalue = {.funcpointer = pointer}}};
  return node;
}


Node node_symbol__new(char* mem, size_t len) {
  return (Node){
    .nodetype = NODE__SYMBOL,
      .nodeval = {
        .nodesymbol = {
          .stringview = {
            .mem = mem,
            .len = len
          }
        }
      }
  };
}

Node node_comment__new(char* mem, size_t len) {
  return (Node) {
    .nodetype = NODE__COMMENT,
      .nodeval = {
        .nodecomment = {
          .stringview = {
            .mem = mem,
            .len = len
          }
        }
      }
  };
}

Node node_int__new(char* mem, size_t len) {
  // convert that thint to actual int
  char* endptr = mem+len-1;
  // converto to int
  intmax_t num = strtoimax(mem, &endptr, 10);
  Node node = {.nodetype=NODE__INT, .nodeval={.nodeint={.val=num}}};
  return node;
}

Node node_eof__new(void) {
  Node node = {.nodetype=NODE__EOF};
  return node;
}

Node node_error__new(Str string) {
  return (Node) {
    .nodetype=NODE__ERR,
      .nodeval = {
        .nodeerror = {
          .string=string
        }
      }
  };
}

Node node_nil__new() {
  return (Node) {
    .nodetype=NODE__NIL
  };
}

Node node_string__new(char* mem, size_t len) {
  Str str = str__new();
  size_t index = 0;
  while(index < len) {
    char ch = mem[index++];
    if (ch == '\\') {
      if (index >= len) {
        continue;
      }
      ch = mem[index];
      str__cappend(&str, ch);
      ++index;
      continue;
    }
    str__cappend(&str, ch);
  }
  return (Node){
    .nodetype = NODE__STRING,
      .nodeval = {
        .nodestring = {
          .str = str
        }
      }
  };
}
