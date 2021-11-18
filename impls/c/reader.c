#include "reader.h"
#include <assert.h>
#include <stdio.h>
#include "types.h"

mal_t *read_list(tokens_iterator_t *tokens_iterator) {
  // skip '('
  cvector_iterator__next(tokens_iterator);

  mal_t *mal = malloc(sizeof(mal_t));
  mals_t *mals = malloc(sizeof(mals_t));

  mal->type = mal_list;
  mal->value = (u64)mals;

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
    string_t* string = new_str_from_cstr("unbalanced paren");
    mal_t* mal = new_mal_error(string);
    return mal;
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

  token_t *token = cvector_iterator__next(tokens_iterator);

  switch (token->token_kind) {
  case token_kind_number: {
    int number = 0;
    for (size_t i = 0; i < token->len; ++i) {
      number = number * 10 + ((token->buffer)[i] - '0');
    }
    mal_t *atom = malloc(sizeof(mal_t));
    atom->type = mal_number;
    atom->value = number;
    return atom;
  }

  case token_kind_string: {
    string_t* string = new_str();

    // just making sure things are well worth it
    size_t size = token -> len;
    size_t index = 0;
    while (index < token -> len) {
      char ch = token -> buffer[index];
      if (ch == '\\') {
        if (token -> buffer[index+1] == 'n')  {
          str_append_char(string, '\n');
          ++index;
        } else if (token -> buffer[index+1] == '"') {
          str_append_char(string, '"');
          ++index;
        } else if (token -> buffer[index+1] == '\\')  {
          str_append_char(string, '\\');
          ++index;
        } else {
          str_append_char(string, '\\');
          // no increment necessary
        }
      } else {
        str_append_char(string, token -> buffer[index]);
      }
      ++index;
    }

    mal_t *atom = malloc(sizeof(mal_t));
    atom->type = mal_string;
    atom->value = (u64)string;

    return atom;
  }

  case token_kind_symbol: {
    char *string = malloc((sizeof(char) * token->len) + sizeof(char));
    memcpy(string, token->buffer, token->len);
    string[token->len] = '\0';
    mal_t *atom = malloc(sizeof(mal_t));
    atom->type = mal_symbol;
    atom->value = (u64)(string);
    return atom;
  }

  case token_kind_special_chr: {
    char *string = malloc((sizeof(char) * token->len) + sizeof(char));
    memcpy(string, token->buffer, token->len);
    string[token->len] = '\0';
    mal_t *atom = malloc(sizeof(mal_t));
    atom->type = mal_symbol;
    atom->value = (u64)(string);
    return atom;
  }

  case token_kind_eol: {
    string_t* string = new_str_from_cstr("Error: \"Expected '\\\"', got EOF");
    return new_mal_error(string);
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
