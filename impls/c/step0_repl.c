#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

string get_input(FILE* file) {
  char* buffer = NULL;
  size_t buffer_cap;
  size_t len = getline(&buffer, &buffer_cap, file);
  buffer[len-1] = 0; // strip new line
  return buffer;
}

int main() {
  for(;;) {
    printf("user> ");
    string input = get_input(stdin);
    if (strlen(input))  {
      printf("%s\n", rep(input));
    }
    if (input) {
      free(input);
    }
  }
  return 0;
}
