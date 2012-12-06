/* Copyright 2012 <MulticorewareInc> */

#ifndef SYNC_TEST_H_
#define SYNC_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestSync: public ::testing::TestWithParam<int>
{

};

struct SyncTest{
  const char* str;
  BrigInstBar ref;
};

struct SyncTest sync_pair[4] = {
//sync_pair[0]
  {
    "sync_global ;",
      {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigSync,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigGlobalLevel
    }
  },
//sync_pair[1]
  {
    "sync_group;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigSync,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigGroupLevel
  }
  },
//sync_pair[2]
  {
    "sync;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigSync,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigGroupLevel|BrigGlobalLevel
  }
  },
//sync_pair[3]
  {
    "sync_partial;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigSync,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigPartialLevel
    },
  }
};

class TestSyncInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_sync_invalid[] = {
  "sync",
  "sync_private;"
};

}  // namespace brig
}  // namespace hsa
#endif //SYNC_TEST_H_
