#pragma once

#include <cstddef>
#include <list>
#include <memory>
#include <shared_mutex>

#include "../skiplist/skiplist.h"

class MemTable {
public:
  MemTable();
  void Put(const std::string &key, const std::string &value);
  std::optional<std::string> Get(const std::string &key);
  void Remove(const std::string &key);

private:
  std::shared_ptr<SkipList> curr_table_;
  std::list<std::shared_ptr<SkipList>> frozen_tables_; // 利用链表实现顺序头插入

  size_t frozen_bytes_; // 记录冻结表字节数
  std::shared_mutex mutex_;
};