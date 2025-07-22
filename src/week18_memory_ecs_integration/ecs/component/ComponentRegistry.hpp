#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Component.hpp"
#include "ComponentConcepts.hpp"
#include "ComponentId.hpp"
#include "ComponentInfo.hpp"

class World;

class ComponentRegistry {
 public:
  template <ComponentType T>
  void registerComponent(std::string_view name, std::function<void(World&, EntityId, const nlohmann::json&)> deserializer) {
    ComponentId id = Component<T>::typeId();

    if (_components.size() <= id) {
      _components.resize(id + 1);
      _components[id] = ComponentInfo{name, sizeof(T), id, std::move(deserializer)};
      _nameToId[std::string(name)] = id;
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

  std::optional<ComponentId> getComponentIdByName(std::string_view name) const {
    auto it = _nameToId.find(std::string(name));
    if (it == _nameToId.end()) {
      return std::nullopt;
    }
    return it->second;
  }

 private:
  std::vector<ComponentInfo> _components;
  std::unordered_map<std::string, ComponentId> _nameToId;
};