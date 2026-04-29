# -------------------------------------------------
# -                    dirvert                    -
# -------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project(
  Dirvert 
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
option(DEV_OPTS "Enable dev opts" FALSE)
option(BUILD_TESTS "Build tests found in 'test/'" FALSE)
option(INTRINSIC "Import immintrin.h for testing" FALSE)

if(DEV_OPTS)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
endif()

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

add_executable(dirvert ${SRC_FILES})

if(INTRINSIC)
  target_compile_options(dirvert PRIVATE -mavx2)
  target_compile_definitions(dirvert PRIVATE USE_IMMINTRIN)
endif()

target_include_directories(dirvert PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
