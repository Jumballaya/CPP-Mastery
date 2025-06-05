#pragma once

#include <cstdint>
#include <string>

// Really just a uint64_t in disguise
struct EntityId {
  uint32_t index;
  uint32_t generation;

  bool operator==(const EntityId& other) const noexcept {
    return index == other.index && generation == other.generation;
  }

  bool operator!=(const EntityId& other) const noexcept {
    return !(*this == other);
  }

  bool operator<(const EntityId& other) const noexcept {
    return (index < other.index) || (index == other.index && generation < other.generation);
  }

  bool operator>(const EntityId& other) const noexcept {
    return other < *this;
  }

  bool operator<=(const EntityId& other) const noexcept {
    return !(other < *this);
  }

  bool operator>=(const EntityId& other) const noexcept {
    return !(*this < other);
  }
};

// Hashing support for unordered_map
namespace std {
template <>
struct hash<EntityId> {
  std::size_t operator()(const EntityId& id) const noexcept {
    // Just smashing the generation and the index into a
    // single uint64_t to use as the hash map key
    return std::hash<uint64_t>{}(static_cast<uint64_t>(id.generation) << 32 | id.index);
  }
};
}  // namespace std