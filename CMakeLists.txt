cmake_minimum_required(VERSION 3.18)
project("rhlib" LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        12.1.0
)

add_library(rhlib STATIC EXCLUDE_FROM_ALL)
set_target_properties(rhlib PROPERTIES
  CXX_STANDARD 20
)

# Determine compiler frontend
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(RHLIB_MSVC_LIKE_CC ON)

  # Assume that __cplusplus definition has a valid value and use UTF-8
  target_compile_options(rhlib PUBLIC "/Zc:__cplusplus" "/utf-8")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    set(RHLIB_MSVC_LIKE_CC ON)
  elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    set(RHLIB_GNU_LIKE_CC ON)
  else()
    message(FATAL_ERROR "Unknown clang frontend: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
  endif()
endif()

if(WIN32)
  target_compile_definitions(
    rhlib PUBLIC
    NOMINMAX                # disable min() and max() defines in Windows.h
    WIN32_LEAN_AND_MEAN     # remove useless shit from Windows.h
    _CRT_SECURE_NO_WARNINGS
  )
endif()

target_include_directories(rhlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")

# Include global header file
set(GLOBAL_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Include/rh/internal/global.h")
if(RHLIB_MSVC_LIKE_CC)
  target_compile_options(rhlib PUBLIC "/FI${GLOBAL_HEADER_PATH}")
elseif(RHLIB_GNU_LIKE_CC)
  target_compile_options(rhlib PUBLIC "-include${GLOBAL_HEADER_PATH}")
endif()

# Collect source files
file(GLOB_RECURSE RHLIB_HEADERS "Include/*.hpp" "Include/*.h")
file(GLOB_RECURSE RHLIB_SOURCES "Lib/*.cpp" "Lib/*.hpp")
target_sources(rhlib PUBLIC ${RHLIB_HEADERS})
target_sources(rhlib PRIVATE ${RHLIB_SOURCES})

FetchContent_MakeAvailable(fmtlib)
target_link_libraries(rhlib PUBLIC fmt::fmt)

# Include tests only when building as standalone project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  add_subdirectory("tests")
endif()
