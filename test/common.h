#ifndef JSON_TESTS_COMMON_H_INCLUDE
#define JSON_TESTS_COMMON_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <exception>

#define ASSERT_MSG(cond, msg...) do { \
    fprintf(stderr, "[%s:%d] ", __FILE__, __LINE__); \
    if (!(cond)) \
      { \
        fprintf(stderr, msg); \
        fprintf(stderr, "\n"); \
        abort(); \
      } \
    else \
      { \
        fprintf(stderr, "assert passed ("); \
        fprintf(stderr, msg); \
        fprintf(stderr, ")\n"); \
      } \
  } while (0)

#define ASSERT(cond) ASSERT_MSG(cond, "Assertion " # cond " failed")
#define ASSERT_EQ(s1, s2) ASSERT_MSG((s1) == (s2), # s1 " and " # s2 " not equal")
#define ASSERT_NE(s1, s2) ASSERT_MSG((s1) != (s2), # s1 " and " # s2 " equal")
#define ASSERT_STREQ(s1, s2) ASSERT_MSG(strcmp(s1, s2) == 0, "\"%s\" and \"%s\" not equal", s1, s2)
#define ASSERT_WSTREQ(s1, s2) ASSERT_MSG(wcscmp(s1, s2) == 0, "\"%ls\" and \"%ls\" not equal", s1, s2)

#define GUARD(call) do { \
    try { call; } \
    catch (const std::exception &e) { ASSERT_MSG(0, "Exception caught: %s", e.what()); } \
  } while (0)

#define ASSERT_THROW(call, exc) do { \
    fprintf(stderr, "Checking if " # call " throws an exception " # exc "\n"); \
    try { call; } \
    catch (const exc &e) { ASSERT_MSG(1, "Exception caught: %s", e.what()); } \
    catch (const std::exception &e) { ASSERT_MSG(0, "Exception caught: %s", e.what()); } \
  } while (0)

#define RUN(call, msg...) do { \
    fprintf(stderr, msg); \
    GUARD(call); \
  } while (0)
#define RUN0(fun) RUN(fun(), "Running " # fun "()\n")
#define RUN1(fun, arg1) RUN(fun(arg1), "Running " # fun "(" # arg1 ")\n")
#define RUN2(fun, arg1, arg2) RUN(fun(arg1, arg2), "Running " # fun "(" # arg1 ", " # arg2 ")\n")

#endif // JSON_TESTS_COMMON_H_INCLUDE
