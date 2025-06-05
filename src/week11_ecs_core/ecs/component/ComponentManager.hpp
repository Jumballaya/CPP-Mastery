#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>

#include "../entity/EntityId.hpp"
#include "ComponentId.hpp"
#include "ComponentStorage.hpp"

class ComponentManager {
 public:
  void debugPrintAll() {
    for (auto& [id, storage] : _storages) {
      storage->debugPrint();
      std::cout << std::endl;
    }
  }

  template <typename T>
  void debugPrint() {
    storage<T>().debugPrint();
    std::cout << std::endl;
  }

  template <typename T>
  void registerStorage() {
    ComponentId id = T::typeId();
    if (_storages.count(id)) return;
    _storages[id] = std::make_unique<ComponentStorage<T>>();
  }

  template <typename T, typename... Args>
  T& emplace(EntityId entity, Args&&... args) {
    return storage<T>().emplace(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T* get(EntityId entity) {
    if (!_storages.count(entity)) {
      throw std::runtime_error("Storage not registered for component!");
    }
    return storage<T>().get(entity);
  }

  template <typename T>
  bool has(EntityId entity) const {
    return storage<T>().has(entity);
  }

  template <typename T>
  void remove(EntityId entity) {
    storage<T>().remove(entity);
  }

  void clearAll() {
    for (auto& [_, storage] : _storages)
      storage->clear();
  }

 private:
  std::unordered_map<ComponentId, std::unique_ptr<IComponentStorage>> _storages;

  template <typename T>
  ComponentStorage<T>& storage() const {
    ComponentId id = T::typeId();
    auto* base = _storages.at(id).get();
    return *static_cast<ComponentStorage<T>*>(base);
  }
};