#pragma once

#include "component/ComponentManager.hpp"
#include "component/ComponentStorage.hpp"
#include "entity/EntityId.hpp"
#include "entity/EntityManager.hpp"
#include "system/SystemScheduler.hpp"

class World {
 public:
  World() = default;
  ~World() = default;
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  World(World&&) noexcept = default;
  World& operator=(World&&) noexcept = default;

  void update(float dt) {
    _systemScheduler.update(*this, dt);
  }

  template <typename Fn>
  void eachEntity(Fn&& fn) {
    for (uint32_t i = 0; i < _entityManager.capacity(); ++i) {
      EntityId id{i, _entityManager.generation(i)};
      if (isAlive(id)) {
        fn(id);
      }
    }
  }

  EntityId createEntity() { return _entityManager.create(); }
  void destroyEntity(EntityId id) { _entityManager.destroy(id); }
  bool isAlive(EntityId id) const { return _entityManager.isAlive(id); }

  template <typename T, typename... Args>
  void addComponent(EntityId id, Args&&... args) {
    if (!isAlive(id)) {
      return;
    }
    _componentManager.emplace<T>(id, std::forward<Args>(args)...);
  }

  template <typename T>
  [[nodiscard]]
  T* getComponent(EntityId id) {
    if (!isAlive(id)) {
      return nullptr;
    }
    return _componentManager.get<T>(id);
  }

  template <typename T>
  bool hasComponent(EntityId id) const {
    if (!isAlive(id)) {
      return false;
    }
    return _componentManager.has<T>(id);
  }

  template <typename T>
  void removeComponent(EntityId id) {
    if (!isAlive(id)) {
      return;
    }
    _componentManager.remove<T>(id);
  }

  template <typename T, typename... Args>
  void registerSystem(Args&&... args) {
    _systemScheduler.registerSystem<T>(std::forward<Args>(args)...);
  }

 private:
  EntityManager _entityManager;
  ComponentManager _componentManager;
  SystemScheduler _systemScheduler;
};