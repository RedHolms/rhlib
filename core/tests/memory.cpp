#include <gtest/gtest.h>

#include <rh.memory.hpp>

#include <Windows.h>

#define EXPECT_ACCESS_VIOLATION(expr)                               \
  __try {                                                           \
    expr;                                                           \
    GTEST_NONFATAL_FAILURE_("Expected access violation, but not");  \
  }                                                                 \
  __except (                                                        \
    GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION                \
      ? EXCEPTION_EXECUTE_HANDLER                                   \
      : EXCEPTION_CONTINUE_SEARCH                                   \
  ) {}

#define EXPECT_NO_ACCESS_VIOLATION(expr)                            \
  __try {                                                           \
    expr;                                                           \
  }                                                                 \
  __except (                                                        \
    GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION                \
      ? EXCEPTION_EXECUTE_HANDLER                                   \
      : EXCEPTION_CONTINUE_SEARCH                                   \
  ) {                                                               \
    GTEST_NONFATAL_FAILURE_("Got access violation");                \
  }

inline void expect_no_access_violation(void(__cdecl* worker)()) {
  __try {
    worker();
  }
  __except (
    GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION 
      ? EXCEPTION_EXECUTE_HANDLER 
      : EXCEPTION_CONTINUE_SEARCH
  ) {
    GTEST_NONFATAL_FAILURE_("Got access violation");
  }
}

TEST(MemoryTests, Access) {
  // uint8_t* some_memory = new uint8_t[1];
  // memory::access original_access = memory::get_access(some_memory, 1);

  // memory::set_access(memory::access::read | memory::access::write, some_memory, 1);
  // EXPECT_NO_ACCESS_VIOLATION({
  //   some_memory[0] = 0xC3;
  // });

  // memory::set_access(memory::access::read, some_memory, 1);
  // EXPECT_ACCESS_VIOLATION({
  // some_memory = nullptr;
  // some_memory[0] = 0;
  // });

  // EXPECT_EQ(some_memory[0], 0xC3);

  // using dummy_function_type = void(__cdecl*)();
  // dummy_function_type dummy_function = (dummy_function_type)some_memory;

  // EXPECT_ACCESS_VIOLATION({
  //   dummy_function();
  // });

  // memory::set_access(memory::access::execute, some_memory, 1);
  // EXPECT_NO_ACCESS_VIOLATION({
  //   dummy_function();
  // });

  // memory::set_access(original_access, some_memory, 1);
  // delete[] some_memory;
}
