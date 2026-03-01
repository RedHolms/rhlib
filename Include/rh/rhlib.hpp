#pragma once

#include <rh/casts.hpp>
#include <rh/internal/rhlib.h>
#include <rh/syntax.hpp>

namespace std {

// Create a shortcut for std::filesystem
namespace filesystem {}
namespace fs = filesystem;

} // namespace std

// Declare rhlib namespace
namespace rh {}
