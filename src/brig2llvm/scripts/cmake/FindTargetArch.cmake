INCLUDE(CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("
#if !defined(__i386__) && !defined(__x86_64__)
#error
#endif
int main(int argc, char **argv) { return 0; }
" IS_X86)

CHECK_CXX_SOURCE_COMPILES("
#if !defined(__arm__)
#error
#endif
int main(int argc, char **argv) { return 0; }
" IS_ARM)

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpmachine
  RESULT_VARIABLE TT_RV
  OUTPUT_VARIABLE LLVM_HOST_TRIPLE
  OUTPUT_STRIP_TRAILING_WHITESPACE)

if (${LLVM_HOST_TRIPLE} STREQUAL "arm-linux-gnueabihf")
  set(LLVM_HOST_TRIPLE "armv7-linux-gnueabihf" CACHE STRING "" )
  set(LLVM_DEFAULT_TARGET_TRIPLE "armv7-linux-gnueabihf" CACHE STRING "" )
else ()
  set(LLVM_HOST_TRIPLE "${LLVM_HOST_TRIPLE}" CACHE STRING "" )
  set(LLVM_DEFAULT_TARGET_TRIPLE "${LLVM_HOST_TRIPLE}" CACHE STRING "" )
endif (${LLVM_HOST_TRIPLE} STREQUAL "arm-linux-gnueabihf")

