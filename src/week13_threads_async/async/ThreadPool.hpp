#pragma once

#include <atomic>
#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

#include "BlockingQueue.hpp"

class ThreadPool {
 public:
  explicit ThreadPool(std::size_t threadCount);
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  ThreadPool(ThreadPool&&) noexcept;
  ThreadPool& operator=(ThreadPool&&) noexcept;

  void enqueue(std::function<void()> task);

  std::size_t size() const noexcept { return _workers.size(); }

 private:
  void start(std::size_t threadCount);
  void stop();

  std::vector<std::thread> _workers;
  BlockingQueue<std::function<void()>> _queue;
  std::atomic<bool> _shutdown{false};
};