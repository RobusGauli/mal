#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef char* string;

string READ(string expr) {
  return expr;
}

string EVAL(string expr) {
  return expr;
}

string PRINT(string expr) {
  return expr;
}

string rep(string expr) {
  return PRINT(EVAL(READ(expr)));
}

typedef struct {
  string value;
  bool ok;
} string_result_t;

string_result_t string_result__new(string value, bool ok) {
  return ((string_result_t){.value=value, .ok=ok});
}

void strip_new_lines_if_exists(char* buffer, size_t len) {
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
    return string_result__new(buffer, false);
  }
  // strips out new line character if exists
  strip_new_lines_if_exists(buffer, len);
  return string_result__new(buffer, true);
}

int main(void) {
  for(;;) {
    printf("user> ");
    string_result_t result__input = get_input(stdin);

    if (!result__input.ok) {
      goto cleanup;
    }

    printf("%s\n",  result__input.value);

    free(result__input.value);
    continue;

cleanup:
    free(result__input.value);
    break;
  }

}
