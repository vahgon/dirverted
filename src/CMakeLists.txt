# archie library
add_library(archie_lib STATIC)

set_target_properties(archie_lib PROPERTIES
  CMAKE_CXX_STANDARD_REQUIRED TRUE
  CMAKE_CXX_EXTENSIONS FALSE
)

# headers
target_include_directories(archie_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# sources
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/archie)

# intrinsic functions
if(INTRINSIC)
  target_compile_options(archie_lib PRIVATE -mavx2)
  target_compile_definitions(archie_lib PRIVATE USE_IMMINTRIN)
endif()

target_compile_options(archie_lib PRIVATE
  -Wall -Wextra -Wpedantic
  $<$<CONFIG:DEBUG>:-O0 -Wconversion -Werror -Wnrvo -g>
  $<$<CONFIG:RELEASE>:-O3>
)

target_compile_features(archie_lib PRIVATE cxx_std_23)
