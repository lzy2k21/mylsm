#include <mutex>
#include <shared_mutex>

#include "../../include/memtable/memtable.h"

MemTable::MemTable() : curr_table_(new SkipList()), frozen_bytes_(0) {}

void MemTable::Put(const std::string &key, const std::string &value) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  curr_table_->Put(key, value);
}

std::optional<std::string> MemTable::Get(const std::string &key) {
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto curr_result = curr_table_->Get(key);
  // 区分nullopt和""，否则可能导致读取BUG，空字符串逻辑交给上层处理
  if (curr_result.has_value()) {
    return curr_result;
  }

  // 只有当活跃跳表物理上找不到key才查找冻结跳表，删除记号直接返回
  for (auto frozen_table : frozen_tables_) {
    auto frozen_result = frozen_table->Get(key);
    if (frozen_result.has_value()) {
      return frozen_result;
    }
  }

  return std::nullopt;
}

void MemTable::Remove(const std::string &key) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  curr_table_->Put(key, "");
}