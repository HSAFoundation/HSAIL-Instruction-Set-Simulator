INCLUDE(CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("
\#include <elf.h>
int main(int argc, char **argv) {
        return 0;
      }
" HAS_ELF)

if ( NOT HAS_ELF )
  MESSAGE(FATAL_ERROR "elf.h is missing")
endif()
