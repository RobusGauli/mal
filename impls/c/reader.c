#include <assert.h>
#include "stdio.h"
#include "reader.h"

void parse_rest(cvector_token_t* cvector_token, cvector_char_iterator_t* cvector_char_iterator) {
  // we can assume that this thing is number
  char* memptr = cvector_iterator__next_ref(cvector_char_iterator);
  size_t len = 1;
  for(;;) {
    if (cvector_iterator__done(cvector_char_iterator)) break;

    char ch = cvector_iterator__peek(cvector_char_iterator);
    if (ch == ' ') {
      break;
    }
    cvector_iterator__next(cvector_char_iterator);
    len++;
  }

  cvector__add(cvector_token, ((Token_t){.token_type = NUMBER, .value=memptr, .value_length=len}));
}

Token_t token__new(Token_type_e token_type) {
  Token_t token = {.token_type=token_type};
  return token;
}

Token_t token__new_with_val(Token_type_e token_type, char* value, size_t value_length) {
  Token_t token =  {.token_type=token_type, .value=value, .value_length=value_length};
  return token;
}

char *token__stringify(Token_type_e token_type) {
  switch (token_type) {
  case COMMENT: {
    return "COMMENT";
  }
  case SYMBOL: {
    return "SYMBOL";
  }

  case NUMBER: {
    return "NUMBER";
  }

  case SEMI_COLON: {
    return "SEMI_COLON";
  }

  case OTHER: {
    return "OTHER";
  }

  case SPECIAL_TWO_CHARS__TILDA_AT: {
    return "SPECIAL_TWO_CHARS__TILDA_AT";
  }

  case SPECIAL_SINGLE_CHAR__AT: {
    return "SPECIAL_SINGLE_CHAR__AT";
  }

  case SPECIAL_SINGLE_CHAR__LEFT_SQ_BR: {
    return "SPECIAL_SINGLE_CHAR__LEFT_SQ_BR";
  }

  case SPECIAL_SINGLE_CHAR__RIGHT_SQ_BR: {
    return "SPECIAL_SINGLE_CHAR__RIGHT_SQ_BR";
  }

  case SPECIAL_SINGLE_CHAR__LEFT_CURLY_BR: {
    return "SPECIAL_SINGLE_CHAR__LEFT_CURLY_BR";
  }

  case SPECIAL_SINGLE_CHAR__RIGHT_CURLY_BR: {
    return "SPECIAL_SINGLE_CHAR__RIGHT_CURLY_BR";
  }

  case SPECIAL_SINGLE_CHAR__LEFT_PAREN: {
    return "SPECIAL_SINGLE_CHAR__LEFT_PAREN";
  }

  case SPECIAL_SINGLE_CHAR__RIGHT_PAREN: {
    return "SPECIAL_SINGLE_CHAR__RIGHT_PAREN";
  }

  case SPECIAL_SINGLE_CHAR__APOSTROPHE: {
    return "SPECIAL_SINGLE_CHAR__APOSTROPHE";
  }

  case SPECIAL_SINGLE_CHAR__WEIRD: {
    return "SPECIAL_SINGLE_CHAR__WEIRD";
  }

  case SPECIAL_SINGLE_CHAR__TILDA: {
    return "SPECIAL_SINGLE_CHAR__TILDA";
  }

  case SPECIAL_SINGLE_CHAR__CARRAT: {
    return "SPECIAL_SINGLE_CHAR__CARRAT";
  }
                                    return "MEH";
  }
}

static cvector_token_t tokenize(char* string) {
  cvector_char_t cvector_chars;
  cvector__init(&cvector_chars);

  cvector_token_t cvector_tokens;
  cvector__init(&cvector_tokens);

  for (int i = 0; i < strlen(string); i++) {
    cvector__add(&cvector_chars, string[i]);
  }

  cvector_char_iterator_t cvector_char_iterator;
  cvector_iterator__init(&cvector_char_iterator, &cvector_chars);

  for(;;) {
    // just make sure things are working fine
    if (cvector_iterator__done(&cvector_char_iterator)) break;
    char* ch = cvector_iterator__next_ref(&cvector_char_iterator);
    if (*ch == '(') {
      Token_t token = token__new(SPECIAL_SINGLE_CHAR__LEFT_PAREN);
      cvector__add(&cvector_tokens, token);
      continue;
    }

    if (*ch == ')') {
      Token_t token = token__new(SPECIAL_SINGLE_CHAR__RIGHT_PAREN);
      cvector__add(&cvector_tokens, token);
      continue;
    }

    if (*ch == ' ' || *ch == '\t' || *ch == '\n' || *ch == ',') {
      continue; // ignore
    }

    if (*ch == '~') {
      // check ot
      char* another_ch = cvector_iterator__peek_ref(&cvector_char_iterator);
      if (*another_ch == '@') {
        Token_t token = token__new_with_val(SPECIAL_TWO_CHARS__TILDA_AT, ch, 2);
        cvector__add(&cvector_tokens, token);
        cvector_iterator__next(&cvector_char_iterator);
        continue;
      }

      Token_t token = token__new(SPECIAL_SINGLE_CHAR__TILDA);
      cvector__add(&cvector_tokens, token);
    }

    if (*ch == '\'') {
      Token_t token = token__new(SPECIAL_SINGLE_CHAR__APOSTROPHE);
      cvector__add(&cvector_tokens, token);
      continue;
    }

    if (*ch == '`') {
      Token_t token = token__new(SPECIAL_SINGLE_CHAR__WEIRD);
      cvector__add(&cvector_tokens, token);
      continue;
    }

    if (*ch == '^') {
      Token_t token = token__new(SPECIAL_SINGLE_CHAR__CARRAT);
      cvector__add(&cvector_tokens, token);
      continue;
    }

    if (*ch == '@') {
      Token_t token = token__new(SPECIAL_SINGLE_CHAR__AT);
      cvector__add(&cvector_tokens, token);
      continue;
    }

    if (*ch == ';') {
      // we have a comment
      size_t len = 1;
      for (;;) {
        if (cvector_iterator__done(&cvector_char_iterator)) {
          Token_t token = token__new_with_val(COMMENT, ch, len);
          cvector__add(&cvector_tokens, token);
          break;
        }

        char *next_char = cvector_iterator__peek_ref(&cvector_char_iterator);
        if (*next_char == '\n') {
          Token_t token = token__new_with_val(COMMENT, ch, len);
          cvector__add(&cvector_tokens, token);
          break;
        }
        cvector_iterator__next(&cvector_char_iterator);
        len++;
      }
      continue;
    }

    size_t len = 1;
    for (;;) {
      if (cvector_iterator__done(&cvector_char_iterator)) {
        Token_t token = token__new_with_val(OTHER, ch, len);
        cvector__add(&cvector_tokens, token);
        break;
      }

      char* next_char = cvector_iterator__peek_ref(&cvector_char_iterator);
      if (*next_char == ' '
          || *next_char == '\n'
          || *next_char == '{'
          || *next_char == '}'
          || *next_char == ']'
          || *next_char == '['
          || *next_char == '`'
          || *next_char == ')'
          || *next_char == '('
          || *next_char == '^'
          || *next_char == '@'
          || *next_char == ';'
          || *next_char == ','
          || *next_char == '\''
          ) {
        Token_t token = token__new_with_val(OTHER, ch, len);
        cvector__add(&cvector_tokens, token);
        break;
      }
      cvector_iterator__next(&cvector_char_iterator);
      len++;
    }
  }

  return cvector_tokens;
}

static cvector_token_iterator_t iterator(cvector_token_t* tokens) {
  cvector_token_iterator_t iterator;
  cvector_iterator__init(&iterator, tokens);
  return iterator;
}

static Token_t reader__next(Reader* reader) {
  return cvector_iterator__next(&(reader -> cvector_tokens_iterator));
}

static Token_t reader__peek(Reader* reader) {
  return cvector_iterator__peek(&(reader -> cvector_tokens_iterator));
}

static bool reader__done(Reader* reader) {
  return cvector_iterator__done(&(reader -> cvector_tokens_iterator));
}


void token__print_value(Token_t token) {
  char* memptr = token.value;
  char buffer[token.value_length+1];
  for (size_t i = 0; i < token.value_length; i++) {
    buffer[i] = memptr[i];
  }
  buffer[token.value_length] = 0;
  printf("%s\n", buffer);
}


void read_list(AST_t* ast, Reader* reader) {
  assert(reader__peek(reader).token_type == SPECIAL_SINGLE_CHAR__LEFT_PAREN);
  reader__next(reader); // skip the left paren
  for(;;) {
    if (reader__done(reader)) break;
    Token_t token = reader__peek(reader);
    if (token.token_type == SPECIAL_SINGLE_CHAR__RIGHT_PAREN) {
      reader__next(reader);
      break;
    }
    read_form(ast, reader);
  }
}

void read_form(AST_t* ast, Reader* reader) {

  // Init the node
  for (;;) {
    if (reader__done(reader)) break;
    Token_t token = reader__peek(reader);
    if(token.token_type == SPECIAL_SINGLE_CHAR__LEFT_PAREN) {
      AST_t* next = malloc(sizeof(AST_t));
      ast -> next = next;
      cvector__init(&(next -> cvector_nodes));
      read_list(next, reader);
    } else {
      Node_t node;
      node.node_type = NODE__SYMBOL;
      node.node_val.node_symbol = (Node_symbol_t){.memptr=token.value, .len=token.value_length};
      cvector__add(&(ast -> cvector_nodes), node);
      reader__next(reader);
      continue;
    }
  }
}

AST_t* read_str(char* string) {

  cvector_token_t tokens = tokenize(string);
  cvector_token_iterator_t tokens_iterator = iterator(&tokens);

  Reader reader = (Reader) {
    .cvector_tokens=tokens,
    .cvector_tokens_iterator=tokens_iterator
  };

  AST_t* ast = malloc(sizeof(AST_t));
  ast -> next = NULL;
  cvector__init(&(ast -> cvector_nodes));
  read_form(ast, &reader);
  cvector__free(&reader.cvector_tokens);

  return ast;
}
