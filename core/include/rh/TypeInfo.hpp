#pragma once
#define _RHLIB_INCLUDED_TYPEINFO

#include <rh.hpp>

// Will define implementation of decltype(typeid(...))
#include <typeinfo> // IWYU pragma: export

_RHLIB_BEGIN

using TypeInfo = decltype(typeid(int));

_RHLIB_END
