cmake_minimum_required(VERSION 3.18)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(_RHLIB_STANDALONE ON)
else()
  set(_RHLIB_STANDALONE OFF)
endif()

option(RHLIB_BUILD_TESTS "Build tests" ${_RHLIB_STANDALONE})

project("rhlib" CXX)

configure_file(.clangd.in ${CMAKE_CURRENT_SOURCE_DIR}/.clangd @ONLY)

if(RHLIB_BUILD_TESTS)
  enable_testing()

  include(FetchContent)
  FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL "https://github.com/google/googletest/archive/f345b2ca6adb1b505049190867eedf24d3b5eaa3.zip"
  )

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  include(GoogleTest)

  function(rhlib_add_test_target target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} PRIVATE rhlib GTest::gtest_main)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 23)
    gtest_discover_tests(${target})
  endfunction()
else()
  function(rhlib_add_test_target)
  endfunction()
endif()

add_library(rhlib STATIC)
add_library(rhlib::rhlib ALIAS rhlib)

set_target_properties(rhlib PROPERTIES CXX_STANDARD 23)

add_subdirectory("core")
add_subdirectory("atomic")
