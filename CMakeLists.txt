project(hsail2brig)
cmake_minimum_required(VERSION 2.8)
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

  SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/src/brig2llvm/scripts/cmake")
  MESSAGE("Module path: ${CMAKE_MODULE_PATH}")
  include (EnsureLLVMisPresent)

  ensure_llvm_is_present(${PROJECT_SOURCE_DIR} src/brig2llvm/compiler)

  set(LLVM_TARGETS_TO_BUILD "X86")
  #set(LLVM_BUILD_TOOLS off)
  set(LLVM_INCLUDE_TOOLS off)
  set(LLVM_INCLUDE_EXAMPLES off)
  set(LLVM_INCLUDE_TESTS on)

  # obtain specific information about llvm setup
  SET(LOCAL_LLVM_INCLUDE src/brig2llvm/compiler/include)

  # setup compilation environment
  if (UNIX)
    SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/build/${CMAKE_CFG_INTDIR}/${CMAKE_BUILD_TYPE}/bin" )
    SET(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/build/${CMAKE_CFG_INTDIR}/${CMAKE_BUILD_TYPE}/lib" )
  else (UNIX)
    SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/build/${CMAKE_BUILD_TYPE}/bin" )
    SET(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/build/${CMAKE_BUILD_TYPE}/lib" )
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})

    SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})
    SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})

    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})
    MESSAGE("(DEBUG|RELEASE) output changed to path:", "${EXECUTABLE_OUTPUT_PATH}")

  endif (UNIX)

  SET(PROJ_SEARCH_PATH "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/${LOCAL_LLVM_INCLUDE}" "${PROJECT_BINARY_DIR}/${LOCAL_LLVM_INCLUDE}" "${PROJECT_SOURCE_DIR}/compiler/utils/unittest/googletest/include")
  include_directories( ${PROJ_SEARCH_PATH} )

  LINK_DIRECTORIES( ${LLVM_LIB_DIR} )

  add_subdirectory(src/brig2llvm/compiler)

  # obtain specific information about llvm setup
  SET(LOCAL_LLVM_INCLUDE src/brig2llvm/compiler/include)

  include("${PROJECT_BINARY_DIR}/src/brig2llvm/compiler/share/llvm/cmake/LLVMConfig.cmake")
  include(HandleLLVMOptions)

  SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}" )

  add_subdirectory(src/brig2llvm)
  set(brig2llvm_test_SOURCES
      test/brig2llvm_test.cc
      gtest/gtest-all.cc
      gtest/gtest_main.cc
     )
  add_executable(brig2llvm_test ${brig2llvm_test_SOURCES})
  target_link_libraries(brig2llvm_test brig2llvm)
endif (ENABLE_BRIG_FE)
