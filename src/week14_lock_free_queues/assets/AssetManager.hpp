#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "AssetBase.hpp"
#include "AssetHandle.hpp"
#include "AssetLoader.hpp"

class AssetManager {
 public:
  template <typename T>
  AssetHandle load(std::string_view id, const std::filesystem::path& path) {
    auto asset = AssetLoader<T>::load(path);
    if (!asset) throw std::runtime_error("Failed to load asset: " + path.string());

    uint32_t hash = static_cast<uint32_t>(std::hash<std::string_view>{}(id));
    _assets[hash] = std::move(asset);
    return AssetHandle{hash};
  }

  template <typename T>
  T* get(const AssetHandle& handle) const {
    auto it = _assets.find(handle.id);
    if (it == _assets.end()) return nullptr;

    auto* base = it->second.get();
    if (base->type() != typeid(T)) return nullptr;

    return static_cast<T*>(base);
  }

 private:
  std::unordered_map<uint32_t, std::unique_ptr<AssetBase>> _assets;
};