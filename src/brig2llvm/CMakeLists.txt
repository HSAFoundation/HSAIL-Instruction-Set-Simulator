cmake_minimum_required( VERSION 2.8 )
project (BRIG2LLVM)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake")
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")

# obtain specific information about llvm setup
SET(LOCAL_LLVM_INCLUDE compiler/include)
SET(LOCAL_LIBHSAIL_INCLUDE HSAIL-Tools/libHSAIL/libHSAIL)
SET(LOCAL_LIBHSAIL_LIB ${PROJECT_SOURCE_DIR}/HSAIL-Tools/libHSAIL/build_linux)

SET(PROJ_SEARCH_PATH 
    "${PROJECT_SOURCE_DIR}/include" 
    "${PROJECT_SOURCE_DIR}/${LOCAL_LLVM_INCLUDE}"
    "${PROJECT_BINARY_DIR}/${LOCAL_LLVM_INCLUDE}"
    "${PROJECT_SOURCE_DIR}/${LOCAL_LIBHSAIL_INCLUDE}"
    "${PROJECT_SOURCE_DIR}/compiler/utils/unittest/googletest/include")
include_directories( ${PROJ_SEARCH_PATH} )

LINK_DIRECTORIES( ${LLVM_LIB_DIR} ${LOCAL_LIBHSAIL_LIB} )

#add_subdirectory(compiler)
#MESSAGE("INCLUDE SEARCH DIRECTORIES: ${PROJ_SEARCH_PATH}")
# library
#include("${PROJECT_BINARY_DIR}/compiler/share/llvm/cmake/LLVMConfig.cmake")
#include(HandleLLVMOptions)

# use libHSAIL makefile to build libHSAIL
set(LibHSAIL_BUILD_DIR
    ${PROJECT_SOURCE_DIR}/HSAIL-Tools/libHSAIL)

set(LLVM_SRC_DIR
    ${PROJECT_SOURCE_DIR}/compiler)

set(LLVM_BUILD_DIR
    ${PROJECT_BINARY_DIR}/compiler)

add_custom_target(build_libhsail ALL
                  COMMAND ${CMAKE_MAKE_PROGRAM} -j LLVM_SRC=${LLVM_SRC_DIR} LLVM_BUILD=${LLVM_BUILD_DIR} 
                  WORKING_DIRECTORY ${LibHSAIL_BUILD_DIR}                  
                  )



set(LLVM_LINK_COMPONENTS core jit mcjit nativecodegen)
add_llvm_library(brig2llvm
  brig2llvm.cc
  brig_module.cc
  brig_function.cc
  brig_symbol.cc
  brig_control_block.cc
  brig_inst_helper.cc
  brig_engine.cc
  brig_runtime.cc
  brig_reader.cc
  brig_writer.cc
  s_fma.c)
llvm_config(brig2llvm ${LLVM_LINK_COMPONENTS})
