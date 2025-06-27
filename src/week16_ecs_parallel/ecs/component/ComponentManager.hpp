#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "../entity/EntityId.hpp"
#include "ComponentConcepts.hpp"
#include "ComponentId.hpp"
#include "ComponentStorage.hpp"

class ComponentManager {
 public:
  template <ComponentType T>
  void registerStorage() {
    ComponentId id = T::typeId();
    if (_storages.count(id)) return;
    _storages[id] = std::make_unique<ComponentStorage<T>>();
  }

  template <ComponentType T, typename... Args>
  T& emplace(EntityId entity, Args&&... args) {
    registerStorage<T>();
    return storage<T>().emplace(entity, std::forward<Args>(args)...);
  }

  template <ComponentType T>
  T* get(EntityId entity) {
    ComponentId id = T::typeId();
    if (!_storages.count(id)) {
      throw std::runtime_error("Storage not registered for component!");
    }
    return storage<T>().get(entity);
  }

  template <ComponentType T>
  bool has(EntityId entity) const {
    return storage<T>().has(entity);
  }

  template <ComponentType T>
  void remove(EntityId entity) {
    storage<T>().remove(entity);
  }

  void clearAll() {
    for (auto& [_, storage] : _storages)
      storage->clear();
  }

  template <ComponentType T>
  ComponentStorage<T>& storage() const {
    ComponentId id = T::typeId();
    assert(_storages.contains(id));
    auto* base = _storages.at(id).get();
    return *static_cast<ComponentStorage<T>*>(base);
  }

  IComponentStorage* rawStorage(ComponentId id) const {
    auto it = _storages.find(id);
    if (it != _storages.end()) return nullptr;
    return it->second.get();
  }

 private:
  std::unordered_map<ComponentId, std::unique_ptr<IComponentStorage>> _storages;
};