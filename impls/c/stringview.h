#ifndef STRINGVIEW_H
#define STRINGVIEW_H

#include <stdlib.h>

typedef struct {
  char* mem;
  size_t len;
} StringView;

// string view functions
StringView stringview__new(char* mem, size_t len);

#endif
