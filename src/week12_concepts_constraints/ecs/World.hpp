#pragma once

#include <algorithm>
#include <initializer_list>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "View.hpp"
#include "component/ComponentConcepts.hpp"
#include "component/ComponentManager.hpp"
#include "component/ComponentStorage.hpp"
#include "entity/EntityId.hpp"
#include "entity/EntityManager.hpp"
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

  void update(float dt) {
    _systemScheduler.update(*this, dt);
  }

  template <ComponentType... Ts>
  View<Ts...> view() {
    return View<Ts...>(_componentManager.storage<Ts>()...);
  }

  EntityId createEntity() { return _entityManager.create(); }
  bool isAlive(EntityId id) const { return _entityManager.isAlive(id); }

  void destroyEntity(EntityId id) {
    if (!isAlive(id)) return;

    auto it = _entityToTags.find(id);
    if (it != _entityToTags.end()) {
      for (TagSymbol tag : it->second) {
        _tagToEntities[tag].erase(id);
      }
      _entityToTags.erase(it);
    }

    _entityManager.destroy(id);
  }

  void addTag(EntityId id, std::string_view tag) {
    if (!isAlive(id)) return;

    TagSymbol symbol = toTagSymbol(tag);
    _tagToEntities[symbol].insert(id);
    _entityToTags[id].insert(symbol);
  }

  void removeTag(EntityId id, std::string_view tag) {
    if (!isAlive(id)) return;
    TagSymbol symbol = toTagSymbol(tag);
    _tagToEntities[symbol].erase(id);
    _entityToTags[id].erase(symbol);

    if (_tagToEntities[symbol].empty()) {
      _tagToEntities.erase(symbol);
    }
    if (_entityToTags[id].empty()) {
      _entityToTags.erase(id);
    }
  }

  bool hasTag(EntityId id, std::string_view tag) const {
    if (!isAlive(id)) return false;
    TagSymbol symbol = toTagSymbol(tag);
    auto it = _entityToTags.find(id);
    if (it == _entityToTags.end()) return false;
    return it->second.contains(symbol);
  }

  const std::unordered_set<EntityId> findWithTag(std::string_view tag) const {
    static const std::unordered_set<EntityId> empty;
    TagSymbol symbol = toTagSymbol(tag);
    auto it = _tagToEntities.find(symbol);
    return it != _tagToEntities.end() ? it->second : empty;
  }

  std::unordered_set<EntityId> findWithTags(std::initializer_list<std::string_view> tags) const {
    std::vector<const std::unordered_set<EntityId>*> sets;
    for (auto tag : tags) {
      TagSymbol symbol = toTagSymbol(tag);
      auto it = _tagToEntities.find(symbol);
      if (it == _tagToEntities.end()) return {};
      sets.push_back(&it->second);
    }

    if (sets.empty()) return {};

    // get the shortest list up front
    std::sort(sets.begin(), sets.end(), [](const std::unordered_set<EntityId>* a, const std::unordered_set<EntityId>* b) {
      return a->size() < b->size();
    });

    // build the intersection of all of the tags' entities
    std::unordered_set<EntityId> result = *sets[0];
    for (size_t i = 1; i < sets.size(); ++i) {
      std::unordered_set<EntityId> temp;
      for (EntityId id : *sets[i]) {
        if (result.contains(id)) {
          temp.insert(id);
        }
      }
      result = std::move(temp);
      if (result.empty()) break;
    }

    return result;
  }

  const std::unordered_set<TagSymbol>& getTags(EntityId id) const {
    static const std::unordered_set<TagSymbol> empty;
    auto it = _entityToTags.find(id);
    return it != _entityToTags.end() ? it->second : empty;
  }

  template <ComponentType T, typename... Args>
  void addComponent(EntityId id, Args&&... args) {
    if (!isAlive(id)) {
      return;
    }
    _componentManager.emplace<T>(id, std::forward<Args>(args)...);
  }

  template <ComponentType T>
  [[nodiscard]]
  T* getComponent(EntityId id) {
    if (!isAlive(id)) {
      return nullptr;
    }
    return _componentManager.get<T>(id);
  }

  template <ComponentType T>
  bool hasComponent(EntityId id) const {
    if (!isAlive(id)) {
      return false;
    }
    return _componentManager.has<T>(id);
  }

  template <ComponentType T>
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

  std::unordered_map<TagSymbol, std::unordered_set<EntityId>> _tagToEntities;
  std::unordered_map<EntityId, std::unordered_set<TagSymbol>> _entityToTags;
};