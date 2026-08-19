#include <memory>
#include <optional>
#include <random>
#include <stdexcept>

#include "../../include/skiplist/skiplist.h"

int SkipList::random_level() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 1);
  int new_level = 1;
  while (new_level < max_level_ && dist(gen) == 1) {
    ++new_level;
  }
  return new_level;
}

void SkipList::Put(const std::string &key, const std::string &value) {
  if (head_ == nullptr) {
    throw std::runtime_error("SkipList not initialized: head_ is null");
  }
  int new_level = random_level();
  auto new_node = std::make_shared<SkipListNode>(key, value, new_level);
  std::vector<std::shared_ptr<SkipListNode>> update(max_level_, nullptr);

  // 查找元素是否存在同时记录插入位置
  auto curr = head_;
  for (int idx = curr_level_ - 1; idx >= 0; --idx) {
    while (curr->forward[idx] != nullptr && curr->forward[idx]->key_ < key) {
      curr = curr->forward[idx];
    }
    if (idx < new_level) {
      update[idx] = curr;
    }
  }

  // 找是不是目标节点
  curr = curr->forward[0];
  if (curr != nullptr && curr->key_ == key) {
    size_bytes_ += value.size() - curr->value_.size();
    curr->value_ = value;
    return;
  }

  // 没有目标节点，需要插入
  if (new_level > curr_level_) {
    // 超出curr_level_的部分前驱节点是表头节点
    for (int idx = curr_level_; idx < new_level; ++idx) {
      update[idx] = head_;
    }
    curr_level_ = new_level;
  }

  // 按照层次从低到高插入
  for (int idx = 0; idx < new_level; ++idx) {
    new_node->forward[idx] = update[idx]->forward[idx];
    update[idx]->forward[idx] = new_node;
    new_node->SetBackWard(update[idx], idx);
    if (new_node->forward[idx] != nullptr) {
      new_node->forward[idx]->SetBackWard(new_node, idx);
    }
  }

  size_bytes_ += key.size() + value.size();
}

// 删除节点，实际上LSM Engine的删除是通过Put(key, "")来实现的
void SkipList::Remove(const std::string &key) {
  if (head_ == nullptr) {
    throw std::runtime_error("SkipList not initialized: head_ is null");
  }

  std::vector<std::shared_ptr<SkipListNode>> update(max_level_, nullptr);

  auto curr = head_;
  for (int idx = curr_level_ - 1; idx >= 0; --idx) {
    while (curr->forward[idx] != nullptr && curr->forward[idx]->key_ < key) {
      curr = curr->forward[idx];
    }
    update[idx] = curr;
  }

  curr = curr->forward[0];
  if (curr == nullptr || curr->key_ != key) {
    return;
  }

  for (int idx = 0; idx < curr_level_; ++idx) {
    if (update[idx]->forward[idx] != curr) {
      break;
    }

    update[idx]->forward[idx] = curr->forward[idx];
    if (update[idx]->forward[idx] != nullptr) {
      update[idx]->forward[idx]->SetBackWard(update[idx], idx);
    }
  }

  size_bytes_ -= curr->key_.size() + curr->value_.size();
  while (curr_level_ > 1 && head_->forward[curr_level_ - 1] == nullptr) {
    --curr_level_;
  }
}

// optional可同时处理key节点物理上不存在和key节点逻辑上被删除的两种情况
std::optional<std::string> SkipList::Get(const std::string &key) {
  if (head_ == nullptr) {
    throw std::runtime_error("SkipList not initialized: head_ is null");
  }

  auto curr = head_;
  for (int idx = curr_level_ - 1; idx >= 0; --idx) {
    while (curr->forward[idx] != nullptr && curr->forward[idx]->key_ < key) {
      curr = curr->forward[idx];
    }
  }

  curr = curr->forward[0];
  if (curr == nullptr) {
    return std::nullopt;
  }
  return std::make_optional(curr->value_);
}