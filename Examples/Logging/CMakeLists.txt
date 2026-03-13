cmake_minimum_required(VERSION 3.18)

project("rhlib-example-Logging" LANGUAGES CXX)

set(RHLIB_MODULES
  Core
  Logging)
add_subdirectory("../.." "${CMAKE_BINARY_DIR}/parent_rhlib")

add_executable(${PROJECT_NAME}
  "main.cpp"
)
target_link_libraries(${PROJECT_NAME} PRIVATE rhlib)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
