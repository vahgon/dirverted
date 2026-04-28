# -------------------------------------------------
# -                    dirvert                    -
# -------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project(
  Dirvert 
  VERSION 0.0 
  DESCRIPTION ""
  LANGUAGES CXX ASM
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
# library ------------------------------------------
# -------
set(DVRT_SRC
  src/dir.cpp
  src/read_file.cpp
  src/streamobj.cpp
  src/checksum.s
)

add_library(dvrt STATIC ${DVRT_SRC})
target_include_directories(dvrt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include(GenerateExportHeader)
generate_export_header(dvrt)

# -------
# compile ------------------------------------------
# -------
add_executable(dirvert ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(dirvert PRIVATE dvrt)
