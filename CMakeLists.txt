cmake_minimum_required(VERSION 3.18)
include(FetchContent)

if(NOT DEFINED RHLIB_ROOT_PROJECT)
  set(RHLIB_ROOT_PROJECT OFF)
  if(NOT DEFINED PROJECT_NAME)
    set(RHLIB_ROOT_PROJECT ON)
  endif()
endif()

project("rhlib" LANGUAGES CXX)

if(RHLIB_ROOT_PROJECT)
  # Add all modules
  file(GLOB RHLIB_MODULES
    LIST_DIRECTORIES true
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/Lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lib/*"
  )
endif()

if(NOT DEFINED RHLIB_MODULES)
  # Default modules:
  set(RHLIB_MODULES "Core")
endif()

if(NOT "Core" IN_LIST RHLIB_MODULES)
  list(INSERT RHLIB_MODULES 0 "Core")
endif()

message(STATUS "rhlib modules: ${RHLIB_MODULES}")

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
    _CRT_SECURE_NO_WARNINGS
  )
endif()

# Include global header file
set(GLOBAL_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Lib/Core/Public/rh/internal/global.h")
if(RHLIB_MSVC_LIKE_CC)
  target_compile_options(rhlib PUBLIC "/FI${GLOBAL_HEADER_PATH}")
elseif(RHLIB_GNU_LIKE_CC)
  target_compile_options(rhlib PUBLIC "-include${GLOBAL_HEADER_PATH}")
endif()

function(_rhlib_add_sources_dir SCOPE DIR_PATH)
  if(IS_DIRECTORY ${DIR_PATH})
    file(
      GLOB_RECURSE _FILES
      "${DIR_PATH}/*.cpp" "${DIR_PATH}/*.c"
      "${DIR_PATH}/*.hpp" "${DIR_PATH}/*.h"
    )

    target_include_directories(rhlib ${SCOPE} "${DIR_PATH}")
    target_sources(rhlib ${SCOPE} ${_FILES})
  endif()
endfunction()

# Add modules
set(_MODULES_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Lib")
foreach(MODULE IN LISTS RHLIB_MODULES)
  set(_MODULE_DIR "${_MODULES_ROOT}/${MODULE}")

  if(NOT IS_DIRECTORY ${_MODULE_DIR})
    message(FATAL_ERROR "Unknown rhlib module: ${MODULE}")
  endif()

  _rhlib_add_sources_dir(PRIVATE "${_MODULE_DIR}/Private")
  _rhlib_add_sources_dir(PUBLIC "${_MODULE_DIR}/Public")
endforeach()

FetchContent_MakeAvailable(fmtlib)
target_link_libraries(rhlib PUBLIC fmt::fmt)

if(RHLIB_ROOT_PROJECT)
  add_subdirectory("tests")
endif()
