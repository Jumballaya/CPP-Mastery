#pragma once

#include <algorithm>
#include <initializer_list>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../tasks/TaskGraph.hpp"
#include "View.hpp"
#include "component/ComponentConcepts.hpp"
#include "component/ComponentManager.hpp"
#include "component/ComponentRegistry.hpp"
#include "component/ComponentStorage.hpp"
#include "entity/EntityBuilder.hpp"
#include "entity/EntityId.hpp"
#include "entity/EntityManager.hpp"
#include "entity/EntityRef.hpp"
#include "entity/TagSymbol.hpp"
#include "system/SystemScheduler.hpp"

class World {
 public:
  World() = default;
  ~World() = default;
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  World(World&&) noexcept = default;
  World& operator=(World&&) noexcept = default;

  EntityRef operator[](EntityId id);

  void buildExecutionGraph(TaskGraph& graph, float dt);

  template <ComponentType... Ts>
  View<Ts...> view();

  // ENTITY API
  EntityBuilder builder();
  EntityId createEntity();
  EntityId createEntity(std::string_view tag);
  bool isAlive(EntityId id) const;
  void destroyEntity(EntityId id);
  EntityId cloneEntity(EntityId src);

  // ENTITY TAGS API
  void addTag(EntityId id, std::string_view tag);
  void removeTag(EntityId id, std::string_view tag);
  void clearTags(EntityId id);
  bool hasTag(EntityId id, std::string_view tag) const;
  void retagEntity(EntityId id, std::string_view tag);
  const std::unordered_set<EntityId> findWithTag(std::string_view tag) const;
  std::unordered_set<EntityId> findWithTags(std::initializer_list<std::string_view> tags) const;
  const std::unordered_set<TagSymbol>& getTags(EntityId id) const;

  // COMPONENT API
  template <ComponentType T>
  void registerComponent();

  template <ComponentType T, typename... Args>
  T& addComponent(EntityId id, Args&&... args);

  template <ComponentType T>
  [[nodiscard]]
  T* getComponent(EntityId id);

  template <ComponentType T>
  bool hasComponent(EntityId id) const;

  template <ComponentType T>
  void removeComponent(EntityId id);

  template <typename T, typename... Args>
  void registerSystem(Args&&... args);

  template <ComponentType T>
  void assertComponent(EntityId id);

  // VALIDATION
  void validate() const;

 private:
  EntityManager _entityManager;
  ComponentManager _componentManager;
  ComponentRegistry _registry;
  SystemScheduler _systemScheduler;

  std::unordered_map<TagSymbol, std::unordered_set<EntityId>> _tagToEntities;
  std::unordered_map<EntityId, std::unordered_set<TagSymbol>> _entityToTags;
};

// TEMPLATED METHODS

template <ComponentType... Ts>
View<Ts...> World::view() {
  return View<Ts...>(_componentManager.storage<Ts>()...);
}

template <ComponentType T>
void World::registerComponent() {
  this->_componentManager.registerStorage<T>();
}

template <ComponentType T, typename... Args>
T& World::addComponent(EntityId id, Args&&... args) {
  if (!_entityManager.isAlive(id)) {
    throw std::runtime_error("Cannot add component to dead entity");
  }
  if (hasComponent<T>(id)) {
    throw std::runtime_error("Component already exists for this entity");
  }
  return _componentManager.emplace<T>(id, std::forward<Args>(args)...);
}

template <ComponentType T>
[[nodiscard]]
T* World::getComponent(EntityId id) {
  if (!isAlive(id)) {
    return nullptr;
  }
  return _componentManager.get<T>(id);
}

template <ComponentType T>
bool World::hasComponent(EntityId id) const {
  if (!isAlive(id)) {
    return false;
  }
  return _componentManager.has<T>(id);
}

template <ComponentType T>
void World::removeComponent(EntityId id) {
  if (!isAlive(id)) {
    return;
  }
  _componentManager.remove<T>(id);
}

template <typename T, typename... Args>
void World::registerSystem(Args&&... args) {
  _systemScheduler.registerSystem<T>(std::forward<Args>(args)...);
}

template <ComponentType T>
void World::assertComponent(EntityId id) {
  if (!hasComponent<T>(id)) {
    throw std::runtime_error(std::string("Missing component: ") + std::string(T::name()));
  }
}

//
//  For the EntityRef API
//
template <typename T>
T* EntityRef::get() const {
  return world->getComponent<T>(id);
}

template <typename T, typename... Args>
T& EntityRef::add(Args&&... args) {
  return world->addComponent<T>(id, std::forward<Args>(args)...);
}

template <typename T>
bool EntityRef::has() const {
  return world->hasComponent<T>(id);
}

template <typename T>
void EntityRef::remove() {
  world->removeComponent<T>(id);
}

//
// For EntityBuilder API
//
template <typename T, typename... Args>
EntityBuilder& EntityBuilder::with(Args&&... args) {
  auto argsTuple = std::make_tuple(std::forward<Args>(args)...);

  _components.emplace_back(
      [this, argsTuple = std::move(argsTuple)]() mutable {
        std::apply([&](auto&&... unpackedArgs) {
          _world.addComponent<T>(_entity, std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
        },
                   std::move(argsTuple));
      });

  return *this;
}

template <typename T, typename Fn>
EntityBuilder& EntityBuilder::with(Fn&& fn)
  requires std::is_invocable_r_v<void, Fn, T&>
{
  auto fnCopy = std::forward<Fn>(fn);

  _components.emplace_back(
      [this, fnCopy = std::move(fnCopy)]() mutable {
        T t{};
        fnCopy(t);
        _world.addComponent<T>(_entity, std::move(t));
      });

  return *this;
}