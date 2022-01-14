include(Documentations)

set(DOXYGEN_TAB_SIZE 2)
set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_PROJECT_LOGO "${CMAKE_CURRENT_SOURCE_DIR}/imgs/icon.svg")
set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR};${PROJECT_BINARY_DIR};${CMAKE_CURRENT_SOURCE_DIR}")
set(DOXYGEN_STRIP_FROM_INC_PATH "${PROJECT_SOURCE_DIR}/include")
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

#Make doxygen happy
list_third_party_licenses(third_party_licenses.md)
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/third_party_licenses.md")
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/doxygen_third_party_licenses.md" "Third-party licenses {#third-party-licenses}\n")
  file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/doxygen_third_party_licenses.md" "====================\n")
  file(READ "${CMAKE_CURRENT_BINARY_DIR}/third_party_licenses.md" THIRD_PARTY_LICENSES_CONTENT)
  file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/doxygen_third_party_licenses.md" "${THIRD_PARTY_LICENSES_CONTENT}")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/LICENSE")
  file(WRITE  "${PROJECT_BINARY_DIR}/License.md" "License {#license}\n")
  file(APPEND "${PROJECT_BINARY_DIR}/License.md" "=======\n")
  file(READ "${PROJECT_SOURCE_DIR}/LICENSE" LICENSES_CONTENT)
  file(APPEND "${PROJECT_BINARY_DIR}/License.md" "${LICENSES_CONTENT}")

  file(COPY "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION "${PROJECT_BINARY_DIR}")
  file(RENAME "${PROJECT_BINARY_DIR}/LICENSE" "${PROJECT_BINARY_DIR}/License.md")
endif()

## HTML OPTIONS ##
set(DOXYGEN_GENERATE_HTML YES)

## LATEX OPTIONS ##
set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_COMPACT_LATEX NO)
set(DOXYGEN_LATEX_BATCHMODE NO)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/header.tex")
  set(DOXYGEN_LATEX_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/header.tex")
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/footer.tex")
  set(DOXYGEN_LATEX_FOOTER "${CMAKE_CURRENT_SOURCE_DIR}/footer.tex")
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/styleSheet.sty")
  set(DOXYGEN_LATEX_EXTRA_STYLESHEET "${CMAKE_CURRENT_SOURCE_DIR}/styleSheet.sty")
endif()
set(DOXYGEN_LATEX_EXTRA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgs/logo.svg")

doxyfile_docs("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/License.md" "${CMAKE_CURRENT_BINARY_DIR}/third_party_licenses.md")
