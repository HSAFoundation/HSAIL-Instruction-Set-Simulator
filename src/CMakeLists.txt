cmake_minimum_required( VERSION 2.8 )
project (HSA2BRIG)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake")
find_package(FLEX)
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")

include_directories( ${PROJ_SEARCH_PATH} )

flex_target(lexer lexer_rules.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
	COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)

MESSAGE("INCLUDE SEARCH DIRECTORIES: ${PROJ_SEARCH_PATH}")
MESSAGE("Flex outputs: ${FLEX_lexer_OUTPUTS}")

add_llvm_library(hsa2brig
  brig_buffer.cc
  context.cc
  error_reporter.cc
  lexer_wrapper.cc
  parser.cc
  parser_wrapper.cc
  ${FLEX_lexer_OUTPUTS}
  )