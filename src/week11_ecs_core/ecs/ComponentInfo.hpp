#pragma once

#include <cstddef>
#include <string_view>

#include "ComponentId.hpp"

struct ComponentInfo {
  std::string_view name;
  size_t size;
  ComponentId id;
};