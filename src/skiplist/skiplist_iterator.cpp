#include <optional>

#include "../../include/skiplist/skiplist_iterator.h"

SkipListIterator::SkipListIterator(std::shared_ptr<SkipListNode> ptr)
    : current_(ptr) {}

SkipListIterator &SkipListIterator::operator++() {
  auto node = current_.lock();
  if (node != nullptr) {
    current_ = node->forward[0];
  }
  return *this;
}

bool SkipListIterator::operator==(const SkipListIterator &other) const {
  return this->current_.lock() == other.current_.lock();
}

bool SkipListIterator::operator!=(const SkipListIterator &other) const {
  return !(*this == other);
}

std::optional<std::pair<std::string, std::string>>
SkipListIterator::Get() const {
  auto node = current_.lock();
  if (node == nullptr) {
    return std::nullopt;
  }
  return std::make_pair(node->key_, node->value_);
}

std::optional<std::string> SkipListIterator::GetKey() const {
  auto node = current_.lock();
  if (node == nullptr) {
    return std::nullopt;
  }
  return node->key_;
}

std::optional<std::string> SkipListIterator::GetValue() const {
  auto node = current_.lock();
  if (node == nullptr) {
    return std::nullopt;
  }
  return node->value_;
}

bool SkipListIterator::IsValid() const { return !current_.expired(); }

bool SkipListIterator::IsEnd() const { return current_.lock() == nullptr; }