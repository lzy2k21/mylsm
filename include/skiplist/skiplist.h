#pragma once

#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

struct SkipListNode {
  std::string key_;
  std::string value_;
  std::vector<std::shared_ptr<SkipListNode>> forward;
  std::vector<std::weak_ptr<SkipListNode>> backward;
  SkipListNode(std::string key, std::string value, int level)
      : key_(key), value_(value), forward(level, nullptr), backward(level) {}
  void SetBackWard(std::shared_ptr<SkipListNode> &other, int idx) {
    backward[idx] = other;
  }
};

class SkipList {
public:
  SkipList(int max_level = 16)
      : max_level_(max_level), curr_level_(1), size_bytes_(0) {
    head_ = std::make_shared<SkipListNode>("", "", max_level_);
  }
  void Put(const std::string &key, const std::string &value);
  void Remove(const std::string &key);
  std::optional<std::string> Get(const std::string &key);
  int GetSize() const { return size_bytes_; }

private:
  std::shared_ptr<SkipListNode> head_;
  int curr_level_;
  int max_level_;
  int size_bytes_;

  int random_level();
};