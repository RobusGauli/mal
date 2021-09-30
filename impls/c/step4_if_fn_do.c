#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

typedef struct {
}  Mal;

Mal m_read(const char* input) {
  return (Mal){};
}

Mal m_eval(Mal mal) {
  return (Mal){};
}

char* m_print(Mal mal) {
  char* src = "echo world";
  char* sample_string = malloc((sizeof(char) * strlen(src)) + (sizeof(char)));
  memcpy(sample_string, src, strlen(src));
  sample_string[strlen(src)] = '\0';
  return sample_string;
}

#define forever for (;;)

int main() {

  forever {
    char* input = readline("user>");

    if (input == NULL) exit(0);

    Mal mal = m_read(input);
    Mal result =  m_eval(mal);
    char* stringified = m_print(result);
    printf("%s\n", stringified);
    //
    // free
    free(stringified);
    free(input);
  }
}
