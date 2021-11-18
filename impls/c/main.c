#include "str.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define die(format, args...)                                                   \
  do {                                                                         \
    fprintf(stderr, "[FATAL]:%s:%d " format "\n", __FILE__, __LINE__, ##args); \
    exit(0);                                                                   \
  } while (0)

#define err(format, args...)                                                   \
  do {                                                                         \
    fprintf(stderr, "[ERROR]:%s:%d " format "\n", __FILE__, __LINE__, ##args); \
  } while (0)


const char *pathname = "just.bin";
const char* number = "23234";
const int int_number = 23234;

#define constant 01
#define another 034
#define just_another 023

// just write out these stuff

typedef struct Person {
  int number;
  int age;
  int color;
} Person;

int main() {

  // create a struct
  Person person =  {
    .number = 3,
    .color = 4,
    .age = 16,
  };
  int buffer;

  printf("the read number before is: %d\n", buffer);
  int fd = openat(AT_FDCWD, pathname, O_RDWR | O_CREAT | O_TRUNC);
  if (fd < 0) {
    die("%s", strerror(errno));
  }
  // this is not raw string \n is 0a and
  write(fd, &person, sizeof(Person));

}
