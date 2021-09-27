#include <assert.h>
#include <math.h>


#include "str.h"

Str str__new(void) {
  cvector_chars_t cvector_chars;
  cvector__init(&cvector_chars);

  return (Str) {
    .cvector_chars=cvector_chars
  };
}

Str str__dclone(const Str* const other) {

  Str str = str__new();

  str__append(
      &str,
      cvector__wrapped_buffer(&(other -> cvector_chars)),
      cvector__size(&(other -> cvector_chars)
  ));

  return str;
}

void str__free(Str* str) {
  cvector__free(&(str -> cvector_chars));
}

const char* str__ascstr(Str* str) {
  return cvector__wrapped_buffer(&(str -> cvector_chars));
}


void str__append(Str* const str, const char* const buffer, size_t len) {
  for (size_t i = 0; i < len; i++) {
    cvector__add(&(str -> cvector_chars), buffer[i]);
  }
}

void str__nappend(Str* const str, const char* const buffer) {
  for (size_t i = 0 ; i < strlen(buffer); ++i) {
    cvector__add(&(str -> cvector_chars), buffer[i]);
  }
}

int ndigits(int arg) {
  int count = 1;
  while (arg > (int)pow(10, count))  {
    count++;
  }
  return count;
}

void str__intappend(Str* const str, int arg) {
  int n = ndigits(arg);

  for (int i = n-1; i >= 0; i--) {
    int num = (arg / (pow(10, i)));
    str__cappend(str, '0' + num);
    arg = (arg - num * pow(10, i));
  }

}

void str__cappend(Str* str, char ch) {
  cvector__add(&(str -> cvector_chars), ch);
}

void str__done(Str* str) {
  cvector__add(&(str -> cvector_chars), 0);
}


void str__add(Str* self, const Str* const other) {
  if(str__isempty(other)) {
    return;
  }

  str__append(self,
      cvector__wrapped_buffer(&(other -> cvector_chars)),
      cvector__size(&(other -> cvector_chars)) - 1 // exclude null character
      );
}

bool str__isnullterminated(const Str* const str) {

  if (!cvector__size(&(str -> cvector_chars)))  {
    return false;
  }

  char ch = cvector__index(
      &(str -> cvector_chars),
      cvector__size(&(str -> cvector_chars)) - 1);

  return ch == '\0';
}


bool str__isempty(const Str* const str) {
  assert(str__isnullterminated(str));
  return str__size(str) == 0;
}

bool str__eq(const Str* const self, const Str* const other) {
  size_t s_size = cvector__size(&(self -> cvector_chars));
  size_t o_size = cvector__size(&(other -> cvector_chars));

  if (s_size != o_size) {
    return false;
  }

  void* s_mem = cvector__wrapped_buffer(&(self -> cvector_chars));
  void* o_mem = cvector__wrapped_buffer(&(other -> cvector_chars));
  return memcmp(s_mem, o_mem, s_size) == 0;

}

size_t str__size(const Str* const str) {
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
