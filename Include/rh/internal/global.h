#ifndef __RHLIB_GLOBAL_INCLUDED
#define __RHLIB_GLOBAL_INCLUDED

#ifdef __cplusplus
#include <rh/rhlib.hpp>
#endif // #ifdef __cplusplus

#include <rh/c/rhlib.h>

#ifdef _WIN32
#include <Windows.h>
#endif // #ifdef _WIN32

#else // ^^^ #ifndef __RHLIB_GLOBAL_INCLUDED
#error "global.h should not be included manually"
#endif
