cmake_minimum_required(VERSION 3.18)

target_sources(
  rhlib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh/memory.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh/Array.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh/InitList.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh/String.hpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/memory.cpp"
)

target_include_directories(rhlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory("tests")
