project(hsail2brig)
cmake_minimum_required(VERSION 2.6.2)
find_package(FLEX)
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DGTEST_HAS_PTHREAD=0)
flex_target(lexer src/example.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
	COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
set(hsail2brig_test_SOURCES
    test/mock_lexer_test.cc
    test/parser_wrapper_test.cc
    test/lexer_wrapper_test.cc    
    test/codegen_test.cc
    test/parser_test.cc
    test/hsail_test.cc
	  gmock/gmock_main.cc
	  gmock/gmock-gtest-all.cc
	  src/parser.cc
    src/brig_buffer.cc
    src/parser_wrapper.cc
    src/lexer_wrapper.cc
    src/hsail2brig.cc
    src/error_reporter.cc
	  ${FLEX_lexer_OUTPUTS}
	  )
add_executable(hsail2brig_test ${hsail2brig_test_SOURCES})
option(ENABLE_BRIG_FE "Build LLVM frontend for BRIG" OFF)
if (ENABLE_BRIG_FE)
  add_subdirectory(src/brig2llvm)
  set(brig2llvm_test_SOURCES
      test/brig2llvm_test.cc
      gtest/gtest-all.cc
      gtest/gtest_main.cc
     )
  add_executable(brig2llvm_test ${brig2llvm_test_SOURCES})
  target_link_libraries(brig2llvm_test brig2llvm)
endif (ENABLE_BRIG_FE)
