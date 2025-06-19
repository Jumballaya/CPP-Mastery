#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

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

int main() {
  demo_2_mpmc();
}