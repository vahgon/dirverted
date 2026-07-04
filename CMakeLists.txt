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

# opts
option(BUILD_TESTS "Build tests"                            FALSE)
option(INTRINSIC "Import immintrin.h"                       FALSE)
option(DOCUMENTATION "Generate documentation via Doxygen"   FALSE)
option(PROFILER "Compile with gprof (-pg)"                  FALSE)
option(EXAMPLE "Compile /example"                           FALSE)

if(BUILD_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(test)
endif()

if(DOCUMENTATION)
 add_subdirectory(${CMAKE_SOURCE_DIR}/docs)
endif()

# create lib
add_library(archie_lib STATIC)

set_target_properties(archie_lib PROPERTIES
  CMAKE_CXX_STANDARD_REQUIRED TRUE
  CMAKE_CXX_EXTENSIONS FALSE
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

target_include_directories(archie_lib
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_features(archie_lib PRIVATE cxx_std_23)

# compile example
if(EXAMPLE)
  add_executable(archie ${CMAKE_SOURCE_DIR}/example/main.cpp)

  set_target_properties(archie PROPERTIES
    CMAKE_CXX_STANDARD_DEFAULT TRUE
    CMAKE_CXX_EXTENSIONS FALSE
  )

  target_compile_features(archie PRIVATE cxx_std_23)

  # -Werror
  target_compile_options(archie PRIVATE
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:DEBUG>:-Wconversion -Wsign-conversion -Wnrvo -g -O0>
    $<$<CONFIG:RELEASE>:-O3>
  )

  target_link_options(archie PRIVATE
    $<$<CONFIG:DEBUG>:-fsanitize=address,undefined,leak>
  )

  target_link_libraries(archie PUBLIC archie_lib)

endif()

if(PROFILER AND EXAMPLE)

  target_compile_options(archie PRIVATE $<$<CONFIG:DEBUG>:-pg>)

endif()

if(INTRINSIC AND EXAMPLE)

  target_compile_options(archie PRIVATE -mavx2)
  target_compile_definitions(archie PRIVATE USE_IMMINTRIN)

endif()
