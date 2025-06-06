#pragma once

#include <queue>
#include <vector>

#include "EntityId.hpp"

class EntityManager {
 public:
  EntityManager() = default;
  ~EntityManager() = default;
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;
  EntityManager(EntityManager&&) noexcept = default;
  EntityManager& operator=(EntityManager&&) noexcept = default;

  EntityId create() {
    uint32_t index;
    if (!_freeIndices.empty()) {
      index = _freeIndices.front();
      _freeIndices.pop();
    } else {
      index = static_cast<uint32_t>(_generations.size());
      _generations.push_back(0);
    }

    return EntityId{index, _generations[index]};
  }

  void destroy(EntityId id) {
    const uint32_t index = id.index;
    if (index >= _generations.size()) {
      return;
    }
    if (_generations[index] != id.generation) {
      return;
    }
    ++_generations[index];
    _freeIndices.push(index);
  }

  bool isAlive(EntityId id) const {
    if (id.index >= _generations.size()) {
      return false;
    }
    return _generations[id.index] == id.generation;
  }

  uint32_t capacity() const {
    return static_cast<uint32_t>(_generations.size());
  }

  uint32_t generation(uint32_t index) const {
    return _generations[index];
  }

 private:
  std::vector<uint32_t> _generations;
  std::queue<uint32_t> _freeIndices;
};