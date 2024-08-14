#include <rh.hpp>

#if _RHLIB_OS == _RHLIB_OS_WINDOWS
# include "windows/memory.cpp"
// #elif _RHLIB_OS == _RHLIB_OS_GNU_LINUX
#else
# error Unsupported OS
#endif
