#include <gtest/gtest.h>

#include "../include/memtable/memtable.h"

TEST(MemTable, BasicOperations) {
  MemTable mt;
  mt.Put("key1", "value1");
  EXPECT_EQ("value1", mt.Get("key1"));
  mt.Remove("key1");
  EXPECT_EQ(mt.Get("key1").value(), "");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}