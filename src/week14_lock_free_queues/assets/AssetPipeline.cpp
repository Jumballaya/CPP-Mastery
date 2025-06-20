#include "AssetPipeline.hpp"

#include <algorithm>
#include <iostream>

AssetPipeline::AssetPipeline(AssetManager& manager, ThreadPool& threadPool)
    : _manager(manager), _threadPool(threadPool) {}

void AssetPipeline::scanDirectory(const std::filesystem::path& dir) {
  if (!std::filesystem::exists(dir)) {
    std::cerr << "[AssetPipeline] Directory does not exist: " << dir << "\n";
    return;
  }

  for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
    if (!entry.is_regular_file()) continue;

    auto ext = entry.path().extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".json" || ext == ".lua" || ext == ".glsl" || ext == ".txt" || ext == ".xml") {
      _files.push_back(entry.path());
    }
  }
}

void AssetPipeline::loadAllAsync() {
  _loadedCount.store(0);
  _totalCount = _files.size();
  for (const auto& path : _files) {
    enqueueLoadJob(path);
  }
}

void AssetPipeline::enqueueLoadJob(const std::filesystem::path& path) {
  const std::string ext = path.extension().string();
  const std::string canonicalPath = std::filesystem::weakly_canonical(path).string();

  if (ext == ".txt" || ext == ".json" || ext == ".glsl" || ext == ".lua") {
    _submittedCount.fetch_add(1, std::memory_order_relaxed);
    _threadPool.enqueue([this, canonicalPath, path]() {
      try {
        auto handle = _manager.load<TextAsset>(canonicalPath, path);
        _pathToHandle[canonicalPath] = handle;
      } catch (const std::exception& e) {
        std::cerr << "[asset] Failed to load " << path << ": " << e.what() << "\n";
      }
      _loadedCount.fetch_add(1, std::memory_order_relaxed);
    });
  } else {
    // Skip unsupported
    _loadedCount.fetch_add(1, std::memory_order_relaxed);
  }
}

size_t AssetPipeline::totalFiles() const {
  return _files.size();
}

size_t AssetPipeline::loadedFiles() const {
  return _loadedCount.load(std::memory_order_relaxed);
}

float AssetPipeline::progress() const {
  size_t submitted = std::max(_submittedCount.load(), size_t(1));
  size_t completed = _loadedCount.load();
  return std::clamp(float(completed) / float(submitted), 0.0f, 1.0f);
}

AssetHandle AssetPipeline::getHandle(const std::string& path) const {
  auto canonicalPath = std::filesystem::weakly_canonical(path).string();
  auto it = _pathToHandle.find(canonicalPath);
  if (it != _pathToHandle.end()) {
    return it->second;
  }
  return {};
}