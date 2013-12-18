cmake_minimum_required( VERSION 2.8 )
project (NEWCORE)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake")
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")

SET(PROJ_SEARCH_PATH "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/compiler/utils/unittest/googletest/include")
include_directories( ${PROJ_SEARCH_PATH} )

MESSAGE("INCLUDE SEARCH DIRECTORIES: ${PROJ_SEARCH_PATH}")

add_llvm_library(newcore
  newcore.cc
  )
