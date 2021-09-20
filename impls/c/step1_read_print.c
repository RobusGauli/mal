#define _POSIX_C_SOURCE  200809L

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "reader.h"
#include "debug.h"

Node READ(char* expr) {
  return read_str(expr);
}

Node EVAL(Node node) {
  return node;
}

char* PRINT(Node node) {
  debug(node);
  return "";
}

int main(void) {
  for(;;) {
    char* input = readline("user> ");

    if (input == NULL) {
      exit(1);
    }

    Node node = READ(input);
    Node evaluated_node = EVAL(node);
    char* result = PRINT(evaluated_node);

    printf("%s\n", result);
  }

}
