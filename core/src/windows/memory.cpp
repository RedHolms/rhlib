#include <rh/memory.hpp>

#include <Windows.h>

namespace memory = rh::memory;

memory::access win32access_to_rh(DWORD win32access) {
  RTL_CRITICAL_SECTION a;
  using memory::access;

  switch (win32access & 0xFF) {
    case PAGE_NOACCESS:
    default:
      return access::none;
    case PAGE_READONLY:
      return access::read;
    case PAGE_READWRITE:
    case PAGE_WRITECOPY:
      return access::read | access::write;
    case PAGE_EXECUTE:
      return access::execute;
    case PAGE_EXECUTE_READ:
      return access::execute | access::read;
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
      return access::execute | access::read | access::write;
  }
}

DWORD rhaccess_to_win32(memory::access rhaccess) {
  using memory::access;

  if (rhaccess == access::none)
    return PAGE_NOACCESS;

  if (rhaccess & access::read) {
    if (rhaccess & access::write) {
      if (rhaccess & access::execute)
        return PAGE_EXECUTE_READWRITE;

      return PAGE_READWRITE;
    }
    
    if (rhaccess & access::execute)
      return PAGE_EXECUTE_READ;

    return PAGE_READONLY;
  }

  if (rhaccess & access::execute)
    return PAGE_EXECUTE;

  // unknown
  return PAGE_NOACCESS;
}

memory::access memory::getAccess(AnyPtr address, size_t bytes_count) {
  DWORD win32access, dummy;
  VirtualProtect(address, bytes_count, PAGE_NOACCESS, &win32access);
  VirtualProtect(address, bytes_count, win32access, &dummy);

  return win32access_to_rh(win32access);
}

void memory::setAccess(access value, AnyPtr address, size_t bytes_count) {
  DWORD dummy;
  VirtualProtect(address, bytes_count, rhaccess_to_win32(value), &dummy);
}
