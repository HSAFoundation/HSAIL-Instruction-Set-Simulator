/* Copyright 2012 <MulticorewareInc> */

#ifndef CODEGEN_TEST_H_
#define CODEGEN_TEST_H_

namespace hsa {
namespace brig {

class Instruction1opNoDTLaneid: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_laneid[] = {
  "laneid $s1;"
};
BrigInstBase outputarray_instruction1opnodt_laneid[] = {
  {
    32,
    BrigEInstBase,
    BrigLaneId,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTDynwaveid: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_dynwaveid[] = {
  "dynwaveid $s1;"
};
BrigInstBase outputarray_instruction1opnodt_dynwaveid[] = {
  {
    32,
    BrigEInstBase,
    BrigDynWaveId,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTDispatchid: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_dispatchid[] = {
  "dispatchid $s1;"
};
BrigInstBase outputarray_instruction1opnodt_dispatchid[] = {
  {
    32,
    BrigEInstBase,
    BrigDispatchId,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTCu: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_cu[] = {
  "cu $s1;"
};
BrigInstBase outputarray_instruction1opnodt_cu[] = {
  {
    32,
    BrigEInstBase,
    BrigCU,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTWorkdim: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_workdim[] = {
  "workdim $s1;"
};
BrigInstBase outputarray_instruction1opnodt_workdim[] = {
  {
    32,
    BrigEInstBase,
    BrigWorkDim,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTWorkitemidFlat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_workitemidflat[] = {
  "workitemid_flat $s1;"
};
BrigInstBase outputarray_instruction1opnodt_workitemidflat[] = {
  {
    32,
    BrigEInstBase,
    Brigworkitemidflat,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTWorkitemaidFlat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_workitemaidflat[] = {
  "workitemaid_flat $s1;"
};
BrigInstBase outputarray_instruction1opnodt_workitemaidflat[] = {
  {
    32,
    BrigEInstBase,
    Brigworkitemaidflat,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTClock: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_clock[] = {
  "clock $d1;"
};
BrigInstBase outputarray_instruction1opnodt_clock[] = {
  {
    32,
    BrigEInstBase,
    BrigClock,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTDebugtrap: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_debugtrap[] = {
  "debugtrap $s0;",
  "debugtrap $s4;",
  "debugtrap 0;",
  "debugtrap 61;"
  "debugtrap WAVESIZE;"
};
BrigInstBase outputarray_instruction1opnodt_debugtrap[] = {
  {
    32,
    BrigEInstBase,
    BrigDebugtrap, 
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

}  // namespace brig
}  // namespace hsa
#endif //CODEGEN_TEST_H_
