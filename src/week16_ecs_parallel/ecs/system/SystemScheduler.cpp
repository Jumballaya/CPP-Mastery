#include "SystemScheduler.hpp"

void SystemScheduler::update(World& world, float dt) {
  for (auto& system : _systems) {
    system->update(world, dt);
  }
}

void SystemScheduler::clear() {
  _systems.clear();
}

void SystemScheduler::removeSystem(System* system) {
  auto it = std::remove_if(
      _systems.begin(),
      _systems.end(),
      [system](const std::shared_ptr<System>& s) {
        return s.get() == system;
      });
  _systems.erase(it, _systems.end());
}