#include <gtest/gtest.h>
#include <string>

#include "../include/skiplist/skiplist.h"
#include "../include/skiplist/skiplist_iterator.h"

TEST(SkipList, BasicOperation) {
  SkipList sl;
  // 验证正常插入
  sl.Put("key1", "value1");
  EXPECT_EQ("value1", sl.Get("key1"));
  // 验证正常覆盖
  sl.Put("key1", "value2");
  sl.Put("key1", "value3");
  EXPECT_EQ("value3", sl.Get("key1"));
  // 验证正常删除
  sl.Remove("key1");
  EXPECT_FALSE(sl.Get("key1").has_value());
}

TEST(SkipList, LargeScalePutAndGet) {
  int test_times = 100000;
  SkipList sl;

  // 测量大量数据
  for (int count = 0; count < test_times; ++count) {
    std::string key = "key" + std::to_string(count);
    std::string value = "value" + std::to_string(count);

    sl.Put(key, value);
  }

  for (int count = 0; count < test_times; ++count) {
    std::string key = "key" + std::to_string(count);
    std::string value = "value" + std::to_string(count);

    EXPECT_EQ(value, sl.Get(key));
  }
}

TEST(SkipList, LargeScaleRemove) {
  int test_times = 100000;
  SkipList sl;

  // 测量大量数据下删除正常
  for (int count = 0; count < test_times; ++count) {
    std::string key = "key" + std::to_string(count);
    std::string value = "value" + std::to_string(count);

    sl.Put(key, value);
  }

  for (int count = 0; count < test_times; ++count) {
    std::string key = "key" + std::to_string(count);

    sl.Remove(key);
  }

  for (int count = 0; count < test_times; ++count) {
    std::string key = "key" + std::to_string(count);

    EXPECT_FALSE(sl.Get(key).has_value());
  }
}

TEST(SkipList, BytesCount) {
  SkipList sl;

  std::string k1 = "key1", v1 = "value1";
  std::string k2 = "key2", v2 = "value2";

  sl.Put(k1, v1);
  sl.Put(k2, v2);
  int size = k1.size() + k2.size() + v1.size() + v2.size();
  EXPECT_EQ(size, sl.GetSize());

  sl.Put(k1, "value1.1");
  size += sizeof("value1.1") - 1 - v1.size();

  EXPECT_EQ(size, sl.GetSize());

  sl.Remove("key2");
  size -= k2.size() + v2.size();

  EXPECT_EQ(size, sl.GetSize());
}

TEST(SkipList, IteratorBasicOperation) {
  SkipList sl;

  auto begin = sl.Begin();
  auto end = sl.End();
  EXPECT_TRUE(begin == end);

  sl.Put("key1", "value1");
  begin = sl.Begin();
  end = sl.End();
  auto kv = begin.Get();
  EXPECT_EQ(kv.value().first, "key1");
  EXPECT_EQ(kv.value().second, "value1");
  EXPECT_FALSE(begin == end);
  EXPECT_TRUE(begin.IsValid());
  EXPECT_FALSE(begin.IsEnd());

  sl.Remove("key1");
  kv = begin.Get();
  EXPECT_EQ(kv, std::nullopt);
  EXPECT_TRUE(begin == end);
  EXPECT_FALSE(begin.IsValid());
  EXPECT_TRUE(begin.IsEnd());
}

TEST(SkipList, IteratorTraversal) {
  SkipList sl;
  int test_times = 10;

  for (int count = 0; count < test_times; ++count) {
    std::string key = "key" + std::to_string(count);
    std::string value = "value" + std::to_string(count);

    sl.Put(key, value);
  }

  auto iterator = sl.Begin();
  auto end = sl.End();

  int count = 0;
  while (iterator != end) {
    std::string key = "key" + std::to_string(count);
    std::string value = "value" + std::to_string(count);
    EXPECT_EQ(iterator.GetKey(), key);
    EXPECT_EQ(iterator.GetValue(), value);
    ++count;
    ++iterator;
  }

  EXPECT_TRUE(iterator == end);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}