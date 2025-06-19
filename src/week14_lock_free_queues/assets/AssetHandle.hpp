#pragma once
#include <cstdint>
#include <unordered_map>

struct AssetHandle {
  uint32_t id = 0;

  bool isValid() const { return id != 0; }
  operator bool() const { return isValid(); }

  bool operator==(const AssetHandle& other) const { return id == other.id; }
};

namespace std {
template <>
struct hash<AssetHandle> {
  std::size_t operator()(const AssetHandle& h) const {
    return std::hash<uint32_t>{}(h.id);
  }
};
}  // namespace std