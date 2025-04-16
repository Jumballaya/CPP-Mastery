#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "UndoableComponent.hpp"

template <typename T>
class UndoableEntity {
 public:
  UndoableEntity() = default;
  ~UndoableEntity() = default;

  UndoableEntity(const UndoableEntity& other) = delete;
  UndoableEntity& operator=(const UndoableEntity& other) = delete;

  T& edit(std::string_view fieldName) {
    auto [it, inserted] = _fields.emplace(std::string(fieldName), UndoableComponent<T>(T{}));
    return it->second.edit();
  }

  void commitAll() {
    for (auto& [key, value] : _fields) {
      value.commit();
    }
  }

  void undo(std::string_view fieldName) {
    auto found = _fields.find(std::string(fieldName));
    if (found == _fields.end()) return;
    found->second.undo();
  }

  void undoAll() {
    for (auto& [key, value] : _fields) {
      value.undo();
    }
  }

  void commit(std::string_view fieldName) {
    auto found = _fields.find(std::string(fieldName));
    if (found == _fields.end()) return;
    found->second.commit();
  }

  const T& value(std::string_view fieldName) const {
    auto found = _fields.find(std::string(fieldName));
    if (found == _fields.end()) throw std::runtime_error("Field not found");
    return found->second.value();
  }

  bool is_dirty(std::string_view fieldName) const {
    auto found = _fields.find(std::string(fieldName));
    if (found == _fields.end()) return false;
    return found->second.is_dirty();
  }

  bool has(std::string_view fieldName) const {
    return _fields.find(std::string(fieldName)) != _fields.end();
  }

  void clear() {
    _fields.clear();
  }

  std::vector<std::string> keys() const {
    std::vector<std::string> keys;
    keys.reserve(_fields.size());

    for (const auto& kv : _fields) {
      keys.push_back(kv.first);
    }

    return keys;
  }

 private:
  std::unordered_map<std::string, UndoableComponent<T>> _fields;
};