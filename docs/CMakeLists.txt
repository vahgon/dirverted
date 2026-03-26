# -------
# doxygen -----------------------------------------
# -------
find_package(Doxygen)

if(NOT Doxygen_FOUND)
  message(WARNING "Could not find Doxygen, ignoring GEN_DOCS...")
  return()
endif()

# - If NO_LATEX=TRUE was passed at configuration
if(NOT NO_LATEX)
  set(CMAKE_LATEX_OPT "YES")
else()
  set(CMAKE_LATEX_OPT "NO")
endif()

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile" @ONLY
)

doxygen_add_docs(
  example_docs
  ALL
  COMMENT "Generating documentation via Doxygen..."
  CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
)

# --------------
# pdf generation ----------------------------------
# --------------
add_custom_target(
  MAKE_PDF
  COMMAND make refman.pdf
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/latex"
)
