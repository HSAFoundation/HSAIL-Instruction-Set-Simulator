project(foo)
cmake_minimum_required(VERSION 2.6.2)
find_package(FLEX)
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DGTEST_HAS_PTHREAD=0)
flex_target(lexer example.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
	COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
set(foo_SOURCES
	test/hsail_test.cc
	gtest/gtest-all.cc
	gtest/gtest_main.cc
	${FLEX_lexer_OUTPUTS}
	)
add_executable(foo ${foo_SOURCES})
