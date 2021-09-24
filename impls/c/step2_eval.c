#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "deps/cdict.h/cdict.h"
#include "reader.h"

#include "env.h"
#include "eval.h"
#include "node.h"


char *PRINT(Node node) {
  debug(node);
  return "";
}

int main(void) {

  for (;;) {
    char *input = readline("user> ");
    if (input == NULL) { // EOF
      exit(0);
    }
    // create environment
    cdict_node_func_t cdict_node_func;
    cdict__init(&cdict_node_func);
    cdict__set_comparator(&cdict_node_func, node_comparator);
    cdict__set_hash(&cdict_node_func, node_hasher);
    // setup environment
    setup_environ(&cdict_node_func);
    // repl
    Node node = READ(input);
    Node evaluated_node = EVAL(node, &cdict_node_func);
    char *result = PRINT(evaluated_node);
    printf("%s\n", result);
  }
}
