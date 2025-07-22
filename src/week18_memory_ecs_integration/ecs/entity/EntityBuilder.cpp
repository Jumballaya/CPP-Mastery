#include "EntityBuilder.hpp"

#include "../World.hpp"

EntityBuilder::EntityBuilder(EntityId id, World& world)
    : _entity(id), _world(world) {}

EntityBuilder& EntityBuilder::tag(std::string_view tag) {
  _tags.emplace_back(tag);
  return *this;
}

EntityId EntityBuilder::build() {
  for (auto& fn : _components) fn();
  for (auto& tag : _tags) _world.addTag(_entity, tag);
  return _entity;
}