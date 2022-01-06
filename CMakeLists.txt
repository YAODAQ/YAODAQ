cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GetCMakeMM)
cmmm(VERSION 2.2 REPOSITORY "flagarde/CMakeMM" DESTINATION "CMakeMM")

cmmm_modules_list(REPOSITORY "flagarde/CMakeCM")

option(BUILD_DOCS "Build the docs" ON)
option(BUILD_EXAMPLES "Build the examples" ON)
option(BUILD_TESTS "Build the tests" ON)

project(YAODAQ VERSION 0.1 DESCRIPTION "Yet An Other DAQ" HOMEPAGE_URL "https://github.com/yaodaq/YAODAQ" LANGUAGES C CXX)

add_subdirectory(src)

add_subdirectory(apps)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()
