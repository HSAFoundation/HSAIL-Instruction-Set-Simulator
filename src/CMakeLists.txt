cmake_minimum_required( VERSION 2.8 )
project (HSA2BRIG)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake")
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")

include_directories( ${PROJ_SEARCH_PATH} )

MESSAGE("INCLUDE SEARCH DIRECTORIES: ${PROJ_SEARCH_PATH}")
