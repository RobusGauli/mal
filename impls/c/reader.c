#include "stdio.h"
#include "reader.h"


void parse_number_token(cvector_token_t* cvector_token, cvector_char_iterator_t* cvector_char_iterator) {
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

  for (;;) {
    if (cvector_iterator__done(&cvector_char_iterator)) break;

    char ch = cvector_iterator__peek(&cvector_char_iterator);
    if (ch == '(') {
      cvector__add(&cvector_tokens, (Token_t){.token_type=LEFT_PAREN});
      cvector_iterator__next(&cvector_char_iterator);
      continue;
    }
    if (ch == ')') {
      cvector__add(&cvector_tokens, (Token_t){.token_type=RIGHT_PAREN});
      cvector_iterator__next(&cvector_char_iterator);
      continue;
    }

    if (ch>=48 || ch <=57 ) {
      parse_number_token(&cvector_tokens, &cvector_char_iterator);
      continue;
    }

    // other special character
    // all other stuff are symbol for now
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

mal_t read_form(Reader* reader) {
  for (;;) {
    if (reader__done(reader)) break;
    Token_t token = reader__next(reader);
    printf("Token Type: %d", token.token_type);
  }
  return (mal_t){};
}

mal_t read_str(char* string) {

  cvector_token_t tokens = tokenize(string);
  cvector_token_iterator_t tokens_iterator = iterator(&tokens);

  Reader reader = (Reader) {
    .cvector_tokens=tokens,
    .cvector_tokens_iterator=tokens_iterator
  };

  mal_t result = read_form(&reader);
  cvector__free(&reader.cvector_tokens);

  return result;
}
