#pragma once

#include <string_view>
#include <unordered_set>

#include "EntityId.hpp"

class World;  // ✅ forward declaration

class EntityRef {
 public:
  EntityRef(EntityId id, World* world);

  EntityId id;

  template <typename T>
  T* get() const;

  template <typename T, typename... Args>
  T& add(Args&&... args);

  template <typename T>
  bool has() const;

  template <typename T>
  void remove();

  void addTag(std::string_view tag);
  void removeTag(std::string_view tag);
  bool hasTag(std::string_view tag) const;
  const std::unordered_set<unsigned int>& tags() const;
  bool alive() const;

 private:
  World* world;
};