#pragma once

#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <unordered_map>

#include "../entity/EntityId.hpp"
#include "ComponentConcepts.hpp"

class IComponentStorage {
 public:
  virtual ~IComponentStorage() = default;
  virtual void remove(EntityId entity) = 0;
  virtual bool has(EntityId entity) const = 0;
  virtual void clear() = 0;
  virtual void cloneComponent(EntityId from, EntityId to) = 0;
  virtual size_t size() const = 0;
};

template <ComponentType T>
class ComponentStorage : public IComponentStorage {
 public:
  using MapType = std::pmr::unordered_map<EntityId, T>;
  MapType::iterator begin() { return _components.begin(); }
  MapType::iterator end() { return _components.end(); }

  ComponentStorage(std::pmr::memory_resource* resource = std::pmr::get_default_resource()) : _resource(resource) {}

  ~ComponentStorage() = default;
  ComponentStorage(const ComponentStorage& other) noexcept = delete;
  ComponentStorage(ComponentStorage&& other) noexcept = delete;
  ComponentStorage& operator=(const ComponentStorage& other) noexcept = delete;
  ComponentStorage& operator=(ComponentStorage&& other) noexcept = delete;

  template <typename... Args>
  T& emplace(EntityId entity, Args&&... args) {
    return _components.try_emplace(entity, std::forward<Args>(args)...).first->second;
  }

  void remove(EntityId entity) override {
    _components.erase(entity);
  }

  bool has(EntityId entity) const override {
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

  template <typename Fn>
  void forEach(Fn&& fn) {
    for (auto& [entity, component] : _components) {
      fn(entity, component);
    }
  }

  void cloneComponent(EntityId from, EntityId to) override {
    if (!has(from)) return;
    _components.try_emplace(to, _components.at(from));
  }

  void clear() override { _components.clear(); }
  size_t size() const override { return _components.size(); }

 private:
  std::pmr::unordered_map<EntityId, T> _components;
  std::pmr::memory_resource* _resource;
};