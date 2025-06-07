#include "EntityRef.hpp"

#include "../World.hpp"

EntityRef::EntityRef(EntityId id, World* world)
    : id(id), world(world) {}

void EntityRef::addTag(std::string_view tag) {
  world->addTag(id, tag);
}

void EntityRef::removeTag(std::string_view tag) {
  world->removeTag(id, tag);
}

bool EntityRef::hasTag(std::string_view tag) const {
  return world->hasTag(id, tag);
}

const std::unordered_set<unsigned int>& EntityRef::tags() const {
  return world->getTags(id);
}

bool EntityRef::alive() const {
  return world->isAlive(id);
}