#include "mal.h"

mals_iterator_t mals_iterator(mals_t* mals) {
  mals_iterator_t mals_iterator;
  cvector_iterator__init(&mals_iterator, mals);
  return mals_iterator;
}
