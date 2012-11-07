project(hsail2brig)
cmake_minimum_required(VERSION 2.8)
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DGTEST_HAS_PTHREAD=0)
if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse")
endif (CMAKE_COMPILER_IS_GNUCXX)
if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpmath=sse")
endif (CMAKE_COMPILER_IS_GNUCC)
set(CMAKE_C_FLAGS "-std=gnu99 -msse3 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-msse3 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-DTEST_PATH=${PROJECT_SOURCE_DIR}/test ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-DBIN_PATH=${PROJECT_BINARY_DIR}/test ${CMAKE_CXX_FLAGS}")

set(LLVM_TARGETS_TO_BUILD "X86")
#set(LLVM_BUILD_TOOLS off)
set(LLVM_INCLUDE_TOOLS off)
set(LLVM_INCLUDE_EXAMPLES off)
set(LLVM_INCLUDE_TESTS on)

# obtain specific information about llvm setup
SET(LOCAL_LLVM_INCLUDE src/brig2llvm/compiler/include)

SET(LLVM_REQUIRES_EH "true")

# obtain specific information about llvm setup
SET(LOCAL_LLVM_INCLUDE src/brig2llvm/compiler/include)
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/src/brig2llvm/scripts/cmake")
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")
include (EnsureLLVMisPresent)
ensure_llvm_is_present(${PROJECT_SOURCE_DIR} src/brig2llvm/compiler)

SET(PROJ_SEARCH_PATH
"${PROJECT_SOURCE_DIR}/include"
"${PROJECT_SOURCE_DIR}/${LOCAL_LLVM_INCLUDE}"
"${PROJECT_BINARY_DIR}/${LOCAL_LLVM_INCLUDE}"
"${PROJECT_BINARY_DIR}/src"
"${PROJECT_SOURCE_DIR}/src"
"${PROJECT_BINARY_DIR}/src")
include_directories( ${PROJ_SEARCH_PATH} )

LINK_DIRECTORIES( ${LLVM_LIB_DIR} )

add_subdirectory(src/brig2llvm/compiler)

include("${PROJECT_BINARY_DIR}/src/brig2llvm/compiler/share/llvm/cmake/LLVMConfig.cmake")
include(HandleLLVMOptions)

add_subdirectory(src)
add_subdirectory(src/brig2llvm)
add_subdirectory(src/hsa_runtime)

option(ENABLE_TEST_CASE "Run test cases" OFF)
if (ENABLE_TEST_CASE)
set(hsail2brig_test_SOURCES
    test/lexer_test.cc
    test/parser_test.cc
    test/codegen_test.cc
    test/codegen_validate.cc
    test/hsail2brig/Atom_test.cc
    test/hsail2brig/AtomicImage_test.cc
    test/hsail2brig/Block_test.cc
    test/hsail2brig/Branch_test.cc
    #test/hsail2brig/Call_test.cc
    test/hsail2brig/Cmp_test.cc
    test/hsail2brig/Cvt_test.cc
    test/hsail2brig/Extension_test.cc
    test/hsail2brig/FileDecl_test.cc
    test/hsail2brig/GlobalDecl_test.cc
    test/hsail2brig/ImageInstruction_test.cc
    test/hsail2brig/Instruction0_test.cc
    test/hsail2brig/Instruction1_test.cc
    test/hsail2brig/Instruction2_test.cc
    test/hsail2brig/Instruction3_test.cc
    test/hsail2brig/Instruction4_test.cc
    test/hsail2brig/Instruction5_test.cc
    test/hsail2brig/Ld_test.cc
    test/hsail2brig/Location_test.cc
    test/hsail2brig/Mov_test.cc
    test/hsail2brig/Mul_test.cc
    test/hsail2brig/Pragma_test.cc
    test/hsail2brig/Query_test.cc
    test/hsail2brig/St_test.cc
    test/hsail2brig/Syscall_test.cc
    test/hsail2brig/Version_test.cc
    #test/hsail2brig/Control_test.cc
    testcase/globaldecl_test.cc
    testcase/version_test.cc
    testcase/operation_test.cc
    testcase/operand_test.cc
    gmock/gmock_main.cc
    gmock/gmock-gtest-all.cc
    )
else (ENABLE_TEST_CASE)
  set(hsail2brig_test_SOURCES
    test/lexer_test.cc
    test/parser_test.cc
    test/codegen_test.cc
    test/codegen_validate.cc
    test/hsail2brig/Atom_test.cc
    test/hsail2brig/AtomicImage_test.cc
    test/hsail2brig/Branch_test.cc
    test/hsail2brig/Block_test.cc
    #test/hsail2brig/Call_test.cc
    test/hsail2brig/Cmp_test.cc
    test/hsail2brig/Cvt_test.cc
    test/hsail2brig/Extension_test.cc
    test/hsail2brig/FileDecl_test.cc
    test/hsail2brig/GlobalDecl_test.cc
    test/hsail2brig/ImageInstruction_test.cc
    test/hsail2brig/Instruction0_test.cc
    test/hsail2brig/Instruction1_test.cc
    test/hsail2brig/Instruction2_test.cc
    test/hsail2brig/Instruction3_test.cc
    test/hsail2brig/Instruction4_test.cc
    test/hsail2brig/Instruction5_test.cc
    test/hsail2brig/Ld_test.cc
    test/hsail2brig/Location_test.cc
    test/hsail2brig/Mov_test.cc
    test/hsail2brig/Mul_test.cc
    test/hsail2brig/Pragma_test.cc
    test/hsail2brig/Query_test.cc
    test/hsail2brig/St_test.cc
    test/hsail2brig/Syscall_test.cc
    test/hsail2brig/Version_test.cc
    #test/hsail2brig/Control_test.cc
    gmock/gmock_main.cc
    gmock/gmock-gtest-all.cc
    )
endif (ENABLE_TEST_CASE)

add_executable(hsail2brig_test ${hsail2brig_test_SOURCES})
target_link_libraries(hsail2brig_test hsa2brig)

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

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}" )

set(brig2llvm_test_SOURCES
  test/brig2llvm_test.cc
  test/llvm_shutdown.cc
  gtest/gtest-all.cc
  gtest/gtest_main.cc
  )
add_executable(brig2llvm_test ${brig2llvm_test_SOURCES})
target_link_libraries(brig2llvm_test brig2llvm)

set(brig_runtime_test_SOURCES
  test/brig_runtime_test.cc
  gtest/gtest-all.cc
  gtest/gtest_main.cc
  )
add_executable(brig_runtime_test ${brig_runtime_test_SOURCES})
target_link_libraries(brig_runtime_test brig2llvm)

set(brig_reader_test_SOURCES
  test/brig_reader_test.cc
  test/llvm_shutdown.cc
  gtest/gtest-all.cc
  gtest/gtest_main.cc
  )
add_executable(brig_reader_test ${brig_reader_test_SOURCES})
target_link_libraries(brig_reader_test hsa2brig brig2llvm)

set(hsa_runtime_test_SOURCES
  test/hsa_runtime_test.cc
  test/llvm_shutdown.cc
  gtest/gtest-all.cc
  gtest/gtest_main.cc
  )
add_executable(hsa_runtime_test ${hsa_runtime_test_SOURCES})
target_link_libraries(hsa_runtime_test hsa hsa2brig brig2llvm)

set(hsaAssembler_SOURCES bin/hsaAssembler)
add_executable(hsaAssembler ${hsaAssembler_SOURCES})
target_link_libraries(hsaAssembler hsa2brig brig2llvm)

set(vectorCopy_SOURCES demo/vectorCopy.cc)
add_executable(vectorCopy ${vectorCopy_SOURCES})
target_link_libraries(vectorCopy hsa hsa2brig brig2llvm)

set(fib_SOURCES demo/fib.cc)
add_executable(fib ${fib_SOURCES})
target_link_libraries(fib hsa hsa2brig brig2llvm)

set(fcos_SOURCES demo/fcos.cc)
add_executable(fcos ${fcos_SOURCES})
target_link_libraries(fcos hsa hsa2brig brig2llvm)

set(debug_SOURCES demo/debug.cc)
add_executable(debug ${debug_SOURCES})
target_link_libraries(debug hsa)

add_subdirectory(test)
