#ifndef STR_H
#define STR_H

#include "deps/cvector/cvector.h"

CVector(char) cvector_chars_t;

typedef struct {
  cvector_chars_t cvector_chars;
} Str;

Str str__new(void);
void done(Str* str);
void str__append(Str* str, char* buffer, size_t len);
void str__nappend(Str* str, char* buffer);
void str__cappend(Str* str, char ch);

const char* str__ascstr(Str* str);
#endif // STR_H
