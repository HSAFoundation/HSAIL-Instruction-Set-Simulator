#include "brig_reader.h"

#include "gtest/gtest.h"

using hsa::brig::BrigReader;

TEST(BrigReaderTest, VectorCopy) {
  BrigReader *reader =
    BrigReader::createBrigReader(TEST_PATH "/VectorCopy.bin");
  EXPECT_TRUE(reader);
  if(!reader) return;

  delete reader;
}
