#pragma once

#include <atomic>
#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "../async/ThreadPool.hpp"
#include "AssetHandle.hpp"
#include "AssetManager.hpp"
#include "TextAsset.hpp"

class AssetPipeline {
 public:
  AssetPipeline(AssetManager& manager, ThreadPool& threadpool);

  void scanDirectory(const std::filesystem::path& dir);
  void loadAllAsync();

  size_t totalFiles() const;
  size_t loadedFiles() const;
  float progress() const;

  AssetHandle getHandle(const std::string& path) const;
  const std::vector<std::string>& error() const { return _errors; }

 private:
  void enqueueLoadJob(const std::filesystem::path& path);

  AssetManager& _manager;
  ThreadPool& _threadPool;

  std::vector<std::filesystem::path> _files;
  std::unordered_map<std::string, AssetHandle> _pathToHandle;
  std::vector<std::string> _errors;

  std::atomic<size_t> _loadedCount{0};
  std::atomic<size_t> _totalCount{0};
};