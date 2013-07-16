cmake_minimum_required( VERSION 2.8 )
project (BRIG2LLVM)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake")
MESSAGE("Module path: ${CMAKE_MODULE_PATH}")

# obtain specific information about llvm setup
SET(LOCAL_LLVM_INCLUDE compiler/include)

SET(PROJ_SEARCH_PATH "${PROJECT_SOURCE_DIR}/include"
"${PROJECT_SOURCE_DIR}/${LOCAL_LLVM_INCLUDE}"
"${PROJECT_BINARY_DIR}/${LOCAL_LLVM_INCLUDE}"
"${PROJECT_SOURCE_DIR}/compiler/utils/unittest/googletest/include")
include_directories( ${PROJ_SEARCH_PATH} )

LINK_DIRECTORIES( ${LLVM_LIB_DIR} )

#add_subdirectory(compiler)
#MESSAGE("INCLUDE SEARCH DIRECTORIES: ${PROJ_SEARCH_PATH}")
# library
#include("${PROJECT_BINARY_DIR}/compiler/share/llvm/cmake/LLVMConfig.cmake")
#include(HandleLLVMOptions)

set(LLVM_LINK_COMPONENTS core jit mcjit nativecodegen debuginfo)
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
  hsailasm_wrapper.cc
  s_fma.c)
llvm_config(brig2llvm ${LLVM_LINK_COMPONENTS})
