#include <assert.h>

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


void str__add(Str* self, Str* other) {
  if(str__isempty(other)) {
    return;
  }

  str__append(self,
      cvector__wrapped_buffer(&(other -> cvector_chars)),
      cvector__size(&(other -> cvector_chars)) - 1 // exclude null character
      );
}

bool str__isnullterminated(Str* str) {

  if (!cvector__size(&(str -> cvector_chars)))  {
    return false;
  }

  char ch = cvector__index(
      &(str -> cvector_chars),
      cvector__size(&(str -> cvector_chars)) - 1);

  return ch == '\0';
}


bool str__isempty(Str* str) {
  assert(str__isnullterminated(str));
  return str__size(str) == 0;
}

size_t str__size(Str* str) {
  return cvector__size(&(str -> cvector_chars)) - 1;
}

StrView strview__new(Str* str) {
  return (StrView){
    .size = cvector__size(&(str -> cvector_chars)) -1 ,
    .index = 0,
    .str = str,
  };
}

StrView strview__shift(StrView* strview) {
  return (StrView) {
    .size = strview -> size,
    .index = strview -> index + 1,
    .str = strview -> str,
  };
}

Str strview__tostr(StrView* strview) {
  Str result = str__new();
  for (size_t i = strview -> index; i <  strview -> size; ++i) {
    str__cappend(&result, cvector__index(&(strview -> str -> cvector_chars), i));
  }
  str__done(&result);
  return result;
}
