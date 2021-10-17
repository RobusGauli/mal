#include "deps/cvector/cvector.h"
#include "str.h"
#include <math.h>


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


char *cstr_cpy(char *arg) {
  size_t size = sizeof(char) * ((strlen(arg)) + 1);
  char *result = malloc(size);
  memcpy(result, arg, size); // should take care of the null character
  return result;
}

string_t* new_str()  {
  string_t* string = malloc(sizeof(string_t));
  cvector__init(string);
  return string;
}

string_t* new_str_from_cstr(char* cstr) {
  string_t* string = new_str();
  str_append_cstr(string, cstr);
  return string;
}

size_t str_length(string_t* string) {
  return cvector__size(string);
}

char* str_memview(string_t* string) {
  return cvector__wrapped_buffer(string);
}

char* str_ascstr(string_t* string) {
  char* buffer = malloc(sizeof(char) * str_length(string) + sizeof(char));
  memcpy(buffer, str_memview(string), sizeof(char) * str_length(string));
  buffer[str_length(string) + 1] =  '\0';
  return buffer;

}

void str_append(string_t* dest, string_t* src) {
  string_iter_t string_iter = new_string_iterator(src);

  for(;;) {
    if (cvector_iterator__done(&string_iter)) break;
    cvector__add(dest, cvector_iterator__next(&string_iter));
  }

}

void str_append_cstr(string_t* dest, char* src) {
  for(size_t i = 0; i < strlen(src); ++i) {
    cvector__add(dest, src[i]);
  }
}
void str_append_char(string_t* dest, char src) {
  cvector__add(dest, src);
}

void str_append_space(string_t* dest) {
  str_append_char(dest, ' ');
}

void str_free(string_t* string) {
  cvector__free(string);
}

// str_iterator
string_iter_t new_string_iterator(string_t* string) {
  string_iter_t string_iter;
  cvector_iterator__init(&string_iter, string);
  return string_iter;
}
