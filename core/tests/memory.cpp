#include <gtest/gtest.h>

#include <rh/memory.hpp>

#include <Windows.h>

namespace memory = rh::memory;

TEST(MemoryTests, Access) {
  // uint8_t* some_memory = new uint8_t[1];
  // memory::access original_access = memory::get_access(some_memory, 1);

  // memory::set_access(memory::access::read | memory::access::write, some_memory, 1);
  // EXPECT_NO_THROW({
  //   some_memory[0] = 0xC3;
  // });

  // memory::set_access(memory::access::read, some_memory, 1);
  // EXPECT_ANY_THROW({
  //   some_memory = nullptr;
  //   some_memory[0] = 0;
  // });

  // EXPECT_EQ(some_memory[0], 0xC3);

  // using dummy_function_type = void(__cdecl*)();
  // dummy_function_type dummy_function = (dummy_function_type)some_memory;

  // EXPECT_ANY_THROW({
  //   dummy_function();
  // });

  // memory::set_access(memory::access::execute, some_memory, 1);
  // EXPECT_NO_THROW({
  //   dummy_function();
  // });

  // memory::set_access(original_access, some_memory, 1);
  // delete[] some_memory;
}
