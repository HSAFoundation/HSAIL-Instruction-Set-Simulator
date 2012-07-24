project(foo)
cmake_minimum_required(VERSION 2.6.2)
find_package(FLEX)
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DGTEST_HAS_PTHREAD=0)
flex_target(lexer example.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
	COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
set(foo_SOURCES
	test/hsail_test.cc
    test/codegen_test.cc
    test/parser_test.cc
    test/hsail_test.cc
	gtest/gtest-all.cc
	gtest/gtest_main.cc
	parser.cc
	${FLEX_lexer_OUTPUTS}
	)
add_executable(foo ${foo_SOURCES})
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
