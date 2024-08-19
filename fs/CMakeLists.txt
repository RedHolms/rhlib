cmake_minimum_required(VERSION 3.18)

target_sources(
  rhlib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh/fs.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/rh/fs/Path.hpp"
)

target_include_directories(rhlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory("tests")
