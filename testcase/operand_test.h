/* Copyright 2012 <MulticorewareInc> */

#ifndef OPERAND_TEST_H_
#define OPERAND_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

// ------------------ BASIC LEXER TESTS INPUT/OUTPUT -----------------
class IntegerConstant : public ::testing::TestWithParam<int>
{

};

struct IntegerConstantTest{
  const char* str;
  unsigned long ref;
};

struct IntegerConstantTest integerconstant_pair[21] = {
//integerconstant_pair[0]
  {
     "0",
     0
  },
//integerconstant_pair[1]
  {
      "1234567",
      1234567
  },
//integerconstant_pair[2]
  {
      "2147483647",
      2147483647
  },
//integerconstant_pair[3]
  {
      "00",
     0
  },
//integerconstant_pair[4]
  {
      "000",
    0
  },
//integerconstant_pair[5]
  {
       "001",
   1
  },
//integerconstant_pair[6]
  {
      "01234567",
   342391
  },
//integerconstant_pair[7]
  {
    "0x00",
     0
  },
//integerconstant_pair[8]
  {
    "0x1234567",
     19088743
  },
//integerconstant_pair[9]
  {
    "0x12ab",
     4779
  },
//integerconstant_pair[10]
  {
    "0x12abEF",
     1223663
  },
//integerconstant_pair[11]
  {
    "0x0aF",
     175
  },
//integerconstant_pair[12]
  {
    "0x0ab",
     171
  },
//integerconstant_pair[13]
  {
    "0x0FF",
     255
  },
//integerconstant_pair[14]
  {
    "0X00",
     0
  },
//integerconstant_pair[15]
  {
    "0X1234567",
     19088743
  },
//integerconstant_pair[16]
  {
    "0X12ab",
     4779
  },
//integerconstant_pair[17]
  {
    "0X12abEF",
    1223663
  },
//integerconstant_pair[18]
  {
    "0X0aF",
    175
  },
//integerconstant_pair[19]
  {
    "0X0ab",
    171
  },
//integerconstant_pair[20]
  {
    "0X0FF",
    255
  }
};

class CRegisterValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_cregister[] = {
  "$c0",
  "$c1",
  "$c2",
  "$c3",
  "$c4",
  "$c5",
  "$c6",
  "$c7"
};

class SRegisterValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_sregister[] = {
  "$s0",
  "$s1",
  "$s2",
  "$s3",
  "$s4",
  "$s5",
  "$s6",
  "$s7",
  "$s8",
  "$s9",
  "$s10",
  "$s11",
  "$s12",
  "$s13",
  "$s14",
  "$s127"
};

class DRegisterValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_dregister[] = {
  "$d0",
  "$d1",
  "$d2",
  "$d3",
  "$d4",
  "$d5",
  "$d6",
  "$d63"
};

class QRegisterValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_qregister[] = {
  "$q0",
  "$q1",
  "$q2",
  "$q3",
  "$q4",
  "$q5",
  "$q6",
  "$q31"
};

class LabelValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_label[] = {
  "@Go_to_this_01",
  "@go_to_this",
  "@GO_TO_THIS",
  "@_go_to_this",
  "@_Go_to_this",
  "@_GO_TO_THIS",
  "@_01",
  "@___",
  "@gotothis"
};

class LocalIdValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_localid[] = {
  "%Test_id_123",
  "%test_id_123",
  "%TEST_ID_123",
  "%TestId",
  "%Test123",
  "%_123",
  "%__",
  "%Test_id",
  "%test123"
};

class GlobalIdValidInput : public ::testing::TestWithParam<int>
{

};

std::string input_globalid[] = {
  "&Test_global_id_123",
  "&test_global_id_123",
  "&TEST_GLOBAL_ID_123",
  "&test_global_id",
  "&Test123",
  "&_123",
  "&__",
  "&Test_id",
  "&test123"
};

// ------------------ BASIC CODEGEN TESTS INPUT/OUTPUT -----------------

class RegisterOperand : public ::testing::TestWithParam<int>
{

};

struct RegisterOperandTest{
  const char* str;
  BrigOperandReg ref;
};

struct RegisterOperandTest registeroperand_pair[32] = {
//registeroperand_pair[0]
  {
    "$c0",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[1]
 {
    "$c1",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[2]
 {
    "$c2",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[3]
 {
    "$c3",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[4]
 {
    "$c4",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[5]
 {
    "$c5",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[6]
 {
    "$c6",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[7]
 {
    "$c7",
     {
      reg_size,
      BrigEOperandReg,
      Brigb1,
      0,
      string_offset
    }
  },
//registeroperand_pair[8]
 {
    "$s0",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[9]
 {
    "$s1",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[10]
 {
    "$s2",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[11]
 {
    "$s3",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[12]
 {
    "$s4",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[13]
 {
    "$s5",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[14]
 {
    "$s6",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[15]
 {
    "$s15",
    {
      reg_size,
      BrigEOperandReg,
      Brigb32,
      0,
      string_offset
    }
  },
//registeroperand_pair[16]
 {
    "$d0",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[17]
 {
    "$d1",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[18]
 {
    "$d2",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[19]
 {
    "$d3",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[20]
 {
    "$d4",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[21]
 {
    "$d5",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[22]
 {
    "$d6",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[23]
 {
    "$d7",
    {
      reg_size,
      BrigEOperandReg,
      Brigb64,
      0,
      string_offset
    }
  },
//registeroperand_pair[24]
 {
    "$q0",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[25]
 {
    "$q1",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[26]
 {
    "$q2",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[27]
 {
    "$q3",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[28]
 {
    "$q4",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[29]
 {
    "$q5",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[30]
 {
    "$q6",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  },
//registeroperand_pair[31]
 {
    "$q7",
    {
      reg_size,
      BrigEOperandReg,
      Brigb128,
      0,
      string_offset
    }
  }
};

class RegOperandInvalidIn : public ::testing::TestWithParam<int>
{

};

std::string invalidinput_registercodegen[] = {
  // "$c16","$s32","$d32","$q16" etc.
  // should be identify as wrong inputs according to the new version PRM
  "$c8",
  "$7",
  "$s",
  "$d64",
  "$d",
  "$q32"
};

class IntegerOperand : public ::testing::TestWithParam<int>
{

};

struct IntegerOperandTest{
  const char* str;
  BrigOperandImmed ref;
};

struct IntegerOperandTest integer_codegen_pair[18] = {
//integer_cogengen_pair[0]
  {
    "0",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  }
  },
 //integer_cogengen_pair[1]
  {
    "+0",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  }
  },
//integer_cogengen_pair[2]
  {
    "-0",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  }
  },
//integer_cogengen_pair[3]
  {
    "10",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 10 }
  }
  },
//integer_cogengen_pair[4]
  {
    "+19",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 19 }
  }
  },
//integer_cogengen_pair[5]
  {
    "-99",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { -99 }
  }
  },
//integer_cogengen_pair[6]
  {
    "-1234567",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { -1234567 }
  }
  },
//integer_cogengen_pair[7]
  {  
    "-2147483648",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { -2147483648}
  }
  },
//integer_cogengen_pair[8]
  {
    "00",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  }
  },
//integer_cogengen_pair[9]
  {
    "07",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 7 }
  }
  },
//integer_cogengen_pair[10]
  {
    "012",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 10 }
  }
  },
//integer_cogengen_pair[11]
 {
    "-012",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { -10 }
  }
  },
//integer_cogengen_pair[12]
  {
    "0x00",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  }
  },
//integer_cogengen_pair[13]
  {
    "0x12",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 18 }
  }
  },
//integer_cogengen_pair[14]
  {
    "-0x12",
    {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    {-18 }
  }
  },
//integer_cogengen_pair[15]
  {
    "0xAb",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 171 }
  }
  },
//integer_cogengen_pair[16]
  {
    "0X0ff",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 255 }
  }
  },
//integer_cogengen_pair[17]
  {
    "0xFF",
     {
    24,                 // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 255 }
  }
  }
};

class IntOperandInvalidIn : public ::testing::TestWithParam<int>
{

};
std::string invalidinput_integercodegen[] = {
  "x",
  "e",
 // "08",
//"0xg",
//"0XG",
//"1a"
};

class DoubleOperand : public ::testing::TestWithParam<int>
{

};
std::string input_double_codegen[] = {
  "+12.345",
  "12.345l",
  "12.345L",
  "1.2345e1",
  "1.2345e+1l",
  "12345e-3L",
  ".12345e2",
  "12345.e-3l",
  "0d4028b0a3d70a3d71",
  "0D4028b0a3d70a3d71",
  "0x1.8b0a3d70a3d71p+3",
  "0x1.8b0a3d70a3d71p+3l",
  "0x1.8b0a3d70a3d71p+3L",
  "-12.345",
  "-12.345l",
  "-12.345L",
  "12345.",
  "12345.L",
  "e3L",
  "e+1",
  "e-3",
  "0101.L",
  "0007l",
  "000.77",
  ".007e0",
  "07L",
  "7l",
  "3e1",
  "0e2l",
  "9e2L"
};
class DoubleOperandInvalid : public ::testing::TestWithParam<int>
{

};
std::string invalidinput_doublecodegen[]={
  ".",
  "-",
  "+",
  //"3.5e",
  //"e-1.3",
  //"3.5e1.3",
 //"2.33.33L",
  "L",
  "l",
  "e"
};
class SingleOperand : public ::testing::TestWithParam<int>
{

};
std::string input_single_codegen[] = {
//need add hexFloatConstant test case
  "+12.345f",
  "12.345F",
  "1.2345e1f",
  "1.2345e+1F",
  "12345e-3f",
  ".12345e2F",
  "12345.e-3f",
  "-12.345f",
  "-12.345F",
  "-1.2345e1f",
  "-1.2345e+1F",
  "12345.F",
  "e3f",
  "-e+1F",
  "e-3F",
  "0101.F",
  "0007f",
  "000.77F",
  ".007e0f",
  "07F",
  "-7f",
  "3e1F",
  "0e2f",
  "-9e2F",
  "0x9.9pf",
  "0X9.9P+1F"
};
class SingleOperandInvalid : public ::testing::TestWithParam<int>
{

};
std::string invalidinput_singlecodegen[]={
  ".",
  "-",
  "+",
  //"3.5ef",
  //"e-1.3F",
  //"3.5e1.3f",
 //"2.33.33F",
  "f",
  "F",
  "e"
};

class WavesizeOperand : public ::testing::TestWithParam<int>
{

};
std::string input_wavesize_codegen[] = {
  "WAVESIZE"
};
class WavesizeOperandInvalid : public ::testing::TestWithParam<int>
{

};
std::string invalidinput_wavesizecodegen[]={
 "Wavesize",
 "waveisze",
  "WAVESIZ"
};

class TestAddressOperand : public ::testing::TestWithParam<int>{
};

struct AddressOperandTest{
  const char* str;
  BrigOperandAddress ref;
};

struct AddressOperandTest address_operand_pair[2] = {
//address_operand_pair[0]
  {
     "[%x];",
    {
      address_size,
      BrigEOperandAddress,
      Brigb64,
      0,
      40
    }
  },
//address_operand_pair[1]
  {
     "[&y];",
    {
      address_size,
      BrigEOperandAddress,
      Brigb64,
      0,
      80
    }
  }
};

class TestIndirectOperand : public ::testing::TestWithParam<int>{
};

struct IndirectOperandTest{
  const char* str;
  BrigOperandIndirect ref;
};

struct IndirectOperandTest indirect_operand_pair[6] = {
//indirect_operand_pair[0]
  {
    "[0];",
    {
      indirect_size,
      BrigEOperandIndirect,
      0,
      Brigb64,
      0,
      0
    }
  },
//indirect_operand_pair[1]
  {
    "[2];",
    {
      indirect_size,
      BrigEOperandIndirect,
      0,
      Brigb64,
      0,
      2
    }
  },
//indirect_operand_pair[2]
  {
    "[0x7f];",
    {
      indirect_size,
      BrigEOperandIndirect,
      0,
      Brigb64,
      0,
      0x7f
    }
  },
//indirect_operand_pair[3]
  {
    "[$s1];\n",
    {
      indirect_size,
      BrigEOperandIndirect,
      operand_offset,
      Brigb32,
      0,
      0
    }
  },
//indirect_operand_pair[4]
  {
    "[$s1 - 2];",
    {
      indirect_size,
      BrigEOperandIndirect,
      operand_offset,
      Brigb32,
      0,
      -2
    }
  },
//indirect_operand_pair[5]
  {
    "[$d1 + 4];",
    {
      indirect_size,
      BrigEOperandIndirect,
      operand_offset,
      Brigb64,
      0,
      4
    }
  }
};

class TestCompoundOperand : public ::testing::TestWithParam<int>{
};

struct CompoundOperandTest{
  const char* str;
  BrigOperandCompound ref;
};

struct CompoundOperandTest compound_operand_pair[6] = {
//compound_operand_pair[0]
  {
    "[%x][0];",
    {
      compound_size,
      BrigEOperandCompound,
      Brigb64,
      0,
      operand_offset,
      0,
      0
    }
  },
//compound_operand_pair[1]
  {
    "[%x][2];",
    {
      compound_size,
      BrigEOperandCompound,
      Brigb64,
      0,
      operand_offset,
      0,
      2
    }
  },
//compound_operand_pair[2]
  {
    "[%x][0x7f];",
    {
      compound_size,
      BrigEOperandCompound,
      Brigb64,
      0,
      operand_offset,
      0,
      0x7f
    }
  },
//compound_operand_pair[3]
  {
    "[&y][$s1];",
    {
      compound_size,
      BrigEOperandCompound,
      Brigb32,
      0,
      operand_offset,
      operand_offset + address_size,
      0
    }
  },
//compound_operand_pair[4]
  {
    "[&y][$s1-2];",
    {
      compound_size,
      BrigEOperandCompound,
      Brigb32,
      0,
      operand_offset,
      operand_offset + address_size,
      -2
    }
  },
//compound_operand_pair[5]
  {
    "[&y][$d1+4];",
    {
      compound_size,
      BrigEOperandCompound,
      Brigb64,
      0,
      operand_offset,
      operand_offset + address_size,
      4
    }
  }
};

class TestMemoryInvalidInput: public ::testing::TestWithParam<int>
{

};

std::string inputarray_memory_invalid[2] = {
  "[&y][$c1+4];",
  "[&y][$q1+4];"
};

}  // namespace brig
}  // namespace hsa
#endif // OPERAND_TEST_H_

