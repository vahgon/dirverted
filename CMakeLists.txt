# -------------------------------------------------
# -                    archie                     -
# -------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project(
  Archie 
  VERSION 0.0
  DESCRIPTION ""
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------
# options -----------------------------------------
# -------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

option(BUILD_TESTS "Build tests found in 'test/'" FALSE)
option(INTRINSIC "Import immintrin.h for testing" FALSE)
option(PROFILER "Build with gprof" FALSE)

if(BUILD_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(test)
endif()

# -------
# compile ------------------------------------------
# -------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(archie ${SRC_FILES})

#---------------
# gcc debug opts
#---------------
set(SANITIZE_OPTS
  -fsanitize=address
  -fsanitize=undefined
  -fsanitize=leak
)

set(DEBUG_FLAGS
  -Wall
  -Wextra
  -Wnrvo
  -Werror
  -Wpedantic
  -Wconversion
  -O0
  ${SANITIZE_OPTS}
)

target_compile_options(archie PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>")
target_link_options(archie PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZE_OPTS}>")

if(PROFILER)
  target_compile_options(archie PRIVATE "$<$<CONFIG:DEBUG>:-pg>")
endif()

# ----------------
# gcc release opts
#-----------------
set(RELEASE_FLAGS
  -Wall
  -Wextra
  -O2
)

target_compile_options(archie PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>")

if(INTRINSIC)
  target_compile_options(archie PRIVATE -mavx2)
  target_compile_definitions(archie PRIVATE USE_IMMINTRIN)
endif()

target_include_directories(archie PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
