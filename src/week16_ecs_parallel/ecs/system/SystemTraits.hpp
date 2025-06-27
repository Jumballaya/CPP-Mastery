#pragma once

#include "TypeList.hpp"

template <typename T>
struct SystemTraits {
  using DependsOn = EmptyList;
  using Provides = EmptyList;
  using Reads = EmptyList;
  using Writes = EmptyList;
};