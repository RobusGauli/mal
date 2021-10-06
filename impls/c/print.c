#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "mal.h"
#include "print.h"
#include "reader.h"

CVector(char) string_t;


char *num_to_string(int num) {
  int length = num == 0 ? 1 : (int)(log10((double)(num)) + 1);
  int index = length;
  char *result = malloc(sizeof(char) * (length+1));
  while (index > 0) {
    int first = num / (int)(pow(10, index - 1));
    result[length - index] = '0' + first;
    num = num - (first * (int)pow(10, index - 1));
    index--;
  }
  result[length] = '\0';
  return result;
}

char *print_list(mal_t *mal) { return "this"; }

char *str_cpy(char *arg) {
  size_t size = sizeof(char) * ((strlen(arg)) + 1);
  char *result = malloc(size);
  memcpy(result, arg, size); // should take care of the null character
  return result;
}

void str_add(string_t* dst, char* arg) {
  for (size_t i=0; i < strlen(arg); ++i) {
    cvector__add(dst, arg[i]);
  }
}


char *PRINT(mal_t *mal) {
  // just return the things out the thing
  switch (mal->type) {
  case mal_string: {
    char *value = str_cpy(as_string(mal));
    return value;
  }

  case mal_number: {
    char *value = num_to_string(as_number(mal));
    return value;
  }
  case mal_list: {
    string_t string;
    cvector__init(&string);
    cvector__add(&string, '(');
    mals_t* mals = as_mals(mal);
    mals_iterator_t mals_iter = mals_iterator(mals);

    while(!cvector_iterator__done(&mals_iter)) {
      mal_t* mal = cvector_iterator__next(&mals_iter);
      char* mal_printable_string = PRINT(mal);
      str_add(&string, mal_printable_string);
      free(mal_printable_string);
    }
    cvector__add(&string, ')');
    cvector__add(&string, '\0');
    return cvector__wrapped_buffer(&string);
  }

  case mal_symbol:
    return str_cpy(as_string(mal));
  default:
    assert(0 && "unreachable");
  }
}
