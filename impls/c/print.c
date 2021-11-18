#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "mal.h"
#include "print.h"
#include "reader.h"
#include "str.h"

char *pr_str(mal_t *mal, bool print_readably) {
  switch (mal->type) {

  case mal_string: {
    if (print_readably) {
      string_t *string = (string_t *)(mal->value);
      char *out = str_ascstr(string);
      string_t *result = new_str();
      str_append_char(result, '"');
      size_t index = 0;

      while (index < strlen(out)) {
        char ch = out[index];
        if (ch == '\n') {
          str_append_char(result, '\\');
          str_append_char(result, 'n');
        } else if (ch == '"') {
          str_append_char(result, '\\');
          str_append_char(result, '"');
        } else if (ch == '\\') {
          str_append_char(result, '\\');
          str_append_char(result, '\\');

        } else {
          str_append_char(result, ch);
        }
        ++index;
      }
      str_append_char(result, '"');
      char *cstr_res = str_ascstr(result);

      str_free(result);
      free(out);

      return cstr_res;
    } else {
      string_t *result = (string_t *)(mal->value);
      return str_ascstr(result);
    }
  }

  case mal_number: {
    char *value = num_to_string(as_number(mal));
    return value;
  }

  case mal_list: {

    string_t *string = new_str();
    str_append_char(string, '(');
    mals_t *mals = as_mals(mal);
    mals_iterator_t mals_iter = mals_iterator(mals);

    while (!cvector_iterator__done(&mals_iter)) {
      if (cvector_iterator__current_index_(&mals_iter) > 0) {
        str_append_space(string);
      }
      mal_t *mal = cvector_iterator__next(&mals_iter);
      char* mal_printable_string = NULL;
      if (print_readably) {
        mal_printable_string = pr_str(mal, true);
      } else {
        mal_printable_string = pr_str(mal, false);
      }
      str_append_cstr(string, mal_printable_string);
      free(mal_printable_string);
    }
    str_append_char(string, ')');
    char *cstr_result = str_ascstr(string);
    str_free(string);
    return cstr_result;
  }

  case mal_symbol:
    return cstr_cpy(as_string(mal));

  case mal_error: {
    string_t *string = (string_t *)mal->value;
    return str_ascstr(string);
  }

  case mal_func: {
    string_t *string = new_str_from_cstr("<function <lambda> at ");
    char *ptr_str = cstr_from_ptr(mal);
    str_append_cstr(string, ptr_str);
    str_append_char(string, '>');
    free(ptr_str);
    return str_ascstr(string);
  }

  case mal_core_func: {

    string_t *string = new_str_from_cstr("<function <core:lambda> at ");
    char *ptr_str = cstr_from_ptr(mal);
    str_append_cstr(string, ptr_str);
    free(ptr_str);
    str_append_char(string, '>');
    return str_ascstr(string);
  }
  case mal_bool_true: {

    string_t *string = new_str_from_cstr("true");
    return str_ascstr(string);
  }
  case mal_bool_false: {
    string_t *string = new_str_from_cstr("false");
    return str_ascstr(string);
  }
  case mal_nil: {

    string_t *string = new_str_from_cstr("nil");
    return str_ascstr(string);
  }
  default:
    fprintf(stderr, "[ERROR]: could not print the mal_type: %s\n",
            mal_kind_name(mal->type));
    assert(0 && "unreachable");
  }
}

char *PRINT(mal_t *arg) { return pr_str(arg, true); }
