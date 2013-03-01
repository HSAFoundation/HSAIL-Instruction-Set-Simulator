INCLUDE(CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("
int main(int argc, char **argv) {
        volatile unsigned long val = 1;
        __sync_synchronize();
        __sync_val_compare_and_swap(&val, 1, 0);
        __sync_add_and_fetch(&val, 1);
        __sync_sub_and_fetch(&val, 1);
        return 0;
      }
" HAS_ATOMICS)

if ( NOT HAS_ATOMICS AND IS_X86 )
  message(STATUS "Warning: Atomics missing, trying to enable prescott features")
  set(CMAKE_C_FLAGS "-march=prescott ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "-march=prescott ${CMAKE_CXX_FLAGS}")
endif()
