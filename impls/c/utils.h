#ifndef UTILS_H
#define UTILS_H

#define die(format, args...)                                                   \
  do {                                                                         \
    fprintf(stderr, "[FATAL]:%s:%d " format "\n", __FILE__, __LINE__, ##args); \
    fflush(stderr);                                                            \
    exit(0);                                                                   \
  } while (0)

#define err(format, args...)                                                   \
  do {                                                                         \
    fprintf(stderr, "[ERROR]:%s:%d " format "\n", __FILE__, __LINE__, ##args); \
  } while (0)

#endif
