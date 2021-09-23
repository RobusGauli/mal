#include "stringview.h"

StringView stringview__new(char *mem, size_t len) {
  return (StringView){.mem = mem, .len = len};
}
