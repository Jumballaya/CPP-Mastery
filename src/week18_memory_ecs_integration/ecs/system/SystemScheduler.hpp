#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "../../tasks/TaskGraph.hpp"
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
  void disableSystem(System& system);
  void enableSystem(System& system);

  void buildTaskGraph(TaskGraph& graph, World& world, float dt);

 private:
  std::vector<std::shared_ptr<System>> _systems;
  std::unordered_map<std::type_index, SystemId> _tagProviders;
  std::unordered_map<SystemId, std::vector<std::type_index>> _systemReads;
  std::unordered_map<SystemId, std::vector<std::type_index>> _systemWrites;
  std::unordered_map<SystemId, std::type_index> _systemTypes;
  std::unordered_map<SystemId, TaskId> _systemJobMap;
  std::unordered_map<std::type_index, std::function<void(SystemScheduler&, SystemId, TaskGraph&)>> _dependencyResolvers;
};

template <typename T, typename... Args>
void SystemScheduler::registerSystem(Args&&... args) {
  static_assert(std::is_base_of_v<System, T>, "T must derive from System");

  auto system = std::make_shared<T>(std::forward<Args>(args)...);
  SystemId id = static_cast<SystemId>(_systems.size());
  std::type_index systemType = std::type_index(typeid(T));
  _systemTypes.emplace(id, std::type_index(typeid(T)));

  // Register provided tags
  TypeListForEach<typename SystemTraits<T>::Provides>::apply(
      [&]<typename Tag>() {
        std::type_index tagIndex = std::type_index(typeid(Tag));
        _tagProviders[tagIndex] = id;
      });

  // Register component reads
  TypeListForEach<typename SystemTraits<T>::Reads>::apply(
      [&]<typename Component>() {
        _systemReads[id].push_back(std::type_index(typeid(Component)));
      });

  // Register component writes
  TypeListForEach<typename SystemTraits<T>::Writes>::apply(
      [&]<typename Component>() {
        _systemWrites[id].push_back(std::type_index(typeid(Component)));
      });

  // Register dependency resolver for this system type (only once per type)
  if (_dependencyResolvers.find(systemType) == _dependencyResolvers.end()) {
    _dependencyResolvers[systemType] = [](SystemScheduler& scheduler, SystemId sid, TaskGraph& graph) {
      TypeListForEach<typename SystemTraits<T>::DependsOn>::apply(
          [&]<typename Tag>() {
            auto it = scheduler._tagProviders.find(std::type_index(typeid(Tag)));
            if (it != scheduler._tagProviders.end()) {
              SystemId providerSid = it->second;
              graph.addDependency(scheduler._systemJobMap[providerSid], scheduler._systemJobMap[sid]);
            }
          });
    };
  }

  _systems.emplace_back(std::move(system));
}