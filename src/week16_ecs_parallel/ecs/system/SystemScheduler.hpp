#pragma once

#include <algorithm>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "System.hpp"
#include "SystemId.hpp"
#include "SystemTraits.hpp"

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
  void registerSystem(Args&&... args);

  void update(World& world, float dt);
  void clear();
  void removeSystem(System* system);

 private:
  std::vector<std::shared_ptr<System>> _systems;
  std::unordered_map<std::type_index, SystemId> _tagProviders;
  std::unordered_map<SystemId, std::vector<std::type_index>> _systemReads;
  std::unordered_map<SystemId, std::vector<std::type_index>> _systemWrites;
};

template <typename T, typename... Args>
void SystemScheduler::registerSystem(Args&&... args) {
  static_assert(std::is_base_of_v<System, T>, "T must derive from System");

  auto system = std::make_shared<T>(std::forward<Args>(args)...);
  SystemId id = GetSystemId<T>();

  // Provides tags
  TypeListForEach<typename SystemTraits<T>::Provides>::apply(
      [&]<typename Tag>() {
        _tagProviders[std::type_index(typeid(Tag))] = id;
      });

  // Reads tags
  TypeListForEach<typename SystemTraits<T>::Reads>::apply(
      [&]<typename Component>() {
        _systemReads[id].push_back(std::type_index(typeid(Component)));
      });

  // Writes tags
  TypeListForEach<typename SystemTraits<T>::Writes>::apply(
      [&]<typename Component>() {
        _systemWrites[id].push_back(std::type_index(typeid(Component)));
      });

  _systems.emplace_back(std::move(system));
}