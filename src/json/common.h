#ifndef JSON_COMMON_H_INCLUDE
#define JSON_COMMON_H_INCLUDE

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define JSON_NORETURN __attribute__ ((__noreturn__))
#else
#define JSON_NORETURN
#endif


#endif // JSON_COMMON_H_INCLUDE
