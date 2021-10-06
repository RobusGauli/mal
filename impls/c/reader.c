#include "reader.h"
#include <assert.h>
#include <stdio.h>

mal_t *read_list(tokens_iterator_t *tokens_iterator) {
  // skip '('
  cvector_iterator__next(tokens_iterator);

  mal_t *mal = malloc(sizeof(mal_t));
  mals_t *mals = malloc(sizeof(mals_t));

  mal->type = mal_list;
  mal->value = (uint64_t)mals;

  cvector__init(mals);

  bool balanced = false;
  while (!cvector_iterator__done(tokens_iterator)) {
    token_t *token = cvector_iterator__peek(tokens_iterator);

    if (token_is_right_paren(token)) {
      balanced = true;
      cvector_iterator__next(tokens_iterator);
      break;
    }

    cvector__add(mals, read_form(tokens_iterator));
  }

  if (!balanced) {
    fprintf(stderr, "Unbalanced paren\n");
    exit(1);
  }

  return mal;
}

mal_t *read_form(tokens_iterator_t *tokens_iterator) {
  while (!cvector_iterator__done(tokens_iterator)) {

    token_t *token = cvector_iterator__peek(tokens_iterator);

    if (token_is_left_paren(token)) {
      return read_list(tokens_iterator);
    }

    return read_atom(tokens_iterator);
  }
  return NULL;
}

mal_t *read_atom(tokens_iterator_t *tokens_iterator) {
  mal_t *atom = malloc(sizeof(mal_t));

  token_t *token = cvector_iterator__next(tokens_iterator);

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
  default:
    assert(0 && "unreachable");
  }
}

mal_t *READ(char *input) {

  tokens_t toks = tokens(input);
  tokens_iterator_t iter = tokens_iterator(&toks);
  tokens_iterator_t iter1 = tokens_iterator(&toks);

  while(!cvector_iterator__done(&iter1)) {
    token_t* tok = cvector_iterator__next(&iter1);
  }


  return read_form(&iter);
}

mals_t *as_mals(mal_t *mal) { return (mals_t *)(mal->value); }

int as_number(mal_t *mal) { return (int)(mal->value); }

char *as_string(mal_t *mal) { return (char *)(mal->value); }

char *as_symbol(mal_t *mal) { return (char *)(mal->value); }
