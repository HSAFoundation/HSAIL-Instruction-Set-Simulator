/* Copyright 2012 <MulticorewareInc> */

#ifndef SYSCALL_TEST_H_
#define SYSCALL_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestSyscall: public ::testing::TestWithParam<int>
{

};

struct SyscallTest{
  const char* str;
  BrigInstBase ref;
};

struct SyscallTest syscall_pair[4] = {
//syscall_pair[0]
  {
    "syscall $s1, 3, $s2, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSysCall,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + reg_size + immed_size, 
      RoundUp8(operand_offset + reg_size) + reg_size * 2 + immed_size
      }
    }
  },
//syscall_pair[1]
  {
    "syscall $s1, 3, 4, 5, 6;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSysCall,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size), 
      RoundUp8(RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + 
        immed_size) + immed_size) + immed_size), 
      }
    }
  },
//syscall_pair[2]
  {
    "syscall $s1, 3, WAVESIZE, WAVESIZE, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSysCall,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size
      }
    }
  },
//syscall_pair[3]
  {
    "syscall $s1, 3, $s2, 5, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSysCall,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size) + 
        immed_size
      }
    }
  }
};

class TestSyscallInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_syscall_invalid[] = {
  "syscall $s1, 3, $s2, $s3, $s4",
  "syscall $s1, 3, $s2, $s3, $s4, $s5;",
  "syscall $s1, 3, $s2, $s3;",
  "syscall 9, 3, $s2, $s3, $s4;",
  "syscall $s1, $s5, $s2, $s3, $s4;",
};

}  // namespace brig
}  // namespace hsa
#endif //SYSCALL_TEST_H_
