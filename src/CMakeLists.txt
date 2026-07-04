# archie sources
target_sources(archie_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/core/file.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/core/buffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/core/archive.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/metadata/crc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/metadata/time.cpp
)

# intrinsics
if(INTRINSIC)
  target_compile_options(archie_lib PRIVATE -mavx2)
  target_compile_definitions(archie_lib PRIVATE USE_IMMINTRIN)
endif()
