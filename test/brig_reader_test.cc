#include "brig_reader.h"
#include "brig_module.h"

#include "gtest/gtest.h"

using hsa::brig::BrigReader;

TEST(BrigReaderTest, VectorCopy) {
  BrigReader *reader =
    BrigReader::createBrigReader(TEST_PATH "/VectorCopy.bin");
  EXPECT_TRUE(reader);
  if(!reader) return;

  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  EXPECT_TRUE(!mod.isValid());

  delete reader;
}
