All headers should have definition at the beggining in this format:
```cxx
#define _RHLIB_INCLUDED_<filestem uppercase>
```
i.e. file `Array.hpp` should define `_RHLIB_INCLUDED_ARRAY`

All include directives goes in that order:
```
// If source file, including associated header first
#include <rh/Class.hpp>

// Otherwise include <rh.hpp>
#include <rh.hpp>

// Then goes STDC headers
#include <stdint.h>

// Then goes STL headers
#include <iostream>

// Then goes system headers
#include <Windows.h>
#include <sys/socket.h>

// Then goes thrid-party headers
#include <gtest/gtest.h>

// Then goes our headers
#include <rh/Something.hpp>

// Then goes "private" headers
#include "impl.hpp"
```

Each header group should be separated with blank line  
Headers in group should be sorted alphabetically.
Only exception when we definitely need to include one header before another (e.g. we need to include `<Windows.h>` before any other windows header)
