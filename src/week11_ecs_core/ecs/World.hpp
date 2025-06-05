#pragma once

#include "component/ComponentManager.hpp"
#include "entity/EntityManager.hpp"
#include "system/SystemScheduler.hpp"

class World {
 public:
  World();
  ~World();

 private:
  EntityManager _entityManager;
  ComponentManager _componentManager;
  SystemScheduler _systemScheduler;
};