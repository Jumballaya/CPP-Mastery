#pragma once

#include <cstdint>

using SystemId = uint32_t;

inline SystemId nextSystemId = 0;

template <typename T>
SystemId GetSystemId() {
  static const SystemId id = nextSystemId++;
  return id;
}