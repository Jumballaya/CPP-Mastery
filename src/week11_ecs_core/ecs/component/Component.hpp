#pragma once

#include <cstddef>
#include <string_view>

#include "ComponentId.hpp"

template <typename T>
class Component {
 public:
  static ComponentId typeId() { return GetComponentId<T>(); }

  static constexpr std::string_view name() { return T::typeName; }
  static constexpr size_t sizeBytes() { return sizeof(T); }
};