# -------------------------------------------------
# -                    archie                     -
# -------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project(
  Dirverted
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
option(DOCUMENTATION "Generate documentation via Doxygen"   FALSE)
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
add_library(dirverted_lib STATIC)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

set_target_properties(dirverted_lib PROPERTIES
  CMAKE_CXX_STANDARD_REQUIRED TRUE
  CMAKE_CXX_EXTENSIONS FALSE
)

target_include_directories(dirverted_lib
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_features(dirverted_lib PUBLIC cxx_std_23)

# compile example
if(EXAMPLE)
  add_executable(dirverted ${CMAKE_SOURCE_DIR}/example/main.cpp)

  set_target_properties(dirverted PROPERTIES
    CMAKE_CXX_STANDARD_DEFAULT TRUE
    CMAKE_CXX_EXTENSIONS FALSE
  )

  target_compile_features(dirverted PRIVATE cxx_std_23)

  # -Werror
  target_compile_options(dirverted PRIVATE
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:DEBUG>:-Wconversion -Wsign-conversion -Wnrvo -g -O0>
    $<$<CONFIG:RELEASE>:-O3>
  )

  target_link_options(dirverted PRIVATE
    $<$<CONFIG:DEBUG>:-fsanitize=address,undefined,leak>
  )

  target_link_libraries(dirverted PUBLIC dirverted_lib)

endif()
