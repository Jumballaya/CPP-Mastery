#pragma once

#include <string_view>
#include <unordered_map>

#include "TrackedComponent.hpp"

class ComponentRegistry {
 public:
  ComponentRegistry() = default;
  ~ComponentRegistry() = default;

  void register_component(std::string_view name, const TrackedComponent& component) {
    auto cpy = component;
    _registry.emplace(name, component);
  }

  TrackedComponent get_copy(std::string_view name) const {
    auto found = _registry.find(name);
    if (found != _registry.end()) {
      return found->second;
    }
    return TrackedComponent();
  }

  void print_all() const {
    for (const auto& pair : _registry) {
      auto comp = pair.second.get();
      std::cout << pair.first << " : size=" << comp.size() << std::endl;
    }
  }

 private:
  std::unordered_map<std::string_view, TrackedComponent> _registry;
};