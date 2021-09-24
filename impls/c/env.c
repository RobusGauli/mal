#include "deps/cdict.h/cdict.h"

#include "env.h"
#include "eval.h"
#include "reader.h"
#include "node.h"

Node env__sum(Node a, Node b) {
  return (Node){
    .nodetype=NODE__INT,
      .nodeval={
        .nodeint={
          .val = (NODE__INT(a) + NODE__INT(b)) }}
  };
}

Node env__minus(Node a, Node b) {
  return (Node) {
    .nodetype = NODE__INT,
      .nodeval = {
        .nodeint = {
          .val = (NODE__INT(a) - NODE__INT(b))
        }
      }
    };
}

Node env__mul(Node a, Node b) {
   return (Node) {
    .nodetype = NODE__INT,
      .nodeval = {
        .nodeint = {
          .val = (NODE__INT(a) * NODE__INT(b))
        }
      }
    };
}

Node env__div(Node a, Node b) {
   return (Node) {
    .nodetype = NODE__INT,
      .nodeval = {
        .nodeint = {
          .val = (NODE__INT(a) / NODE__INT(b))
        }
      }
    };

}

void setup_environ(cdict_node_func_t *cdict_node_func) {
  cdict__add(cdict_node_func, node_symbol__new("+", 1), env__sum);
  cdict__add(cdict_node_func, node_symbol__new("-", 1), env__minus);
  cdict__add(cdict_node_func, node_symbol__new("*", 1), env__mul);
  cdict__add(cdict_node_func, node_symbol__new("/", 1), env__div);
}

bool node_comparator(Node *self, Node *other) {
  return memcmp(self->nodeval.nodesymbol.stringview.mem,
                other->nodeval.nodesymbol.stringview.mem,
                self->nodeval.nodesymbol.stringview.len) == 0;
}

cdict__u64 node_hasher(Node *self, cdict__u64 (*hash)(void *, size_t)) {
  cdict__u64 value = hash(self->nodeval.nodesymbol.stringview.mem,
                          self->nodeval.nodesymbol.stringview.len);
  return value;
}
