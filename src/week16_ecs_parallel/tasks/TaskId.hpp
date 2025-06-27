#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>

struct TaskId {
  size_t id;

  bool operator==(const TaskId& other) const noexcept { return id == other.id; }
  bool operator!=(const TaskId& other) const noexcept { return id != other.id; }
};

namespace std {
template <>
struct hash<TaskId> {
  size_t operator()(const TaskId& taskId) const noexcept {
    return hash<size_t>{}(taskId.id);
  }
};
}  // namespace std
