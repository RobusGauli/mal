#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdint.h>
#include <stdio.h>

#include "deps/cvector/cvector.h"
#include "mal.h"
#include "token.h"

t_cvector_mal *as_cvector_mal(Mal *mal) {
  return (t_cvector_mal *)(mal->value);
}

int as_number(Mal *mal) { return (int)(mal->value); }

char *as_string(Mal *mal) { return (char *)(mal->value); }

char *as_symbol(Mal *mal) { return (char *)(mal->value); }

Mal *read_atom(token_t *token) {
  Mal *atom = malloc(sizeof(Mal));

  switch (token->token_kind) {
  case token_kind_number: {
    int number = 0;
    for (size_t i = 0; i < token->len; ++i) {
      number = number * 10 + ((token->buffer)[i] - '0');
    }
    atom->type = mal_number;
    atom->value = number;
    return atom;
  }

  case token_kind_string: {
    // string
    char *string = malloc((sizeof(char) * token->len) + sizeof(char));
    memcpy(string, token->buffer, token->len);
    string[token->len] = '\0';
    atom->type = mal_string;
    atom->value = (uint64_t)string;
    return atom;
  }

  case token_kind_symbol: {
    char *string = malloc((sizeof(char) * token->len) + sizeof(char));
    memcpy(string, token->buffer, token->len);
    string[token->len] = '\0';
    atom->type = mal_symbol;
    atom->value = (uint64_t)(string);
    return atom;
  }

  case token_kind_special_chr: {
    if (strncmp("(", token->buffer, 1) == 0) {
      return read_list(token);
    }
    char *string = malloc((sizeof(char) * token->len) + sizeof(char));
    memcpy(string, token->buffer, token->len);
    string[token->len] = '\0';
    atom->type = mal_symbol;
    atom->value = (uint64_t)(string);
    return atom;
  }

  case token_kind_eol: {
    atom->type = mal_error;
    return atom;
  }

  case token_kind_unreachable:
    assert(0 && "unreachable");
  }
}

Mal *read_list(token_t *token) { return NULL; }

Mal *m_read(char *input) {
  tokens_t toks = tokens(input);
  tokens_iterator_t iter = tokens_iterator(&toks);

  while (!cvector_iterator__done(&iter)) {
    token_t *token = cvector_iterator__peek(&iter);
    printf("%s -> %.*s\n", token_kind_name(token), (int)token->len,
           token->buffer);
    cvector_iterator__next(&iter);
  }
  return NULL;
}

Mal *m_eval(Mal *mal) { return mal; }

void debug(Mal *mal) {
  if (!mal) {
    return;
  }

  switch (mal->type) {
  case mal_string: {
    char *string = as_string(mal);
    printf("%s", string);
    break;
  }

  case mal_number: {
    int val = as_number(mal);
    printf("%d", val);
    break;
  }

  case mal_list: {
    printf("(");
    t_cvector_mal *cvector = as_cvector_mal(mal);
    t_cvector_mal_iterator iterator;
    cvector_iterator__init(&iterator, cvector);
    for (;;) {
      if (cvector_iterator__done(&iterator))
        break;
      Mal *next = cvector_iterator__next(&iterator);
      debug(next);
    }
    printf(")");
    break;
  }

  case mal_symbol: {
    printf("%s", as_symbol(mal));
    break;
  }

  case mal_error:
    printf("error");
    break;
  }
}

char *m_print(Mal *mal) {
  debug(mal);
  return NULL;
}

// as ptr

int main() {

  // as pointer
  for (;;) {
    char *input = readline("user> ");

    if (input == NULL) {
      exit(0);
    };

    Mal *mal = m_read(input);
    free(input);
  }
}
