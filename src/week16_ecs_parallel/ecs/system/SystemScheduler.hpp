#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "System.hpp"

class World;

class SystemScheduler {
 public:
  SystemScheduler() = default;
  ~SystemScheduler() = default;
  SystemScheduler(const SystemScheduler&) = delete;
  SystemScheduler& operator=(const SystemScheduler&) = delete;
  SystemScheduler(SystemScheduler&&) noexcept = default;
  SystemScheduler& operator=(SystemScheduler&&) noexcept = default;

  template <typename T, typename... Args>
  void registerSystem(Args&&... args) {
    static_assert(std::is_base_of_v<System, T>, "T must derive from System");
    _systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
  }

  void update(World& world, float dt) {
    for (auto& system : _systems) {
      system->update(world, dt);
    }
  }

  void clear() {
    _systems.clear();
  }

  void removeSystem(System* system) {
    auto it = std::remove_if(
        _systems.begin(),
        _systems.end(),
        [system](const std::unique_ptr<System>& s) {
          return s.get() == system;
        });
    _systems.erase(it, _systems.end());
  }

 private:
  std::vector<std::unique_ptr<System>> _systems;
};