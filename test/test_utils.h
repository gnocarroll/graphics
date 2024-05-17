#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>

#define TEST_ASSERT(x) do { if (!(x)) { PRINT_MSG_AND_RETURN(x); }} while (0)

// internals of above

#define FAIL_PARAMS(x) __FILE__, __LINE__, __func__, #x
#define PRINT_MSG_AND_RETURN(x) print_fail_msg(FAIL_PARAMS(x)); return -1

inline static void print_fail_msg(const char *file, int line, const char *func,
                                  const char *condition_as_str) {
  fprintf(stderr, "%s at line %d in %s: assertion \"%s\" failed\n",
          file, line, func, condition_as_str);
}

#endif // TEST_UTILS_H
