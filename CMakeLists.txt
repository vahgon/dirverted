CMAKE_MINIMUM_REQUIRED( VERSION 3.12 )
SET( CMAKE_CXX_STANDARD 20 )
SET( CMAKE_CXX_STANDARD_REQUIRED True )
PROJECT( Dirvert VERSION 0.1 )

set ( CMAKE_EXPORT_COMPILE_COMMANDS True )

ADD_LIBRARY( dirvert_lib
  ./src/dirvert.cpp
  ./src/dirroot.cpp
  ./src/opts.cpp
  )

ADD_EXECUTABLE( dirvert src/main.cpp )

TARGET_LINK_LIBRARIES( dirvert PUBLIC dirvert_lib )
TARGET_INCLUDE_DIRECTORIES( dirvert PUBLIC dirvert_lib)
