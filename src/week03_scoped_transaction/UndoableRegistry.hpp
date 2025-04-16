#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "UndoableComponent.hpp"

template <typename T>
class UndoableRegistry {
 public:
  UndoableRegistry() = default;
  ~UndoableRegistry() = default;

  T& edit(std::string_view name) {
    auto [it, inserted] = _registry.emplace(std::string(name), UndoableComponent<T>(T{}));
    return it->second.edit();
  }

  void commit(std::string_view name) {
    auto key = std::string(name);
    auto it = _registry.find(key);
    if (it == _registry.end()) return;
    it->second.commit();
  }

  void undo(std::string_view name) {
    auto key = std::string(name);
    auto it = _registry.find(key);
    if (it == _registry.end()) return;
    it->second.undo();
  }

  const T* get(std::string_view name) const {
    auto it = _registry.find(std::string(name));
    if (it == _registry.end()) return nullptr;
    return &it->second.undo();
  }

 private:
  std::unordered_map<std::string, UndoableComponent<T>> _registry;
};