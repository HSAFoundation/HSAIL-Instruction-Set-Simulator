project(hsail2brig)
cmake_minimum_required(VERSION 2.8)
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DGTEST_HAS_PTHREAD=0)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/src/brig2llvm/scripts/cmake")
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")
include (FindTargetArch)

if (IS_X86)
  if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse")
  endif (CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpmath=sse")
  endif (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "-msse3 ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "-msse3 ${CMAKE_CXX_FLAGS}")
endif(IS_X86)

set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-DTEST_PATH=${PROJECT_SOURCE_DIR}/test ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-DBIN_PATH=${PROJECT_BINARY_DIR}/test ${CMAKE_CXX_FLAGS}")

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

if (IS_X86)
  set(LLVM_TARGET_ARCH "X86" CACHE STRING "")
  set(LLVM_TARGETS_TO_BUILD "X86" CACHE STRING "")
elseif (IS_ARM)
  set(LLVM_TARGET_ARCH "ARM" CACHE STRING "")
  set(LLVM_TARGETS_TO_BUILD "ARM" CACHE STRING "")
endif (IS_X86)
set(LLVM_BUILD_TOOLS off CACHE STRING "")
set(LLVM_INCLUDE_TOOLS off CACHE STRING "")
set(LLVM_INCLUDE_EXAMPLES off CACHE STRING "")
set(LLVM_INCLUDE_TESTS off CACHE STRING "")
set(LLVM_INCLUDE_RUNTIME off CACHE STRING "")
set(LLVM_BINARY_DIR "${PROJECT_BINARY_DIR}/src/brig2llvm/compiler")

SET(LLVM_REQUIRES_EH "true")

# obtain specific information about llvm setup
SET(LOCAL_LLVM_INCLUDE src/brig2llvm/compiler/include)

include (EnsureAtomicOps)
include (EnsureLLVMisPresent)
ensure_llvm_is_present(${PROJECT_SOURCE_DIR} src/brig2llvm/compiler)

SET(PROJ_SEARCH_PATH
"${PROJECT_SOURCE_DIR}/include"
"${PROJECT_SOURCE_DIR}/${LOCAL_LLVM_INCLUDE}"
"${PROJECT_BINARY_DIR}/${LOCAL_LLVM_INCLUDE}"
"${PROJECT_BINARY_DIR}/src")
include_directories( ${PROJ_SEARCH_PATH} )

LINK_DIRECTORIES( ${LLVM_LIB_DIR} )

add_subdirectory(src/brig2llvm/compiler)

include("${PROJECT_BINARY_DIR}/src/brig2llvm/compiler/share/llvm/cmake/LLVMConfig.cmake")
include(HandleLLVMOptions)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-Werror -Wunknown-warning -Wno-return-type-c-linkage" SUPPORTS_NO_RET_C_LINK)
if (SUPPORTS_NO_RET_C_LINK)
  add_llvm_definitions(-Wno-return-type-c-linkage)
endif (SUPPORTS_NO_RET_C_LINK)

add_subdirectory(src)
add_subdirectory(src/brig2llvm)
add_subdirectory(src/hsa_runtime)

set(hsail2brig_test_SOURCES
    test/lexer_test.cc
    test/parser_test.cc
    test/codegen_validate.cc
    test/hsail2brig/Atom_test.cc
    test/hsail2brig/AtomicImage_test.cc
    test/hsail2brig/Branch_test.cc
    test/hsail2brig/Block_test.cc
    test/hsail2brig/Call_test.cc
    test/hsail2brig/Comment_test.cc
    test/hsail2brig/Control_test.cc
    test/hsail2brig/Cmp_test.cc
    test/hsail2brig/Cvt_test.cc
    test/hsail2brig/Extension_test.cc
    test/hsail2brig/Example1_test.cc
    test/hsail2brig/Example2_test.cc
    test/hsail2brig/Example3_test.cc
    test/hsail2brig/Example4_test.cc
    test/hsail2brig/Example5_test.cc
    test/hsail2brig/Example6_test.cc
    test/hsail2brig/FileDecl_test.cc
    test/hsail2brig/GlobalDecl_test.cc
    test/hsail2brig/ImageInstruction_test.cc
    test/hsail2brig/Instruction0_test.cc
    test/hsail2brig/Instruction1_test.cc
    test/hsail2brig/Instruction2_test.cc
    test/hsail2brig/Instruction3_test.cc
    test/hsail2brig/Instruction4_test.cc
    test/hsail2brig/Instruction5_test.cc
    test/hsail2brig/Kernel_test.cc
    test/hsail2brig/Ld_test.cc
    test/hsail2brig/Label_test.cc
    test/hsail2brig/Location_test.cc
    test/hsail2brig/Mov_test.cc
    test/hsail2brig/Mul_test.cc
    test/hsail2brig/Pragma_test.cc
    test/hsail2brig/Query_test.cc
    test/hsail2brig/St_test.cc
    test/hsail2brig/Syscall_test.cc
    test/hsail2brig/Version_test.cc
    test/hsail2brig/Operand_test.cc
    testcase/atom_test.cc
    testcase/bar_test.cc
    testcase/branch_test.cc
    testcase/cmp_test.cc
    testcase/cvt_test.cc
    testcase/declare_test.cc
    testcase/directive_test.cc
    testcase/image_test.cc
    testcase/instruction0_test.cc
    testcase/instruction1_test.cc
    testcase/instruction2_test.cc
    testcase/instruction3_test.cc
    testcase/instruction4_test.cc
    testcase/instruction5_test.cc
    testcase/label_test.cc
    testcase/ld_test.cc
    testcase/location_test.cc
    testcase/mov_test.cc
    testcase/mul_test.cc
    testcase/operand_test.cc
    testcase/query_test.cc
    testcase/ret_test.cc
    testcase/segp_test.cc
    testcase/st_test.cc
    testcase/sync_test.cc
    testcase/syscall_test.cc
    testcase/version_test.cc
    gmock/gmock_main.cc
    gmock/gmock-gtest-all.cc
    )

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

set(brig_validate_SOURCES test/brig_validate.cc)
add_executable(brig_validate ${brig_validate_SOURCES})
target_link_libraries(brig_validate hsa2brig brig2llvm)

set(debug_SOURCES demo/debug.cc)
add_executable(debug ${debug_SOURCES})
target_link_libraries(debug hsa)

add_subdirectory(test)
