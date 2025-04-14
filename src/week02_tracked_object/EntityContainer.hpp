#pragma once

#include <string_view>

#include "TrackedVector.hpp"

template <typename T>
class EntityContainer {
 public:
  EntityContainer() : _components(TrackedVector<T>(5)) {}
  ~EntityContainer() {}

  void add(T component) {
    _components.push_back(component);
  };

  void remove(int id) {
    _components.remove(id);
  }

 private:
  TrackedVector<T> _components;
};