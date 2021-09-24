#include "str.h"

Str str__new(void) {
  cvector_chars_t cvector_chars;
  cvector__init(&cvector_chars);

  return (Str) {
    .cvector_chars=cvector_chars
  };
}

const char* str__ascstr(Str* str) {
  return cvector__wrapped_buffer(&(str -> cvector_chars));
}


void str__append(Str* str, char* buffer, size_t len) {
  for (size_t i = 0; i < len; i++) {
    cvector__add(&(str -> cvector_chars), buffer[i]);
  }
}

void str__nappend(Str* str, char* buffer) {
  for (size_t i = 0 ; i < strlen(buffer); ++i) {
    cvector__add(&(str -> cvector_chars), buffer[i]);
  }
}

void str__cappend(Str* str, char ch) {
  cvector__add(&(str -> cvector_chars), ch);
}

void str__done(Str* str) {
  cvector__add(&(str -> cvector_chars), 0);
}
