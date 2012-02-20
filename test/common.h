#ifndef JSON_TESTS_COMMON_H_INCLUDE
#define JSON_TESTS_COMMON_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <exception>

#define ASSERT_MSG(cond, msg...) do { \
  if (!(cond)) \
    { \
      fprintf(stderr, "[%s:%d] ", __FILE__, __LINE__); \
      fprintf(stderr, msg); \
      fprintf(stderr, "\n"); \
      abort(); \
    } \
  } while (0)

#define ASSERT(cond) ASSERT_MSG(cond, "Assertion " # cond " failed")
#define ASSERT_STREQ(s1, s2) ASSERT_MSG(strcmp(s1, s2) == 0, "\"%s\" and \"%s\" not equal", s1, s2)
#define ASSERT_WSTREQ(s1, s2) ASSERT_MSG(wcscmp(s1, s2) == 0, "\"%ls\" and \"%ls\" not equal", s1, s2)

#define RUN(fun, msg...) do { \
  fprintf(stderr, msg); \
  try { fun(); } \
  catch (const std::exception &e) { ASSERT_MSG(0, "Exception caught: %s", e.what()); } \
  } while (0)
#define RUN0(fun) RUN(fun, "Running " # fun "()\n")

#endif // JSON_TESTS_COMMON_H_INCLUDE
