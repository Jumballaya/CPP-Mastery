#include "World.hpp"

EntityRef World::operator[](EntityId id) {
  return EntityRef{id, this};
}

void World::buildExecutionGraph(TaskGraph& graph, float dt) {
  _systemScheduler.buildTaskGraph(graph, *this, dt);
}

EntityBuilder World::builder() {
  EntityId id = createEntity();
  return EntityBuilder(id, *this);
}

EntityId World::createEntity() {
  return _entityManager.create();
}

EntityId World::createEntity(std::string_view tag) {
  EntityId id = createEntity();
  addTag(id, tag);
  return id;
}

bool World::isAlive(EntityId id) const {
  return _entityManager.isAlive(id);
}

void World::destroyEntity(EntityId id) {
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

EntityId World::cloneEntity(EntityId src) {
  if (!isAlive(src)) return EntityId{};
  EntityId dst = createEntity();

  const auto& tags = getTags(src);
  for (TagSymbol tag : tags) {
    _tagToEntities[tag].insert(dst);
    _entityToTags[dst].insert(tag);
  }

  _registry.forEachRegisteredComponent([&](ComponentId id) {
    auto* base = _componentManager.rawStorage(id);
    if (!base) return;

    if (base->has(src)) {
      base->cloneComponent(src, dst);
    }
  });
  return dst;
}

void World::addTag(EntityId id, std::string_view tag) {
  if (!isAlive(id)) return;

  TagSymbol symbol = toTagSymbol(tag);
  _tagToEntities[symbol].insert(id);
  _entityToTags[id].insert(symbol);
}

void World::removeTag(EntityId id, std::string_view tag) {
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

void World::clearTags(EntityId id) {
  if (!isAlive(id)) return;
  auto it = _entityToTags.find(id);
  if (it == _entityToTags.end()) return;

  for (TagSymbol tag : it->second) {
    _tagToEntities[tag].erase(id);
  }

  _entityToTags.erase(it);
}

bool World::hasTag(EntityId id, std::string_view tag) const {
  if (!isAlive(id)) return false;
  TagSymbol symbol = toTagSymbol(tag);
  auto it = _entityToTags.find(id);
  if (it == _entityToTags.end()) return false;
  return it->second.contains(symbol);
}

void World::retagEntity(EntityId id, std::string_view tag) {
  clearTags(id);
  addTag(id, tag);
}

const std::unordered_set<EntityId> World::findWithTag(std::string_view tag) const {
  static const std::unordered_set<EntityId> empty;
  TagSymbol symbol = toTagSymbol(tag);
  auto it = _tagToEntities.find(symbol);
  return it != _tagToEntities.end() ? it->second : empty;
}

std::unordered_set<EntityId> World::findWithTags(std::initializer_list<std::string_view> tags) const {
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

const std::unordered_set<TagSymbol>& World::getTags(EntityId id) const {
  static const std::unordered_set<TagSymbol> empty;
  auto it = _entityToTags.find(id);
  return it != _entityToTags.end() ? it->second : empty;
}

void World::validate() const {
  for (const auto& [id, tags] : _entityToTags) {
    if (!isAlive(id)) {
      throw std::runtime_error("Entity has tags but is not alive.");
    }
  }
}

const ComponentRegistry& World::getRegistry() const {
  return _registry;
}