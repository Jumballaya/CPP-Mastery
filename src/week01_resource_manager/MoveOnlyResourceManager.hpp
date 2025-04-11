#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

template <typename T>
class MoveOnlyResourceManager {
 public:
  MoveOnlyResourceManager() = default;

  void load(const std::string &name, T &&resource) {
    auto [it, inserted] = resources_.try_emplace(name, std::move(resource));
  }

  // Non-mutable (read-only) access
  const T *get(const std::string &name) const {
    auto it = resources_.find(name);
    if (it == resources_.end()) return nullptr;
    return &it->second;
  }

  // Mutable access
  T *get(const std::string &name) {
    auto it = resources_.find(name);
    if (it == resources_.end()) return nullptr;
    return &it->second;
  }

  void unload(const std::string &name) {
    resources_.erase(name);
  }

  void clear() {
    resources_.clear();
  }

 private:
  std::unordered_map<std::string, T> resources_;
};