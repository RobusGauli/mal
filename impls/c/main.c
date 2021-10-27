#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include <errno.h>


#define die(format, args...) \
  do {  \
    fprintf(stderr, "[FATAL]:%s:%d " format"\n",__FILE__, __LINE__, ##args); \
    exit(0); \
  } while(0)

#define err(format, args...) \
  do {  \
    fprintf(stderr, "[ERROR]:%s:%d " format"\n",__FILE__, __LINE__, ##args); \
  } while(0)



int main() {
  /*FILE *file = NULL;*/

  /*file = fopen("just.txt", "rb");*/

  /*if (file == NULL) {*/
    /*die("could not open file: %s", strerror(errno));*/
  /*}*/

  /*int result = fseek(file, 0, SEEK_END);*/

  /*int res = ftell(file);*/
  /*printf("the result is another: %d\n", res);*/
  /*fseek(file, 0, SEEK_SET);*/

  /*char buffer[res];*/
  /*int r = fread(buffer, sizeof(char), res, file);*/

  /*for (size_t i =0; i < res; ++i) {*/
    /*char ch = buffer[i];*/
    /*printf("the value is: %c\n", ch);*/
    /*if (ch == '\n') {*/
      /*printf("we hae one");*/
    /*}*/
  /*}*/
  /*fclose(file);*/

  /*string_t* string = new_str();*/
  char* we = "this\\isanother";
  printf("%s\n", we);

}
