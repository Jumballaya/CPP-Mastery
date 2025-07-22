#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "EntityId.hpp"

class World;

class EntityBuilder {
 public:
  EntityBuilder(EntityId id, World& world);

  template <typename T, typename... Args>
  EntityBuilder& with(Args&&... args);

  template <typename T, typename Fn>
  EntityBuilder& with(Fn&& fn)
    requires std::is_invocable_r_v<void, Fn, T&>;

  EntityBuilder& tag(std::string_view tag);

  EntityId build();

  EntityId entity() const { return _entity; }

 private:
  EntityId _entity;
  World& _world;
  std::vector<std::function<void()>> _components;
  std::vector<std::string> _tags;
};
