#pragma once

#include <cstddef>
#include <string_view>

#include "ComponentId.hpp"

template <typename T>
class Component {
 public:
  static ComponentId typeId() { return TypeId<T>(); }

  static constexpr std::string_view name() { return typeName; }
  static constexpr size_t sizeBytes() { return sizeof(T); }

 private:
  static constexpr std::string_view typeName = "TYPENAME";
};