INCLUDE(CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("
\#include <libdwarf.h>
int main(int argc, char **argv) {
        return 0;
      }
" HAS_LIBDWARF)

if ( NOT HAS_LIBDWARF )
  MESSAGE(FATAL_ERROR "libdwarf.h is missing")
endif()

CHECK_CXX_SOURCE_COMPILES("
\#include <dwarf.h>
int main(int argc, char **argv) {
        return 0;
      }
" HAS_DWARF)

if ( NOT HAS_DWARF )
  MESSAGE(FATAL_ERROR "dwarf.h is missing")
endif()
