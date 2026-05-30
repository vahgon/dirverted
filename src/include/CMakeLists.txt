target_include_directories(archie_lib
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/archie/internal
)
