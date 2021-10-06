#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdint.h>
#include <stdio.h>

#include "deps/cvector/cvector.h"
#include "mal.h"
#include "token.h"
#include "reader.h"
#include "print.h"

mal_t *m_eval(mal_t *mal) { return mal; }

void debug(mal_t *mal) {
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
    mals_t *cvector = as_mals(mal);
    mals_iterator_t iterator;
    cvector_iterator__init(&iterator, cvector);
    for (;;) {
      if (cvector_iterator__done(&iterator))
        break;
      mal_t *next = cvector_iterator__next(&iterator);
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

int main() {

  // as pointer
  for (;;) {
    char *input = readline("user> ");

    if (input == NULL) {
      exit(0);
    };

    mal_t *mal = READ(input);
    char* value = PRINT(mal);
    printf("%s\n", value);
    free(input);
  }
}
