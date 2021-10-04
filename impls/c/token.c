#include <assert.h>
#include <stdbool.h>

#include "token.h"

const char *token_kind_name(token_t *token) {
  switch (token->token_kind) {
  case token_kind_number:
    return "token_kind_number";
  case token_kind_string:
    return "token_kind_string";
  case token_kind_symbol:
    return "token_kind_symbol";
  case token_kind_special_chr:
    return "token_kind_special_chr";
  case token_kind_eol:
    return "token_kind_eol";
  case token_kind_unreachable:
    return "token_kind_unreachable";
  }
}

token_t *token_next(char **input) {

  if (**input == '\0') {
    return NULL;
  }

  for (;;) {

    if (**input == '\0') {
      return NULL;
    }

    if ((**input) == ' ' || (**input) == ',') {
      (*input)++;
      continue;
    }

    break;
  }

  if (**input == ';') {
    for (;;) {
      if (**input == '\n' || **input == '\0') {
        break;
      }
      (*input)++;
    }

    if ((**input) == '\0')
      return NULL;
  }

  assert(**input != '\0');

  if ((**input) == '~') {
    char _next = (*(*input + 1));
    if (_next == '@') {
      token_t *token = malloc(sizeof(token_t));
      token->buffer = *input;
      token->token_kind = token_kind_special_chr;
      token->len = 2;
      (*input) += 2;
      return token;
    }
  }

  assert(**input != '\0');

  if ((**input) == '~' || (**input) == '@' || (**input) == '^' ||
      (**input) == '[' || (**input) == ']' || (**input) == '^' ||
      (**input) == '(' || (**input) == ')' || (**input) == '}' ||
      (**input) == '{' || (**input) == '\'' || (**input) == '`') {
    token_t *token = malloc(sizeof(token_t));
    token->buffer = *input;
    token->token_kind = token_kind_special_chr;
    token->len = 1;
    (*input)++;

    return token;
  }

  if ((**input) == '"') {
    char *start = *input;
    (*input)++;

    for (;;) {
      if (**input == '\n' || **input == '\0') {
        token_t *token = malloc(sizeof(token_t));
        token->buffer = start;
        token->token_kind = token_kind_eol;
        token->len = ((*input) - start);
        return token;
      }

      if (**input == '"') {
        token_t *token = malloc(sizeof(token_t));
        token->buffer = start;
        token->token_kind = token_kind_string;
        token->len = ((*input) - start + 1);
        (*input)++;
        return token;
      }

      (*input)++;
    }
  }

  bool num = true;
  char *_start = *input;
  for (;;) {
    if ((**input) == '~' || (**input) == '@' || (**input) == '^' ||
        (**input) == '[' || (**input) == ']' || (**input) == '^' ||
        (**input) == '(' || (**input) == ')' || (**input) == '}' ||
        (**input) == '{' || (**input) == '\'' || (**input) == '`' ||
        **input == '\n' || **input == '\0' || **input == ' ') {
      break;
    }
    if (**input < '0' || **input > '9') {
      num = false;
    }
    (*input)++;
  }


  token_t *token = malloc(sizeof(token_t));
  token->buffer = _start;
  token->token_kind = num ? token_kind_number : token_kind_symbol;
  token->len = (*input - _start);

  return token;
}

tokens_t tokens(char *input) {
  tokens_t tokens;
  cvector__init(&tokens);
  for (;;) {
    token_t *tok = token_next(&input);
    if (!tok)
      break;
    cvector__add(&tokens, tok);
  }
  return tokens;
}

tokens_iterator_t tokens_iterator(tokens_t *tokens) {
  tokens_iterator_t tokens_iterator;
  cvector_iterator__init(&tokens_iterator, tokens);
  return tokens_iterator;
}
// helper functions to recognize tokens
bool token_is_left_paren(token_t* token) {
  return (token -> len == 1) && strncmp(token -> buffer, "(", 1) == 0;
}


bool token_is_right_paren(token_t* token) {
  return (token -> len == 1) && strncmp(token -> buffer, ")", 1) == 0;
}

bool token_is_symbol(token_t* token) {
  return (token -> token_kind) == (token_kind_symbol);
}
