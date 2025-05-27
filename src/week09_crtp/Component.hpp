#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

using ComponentId = uint32_t;

inline ComponentId nextComponentId = 0;

template <typename T>
ComponentId TypeId() {
  static const ComponentId id = nextComponentId++;
  return id;
}

template <typename T>
class Component {
 public:
  static ComponentId typeId() { return TypeId<T>(); }

  static constexpr std::string_view name() { return typeNamel }
  static constexpr size_t sizeBytes() { return sizeof(T); }

 private:
  static constexpr std::string_view typeName = "TYPENAME";
};