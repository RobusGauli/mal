#ifndef STR_H
#define STR_H

#include "deps/cvector/cvector.h"

CVector(char) cvector_chars_t;

typedef struct {
  cvector_chars_t cvector_chars;
} Str;

typedef struct {
  Str* str;
  size_t size;
  size_t index;
} StrView;

// just make sure things
StrView strview__new(Str* str);
Str strview__tostr(StrView* strview);

Str str__new(void);
void str__done(Str* str);
size_t str__size(Str* str);
void str__append(Str* str, char* buffer, size_t len);
void str__nappend(Str* str, char* buffer);
void str__cappend(Str* str, char ch);
bool str__isnullterminated(Str* str);
bool str__isempty(Str* str);
void str__add(Str* self, Str* other);

const char* str__ascstr(Str* str);
#endif // STR_H
