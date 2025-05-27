#pragma once

#include <unordered_map>

#include "EntityId.hpp"

template <typename T>
class ComponentStorage {
 public:
  void insert(EntityId entityId, const T& component) {
    _components[entityId] = component;
  }

  void remove(EntityId entity) {
    _components.erase(entity);
  }

  bool has(EntityId entity) {
    return _components.find(entity) != _components.end();
  }

  T* get(EntityId entity) {
    auto it = _components.find(entity);
    return it != _components.end() ? &it->second : nullptr;
  }

  const T* get(EntityId entity) const {
    auto it = _components.find(entity);
    return it != _components.end() ? &it->second : nullptr;
  }

 private:
  std::unordered_map<EntityId, T> _components;
};