#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdint.h>
#include <stdio.h>

#include "deps/cvector/cvector.h"
#include "mal.h"
#include "print.h"
#include "reader.h"
#include "token.h"
#include "eval.h"
#include "types.h"
#include "env.h"

void debug(mal_t *mal) {
  if (!mal) {
    return;
  }

  switch (mal->type) {
  case mal_string: {
    char *string = as_string(mal);
    printf("%s", string);
    break;
  }

  case mal_number: {
    int val = as_number(mal);
    printf("%d", val);
    break;
  }

  case mal_list: {
    printf("(");
    mals_t *cvector = as_mals(mal);
    mals_iterator_t iterator;
    cvector_iterator__init(&iterator, cvector);
    for (;;) {
      if (cvector_iterator__done(&iterator))
        break;
      mal_t *next = cvector_iterator__next(&iterator);
      debug(next);
    }
    printf(")");
    break;
  }

  case mal_symbol: {
    printf("%s", as_symbol(mal));
    break;
  }


  case mal_error:
    printf("error");
    break;
  }
}

mal_t* create_symbol_for(char* string) {
  mal_t* sym = malloc(sizeof(mal_t));
  sym -> type = mal_symbol;
  sym -> value = (u64)string;
  return sym;
}

mal_t* create_func_for(void* func) {
  mal_t* f = malloc(sizeof(mal_t));
  f -> type = mal_core_func;
  f -> value = (u64)(func);
  return f;
}

void setup_core_env(Env* env) {
  // Initial setup
  env_set(env, create_symbol_for("+"), create_func_for(core_add));
  env_set(env, create_symbol_for("*"), create_func_for(core_mul));
  env_set(env, create_symbol_for("/"), create_func_for(core_div));
  env_set(env, create_symbol_for("-"), create_func_for(core_sub));
  env_set(env, create_symbol_for(">"), create_func_for(core_greater_than));
  env_set(env, create_symbol_for(">="), create_func_for(core_greater_than_equal));
  env_set(env, create_symbol_for("<"), create_func_for(core_less_than));
  env_set(env, create_symbol_for("<="), create_func_for(core_less_than_equal));
  env_set(env, create_symbol_for("="), create_func_for(core_is_equal));
  env_set(env, create_symbol_for("list"), create_func_for(core_list));
  env_set(env, create_symbol_for("list?"), create_func_for(core_is_list));
  env_set(env, create_symbol_for("empty?"), create_func_for(core_is_list_empty));
  env_set(env, create_symbol_for("count"), create_func_for(core_list_count));
  env_set(env, create_symbol_for("pr-str"), create_func_for(core_pr_str));
  env_set(env, create_symbol_for("str"), create_func_for(core_str));
  env_set(env, create_symbol_for("prn"), create_func_for(core_prn));
  env_set(env, create_symbol_for("println"), create_func_for(core_println));

  // symbol for true
  mal_t* mal_true = malloc(sizeof(mal_t));
  mal_true ->  type= mal_bool_true;
  env_set(env, create_symbol_for("true"), mal_true);

  // symbol for false
  mal_t* mal_false = malloc(sizeof(mal_t));
  mal_false -> type = mal_bool_false;
  env_set(env, create_symbol_for("false"), mal_false);

  // symbol for nil
  mal_t* nil = new_mal();
  nil -> type = mal_nil;
  env_set(env, create_symbol_for("nil"), nil);
}

char* rep(char* input, Env* env) {
  return PRINT(EVAL(READ(input), env));
}

int main() {

  // create a new environment/
  // setup of root environment
  Env* env = new_env(NULL);

  setup_core_env(env);

  // 'not' function using mal itself
  rep("(def! not (fn* (a) (if a false true)))", env);

  for (;;) {
    char *input = readline("user> ");

    if (input == NULL) {
      exit(0);
    };

    mal_t *mal = READ(input);
    mal_t *evaluated = EVAL(mal, env);
    char *value = PRINT(evaluated);
    printf("%s\n", value);
    free(input);
  }
}
