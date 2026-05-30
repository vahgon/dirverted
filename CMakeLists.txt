# -------------------------------------------------
# -                    archie                     -
# -------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project(
  Archie 
  DESCRIPTION ""
  LANGUAGES CXX
)

set(CMAKE_MAJOR_VERSION 0)
set(CMAKE_MINOR_VERSION 0)
set(CMAKE_PATCH_VERSION 0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

option(BUILD_TESTS "Build tests" FALSE)
option(INTRINSIC "Import immintrin.h" FALSE)
option(PROFILER "Compile with gprof (-pg)" FALSE)

# options -----------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

if(BUILD_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(test)
endif()

# create library
add_subdirectory(src)

# executable
add_executable(archie ${CMAKE_SOURCE_DIR}/src/archie/main.cpp)

set_target_properties(archie PROPERTIES
  CMAKE_CXX_STANDARD_DEFAULT TRUE
  CMAKE_CXX_EXTENSIONS FALSE
)

target_compile_features(archie PRIVATE cxx_std_23)

target_compile_options(archie PRIVATE
  -Wall -Wextra -Wpedantic
  $<$<CONFIG:DEBUG>:-Wconversion -Werror -Wnrvo -g -O0>
  $<$<CONFIG:RELEASE>:-O3>
)
target_link_options(archie PRIVATE
  $<$<CONFIG:DEBUG>:-fsanitize=address,undefined,leak>
)

if(PROFILER)
  target_compile_options(archie PRIVATE $<$<CONFIG:DEBUG>:-pg>)
endif()

target_link_libraries(archie PUBLIC archie_lib)
