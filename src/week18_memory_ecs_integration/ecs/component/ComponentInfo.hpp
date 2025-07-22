#pragma once

#include <cstddef>
#include <functional>
#include <nlohmann/json.hpp>
#include <string_view>

#include "../entity/EntityId.hpp"
#include "ComponentId.hpp"

class World;

struct ComponentInfo {
  std::string_view name;
  size_t size;
  ComponentId id;
  std::function<void(World&, EntityId, const nlohmann::json&)> deserializeFn;

  explicit operator bool() const {
    return !name.empty();
  }
};