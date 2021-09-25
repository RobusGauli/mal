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
Str str__dclone(const Str* const other); // deep clone
void str__done(Str* const str);
size_t str__size(const Str* const str);
void str__free(Str* str);
void str__append(Str* const str, const char* const buffer, size_t len);
void str__nappend(Str* const str, const char* const buffer);
void str__cappend(Str* const str, char ch);
bool str__isnullterminated(const Str* const str);
bool str__isempty(const Str* const str);
void str__add(Str* constself, const Str* const other);

const char* str__ascstr(Str* str);
#endif // STR_H
