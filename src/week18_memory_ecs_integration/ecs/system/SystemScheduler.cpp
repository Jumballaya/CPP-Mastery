#include "SystemScheduler.hpp"

#include "../../tasks/TaskId.hpp"
#include "../World.hpp"

void SystemScheduler::update(World& world, float dt) {
  for (auto& system : _systems) {
    if (!system->enabled) {
      continue;
    }
    system->update(world, dt);
  }
}

void SystemScheduler::clear() {
  _systems.clear();
  _tagProviders.clear();
  _systemReads.clear();
  _systemWrites.clear();
  _systemTypes.clear();
  _systemJobMap.clear();
  _dependencyResolvers.clear();
}

void SystemScheduler::disableSystem(System& system) {
  for (auto& s : _systems) {
    if (s.get() == &system) {
      s->enabled = false;
      break;
    }
  }
}

void SystemScheduler::enableSystem(System& system) {
  for (auto& s : _systems) {
    if (s.get() == &system) {
      s->enabled = true;
      break;
    }
  }
}

void SystemScheduler::buildTaskGraph(TaskGraph& graph, World& world, float dt) {
  _systemJobMap.clear();

  for (SystemId sid = 0; sid < _systems.size(); ++sid) {
    if (!_systems[sid]->enabled) {
      continue;
    }

    TaskId tid = graph.addTask([this, sid, &world, dt]() {
      _systems[sid]->update(world, dt);
    });
    _systemJobMap[sid] = tid;
  }

  for (SystemId sid = 0; sid < _systems.size(); ++sid) {
    if (!_systems[sid]->enabled) {
      continue;
    }
    auto it = _systemTypes.find(sid);
    if (it == _systemTypes.end()) {
      continue;
    }
    std::type_index systemType = it->second;
    auto resolverIt = _dependencyResolvers.find(systemType);
    if (resolverIt != _dependencyResolvers.end()) {
      resolverIt->second(*this, sid, graph);
    }
  }
}