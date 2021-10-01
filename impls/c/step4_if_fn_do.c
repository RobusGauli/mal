#define _POSIX_C_SOURCE 200809L
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
} Mal;

typedef enum {
  token_number,
  token_string,
  token_symbol,
  token_left_paren,
} e_toktype;

const char *toktype_dbg(e_toktype toktype) {
  switch (toktype) {
  case token_number:
    return "token_number";
  case token_string:
    return "token_string";
  case token_symbol:
    return "token_symbol";
  case token_left_paren:
    return "token_left_paren";
  }
}
// array of struct with token type, token buffer & token length
typedef struct {
  e_toktype type;
  const char *buffer;
  size_t len;
} t_token;

t_token *shift(char **input) {
  t_token *token = malloc(sizeof(t_token));
  token->buffer = NULL;

  if ((**input) == '(') {
    token->buffer = *input;
    token->type = token_left_paren;
    token->len = 1;
    (*input)++;
  }
  return token;
}
// this thing need a pointer to char
Mal m_read(char *input) {
  t_token *token = shift(&input);
  printf("token type: %s\n", toktype_dbg(token -> type ));
  return (Mal){};
}

Mal m_eval(Mal mal) { return (Mal){}; }

char *m_print(Mal mal) {
  char *src = "echo world";
  char *sample_string = malloc((sizeof(char) * strlen(src)) + (sizeof(char)));
  memcpy(sample_string, src, strlen(src));
  sample_string[strlen(src)] = '\0';
  return sample_string;
}

int main() {

  for (;;) {
    char *input = readline("user>");

    if (input == NULL) {
      exit(0);
    };

    Mal mal = m_read(input);
    Mal result = m_eval(mal);
    char *stringified = m_print(result);
    printf("%s\n", stringified);
    free(stringified);
    free(input);
  }
}
