project(hsail2brig)
cmake_minimum_required(VERSION 2.6.2)
find_package(FLEX)
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DGTEST_HAS_PTHREAD=0)
flex_target(lexer src/lexer_rules.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
	COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
set(hsail2brig_test_SOURCES
    test/lexer_test.cc
    test/parser_test.cc
    test/codegen_test.cc
	  gmock/gmock_main.cc
	  gmock/gmock-gtest-all.cc
	  src/parser.cc
    src/brig_buffer.cc
    src/parser_wrapper.cc
    src/lexer_wrapper.cc
    src/error_reporter.cc
    src/context.cc
	  ${FLEX_lexer_OUTPUTS}
	  )
add_executable(hsail2brig_test ${hsail2brig_test_SOURCES})
option(ENABLE_BRIG_FE "Build LLVM frontend for BRIG" OFF)
if (ENABLE_BRIG_FE)

   # obtain specific information about llvm setup
   SET(LOCAL_LLVM_INCLUDE src/brig2llvm/compiler/include)

  SET(PROJ_SEARCH_PATH "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/${LOCAL_LLVM_INCLUDE}" "${PROJECT_BINARY_DIR}/${LOCAL_LLVM_INCLUDE}" "${PROJECT_SOURCE_DIR}/compiler/utils/unittest/googletest/include")
  include_directories( ${PROJ_SEARCH_PATH} )

  include("${PROJECT_BINARY_DIR}/src/brig2llvm/compiler/share/llvm/cmake/LLVMConfig.cmake")
  include(HandleLLVMOptions)

  add_subdirectory(src/brig2llvm)
  set(brig2llvm_test_SOURCES
      test/brig2llvm_test.cc
      gtest/gtest-all.cc
      gtest/gtest_main.cc
     )
  add_executable(brig2llvm_test ${brig2llvm_test_SOURCES})
  target_link_libraries(brig2llvm_test brig2llvm)
endif (ENABLE_BRIG_FE)
