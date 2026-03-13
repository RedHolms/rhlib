#ifndef __RHLIB_C_MAIN_HEADER_INCLUDED
#define __RHLIB_C_MAIN_HEADER_INCLUDED

#include <stdint.h>

#ifdef _WIN32

/**
 * nullhandle - macro containing invalid (zero) value of HANDLE type
 */
#ifdef __cplusplus
#define nullhandle nullptr
#else
#define nullhandle 0
#endif

#endif // #ifdef _WIN32

typedef unsigned int uint;
typedef ptrdiff_t ssize_t;

#ifdef __cplusplus
extern "C"
#endif
  /**
   * _UnreachableCodeHandler() - user-defined function that is called when reached UNREACHABLE()
   *   statement.
   * This function must not return.
   */
  [[noreturn]] void _UnreachableCodeHandler(const wchar_t* file, uint line);

#ifdef __cplusplus
extern "C"
#endif
  /**
   * _AssertationFailureHandler() - user-defined function that is called when expression in ASSERT()
   *   results to a zero value.
   * At the most time this function must not return, but may return in some scenarios.
   */
  void _AssertationFailureHandler(const char* expr, const wchar_t* file, uint line);

#define _RHLIB_WIDE2(x) L##x
#define _RHLIB_WIDE1(x) _RHLIB_WIDE2(x)
#define _RHLIB_WFILE    _RHLIB_WIDE1(__FILE__)

/**
 * UNREACHABLE() - use this macro to mark code that must be unreachable.
 * Will call _UnreachableCodeHandler()
 */
#define UNREACHABLE() _UnreachableCodeHandler(_RHLIB_WFILE, __LINE__)

/**
 * ASSERT(EXPR) - use this macro to assert that EXPR results to a non-zero value.
 * Will call _AssertationFailureHandler()
 */
#define ASSERT(EXPR)                                                                               \
  (void)(!(EXPR) && (_AssertationFailureHandler(#EXPR, _RHLIB_WFILE, __LINE__), 0))

#if defined(__cplusplus) && !defined(__RHLIB_MAIN_HEADER_INCLUDED)
#error "rhlib's C header should not be included directly from C++"
#endif

#endif // #ifndef __RHLIB_C_MAIN_HEADER_INCLUDED
