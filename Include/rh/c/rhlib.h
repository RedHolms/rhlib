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

#endif

typedef unsigned int uint;
typedef ptrdiff_t ssize_t;

/**
 * _UnreachableCodeHandler() - user-defined function that is called when reached UNREACHABLE()
 *   statement.
 * This function must not return.
 */
#ifdef __cplusplus
extern "C"
#endif
  [[noreturn]] void _UnreachableCodeHandler(const wchar_t* file, uint line);

#define _RHLIB_WIDE2(x) L##x
#define _RHLIB_WIDE1(x) _RHLIB_WIDE2(x)
#define _RHLIB_WFILE _RHLIB_WIDE1(__FILE__)

/**
 * UNREACHABLE() - use this macro to mark code that must be unreachable.
 * Will call _UnreachableCodeHandler()
 */
#define UNREACHABLE() _UnreachableCodeHandler(_RHLIB_WFILE, __LINE__)

#if defined(__cplusplus) && !defined(__RHLIB_MAIN_HEADER_INCLUDED)
#warning "rhlib's C header should not be included from C++"
#include <rh/rhlib.hpp>
#endif

#endif // #ifndef __RHLIB_C_MAIN_HEADER_INCLUDED
