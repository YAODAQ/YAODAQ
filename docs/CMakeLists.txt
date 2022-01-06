include(Documentations)

set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_PROJECT_LOGO "${CMAKE_CURRENT_SOURCE_DIR}/imgs/icon.svg")
set(DOXYGEN_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/imgs/")

doxyfile_docs()
