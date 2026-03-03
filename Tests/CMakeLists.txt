cmake_minimum_required(VERSION 3.18)

include(FetchContent)

enable_testing()

FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL "https://github.com/google/googletest/archive/f345b2ca6adb1b505049190867eedf24d3b5eaa3.zip"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(rhlib_tests
  "StringTests.cpp"
)

target_link_libraries(rhlib_tests PRIVATE rhlib GTest::gtest_main GTest::gmock)
set_target_properties(rhlib_tests PROPERTIES CXX_STANDARD 20)

gtest_discover_tests(rhlib_tests)
