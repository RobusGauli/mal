#ifndef STR_H
#define STR_H
#include <bits/stdint-intn.h>
#include "deps/cvector/cvector.h"

CVector(char) string_t;
CVector_iterator(string_t) string_iter_t;

string_t *new_str();
string_t *new_str_from_cstr(char *cstr);
void str_add(string_t *dst, char *arg);
void str_append(string_t *dest, string_t *src);
void str_append_cstr(string_t* dest, char* src);
void str_append_char(string_t* dest, char src);
void str_append_space(string_t* dest);
char *str_ascstr(string_t *string);
void str_free(string_t *string);
size_t str_length(string_t *string);
char *str_memview(string_t *string);


// Iterators
string_iter_t new_string_iterator(string_t* string);

char *num_to_string(int num);

// Cstr
char *cstr_cpy(char *arg);
char* cstr_from_i32(int32_t arg);
char* cstr_from_ptr(void* ptr);
#endif
