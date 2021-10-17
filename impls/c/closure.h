
#ifndef CLOSURE_H
#define CLOSURE_H

#include "mal.h"
#include "env.h"

typedef struct closure_t {
  Env* env;
  mal_t* binds;
  mal_t* body;
} closure_t;

#endif
