#pragma once
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>

#include "TextAsset.hpp"

template <typename T>
struct AssetLoader;  // leave primary template undefined

template <>
struct AssetLoader<TextAsset> {
  static std::unique_ptr<TextAsset> load(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) return nullptr;

    std::ostringstream ss;
    ss << file.rdbuf();

    return std::make_unique<TextAsset>(path.string(), ss.str());
  }
};