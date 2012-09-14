/* Copyright 2012 <MulticorewareInc> */

#ifndef CODEGEN_TEST_H_
#define CODEGEN_TEST_H_

namespace hsa {
namespace brig {
// ------------------ BASIC LEXER TESTS INPUT/OUTPUT -----------------
class IntegerConstant : public ::testing::TestWithParam<int>
{

};
std::string input_integerconstant[] = {
  "0",
  "1234567",
  "2147483647",
  // "2147483648",
  // "4294967295",
  "00",
  "000",
  "001",
  "01234567",
  "0x00",
  "0x1234567",
  "0x12ab",
  "0x12abEF",
  "0x0aF",
  "0x0ab",
  "0x0FF",
  "0X00",
  "0X1234567",
  "0X12ab",
  "0X12abEF",
  "0X0aF",
  "0X0ab",
  "0X0FF",
};
unsigned long output_integerconstant[] = {
  0,
  1234567,
  2147483647,
  // 2147483648,
  // 4294967295,
  0,
  0,
  1,
  342391,
  0,
  19088743,
  4779,
  1223663,
  175,
  171,
  255,
  0,
  19088743,
  4779,
  1223663,
  175,
  171,
  255,
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
// $16-$31 are acceptable inputs in the new version PRM
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
  "$s15"
};

class DRegisterValidInput : public ::testing::TestWithParam<int>
{

};
std::string input_dregister[] = {
// $8-$31 are acceptable inputs in the new version PRM
  "$d0",
  "$d1",
  "$d2",
  "$d3",
  "$d4",
  "$d5",
  "$d6",
  "$d7"
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
  "$q7"
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

}  // namespace brig
}  // namespace hsa
#endif // CODEGEN_TEST_H_

