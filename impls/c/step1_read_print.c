#define _POSIX_C_SOURCE  200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "reader.h"


typedef char* string;

AST_t* READ(string expr) {
  return read_str(expr);
}

AST_t* EVAL(AST_t* ast) {
  return ast;
}

void print_symbol(Node_symbol_t symbol) {
  char buffer[symbol.len+1];
  for (size_t i = 0; i < symbol.len; i++) {
    buffer[i] = symbol.memptr[i];
  }
  buffer[symbol.len] = 0;
  printf("%s", buffer);
}

void debug_symbol(Node_t node) {
  print_symbol(node.node_val.node_symbol);
}
void debug(AST_t* ast) {
  // just
  cvector_nodes_t nodes = ast -> cvector_nodes;
  cvector_nodes_iterator_t iterator;
  cvector_iterator__init(&iterator, &nodes);

  for(;;) {
    if (cvector_iterator__done(&iterator)) break;
    Node_t node = cvector_iterator__next(&iterator);
    if (node.node_type == NODE__SYMBOL) {
      debug_symbol(node);
    }
  }

  if(ast -> next) {
    // if that is the case
    printf("(");
    debug(ast -> next);
    printf(")");
  }




}

char* PRINT(AST_t* ast) {
  // debug AST
  debug(ast);
  return "";
}

string rep(string expr) {
  return PRINT(EVAL(READ(expr)));
}

typedef struct {
  string value;
  bool ok;
  bool is_eof;
} string_result_t;

string_result_t string_result__new(string value, bool ok, bool is_eof) {
  return ((string_result_t){.value=value, .ok=ok, .is_eof=is_eof});
}

void strip_new_line_if_exists(char* buffer, size_t len) {
  char ch = ((buffer)[len-1]);
  if (ch == '\n') {
    buffer[len-1] = 0;
  }
}

string_result_t get_input(FILE* file) {
  string buffer = NULL;
  size_t buffer_cap;
  size_t len = getline(&buffer, &buffer_cap, file);
  if (len == -1) {
    // EOF
    if (errno == 0) {
      return string_result__new(buffer, false, true);
    }
    return string_result__new(buffer, false, false);
  }
  // strips out new line character if exists
  strip_new_line_if_exists(buffer, len);
  return string_result__new(buffer, true, false);
}

bool string_result__iseof(string_result_t* result) {
  return result -> is_eof;
}

bool string_result__ok(string_result_t* result) {
  return result -> ok;
}


string string_result__val(string_result_t* result) {
  return result -> value;
}

void string_result__free(string_result_t* result) {
  free(result -> value);
}

size_t string_result__len(string_result_t* result) {
  return strlen(result -> value);
}

// (  ) -> node in a tree and everything else is a lead in the treejJ;w
// k
//
int main(void) {
  for(;;) {
    printf("user> ");
    string_result_t result = get_input(stdin);

    if (string_result__iseof(&result)) {
      string_result__free(&result);
      return 0;
    }

    if (!string_result__ok(&result)) {
      string_result__free(&result);
      perror("unable to read line");
      return 1;
    }

    if (!string_result__len(&result)) {
      string_result__free(&result);
      continue;
    }

    AST_t* read_result = READ(string_result__val(&result));
    AST_t* eval_result = EVAL(read_result);
    string print_result = PRINT(eval_result);

    printf("%s\n", print_result);

    string_result__free(&result);
  }
}
