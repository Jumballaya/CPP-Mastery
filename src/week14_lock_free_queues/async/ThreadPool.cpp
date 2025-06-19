#include "ThreadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool(std::size_t count, std::size_t queueCapacity = 1024)
    : _queue(queueCapacity) {
  start(count);
}

ThreadPool::~ThreadPool() {
  stop();
}

void ThreadPool::start(std::size_t count) {
  std::cout << "[threadpool] started " << count << " threads\n";
  for (std::size_t i = 0; i < count; ++i) {
    _threads.emplace_back([this] {
      try {
        while (_queue.is_valid()) {
          Job job;
          if (_queue.try_dequeue(job)) {
            std::cout << "[worker] got job\n";
            job();
            std::cout << "[worker] finished job\n";
          } else {
            std::this_thread::yield();
          }
        }
      } catch (const std::exception& e) {
        std::cerr << "[worker] crashed: " << e.what() << "\n";
      } catch (...) {
        std::cerr << "[worker] crashed: unknown error\n";
      }
    });
  }
}

void ThreadPool::stop() {
  _shutdown.store(true);
  _queue.shutdown();
  for (auto& t : _threads) {
    if (t.joinable()) t.join();
  }
  _threads.clear();
}