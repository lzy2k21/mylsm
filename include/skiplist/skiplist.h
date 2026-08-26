#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

class SkipListIterator;

struct SkipListNode {
  std::string key_;
  std::string value_;
  std::vector<std::shared_ptr<SkipListNode>> forward;
  std::vector<std::weak_ptr<SkipListNode>> backward;
  SkipListNode(std::string key, std::string value, int level);
  void SetBackWard(std::shared_ptr<SkipListNode> &other, int idx);
};

class SkipList {
public:
  SkipList(int max_level = 16);
  ~SkipList();
  void Put(const std::string &key, const std::string &value);
  void Remove(const std::string &key);
  std::optional<std::string> Get(const std::string &key);
  size_t GetSize() const;

  // SkipList迭代器只能通过SkipList初始化得到
  SkipListIterator Begin();
  SkipListIterator End();

private:
  std::shared_ptr<SkipListNode> head_;
  int curr_level_;
  int max_level_;
  size_t size_bytes_;

  int random_level();
  std::mt19937 gen_;
  std::uniform_int_distribution<int> dist_;
};