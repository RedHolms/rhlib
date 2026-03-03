#pragma once
#define __RHLIB_MAIN_HEADER_INCLUDED

#include <rh/c/rhlib.h>
#include <rh/casts.hpp>
#include <rh/syntax.hpp>

namespace std {

// Create a shortcut for std::filesystem
namespace filesystem {}
namespace fs = filesystem;

} // namespace std

// Declare rhlib namespace
namespace rh {}
