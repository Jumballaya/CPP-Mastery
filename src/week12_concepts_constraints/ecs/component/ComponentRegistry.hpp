#pragma once

#include <string_view>
#include <vector>

#include "Component.hpp"
#include "ComponentConcepts.hpp"
#include "ComponentId.hpp"
#include "ComponentInfo.hpp"

class ComponentRegistry {
 public:
  template <ComponentType T>
  void registerComponent(std::string_view name) {
    ComponentId id = Component<T>::typeId();

    if (_components.size() <= id) {
      _components.resize(id + 1);

      _components[id] = ComponentInfo{
          .name = name,
          .size = sizeof(T),
          .id = id};
    }
  }

  void forEachRegisteredComponent(auto&& fn) {
    for (ComponentId id = 0; id < _components.size(); ++id) {
      if (_components[id]) fn(id);
    }
  }

  const ComponentInfo* getInfo(ComponentId id) const {
    if (id >= _components.size()) return nullptr;
    return &_components[id];
  }

 private:
  std::vector<ComponentInfo> _components;
};