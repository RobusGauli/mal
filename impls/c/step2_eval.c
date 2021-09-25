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
#include "str.h"

Str PRINT(Node node) { return debug(node); }

int main(void) {

  Env env = env__new();
  env__setup_initial(&env);

  for (;;) {
    char *input = readline("user> ");
    if (input == NULL) { // EOF
      exit(0);
    }

    Node node = READ(input);
    Node evaluated_node = EVAL(node, env);
    Str result = PRINT(evaluated_node);
    if (str__isempty(&result)) {
      continue;
    }
    printf("%s\n", str__ascstr(&result));
  }
}
