CMAKE_MINIMUM_REQUIRED( VERSION 3.12 )
SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED True )
PROJECT( Dirvert VERSION 0.1 )

ADD_LIBRARY( dirvert_lib
  ./src/dirvert.cpp
  ./src/dirbase.cpp
  )

ADD_EXECUTABLE( dirvert src/main.cpp )

TARGET_LINK_LIBRARIES( dirvert PUBLIC dirvert_lib )
TARGET_INCLUDE_DIRECTORIES( dirvert PUBLIC dirvert_lib)
