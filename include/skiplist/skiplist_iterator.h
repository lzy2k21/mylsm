#pragma once

#include <optional>
#include <utility>

#include "skiplist.h"
struct SkipListNode;

// skiplist迭代器在指向的节点被删除后将失效
// 尽管在skiplist中应该不存在删除物理节点的情况
class SkipListIterator {
public:
  SkipListIterator(std::shared_ptr<SkipListNode> ptr = nullptr);

  SkipListIterator &operator++();
  bool operator==(const SkipListIterator &other) const;
  bool operator!=(const SkipListIterator &other) const;
  std::optional<std::pair<std::string, std::string>> Get() const;
  std::optional<std::string> GetKey() const;
  std::optional<std::string> GetValue() const;
  bool IsValid() const;
  bool IsEnd() const;

private:
  std::weak_ptr<SkipListNode> current_;
};