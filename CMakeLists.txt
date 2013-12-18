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
set(CMAKE_CXX_FLAGS "-DBIN_PATH=${PROJECT_BINARY_DIR}/HSAIL-Tools ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-DOBJ_PATH=${PROJECT_BINARY_DIR}/test ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-I/usr/include/libdwarf ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -UNDEBUG")

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
include (EnsureLibHSAILisPresent)
include (EnsureELFisPresent)
include (EnsureLibDwarfisPresent)
ensure_llvm_is_present(${PROJECT_SOURCE_DIR} src/brig2llvm/compiler)
ensure_libHSAIL_is_present(${PROJECT_SOURCE_DIR} src/HSAIL-Tools)

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
CHECK_CXX_COMPILER_FLAG("-Werror -Wunknown-warning-option -Wno-return-type-c-linkage" SUPPORTS_NO_RET_C_LINK)
if (SUPPORTS_NO_RET_C_LINK)
  add_llvm_definitions(-Wno-return-type-c-linkage)
endif (SUPPORTS_NO_RET_C_LINK)

add_subdirectory(src)
add_subdirectory(src/brig2llvm)
add_subdirectory(src/hsa_runtime)
add_subdirectory(src/newcore)

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
set(LLVM_SRC_DIR
  ${PROJECT_SOURCE_DIR}/src/brig2llvm/compiler)

# use libHSAIL makefile to build libHSAIL
set(HSAIL_TOOLS_SRC_DIR
    ${PROJECT_SOURCE_DIR}/src/HSAIL-Tools/libHSAIL)
set(HSAIL_TOOLS_BINARY_DIR
    ${PROJECT_BINARY_DIR}/HSAIL-Tools)
Find_Package(Git)

SET( MAKE_OPTIONS
     " LLVM_SRC=${LLVM_SRC_DIR}"
     " LLVM_BUILD=${LLVM_BINARY_DIR}"
     " BUILD_DIR=${HSAIL_TOOLS_BINARY_DIR}"
     " CXX=${CMAKE_CXX_COMPILER}"
     " AR=${CMAKE_AR}"
     " CC=${CMAKE_C_COMPILER}"
     " CXXFLAGS=${CMAKE_CXX_FLAGS}"
     " LDFLAGS=${CMAKE_EXE_LINKER_FLAGS}")

add_custom_target( build_libhsail ALL
  DEPENDS brig2llvm
  COMMAND ${GIT_EXECUTABLE} pull https://github.com/HSAFoundation/HSAIL-Tools.git
  COMMAND ${CMAKE_MAKE_PROGRAM} ${MAKE_OPTIONS}
  WORKING_DIRECTORY ${HSAIL_TOOLS_SRC_DIR} )
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${HSAIL_TOOLS_BINARY_DIR})

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
target_link_libraries(brig_reader_test brig2llvm)

set(hsa_runtime_test_SOURCES
  test/hsa_runtime_test.cc
  test/llvm_shutdown.cc
  gtest/gtest-all.cc
  gtest/gtest_main.cc
  )
add_executable(hsa_runtime_test ${hsa_runtime_test_SOURCES})
target_link_libraries(hsa_runtime_test hsa brig2llvm)

set(vectorCopy_SOURCES demo/vectorCopy.cc)
add_executable(vectorCopy ${vectorCopy_SOURCES})
target_link_libraries(vectorCopy hsa brig2llvm)

set(fib_SOURCES demo/fib.cc)
add_executable(fib ${fib_SOURCES})
target_link_libraries(fib hsa brig2llvm)

set(barrierTest_SOURCES demo/barrierTest.cc)
add_executable(barrierTest ${barrierTest_SOURCES})
target_link_libraries(barrierTest hsa brig2llvm)

set(fcos_SOURCES demo/fcos.cc)
add_executable(fcos ${fcos_SOURCES})
target_link_libraries(fcos hsa brig2llvm)

set(brig_validate_SOURCES test/brig_validate.cc)
add_executable(brig_validate ${brig_validate_SOURCES})
target_link_libraries(brig_validate brig2llvm)

set(fibDebug_SOURCES demo/fibDebug.cc)
add_executable(fibDebug ${fibDebug_SOURCES})
target_link_libraries(fibDebug brig2llvm)

set(aqlDemo_SOURCES demo/aqlDemo.cc)
add_executable(aqlDemo ${aqlDemo_SOURCES})
target_link_libraries(aqlDemo newcore brig2llvm)

set(debug_SOURCES demo/debug.cc)
add_executable(debug ${debug_SOURCES})
target_link_libraries(debug hsa)

set(dumpDebugInfo_SOURCES demo/dumpDebugInfo.cc)
add_executable(dumpDebugInfo ${dumpDebugInfo_SOURCES})
target_link_libraries(dumpDebugInfo brig2llvm)

add_subdirectory(test)
