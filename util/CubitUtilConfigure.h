
#ifndef CUBIT_UTIL_EXPORTS_H
#define CUBIT_UTIL_EXPORTS_H

/* #undef CUBIT_UTIL_BUILD_SHARED_LIBS */

#if defined(CUBIT_UTIL_BUILD_SHARED_LIBS)
#if defined(cubit_util_EXPORTS)
# if defined(WIN32)
#  define CUBIT_UTIL_EXPORT __declspec(dllexport)
# elif defined(__GNUC__) && __GNUC__ >= 4
#  define CUBIT_UTIL_EXPORT __attribute__ ((visibility("default")))
# endif
#else
# if defined(WIN32)
#  define CUBIT_UTIL_EXPORT __declspec(dllimport)
# endif
#endif
#endif

#ifndef CUBIT_UTIL_EXPORT
#define CUBIT_UTIL_EXPORT
#endif

#endif // CUBIT_UTIL_EXPORTS_H

