#include <atomic>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#include "assets/AssetManager.hpp"
#include "assets/AssetPipeline.hpp"
#include "async/LockFreeQueue.hpp"
#include "async/ThreadPool.hpp"

void demo_1_lock_free_queue() {
  LockFreeQueue<int> q(8);

  bool ok = q.try_enqueue(42);

  if (ok) {
    std::cout << "Passed" << std::endl;
  }
}

void demo_2_mpmc() {
  constexpr size_t numThreads = 4;
  constexpr size_t queueCapacity = 256;
  constexpr size_t numProducers = 8;
  constexpr size_t tasksPerProducer = 1;

  ThreadPool pool(numThreads, queueCapacity);
  std::atomic<size_t> counter{0};

  std::vector<std::thread> producers;

  for (size_t i = 0; i < numProducers; ++i) {
    producers.emplace_back([&pool, &counter]() {
      for (size_t j = 0; j < tasksPerProducer; ++j) {
        pool.enqueue([&counter]() {
          counter.fetch_add(1, std::memory_order_relaxed);
          std::cout << "[job] incremented counter\n";
        });
      }
    });
  }

  // Wait for all producer threads to finish submitting
  for (auto& p : producers) {
    if (p.joinable()) p.join();
  }

  // Wait until all tasks are processed
  while (counter.load(std::memory_order_relaxed) < numProducers * tasksPerProducer) {
    std::this_thread::yield();
  }

  std::cout << "[demo_1_mpmc] Completed " << counter.load() << " tasks." << std::endl;
}

void demo_3_asset_pipeline(std::string& filesDir) {
  AssetManager manager;
  ThreadPool pool(4, 512);
  AssetPipeline pipeline(manager, pool);

  pipeline.scanDirectory(filesDir);
  pipeline.loadAllAsync();

  const size_t total = pipeline.totalFiles();  // cache it up front

  // Wait until at least one file is loaded
  while (pipeline.loadedFiles() == 0) {
    std::this_thread::yield();
  }

  // Show loading progress bar
  while (pipeline.loadedFiles() < pipeline.totalFiles()) {
    float p = pipeline.progress();
    std::cout << "[loading] " << int(p * 100) << "% complete" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
  std::cout << "[loading] 100% complete" << std::endl;

  std::cout << "Asset load complete!" << std::endl;
}

int main(int argc, char** argv) {
  std::filesystem::path exePath = std::filesystem::absolute(argv[0]);
  std::filesystem::path exeDir = exePath.parent_path();
  std::filesystem::path filesDir = exeDir / "files";
  std::string dir = filesDir.string();
  demo_3_asset_pipeline(dir);
}