#pragma once

#include <cstdint>
#include <string>

using ComponentId = uint32_t;

inline ComponentId nextComponentId = 0;

template <typename T>
ComponentId GetComponentId() {
  static const ComponentId id = nextComponentId++;
  return id;
}