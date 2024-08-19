#include <rh.hpp>

#if _RHLIB_OS == _RHLIB_OS_WINDOWS
# include "windows/Mutex.cpp"
// #elif _RHLIB_OS == _RHLIB_OS_GNU_LINUX
#else
# error Unsupported OS
#endif
